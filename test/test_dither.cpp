
#include <gtest/gtest.h>
#include <ratl/detail/dither_generator.hpp>

namespace ratl
{
namespace test
{
template<typename Generator>
class DitherGeneratorTest : public testing::Test
{
protected:
    using DitherGenerator = Generator;
    static constexpr std::size_t num_tests = 10000000;
    DitherGenerator generator_;
};

using DitherGenerators = testing::Types<
    ratl::detail::null_dither_generator,
    ratl::detail::triangular_dither_generator,
    ratl::detail::shaped_dither_generator>;
TYPED_TEST_SUITE(DitherGeneratorTest, DitherGenerators, );

TYPED_TEST(DitherGeneratorTest, Int16Limits)
{
    static constexpr int32_t limit_max = (TestFixture::DitherGenerator::int16_bits == 0)
                                             ? 0
                                             : (0xffffffff >> (32 - TestFixture::DitherGenerator::int16_bits));
    static constexpr int32_t limit_min =
        (TestFixture::DitherGenerator::int16_bits == 0) ? 0 : (0xffffffff << TestFixture::DitherGenerator::int16_bits);
    int32_t max = 0;
    int32_t min = 0;
    for (std::size_t i = 0; i < this->num_tests; ++i)
    {
        auto dither = this->generator_.generate_int16();
        EXPECT_LE(dither, limit_max);
        EXPECT_GE(dither, limit_min);
        max = std::max(dither, max);
        min = std::min(dither, min);
    }
    EXPECT_GE(max, limit_max * 0.95);
    EXPECT_LE(min, limit_min * 0.95);
}

TYPED_TEST(DitherGeneratorTest, Float32Limits)
{
    static constexpr float32_t limit_max = TestFixture::DitherGenerator::float32_max;
    static constexpr float32_t limit_min = -limit_max;
    float32_t max = 0;
    float32_t min = 0;
    for (std::size_t i = 0; i < this->num_tests; ++i)
    {
        auto dither = this->generator_.generate_float32();
        EXPECT_LE(dither, limit_max);
        EXPECT_GE(dither, limit_min);
        max = std::max(dither, max);
        min = std::min(dither, min);
    }
    EXPECT_GE(max, limit_max * 0.95);
    EXPECT_LE(min, limit_min * 0.95);
}

} // namespace test
} // namespace ratl
