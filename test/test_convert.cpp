/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

// ratl test includes
#include "test_utils.hpp"

// other includes
#include <cstring>

namespace ratl
{
namespace test
{
// int16

class ConvertInt16 : public ::testing::Test
{
protected:
    sample<int16_t> sample_{0x1234};
};

TEST_F(ConvertInt16, Int16)
{
    EXPECT_EQ(reference_convert<sample<int16_t>>(sample_), sample<int16_t>(0x1234));
}

TEST_F(ConvertInt16, Int24)
{
    EXPECT_EQ(reference_convert<sample<int24_t>>(sample_), sample<int24_t>(0x123400));
}

TEST_F(ConvertInt16, Int32)
{
    EXPECT_EQ(reference_convert<sample<int32_t>>(sample_), sample<int32_t>(0x12340000));
}

TEST_F(ConvertInt16, Float32)
{
    uint32_t output = 0x3e11a000;
    float32_t float_output;
    std::memcpy(&float_output, &output, sizeof(float32_t));
    EXPECT_EQ(reference_convert<sample<float32_t>>(sample_), sample<float32_t>(float_output));
}

TEST_F(ConvertInt16, NetworkInt16)
{
    EXPECT_EQ(
        reference_convert<network_sample<int16_t>>(sample_),
        network_sample<int16_t>(detail::network_underlying_to_network_cast<int16_t>(0x3412)));
}

TEST_F(ConvertInt16, NetworkInt24)
{
    EXPECT_EQ(
        reference_convert<network_sample<int24_t>>(sample_),
        network_sample<int24_t>(detail::network_underlying_to_network_cast<int24_t>(0x003412)));
}

TEST_F(ConvertInt16, NetworkInt32)
{
    EXPECT_EQ(
        reference_convert<network_sample<int32_t>>(sample_),
        network_sample<int32_t>(detail::network_underlying_to_network_cast<int32_t>(0x00003412)));
}

TEST_F(ConvertInt16, NetworkFloat32)
{
    EXPECT_EQ(
        reference_convert<network_sample<float32_t>>(sample_),
        network_sample<float32_t>(detail::network_underlying_to_network_cast<float32_t>(0x00a0113e)));
}

// int24

class ConvertInt24 : public ::testing::Test
{
protected:
    sample<int24_t> sample_{0x123456};
};

TEST_F(ConvertInt24, Int16)
{
    EXPECT_EQ(reference_convert<sample<int16_t>>(sample_), sample<int16_t>(0x1234));
}

TEST_F(ConvertInt24, Int24)
{
    EXPECT_EQ(reference_convert<sample<int24_t>>(sample_), sample<int24_t>(0x123456));
}

TEST_F(ConvertInt24, Int32)
{
    EXPECT_EQ(reference_convert<sample<int32_t>>(sample_), sample<int32_t>(0x12345600));
}

TEST_F(ConvertInt24, Float32)
{
    uint32_t output = 0x3e11a2b0;
    float32_t float_output;
    std::memcpy(&float_output, &output, sizeof(float32_t));
    EXPECT_EQ(reference_convert<sample<float32_t>>(sample_), sample<float32_t>(float_output));
}

TEST_F(ConvertInt24, NetworkInt16)
{
    EXPECT_EQ(
        reference_convert<network_sample<int16_t>>(sample_),
        network_sample<int16_t>(detail::network_underlying_to_network_cast<int16_t>(0x3412)));
}

TEST_F(ConvertInt24, NetworkInt24)
{
    EXPECT_EQ(
        reference_convert<network_sample<int24_t>>(sample_),
        network_sample<int24_t>(detail::network_underlying_to_network_cast<int24_t>(0x563412)));
}

TEST_F(ConvertInt24, NetworkInt32)
{
    EXPECT_EQ(
        reference_convert<network_sample<int32_t>>(sample_),
        network_sample<int32_t>(detail::network_underlying_to_network_cast<int32_t>(0x00563412)));
}

TEST_F(ConvertInt24, NetworkFloat32)
{
    EXPECT_EQ(
        reference_convert<network_sample<float32_t>>(sample_),
        network_sample<float32_t>(detail::network_underlying_to_network_cast<float32_t>(0xb0a2113e)));
}

// int32

class ConvertInt32 : public ::testing::Test
{
protected:
    sample<int32_t> sample_{0x12345678};
};

TEST_F(ConvertInt32, Int16)
{
    EXPECT_EQ(reference_convert<sample<int16_t>>(sample_), sample<int16_t>(0x1234));
}

TEST_F(ConvertInt32, Int24)
{
    EXPECT_EQ(reference_convert<sample<int24_t>>(sample_), sample<int24_t>(0x123456));
}

TEST_F(ConvertInt32, Int32)
{
    EXPECT_EQ(reference_convert<sample<int32_t>>(sample_), sample<int32_t>(0x12345678));
}

TEST_F(ConvertInt32, Float32)
{
    uint32_t output = 0x3e11a2b4;
    float32_t float_output;
    std::memcpy(&float_output, &output, sizeof(float32_t));
    EXPECT_EQ(reference_convert<sample<float32_t>>(sample_), sample<float32_t>(float_output));
}

TEST_F(ConvertInt32, NetworkInt16)
{
    EXPECT_EQ(
        reference_convert<network_sample<int16_t>>(sample_),
        network_sample<int16_t>(detail::network_underlying_to_network_cast<int16_t>(0x3412)));
}

TEST_F(ConvertInt32, NetworkInt24)
{
    EXPECT_EQ(
        reference_convert<network_sample<int24_t>>(sample_),
        network_sample<int24_t>(detail::network_underlying_to_network_cast<int24_t>(0x563412)));
}

TEST_F(ConvertInt32, NetworkInt32)
{
    EXPECT_EQ(
        reference_convert<network_sample<int32_t>>(sample_),
        network_sample<int32_t>(detail::network_underlying_to_network_cast<int32_t>(0x78563412)));
}

TEST_F(ConvertInt32, NetworkFloat32)
{
    EXPECT_EQ(
        reference_convert<network_sample<float32_t>>(sample_),
        network_sample<float32_t>(detail::network_underlying_to_network_cast<float32_t>(0xb4a2113e)));
}

// float32

class ConvertFloat32 : public ::testing::Test
{
protected:
    sample<float32_t> sample_{0.1234};
};

TEST_F(ConvertFloat32, NetworkInt16)
{
    EXPECT_EQ(
        reference_convert<network_sample<int16_t>>(sample_),
        network_sample<int16_t>(detail::network_underlying_to_network_cast<int16_t>(0xcc0f)));
}

TEST_F(ConvertFloat32, NetworkInt24)
{
    EXPECT_EQ(
        reference_convert<network_sample<int24_t>>(sample_),
        network_sample<int24_t>(detail::network_underlying_to_network_cast<int24_t>(0x92cb0f)));
}

TEST_F(ConvertFloat32, NetworkInt32)
{
    EXPECT_EQ(
        reference_convert<network_sample<int32_t>>(sample_),
        network_sample<int32_t>(detail::network_underlying_to_network_cast<int32_t>(0x4092cb0f)));
}

TEST_F(ConvertFloat32, NetworkFloat32)
{
    EXPECT_EQ(
        reference_convert<network_sample<float32_t>>(sample_),
        network_sample<float32_t>(detail::network_underlying_to_network_cast<float32_t>(0x24b9fc3d)));
}

// network int16

class ConvertNetworkInt16 : public ::testing::Test
{
protected:
    network_sample<int16_t> network_{detail::network_underlying_to_network_cast<int16_t>(0x3412)};
};

TEST_F(ConvertNetworkInt16, Int16)
{
    EXPECT_EQ(reference_convert<sample<int16_t>>(network_), sample<int16_t>(0x1234));
}

TEST_F(ConvertNetworkInt16, Int24)
{
    EXPECT_EQ(reference_convert<sample<int24_t>>(network_), sample<int24_t>(0x123400));
}

TEST_F(ConvertNetworkInt16, Int32)
{
    EXPECT_EQ(reference_convert<sample<int32_t>>(network_), sample<int32_t>(0x12340000));
}

TEST_F(ConvertNetworkInt16, Float32)
{
    uint32_t output = 0x3e11a000;
    float32_t float_output;
    std::memcpy(&float_output, &output, sizeof(float32_t));
    EXPECT_EQ(reference_convert<sample<float32_t>>(network_), sample<float32_t>(float_output));
}

// network int24

class ConvertNetworkInt24 : public ::testing::Test
{
protected:
    network_sample<int24_t> network_{detail::network_underlying_to_network_cast<int24_t>(0x563412)};
};

TEST_F(ConvertNetworkInt24, Int16)
{
    EXPECT_EQ(reference_convert<sample<int16_t>>(network_), sample<int16_t>(0x1234));
}

TEST_F(ConvertNetworkInt24, Int24)
{
    EXPECT_EQ(reference_convert<sample<int24_t>>(network_), sample<int24_t>(0x123456));
}

TEST_F(ConvertNetworkInt24, Int32)
{
    EXPECT_EQ(reference_convert<sample<int32_t>>(network_), sample<int32_t>(0x12345600));
}

TEST_F(ConvertNetworkInt24, Float32)
{
    uint32_t output = 0x3e11a2b0;
    float32_t float_output;
    std::memcpy(&float_output, &output, sizeof(float32_t));
    EXPECT_EQ(reference_convert<sample<float32_t>>(network_), sample<float32_t>(float_output));
}

// network int32

class ConvertNetworkInt32 : public ::testing::Test
{
protected:
    network_sample<int32_t> network_{detail::network_underlying_to_network_cast<int32_t>(0x78563412)};
};

TEST_F(ConvertNetworkInt32, Int16)
{
    EXPECT_EQ(reference_convert<sample<int16_t>>(network_), sample<int16_t>(0x1234));
}

TEST_F(ConvertNetworkInt32, Int24)
{
    EXPECT_EQ(reference_convert<sample<int24_t>>(network_), sample<int24_t>(0x123456));
}

TEST_F(ConvertNetworkInt32, Int32)
{
    EXPECT_EQ(reference_convert<sample<int32_t>>(network_), sample<int32_t>(0x12345678));
}

TEST_F(ConvertNetworkInt32, Float32)
{
    uint32_t output = 0x3e11a2b4;
    float32_t float_output;
    std::memcpy(&float_output, &output, sizeof(float32_t));
    EXPECT_EQ(reference_convert<sample<float32_t>>(network_), sample<float32_t>(float_output));
}

// network float32

class ConvertNetworkFloat32 : public ::testing::Test
{
protected:
    network_sample<float32_t> network_{detail::network_underlying_to_network_cast<float32_t>(0x24b9fc3d)};
};

TEST_F(ConvertNetworkFloat32, Int16)
{
    EXPECT_EQ(reference_convert<sample<int16_t>>(network_), sample<int16_t>(0x0fcc));
}

TEST_F(ConvertNetworkFloat32, Int24)
{
    EXPECT_EQ(reference_convert<sample<int24_t>>(network_), sample<int24_t>(0x0fcb92));
}

TEST_F(ConvertNetworkFloat32, Int32)
{
    EXPECT_EQ(reference_convert<sample<int32_t>>(network_), sample<int32_t>(0x0fcb9240));
}

TEST_F(ConvertNetworkFloat32, Float32)
{
    EXPECT_EQ(reference_convert<sample<float32_t>>(network_), sample<float32_t>(0.1234));
}

// misc
TEST(Converter, Test)
{
    using test_dither_generator = detail::null_dither_generator;
    test_dither_generator dither_gen;
    auto converter =
        detail::default_sample_converter<sample<int16_t>, sample<float32_t>, test_dither_generator>(dither_gen);
    uint32_t output = 0x3e11a000;
    float32_t float_output;
    std::memcpy(&float_output, &output, sizeof(float32_t));
    EXPECT_EQ(convert(sample<int16_t>(0x1234), converter), sample<float32_t>(float_output));
}

TEST(Convert, CustomConverterFunctor)
{
    auto result = convert(
        sample<int16_t>(0x1234),
        [](const sample<int16_t>& input)
        {
            return convert<sample<float32_t>>(input);
        });
    uint32_t output = 0x3e11a000;
    float32_t float_output;
    std::memcpy(&float_output, &output, sizeof(float32_t));
    EXPECT_EQ(result, sample<float32_t>(float_output));
}

// ConvertFloat32Correct

template<typename OutputSampleTypeArg>
class ConvertFloat32Correct : public ::testing::Test
{
protected:
    using OuputSampleType = OutputSampleTypeArg;
    using output_sample = sample<OuputSampleType>;

    struct reference_converter
    {
        const float32_t sample_abs_max_ = static_cast<float32_t>(
            std::max(std::abs(sample_limits<OuputSampleType>::max), std::abs(sample_limits<OuputSampleType>::min)));

        output_sample operator()(sample<float32_t> input) const noexcept
        {
            float32_t scaled = input.get() * sample_abs_max_;
            if (scaled >= static_cast<float32_t>(sample_limits<OuputSampleType>::max))
            {
                return output_sample(sample_limits<OuputSampleType>::max);
            }
            if (scaled < static_cast<float32_t>(sample_limits<OuputSampleType>::min))
            {
                return output_sample(sample_limits<OuputSampleType>::min);
            }
            return output_sample(std::lrint(scaled));
        }
    };
};

TYPED_TEST_SUITE(ConvertFloat32Correct, PossibleIntSampleTypes, );

TYPED_TEST(ConvertFloat32Correct, Typical)
{
    using reference_converter = typename TestFixture::reference_converter;
    using output_sample = typename TestFixture::output_sample;

    auto min = detail::float_convert_traits<typename TestFixture::OuputSampleType>::divisor / 10;
    for (float32_t f = 1.1; f > min; f = std::nextafter(f, 0.f))
    {
        auto input_sample = sample<float32_t>(f);
        EXPECT_EQ(reference_convert<output_sample>(input_sample), convert(input_sample, reference_converter{}));
    }
    for (float32_t f = -1.1; f < -min; f = std::nextafter(f, 0.f))
    {
        auto input_sample = sample<float32_t>(f);
        EXPECT_EQ(reference_convert<output_sample>(input_sample), convert(input_sample, reference_converter{}));
    }
}

// ConvertFloat32Transparent

template<typename SampleTypeArg>
class ConvertFloat32Transparent : public ::testing::Test
{
protected:
    using sample_type = SampleTypeArg;

    static constexpr long min()
    {
        return sample_limits<sample_type>::min;
    }

    static constexpr long max()
    {
        return sample_limits<sample_type>::max;
    }
};

using ConvertFloat32TransparentTypes = ::testing::Types<int16_t, int24_t>;

TYPED_TEST_SUITE(ConvertFloat32Transparent, ConvertFloat32TransparentTypes, );

TYPED_TEST(ConvertFloat32Transparent, Typical)
{
    using input_sample = sample<typename TestFixture::sample_type>;

    for (long long i = this->min(); i <= this->max(); ++i)
    {
        input_sample input(i);
        auto temp_float = reference_convert<sample<float32_t>>(input);
        auto output = reference_convert<input_sample>(temp_float);
        EXPECT_EQ(output, input);
    }
}

// ConvertFloatIntNoWrapping

template<typename SampleTypeArg>
class ConvertFloatIntNoWrapping : public ::testing::Test
{
protected:
    using sample_type = SampleTypeArg;
};

TYPED_TEST_SUITE(ConvertFloatIntNoWrapping, PossibleIntSampleTypes, );

TYPED_TEST(ConvertFloatIntNoWrapping, ConvertFloatIntNoWrapping)
{
    using output_sample = sample<typename TestFixture::sample_type>;

    for (float32_t f = 1.1; f > 0.9; f = std::nextafter(f, 0.f))
    {
        EXPECT_GT(reference_convert<output_sample>(sample<float32_t>(f)), output_sample(0));
    }
    for (float32_t f = -1.1; f < -0.9; f = std::nextafter(f, 0.f))
    {
        EXPECT_LT(reference_convert<output_sample>(sample<float32_t>(f)), output_sample(0));
    }
}

// ConvertIntInputNegativeTransparent

template<typename SampleTypeCombinationArg>
class ConvertIntInputNegativeTransparent : public ::testing::Test
{
protected:
    using input_sample_type = typename SampleTypeCombinationArg::input_sample_type;
    using output_sample_type = typename SampleTypeCombinationArg::output_sample_type;
};

TYPED_TEST_SUITE(ConvertIntInputNegativeTransparent, PossibleIntInputSampleTypeCombinations, );

TYPED_TEST(ConvertIntInputNegativeTransparent, IntInputConvert)
{
    using input_sample = sample<typename TestFixture::input_sample_type>;
    using input_sample_limits = sample_limits<typename TestFixture::input_sample_type>;
    using output_sample = sample<typename TestFixture::output_sample_type>;
    using output_sample_limits = sample_limits<typename TestFixture::output_sample_type>;
    for (std::int64_t i = 0; i < input_sample_limits::max; ++i)
    {
        auto sample_value = static_cast<typename TestFixture::input_sample_type>(i);
        auto positive_input = input_sample(sample_value);
        auto positive_output = reference_convert<output_sample>(positive_input);
        auto negative_input = input_sample(-sample_value);
        auto negative_output = reference_convert<output_sample>(negative_input);
        if (negative_output.get() != output_sample_limits::min)
        {
            EXPECT_EQ(positive_output.get(), -negative_output.get());
        }
    }
}

// ConvertFloatInputNegativeTransparent

template<typename OutputSampleType>
class ConvertFloatInputNegativeTransparent : public ::testing::Test
{
protected:
    using output_sample_type = OutputSampleType;
};

TYPED_TEST_SUITE(ConvertFloatInputNegativeTransparent, PossibleSampleTypes, );

TYPED_TEST(ConvertFloatInputNegativeTransparent, FloatInputConvert)
{
    using input_sample = sample<float32_t>;
    using output_sample = sample<typename TestFixture::output_sample_type>;
    using Int32SampleTypeLimits = sample_limits<int32_t>;
    using output_sample_limits = sample_limits<typename TestFixture::output_sample_type>;
    for (std::int64_t i = 0; i < Int32SampleTypeLimits::max; ++i)
    {
        auto sample_value = reference_convert<input_sample>(sample<int32_t>(static_cast<int32_t>(i))).get();
        auto positive_input = input_sample(sample_value);
        auto positive_output = reference_convert<output_sample>(positive_input);
        auto negative_input = input_sample(-sample_value);
        auto negative_output = reference_convert<output_sample>(negative_input);
        if (negative_output.get() != output_sample_limits::min)
        {
            EXPECT_EQ(positive_output.get(), -negative_output.get());
        }
    }
}

} // namespace test
} // namespace ratl
