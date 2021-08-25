
#include "test_utils.hpp"

namespace ratl
{
namespace test
{
// sample limits

TEST(SampleLimits, Int16Min)
{
    EXPECT_EQ(ratl::sample_limits<int16_t>::min, -32768);
}

TEST(SampleLimits, Int16Max)
{
    EXPECT_EQ(ratl::sample_limits<int16_t>::max, 32767);
}

TEST(SampleLimits, Int24Min)
{
    EXPECT_EQ(ratl::sample_limits<int24_t>::min, -8388608);
}

TEST(SampleLimits, Int24Max)
{
    EXPECT_EQ(ratl::sample_limits<int24_t>::max, 8388607);
}

TEST(SampleLimits, Int32Min)
{
    EXPECT_EQ(ratl::sample_limits<int32_t>::min, (-2147483647 - 1));
}

TEST(SampleLimits, Int32Max)
{
    EXPECT_EQ(ratl::sample_limits<int32_t>::max, 2147483647);
}

TEST(SampleLimits, Float32Min)
{
    EXPECT_EQ(ratl::sample_limits<float32_t>::min, -1);
}

TEST(SampleLimits, Float32Max)
{
    EXPECT_EQ(ratl::sample_limits<float32_t>::max, 1);
}

// sample_int16

TEST(sample_int16, CopySampleInt16Pos)
{
    ratl::sample<int16_t> sample{1};
    EXPECT_EQ(ratl::sample<int16_t>{sample}, ratl::sample<int16_t>{1});
}

TEST(sample_int16, EqualSampleInt16Pos)
{
    EXPECT_EQ(ratl::sample<int16_t>{1}, ratl::sample<int16_t>{1});
}

TEST(sample_int16, CopySampleInt16Neg)
{
    ratl::sample<int16_t> sample{-1};
    EXPECT_EQ(ratl::sample<int16_t>{sample}, ratl::sample<int16_t>{-1});
}

TEST(sample_int16, EqualSampleInt16Neg)
{
    EXPECT_EQ(ratl::sample<int16_t>{-1}, ratl::sample<int16_t>{-1});
}

// sample_int24

TEST(sample_int24, CopySampleInt24Pos)
{
    ratl::sample<int24_t> sample{1};
    EXPECT_EQ(ratl::sample<int24_t>{sample}, ratl::sample<int24_t>{1});
}

TEST(sample_int24, EqualSampleInt24Pos)
{
    EXPECT_EQ(ratl::sample<int24_t>{1}, ratl::sample<int24_t>{1});
}

TEST(sample_int24, CopySampleInt24Neg)
{
    ratl::sample<int24_t> sample{-1};
    EXPECT_EQ(ratl::sample<int24_t>{sample}, ratl::sample<int24_t>{-1});
}

TEST(sample_int24, EqualSampleInt24Neg)
{
    EXPECT_EQ(ratl::sample<int24_t>{-1}, ratl::sample<int24_t>{-1});
}

// sample_int32

TEST(sample_int32, CopySampleInt32Pos)
{
    ratl::sample<int32_t> sample{1};
    EXPECT_EQ(ratl::sample<int32_t>{sample}, ratl::sample<int32_t>{1});
}

TEST(sample_int32, EqualSampleInt32Pos)
{
    EXPECT_EQ(ratl::sample<int32_t>{1}, ratl::sample<int32_t>{1});
}

TEST(sample_int32, CopySampleInt32Neg)
{
    ratl::sample<int32_t> sample{-1};
    EXPECT_EQ(ratl::sample<int32_t>{sample}, ratl::sample<int32_t>{-1});
}

TEST(sample_int32, EqualSampleInt32Neg)
{
    EXPECT_EQ(ratl::sample<int32_t>{-1}, ratl::sample<int32_t>{-1});
}

// sample_float32

TEST(sample_float32, CopySampleFloat32Pos)
{
    ratl::sample<float32_t> sample{1.f};
    EXPECT_EQ(ratl::sample<float32_t>{sample}, ratl::sample<float32_t>{1.f});
}

TEST(sample_float32, EqualSampleFloat32Pos)
{
    EXPECT_EQ(ratl::sample<float32_t>{1.f}, ratl::sample<float32_t>{1.f});
}

TEST(sample_float32, CopySampleFloat32Neg)
{
    ratl::sample<float32_t> sample{-1.f};
    EXPECT_EQ(ratl::sample<float32_t>{sample}, ratl::sample<float32_t>{-1.f});
}

TEST(sample_float32, EqualSampleFloat32Neg)
{
    EXPECT_EQ(ratl::sample<float32_t>{-1.f}, ratl::sample<float32_t>{-1.f});
}

} // namespace test
} // namespace ratl
