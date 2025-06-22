/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

// ratl includes
#include <ratl/chrono.hpp>

// other includes
#include <pybind11/pybind11.h>

namespace py = pybind11;

class DummySampleClock
{
};

using ForwardDelayLockedLoop =
    ratl::chrono::forward_delay_locked_loop<std::chrono::high_resolution_clock, DummySampleClock>;

PYBIND11_MODULE(clock_simulation, m)
{
    py::class_<ForwardDelayLockedLoop>(m, "ForwardDelayLockedLoop")
        .def(py::init<std::size_t>())
        .def(
            "get_projected_time",
            [](ForwardDelayLockedLoop& forward_dll,
               std::uint64_t current_clock_time,
               std::int64_t current_sample_time,
               std::int64_t projection_start_sample_time,
               std::int64_t projection_end_sample_time)
            {
                auto myResult = forward_dll.get_projected_time(
                    std::chrono::high_resolution_clock::time_point{
                        std::chrono::high_resolution_clock::time_point::duration{current_clock_time}},
                    ratl::chrono::sample_time_point<DummySampleClock>{
                        ratl::chrono::sample_time_point<DummySampleClock>::duration{
                            current_sample_time, forward_dll.get_nominal_sample_rate()}},
                    ratl::chrono::sample_time_point<DummySampleClock>{
                        ratl::chrono::sample_time_point<DummySampleClock>::duration{
                            projection_start_sample_time, forward_dll.get_nominal_sample_rate()}},
                    ratl::chrono::sample_time_point<DummySampleClock>{
                        ratl::chrono::sample_time_point<DummySampleClock>::duration{
                            projection_end_sample_time, forward_dll.get_nominal_sample_rate()}});
                return std::tuple<std::uint64_t, std::uint64_t, double>{
                    std::get<0>(myResult).time_since_epoch().count(),
                    std::get<1>(myResult).time_since_epoch().count(),
                    forward_dll.get_estimated_sample_rate()};
            });
}