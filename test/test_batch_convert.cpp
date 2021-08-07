
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
    using InputSampleType = typename SampleTypeCombinationArg::InputSampleType;
    using OutputSampleType = typename SampleTypeCombinationArg::OutputSampleType;
};

TYPED_TEST_SUITE(IntInputBatchConvertSameAsConvert, PossibleIntInputSampleTypeCombinations, );

TYPED_TEST(IntInputBatchConvertSameAsConvert, IntInputBatchConvert)
{
    using InputSample = Sample<typename TestFixture::InputSampleType>;
    using InputBatchSample = detail::BatchSampleValueType_t<typename TestFixture::InputSampleType>;
    using InputSampleTypeLimits = SampleTypeLimits<typename TestFixture::InputSampleType>;
    using OutputSample = Sample<typename TestFixture::OutputSampleType>;
    using OutputBatchSample = detail::BatchSampleValueType_t<typename TestFixture::OutputSampleType>;
    using TestDitherGenerator = detail::BatchNullDitherGenerator;
    for (std::int64_t i = InputSampleTypeLimits::min; i <= InputSampleTypeLimits::max; i += detail::BatchSize)
    {
        InputBatchSample batch_input{};
        OutputBatchSample batch_refrence_output{};
        for (std::int64_t j = i; j < std::min(
                                         static_cast<std::int64_t>(i + detail::BatchSize),
                                         static_cast<std::int64_t>(InputSampleTypeLimits::max) + 1);
             ++j)
        {
            auto sample_value = static_cast<typename TestFixture::InputSampleType>(j);
            batch_input[j - i] = sample_value;
            batch_refrence_output[j - i] = reference_convert<OutputSample>(InputSample(sample_value)).get();
        }
        TestDitherGenerator dither_generator;
        auto batch_convert_output = convert(
            batch_input, ReferenceSampleConverter<InputSample, OutputSample, TestDitherGenerator>{dither_generator});
        for (std::size_t j = 0; j < detail::BatchSize; ++j)
        {
            auto refrence_output = static_cast<typename TestFixture::OutputSampleType>(batch_refrence_output[j]);
            auto convert_output = static_cast<typename TestFixture::OutputSampleType>(batch_convert_output[j]);
            EXPECT_EQ(convert_output, refrence_output);
        }
    }
}

TYPED_TEST(IntInputBatchConvertSameAsConvert, IntInputNetworkOutputBatchConvert)
{
    using InputSample = Sample<typename TestFixture::InputSampleType>;
    using InputBatchSample = detail::BatchSampleValueType_t<typename TestFixture::InputSampleType>;
    using InputSampleTypeLimits = SampleTypeLimits<typename TestFixture::InputSampleType>;
    using OutputSample = NetworkSample<typename TestFixture::OutputSampleType>;
    using OutputBatchSample = detail::BatchNetworkSampleValueType_t<typename TestFixture::OutputSampleType>;
    using TestDitherGenerator = detail::BatchNullDitherGenerator;
    for (std::int64_t i = InputSampleTypeLimits::min; i <= InputSampleTypeLimits::max; i += detail::BatchSize)
    {
        InputBatchSample batch_input{};
        OutputBatchSample batch_refrence_output{};
        for (std::int64_t j = i; j < std::min(
                                         static_cast<std::int64_t>(i + detail::BatchSize),
                                         static_cast<std::int64_t>(InputSampleTypeLimits::max) + 1);
             ++j)
        {
            auto sample_value = static_cast<typename TestFixture::InputSampleType>(j);
            batch_input[j - i] = sample_value;
            batch_refrence_output[j - i] =
                detail::networkToNetworkUnderlyingCast<typename TestFixture::OutputSampleType>(
                    reference_convert<OutputSample>(InputSample(sample_value)).get());
        }
        TestDitherGenerator dither_generator;
        auto batch_convert_output = convert(
            batch_input, ReferenceSampleConverter<InputSample, OutputSample, TestDitherGenerator>{dither_generator});
        for (std::size_t j = 0; j < detail::BatchSize; ++j)
        {
            auto refrence_output =
                static_cast<detail::NetworkSampleValueUnderlyingType_t<typename TestFixture::OutputSampleType>>(
                    batch_refrence_output[j]);
            auto convert_output =
                static_cast<detail::NetworkSampleValueUnderlyingType_t<typename TestFixture::OutputSampleType>>(
                    batch_convert_output[j]);
            EXPECT_EQ(convert_output, refrence_output);
        }
    }
}

TYPED_TEST(IntInputBatchConvertSameAsConvert, IntInputNetworkInputBatchConvert)
{
    using InputSample = NetworkSample<typename TestFixture::InputSampleType>;
    using InputBatchSample = detail::BatchNetworkSampleValueType_t<typename TestFixture::InputSampleType>;
    using InputSampleTypeLimits = SampleTypeLimits<typename TestFixture::InputSampleType>;
    using OutputSample = Sample<typename TestFixture::OutputSampleType>;
    using OutputBatchSample = detail::BatchSampleValueType_t<typename TestFixture::OutputSampleType>;
    using TestDitherGenerator = detail::BatchNullDitherGenerator;
    for (std::int64_t i = InputSampleTypeLimits::min; i <= InputSampleTypeLimits::max; i += detail::BatchSize)
    {
        InputBatchSample batch_input{};
        OutputBatchSample batch_refrence_output{};
        for (std::int64_t j = i; j < std::min(
                                         static_cast<std::int64_t>(i + detail::BatchSize),
                                         static_cast<std::int64_t>(InputSampleTypeLimits::max) + 1);
             ++j)
        {
            auto sample_value = detail::sampleToNetworkSample(static_cast<typename TestFixture::InputSampleType>(j));
            batch_input[j - i] =
                detail::networkToNetworkUnderlyingCast<typename TestFixture::InputSampleType>(sample_value);
            batch_refrence_output[j - i] = reference_convert<OutputSample>(InputSample(sample_value)).get();
        }
        TestDitherGenerator dither_generator;
        auto batch_convert_output = convert(
            batch_input, ReferenceSampleConverter<InputSample, OutputSample, TestDitherGenerator>{dither_generator});
        for (std::size_t j = 0; j < detail::BatchSize; ++j)
        {
            auto refrence_output = static_cast<typename TestFixture::OutputSampleType>(batch_refrence_output[j]);
            auto convert_output = static_cast<typename TestFixture::OutputSampleType>(batch_convert_output[j]);
            EXPECT_EQ(convert_output, refrence_output);
        }
    }
}

template<typename OutputSampleTypeArg>
class FloatInputBatchConvertSameAsConvert : public ::testing::Test
{
protected:
    using OutputSampleType = OutputSampleTypeArg;
};

TYPED_TEST_SUITE(FloatInputBatchConvertSameAsConvert, PossibleSampleTypes, );

TYPED_TEST(FloatInputBatchConvertSameAsConvert, FloatInputConvert)
{
    using InputSample = Sample<float32_t>;
    using InputBatchSample = detail::BatchSampleValueType_t<float32_t>;
    using OutputSample = Sample<typename TestFixture::OutputSampleType>;
    using OutputBatchSample = detail::BatchSampleValueType_t<typename TestFixture::OutputSampleType>;
    using Int32SampleTypeLimits = SampleTypeLimits<int32_t>;
    using TestDitherGenerator = detail::BatchNullDitherGenerator;
    for (std::int64_t i = Int32SampleTypeLimits::min; i <= Int32SampleTypeLimits::max; i += detail::BatchSize)
    {
        InputBatchSample batch_input{};
        OutputBatchSample batch_refrence_output{};
        for (std::int64_t j = i; j < std::min(
                                         static_cast<std::int64_t>(i + detail::BatchSize),
                                         static_cast<std::int64_t>(Int32SampleTypeLimits::max) + 1);
             ++j)
        {
            auto sample_value = reference_convert<InputSample>(Sample<int32_t>(static_cast<int32_t>(j))).get();
            batch_input[j - i] = sample_value;
            batch_refrence_output[j - i] = reference_convert<OutputSample>(InputSample(sample_value)).get();
        }
        TestDitherGenerator dither_generator;
        auto batch_convert_output = convert(
            batch_input, ReferenceSampleConverter<InputSample, OutputSample, TestDitherGenerator>{dither_generator});
        for (std::size_t j = 0; j < detail::BatchSize; ++j)
        {
            auto refrence_output = static_cast<typename TestFixture::OutputSampleType>(batch_refrence_output[j]);
            auto convert_output = static_cast<typename TestFixture::OutputSampleType>(batch_convert_output[j]);
            EXPECT_EQ(convert_output, refrence_output);
        }
    }
}

TYPED_TEST(FloatInputBatchConvertSameAsConvert, FloatInputNetworkOutputConvert)
{
    using InputSample = Sample<float32_t>;
    using InputBatchSample = detail::BatchSampleValueType_t<float32_t>;
    using OutputSample = NetworkSample<typename TestFixture::OutputSampleType>;
    using OutputBatchSample = detail::BatchNetworkSampleValueType_t<typename TestFixture::OutputSampleType>;
    using Int32SampleTypeLimits = SampleTypeLimits<int32_t>;
    using TestDitherGenerator = detail::BatchNullDitherGenerator;
    for (std::int64_t i = Int32SampleTypeLimits::min; i <= Int32SampleTypeLimits::max; i += detail::BatchSize)
    {
        InputBatchSample batch_input{};
        OutputBatchSample batch_refrence_output{};
        for (std::int64_t j = i; j < std::min(
                                         static_cast<std::int64_t>(i + detail::BatchSize),
                                         static_cast<std::int64_t>(Int32SampleTypeLimits::max) + 1);
             ++j)
        {
            auto sample_value = reference_convert<InputSample>(Sample<int32_t>(static_cast<int32_t>(j))).get();
            batch_input[j - i] = sample_value;
            batch_refrence_output[j - i] =
                detail::networkToNetworkUnderlyingCast<typename TestFixture::OutputSampleType>(
                    reference_convert<OutputSample>(InputSample(sample_value)).get());
        }
        TestDitherGenerator dither_generator;
        auto batch_convert_output = convert(
            batch_input, ReferenceSampleConverter<InputSample, OutputSample, TestDitherGenerator>{dither_generator});
        for (std::size_t j = 0; j < detail::BatchSize; ++j)
        {
            auto refrence_output =
                static_cast<detail::NetworkSampleValueUnderlyingType_t<typename TestFixture::OutputSampleType>>(
                    batch_refrence_output[j]);
            auto convert_output =
                static_cast<detail::NetworkSampleValueUnderlyingType_t<typename TestFixture::OutputSampleType>>(
                    batch_convert_output[j]);
            EXPECT_EQ(convert_output, refrence_output);
        }
    }
}

TYPED_TEST(FloatInputBatchConvertSameAsConvert, FloatInputNetworkInputConvert)
{
    using InputSample = NetworkSample<float32_t>;
    using InputBatchSample = detail::BatchNetworkSampleValueType_t<float32_t>;
    using OutputSample = Sample<typename TestFixture::OutputSampleType>;
    using OutputBatchSample = detail::BatchSampleValueType_t<typename TestFixture::OutputSampleType>;
    using Int32SampleTypeLimits = SampleTypeLimits<int32_t>;
    using TestDitherGenerator = detail::BatchNullDitherGenerator;
    for (std::int64_t i = Int32SampleTypeLimits::min; i <= Int32SampleTypeLimits::max; i += detail::BatchSize)
    {
        InputBatchSample batch_input{};
        OutputBatchSample batch_refrence_output{};
        for (std::int64_t j = i; j < std::min(
                                         static_cast<std::int64_t>(i + detail::BatchSize),
                                         static_cast<std::int64_t>(Int32SampleTypeLimits::max) + 1);
             ++j)
        {
            auto sample_value = detail::sampleToNetworkSample(
                reference_convert<Sample<float32_t>>(Sample<int32_t>(static_cast<int32_t>(j))).get());
            batch_input[j - i] = detail::networkToNetworkUnderlyingCast<float32_t>(sample_value);
            batch_refrence_output[j - i] = reference_convert<OutputSample>(InputSample(sample_value)).get();
        }
        TestDitherGenerator dither_generator;
        auto batch_convert_output = convert(
            batch_input, ReferenceSampleConverter<InputSample, OutputSample, TestDitherGenerator>{dither_generator});
        for (std::size_t j = 0; j < detail::BatchSize; ++j)
        {
            auto refrence_output = static_cast<typename TestFixture::OutputSampleType>(batch_refrence_output[j]);
            auto convert_output = static_cast<typename TestFixture::OutputSampleType>(batch_convert_output[j]);
            EXPECT_EQ(convert_output, refrence_output);
        }
    }
}

} // namespace test
} // namespace ratl

#endif
