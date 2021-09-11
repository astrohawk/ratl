/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

// ratl includes
#include "alsa_playback.hpp"

// other includes
#include <stdexcept>

namespace ratl
{
namespace example
{
namespace alsa
{
result::operator bool() const
{
    return result_ == 0;
}

const char* result::c_str() const
{
    return result_ != 0 ? snd_strerror(result_) : nullptr;
}

optional_result<snd_pcm_access_t> alsa_playback::hardware_params::get_access()
{
    snd_pcm_access_t access;
    auto res = snd_pcm_hw_params_get_access(pcm_hw_params_.get(), &access);
    if (res < 0)
    {
        return {optional_result<snd_pcm_access_t>::fail{}, res};
    }
    return {optional_result<snd_pcm_access_t>::success{}, access};
}

result alsa_playback::hardware_params::set_access(snd_pcm_access_t access)
{
    return result(snd_pcm_hw_params_set_access(&pcm_handle_.get(), pcm_hw_params_.get(), access));
}

optional_result<snd_pcm_format_t> alsa_playback::hardware_params::get_format()
{
    snd_pcm_format_t format;
    auto res = snd_pcm_hw_params_get_format(pcm_hw_params_.get(), &format);
    if (res < 0)
    {
        return {optional_result<snd_pcm_format_t>::fail{}, res};
    }
    return {optional_result<snd_pcm_format_t>::success{}, format};
}

result alsa_playback::hardware_params::set_format(snd_pcm_format_t format)
{
    return result(snd_pcm_hw_params_set_format(&pcm_handle_.get(), pcm_hw_params_.get(), format));
}

optional_result<std::size_t> alsa_playback::hardware_params::get_channels()
{
    unsigned int channels;
    auto res = snd_pcm_hw_params_get_channels(pcm_hw_params_.get(), &channels);
    if (res < 0)
    {
        return {optional_result<std::size_t>::fail{}, res};
    }
    return {optional_result<std::size_t>::success{}, static_cast<std::size_t>(channels)};
}

result alsa_playback::hardware_params::set_channels(std::size_t channels)
{
    return result(
        snd_pcm_hw_params_set_channels(&pcm_handle_.get(), pcm_hw_params_.get(), static_cast<unsigned int>(channels)));
}

optional_result<std::size_t> alsa_playback::hardware_params::get_rate()
{
    unsigned int rate;
    auto res = snd_pcm_hw_params_get_rate(pcm_hw_params_.get(), &rate, nullptr);
    if (res < 0)
    {
        return {optional_result<std::size_t>::fail{}, res};
    }
    return {optional_result<std::size_t>::success{}, static_cast<std::size_t>(rate)};
}

result alsa_playback::hardware_params::set_rate(std::size_t rate)
{
    return result(snd_pcm_hw_params_set_rate(&pcm_handle_.get(), pcm_hw_params_.get(), rate, 0));
}

optional_result<std::size_t> alsa_playback::hardware_params::get_period_time()
{
    unsigned int period_time;
    auto res = snd_pcm_hw_params_get_period_time(pcm_hw_params_.get(), &period_time, nullptr);
    if (res < 0)
    {
        return {optional_result<std::size_t>::fail{}, res};
    }
    return {optional_result<std::size_t>::success{}, static_cast<std::size_t>(period_time)};
}

result alsa_playback::hardware_params::set_period_time(std::size_t period_time)
{
    return result(snd_pcm_hw_params_set_period_time(
        &pcm_handle_.get(), pcm_hw_params_.get(), static_cast<unsigned int>(period_time), 0));
}

optional_result<std::size_t> alsa_playback::hardware_params::get_period_size()
{
    snd_pcm_uframes_t period_size;
    auto res = snd_pcm_hw_params_get_period_size(pcm_hw_params_.get(), &period_size, nullptr);
    if (res < 0)
    {
        return {optional_result<std::size_t>::fail{}, res};
    }
    return {optional_result<std::size_t>::success{}, static_cast<std::size_t>(period_size)};
}

result alsa_playback::hardware_params::set_period_size(std::size_t period_size)
{
    return result(snd_pcm_hw_params_set_period_size(
        &pcm_handle_.get(), pcm_hw_params_.get(), static_cast<snd_pcm_uframes_t>(period_size), 0));
}

alsa_playback::hardware_params::hardware_params(pcm_ptr& pcm) :
    pcm_handle_(*pcm), pcm_hw_params_(make_hardware_params_ptr(pcm_handle_))
{
}

snd_pcm_hw_params_t* alsa_playback::hardware_params::get()
{
    return pcm_hw_params_.get();
}

alsa_playback::hardware_params::pcm_hw_params_ptr alsa_playback::hardware_params::make_hardware_params_ptr(
    std::reference_wrapper<snd_pcm_t> pcm)
{
    snd_pcm_hw_params_t* pcm_hw_params;
    auto result = snd_pcm_hw_params_malloc(&pcm_hw_params);
    if (result < 0)
    {
        throw std::runtime_error(std::string() + "Can't malloc hw params: " + snd_strerror(result));
    }
    result = snd_pcm_hw_params_any(&pcm.get(), pcm_hw_params);
    if (result < 0)
    {
        throw std::runtime_error(
            std::string() + "Unable to fill hw params with full configuration space: " + snd_strerror(result));
    }
    return {
        pcm_hw_params,
        [](snd_pcm_hw_params_t* pcm_hw_params)
        {
            snd_pcm_hw_params_free(pcm_hw_params);
        }};
}

alsa_playback::alsa_playback(const std::string& device_name) : pcm_handle_(make_pcm_ptr(device_name)) {}

alsa_playback::hardware_params alsa_playback::construct_hardware_params()
{
    return hardware_params(pcm_handle_);
}

result alsa_playback::write_hardware_params(hardware_params& params)
{
    return result(snd_pcm_hw_params(pcm_handle_.get(), params.get()));
}

alsa_playback::pcm_ptr alsa_playback::make_pcm_ptr(const std::string& device_name)
{
    snd_pcm_t* pcm;
    auto result = snd_pcm_open(&pcm, device_name.c_str(), SND_PCM_STREAM_PLAYBACK, 0);
    if (result < 0)
    {
        throw std::runtime_error(std::string() + "Can't open " + device_name + " PCM device: " + snd_strerror(result));
    }
    return {
        pcm,
        [](snd_pcm_t* pcm)
        {
            snd_pcm_drain(pcm);
            snd_pcm_close(pcm);
        }};
}

} // namespace alsa
} // namespace example
} // namespace ratl