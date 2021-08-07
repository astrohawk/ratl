#ifndef _ratl_detail_batch_reference_sample_converter_impl_
#define _ratl_detail_batch_reference_sample_converter_impl_

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
#include <ratl/detail/reference_sample_converter_impl.hpp>
#include <ratl/sample_type_limits.hpp>

namespace ratl
{
namespace detail
{
#if defined(RATL_HAS_XSIMD)

// BaseBatchReferenceSampleConverterImpl

template<class InputSample, class OutputSample, class DitherGenerator>
struct BaseBatchReferenceSampleConverterImpl;

template<class SampleType, class DitherGenerator>
struct BaseBatchReferenceSampleConverterImpl<Sample<SampleType>, Sample<SampleType>, DitherGenerator>
{
    static inline const BatchSampleValueType_t<SampleType>& batch_convert(
        const BatchSampleValueType_t<SampleType>& sample, DitherGenerator&) noexcept
    {
        return sample;
    }
};

template<class DitherGenerator>
struct BaseBatchReferenceSampleConverterImpl<Sample<int16_t>, Sample<int24_t>, DitherGenerator>
{
    static inline BatchSampleValueType_t<int24_t> batch_convert(
        const BatchSampleValueType_t<int16_t>& sample, DitherGenerator&) noexcept
    {
        return batchSampleCast<int24_t>(sample) << 8;
    }
};

template<class DitherGenerator>
struct BaseBatchReferenceSampleConverterImpl<Sample<int16_t>, Sample<int32_t>, DitherGenerator>
{
    static inline BatchSampleValueType_t<int32_t> batch_convert(
        const BatchSampleValueType_t<int16_t>& sample, DitherGenerator&) noexcept
    {
        return batchSampleCast<int32_t>(sample) << 16;
    }
};

template<class DitherGenerator>
struct BaseBatchReferenceSampleConverterImpl<Sample<int24_t>, Sample<int16_t>, DitherGenerator>
{
    static constexpr int32_t SampleInMax = static_cast<int32_t>(0x007FFF80);
    static constexpr int16_t SampleOutMax = SampleTypeLimits<int16_t>::max;
    static constexpr int32_t Rounding = static_cast<int32_t>(0x80);
    static constexpr std::size_t TotalShift = 8;
    static constexpr std::size_t PreDitherShift =
        DitherGenerator::Int16Bits > 0 ? DitherGenerator::Int16Bits - TotalShift : 0;
    static constexpr std::size_t PostDitherShift = TotalShift + PreDitherShift;

    static inline BatchSampleValueType_t<int16_t> batch_convert(
        const BatchSampleValueType_t<int24_t>& sample, DitherGenerator& dither_generator) noexcept
    {
        static const BatchSampleValueType_t<int24_t> max(SampleOutMax);
        auto cmp = sample >= SampleInMax;
        auto temp =
            (((sample + (Rounding + (sample >> 31))) << PreDitherShift) + dither_generator.generateBatchInt16()) >>
            PostDitherShift;
        return batchSampleCast<int16_t>(xsimd::select(cmp, max, temp));
    }
};

template<class DitherGenerator>
constexpr int32_t BaseBatchReferenceSampleConverterImpl<Sample<int24_t>, Sample<int16_t>, DitherGenerator>::SampleInMax;
template<class DitherGenerator>
constexpr int16_t
    BaseBatchReferenceSampleConverterImpl<Sample<int24_t>, Sample<int16_t>, DitherGenerator>::SampleOutMax;
template<class DitherGenerator>
constexpr int32_t BaseBatchReferenceSampleConverterImpl<Sample<int24_t>, Sample<int16_t>, DitherGenerator>::Rounding;
template<class DitherGenerator>
constexpr std::size_t
    BaseBatchReferenceSampleConverterImpl<Sample<int24_t>, Sample<int16_t>, DitherGenerator>::TotalShift;
template<class DitherGenerator>
constexpr std::size_t
    BaseBatchReferenceSampleConverterImpl<Sample<int24_t>, Sample<int16_t>, DitherGenerator>::PreDitherShift;
template<class DitherGenerator>
constexpr std::size_t
    BaseBatchReferenceSampleConverterImpl<Sample<int24_t>, Sample<int16_t>, DitherGenerator>::PostDitherShift;

template<class DitherGenerator>
struct BaseBatchReferenceSampleConverterImpl<Sample<int24_t>, Sample<int32_t>, DitherGenerator>
{
    static inline BatchSampleValueType_t<int32_t> batch_convert(
        const BatchSampleValueType_t<int24_t>& sample, DitherGenerator&) noexcept
    {
        return batchSampleCast<int24_t>(sample << 8);
    }
};

template<class DitherGenerator>
struct BaseBatchReferenceSampleConverterImpl<Sample<int32_t>, Sample<int16_t>, DitherGenerator>
{
    static constexpr int32_t SampleInMax = static_cast<int32_t>(0x7FFF8000);
    static constexpr int16_t SampleOutMax = SampleTypeLimits<int16_t>::max;
    static constexpr int32_t Rounding = static_cast<int32_t>(0x8000);
    static constexpr std::size_t TotalShift = 16;
    static constexpr std::size_t PreDitherShift = TotalShift - DitherGenerator::Int16Bits;
    static constexpr std::size_t PostDitherShift = TotalShift - PreDitherShift;

    static inline BatchSampleValueType_t<int16_t> batch_convert(
        const BatchSampleValueType_t<int32_t>& sample, DitherGenerator& dither_generator) noexcept
    {
        static const BatchSampleValueType_t<int32_t> max(SampleOutMax);
        auto cmp = sample >= SampleInMax;
        auto temp =
            (((sample + (Rounding + (sample >> 31))) >> PreDitherShift) + dither_generator.generateBatchInt16()) >>
            PostDitherShift;
        return batchSampleCast<int16_t>(xsimd::select(cmp, max, temp));
    }
};

template<class DitherGenerator>
constexpr int32_t BaseBatchReferenceSampleConverterImpl<Sample<int32_t>, Sample<int16_t>, DitherGenerator>::SampleInMax;
template<class DitherGenerator>
constexpr int16_t
    BaseBatchReferenceSampleConverterImpl<Sample<int32_t>, Sample<int16_t>, DitherGenerator>::SampleOutMax;
template<class DitherGenerator>
constexpr int32_t BaseBatchReferenceSampleConverterImpl<Sample<int32_t>, Sample<int16_t>, DitherGenerator>::Rounding;
template<class DitherGenerator>
constexpr std::size_t
    BaseBatchReferenceSampleConverterImpl<Sample<int32_t>, Sample<int16_t>, DitherGenerator>::TotalShift;
template<class DitherGenerator>
constexpr std::size_t
    BaseBatchReferenceSampleConverterImpl<Sample<int32_t>, Sample<int16_t>, DitherGenerator>::PreDitherShift;
template<class DitherGenerator>
constexpr std::size_t
    BaseBatchReferenceSampleConverterImpl<Sample<int32_t>, Sample<int16_t>, DitherGenerator>::PostDitherShift;

template<class DitherGenerator>
struct BaseBatchReferenceSampleConverterImpl<Sample<int32_t>, Sample<int24_t>, DitherGenerator>
{
    static constexpr int32_t SampleInMax = static_cast<int32_t>(0x7FFFFF80);
    static constexpr int24_t SampleOutMax = SampleTypeLimits<int24_t>::max;
    static constexpr int32_t Rounding = static_cast<int32_t>(0x80);

    static inline BatchSampleValueType_t<int24_t> batch_convert(
        const BatchSampleValueType_t<int32_t>& sample, DitherGenerator&) noexcept
    {
        static const BatchSampleValueType_t<int32_t> max(SampleOutMax);
        auto cmp = sample >= SampleInMax;
        auto temp = (sample + (Rounding + (sample >> 31))) >> 8;
        return batchSampleCast<int24_t>(xsimd::select(cmp, max, temp));
    }
};

template<class DitherGenerator>
constexpr int32_t BaseBatchReferenceSampleConverterImpl<Sample<int32_t>, Sample<int24_t>, DitherGenerator>::SampleInMax;
template<class DitherGenerator>
constexpr int24_t
    BaseBatchReferenceSampleConverterImpl<Sample<int32_t>, Sample<int24_t>, DitherGenerator>::SampleOutMax;
template<class DitherGenerator>
constexpr int32_t BaseBatchReferenceSampleConverterImpl<Sample<int32_t>, Sample<int24_t>, DitherGenerator>::Rounding;

template<class SampleType, class DitherGenerator>
struct BaseBatchReferenceSampleConverterImpl<Sample<SampleType>, Sample<float32_t>, DitherGenerator>
{
    static constexpr float32_t Scaler = FloatConvertTraits<SampleType>::Divisor;

    static inline BatchSampleValueType_t<float32_t> batch_convert(
        const BatchSampleValueType_t<SampleType>& sample, DitherGenerator&) noexcept
    {
        return xsimd::to_float(batchSampleCast<int32_t>(sample)) * Scaler;
    }
};

template<class SampleType, class DitherGenerator>
constexpr float32_t
    BaseBatchReferenceSampleConverterImpl<Sample<SampleType>, Sample<float32_t>, DitherGenerator>::Scaler;

template<class SampleType, class DitherGenerator>
struct BaseBatchReferenceSampleConverterImpl<Sample<float32_t>, Sample<SampleType>, DitherGenerator>
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
    static inline BatchSampleValueType_t<SampleType> batch_convert(
        BatchSampleValueType_t<float32_t> sample, DitherGenerator& dither_generator) noexcept
    {
        static const BatchSampleValueType_t<int32_t> min(SampleOutMin);
        static const BatchSampleValueType_t<int32_t> max(SampleOutMax);
        auto out = batchRoundFloat32ToInt32((sample * Scaler) + dither_generator.generateBatchFloat32());
        out = xsimd::select(xsimd::bool_cast(sample >= SampleInMax), max, out);
        out = xsimd::select(xsimd::bool_cast(sample < SampleInMin), min, out);
        return batchSampleCast<SampleType>(out);
    }
};

template<class SampleType, class DitherGenerator>
constexpr float32_t
    BaseBatchReferenceSampleConverterImpl<Sample<float32_t>, Sample<SampleType>, DitherGenerator>::SampleInMax;
template<class SampleType, class DitherGenerator>
constexpr SampleType
    BaseBatchReferenceSampleConverterImpl<Sample<float32_t>, Sample<SampleType>, DitherGenerator>::SampleOutMax;
template<class SampleType, class DitherGenerator>
constexpr float32_t
    BaseBatchReferenceSampleConverterImpl<Sample<float32_t>, Sample<SampleType>, DitherGenerator>::SampleInMin;
template<class SampleType, class DitherGenerator>
constexpr SampleType
    BaseBatchReferenceSampleConverterImpl<Sample<float32_t>, Sample<SampleType>, DitherGenerator>::SampleOutMin;
template<class SampleType, class DitherGenerator>
constexpr float32_t
    BaseBatchReferenceSampleConverterImpl<Sample<float32_t>, Sample<SampleType>, DitherGenerator>::Scaler;

template<class DitherGenerator>
struct BaseBatchReferenceSampleConverterImpl<Sample<float32_t>, Sample<float32_t>, DitherGenerator>
{
    static inline BatchSampleValueType_t<float32_t> batch_convert(
        const BatchSampleValueType_t<float32_t>& sample, DitherGenerator&) noexcept
    {
        return sample;
    }
};

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
struct BaseBatchReferenceSampleConverterImpl<Sample<InputSampleType>, NetworkSample<OutputSampleType>, DitherGenerator>
{
    static inline BatchNetworkSampleValueType_t<OutputSampleType> batch_convert(
        const BatchSampleValueType_t<InputSampleType>& sample, DitherGenerator& dither_generator) noexcept
    {
        return BaseBatchReferenceSampleConverterImpl<
            Sample<OutputSampleType>,
            NetworkSample<OutputSampleType>,
            DitherGenerator>::
            batch_convert(
                BaseBatchReferenceSampleConverterImpl<
                    Sample<InputSampleType>,
                    Sample<OutputSampleType>,
                    DitherGenerator>::batch_convert(sample, dither_generator),
                dither_generator);
    }
};

template<class SampleType, class DitherGenerator>
struct BaseBatchReferenceSampleConverterImpl<Sample<SampleType>, NetworkSample<SampleType>, DitherGenerator>
{
    static inline BatchNetworkSampleValueType_t<SampleType> batch_convert(
        const BatchSampleValueType_t<SampleType>& sample, DitherGenerator&) noexcept
    {
        return batchSampleToNetworkSample<SampleType>(sample);
    }
};

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
struct BaseBatchReferenceSampleConverterImpl<NetworkSample<InputSampleType>, Sample<OutputSampleType>, DitherGenerator>
{
    static inline BatchSampleValueType_t<OutputSampleType> batch_convert(
        const BatchNetworkSampleValueType_t<InputSampleType>& sample, DitherGenerator& dither_generator) noexcept
    {
        return BaseBatchReferenceSampleConverterImpl<
            Sample<InputSampleType>,
            Sample<OutputSampleType>,
            DitherGenerator>::
            batch_convert(
                BaseBatchReferenceSampleConverterImpl<
                    NetworkSample<InputSampleType>,
                    Sample<InputSampleType>,
                    DitherGenerator>::batch_convert(sample, dither_generator),
                dither_generator);
    }
};

template<class SampleType, class DitherGenerator>
struct BaseBatchReferenceSampleConverterImpl<NetworkSample<SampleType>, Sample<SampleType>, DitherGenerator>
{
    static inline BatchSampleValueType_t<SampleType> batch_convert(
        const BatchNetworkSampleValueType_t<SampleType>& sample, DitherGenerator&) noexcept
    {
        return batchNetworkSampleToSample<SampleType>(sample);
    }
};

// BatchReferenceSampleConverterImpl

template<class InputSample, class OutputSample, class DitherGenerator>
struct BatchReferenceSampleConverterImpl :
    BaseBatchReferenceSampleConverterImpl<InputSample, OutputSample, DitherGenerator>,
    ReferenceSampleConverterImpl<InputSample, OutputSample, DitherGenerator>
{
};

#else

template<class InputSample, class OutputSample, class DitherGenerator>
using BatchReferenceSampleConverterImpl = ReferenceSampleConverterImpl<InputSample, OutputSample, DitherGenerator>;

#endif

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_batch_reference_sample_converter_impl_
