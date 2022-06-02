/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_chrono_subsample_time_point_
#define _ratl_chrono_subsample_time_point_

// ratl includes
#include <ratl/chrono/sample_time_point.hpp>
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
class subsample_time_point
{
public:
    using duration = subsample_duration;

    inline subsample_time_point() = default;

    inline explicit subsample_time_point(const duration& duration) : duration_{duration} {}

    template<typename Clock, typename Duration>
    inline subsample_time_point(const std::chrono::time_point<Clock, Duration>& time_point, std::size_t sample_rate) :
        duration_(time_point.time_since_epoch(), sample_rate)
    {
    }

    inline subsample_time_point(const sample_time_point& sample_time_point) :
        duration_(sample_time_point.time_since_epoch())
    {
    }

    inline duration time_since_epoch() const noexcept
    {
        return duration_;
    }

    inline subsample_time_point& operator+=(const duration& other)
    {
        duration_ += other;
        return *this;
    }

    inline subsample_time_point& operator+=(const sample_duration& other)
    {
        duration_ += other;
        return *this;
    }

    template<typename Rep, typename Period>
    inline subsample_time_point& operator+=(const std::chrono::duration<Rep, Period>& other)
    {
        duration_ += other;
        return *this;
    }

    inline subsample_time_point& operator-=(const duration& other)
    {
        duration_ -= other;
        return *this;
    }

    inline subsample_time_point& operator-=(const sample_duration& other)
    {
        duration_ -= other;
        return *this;
    }

    template<typename Rep, typename Period>
    inline subsample_time_point& operator-=(const std::chrono::duration<Rep, Period>& other)
    {
        duration_ -= other;
        return *this;
    }

private:
    duration duration_;
};

inline subsample_time_point operator+(const subsample_time_point& a, const subsample_duration& b)
{
    return subsample_time_point(a.time_since_epoch() + b);
}

inline subsample_time_point operator+(const subsample_duration& a, const subsample_time_point& b)
{
    return subsample_time_point(b.time_since_epoch() + a);
}

inline subsample_time_point operator+(const sample_time_point& a, const subsample_duration& b)
{
    return subsample_time_point(a.time_since_epoch() + b);
}

inline subsample_time_point operator+(const subsample_duration& a, const sample_time_point& b)
{
    return subsample_time_point(b.time_since_epoch() + a);
}

inline subsample_duration operator-(const subsample_time_point& a, const subsample_time_point& b)
{
    return a.time_since_epoch() - b.time_since_epoch();
}

inline subsample_duration operator-(const subsample_time_point& a, const sample_time_point& b)
{
    return a.time_since_epoch() - b.time_since_epoch();
}

inline subsample_duration operator-(const sample_time_point& a, const subsample_time_point& b)
{
    return a.time_since_epoch() - b.time_since_epoch();
}

inline subsample_time_point operator-(const subsample_time_point& a, const subsample_duration& b)
{
    return subsample_time_point(a.time_since_epoch() - b);
}

inline subsample_time_point operator-(const subsample_time_point& a, const sample_duration& b)
{
    return subsample_time_point(a.time_since_epoch() - b);
}

inline subsample_time_point operator-(const sample_time_point& a, const subsample_duration& b)
{
    return subsample_time_point(a.time_since_epoch() - b);
}

inline bool operator==(const subsample_time_point& a, const subsample_time_point& b)
{
    return a.time_since_epoch() == b.time_since_epoch();
}

inline bool operator!=(const subsample_time_point& a, const subsample_time_point& b)
{
    return a.time_since_epoch() != b.time_since_epoch();
}

inline bool operator<(const subsample_time_point& a, const subsample_time_point& b)
{
    return a.time_since_epoch() < b.time_since_epoch();
}

inline bool operator>(const subsample_time_point& a, const subsample_time_point& b)
{
    return a.time_since_epoch() > b.time_since_epoch();
}

inline bool operator<=(const subsample_time_point& a, const subsample_time_point& b)
{
    return a.time_since_epoch() <= b.time_since_epoch();
}

inline bool operator>=(const subsample_time_point& a, const subsample_time_point& b)
{
    return a.time_since_epoch() >= b.time_since_epoch();
}

inline sample_time_point sample_time_point_cast(const subsample_time_point& time_point)
{
    return sample_time_point(sample_duration_cast(time_point.time_since_epoch()));
}

} // namespace chrono
} // namespace ratl

#endif // _ratl_chrono_subsample_time_point_
