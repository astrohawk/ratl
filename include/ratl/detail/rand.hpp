/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_rand_
#define _ratl_detail_rand_

// ratl includes
#include <ratl/detail/config.hpp>

// other includes
#include <cstdint>

namespace ratl
{
namespace detail
{
class linear_congruential_generator
{
public:
    explicit constexpr linear_congruential_generator(std::uint32_t seed) noexcept : state_(seed) {}

    ~linear_congruential_generator() = default;

    linear_congruential_generator(const linear_congruential_generator&) = delete;

    linear_congruential_generator& operator=(const linear_congruential_generator&) = delete;

    linear_congruential_generator(linear_congruential_generator&&) noexcept = default;

    linear_congruential_generator& operator=(linear_congruential_generator&&) noexcept = default;

    inline constexpr std::uint32_t operator()() noexcept
    {
        state_ = (multiplier * state_) + increment;
        return state_;
    }

    inline constexpr void jump() noexcept
    {
        state_ = (*this)() ^ jump_mask;
    }

private:
    static constexpr std::uint32_t multiplier = 0x0bb38435;
    static constexpr std::uint32_t increment = 0x3619636b;
    static constexpr std::uint32_t jump_mask = 0x8739cbf1;
    std::uint32_t state_;
};

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_rand_
