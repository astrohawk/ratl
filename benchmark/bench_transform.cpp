/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

// ratl includes
#include <ratl/ratl.hpp>

// other includes
#include <benchmark/benchmark.h>
#include <chrono>
#include <random>
#include <thread>

namespace ratl
{
template<class InputSampleType>
basic_interleaved<InputSampleType> generateRandomInterleaved(std::size_t num_channels, std::size_t num_frames)
{
    std::random_device random_device;
    std::default_random_engine random_engine(random_device());
    std::uniform_real_distribution<float32_t> uniform_dist(-0.8, 0.8);

    interleaved<float32_t> float_interleaved(num_channels, num_frames);
    for (auto frame : float_interleaved)
    {
        for (auto& input : frame)
        {
            input = sample<float32_t>(uniform_dist(random_engine));
        }
    }

    basic_interleaved<InputSampleType> input_interleaved(num_channels, num_frames);
    transform(float_interleaved.begin(), float_interleaved.end(), input_interleaved.begin());
    return input_interleaved;
}

template<class InputSampleType, class OutputSampleType>
void benchSampleTransform(benchmark::State& state)
{
    static constexpr std::size_t num_channels = 32;
    static constexpr std::size_t num_frames = 480;

    auto input = generateRandomInterleaved<InputSampleType>(num_channels, num_frames);
    auto output = basic_noninterleaved<OutputSampleType>(num_channels, num_frames);
    dither_generator dither_gen;
    for (auto _ : state)
    {
        for (std::size_t i = 0; i < num_channels; ++i)
        {
            transform(input.channel(i).begin(), input.channel(i).end(), output.channel(i).begin(), dither_gen);
        }
    }
}

BENCHMARK_TEMPLATE(benchSampleTransform, sample<int16_t>, sample<int16_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<int16_t>, sample<int24_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<int16_t>, sample<int32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<int16_t>, sample<float32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<int24_t>, sample<int16_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<int24_t>, sample<int24_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<int24_t>, sample<int32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<int24_t>, sample<float32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<int32_t>, sample<int16_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<int32_t>, sample<int24_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<int32_t>, sample<int32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<int32_t>, sample<float32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<float32_t>, sample<int16_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<float32_t>, sample<int24_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<float32_t>, sample<int32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<float32_t>, sample<float32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<int16_t>, network_sample<int16_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<int16_t>, network_sample<int24_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<int16_t>, network_sample<int32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<int16_t>, network_sample<float32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<int24_t>, network_sample<int16_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<int24_t>, network_sample<int24_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<int24_t>, network_sample<int32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<int24_t>, network_sample<float32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<int32_t>, network_sample<int16_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<int32_t>, network_sample<int24_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<int32_t>, network_sample<int32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<int32_t>, network_sample<float32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<float32_t>, network_sample<int16_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<float32_t>, network_sample<int24_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<float32_t>, network_sample<int32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, sample<float32_t>, network_sample<float32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, network_sample<int16_t>, sample<int16_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, network_sample<int16_t>, sample<int24_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, network_sample<int16_t>, sample<int32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, network_sample<int16_t>, sample<float32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, network_sample<int24_t>, sample<int16_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, network_sample<int24_t>, sample<int24_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, network_sample<int24_t>, sample<int32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, network_sample<int24_t>, sample<float32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, network_sample<int32_t>, sample<int16_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, network_sample<int32_t>, sample<int24_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, network_sample<int32_t>, sample<int32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, network_sample<int32_t>, sample<float32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, network_sample<float32_t>, sample<int16_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, network_sample<float32_t>, sample<int24_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, network_sample<float32_t>, sample<int32_t>);
BENCHMARK_TEMPLATE(benchSampleTransform, network_sample<float32_t>, sample<float32_t>);

} // namespace ratl

BENCHMARK_MAIN();
