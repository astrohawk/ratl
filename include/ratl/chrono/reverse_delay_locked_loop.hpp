/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_chrono_reverse_delay_locked_loop_
#define _ratl_chrono_reverse_delay_locked_loop_

// ratl includes
#include <ratl/chrono/subsample_time_point.hpp>
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

//namespace detail
//{
//double constexpr sqrt_newton_raphson(double x, double curr, double prev)
//{
//    return curr == prev ? curr : sqrt_newton_raphson(x, 0.5 * (curr + x / curr), curr);
//}
//} // namespace detail
//
///*
// * Constexpr version of the square root
// * Return value:
// *	- For a finite and non-negative value of "x", returns an approximation for the square root of "x"
// *  - Otherwise, returns NaN
// */
//double constexpr sqrt(double x)
//{
//    return x >= 0 && x < std::numeric_limits<double>::infinity() ? detail::sqrt_newton_raphson(x, x, 0)
//                                                                 : std::numeric_limits<double>::quiet_NaN();
//}

// This is based off of the approach laid out in https://kokkinizita.linuxaudio.org/papers/usingdll.pdf
template<typename Clock, typename SampleClock>
class reverse_delay_locked_loop
{
public:
    using clock_time_point = typename Clock::time_point;
    using clock_duration = typename clock_time_point::duration;

    inline explicit reverse_delay_locked_loop(std::size_t nominal_sample_rate) :
        filter_coefficients_{},
        sample_duration_per_clock_{
            (static_cast<double>(nominal_sample_rate) * static_cast<double>(clock_duration::period::num) /
             static_cast<double>(clock_duration::period::den))}
    {
    }

    std::tuple<subsample_time_point<SampleClock>, subsample_time_point<SampleClock>> get_projected_time(
        const clock_time_point& current_clock_time,
        const sample_time_point<SampleClock>& current_sample_time,
        const clock_time_point& projection_start_clock_time,
        const clock_time_point& projection_end_clock_time)
    {
        if (projection_end_clock_time < projection_start_clock_time)
        {
            throw std::invalid_argument{"projection end clock time is before projection start clock time"};
        }

        if (projection_end_subsample_time_ == subsample_time_point<SampleClock>{})
        {
            auto projection_start_subsample_time =
                current_sample_time +
                subsample_duration{
                    static_cast<double>((projection_start_clock_time - current_clock_time).count()) *
                        sample_duration_per_clock_,
                    current_sample_time.time_since_epoch().sample_rate()};
            projection_end_subsample_time_ =
                current_sample_time +
                subsample_duration{
                    static_cast<double>((projection_end_clock_time - current_clock_time).count()) *
                        sample_duration_per_clock_,
                    current_sample_time.time_since_epoch().sample_rate()};
            previous_projection_end_clock_time_ = projection_end_clock_time;
            return {projection_start_subsample_time, projection_end_subsample_time_};
        }

        if (projection_start_clock_time != previous_projection_end_clock_time_)
        {
            throw std::invalid_argument{
                "projection clock time is not equal to the clock time of the end of the previous projection"};
        }

        if (projection_end_clock_time == previous_projection_end_clock_time_)
        {
            return {projection_end_subsample_time_, projection_end_subsample_time_};
        }

        auto sample_projection_clock_duration =
            static_cast<double>((current_clock_time - previous_projection_end_clock_time_).count());
        auto projected_sample_duration = sample_projection_clock_duration * sample_duration_per_clock_;
        auto actual_subsample_duration = current_sample_time - projection_end_subsample_time_;
        auto sample_duration_error = actual_subsample_duration.subsample_count() - projected_sample_duration;

        auto projection_start_subsample_time = projection_end_subsample_time_;
        auto projection_clock_duration =
            static_cast<double>((projection_end_clock_time - previous_projection_end_clock_time_).count());
        auto error_multiplier = projection_clock_duration * sample_duration_error;
        auto sample_duration_per_clock_delta = filter_coefficients_.get_c() * error_multiplier;
        sample_duration_per_clock_ += sample_duration_per_clock_delta;
        auto projection_end_sample_time_delta = (filter_coefficients_.get_b() * error_multiplier) +
                                                (sample_duration_per_clock_ * projection_clock_duration);
        projection_end_subsample_time_ += subsample_duration{
            projection_end_sample_time_delta, projection_end_subsample_time_.time_since_epoch().sample_rate()};
        previous_projection_end_clock_time_ = projection_end_clock_time;

        // clang-format off
//        std::cout.setf(std::ios::fixed, std::ios::floatfield);
//        std::cout.precision(20);
//        std::cout << "current_clock_time = " << current_clock_time.time_since_epoch().count() << ", "
//                  << "current_sample_time = " << current_sample_time.time_since_epoch().sample_count() << ", "
//                  << "projection_start_clock_time = " << projection_start_clock_time.time_since_epoch().count() << ", "
//                  << "projection_end_clock_time = " << projection_end_clock_time.time_since_epoch().count() << ", "
//                  << "projection_clock_duration = " << std::llround(projection_clock_duration) << ", "
//                  << "projection_start_sample_time = " << projection_start_subsample_time.time_since_epoch().sample_count() << ", "
//                  << "projection_end_sample_time_ = " << projection_end_subsample_time_.time_since_epoch().sample_count() << ", "
//                  << "sample_duration_per_clock_ = " << sample_duration_per_clock_ << ", "
//                  << "sample_rate = " << (static_cast<double>(clock_duration::period::den) * sample_duration_per_clock_) << ", "
//                  << "sample_projection_clock_duration = " << std::llround(sample_projection_clock_duration) << ", "
//                  << "projected_sample_duration = " << projected_sample_duration << ", "
//                  << "actual_subsample_duration = " << actual_subsample_duration.subsample_count() << ", "
//                  << "sample_duration_error = " << sample_duration_error << ", "
//                  << "percent error = " << std::abs((sample_duration_error / projected_sample_duration) * 100)<< ", "
//                  << "error_multiplier = " << error_multiplier << ", "
//                  << "filter_coefficients_.get_c() = " << filter_coefficients_.get_c() << ", "
//                  << "filter_coefficients_.get_b() = " << filter_coefficients_.get_b() << ", "
//                  << "sample_duration_per_clock_delta = " << sample_duration_per_clock_delta << ", "
//                  << "projection_end_sample_time_delta = " << projection_end_sample_time_delta << std::endl;
        // clang-format on

        return {projection_start_subsample_time, projection_end_subsample_time_};
    }

private:
    class filter_coefficients
    {
    public:
        inline double get_b() const noexcept
        {
            return B;
        }

        inline double get_c() const noexcept
        {
            return C;
        }

    private:
        static constexpr double Bandwidth = 0.1;
        static constexpr double Omega = ratl::detail::constants::Tau * Bandwidth *
                                        static_cast<double>(clock_duration::period::num) /
                                        static_cast<double>(clock_duration::period::den);
        static constexpr double B = ratl::detail::constants::Sqrt2 * Omega;
        static constexpr double C = Omega * Omega;
    };

    const filter_coefficients filter_coefficients_;
    subsample_time_point<SampleClock> projection_end_subsample_time_{};
    clock_time_point previous_projection_end_clock_time_{};
    double sample_duration_per_clock_;
};

} // namespace chrono
} // namespace ratl

#endif // _ratl_chrono_reverse_delay_locked_loop_
