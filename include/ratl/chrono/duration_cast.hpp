/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_chrono_duration_cast_
#define _ratl_chrono_duration_cast_

// ratl includes
#include <ratl/chrono/sample_duration.hpp>
#include <ratl/chrono/subsample_duration.hpp>
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
template<typename ToDuration>
struct duration_cast_impl;

template<>
struct duration_cast_impl<sample_duration>
{
    static inline sample_duration cast(const sample_duration& sample_duration)
    {
        return sample_duration;
    }

    static inline sample_duration cast(const subsample_duration& subsample_duration)
    {
        return detail::convert_to_sample_duration(subsample_duration);
    }
};

template<>
struct duration_cast_impl<subsample_duration>
{
    static inline subsample_duration cast(const sample_duration& sample_duration)
    {
        return detail::convert_to_subsample_duration(sample_duration);
    }

    static inline subsample_duration cast(const subsample_duration& subsample_duration)
    {
        return subsample_duration;
    }
};

template<typename Rep, typename Period>
struct duration_cast_impl<std::chrono::duration<Rep, Period>>
{
    static inline std::chrono::duration<Rep, Period> cast(const sample_duration& sample_duration)
    {
        return detail::convert_to_duration<Rep, Period>(sample_duration);
    }

    static inline std::chrono::duration<Rep, Period> cast(const subsample_duration& subsample_duration)
    {
        return detail::convert_to_duration<Rep, Period>(subsample_duration);
    }
};
} // namespace detail

template<typename ToDuration>
inline ToDuration duration_cast(const sample_duration& duration)
{
    return detail::duration_cast_impl<ToDuration>::cast(duration);
}

template<typename ToDuration>
inline ToDuration duration_cast(const subsample_duration& duration)
{
    return detail::duration_cast_impl<ToDuration>::cast(duration);
}

} // namespace chrono
} // namespace ratl

#endif // _ratl_chrono_duration_cast_
