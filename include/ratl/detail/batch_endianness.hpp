#ifndef _ratl_detail_batch_endianness_
#define _ratl_detail_batch_endianness_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/detail/batch_sample_fix.hpp>
#include <ratl/detail/batch_traits.hpp>
#include <ratl/detail/config.hpp>

#if defined(RATL_HAS_XSIMD)

namespace ratl
{
namespace detail
{
// BatchReverseEndiannessImpl

template<class NetworkSampleValueUnderlyingType>
struct BatchReverseEndiannessImpl;

template<>
struct BatchReverseEndiannessImpl<uint16_t>
{
    template<class BatchNetworkSampleValueType>
    static inline BatchNetworkSampleValueType reverse(const BatchNetworkSampleValueType& input) noexcept
    {
        return ((input & 0xff00) >> 8) | ((input & 0x00ff) << 8);
    }
};

template<>
struct BatchReverseEndiannessImpl<uint24_t>
{
    template<class BatchNetworkSampleValueType>
    static inline BatchNetworkSampleValueType reverse(const BatchNetworkSampleValueType& input) noexcept
    {
        return ((input & 0xff0000) >> 16) | (input & 0x00ff00) | ((input & 0x0000ff) << 16);
    }
};

template<>
struct BatchReverseEndiannessImpl<uint32_t>
{
    template<class BatchNetworkSampleValueType>
    static inline BatchNetworkSampleValueType reverse(const BatchNetworkSampleValueType& input) noexcept
    {
        return ((input & 0xff000000) >> 24) | ((input & 0x00ff0000) >> 8) | ((input & 0x0000ff00) << 8) |
               ((input & 0x000000ff) << 24);
    }
};

// batchReverseEndianness

template<class NetworkSampleValueUnderlyingType, class BatchNetworkSampleValueType>
inline BatchNetworkSampleValueType batchReverseEndianness(const BatchNetworkSampleValueType& input) noexcept
{
    return BatchReverseEndiannessImpl<NetworkSampleValueUnderlyingType>::reverse(input);
}

// batchHostToNetwork

template<class NetworkSampleValueUnderlyingType, class BatchNetworkSampleValueType>
inline BatchNetworkSampleValueType batchHostToNetwork(const BatchNetworkSampleValueType& input) noexcept
{
#    if defined(RATL_CPP_LITTLE_ENDIAN)
    return batchReverseEndianness<NetworkSampleValueUnderlyingType>(input);
#    else
    return input;
#    endif
}

// batchNetworkToHost

template<class NetworkSampleValueUnderlyingType, class BatchNetworkSampleValueType>
inline BatchNetworkSampleValueType batchNetworkToHost(const BatchNetworkSampleValueType& input) noexcept
{
#    if defined(RATL_CPP_LITTLE_ENDIAN)
    return batchReverseEndianness<NetworkSampleValueUnderlyingType>(input);
#    else
    return input;
#    endif
}

// BatchSampleToNetworkSampleImpl

template<class SampleType>
struct BatchReverseEndiannessImpl
{
    static inline BatchNetworkSampleValueType_t<SampleType> convert(
        const BatchSampleValueType_t<SampleType>& input) noexcept
    {
        return batchHostToNetwork<NetworkSampleValueUnderlyingType_t<SampleType>>(
            BatchNetworkSampleValueType_t<SampleType>(input));
    }
};

template<>
struct BatchReverseEndiannessImpl<float32_t>
{
    static inline BatchNetworkSampleValueType_t<float32_t> convert(
        const BatchSampleValueType_t<float32_t>& input) noexcept
    {
        return batchHostToNetwork<NetworkSampleValueUnderlyingType_t<float32_t>>(
            BatchNetworkSampleValueType_t<float32_t>(xsimd::bitwise_cast<BatchSampleValueType_t<int32_t>>(input)));
    }
};

// batchSampleToNetworkSample

template<class SampleType>
inline BatchNetworkSampleValueType_t<SampleType> batchSampleToNetworkSample(
    const BatchSampleValueType_t<SampleType>& input) noexcept
{
    return BatchReverseEndiannessImpl<SampleType>::convert(input);
}

// BatchNetworkSampleToSampleImpl

template<class SampleType>
struct BatchNetworkSampleToSampleImpl
{
    static inline BatchSampleValueType_t<SampleType> convert(
        const BatchNetworkSampleValueType_t<SampleType>& input) noexcept
    {
        return batchFixNegativeSamples<SampleType>(BatchSampleValueType_t<SampleType>(
            batchNetworkToHost<NetworkSampleValueUnderlyingType_t<SampleType>>(input)));
    }
};

template<>
struct BatchNetworkSampleToSampleImpl<float32_t>
{
    static inline BatchSampleValueType_t<float32_t> convert(
        const BatchNetworkSampleValueType_t<float32_t>& input) noexcept
    {
        return xsimd::bitwise_cast<BatchSampleValueType_t<float32_t>>(
            BatchSampleValueType_t<int32_t>(batchNetworkToHost<NetworkSampleValueUnderlyingType_t<float32_t>>(input)));
    }
};

// batchNetworkSampleToSample

template<class SampleType>
inline BatchSampleValueType_t<SampleType> batchNetworkSampleToSample(
    const BatchNetworkSampleValueType_t<SampleType>& input) noexcept
{
    return BatchNetworkSampleToSampleImpl<SampleType>::convert(input);
}

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_detail_batch_endianness_
