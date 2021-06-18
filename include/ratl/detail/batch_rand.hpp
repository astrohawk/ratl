#ifndef _ratl_detail_batch_rand_
#define _ratl_detail_batch_rand_

// C++ Standard Library includes
#include <cstdint>

// ratl includes
#include <ratl/detail/batch_base_traits.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/detail/rand.hpp>

#if defined(RATL_HAS_XSIMD)

namespace ratl
{
namespace detail
{
class BatchLinearCongruentialGenerator
{
    using Batch = xsimd::batch<uint32_t, BatchSize>;

public:
    explicit BatchLinearCongruentialGenerator(uint32_t seed) noexcept :
        state_(makeBatchState(seed, std::make_index_sequence<BatchSize>{}))
    {
    }

    ~BatchLinearCongruentialGenerator() = default;

    BatchLinearCongruentialGenerator(const BatchLinearCongruentialGenerator&) = delete;

    BatchLinearCongruentialGenerator& operator=(const BatchLinearCongruentialGenerator&) = delete;

    BatchLinearCongruentialGenerator(BatchLinearCongruentialGenerator&&) noexcept = default;

    BatchLinearCongruentialGenerator& operator=(BatchLinearCongruentialGenerator&&) noexcept = default;

    inline Batch operator()() noexcept
    {
        state_ = (Multiplier * state_) + Increment;
        return state_;
    }

    inline void jump() noexcept
    {
        state_ = (*this)() ^ JumpMask;
    }

private:
    static uint32_t makeState(LinearCongruentialGenerator& gen, std::size_t)
    {
        auto state = gen();
        gen.jump();
        return state;
    }

    template<std::size_t... I>
    static Batch makeBatchState(uint32_t seed, std::index_sequence<I...>)
    {
        LinearCongruentialGenerator gen{seed};
        return Batch{makeState(gen, I)...};
    }

    static constexpr uint32_t Multiplier = 0x0bb38435;
    static constexpr uint32_t Increment = 0x3619636b;
    static constexpr uint32_t JumpMask = 0x8739cbf1;
    Batch state_;
};

constexpr uint32_t BatchLinearCongruentialGenerator::Multiplier;
constexpr uint32_t BatchLinearCongruentialGenerator::Increment;
constexpr uint32_t BatchLinearCongruentialGenerator::JumpMask;

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_detail_batch_rand_
