
set(BUILD_TESTS OFF CACHE BOOL "Disable third party tests")
set(BUILD_BENCHMARKS OFF CACHE BOOL "Disable third party benchmarks")
set(BUILD_EXAMPLES OFF CACHE BOOL "Disable third party examples")

set(BENCHMARK_ENABLE_TESTING OFF)
add_subdirectory(benchmark)

add_subdirectory(Gist)

add_subdirectory(googletest)

add_subdirectory(pybind11)

add_subdirectory(xsimd)
