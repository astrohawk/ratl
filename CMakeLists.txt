
cmake_minimum_required(VERSION 3.3)

project(ratl)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")

add_definitions(-DRATL_HAS_XSIMD)
add_subdirectory(third_party)

add_subdirectory(test)
add_subdirectory(benchmark)
add_subdirectory(examples)

set(ratl_INCLUDE_DIR include)
set(ratl_HEADERS
        ${ratl_INCLUDE_DIR}/ratl/ratl.hpp
        ${ratl_INCLUDE_DIR}/ratl/detail/batch_convert.hpp
        ${ratl_INCLUDE_DIR}/ratl/detail/batch_creator.hpp
        ${ratl_INCLUDE_DIR}/ratl/detail/batch_sample_converter.hpp
        ${ratl_INCLUDE_DIR}/ratl/detail/batch_sample_transform.hpp
        ${ratl_INCLUDE_DIR}/ratl/detail/batch_traits.hpp
        ${ratl_INCLUDE_DIR}/ratl/detail/channel_iterator.hpp
        ${ratl_INCLUDE_DIR}/ratl/detail/config.hpp
        ${ratl_INCLUDE_DIR}/ratl/detail/frame_iterator.hpp
        ${ratl_INCLUDE_DIR}/ratl/detail/interleaved_iterator.hpp
        ${ratl_INCLUDE_DIR}/ratl/detail/operator_arrow_proxy.hpp
        ${ratl_INCLUDE_DIR}/ratl/detail/sample_converter.hpp
        ${ratl_INCLUDE_DIR}/ratl/detail/sample_transform.hpp
        ${ratl_INCLUDE_DIR}/ratl/detail/types.hpp
        ${ratl_INCLUDE_DIR}/ratl/allocator.hpp
        ${ratl_INCLUDE_DIR}/ratl/channel_span.hpp
        ${ratl_INCLUDE_DIR}/ratl/convert.hpp
        ${ratl_INCLUDE_DIR}/ratl/frame_span.hpp
        ${ratl_INCLUDE_DIR}/ratl/interleaved.hpp
        ${ratl_INCLUDE_DIR}/ratl/interleaved_span.hpp
        ${ratl_INCLUDE_DIR}/ratl/network_sample.hpp
        ${ratl_INCLUDE_DIR}/ratl/sample.hpp
        ${ratl_INCLUDE_DIR}/ratl/sample_traits.hpp
        ${ratl_INCLUDE_DIR}/ratl/sample_type_limits.hpp
        ${ratl_INCLUDE_DIR}/ratl/transform.hpp
        ${ratl_INCLUDE_DIR}/ratl/type_traits.hpp
        )
file(GLOB_RECURSE ratl_HEADERS_GLOB RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${ratl_INCLUDE_DIR}/*.hpp)
source_group("ratl" FILES ${ratl_HEADERS_GLOB})

add_library(ratl INTERFACE)

target_link_libraries(ratl INTERFACE
        xsimd
        )
target_include_directories(ratl INTERFACE
        ${ratl_INCLUDE_DIR}
        )
