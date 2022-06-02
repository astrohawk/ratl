/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_chrono_sample_duration_
#define _ratl_chrono_sample_duration_

// ratl includes
#include <ratl/detail/config.hpp>

// other includes
#include <chrono>
#include <cmath>
#include <stdexcept>

namespace ratl
{
namespace chrono
{
class sample_duration
{
public:
    using samples_rep = std::int64_t;

    inline sample_duration() = default;

    inline sample_duration(samples_rep sample_count, std::size_t sample_rate) :
        sample_count_(sample_count), sample_rate_(sample_rate)
    {
    }

    inline samples_rep sample_count() const noexcept
    {
        return sample_count_;
    }

    inline std::size_t sample_rate() const noexcept
    {
        return sample_rate_;
    }

    inline sample_duration& operator+=(const sample_duration& b)
    {
        if (sample_rate_ != b.sample_rate_)
        {
            throw std::invalid_argument("sample rates are different");
        }
        sample_count_ += b.sample_count_;
        return *this;
    }

    inline sample_duration& operator-=(const sample_duration& b)
    {
        if (sample_rate_ != b.sample_rate_)
        {
            throw std::invalid_argument("sample rates are different");
        }
        sample_count_ -= b.sample_count_;
        return *this;
    }

private:
    samples_rep sample_count_{};
    std::size_t sample_rate_{};
};

inline sample_duration operator+(const sample_duration& a, const sample_duration& b)
{
    auto tmp = a;
    tmp += b;
    return tmp;
}

inline sample_duration operator-(const sample_duration& a, const sample_duration& b)
{
    auto tmp = a;
    tmp -= b;
    return tmp;
}

inline bool operator==(const sample_duration& a, const sample_duration& b)
{
    if (a.sample_count() == 0)
    {
        return b.sample_count() == 0;
    }
    return a.sample_count() == b.sample_count() && a.sample_rate() == b.sample_rate();
}

inline bool operator!=(const sample_duration& a, const sample_duration& b)
{
    return !(a == b);
}

inline bool operator<(const sample_duration& a, const sample_duration& b)
{
    if (a.sample_rate() != b.sample_rate())
    {
        throw std::invalid_argument("sample rates are different");
    }
    return a.sample_count() < b.sample_count();
}

inline bool operator>(const sample_duration& a, const sample_duration& b)
{
    return b < a;
}

inline bool operator<=(const sample_duration& a, const sample_duration& b)
{
    return !(b > a);
}

inline bool operator>=(const sample_duration& a, const sample_duration& b)
{
    return !(a < b);
}

} // namespace chrono
} // namespace ratl

#endif // _ratl_chrono_sample_duration_
