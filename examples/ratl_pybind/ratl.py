import sys

import matplotlib.pyplot as plt

sys.path.append(sys.argv[1])
import ratl_pybind

if __name__ == "__main__":
    x_axis = ratl_pybind.x_axis()
    plt.plot(x_axis, ratl_pybind.reference_double_sine())
    plt.plot(x_axis, ratl_pybind.reference_int_sine())
    plt.plot(x_axis, ratl_pybind.test_int_sine())
    plt.xlim(x_axis[0], x_axis[-1])
    plt.ylim(top=0)
    # plt.ylim(10**-14, 10**0)
    # plt.yscale("log")
    plt.show()

    # plt.hist(ratl_pybind.triangular_pdf(), bins=100)
    # plt.hist(ratl_pybind.gaussian_pdf(), bins=100)
    # plt.show()
