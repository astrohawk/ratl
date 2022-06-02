/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_chrono_subsample_duration_
#define _ratl_chrono_subsample_duration_

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
class subsample_duration
{
public:
    using samples_rep = std::int64_t;

    inline subsample_duration() = default;

    inline subsample_duration(double duration, std::size_t sample_rate) :
        sample_count_(static_cast<samples_rep>(duration)),
        sample_fraction_(duration - static_cast<double>(sample_count_)),
        sample_rate_(sample_rate)
    {
    }

    template<typename Rep, typename Period>
    inline subsample_duration(const std::chrono::duration<Rep, Period>& duration, std::size_t sample_rate) :
        subsample_duration(make_subsample_duration(duration, sample_rate))
    {
    }

    inline subsample_duration(const sample_duration& sample_duration) :
        sample_count_(sample_duration.sample_count()), sample_rate_(sample_duration.sample_rate())
    {
    }

    inline samples_rep sample_count() const noexcept
    {
        return sample_count_;
    }

    inline double sample_fraction() const noexcept
    {
        return sample_fraction_;
    }

    inline double subsample_count() const noexcept
    {
        return static_cast<double>(sample_count_) + sample_fraction_;
    }

    inline std::size_t sample_rate() const noexcept
    {
        return sample_rate_;
    }

    subsample_duration& operator+=(const subsample_duration& other)
    {
        if (sample_rate_ != other.sample_rate_)
        {
            throw std::invalid_argument("sample rates are different");
        }
        sample_count_ += other.sample_count_;
        sample_fraction_ += other.sample_fraction_;
        if (sample_fraction_ > 1)
        {
            sample_count_ += 1;
            sample_fraction_ -= 1;
        }
        return *this;
    }

    inline subsample_duration& operator+=(const sample_duration& other)
    {
        if (sample_rate_ != other.sample_rate())
        {
            throw std::invalid_argument("sample rates are different");
        }
        sample_count_ += other.sample_count();
        return *this;
    }

    template<typename Rep, typename Period>
    inline subsample_duration& operator+=(const std::chrono::duration<Rep, Period>& other)
    {
        *this += subsample_duration(other, sample_rate_);
        return *this;
    }

    subsample_duration& operator-=(const subsample_duration& other)
    {
        if (sample_rate_ != other.sample_rate_)
        {
            throw std::invalid_argument("sample rates are different");
        }
        sample_count_ -= other.sample_count_;
        sample_fraction_ -= other.sample_fraction_;
        if (sample_fraction_ < 0)
        {
            sample_count_ -= 1;
            sample_fraction_ += 1;
        }
        return *this;
    }

    inline subsample_duration& operator-=(const sample_duration& other)
    {
        if (sample_rate_ != other.sample_rate())
        {
            throw std::invalid_argument("sample rates are different");
        }
        sample_count_ -= other.sample_count();
        return *this;
    }

    template<typename Rep, typename Period>
    inline subsample_duration& operator-=(const std::chrono::duration<Rep, Period>& other)
    {
        *this -= subsample_duration(other, sample_rate_);
        return *this;
    }

private:
    inline subsample_duration(samples_rep sample_count, double sample_fraction, std::size_t sample_rate) :
        sample_count_{sample_count}, sample_fraction_{sample_fraction}, sample_rate_{sample_rate}
    {
    }

    template<typename Rep, typename Period>
    static subsample_duration make_subsample_duration(
        const std::chrono::duration<Rep, Period>& duration, std::size_t sample_rate)
    {
        auto current_time_seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
        auto current_time_subsecond_duration = duration - std::chrono::duration<Rep, Period>(current_time_seconds);
        auto sample_count_seconds =
            static_cast<samples_rep>(current_time_seconds.count()) * static_cast<samples_rep>(sample_rate);
        auto sample_count_subsecond_duration_and_proportion =
            static_cast<double>(
                current_time_subsecond_duration.count() * static_cast<samples_rep>(sample_rate) * Period::num) /
            static_cast<double>(Period::den);
        auto sample_count_subsecond_duration = std::trunc(sample_count_subsecond_duration_and_proportion);
        auto sample_count = sample_count_seconds + static_cast<samples_rep>(sample_count_subsecond_duration);
        auto sample_fraction = sample_count_subsecond_duration_and_proportion - sample_count_subsecond_duration;
        return {sample_count, sample_fraction, sample_rate};
    }

    samples_rep sample_count_{};
    double sample_fraction_{};
    std::size_t sample_rate_{};
};

inline subsample_duration operator+(const subsample_duration& a, const subsample_duration& b)
{
    auto tmp = a;
    tmp += b;
    return tmp;
}

inline subsample_duration operator+(const subsample_duration& a, const sample_duration& b)
{
    auto tmp = a;
    tmp += b;
    return tmp;
}

inline subsample_duration operator+(const sample_duration& a, const subsample_duration& b)
{
    auto tmp = b;
    tmp += a;
    return tmp;
}

template<typename Rep, typename Period>
inline subsample_duration operator+(const subsample_duration& a, const std::chrono::duration<Rep, Period>& b)
{
    auto tmp = a;
    tmp += b;
    return tmp;
}

template<typename Rep, typename Period>
inline subsample_duration operator+(const std::chrono::duration<Rep, Period>& a, const subsample_duration& b)
{
    auto tmp = b;
    tmp += a;
    return tmp;
}

template<typename Rep, typename Period>
inline subsample_duration operator+(const sample_duration& a, const std::chrono::duration<Rep, Period>& b)
{
    auto tmp = subsample_duration(a);
    tmp += b;
    return tmp;
}

template<typename Rep, typename Period>
inline subsample_duration operator+(const std::chrono::duration<Rep, Period>& a, const sample_duration& b)
{
    auto tmp = subsample_duration(b);
    tmp += a;
    return tmp;
}

inline subsample_duration operator-(const subsample_duration& a, const subsample_duration& b)
{
    auto tmp = a;
    tmp -= b;
    return tmp;
}

inline subsample_duration operator-(const subsample_duration& a, const sample_duration& b)
{
    auto tmp = a;
    tmp -= b;
    return tmp;
}

inline subsample_duration operator-(const sample_duration& a, const subsample_duration& b)
{
    auto tmp = subsample_duration(a);
    tmp -= b;
    return tmp;
}

template<typename Rep, typename Period>
inline subsample_duration operator-(const subsample_duration& a, const std::chrono::duration<Rep, Period>& b)
{
    auto tmp = a;
    tmp -= b;
    return tmp;
}

template<typename Rep, typename Period>
inline subsample_duration operator-(const std::chrono::duration<Rep, Period>& a, const subsample_duration& b)
{
    auto tmp = subsample_duration(a);
    tmp -= b;
    return tmp;
}

template<typename Rep, typename Period>
inline subsample_duration operator-(const sample_duration& a, const std::chrono::duration<Rep, Period>& b)
{
    auto tmp = subsample_duration(a);
    tmp -= b;
    return tmp;
}

template<typename Rep, typename Period>
inline subsample_duration operator-(const std::chrono::duration<Rep, Period>& a, const sample_duration& b)
{
    auto tmp = subsample_duration(a);
    tmp -= b;
    return tmp;
}

inline bool operator==(const subsample_duration& a, const subsample_duration& b)
{
    if (a.sample_count() == 0 && a.sample_fraction() == 0)
    {
        return b.sample_count() == 0 && b.sample_fraction() == 0;
    }
    return a.sample_count() == b.sample_count() && a.sample_fraction() == b.sample_fraction() &&
           a.sample_rate() == b.sample_rate();
}

inline bool operator!=(const subsample_duration& a, const subsample_duration& b)
{
    return !(a == b);
}

inline bool operator<(const subsample_duration& a, const subsample_duration& b)
{
    if (a.sample_rate() != b.sample_rate())
    {
        throw std::invalid_argument("sample rates are different");
    }
    if (a.sample_count() != b.sample_count())
    {
        return a.sample_count() < b.sample_count();
    }
    return a.sample_fraction() < b.sample_fraction();
}

inline bool operator>(const subsample_duration& a, const subsample_duration& b)
{
    return b < a;
}

inline bool operator<=(const subsample_duration& a, const subsample_duration& b)
{
    return !(b > a);
}

inline bool operator>=(const subsample_duration& a, const subsample_duration& b)
{
    return !(a < b);
}

inline sample_duration sample_duration_cast(const subsample_duration& duration)
{
    return {duration.sample_count(), duration.sample_rate()};
}

} // namespace chrono
} // namespace ratl

#endif // _ratl_chrono_subsample_duration_
