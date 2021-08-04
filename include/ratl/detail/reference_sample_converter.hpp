#ifndef _ratl_detail_reference_sample_converter_
#define _ratl_detail_reference_sample_converter_

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
// SampleConverter

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
struct ReferenceSampleToSampleConverter;

// intXX_t -> intXX_t

template<class SampleType, class DitherGenerator>
struct ReferenceSampleToSampleConverter<SampleType, SampleType, DitherGenerator>
{
    static inline SampleType convert(SampleType sample, DitherGenerator&) noexcept
    {
        return sample;
    }
};

// int16_t -> intXX_t

template<class DitherGenerator>
struct ReferenceSampleToSampleConverter<int16_t, int24_t, DitherGenerator>
{
    static inline int24_t convert(int32_t sample, DitherGenerator&) noexcept
    {
        return static_cast<int24_t>(sample << 8);
    }
};

template<class DitherGenerator>
struct ReferenceSampleToSampleConverter<int16_t, int32_t, DitherGenerator>
{
    static inline int32_t convert(int32_t sample, DitherGenerator&) noexcept
    {
        return sample << 16;
    }
};

// int24_t -> intXX_t
// sample >> 31 results in 0 if sample is positive or 0, and -1 if sample is negative
// this is added to the rounding constant to perform round half away from zero as opposed to just round half up

template<class DitherGenerator>
struct ReferenceSampleToSampleConverter<int24_t, int16_t, DitherGenerator>
{
    static constexpr int32_t SampleInMax = static_cast<int32_t>(0x007FFF80);
    static constexpr int16_t SampleOutMax = SampleTypeLimits<int16_t>::max;
    static constexpr int32_t Rounding = static_cast<int32_t>(0x80);
    static constexpr std::size_t TotalShift = 8;
    static constexpr std::size_t PreDitherShift =
        DitherGenerator::Int16Bits > 0 ? DitherGenerator::Int16Bits - TotalShift : 0;
    static constexpr std::size_t PostDitherShift = TotalShift + PreDitherShift;

    static inline int16_t convert(int32_t sample, DitherGenerator& dither_generator) noexcept
    {
        if (sample >= SampleInMax)
        {
            return SampleOutMax;
        }
        return static_cast<int16_t>(
            (((sample + (Rounding + (sample >> 31))) << PreDitherShift) + dither_generator.generateInt16()) >>
            PostDitherShift);
    }
};

template<class DitherGenerator>
constexpr int16_t ReferenceSampleToSampleConverter<int24_t, int16_t, DitherGenerator>::SampleOutMax;

template<class DitherGenerator>
struct ReferenceSampleToSampleConverter<int24_t, int32_t, DitherGenerator>
{
    static inline int32_t convert(int32_t sample, DitherGenerator&) noexcept
    {
        return sample << 8;
    }
};

// int32_t -> intXX_t
// sample >> 31 results in 0 if sample is positive or 0, and -1 if sample is negative
// this is added to the rounding constant to perform round half away from zero as opposed to just round half up

template<class DitherGenerator>
struct ReferenceSampleToSampleConverter<int32_t, int16_t, DitherGenerator>
{
    static constexpr int32_t SampleInMax = static_cast<int32_t>(0x7FFF8000);
    static constexpr int16_t SampleOutMax = SampleTypeLimits<int16_t>::max;
    static constexpr int32_t Rounding = static_cast<int32_t>(0x8000);
    static constexpr std::size_t TotalShift = 16;
    static constexpr std::size_t PreDitherShift = TotalShift - DitherGenerator::Int16Bits;
    static constexpr std::size_t PostDitherShift = TotalShift - PreDitherShift;

    static inline int16_t convert(int32_t sample, DitherGenerator& dither_generator) noexcept
    {
        if (sample >= SampleInMax)
        {
            return SampleOutMax;
        }
        return static_cast<int16_t>(
            (((sample + (Rounding + (sample >> 31))) >> PreDitherShift) + dither_generator.generateInt16()) >>
            PostDitherShift);
    }
};

template<class DitherGenerator>
constexpr int16_t ReferenceSampleToSampleConverter<int32_t, int16_t, DitherGenerator>::SampleOutMax;

template<class DitherGenerator>
struct ReferenceSampleToSampleConverter<int32_t, int24_t, DitherGenerator>
{
    static constexpr int32_t SampleInMax = static_cast<int32_t>(0x7FFFFF80);
    static constexpr int24_t SampleOutMax = SampleTypeLimits<int24_t>::max;
    static constexpr int32_t Rounding = static_cast<int32_t>(0x80);

    static inline int24_t convert(int32_t sample, DitherGenerator&) noexcept
    {
        if (sample >= SampleInMax)
        {
            return SampleOutMax;
        }
        return static_cast<int24_t>((sample + (Rounding + (sample >> 31))) >> 8);
    }
};

template<class DitherGenerator>
constexpr int24_t ReferenceSampleToSampleConverter<int32_t, int24_t, DitherGenerator>::SampleOutMax;

// intXX_t -> float32_t

template<class SampleType, class DitherGenerator>
struct ReferenceSampleToSampleConverter<SampleType, float32_t, DitherGenerator>
{
    static constexpr float32_t Scaler = FloatConvertTraits<SampleType>::Divisor;

    static inline float32_t convert(SampleType sample, DitherGenerator&) noexcept
    {
        return static_cast<float32_t>(sample) * Scaler;
    }
};

// float32_t -> intXX_t
// sample >> 31 results in 0 if sample is positive or 0, and -1 if sample is negative
// this is added to the rounding constant to perform round half away from zero as opposed to just round half up

template<class SampleType, class DitherGenerator>
struct ReferenceSampleToSampleConverter<float32_t, SampleType, DitherGenerator>
{
private:
    static constexpr float32_t SampleInMax =
        static_cast<float32_t>(SampleTypeLimits<SampleType>::max) * FloatConvertTraits<SampleType>::Divisor;
    static constexpr SampleType SampleOutMax = SampleTypeLimits<SampleType>::max;
    static constexpr float32_t SampleInMin =
        static_cast<float32_t>(SampleTypeLimits<SampleType>::min) * FloatConvertTraits<SampleType>::Divisor;
    static constexpr SampleType SampleOutMin = SampleTypeLimits<SampleType>::min;
    static constexpr float32_t Scaler = FloatConvertTraits<SampleType>::Multiplier - DitherGenerator::MaxFloat32;

public:
    static inline int32_t convert(float32_t sample, DitherGenerator& dither_generator) noexcept
    {
        if RATL_UNLIKELY (sample >= SampleInMax)
        {
            return SampleOutMax;
        }
        if RATL_UNLIKELY (sample < SampleInMin)
        {
            return SampleOutMin;
        }
        return roundFloat32ToInt32((sample * Scaler) + dither_generator.generateFloat32());
    }
};

template<class SampleType, class DitherGenerator>
constexpr SampleType ReferenceSampleToSampleConverter<float32_t, SampleType, DitherGenerator>::SampleOutMax;
template<class SampleType, class DitherGenerator>
constexpr SampleType ReferenceSampleToSampleConverter<float32_t, SampleType, DitherGenerator>::SampleOutMin;

// float32_t -> float32_t
// This is required as SampleConverter<float32_t, SampleType> is more specific than
// SampleConverter<SampleType, SampleType>

template<class DitherGenerator>
struct ReferenceSampleToSampleConverter<float32_t, float32_t, DitherGenerator>
{
    static inline float32_t convert(float32_t sample, DitherGenerator&) noexcept
    {
        return sample;
    }
};

// SampleToNetworkConverter

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
struct ReferenceSampleToNetworkSampleConverter
{
    static inline NetworkSampleValueType_t<OutputSampleType> convert(
        InputSampleType sample, DitherGenerator& dither_generator) noexcept
    {
        return ReferenceSampleToNetworkSampleConverter<OutputSampleType, OutputSampleType, DitherGenerator>::convert(
            ReferenceSampleToSampleConverter<InputSampleType, OutputSampleType, DitherGenerator>::convert(
                sample, dither_generator),
            dither_generator);
    }
};

// XX_t -> XX_t

template<class SampleType, class DitherGenerator>
struct ReferenceSampleToNetworkSampleConverter<SampleType, SampleType, DitherGenerator>
{
    static inline NetworkSampleValueType_t<SampleType> convert(SampleType sample, DitherGenerator&) noexcept
    {
        return sampleToNetworkSample<SampleType>(sample);
    }
};

// SampleToNetworkConverter

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
struct ReferenceNetworkSampleToSampleConverter
{
    static inline OutputSampleType convert(
        NetworkSampleValueType_t<InputSampleType> sample, DitherGenerator& dither_generator) noexcept
    {
        return ReferenceSampleToSampleConverter<InputSampleType, OutputSampleType, DitherGenerator>::convert(
            ReferenceNetworkSampleToSampleConverter<InputSampleType, InputSampleType, DitherGenerator>::convert(
                sample, dither_generator),
            dither_generator);
    }
};

// XX_t -> XX_t

template<class SampleType, class DitherGenerator>
struct ReferenceNetworkSampleToSampleConverter<SampleType, SampleType, DitherGenerator>
{
    static inline SampleType convert(NetworkSampleValueType_t<SampleType> sample, DitherGenerator&) noexcept
    {
        return networkSampleToSample<SampleType>(sample);
    }
};

struct ReferenceSampleConverter
{
    template<class InputSampleType, class OutputSampleType, class DitherGenerator>
    using SampleToSampleConverter =
        ReferenceSampleToSampleConverter<InputSampleType, OutputSampleType, DitherGenerator>;

    template<class InputSampleType, class OutputSampleType, class DitherGenerator>
    using SampleToNetworkSampleConverter =
        ReferenceSampleToNetworkSampleConverter<InputSampleType, OutputSampleType, DitherGenerator>;

    template<class InputSampleType, class OutputSampleType, class DitherGenerator>
    using NetworkSampleToSampleConverter =
        ReferenceNetworkSampleToSampleConverter<InputSampleType, OutputSampleType, DitherGenerator>;
};

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_reference_sample_converter_
