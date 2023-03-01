/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

// ratl includes
#include <ratl/ratl.hpp>

// other includes
#include <benchmark/benchmark.h>
#include <random>

namespace ratl
{
static int24_t generateRandom()
{
    static class
    {
    public:
        int24_t generate()
        {
            return static_cast<int24_t>(distribution_(random_generator_));
        }

    private:
        std::random_device random_device_;
        std::mt19937 random_generator_{random_device_()};
        std::uniform_int_distribution<uint32_t> distribution_{
            static_cast<uint32_t>(sample_limits<int32_t>::min()), static_cast<uint32_t>(sample_limits<int32_t>::max())};
    } random_state;
    return random_state.generate();
}

template<typename Converter>
static void benchInt24ToInt32(benchmark::State& state)
{
    std::array<int24_t, 1000000> input;
    for (auto& in : input)
    {
        in = generateRandom();
    }
    std::array<int32_t, 1000000> output;
    for (auto _ : state)
    {
        auto dummy = std::transform(
            input.begin(),
            input.end(),
            output.begin(),
            [](int24_t in)
            {
                return Converter::convert(in);
            });
        benchmark::DoNotOptimize(dummy);
    }
}

struct CurrentConverter
{
    static int32_t convert(int24_t in) noexcept
    {
        return static_cast<int32_t>(in);
    }
};
BENCHMARK_TEMPLATE(benchInt24ToInt32, CurrentConverter);

struct SignExtendConverter
{
    static int32_t convert(int24_t in) noexcept
    {
        return (static_cast<int32_t>(
                    (static_cast<uint32_t>(in.storage_[0])) | (static_cast<uint32_t>(in.storage_[1]) << 8) |
                    (static_cast<uint32_t>(in.storage_[2]) << 16))
                << 8) >>
               8;
    }
};
BENCHMARK_TEMPLATE(benchInt24ToInt32, SignExtendConverter);

struct TopByteCastConverter
{
    static int32_t convert(int24_t in) noexcept
    {
        return static_cast<int32_t>(
            (static_cast<uint32_t>(in.storage_[0])) | (static_cast<uint32_t>(in.storage_[1]) << 8) |
            (static_cast<uint32_t>(static_cast<int32_t>(static_cast<int8_t>(in.storage_[2]))) << 16));
    }
};
BENCHMARK_TEMPLATE(benchInt24ToInt32, TopByteCastConverter);

struct MemcpySignExtendConverter
{
    static int32_t convert(int24_t in) noexcept
    {
        int32_t temp;
        std::memcpy(&temp, &in.storage_, 3);
        return (temp << 8) >> 8;
    }
};
BENCHMARK_TEMPLATE(benchInt24ToInt32, MemcpySignExtendConverter);

} // namespace ratl

BENCHMARK_MAIN();
