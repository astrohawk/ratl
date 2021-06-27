
#include "test_utils.hpp"

namespace ratl
{
namespace test
{
// Sample limits

TEST(SampleLimits, Int16Min)
{
    EXPECT_EQ(ratl::SampleTypeLimits<int16_t>::min, -32768);
}

TEST(SampleLimits, Int16Max)
{
    EXPECT_EQ(ratl::SampleTypeLimits<int16_t>::max, 32767);
}

TEST(SampleLimits, Int24Min)
{
    EXPECT_EQ(ratl::SampleTypeLimits<int24_t>::min, -8388608);
}

TEST(SampleLimits, Int24Max)
{
    EXPECT_EQ(ratl::SampleTypeLimits<int24_t>::max, 8388607);
}

TEST(SampleLimits, Int32Min)
{
    EXPECT_EQ(ratl::SampleTypeLimits<int32_t>::min, (-2147483647 - 1));
}

TEST(SampleLimits, Int32Max)
{
    EXPECT_EQ(ratl::SampleTypeLimits<int32_t>::max, 2147483647);
}

TEST(SampleLimits, Float32Min)
{
    EXPECT_EQ(ratl::SampleTypeLimits<float32_t>::min, -1);
}

TEST(SampleLimits, Float32Max)
{
    EXPECT_EQ(ratl::SampleTypeLimits<float32_t>::max, 1);
}

// SampleInt16

TEST(SampleInt16, CopySampleInt16Pos)
{
    ratl::Sample<int16_t> sample{1};
    EXPECT_EQ(ratl::Sample<int16_t>{sample}, ratl::Sample<int16_t>{1});
}

TEST(SampleInt16, EqualSampleInt16Pos)
{
    EXPECT_EQ(ratl::Sample<int16_t>{1}, ratl::Sample<int16_t>{1});
}

TEST(SampleInt16, CopySampleInt16Neg)
{
    ratl::Sample<int16_t> sample{-1};
    EXPECT_EQ(ratl::Sample<int16_t>{sample}, ratl::Sample<int16_t>{-1});
}

TEST(SampleInt16, EqualSampleInt16Neg)
{
    EXPECT_EQ(ratl::Sample<int16_t>{-1}, ratl::Sample<int16_t>{-1});
}

// SampleInt24

TEST(SampleInt24, CopySampleInt24Pos)
{
    ratl::Sample<int24_t> sample{1};
    EXPECT_EQ(ratl::Sample<int24_t>{sample}, ratl::Sample<int24_t>{1});
}

TEST(SampleInt24, EqualSampleInt24Pos)
{
    EXPECT_EQ(ratl::Sample<int24_t>{1}, ratl::Sample<int24_t>{1});
}

TEST(SampleInt24, CopySampleInt24Neg)
{
    ratl::Sample<int24_t> sample{-1};
    EXPECT_EQ(ratl::Sample<int24_t>{sample}, ratl::Sample<int24_t>{-1});
}

TEST(SampleInt24, EqualSampleInt24Neg)
{
    EXPECT_EQ(ratl::Sample<int24_t>{-1}, ratl::Sample<int24_t>{-1});
}

// SampleInt32

TEST(SampleInt32, CopySampleInt32Pos)
{
    ratl::Sample<int32_t> sample{1};
    EXPECT_EQ(ratl::Sample<int32_t>{sample}, ratl::Sample<int32_t>{1});
}

TEST(SampleInt32, EqualSampleInt32Pos)
{
    EXPECT_EQ(ratl::Sample<int32_t>{1}, ratl::Sample<int32_t>{1});
}

TEST(SampleInt32, CopySampleInt32Neg)
{
    ratl::Sample<int32_t> sample{-1};
    EXPECT_EQ(ratl::Sample<int32_t>{sample}, ratl::Sample<int32_t>{-1});
}

TEST(SampleInt32, EqualSampleInt32Neg)
{
    EXPECT_EQ(ratl::Sample<int32_t>{-1}, ratl::Sample<int32_t>{-1});
}

// SampleFloat32

TEST(SampleFloat32, CopySampleFloat32Pos)
{
    ratl::Sample<float32_t> sample{1.f};
    EXPECT_EQ(ratl::Sample<float32_t>{sample}, ratl::Sample<float32_t>{1.f});
}

TEST(SampleFloat32, EqualSampleFloat32Pos)
{
    EXPECT_EQ(ratl::Sample<float32_t>{1.f}, ratl::Sample<float32_t>{1.f});
}

TEST(SampleFloat32, CopySampleFloat32Neg)
{
    ratl::Sample<float32_t> sample{-1.f};
    EXPECT_EQ(ratl::Sample<float32_t>{sample}, ratl::Sample<float32_t>{-1.f});
}

TEST(SampleFloat32, EqualSampleFloat32Neg)
{
    EXPECT_EQ(ratl::Sample<float32_t>{-1.f}, ratl::Sample<float32_t>{-1.f});
}

} // namespace test
} // namespace ratl
