import math
import sys

import numpy as np
import matplotlib.pyplot as plt
from ClockDriftModel import ClockDriftModel

print(sys.argv[1])
sys.path.insert(0, sys.argv[1])
from clock_simulation import ForwardDelayLockedLoop


def simulate_batch_with_interrupt(dll, nominal_sample_rate):
    simulation_duration_s = 100
    samples_per_batch = 480

    clock_drift_point_interval_ns = 1e9
    clock_drift_range_ppb = (-1000, 3000)
    clock_drift_jitter_ns = 100

    interrupt_local_time_base_delay_ns = 1e7
    interrupt_local_time_jitter_range_ns = (-1e5, 1e7)

    initial_sample_time = int(1e12)
    initial_local_time = int(1e14)

    clock_drift_duration_ns = ((simulation_duration_s * 1e9) + interrupt_local_time_base_delay_ns +
                               interrupt_local_time_jitter_range_ns[1]) * 1.2
    clock_drift_num_points = max(int(clock_drift_duration_ns / clock_drift_point_interval_ns), 2)
    clock_drift_model = ClockDriftModel(duration_ns=clock_drift_duration_ns, num_points=clock_drift_num_points,
                                        drift_range_ppb=clock_drift_range_ppb, jitter_ns=clock_drift_jitter_ns)

    local_times_s = []
    sample_times = []
    projected_local_time_durations_per_sample = []
    projected_local_time_errors = []
    actual_sample_rates = []
    estimated_sample_rates = []

    nominal_sample_period_ns = 1e9 / nominal_sample_rate
    num_batches = int((simulation_duration_s * 1e9) / (samples_per_batch * nominal_sample_period_ns))
    for batch_num in range(num_batches):
        interrupt_local_time_jitter_ns = np.random.triangular(interrupt_local_time_jitter_range_ns[0], 0,
                                                              interrupt_local_time_jitter_range_ns[1])
        interrupt_local_time_delay_ns = interrupt_local_time_base_delay_ns + interrupt_local_time_jitter_ns

        batch_sample_time_begin = batch_num * samples_per_batch
        batch_sample_time_end = batch_sample_time_begin + samples_per_batch
        batch_sample_time_now = batch_sample_time_end + (interrupt_local_time_delay_ns / nominal_sample_period_ns)

        batch_local_time_begin = clock_drift_model.to_other_clock(batch_sample_time_begin * nominal_sample_period_ns)
        batch_local_time_end = clock_drift_model.to_other_clock(batch_sample_time_end * nominal_sample_period_ns)
        batch_local_time_now = clock_drift_model.to_other_clock(batch_sample_time_now * nominal_sample_period_ns)

        sample_time_begin = int(initial_sample_time + batch_sample_time_begin)
        sample_time_end = int(initial_sample_time + batch_sample_time_end)
        sample_time_now = int(initial_sample_time + batch_sample_time_now)

        local_time_begin = int(initial_local_time + batch_local_time_begin)
        local_time_end = int(initial_local_time + batch_local_time_end)
        local_time_now = int(initial_local_time + batch_local_time_now)

        projected_local_time_begin, projected_local_time_end, estimated_sample_rate = dll.get_projected_time(
            local_time_now, sample_time_now, sample_time_begin, sample_time_end
        )

        local_time_s = local_time_begin / 1e9
        sample_time = sample_time_begin
        projected_local_time_duration_per_sample = ((projected_local_time_end - projected_local_time_begin) / (
                sample_time_end - sample_time_begin)) / nominal_sample_period_ns
        projected_local_time_error = (local_time_end - projected_local_time_end) / nominal_sample_period_ns
        actual_sample_rate = 1e9 / (clock_drift_model.drift_multiplier(
            local_time_begin - initial_local_time) * nominal_sample_period_ns)

        local_times_s.append(local_time_s)
        sample_times.append(sample_time)
        projected_local_time_durations_per_sample.append(projected_local_time_duration_per_sample)
        projected_local_time_errors.append(projected_local_time_error)
        actual_sample_rates.append(actual_sample_rate)
        estimated_sample_rates.append(estimated_sample_rate)

    abs_max_projected_local_time_error = max([math.ceil(abs(x) * 1.1) for x in projected_local_time_errors] + [1])

    # Plot results
    plt.figure(figsize=(18, 5))

    ax = plt.subplot(1, 3, 1)
    plt.title("Projected Local Time Duration")
    plt.plot(local_times_s, projected_local_time_durations_per_sample)
    plt.xlabel("Time (s)")
    plt.ylabel("Samples Per Nominal Sample")
    plt.ylim(bottom=0.999, top=1.001)
    ax.ticklabel_format(style='plain', useOffset=False, axis='y')

    ax = plt.subplot(1, 3, 2)
    plt.title("Projected Local Time Error")
    plt.plot(local_times_s, projected_local_time_errors)
    plt.xlabel("Time (s)")
    plt.ylabel("Sample Error Per Nominal Sample")
    plt.ylim(bottom=-abs_max_projected_local_time_error, top=abs_max_projected_local_time_error)
    ax.ticklabel_format(style='plain', useOffset=False, axis='y')

    ax = plt.subplot(1, 3, 3)
    plt.title("Actual vs Estimated Sample Rates")
    plt.plot(local_times_s, actual_sample_rates, label="Actual")
    plt.plot(local_times_s, estimated_sample_rates, label="Estimated")
    plt.xlabel("Time (s)")
    plt.ylabel("Sample Rate (Hz)")
    ax.ticklabel_format(style='plain', useOffset=False, axis='y')
    plt.legend()

    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    nominal_sample_rate = 48000
    simulate_batch_with_interrupt(ForwardDelayLockedLoop(nominal_sample_rate), nominal_sample_rate)
