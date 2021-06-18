#ifndef _ratl_detail_batch_sample_converter_
#define _ratl_detail_batch_sample_converter_

// C++ Standard Library includes
#include <cmath>
#include <type_traits>

// ratl includes
#include <ratl/detail/batch_endianness.hpp>
#include <ratl/detail/batch_round.hpp>
#include <ratl/detail/batch_sample_fix.hpp>
#include <ratl/detail/batch_traits.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/sample_type_limits.hpp>

#if defined(RATL_HAS_XSIMD)

namespace ratl
{
namespace detail
{
// BatchSampleConverter

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
struct BatchSampleConverter;

// intXX_t -> intXX_t

template<class SampleType, class DitherGenerator>
struct BatchSampleConverter<SampleType, SampleType, DitherGenerator>
{
    static inline const BatchSampleValueType_t<SampleType>& convert(
        const BatchSampleValueType_t<SampleType>& sample, DitherGenerator&) noexcept
    {
        return sample;
    }
};

// int16_t -> intXX_t

template<class DitherGenerator>
struct BatchSampleConverter<int16_t, int24_t, DitherGenerator>
{
    static inline BatchSampleValueType_t<int24_t> convert(
        const BatchSampleValueType_t<int16_t>& sample, DitherGenerator&) noexcept
    {
        return xsimd::batch_cast<int32_t>(sample) << 8;
    }
};

template<class DitherGenerator>
struct BatchSampleConverter<int16_t, int32_t, DitherGenerator>
{
    static inline BatchSampleValueType_t<int32_t> convert(
        const BatchSampleValueType_t<int16_t>& sample, DitherGenerator&) noexcept
    {
        return xsimd::batch_cast<int32_t>(sample) << 16;
    }
};

// int24_t -> intXX_t

template<class DitherGenerator>
struct BatchSampleConverter<int24_t, int16_t, DitherGenerator>
{
    static constexpr int32_t SampleInMax = static_cast<int32_t>(0x007FFF80);
    static constexpr int16_t SampleOutMax = SampleTypeLimits<int16_t>::max;
    static constexpr int32_t Rounding = static_cast<int32_t>(0x80);
    static constexpr std::size_t TotalShift = 8;
    static constexpr std::size_t PreDitherShift =
        DitherGenerator::Int16Bits > 0 ? DitherGenerator::Int16Bits - TotalShift : 0;
    static constexpr std::size_t PostDitherShift = TotalShift + PreDitherShift;

    static inline BatchSampleValueType_t<int16_t> convert(
        const BatchSampleValueType_t<int24_t>& sample, DitherGenerator& dither_generator) noexcept
    {
        auto cmp = sample >= BatchSampleValueType_t<int32_t>(SampleInMax);
        auto fixed_sample = batchFixNegativeSamples<int24_t>(sample);
        auto temp = (((fixed_sample + (Rounding + (fixed_sample >> 31))) << PreDitherShift) +
                     dither_generator.generateBatchInt16()) >>
                    PostDitherShift;
        auto max = BatchSampleValueType_t<int24_t>(SampleOutMax);
        return xsimd::batch_cast<int16_t>(xsimd::select(cmp, max, temp));
    }
};

template<class DitherGenerator>
constexpr int32_t BatchSampleConverter<int24_t, int16_t, DitherGenerator>::SampleInMax;
template<class DitherGenerator>
constexpr int16_t BatchSampleConverter<int24_t, int16_t, DitherGenerator>::SampleOutMax;
template<class DitherGenerator>
constexpr int32_t BatchSampleConverter<int24_t, int16_t, DitherGenerator>::Rounding;
template<class DitherGenerator>
constexpr std::size_t BatchSampleConverter<int24_t, int16_t, DitherGenerator>::TotalShift;
template<class DitherGenerator>
constexpr std::size_t BatchSampleConverter<int24_t, int16_t, DitherGenerator>::PreDitherShift;
template<class DitherGenerator>
constexpr std::size_t BatchSampleConverter<int24_t, int16_t, DitherGenerator>::PostDitherShift;

template<class DitherGenerator>
struct BatchSampleConverter<int24_t, int32_t, DitherGenerator>
{
    static inline BatchSampleValueType_t<int32_t> convert(
        const BatchSampleValueType_t<int24_t>& sample, DitherGenerator&) noexcept
    {
        return sample << 8;
    }
};

// int32_t -> intXX_t

template<class DitherGenerator>
struct BatchSampleConverter<int32_t, int16_t, DitherGenerator>
{
    static constexpr int32_t SampleInMax = static_cast<int32_t>(0x7FFF8000);
    static constexpr int16_t SampleOutMax = SampleTypeLimits<int16_t>::max;
    static constexpr int32_t Rounding = static_cast<int32_t>(0x8000);
    static constexpr std::size_t TotalShift = 16;
    static constexpr std::size_t PreDitherShift = TotalShift - DitherGenerator::Int16Bits;
    static constexpr std::size_t PostDitherShift = TotalShift - PreDitherShift;

    static inline BatchSampleValueType_t<int16_t> convert(
        const BatchSampleValueType_t<int32_t>& sample, DitherGenerator& dither_generator) noexcept
    {
        auto cmp = sample >= BatchSampleValueType_t<int32_t>(SampleInMax);
        auto temp =
            (((sample + (Rounding + (sample >> 31))) >> PreDitherShift) + dither_generator.generateBatchInt16()) >>
            PostDitherShift;
        auto max = BatchSampleValueType_t<int32_t>(SampleOutMax);
        return xsimd::batch_cast<int16_t>(xsimd::select(cmp, max, temp));
    }
};

template<class DitherGenerator>
constexpr int32_t BatchSampleConverter<int32_t, int16_t, DitherGenerator>::SampleInMax;
template<class DitherGenerator>
constexpr int16_t BatchSampleConverter<int32_t, int16_t, DitherGenerator>::SampleOutMax;
template<class DitherGenerator>
constexpr int32_t BatchSampleConverter<int32_t, int16_t, DitherGenerator>::Rounding;
template<class DitherGenerator>
constexpr std::size_t BatchSampleConverter<int32_t, int16_t, DitherGenerator>::TotalShift;
template<class DitherGenerator>
constexpr std::size_t BatchSampleConverter<int32_t, int16_t, DitherGenerator>::PreDitherShift;
template<class DitherGenerator>
constexpr std::size_t BatchSampleConverter<int32_t, int16_t, DitherGenerator>::PostDitherShift;

template<class DitherGenerator>
struct BatchSampleConverter<int32_t, int24_t, DitherGenerator>
{
    static constexpr int32_t SampleInMax = static_cast<int32_t>(0x7FFFFF80);
    static constexpr int24_t SampleOutMax = SampleTypeLimits<int24_t>::max;
    static constexpr int32_t Rounding = static_cast<int32_t>(0x80);

    static inline BatchSampleValueType_t<int24_t> convert(
        const BatchSampleValueType_t<int32_t>& sample, DitherGenerator&) noexcept
    {
        auto cmp = sample >= BatchSampleValueType_t<int32_t>(SampleInMax);
        auto temp = (sample + (Rounding + (sample >> 31))) >> 8;
        auto max = BatchSampleValueType_t<int32_t>(SampleOutMax);
        return xsimd::select(cmp, max, temp);
    }
};

template<class DitherGenerator>
constexpr int32_t BatchSampleConverter<int32_t, int24_t, DitherGenerator>::SampleInMax;
template<class DitherGenerator>
constexpr int24_t BatchSampleConverter<int32_t, int24_t, DitherGenerator>::SampleOutMax;
template<class DitherGenerator>
constexpr int32_t BatchSampleConverter<int32_t, int24_t, DitherGenerator>::Rounding;

// intXX_t -> float32_t

template<class SampleType, class DitherGenerator>
struct BatchSampleConverter<SampleType, float32_t, DitherGenerator>
{
    static constexpr float32_t Scaler = FloatConvertTraits<SampleType>::Divisor;

    static inline BatchSampleValueType_t<float32_t> convert(
        const BatchSampleValueType_t<SampleType>& sample, DitherGenerator&) noexcept
    {
        return xsimd::to_float(xsimd::batch_cast<int32_t>(sample)) * Scaler;
    }
};

template<class SampleType, class DitherGenerator>
constexpr float32_t BatchSampleConverter<SampleType, float32_t, DitherGenerator>::Scaler;

// float32_t -> intXX_t

template<class SampleType, class DitherGenerator>
struct BatchSampleConverter<float32_t, SampleType, DitherGenerator>
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
    static inline BatchSampleValueType_t<SampleType> convert(
        BatchSampleValueType_t<float32_t> sample, DitherGenerator& dither_generator) noexcept
    {
        auto out = batchRoundFloat32ToInt32((sample * Scaler) + dither_generator.generateBatchFloat32());
        out =
            xsimd::select(xsimd::bool_cast(sample >= SampleInMax), BatchSampleValueType_t<int32_t>(SampleOutMax), out);
        out = xsimd::select(xsimd::bool_cast(sample < SampleInMin), BatchSampleValueType_t<int32_t>(SampleOutMin), out);
        return xsimd::batch_cast<typename BatchSampleValueType_t<SampleType>::value_type>(out);
    }
};

template<class SampleType, class DitherGenerator>
constexpr float32_t BatchSampleConverter<float32_t, SampleType, DitherGenerator>::SampleInMax;
template<class SampleType, class DitherGenerator>
constexpr SampleType BatchSampleConverter<float32_t, SampleType, DitherGenerator>::SampleOutMax;
template<class SampleType, class DitherGenerator>
constexpr float32_t BatchSampleConverter<float32_t, SampleType, DitherGenerator>::SampleInMin;
template<class SampleType, class DitherGenerator>
constexpr SampleType BatchSampleConverter<float32_t, SampleType, DitherGenerator>::SampleOutMin;
template<class SampleType, class DitherGenerator>
constexpr float32_t BatchSampleConverter<float32_t, SampleType, DitherGenerator>::Scaler;

// float32_t -> float32_t

template<class DitherGenerator>
struct BatchSampleConverter<float32_t, float32_t, DitherGenerator>
{
    static inline BatchSampleValueType_t<float32_t> convert(
        const BatchSampleValueType_t<float32_t>& sample, DitherGenerator&) noexcept
    {
        return sample;
    }
};

// BatchSampleToNetworkConverter

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
struct BatchSampleToNetworkConverter
{
    static inline BatchNetworkSampleValueType_t<OutputSampleType> convert(
        const BatchSampleValueType_t<InputSampleType>& sample, DitherGenerator& dither_generator) noexcept
    {
        return BatchSampleToNetworkConverter<OutputSampleType, OutputSampleType, DitherGenerator>::convert(
            BatchSampleConverter<InputSampleType, OutputSampleType, DitherGenerator>::convert(sample, dither_generator),
            dither_generator);
    }
};

// XX_t -> XX_t

template<class SampleType, class DitherGenerator>
struct BatchSampleToNetworkConverter<SampleType, SampleType, DitherGenerator>
{
    static inline BatchNetworkSampleValueType_t<SampleType> convert(
        const BatchSampleValueType_t<SampleType>& sample, DitherGenerator&) noexcept
    {
        return batchSampleToNetworkSample<SampleType>(sample);
    }
};

// BatchNetworkToSampleConverter

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
struct BatchNetworkToSampleConverter
{
    static inline BatchSampleValueType_t<OutputSampleType> convert(
        const BatchNetworkSampleValueType_t<InputSampleType>& sample, DitherGenerator& dither_generator) noexcept
    {
        return BatchSampleConverter<InputSampleType, OutputSampleType, DitherGenerator>::convert(
            BatchNetworkToSampleConverter<InputSampleType, InputSampleType, DitherGenerator>::convert(
                sample, dither_generator),
            dither_generator);
    }
};

// XX_t -> XX_t

template<class SampleType, class DitherGenerator>
struct BatchNetworkToSampleConverter<SampleType, SampleType, DitherGenerator>
{
    static inline BatchSampleValueType_t<SampleType> convert(
        const BatchNetworkSampleValueType_t<SampleType>& sample, DitherGenerator&) noexcept
    {
        return batchNetworkSampleToSample<SampleType>(sample);
    }
};

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_detail_batch_sample_converter_
