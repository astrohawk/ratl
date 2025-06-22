import math
import sys

import numpy as np
import matplotlib.pyplot as plt
from ClockDriftModel import ClockDriftModel

print(sys.argv[1])
sys.path.insert(0, sys.argv[1])
from clock_simulation import ClockMapper


def simulate_batch_with_interrupt(clock_mapper, nominal_sample_rate):
    simulation_duration_s = 100
    samples_per_batch = 480

    clock_drift_point_interval_ns = 1e9
    clock_drift_range_ppb = (-1000, 3000)
    clock_drift_jitter_ns = 100

    interrupt_duration_base_ns = 1e7
    interrupt_duration_jitter_range_ns = (-1e5, 1e7)

    initial_source_time_samples = int(1e12)
    initial_dest_time_ns = int(1e14)

    clock_drift_duration_ns = ((simulation_duration_s * 1e9) + interrupt_duration_base_ns +
                               interrupt_duration_jitter_range_ns[1]) * 1.2
    clock_drift_num_points = max(int(clock_drift_duration_ns / clock_drift_point_interval_ns), 2)
    clock_drift_model = ClockDriftModel(duration_ns=clock_drift_duration_ns, num_points=clock_drift_num_points,
                                        drift_range_ppb=clock_drift_range_ppb, jitter_ns=clock_drift_jitter_ns)

    source_time_s_history = []
    actual_dest_ticks_per_source_ticks_history = []
    estimated_dest_ticks_per_source_ticks_history = []
    projected_dest_ticks_per_source_ticks_history = []
    projected_dest_time_error_ns_history = []

    nominal_sample_period_ns = 1e9 / nominal_sample_rate
    initial_source_time_ns = int(math.floor(initial_source_time_samples) * nominal_sample_period_ns)
    num_batches = int((simulation_duration_s * 1e9) / (samples_per_batch * nominal_sample_period_ns))
    for batch_num in range(num_batches):
        interrupt_duration_jitter_ns = int(np.random.triangular(interrupt_duration_jitter_range_ns[0], 0,
                                                                interrupt_duration_jitter_range_ns[1]))
        interrupt_duration_ns = int(interrupt_duration_base_ns + interrupt_duration_jitter_ns)

        batch_source_time_begin_samples = batch_num * samples_per_batch
        batch_source_time_end_samples = batch_source_time_begin_samples + samples_per_batch
        batch_source_time_now_samples = batch_source_time_end_samples + (
                    interrupt_duration_ns / nominal_sample_period_ns)

        batch_source_time_begin_ns = int(math.floor(batch_source_time_begin_samples) * nominal_sample_period_ns)
        batch_source_time_end_ns = int(math.floor(batch_source_time_end_samples) * nominal_sample_period_ns)
        batch_source_time_now_ns = int(math.floor(batch_source_time_now_samples) * nominal_sample_period_ns)

        batch_dest_time_begin_ns = int(
            clock_drift_model.to_other_clock(batch_source_time_begin_samples * nominal_sample_period_ns))
        batch_dest_time_end_ns = int(
            clock_drift_model.to_other_clock(batch_source_time_end_samples * nominal_sample_period_ns))
        batch_dest_time_now_ns = int(
            clock_drift_model.to_other_clock(batch_source_time_now_samples * nominal_sample_period_ns))

        source_time_begin_ns = int(initial_source_time_ns + batch_source_time_begin_ns)
        source_time_end_ns = int(initial_source_time_ns + batch_source_time_end_ns)
        source_time_now_ns = int(initial_source_time_ns + batch_source_time_now_ns)

        dest_time_begin_ns = int(initial_dest_time_ns + batch_dest_time_begin_ns)
        dest_time_end_ns = int(initial_dest_time_ns + batch_dest_time_end_ns)
        dest_time_now_ns = int(initial_dest_time_ns + batch_dest_time_now_ns)

        projected_dest_time_begin_ns, projected_dest_time_end_ns = clock_mapper.get_projected_time(
            source_time_begin_ns, source_time_end_ns, source_time_now_ns, dest_time_now_ns
        )

        source_time_s = batch_source_time_begin_ns / 1e9
        actual_dest_ticks_per_source_ticks = clock_drift_model.drift_multiplier(
            dest_time_begin_ns - initial_dest_time_ns)
        estimated_dest_ticks_per_source_ticks = clock_mapper.get_estimated_dest_ticks_per_source_ticks()
        projected_dest_ticks_per_source_ticks = ((projected_dest_time_end_ns - projected_dest_time_begin_ns) / (
                source_time_end_ns - source_time_begin_ns))
        projected_dest_time_error_ns = projected_dest_time_end_ns - dest_time_end_ns

        source_time_s_history.append(source_time_s)
        actual_dest_ticks_per_source_ticks_history.append(actual_dest_ticks_per_source_ticks)
        estimated_dest_ticks_per_source_ticks_history.append(estimated_dest_ticks_per_source_ticks)
        projected_dest_ticks_per_source_ticks_history.append(projected_dest_ticks_per_source_ticks)
        projected_dest_time_error_ns_history.append(projected_dest_time_error_ns)

    # Plot results
    plt.figure(figsize=(12, 5))

    ax = plt.subplot(1, 2, 1)
    plt.title("Dest Ns per Source Ns")
    plt.plot(source_time_s_history, actual_dest_ticks_per_source_ticks_history, label="Actual", zorder=1)
    plt.plot(source_time_s_history, estimated_dest_ticks_per_source_ticks_history, label="Estimated", zorder=2)
    plt.plot(source_time_s_history, projected_dest_ticks_per_source_ticks_history, label="Projected", zorder=0)
    plt.xlabel("Time (s)")
    plt.ylabel("Dest Ns per Source Ns")
    ax.ticklabel_format(style='plain', useOffset=False, axis='y')
    plt.legend()

    ax = plt.subplot(1, 2, 2)
    plt.title("Projection Error")
    plt.plot(source_time_s_history, projected_dest_time_error_ns_history)
    plt.xlabel("Time (s)")
    plt.ylabel("Error (ns)")
    ax.ticklabel_format(style='plain', useOffset=False, axis='y')

    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    nominal_sample_rate = 48000
    simulate_batch_with_interrupt(ClockMapper(), nominal_sample_rate)
