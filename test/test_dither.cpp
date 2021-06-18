
#include <gtest/gtest.h>
#include <ratl/detail/dither_generator.hpp>

namespace ratl
{
template<typename Generator>
class DitherGeneratorTest : public testing::Test
{
protected:
    using DitherGenerator = Generator;
    static constexpr std::size_t NumTests = 10000000;
    DitherGenerator generator_;
};

using DitherGenerators = testing::Types<
    ratl::detail::NullDitherGenerator,
    ratl::detail::TriangularDitherGenerator,
    ratl::detail::ShapedDitherGenerator>;
TYPED_TEST_SUITE(DitherGeneratorTest, DitherGenerators, );

TYPED_TEST(DitherGeneratorTest, Int16Limits)
{
    static constexpr int32_t LimitMax = (TestFixture::DitherGenerator::Int16Bits == 0)
                                            ? 0
                                            : (0xffffffff >> (32 - TestFixture::DitherGenerator::Int16Bits));
    static constexpr int32_t LimitMin =
        (TestFixture::DitherGenerator::Int16Bits == 0) ? 0 : (0xffffffff << TestFixture::DitherGenerator::Int16Bits);
    int32_t max = 0;
    int32_t min = 0;
    for (std::size_t i = 0; i < this->NumTests; ++i)
    {
        auto dither = this->generator_.generateInt16();
        EXPECT_LE(dither, LimitMax);
        EXPECT_GE(dither, LimitMin);
        max = std::max(dither, max);
        min = std::min(dither, min);
    }
    EXPECT_GE(max, LimitMax * 0.95);
    EXPECT_LE(min, LimitMin * 0.95);
}

TYPED_TEST(DitherGeneratorTest, Float32Limits)
{
    static constexpr float32_t LimitMax = TestFixture::DitherGenerator::MaxFloat32;
    static constexpr float32_t LimitMin = -LimitMax;
    float32_t max = 0;
    float32_t min = 0;
    for (std::size_t i = 0; i < this->NumTests; ++i)
    {
        auto dither = this->generator_.generateFloat32();
        EXPECT_LE(dither, LimitMax);
        EXPECT_GE(dither, LimitMin);
        max = std::max(dither, max);
        min = std::min(dither, min);
    }
    EXPECT_GE(max, LimitMax * 0.95);
    EXPECT_LE(min, LimitMin * 0.95);
}

} // namespace ratl
