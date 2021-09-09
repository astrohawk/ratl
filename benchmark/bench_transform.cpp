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
template<typename InputType>
InputType generateRandomInput(std::size_t num_channels, std::size_t num_frames)
{
    std::random_device random_device;
    std::default_random_engine random_engine(random_device());
    std::uniform_real_distribution<float32_t> uniform_dist(-0.8f, 0.8f);

    interleaved<float32_t> float_container(num_channels, num_frames);
    for (auto frame : float_container)
    {
        for (auto& input : frame)
        {
            input = sample<float32_t>(uniform_dist(random_engine));
        }
    }

    InputType input(num_channels, num_frames);
    transform(float_container.begin(), float_container.end(), input.begin());
    return input;
}

template<typename InputSampleType, typename OutputSampleType>
void benchTransform(benchmark::State& state)
{
    using input_type = basic_interleaved<InputSampleType>;
    using output_type = basic_noninterleaved<OutputSampleType>;

    static constexpr std::size_t num_channels = 32;
    static constexpr std::size_t num_frames = 480;

    auto input = generateRandomInput<input_type>(num_channels, num_frames);
    auto output = output_type(num_channels, num_frames);
    dither_generator dither_gen;
    for (auto _ : state)
    {
        reference_transform(input.cbegin(), input.cend(), output.begin(), dither_gen);
    }
}

BENCHMARK_TEMPLATE(benchTransform, sample<int16_t>, sample<int16_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<int16_t>, sample<int24_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<int16_t>, sample<int32_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<int16_t>, sample<float32_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<int24_t>, sample<int16_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<int24_t>, sample<int24_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<int24_t>, sample<int32_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<int24_t>, sample<float32_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<int32_t>, sample<int16_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<int32_t>, sample<int24_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<int32_t>, sample<int32_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<int32_t>, sample<float32_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<float32_t>, sample<int16_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<float32_t>, sample<int24_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<float32_t>, sample<int32_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<float32_t>, sample<float32_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<int16_t>, network_sample<int16_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<int16_t>, network_sample<int24_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<int16_t>, network_sample<int32_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<int16_t>, network_sample<float32_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<int24_t>, network_sample<int16_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<int24_t>, network_sample<int24_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<int24_t>, network_sample<int32_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<int24_t>, network_sample<float32_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<int32_t>, network_sample<int16_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<int32_t>, network_sample<int24_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<int32_t>, network_sample<int32_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<int32_t>, network_sample<float32_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<float32_t>, network_sample<int16_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<float32_t>, network_sample<int24_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<float32_t>, network_sample<int32_t>);
BENCHMARK_TEMPLATE(benchTransform, sample<float32_t>, network_sample<float32_t>);
BENCHMARK_TEMPLATE(benchTransform, network_sample<int16_t>, sample<int16_t>);
BENCHMARK_TEMPLATE(benchTransform, network_sample<int16_t>, sample<int24_t>);
BENCHMARK_TEMPLATE(benchTransform, network_sample<int16_t>, sample<int32_t>);
BENCHMARK_TEMPLATE(benchTransform, network_sample<int16_t>, sample<float32_t>);
BENCHMARK_TEMPLATE(benchTransform, network_sample<int24_t>, sample<int16_t>);
BENCHMARK_TEMPLATE(benchTransform, network_sample<int24_t>, sample<int24_t>);
BENCHMARK_TEMPLATE(benchTransform, network_sample<int24_t>, sample<int32_t>);
BENCHMARK_TEMPLATE(benchTransform, network_sample<int24_t>, sample<float32_t>);
BENCHMARK_TEMPLATE(benchTransform, network_sample<int32_t>, sample<int16_t>);
BENCHMARK_TEMPLATE(benchTransform, network_sample<int32_t>, sample<int24_t>);
BENCHMARK_TEMPLATE(benchTransform, network_sample<int32_t>, sample<int32_t>);
BENCHMARK_TEMPLATE(benchTransform, network_sample<int32_t>, sample<float32_t>);
BENCHMARK_TEMPLATE(benchTransform, network_sample<float32_t>, sample<int16_t>);
BENCHMARK_TEMPLATE(benchTransform, network_sample<float32_t>, sample<int24_t>);
BENCHMARK_TEMPLATE(benchTransform, network_sample<float32_t>, sample<int32_t>);
BENCHMARK_TEMPLATE(benchTransform, network_sample<float32_t>, sample<float32_t>);

} // namespace ratl

BENCHMARK_MAIN();
