import sys

import numpy as np
import matplotlib.pyplot as plt
from ClockDriftModel import ClockDriftModel

sys.path.insert(0, sys.argv[1])
import ratl_chrono


def simulate_batch_with_interrupt(clock_mapper):
    nominal_sample_rate = 48000
    simulation_duration_ns = ratl_chrono.NsDuration(int(2 * 60 * 1e9))
    samples_per_batch = ratl_chrono.SampleDuration(480, nominal_sample_rate)

    clock_drift_point_interval_ns = ratl_chrono.NsDuration(int(1e9))
    clock_drift_range_ppb = (-1000, 3000)
    clock_drift_jitter_ns = ratl_chrono.NsDuration(100)

    batch_source_time_start_offset_samples = ratl_chrono.SampleDuration(samples_per_batch.sample_count(),
                                                                        nominal_sample_rate)
    interrupt_duration_base_ns = ratl_chrono.NsDuration(int(1e5))
    interrupt_duration_jitter_range_ns = (ratl_chrono.NsDuration(int(-1e3)), ratl_chrono.NsDuration(int(1e4)))

    initial_source_time_samples = ratl_chrono.SampleTimePoint(
        ratl_chrono.SampleDuration(int(1e12), nominal_sample_rate))
    initial_dest_time_ns = ratl_chrono.NsTimePoint(ratl_chrono.NsDuration(int(1e14)))

    clock_drift_duration_ns = (simulation_duration_ns + interrupt_duration_base_ns + interrupt_duration_jitter_range_ns[
        1]) * 1.2
    clock_drift_num_points = max(clock_drift_duration_ns / clock_drift_point_interval_ns, 2)
    clock_drift_model = ClockDriftModel(duration_ns=clock_drift_duration_ns.count(), num_points=clock_drift_num_points,
                                        drift_range_ppb=clock_drift_range_ppb, jitter_ns=clock_drift_jitter_ns.count())

    source_time_s_history = []
    actual_dest_ticks_per_source_ticks_history = []
    estimated_dest_ticks_per_source_ticks_history = []
    projected_dest_ticks_per_source_ticks_history = []
    projected_dest_time_error_ns_history = []

    initial_source_time_ns = ratl_chrono.NsTimePoint.from_sample_time_point(initial_source_time_samples)
    num_batches = simulation_duration_ns / ratl_chrono.NsDuration.from_sample_duration(samples_per_batch)
    for batch_num in range(num_batches):
        interrupt_duration_jitter_ns = ratl_chrono.NsDuration(
            int(np.random.triangular(interrupt_duration_jitter_range_ns[0].count(), 0,
                                     interrupt_duration_jitter_range_ns[1].count())))
        interrupt_duration_ns = interrupt_duration_base_ns + interrupt_duration_jitter_ns

        batch_source_time_now_samples = batch_num * samples_per_batch
        batch_source_time_begin_samples = batch_source_time_now_samples + batch_source_time_start_offset_samples
        batch_source_time_end_samples = batch_source_time_begin_samples + samples_per_batch
        batch_source_time_interrupt_subsamples = batch_source_time_now_samples + ratl_chrono.SubsampleDuration.from_duration(
            interrupt_duration_ns, nominal_sample_rate)

        batch_source_time_now_ns = ratl_chrono.NsDuration.from_sample_duration(batch_source_time_now_samples)
        batch_source_time_begin_ns = ratl_chrono.NsDuration.from_sample_duration(batch_source_time_begin_samples)
        batch_source_time_end_ns = ratl_chrono.NsDuration.from_sample_duration(batch_source_time_end_samples)
        batch_source_time_interrupt_ns = ratl_chrono.NsDuration.from_sample_duration(
            ratl_chrono.SampleDuration.from_subsample_duration(batch_source_time_interrupt_subsamples))

        batch_dest_time_now_ns = ratl_chrono.NsDuration(int(
            clock_drift_model.to_other_clock(
                ratl_chrono.NsDuration.from_sample_duration(batch_source_time_now_samples).count())))
        batch_dest_time_begin_ns = ratl_chrono.NsDuration(int(
            clock_drift_model.to_other_clock(
                ratl_chrono.NsDuration.from_sample_duration(batch_source_time_begin_samples).count())))
        batch_dest_time_end_ns = ratl_chrono.NsDuration(int(
            clock_drift_model.to_other_clock(
                ratl_chrono.NsDuration.from_sample_duration(batch_source_time_end_samples).count())))
        batch_dest_time_interrupt_ns = ratl_chrono.NsDuration(int(
            clock_drift_model.to_other_clock(
                ratl_chrono.NsDuration.from_subsample_duration(batch_source_time_interrupt_subsamples).count())))

        source_time_now_ns = initial_source_time_ns + batch_source_time_now_ns
        source_time_begin_ns = initial_source_time_ns + batch_source_time_begin_ns
        source_time_end_ns = initial_source_time_ns + batch_source_time_end_ns
        source_time_interrupt_ns = initial_source_time_ns + batch_source_time_interrupt_ns

        dest_time_now_ns = initial_dest_time_ns + batch_dest_time_now_ns
        dest_time_begin_ns = initial_dest_time_ns + batch_dest_time_begin_ns
        dest_time_end_ns = initial_dest_time_ns + batch_dest_time_end_ns
        dest_time_interrupt_ns = initial_dest_time_ns + batch_dest_time_interrupt_ns

        projected_dest_time_begin_ns, projected_dest_time_end_ns = clock_mapper.projected_time(
            source_time_begin_ns, source_time_end_ns, source_time_interrupt_ns, dest_time_interrupt_ns
        )

        source_time_s = batch_source_time_now_ns.count() / 1e9
        actual_dest_ticks_per_source_ticks = clock_drift_model.drift_multiplier(
            (dest_time_begin_ns - initial_dest_time_ns).count())
        estimated_dest_ticks_per_source_ticks = clock_mapper.estimated_dest_ticks_per_source_ticks()
        projected_dest_ticks_per_source_ticks = (projected_dest_time_end_ns - projected_dest_time_begin_ns).count() / (
                source_time_end_ns - source_time_begin_ns).count()
        projected_dest_time_error_ns = (projected_dest_time_end_ns - dest_time_end_ns).count()

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
    simulate_batch_with_interrupt(ratl_chrono.ClockMapper())
