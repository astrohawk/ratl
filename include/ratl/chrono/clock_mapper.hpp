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

    inline explicit clock_mapper() : estimated_dest_ticks_per_source_ticks_{NominalDestTicksPerSourceTicks} {}

    std::tuple<dest_time_point, dest_time_point> get_projected_time(
        const source_time_point& projection_start_source_time,
        const source_time_point& projection_end_source_time,
        const source_time_point& current_source_time,
        const dest_time_point& current_dest_time)
    {
        if (projection_end_source_time < projection_start_source_time)
        {
            throw std::invalid_argument{"projection end sample time is before projection start sample time"};
        }

        if (projection_end_dest_time_ == dest_time_point{})
        {
            auto projection_start_dest_time =
                current_dest_time +
                dest_clock_duration{static_cast<typename dest_clock_duration::rep>(std::llround(
                    static_cast<double>((projection_start_source_time - current_source_time).count()) *
                    estimated_dest_ticks_per_source_ticks_))};
            projection_end_dest_time_ =
                current_dest_time +
                dest_clock_duration{static_cast<typename dest_clock_duration::rep>(std::llround(
                    static_cast<double>((projection_end_source_time - current_source_time).count()) *
                    estimated_dest_ticks_per_source_ticks_))};
            previous_projection_end_source_time_ = projection_end_source_time;
            return {projection_start_dest_time, projection_end_dest_time_};
        }

        if (projection_start_source_time != previous_projection_end_source_time_)
        {
            throw std::invalid_argument{
                "projection sample time is not equal to the sample time of the end of the previous projection"};
        }

        if (projection_end_source_time == previous_projection_end_source_time_)
        {
            return {projection_end_dest_time_, projection_end_dest_time_};
        }

        auto dest_ticks_per_source_ticks_error =
            calculate_dest_ticks_per_source_ticks_error(current_source_time, current_dest_time);

        auto projection_start_dest_time = projection_end_dest_time_;
        auto projection_source_duration =
            static_cast<double>((projection_end_source_time - previous_projection_end_source_time_).count());
        auto filter_coefficients = dll_filter_coefficients{projection_source_duration};
        auto projection_end_dest_time_delta =
            dest_clock_duration{static_cast<typename dest_clock_duration::rep>(std::llround(
                ((filter_coefficients.get_b() * dest_ticks_per_source_ticks_error) +
                 estimated_dest_ticks_per_source_ticks_) *
                projection_source_duration))};
        auto dest_ticks_per_source_ticks_delta = filter_coefficients.get_c() * dest_ticks_per_source_ticks_error;

        previous_projection_end_source_time_ = projection_end_source_time;
        projection_end_dest_time_ += projection_end_dest_time_delta;
        estimated_dest_ticks_per_source_ticks_ += dest_ticks_per_source_ticks_delta;

        return {projection_start_dest_time, projection_end_dest_time_};
    }

    inline double get_estimated_dest_ticks_per_source_ticks() const noexcept
    {
        return estimated_dest_ticks_per_source_ticks_;
    }

private:
    static constexpr double NominalDestTicksPerSourceTicks = (static_cast<double>(source_clock_duration::period::num) *
                                                              static_cast<double>(dest_clock_duration::period::den)) /
                                                             (static_cast<double>(source_clock_duration::period::den) *
                                                              static_cast<double>(dest_clock_duration::period::num));

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

    double calculate_dest_ticks_per_source_ticks_error(
        const source_time_point& current_source_time, const dest_time_point& current_dest_time) const
    {
        auto clock_projection_source_duration =
            static_cast<double>((current_source_time - previous_projection_end_source_time_).count());
        auto projected_dest_duration = clock_projection_source_duration * estimated_dest_ticks_per_source_ticks_;
        auto actual_dest_duration = static_cast<double>((current_dest_time - projection_end_dest_time_).count());
        auto dest_duration_error = actual_dest_duration - projected_dest_duration;

        return dest_duration_error / clock_projection_source_duration;
    }

    source_time_point previous_projection_end_source_time_{};
    dest_time_point projection_end_dest_time_{};
    double estimated_dest_ticks_per_source_ticks_;
};

} // namespace chrono
} // namespace ratl

#endif // _ratl_chrono_clock_mapper_
