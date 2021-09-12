/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_convert_traits_
#define _ratl_detail_convert_traits_

// ratl includes
#include <ratl/sample_limits.hpp>

namespace ratl
{
namespace detail
{
// symmetric_float_convert_traits

template<typename IntSampleValueType>
struct symmetric_float_convert_traits
{
    static_assert(sample_limits<IntSampleValueType>::is_integer, "IntSampleValueType must be an integer type");
    static constexpr float32_t float_to_int_scaler = -static_cast<float32_t>(sample_limits<IntSampleValueType>::min());
    static constexpr float32_t int_to_float_scaler = 1.f / float_to_int_scaler;
};

#if !defined(RATL_CPP_VERSION_HAS_CPP17)
template<typename SampleValueType>
constexpr float32_t symmetric_float_convert_traits<SampleValueType>::float_to_int_scaler;
template<typename SampleValueType>
constexpr float32_t symmetric_float_convert_traits<SampleValueType>::int_to_float_scaler;
#endif

// asymmetric_float_convert_traits

template<typename IntSampleValueType>
struct asymmetric_float_convert_traits
{
    static_assert(sample_limits<IntSampleValueType>::is_integer, "IntSampleValueType must be an integer type");
    static constexpr float32_t float_to_int_scaler = static_cast<float32_t>(sample_limits<IntSampleValueType>::max());
    static constexpr float32_t int_to_float_scaler =
        1.f / -static_cast<float32_t>(sample_limits<IntSampleValueType>::min());
};

#if !defined(RATL_CPP_VERSION_HAS_CPP17)
template<typename SampleValueType>
constexpr float32_t asymmetric_float_convert_traits<SampleValueType>::float_to_int_scaler;
template<typename SampleValueType>
constexpr float32_t asymmetric_float_convert_traits<SampleValueType>::int_to_float_scaler;
#endif

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_convert_traits_
