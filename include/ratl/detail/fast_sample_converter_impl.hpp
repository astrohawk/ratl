#ifndef _ratl_detail_fast_sample_converter_impl_
#define _ratl_detail_fast_sample_converter_impl_

// C++ Standard Library includes
#include <cfenv>
#include <cmath>
#include <type_traits>

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/convert_traits.hpp>
#include <ratl/detail/endianness.hpp>
#include <ratl/detail/round.hpp>
#include <ratl/network_sample.hpp>
#include <ratl/sample.hpp>
#include <ratl/sample_type_limits.hpp>

namespace ratl
{
namespace detail
{
// FastSampleConverterImpl

template<class InputSample, class OutputSample, class DitherGenerator>
struct FastSampleConverterImpl;

template<class SampleType, class DitherGenerator>
struct FastSampleConverterImpl<Sample<SampleType>, Sample<SampleType>, DitherGenerator>
{
    static const SampleType& convert(const SampleType& sample, DitherGenerator&) noexcept
    {
        return sample;
    }
};

template<class DitherGenerator>
struct FastSampleConverterImpl<Sample<int16_t>, Sample<int24_t>, DitherGenerator>
{
    static int24_t convert(int32_t sample, DitherGenerator&) noexcept
    {
        return static_cast<int24_t>(sample << 8);
    }
};

template<class DitherGenerator>
struct FastSampleConverterImpl<Sample<int16_t>, Sample<int32_t>, DitherGenerator>
{
    static int32_t convert(int32_t sample, DitherGenerator&) noexcept
    {
        return sample << 16;
    }
};

template<class DitherGenerator>
struct FastSampleConverterImpl<Sample<int24_t>, Sample<int16_t>, DitherGenerator>
{
    static constexpr std::size_t TotalShift = 8;
    static constexpr std::size_t PreDitherShift =
        DitherGenerator::Int16Bits > 0 ? DitherGenerator::Int16Bits - TotalShift : 0;
    static constexpr std::size_t PostDitherShift = TotalShift + PreDitherShift;

    static int16_t convert(int32_t sample, DitherGenerator& dither_generator) noexcept
    {
        return static_cast<int24_t>(((sample << PreDitherShift) + dither_generator.generateInt16()) >> PostDitherShift);
    }
};

template<class DitherGenerator>
struct FastSampleConverterImpl<Sample<int24_t>, Sample<int32_t>, DitherGenerator>
{
    static int32_t convert(int32_t sample, DitherGenerator&) noexcept
    {
        return sample << 8;
    }
};

template<class DitherGenerator>
struct FastSampleConverterImpl<Sample<int32_t>, Sample<int16_t>, DitherGenerator>
{
    static constexpr std::size_t TotalShift = 16;
    static constexpr std::size_t PreDitherShift = TotalShift - DitherGenerator::Int16Bits;
    static constexpr std::size_t PostDitherShift = TotalShift - PreDitherShift;

    static int16_t convert(int32_t sample, DitherGenerator& dither_generator) noexcept
    {
        return static_cast<int16_t>(((sample >> PreDitherShift) + dither_generator.generateInt16()) >> PostDitherShift);
    }
};

template<class DitherGenerator>
struct FastSampleConverterImpl<Sample<int32_t>, Sample<int24_t>, DitherGenerator>
{
    static int24_t convert(int32_t sample, DitherGenerator&) noexcept
    {
        return static_cast<int24_t>(sample >> 8);
    }
};

template<class SampleType, class DitherGenerator>
struct FastSampleConverterImpl<Sample<SampleType>, Sample<float32_t>, DitherGenerator>
{
    static constexpr float32_t Scaler = FloatConvertTraits<SampleType>::Divisor;

    static inline float32_t convert(SampleType sample, DitherGenerator&) noexcept
    {
        return static_cast<float32_t>(sample) * Scaler;
    }
};

template<class SampleType, class DitherGenerator>
struct FastSampleConverterImpl<Sample<float32_t>, Sample<SampleType>, DitherGenerator>
{
private:
    static constexpr float32_t PositiveScaler =
        static_cast<float32_t>(SampleTypeLimits<SampleType>::max) - DitherGenerator::MaxFloat32;
    static constexpr float32_t NegativeScaler =
        -static_cast<float32_t>(SampleTypeLimits<SampleType>::min) - DitherGenerator::MaxFloat32;

public:
    static inline int32_t convert(float32_t sample, DitherGenerator& dither_generator) noexcept
    {
        return roundFloat32ToInt32Fast(
            (sample * (sample < 0 ? NegativeScaler : PositiveScaler)) + dither_generator.generateFloat32());
    }
};

// This is required as SampleConverter<float32_t, SampleType> is more specific than
// SampleConverter<SampleType, SampleType>

template<class DitherGenerator>
struct FastSampleConverterImpl<Sample<float32_t>, Sample<float32_t>, DitherGenerator>
{
    static inline float32_t convert(float32_t sample, DitherGenerator&) noexcept
    {
        return sample;
    }
};

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
struct FastSampleConverterImpl<Sample<InputSampleType>, NetworkSample<OutputSampleType>, DitherGenerator>
{
    static inline NetworkSampleValueType_t<OutputSampleType> convert(
        InputSampleType sample, DitherGenerator& dither_generator) noexcept
    {
        return FastSampleConverterImpl<Sample<OutputSampleType>, NetworkSample<OutputSampleType>, DitherGenerator>::
            convert(
                FastSampleConverterImpl<Sample<InputSampleType>, Sample<OutputSampleType>, DitherGenerator>::convert(
                    sample, dither_generator),
                dither_generator);
    }
};

template<class SampleType, class DitherGenerator>
struct FastSampleConverterImpl<Sample<SampleType>, NetworkSample<SampleType>, DitherGenerator>
{
    static inline NetworkSampleValueType_t<SampleType> convert(SampleType sample, DitherGenerator&) noexcept
    {
        return sampleToNetworkSample<SampleType>(sample);
    }
};

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
struct FastSampleConverterImpl<NetworkSample<InputSampleType>, Sample<OutputSampleType>, DitherGenerator>
{
    static inline OutputSampleType convert(
        NetworkSampleValueType_t<InputSampleType> sample, DitherGenerator& dither_generator) noexcept
    {
        return FastSampleConverterImpl<Sample<InputSampleType>, Sample<OutputSampleType>, DitherGenerator>::convert(
            FastSampleConverterImpl<NetworkSample<InputSampleType>, Sample<InputSampleType>, DitherGenerator>::convert(
                sample, dither_generator),
            dither_generator);
    }
};

template<class SampleType, class DitherGenerator>
struct FastSampleConverterImpl<NetworkSample<SampleType>, Sample<SampleType>, DitherGenerator>
{
    static inline SampleType convert(NetworkSampleValueType_t<SampleType> sample, DitherGenerator&) noexcept
    {
        return networkSampleToSample<SampleType>(sample);
    }
};

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_fast_sample_converter_impl_
