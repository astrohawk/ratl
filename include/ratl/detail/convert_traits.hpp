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
// float_convert_traits

template<typename SampleValueType>
struct float_convert_traits
{
    static constexpr float32_t multiplier = -static_cast<float32_t>(sample_limits<SampleValueType>::min);
    static constexpr float32_t divisor = 1.f / multiplier;
};

template<typename SampleValueType>
constexpr float32_t float_convert_traits<SampleValueType>::multiplier;
template<typename SampleValueType>
constexpr float32_t float_convert_traits<SampleValueType>::divisor;

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_convert_traits_
