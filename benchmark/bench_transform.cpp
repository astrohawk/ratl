/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

// ratl bench includes
#include "bench_utils.hpp"

namespace ratl
{
template<typename InputSampleType, typename OutputSampleType>
void benchTransform(benchmark::State& state)
{
    using input_type = basic_interleaved<InputSampleType>;
    using output_type = basic_noninterleaved<OutputSampleType>;

    static constexpr std::size_t num_channels = 32;
    static constexpr std::size_t num_frames = 480;

    auto input = utils::generateRandomInput<input_type>(num_channels, num_frames);
    auto output = output_type(num_channels, num_frames);
    dither_generator dither_gen;
    for (auto _ : state)
    {
        reference_transform(input.begin(), input.end(), output.begin(), dither_gen);
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
