/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

// ratl includes
#include "bench_utils.hpp"

// other includes
#include <pa_converters.h>
#include <pa_dither.h>

namespace ratl
{
template<typename Transformer, typename InputSampleType, typename OutputSampleType>
void benchTransform(benchmark::State& state)
{
    using input_type = basic_interleaved<InputSampleType>;
    using output_type = basic_noninterleaved<OutputSampleType>;

    static constexpr std::size_t num_channels = 32;
    static constexpr std::size_t num_frames = 480;

    auto input = utils::generateRandomInput<input_type>(num_channels, num_frames);
    auto output = output_type(num_channels, num_frames);

    for (auto _ : state)
    {
        Transformer::transform(input, output);
    }
}

struct RatlTransformer
{
    template<typename InputType, typename OutputType>
    static void transform(InputType& input, OutputType& output)
    {
        ratl::transform(input.begin(), input.end(), output.begin());
    }
};

template<typename SampleType>
struct PortAudioSampleFormat;

template<>
struct PortAudioSampleFormat<int16_t> : std::integral_constant<PaSampleFormat, paInt16>
{
};

template<>
struct PortAudioSampleFormat<int24_t> : std::integral_constant<PaSampleFormat, paInt24>
{
};

template<>
struct PortAudioSampleFormat<int32_t> : std::integral_constant<PaSampleFormat, paInt32>
{
};

template<>
struct PortAudioSampleFormat<float32_t> : std::integral_constant<PaSampleFormat, paFloat32>
{
};

struct PortAudioTransformer
{
private:
    template<typename InputSampleType, typename OutputSampleType>
    static inline PaUtilConverter& select_converter()
    {
        PaStreamFlags flags = paNoFlag;
        flags |= paClipOff;
        flags |= paDitherOff;
        auto converter = PaUtil_SelectConverter(
            PortAudioSampleFormat<detail::sample_underlying_type_t<InputSampleType>>::value,
            PortAudioSampleFormat<detail::sample_underlying_type_t<OutputSampleType>>::value,
            flags);
        if (converter == nullptr)
        {
            throw std::runtime_error{"unable to find PortAudio converter"};
        }
        return *converter;
    }

public:
    template<typename InputSampleType, typename OutputSampleType>
    static void transform(basic_interleaved<InputSampleType>& input, basic_interleaved<OutputSampleType>& output)
    {
        PaUtilTriangularDitherGenerator dither_gen;
        PaUtil_InitializeTriangularDitherState(&dither_gen);

        auto& converter = select_converter<InputSampleType, OutputSampleType>();
        converter(
            (void*)output.data(),
            (signed int)1,
            (void*)input.data(),
            (signed int)1,
            (unsigned int)input.samples(),
            &dither_gen);
    }

    template<typename InputSampleType, typename OutputSampleType>
    static void transform(basic_noninterleaved<InputSampleType>& input, basic_noninterleaved<OutputSampleType>& output)
    {
        PaUtilTriangularDitherGenerator dither_gen;
        PaUtil_InitializeTriangularDitherState(&dither_gen);

        auto& converter = select_converter<InputSampleType, OutputSampleType>();
        converter(
            (void*)output.data(),
            (signed int)1,
            (void*)input.data(),
            (signed int)1,
            (unsigned int)input.samples(),
            &dither_gen);
    }

    template<typename InputSampleType, typename OutputSampleType>
    static void transform(basic_interleaved<InputSampleType>& input, basic_noninterleaved<OutputSampleType>& output)
    {
        PaUtilTriangularDitherGenerator dither_gen;
        PaUtil_InitializeTriangularDitherState(&dither_gen);

        auto& converter = select_converter<InputSampleType, OutputSampleType>();
        for (std::size_t channel = 0; channel < input.channels(); ++channel)
        {
            converter(
                (void*)output.channel(channel).data(),
                (signed int)1,
                (void*)input.channel(channel).data(),
                (signed int)input.channels(),
                (unsigned int)input.frames(),
                &dither_gen);
        }
    }

    template<typename InputSampleType, typename OutputSampleType>
    static void transform(basic_noninterleaved<InputSampleType>& input, basic_interleaved<OutputSampleType>& output)
    {
        PaUtilTriangularDitherGenerator dither_gen;
        PaUtil_InitializeTriangularDitherState(&dither_gen);

        auto& converter = select_converter<InputSampleType, OutputSampleType>();
        for (std::size_t channel = 0; channel < input.channels(); ++channel)
        {
            converter(
                (void*)output.channel(channel).data(),
                (signed int)output.channels(),
                (void*)input.channel(channel).data(),
                (signed int)1,
                (unsigned int)input.frames(),
                &dither_gen);
        }
    }
};

BENCHMARK_TEMPLATE(benchTransform, RatlTransformer, sample<int16_t>, sample<int16_t>);
BENCHMARK_TEMPLATE(benchTransform, PortAudioTransformer, sample<int16_t>, sample<int16_t>);
BENCHMARK_TEMPLATE(benchTransform, RatlTransformer, sample<int16_t>, sample<int24_t>);
BENCHMARK_TEMPLATE(benchTransform, PortAudioTransformer, sample<int16_t>, sample<int24_t>);
BENCHMARK_TEMPLATE(benchTransform, RatlTransformer, sample<int16_t>, sample<int32_t>);
BENCHMARK_TEMPLATE(benchTransform, PortAudioTransformer, sample<int16_t>, sample<int32_t>);
BENCHMARK_TEMPLATE(benchTransform, RatlTransformer, sample<int16_t>, sample<float32_t>);
BENCHMARK_TEMPLATE(benchTransform, PortAudioTransformer, sample<int16_t>, sample<float32_t>);
BENCHMARK_TEMPLATE(benchTransform, RatlTransformer, sample<int24_t>, sample<int16_t>);
BENCHMARK_TEMPLATE(benchTransform, PortAudioTransformer, sample<int24_t>, sample<int16_t>);
BENCHMARK_TEMPLATE(benchTransform, RatlTransformer, sample<int24_t>, sample<int24_t>);
BENCHMARK_TEMPLATE(benchTransform, PortAudioTransformer, sample<int24_t>, sample<int24_t>);
BENCHMARK_TEMPLATE(benchTransform, RatlTransformer, sample<int24_t>, sample<int32_t>);
BENCHMARK_TEMPLATE(benchTransform, PortAudioTransformer, sample<int24_t>, sample<int32_t>);
BENCHMARK_TEMPLATE(benchTransform, RatlTransformer, sample<int24_t>, sample<float32_t>);
BENCHMARK_TEMPLATE(benchTransform, PortAudioTransformer, sample<int24_t>, sample<float32_t>);
BENCHMARK_TEMPLATE(benchTransform, RatlTransformer, sample<int32_t>, sample<int16_t>);
BENCHMARK_TEMPLATE(benchTransform, PortAudioTransformer, sample<int32_t>, sample<int16_t>);
BENCHMARK_TEMPLATE(benchTransform, RatlTransformer, sample<int32_t>, sample<int24_t>);
BENCHMARK_TEMPLATE(benchTransform, PortAudioTransformer, sample<int32_t>, sample<int24_t>);
BENCHMARK_TEMPLATE(benchTransform, RatlTransformer, sample<int32_t>, sample<int32_t>);
BENCHMARK_TEMPLATE(benchTransform, PortAudioTransformer, sample<int32_t>, sample<int32_t>);
BENCHMARK_TEMPLATE(benchTransform, RatlTransformer, sample<int32_t>, sample<float32_t>);
BENCHMARK_TEMPLATE(benchTransform, PortAudioTransformer, sample<int32_t>, sample<float32_t>);
BENCHMARK_TEMPLATE(benchTransform, RatlTransformer, sample<float32_t>, sample<int16_t>);
BENCHMARK_TEMPLATE(benchTransform, PortAudioTransformer, sample<float32_t>, sample<int16_t>);
BENCHMARK_TEMPLATE(benchTransform, RatlTransformer, sample<float32_t>, sample<int24_t>);
BENCHMARK_TEMPLATE(benchTransform, PortAudioTransformer, sample<float32_t>, sample<int24_t>);
BENCHMARK_TEMPLATE(benchTransform, RatlTransformer, sample<float32_t>, sample<int32_t>);
BENCHMARK_TEMPLATE(benchTransform, PortAudioTransformer, sample<float32_t>, sample<int32_t>);
BENCHMARK_TEMPLATE(benchTransform, RatlTransformer, sample<float32_t>, sample<float32_t>);
BENCHMARK_TEMPLATE(benchTransform, PortAudioTransformer, sample<float32_t>, sample<float32_t>);

} // namespace ratl

BENCHMARK_MAIN();
