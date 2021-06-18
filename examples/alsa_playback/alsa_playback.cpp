/*
 * Simple sound playback using ALSA API and libasound.
 *
 * Compile:
 * $ cc -o play sound_playback.c -lasound
 *
 * Usage:
 * $ ./play <sample_rate> <channels> <seconds> < <file>
 *
 * Examples:
 * $ ./play 44100 2 5 < /dev/urandom
 * $ ./play 22050 1 8 < /path/to/file.wav
 *
 * Copyright (C) 2009 Alessandro Ghedini <al3xbio@gmail.com>
 * --------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Alessandro Ghedini wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we
 * meet some day, and you think this stuff is worth it, you can
 * buy me a beer in return.
 * --------------------------------------------------------------
 */

#include <alsa/asoundlib.h>
#include <cstdio>
#include <ratl/ratl.hpp>
#include <string>

#define PCM_DEVICE "default"

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
    if (argc < 4)
    {
        printf("Usage: %s <sample_rate> <channels> <seconds>\n", argv[0]);
        return -1;
    }

    auto rate = static_cast<unsigned int>(std::stoul(argv[1]));
    auto channels = static_cast<unsigned int>(std::stoul(argv[2]));
    auto seconds = static_cast<unsigned int>(std::stoul(argv[3]));

    /* Open the PCM device in playback mode */
    snd_pcm_t* pcm_handle;
    auto open_result = snd_pcm_open(&pcm_handle, PCM_DEVICE, SND_PCM_STREAM_PLAYBACK, 0);
    if (open_result < 0)
    {
        printf("ERROR: Can't open \"%s\" PCM device. %s\n", PCM_DEVICE, snd_strerror(open_result));
    }

    /* Allocate parameters object and fill it with default values*/
    snd_pcm_hw_params_t* params;
    snd_pcm_hw_params_alloca(&params);

    snd_pcm_hw_params_any(pcm_handle, params);

    /* Set parameters */
    auto access_result = snd_pcm_hw_params_set_access(pcm_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    if (access_result < 0)
    {
        printf("ERROR: Can't set interleaved mode. %s\n", snd_strerror(access_result));
    }

    auto format_result = snd_pcm_hw_params_set_format(pcm_handle, params, SND_PCM_FORMAT_S16_LE);
    if (format_result < 0)
    {
        printf("ERROR: Can't set format. %s\n", snd_strerror(format_result));
    }

    auto channels_result = snd_pcm_hw_params_set_channels(pcm_handle, params, channels);
    if (channels_result < 0)
    {
        printf("ERROR: Can't set channels number. %s\n", snd_strerror(channels_result));
    }

    auto rate_result = snd_pcm_hw_params_set_rate_near(pcm_handle, params, &rate, nullptr);
    if (rate_result < 0)
    {
        printf("ERROR: Can't set rate. %s\n", snd_strerror(rate_result));
    }

    /* Write parameters */
    auto params_result = snd_pcm_hw_params(pcm_handle, params);
    if (params_result < 0)
    {
        printf("ERROR: Can't set harware parameters. %s\n", snd_strerror(params_result));
    }

    /* Resume information */
    printf("PCM name: '%s'\n", snd_pcm_name(pcm_handle));
    printf("PCM state: %s\n", snd_pcm_state_name(snd_pcm_state(pcm_handle)));

    unsigned int tmp_channels;
    snd_pcm_hw_params_get_channels(params, &tmp_channels);
    printf("channels: %i ", tmp_channels);
    if (tmp_channels == 1)
    {
        printf("(mono)\n");
    }
    else if (tmp_channels == 2)
    {
        printf("(stereo)\n");
    }

    unsigned int tmp_rate;
    snd_pcm_hw_params_get_rate(params, &tmp_rate, nullptr);
    printf("rate: %d bps\n", tmp_rate);

    printf("seconds: %d\n", seconds);

    /* Allocate buffer to hold single period */
    snd_pcm_uframes_t frames;
    snd_pcm_hw_params_get_period_size(params, &frames, nullptr);

    ratl::Interleaved<ratl::float32_t> float_interleaved{tmp_channels, frames};
    ratl::Interleaved<ratl::int16_t> int_interleaved{tmp_channels, frames};

    unsigned int tmp_period;
    snd_pcm_hw_params_get_period_time(params, &tmp_period, nullptr);

    std::size_t num_loops = (seconds * 1000000) / tmp_period;
    std::size_t wave_position = 0;
    std::size_t wave_frames = tmp_rate / 1000;
    ratl::DitherGenerator dither_generator;
    for (std::size_t i = 0; i < num_loops; ++i)
    {
        for (auto frame : float_interleaved)
        {
            auto sine_sample = ratl::Sample<ratl::float32_t>{static_cast<float>(
                sine(static_cast<double>(wave_position) / static_cast<double>(wave_frames)) * Amplitude)};
            for (auto& sample : frame)
            {
                sample = sine_sample;
            }
            wave_position = (wave_position + 1) % wave_frames;
        }

        ratl::transform(float_interleaved.begin(), float_interleaved.end(), int_interleaved.begin(), dither_generator);

        auto writei_result = snd_pcm_writei(pcm_handle, int_interleaved.data(), int_interleaved.frames());
        if (writei_result == -EPIPE)
        {
            printf("XRUN.\n");
            snd_pcm_prepare(pcm_handle);
        }
        else if (writei_result < 0)
        {
            printf("ERROR. Can't write to PCM device. %s\n", snd_strerror(writei_result));
        }
    }

    snd_pcm_drain(pcm_handle);
    snd_pcm_close(pcm_handle);

    return 0;
}