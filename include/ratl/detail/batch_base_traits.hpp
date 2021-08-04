#ifndef _ratl_detail_batch_base_traits_
#define _ratl_detail_batch_base_traits_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/type_traits.hpp>
#include <ratl/detail/xsimd.hpp>

#if defined(RATL_HAS_XSIMD)

namespace ratl
{
namespace detail
{
static constexpr std::size_t BatchSize = std::min(XSIMD_BATCH_INT32_SIZE, XSIMD_BATCH_FLOAT_SIZE);

// HasBatchType

template<typename SampleType>
struct HasBatchType : IsComplete<xsimd::batch<SampleType, BatchSize>>
{
};

// HasBatchType_v

template<typename SampleType>
static constexpr bool HasBatchType_v = HasBatchType<SampleType>::value;

// BatchSampleValueType

template<class SampleType, class = void>
struct BatchSampleValueType;

template<class SampleType>
struct BatchSampleValueType<SampleType, typename std::enable_if<HasBatchType_v<SampleType>>::type>
{
    using type = xsimd::batch<SampleType, BatchSize>;
};

template<class SampleType>
struct BatchSampleValueType<SampleType, typename std::enable_if<!HasBatchType_v<SampleType>>::type>
{
    using type = xsimd::batch<int32_t, BatchSize>;
};

template<>
struct BatchSampleValueType<float32_t>
{
    using type = xsimd::batch<float32_t, BatchSize>;
};

template<class SampleType>
using BatchSampleValueType_t = typename BatchSampleValueType<SampleType>::type;

// BatchNetworkSampleValueType

template<class SampleType, class = void>
struct BatchNetworkSampleValueType;

template<class SampleType>
struct BatchNetworkSampleValueType<
    SampleType,
    typename std::enable_if<HasBatchType_v<NetworkSampleValueUnderlyingType_t<SampleType>>>::type>
{
    using type = xsimd::batch<NetworkSampleValueUnderlyingType_t<SampleType>, BatchSize>;
};

template<class SampleType>
struct BatchNetworkSampleValueType<
    SampleType,
    typename std::enable_if<!HasBatchType_v<NetworkSampleValueUnderlyingType_t<SampleType>>>::type>
{
    using type = xsimd::batch<uint32_t, BatchSize>;
};

template<class SampleType>
using BatchNetworkSampleValueType_t = typename BatchNetworkSampleValueType<SampleType>::type;

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_detail_batch_base_traits_
