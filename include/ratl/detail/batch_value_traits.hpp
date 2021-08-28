/**
 * Copyright (c) 2018-2021 Hamish Cook
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

template<typename SampleValueType>
struct has_batch_type : is_complete<xsimd::batch<SampleValueType, batch_size>>
{
};

// has_batch_type_v

template<typename SampleValueType>
static constexpr bool has_batch_type_v = has_batch_type<SampleValueType>::value;

// batch_sample_value_type

template<typename SampleValueType, typename = void>
struct batch_sample_value_type;

template<typename SampleValueType>
struct batch_sample_value_type<SampleValueType, typename std::enable_if<has_batch_type_v<SampleValueType>>::type>
{
    using type = xsimd::batch<SampleValueType, batch_size>;
};

template<typename SampleValueType>
struct batch_sample_value_type<SampleValueType, typename std::enable_if<!has_batch_type_v<SampleValueType>>::type>
{
    using type = xsimd::batch<std::int32_t, batch_size>;
};

template<>
struct batch_sample_value_type<float32_t>
{
    using type = xsimd::batch<float, batch_size>;
};

template<typename SampleValueType>
using batch_sample_value_type_t = typename batch_sample_value_type<SampleValueType>::type;

// batch_network_sample_value_type

template<typename SampleValueType, typename = void>
struct batch_network_sample_value_type;

template<typename SampleValueType>
struct batch_network_sample_value_type<
    SampleValueType,
    typename std::enable_if<has_batch_type_v<network_sample_value_underlying_type_t<SampleValueType>>>::type>
{
    using type = xsimd::batch<network_sample_value_underlying_type_t<SampleValueType>, batch_size>;
};

template<typename SampleValueType>
struct batch_network_sample_value_type<
    SampleValueType,
    typename std::enable_if<!has_batch_type_v<network_sample_value_underlying_type_t<SampleValueType>>>::type>
{
    using type = xsimd::batch<std::uint32_t, batch_size>;
};

template<typename SampleValueType>
using batch_network_sample_value_type_t = typename batch_network_sample_value_type<SampleValueType>::type;

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_detail_batch_value_traits_
