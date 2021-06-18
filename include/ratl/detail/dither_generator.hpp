#ifndef _ratl_detail_dither_generator_
#define _ratl_detail_dither_generator_

// C++ Standard Library includes
#include <cstddef>
#include <cstdint>

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/rand.hpp>
#include <ratl/detail/sample_converter.hpp>

namespace ratl
{
namespace detail
{
class NullDitherGenerator
{
public:
    static constexpr std::size_t Int16Bits = 0;
    static constexpr float32_t MaxFloat32 = 0;

    constexpr inline int32_t generateInt16() noexcept
    {
        return 0;
    }

    constexpr inline float32_t generateFloat32() noexcept
    {
        return 0;
    }
};

class TriangularDitherGenerator
{
public:
    static constexpr std::size_t Int16Bits = 15;
    static constexpr float32_t MaxFloat32 = 1.0;

    constexpr inline int32_t generateInt16() noexcept
    {
        return (static_cast<int32_t>(rng_()) >> Int16Shift) + (static_cast<int32_t>(rng_()) >> Int16Shift);
    }

    constexpr inline float32_t generateFloat32() noexcept
    {
        auto current = (static_cast<int32_t>(rng_()) >> Float32Shift) + (static_cast<int32_t>(rng_()) >> Float32Shift);
        return static_cast<float32_t>(current) * Float32Scaler;
    }

private:
    static constexpr uint32_t DefaultSeed = 0x1942da21;
    static constexpr std::size_t Int16Shift = (32 - Int16Bits);
    static constexpr std::size_t Float32Shift = 1;
    static constexpr float32_t Float32Scaler = detail::FloatConvertTraits<int32_t>::Divisor;

    detail::LinearCongruentialGenerator rng_{DefaultSeed};
};

class ShapedDitherGenerator
{
public:
    static constexpr std::size_t Int16Bits = 15;
    static constexpr float32_t MaxFloat32 = 1.0;

    constexpr inline int32_t generateInt16() noexcept
    {
        return generateHighPass() >> Int16Shift;
    }

    constexpr inline float32_t generateFloat32() noexcept
    {
        return static_cast<float32_t>(generateHighPass()) * Float32Scaler;
    }

private:
    constexpr inline int32_t generateHighPass() noexcept
    {
        auto current = (static_cast<int32_t>(rng_()) >> InitialShift) + (static_cast<int32_t>(rng_()) >> InitialShift);
        auto high_pass = current - previous_;
        previous_ = current;
        return high_pass;
    }

    static constexpr uint32_t DefaultSeed = 0xac9ad704;
    static constexpr std::size_t InitialShift = 2;
    static constexpr std::size_t Int16Shift = 32 - Int16Bits - (InitialShift - 1);
    static constexpr float32_t Float32Scaler = detail::FloatConvertTraits<int32_t>::Divisor;

    detail::LinearCongruentialGenerator rng_{DefaultSeed};
    int32_t previous_{};
};

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_dither_generator_
