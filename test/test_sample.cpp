/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

// ratl test includes
#include "test_utils.hpp"

namespace ratl
{
namespace test
{
static_assert(std::is_trivial<sample<int16_t>>::value, "sample<int16_t> is not a trivial type");
static_assert(
    std::is_trivially_default_constructible<sample<int16_t>>::value,
    "sample<int16_t> is not a trivially default constructible type");
static_assert(std::is_trivially_copyable<sample<int16_t>>::value, "sample<int16_t> is not a trivially copyable type");
static_assert(std::is_standard_layout<sample<int16_t>>::value, "sample<int16_t> is not a standard layout type");
static_assert(std::is_trivial<sample<int24_t>>::value, "sample<int24_t> is not a trivial type");
static_assert(
    std::is_trivially_default_constructible<sample<int24_t>>::value,
    "sample<int24_t> is not a trivially default constructible type");
static_assert(std::is_trivially_copyable<sample<int24_t>>::value, "sample<int24_t> is not a trivially copyable type");
static_assert(std::is_standard_layout<sample<int24_t>>::value, "sample<int24_t> is not a standard layout type");
static_assert(std::is_trivial<sample<int32_t>>::value, "sample<int32_t> is not a trivial type");
static_assert(
    std::is_trivially_default_constructible<sample<int32_t>>::value,
    "sample<int32_t> is not a trivially default constructible type");
static_assert(std::is_trivially_copyable<sample<int32_t>>::value, "sample<int32_t> is not a trivially copyable type");
static_assert(std::is_standard_layout<sample<int32_t>>::value, "sample<int32_t> is not a standard layout type");
static_assert(std::is_trivial<sample<float32_t>>::value, "sample<float32_t> is not a trivial type");
static_assert(
    std::is_trivially_default_constructible<sample<float32_t>>::value,
    "sample<float32_t> is not a trivially default constructible type");
static_assert(
    std::is_trivially_copyable<sample<float32_t>>::value, "sample<float32_t> is not a trivially copyable type");
static_assert(std::is_standard_layout<sample<float32_t>>::value, "sample<float32_t> is not a standard layout type");

static_assert(std::is_trivial<network_sample<int16_t>>::value, "network_sample<int16_t> is not a trivial type");
static_assert(
    std::is_trivially_default_constructible<network_sample<int16_t>>::value,
    "network_sample<int16_t> is not a trivially default constructible type");
static_assert(
    std::is_trivially_copyable<network_sample<int16_t>>::value,
    "network_sample<int16_t> is not a trivially copyable type");
static_assert(
    std::is_standard_layout<network_sample<int16_t>>::value, "network_sample<int16_t> is not a standard layout type");
static_assert(std::is_trivial<network_sample<int24_t>>::value, "network_sample<int24_t> is not a trivial type");
static_assert(
    std::is_trivially_default_constructible<network_sample<int24_t>>::value,
    "network_sample<int24_t> is not a trivially default constructible type");
static_assert(
    std::is_trivially_copyable<network_sample<int24_t>>::value,
    "network_sample<int24_t> is not a trivially copyable type");
static_assert(
    std::is_standard_layout<network_sample<int24_t>>::value, "network_sample<int24_t> is not a standard layout type");
static_assert(std::is_trivial<network_sample<int32_t>>::value, "network_sample<int32_t> is not a trivial type");
static_assert(
    std::is_trivially_default_constructible<network_sample<int32_t>>::value,
    "network_sample<int32_t> is not a trivially default constructible type");
static_assert(
    std::is_trivially_copyable<network_sample<int32_t>>::value,
    "network_sample<int32_t> is not a trivially copyable type");
static_assert(
    std::is_standard_layout<network_sample<int32_t>>::value, "network_sample<int32_t> is not a standard layout type");
static_assert(std::is_trivial<network_sample<float32_t>>::value, "network_sample<float32_t> is not a trivial type");
static_assert(
    std::is_trivially_default_constructible<network_sample<float32_t>>::value,
    "network_sample<float32_t> is not a trivially default constructible type");
static_assert(
    std::is_trivially_copyable<network_sample<float32_t>>::value,
    "network_sample<float32_t> is not a trivially copyable type");
static_assert(
    std::is_standard_layout<network_sample<float32_t>>::value,
    "network_sample<float32_t> is not a standard layout type");

// sample limits

TEST(SampleLimits, Int16Min)
{
    EXPECT_EQ(ratl::sample_limits<int16_t>::min(), static_cast<int16_t>(-32768));
}

TEST(SampleLimits, Int16Max)
{
    EXPECT_EQ(ratl::sample_limits<int16_t>::max(), static_cast<int16_t>(32767));
}

TEST(SampleLimits, Int24Min)
{
    EXPECT_EQ(ratl::sample_limits<int24_t>::min(), static_cast<int24_t>(-8388608));
}

TEST(SampleLimits, Int24Max)
{
    EXPECT_EQ(ratl::sample_limits<int24_t>::max(), static_cast<int24_t>(8388607));
}

TEST(SampleLimits, Int32Min)
{
    EXPECT_EQ(ratl::sample_limits<int32_t>::min(), static_cast<int32_t>(-2147483647 - 1));
}

TEST(SampleLimits, Int32Max)
{
    EXPECT_EQ(ratl::sample_limits<int32_t>::max(), static_cast<int32_t>(2147483647));
}

TEST(SampleLimits, Float32Min)
{
    EXPECT_EQ(ratl::sample_limits<float32_t>::min(), static_cast<float32_t>(-1));
}

TEST(SampleLimits, Float32Max)
{
    EXPECT_EQ(ratl::sample_limits<float32_t>::max(), static_cast<float32_t>(1));
}

// sample_int16

TEST(SampleInt16, CopySampleInt16Pos)
{
    ratl::sample<int16_t> sample(static_cast<int16_t>(1));
    EXPECT_EQ(ratl::sample<int16_t>(sample), ratl::sample<int16_t>(static_cast<int16_t>(1)));
}

TEST(SampleInt16, EqualSampleInt16Pos)
{
    EXPECT_EQ(ratl::sample<int16_t>(static_cast<int16_t>(1)), ratl::sample<int16_t>(static_cast<int16_t>(1)));
}

TEST(SampleInt16, CopySampleInt16Neg)
{
    ratl::sample<int16_t> sample(static_cast<int16_t>(-1));
    EXPECT_EQ(ratl::sample<int16_t>(sample), ratl::sample<int16_t>(static_cast<int16_t>(-1)));
}

TEST(SampleInt16, EqualSampleInt16Neg)
{
    EXPECT_EQ(ratl::sample<int16_t>(static_cast<int16_t>(-1)), ratl::sample<int16_t>(static_cast<int16_t>(-1)));
}

// sample_int24

TEST(SampleInt24, CopySampleInt24Pos)
{
    ratl::sample<int24_t> sample(static_cast<int24_t>(1));
    EXPECT_EQ(ratl::sample<int24_t>(sample), ratl::sample<int24_t>(static_cast<int24_t>(1)));
}

TEST(SampleInt24, EqualSampleInt24Pos)
{
    EXPECT_EQ(ratl::sample<int24_t>(static_cast<int24_t>(1)), ratl::sample<int24_t>(static_cast<int24_t>(1)));
}

TEST(SampleInt24, CopySampleInt24Neg)
{
    ratl::sample<int24_t> sample(static_cast<int24_t>(-1));
    EXPECT_EQ(ratl::sample<int24_t>(sample), ratl::sample<int24_t>(static_cast<int24_t>(-1)));
}

TEST(SampleInt24, EqualSampleInt24Neg)
{
    EXPECT_EQ(ratl::sample<int24_t>(static_cast<int24_t>(-1)), ratl::sample<int24_t>(static_cast<int24_t>(-1)));
}

// sample_int32

TEST(SampleInt32, CopySampleInt32Pos)
{
    ratl::sample<int32_t> sample(static_cast<int32_t>(1));
    EXPECT_EQ(ratl::sample<int32_t>(sample), ratl::sample<int32_t>(static_cast<int32_t>(1)));
}

TEST(SampleInt32, EqualSampleInt32Pos)
{
    EXPECT_EQ(ratl::sample<int32_t>(static_cast<int32_t>(1)), ratl::sample<int32_t>(static_cast<int32_t>(1)));
}

TEST(SampleInt32, CopySampleInt32Neg)
{
    ratl::sample<int32_t> sample(static_cast<int32_t>(-1));
    EXPECT_EQ(ratl::sample<int32_t>(sample), ratl::sample<int32_t>(static_cast<int32_t>(-1)));
}

TEST(SampleInt32, EqualSampleInt32Neg)
{
    EXPECT_EQ(ratl::sample<int32_t>(static_cast<int32_t>(-1)), ratl::sample<int32_t>(static_cast<int32_t>(-1)));
}

// sample_float32

TEST(SampleFloat32, CopySampleFloat32Pos)
{
    ratl::sample<float32_t> sample(static_cast<float32_t>(1));
    EXPECT_EQ(ratl::sample<float32_t>(sample), ratl::sample<float32_t>(static_cast<float32_t>(1)));
}

TEST(SampleFloat32, EqualSampleFloat32Pos)
{
    EXPECT_EQ(ratl::sample<float32_t>(static_cast<float32_t>(1)), ratl::sample<float32_t>(static_cast<float32_t>(1)));
}

TEST(SampleFloat32, CopySampleFloat32Neg)
{
    ratl::sample<float32_t> sample(static_cast<float32_t>(-1));
    EXPECT_EQ(ratl::sample<float32_t>(sample), ratl::sample<float32_t>(static_cast<float32_t>(-1)));
}

TEST(SampleFloat32, EqualSampleFloat32Neg)
{
    EXPECT_EQ(ratl::sample<float32_t>(static_cast<float32_t>(-1)), ratl::sample<float32_t>(static_cast<float32_t>(-1)));
}

} // namespace test
} // namespace ratl
