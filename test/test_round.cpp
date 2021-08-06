
#include "test_utils.hpp"

namespace ratl
{
namespace test
{
template<typename SampleType, typename Converter>
SampleType convert(ratl::float32_t sample)
{
    if RATL_UNLIKELY (sample >= ratl::SampleTypeLimits<ratl::float32_t>::max)
    {
        return ratl::SampleTypeLimits<SampleType>::max;
    }
    if RATL_UNLIKELY (sample < ratl::SampleTypeLimits<ratl::float32_t>::min)
    {
        return ratl::SampleTypeLimits<SampleType>::min;
    }
    return static_cast<SampleType>(Converter::convert(sample * detail::FloatConvertTraits<SampleType>::Multiplier));
}

struct RoundHalfUpConverter
{
    static int32_t convert(ratl::float32_t sample) noexcept
    {
        return static_cast<int32_t>(std::floor(sample + 0.5));
    }
};

struct MagicConverter
{
    static int32_t convert(ratl::float32_t sample) noexcept
    {
        return detail::roundFloat32ToInt32Magic(sample);
    }
};

template<typename SampleTypeArg>
class TestRound : public testing::Test
{
public:
    using SampleType = SampleTypeArg;
};

TYPED_TEST_SUITE(TestRound, PossibleIntSampleTypes, );

TYPED_TEST(TestRound, magicEqualToLrintForAllInt32Inputs)
{
    using SampleType = typename TestFixture::SampleType;

    for (std::int64_t i = ratl::SampleTypeLimits<SampleType>::min; i <= ratl::SampleTypeLimits<SampleType>::max; ++i)
    {
        auto original = static_cast<SampleType>(i);
        auto input = ratl::convert<ratl::Sample<ratl::float32_t>>(ratl::Sample<SampleType>(original)).get();
        auto reference_output = convert<SampleType, RoundHalfUpConverter>(input);
        auto test_output = convert<SampleType, MagicConverter>(input);
        EXPECT_EQ(test_output, reference_output);
    }
}

TYPED_TEST(TestRound, magicCloseToLrintForAllFloatInputs)
{
    using SampleType = typename TestFixture::SampleType;

    static constexpr float32_t Min = 1e-10;

    for (float32_t input = ratl::SampleTypeLimits<float32_t>::max; input > Min; input = std::nextafter(input, 0.f))
    {
        auto reference_output = static_cast<std::int64_t>(convert<SampleType, RoundHalfUpConverter>(input));
        auto test_output = static_cast<std::int64_t>(convert<SampleType, MagicConverter>(input));
        EXPECT_EQ(test_output, reference_output);
    }
    for (float32_t input = ratl::SampleTypeLimits<float32_t>::min; input < -Min; input = std::nextafter(input, 0.f))
    {
        auto reference_output = static_cast<std::int64_t>(convert<SampleType, RoundHalfUpConverter>(input));
        auto test_output = static_cast<std::int64_t>(convert<SampleType, MagicConverter>(input));
        EXPECT_EQ(test_output, reference_output);
    }
}

} // namespace test
} // namespace ratl
