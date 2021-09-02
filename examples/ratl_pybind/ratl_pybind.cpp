/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

// ratl includes
#include <ratl/ratl.hpp>

// other includes
#include <cmath>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#if !defined(M_PI)
#    define M_PI 3.14159265358979323846
#endif

namespace py = pybind11;

double sine(double position)
{
    return std::sin(2 * M_PI * position);
}

double square(double position)
{
    return std::fmod(position, 1.0) >= 0.5 ? 1.0 : -1.0;
}

double triangle(double position)
{
    return 1 - std::fabs(2 * ((2 * std::fmod(position, 1.0)) - 1));
}

double sawtooth(double position)
{
    return 2 * (std::fmod(position, 1.0) - 1);
}

static constexpr std::size_t frame_size = 65536;
static constexpr std::size_t sample_rate = 44100;
static constexpr std::size_t frequency = 1000;
static constexpr double amplitude = 0.8;

PYBIND11_MODULE(ratl_pybind, m)
{
    static constexpr auto tone = sine;

    m.def(
        "triangular_pdf",
        []()
        {
            ratl::detail::triangular_dither_generator gen;
            std::vector<double> samples(1000000);
            for (auto& sample : samples)
            {
                sample = gen.generate_float32();
            }
            return samples;
        });
    m.def(
        "gaussian_pdf",
        []()
        {
            ratl::detail::shaped_dither_generator gen;
            std::vector<double> samples(1000000);
            for (auto& sample : samples)
            {
                sample = gen.generate_float32();
            }
            return samples;
        });
    m.def(
        "samplerate",
        []()
        {
            return sample_rate;
        });
    m.def(
        "num_samples",
        []()
        {
            return frame_size;
        });
    m.def(
        "reference_double_tone",
        []()
        {
            std::vector<double> samples(frame_size);
            double samples_per_cycle = static_cast<double>(sample_rate) / static_cast<double>(frequency);
            for (std::size_t i = 0; i < samples.size(); ++i)
            {
                samples[i] = amplitude * tone(static_cast<double>(i) / samples_per_cycle);
            }

            return samples;
        });
    m.def(
        "reference_int16_tone",
        []()
        {
            ratl::interleaved<ratl::float32_t> input(1, frame_size);
            double samples_per_cycle = static_cast<double>(sample_rate) / static_cast<double>(frequency);
            for (std::size_t i = 0; i < input.frames(); ++i)
            {
                input.channel(0)[i].get() =
                    static_cast<float>(amplitude * tone(static_cast<double>(i) / samples_per_cycle));
            }

            ratl::interleaved<ratl::int32_t> temp1(1, frame_size);
            ratl::transform(input.begin(), input.end(), temp1.begin());

            ratl::interleaved<ratl::int16_t> temp2(1, frame_size);
            ratl::transform(temp1.begin(), temp1.end(), temp2.begin());

            ratl::interleaved<ratl::float32_t> output(1, frame_size);
            ratl::transform(temp2.begin(), temp2.end(), output.begin());

            std::vector<float> samples(frame_size);
            for (std::size_t i = 0; i < samples.size(); ++i)
            {
                samples[i] = output.channel(0)[i].get();
            }
            return samples;
        });
    m.def(
        "dither_int16_tone",
        []()
        {
            ratl::dither_generator dither_gen;

            ratl::interleaved<ratl::float32_t> input(1, frame_size);
            double samples_per_cycle = static_cast<double>(sample_rate) / static_cast<double>(frequency);
            for (std::size_t i = 0; i < input.frames(); ++i)
            {
                input.channel(0)[i].get() =
                    static_cast<float>(amplitude * tone(static_cast<double>(i) / samples_per_cycle));
            }

            ratl::interleaved<ratl::int32_t> temp1(1, frame_size);
            ratl::transform(input.begin(), input.end(), temp1.begin(), dither_gen);

            ratl::interleaved<ratl::int16_t> temp2(1, frame_size);
            ratl::transform(temp1.begin(), temp1.end(), temp2.begin(), dither_gen);

            ratl::interleaved<ratl::float32_t> output(1, frame_size);
            ratl::transform(temp2.begin(), temp2.end(), output.begin(), dither_gen);

            std::vector<float> samples(frame_size);
            for (std::size_t i = 0; i < samples.size(); ++i)
            {
                samples[i] = output.channel(0)[i].get();
            }
            return samples;
        });
}