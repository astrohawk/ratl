#ifndef _ratl_detail_batch_fast_sample_converter_
#define _ratl_detail_batch_fast_sample_converter_

// C++ Standard Library includes
#include <cmath>
#include <type_traits>

// ratl includes
#include <ratl/detail/batch_cast.hpp>
#include <ratl/detail/batch_endianness.hpp>
#include <ratl/detail/batch_round.hpp>
#include <ratl/detail/batch_traits.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/detail/convert_traits.hpp>
#include <ratl/sample_type_limits.hpp>

#if defined(RATL_HAS_XSIMD)

namespace ratl
{
namespace detail
{
// BatchFastSampleConverter

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
struct BatchFastSampleConverter;

// intXX_t -> intXX_t

template<class SampleType, class DitherGenerator>
struct BatchFastSampleConverter<SampleType, SampleType, DitherGenerator>
{
    static inline const BatchSampleValueType_t<SampleType>& convert(
        const BatchSampleValueType_t<SampleType>& sample, DitherGenerator&) noexcept
    {
        return sample;
    }
};

// int16_t -> intXX_t

template<class DitherGenerator>
struct BatchFastSampleConverter<int16_t, int24_t, DitherGenerator>
{
    static inline BatchSampleValueType_t<int24_t> convert(
        const BatchSampleValueType_t<int16_t>& sample, DitherGenerator&) noexcept
    {
        return batchSampleCast<int24_t>(sample) << 8;
    }
};

template<class DitherGenerator>
struct BatchFastSampleConverter<int16_t, int32_t, DitherGenerator>
{
    static inline BatchSampleValueType_t<int32_t> convert(
        const BatchSampleValueType_t<int16_t>& sample, DitherGenerator&) noexcept
    {
        return batchSampleCast<int32_t>(sample) << 16;
    }
};

// int24_t -> intXX_t

template<class DitherGenerator>
struct BatchFastSampleConverter<int24_t, int16_t, DitherGenerator>
{
    static constexpr std::size_t TotalShift = 8;
    static constexpr std::size_t PreDitherShift =
        DitherGenerator::Int16Bits > 0 ? DitherGenerator::Int16Bits - TotalShift : 0;
    static constexpr std::size_t PostDitherShift = TotalShift + PreDitherShift;

    static inline BatchSampleValueType_t<int16_t> convert(
        const BatchSampleValueType_t<int24_t>& sample, DitherGenerator& dither_generator) noexcept
    {
        return batchSampleCast<int16_t>(
            ((sample << PreDitherShift) + dither_generator.generateBatchInt16()) >> PostDitherShift);
    }
};

template<class DitherGenerator>
constexpr std::size_t BatchFastSampleConverter<int24_t, int16_t, DitherGenerator>::TotalShift;
template<class DitherGenerator>
constexpr std::size_t BatchFastSampleConverter<int24_t, int16_t, DitherGenerator>::PreDitherShift;
template<class DitherGenerator>
constexpr std::size_t BatchFastSampleConverter<int24_t, int16_t, DitherGenerator>::PostDitherShift;

template<class DitherGenerator>
struct BatchFastSampleConverter<int24_t, int32_t, DitherGenerator>
{
    static inline BatchSampleValueType_t<int32_t> convert(
        const BatchSampleValueType_t<int24_t>& sample, DitherGenerator&) noexcept
    {
        return batchSampleCast<int24_t>(sample << 8);
    }
};

// int32_t -> intXX_t

template<class DitherGenerator>
struct BatchFastSampleConverter<int32_t, int16_t, DitherGenerator>
{
    static constexpr std::size_t TotalShift = 16;
    static constexpr std::size_t PreDitherShift = TotalShift - DitherGenerator::Int16Bits;
    static constexpr std::size_t PostDitherShift = TotalShift - PreDitherShift;

    static inline BatchSampleValueType_t<int16_t> convert(
        const BatchSampleValueType_t<int32_t>& sample, DitherGenerator& dither_generator) noexcept
    {
        return batchSampleCast<int16_t>(
            ((sample >> PreDitherShift) + dither_generator.generateBatchInt16()) >> PostDitherShift);
    }
};

template<class DitherGenerator>
constexpr std::size_t BatchFastSampleConverter<int32_t, int16_t, DitherGenerator>::TotalShift;
template<class DitherGenerator>
constexpr std::size_t BatchFastSampleConverter<int32_t, int16_t, DitherGenerator>::PreDitherShift;
template<class DitherGenerator>
constexpr std::size_t BatchFastSampleConverter<int32_t, int16_t, DitherGenerator>::PostDitherShift;

template<class DitherGenerator>
struct BatchFastSampleConverter<int32_t, int24_t, DitherGenerator>
{
    static inline BatchSampleValueType_t<int24_t> convert(
        const BatchSampleValueType_t<int32_t>& sample, DitherGenerator&) noexcept
    {
        return batchSampleCast<int24_t>(sample >> 8);
    }
};

// intXX_t -> float32_t

template<class SampleType, class DitherGenerator>
struct BatchFastSampleConverter<SampleType, float32_t, DitherGenerator>
{
    static constexpr float32_t Scaler = FloatConvertTraits<SampleType>::Divisor;

    static inline BatchSampleValueType_t<float32_t> convert(
        const BatchSampleValueType_t<SampleType>& sample, DitherGenerator&) noexcept
    {
        return xsimd::to_float(batchSampleCast<int32_t>(sample)) * Scaler;
    }
};

template<class SampleType, class DitherGenerator>
constexpr float32_t BatchFastSampleConverter<SampleType, float32_t, DitherGenerator>::Scaler;

// float32_t -> intXX_t

template<class SampleType, class DitherGenerator>
struct BatchFastSampleConverter<float32_t, SampleType, DitherGenerator>
{
private:
    static constexpr float32_t PositiveScaler =
        static_cast<float32_t>(SampleTypeLimits<SampleType>::max) - DitherGenerator::MaxFloat32;
    static constexpr float32_t NegativeScaler =
        -static_cast<float32_t>(SampleTypeLimits<SampleType>::min) - DitherGenerator::MaxFloat32;

public:
    static inline BatchSampleValueType_t<SampleType> convert(
        BatchSampleValueType_t<float32_t> sample, DitherGenerator& dither_generator) noexcept
    {
        auto dither = dither_generator.generateBatchFloat32();
        auto positive_out = batchRoundFloat32ToInt32((sample * PositiveScaler) + dither);
        auto negative_out = batchRoundFloat32ToInt32((sample * NegativeScaler) + dither);
        return batchSampleCast<SampleType>(xsimd::select(xsimd::bool_cast(sample < 0), negative_out, positive_out));
    }
};

template<class SampleType, class DitherGenerator>
constexpr float32_t BatchFastSampleConverter<float32_t, SampleType, DitherGenerator>::PositiveScaler;
template<class SampleType, class DitherGenerator>
constexpr float32_t BatchFastSampleConverter<float32_t, SampleType, DitherGenerator>::NegativeScaler;

// float32_t -> float32_t

template<class DitherGenerator>
struct BatchFastSampleConverter<float32_t, float32_t, DitherGenerator>
{
    static inline BatchSampleValueType_t<float32_t> convert(
        const BatchSampleValueType_t<float32_t>& sample, DitherGenerator&) noexcept
    {
        return sample;
    }
};

// BatchSampleToNetworkConverter

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
struct BatchFastSampleToNetworkConverter
{
    static inline BatchNetworkSampleValueType_t<OutputSampleType> convert(
        const BatchSampleValueType_t<InputSampleType>& sample, DitherGenerator& dither_generator) noexcept
    {
        return BatchFastSampleToNetworkConverter<OutputSampleType, OutputSampleType, DitherGenerator>::convert(
            BatchFastSampleConverter<InputSampleType, OutputSampleType, DitherGenerator>::convert(
                sample, dither_generator),
            dither_generator);
    }
};

// XX_t -> XX_t

template<class SampleType, class DitherGenerator>
struct BatchFastSampleToNetworkConverter<SampleType, SampleType, DitherGenerator>
{
    static inline BatchNetworkSampleValueType_t<SampleType> convert(
        const BatchSampleValueType_t<SampleType>& sample, DitherGenerator&) noexcept
    {
        return batchSampleToNetworkSample<SampleType>(sample);
    }
};

// BatchNetworkToSampleConverter

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
struct BatchFastNetworkToSampleConverter
{
    static inline BatchSampleValueType_t<OutputSampleType> convert(
        const BatchNetworkSampleValueType_t<InputSampleType>& sample, DitherGenerator& dither_generator) noexcept
    {
        return BatchFastSampleConverter<InputSampleType, OutputSampleType, DitherGenerator>::convert(
            BatchFastNetworkToSampleConverter<InputSampleType, InputSampleType, DitherGenerator>::convert(
                sample, dither_generator),
            dither_generator);
    }
};

// XX_t -> XX_t

template<class SampleType, class DitherGenerator>
struct BatchFastNetworkToSampleConverter<SampleType, SampleType, DitherGenerator>
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

#endif // _ratl_detail_batch_fast_sample_converter_
