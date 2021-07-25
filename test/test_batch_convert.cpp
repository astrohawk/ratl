
#include "test_utils.hpp"

#include <ratl/detail/batch_sample_converter.hpp>

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
    using InputSample = ratl::Sample<typename TestFixture::InputSampleType>;
    using InputBatchSample = ratl::detail::BatchSampleValueType_t<typename TestFixture::InputSampleType>;
    using InputSampleTypeLimits = ratl::SampleTypeLimits<typename TestFixture::InputSampleType>;
    using OutputSample = ratl::Sample<typename TestFixture::OutputSampleType>;
    using OutputBatchSample = ratl::detail::BatchSampleValueType_t<typename TestFixture::OutputSampleType>;
    using TestDitherGenerator = ratl::detail::BatchNullDitherGenerator;
    for (std::int64_t i = InputSampleTypeLimits::min; i <= InputSampleTypeLimits::max; i += ratl::detail::BatchSize)
    {
        InputBatchSample batch_input{};
        OutputBatchSample batch_refrence_output{};
        for (std::int64_t j = i; j < std::min(
                                         static_cast<std::int64_t>(i + ratl::detail::BatchSize),
                                         static_cast<std::int64_t>(InputSampleTypeLimits::max) + 1);
             ++j)
        {
            auto sample_value = static_cast<typename TestFixture::InputSampleType>(j);
            batch_input[j - i] = sample_value;
            batch_refrence_output[j - i] = ratl::convert<OutputSample>(InputSample(sample_value)).get();
        }
        TestDitherGenerator dither_generator;
        auto batch_convert_output = ratl::detail::BatchSampleConverter<
            typename TestFixture::InputSampleType,
            typename TestFixture::OutputSampleType,
            TestDitherGenerator>::convert(batch_input, dither_generator);
        for (std::size_t j = 0; j < ratl::detail::BatchSize; ++j)
        {
            EXPECT_EQ(
                static_cast<typename TestFixture::OutputSampleType>(batch_convert_output[j]),
                static_cast<typename TestFixture::OutputSampleType>(batch_refrence_output[j]));
        }
    }
}

TYPED_TEST(IntInputBatchConvertSameAsConvert, IntInputNetworkOutputBatchConvert)
{
    using InputSample = ratl::Sample<typename TestFixture::InputSampleType>;
    using InputBatchSample = ratl::detail::BatchSampleValueType_t<typename TestFixture::InputSampleType>;
    using InputSampleTypeLimits = ratl::SampleTypeLimits<typename TestFixture::InputSampleType>;
    using OutputSample = ratl::NetworkSample<typename TestFixture::OutputSampleType>;
    using OutputBatchSample = ratl::detail::BatchNetworkSampleValueType_t<typename TestFixture::OutputSampleType>;
    using TestDitherGenerator = ratl::detail::BatchNullDitherGenerator;
    for (std::int64_t i = InputSampleTypeLimits::min; i <= InputSampleTypeLimits::max; i += ratl::detail::BatchSize)
    {
        InputBatchSample batch_input{};
        OutputBatchSample batch_refrence_output{};
        for (std::int64_t j = i; j < std::min(
                                         static_cast<std::int64_t>(i + ratl::detail::BatchSize),
                                         static_cast<std::int64_t>(InputSampleTypeLimits::max) + 1);
             ++j)
        {
            auto sample_value = static_cast<typename TestFixture::InputSampleType>(j);
            batch_input[j - i] = sample_value;
            batch_refrence_output[j - i] =
                ratl::detail::networkToNetworkUnderlyingCast<typename TestFixture::OutputSampleType>(
                    ratl::convert<OutputSample>(InputSample(sample_value)).get());
        }
        TestDitherGenerator dither_generator;
        auto batch_convert_output = ratl::detail::BatchSampleToNetworkConverter<
            typename TestFixture::InputSampleType,
            typename TestFixture::OutputSampleType,
            TestDitherGenerator>::convert(batch_input, dither_generator);
        for (std::size_t j = 0; j < ratl::detail::BatchSize; ++j)
        {
            EXPECT_EQ(
                static_cast<ratl::detail::NetworkSampleValueUnderlyingType_t<typename TestFixture::OutputSampleType>>(
                    batch_convert_output[j]),
                static_cast<ratl::detail::NetworkSampleValueUnderlyingType_t<typename TestFixture::OutputSampleType>>(
                    batch_refrence_output[j]));
        }
    }
}

TYPED_TEST(IntInputBatchConvertSameAsConvert, IntInputNetworkInputBatchConvert)
{
    using InputSample = ratl::NetworkSample<typename TestFixture::InputSampleType>;
    using InputBatchSample = ratl::detail::BatchNetworkSampleValueType_t<typename TestFixture::InputSampleType>;
    using InputSampleTypeLimits = ratl::SampleTypeLimits<typename TestFixture::InputSampleType>;
    using OutputSample = ratl::Sample<typename TestFixture::OutputSampleType>;
    using OutputBatchSample = ratl::detail::BatchSampleValueType_t<typename TestFixture::OutputSampleType>;
    using TestDitherGenerator = ratl::detail::BatchNullDitherGenerator;
    for (std::int64_t i = InputSampleTypeLimits::min; i <= InputSampleTypeLimits::max; i += ratl::detail::BatchSize)
    {
        InputBatchSample batch_input{};
        OutputBatchSample batch_refrence_output{};
        for (std::int64_t j = i; j < std::min(
                                         static_cast<std::int64_t>(i + ratl::detail::BatchSize),
                                         static_cast<std::int64_t>(InputSampleTypeLimits::max) + 1);
             ++j)
        {
            auto sample_value =
                ratl::detail::sampleToNetworkSample(static_cast<typename TestFixture::InputSampleType>(j));
            batch_input[j - i] =
                ratl::detail::networkToNetworkUnderlyingCast<typename TestFixture::InputSampleType>(sample_value);
            batch_refrence_output[j - i] = ratl::convert<OutputSample>(InputSample(sample_value)).get();
        }
        TestDitherGenerator dither_generator;
        auto batch_convert_output = ratl::detail::BatchNetworkToSampleConverter<
            typename TestFixture::InputSampleType,
            typename TestFixture::OutputSampleType,
            TestDitherGenerator>::convert(batch_input, dither_generator);
        for (std::size_t j = 0; j < ratl::detail::BatchSize; ++j)
        {
            EXPECT_EQ(
                static_cast<typename TestFixture::OutputSampleType>(batch_convert_output[j]),
                static_cast<typename TestFixture::OutputSampleType>(batch_refrence_output[j]));
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
    using InputSample = ratl::Sample<ratl::float32_t>;
    using InputBatchSample = ratl::detail::BatchSampleValueType_t<ratl::float32_t>;
    using OutputSample = ratl::Sample<typename TestFixture::OutputSampleType>;
    using OutputBatchSample = ratl::detail::BatchSampleValueType_t<typename TestFixture::OutputSampleType>;
    using Int32SampleTypeLimits = ratl::SampleTypeLimits<ratl::int32_t>;
    using TestDitherGenerator = ratl::detail::BatchNullDitherGenerator;
    for (std::int64_t i = Int32SampleTypeLimits::min; i <= Int32SampleTypeLimits::max; i += ratl::detail::BatchSize)
    {
        InputBatchSample batch_input{};
        OutputBatchSample batch_refrence_output{};
        for (std::int64_t j = i; j < std::min(
                                         static_cast<std::int64_t>(i + ratl::detail::BatchSize),
                                         static_cast<std::int64_t>(Int32SampleTypeLimits::max) + 1);
             ++j)
        {
            auto sample_value =
                ratl::convert<InputSample>(ratl::Sample<ratl::int32_t>(static_cast<ratl::int32_t>(j))).get();
            batch_input[j - i] = sample_value;
            batch_refrence_output[j - i] = ratl::convert<OutputSample>(InputSample(sample_value)).get();
        }
        TestDitherGenerator dither_generator;
        auto batch_convert_output = ratl::detail::
            BatchSampleConverter<ratl::float32_t, typename TestFixture::OutputSampleType, TestDitherGenerator>::convert(
                batch_input, dither_generator);
        for (std::size_t j = 0; j < ratl::detail::BatchSize; ++j)
        {
            EXPECT_EQ(
                static_cast<typename TestFixture::OutputSampleType>(batch_convert_output[j]),
                static_cast<typename TestFixture::OutputSampleType>(batch_refrence_output[j]));
        }
    }
}

TYPED_TEST(FloatInputBatchConvertSameAsConvert, FloatInputNetworkOutputConvert)
{
    using InputSample = ratl::Sample<ratl::float32_t>;
    using InputBatchSample = ratl::detail::BatchSampleValueType_t<ratl::float32_t>;
    using OutputSample = ratl::NetworkSample<typename TestFixture::OutputSampleType>;
    using OutputBatchSample = ratl::detail::BatchNetworkSampleValueType_t<typename TestFixture::OutputSampleType>;
    using Int32SampleTypeLimits = ratl::SampleTypeLimits<ratl::int32_t>;
    using TestDitherGenerator = ratl::detail::BatchNullDitherGenerator;
    for (std::int64_t i = Int32SampleTypeLimits::min; i <= Int32SampleTypeLimits::max; i += ratl::detail::BatchSize)
    {
        InputBatchSample batch_input{};
        OutputBatchSample batch_refrence_output{};
        for (std::int64_t j = i; j < std::min(
                                         static_cast<std::int64_t>(i + ratl::detail::BatchSize),
                                         static_cast<std::int64_t>(Int32SampleTypeLimits::max) + 1);
             ++j)
        {
            auto sample_value =
                ratl::convert<InputSample>(ratl::Sample<ratl::int32_t>(static_cast<ratl::int32_t>(j))).get();
            batch_input[j - i] = sample_value;
            batch_refrence_output[j - i] =
                ratl::detail::networkToNetworkUnderlyingCast<typename TestFixture::OutputSampleType>(
                    ratl::convert<OutputSample>(InputSample(sample_value)).get());
        }
        TestDitherGenerator dither_generator;
        auto batch_convert_output = ratl::detail::BatchSampleToNetworkConverter<
            ratl::float32_t,
            typename TestFixture::OutputSampleType,
            TestDitherGenerator>::convert(batch_input, dither_generator);
        for (std::size_t j = 0; j < ratl::detail::BatchSize; ++j)
        {
            EXPECT_EQ(
                static_cast<ratl::detail::NetworkSampleValueUnderlyingType_t<typename TestFixture::OutputSampleType>>(
                    batch_convert_output[j]),
                static_cast<ratl::detail::NetworkSampleValueUnderlyingType_t<typename TestFixture::OutputSampleType>>(
                    batch_refrence_output[j]));
        }
    }
}

TYPED_TEST(FloatInputBatchConvertSameAsConvert, FloatInputNetworkInputConvert)
{
    using InputSample = ratl::NetworkSample<ratl::float32_t>;
    using InputBatchSample = ratl::detail::BatchNetworkSampleValueType_t<ratl::float32_t>;
    using OutputSample = ratl::Sample<typename TestFixture::OutputSampleType>;
    using OutputBatchSample = ratl::detail::BatchSampleValueType_t<typename TestFixture::OutputSampleType>;
    using Int32SampleTypeLimits = ratl::SampleTypeLimits<ratl::int32_t>;
    using TestDitherGenerator = ratl::detail::BatchNullDitherGenerator;
    for (std::int64_t i = Int32SampleTypeLimits::min; i <= Int32SampleTypeLimits::max; i += ratl::detail::BatchSize)
    {
        InputBatchSample batch_input{};
        OutputBatchSample batch_refrence_output{};
        for (std::int64_t j = i; j < std::min(
                                         static_cast<std::int64_t>(i + ratl::detail::BatchSize),
                                         static_cast<std::int64_t>(Int32SampleTypeLimits::max) + 1);
             ++j)
        {
            auto sample_value = ratl::detail::sampleToNetworkSample(
                ratl::convert<ratl::Sample<ratl::float32_t>>(ratl::Sample<ratl::int32_t>(static_cast<ratl::int32_t>(j)))
                    .get());
            batch_input[j - i] = ratl::detail::networkToNetworkUnderlyingCast<ratl::float32_t>(sample_value);
            batch_refrence_output[j - i] = ratl::convert<OutputSample>(InputSample(sample_value)).get();
        }
        TestDitherGenerator dither_generator;
        auto batch_convert_output = ratl::detail::BatchNetworkToSampleConverter<
            ratl::float32_t,
            typename TestFixture::OutputSampleType,
            TestDitherGenerator>::convert(batch_input, dither_generator);
        for (std::size_t j = 0; j < ratl::detail::BatchSize; ++j)
        {
            EXPECT_EQ(
                static_cast<typename TestFixture::OutputSampleType>(batch_convert_output[j]),
                static_cast<typename TestFixture::OutputSampleType>(batch_refrence_output[j]));
        }
    }
}

} // namespace test
} // namespace ratl

#endif
