#include <benchmark/benchmark.h>
#include <chrono>
#include <random>
#include <ratl/ratl.hpp>
#include <thread>

namespace ratl
{
template<class Sample>
ratl::BasicInterleaved<Sample> generateRandomInterleaved(std::size_t num_channels, std::size_t num_frames)
{
    std::random_device random_device;
    std::default_random_engine random_engine(random_device());
    std::uniform_real_distribution<float32_t> uniform_dist(-0.8, 0.8);

    ratl::Interleaved<float32_t> float_interleaved{num_channels, num_frames};
    for (auto frame : float_interleaved)
    {
        for (auto& sample : frame)
        {
            sample = ratl::Sample<float32_t>{uniform_dist(random_engine)};
        }
    }

    ratl::BasicInterleaved<Sample> input_interleaved{num_channels, num_frames};
    ratl::transform(float_interleaved.begin(), float_interleaved.end(), input_interleaved.begin());
    return input_interleaved;
}

template<class InputSample, class OutputSample>
void benchSampleTransform(benchmark::State& state)
{
    static constexpr std::size_t num_channels = 32;
    static constexpr std::size_t num_frames = 480;

    auto input = generateRandomInterleaved<InputSample>(num_channels, num_frames);
    auto output = ratl::BasicNoninterleaved<OutputSample>{num_channels, num_frames};
    ratl::DitherGenerator dither_generator;
    for (auto _ : state)
    {
        for (std::size_t i = 0; i < num_channels; ++i)
        {
            ratl::transform(
                input.channel(i).begin(), input.channel(i).end(), output.channel(i).begin(), dither_generator);
        }
    }
}

BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<int16_t>, ratl::Sample<int16_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<int16_t>, ratl::Sample<int24_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<int16_t>, ratl::Sample<int32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<int16_t>, ratl::Sample<float32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<int24_t>, ratl::Sample<int16_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<int24_t>, ratl::Sample<int24_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<int24_t>, ratl::Sample<int32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<int24_t>, ratl::Sample<float32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<int32_t>, ratl::Sample<int16_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<int32_t>, ratl::Sample<int24_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<int32_t>, ratl::Sample<int32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<int32_t>, ratl::Sample<float32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<float32_t>, ratl::Sample<int16_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<float32_t>, ratl::Sample<int24_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<float32_t>, ratl::Sample<int32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<float32_t>, ratl::Sample<float32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<int16_t>, ratl::NetworkSample<int16_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<int16_t>, ratl::NetworkSample<int24_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<int16_t>, ratl::NetworkSample<int32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<int16_t>, ratl::NetworkSample<float32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<int24_t>, ratl::NetworkSample<int16_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<int24_t>, ratl::NetworkSample<int24_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<int24_t>, ratl::NetworkSample<int32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<int24_t>, ratl::NetworkSample<float32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<int32_t>, ratl::NetworkSample<int16_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<int32_t>, ratl::NetworkSample<int24_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<int32_t>, ratl::NetworkSample<int32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<int32_t>, ratl::NetworkSample<float32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<float32_t>, ratl::NetworkSample<int16_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<float32_t>, ratl::NetworkSample<int24_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<float32_t>, ratl::NetworkSample<int32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::Sample<float32_t>, ratl::NetworkSample<float32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::NetworkSample<int16_t>, ratl::Sample<int16_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::NetworkSample<int16_t>, ratl::Sample<int24_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::NetworkSample<int16_t>, ratl::Sample<int32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::NetworkSample<int16_t>, ratl::Sample<float32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::NetworkSample<int24_t>, ratl::Sample<int16_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::NetworkSample<int24_t>, ratl::Sample<int24_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::NetworkSample<int24_t>, ratl::Sample<int32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::NetworkSample<int24_t>, ratl::Sample<float32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::NetworkSample<int32_t>, ratl::Sample<int16_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::NetworkSample<int32_t>, ratl::Sample<int24_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::NetworkSample<int32_t>, ratl::Sample<int32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::NetworkSample<int32_t>, ratl::Sample<float32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::NetworkSample<float32_t>, ratl::Sample<int16_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::NetworkSample<float32_t>, ratl::Sample<int24_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::NetworkSample<float32_t>, ratl::Sample<int32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, ratl::NetworkSample<float32_t>, ratl::Sample<float32_t>);

} // namespace ratl

BENCHMARK_MAIN();
