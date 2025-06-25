/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

// ratl includes
#include <ratl/chrono.hpp>

// other includes
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace py::literals;

struct DummyClock
{
    using duration = std::chrono::nanoseconds;
    using time_point = std::chrono::time_point<DummyClock>;

    using sample_duration = ratl::chrono::sample_duration;
    using sample_time_point = ratl::chrono::sample_time_point<DummyClock>;

    using subsample_duration = ratl::chrono::subsample_duration;
    using subsample_time_point = ratl::chrono::subsample_time_point<DummyClock>;
};

using ClockMapper = ratl::chrono::clock_mapper<DummyClock, DummyClock>;

PYBIND11_MODULE(ratl_chrono, m)
{
    py::class_<DummyClock::duration>(m, "NsDuration")
        .def(py::init<const DummyClock::duration::rep&>(), py::arg("rep"))
        .def(
            "__copy__",
            [](const DummyClock::duration& self)
            {
                return DummyClock::duration(self);
            })
        .def(
            "__deepcopy__",
            [](const DummyClock::duration& self, py::dict)
            {
                return DummyClock::duration(self);
            },
            "memo"_a)
        .def("count", &DummyClock::duration::count)
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self * DummyClock::duration::rep())
        .def(DummyClock::duration::rep() * py::self)
        .def(
            "__mul__",
            [](const DummyClock::duration& a, float b)
            {
                return std::chrono::duration_cast<DummyClock::duration>(a * b);
            },
            py::is_operator())
        .def(
            "__mul__",
            [](float a, const DummyClock::duration& b)
            {
                return std::chrono::duration_cast<DummyClock::duration>(a * b);
            },
            py::is_operator())
        .def(py::self / py::self)
        .def(py::self / DummyClock::duration::rep())
        .def(
            "__div__",
            [](const DummyClock::duration& a, float b)
            {
                return std::chrono::duration_cast<DummyClock::duration>(a / b);
            },
            py::is_operator())
        .def(py::self % py::self)
        .def(py::self % DummyClock::duration::rep())
        .def(py::self += py::self)
        .def(py::self -= py::self)
        .def(py::self *= DummyClock::duration::rep())
        .def(
            "__imul__",
            [](DummyClock::duration& a, float b)
            {
                a = std::chrono::duration_cast<DummyClock::duration>(a * b);
            },
            py::is_operator())
        .def(
            "__idiv__",
            [](DummyClock::duration& a, float b)
            {
                a = std::chrono::duration_cast<DummyClock::duration>(a / b);
            },
            py::is_operator())
        .def(py::self /= DummyClock::duration::rep())
        .def(py::self %= py::self)
        .def(py::self %= DummyClock::duration::rep())
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def(py::self < py::self)
        .def(py::self > py::self)
        .def(py::self <= py::self)
        .def(py::self >= py::self)
        .def(
            "__repr__",
            [](const DummyClock::duration& self)
            {
                return "NsDuration(count=" + std::to_string(self.count()) + ")";
            })
        .def_static(
            "from_sample_duration",
            [](const DummyClock::sample_duration& sample_duration)
            {
                return ratl::chrono::detail::
                    convert_to_duration<DummyClock::duration::rep, DummyClock::duration::period>(sample_duration);
            },
            py::arg("sample_duration"))
        .def_static(
            "from_subsample_duration",
            [](const DummyClock::subsample_duration& subsample_duration)
            {
                return ratl::chrono::detail::
                    convert_to_duration<DummyClock::duration::rep, DummyClock::duration::period>(subsample_duration);
            },
            py::arg("subsample_duration"));

    py::class_<DummyClock::time_point>(m, "NsTimePoint")
        .def(py::init<const DummyClock::duration&>(), py::arg("duration"))
        .def(
            "__copy__",
            [](const DummyClock::time_point& self)
            {
                return DummyClock::time_point(self);
            })
        .def(
            "__deepcopy__",
            [](const DummyClock::time_point& self, py::dict)
            {
                return DummyClock::time_point(self);
            },
            "memo"_a)
        .def("time_since_epoch", &DummyClock::time_point::time_since_epoch)
        .def(py::self + DummyClock::duration())
        .def(DummyClock::duration() + py::self)
        .def(py::self - py::self)
        .def(py::self - DummyClock::duration())
        .def(py::self += DummyClock::duration())
        .def(py::self -= DummyClock::duration())
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def(py::self < py::self)
        .def(py::self > py::self)
        .def(py::self <= py::self)
        .def(py::self >= py::self)
        .def(
            "__repr__",
            [](const DummyClock::time_point& self)
            {
                return "NsTimePoint(time_since_epoch=NsDuration(count=" +
                       std::to_string(self.time_since_epoch().count()) + "))";
            })
        .def_static(
            "from_sample_time_point",
            [](const DummyClock::sample_time_point& sample_time_point)
            {
                return ratl::chrono::detail::convert_to_time_point<DummyClock::duration>(sample_time_point);
            },
            py::arg("sample_time_point"))
        .def_static(
            "from_subsample_time_point",
            [](const DummyClock::subsample_time_point& subsample_time_point)
            {
                return ratl::chrono::detail::convert_to_time_point<DummyClock::duration>(subsample_time_point);
            },
            py::arg("subsample_time_point"));

    py::class_<DummyClock::sample_duration>(m, "SampleDuration")
        .def(py::init<DummyClock::sample_duration::samples_rep, std::size_t>(), py::arg("rep"), py::arg("sample_rate"))
        .def(
            "__copy__",
            [](const DummyClock::sample_duration& self)
            {
                return DummyClock::sample_duration(self);
            })
        .def(
            "__deepcopy__",
            [](const DummyClock::sample_duration& self, py::dict)
            {
                return DummyClock::sample_duration(self);
            },
            "memo"_a)
        .def("sample_count", &DummyClock::sample_duration::sample_count)
        .def("sample_rate", &DummyClock::sample_duration::sample_rate)
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self * DummyClock::sample_duration::samples_rep())
        .def(DummyClock::sample_duration::samples_rep() * py::self)
        .def(py::self / py::self)
        .def(py::self / DummyClock::sample_duration::samples_rep())
        .def(py::self % py::self)
        .def(py::self % DummyClock::sample_duration::samples_rep())
        .def(py::self += py::self)
        .def(py::self -= py::self)
        .def(py::self *= DummyClock::sample_duration::samples_rep())
        .def(py::self /= DummyClock::sample_duration::samples_rep())
        .def(py::self %= py::self)
        .def(py::self %= DummyClock::sample_duration::samples_rep())
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def(py::self < py::self)
        .def(py::self > py::self)
        .def(py::self <= py::self)
        .def(py::self >= py::self)
        .def(
            "__repr__",
            [](const DummyClock::sample_duration& self)
            {
                return "SampleDuration(sample_count=" + std::to_string(self.sample_count()) +
                       ", sample_rate=" + std::to_string(self.sample_rate()) + ")";
            })
        .def_static(
            "from_duration",
            [](const DummyClock::duration& duration, std::size_t sample_rate)
            {
                return ratl::chrono::detail::
                    convert_to_sample_duration<DummyClock::duration::rep, DummyClock::duration::period>(
                        duration, sample_rate);
            },
            py::arg("duration"),
            py::arg("sample_rate"))
        .def_static(
            "from_subsample_duration",
            [](const DummyClock::subsample_duration& subsample_duration)
            {
                return ratl::chrono::detail::convert_to_sample_duration(subsample_duration);
            },
            py::arg("subsample_duration"));

    py::class_<DummyClock::sample_time_point>(m, "SampleTimePoint")
        .def(py::init<const DummyClock::sample_duration&>(), py::arg("sample_duration"))
        .def(
            "__copy__",
            [](const DummyClock::sample_time_point& self)
            {
                return DummyClock::sample_time_point(self);
            })
        .def(
            "__deepcopy__",
            [](const DummyClock::sample_time_point& self, py::dict)
            {
                return DummyClock::sample_time_point(self);
            },
            "memo"_a)
        .def("time_since_epoch", &DummyClock::sample_time_point::time_since_epoch)
        .def(py::self + DummyClock::sample_duration())
        .def(DummyClock::sample_duration() + py::self)
        .def(py::self - py::self)
        .def(py::self - DummyClock::sample_duration())
        .def(py::self += DummyClock::sample_duration())
        .def(py::self -= DummyClock::sample_duration())
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def(py::self < py::self)
        .def(py::self > py::self)
        .def(py::self <= py::self)
        .def(py::self >= py::self)
        .def(
            "__repr__",
            [](const DummyClock::sample_time_point& self)
            {
                return "SampleTimePoint(time_since_epoch=SampleDuration(sample_count=" +
                       std::to_string(self.time_since_epoch().sample_count()) +
                       ", sample_rate=" + std::to_string(self.time_since_epoch().sample_rate()) + "))";
            })
        .def_static(
            "from_time_point",
            [](const DummyClock::time_point& time_point, std::size_t sample_rate)
            {
                return ratl::chrono::detail::convert_to_sample_time_point(time_point, sample_rate);
            },
            py::arg("time_point"),
            py::arg("sample_rate"))
        .def_static(
            "from_subsample_time_point",
            [](const DummyClock::subsample_time_point& subsample_time_point)
            {
                return ratl::chrono::detail::convert_to_sample_time_point(subsample_time_point);
            },
            py::arg("subsample_time_point"));

    py::class_<DummyClock::subsample_duration>(m, "SubsampleDuration")
        .def(
            py::init<DummyClock::subsample_duration::samples_rep, std::size_t>(),
            py::arg("rep"),
            py::arg("sample_rate"))
        .def(
            "__copy__",
            [](const DummyClock::subsample_duration& self)
            {
                return DummyClock::subsample_duration(self);
            })
        .def(
            "__deepcopy__",
            [](const DummyClock::subsample_duration& self, py::dict)
            {
                return DummyClock::subsample_duration(self);
            },
            "memo"_a)
        .def("sample_count", &DummyClock::subsample_duration::sample_count)
        .def("sample_fraction", &DummyClock::subsample_duration::sample_fraction)
        .def("subsample_count", &DummyClock::subsample_duration::subsample_count)
        .def("sample_rate", &DummyClock::subsample_duration::sample_rate)
        .def(py::self + py::self)
        .def(DummyClock::sample_duration() + py::self)
        .def(py::self + DummyClock::sample_duration())
        .def(py::self - py::self)
        .def(DummyClock::sample_duration() - py::self)
        .def(py::self - DummyClock::sample_duration())
        .def(py::self * DummyClock::subsample_duration::samples_rep())
        .def(DummyClock::subsample_duration::samples_rep() * py::self)
        .def(py::self * double())
        .def(double() * py::self)
        .def(py::self / DummyClock::subsample_duration::samples_rep())
        .def(py::self / double())
        .def(py::self / py::self)
        .def(py::self += py::self)
        .def(py::self += DummyClock::sample_duration())
        .def(py::self -= py::self)
        .def(py::self -= DummyClock::sample_duration())
        .def(py::self *= DummyClock::subsample_duration::samples_rep())
        .def(py::self *= double())
        .def(py::self /= DummyClock::subsample_duration::samples_rep())
        .def(py::self /= double())
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def(py::self < py::self)
        .def(py::self > py::self)
        .def(py::self <= py::self)
        .def(py::self >= py::self)
        .def(
            "__repr__",
            [](const DummyClock::subsample_duration& self)
            {
                return "SubsampleDuration(sample_count=" + std::to_string(self.sample_count()) +
                       ", sample_fraction=" + std::to_string(self.sample_fraction()) +
                       ", sample_rate=" + std::to_string(self.sample_rate()) + ")";
            })
        .def_static(
            "from_duration",
            [](const DummyClock::duration& duration, std::size_t sample_rate)
            {
                return ratl::chrono::detail::
                    convert_to_subsample_duration<DummyClock::duration::rep, DummyClock::duration::period>(
                        duration, sample_rate);
            },
            py::arg("duration"),
            py::arg("sample_rate"))
        .def_static(
            "from_sample_duration",
            [](const DummyClock::sample_duration& sample_duration)
            {
                return ratl::chrono::detail::convert_to_subsample_duration(sample_duration);
            },
            py::arg("sample_duration"));

    py::class_<DummyClock::subsample_time_point>(m, "SubsampleTimePoint")
        .def(py::init<const DummyClock::subsample_duration&>(), py::arg("subsample_duration"))
        .def(
            "__copy__",
            [](const DummyClock::subsample_time_point& self)
            {
                return DummyClock::subsample_time_point(self);
            })
        .def(
            "__deepcopy__",
            [](const DummyClock::subsample_time_point& self, py::dict)
            {
                return DummyClock::subsample_time_point(self);
            },
            "memo"_a)
        .def("time_since_epoch", &DummyClock::subsample_time_point::time_since_epoch)
        .def(py::self + DummyClock::subsample_duration())
        .def(py::self + DummyClock::sample_duration())
        .def(DummyClock::subsample_duration() + py::self)
        .def(DummyClock::sample_duration() + py::self)
        .def(py::self - py::self)
        .def(py::self - DummyClock::subsample_duration())
        .def(py::self - DummyClock::sample_duration())
        .def(py::self += DummyClock::subsample_duration())
        .def(py::self += DummyClock::sample_duration())
        .def(py::self -= DummyClock::subsample_duration())
        .def(py::self -= DummyClock::sample_duration())
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def(py::self < py::self)
        .def(py::self > py::self)
        .def(py::self <= py::self)
        .def(py::self >= py::self)
        .def(
            "__repr__",
            [](const DummyClock::subsample_time_point& self)
            {
                return "SubsampleTimePoint(time_since_epoch=SubsampleDuration(sample_count=" +
                       std::to_string(self.time_since_epoch().sample_count()) +
                       ", sample_fraction=" + std::to_string(self.time_since_epoch().sample_fraction()) +
                       ", sample_rate=" + std::to_string(self.time_since_epoch().sample_rate()) + "))";
            })
        .def_static(
            "from_time_point",
            [](const DummyClock::time_point& time_point, std::size_t sample_rate)
            {
                return ratl::chrono::detail::convert_to_subsample_time_point(time_point, sample_rate);
            },
            py::arg("time_point"),
            py::arg("sample_rate"))
        .def_static(
            "from_sample_time_point",
            [](const DummyClock::sample_time_point& sample_time_point)
            {
                return ratl::chrono::detail::convert_to_subsample_time_point(sample_time_point);
            },
            py::arg("from_sample_time_point"));

    py::class_<ClockMapper>(m, "ClockMapper")
        .def(py::init<>())
        .def(
            "projected_time",
            &ClockMapper::get_projected_time,
            py::arg("projection_start_source_time"),
            py::arg("projection_end_source_time"),
            py::arg("current_source_time"),
            py::arg("current_dest_time"))
        .def("estimated_dest_ticks_per_source_ticks", &ClockMapper::get_estimated_dest_ticks_per_source_ticks)
        .def(
            "__repr__",
            [](const ClockMapper& self)
            {
                return "ClockMapper(estimated_dest_ticks_per_source_ticks=" +
                       std::to_string(self.get_estimated_dest_ticks_per_source_ticks()) + ")";
            });
}