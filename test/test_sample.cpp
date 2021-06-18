
#include "test_utils.hpp"

namespace ratl
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

TEST(SampleInt16, ConvertSampleInt24Pos)
{
    ratl::Sample<int24_t> sample{256};
    EXPECT_EQ(ratl::Sample<int16_t>{sample}, ratl::Sample<int16_t>{1});
}

TEST(SampleInt16, EqualSampleInt24Pos)
{
    EXPECT_EQ(ratl::Sample<int16_t>{1}, ratl::Sample<int24_t>{256});
}

TEST(SampleInt16, ConvertSampleInt24Neg)
{
    ratl::Sample<int24_t> sample{-255};
    EXPECT_EQ(ratl::Sample<int16_t>{sample}, ratl::Sample<int16_t>{-1});
}

TEST(SampleInt16, EqualSampleInt24Neg)
{
    EXPECT_EQ(ratl::Sample<int16_t>{-1}, ratl::Sample<int24_t>{-255});
}

TEST(SampleInt16, ConvertSampleInt32Pos)
{
    ratl::Sample<int32_t> sample{65536};
    EXPECT_EQ(ratl::Sample<int16_t>{sample}, ratl::Sample<int16_t>{1});
}

TEST(SampleInt16, EqualSampleInt32Pos)
{
    EXPECT_EQ(ratl::Sample<int16_t>{1}, ratl::Sample<int32_t>{65536});
}

TEST(SampleInt16, ConvertSampleInt32Neg)
{
    ratl::Sample<int32_t> sample{-65536};
    EXPECT_EQ(ratl::Sample<int16_t>{sample}, ratl::Sample<int16_t>{-1});
}

TEST(SampleInt16, EqualSampleInt32Neg)
{
    EXPECT_EQ(ratl::Sample<int16_t>{-1}, ratl::Sample<int32_t>{-65536});
}

TEST(SampleInt16, ConvertSampleFloat32Pos)
{
    ratl::Sample<float32_t> sample{1.f / 32768.f};
    EXPECT_EQ(ratl::Sample<int16_t>{sample}, ratl::Sample<int16_t>{1});
}

TEST(SampleInt16, EqualSampleFloat32Pos)
{
    EXPECT_EQ(ratl::Sample<int16_t>{1}, ratl::Sample<float32_t>{1.f / 32768.f});
}

TEST(SampleInt16, ConvertSampleFloat32Neg)
{
    ratl::Sample<float32_t> sample{-1.f / 32768.f};
    EXPECT_EQ(ratl::Sample<int16_t>{sample}, ratl::Sample<int16_t>{-1});
}

TEST(SampleInt16, EqualSampleFloat32Neg)
{
    EXPECT_EQ(ratl::Sample<int16_t>{-1}, ratl::Sample<float32_t>{-1.f / 32768.f});
}

// SampleInt24

TEST(SampleInt24, ConvertSampleInt16Pos)
{
    ratl::Sample<int16_t> sample{1};
    EXPECT_EQ(ratl::Sample<int24_t>{sample}, ratl::Sample<int24_t>{256});
}

TEST(SampleInt24, EqualSampleInt16Pos)
{
    EXPECT_EQ(ratl::Sample<int24_t>{256}, ratl::Sample<int16_t>{1});
}

TEST(SampleInt24, ConvertSampleInt16Neg)
{
    ratl::Sample<int16_t> sample{-1};
    EXPECT_EQ(ratl::Sample<int24_t>{sample}, ratl::Sample<int24_t>{-256});
}

TEST(SampleInt24, EqualSampleInt16Neg)
{
    EXPECT_EQ(ratl::Sample<int24_t>{-256}, ratl::Sample<int16_t>{-1});
}

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

TEST(SampleInt24, ConvertSampleInt32Pos)
{
    ratl::Sample<int32_t> sample{256};
    EXPECT_EQ(ratl::Sample<int24_t>{sample}, ratl::Sample<int24_t>{1});
}

TEST(SampleInt24, EqualSampleInt32Pos)
{
    EXPECT_EQ(ratl::Sample<int24_t>{1}, ratl::Sample<int32_t>{256});
}

TEST(SampleInt24, ConvertSampleInt32Neg)
{
    ratl::Sample<int32_t> sample{-255};
    EXPECT_EQ(ratl::Sample<int24_t>{sample}, ratl::Sample<int24_t>{-1});
}

TEST(SampleInt24, EqualSampleInt32Neg)
{
    EXPECT_EQ(ratl::Sample<int24_t>{-1}, ratl::Sample<int32_t>{-255});
}

TEST(SampleInt24, ConvertSampleFloat32Pos)
{
    ratl::Sample<float32_t> sample{1.f / 8388608.f};
    EXPECT_EQ(ratl::Sample<int24_t>{sample}, ratl::Sample<int24_t>{1});
}

TEST(SampleInt24, EqualSampleFloat32Pos)
{
    EXPECT_EQ(ratl::Sample<int24_t>{1}, ratl::Sample<float32_t>{1.f / 8388608.f});
}

TEST(SampleInt24, ConvertSampleFloat32Neg)
{
    ratl::Sample<float32_t> sample{-1.f / 8388608.f};
    EXPECT_EQ(ratl::Sample<int24_t>{sample}, ratl::Sample<int24_t>{-1});
}

TEST(SampleInt24, EqualSampleFloat32Neg)
{
    EXPECT_EQ(ratl::Sample<int24_t>{-1}, ratl::Sample<float32_t>{-1.f / 8388608.f});
}

// SampleInt32

TEST(SampleInt32, ConvertSampleInt16Pos)
{
    ratl::Sample<int16_t> sample{1};
    EXPECT_EQ(ratl::Sample<int32_t>{sample}, ratl::Sample<int32_t>{65536});
}

TEST(SampleInt32, EqualSampleInt16Pos)
{
    EXPECT_EQ(ratl::Sample<int32_t>{65536}, ratl::Sample<int16_t>{1});
}

TEST(SampleInt32, ConvertSampleInt16Neg)
{
    ratl::Sample<int16_t> sample{-1};
    EXPECT_EQ(ratl::Sample<int32_t>{sample}, ratl::Sample<int32_t>{-65536});
}

TEST(SampleInt32, EqualSampleInt16Neg)
{
    EXPECT_EQ(ratl::Sample<int32_t>{-65536}, ratl::Sample<int16_t>{-1});
}

TEST(SampleInt32, ConvertSampleInt24Pos)
{
    ratl::Sample<int24_t> sample{1};
    EXPECT_EQ(ratl::Sample<int32_t>{sample}, ratl::Sample<int32_t>{256});
}

TEST(SampleInt32, EqualSampleInt24Pos)
{
    EXPECT_EQ(ratl::Sample<int32_t>{256}, ratl::Sample<int24_t>{1});
}

TEST(SampleInt32, ConvertSampleInt24Neg)
{
    ratl::Sample<int24_t> sample{-1};
    EXPECT_EQ(ratl::Sample<int32_t>{sample}, ratl::Sample<int32_t>{-256});
}

TEST(SampleInt32, EqualSampleInt24Neg)
{
    EXPECT_EQ(ratl::Sample<int32_t>{-256}, ratl::Sample<int24_t>{-1});
}

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

TEST(SampleInt32, ConvertSampleFloat32Pos)
{
    ratl::Sample<float32_t> sample{1.f / 2147483648.f};
    EXPECT_EQ(ratl::Sample<int32_t>{sample}, ratl::Sample<int32_t>{1});
}

TEST(SampleInt32, EqualSampleFloat32Pos)
{
    EXPECT_EQ(ratl::Sample<int32_t>{1}, ratl::Sample<float32_t>{1.f / 2147483648.f});
}

TEST(SampleInt32, ConvertSampleFloat32Neg)
{
    ratl::Sample<float32_t> sample{-1.f / 2147483648.f};
    EXPECT_EQ(ratl::Sample<int32_t>{sample}, ratl::Sample<int32_t>{-1});
}

TEST(SampleInt32, EqualSampleFloat32Neg)
{
    EXPECT_EQ(ratl::Sample<int32_t>{-1}, ratl::Sample<float32_t>{-1.f / 2147483648.f});
}

// SampleFloat32

TEST(SampleFloat32, ConvertSampleInt16Pos)
{
    ratl::Sample<int16_t> sample{1};
    EXPECT_EQ(ratl::Sample<float32_t>{sample}, ratl::Sample<float32_t>{1.f / 32768.f});
}

TEST(SampleFloat32, EqualSampleInt16Pos)
{
    EXPECT_EQ(ratl::Sample<float32_t>{1.f / 32768.f}, ratl::Sample<int16_t>{1});
}

TEST(SampleFloat32, ConvertSampleInt16Neg)
{
    ratl::Sample<int16_t> sample{-1};
    EXPECT_EQ(ratl::Sample<float32_t>{sample}, ratl::Sample<float32_t>{-1.f / 32768.f});
}

TEST(SampleFloat32, EqualSampleInt16Neg)
{
    EXPECT_EQ(ratl::Sample<float32_t>{-1.f / 32768.f}, ratl::Sample<int16_t>{-1});
}

TEST(SampleFloat32, ConvertSampleInt24Pos)
{
    ratl::Sample<int24_t> sample{1};
    EXPECT_EQ(ratl::Sample<float32_t>{sample}, ratl::Sample<float32_t>{1.f / 8388608.f});
}

TEST(SampleFloat32, EqualSampleInt24Pos)
{
    EXPECT_EQ(ratl::Sample<float32_t>{1.f / 8388608.f}, ratl::Sample<int24_t>{1});
}

TEST(SampleFloat32, ConvertSampleInt24Neg)
{
    ratl::Sample<int24_t> sample{-1};
    EXPECT_EQ(ratl::Sample<float32_t>{sample}, ratl::Sample<float32_t>{-1.f / 8388608.f});
}

TEST(SampleFloat32, EqualSampleInt24Neg)
{
    EXPECT_EQ(ratl::Sample<float32_t>{-1.f / 8388608.f}, ratl::Sample<int24_t>{-1});
}

TEST(SampleFloat32, ConvertSampleInt32Pos)
{
    ratl::Sample<int32_t> sample{1};
    EXPECT_EQ(ratl::Sample<float32_t>{sample}, ratl::Sample<float32_t>{1.f / 2147483648.f});
}

TEST(SampleFloat32, EqualSampleInt32Pos)
{
    EXPECT_EQ(ratl::Sample<float32_t>{1.f / 2147483648.f}, ratl::Sample<int32_t>{1});
}

TEST(SampleFloat32, ConvertSampleInt32Neg)
{
    ratl::Sample<int32_t> sample{-1};
    EXPECT_EQ(ratl::Sample<float32_t>{sample}, ratl::Sample<float32_t>{-1.f / 2147483648.f});
}

TEST(SampleFloat32, EqualSampleInt32Neg)
{
    EXPECT_EQ(ratl::Sample<float32_t>{-1.f / 2147483648.f}, ratl::Sample<int32_t>{-1});
}

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

// SampleConvertTransparent

template<class Tp>
class SampleConvertTransparent : public ::testing::Test
{
protected:
    using sample_type = Tp;

    static constexpr long min()
    {
        return ratl::SampleTypeLimits<sample_type>::min;
    }

    static constexpr long max()
    {
        return ratl::SampleTypeLimits<sample_type>::max;
    }
};

using SampleConvertTransparentTypes = ::testing::Types<int16_t, int24_t>;

TYPED_TEST_SUITE(SampleConvertTransparent, SampleConvertTransparentTypes, );

TYPED_TEST(SampleConvertTransparent, IntFloatInt)
{
    using sample_type = typename TestFixture::sample_type;
    using sample = ratl::Sample<sample_type>;

    for (long long i = this->min(); i <= this->max(); ++i)
    {
        sample input_sample{i};
        ratl::Sample<float32_t> temp_float{input_sample};
        sample output_sample{temp_float};
        EXPECT_EQ(output_sample, input_sample);
    }
}

// SampleConvertFloatIntWrap

template<class Tp>
class SampleConvertFloatIntNoWrapping : public ::testing::Test
{
protected:
    using sample_type = Tp;
};

using SampleIntTypes = ::testing::Types<int16_t, int24_t, int32_t>;

TYPED_TEST_SUITE(SampleConvertFloatIntNoWrapping, SampleIntTypes, );

TYPED_TEST(SampleConvertFloatIntNoWrapping, Pos)
{
    using sample_type = typename TestFixture::sample_type;
    using sample = ratl::Sample<sample_type>;

    for (float32_t f = 1.1; f > 0.9; f = std::nextafter(f, 0.f))
    {
        ratl::Sample<float32_t> temp{f};
        EXPECT_GT(sample{temp}, sample{0});
    }
}

TYPED_TEST(SampleConvertFloatIntNoWrapping, Neg)
{
    using sample_type = typename TestFixture::sample_type;
    using sample = ratl::Sample<sample_type>;

    for (float32_t f = -1.1; f < -0.9; f = std::nextafter(f, 0.f))
    {
        ratl::Sample<float32_t> temp{f};
        EXPECT_LT(sample{temp}, sample{0});
    }
}

} // namespace ratl
