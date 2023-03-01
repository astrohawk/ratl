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
    using sample_fraction_rep = std::uint32_t;

    inline subsample_duration() = default;

    inline subsample_duration(double duration, std::size_t sample_rate) :
        sample_count_(static_cast<samples_rep>(duration)),
        sample_fraction_(static_cast<sample_fraction_rep>(std::round(
            (duration - static_cast<double>(sample_count_)) *
            static_cast<double>(std::numeric_limits<sample_fraction_rep>::max())))),
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

    inline sample_fraction_rep sample_fraction() const noexcept
    {
        return sample_fraction_;
    }

    inline double subsample_count() const noexcept
    {
        return static_cast<double>(sample_count_) +
               (static_cast<double>(sample_fraction_) /
                static_cast<double>(std::numeric_limits<sample_fraction_rep>::max()));
    }

    inline std::size_t sample_rate() const noexcept
    {
        return sample_rate_;
    }

    subsample_duration& operator+=(const subsample_duration& other)
    {
        using temp_sample_fraction_rep = std::uint64_t;
        static constexpr auto max_temp_sample_fraction =
            static_cast<temp_sample_fraction_rep>(std::numeric_limits<sample_fraction_rep>::max());

        if (sample_rate_ != other.sample_rate_)
        {
            throw std::invalid_argument("sample rates are different");
        }
        sample_count_ += other.sample_count_;
        auto temp_sample_fraction = static_cast<temp_sample_fraction_rep>(sample_fraction_);
        temp_sample_fraction += static_cast<temp_sample_fraction_rep>(other.sample_fraction_);
        if (temp_sample_fraction > max_temp_sample_fraction)
        {
            sample_count_ += 1;
            temp_sample_fraction -= max_temp_sample_fraction;
        }
        sample_fraction_ = static_cast<sample_fraction_rep>(temp_sample_fraction);
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
        using temp_sample_fraction_rep = std::int64_t;
        static constexpr auto max_temp_sample_fraction =
            static_cast<temp_sample_fraction_rep>(std::numeric_limits<sample_fraction_rep>::max());

        if (sample_rate_ != other.sample_rate_)
        {
            throw std::invalid_argument("sample rates are different");
        }
        sample_count_ -= other.sample_count_;
        auto temp_sample_fraction = static_cast<temp_sample_fraction_rep>(sample_fraction_);
        temp_sample_fraction -= static_cast<temp_sample_fraction_rep>(other.sample_fraction_);
        if (temp_sample_fraction < 0)
        {
            sample_count_ -= 1;
            temp_sample_fraction += max_temp_sample_fraction;
        }
        sample_fraction_ = static_cast<sample_fraction_rep>(temp_sample_fraction);
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
    inline subsample_duration(samples_rep sample_count, sample_fraction_rep sample_fraction, std::size_t sample_rate) :
        sample_count_{sample_count}, sample_fraction_{sample_fraction}, sample_rate_{sample_rate}
    {
    }

    template<typename Rep, typename Period>
    static subsample_duration make_subsample_duration(
        const std::chrono::duration<Rep, Period>& duration, std::size_t sample_rate)
    {
        static_assert(
            (Period::num & std::numeric_limits<std::uint32_t>::max()) == Period::num,
            "overflow risk as duration period ratio numerator is too large");
        static_assert(
            (Period::den & std::numeric_limits<std::uint32_t>::max()) == Period::den,
            "overflow risk as duration period ratio denominator is too large");

        using unscaled_sample_fraction_rep = std::uint64_t;

        static constexpr auto sample_scaler_num = static_cast<samples_rep>(Period::num);
        static constexpr auto sample_scaler_den = static_cast<samples_rep>(Period::den);

        static constexpr auto unscaled_sample_fraction_num = static_cast<unscaled_sample_fraction_rep>(Period::num);
        static constexpr auto unscaled_sample_fraction_den = static_cast<unscaled_sample_fraction_rep>(Period::den);
        static constexpr auto unscaled_sample_fraction_max = static_cast<unscaled_sample_fraction_rep>(
            std::numeric_limits<subsample_duration::sample_fraction_rep>::max());

        auto duration_seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
        auto duration_subseconds_remainder =
            duration - std::chrono::duration_cast<std::chrono::duration<Rep, Period>>(duration_seconds);
        auto sample_count_seconds =
            static_cast<samples_rep>(duration_seconds.count()) * static_cast<samples_rep>(sample_rate);
        auto sample_count_subseconds_remainder_unscaled =
            static_cast<unscaled_sample_fraction_rep>(duration_subseconds_remainder.count()) *
            static_cast<unscaled_sample_fraction_rep>(sample_rate);
        auto sample_count_subseconds_remainder =
            (static_cast<samples_rep>(sample_count_subseconds_remainder_unscaled) * sample_scaler_num) /
            sample_scaler_den;
        auto sample_count = sample_count_seconds + sample_count_subseconds_remainder;

        auto sample_fraction_unscaled =
            sample_count_subseconds_remainder_unscaled -
            static_cast<unscaled_sample_fraction_rep>(
                (sample_count_subseconds_remainder * sample_scaler_den) / sample_scaler_num);
        auto sample_fraction = static_cast<sample_fraction_rep>(
            ((sample_fraction_unscaled * unscaled_sample_fraction_max) / unscaled_sample_fraction_den) *
            unscaled_sample_fraction_num);

        return {sample_count, sample_fraction, sample_rate};
    }

    samples_rep sample_count_{};
    sample_fraction_rep sample_fraction_{};
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

} // namespace chrono
} // namespace ratl

#endif // _ratl_chrono_subsample_duration_
