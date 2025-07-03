import copy
import math

from simulation import simulate_batch_with_interrupt


class ReferenceClockMapper:
    def __init__(self):
        self.estimated_rate_ = 1.

        self.previous_end_source_time_ = None
        self.previous_estimated_end_dest_time_ = None

    def projected_time(self, start_source_time, end_source_time, current_source_time, current_dest_time):
        if self.previous_estimated_end_dest_time_ is None:
            self.previous_end_source_time_ = end_source_time

            estimated_start_dest_time = current_dest_time - (
                    (current_source_time - start_source_time) * self.estimated_rate_)
            self.previous_estimated_end_dest_time_ = current_dest_time - (
                    (current_source_time - end_source_time) * self.estimated_rate_)

            return estimated_start_dest_time, self.previous_estimated_end_dest_time_

        assert start_source_time == self.previous_end_source_time_

        if end_source_time == self.previous_end_source_time_:
            return self.previous_estimated_end_dest_time_, self.previous_estimated_end_dest_time_

        dest_duration_error, rate_error = self._calculate_errors(current_source_time, current_dest_time)

        estimated_start_dest_time = copy.deepcopy(self.previous_estimated_end_dest_time_)

        source_duration = end_source_time - self.previous_end_source_time_
        filter_coefficients = self.FilterCoefficients(source_duration)

        estimated_rate_delta = filter_coefficients.c * rate_error
        self.estimated_rate_ += estimated_rate_delta

        self.previous_end_source_time_ = end_source_time

        estimated_dest_duration = (source_duration * self.estimated_rate_) + (dest_duration_error * filter_coefficients.b)
        self.previous_estimated_end_dest_time_ += estimated_dest_duration

        return estimated_start_dest_time, self.previous_estimated_end_dest_time_

    def estimated_rate(self):
        return self.estimated_rate_

    def _calculate_errors(self, current_source_time, current_dest_time):
        source_duration = current_source_time - self.previous_end_source_time_
        assert source_duration.count() != 0
        estimated_dest_duration = source_duration * self.estimated_rate_
        actual_dest_duration = current_dest_time - self.previous_estimated_end_dest_time_
        dest_duration_error = actual_dest_duration - estimated_dest_duration
        return dest_duration_error, dest_duration_error.count() / abs(source_duration.count())

    class FilterCoefficients:
        BANDWIDTH = 1 / 8
        OMEGA_CONSTANT = 2 * math.pi * BANDWIDTH / 1e9
        SQRT_2 = math.sqrt(2)

        def __init__(self, source_duration):
            omega = self.OMEGA_CONSTANT * source_duration.count()
            self.b = self.SQRT_2 * omega
            self.c = omega ** 2


if __name__ == "__main__":
    simulate_batch_with_interrupt(ReferenceClockMapper())
