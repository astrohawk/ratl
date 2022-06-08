/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

// ratl includes
#include <ratl/detail/batch_creator.hpp>

// ratl test includes
#include "test_utils.hpp"

#if defined(RATL_HAS_XSIMD)

namespace ratl
{
namespace test
{
template<class SampleType>
class BatchCreatorSamplePointerTest : public ::testing::Test
{
protected:
    using sample_type = SampleType;
    using test_batch_creator = detail::batch_creator<sample_type>;
    using sample_converter = detail::batch_creator_sample_converter<sample_type>;
    using batch_type = typename test_batch_creator::batch_type;

    static constexpr std::size_t batch_size = detail::batch_size;
    static constexpr std::size_t batch_alignment = detail::batch_alignment<batch_type>;

    static std::array<sample_type, batch_size + 1> generate_input_array()
    {
        alignas(detail::batch_alignment<batch_type>) std::array<sample_type, batch_size + 1> input_array{};
        for (std::size_t i = 0; i < input_array.size(); ++i)
        {
            input_array[i] = sample_converter::convert_output(static_cast<typename batch_type::value_type>(i));
        }
        return input_array;
    }

    static std::array<sample_type, batch_size + 1> generate_output_array()
    {
        return {};
    }

    static batch_type to_batch(const std::array<sample_type, batch_size + 1>& input_array)
    {
        alignas(detail::batch_alignment<batch_type>) std::array<typename batch_type::value_type, batch_size> buffer{};
        for (std::size_t i = 0; i < batch_size; ++i)
        {
            buffer[i] = sample_converter::convert_input(input_array[i].get());
        };
        return batch_type(buffer.data(), xsimd::aligned_mode());
    }

    static std::array<sample_type, batch_size> to_array(const batch_type& input)
    {
        alignas(detail::batch_alignment<batch_type>) std::array<typename batch_type::value_type, batch_size> buffer{};
        input.store_aligned(buffer.data());
        std::array<sample_type, batch_size> output{};
        std::transform(
            buffer.begin(),
            buffer.end(),
            output.begin(),
            [](typename batch_type::value_type value)
            {
                return sample_type(sample_converter::convert_output(value));
            });
        return output;
    }
};

TYPED_TEST_SUITE(BatchCreatorSamplePointerTest, PossibleSampleTypes, );

TYPED_TEST(BatchCreatorSamplePointerTest, AlignedPointerLoad)
{
    alignas(TestFixture::batch_alignment) auto input_array = TestFixture::generate_input_array();

    auto output_batch =
        TestFixture::test_batch_creator::load(input_array.data(), detail::batch_alignment_mode::aligned());

    auto output_array = TestFixture::to_array(output_batch);
    for (std::size_t i = 0; i < TestFixture::batch_size; ++i)
    {
        EXPECT_EQ(output_array[i], input_array[i]);
    }
}

TYPED_TEST(BatchCreatorSamplePointerTest, AlignedPointerStore)
{
    auto input_array = TestFixture::generate_input_array();
    auto input_batch = TestFixture::to_batch(input_array);

    alignas(TestFixture::batch_alignment) auto output_array = TestFixture::generate_output_array();
    TestFixture::test_batch_creator::store(input_batch, output_array.data(), detail::batch_alignment_mode::aligned());

    for (std::size_t i = 0; i < TestFixture::batch_size; ++i)
    {
        EXPECT_EQ(output_array[i], input_array[i]);
    }
}

TYPED_TEST(BatchCreatorSamplePointerTest, UnalignedPointerLoad)
{
    alignas(TestFixture::batch_alignment) auto input_array = TestFixture::generate_input_array();

    auto output_batch =
        TestFixture::test_batch_creator::load(input_array.data() + 1, detail::batch_alignment_mode::unaligned());

    auto output_array = TestFixture::to_array(output_batch);
    for (std::size_t i = 0; i < TestFixture::batch_size; ++i)
    {
        EXPECT_EQ(output_array[i], input_array[i + 1]);
    }
}

TYPED_TEST(BatchCreatorSamplePointerTest, UnalignedPointerStore)
{
    auto input_array = TestFixture::generate_input_array();
    auto input_batch = TestFixture::to_batch(input_array);

    alignas(TestFixture::batch_alignment) auto output_array = TestFixture::generate_output_array();
    TestFixture::test_batch_creator::store(
        input_batch, output_array.data() + 1, detail::batch_alignment_mode::unaligned());

    for (std::size_t i = 0; i < TestFixture::batch_size; ++i)
    {
        EXPECT_EQ(output_array[i + 1], input_array[i]);
    }
}
} // namespace test
} // namespace ratl

#endif
