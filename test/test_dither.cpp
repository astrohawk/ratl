/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

// ratl includes
#include <ratl/detail/dither_generator.hpp>

// other includes
#include <gtest/gtest.h>

namespace ratl
{
namespace test
{
template<typename Generator>
class DitherGeneratorTest : public testing::Test
{
public:
    using generator = Generator;

    static constexpr std::size_t num_tests = 10000000;

    generator generator_;
};

using DitherGenerators = testing::Types<
    ratl::detail::null_dither_generator,
    ratl::detail::triangular_dither_generator,
    ratl::detail::shaped_dither_generator>;
TYPED_TEST_SUITE(DitherGeneratorTest, DitherGenerators, );

TYPED_TEST(DitherGeneratorTest, Int16Limits)
{
    static constexpr auto int16_bits = TestFixture::generator::int16_bits;
    static constexpr auto limit_max = static_cast<int32_t>((int16_bits == 0) ? 0 : ~(0xffffffff << int16_bits));
    static constexpr auto limit_min = static_cast<int32_t>((int16_bits == 0) ? 0 : (0xffffffff << int16_bits));
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
    static constexpr float32_t limit_max = TestFixture::generator::float32_max;
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
