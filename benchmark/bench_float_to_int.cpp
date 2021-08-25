#include <benchmark/benchmark.h>
#include <random>
#include <ratl/ratl.hpp>

#if (defined(RATL_CPP_ARCH_X86) || defined(RATL_CPP_ARCH_X86_64)) && defined(__SSE__)
#    include <ratl/detail/intrin.hpp>
#endif
#if defined(RATL_CPP_ARCH_AARCH64) && defined(__ARM_NEON)
#    include <ratl/detail/intrin.hpp>
#endif

namespace ratl
{
static float32_t generateRandom()
{
    static class
    {
    public:
        float32_t generate()
        {
            return distribution_(random_generator_);
        }

    private:
        std::random_device random_device_;
        std::mt19937 random_generator_{random_device_()};
        std::uniform_real_distribution<float32_t> distribution_{
            sample_limits<float32_t>::min, sample_limits<float32_t>::max};
    } random_state;
    return random_state.generate();
}

std::array<float32_t, 1000> generateRandomFloatArray()
{
    std::array<float32_t, 1000> array;
    for (auto& element : array)
    {
        element = generateRandom();
    }
    return array;
}

template<typename Converter>
static void benchFloatToInt(benchmark::State& state)
{
    auto input = generateRandomFloatArray();
    auto output = std::array<int32_t, 1000>{};
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(std::transform(
            input.begin(),
            input.end(),
            output.begin(),
            [](float32_t sample)
            {
                if (RATL_UNLIKELY (sample >= sample_limits<float32_t>::max))
                {
                    return sample_limits<int32_t>::max;
                }
                if (RATL_UNLIKELY (sample < sample_limits<float32_t>::min))
                {
                    return sample_limits<int32_t>::min;
                }
                return Converter::template convert<detail::float_convert_traits<int32_t>>(sample);
            }));
    }
}

struct PositiveAsymmetricFloatConvertTraits
{
    static constexpr float32_t multiplier = static_cast<float32_t>(sample_limits<int32_t>::max);
};
struct NegativeAsymmetricFloatConvertTraits
{
    static constexpr float32_t multiplier = -static_cast<float32_t>(sample_limits<int32_t>::min);
};

template<typename Converter>
static void benchAsymmetricFloatToInt(benchmark::State& state)
{
    auto input = generateRandomFloatArray();
    auto output = std::array<int32_t, 1000>{};
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(std::transform(
            input.begin(),
            input.end(),
            output.begin(),
            [](float32_t sample)
            {
                return sample < 0 ? Converter::template convert<NegativeAsymmetricFloatConvertTraits>(sample)
                                  : Converter::template convert<PositiveAsymmetricFloatConvertTraits>(sample);
            }));
    }
}

struct LrintConverter
{
    template<typename float_convert_traits>
    static int32_t convert(float32_t sample) noexcept
    {
        return static_cast<int32_t>(std::lrint(sample * float_convert_traits::multiplier));
    }
};
BENCHMARK_TEMPLATE(benchFloatToInt, LrintConverter);
BENCHMARK_TEMPLATE(benchAsymmetricFloatToInt, LrintConverter);

#if defined(RATL_CPP_COMPILER_CLANG) || defined(RATL_CPP_COMPILER_GCC)
struct BuiltinLrintConverter
{
    template<typename float_convert_traits>
    static int32_t convert(float32_t sample) noexcept
    {
        return static_cast<int32_t>(__builtin_lrint(sample * float_convert_traits::multiplier));
    }
};
BENCHMARK_TEMPLATE(benchFloatToInt, BuiltinLrintConverter);
BENCHMARK_TEMPLATE(benchAsymmetricFloatToInt, BuiltinLrintConverter);
#endif

#if (defined(RATL_CPP_ARCH_X86) || defined(RATL_CPP_ARCH_X86_64)) && defined(__SSE__)
struct SseIntrinsicsConverter
{
    template<typename float_convert_traits>
    static int32_t convert(float32_t sample) noexcept
    {
        return _mm_cvt_ss2si(_mm_set_ss((sample * float_convert_traits::multiplier)));
    }
};
BENCHMARK_TEMPLATE(benchFloatToInt, SseIntrinsicsConverter);
BENCHMARK_TEMPLATE(benchAsymmetricFloatToInt, SseIntrinsicsConverter);
#endif

#if defined(RATL_CPP_ARCH_AARCH64) && defined(__ARM_NEON)
struct NeonIntrinsicsConverter
{
    template<typename float_convert_traits>
    static int32_t convert(float32_t sample) noexcept
    {
        return vgetq_lane_s32(vcvtnq_s32_f32(vdupq_n_f32(sample * float_convert_traits::multiplier)), 0);
    }
};
BENCHMARK_TEMPLATE(benchFloatToInt, NeonIntrinsicsConverter);
BENCHMARK_TEMPLATE(benchAsymmetricFloatToInt, NeonIntrinsicsConverter);
#endif

struct MagicConverter
{
    template<typename float_convert_traits>
    static int32_t convert(float32_t sample) noexcept
    {
        return ratl::detail::round_float32_to_int32_magic(sample * float_convert_traits::multiplier);
    }
};
BENCHMARK_TEMPLATE(benchFloatToInt, MagicConverter);
BENCHMARK_TEMPLATE(benchAsymmetricFloatToInt, MagicConverter);

} // namespace ratl

BENCHMARK_MAIN();
