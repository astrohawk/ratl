if (UNIX)
    add_executable(bench_transform
            bench_transform.cpp
            )
    target_link_libraries(bench_transform
            ratl
            benchmark::benchmark_main
            )

    add_executable(bench_rand
            bench_rand.cpp
            )
    target_link_libraries(bench_rand
            ratl
            benchmark::benchmark_main
            )
endif ()
