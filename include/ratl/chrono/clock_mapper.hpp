/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_chrono_clock_mapper_
#define _ratl_chrono_clock_mapper_

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/constants.hpp>

// other includes
#include <chrono>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <tuple>

namespace ratl
{
namespace chrono
{
// This is based off of the approach laid out in https://kokkinizita.linuxaudio.org/papers/usingdll.pdf
template<typename SourceClock, typename DestClock>
class clock_mapper
{
public:
    using source_time_point = typename SourceClock::time_point;
    using source_clock_duration = typename source_time_point::duration;
    using dest_time_point = typename DestClock::time_point;
    using dest_clock_duration = typename dest_time_point::duration;

    inline explicit clock_mapper() : estimated_rate_{NominalRate} {}

    std::tuple<dest_time_point, dest_time_point> get_projected_time(
        const source_time_point& start_source_time,
        const source_time_point& end_source_time,
        const source_time_point& current_source_time,
        const dest_time_point& current_dest_time)
    {
        if (end_source_time < start_source_time)
        {
            throw std::invalid_argument{"end source time of the projection is before start source time"};
        }

        if (current_source_time == start_source_time)
        {
            throw std::invalid_argument{"current source time cannot be equal to start source time of the projection"};
        }

        if (previous_estimated_end_dest_time_ == dest_time_point{})
        {
            auto estimated_start_dest_time =
                current_dest_time +
                dest_clock_duration{static_cast<typename dest_clock_duration::rep>(std::llround(
                    static_cast<double>((start_source_time - current_source_time).count()) * estimated_rate_))};
            previous_estimated_end_dest_time_ =
                current_dest_time +
                dest_clock_duration{static_cast<typename dest_clock_duration::rep>(std::llround(
                    static_cast<double>((end_source_time - current_source_time).count()) * estimated_rate_))};
            previous_end_source_time_ = end_source_time;
            return {estimated_start_dest_time, previous_estimated_end_dest_time_};
        }

        if (start_source_time != previous_end_source_time_)
        {
            throw std::invalid_argument{"start source time of the current projection is not equal to the end source "
                                        "time of the previous projection"};
        }

        if (end_source_time == previous_end_source_time_)
        {
            return {previous_estimated_end_dest_time_, previous_estimated_end_dest_time_};
        }

        auto errors = calculate_errors(current_source_time, current_dest_time);
        auto dest_duration_error = std::get<0>(errors);
        auto rate_error = std::get<1>(errors);

        auto estimated_start_dest_time = previous_estimated_end_dest_time_;

        auto source_duration = static_cast<double>((end_source_time - previous_end_source_time_).count());
        auto filter_coefficients = dll_filter_coefficients{source_duration};

        auto rate_delta = filter_coefficients.get_c() * rate_error;
        estimated_rate_ += rate_delta;

        previous_end_source_time_ = end_source_time;

        auto estimated_dest_duration = dest_clock_duration{static_cast<typename dest_clock_duration::rep>(
            std::llround((source_duration * estimated_rate_) + (filter_coefficients.get_b() * dest_duration_error)))};
        previous_estimated_end_dest_time_ += estimated_dest_duration;

        return {estimated_start_dest_time, previous_estimated_end_dest_time_};
    }

    inline double get_estimated_rate() const noexcept
    {
        return estimated_rate_;
    }

private:
    class dll_filter_coefficients
    {
        class DummyTag
        {
        };

    public:
        inline explicit dll_filter_coefficients(double loop_period) :
            dll_filter_coefficients{UnscaledOmega * loop_period, DummyTag{}}
        {
        }

        inline double get_b() const noexcept
        {
            return b_;
        }

        inline double get_c() const noexcept
        {
            return c_;
        }

    private:
        inline explicit dll_filter_coefficients(double omega, DummyTag) :
            b_{ratl::detail::constants::Sqrt2 * omega}, c_{omega * omega}
        {
        }

        static constexpr double Bandwidth = 1. / 8.;
        static constexpr double UnscaledOmega = ratl::detail::constants::Tau * Bandwidth *
                                                static_cast<double>(source_clock_duration::period::num) /
                                                static_cast<double>(source_clock_duration::period::den);
        double b_;
        double c_;
    };

    std::tuple<double, double> calculate_errors(
        const source_time_point& current_source_time, const dest_time_point& current_dest_time) const
    {
        auto source_duration = static_cast<double>((current_source_time - previous_end_source_time_).count());
        auto estimated_dest_duration = source_duration * estimated_rate_;
        auto actual_dest_duration =
            static_cast<double>((current_dest_time - previous_estimated_end_dest_time_).count());
        auto dest_duration_error = actual_dest_duration - estimated_dest_duration;
        return {dest_duration_error, dest_duration_error / std::abs(source_duration)};
    }

    static constexpr double NominalRate = (static_cast<double>(source_clock_duration::period::num) *
                                           static_cast<double>(dest_clock_duration::period::den)) /
                                          (static_cast<double>(source_clock_duration::period::den) *
                                           static_cast<double>(dest_clock_duration::period::num));

    source_time_point previous_end_source_time_{};
    dest_time_point previous_estimated_end_dest_time_{};
    double estimated_rate_;
};

} // namespace chrono
} // namespace ratl

#endif // _ratl_chrono_clock_mapper_
