/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_sample_limits_
#define _ratl_sample_limits_

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/sample_value_traits.hpp>

// other includes
#include <limits>
#include <type_traits>

namespace ratl
{
template<typename SampleValueType>
struct sample_limits;

template<>
struct sample_limits<int16_t>
{
    using value_type = int16_t;

    static constexpr int digits = std::numeric_limits<int16_t>::digits;
    static constexpr bool is_integer = std::numeric_limits<int16_t>::is_integer;

    static constexpr int16_t min = std::numeric_limits<int16_t>::min();
    static constexpr int16_t max = std::numeric_limits<int16_t>::max();
};

#if !defined(RATL_CPP_VERSION_HAS_CPP17)
constexpr int sample_limits<int16_t>::digits;
constexpr bool sample_limits<int16_t>::is_integer;
constexpr typename sample_limits<int16_t>::value_type sample_limits<int16_t>::min;
constexpr typename sample_limits<int16_t>::value_type sample_limits<int16_t>::max;
#endif

template<>
struct sample_limits<int24_t>
{
    using value_type = int24_t;

    static constexpr int digits = RATL_INT24_DIGITS;
    static constexpr bool is_integer = true;

    static constexpr value_type min = RATL_INT24_MIN;
    static constexpr value_type max = RATL_INT24_MAX;
};

#if !defined(RATL_CPP_VERSION_HAS_CPP17)
constexpr int sample_limits<int24_t>::digits;
constexpr bool sample_limits<int24_t>::is_integer;
constexpr typename sample_limits<int24_t>::value_type sample_limits<int24_t>::min;
constexpr typename sample_limits<int24_t>::value_type sample_limits<int24_t>::max;
#endif

template<>
struct sample_limits<int32_t>
{
    using value_type = int32_t;

    static constexpr int digits = std::numeric_limits<int32_t>::digits;
    static constexpr bool is_integer = std::numeric_limits<int32_t>::is_integer;

    static constexpr int32_t min = std::numeric_limits<int32_t>::min();
    static constexpr int32_t max = std::numeric_limits<int32_t>::max();
};

#if !defined(RATL_CPP_VERSION_HAS_CPP17)
constexpr int sample_limits<int32_t>::digits;
constexpr bool sample_limits<int32_t>::is_integer;
constexpr typename sample_limits<int32_t>::value_type sample_limits<int32_t>::min;
constexpr typename sample_limits<int32_t>::value_type sample_limits<int32_t>::max;
#endif

template<>
struct sample_limits<float32_t>
{
    using value_type = float32_t;

    static constexpr int digits = std::numeric_limits<float32_t>::digits;
    static constexpr bool is_integer = std::numeric_limits<float32_t>::is_integer;

    static constexpr value_type min = -1.f;
    static constexpr value_type max = 1.f;
};

#if !defined(RATL_CPP_VERSION_HAS_CPP17)
constexpr int sample_limits<float32_t>::digits;
constexpr bool sample_limits<float32_t>::is_integer;
constexpr typename sample_limits<float32_t>::value_type sample_limits<float32_t>::min;
constexpr typename sample_limits<float32_t>::value_type sample_limits<float32_t>::max;
#endif

} // namespace ratl

#endif // _ratl_sample_limits_
