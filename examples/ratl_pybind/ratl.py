import sys

import matplotlib.pyplot as plt
import numpy

sys.path.append(sys.argv[1])
import ratl_pybind


def plot_pdfs():
    plt.hist(ratl_pybind.triangular_pdf(), bins=100)
    plt.hist(ratl_pybind.gaussian_pdf(), bins=100)
    plt.show()


def plot_spectrums():
    num_samples = ratl_pybind.num_samples()
    num_freq = int(num_samples / 2)

    def spectrum_db(audio_data):
        assert (len(audio_data) == num_samples)
        fft_out = numpy.fft.fft(audio_data * numpy.hanning(num_samples))
        magnitude = numpy.abs(fft_out[0:num_freq])
        magnitude_db = 20 * numpy.log10(magnitude / num_freq)
        return magnitude_db

    x_axis = numpy.linspace(0, int(ratl_pybind.samplerate() / 2), num_freq)
    plt.plot(x_axis, spectrum_db(ratl_pybind.reference_double_sine()))
    plt.plot(x_axis, spectrum_db(ratl_pybind.test_int_sine()))
    plt.xlim(x_axis[0], x_axis[-1])
    plt.ylim(top=0)
    plt.show()


def main():
    # plot_pdfs()
    plot_spectrums()


if __name__ == "__main__":
    main()
