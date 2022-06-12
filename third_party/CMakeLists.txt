#
# Copyright (c) 2018-2022 Hamish Cook
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.
#

option(RATL_BENCHMARK_SOURCE "Build benchmark from source located in this dir" OFF)
if (RATL_BENCHMARK_SOURCE)
    set(BENCHMARK_ENABLE_TESTING OFF)
    add_subdirectory(benchmark)
endif ()

option(RATL_GOOGLETEST_SOURCE "Build googletest from source located in this dir" OFF)
option(RATL_GOOGLETEST_PACKAGE "Use googletest from installed package" OFF)
if (RATL_GOOGLETEST_SOURCE)
    add_subdirectory(googletest)
    add_library(GTest::GTest ALIAS gtest)
    add_library(GTest::Main ALIAS gtest_main)
elseif(RATL_GOOGLETEST_PACKAGE)
    find_package(GTest REQUIRED)
    set_target_properties(GTest::GTest PROPERTIES IMPORTED_GLOBAL TRUE)
    set_target_properties(GTest::Main PROPERTIES IMPORTED_GLOBAL TRUE)
endif ()

option(RATL_PYBIND11_SOURCE "Build pybind11 from source located in this dir" OFF)
if (RATL_PYBIND11_SOURCE)
    add_subdirectory(pybind11)
endif ()

option(RATL_XSIMD_SOURCE "Build xsimd from source located in this dir" OFF)
if (RATL_XSIMD_SOURCE)
    add_subdirectory(xsimd)
endif ()
