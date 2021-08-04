
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

struct LrintConverter
{
    static int32_t convert(ratl::float32_t sample) noexcept
    {
        return std::lrint(sample);
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
        auto lrint_output = convert<SampleType, LrintConverter>(input);
        auto magic_output = convert<SampleType, MagicConverter>(input);
        EXPECT_EQ(magic_output, lrint_output);
    }
}

TYPED_TEST(TestRound, magicCloseToLrintForAllFloatInputs)
{
    using SampleType = typename TestFixture::SampleType;

    static constexpr float32_t Min = 1e-10;

    std::uint64_t num_trials = 0;
    std::uint64_t num_errors = 0;
    for (float32_t input = ratl::SampleTypeLimits<float32_t>::max; input > Min; input = std::nextafter(input, 0.f))
    {
        auto lrint_output = static_cast<std::int64_t>(convert<SampleType, LrintConverter>(input));
        auto magic_output = static_cast<std::int64_t>(convert<SampleType, MagicConverter>(input));
        EXPECT_GE(magic_output, lrint_output - 1);
        EXPECT_LE(magic_output, lrint_output + 1);
        ++num_trials;
        if (magic_output != lrint_output)
        {
            ++num_errors;
        }
    }
    for (float32_t input = ratl::SampleTypeLimits<float32_t>::min; input < -Min; input = std::nextafter(input, 0.f))
    {
        auto lrint_output = static_cast<std::int64_t>(convert<SampleType, LrintConverter>(input));
        auto magic_output = static_cast<std::int64_t>(convert<SampleType, MagicConverter>(input));
        EXPECT_GE(magic_output, lrint_output - 1);
        EXPECT_LE(magic_output, lrint_output + 1);
        ++num_trials;
        if (magic_output != lrint_output)
        {
            ++num_errors;
        }
    }
    auto error_percent = (static_cast<double>(num_errors) * 100) / static_cast<double>(num_trials);
    std::cout << "num errors: " << num_errors << "/" << num_trials << " (" << error_percent << "%)" << std::endl;
}

} // namespace test
} // namespace ratl
