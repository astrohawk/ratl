#ifndef _ratl_detail_batch_dither_generator_
#define _ratl_detail_batch_dither_generator_

// C++ Standard Library includes
#include <cstddef>
#include <cstdint>

// ratl includes
#include <ratl/detail/batch_base_traits.hpp>
#include <ratl/detail/batch_rand.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/detail/dither_generator.hpp>
#include <ratl/detail/convert_traits.hpp>

namespace ratl
{
namespace detail
{
#if defined(RATL_HAS_XSIMD)

class BatchNullDitherGenerator : public NullDitherGenerator
{
public:
    inline detail::BatchSampleValueType_t<int32_t> generateBatchInt16() noexcept
    {
        return detail::BatchSampleValueType_t<int32_t>{0};
    }

    inline detail::BatchSampleValueType_t<float32_t> generateBatchFloat32() noexcept
    {
        return detail::BatchSampleValueType_t<float32_t>{0.f};
    }
};

class BatchTriangularDitherGenerator : public TriangularDitherGenerator
{
public:
    inline detail::BatchSampleValueType_t<int32_t> generateBatchInt16() noexcept
    {
        return (xsimd::batch_cast<int32_t>(rng_()) >> Int16Shift) + (xsimd::batch_cast<int32_t>(rng_()) >> Int16Shift);
    }

    inline detail::BatchSampleValueType_t<float32_t> generateBatchFloat32() noexcept
    {
        auto current =
            (xsimd::batch_cast<int32_t>(rng_()) >> Float32Shift) + (xsimd::batch_cast<int32_t>(rng_()) >> Float32Shift);
        return xsimd::to_float(current) * Float32Scaler;
    }

private:
    static constexpr uint32_t DefaultSeed = 0xfad46483;
    static constexpr std::size_t Int16Shift = (32 - Int16Bits);
    static constexpr std::size_t Float32Shift = 1;
    static constexpr float32_t Float32Scaler = detail::FloatConvertTraits<int32_t>::Divisor;

    detail::BatchLinearCongruentialGenerator rng_{DefaultSeed};
};

constexpr uint32_t BatchTriangularDitherGenerator::DefaultSeed;
constexpr std::size_t BatchTriangularDitherGenerator::Int16Shift;
constexpr std::size_t BatchTriangularDitherGenerator::Float32Shift;
constexpr float32_t BatchTriangularDitherGenerator::Float32Scaler;

class BatchShapedDitherGenerator : public ShapedDitherGenerator
{
public:
    inline detail::BatchSampleValueType_t<int32_t> generateBatchInt16() noexcept
    {
        return generateHighPass() >> Int16Shift;
    }

    inline detail::BatchSampleValueType_t<float32_t> generateBatchFloat32() noexcept
    {
        return xsimd::to_float(generateHighPass()) * Float32Scaler;
    }

private:
    inline detail::BatchSampleValueType_t<int32_t> generateHighPass() noexcept
    {
        auto current =
            (xsimd::batch_cast<int32_t>(rng_()) >> InitialShift) + (xsimd::batch_cast<int32_t>(rng_()) >> InitialShift);
        auto high_pass = current - previous_;
        previous_ = current;
        return high_pass;
    }

    static constexpr uint32_t DefaultSeed = 0x8914c30c;
    static constexpr std::size_t InitialShift = 2;
    static constexpr std::size_t Int16Shift = 32 - Int16Bits - (InitialShift - 1);
    static constexpr float32_t Float32Scaler = detail::FloatConvertTraits<int32_t>::Divisor;

    detail::BatchLinearCongruentialGenerator rng_{DefaultSeed};
    detail::BatchSampleValueType_t<int32_t> previous_{};
};

constexpr uint32_t BatchShapedDitherGenerator::DefaultSeed;
constexpr std::size_t BatchShapedDitherGenerator::InitialShift;
constexpr std::size_t BatchShapedDitherGenerator::Int16Shift;
constexpr float32_t BatchShapedDitherGenerator::Float32Scaler;

#else

using BatchNullDitherGenerator = NullDitherGenerator;

using BatchTriangularDitherGenerator = TriangularDitherGenerator;

using BatchShapedDitherGenerator = ShapedDitherGenerator;

#endif

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_batch_dither_generator_
