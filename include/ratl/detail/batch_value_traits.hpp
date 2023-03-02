/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_batch_value_traits_
#define _ratl_detail_batch_value_traits_

// other includes
#include <type_traits>

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/sample_value_traits.hpp>
#include <ratl/detail/type_traits.hpp>
#include <ratl/detail/xsimd.hpp>

#if defined(RATL_HAS_XSIMD)

namespace ratl
{
namespace detail
{
static constexpr std::size_t batch_size = std::min(XSIMD_BATCH_INT32_SIZE, XSIMD_BATCH_FLOAT_SIZE);

// has_batch_type

template<typename SampleValueType, std::size_t BatchSize = batch_size>
struct has_batch_type : is_complete<xsimd::batch<SampleValueType, BatchSize>>
{
};

// has_batch_type_v

#    if defined(RATL_CPP_VERSION_HAS_CPP17)
template<typename SampleValueType, std::size_t BatchSize = batch_size>
inline constexpr bool has_batch_type_v = has_batch_type<SampleValueType, BatchSize>::value;
#    endif

// batch_sample_value_type

template<typename SampleValueType, std::size_t BatchSize, typename = void>
struct batch_sample_value_type;

template<typename SampleValueType, std::size_t BatchSize>
struct batch_sample_value_type<
    SampleValueType,
    BatchSize,
    std::enable_if_t<has_batch_type<SampleValueType, BatchSize>::value>>
{
    using type = xsimd::batch<SampleValueType, BatchSize>;
};

template<typename SampleValueType, std::size_t BatchSize>
struct batch_sample_value_type<
    SampleValueType,
    BatchSize,
    std::enable_if_t<
        !has_batch_type<SampleValueType, BatchSize>::value && has_batch_type<std::int32_t, BatchSize>::value>>
{
    using type = xsimd::batch<std::int32_t, BatchSize>;
};

template<std::size_t BatchSize>
struct batch_sample_value_type<float32_t, BatchSize, std::enable_if_t<has_batch_type<float, BatchSize>::value>>
{
    using type = xsimd::batch<float, BatchSize>;
};

template<typename SampleValueType, std::size_t BatchSize = batch_size>
using batch_sample_value_type_t = typename batch_sample_value_type<SampleValueType, BatchSize>::type;

// batch_network_sample_value_type

template<typename SampleValueType, std::size_t BatchSize, typename = void>
struct batch_network_sample_value_type;

template<typename SampleValueType, std::size_t BatchSize>
struct batch_network_sample_value_type<
    SampleValueType,
    BatchSize,
    std::enable_if_t<has_batch_type<network_sample_value_underlying_type_t<SampleValueType>, BatchSize>::value>>
{
    using type = xsimd::batch<network_sample_value_underlying_type_t<SampleValueType>, BatchSize>;
};

template<typename SampleValueType, std::size_t BatchSize>
struct batch_network_sample_value_type<
    SampleValueType,
    BatchSize,
    std::enable_if_t<
        !has_batch_type<network_sample_value_underlying_type_t<SampleValueType>, BatchSize>::value &&
        has_batch_type<std::uint32_t, BatchSize>::value>>
{
    using type = xsimd::batch<std::uint32_t, BatchSize>;
};

template<typename SampleValueType, std::size_t BatchSize = batch_size>
using batch_network_sample_value_type_t = typename batch_network_sample_value_type<SampleValueType, BatchSize>::type;

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_detail_batch_value_traits_
