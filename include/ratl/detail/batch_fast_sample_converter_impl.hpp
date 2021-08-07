#ifndef _ratl_detail_batch_fast_sample_converter_impl_
#define _ratl_detail_batch_fast_sample_converter_impl_

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
#include <ratl/detail/fast_sample_converter_impl.hpp>
#include <ratl/sample_type_limits.hpp>

namespace ratl
{
namespace detail
{
#if defined(RATL_HAS_XSIMD)

// BaseBatchFastSampleConverterImpl

template<class InputSample, class OutputSample, class DitherGenerator>
struct BaseBatchFastSampleConverterImpl;

template<class SampleType, class DitherGenerator>
struct BaseBatchFastSampleConverterImpl<Sample<SampleType>, Sample<SampleType>, DitherGenerator>
{
    static inline const BatchSampleValueType_t<SampleType>& batch_convert(
        const BatchSampleValueType_t<SampleType>& sample, DitherGenerator&) noexcept
    {
        return sample;
    }
};

template<class DitherGenerator>
struct BaseBatchFastSampleConverterImpl<Sample<int16_t>, Sample<int24_t>, DitherGenerator>
{
    static inline BatchSampleValueType_t<int24_t> batch_convert(
        const BatchSampleValueType_t<int16_t>& sample, DitherGenerator&) noexcept
    {
        return batchSampleCast<int24_t>(sample) << 8;
    }
};

template<class DitherGenerator>
struct BaseBatchFastSampleConverterImpl<Sample<int16_t>, Sample<int32_t>, DitherGenerator>
{
    static inline BatchSampleValueType_t<int32_t> batch_convert(
        const BatchSampleValueType_t<int16_t>& sample, DitherGenerator&) noexcept
    {
        return batchSampleCast<int32_t>(sample) << 16;
    }
};

template<class DitherGenerator>
struct BaseBatchFastSampleConverterImpl<Sample<int24_t>, Sample<int16_t>, DitherGenerator>
{
    static constexpr std::size_t TotalShift = 8;
    static constexpr std::size_t PreDitherShift =
        DitherGenerator::Int16Bits > 0 ? DitherGenerator::Int16Bits - TotalShift : 0;
    static constexpr std::size_t PostDitherShift = TotalShift + PreDitherShift;

    static inline BatchSampleValueType_t<int16_t> batch_convert(
        const BatchSampleValueType_t<int24_t>& sample, DitherGenerator& dither_generator) noexcept
    {
        return batchSampleCast<int16_t>(
            ((sample << PreDitherShift) + dither_generator.generateBatchInt16()) >> PostDitherShift);
    }
};

template<class DitherGenerator>
constexpr std::size_t BaseBatchFastSampleConverterImpl<Sample<int24_t>, Sample<int16_t>, DitherGenerator>::TotalShift;
template<class DitherGenerator>
constexpr std::size_t
    BaseBatchFastSampleConverterImpl<Sample<int24_t>, Sample<int16_t>, DitherGenerator>::PreDitherShift;
template<class DitherGenerator>
constexpr std::size_t
    BaseBatchFastSampleConverterImpl<Sample<int24_t>, Sample<int16_t>, DitherGenerator>::PostDitherShift;

template<class DitherGenerator>
struct BaseBatchFastSampleConverterImpl<Sample<int24_t>, Sample<int32_t>, DitherGenerator>
{
    static inline BatchSampleValueType_t<int32_t> batch_convert(
        const BatchSampleValueType_t<int24_t>& sample, DitherGenerator&) noexcept
    {
        return batchSampleCast<int24_t>(sample << 8);
    }
};

template<class DitherGenerator>
struct BaseBatchFastSampleConverterImpl<Sample<int32_t>, Sample<int16_t>, DitherGenerator>
{
    static constexpr std::size_t TotalShift = 16;
    static constexpr std::size_t PreDitherShift = TotalShift - DitherGenerator::Int16Bits;
    static constexpr std::size_t PostDitherShift = TotalShift - PreDitherShift;

    static inline BatchSampleValueType_t<int16_t> batch_convert(
        const BatchSampleValueType_t<int32_t>& sample, DitherGenerator& dither_generator) noexcept
    {
        return batchSampleCast<int16_t>(
            ((sample >> PreDitherShift) + dither_generator.generateBatchInt16()) >> PostDitherShift);
    }
};

template<class DitherGenerator>
constexpr std::size_t BaseBatchFastSampleConverterImpl<Sample<int32_t>, Sample<int16_t>, DitherGenerator>::TotalShift;
template<class DitherGenerator>
constexpr std::size_t
    BaseBatchFastSampleConverterImpl<Sample<int32_t>, Sample<int16_t>, DitherGenerator>::PreDitherShift;
template<class DitherGenerator>
constexpr std::size_t
    BaseBatchFastSampleConverterImpl<Sample<int32_t>, Sample<int16_t>, DitherGenerator>::PostDitherShift;

template<class DitherGenerator>
struct BaseBatchFastSampleConverterImpl<Sample<int32_t>, Sample<int24_t>, DitherGenerator>
{
    static inline BatchSampleValueType_t<int24_t> batch_convert(
        const BatchSampleValueType_t<int32_t>& sample, DitherGenerator&) noexcept
    {
        return batchSampleCast<int24_t>(sample >> 8);
    }
};

template<class SampleType, class DitherGenerator>
struct BaseBatchFastSampleConverterImpl<Sample<SampleType>, Sample<float32_t>, DitherGenerator>
{
    static constexpr float32_t Scaler = FloatConvertTraits<SampleType>::Divisor;

    static inline BatchSampleValueType_t<float32_t> batch_convert(
        const BatchSampleValueType_t<SampleType>& sample, DitherGenerator&) noexcept
    {
        return xsimd::to_float(batchSampleCast<int32_t>(sample)) * Scaler;
    }
};

template<class SampleType, class DitherGenerator>
constexpr float32_t BaseBatchFastSampleConverterImpl<Sample<SampleType>, Sample<float32_t>, DitherGenerator>::Scaler;

template<class SampleType, class DitherGenerator>
struct BaseBatchFastSampleConverterImpl<Sample<float32_t>, Sample<SampleType>, DitherGenerator>
{
private:
    static constexpr float32_t PositiveScaler =
        static_cast<float32_t>(SampleTypeLimits<SampleType>::max) - DitherGenerator::MaxFloat32;
    static constexpr float32_t NegativeScaler =
        -static_cast<float32_t>(SampleTypeLimits<SampleType>::min) - DitherGenerator::MaxFloat32;

public:
    static inline BatchSampleValueType_t<SampleType> batch_convert(
        BatchSampleValueType_t<float32_t> sample, DitherGenerator& dither_generator) noexcept
    {
        auto dither = dither_generator.generateBatchFloat32();
        auto positive_out = batchRoundFloat32ToInt32((sample * PositiveScaler) + dither);
        auto negative_out = batchRoundFloat32ToInt32((sample * NegativeScaler) + dither);
        return batchSampleCast<SampleType>(xsimd::select(xsimd::bool_cast(sample < 0), negative_out, positive_out));
    }
};

template<class SampleType, class DitherGenerator>
constexpr float32_t
    BaseBatchFastSampleConverterImpl<Sample<float32_t>, Sample<SampleType>, DitherGenerator>::PositiveScaler;
template<class SampleType, class DitherGenerator>
constexpr float32_t
    BaseBatchFastSampleConverterImpl<Sample<float32_t>, Sample<SampleType>, DitherGenerator>::NegativeScaler;

template<class DitherGenerator>
struct BaseBatchFastSampleConverterImpl<Sample<float32_t>, Sample<float32_t>, DitherGenerator>
{
    static inline BatchSampleValueType_t<float32_t> batch_convert(
        const BatchSampleValueType_t<float32_t>& sample, DitherGenerator&) noexcept
    {
        return sample;
    }
};

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
struct BaseBatchFastSampleConverterImpl<Sample<InputSampleType>, NetworkSample<OutputSampleType>, DitherGenerator>
{
    static inline BatchNetworkSampleValueType_t<OutputSampleType> batch_convert(
        const BatchSampleValueType_t<InputSampleType>& sample, DitherGenerator& dither_generator) noexcept
    {
        return BaseBatchFastSampleConverterImpl<
            Sample<OutputSampleType>,
            NetworkSample<OutputSampleType>,
            DitherGenerator>::
            batch_convert(
                BaseBatchFastSampleConverterImpl<Sample<InputSampleType>, Sample<OutputSampleType>, DitherGenerator>::
                    batch_convert(sample, dither_generator),
                dither_generator);
    }
};

template<class SampleType, class DitherGenerator>
struct BaseBatchFastSampleConverterImpl<Sample<SampleType>, NetworkSample<SampleType>, DitherGenerator>
{
    static inline BatchNetworkSampleValueType_t<SampleType> batch_convert(
        const BatchSampleValueType_t<SampleType>& sample, DitherGenerator&) noexcept
    {
        return batchSampleToNetworkSample<SampleType>(sample);
    }
};

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
struct BaseBatchFastSampleConverterImpl<NetworkSample<InputSampleType>, Sample<OutputSampleType>, DitherGenerator>
{
    static inline BatchSampleValueType_t<OutputSampleType> batch_convert(
        const BatchNetworkSampleValueType_t<InputSampleType>& sample, DitherGenerator& dither_generator) noexcept
    {
        return BaseBatchFastSampleConverterImpl<Sample<InputSampleType>, Sample<OutputSampleType>, DitherGenerator>::
            batch_convert(
                BaseBatchFastSampleConverterImpl<
                    NetworkSample<InputSampleType>,
                    Sample<InputSampleType>,
                    DitherGenerator>::batch_convert(sample, dither_generator),
                dither_generator);
    }
};

template<class SampleType, class DitherGenerator>
struct BaseBatchFastSampleConverterImpl<NetworkSample<SampleType>, Sample<SampleType>, DitherGenerator>
{
    static inline BatchSampleValueType_t<SampleType> batch_convert(
        const BatchNetworkSampleValueType_t<SampleType>& sample, DitherGenerator&) noexcept
    {
        return batchNetworkSampleToSample<SampleType>(sample);
    }
};

// BatchFastSampleConverterImpl

template<class InputSample, class OutputSample, class DitherGenerator>
struct BatchFastSampleConverterImpl :
    BaseBatchFastSampleConverterImpl<InputSample, OutputSample, DitherGenerator>,
    FastSampleConverterImpl<InputSample, OutputSample, DitherGenerator>
{
};

#else

template<class InputSample, class OutputSample, class DitherGenerator>
using BatchFastSampleConverterImpl = FastSampleConverterImpl<InputSample, OutputSample, DitherGenerator>;

#endif

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_batch_fast_sample_converter_impl_
