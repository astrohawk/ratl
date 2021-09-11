/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

// ratl includes
#include "alsa_playback.hpp"

#include <ratl/ratl.hpp>

// other includes
#include <alsa/asoundlib.h>
#include <cstdio>
#include <iostream>
#include <string>

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

static constexpr double Amplitude = 0.5;

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " <channels> <sample_rate>" << std::endl;
        return -1;
    }

    auto device = std::string("default");
    auto channels = static_cast<std::size_t>(std::stoul(argv[1]));
    auto rate = static_cast<std::size_t>(std::stoul(argv[2]));

    ratl::example::alsa::alsa_playback playback(device);

    auto playback_params = playback.construct_hardware_params();
    auto result = playback_params.set_access(SND_PCM_ACCESS_RW_INTERLEAVED);
    if (!result)
    {
        std::cout << "ERROR: Can't set access: " << result.c_str() << std::endl;
    }
    result = playback_params.set_format(SND_PCM_FORMAT_S16_LE);
    if (!result)
    {
        std::cout << "ERROR: Can't set format: " << result.c_str() << std::endl;
    }
    result = playback_params.set_channels(channels);
    if (!result)
    {
        std::cout << "ERROR: Can't set channels: " << result.c_str() << std::endl;
    }
    result = playback_params.set_rate(rate);
    if (!result)
    {
        std::cout << "ERROR: Can't set rate: " << result.c_str() << std::endl;
    }

    result = playback.write_hardware_params(playback_params);
    if (!result)
    {
        std::cout << "ERROR: Can't write hardware params: " << result.c_str() << std::endl;
    }

    auto period_size_result = playback_params.get_period_size();
    if (!period_size_result)
    {
        std::cout << "ERROR: Can't get period size: " << period_size_result.c_str() << std::endl;
    }
    auto frames = *period_size_result;

    ratl::interleaved<ratl::float32_t> float_interleaved(channels, frames);
    ratl::interleaved<ratl::int16_t> int_interleaved(channels, frames);

    auto wave_position = static_cast<std::size_t>(0);
    auto wave_frames = static_cast<std::size_t>(rate / 1000);
    ratl::dither_generator dither_gen;
    result = playback.write_interleaved_loop(
        [&]() -> const ratl::interleaved<ratl::int16_t>&
        {
            for (auto frame : float_interleaved)
            {
                auto sine_sample = ratl::sample<ratl::float32_t>(static_cast<float>(
                    sine(static_cast<double>(wave_position) / static_cast<double>(wave_frames)) * Amplitude));
                for (auto& sample : frame)
                {
                    sample = sine_sample;
                }
                wave_position = (wave_position + 1) % wave_frames;
            }
            ratl::transform(float_interleaved.begin(), float_interleaved.end(), int_interleaved.begin(), dither_gen);
            return int_interleaved;
        });
    if (!result)
    {
        std::cout << "ERROR: write interleaved failed: " << result.c_str() << std::endl;
    }

    return 0;
}