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
class sample_time_point
{
public:
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

inline sample_time_point operator+(const sample_time_point& a, const sample_duration& b)
{
    return sample_time_point(a.time_since_epoch() + b);
}

inline sample_time_point operator+(const sample_duration& a, const sample_time_point& b)
{
    return sample_time_point(b.time_since_epoch() + a);
}

inline sample_time_point operator-(const sample_time_point& a, const sample_duration& b)
{
    return sample_time_point(a.time_since_epoch() - b);
}

inline sample_duration operator-(const sample_time_point& a, const sample_time_point& b)
{
    return a.time_since_epoch() - b.time_since_epoch();
}

inline bool operator==(const sample_time_point& a, const sample_time_point& b)
{
    return a.time_since_epoch() == b.time_since_epoch();
}

inline bool operator!=(const sample_time_point& a, const sample_time_point& b)
{
    return a.time_since_epoch() != b.time_since_epoch();
}

inline bool operator<(const sample_time_point& a, const sample_time_point& b)
{
    return a.time_since_epoch() < b.time_since_epoch();
}

inline bool operator>(const sample_time_point& a, const sample_time_point& b)
{
    return a.time_since_epoch() > b.time_since_epoch();
}

inline bool operator<=(const sample_time_point& a, const sample_time_point& b)
{
    return a.time_since_epoch() <= b.time_since_epoch();
}

inline bool operator>=(const sample_time_point& a, const sample_time_point& b)
{
    return a.time_since_epoch() >= b.time_since_epoch();
}

} // namespace chrono
} // namespace ratl

#endif // _ratl_chrono_sample_time_point_
