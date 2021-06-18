#ifndef _ratl_detail_rand_
#define _ratl_detail_rand_

// C++ Standard Library includes
#include <cstdint>

// ratl includes
#include <ratl/detail/config.hpp>

namespace ratl
{
namespace detail
{
class LinearCongruentialGenerator
{
public:
    explicit constexpr LinearCongruentialGenerator(uint32_t seed) noexcept : state_{seed} {}

    ~LinearCongruentialGenerator() = default;

    LinearCongruentialGenerator(const LinearCongruentialGenerator&) = delete;

    LinearCongruentialGenerator& operator=(const LinearCongruentialGenerator&) = delete;

    LinearCongruentialGenerator(LinearCongruentialGenerator&&) noexcept = default;

    LinearCongruentialGenerator& operator=(LinearCongruentialGenerator&&) noexcept = default;

    constexpr inline uint32_t operator()() noexcept
    {
        state_ = (Multiplier * state_) + Increment;
        return state_;
    }

    constexpr inline void jump() noexcept
    {
        state_ = (*this)() ^ JumpMask;
    }

private:
    static constexpr uint32_t Multiplier = 0x0bb38435;
    static constexpr uint32_t Increment = 0x3619636b;
    static constexpr uint32_t JumpMask = 0x8739cbf1;
    uint32_t state_;
};

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_rand_
