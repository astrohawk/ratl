#include <benchmark/benchmark.h>
#include <ratl/detail/batch_rand.hpp>
#include <ratl/detail/rand.hpp>

template<class Generator>
void benchPrng(benchmark::State& state)
{
    Generator gen{0x12345678};
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
