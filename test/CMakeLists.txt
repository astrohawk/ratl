#
# Copyright (c) 2018-2021 Hamish Cook
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.
#

if (NOT TARGET GTest::Main)
    message(FATAL_ERROR "Testing enabled but unable to find GTest::Main target")
endif ()

if (UNIX AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options($<$<CONFIG:DEBUG>:-fsanitize=address> $<$<CONFIG:DEBUG>:-fsanitize=undefined>)
    add_link_options($<$<CONFIG:DEBUG>:-fsanitize=address>)
endif ()


add_executable(test_int24
        ${CMAKE_CURRENT_LIST_DIR}/test_int24.cpp)
target_link_libraries(test_int24
        ratl
        GTest::Main)

add_test(NAME test_int24 COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_int24 --gtest_output=xml:test_int24.xml)


add_executable(test_uint24
        ${CMAKE_CURRENT_LIST_DIR}/test_uint24.cpp)
target_link_libraries(test_uint24
        ratl
        GTest::Main)

add_test(NAME test_uint24 COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_uint24 --gtest_output=xml:test_uint24.xml)


add_executable(test_sample
        ${CMAKE_CURRENT_LIST_DIR}/test_sample.cpp
        ${CMAKE_CURRENT_LIST_DIR}/test_utils.hpp)
target_link_libraries(test_sample
        ratl
        GTest::Main)

add_test(NAME test_sample COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_sample --gtest_output=xml:test_sample.xml)


add_executable(test_iterators
        ${CMAKE_CURRENT_LIST_DIR}/test_iterators.cpp
        ${CMAKE_CURRENT_LIST_DIR}/test_utils.hpp)
target_link_libraries(test_iterators
        ratl
        GTest::Main)

add_test(NAME test_iterators COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_iterators --gtest_output=xml:test_iterators.xml)


add_executable(test_round
        ${CMAKE_CURRENT_LIST_DIR}/test_round.cpp
        ${CMAKE_CURRENT_LIST_DIR}/test_utils.hpp)
target_link_libraries(test_round
        ratl
        GTest::Main)

add_test(NAME test_round COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_round --gtest_output=xml:test_round.xml)


add_executable(test_convert
        ${CMAKE_CURRENT_LIST_DIR}/test_convert.cpp
        ${CMAKE_CURRENT_LIST_DIR}/test_utils.hpp)
target_link_libraries(test_convert
        ratl
        GTest::Main)

add_test(NAME test_convert COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_convert --gtest_output=xml:test_convert.xml)


add_executable(test_batch_convert
        ${CMAKE_CURRENT_LIST_DIR}/test_batch_convert.cpp
        ${CMAKE_CURRENT_LIST_DIR}/test_utils.hpp)
target_link_libraries(test_batch_convert
        ratl
        GTest::Main)

add_test(NAME test_batch_convert COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_batch_convert --gtest_output=xml:test_batch_convert.xml)


add_executable(test_interleaved
        ${CMAKE_CURRENT_LIST_DIR}/test_interleaved.cpp
        ${CMAKE_CURRENT_LIST_DIR}/test_utils.hpp)
target_link_libraries(test_interleaved
        ratl
        GTest::Main)

add_test(NAME test_interleaved COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_interleaved --gtest_output=xml:test_interleaved.xml)


add_executable(test_noninterleaved
        ${CMAKE_CURRENT_LIST_DIR}/test_noninterleaved.cpp
        ${CMAKE_CURRENT_LIST_DIR}/test_utils.hpp)
target_link_libraries(test_noninterleaved
        ratl
        GTest::Main)

add_test(NAME test_noninterleaved COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_noninterleaved --gtest_output=xml:test_noninterleaved.xml)


add_executable(test_transform_interleaved
        ${CMAKE_CURRENT_LIST_DIR}/test_transform_interleaved.cpp
        ${CMAKE_CURRENT_LIST_DIR}/test_utils.hpp)
target_link_libraries(test_transform_interleaved
        ratl
        GTest::Main)

add_test(NAME test_transform_interleaved COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_transform_interleaved --gtest_output=xml: test_transform_interleaved.xml)


add_executable(test_transform_noninterleaved
        ${CMAKE_CURRENT_LIST_DIR}/test_transform_noninterleaved.cpp
        ${CMAKE_CURRENT_LIST_DIR}/test_utils.hpp)
target_link_libraries(test_transform_noninterleaved
        ratl
        GTest::Main)

add_test(NAME test_transform_noninterleaved COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_transform_noninterleaved --gtest_output=xml: test_transform_noninterleaved.xml)


add_executable(test_dither
        ${CMAKE_CURRENT_LIST_DIR}/test_dither.cpp
        ${CMAKE_CURRENT_LIST_DIR}/test_utils.hpp)
target_link_libraries(test_dither
        ratl
        GTest::Main)

add_test(NAME test_dither COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_dither --gtest_output=xml: test_dither.xml)
