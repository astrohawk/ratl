/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_chrono_sample_time_point_
#define _ratl_chrono_sample_time_point_

// ratl includes
#include <ratl/chrono/sample_duration.hpp>
#include <ratl/detail/config.hpp>

// other includes
#include <chrono>
#include <cmath>
#include <stdexcept>

namespace ratl
{
namespace chrono
{
template<typename Clock>
class sample_time_point
{
public:
    using clock = Clock;
    using duration = sample_duration;

    inline sample_time_point() = default;

    inline explicit sample_time_point(const duration& duration) : duration_{duration} {}

    inline duration time_since_epoch() const noexcept
    {
        return duration_;
    }

    inline sample_time_point& operator+=(const duration& b)
    {
        duration_ += b;
        return *this;
    }

    inline sample_time_point& operator-=(const duration& b)
    {
        duration_ += b;
        return *this;
    }

private:
    duration duration_;
};

namespace detail
{
template<typename Clock, typename Duration>
inline sample_time_point<Clock> convert_to_sample_time_point(
    const std::chrono::time_point<Clock, Duration>& duration, std::size_t sample_rate)
{
    return sample_time_point<Clock>{convert_to_sample_duration(duration.time_since_epoch(), sample_rate)};
}

template<typename Duration, typename Clock>
inline std::chrono::time_point<Clock, Duration> convert_to_time_point(const sample_time_point<Clock>& sample_time_point)
{
    return std::chrono::time_point<Clock, Duration>{
        convert_to_duration<typename Duration::rep, typename Duration::period>(sample_time_point.time_since_epoch())};
}
} // namespace detail

template<typename Clock>
inline sample_time_point<Clock> operator+(const sample_time_point<Clock>& a, const sample_duration& b)
{
    return sample_time_point<Clock>(a.time_since_epoch() + b);
}

template<typename Clock>
inline sample_time_point<Clock> operator+(const sample_duration& a, const sample_time_point<Clock>& b)
{
    return sample_time_point<Clock>(a + b.time_since_epoch());
}

template<typename Clock>
inline sample_time_point<Clock> operator-(const sample_time_point<Clock>& a, const sample_duration& b)
{
    return sample_time_point<Clock>(a.time_since_epoch() - b);
}

template<typename Clock>
inline sample_duration operator-(const sample_time_point<Clock>& a, const sample_time_point<Clock>& b)
{
    return a.time_since_epoch() - b.time_since_epoch();
}

template<typename Clock>
inline bool operator==(const sample_time_point<Clock>& a, const sample_time_point<Clock>& b)
{
    return a.time_since_epoch() == b.time_since_epoch();
}

template<typename Clock>
inline bool operator!=(const sample_time_point<Clock>& a, const sample_time_point<Clock>& b)
{
    return a.time_since_epoch() != b.time_since_epoch();
}

template<typename Clock>
inline bool operator<(const sample_time_point<Clock>& a, const sample_time_point<Clock>& b)
{
    return a.time_since_epoch() < b.time_since_epoch();
}

template<typename Clock>
inline bool operator>(const sample_time_point<Clock>& a, const sample_time_point<Clock>& b)
{
    return a.time_since_epoch() > b.time_since_epoch();
}

template<typename Clock>
inline bool operator<=(const sample_time_point<Clock>& a, const sample_time_point<Clock>& b)
{
    return a.time_since_epoch() <= b.time_since_epoch();
}

template<typename Clock>
inline bool operator>=(const sample_time_point<Clock>& a, const sample_time_point<Clock>& b)
{
    return a.time_since_epoch() >= b.time_since_epoch();
}

} // namespace chrono
} // namespace ratl

#endif // _ratl_chrono_sample_time_point_
