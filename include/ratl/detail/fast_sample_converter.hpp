#ifndef _ratl_detail_fast_sample_converter_
#define _ratl_detail_fast_sample_converter_

// C++ Standard Library includes
#include <cfenv>
#include <cmath>
#include <type_traits>

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/convert_traits.hpp>
#include <ratl/detail/endianness.hpp>
#include <ratl/detail/round.hpp>
#include <ratl/sample_type_limits.hpp>

namespace ratl
{
namespace detail
{
// FastSampleToSampleConverter

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
struct FastSampleToSampleConverter;

// intXX_t -> intXX_t

template<class SampleType, class DitherGenerator>
struct FastSampleToSampleConverter<SampleType, SampleType, DitherGenerator>
{
    static const SampleType& convert(const SampleType& sample, DitherGenerator&) noexcept
    {
        return sample;
    }
};

// int16_t -> intXX_t

template<class DitherGenerator>
struct FastSampleToSampleConverter<int16_t, int24_t, DitherGenerator>
{
    static int24_t convert(int32_t sample, DitherGenerator&) noexcept
    {
        return static_cast<int24_t>(sample << 8);
    }
};

template<class DitherGenerator>
struct FastSampleToSampleConverter<int16_t, int32_t, DitherGenerator>
{
    static int32_t convert(int32_t sample, DitherGenerator&) noexcept
    {
        return sample << 16;
    }
};

// int24_t -> intXX_t

template<class DitherGenerator>
struct FastSampleToSampleConverter<int24_t, int16_t, DitherGenerator>
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
struct FastSampleToSampleConverter<int24_t, int32_t, DitherGenerator>
{
    static int32_t convert(int32_t sample, DitherGenerator&) noexcept
    {
        return sample << 8;
    }
};

// int32_t -> intXX_t

template<class DitherGenerator>
struct FastSampleToSampleConverter<int32_t, int16_t, DitherGenerator>
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
struct FastSampleToSampleConverter<int32_t, int24_t, DitherGenerator>
{
    static int24_t convert(int32_t sample, DitherGenerator&) noexcept
    {
        return static_cast<int24_t>(sample >> 8);
    }
};

// intXX_t -> float32_t

template<class SampleType, class DitherGenerator>
struct FastSampleToSampleConverter<SampleType, float32_t, DitherGenerator>
{
    static constexpr float32_t Scaler = FloatConvertTraits<SampleType>::Divisor;

    static inline float32_t convert(SampleType sample, DitherGenerator&) noexcept
    {
        return static_cast<float32_t>(sample) * Scaler;
    }
};

// float32_t -> intXX_t

template<class SampleType, class DitherGenerator>
struct FastSampleToSampleConverter<float32_t, SampleType, DitherGenerator>
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

// float32_t -> float32_t
// This is required as SampleConverter<float32_t, SampleType> is more specific than
// SampleConverter<SampleType, SampleType>

template<class DitherGenerator>
struct FastSampleToSampleConverter<float32_t, float32_t, DitherGenerator>
{
    static inline float32_t convert(float32_t sample, DitherGenerator&) noexcept
    {
        return sample;
    }
};

// FastSampleToNetworkSampleConverter

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
struct FastSampleToNetworkSampleConverter
{
    static inline NetworkSampleValueType_t<OutputSampleType> convert(
        InputSampleType sample, DitherGenerator& dither_generator) noexcept
    {
        return FastSampleToNetworkSampleConverter<OutputSampleType, OutputSampleType, DitherGenerator>::convert(
            FastSampleToSampleConverter<InputSampleType, OutputSampleType, DitherGenerator>::convert(
                sample, dither_generator),
            dither_generator);
    }
};

// XX_t -> XX_t

template<class SampleType, class DitherGenerator>
struct FastSampleToNetworkSampleConverter<SampleType, SampleType, DitherGenerator>
{
    static inline NetworkSampleValueType_t<SampleType> convert(SampleType sample, DitherGenerator&) noexcept
    {
        return sampleToNetworkSample<SampleType>(sample);
    }
};

// FastSampleToNetworkSampleConverter

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
struct FastNetworkSampleToSampleConverter
{
    static inline OutputSampleType convert(
        NetworkSampleValueType_t<InputSampleType> sample, DitherGenerator& dither_generator) noexcept
    {
        return FastSampleToSampleConverter<InputSampleType, OutputSampleType, DitherGenerator>::convert(
            FastNetworkSampleToSampleConverter<InputSampleType, InputSampleType, DitherGenerator>::convert(
                sample, dither_generator),
            dither_generator);
    }
};

// XX_t -> XX_t

template<class SampleType, class DitherGenerator>
struct FastNetworkSampleToSampleConverter<SampleType, SampleType, DitherGenerator>
{
    static inline SampleType convert(NetworkSampleValueType_t<SampleType> sample, DitherGenerator&) noexcept
    {
        return networkSampleToSample<SampleType>(sample);
    }
};

struct FastSampleConverter
{
    template<class InputSampleType, class OutputSampleType, class DitherGenerator>
    using SampleToSampleConverter = FastSampleToSampleConverter<InputSampleType, OutputSampleType, DitherGenerator>;

    template<class InputSampleType, class OutputSampleType, class DitherGenerator>
    using SampleToNetworkSampleConverter =
        FastSampleToNetworkSampleConverter<InputSampleType, OutputSampleType, DitherGenerator>;

    template<class InputSampleType, class OutputSampleType, class DitherGenerator>
    using NetworkSampleToSampleConverter =
        FastNetworkSampleToSampleConverter<InputSampleType, OutputSampleType, DitherGenerator>;
};

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_fast_sample_converter_
