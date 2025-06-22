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

using ClockMapper = ratl::chrono::clock_mapper<std::chrono::high_resolution_clock, std::chrono::high_resolution_clock>;

PYBIND11_MODULE(clock_simulation, m)
{
    py::class_<ClockMapper>(m, "ClockMapper")
        .def(py::init<>())
        .def(
            "get_projected_time",
            [](ClockMapper& clock_mapper,
               std::int64_t projection_start_source_time,
               std::int64_t projection_end_source_time,
               std::uint64_t current_source_time,
               std::int64_t current_dest_time)
            {
                auto myResult = clock_mapper.get_projected_time(
                    std::chrono::high_resolution_clock::time_point{
                        std::chrono::high_resolution_clock::time_point::duration{projection_start_source_time}},
                    std::chrono::high_resolution_clock::time_point{
                        std::chrono::high_resolution_clock::time_point::duration{projection_end_source_time}},
                    std::chrono::high_resolution_clock::time_point{
                        std::chrono::high_resolution_clock::time_point::duration{current_source_time}},
                    std::chrono::high_resolution_clock::time_point{
                        std::chrono::high_resolution_clock::time_point::duration{current_dest_time}});
                return std::tuple<std::uint64_t, std::uint64_t>{
                    std::get<0>(myResult).time_since_epoch().count(), std::get<1>(myResult).time_since_epoch().count()};
            })
        .def("get_estimated_dest_ticks_per_source_ticks", &ClockMapper::get_estimated_dest_ticks_per_source_ticks);
}