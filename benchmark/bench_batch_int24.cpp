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

struct full_load
{
};
struct partial_load
{
};

inline static constexpr std::size_t bench_batch_size = 8;
using batch_type = detail::batch_sample_type_t<sample<int24_t>, bench_batch_size>;
static_assert(std::is_same<batch_type, xsimd::batch<ratl::int32_t, bench_batch_size>>::value, "");

template<typename BatchLoader>
static void benchInt24BatchLoad(benchmark::State& state)
{
    static constexpr std::size_t input_size = 1000000;
    std::vector<int24_t> input;
    input.resize(input_size);
    for (auto& in : input)
    {
        in = generateRandom();
    }

    static constexpr std::size_t output_size = (input_size / bench_batch_size) + (input_size % bench_batch_size ? 1 : 0);
    std::vector<batch_type> output;
    output.reserve(output_size);

    for (auto _ : state)
    {
        auto iter = input.begin();
        auto distance_remaining = input.size();
        for (; distance_remaining >= bench_batch_size; iter += bench_batch_size, distance_remaining -= bench_batch_size)
        {
            output.emplace_back(BatchLoader::load(batch_type{}, full_load{}, iter.base(), distance_remaining));
        }
        if (distance_remaining > 0)
        {
            output.emplace_back(BatchLoader::load(batch_type{}, partial_load{}, iter.base(), distance_remaining));
        }

        state.PauseTiming();

        if (output.size() != output_size)
        {
            throw std::runtime_error{"incorrect output size"};
        }
        for (std::size_t i = 0; i < input.size(); ++i)
        {
            if (output[i / bench_batch_size][i % bench_batch_size] != static_cast<std::int32_t>(input[i]))
            {
                throw std::runtime_error{"incorrect conversion"};
            }
        }
        output.clear();

        state.ResumeTiming();
    }
}

struct CurrentBatchLoader
{
    static batch_type load(batch_type, full_load, const int24_t* input, std::size_t) noexcept
    {
        return detail::batch_creator<sample<int24_t>, bench_batch_size>::load(
            reinterpret_cast<const sample<int24_t>*>(input), detail::batch_alignment_mode::unalignable{});
    }

    static batch_type load(batch_type, partial_load, const int24_t* input, std::size_t size) noexcept
    {
        return detail::batch_creator<sample<int24_t>, bench_batch_size>::load(
            reinterpret_cast<const sample<int24_t>*>(input), size, detail::batch_alignment_mode::unalignable{});
    }
};
BENCHMARK_TEMPLATE(benchInt24BatchLoad, CurrentBatchLoader);

struct GatherAlwaysMemcpyBatchLoader
{
#if defined(RATL_CPP_VERSION_HAS_CPP17)
private:
    inline static const __m128i x4_index = _mm_set_epi32(9, 6, 3, 0);
    inline static const __m256i x8_index = _mm256_set_epi32(21, 18, 15, 12, 9, 6, 3, 0);

public:
#endif
    template<typename LoadType>
    static xsimd::batch<ratl::int32_t, 4> load(xsimd::batch<ratl::int32_t, 4>, LoadType, const int24_t* input, std::size_t size) noexcept
    {
        static constexpr std::size_t batch_size = 4;
        static constexpr std::size_t batch_bytes_size = sizeof(ratl::int32_t) * batch_size;
        static constexpr std::size_t int24_bytes_size = sizeof(ratl::int24_t);

#if !defined(RATL_CPP_VERSION_HAS_CPP17)
        static const __m128i x4_index = _mm_set_epi32(9, 6, 3, 0);
#endif

        alignas(batch_bytes_size) std::array<ratl::int32_t, batch_size> temp_array; // NOLINT(cppcoreguidelines-pro-type-member-init)
        std::memcpy(temp_array.data(), input, std::min(size * int24_bytes_size, batch_bytes_size));
        return (xsimd::batch<ratl::int32_t, batch_size>{_mm_i32gather_epi32(temp_array.data(), x4_index, 1)} << 8) >> 8;
    }

    template<typename LoadType>
    static xsimd::batch<ratl::int32_t, 8> load(xsimd::batch<ratl::int32_t, 8>, LoadType, const int24_t* input, std::size_t size) noexcept
    {
        static constexpr std::size_t batch_size = 8;
        static constexpr std::size_t batch_bytes_size = sizeof(ratl::int32_t) * batch_size;
        static constexpr std::size_t int24_bytes_size = sizeof(ratl::int24_t);

#if !defined(RATL_CPP_VERSION_HAS_CPP17)
        static const __m256i x8_index = _mm256_set_epi32(21, 18, 15, 12, 9, 6, 3, 0);
#endif

        alignas(batch_bytes_size) std::array<ratl::int32_t, batch_size> temp_array; // NOLINT(cppcoreguidelines-pro-type-member-init)
        std::memcpy(temp_array.data(), input, std::min(size * int24_bytes_size, batch_bytes_size));
        return (xsimd::batch<ratl::int32_t, batch_size>{_mm256_i32gather_epi32(temp_array.data(), x8_index, 1)} << 8) >> 8;
    }
};
BENCHMARK_TEMPLATE(benchInt24BatchLoad, GatherAlwaysMemcpyBatchLoader);

struct GatherBranchMemcpyBatchLoader
{
#if defined(RATL_CPP_VERSION_HAS_CPP17)
private:
    inline static const __m128i x4_index = _mm_set_epi32(9, 6, 3, 0);
    inline static const __m256i x8_index = _mm256_set_epi32(21, 18, 15, 12, 9, 6, 3, 0);

public:
#endif
    template<typename LoadType>
    static xsimd::batch<ratl::int32_t, 4> load(xsimd::batch<ratl::int32_t, 4>, LoadType, const int24_t* input, std::size_t size) noexcept
    {
        static constexpr std::size_t batch_size = 4;
        static constexpr std::size_t batch_bytes_size = sizeof(ratl::int32_t) * batch_size;
        static constexpr std::size_t int24_bytes_size = sizeof(ratl::int24_t);
        static constexpr std::size_t min_full_load_size = batch_bytes_size / int24_bytes_size + (batch_bytes_size % int24_bytes_size ? 1 : 0);

#if !defined(RATL_CPP_VERSION_HAS_CPP17)
        static const __m128i x4_index = _mm_set_epi32(9, 6, 3, 0);
#endif

        alignas(batch_bytes_size) std::array<ratl::int32_t, batch_size> temp_array; // NOLINT(cppcoreguidelines-pro-type-member-init)
        const int* ptr;
        if (size < min_full_load_size)
        {
            std::memcpy(temp_array.data(), input, size * int24_bytes_size);
            ptr = temp_array.data();
        }
        else
        {
            ptr = reinterpret_cast<const int*>(input);
        }
        return (xsimd::batch<ratl::int32_t, batch_size>{_mm_i32gather_epi32(ptr, x4_index, 1)} << 8) >> 8;
    }

    template<typename LoadType>
    static xsimd::batch<ratl::int32_t, 8> load(xsimd::batch<ratl::int32_t, 8>, LoadType, const int24_t* input, std::size_t size) noexcept
    {
        static constexpr std::size_t batch_size = 8;
        static constexpr std::size_t batch_bytes_size = sizeof(ratl::int32_t) * batch_size;
        static constexpr std::size_t int24_bytes_size = sizeof(ratl::int24_t);
        static constexpr std::size_t min_full_load_size = batch_bytes_size / int24_bytes_size + (batch_bytes_size % int24_bytes_size ? 1 : 0);

#if !defined(RATL_CPP_VERSION_HAS_CPP17)
        static const __m256i x8_index = _mm256_set_epi32(21, 18, 15, 12, 9, 6, 3, 0);
#endif

        alignas(batch_bytes_size) std::array<ratl::int32_t, batch_size> temp_array; // NOLINT(cppcoreguidelines-pro-type-member-init)
        const int* ptr;
        if (size < min_full_load_size)
        {
            std::memcpy(temp_array.data(), input, size * int24_bytes_size);
            ptr = temp_array.data();
        }
        else
        {
            ptr = reinterpret_cast<const int*>(input);
        }
        return (xsimd::batch<ratl::int32_t, batch_size>{_mm256_i32gather_epi32(ptr, x8_index, 1)} << 8) >> 8;
    }
};
BENCHMARK_TEMPLATE(benchInt24BatchLoad, GatherBranchMemcpyBatchLoader);

} // namespace ratl

BENCHMARK_MAIN();
