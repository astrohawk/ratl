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
using sample_type = int24_t;
using NetworkSampleType = detail::network_sample_value_underlying_type_t<sample_type>;

static NetworkSampleType generateRandom()
{
    static class
    {
    public:
        NetworkSampleType generate()
        {
            return static_cast<NetworkSampleType>(distribution_(random_generator_));
        }

    private:
        std::random_device random_device_;
        std::mt19937 random_generator_{random_device_()};
        std::uniform_int_distribution<uint32_t> distribution_{
            static_cast<uint32_t>(sample_limits<sample_type>::min()),
            static_cast<uint32_t>(sample_limits<sample_type>::max())};
    } random_state;
    return random_state.generate();
}

inline NetworkSampleType reverseEndiannessTest(NetworkSampleType input) noexcept
{
    NetworkSampleType output;
    output.storage_[0] = input.storage_[2];
    output.storage_[1] = input.storage_[1];
    output.storage_[2] = input.storage_[0];
    return output;
}

static void benchReverseEndianness(benchmark::State& state)
{
    std::array<NetworkSampleType, 1000> input;
    for (auto& in : input)
    {
        in = generateRandom();
    }
    std::array<NetworkSampleType, 1000> output;
    for (auto _ : state)
    {
        std::transform(
            input.begin(),
            input.end(),
            output.begin(),
            [](NetworkSampleType in)
            {
                return detail::reverse_endianness(in);
            });
    }
}
BENCHMARK(benchReverseEndianness);

static void benchReverseEndiannessTest(benchmark::State& state)
{
    std::array<NetworkSampleType, 1000> input;
    for (auto& in : input)
    {
        in = generateRandom();
    }
    std::array<NetworkSampleType, 1000> output;
    for (auto _ : state)
    {
        std::transform(
            input.begin(),
            input.end(),
            output.begin(),
            [](NetworkSampleType in)
            {
                return reverseEndiannessTest(in);
            });
    }
}
BENCHMARK(benchReverseEndiannessTest);

} // namespace ratl

BENCHMARK_MAIN();
