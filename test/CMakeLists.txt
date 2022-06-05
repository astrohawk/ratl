#
# Copyright (c) 2018-2022 Hamish Cook
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.
#

if (NOT TARGET GTest::Main)
    message(FATAL_ERROR "Testing enabled but unable to find GTest::Main target")
endif ()

include(GoogleTest)

function(ratl_add_test test_target)
    add_executable(${test_target}
            ${CMAKE_CURRENT_LIST_DIR}/${test_target}.cpp)
    target_link_libraries(${test_target}
            ratl::ratl
            GTest::Main)
    if (UNIX AND CMAKE_CXX_COMPILER_ID MATCHES "(Clang|AppleClang)")
        target_compile_options(${test_target}
                PRIVATE $<$<CONFIG:DEBUG>:-fsanitize=address> $<$<CONFIG:DEBUG>:-fsanitize=undefined>)
        target_link_options(${test_target}
                PRIVATE $<$<CONFIG:DEBUG>:-fsanitize=address>)
    endif ()

    gtest_discover_tests(${test_target})
endfunction()

ratl_add_test(test_int24)
ratl_add_test(test_uint24)
ratl_add_test(test_sample)
ratl_add_test(test_iterators)
ratl_add_test(test_round)
ratl_add_test(test_convert)
ratl_add_test(test_batch_convert)
ratl_add_test(test_interleaved)
ratl_add_test(test_noninterleaved)
ratl_add_test(test_transform_interleaved)
ratl_add_test(test_transform_noninterleaved)
ratl_add_test(test_transform_interleaved_to_noninterleaved)
ratl_add_test(test_transform_noninterleaved_to_interleaved)
ratl_add_test(test_dither)
