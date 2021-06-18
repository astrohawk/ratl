
#include "test_utils.hpp"

#include <ratl/detail/batch_sample_converter.hpp>

#if defined(RATL_HAS_XSIMD)

namespace ratl
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
    using TestDitherGenerator = ratl::detail::BatchNullDitherGenerator;
    for (std::int64_t i = InputSampleTypeLimits::min; i < InputSampleTypeLimits::max; ++i)
    {
        auto sample_value = static_cast<typename TestFixture::InputSampleType>(i);
        auto input = InputSample(sample_value);
        auto output = ratl::convert<OutputSample>(input);
        auto batch_input = InputBatchSample(sample_value);
        TestDitherGenerator dither_generator;
        auto batch_output = ratl::detail::BatchSampleConverter<
            typename TestFixture::InputSampleType,
            typename TestFixture::OutputSampleType,
            TestDitherGenerator>::convert(batch_input, dither_generator);
        EXPECT_EQ(output.get(), batch_output[0]);
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
    using Int32SampleTypeLimits = ratl::SampleTypeLimits<ratl::int32_t>;
    using TestDitherGenerator = ratl::detail::BatchNullDitherGenerator;
    for (std::int64_t i = Int32SampleTypeLimits::min; i < Int32SampleTypeLimits::max; ++i)
    {
        auto sample_value =
            ratl::convert<InputSample>(ratl::Sample<ratl::int32_t>(static_cast<ratl::int32_t>(i))).get();
        auto input = InputSample(sample_value);
        auto output = ratl::convert<OutputSample>(input);
        auto batch_input = InputBatchSample(sample_value);
        TestDitherGenerator dither_generator;
        auto batch_output = ratl::detail::
            BatchSampleConverter<ratl::float32_t, typename TestFixture::OutputSampleType, TestDitherGenerator>::convert(
                batch_input, dither_generator);
        EXPECT_EQ(output.get(), batch_output[0]);
        if (output.get() != batch_output[0])
        {
            EXPECT_EQ(output.get(), batch_output[0]);
        }
    }
}

} // namespace ratl

#endif
