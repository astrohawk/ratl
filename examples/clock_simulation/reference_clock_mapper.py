import copy
import sys
import math

from simulation import simulate_batch_with_interrupt

sys.path.insert(0, sys.argv[1])
import ratl_chrono


class ReferenceClockMapper:
    def __init__(self):
        self.estimated_dest_ticks_per_source_ticks_ = 1.

        self.projection_end_dest_time_ = None
        self.previous_projection_end_source_time_ = None

    def projected_time(self, projection_start_source_time, projection_end_source_time,
                           current_source_time, current_dest_time):
        if self.projection_end_dest_time_ is None:
            projection_start_dest_time = current_dest_time - (
                    (current_source_time - projection_start_source_time) * self.estimated_dest_ticks_per_source_ticks_)
            self.projection_end_dest_time_ = current_dest_time - (
                    (current_source_time - projection_end_source_time) * self.estimated_dest_ticks_per_source_ticks_)
            self.previous_projection_end_source_time_ = projection_end_source_time
            return projection_start_dest_time, self.projection_end_dest_time_

        assert projection_start_source_time == self.previous_projection_end_source_time_

        if projection_end_source_time == self.previous_projection_end_source_time_:
            return self.projection_end_dest_time_, self.projection_end_dest_time_

        dest_ticks_per_source_ticks_error = self._calculate_dest_ticks_per_source_ticks_error(current_source_time,
                                                                                              current_dest_time)

        projection_start_dest_time = copy.deepcopy(self.projection_end_dest_time_)
        projection_source_duration = projection_end_source_time - self.previous_projection_end_source_time_
        filter_coefficients = self.FilterCoefficients(projection_source_duration)
        projection_end_dest_time_delta = ratl_chrono.NsDuration(
            int(((filter_coefficients.b * dest_ticks_per_source_ticks_error) +
                 self.estimated_dest_ticks_per_source_ticks_) * projection_source_duration.count()))
        dest_ticks_per_source_ticks_delta = filter_coefficients.c * dest_ticks_per_source_ticks_error

        self.previous_projection_end_source_time_ = projection_end_source_time
        self.projection_end_dest_time_ += projection_end_dest_time_delta
        self.estimated_dest_ticks_per_source_ticks_ += dest_ticks_per_source_ticks_delta

        return projection_start_dest_time, self.projection_end_dest_time_

    def estimated_dest_ticks_per_source_ticks(self):
        return self.estimated_dest_ticks_per_source_ticks_

    def _calculate_dest_ticks_per_source_ticks_error(self, current_source_time, current_dest_time):
        clock_projection_source_duration = current_source_time - self.previous_projection_end_source_time_
        assert clock_projection_source_duration.count() != 0
        projected_clock_duration = clock_projection_source_duration * self.estimated_dest_ticks_per_source_ticks_
        actual_clock_duration = current_dest_time - self.projection_end_dest_time_
        clock_duration_error = actual_clock_duration - projected_clock_duration
        return clock_duration_error.count() / abs(clock_projection_source_duration.count())

    class FilterCoefficients:
        BANDWIDTH = 1 / 8
        OMEGA_CONSTANT = 2 * math.pi * BANDWIDTH / 1e9
        SQRT_2 = math.sqrt(2)

        def __init__(self, projection_source_duration):
            omega = self.OMEGA_CONSTANT * projection_source_duration.count()
            self.b = self.SQRT_2 * omega
            self.c = omega ** 2


if __name__ == "__main__":
    simulate_batch_with_interrupt(ReferenceClockMapper())
