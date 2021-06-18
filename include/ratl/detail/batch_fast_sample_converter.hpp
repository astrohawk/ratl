#ifndef _ratl_detail_batch_sample_converter_
#define _ratl_detail_batch_sample_converter_

// C++ Standard Library includes
#include <cmath>
#include <type_traits>

// ratl includes
#include <ratl/detail/batch_traits.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/sample_type_limits.hpp>

#if defined(RATL_HAS_XSIMD)

namespace ratl
{
namespace detail
{
// BatchSampleConverter

template<class InputSampleType, class OutputSampleType>
struct BatchSampleConverter;

// intXX_t -> intXX_t

template<class SampleType>
struct BatchSampleConverter<SampleType, SampleType>
{
    static inline const BatchSampleValueType_t<SampleType>& convert(
        const BatchSampleValueType_t<SampleType>& sample) noexcept
    {
        return sample;
    }
};

// int16_t -> intXX_t

template<>
struct BatchSampleConverter<int16_t, int24_t>
{
    static inline BatchSampleValueType_t<int24_t> convert(const BatchSampleValueType_t<int16_t>& sample) noexcept
    {
        return sample << 8;
    }
};

template<>
struct BatchSampleConverter<int16_t, int32_t>
{
    static inline BatchSampleValueType_t<int32_t> convert(const BatchSampleValueType_t<int16_t>& sample) noexcept
    {
        return sample << 16;
    }
};

// int24_t -> intXX_t

template<>
struct BatchSampleConverter<int24_t, int16_t>
{
    static inline BatchSampleValueType_t<int16_t> convert(const BatchSampleValueType_t<int24_t>& sample) noexcept
    {
        auto cmp = sample >= BatchSampleValueType_t<int32_t>(0x007FFF80);
        auto temp = (sample + 0x80) >> 8;
        auto max = BatchSampleValueType_t<int24_t>(SampleTypeLimits<int24_t>::max);
        return xsimd::select(cmp, max, temp);
    }
};

template<>
struct BatchSampleConverter<int24_t, int32_t>
{
    static inline BatchSampleValueType_t<int32_t> convert(const BatchSampleValueType_t<int24_t>& sample) noexcept
    {
        return sample << 8;
    }
};

// int32_t -> intXX_t

template<>
struct BatchSampleConverter<int32_t, int16_t>
{
    static inline BatchSampleValueType_t<int16_t> convert(const BatchSampleValueType_t<int32_t>& sample) noexcept
    {
        auto cmp = sample >= BatchSampleValueType_t<int32_t>(0x7FFF8000);
        auto temp = (sample + 0x8000) >> 16;
        auto max = BatchSampleValueType_t<int32_t>(SampleTypeLimits<int32_t>::max);
        return xsimd::select(cmp, max, temp);
    }
};

template<>
struct BatchSampleConverter<int32_t, int24_t>
{
    static inline BatchSampleValueType_t<int24_t> convert(const BatchSampleValueType_t<int32_t>& sample) noexcept
    {
        auto cmp = sample >= BatchSampleValueType_t<int32_t>(0x7FFFFF80);
        auto temp = (sample + 0x80) >> 8;
        auto max = BatchSampleValueType_t<int32_t>(SampleTypeLimits<int32_t>::max);
        return xsimd::select(cmp, max, temp);
    }
};

// intXX_t -> float32_t

template<class SampleType>
struct BatchSampleConverter<SampleType, float32_t>
{
    static inline BatchSampleValueType_t<float32_t> convert(const BatchSampleValueType_t<SampleType>& sample) noexcept
    {
        return xsimd::to_float(sample) * FloatConvertTraits<SampleType>::Divisor;
    }
};

// float32_t -> intXX_t

template<class SampleType>
struct BatchSampleConverter<float32_t, SampleType>
{
    static inline BatchSampleValueType_t<SampleType> convert(BatchSampleValueType_t<float32_t> sample) noexcept
    {
        auto cmp_max = sample >= SampleTypeLimits<float32_t>::max;
        auto cmp_min = sample < SampleTypeLimits<float32_t>::min;
        sample *= FloatConvertTraits<SampleType>::Multiplier;
        //        auto out = xsimd::to_int(xsimd::round(sample));
        sample -= 0.5;
        auto out = xsimd_extra::to_int_round(sample);
        out = xsimd::select(
            xsimd::bool_cast(cmp_max), BatchSampleValueType_t<SampleType>(SampleTypeLimits<SampleType>::max), out);
        out = xsimd::select(
            xsimd::bool_cast(cmp_min), BatchSampleValueType_t<SampleType>(SampleTypeLimits<SampleType>::min), out);
        return out;
    }
};

// float32_t -> float32_t

template<>
struct BatchSampleConverter<float32_t, float32_t>
{
    static inline BatchSampleValueType_t<float32_t> convert(const BatchSampleValueType_t<float32_t>& sample) noexcept
    {
        return sample;
    }
};

// BatchSampleToNetworkConverter

template<class InputSampleType, class OutputSampleType>
struct BatchSampleToNetworkConverter;

// int16_t -> intXX_t

template<>
struct BatchSampleToNetworkConverter<int16_t, int16_t>
{
    static inline BatchNetworkSampleValueType_t<int16_t> convert(const BatchSampleValueType_t<int16_t>& sample) noexcept
    {
#    if defined(RATL_CPP_LITTLE_ENDIAN)
        return batchNetworkSampleTypeCast<int16_t>(((sample >> 8) & 0x00ff) | ((sample << 8) & 0xff00));
#    else
        return batchNetworkSampleTypeCast<int16_t>(sample);
#    endif
    }
};

template<>
struct BatchSampleToNetworkConverter<int16_t, int24_t>
{
    static inline BatchNetworkSampleValueType_t<int24_t> convert(const BatchSampleValueType_t<int16_t>& sample) noexcept
    {
#    if defined(RATL_CPP_LITTLE_ENDIAN)
        return batchNetworkSampleTypeCast<int24_t>(((sample >> 8) & 0x00ff) | ((sample << 8) & 0xff00));
#    else
        return batchNetworkSampleTypeCast<int24_t>(SampleConverter<int16_t, int24_t>::convert(sample));
#    endif
    }
};

template<>
struct BatchSampleToNetworkConverter<int16_t, int32_t>
{
    static inline BatchNetworkSampleValueType_t<int32_t> convert(const BatchSampleValueType_t<int16_t>& sample) noexcept
    {
#    if defined(RATL_CPP_LITTLE_ENDIAN)
        return batchNetworkSampleTypeCast<int32_t>(((sample >> 8) & 0x00ff) | ((sample << 8) & 0xff00));
#    else
        return batchNetworkSampleTypeCast<int24_t>(SampleConverter<int16_t, int32_t>::convert(sample));
#    endif
    }
};

// int24_t -> intXX_t

template<>
struct BatchSampleToNetworkConverter<int24_t, int16_t>
{
    static inline BatchNetworkSampleValueType_t<int16_t> convert(const BatchSampleValueType_t<int24_t>& sample) noexcept
    {
#    if defined(RATL_CPP_LITTLE_ENDIAN)
        return batchNetworkSampleTypeCast<int16_t>(((sample >> 16) & 0x00ff) | (sample & 0xff00));
#    else
        return batchNetworkSampleTypeCast<int16_t>(SampleConverter<int24_t, int16_t>::convert(sample));
#    endif
    }
};

template<>
struct BatchSampleToNetworkConverter<int24_t, int24_t>
{
    static inline BatchNetworkSampleValueType_t<int24_t> convert(const BatchSampleValueType_t<int24_t>& sample) noexcept
    {
#    if defined(RATL_CPP_LITTLE_ENDIAN)
        return batchNetworkSampleTypeCast<int24_t>(
            ((sample >> 16) & 0x000000ff) | (sample & 0x0000ff00) | ((sample << 16) & 0x00ff0000));
#    else
        return batchNetworkSampleTypeCast<int24_t>(sample);
#    endif
    }
};

template<>
struct BatchSampleToNetworkConverter<int24_t, int32_t>
{
    static inline BatchNetworkSampleValueType_t<int32_t> convert(const BatchSampleValueType_t<int24_t>& sample) noexcept
    {
#    if defined(RATL_CPP_LITTLE_ENDIAN)
        return batchNetworkSampleTypeCast<int32_t>(
            ((sample >> 16) & 0x000000ff) | (sample & 0x0000ff00) | ((sample << 16) & 0x00ff0000));
#    else
        return batchNetworkSampleTypeCast<int32_t>(SampleConverter<int24_t, int32_t>::convert(sample));
#    endif
    }
};

// int32_t -> intXX_t

template<>
struct BatchSampleToNetworkConverter<int32_t, int16_t>
{
    static inline BatchNetworkSampleValueType_t<int16_t> convert(const BatchSampleValueType_t<int32_t>& sample) noexcept
    {
#    if defined(RATL_CPP_LITTLE_ENDIAN)
        return batchNetworkSampleTypeCast<int16_t>(((sample >> 24) & 0x000000ff) | ((sample >> 8) & 0x0000ff00));
#    else
        return batchNetworkSampleTypeCast<int16_t>(SampleConverter<int32_t, int16_t>::convert(sample));
#    endif
    }
};

template<>
struct BatchSampleToNetworkConverter<int32_t, int24_t>
{
    static inline BatchNetworkSampleValueType_t<int24_t> convert(const BatchSampleValueType_t<int32_t>& sample) noexcept
    {
#    if defined(RATL_CPP_LITTLE_ENDIAN)
        return batchNetworkSampleTypeCast<int24_t>(
            ((sample >> 24) & 0x000000ff) | ((sample >> 8) & 0x0000ff00) | ((sample << 8) & 0x00ff0000));
#    else
        return batchNetworkSampleTypeCast<int24_t>(SampleConverter<int32_t, int24_t>::convert(sample));
#    endif
    }
};

template<>
struct BatchSampleToNetworkConverter<int32_t, int32_t>
{
    static inline BatchNetworkSampleValueType_t<int32_t> convert(const BatchSampleValueType_t<int32_t>& sample) noexcept
    {
#    if defined(RATL_CPP_LITTLE_ENDIAN)
        return batchNetworkSampleTypeCast<int32_t>(
            ((sample >> 24) & 0x000000ff) | ((sample >> 8) & 0x0000ff00) | ((sample << 8) & 0x00ff0000) |
            ((sample << 24) & 0xff000000));
#    else
        return batchNetworkSampleTypeCast<int32_t>(sample);
#    endif
    }
};

// float32_t -> float32_t

template<>
struct BatchSampleToNetworkConverter<float32_t, float32_t>
{
    static inline BatchNetworkSampleValueType_t<float32_t> convert(
        const BatchSampleValueType_t<float32_t>& sample) noexcept
    {
#    if defined(RATL_CPP_LITTLE_ENDIAN)
        return BatchSampleToNetworkConverter<int32_t, int32_t>::convert(batchSampleTypeCast<int32_t>(sample));
#    else
        return sample;
#    endif
    }
};

// intXX_t -> float32_t

template<class SampleType>
struct BatchSampleToNetworkConverter<SampleType, float32_t>
{
    static inline BatchNetworkSampleValueType_t<float32_t> convert(
        const BatchSampleValueType_t<SampleType>& sample) noexcept
    {
        return BatchSampleToNetworkConverter<float32_t, float32_t>::convert(
            BatchSampleConverter<SampleType, float32_t>::convert(sample));
    }
};

// float32_t -> intXX_t

template<class SampleType>
struct BatchSampleToNetworkConverter<float32_t, SampleType>
{
    static inline BatchNetworkSampleValueType_t<SampleType> convert(
        const BatchSampleValueType_t<float32_t>& sample) noexcept
    {
        return BatchSampleToNetworkConverter<SampleType, SampleType>::convert(
            BatchSampleConverter<float32_t, SampleType>::convert(sample));
    }
};

// sample_converter_impl

template<class InputSampleType, class OutputSampleType>
struct BatchNetworkToSampleConverter;

// int16_t -> intXX_t

template<>
struct BatchNetworkToSampleConverter<int16_t, int16_t>
{
    static inline BatchSampleValueType_t<int16_t> convert(const BatchNetworkSampleValueType_t<int16_t>& sample) noexcept
    {
#    if defined(RATL_CPP_LITTLE_ENDIAN)
        BatchSampleValueType_t<int16_t> cast = batchSampleTypeCast<int16_t>(sample);
        return ((cast >> 8) & 0x000000ff) | ((cast << 8) & 0x0000ff00);
#    else
        return batchSampleTypeCast<int16_t>(sample);
#    endif
    }
};

template<>
struct BatchNetworkToSampleConverter<int16_t, int24_t>
{
    static inline BatchSampleValueType_t<int24_t> convert(const BatchNetworkSampleValueType_t<int16_t>& sample) noexcept
    {
#    if defined(RATL_CPP_LITTLE_ENDIAN)
        BatchSampleValueType_t<int24_t> cast = batchSampleTypeCast<int24_t>(sample);
        return (cast & 0x0000ff00) | ((cast << 16) & 0x00ff0000);
#    else
        return SampleConverter<int16_t, int24_t>::convert(batchSampleTypeCast<int24_t>(sample));
#    endif
    }
};

template<>
struct BatchNetworkToSampleConverter<int16_t, int32_t>
{
    static inline BatchSampleValueType_t<int32_t> convert(const BatchNetworkSampleValueType_t<int16_t>& sample) noexcept
    {
#    if defined(RATL_CPP_LITTLE_ENDIAN)
        BatchSampleValueType_t<int32_t> cast = batchSampleTypeCast<int32_t>(sample);
        return ((cast << 8) & 0x00ff0000) | ((cast << 24) & 0xff000000);
#    else
        return SampleConverter<int16_t, int32_t>::convert(batchSampleTypeCast<int32_t>(sample));
#    endif
    }
};

// int24_t -> intXX_t

template<>
struct BatchNetworkToSampleConverter<int24_t, int16_t>
{
    static inline BatchSampleValueType_t<int16_t> convert(const BatchNetworkSampleValueType_t<int24_t>& sample) noexcept
    {
#    if defined(RATL_CPP_LITTLE_ENDIAN)
        BatchSampleValueType_t<int16_t> cast = batchSampleTypeCast<int16_t>(sample);
        return ((cast >> 8) & 0x000000ff) | ((cast << 8) & 0x0000ff00);
#    else
        return SampleConverter<int24_t, int16_t>::convert(batchSampleTypeCast<int16_t>(sample));
#    endif
    }
};

template<>
struct BatchNetworkToSampleConverter<int24_t, int24_t>
{
    static inline BatchSampleValueType_t<int24_t> convert(const BatchNetworkSampleValueType_t<int24_t>& sample) noexcept
    {
#    if defined(RATL_CPP_LITTLE_ENDIAN)
        BatchSampleValueType_t<int24_t> cast = batchSampleTypeCast<int24_t>(sample);
        return ((cast >> 16) & 0x000000ff) | (cast & 0x0000ff00) | ((cast << 16) & 0x00ff0000);
#    else
        return batchSampleTypeCast<int24_t>(sample);
#    endif
    }
};

template<>
struct BatchNetworkToSampleConverter<int24_t, int32_t>
{
    static inline BatchSampleValueType_t<int32_t> convert(const BatchNetworkSampleValueType_t<int24_t>& sample) noexcept
    {
#    if defined(RATL_CPP_LITTLE_ENDIAN)
        BatchSampleValueType_t<int32_t> cast = batchSampleTypeCast<int32_t>(sample);
        return ((cast >> 8) & 0x0000ff00) | ((cast << 8) & 0x00ff0000) | ((cast << 24) & 0xff000000);
#    else
        return SampleConverter<int24_t, int32_t>::convert(batchSampleTypeCast<int32_t>(sample));
#    endif
    }
};

// int32_t -> intXX_t

template<>
struct BatchNetworkToSampleConverter<int32_t, int16_t>
{
    static inline BatchSampleValueType_t<int16_t> convert(const BatchNetworkSampleValueType_t<int32_t>& sample) noexcept
    {
#    if defined(RATL_CPP_LITTLE_ENDIAN)
        BatchSampleValueType_t<int16_t> cast = batchSampleTypeCast<int16_t>(sample);
        return ((cast >> 8) & 0x000000ff) | ((cast << 8) & 0x0000ff00);
#    else
        return SampleConverter<int32_t, int16_t>::convert(batchSampleTypeCast<int16_t>(sample));
#    endif
    }
};

template<>
struct BatchNetworkToSampleConverter<int32_t, int24_t>
{
    static inline BatchSampleValueType_t<int24_t> convert(const BatchNetworkSampleValueType_t<int32_t>& sample) noexcept
    {
#    if defined(RATL_CPP_LITTLE_ENDIAN)
        BatchSampleValueType_t<int24_t> cast = batchSampleTypeCast<int24_t>(sample);
        return ((cast >> 16) & 0x000000ff) | (cast & 0x0000ff00) | ((cast << 16) & 0x00ff0000);
#    else
        return SampleConverter<int32_t, int24_t>::convert(batchSampleTypeCast<int24_t>(sample));
#    endif
    }
};

template<>
struct BatchNetworkToSampleConverter<int32_t, int32_t>
{
    static inline BatchSampleValueType_t<int32_t> convert(const BatchNetworkSampleValueType_t<int32_t>& sample) noexcept
    {
#    if defined(RATL_CPP_LITTLE_ENDIAN)
        BatchSampleValueType_t<int32_t> cast = batchSampleTypeCast<int32_t>(sample);
        return ((cast >> 24) & 0x000000ff) | ((cast >> 8) & 0x0000ff00) | ((cast << 8) & 0x00ff0000) |
               ((cast << 24) & 0xff000000);
#    else
        return batchSampleTypeCast<int32_t>(sample);
#    endif
    }
};

// float32_t -> float32_t

template<>
struct BatchNetworkToSampleConverter<float32_t, float32_t>
{
    static inline BatchSampleValueType_t<float32_t> convert(
        const BatchNetworkSampleValueType_t<float32_t>& sample) noexcept
    {
#    if defined(RATL_CPP_LITTLE_ENDIAN)
        return batchSampleTypeCast<float32_t>(
            BatchNetworkToSampleConverter<int32_t, int32_t>::convert(batchNetworkSampleTypeCast<int32_t>(sample)));
#    else
        return batchSampleTypeCast<float32_t>(sample);
#    endif
    }
};

// intXX_t -> float32_t

template<class SampleType>
struct BatchNetworkToSampleConverter<SampleType, float32_t>
{
    static inline BatchSampleValueType_t<float32_t> convert(
        const BatchNetworkSampleValueType_t<SampleType>& sample) noexcept
    {
        BatchSampleValueType_t<SampleType> temp =
            BatchNetworkToSampleConverter<SampleType, SampleType>::convert(sample);
        temp = batchFixNegativeSamples<SampleType>(temp);
        return BatchSampleConverter<SampleType, float32_t>::convert(temp);
    }
};

// float32_t -> intXX_t

template<class SampleType>
struct BatchNetworkToSampleConverter<float32_t, SampleType>
{
    static inline BatchSampleValueType_t<SampleType> convert(
        const BatchNetworkSampleValueType_t<float32_t>& sample) noexcept
    {
        return BatchSampleConverter<float32_t, SampleType>::convert(
            BatchNetworkToSampleConverter<float32_t, float32_t>::convert(sample));
    }
};

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_detail_batch_sample_converter_
