/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_batch_dither_generator_
#define _ratl_detail_batch_dither_generator_

// ratl includes
#include <ratl/detail/batch_rand.hpp>
#include <ratl/detail/batch_value_traits.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/detail/convert_traits.hpp>
#include <ratl/detail/dither_generator.hpp>

// other includes
#include <cstddef>
#include <cstdint>

namespace ratl
{
namespace detail
{
#if defined(RATL_HAS_XSIMD)

class batch_null_dither_generator : public null_dither_generator
{
public:
    inline detail::batch_sample_value_type_t<int32_t> generate_batch_int16() noexcept
    {
        return detail::batch_sample_value_type_t<int32_t>(0);
    }

    inline detail::batch_sample_value_type_t<float32_t> generate_batch_float32() noexcept
    {
        return detail::batch_sample_value_type_t<float32_t>(0.f);
    }
};

class batch_triangular_dither_generator : public triangular_dither_generator
{
public:
    inline detail::batch_sample_value_type_t<int32_t> generate_batch_int16() noexcept
    {
        return (xsimd::batch_cast<int32_t>(rng_()) >> static_cast<int32_t>(int16_shift())) +
               (xsimd::batch_cast<int32_t>(rng_()) >> static_cast<int32_t>(int16_shift()));
    }

    inline detail::batch_sample_value_type_t<float32_t> generate_batch_float32() noexcept
    {
        auto current = (xsimd::batch_cast<int32_t>(rng_()) >> static_cast<int32_t>(float32_shift())) +
                       (xsimd::batch_cast<int32_t>(rng_()) >> static_cast<int32_t>(float32_shift()));
        return xsimd::to_float(current) * float32_scaler();
    }

private:
    // We cannot use static constexpr members here as they are ODR-used and C++14 requires a definition for ODR-used
    // static constexpr members. Unfortunately any definition here in the header would result in multiple definitions
    // across the different translation units, therefore we are forced to do it this way. Once we drop support for C++14
    // we can change these to be static constexpr member variables which are implicitly inline and therefore won't
    // result in multiple definitions.
    static constexpr std::size_t int16_shift() noexcept
    {
        return 32 - int16_bits;
    }
    static constexpr std::size_t float32_shift() noexcept
    {
        return 1;
    }
    static constexpr float32_t float32_scaler() noexcept
    {
        return detail::symmetric_float_convert_traits<int32_t>::int_to_float_scaler;
    }

    static constexpr std::uint32_t default_seed = 0xfad46483;

    detail::batch_linear_congruential_generator rng_{default_seed};
};

class batch_shaped_dither_generator : public shaped_dither_generator
{
public:
    inline detail::batch_sample_value_type_t<int32_t> generate_batch_int16() noexcept
    {
        return generate_high_pass() >> static_cast<int32_t>(int16_shift());
    }

    inline detail::batch_sample_value_type_t<float32_t> generate_batch_float32() noexcept
    {
        return xsimd::to_float(generate_high_pass()) * float32_scaler();
    }

private:
    inline detail::batch_sample_value_type_t<int32_t> generate_high_pass() noexcept
    {
        auto current = (xsimd::batch_cast<int32_t>(rng_()) >> static_cast<int32_t>(initial_shift())) +
                       (xsimd::batch_cast<int32_t>(rng_()) >> static_cast<int32_t>(initial_shift()));
        auto high_pass = current - previous_;
        previous_ = current;
        return high_pass;
    }

    // We cannot use static constexpr members here as they are ODR-used and C++14 requires a definition for ODR-used
    // static constexpr members. Unfortunately any definition here in the header would result in multiple definitions
    // across the different translation units, therefore we are forced to do it this way. Once we drop support for C++14
    // we can change these to be static constexpr member variables which are implicitly inline and therefore won't
    // result in multiple definitions.
    static constexpr std::size_t initial_shift() noexcept
    {
        return 2;
    }
    static constexpr std::size_t int16_shift() noexcept
    {
        return 32 - int16_bits - (initial_shift() - 1);
    }
    static constexpr float32_t float32_scaler() noexcept
    {
        return detail::symmetric_float_convert_traits<int32_t>::int_to_float_scaler;
    }

    static constexpr std::uint32_t default_seed = 0x8914c30c;

    detail::batch_linear_congruential_generator rng_{default_seed};
    detail::batch_sample_value_type_t<int32_t> previous_{};
};

#else

using batch_null_dither_generator = null_dither_generator;

using batch_triangular_dither_generator = triangular_dither_generator;

using batch_shaped_dither_generator = shaped_dither_generator;

#endif

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_batch_dither_generator_
