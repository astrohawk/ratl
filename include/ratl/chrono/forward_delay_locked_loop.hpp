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
        nominal_sample_rate_{nominal_sample_rate},
        filter_coefficients_generator_{nominal_sample_rate_},
        clock_duration_per_sample_{
            static_cast<double>(clock_duration::period::den) /
            (static_cast<double>(nominal_sample_rate_) * static_cast<double>(clock_duration::period::num))}
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

        auto clock_duration_per_sample_error =
            calculate_clock_duration_per_sample_error(current_clock_time, current_sample_time);

        auto projection_start_clock_time = projection_end_clock_time_;
        auto projection_sample_duration =
            static_cast<double>((projection_end_sample_time - previous_projection_end_sample_time_).sample_count());
        auto filter_coefficients = filter_coefficients_generator_(projection_sample_duration);
        auto projection_end_clock_time_delta = clock_duration{static_cast<typename clock_duration::rep>(std::llround(
            ((filter_coefficients.get_b() * clock_duration_per_sample_error) + clock_duration_per_sample_) *
            projection_sample_duration))};
        projection_end_clock_time_ += projection_end_clock_time_delta;
        auto clock_duration_per_sample_delta = filter_coefficients.get_c() * clock_duration_per_sample_error;
        clock_duration_per_sample_ += clock_duration_per_sample_delta;

        previous_projection_end_sample_time_ = projection_end_sample_time;

        return {projection_start_clock_time, projection_end_clock_time_};
    }

    inline std::size_t get_nominal_sample_rate() const noexcept
    {
        return nominal_sample_rate_;
    }

    inline double get_estimated_sample_rate() const noexcept
    {
        return static_cast<double>(clock_duration::period::den) /
               (static_cast<double>(clock_duration_per_sample_) * static_cast<double>(clock_duration::period::num));
    }

private:
    class filter_coefficients
    {
    public:
        class generator
        {
        public:
            inline explicit generator(std::size_t nominal_sample_rate) :
                unscaled_omega_{OmegaMultiplier / static_cast<double>(nominal_sample_rate)}
            {
            }

            inline filter_coefficients operator()(double loop_period) const noexcept
            {
                return filter_coefficients{unscaled_omega_ * loop_period};
            }

        private:
            double unscaled_omega_;
        };

        inline double get_b() const noexcept
        {
            return b_;
        }

        inline double get_c() const noexcept
        {
            return c_;
        }

    private:
        friend generator;

        inline explicit filter_coefficients(double omega) :
            b_{ratl::detail::constants::Sqrt2 * omega}, c_{omega * omega}
        {
        }

        static constexpr double Bandwidth = 1. / 8.;
        static constexpr double OmegaMultiplier = ratl::detail::constants::Tau * Bandwidth;
        double b_;
        double c_;
    };

    double calculate_clock_duration_per_sample_error(
        const clock_time_point& current_clock_time, const sample_time_point<SampleClock>& current_sample_time) const
    {
        auto clock_projection_sample_duration =
            static_cast<double>((current_sample_time - previous_projection_end_sample_time_).sample_count());
        auto projected_clock_duration = clock_projection_sample_duration * clock_duration_per_sample_;
        auto actual_clock_duration = static_cast<double>((current_clock_time - projection_end_clock_time_).count());
        auto clock_duration_error = actual_clock_duration - projected_clock_duration;
        return clock_duration_error / clock_projection_sample_duration;
    }

    std::size_t nominal_sample_rate_;
    typename filter_coefficients::generator filter_coefficients_generator_;
    clock_time_point projection_end_clock_time_{};
    sample_time_point<SampleClock> previous_projection_end_sample_time_{};
    double clock_duration_per_sample_;
};

} // namespace chrono
} // namespace ratl

#endif // _ratl_chrono_forward_delay_locked_loop_
