/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_example_alsa_playback_
#define _ratl_example_alsa_playback_

// other includes
#include <alsa/asoundlib.h>
#include <memory>
#include <string>
#include <utility>

namespace ratl
{
namespace example
{
namespace alsa
{
class result
{
    int result_{};

public:
    result() = default;

    explicit result(int result) : result_(result) {}

    operator bool() const;

    const char* c_str() const;
};

template<typename T>
class optional_result
{
    T value_{};
    result result_{};

public:
    struct success
    {
    };
    struct fail
    {
    };

    template<typename ArgT>
    optional_result(success, ArgT&& value) : value_(std::forward<ArgT>(value))
    {
    }

    optional_result(fail, int result) : result_(result) {}

    operator bool() const
    {
        return result_;
    }

    const T* operator->() const
    {
        return &value_;
    }

    T* operator->()
    {
        return &value_;
    }

    const T& operator*() const
    {
        return value_;
    }

    T& operator*()
    {
        return value_;
    }

    const char* c_str() const
    {
        return !result_ ? result_.c_str() : nullptr;
    }
};

class alsa_playback
{
    using pcm_ptr = std::unique_ptr<snd_pcm_t, void (*)(snd_pcm_t*)>;

    pcm_ptr pcm_handle_;

public:
    class hardware_params
    {
        using pcm_hw_params_ptr = std::unique_ptr<snd_pcm_hw_params_t, void (*)(snd_pcm_hw_params_t*)>;

        std::reference_wrapper<snd_pcm_t> pcm_handle_;
        pcm_hw_params_ptr pcm_hw_params_;

    public:
        optional_result<snd_pcm_access_t> get_access();
        result set_access(snd_pcm_access_t access);

        optional_result<snd_pcm_format_t> get_format();
        result set_format(snd_pcm_format_t format);

        optional_result<std::size_t> get_channels();
        result set_channels(std::size_t channels);

        optional_result<std::size_t> get_rate();
        result set_rate(std::size_t rate);

        optional_result<std::size_t> get_period_time();
        result set_period_time(std::size_t period_time);

        optional_result<std::size_t> get_period_size();
        result set_period_size(std::size_t period_size);

    private:
        friend alsa_playback;
        explicit hardware_params(pcm_ptr& pcm);

        snd_pcm_hw_params_t* get();

        static pcm_hw_params_ptr make_hardware_params_ptr(std::reference_wrapper<snd_pcm_t> pcm);
    };

    explicit alsa_playback(const std::string& device_name);

    hardware_params construct_hardware_params();
    result write_hardware_params(hardware_params& params);

    template<typename Fn>
    result write_interleaved_loop(Fn write_function)
    {
        while (true)
        {
            const auto& interleaved = write_function();
            auto res = snd_pcm_writei(
                pcm_handle_.get(), interleaved.data(), static_cast<snd_pcm_uframes_t>(interleaved.frames()));
            if (res == -EPIPE)
            {
                snd_pcm_prepare(pcm_handle_.get());
            }
            else if (res < 0)
            {
                return result(res);
            }
        }
    }

private:
    static pcm_ptr make_pcm_ptr(const std::string& device_name);
};

} // namespace alsa
} // namespace example
} // namespace ratl

#endif // _ratl_example_alsa_playback_
