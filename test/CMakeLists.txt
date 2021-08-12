
if (UNIX AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options($<$<CONFIG:DEBUG>:-fsanitize=address> $<$<CONFIG:DEBUG>:-fsanitize=undefined>)
    add_link_options($<$<CONFIG:DEBUG>:-fsanitize=address>)
endif ()


add_executable(test_int24
        test_int24.cpp
        )
target_link_libraries(test_int24
        ratl
        gtest_main
        )

add_test(NAME test_int24 COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_int24 --gtest_output=xml:test_int24.xml)


add_executable(test_uint24
        test_uint24.cpp
        )
target_link_libraries(test_uint24
        ratl
        gtest_main
        )

add_test(NAME test_uint24 COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_uint24 --gtest_output=xml:test_uint24.xml)


add_executable(test_sample
        test_sample.cpp
        test_utils.hpp
        )
target_link_libraries(test_sample
        ratl
        gtest_main
        )

add_test(NAME test_sample COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_sample --gtest_output=xml:test_sample.xml)


add_executable(test_iterators
        test_iterators.cpp
        test_utils.hpp
        )
target_link_libraries(test_iterators
        ratl
        gtest_main
        )

add_test(NAME test_iterators COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_iterators --gtest_output=xml:test_iterators.xml)


add_executable(test_round
        test_round.cpp
        test_utils.hpp
        )
target_link_libraries(test_round
        ratl
        gtest_main
        )

add_test(NAME test_round COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_round --gtest_output=xml:test_round.xml)


add_executable(test_convert
        test_convert.cpp
        test_utils.hpp
        )
target_link_libraries(test_convert
        ratl
        gtest_main
        )

add_test(NAME test_convert COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_convert --gtest_output=xml:test_convert.xml)


add_executable(test_batch_convert
        test_batch_convert.cpp
        test_utils.hpp
        )
target_link_libraries(test_batch_convert
        ratl
        gtest_main
        )

add_test(NAME test_batch_convert COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_batch_convert --gtest_output=xml:test_batch_convert.xml)


add_executable(test_interleaved
        test_interleaved.cpp
        test_utils.hpp
        )
target_link_libraries(test_interleaved
        ratl
        gtest_main
        )

add_test(NAME test_interleaved COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_interleaved --gtest_output=xml:test_interleaved.xml)


add_executable(test_noninterleaved
        test_noninterleaved.cpp
        test_utils.hpp
        )
target_link_libraries(test_noninterleaved
        ratl
        gtest_main
        )

add_test(NAME test_noninterleaved COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_noninterleaved --gtest_output=xml:test_noninterleaved.xml)


add_executable(test_transform_interleaved
        test_transform_interleaved.cpp
        test_utils.hpp
        )
target_link_libraries(test_transform_interleaved
        ratl
        gtest_main
        )

add_test(NAME test_transform_interleaved COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_transform_interleaved --gtest_output=xml: test_transform_interleaved.xml)


add_executable(test_transform_noninterleaved
        test_transform_noninterleaved.cpp
        test_utils.hpp
        )
target_link_libraries(test_transform_noninterleaved
        ratl
        gtest_main
        )

add_test(NAME test_transform_noninterleaved COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_transform_noninterleaved --gtest_output=xml: test_transform_noninterleaved.xml)


add_executable(test_dither
        test_dither.cpp
        test_utils.hpp
        )
target_link_libraries(test_dither
        ratl
        gtest_main
        )

add_test(NAME test_dither COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_dither --gtest_output=xml: test_dither.xml)
