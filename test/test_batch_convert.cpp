/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

// ratl includes
#include <ratl/detail/batch_reference_sample_converter_impl.hpp>

// ratl test includes
#include "test_utils.hpp"

#if defined(RATL_HAS_XSIMD)

namespace ratl
{
namespace test
{
template<typename SampleValueTypeCombination>
class IntInputBatchConvertSameAsConvert : public ::testing::Test
{
protected:
    using input_sample_value_type = typename SampleValueTypeCombination::input_sample_value_type;
    using output_sample_value_type = typename SampleValueTypeCombination::output_sample_value_type;
};

TYPED_TEST_SUITE(IntInputBatchConvertSameAsConvert, PossibleIntInputSampleValueTypeCombinations, );

TYPED_TEST(IntInputBatchConvertSameAsConvert, IntInputBatchConvert)
{
    using input_sample_value_type = typename TestFixture::input_sample_value_type;
    using input_sample_type = sample<input_sample_value_type>;
    using input_batch_sample_type = detail::batch_sample_value_type_t<input_sample_value_type>;
    using input_sample_limits = sample_limits<input_sample_value_type>;
    using output_sample_value_type = typename TestFixture::output_sample_value_type;
    using output_sample_type = sample<output_sample_value_type>;
    using output_batch_sample_type = detail::batch_sample_value_type_t<output_sample_value_type>;
    using test_dither_generator = detail::batch_null_dither_generator;
    for (std::int64_t i = input_sample_limits::min; i <= input_sample_limits::max; i += detail::batch_size)
    {
        input_batch_sample_type batch_input{};
        output_batch_sample_type batch_refrence_output{};
        for (std::int64_t j = i; j < std::min(
                                         static_cast<std::int64_t>(i + detail::batch_size),
                                         static_cast<std::int64_t>(input_sample_limits::max) + 1);
             ++j)
        {
            auto sample_value = static_cast<input_sample_value_type>(j);
            batch_input[j - i] = sample_value;
            batch_refrence_output[j - i] = reference_convert<output_sample_type>(input_sample_type(sample_value)).get();
        }
        test_dither_generator dither_gen;
        auto batch_convert_output = convert(
            batch_input,
            detail::reference_sample_converter<input_sample_type, output_sample_type, test_dither_generator>(dither_gen));
        for (std::size_t j = 0; j < detail::batch_size; ++j)
        {
            auto refrence_output = static_cast<output_sample_value_type>(batch_refrence_output[j]);
            auto convert_output = static_cast<output_sample_value_type>(batch_convert_output[j]);
            EXPECT_EQ(convert_output, refrence_output);
        }
    }
}

TYPED_TEST(IntInputBatchConvertSameAsConvert, IntInputNetworkOutputBatchConvert)
{
    using input_sample_value_type = typename TestFixture::input_sample_value_type;
    using input_sample_type = sample<input_sample_value_type>;
    using input_batch_sample = detail::batch_sample_value_type_t<input_sample_value_type>;
    using input_sample_limits = sample_limits<input_sample_value_type>;
    using output_sample_value_type = typename TestFixture::output_sample_value_type;
    using output_sample_type = network_sample<output_sample_value_type>;
    using output_batch_sample = detail::batch_network_sample_value_type_t<output_sample_value_type>;
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
            auto sample_value = static_cast<input_sample_value_type>(j);
            batch_input[j - i] = sample_value;
            batch_refrence_output[j - i] =
                detail::network_to_network_underlying_cast<output_sample_value_type>(
                    reference_convert<output_sample_type>(input_sample_type(sample_value)).get());
        }
        test_dither_generator dither_gen;
        auto batch_convert_output = convert(
            batch_input,
            detail::reference_sample_converter<input_sample_type, output_sample_type, test_dither_generator>(dither_gen));
        for (std::size_t j = 0; j < detail::batch_size; ++j)
        {
            auto refrence_output =
                static_cast<detail::network_sample_value_underlying_type_t<output_sample_value_type>>(
                    batch_refrence_output[j]);
            auto convert_output =
                static_cast<detail::network_sample_value_underlying_type_t<output_sample_value_type>>(
                    batch_convert_output[j]);
            EXPECT_EQ(convert_output, refrence_output);
        }
    }
}

TYPED_TEST(IntInputBatchConvertSameAsConvert, IntInputNetworkInputBatchConvert)
{
    using input_sample_value_type = typename TestFixture::input_sample_value_type;
    using input_sample_type = network_sample<input_sample_value_type>;
    using input_batch_sample = detail::batch_network_sample_value_type_t<input_sample_value_type>;
    using input_sample_limits = sample_limits<input_sample_value_type>;
    using output_sample_value_type = typename TestFixture::output_sample_value_type;
    using output_sample_type = sample<output_sample_value_type>;
    using output_batch_sample = detail::batch_sample_value_type_t<output_sample_value_type>;
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
                detail::sample_to_network_sample(static_cast<input_sample_value_type>(j));
            batch_input[j - i] =
                detail::network_to_network_underlying_cast<input_sample_value_type>(sample_value);
            batch_refrence_output[j - i] = reference_convert<output_sample_type>(input_sample_type(sample_value)).get();
        }
        test_dither_generator dither_gen;
        auto batch_convert_output = convert(
            batch_input,
            detail::reference_sample_converter<input_sample_type, output_sample_type, test_dither_generator>(dither_gen));
        for (std::size_t j = 0; j < detail::batch_size; ++j)
        {
            auto refrence_output = static_cast<output_sample_value_type>(batch_refrence_output[j]);
            auto convert_output = static_cast<output_sample_value_type>(batch_convert_output[j]);
            EXPECT_EQ(convert_output, refrence_output);
        }
    }
}

template<typename OutputSampleValueType>
class FloatInputBatchConvertSameAsConvert : public ::testing::Test
{
protected:
    using output_sample_value_type = OutputSampleValueType;
};

TYPED_TEST_SUITE(FloatInputBatchConvertSameAsConvert, PossibleSampleValueTypes, );

TYPED_TEST(FloatInputBatchConvertSameAsConvert, FloatInputConvert)
{
    using input_sample_type = sample<float32_t>;
    using input_batch_sample = detail::batch_sample_value_type_t<float32_t>;
    using output_sample_value_type = typename TestFixture::output_sample_value_type;
    using output_sample_type = sample<output_sample_value_type>;
    using output_batch_sample = detail::batch_sample_value_type_t<output_sample_value_type>;
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
            auto sample_value = reference_convert<input_sample_type>(sample<int32_t>(static_cast<int32_t>(j))).get();
            batch_input[j - i] = sample_value;
            batch_refrence_output[j - i] = reference_convert<output_sample_type>(input_sample_type(sample_value)).get();
        }
        test_dither_generator dither_gen;
        auto batch_convert_output = convert(
            batch_input,
            detail::reference_sample_converter<input_sample_type, output_sample_type, test_dither_generator>(dither_gen));
        for (std::size_t j = 0; j < detail::batch_size; ++j)
        {
            auto refrence_output = static_cast<output_sample_value_type>(batch_refrence_output[j]);
            auto convert_output = static_cast<output_sample_value_type>(batch_convert_output[j]);
            EXPECT_EQ(convert_output, refrence_output);
        }
    }
}

TYPED_TEST(FloatInputBatchConvertSameAsConvert, FloatInputNetworkOutputConvert)
{
    using input_sample_type = sample<float32_t>;
    using input_batch_sample = detail::batch_sample_value_type_t<float32_t>;
    using output_sample_value_type = typename TestFixture::output_sample_value_type;
    using output_sample_type = network_sample<output_sample_value_type>;
    using output_batch_sample = detail::batch_network_sample_value_type_t<output_sample_value_type>;
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
            auto sample_value = reference_convert<input_sample_type>(sample<int32_t>(static_cast<int32_t>(j))).get();
            batch_input[j - i] = sample_value;
            batch_refrence_output[j - i] =
                detail::network_to_network_underlying_cast<output_sample_value_type>(
                    reference_convert<output_sample_type>(input_sample_type(sample_value)).get());
        }
        test_dither_generator dither_gen;
        auto batch_convert_output = convert(
            batch_input,
            detail::reference_sample_converter<input_sample_type, output_sample_type, test_dither_generator>(dither_gen));
        for (std::size_t j = 0; j < detail::batch_size; ++j)
        {
            auto refrence_output =
                static_cast<detail::network_sample_value_underlying_type_t<output_sample_value_type>>(
                    batch_refrence_output[j]);
            auto convert_output =
                static_cast<detail::network_sample_value_underlying_type_t<output_sample_value_type>>(
                    batch_convert_output[j]);
            EXPECT_EQ(convert_output, refrence_output);
        }
    }
}

TYPED_TEST(FloatInputBatchConvertSameAsConvert, FloatInputNetworkInputConvert)
{
    using input_sample_type = network_sample<float32_t>;
    using input_batch_sample = detail::batch_network_sample_value_type_t<float32_t>;
    using output_sample_value_type = typename TestFixture::output_sample_value_type;
    using output_sample_type = sample<output_sample_value_type>;
    using output_batch_sample = detail::batch_sample_value_type_t<output_sample_value_type>;
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
            batch_refrence_output[j - i] = reference_convert<output_sample_type>(input_sample_type(sample_value)).get();
        }
        test_dither_generator dither_gen;
        auto batch_convert_output = convert(
            batch_input,
            detail::reference_sample_converter<input_sample_type, output_sample_type, test_dither_generator>(dither_gen));
        for (std::size_t j = 0; j < detail::batch_size; ++j)
        {
            auto refrence_output = static_cast<output_sample_value_type>(batch_refrence_output[j]);
            auto convert_output = static_cast<output_sample_value_type>(batch_convert_output[j]);
            EXPECT_EQ(convert_output, refrence_output);
        }
    }
}

} // namespace test
} // namespace ratl

#endif
