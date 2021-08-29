#
# Copyright (c) 2018-2021 Hamish Cook
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.
#

cmake_minimum_required(VERSION 3.11)

project(ratl
        LANGUAGES CXX)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(RATL_CXX_STANDARD 14 CACHE STRING "C++ standard")
    set(CMAKE_CXX_STANDARD ${RATL_CXX_STANDARD})
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    if (MSVC)
        add_compile_options(/W4 /WX)
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
        add_compile_options(-march=native)
        add_compile_options(-Wall -Wextra -Wpedantic -Wcast-qual -Wnon-virtual-dtor -Werror)
    endif ()
endif ()

message(STATUS "C++ standard set to C++${CMAKE_CXX_STANDARD}")

add_subdirectory(third_party)

set(RATL_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(RATL_HEADERS
        ${RATL_INCLUDE_DIR}/ratl/detail/batch_cast.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/batch_creator.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/batch_dither_generator.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/batch_endianness.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/batch_fast_sample_converter_impl.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/batch_rand.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/batch_reference_sample_converter_impl.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/batch_round.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/batch_sample_fix.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/batch_traits.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/batch_value_traits.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/cast.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/channel_iterator.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/config.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/convert_traits.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/dither_generator.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/endianness.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/fast_sample_converter_impl.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/frame_iterator.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/interleaved_iterator.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/intrin.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/noninterleaved_iterator.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/operator_arrow_proxy.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/rand.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/reference_sample_converter_impl.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/round.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/sample_converter.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/sample_iterator.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/sample_span.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/sample_traits.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/sample_value_traits.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/transformer.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/type_traits.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/types.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/xsimd.hpp
        ${RATL_INCLUDE_DIR}/ratl/allocator.hpp
        ${RATL_INCLUDE_DIR}/ratl/channel.hpp
        ${RATL_INCLUDE_DIR}/ratl/channel_span.hpp
        ${RATL_INCLUDE_DIR}/ratl/convert.hpp
        ${RATL_INCLUDE_DIR}/ratl/dither_generator.hpp
        ${RATL_INCLUDE_DIR}/ratl/frame.hpp
        ${RATL_INCLUDE_DIR}/ratl/frame_span.hpp
        ${RATL_INCLUDE_DIR}/ratl/int24.hpp
        ${RATL_INCLUDE_DIR}/ratl/interleaved.hpp
        ${RATL_INCLUDE_DIR}/ratl/interleaved_span.hpp
        ${RATL_INCLUDE_DIR}/ratl/network_sample.hpp
        ${RATL_INCLUDE_DIR}/ratl/noninterleaved.hpp
        ${RATL_INCLUDE_DIR}/ratl/noninterleaved_span.hpp
        ${RATL_INCLUDE_DIR}/ratl/ratl.hpp
        ${RATL_INCLUDE_DIR}/ratl/sample.hpp
        ${RATL_INCLUDE_DIR}/ratl/sample_limits.hpp
        ${RATL_INCLUDE_DIR}/ratl/transform.hpp
        ${RATL_INCLUDE_DIR}/ratl/types.hpp
        ${RATL_INCLUDE_DIR}/ratl/uint24.hpp)

add_library(ratl INTERFACE)
target_include_directories(ratl INTERFACE
        ${RATL_INCLUDE_DIR})
if (TARGET xsimd)
    target_compile_definitions(ratl INTERFACE
            RATL_HAS_XSIMD)
    target_link_libraries(ratl INTERFACE
            xsimd)
endif ()

option(RATL_BUILD_TESTING "Build tests" OFF)
if (RATL_BUILD_TESTING AND BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif ()

option(RATL_BUILD_BENCHMARKS "Build benchmarks" OFF)
if (RATL_BUILD_BENCHMARKS)
    add_subdirectory(benchmark)
endif ()

option(RATL_BUILD_EXAMPLES "Build examples" OFF)
if (RATL_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()
