
#include "test_utils.hpp"

#include <cstring>

namespace ratl
{
namespace test
{
// int16

class ConvertInt16 : public ::testing::Test
{
protected:
    Sample<int16_t> sample_{0x1234};
};

TEST_F(ConvertInt16, Int16)
{
    EXPECT_EQ(referenceConvert<Sample<int16_t>>(sample_), Sample<int16_t>{0x1234});
}

TEST_F(ConvertInt16, Int24)
{
    EXPECT_EQ(referenceConvert<Sample<int24_t>>(sample_), Sample<int24_t>{0x123400});
}

TEST_F(ConvertInt16, Int32)
{
    EXPECT_EQ(referenceConvert<Sample<int32_t>>(sample_), Sample<int32_t>{0x12340000});
}

TEST_F(ConvertInt16, Float32)
{
    uint32_t output = 0x3e11a000;
    float32_t float_output;
    std::memcpy(&float_output, &output, sizeof(float32_t));
    EXPECT_EQ(referenceConvert<Sample<float32_t>>(sample_), Sample<float32_t>{float_output});
}

TEST_F(ConvertInt16, NetworkInt16)
{
    EXPECT_EQ(
        referenceConvert<NetworkSample<int16_t>>(sample_),
        NetworkSample<int16_t>{detail::networkUnderlyingToNetworkCast<int16_t>(0x3412)});
}

TEST_F(ConvertInt16, NetworkInt24)
{
    EXPECT_EQ(
        referenceConvert<NetworkSample<int24_t>>(sample_),
        NetworkSample<int24_t>{detail::networkUnderlyingToNetworkCast<int24_t>(0x003412)});
}

TEST_F(ConvertInt16, NetworkInt32)
{
    EXPECT_EQ(
        referenceConvert<NetworkSample<int32_t>>(sample_),
        NetworkSample<int32_t>{detail::networkUnderlyingToNetworkCast<int32_t>(0x00003412)});
}

TEST_F(ConvertInt16, NetworkFloat32)
{
    EXPECT_EQ(
        referenceConvert<NetworkSample<float32_t>>(sample_),
        NetworkSample<float32_t>{detail::networkUnderlyingToNetworkCast<float32_t>(0x00a0113e)});
}

// int24

class ConvertInt24 : public ::testing::Test
{
protected:
    Sample<int24_t> sample_{0x123456};
};

TEST_F(ConvertInt24, Int16)
{
    EXPECT_EQ(referenceConvert<Sample<int16_t>>(sample_), Sample<int16_t>{0x1234});
}

TEST_F(ConvertInt24, Int24)
{
    EXPECT_EQ(referenceConvert<Sample<int24_t>>(sample_), Sample<int24_t>{0x123456});
}

TEST_F(ConvertInt24, Int32)
{
    EXPECT_EQ(referenceConvert<Sample<int32_t>>(sample_), Sample<int32_t>{0x12345600});
}

TEST_F(ConvertInt24, Float32)
{
    uint32_t output = 0x3e11a2b0;
    float32_t float_output;
    std::memcpy(&float_output, &output, sizeof(float32_t));
    EXPECT_EQ(referenceConvert<Sample<float32_t>>(sample_), Sample<float32_t>{float_output});
}

TEST_F(ConvertInt24, NetworkInt16)
{
    EXPECT_EQ(
        referenceConvert<NetworkSample<int16_t>>(sample_),
        NetworkSample<int16_t>{detail::networkUnderlyingToNetworkCast<int16_t>(0x3412)});
}

TEST_F(ConvertInt24, NetworkInt24)
{
    EXPECT_EQ(
        referenceConvert<NetworkSample<int24_t>>(sample_),
        NetworkSample<int24_t>{detail::networkUnderlyingToNetworkCast<int24_t>(0x563412)});
}

TEST_F(ConvertInt24, NetworkInt32)
{
    EXPECT_EQ(
        referenceConvert<NetworkSample<int32_t>>(sample_),
        NetworkSample<int32_t>{detail::networkUnderlyingToNetworkCast<int32_t>(0x00563412)});
}

TEST_F(ConvertInt24, NetworkFloat32)
{
    EXPECT_EQ(
        referenceConvert<NetworkSample<float32_t>>(sample_),
        NetworkSample<float32_t>{detail::networkUnderlyingToNetworkCast<float32_t>(0xb0a2113e)});
}

// int32

class ConvertInt32 : public ::testing::Test
{
protected:
    Sample<int32_t> sample_{0x12345678};
};

TEST_F(ConvertInt32, Int16)
{
    EXPECT_EQ(referenceConvert<Sample<int16_t>>(sample_), Sample<int16_t>{0x1234});
}

TEST_F(ConvertInt32, Int24)
{
    EXPECT_EQ(referenceConvert<Sample<int24_t>>(sample_), Sample<int24_t>{0x123456});
}

TEST_F(ConvertInt32, Int32)
{
    EXPECT_EQ(referenceConvert<Sample<int32_t>>(sample_), Sample<int32_t>{0x12345678});
}

TEST_F(ConvertInt32, Float32)
{
    uint32_t output = 0x3e11a2b4;
    float32_t float_output;
    std::memcpy(&float_output, &output, sizeof(float32_t));
    EXPECT_EQ(referenceConvert<Sample<float32_t>>(sample_), Sample<float32_t>{float_output});
}

TEST_F(ConvertInt32, NetworkInt16)
{
    EXPECT_EQ(
        referenceConvert<NetworkSample<int16_t>>(sample_),
        NetworkSample<int16_t>{detail::networkUnderlyingToNetworkCast<int16_t>(0x3412)});
}

TEST_F(ConvertInt32, NetworkInt24)
{
    EXPECT_EQ(
        referenceConvert<NetworkSample<int24_t>>(sample_),
        NetworkSample<int24_t>{detail::networkUnderlyingToNetworkCast<int24_t>(0x563412)});
}

TEST_F(ConvertInt32, NetworkInt32)
{
    EXPECT_EQ(
        referenceConvert<NetworkSample<int32_t>>(sample_),
        NetworkSample<int32_t>{detail::networkUnderlyingToNetworkCast<int32_t>(0x78563412)});
}

TEST_F(ConvertInt32, NetworkFloat32)
{
    EXPECT_EQ(
        referenceConvert<NetworkSample<float32_t>>(sample_),
        NetworkSample<float32_t>{detail::networkUnderlyingToNetworkCast<float32_t>(0xb4a2113e)});
}

// float32

class ConvertFloat32 : public ::testing::Test
{
protected:
    Sample<float32_t> sample_{0.1234};
};

TEST_F(ConvertFloat32, NetworkInt16)
{
    EXPECT_EQ(
        referenceConvert<NetworkSample<int16_t>>(sample_),
        NetworkSample<int16_t>{detail::networkUnderlyingToNetworkCast<int16_t>(0xcc0f)});
}

TEST_F(ConvertFloat32, NetworkInt24)
{
    EXPECT_EQ(
        referenceConvert<NetworkSample<int24_t>>(sample_),
        NetworkSample<int24_t>{detail::networkUnderlyingToNetworkCast<int24_t>(0x92cb0f)});
}

TEST_F(ConvertFloat32, NetworkInt32)
{
    EXPECT_EQ(
        referenceConvert<NetworkSample<int32_t>>(sample_),
        NetworkSample<int32_t>{detail::networkUnderlyingToNetworkCast<int32_t>(0x4092cb0f)});
}

TEST_F(ConvertFloat32, NetworkFloat32)
{
    EXPECT_EQ(
        referenceConvert<NetworkSample<float32_t>>(sample_),
        NetworkSample<float32_t>{detail::networkUnderlyingToNetworkCast<float32_t>(0x24b9fc3d)});
}

// network int16

class ConvertNetworkInt16 : public ::testing::Test
{
protected:
    NetworkSample<int16_t> network_{detail::networkUnderlyingToNetworkCast<int16_t>(0x3412)};
};

TEST_F(ConvertNetworkInt16, Int16)
{
    EXPECT_EQ(referenceConvert<Sample<int16_t>>(network_), Sample<int16_t>{0x1234});
}

TEST_F(ConvertNetworkInt16, Int24)
{
    EXPECT_EQ(referenceConvert<Sample<int24_t>>(network_), Sample<int24_t>{0x123400});
}

TEST_F(ConvertNetworkInt16, Int32)
{
    EXPECT_EQ(referenceConvert<Sample<int32_t>>(network_), Sample<int32_t>{0x12340000});
}

TEST_F(ConvertNetworkInt16, Float32)
{
    uint32_t output = 0x3e11a000;
    float32_t float_output;
    std::memcpy(&float_output, &output, sizeof(float32_t));
    EXPECT_EQ(referenceConvert<Sample<float32_t>>(network_), Sample<float32_t>{float_output});
}

// network int24

class ConvertNetworkInt24 : public ::testing::Test
{
protected:
    NetworkSample<int24_t> network_{detail::networkUnderlyingToNetworkCast<int24_t>(0x563412)};
};

TEST_F(ConvertNetworkInt24, Int16)
{
    EXPECT_EQ(referenceConvert<Sample<int16_t>>(network_), Sample<int16_t>{0x1234});
}

TEST_F(ConvertNetworkInt24, Int24)
{
    EXPECT_EQ(referenceConvert<Sample<int24_t>>(network_), Sample<int24_t>{0x123456});
}

TEST_F(ConvertNetworkInt24, Int32)
{
    EXPECT_EQ(referenceConvert<Sample<int32_t>>(network_), Sample<int32_t>{0x12345600});
}

TEST_F(ConvertNetworkInt24, Float32)
{
    uint32_t output = 0x3e11a2b0;
    float32_t float_output;
    std::memcpy(&float_output, &output, sizeof(float32_t));
    EXPECT_EQ(referenceConvert<Sample<float32_t>>(network_), Sample<float32_t>{float_output});
}

// network int32

class ConvertNetworkInt32 : public ::testing::Test
{
protected:
    NetworkSample<int32_t> network_{detail::networkUnderlyingToNetworkCast<int32_t>(0x78563412)};
};

TEST_F(ConvertNetworkInt32, Int16)
{
    EXPECT_EQ(referenceConvert<Sample<int16_t>>(network_), Sample<int16_t>{0x1234});
}

TEST_F(ConvertNetworkInt32, Int24)
{
    EXPECT_EQ(referenceConvert<Sample<int24_t>>(network_), Sample<int24_t>{0x123456});
}

TEST_F(ConvertNetworkInt32, Int32)
{
    EXPECT_EQ(referenceConvert<Sample<int32_t>>(network_), Sample<int32_t>{0x12345678});
}

TEST_F(ConvertNetworkInt32, Float32)
{
    uint32_t output = 0x3e11a2b4;
    float32_t float_output;
    std::memcpy(&float_output, &output, sizeof(float32_t));
    EXPECT_EQ(referenceConvert<Sample<float32_t>>(network_), Sample<float32_t>{float_output});
}

// network float32

class ConvertNetworkFloat32 : public ::testing::Test
{
protected:
    NetworkSample<float32_t> network_{detail::networkUnderlyingToNetworkCast<float32_t>(0x24b9fc3d)};
};

TEST_F(ConvertNetworkFloat32, Int16)
{
    EXPECT_EQ(referenceConvert<Sample<int16_t>>(network_), Sample<int16_t>{0x0fcc});
}

TEST_F(ConvertNetworkFloat32, Int24)
{
    EXPECT_EQ(referenceConvert<Sample<int24_t>>(network_), Sample<int24_t>{0x0fcb92});
}

TEST_F(ConvertNetworkFloat32, Int32)
{
    EXPECT_EQ(referenceConvert<Sample<int32_t>>(network_), Sample<int32_t>{0x0fcb9240});
}

TEST_F(ConvertNetworkFloat32, Float32)
{
    EXPECT_EQ(referenceConvert<Sample<float32_t>>(network_), Sample<float32_t>{0.1234});
}

// misc
TEST(Converter, Test)
{
    using TestDitherGenerator = detail::NullDitherGenerator;
    TestDitherGenerator dither_generator;
    auto converter =
        detail::DefaultSampleConverter<Sample<int16_t>, Sample<float32_t>, TestDitherGenerator>{dither_generator};
    uint32_t output = 0x3e11a000;
    float32_t float_output;
    std::memcpy(&float_output, &output, sizeof(float32_t));
    EXPECT_EQ(convert(Sample<int16_t>{0x1234}, converter), Sample<float32_t>{float_output});
}

TEST(Convert, CustomConverterFunctor)
{
    auto result = convert(
        Sample<int16_t>{0x1234},
        [](const Sample<int16_t>& sample)
        {
            return convert<Sample<float32_t>>(sample);
        });
    uint32_t output = 0x3e11a000;
    float32_t float_output;
    std::memcpy(&float_output, &output, sizeof(float32_t));
    EXPECT_EQ(result, Sample<float32_t>{float_output});
}

// ConvertFloat32Correct

template<typename OutputSampleTypeArg>
class ConvertFloat32Correct : public ::testing::Test
{
protected:
    using OuputSampleType = OutputSampleTypeArg;
    using OutputSample = Sample<OuputSampleType>;

    struct ReferenceConverter
    {
        const float32_t sample_abs_max_ = static_cast<float32_t>(std::max(
            std::abs(SampleTypeLimits<OuputSampleType>::max), std::abs(SampleTypeLimits<OuputSampleType>::min)));

        OutputSample operator()(Sample<float32_t> input) const noexcept
        {
            float32_t scaled = input.get() * sample_abs_max_;
            if (scaled >= static_cast<float32_t>(SampleTypeLimits<OuputSampleType>::max))
            {
                return OutputSample{SampleTypeLimits<OuputSampleType>::max};
            }
            if (scaled < static_cast<float32_t>(SampleTypeLimits<OuputSampleType>::min))
            {
                return OutputSample{SampleTypeLimits<OuputSampleType>::min};
            }
            return OutputSample{std::lrint(scaled)};
        }
    };
};

TYPED_TEST_SUITE(ConvertFloat32Correct, PossibleIntSampleTypes, );

TYPED_TEST(ConvertFloat32Correct, Typical)
{
    using ReferenceConverter = typename TestFixture::ReferenceConverter;
    using OutputSample = typename TestFixture::OutputSample;

    auto min = detail::FloatConvertTraits<typename TestFixture::OuputSampleType>::Divisor / 10;
    for (float32_t f = 1.1; f > min; f = std::nextafter(f, 0.f))
    {
        auto input_sample = Sample<float32_t>{f};
        EXPECT_EQ(referenceConvert<OutputSample>(input_sample), convert(input_sample, ReferenceConverter{}));
    }
    for (float32_t f = -1.1; f < -min; f = std::nextafter(f, 0.f))
    {
        auto input_sample = Sample<float32_t>{f};
        EXPECT_EQ(referenceConvert<OutputSample>(input_sample), convert(input_sample, ReferenceConverter{}));
    }
}

// ConvertFloat32Transparent

template<class SampleTypeArg>
class ConvertFloat32Transparent : public ::testing::Test
{
protected:
    using SampleType = SampleTypeArg;

    static constexpr long min()
    {
        return SampleTypeLimits<SampleType>::min;
    }

    static constexpr long max()
    {
        return SampleTypeLimits<SampleType>::max;
    }
};

using ConvertFloat32TransparentTypes = ::testing::Types<int16_t, int24_t>;

TYPED_TEST_SUITE(ConvertFloat32Transparent, ConvertFloat32TransparentTypes, );

TYPED_TEST(ConvertFloat32Transparent, Typical)
{
    using InputSample = Sample<typename TestFixture::SampleType>;

    for (long long i = this->min(); i <= this->max(); ++i)
    {
        InputSample input_sample{i};
        auto temp_float = referenceConvert<Sample<float32_t>>(input_sample);
        auto output_sample = referenceConvert<InputSample>(temp_float);
        EXPECT_EQ(output_sample, input_sample);
    }
}

// ConvertFloatIntNoWrapping

template<class SampleTypeArg>
class ConvertFloatIntNoWrapping : public ::testing::Test
{
protected:
    using SampleType = SampleTypeArg;
};

TYPED_TEST_SUITE(ConvertFloatIntNoWrapping, PossibleIntSampleTypes, );

TYPED_TEST(ConvertFloatIntNoWrapping, ConvertFloatIntNoWrapping)
{
    using OutputSample = Sample<typename TestFixture::SampleType>;

    for (float32_t f = 1.1; f > 0.9; f = std::nextafter(f, 0.f))
    {
        EXPECT_GT(referenceConvert<OutputSample>(Sample<float32_t>{f}), OutputSample{0});
    }
    for (float32_t f = -1.1; f < -0.9; f = std::nextafter(f, 0.f))
    {
        EXPECT_LT(referenceConvert<OutputSample>(Sample<float32_t>{f}), OutputSample{0});
    }
}

// ConvertIntInputNegativeTransparent

template<typename SampleTypeCombinationArg>
class ConvertIntInputNegativeTransparent : public ::testing::Test
{
protected:
    using InputSampleType = typename SampleTypeCombinationArg::InputSampleType;
    using OutputSampleType = typename SampleTypeCombinationArg::OutputSampleType;
};

TYPED_TEST_SUITE(ConvertIntInputNegativeTransparent, PossibleIntInputSampleTypeCombinations, );

TYPED_TEST(ConvertIntInputNegativeTransparent, IntInputConvert)
{
    using InputSample = Sample<typename TestFixture::InputSampleType>;
    using InputSampleTypeLimits = SampleTypeLimits<typename TestFixture::InputSampleType>;
    using OutputSample = Sample<typename TestFixture::OutputSampleType>;
    using OutputSampleTypeLimits = SampleTypeLimits<typename TestFixture::OutputSampleType>;
    for (std::int64_t i = 0; i < InputSampleTypeLimits::max; ++i)
    {
        auto sample_value = static_cast<typename TestFixture::InputSampleType>(i);
        auto positive_input = InputSample(sample_value);
        auto positive_output = referenceConvert<OutputSample>(positive_input);
        auto negative_input = InputSample(-sample_value);
        auto negative_output = referenceConvert<OutputSample>(negative_input);
        if (negative_output.get() != OutputSampleTypeLimits::min)
        {
            EXPECT_EQ(positive_output.get(), -negative_output.get());
        }
    }
}

// ConvertFloatInputNegativeTransparent

template<typename OutputSampleTypeArg>
class ConvertFloatInputNegativeTransparent : public ::testing::Test
{
protected:
    using OutputSampleType = OutputSampleTypeArg;
};

TYPED_TEST_SUITE(ConvertFloatInputNegativeTransparent, PossibleSampleTypes, );

TYPED_TEST(ConvertFloatInputNegativeTransparent, FloatInputConvert)
{
    using InputSample = Sample<float32_t>;
    using OutputSample = Sample<typename TestFixture::OutputSampleType>;
    using Int32SampleTypeLimits = SampleTypeLimits<int32_t>;
    using OutputSampleTypeLimits = SampleTypeLimits<typename TestFixture::OutputSampleType>;
    for (std::int64_t i = 0; i < Int32SampleTypeLimits::max; ++i)
    {
        auto sample_value = referenceConvert<InputSample>(Sample<int32_t>(static_cast<int32_t>(i))).get();
        auto positive_input = InputSample(sample_value);
        auto positive_output = referenceConvert<OutputSample>(positive_input);
        auto negative_input = InputSample(-sample_value);
        auto negative_output = referenceConvert<OutputSample>(negative_input);
        if (negative_output.get() != OutputSampleTypeLimits::min)
        {
            EXPECT_EQ(positive_output.get(), -negative_output.get());
        }
    }
}

} // namespace test
} // namespace ratl
