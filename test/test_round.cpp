
#include "test_utils.hpp"

namespace ratl
{
namespace test
{
template<typename SampleValueType, typename Converter>
SampleValueType convert(ratl::float32_t sample)
{
    if (RATL_UNLIKELY (sample >= ratl::sample_limits<ratl::float32_t>::max))
    {
        return ratl::sample_limits<SampleValueType>::max;
    }
    if (RATL_UNLIKELY (sample < ratl::sample_limits<ratl::float32_t>::min))
    {
        return ratl::sample_limits<SampleValueType>::min;
    }
    return static_cast<SampleValueType>(
        Converter::convert(sample * detail::float_convert_traits<SampleValueType>::multiplier));
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
        return detail::round_float32_to_int32_magic(sample);
    }
};

template<typename SampleTypeArg>
class TestRound : public testing::Test
{
public:
    using sample_type = SampleTypeArg;
};

TYPED_TEST_SUITE(TestRound, PossibleIntSampleTypes, );

TYPED_TEST(TestRound, MagicEqualToRoundHalfUp)
{
    using sample_type = typename TestFixture::sample_type;

    static constexpr float32_t FloatMin = 1e-10;

    for (float32_t input = ratl::sample_limits<float32_t>::max; input > FloatMin;
         input = std::nextafter(input, 0.f))
    {
        auto reference_output = static_cast<std::int64_t>(convert<sample_type, RoundHalfUpConverter>(input));
        auto test_output = static_cast<std::int64_t>(convert<sample_type, MagicConverter>(input));
        EXPECT_EQ(test_output, reference_output);
    }
    for (float32_t input = ratl::sample_limits<float32_t>::min; input < -FloatMin;
         input = std::nextafter(input, 0.f))
    {
        auto reference_output = static_cast<std::int64_t>(convert<sample_type, RoundHalfUpConverter>(input));
        auto test_output = static_cast<std::int64_t>(convert<sample_type, MagicConverter>(input));
        EXPECT_EQ(test_output, reference_output);
    }
}

} // namespace test
} // namespace ratl
