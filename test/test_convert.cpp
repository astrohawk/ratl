
#include "test_utils.hpp"

namespace ratl
{
// int16

class ConvertInt16 : public ::testing::Test
{
protected:
    ratl::Sample<int16_t> sample_{0x1234};
};

TEST_F(ConvertInt16, Int16)
{
    EXPECT_EQ(ratl::convert<ratl::Sample<int16_t>>(sample_), ratl::Sample<int16_t>{0x1234});
}

TEST_F(ConvertInt16, Int24)
{
    EXPECT_EQ(ratl::convert<ratl::Sample<int24_t>>(sample_), ratl::Sample<int24_t>{0x123400});
}

TEST_F(ConvertInt16, Int32)
{
    EXPECT_EQ(ratl::convert<ratl::Sample<int32_t>>(sample_), ratl::Sample<int32_t>{0x12340000});
}

TEST_F(ConvertInt16, Float32)
{
    uint32_t output = 0x3e11a000;
    float32_t float_output;
    std::memcpy(&float_output, &output, sizeof(float32_t));
    EXPECT_EQ(ratl::convert<ratl::Sample<float32_t>>(sample_), ratl::Sample<float32_t>{float_output});
}

TEST_F(ConvertInt16, NetworkInt16)
{
    EXPECT_EQ(
        ratl::convert<ratl::NetworkSample<int16_t>>(sample_),
        ratl::NetworkSample<int16_t>{ratl::detail::networkUnderlyingToNetworkCast<int16_t>(0x3412)});
}

TEST_F(ConvertInt16, NetworkInt24)
{
    EXPECT_EQ(
        ratl::convert<ratl::NetworkSample<int24_t>>(sample_),
        ratl::NetworkSample<int24_t>{ratl::detail::networkUnderlyingToNetworkCast<int24_t>(0x003412)});
}

TEST_F(ConvertInt16, NetworkInt32)
{
    EXPECT_EQ(
        ratl::convert<ratl::NetworkSample<int32_t>>(sample_),
        ratl::NetworkSample<int32_t>{ratl::detail::networkUnderlyingToNetworkCast<int32_t>(0x00003412)});
}

TEST_F(ConvertInt16, NetworkFloat32)
{
    EXPECT_EQ(
        ratl::convert<ratl::NetworkSample<float32_t>>(sample_),
        ratl::NetworkSample<float32_t>{ratl::detail::networkUnderlyingToNetworkCast<float32_t>(0x00a0113e)});
}

// int24

class ConvertInt24 : public ::testing::Test
{
protected:
    ratl::Sample<int24_t> sample_{0x123456};
};

TEST_F(ConvertInt24, Int16)
{
    EXPECT_EQ(ratl::convert<ratl::Sample<int16_t>>(sample_), ratl::Sample<int16_t>{0x1234});
}

TEST_F(ConvertInt24, Int24)
{
    EXPECT_EQ(ratl::convert<ratl::Sample<int24_t>>(sample_), ratl::Sample<int24_t>{0x123456});
}

TEST_F(ConvertInt24, Int32)
{
    EXPECT_EQ(ratl::convert<ratl::Sample<int32_t>>(sample_), ratl::Sample<int32_t>{0x12345600});
}

TEST_F(ConvertInt24, Float32)
{
    uint32_t output = 0x3e11a2b0;
    float32_t float_output;
    std::memcpy(&float_output, &output, sizeof(float32_t));
    EXPECT_EQ(ratl::convert<ratl::Sample<float32_t>>(sample_), ratl::Sample<float32_t>{float_output});
}

TEST_F(ConvertInt24, NetworkInt16)
{
    EXPECT_EQ(
        ratl::convert<ratl::NetworkSample<int16_t>>(sample_),
        ratl::NetworkSample<int16_t>{ratl::detail::networkUnderlyingToNetworkCast<int16_t>(0x3412)});
}

TEST_F(ConvertInt24, NetworkInt24)
{
    EXPECT_EQ(
        ratl::convert<ratl::NetworkSample<int24_t>>(sample_),
        ratl::NetworkSample<int24_t>{ratl::detail::networkUnderlyingToNetworkCast<int24_t>(0x563412)});
}

TEST_F(ConvertInt24, NetworkInt32)
{
    EXPECT_EQ(
        ratl::convert<ratl::NetworkSample<int32_t>>(sample_),
        ratl::NetworkSample<int32_t>{ratl::detail::networkUnderlyingToNetworkCast<int32_t>(0x00563412)});
}

TEST_F(ConvertInt24, NetworkFloat32)
{
    EXPECT_EQ(
        ratl::convert<ratl::NetworkSample<float32_t>>(sample_),
        ratl::NetworkSample<float32_t>{ratl::detail::networkUnderlyingToNetworkCast<float32_t>(0xb0a2113e)});
}

// int32

class ConvertInt32 : public ::testing::Test
{
protected:
    ratl::Sample<int32_t> sample_{0x12345678};
};

TEST_F(ConvertInt32, Int16)
{
    EXPECT_EQ(ratl::convert<ratl::Sample<int16_t>>(sample_), ratl::Sample<int16_t>{0x1234});
}

TEST_F(ConvertInt32, Int24)
{
    EXPECT_EQ(ratl::convert<ratl::Sample<int24_t>>(sample_), ratl::Sample<int24_t>{0x123456});
}

TEST_F(ConvertInt32, Int32)
{
    EXPECT_EQ(ratl::convert<ratl::Sample<int32_t>>(sample_), ratl::Sample<int32_t>{0x12345678});
}

TEST_F(ConvertInt32, Float32)
{
    uint32_t output = 0x3e11a2b4;
    float32_t float_output;
    std::memcpy(&float_output, &output, sizeof(float32_t));
    EXPECT_EQ(ratl::convert<ratl::Sample<float32_t>>(sample_), ratl::Sample<float32_t>{float_output});
}

TEST_F(ConvertInt32, NetworkInt16)
{
    EXPECT_EQ(
        ratl::convert<ratl::NetworkSample<int16_t>>(sample_),
        ratl::NetworkSample<int16_t>{ratl::detail::networkUnderlyingToNetworkCast<int16_t>(0x3412)});
}

TEST_F(ConvertInt32, NetworkInt24)
{
    EXPECT_EQ(
        ratl::convert<ratl::NetworkSample<int24_t>>(sample_),
        ratl::NetworkSample<int24_t>{ratl::detail::networkUnderlyingToNetworkCast<int24_t>(0x563412)});
}

TEST_F(ConvertInt32, NetworkInt32)
{
    EXPECT_EQ(
        ratl::convert<ratl::NetworkSample<int32_t>>(sample_),
        ratl::NetworkSample<int32_t>{ratl::detail::networkUnderlyingToNetworkCast<int32_t>(0x78563412)});
}

TEST_F(ConvertInt32, NetworkFloat32)
{
    EXPECT_EQ(
        ratl::convert<ratl::NetworkSample<float32_t>>(sample_),
        ratl::NetworkSample<float32_t>{ratl::detail::networkUnderlyingToNetworkCast<float32_t>(0xb4a2113e)});
}

// float32

class ConvertFloat32 : public ::testing::Test
{
protected:
    ratl::Sample<float32_t> sample_{0.1234};
};

TEST_F(ConvertFloat32, NetworkInt16)
{
    EXPECT_EQ(
        ratl::convert<ratl::NetworkSample<int16_t>>(sample_),
        ratl::NetworkSample<int16_t>{ratl::detail::networkUnderlyingToNetworkCast<int16_t>(0xcc0f)});
}

TEST_F(ConvertFloat32, NetworkInt24)
{
    EXPECT_EQ(
        ratl::convert<ratl::NetworkSample<int24_t>>(sample_),
        ratl::NetworkSample<int24_t>{ratl::detail::networkUnderlyingToNetworkCast<int24_t>(0x92cb0f)});
}

TEST_F(ConvertFloat32, NetworkInt32)
{
    EXPECT_EQ(
        ratl::convert<ratl::NetworkSample<int32_t>>(sample_),
        ratl::NetworkSample<int32_t>{ratl::detail::networkUnderlyingToNetworkCast<int32_t>(0x4092cb0f)});
}

TEST_F(ConvertFloat32, NetworkFloat32)
{
    EXPECT_EQ(
        ratl::convert<ratl::NetworkSample<float32_t>>(sample_),
        ratl::NetworkSample<float32_t>{ratl::detail::networkUnderlyingToNetworkCast<float32_t>(0x24b9fc3d)});
}

// network int16

class ConvertNetworkInt16 : public ::testing::Test
{
protected:
    ratl::NetworkSample<int16_t> network_{ratl::detail::networkUnderlyingToNetworkCast<int16_t>(0x3412)};
};

TEST_F(ConvertNetworkInt16, Int16)
{
    EXPECT_EQ(ratl::convert<ratl::Sample<int16_t>>(network_), ratl::Sample<int16_t>{0x1234});
}

TEST_F(ConvertNetworkInt16, Int24)
{
    EXPECT_EQ(ratl::convert<ratl::Sample<int24_t>>(network_), ratl::Sample<int24_t>{0x123400});
}

TEST_F(ConvertNetworkInt16, Int32)
{
    EXPECT_EQ(ratl::convert<ratl::Sample<int32_t>>(network_), ratl::Sample<int32_t>{0x12340000});
}

TEST_F(ConvertNetworkInt16, Float32)
{
    uint32_t output = 0x3e11a000;
    float32_t float_output;
    std::memcpy(&float_output, &output, sizeof(float32_t));
    EXPECT_EQ(ratl::convert<ratl::Sample<float32_t>>(network_), ratl::Sample<float32_t>{float_output});
}

// network int24

class ConvertNetworkInt24 : public ::testing::Test
{
protected:
    ratl::NetworkSample<int24_t> network_{ratl::detail::networkUnderlyingToNetworkCast<int24_t>(0x563412)};
};

TEST_F(ConvertNetworkInt24, Int16)
{
    EXPECT_EQ(ratl::convert<ratl::Sample<int16_t>>(network_), ratl::Sample<int16_t>{0x1234});
}

TEST_F(ConvertNetworkInt24, Int24)
{
    EXPECT_EQ(ratl::convert<ratl::Sample<int24_t>>(network_), ratl::Sample<int24_t>{0x123456});
}

TEST_F(ConvertNetworkInt24, Int32)
{
    EXPECT_EQ(ratl::convert<ratl::Sample<int32_t>>(network_), ratl::Sample<int32_t>{0x12345600});
}

TEST_F(ConvertNetworkInt24, Float32)
{
    uint32_t output = 0x3e11a2b0;
    float32_t float_output;
    std::memcpy(&float_output, &output, sizeof(float32_t));
    EXPECT_EQ(ratl::convert<ratl::Sample<float32_t>>(network_), ratl::Sample<float32_t>{float_output});
}

// network int32

class ConvertNetworkInt32 : public ::testing::Test
{
protected:
    ratl::NetworkSample<int32_t> network_{ratl::detail::networkUnderlyingToNetworkCast<int32_t>(0x78563412)};
};

TEST_F(ConvertNetworkInt32, Int16)
{
    EXPECT_EQ(ratl::convert<ratl::Sample<int16_t>>(network_), ratl::Sample<int16_t>{0x1234});
}

TEST_F(ConvertNetworkInt32, Int24)
{
    EXPECT_EQ(ratl::convert<ratl::Sample<int24_t>>(network_), ratl::Sample<int24_t>{0x123456});
}

TEST_F(ConvertNetworkInt32, Int32)
{
    EXPECT_EQ(ratl::convert<ratl::Sample<int32_t>>(network_), ratl::Sample<int32_t>{0x12345678});
}

TEST_F(ConvertNetworkInt32, Float32)
{
    uint32_t output = 0x3e11a2b4;
    float32_t float_output;
    std::memcpy(&float_output, &output, sizeof(float32_t));
    EXPECT_EQ(ratl::convert<ratl::Sample<float32_t>>(network_), ratl::Sample<float32_t>{float_output});
}

// network float32

class ConvertNetworkFloat32 : public ::testing::Test
{
protected:
    ratl::NetworkSample<float32_t> network_{ratl::detail::networkUnderlyingToNetworkCast<float32_t>(0x24b9fc3d)};
};

TEST_F(ConvertNetworkFloat32, Int16)
{
    EXPECT_EQ(ratl::convert<ratl::Sample<int16_t>>(network_), ratl::Sample<int16_t>{0x0fcc});
}

TEST_F(ConvertNetworkFloat32, Int24)
{
    EXPECT_EQ(ratl::convert<ratl::Sample<int24_t>>(network_), ratl::Sample<int24_t>{0x0fcb92});
}

TEST_F(ConvertNetworkFloat32, Int32)
{
    EXPECT_EQ(ratl::convert<ratl::Sample<int32_t>>(network_), ratl::Sample<int32_t>{0x0fcb9240});
}

TEST_F(ConvertNetworkFloat32, Float32)
{
    EXPECT_EQ(ratl::convert<ratl::Sample<float32_t>>(network_), ratl::Sample<float32_t>{0.1234});
}

// misc
TEST(Converter, Test)
{
    using TestDitherGenerator = ratl::detail::NullDitherGenerator;
    TestDitherGenerator dither_generator;
    auto converter =
        ratl::detail::DefaultConverter<ratl::Sample<int16_t>, ratl::Sample<float32_t>, TestDitherGenerator>{
            dither_generator};
    uint32_t output = 0x3e11a000;
    float32_t float_output;
    std::memcpy(&float_output, &output, sizeof(float32_t));
    EXPECT_EQ(ratl::convert(ratl::Sample<int16_t>{0x1234}, converter), ratl::Sample<float32_t>{float_output});
}

TEST(Convert, CustomConverterFunctor)
{
    auto result = ratl::convert(
        ratl::Sample<int16_t>{0x1234},
        [](const ratl::Sample<int16_t>& sample)
        {
            return ratl::convert<ratl::Sample<float32_t>>(sample);
        });
    uint32_t output = 0x3e11a000;
    float32_t float_output;
    std::memcpy(&float_output, &output, sizeof(float32_t));
    EXPECT_EQ(result, ratl::Sample<float32_t>{float_output});
}

using PossibleOutputSampleTypes = ::testing::Types<int16_t, int24_t, int32_t>;

template<typename OutputSampleTypeArg>
class ConvertFloat32Correct : public ::testing::Test
{
protected:
    using OuputSampleType = OutputSampleTypeArg;
    using OutputSample = ratl::Sample<OuputSampleType>;

    struct ReferenceConverter
    {
        const float32_t sample_abs_max_ = static_cast<float32_t>(std::max(
            std::abs(ratl::SampleTypeLimits<OuputSampleType>::max),
            std::abs(ratl::SampleTypeLimits<OuputSampleType>::min)));

        OutputSample operator()(ratl::Sample<float32_t> input) const noexcept
        {
            float32_t scaled = input.get() * sample_abs_max_;
            if (scaled >= static_cast<float32_t>(ratl::SampleTypeLimits<OuputSampleType>::max))
            {
                return OutputSample{ratl::SampleTypeLimits<OuputSampleType>::max};
            }
            if (scaled < static_cast<float32_t>(ratl::SampleTypeLimits<OuputSampleType>::min))
            {
                return OutputSample{ratl::SampleTypeLimits<OuputSampleType>::min};
            }
            return OutputSample{std::lrint(scaled)};
        }
    };
};

TYPED_TEST_SUITE(ConvertFloat32Correct, PossibleOutputSampleTypes, );

TYPED_TEST(ConvertFloat32Correct, Typical)
{
    using ReferenceConverter = typename TestFixture::ReferenceConverter;
    using OutputSample = typename TestFixture::OutputSample;

    auto min = ratl::detail::FloatConvertTraits<typename TestFixture::OuputSampleType>::Divisor / 10;
    for (float32_t f = 1.1; f > min; f = std::nextafter(f, 0.f))
    {
        auto input_sample = ratl::Sample<float32_t>{f};
        EXPECT_EQ(ratl::convert<OutputSample>(input_sample), ratl::convert(input_sample, ReferenceConverter{}));
    }
    for (float32_t f = -1.1; f < -min; f = std::nextafter(f, 0.f))
    {
        auto input_sample = ratl::Sample<float32_t>{f};
        EXPECT_EQ(ratl::convert<OutputSample>(input_sample), ratl::convert(input_sample, ReferenceConverter{}));
    }
}

template<typename SampleTypeCombinationArg>
class IntInputConvertNegativeTransparent : public ::testing::Test
{
protected:
    using InputSampleType = typename SampleTypeCombinationArg::InputSampleType;
    using OutputSampleType = typename SampleTypeCombinationArg::OutputSampleType;
};

TYPED_TEST_SUITE(IntInputConvertNegativeTransparent, PossibleIntInputSampleTypeCombinations, );

TYPED_TEST(IntInputConvertNegativeTransparent, IntInputConvert)
{
    using InputSample = ratl::Sample<typename TestFixture::InputSampleType>;
    using InputSampleTypeLimits = ratl::SampleTypeLimits<typename TestFixture::InputSampleType>;
    using OutputSample = ratl::Sample<typename TestFixture::OutputSampleType>;
    using OutputSampleTypeLimits = ratl::SampleTypeLimits<typename TestFixture::OutputSampleType>;
    for (std::int64_t i = 0; i < InputSampleTypeLimits::max; ++i)
    {
        auto sample_value = static_cast<typename TestFixture::InputSampleType>(i);
        auto positive_input = InputSample(sample_value);
        auto positive_output = ratl::convert<OutputSample>(positive_input);
        auto negative_input = InputSample(-sample_value);
        auto negative_output = ratl::convert<OutputSample>(negative_input);
        if (negative_output.get() != OutputSampleTypeLimits::min)
        {
            EXPECT_EQ(positive_output.get(), -negative_output.get());
        }
    }
}

template<typename OutputSampleTypeArg>
class FloatInputConvertNegativeTransparent : public ::testing::Test
{
protected:
    using OutputSampleType = OutputSampleTypeArg;
};

TYPED_TEST_SUITE(FloatInputConvertNegativeTransparent, PossibleSampleTypes, );

TYPED_TEST(FloatInputConvertNegativeTransparent, FloatInputConvert)
{
    using InputSample = ratl::Sample<ratl::float32_t>;
    using OutputSample = ratl::Sample<typename TestFixture::OutputSampleType>;
    using Int32SampleTypeLimits = ratl::SampleTypeLimits<ratl::int32_t>;
    using OutputSampleTypeLimits = ratl::SampleTypeLimits<typename TestFixture::OutputSampleType>;
    for (std::int64_t i = 0; i < Int32SampleTypeLimits::max; ++i)
    {
        auto sample_value = ratl::convert<InputSample>(ratl::Sample<ratl::int32_t>(static_cast<ratl::int32_t>(i))).get();
        auto positive_input = InputSample(sample_value);
        auto positive_output = ratl::convert<OutputSample>(positive_input);
        auto negative_input = InputSample(-sample_value);
        auto negative_output = ratl::convert<OutputSample>(negative_input);
        if (negative_output.get() != OutputSampleTypeLimits::min)
        {
            EXPECT_EQ(positive_output.get(), -negative_output.get());
        }
    }
}

} // namespace ratl
