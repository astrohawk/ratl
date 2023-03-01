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
    static inline sample_duration cast(const sample_duration& duration)
    {
        return duration;
    }

    static inline sample_duration cast(const subsample_duration& duration)
    {
        return {duration.sample_count(), duration.sample_rate()};
    }
};

template<>
struct duration_cast_impl<subsample_duration>
{
    static inline subsample_duration cast(const sample_duration& duration)
    {
        return duration;
    }

    static inline subsample_duration cast(const subsample_duration& duration)
    {
        return duration;
    }
};

template<typename Rep, typename Period>
struct duration_cast_impl<std::chrono::duration<Rep, Period>>
{
    static inline std::chrono::duration<Rep, Period> cast(const sample_duration& duration)
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

        auto seconds_count = duration.sample_count() / static_cast<samples_rep>(duration.sample_rate());
        auto samples_remainder =
            duration.sample_count() - (seconds_count * static_cast<samples_rep>(duration.sample_rate()));
        auto duration_remainder = static_cast<Rep>(
            (samples_remainder * sample_scaler_den) /
            (static_cast<samples_rep>(duration.sample_rate()) * sample_scaler_num));
        return std::chrono::duration_cast<std::chrono::duration<Rep, Period>>(std::chrono::seconds(seconds_count)) +
               std::chrono::duration<Rep, Period>(duration_remainder);
    }

    static inline std::chrono::duration<Rep, Period> cast(const subsample_duration& duration)
    {
        using unscaled_sample_fraction_rep = std::uint64_t;

        static constexpr auto unscaled_sample_fraction_num = static_cast<unscaled_sample_fraction_rep>(Period::num);
        static constexpr auto unscaled_sample_fraction_den = static_cast<unscaled_sample_fraction_rep>(Period::den);
        static constexpr auto unscaled_sample_fraction_max = static_cast<unscaled_sample_fraction_rep>(
            std::numeric_limits<subsample_duration::sample_fraction_rep>::max());

        auto fractional_duration = std::chrono::duration<Rep, Period>(static_cast<Rep>(
            ((static_cast<unscaled_sample_fraction_rep>(duration.sample_fraction()) * unscaled_sample_fraction_den) /
             (static_cast<unscaled_sample_fraction_rep>(duration.sample_rate()) * unscaled_sample_fraction_max)) /
            unscaled_sample_fraction_num));

        return duration_cast_impl<std::chrono::duration<Rep, Period>>::cast(
                   duration_cast_impl<sample_duration>::cast(duration)) +
               fractional_duration;
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
