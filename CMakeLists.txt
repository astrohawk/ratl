#
# Copyright (c) 2018-2022 Hamish Cook
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.
#

cmake_minimum_required(VERSION 3.15)

project(ratl
        LANGUAGES CXX)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(RATL_CXX_STANDARD 14 CACHE STRING "C++ standard")
    set(CMAKE_CXX_STANDARD ${RATL_CXX_STANDARD})
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    message(STATUS "C++ standard set to C++${CMAKE_CXX_STANDARD}")

    if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC" OR CMAKE_CXX_COMPILER_FRONTEND_VARIANT MATCHES "MSVC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /wd4996")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /bigobj")
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
        if (NOT CMAKE_CXX_FLAGS MATCHES "/arch")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2")
        endif()
        if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
        endif ()
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang|AppleClang)")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wcast-qual -Wnon-virtual-dtor -Werror")
        if (NOT CMAKE_CXX_FLAGS MATCHES "-march")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
            if (NOT CMAKE_CXX_FLAGS MATCHES "-mtune")
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mtune=native")
            endif ()
        endif ()
        if (WIN32)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-microsoft-enum-value -Wno-language-extension-token")
        endif ()
    endif ()

    option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
endif ()

add_subdirectory(third_party)

set(RATL_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(RATL_HEADERS
        ${RATL_INCLUDE_DIR}/ratl/chrono/duration_cast.hpp
        ${RATL_INCLUDE_DIR}/ratl/chrono/forward_delay_locked_loop.hpp
        ${RATL_INCLUDE_DIR}/ratl/chrono/reverse_delay_locked_loop.hpp
        ${RATL_INCLUDE_DIR}/ratl/chrono/sample_duration.hpp
        ${RATL_INCLUDE_DIR}/ratl/chrono/sample_time_point.hpp
        ${RATL_INCLUDE_DIR}/ratl/chrono/subsample_duration.hpp
        ${RATL_INCLUDE_DIR}/ratl/chrono/subsample_time_point.hpp
        ${RATL_INCLUDE_DIR}/ratl/chrono/time_point_cast.hpp
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
        ${RATL_INCLUDE_DIR}/ratl/detail/blit_iterator.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/cast.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/channel_iterator.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/config.hpp
        ${RATL_INCLUDE_DIR}/ratl/detail/constants.hpp
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
        ${RATL_INCLUDE_DIR}/ratl/detail/utility.hpp
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
add_library(ratl::ratl ALIAS ratl)

option(RATL_BUILD_TESTING "Build tests" OFF)
if (RATL_BUILD_TESTING)
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
