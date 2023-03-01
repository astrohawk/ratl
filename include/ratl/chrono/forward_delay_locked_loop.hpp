/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_chrono_forward_delay_locked_loop_
#define _ratl_chrono_forward_delay_locked_loop_

// ratl includes
#include <ratl/chrono/sample_time_point.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/detail/constants.hpp>

// other includes
#include <chrono>
#include <cmath>
#include <stdexcept>
#include <tuple>
//#include <iostream>

namespace ratl
{
namespace chrono
{
// This is based off of the approach laid out in https://kokkinizita.linuxaudio.org/papers/usingdll.pdf
template<typename Clock, typename SampleClock>
class forward_delay_locked_loop
{
public:
    using clock_time_point = typename Clock::time_point;
    using clock_duration = typename clock_time_point::duration;

    inline explicit forward_delay_locked_loop(std::size_t nominal_sample_rate) :
        filter_coefficients_{nominal_sample_rate},
        clock_duration_per_sample_{
            static_cast<double>(clock_duration::period::den) /
            (static_cast<double>(nominal_sample_rate) * static_cast<double>(clock_duration::period::num))}
    {
    }

    std::tuple<clock_time_point, clock_time_point> get_projected_time(
        const clock_time_point& current_clock_time,
        const sample_time_point<SampleClock>& current_sample_time,
        const sample_time_point<SampleClock>& projection_start_sample_time,
        const sample_time_point<SampleClock>& projection_end_sample_time)
    {
        if (projection_end_sample_time < projection_start_sample_time)
        {
            throw std::invalid_argument{"projection end sample time is before projection start sample time"};
        }

        if (projection_end_clock_time_ == clock_time_point{})
        {
            auto projection_start_clock_time =
                current_clock_time +
                clock_duration{static_cast<typename clock_duration::rep>(std::llround(
                    static_cast<double>((projection_start_sample_time - current_sample_time).sample_count()) *
                    clock_duration_per_sample_))};
            projection_end_clock_time_ =
                current_clock_time +
                clock_duration{static_cast<typename clock_duration::rep>(std::llround(
                    static_cast<double>((projection_end_sample_time - current_sample_time).sample_count()) *
                    clock_duration_per_sample_))};
            previous_projection_end_sample_time_ = projection_end_sample_time;
            return {projection_start_clock_time, projection_end_clock_time_};
        }

        if (projection_start_sample_time != previous_projection_end_sample_time_)
        {
            throw std::invalid_argument{
                "projection sample time is not equal to the sample time of the end of the previous projection"};
        }

        if (projection_end_sample_time == previous_projection_end_sample_time_)
        {
            return {projection_end_clock_time_, projection_end_clock_time_};
        }

        auto clock_projection_sample_duration =
            static_cast<double>((current_sample_time - previous_projection_end_sample_time_).sample_count());
        auto projected_clock_duration = clock_projection_sample_duration * clock_duration_per_sample_;
        auto actual_clock_duration = static_cast<double>((current_clock_time - projection_end_clock_time_).count());
        auto clock_duration_error = actual_clock_duration - projected_clock_duration;

        auto projection_start_clock_time = projection_end_clock_time_;
        auto projection_sample_duration =
            static_cast<double>((projection_end_sample_time - previous_projection_end_sample_time_).sample_count());
        auto error_multiplier = projection_sample_duration * clock_duration_error;
        auto clock_duration_per_sample_delta = filter_coefficients_.get_c() * error_multiplier;
        clock_duration_per_sample_ += clock_duration_per_sample_delta;
        auto projection_end_clock_time_delta = clock_duration{static_cast<typename clock_duration::rep>(std::llround(
            (filter_coefficients_.get_b() * error_multiplier) +
            (clock_duration_per_sample_ * projection_sample_duration)))};
        projection_end_clock_time_ += projection_end_clock_time_delta;
        previous_projection_end_sample_time_ = projection_end_sample_time;

        // clang-format off
//        std::cout.setf(std::ios::fixed, std::ios::floatfield);
//        std::cout.precision(20);
//        std::cout << "current_clock_time = " << current_clock_time.time_since_epoch().count() << ", "
//                  << "current_sample_time = " << current_sample_time.time_since_epoch().sample_count() << ", "
//                  << "projection_start_sample_time = " << projection_start_sample_time.time_since_epoch().sample_count() << ", "
//                  << "projection_end_sample_time = " << projection_end_sample_time.time_since_epoch().sample_count() << ", "
//                  << "projection_sample_duration = " << std::llround(projection_sample_duration) << ", "
//                  << "projection_start_clock_time = " << projection_start_clock_time.time_since_epoch().count() << ", "
//                  << "projection_end_clock_time_ = " << projection_end_clock_time_.time_since_epoch().count() << ", "
//                  << "clock_duration_per_sample_ = " << clock_duration_per_sample_ << ", "
//                  << "sample_rate = " << (static_cast<double>(clock_duration::period::den) / clock_duration_per_sample_) << ", "
//                  << "clock_projection_sample_duration = " << std::llround(clock_projection_sample_duration) << ", "
//                  << "projected_clock_duration = " << projected_clock_duration << ", "
//                  << "actual_clock_duration = " << std::llround(actual_clock_duration) << ", "
//                  << "clock_duration_error = " << clock_duration_error << ", "
//                  << "percent error = " << std::abs((clock_duration_error / projected_clock_duration) * 100) << ", "
//                  << "error_multiplier = " << error_multiplier << ", "
//                  << "filter_coefficients_.get_c() = " << filter_coefficients_.get_c() << ", "
//                  << "filter_coefficients_.get_b() = " << filter_coefficients_.get_b() << ", "
//                  << "clock_duration_per_sample_delta = " << clock_duration_per_sample_delta << ", "
//                  << "projection_end_clock_time_delta = " << projection_end_clock_time_delta.count() << std::endl;
        // clang-format on

        return {projection_start_clock_time, projection_end_clock_time_};
    }

private:
    class filter_coefficients
    {
    public:
        inline explicit filter_coefficients(std::size_t sample_rate)
        {
            auto omega = ratl::detail::constants::Tau * Bandwidth / static_cast<double>(sample_rate);
            b_ = ratl::detail::constants::Sqrt2 * omega;
            c_ = omega * omega;
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
        static constexpr double Bandwidth = 0.1;
        double b_{};
        double c_{};
    };

    const filter_coefficients filter_coefficients_;
    clock_time_point projection_end_clock_time_{};
    sample_time_point<SampleClock> previous_projection_end_sample_time_{};
    double clock_duration_per_sample_;
};

} // namespace chrono
} // namespace ratl

#endif // _ratl_chrono_forward_delay_locked_loop_
