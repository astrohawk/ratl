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

namespace detail
{
template<typename Rep, typename Period>
inline sample_duration convert_to_sample_duration(
    const std::chrono::duration<Rep, Period>& duration, std::size_t sample_rate)
{
    static_assert(
        (Period::num & std::numeric_limits<std::uint32_t>::max()) == Period::num,
        "overflow risk as duration period ratio numerator is too large");
    static_assert(
        (Period::den & std::numeric_limits<std::uint32_t>::max()) == Period::den,
        "overflow risk as duration period ratio denominator is too large");

    using unscaled_sample_fraction_rep = std::uint64_t;

    static constexpr auto sample_scaler_num = static_cast<sample_duration::samples_rep>(Period::num);
    static constexpr auto sample_scaler_den = static_cast<sample_duration::samples_rep>(Period::den);

    auto duration_seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
    auto duration_subseconds_remainder =
        duration - std::chrono::duration_cast<std::chrono::duration<Rep, Period>>(duration_seconds);
    auto sample_count_seconds = static_cast<sample_duration::samples_rep>(duration_seconds.count()) *
                                static_cast<sample_duration::samples_rep>(sample_rate);
    auto sample_count_subseconds_remainder_unscaled =
        static_cast<unscaled_sample_fraction_rep>(duration_subseconds_remainder.count()) *
        static_cast<unscaled_sample_fraction_rep>(sample_rate);
    auto sample_count_subseconds_remainder =
        (static_cast<sample_duration::samples_rep>(sample_count_subseconds_remainder_unscaled) * sample_scaler_num) /
        sample_scaler_den;
    auto sample_count = sample_count_seconds + sample_count_subseconds_remainder;

    return sample_duration{sample_count, sample_rate};
}

template<typename Rep, typename Period>
inline std::chrono::duration<Rep, Period> convert_to_duration(const sample_duration& sample_duration)
{
    static_assert(
        (Period::num & std::numeric_limits<std::uint32_t>::max()) == Period::num,
        "overflow risk as duration period ratio numerator is too large");
    static_assert(
        (Period::den & std::numeric_limits<std::uint32_t>::max()) == Period::den,
        "overflow risk as duration period ratio denominator is too large");

    using samples_rep = sample_duration::samples_rep;

    static constexpr auto sample_scaler_num = static_cast<samples_rep>(Period::num);
    static constexpr auto sample_scaler_den = static_cast<samples_rep>(Period::den);

    auto seconds_count = sample_duration.sample_count() / static_cast<samples_rep>(sample_duration.sample_rate());
    auto samples_remainder =
        sample_duration.sample_count() - (seconds_count * static_cast<samples_rep>(sample_duration.sample_rate()));
    auto duration_remainder = static_cast<Rep>(
        (samples_remainder * sample_scaler_den) /
        (static_cast<samples_rep>(sample_duration.sample_rate()) * sample_scaler_num));
    return std::chrono::duration_cast<std::chrono::duration<Rep, Period>>(std::chrono::seconds(seconds_count)) +
           std::chrono::duration<Rep, Period>(duration_remainder);
}
} // namespace detail

//template<typename Rep, typename Period>
//inline std::chrono::duration<Rep, Period> operator+(
//    const std::chrono::duration<Rep, Period>& a, const sample_duration& b)
//{
//    return a + detail::convert_to_duration<Rep, Period>(b);
//}

inline sample_duration operator+(const sample_duration& a, const sample_duration& b)
{
    auto tmp = a;
    tmp += b;
    return tmp;
}

//template<typename Rep, typename Period>
//inline std::chrono::duration<Rep, Period> operator-(
//    const std::chrono::duration<Rep, Period>& a, const sample_duration& b)
//{
//    return a - detail::convert_to_duration<Rep, Period>(b);
//}

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
