/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_batch_traits_
#define _ratl_detail_batch_traits_

// other includes
#include <type_traits>

// ratl includes
#include <ratl/detail/batch_value_traits.hpp>
#include <ratl/detail/sample_traits.hpp>
#include <ratl/network_sample.hpp>
#include <ratl/sample.hpp>

#if defined(RATL_HAS_XSIMD)

namespace ratl
{
namespace detail
{
// batch_sample_type

template<typename SampleType, std::size_t BatchSize = batch_size>
struct batch_sample_type;

template<typename SampleValueType, std::size_t BatchSize>
struct batch_sample_type<sample<SampleValueType>, BatchSize>
{
    using type = batch_sample_value_type_t<SampleValueType, BatchSize>;
};

template<typename SampleValueType, std::size_t BatchSize>
struct batch_sample_type<network_sample<SampleValueType>, BatchSize>
{
    using type = batch_network_sample_value_type_t<SampleValueType, BatchSize>;
};

template<typename SampleType, std::size_t BatchSize = batch_size>
using batch_sample_type_t = typename batch_sample_type<SampleType, BatchSize>::type;

// is_native_batch_type

template<typename SampleType, typename BatchType>
struct is_native_batch_type : std::is_same<sample_underlying_type_t<SampleType>, typename BatchType::value_type>
{
};

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_detail_batch_traits_
