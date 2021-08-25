/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_dither_generator_
#define _ratl_detail_dither_generator_

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/convert_traits.hpp>
#include <ratl/detail/rand.hpp>

// other includes
#include <cstddef>
#include <cstdint>

namespace ratl
{
namespace detail
{
class null_dither_generator
{
public:
    static constexpr std::size_t int16_bits = 0;
    static constexpr float32_t float32_max = 0;

    inline constexpr int32_t generate_int16() noexcept
    {
        return 0;
    }

    inline constexpr float32_t generate_float32() noexcept
    {
        return 0;
    }
};

class triangular_dither_generator
{
public:
    static constexpr std::size_t int16_bits = 15;
    static constexpr float32_t float32_max = 1.0;

    inline constexpr int32_t generate_int16() noexcept
    {
        return (static_cast<int32_t>(rng_()) >> int16_shift) + (static_cast<int32_t>(rng_()) >> int16_shift);
    }

    inline constexpr float32_t generate_float32() noexcept
    {
        auto current =
            (static_cast<int32_t>(rng_()) >> float32_shift) + (static_cast<int32_t>(rng_()) >> float32_shift);
        return static_cast<float32_t>(current) * float32_scaler;
    }

private:
    static constexpr uint32_t default_seed = 0x1942da21;
    static constexpr std::size_t int16_shift = (32 - int16_bits);
    static constexpr std::size_t float32_shift = 1;
    static constexpr float32_t float32_scaler = detail::float_convert_traits<int32_t>::divisor;

    detail::linear_congruential_generator rng_{default_seed};
};

class shaped_dither_generator
{
public:
    static constexpr std::size_t int16_bits = 15;
    static constexpr float32_t float32_max = 1.0;

    inline constexpr int32_t generate_int16() noexcept
    {
        return generate_high_pass() >> int16_shift;
    }

    inline constexpr float32_t generate_float32() noexcept
    {
        return static_cast<float32_t>(generate_high_pass()) * float32_scaler;
    }

private:
    inline constexpr int32_t generate_high_pass() noexcept
    {
        auto current =
            (static_cast<int32_t>(rng_()) >> initial_shift) + (static_cast<int32_t>(rng_()) >> initial_shift);
        auto high_pass = current - previous_;
        previous_ = current;
        return high_pass;
    }

    static constexpr uint32_t default_seed = 0xac9ad704;
    static constexpr std::size_t initial_shift = 2;
    static constexpr std::size_t int16_shift = 32 - int16_bits - (initial_shift - 1);
    static constexpr float32_t float32_scaler = detail::float_convert_traits<int32_t>::divisor;

    detail::linear_congruential_generator rng_{default_seed};
    int32_t previous_{};
};

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_dither_generator_
