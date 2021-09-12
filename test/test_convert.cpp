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
public:
    sample<int16_t> sample_{static_cast<int16_t>(0x1234)};
};

TEST_F(ConvertInt16, Int16)
{
    EXPECT_EQ(reference_convert<sample<int16_t>>(sample_), sample<int16_t>(static_cast<int16_t>(0x1234)));
}

TEST_F(ConvertInt16, Int24)
{
    EXPECT_EQ(reference_convert<sample<int24_t>>(sample_), sample<int24_t>(static_cast<int24_t>(0x123400)));
}

TEST_F(ConvertInt16, Int32)
{
    EXPECT_EQ(reference_convert<sample<int32_t>>(sample_), sample<int32_t>(static_cast<int32_t>(0x12340000)));
}

TEST_F(ConvertInt16, Float32)
{
    EXPECT_EQ(
        reference_convert<sample<float32_t>>(sample_),
        sample<float32_t>(detail::bit_cast<float32_t>(static_cast<uint32_t>(0x3e11a000))));
}

TEST_F(ConvertInt16, NetworkInt16)
{
    EXPECT_EQ(
        reference_convert<network_sample<int16_t>>(sample_),
        network_sample<int16_t>(detail::network_underlying_to_network_cast<int16_t>(static_cast<uint16_t>(0x3412))));
}

TEST_F(ConvertInt16, NetworkInt24)
{
    EXPECT_EQ(
        reference_convert<network_sample<int24_t>>(sample_),
        network_sample<int24_t>(detail::network_underlying_to_network_cast<int24_t>(static_cast<uint24_t>(0x003412))));
}

TEST_F(ConvertInt16, NetworkInt32)
{
    EXPECT_EQ(
        reference_convert<network_sample<int32_t>>(sample_),
        network_sample<int32_t>(
            detail::network_underlying_to_network_cast<int32_t>(static_cast<uint32_t>(0x00003412))));
}

TEST_F(ConvertInt16, NetworkFloat32)
{
    EXPECT_EQ(
        reference_convert<network_sample<float32_t>>(sample_),
        network_sample<float32_t>(
            detail::network_underlying_to_network_cast<float32_t>(static_cast<uint32_t>(0x00a0113e))));
}

// int24

class ConvertInt24 : public ::testing::Test
{
public:
    sample<int24_t> sample_{static_cast<int24_t>(0x123456)};
};

TEST_F(ConvertInt24, Int16)
{
    EXPECT_EQ(reference_convert<sample<int16_t>>(sample_), sample<int16_t>(static_cast<int16_t>(0x1234)));
}

TEST_F(ConvertInt24, Int24)
{
    EXPECT_EQ(reference_convert<sample<int24_t>>(sample_), sample<int24_t>(static_cast<int24_t>(0x123456)));
}

TEST_F(ConvertInt24, Int32)
{
    EXPECT_EQ(reference_convert<sample<int32_t>>(sample_), sample<int32_t>(static_cast<int32_t>(0x12345600)));
}

TEST_F(ConvertInt24, Float32)
{
    EXPECT_EQ(
        reference_convert<sample<float32_t>>(sample_),
        sample<float32_t>(detail::bit_cast<float32_t>(static_cast<uint32_t>(0x3e11a2b0))));
}

TEST_F(ConvertInt24, NetworkInt16)
{
    EXPECT_EQ(
        reference_convert<network_sample<int16_t>>(sample_),
        network_sample<int16_t>(detail::network_underlying_to_network_cast<int16_t>(static_cast<uint16_t>(0x3412))));
}

TEST_F(ConvertInt24, NetworkInt24)
{
    EXPECT_EQ(
        reference_convert<network_sample<int24_t>>(sample_),
        network_sample<int24_t>(detail::network_underlying_to_network_cast<int24_t>(static_cast<uint24_t>(0x563412))));
}

TEST_F(ConvertInt24, NetworkInt32)
{
    EXPECT_EQ(
        reference_convert<network_sample<int32_t>>(sample_),
        network_sample<int32_t>(
            detail::network_underlying_to_network_cast<int32_t>(static_cast<uint32_t>(0x00563412))));
}

TEST_F(ConvertInt24, NetworkFloat32)
{
    EXPECT_EQ(
        reference_convert<network_sample<float32_t>>(sample_),
        network_sample<float32_t>(
            detail::network_underlying_to_network_cast<float32_t>(static_cast<uint32_t>(0xb0a2113e))));
}

// int32

class ConvertInt32 : public ::testing::Test
{
public:
    sample<int32_t> sample_{0x12345678};
};

TEST_F(ConvertInt32, Int16)
{
    EXPECT_EQ(reference_convert<sample<int16_t>>(sample_), sample<int16_t>(static_cast<int16_t>(0x1234)));
}

TEST_F(ConvertInt32, Int24)
{
    EXPECT_EQ(reference_convert<sample<int24_t>>(sample_), sample<int24_t>(static_cast<int24_t>(0x123456)));
}

TEST_F(ConvertInt32, Int32)
{
    EXPECT_EQ(reference_convert<sample<int32_t>>(sample_), sample<int32_t>(static_cast<int32_t>(0x12345678)));
}

TEST_F(ConvertInt32, Float32)
{
    EXPECT_EQ(
        reference_convert<sample<float32_t>>(sample_),
        sample<float32_t>(detail::bit_cast<float32_t>(static_cast<uint32_t>(0x3e11a2b4))));
}

TEST_F(ConvertInt32, NetworkInt16)
{
    EXPECT_EQ(
        reference_convert<network_sample<int16_t>>(sample_),
        network_sample<int16_t>(detail::network_underlying_to_network_cast<int16_t>(static_cast<uint16_t>(0x3412))));
}

TEST_F(ConvertInt32, NetworkInt24)
{
    EXPECT_EQ(
        reference_convert<network_sample<int24_t>>(sample_),
        network_sample<int24_t>(detail::network_underlying_to_network_cast<int24_t>(static_cast<uint24_t>(0x563412))));
}

TEST_F(ConvertInt32, NetworkInt32)
{
    EXPECT_EQ(
        reference_convert<network_sample<int32_t>>(sample_),
        network_sample<int32_t>(
            detail::network_underlying_to_network_cast<int32_t>(static_cast<uint32_t>(0x78563412))));
}

TEST_F(ConvertInt32, NetworkFloat32)
{
    EXPECT_EQ(
        reference_convert<network_sample<float32_t>>(sample_),
        network_sample<float32_t>(
            detail::network_underlying_to_network_cast<float32_t>(static_cast<uint32_t>(0xb4a2113e))));
}

// float32

class ConvertFloat32 : public ::testing::Test
{
public:
    sample<float32_t> sample_{static_cast<float32_t>(0.1234f)};
};

TEST_F(ConvertFloat32, NetworkInt16)
{
    EXPECT_EQ(
        reference_convert<network_sample<int16_t>>(sample_),
        network_sample<int16_t>(detail::network_underlying_to_network_cast<int16_t>(static_cast<uint16_t>(0xcc0f))));
}

TEST_F(ConvertFloat32, NetworkInt24)
{
    EXPECT_EQ(
        reference_convert<network_sample<int24_t>>(sample_),
        network_sample<int24_t>(detail::network_underlying_to_network_cast<int24_t>(static_cast<uint24_t>(0x92cb0f))));
}

TEST_F(ConvertFloat32, NetworkInt32)
{
    EXPECT_EQ(
        reference_convert<network_sample<int32_t>>(sample_),
        network_sample<int32_t>(
            detail::network_underlying_to_network_cast<int32_t>(static_cast<uint32_t>(0x4092cb0f))));
}

TEST_F(ConvertFloat32, NetworkFloat32)
{
    EXPECT_EQ(
        reference_convert<network_sample<float32_t>>(sample_),
        network_sample<float32_t>(
            detail::network_underlying_to_network_cast<float32_t>(static_cast<uint32_t>(0x24b9fc3d))));
}

// network int16

class ConvertNetworkInt16 : public ::testing::Test
{
public:
    network_sample<int16_t> network_{
        detail::network_underlying_to_network_cast<int16_t>(static_cast<uint16_t>(0x3412))};
};

TEST_F(ConvertNetworkInt16, Int16)
{
    EXPECT_EQ(reference_convert<sample<int16_t>>(network_), sample<int16_t>(static_cast<int16_t>(0x1234)));
}

TEST_F(ConvertNetworkInt16, Int24)
{
    EXPECT_EQ(reference_convert<sample<int24_t>>(network_), sample<int24_t>(static_cast<int24_t>(0x123400)));
}

TEST_F(ConvertNetworkInt16, Int32)
{
    EXPECT_EQ(reference_convert<sample<int32_t>>(network_), sample<int32_t>(static_cast<int32_t>(0x12340000)));
}

TEST_F(ConvertNetworkInt16, Float32)
{
    EXPECT_EQ(
        reference_convert<sample<float32_t>>(network_),
        sample<float32_t>(detail::bit_cast<float32_t>(static_cast<uint32_t>(0x3e11a000))));
}

// network int24

class ConvertNetworkInt24 : public ::testing::Test
{
public:
    network_sample<int24_t> network_{
        detail::network_underlying_to_network_cast<int24_t>(static_cast<uint24_t>(0x563412))};
};

TEST_F(ConvertNetworkInt24, Int16)
{
    EXPECT_EQ(reference_convert<sample<int16_t>>(network_), sample<int16_t>(static_cast<int16_t>(0x1234)));
}

TEST_F(ConvertNetworkInt24, Int24)
{
    EXPECT_EQ(reference_convert<sample<int24_t>>(network_), sample<int24_t>(static_cast<int24_t>(0x123456)));
}

TEST_F(ConvertNetworkInt24, Int32)
{
    EXPECT_EQ(reference_convert<sample<int32_t>>(network_), sample<int32_t>(static_cast<int32_t>(0x12345600)));
}

TEST_F(ConvertNetworkInt24, Float32)
{
    EXPECT_EQ(
        reference_convert<sample<float32_t>>(network_),
        sample<float32_t>(detail::bit_cast<float32_t>(static_cast<uint32_t>(0x3e11a2b0))));
}

// network int32

class ConvertNetworkInt32 : public ::testing::Test
{
public:
    network_sample<int32_t> network_{
        detail::network_underlying_to_network_cast<int32_t>(static_cast<uint32_t>(0x78563412))};
};

TEST_F(ConvertNetworkInt32, Int16)
{
    EXPECT_EQ(reference_convert<sample<int16_t>>(network_), sample<int16_t>(static_cast<int16_t>(0x1234)));
}

TEST_F(ConvertNetworkInt32, Int24)
{
    EXPECT_EQ(reference_convert<sample<int24_t>>(network_), sample<int24_t>(static_cast<int24_t>(0x123456)));
}

TEST_F(ConvertNetworkInt32, Int32)
{
    EXPECT_EQ(reference_convert<sample<int32_t>>(network_), sample<int32_t>(static_cast<int32_t>(0x12345678)));
}

TEST_F(ConvertNetworkInt32, Float32)
{
    EXPECT_EQ(
        reference_convert<sample<float32_t>>(network_),
        sample<float32_t>(detail::bit_cast<float32_t>(static_cast<uint32_t>(0x3e11a2b4))));
}

// network float32

class ConvertNetworkFloat32 : public ::testing::Test
{
public:
    network_sample<float32_t> network_{
        detail::network_underlying_to_network_cast<float32_t>(static_cast<uint32_t>(0x24b9fc3d))};
};

TEST_F(ConvertNetworkFloat32, Int16)
{
    EXPECT_EQ(reference_convert<sample<int16_t>>(network_), sample<int16_t>(static_cast<int16_t>(0x0fcc)));
}

TEST_F(ConvertNetworkFloat32, Int24)
{
    EXPECT_EQ(reference_convert<sample<int24_t>>(network_), sample<int24_t>(static_cast<int24_t>(0x0fcb92)));
}

TEST_F(ConvertNetworkFloat32, Int32)
{
    EXPECT_EQ(reference_convert<sample<int32_t>>(network_), sample<int32_t>(static_cast<int32_t>(0x0fcb9240)));
}

TEST_F(ConvertNetworkFloat32, Float32)
{
    EXPECT_EQ(reference_convert<sample<float32_t>>(network_), sample<float32_t>(static_cast<float32_t>(0.1234f)));
}

// misc
TEST(Converter, Test)
{
    using test_dither_generator = detail::null_dither_generator;
    test_dither_generator dither_gen;
    auto converter =
        detail::default_sample_converter<sample<int16_t>, sample<float32_t>, test_dither_generator>(dither_gen);
    EXPECT_EQ(
        convert(sample<int16_t>(static_cast<int16_t>(0x1234)), converter),
        sample<float32_t>(detail::bit_cast<float32_t>(static_cast<uint32_t>(0x3e11a000))));
}

TEST(Convert, CustomConverterFunctor)
{
    auto result = convert(
        sample<int16_t>(static_cast<int16_t>(0x1234)),
        [](const sample<int16_t>& input)
        {
            return convert<sample<float32_t>>(input);
        });
    EXPECT_EQ(result, sample<float32_t>(detail::bit_cast<float32_t>(static_cast<uint32_t>(0x3e11a000))));
}

// ConvertFloat32Correct

template<typename OutputSampleValueType>
class ConvertFloat32Correct : public ::testing::Test
{
public:
    using output_sample_value_type = OutputSampleValueType;
    using output_sample_type = sample<output_sample_value_type>;

    struct reference_converter
    {
        const float32_t sample_abs_max_ = static_cast<float32_t>(std::max(
            std::abs(sample_limits<output_sample_value_type>::max()),
            std::abs(sample_limits<output_sample_value_type>::min())));

        output_sample_type operator()(sample<float32_t> input) const noexcept
        {
            float32_t scaled = input.get() * sample_abs_max_;
            if (scaled >= static_cast<float32_t>(sample_limits<output_sample_value_type>::max()))
            {
                return output_sample_type(sample_limits<output_sample_value_type>::max());
            }
            if (scaled < static_cast<float32_t>(sample_limits<output_sample_value_type>::min()))
            {
                return output_sample_type(sample_limits<output_sample_value_type>::min());
            }
            return output_sample_type(static_cast<output_sample_value_type>(std::lrint(scaled)));
        }
    };
};

TYPED_TEST_SUITE(ConvertFloat32Correct, PossibleIntSampleValueTypes, );

TYPED_TEST(ConvertFloat32Correct, Typical)
{
    using output_sample_value_type = typename TestFixture::output_sample_value_type;
    using output_sample_type = typename TestFixture::output_sample_type;
    using reference_converter = typename TestFixture::reference_converter;

    auto min = detail::symmetric_float_convert_traits<output_sample_value_type>::int_to_float_scaler / 10;
    for (float32_t f = 1.1f; f > min; f = std::nextafter(f, 0.f))
    {
        auto input_sample_type = sample<float32_t>(f);
        EXPECT_EQ(
            reference_convert<output_sample_type>(input_sample_type),
            convert(input_sample_type, reference_converter{}));
    }
    for (float32_t f = -1.1f; f < -min; f = std::nextafter(f, 0.f))
    {
        auto input_sample_type = sample<float32_t>(f);
        EXPECT_EQ(
            reference_convert<output_sample_type>(input_sample_type),
            convert(input_sample_type, reference_converter{}));
    }
}

// ConvertFloat32Transparent

template<typename SampleValueType>
class ConvertFloat32Transparent : public ::testing::Test
{
public:
    using sample_value_type = SampleValueType;
};

using ConvertFloat32TransparentTypes = ::testing::Types<int16_t, int24_t>;

TYPED_TEST_SUITE(ConvertFloat32Transparent, ConvertFloat32TransparentTypes, );

TYPED_TEST(ConvertFloat32Transparent, Typical)
{
    using sample_value_type = typename TestFixture::sample_value_type;
    using sample_type = sample<sample_value_type>;
    using sample_limits = sample_limits<sample_value_type>;

    for (std::int64_t i = sample_limits::min(); i <= sample_limits::max(); ++i)
    {
        auto input = sample_type(static_cast<sample_value_type>(i));
        auto temp_float = reference_convert<sample<float32_t>>(input);
        auto output = reference_convert<sample_type>(temp_float);
        EXPECT_EQ(output, input);
    }
}

// ConvertFloatIntNoWrapping

template<typename SampleValueType>
class ConvertFloatIntNoWrapping : public ::testing::Test
{
public:
    using sample_value_type = SampleValueType;
};

TYPED_TEST_SUITE(ConvertFloatIntNoWrapping, PossibleIntSampleValueTypes, );

TYPED_TEST(ConvertFloatIntNoWrapping, ConvertFloatIntNoWrapping)
{
    using sample_value_type = typename TestFixture::sample_value_type;
    using sample_type = sample<sample_value_type>;

    for (float32_t f = 1.1f; f > 0.9f; f = std::nextafter(f, 0.f))
    {
        EXPECT_GT(reference_convert<sample_type>(sample<float32_t>(f)), sample_type(static_cast<sample_value_type>(0)));
    }
    for (float32_t f = -1.1f; f < -0.9f; f = std::nextafter(f, 0.f))
    {
        EXPECT_LT(reference_convert<sample_type>(sample<float32_t>(f)), sample_type(static_cast<sample_value_type>(0)));
    }
}

// ConvertIntInputNegativeTransparent

template<typename SampleValueTypeCombination>
class ConvertIntInputNegativeTransparent : public ::testing::Test
{
public:
    using input_sample_value_type = typename SampleValueTypeCombination::input_sample_value_type;
    using output_sample_value_type = typename SampleValueTypeCombination::output_sample_value_type;
};

TYPED_TEST_SUITE(ConvertIntInputNegativeTransparent, PossibleIntInputSampleValueTypeCombinations, );

TYPED_TEST(ConvertIntInputNegativeTransparent, IntInputConvert)
{
    using input_sample_value_type = typename TestFixture::input_sample_value_type;
    using input_sample_type = sample<input_sample_value_type>;
    using input_sample_limits = sample_limits<input_sample_value_type>;
    using output_sample_value_type = typename TestFixture::output_sample_value_type;
    using output_sample_type = sample<output_sample_value_type>;
    using output_sample_limits = sample_limits<output_sample_value_type>;

    for (std::int64_t i = 0; i < static_cast<std::int64_t>(input_sample_limits::max()); ++i)
    {
        auto positive_input = input_sample_type(static_cast<input_sample_value_type>(i));
        auto positive_output = reference_convert<output_sample_type>(positive_input);
        auto negative_input = input_sample_type(static_cast<input_sample_value_type>(-i));
        auto negative_output = reference_convert<output_sample_type>(negative_input);
        if (negative_output.get() != output_sample_limits::min())
        {
            EXPECT_EQ(positive_output.get(), -negative_output.get());
        }
    }
}

// ConvertFloatInputNegativeTransparent

template<typename OutputSampleValueType>
class ConvertFloatInputNegativeTransparent : public ::testing::Test
{
public:
    using output_sample_value_type = OutputSampleValueType;
};

TYPED_TEST_SUITE(ConvertFloatInputNegativeTransparent, PossibleSampleValueTypes, );

TYPED_TEST(ConvertFloatInputNegativeTransparent, FloatInputConvert)
{
    using input_sample_type = sample<float32_t>;
    using output_sample_value_type = typename TestFixture::output_sample_value_type;
    using output_sample_type = sample<output_sample_value_type>;
    using output_sample_limits = sample_limits<output_sample_value_type>;
    using int32_sample_limits = sample_limits<int32_t>;
    for (std::int64_t i = 0; i < static_cast<std::int64_t>(int32_sample_limits::max()); ++i)
    {
        auto sample_value = reference_convert<input_sample_type>(sample<int32_t>(static_cast<int32_t>(i))).get();
        auto positive_input = input_sample_type(sample_value);
        auto positive_output = reference_convert<output_sample_type>(positive_input);
        auto negative_input = input_sample_type(-sample_value);
        auto negative_output = reference_convert<output_sample_type>(negative_input);
        if (negative_output.get() != output_sample_limits::min())
        {
            EXPECT_EQ(positive_output.get(), -negative_output.get());
        }
    }
}

} // namespace test
} // namespace ratl
