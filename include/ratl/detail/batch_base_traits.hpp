#ifndef _ratl_batch_base_traits_
#define _ratl_batch_base_traits_

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

template<class SampleType>
struct BatchSampleValueTypeImpl;

template<>
struct BatchSampleValueTypeImpl<int16_t>
{
    using type = xsimd::batch<int16_t, BatchSize>;
};

template<>
struct BatchSampleValueTypeImpl<int24_t>
{
    using type = xsimd::batch<int32_t, BatchSize>;
};

template<>
struct BatchSampleValueTypeImpl<int32_t>
{
    using type = xsimd::batch<int32_t, BatchSize>;
};

template<>
struct BatchSampleValueTypeImpl<float32_t>
{
    using type = xsimd::batch<float32_t, BatchSize>;
};

// BatchSampleValueType

template<class SampleType>
struct BatchSampleValueType
{
    using type = typename BatchSampleValueTypeImpl<SampleType>::type;
};

template<class SampleType>
using BatchSampleValueType_t = typename BatchSampleValueType<SampleType>::type;

// BatchNetworkSampleValueType

template<class SampleType>
struct BatchNetworkSampleValueType;

template<>
struct BatchNetworkSampleValueType<int16_t>
{
    using type = xsimd::batch<uint16_t, BatchSize>;
};

template<>
struct BatchNetworkSampleValueType<int24_t>
{
    using type = xsimd::batch<uint32_t, BatchSize>;
};

template<>
struct BatchNetworkSampleValueType<int32_t>
{
    using type = xsimd::batch<uint32_t, BatchSize>;
};

template<>
struct BatchNetworkSampleValueType<float32_t>
{
    using type = xsimd::batch<uint32_t, BatchSize>;
};

template<class SampleType>
using BatchNetworkSampleValueType_t = typename BatchNetworkSampleValueType<SampleType>::type;

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_batch_base_traits_
