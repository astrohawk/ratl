/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_chrono_time_point_cast_
#define _ratl_chrono_time_point_cast_

// ratl includes
#include <ratl/chrono/sample_time_point.hpp>
#include <ratl/chrono/subsample_time_point.hpp>
#include <ratl/chrono/duration_cast.hpp>
#include <ratl/detail/config.hpp>

// other includes
#include <chrono>
#include <cmath>
#include <stdexcept>

namespace ratl
{
namespace chrono
{
namespace detail
{
template<typename ToDuration, typename Clock>
struct time_point_cast_impl;

template<typename Clock>
struct time_point_cast_impl<sample_duration, Clock>
{
    using type = sample_time_point<Clock>;
};

template<typename Clock>
struct time_point_cast_impl<subsample_duration, Clock>
{
    using type = subsample_time_point<Clock>;
};

template<typename Rep, typename Period, typename Clock>
struct time_point_cast_impl<std::chrono::duration<Rep, Period>, Clock>
{
    using type = std::chrono::time_point<std::chrono::duration<Rep, Period>, Clock>;
};

template<typename ToDuration, typename Clock>
using time_point_cast_impl_t = typename detail::time_point_cast_impl<ToDuration, Clock>::type;
} // namespace detail

template<typename ToDuration, typename Clock>
inline ToDuration time_point_cast(const sample_time_point<Clock>& time_point)
{
    using ToTimePoint = detail::time_point_cast_impl_t<ToDuration, Clock>;
    return ToTimePoint(duration_cast<ToDuration>(time_point.time_since_epoch()));
}

template<typename ToDuration, typename Clock>
inline ToDuration time_point_cast(const subsample_time_point<Clock>& time_point)
{
    using ToTimePoint = detail::time_point_cast_impl_t<ToDuration, Clock>;
    return ToTimePoint(duration_cast<ToDuration>(time_point.time_since_epoch()));
}

} // namespace chrono
} // namespace ratl

#endif // _ratl_chrono_time_point_cast_
