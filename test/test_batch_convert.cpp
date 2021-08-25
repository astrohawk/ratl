
#include "test_utils.hpp"

#include <ratl/detail/batch_reference_sample_converter_impl.hpp>

#if defined(RATL_HAS_XSIMD)

namespace ratl
{
namespace test
{
template<typename SampleTypeCombinationArg>
class IntInputBatchConvertSameAsConvert : public ::testing::Test
{
protected:
    using input_sample_type = typename SampleTypeCombinationArg::input_sample_type;
    using output_sample_type = typename SampleTypeCombinationArg::output_sample_type;
};

TYPED_TEST_SUITE(IntInputBatchConvertSameAsConvert, PossibleIntInputSampleTypeCombinations, );

TYPED_TEST(IntInputBatchConvertSameAsConvert, IntInputBatchConvert)
{
    using input_sample = sample<typename TestFixture::input_sample_type>;
    using input_batch_sample = detail::batch_sample_value_type_t<typename TestFixture::input_sample_type>;
    using input_sample_limits = sample_limits<typename TestFixture::input_sample_type>;
    using output_sample = sample<typename TestFixture::output_sample_type>;
    using output_batch_sample = detail::batch_sample_value_type_t<typename TestFixture::output_sample_type>;
    using test_dither_generator = detail::batch_null_dither_generator;
    for (std::int64_t i = input_sample_limits::min; i <= input_sample_limits::max; i += detail::batch_size)
    {
        input_batch_sample batch_input{};
        output_batch_sample batch_refrence_output{};
        for (std::int64_t j = i; j < std::min(
                                         static_cast<std::int64_t>(i + detail::batch_size),
                                         static_cast<std::int64_t>(input_sample_limits::max) + 1);
             ++j)
        {
            auto sample_value = static_cast<typename TestFixture::input_sample_type>(j);
            batch_input[j - i] = sample_value;
            batch_refrence_output[j - i] = reference_convert<output_sample>(input_sample(sample_value)).get();
        }
        test_dither_generator dither_gen;
        auto batch_convert_output = convert(
            batch_input,
            detail::reference_sample_converter<input_sample, output_sample, test_dither_generator>{dither_gen});
        for (std::size_t j = 0; j < detail::batch_size; ++j)
        {
            auto refrence_output = static_cast<typename TestFixture::output_sample_type>(batch_refrence_output[j]);
            auto convert_output = static_cast<typename TestFixture::output_sample_type>(batch_convert_output[j]);
            EXPECT_EQ(convert_output, refrence_output);
        }
    }
}

TYPED_TEST(IntInputBatchConvertSameAsConvert, IntInputNetworkOutputBatchConvert)
{
    using input_sample = sample<typename TestFixture::input_sample_type>;
    using input_batch_sample = detail::batch_sample_value_type_t<typename TestFixture::input_sample_type>;
    using input_sample_limits = sample_limits<typename TestFixture::input_sample_type>;
    using output_sample = network_sample<typename TestFixture::output_sample_type>;
    using output_batch_sample = detail::batch_network_sample_value_type_t<typename TestFixture::output_sample_type>;
    using test_dither_generator = detail::batch_null_dither_generator;
    for (std::int64_t i = input_sample_limits::min; i <= input_sample_limits::max; i += detail::batch_size)
    {
        input_batch_sample batch_input{};
        output_batch_sample batch_refrence_output{};
        for (std::int64_t j = i; j < std::min(
                                         static_cast<std::int64_t>(i + detail::batch_size),
                                         static_cast<std::int64_t>(input_sample_limits::max) + 1);
             ++j)
        {
            auto sample_value = static_cast<typename TestFixture::input_sample_type>(j);
            batch_input[j - i] = sample_value;
            batch_refrence_output[j - i] =
                detail::network_to_network_underlying_cast<typename TestFixture::output_sample_type>(
                    reference_convert<output_sample>(input_sample(sample_value)).get());
        }
        test_dither_generator dither_gen;
        auto batch_convert_output = convert(
            batch_input,
            detail::reference_sample_converter<input_sample, output_sample, test_dither_generator>{dither_gen});
        for (std::size_t j = 0; j < detail::batch_size; ++j)
        {
            auto refrence_output =
                static_cast<detail::network_sample_value_underlying_type_t<typename TestFixture::output_sample_type>>(
                    batch_refrence_output[j]);
            auto convert_output =
                static_cast<detail::network_sample_value_underlying_type_t<typename TestFixture::output_sample_type>>(
                    batch_convert_output[j]);
            EXPECT_EQ(convert_output, refrence_output);
        }
    }
}

TYPED_TEST(IntInputBatchConvertSameAsConvert, IntInputNetworkInputBatchConvert)
{
    using input_sample = network_sample<typename TestFixture::input_sample_type>;
    using input_batch_sample = detail::batch_network_sample_value_type_t<typename TestFixture::input_sample_type>;
    using input_sample_limits = sample_limits<typename TestFixture::input_sample_type>;
    using output_sample = sample<typename TestFixture::output_sample_type>;
    using output_batch_sample = detail::batch_sample_value_type_t<typename TestFixture::output_sample_type>;
    using test_dither_generator = detail::batch_null_dither_generator;
    for (std::int64_t i = input_sample_limits::min; i <= input_sample_limits::max; i += detail::batch_size)
    {
        input_batch_sample batch_input{};
        output_batch_sample batch_refrence_output{};
        for (std::int64_t j = i; j < std::min(
                                         static_cast<std::int64_t>(i + detail::batch_size),
                                         static_cast<std::int64_t>(input_sample_limits::max) + 1);
             ++j)
        {
            auto sample_value =
                detail::sample_to_network_sample(static_cast<typename TestFixture::input_sample_type>(j));
            batch_input[j - i] =
                detail::network_to_network_underlying_cast<typename TestFixture::input_sample_type>(sample_value);
            batch_refrence_output[j - i] = reference_convert<output_sample>(input_sample(sample_value)).get();
        }
        test_dither_generator dither_gen;
        auto batch_convert_output = convert(
            batch_input,
            detail::reference_sample_converter<input_sample, output_sample, test_dither_generator>{dither_gen});
        for (std::size_t j = 0; j < detail::batch_size; ++j)
        {
            auto refrence_output = static_cast<typename TestFixture::output_sample_type>(batch_refrence_output[j]);
            auto convert_output = static_cast<typename TestFixture::output_sample_type>(batch_convert_output[j]);
            EXPECT_EQ(convert_output, refrence_output);
        }
    }
}

template<typename OutputSampleTypeArg>
class FloatInputBatchConvertSameAsConvert : public ::testing::Test
{
protected:
    using output_sample_type = OutputSampleTypeArg;
};

TYPED_TEST_SUITE(FloatInputBatchConvertSameAsConvert, PossibleSampleTypes, );

TYPED_TEST(FloatInputBatchConvertSameAsConvert, FloatInputConvert)
{
    using input_sample = sample<float32_t>;
    using input_batch_sample = detail::batch_sample_value_type_t<float32_t>;
    using output_sample = sample<typename TestFixture::output_sample_type>;
    using output_batch_sample = detail::batch_sample_value_type_t<typename TestFixture::output_sample_type>;
    using int32_sample_limits = sample_limits<int32_t>;
    using test_dither_generator = detail::batch_null_dither_generator;
    for (std::int64_t i = int32_sample_limits::min; i <= int32_sample_limits::max; i += detail::batch_size)
    {
        input_batch_sample batch_input{};
        output_batch_sample batch_refrence_output{};
        for (std::int64_t j = i; j < std::min(
                                         static_cast<std::int64_t>(i + detail::batch_size),
                                         static_cast<std::int64_t>(int32_sample_limits::max) + 1);
             ++j)
        {
            auto sample_value = reference_convert<input_sample>(sample<int32_t>(static_cast<int32_t>(j))).get();
            batch_input[j - i] = sample_value;
            batch_refrence_output[j - i] = reference_convert<output_sample>(input_sample(sample_value)).get();
        }
        test_dither_generator dither_gen;
        auto batch_convert_output = convert(
            batch_input,
            detail::reference_sample_converter<input_sample, output_sample, test_dither_generator>{dither_gen});
        for (std::size_t j = 0; j < detail::batch_size; ++j)
        {
            auto refrence_output = static_cast<typename TestFixture::output_sample_type>(batch_refrence_output[j]);
            auto convert_output = static_cast<typename TestFixture::output_sample_type>(batch_convert_output[j]);
            EXPECT_EQ(convert_output, refrence_output);
        }
    }
}

TYPED_TEST(FloatInputBatchConvertSameAsConvert, FloatInputNetworkOutputConvert)
{
    using input_sample = sample<float32_t>;
    using input_batch_sample = detail::batch_sample_value_type_t<float32_t>;
    using output_sample = network_sample<typename TestFixture::output_sample_type>;
    using output_batch_sample = detail::batch_network_sample_value_type_t<typename TestFixture::output_sample_type>;
    using int32_sample_limits = sample_limits<int32_t>;
    using test_dither_generator = detail::batch_null_dither_generator;
    for (std::int64_t i = int32_sample_limits::min; i <= int32_sample_limits::max; i += detail::batch_size)
    {
        input_batch_sample batch_input{};
        output_batch_sample batch_refrence_output{};
        for (std::int64_t j = i; j < std::min(
                                         static_cast<std::int64_t>(i + detail::batch_size),
                                         static_cast<std::int64_t>(int32_sample_limits::max) + 1);
             ++j)
        {
            auto sample_value = reference_convert<input_sample>(sample<int32_t>(static_cast<int32_t>(j))).get();
            batch_input[j - i] = sample_value;
            batch_refrence_output[j - i] =
                detail::network_to_network_underlying_cast<typename TestFixture::output_sample_type>(
                    reference_convert<output_sample>(input_sample(sample_value)).get());
        }
        test_dither_generator dither_gen;
        auto batch_convert_output = convert(
            batch_input,
            detail::reference_sample_converter<input_sample, output_sample, test_dither_generator>{dither_gen});
        for (std::size_t j = 0; j < detail::batch_size; ++j)
        {
            auto refrence_output =
                static_cast<detail::network_sample_value_underlying_type_t<typename TestFixture::output_sample_type>>(
                    batch_refrence_output[j]);
            auto convert_output =
                static_cast<detail::network_sample_value_underlying_type_t<typename TestFixture::output_sample_type>>(
                    batch_convert_output[j]);
            EXPECT_EQ(convert_output, refrence_output);
        }
    }
}

TYPED_TEST(FloatInputBatchConvertSameAsConvert, FloatInputNetworkInputConvert)
{
    using input_sample = network_sample<float32_t>;
    using input_batch_sample = detail::batch_network_sample_value_type_t<float32_t>;
    using output_sample = sample<typename TestFixture::output_sample_type>;
    using output_batch_sample = detail::batch_sample_value_type_t<typename TestFixture::output_sample_type>;
    using int32_sample_limits = sample_limits<int32_t>;
    using test_dither_generator = detail::batch_null_dither_generator;
    for (std::int64_t i = int32_sample_limits::min; i <= int32_sample_limits::max; i += detail::batch_size)
    {
        input_batch_sample batch_input{};
        output_batch_sample batch_refrence_output{};
        for (std::int64_t j = i; j < std::min(
                                         static_cast<std::int64_t>(i + detail::batch_size),
                                         static_cast<std::int64_t>(int32_sample_limits::max) + 1);
             ++j)
        {
            auto sample_value = detail::sample_to_network_sample(
                reference_convert<sample<float32_t>>(sample<int32_t>(static_cast<int32_t>(j))).get());
            batch_input[j - i] = detail::network_to_network_underlying_cast<float32_t>(sample_value);
            batch_refrence_output[j - i] = reference_convert<output_sample>(input_sample(sample_value)).get();
        }
        test_dither_generator dither_gen;
        auto batch_convert_output = convert(
            batch_input,
            detail::reference_sample_converter<input_sample, output_sample, test_dither_generator>{dither_gen});
        for (std::size_t j = 0; j < detail::batch_size; ++j)
        {
            auto refrence_output = static_cast<typename TestFixture::output_sample_type>(batch_refrence_output[j]);
            auto convert_output = static_cast<typename TestFixture::output_sample_type>(batch_convert_output[j]);
            EXPECT_EQ(convert_output, refrence_output);
        }
    }
}

} // namespace test
} // namespace ratl

#endif
