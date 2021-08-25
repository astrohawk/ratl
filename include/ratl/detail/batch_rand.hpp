/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_batch_rand_
#define _ratl_detail_batch_rand_

// ratl includes
#include <ratl/detail/batch_value_traits.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/detail/rand.hpp>

// other includes
#include <cstdint>

#if defined(RATL_HAS_XSIMD)

namespace ratl
{
namespace detail
{
class batch_linear_congruential_generator
{
    using batch_type = xsimd::batch<std::uint32_t, batch_size>;

public:
    explicit batch_linear_congruential_generator(uint32_t seed) noexcept :
        state_(make_batch_state(seed, std::make_index_sequence<batch_size>{}))
    {
    }

    ~batch_linear_congruential_generator() = default;

    batch_linear_congruential_generator(const batch_linear_congruential_generator&) = delete;

    batch_linear_congruential_generator& operator=(const batch_linear_congruential_generator&) = delete;

    batch_linear_congruential_generator(batch_linear_congruential_generator&&) noexcept = default;

    batch_linear_congruential_generator& operator=(batch_linear_congruential_generator&&) noexcept = default;

    inline batch_type operator()() noexcept
    {
        state_ = (multiplier * state_) + increment;
        return state_;
    }

    inline void jump() noexcept
    {
        state_ = (*this)() ^ jump_mask;
    }

private:
    static uint32_t make_state(linear_congruential_generator& gen, std::size_t)
    {
        auto state = gen();
        gen.jump();
        return state;
    }

    template<std::size_t... I>
    static batch_type make_batch_state(uint32_t seed, std::index_sequence<I...>)
    {
        linear_congruential_generator gen{seed};
        return batch_type{make_state(gen, I)...};
    }

    static constexpr uint32_t multiplier = 0x0bb38435;
    static constexpr uint32_t increment = 0x3619636b;
    static constexpr uint32_t jump_mask = 0x8739cbf1;
    batch_type state_;
};

constexpr uint32_t batch_linear_congruential_generator::multiplier;
constexpr uint32_t batch_linear_congruential_generator::increment;
constexpr uint32_t batch_linear_congruential_generator::jump_mask;

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_detail_batch_rand_
