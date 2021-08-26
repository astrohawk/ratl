/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

// ratl includes
#include <ratl/detail/batch_rand.hpp>
#include <ratl/detail/rand.hpp>

// other includes
#include <benchmark/benchmark.h>

template<class Generator>
void benchPrng(benchmark::State& state)
{
    Generator gen(0x12345678);
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(gen());
    }
}

BENCHMARK_TEMPLATE(benchPrng, ratl::detail::linear_congruential_generator);
#if defined(RATL_HAS_XSIMD)
BENCHMARK_TEMPLATE(benchPrng, ratl::detail::batch_linear_congruential_generator);
#endif

BENCHMARK_MAIN();
