import numpy as np
from scipy.interpolate import CubicSpline
import random
import matplotlib.pyplot as plt


class ClockDriftModel:
    def __init__(self, duration_ns, num_points=20, drift_range_ppb=(-100000, 100000), jitter_ns=100, seed=None):
        """
        duration_ns: Duration of the simulation in nanoseconds
        drift_range_ppb: Random component range in parts per million
        dc_offset_ppb: Constant offset added to all drift values
        """
        self.t0_ns = 0
        self.t1_ns = duration_ns
        self.jitter_ns=jitter_ns
        self.seed = seed or random.randint(0, 1_000_000)

        rng = random.Random(self.seed)

        self.base_times = np.linspace(self.t0_ns, self.t1_ns, num_points)

        # Generate random drift values and apply DC offset
        drift_ppb = np.array([rng.uniform(*drift_range_ppb) for _ in range(num_points)])
        self.drift_multipliers = 1.0 + drift_ppb / 1e9

        # Spline interpolation
        self.drift_spline = CubicSpline(self.base_times, self.drift_multipliers, bc_type='natural')

        # Build integral
        self._build_integrated_curve(num_points)

    def _build_integrated_curve(self, num_points, num_samples_per_point=1000):
        self.sampled_base_times = np.linspace(self.t0_ns, self.t1_ns, ((num_points - 1) * num_samples_per_point) + 1)
        drift_values = self.drift_spline(self.sampled_base_times)
        delta_t = np.diff(self.sampled_base_times)
        integrand = (drift_values[:-1] + drift_values[1:]) / 2 * delta_t
        self.integrated_other_times = np.concatenate(([0], np.cumsum(integrand)))

    def _generate_jitter(self):
        return np.random.triangular(-self.jitter_ns, 0, self.jitter_ns)

    def to_other_clock(self, t_base_ns):
        if t_base_ns <= self.sampled_base_times[0]:
            return self.integrated_other_times[0] + self._generate_jitter()
        if t_base_ns >= self.sampled_base_times[-1]:
            return self.integrated_other_times[-1] + self._generate_jitter()

        idx = np.searchsorted(self.sampled_base_times, t_base_ns) - 1
        t0, t1 = self.sampled_base_times[idx], self.sampled_base_times[idx + 1]
        y0, y1 = self.integrated_other_times[idx], self.integrated_other_times[idx + 1]
        alpha = (t_base_ns - t0) / (t1 - t0)
        return y0 + alpha * (y1 - y0) + self._generate_jitter()

    def to_base_clock(self, t_other_ns):
        if t_other_ns <= self.integrated_other_times[0]:
            return self.sampled_base_times[0] + self._generate_jitter()
        if t_other_ns >= self.integrated_other_times[-1]:
            return self.sampled_base_times[-1] + self._generate_jitter()

        idx = np.searchsorted(self.integrated_other_times, t_other_ns) - 1
        y0, y1 = self.integrated_other_times[idx], self.integrated_other_times[idx + 1]
        t0, t1 = self.sampled_base_times[idx], self.sampled_base_times[idx + 1]
        alpha = (t_other_ns - y0) / (y1 - y0)
        return t0 + alpha * (t1 - t0) + self._generate_jitter()

    def drift_multiplier(self, t_base_ns):
        return self.drift_spline(t_base_ns)


def main():
    model = ClockDriftModel(duration_ns=1000.0 * 1e9, num_points=20, drift_range_ppb=(-10000, 30000), jitter_ns=1000)

    # Sample base times
    num_samples = 1000
    t_base_ns = np.linspace(model.t0_ns, model.t1_ns, num_samples)

    # Compute corresponding other clock times
    t_other_ns = np.array([model.to_other_clock(t_ns) for t_ns in t_base_ns])

    t_base_s = [t_ns / 1e9 for t_ns in t_base_ns]
    t_other_samples = [t_ns * 48000 / 1e9 for t_ns in t_other_ns]

    # Estimate instantaneous frequency ratio (df/dt_other / df/dt_base = dt_other/dt_base)
    freq_ratio = np.gradient(t_other_samples, t_base_s)

    # Plot frequency ratio vs. base time
    plt.figure(figsize=(20, 10))
    plt.plot(t_base_s, freq_ratio, label='Relative Frequency (Other/Base)')
    plt.xlabel('Base Clock Time (s)')
    plt.ylabel('Relative Frequency')
    plt.title('Other Clock Frequency vs. Base Clock Time')
    plt.gca().ticklabel_format(style='plain', useOffset=False, axis='y')
    plt.show()


if __name__ == "__main__":
    main()
