#
# Copyright (c) 2018-2022 Hamish Cook
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.
#

if (NOT TARGET benchmark::benchmark_main)
    message(FATAL_ERROR "Benchmarking enabled but unable to find benchmark::benchmark_main target")
endif ()

add_executable(bench_float_to_int
        ${CMAKE_CURRENT_LIST_DIR}/bench_float_to_int.cpp)
target_link_libraries(bench_float_to_int
        ratl::ratl
        benchmark::benchmark_main)

add_executable(bench_rand
        ${CMAKE_CURRENT_LIST_DIR}/bench_rand.cpp)
target_link_libraries(bench_rand
        ratl::ratl
        benchmark::benchmark_main)

add_executable(bench_reverse_endianness
        ${CMAKE_CURRENT_LIST_DIR}/bench_reverse_endianness.cpp)
target_link_libraries(bench_reverse_endianness
        ratl::ratl
        benchmark::benchmark_main)

add_executable(bench_transform
        ${CMAKE_CURRENT_LIST_DIR}/bench_transform.cpp)
target_link_libraries(bench_transform
        ratl::ratl
        benchmark::benchmark_main)
