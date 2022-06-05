/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

// ratl test includes
#include "test_utils.hpp"

namespace ratl
{
namespace test
{
template<typename NoninterleavedToInterleavedTransformCombination>
class TransformNoninterleaved : public TypicalTransformTestBase<NoninterleavedToInterleavedTransformCombination>
{
};

TYPED_TEST_SUITE(TransformNoninterleaved, SelectedNoninterleavedToInterleavedTransformCombinations, );

TYPED_TEST(TransformNoninterleaved, Typical)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper = TestFixture::generate_output_wrapper(input_container.channels(), input_container.frames());
    auto& output_container = output_wrapper.container();

    auto output_end = reference_transform(input_container.begin(), input_container.end(), output_container.begin());
    EXPECT_EQ(output_end, output_container.end());

    // verify all samples have been copied correctly
    for (size_t channel_num = 0; channel_num < input_container.channels(); ++channel_num)
    {
        for (size_t frame_num = 0; frame_num < input_container.frames(); ++frame_num)
        {
            EXPECT_EQ(
                output_container[frame_num][channel_num],
                reference_convert<typename TestFixture::output_sample_type>(input_container[channel_num][frame_num]));
        }
    }
}

TYPED_TEST(TransformNoninterleaved, MoreOutputFrames)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper =
        TestFixture::generate_output_wrapper(input_container.channels(), input_container.frames() + 1);
    auto& output_container = output_wrapper.container();

    auto output_end = reference_transform(input_container.begin(), input_container.end(), output_container.begin());
    EXPECT_EQ(output_end, output_container.end() - 1);

    // verify all samples have been copied correctly
    for (size_t channel_num = 0; channel_num < input_container.channels(); ++channel_num)
    {
        for (size_t frame_num = 0; frame_num < input_container.frames(); ++frame_num)
        {
            EXPECT_EQ(
                output_container[frame_num][channel_num],
                reference_convert<typename TestFixture::output_sample_type>(input_container[channel_num][frame_num]));
        }
    }

    // verify extra output frame is still empty
    auto extra_frame = output_container.frame(input_container.frames());
    for (auto sample_iter = extra_frame.begin(); sample_iter != extra_frame.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample_type());
    }
}

TYPED_TEST(TransformNoninterleaved, MoreOutputChannels)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper =
        TestFixture::generate_output_wrapper(input_container.channels() + 1, input_container.frames());
    auto& output_container = output_wrapper.container();

    auto output_end = reference_transform(input_container.begin(), input_container.end(), output_container.begin());
    EXPECT_EQ(output_end, output_container.end());

    // verify all samples have been copied correctly
    for (size_t channel_num = 0; channel_num < input_container.channels(); ++channel_num)
    {
        for (size_t frame_num = 0; frame_num < input_container.frames(); ++frame_num)
        {
            EXPECT_EQ(
                output_container[frame_num][channel_num],
                reference_convert<typename TestFixture::output_sample_type>(input_container[channel_num][frame_num]));
        }
    }

    // verify extra output channel is still empty
    auto extra_channel = output_container.channel(input_container.channels());
    for (auto sample_iter = extra_channel.begin(); sample_iter != extra_channel.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample_type());
    }
}

TYPED_TEST(TransformNoninterleaved, MoreOutputFramesMoreOutputChannels)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper =
        TestFixture::generate_output_wrapper(input_container.channels() + 1, input_container.frames() + 1);
    auto& output_container = output_wrapper.container();

    auto output_end = reference_transform(input_container.begin(), input_container.end(), output_container.begin());
    EXPECT_EQ(output_end, output_container.end() - 1);

    // verify all samples have been copied correctly
    for (size_t channel_num = 0; channel_num < input_container.channels(); ++channel_num)
    {
        for (size_t frame_num = 0; frame_num < input_container.frames(); ++frame_num)
        {
            EXPECT_EQ(
                output_container[frame_num][channel_num],
                reference_convert<typename TestFixture::output_sample_type>(input_container[channel_num][frame_num]));
        }
    }

    // verify extra output channel is still empty
    auto extra_channel = output_container.channel(input_container.channels());
    for (auto sample_iter = extra_channel.begin(); sample_iter != extra_channel.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample_type());
    }

    // verify extra output frame is still empty
    auto extra_frame = output_container.frame(input_container.frames());
    for (auto sample_iter = extra_frame.begin(); sample_iter != extra_frame.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample_type());
    }
}

TYPED_TEST(TransformNoninterleaved, ConstInputIterators)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper = TestFixture::generate_output_wrapper(input_container.channels(), input_container.frames());
    auto& output_container = output_wrapper.container();

    // const input iterators
    const auto input_begin = input_container.begin();
    const auto input_end = input_container.end();

    auto output_end = reference_transform(input_begin, input_end, output_container.begin());
    EXPECT_EQ(output_end, output_container.end());

    // verify all samples have been copied correctly
    for (size_t channel_num = 0; channel_num < input_container.channels(); ++channel_num)
    {
        for (size_t frame_num = 0; frame_num < input_container.frames(); ++frame_num)
        {
            EXPECT_EQ(
                output_container[frame_num][channel_num],
                reference_convert<typename TestFixture::output_sample_type>(input_container[channel_num][frame_num]));
        }
    }
}

template<typename NoninterleavedToInterleavedTransformCombination>
class TransformFrame : public TypicalTransformTestBase<NoninterleavedToInterleavedTransformCombination>
{
};

TYPED_TEST_SUITE(TransformFrame, SelectedNoninterleavedToInterleavedTransformCombinations, );

TYPED_TEST(TransformFrame, Typical)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper = TestFixture::generate_output_wrapper(input_container.channels(), input_container.frames());
    auto& output_container = output_wrapper.container();

    auto input_frame = input_container.frame(0);
    auto output_frame = output_container.frame(0);

    auto output_end = reference_transform(input_frame.begin(), input_frame.end(), output_frame.begin());
    EXPECT_EQ(output_end, output_frame.end());

    // verify all channels have been copied correctly
    for (size_t channel_num = 0; channel_num < input_frame.channels(); ++channel_num)
    {
        EXPECT_EQ(
            output_frame[channel_num],
            reference_convert<typename TestFixture::output_sample_type>(input_frame[channel_num]));
    }

    // verify all other output frames are still empty
    for (auto frame_iter = output_container.begin() + 1; frame_iter != output_container.end(); ++frame_iter)
    {
        for (auto sample_iter = frame_iter->begin(); sample_iter != frame_iter->end(); ++sample_iter)
        {
            EXPECT_EQ(*sample_iter, typename TestFixture::output_sample_type());
        }
    }
}

TYPED_TEST(TransformFrame, MoreOutputFrames)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper =
        TestFixture::generate_output_wrapper(input_container.channels(), input_container.frames() + 1);
    auto& output_container = output_wrapper.container();

    auto input_frame = input_container.frame(0);
    auto output_frame = output_container.frame(0);

    auto output_end = reference_transform(input_frame.begin(), input_frame.end(), output_frame.begin());
    EXPECT_EQ(output_end, output_frame.end());

    // verify all channels have been copied correctly
    for (size_t channel_num = 0; channel_num < input_frame.channels(); ++channel_num)
    {
        EXPECT_EQ(
            output_frame[channel_num],
            reference_convert<typename TestFixture::output_sample_type>(input_frame[channel_num]));
    }

    // verify all other output frames are still empty
    for (auto frame_iter = output_container.begin() + 1; frame_iter != output_container.end(); ++frame_iter)
    {
        for (auto sample_iter = frame_iter->begin(); sample_iter != frame_iter->end(); ++sample_iter)
        {
            EXPECT_EQ(*sample_iter, typename TestFixture::output_sample_type());
        }
    }
}

TYPED_TEST(TransformFrame, MoreOutputChannels)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper =
        TestFixture::generate_output_wrapper(input_container.channels() + 1, input_container.frames());
    auto& output_container = output_wrapper.container();

    auto input_frame = input_container.frame(0);
    auto output_frame = output_container.frame(0);

    auto output_end = reference_transform(input_frame.begin(), input_frame.end(), output_frame.begin());
    EXPECT_EQ(output_end, output_frame.end() - 1);

    // verify all channels have been copied correctly
    for (size_t channel_num = 0; channel_num < input_frame.channels(); ++channel_num)
    {
        EXPECT_EQ(
            output_frame[channel_num],
            reference_convert<typename TestFixture::output_sample_type>(input_frame[channel_num]));
    }

    // verify all other output frames are still empty
    for (auto frame_iter = output_container.begin() + 1; frame_iter != output_container.end(); ++frame_iter)
    {
        for (auto sample_iter = frame_iter->begin(); sample_iter != frame_iter->end(); ++sample_iter)
        {
            EXPECT_EQ(*sample_iter, typename TestFixture::output_sample_type());
        }
    }

    // verify extra output channel is still empty
    EXPECT_EQ(output_frame[input_frame.channels()], typename TestFixture::output_sample_type());
}

TYPED_TEST(TransformFrame, MoreOutputFramesMoreOutputChannels)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper =
        TestFixture::generate_output_wrapper(input_container.channels() + 1, input_container.frames() + 1);
    auto& output_container = output_wrapper.container();

    auto input_frame = input_container.frame(0);
    auto output_frame = output_container.frame(0);

    auto output_end = reference_transform(input_frame.begin(), input_frame.end(), output_frame.begin());
    EXPECT_EQ(output_end, output_frame.end() - 1);

    // verify all channels have been copied correctly
    for (size_t channel_num = 0; channel_num < input_frame.channels(); ++channel_num)
    {
        EXPECT_EQ(
            output_frame[channel_num],
            reference_convert<typename TestFixture::output_sample_type>(input_frame[channel_num]));
    }

    // verify all other output frames are still empty
    for (auto frame_iter = output_container.begin() + 1; frame_iter != output_container.end(); ++frame_iter)
    {
        for (auto sample_iter = frame_iter->begin(); sample_iter != frame_iter->end(); ++sample_iter)
        {
            EXPECT_EQ(*sample_iter, typename TestFixture::output_sample_type());
        }
    }

    // verify extra output channel is still empty
    EXPECT_EQ(output_frame[input_frame.channels()], typename TestFixture::output_sample_type());
}

TYPED_TEST(TransformFrame, ConstInputIterators)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper = TestFixture::generate_output_wrapper(input_container.channels(), input_container.frames());
    auto& output_container = output_wrapper.container();

    auto input_frame = input_container.frame(0);
    auto output_frame = output_container.frame(0);

    // const input iterators
    const auto input_begin = input_frame.begin();
    const auto input_end = input_frame.end();

    auto output_end = reference_transform(input_begin, input_end, output_frame.begin());
    EXPECT_EQ(output_end, output_frame.end());

    // verify all channels have been copied correctly
    for (size_t channel_num = 0; channel_num < input_frame.channels(); ++channel_num)
    {
        EXPECT_EQ(
            output_frame[channel_num],
            reference_convert<typename TestFixture::output_sample_type>(input_frame[channel_num]));
    }

    // verify all other output frames are still empty
    for (auto frame_iter = output_container.begin() + 1; frame_iter != output_container.end(); ++frame_iter)
    {
        for (auto sample_iter = frame_iter->begin(); sample_iter != frame_iter->end(); ++sample_iter)
        {
            EXPECT_EQ(*sample_iter, typename TestFixture::output_sample_type());
        }
    }
}

template<typename NoninterleavedToInterleavedTransformCombination>
class TransformChannel : public TypicalTransformTestBase<NoninterleavedToInterleavedTransformCombination>
{
};

TYPED_TEST_SUITE(TransformChannel, SelectedNoninterleavedToInterleavedTransformCombinations, );

TYPED_TEST(TransformChannel, Typical)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper = TestFixture::generate_output_wrapper(input_container.channels(), input_container.frames());
    auto& output_container = output_wrapper.container();

    auto input_channel = input_container.channel(0);
    auto output_channel = output_container.channel(0);

    auto output_end = reference_transform(input_channel.begin(), input_channel.end(), output_channel.begin());
    EXPECT_EQ(output_end, output_channel.end());

    // verify all samples have been copied correctly
    for (size_t sample_num = 0; sample_num < input_channel.samples(); ++sample_num)
    {
        EXPECT_EQ(
            output_channel[sample_num],
            reference_convert<typename TestFixture::output_sample_type>(input_channel[sample_num]));
    }

    // verify all other output channels are still empty
    for (auto frame_iter = output_container.begin(); frame_iter != output_container.end(); ++frame_iter)
    {
        for (auto sample_iter = frame_iter->begin() + 1; sample_iter != frame_iter->end(); ++sample_iter)
        {
            EXPECT_EQ(*sample_iter, typename TestFixture::output_sample_type());
        }
    }
}

TYPED_TEST(TransformChannel, MoreOutputFrames)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper =
        TestFixture::generate_output_wrapper(input_container.channels(), input_container.frames() + 1);
    auto& output_container = output_wrapper.container();

    auto input_channel = input_container.channel(0);
    auto output_channel = output_container.channel(0);

    auto output_end = reference_transform(input_channel.begin(), input_channel.end(), output_channel.begin());
    EXPECT_EQ(output_end, output_channel.end() - 1);

    // verify all samples have been copied correctly
    for (size_t sample_num = 0; sample_num < input_channel.samples(); ++sample_num)
    {
        EXPECT_EQ(
            output_channel[sample_num],
            reference_convert<typename TestFixture::output_sample_type>(input_channel[sample_num]));
    }

    // verify all other output channels are still empty
    for (auto frame_iter = output_container.begin(); frame_iter != output_container.end(); ++frame_iter)
    {
        for (auto sample_iter = frame_iter->begin() + 1; sample_iter != frame_iter->end(); ++sample_iter)
        {
            EXPECT_EQ(*sample_iter, typename TestFixture::output_sample_type());
        }
    }

    // verify extra output frame is still empty
    EXPECT_EQ(output_channel[input_channel.samples()], typename TestFixture::output_sample_type());
}

TYPED_TEST(TransformChannel, MoreOutputChannels)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper =
        TestFixture::generate_output_wrapper(input_container.channels() + 1, input_container.frames());
    auto& output_container = output_wrapper.container();

    auto input_channel = input_container.channel(0);
    auto output_channel = output_container.channel(0);

    auto output_end = reference_transform(input_channel.begin(), input_channel.end(), output_channel.begin());
    EXPECT_EQ(output_end, output_channel.end());

    // verify all samples have been copied correctly
    for (size_t sample_num = 0; sample_num < input_channel.samples(); ++sample_num)
    {
        EXPECT_EQ(
            output_channel[sample_num],
            reference_convert<typename TestFixture::output_sample_type>(input_channel[sample_num]));
    }

    // verify all other output channels are still empty
    for (auto frame_iter = output_container.begin(); frame_iter != output_container.end(); ++frame_iter)
    {
        for (auto sample_iter = frame_iter->begin() + 1; sample_iter != frame_iter->end(); ++sample_iter)
        {
            EXPECT_EQ(*sample_iter, typename TestFixture::output_sample_type());
        }
    }
}

TYPED_TEST(TransformChannel, MoreOutputFramesMoreOutputChannels)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper =
        TestFixture::generate_output_wrapper(input_container.channels() + 1, input_container.frames() + 1);
    auto& output_container = output_wrapper.container();

    auto input_channel = input_container.channel(0);
    auto output_channel = output_container.channel(0);

    auto output_end = reference_transform(input_channel.begin(), input_channel.end(), output_channel.begin());
    EXPECT_EQ(output_end, output_channel.end() - 1);

    // verify all samples have been copied correctly
    for (size_t sample_num = 0; sample_num < input_channel.samples(); ++sample_num)
    {
        EXPECT_EQ(
            output_channel[sample_num],
            reference_convert<typename TestFixture::output_sample_type>(input_channel[sample_num]));
    }

    // verify all other output channels are still empty
    for (auto frame_iter = output_container.begin(); frame_iter != output_container.end(); ++frame_iter)
    {
        for (auto sample_iter = frame_iter->begin() + 1; sample_iter != frame_iter->end(); ++sample_iter)
        {
            EXPECT_EQ(*sample_iter, typename TestFixture::output_sample_type());
        }
    }

    // verify extra output frame is still empty
    EXPECT_EQ(output_channel[input_channel.samples()], typename TestFixture::output_sample_type());
}

TYPED_TEST(TransformChannel, ConstInputIterators)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper = TestFixture::generate_output_wrapper(input_container.channels(), input_container.frames());
    auto& output_container = output_wrapper.container();

    auto input_channel = input_container.channel(0);
    auto output_channel = output_container.channel(0);

    // const input iterators
    const auto input_begin = input_channel.begin();
    const auto input_end = input_channel.end();

    auto output_end = reference_transform(input_begin, input_end, output_channel.begin());
    EXPECT_EQ(output_end, output_channel.end());

    // verify all samples have been copied correctly
    for (size_t sample_num = 0; sample_num < input_channel.samples(); ++sample_num)
    {
        EXPECT_EQ(
            output_channel[sample_num],
            reference_convert<typename TestFixture::output_sample_type>(input_channel[sample_num]));
    }

    // verify all other output channels are still empty
    for (auto frame_iter = output_container.begin(); frame_iter != output_container.end(); ++frame_iter)
    {
        for (auto sample_iter = frame_iter->begin() + 1; sample_iter != frame_iter->end(); ++sample_iter)
        {
            EXPECT_EQ(*sample_iter, typename TestFixture::output_sample_type());
        }
    }
}

template<typename NoninterleavedToInterleavedTransformCombination>
class TransformNoninterleavedSingleFrame :
    public SingleFrameTransformTestBase<NoninterleavedToInterleavedTransformCombination>
{
};

TYPED_TEST_SUITE(TransformNoninterleavedSingleFrame, SelectedNoninterleavedToInterleavedTransformCombinations, );

TYPED_TEST(TransformNoninterleavedSingleFrame, Typical)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper = TestFixture::generate_output_wrapper(input_container.channels(), input_container.frames());
    auto& output_container = output_wrapper.container();

    auto output_end = reference_transform(input_container.begin(), input_container.end(), output_container.begin());
    EXPECT_EQ(output_end, output_container.end());

    // verify all samples have been copied correctly
    for (size_t channel_num = 0; channel_num < input_container.channels(); ++channel_num)
    {
        EXPECT_EQ(
            output_container[0][channel_num],
            reference_convert<typename TestFixture::output_sample_type>(input_container[channel_num][0]));
    }
}

TYPED_TEST(TransformNoninterleavedSingleFrame, MoreOutputFrames)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper =
        TestFixture::generate_output_wrapper(input_container.channels(), input_container.frames() + 1);
    auto& output_container = output_wrapper.container();

    auto output_end = reference_transform(input_container.begin(), input_container.end(), output_container.begin());
    EXPECT_EQ(output_end, output_container.end() - 1);

    // verify all samples have been copied correctly
    for (size_t channel_num = 0; channel_num < input_container.channels(); ++channel_num)
    {
        EXPECT_EQ(
            output_container[0][channel_num],
            reference_convert<typename TestFixture::output_sample_type>(input_container[channel_num][0]));
    }

    // verify extra output frame is still empty
    auto extra_frame = output_container.frame(1);
    for (auto sample_iter = extra_frame.begin(); sample_iter != extra_frame.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample_type());
    }
}

TYPED_TEST(TransformNoninterleavedSingleFrame, MoreOutputChannels)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper =
        TestFixture::generate_output_wrapper(input_container.channels() + 1, input_container.frames());
    auto& output_container = output_wrapper.container();

    auto output_end = reference_transform(input_container.begin(), input_container.end(), output_container.begin());
    EXPECT_EQ(output_end, output_container.end());

    // verify all samples have been copied correctly
    for (size_t channel_num = 0; channel_num < input_container.channels(); ++channel_num)
    {
        EXPECT_EQ(
            output_container[0][channel_num],
            reference_convert<typename TestFixture::output_sample_type>(input_container[channel_num][0]));
    }

    // verify extra output channel is still empty
    EXPECT_EQ(output_container[0][input_container.channels()], typename TestFixture::output_sample_type());
}

TYPED_TEST(TransformNoninterleavedSingleFrame, MoreOutputFramesMoreOutputChannels)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper =
        TestFixture::generate_output_wrapper(input_container.channels() + 1, input_container.frames() + 1);
    auto& output_container = output_wrapper.container();

    auto output_end = reference_transform(input_container.begin(), input_container.end(), output_container.begin());
    EXPECT_EQ(output_end, output_container.end() - 1);

    // verify all samples have been copied correctly
    for (size_t channel_num = 0; channel_num < input_container.channels(); ++channel_num)
    {
        EXPECT_EQ(
            output_container[0][channel_num],
            reference_convert<typename TestFixture::output_sample_type>(input_container[channel_num][0]));
    }

    // verify extra output frame is still empty
    auto extra_frame = output_container.frame(1);
    for (auto sample_iter = extra_frame.begin(); sample_iter != extra_frame.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample_type());
    }

    // verify extra output channel is still empty
    EXPECT_EQ(output_container[0][input_container.channels()], typename TestFixture::output_sample_type());
}

template<typename NoninterleavedToInterleavedTransformCombination>
class TransformFrameSingleFrame : public SingleFrameTransformTestBase<NoninterleavedToInterleavedTransformCombination>
{
};

TYPED_TEST_SUITE(TransformFrameSingleFrame, SelectedNoninterleavedToInterleavedTransformCombinations, );

TYPED_TEST(TransformFrameSingleFrame, Typical)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper = TestFixture::generate_output_wrapper(input_container.channels(), input_container.frames());
    auto& output_container = output_wrapper.container();

    auto input_frame = input_container.frame(0);
    auto output_frame = output_container.frame(0);

    auto output_end = reference_transform(input_frame.begin(), input_frame.end(), output_frame.begin());
    EXPECT_EQ(output_end, output_frame.end());

    // verify all channels have been copied correctly
    for (size_t channel_num = 0; channel_num < input_frame.channels(); ++channel_num)
    {
        EXPECT_EQ(
            output_frame[channel_num],
            reference_convert<typename TestFixture::output_sample_type>(input_frame[channel_num]));
    }
}

TYPED_TEST(TransformFrameSingleFrame, MoreOutputFrames)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper =
        TestFixture::generate_output_wrapper(input_container.channels(), input_container.frames() + 1);
    auto& output_container = output_wrapper.container();

    auto input_frame = input_container.frame(0);
    auto output_frame = output_container.frame(0);

    auto output_end = reference_transform(input_frame.begin(), input_frame.end(), output_frame.begin());
    EXPECT_EQ(output_end, output_frame.end());

    // verify all channels have been copied correctly
    for (size_t channel_num = 0; channel_num < input_frame.channels(); ++channel_num)
    {
        EXPECT_EQ(
            output_frame[channel_num],
            reference_convert<typename TestFixture::output_sample_type>(input_frame[channel_num]));
    }

    // verify extra output frame is still empty
    auto extra_frame = output_container.frame(1);
    for (auto sample_iter = extra_frame.begin(); sample_iter != extra_frame.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample_type());
    }
}

TYPED_TEST(TransformFrameSingleFrame, MoreOutputChannels)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper =
        TestFixture::generate_output_wrapper(input_container.channels() + 1, input_container.frames());
    auto& output_container = output_wrapper.container();

    auto input_frame = input_container.frame(0);
    auto output_frame = output_container.frame(0);

    auto output_end = reference_transform(input_frame.begin(), input_frame.end(), output_frame.begin());
    EXPECT_EQ(output_end, output_frame.end() - 1);

    // verify all channels have been copied correctly
    for (size_t channel_num = 0; channel_num < input_frame.channels(); ++channel_num)
    {
        EXPECT_EQ(
            output_frame[channel_num],
            reference_convert<typename TestFixture::output_sample_type>(input_frame[channel_num]));
    }

    // verify all other output frames are still empty
    for (auto frame_iter = output_container.begin() + 1; frame_iter != output_container.end(); ++frame_iter)
    {
        for (auto sample_iter = frame_iter->begin(); sample_iter != frame_iter->end(); ++sample_iter)
        {
            EXPECT_EQ(*sample_iter, typename TestFixture::output_sample_type());
        }
    }

    // verify extra output channel is still empty
    EXPECT_EQ(output_frame[input_frame.channels()], typename TestFixture::output_sample_type());
}

TYPED_TEST(TransformFrameSingleFrame, MoreOutputFramesMoreOutputChannels)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper =
        TestFixture::generate_output_wrapper(input_container.channels() + 1, input_container.frames() + 1);
    auto& output_container = output_wrapper.container();

    auto input_frame = input_container.frame(0);
    auto output_frame = output_container.frame(0);

    auto output_end = reference_transform(input_frame.begin(), input_frame.end(), output_frame.begin());
    EXPECT_EQ(output_end, output_frame.end() - 1);

    // verify all channels have been copied correctly
    for (size_t channel_num = 0; channel_num < input_frame.channels(); ++channel_num)
    {
        EXPECT_EQ(
            output_frame[channel_num],
            reference_convert<typename TestFixture::output_sample_type>(input_frame[channel_num]));
    }

    // verify extra output frame is still empty
    auto extra_frame = output_container.frame(1);
    for (auto sample_iter = extra_frame.begin(); sample_iter != extra_frame.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample_type());
    }

    // verify extra output channel is still empty
    EXPECT_EQ(output_frame[input_frame.channels()], typename TestFixture::output_sample_type());
}

template<typename NoninterleavedToInterleavedTransformCombination>
class TransformChannelSingleFrame : public SingleFrameTransformTestBase<NoninterleavedToInterleavedTransformCombination>
{
};

TYPED_TEST_SUITE(TransformChannelSingleFrame, SelectedNoninterleavedToInterleavedTransformCombinations, );

TYPED_TEST(TransformChannelSingleFrame, Typical)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper = TestFixture::generate_output_wrapper(input_container.channels(), input_container.frames());
    auto& output_container = output_wrapper.container();

    auto input_channel = input_container.channel(0);
    auto output_channel = output_container.channel(0);

    auto output_end = reference_transform(input_channel.begin(), input_channel.end(), output_channel.begin());
    EXPECT_EQ(output_end, output_channel.end());

    // verify all samples have been copied correctly
    EXPECT_EQ(output_channel[0], reference_convert<typename TestFixture::output_sample_type>(input_channel[0]));

    // verify all other output channels are still empty
    for (auto channel_iter = output_channel.begin() + 1; channel_iter != output_channel.end(); ++channel_iter)
    {
        EXPECT_EQ(*channel_iter, typename TestFixture::output_sample_type());
    }
}

TYPED_TEST(TransformChannelSingleFrame, MoreOutputFrames)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper =
        TestFixture::generate_output_wrapper(input_container.channels(), input_container.frames() + 1);
    auto& output_container = output_wrapper.container();

    auto input_channel = input_container.channel(0);
    auto output_channel = output_container.channel(0);

    auto output_end = reference_transform(input_channel.begin(), input_channel.end(), output_channel.begin());
    EXPECT_EQ(output_end, output_channel.end() - 1);

    // verify all samples have been copied correctly
    EXPECT_EQ(output_channel[0], reference_convert<typename TestFixture::output_sample_type>(input_channel[0]));

    // verify all other output channels are still empty
    for (auto channel_iter = output_channel.begin() + 1; channel_iter != output_channel.end(); ++channel_iter)
    {
        EXPECT_EQ(*channel_iter, typename TestFixture::output_sample_type());
    }

    // verify extra output frame is still empty
    auto extra_frame = output_container.frame(1);
    for (auto sample_iter = extra_frame.begin(); sample_iter != extra_frame.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample_type());
    }
}

TYPED_TEST(TransformChannelSingleFrame, MoreOutputChannels)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper =
        TestFixture::generate_output_wrapper(input_container.channels() + 1, input_container.frames());
    auto& output_container = output_wrapper.container();

    auto input_channel = input_container.channel(0);
    auto output_channel = output_container.channel(0);

    auto output_end = reference_transform(input_channel.begin(), input_channel.end(), output_channel.begin());
    EXPECT_EQ(output_end, output_channel.end());

    // verify all samples have been copied correctly
    EXPECT_EQ(output_channel[0], reference_convert<typename TestFixture::output_sample_type>(input_channel[0]));

    // verify all other output channels are still empty
    for (auto channel_iter = output_channel.begin() + 1; channel_iter != output_channel.end(); ++channel_iter)
    {
        EXPECT_EQ(*channel_iter, typename TestFixture::output_sample_type());
    }
}

TYPED_TEST(TransformChannelSingleFrame, MoreOutputFramesMoreOutputChannels)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper =
        TestFixture::generate_output_wrapper(input_container.channels() + 1, input_container.frames() + 1);
    auto& output_container = output_wrapper.container();

    auto input_channel = input_container.channel(0);
    auto output_channel = output_container.channel(0);

    auto output_end = reference_transform(input_channel.begin(), input_channel.end(), output_channel.begin());
    EXPECT_EQ(output_end, output_channel.end() - 1);

    // verify all samples have been copied correctly
    EXPECT_EQ(output_channel[0], reference_convert<typename TestFixture::output_sample_type>(input_channel[0]));

    // verify all other output channels are still empty
    for (auto channel_iter = output_channel.begin() + 1; channel_iter != output_channel.end(); ++channel_iter)
    {
        EXPECT_EQ(*channel_iter, typename TestFixture::output_sample_type());
    }

    // verify extra output frame is still empty
    auto extra_frame = output_container.frame(1);
    for (auto sample_iter = extra_frame.begin(); sample_iter != extra_frame.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample_type());
    }
}

template<typename NoninterleavedToInterleavedTransformCombination>
class TransformNoninterleavedSingleChannel :
    public SingleChannelTransformTestBase<NoninterleavedToInterleavedTransformCombination>
{
};

TYPED_TEST_SUITE(TransformNoninterleavedSingleChannel, SelectedNoninterleavedToInterleavedTransformCombinations, );

TYPED_TEST(TransformNoninterleavedSingleChannel, Typical)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper = TestFixture::generate_output_wrapper(input_container.channels(), input_container.frames());
    auto& output_container = output_wrapper.container();

    auto output_end = reference_transform(input_container.begin(), input_container.end(), output_container.begin());
    EXPECT_EQ(output_end, output_container.end());

    // verify all samples have been copied correctly
    for (size_t frame_num = 0; frame_num < input_container.frames(); ++frame_num)
    {
        EXPECT_EQ(
            output_container[frame_num][0],
            reference_convert<typename TestFixture::output_sample_type>(input_container[0][frame_num]));
    }
}

TYPED_TEST(TransformNoninterleavedSingleChannel, MoreOutputFrames)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper =
        TestFixture::generate_output_wrapper(input_container.channels(), input_container.frames() + 1);
    auto& output_container = output_wrapper.container();

    auto output_end = reference_transform(input_container.begin(), input_container.end(), output_container.begin());
    EXPECT_EQ(output_end, output_container.end() - 1);

    // verify all samples have been copied correctly
    for (size_t frame_num = 0; frame_num < input_container.frames(); ++frame_num)
    {
        EXPECT_EQ(
            output_container[frame_num][0],
            reference_convert<typename TestFixture::output_sample_type>(input_container[0][frame_num]));
    }

    // verify extra output frame is still empty
    EXPECT_EQ(output_container[input_container.frames()][0], typename TestFixture::output_sample_type());
}

TYPED_TEST(TransformNoninterleavedSingleChannel, MoreOutputChannels)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper =
        TestFixture::generate_output_wrapper(input_container.channels() + 1, input_container.frames());
    auto& output_container = output_wrapper.container();

    auto output_end = reference_transform(input_container.begin(), input_container.end(), output_container.begin());
    EXPECT_EQ(output_end, output_container.end());

    // verify all samples have been copied correctly
    for (size_t frame_num = 0; frame_num < input_container.frames(); ++frame_num)
    {
        EXPECT_EQ(
            output_container[frame_num][0],
            reference_convert<typename TestFixture::output_sample_type>(input_container[0][frame_num]));
    }

    // verify extra output channel is still empty
    auto extra_channel = output_container.channel(1);
    for (auto sample_iter = extra_channel.begin(); sample_iter != extra_channel.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample_type());
    }
}

TYPED_TEST(TransformNoninterleavedSingleChannel, MoreOutputFramesMoreOutputChannels)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper =
        TestFixture::generate_output_wrapper(input_container.channels() + 1, input_container.frames() + 1);
    auto& output_container = output_wrapper.container();

    auto output_end = reference_transform(input_container.begin(), input_container.end(), output_container.begin());
    EXPECT_EQ(output_end, output_container.end() - 1);

    // verify all samples have been copied correctly
    for (size_t frame_num = 0; frame_num < input_container.frames(); ++frame_num)
    {
        EXPECT_EQ(
            output_container[frame_num][0],
            reference_convert<typename TestFixture::output_sample_type>(input_container[0][frame_num]));
    }

    // verify extra output frame is still empty
    EXPECT_EQ(output_container[input_container.frames()][0], typename TestFixture::output_sample_type());

    // verify extra output channel is still empty
    auto extra_channel = output_container.channel(1);
    for (auto sample_iter = extra_channel.begin(); sample_iter != extra_channel.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample_type());
    }
}

template<typename NoninterleavedToInterleavedTransformCombination>
class TransformFrameSingleChannel :
    public SingleChannelTransformTestBase<NoninterleavedToInterleavedTransformCombination>
{
};

TYPED_TEST_SUITE(TransformFrameSingleChannel, SelectedNoninterleavedToInterleavedTransformCombinations, );

TYPED_TEST(TransformFrameSingleChannel, Typical)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper = TestFixture::generate_output_wrapper(input_container.channels(), input_container.frames());
    auto& output_container = output_wrapper.container();

    auto input_frame = input_container.frame(0);
    auto output_frame = output_container.frame(0);

    auto output_end = reference_transform(input_frame.begin(), input_frame.end(), output_frame.begin());
    EXPECT_EQ(output_end, output_frame.end());

    // verify sample have been copied correctly
    EXPECT_EQ(output_frame[0], reference_convert<typename TestFixture::output_sample_type>(input_frame[0]));

    // verify all other output frames are still empty
    for (auto frame_iter = output_container.begin() + 1; frame_iter != output_container.end(); ++frame_iter)
    {
        EXPECT_EQ((*frame_iter)[0], typename TestFixture::output_sample_type());
    }
}

TYPED_TEST(TransformFrameSingleChannel, MoreOutputFrames)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper =
        TestFixture::generate_output_wrapper(input_container.channels(), input_container.frames() + 1);
    auto& output_container = output_wrapper.container();

    auto input_frame = input_container.frame(0);
    auto output_frame = output_container.frame(0);

    auto output_end = reference_transform(input_frame.begin(), input_frame.end(), output_frame.begin());
    EXPECT_EQ(output_end, output_frame.end());

    // verify sample have been copied correctly
    EXPECT_EQ(output_frame[0], reference_convert<typename TestFixture::output_sample_type>(input_frame[0]));

    // verify all other output frames are still empty
    for (auto frame_iter = output_container.begin() + 1; frame_iter != output_container.end(); ++frame_iter)
    {
        EXPECT_EQ((*frame_iter)[0], typename TestFixture::output_sample_type());
    }
}

TYPED_TEST(TransformFrameSingleChannel, MoreOutputChannels)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper =
        TestFixture::generate_output_wrapper(input_container.channels() + 1, input_container.frames());
    auto& output_container = output_wrapper.container();

    auto input_frame = input_container.frame(0);
    auto output_frame = output_container.frame(0);

    auto output_end = reference_transform(input_frame.begin(), input_frame.end(), output_frame.begin());
    EXPECT_EQ(output_end, output_frame.end() - 1);

    // verify sample have been copied correctly
    EXPECT_EQ(output_frame[0], reference_convert<typename TestFixture::output_sample_type>(input_frame[0]));

    // verify all other output frames are still empty
    for (auto frame_iter = output_container.begin() + 1; frame_iter != output_container.end(); ++frame_iter)
    {
        EXPECT_EQ((*frame_iter)[0], typename TestFixture::output_sample_type());
    }

    // verify extra output channel is still empty
    auto extra_channel = output_container.channel(1);
    for (auto sample_iter = extra_channel.begin(); sample_iter != extra_channel.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample_type());
    }
}

TYPED_TEST(TransformFrameSingleChannel, MoreOutputFramesMoreOutputChannels)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper =
        TestFixture::generate_output_wrapper(input_container.channels() + 1, input_container.frames() + 1);
    auto& output_container = output_wrapper.container();

    auto input_frame = input_container.frame(0);
    auto output_frame = output_container.frame(0);

    auto output_end = reference_transform(input_frame.begin(), input_frame.end(), output_frame.begin());
    EXPECT_EQ(output_end, output_frame.end() - 1);

    // verify sample have been copied correctly
    EXPECT_EQ(output_frame[0], reference_convert<typename TestFixture::output_sample_type>(input_frame[0]));

    // verify all other output frames are still empty
    for (auto frame_iter = output_container.begin() + 1; frame_iter != output_container.end(); ++frame_iter)
    {
        EXPECT_EQ((*frame_iter)[0], typename TestFixture::output_sample_type());
    }

    // verify extra output channel is still empty
    auto extra_channel = output_container.channel(1);
    for (auto sample_iter = extra_channel.begin(); sample_iter != extra_channel.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample_type());
    }
}

template<typename NoninterleavedToInterleavedTransformCombination>
class TransformChannelSingleChannel :
    public SingleChannelTransformTestBase<NoninterleavedToInterleavedTransformCombination>
{
};

TYPED_TEST_SUITE(TransformChannelSingleChannel, SelectedNoninterleavedToInterleavedTransformCombinations, );

TYPED_TEST(TransformChannelSingleChannel, Typical)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper = TestFixture::generate_output_wrapper(input_container.channels(), input_container.frames());
    auto& output_container = output_wrapper.container();

    auto input_channel = input_container.channel(0);
    auto output_channel = output_container.channel(0);

    auto output_end = reference_transform(input_channel.begin(), input_channel.end(), output_channel.begin());
    EXPECT_EQ(output_end, output_channel.end());

    // verify all samples have been copied correctly
    for (size_t sample_num = 0; sample_num < input_channel.samples(); ++sample_num)
    {
        EXPECT_EQ(
            output_channel[sample_num],
            reference_convert<typename TestFixture::output_sample_type>(input_channel[sample_num]));
    }
}

TYPED_TEST(TransformChannelSingleChannel, MoreOutputFrames)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper =
        TestFixture::generate_output_wrapper(input_container.channels(), input_container.frames() + 1);
    auto& output_container = output_wrapper.container();

    auto input_channel = input_container.channel(0);
    auto output_channel = output_container.channel(0);

    auto output_end = reference_transform(input_channel.begin(), input_channel.end(), output_channel.begin());
    EXPECT_EQ(output_end, output_channel.end() - 1);

    // verify all samples have been copied correctly
    for (size_t sample_num = 0; sample_num < input_channel.samples(); ++sample_num)
    {
        EXPECT_EQ(
            output_channel[sample_num],
            reference_convert<typename TestFixture::output_sample_type>(input_channel[sample_num]));
    }

    // verify extra output frame is still empty
    EXPECT_EQ(output_channel[input_channel.samples()], typename TestFixture::output_sample_type());
}

TYPED_TEST(TransformChannelSingleChannel, MoreOutputChannels)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper =
        TestFixture::generate_output_wrapper(input_container.channels() + 1, input_container.frames());
    auto& output_container = output_wrapper.container();

    auto input_channel = input_container.channel(0);
    auto output_channel = output_container.channel(0);

    auto output_end = reference_transform(input_channel.begin(), input_channel.end(), output_channel.begin());
    EXPECT_EQ(output_end, output_channel.end());

    // verify all samples have been copied correctly
    for (size_t sample_num = 0; sample_num < input_channel.samples(); ++sample_num)
    {
        EXPECT_EQ(
            output_channel[sample_num],
            reference_convert<typename TestFixture::output_sample_type>(input_channel[sample_num]));
    }

    // verify extra output channel is still empty
    auto extra_channel = output_container.channel(1);
    for (auto sample_iter = extra_channel.begin(); sample_iter != extra_channel.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample_type());
    }
}

TYPED_TEST(TransformChannelSingleChannel, MoreOutputFramesMoreOutputChannels)
{
    const auto& input_container = TestFixture::container();
    auto output_wrapper =
        TestFixture::generate_output_wrapper(input_container.channels() + 1, input_container.frames() + 1);
    auto& output_container = output_wrapper.container();

    auto input_channel = input_container.channel(0);
    auto output_channel = output_container.channel(0);

    auto output_end = reference_transform(input_channel.begin(), input_channel.end(), output_channel.begin());
    EXPECT_EQ(output_end, output_channel.end() - 1);

    // verify all samples have been copied correctly
    for (size_t sample_num = 0; sample_num < input_channel.samples(); ++sample_num)
    {
        EXPECT_EQ(
            output_channel[sample_num],
            reference_convert<typename TestFixture::output_sample_type>(input_channel[sample_num]));
    }

    // verify extra output frame is still empty
    EXPECT_EQ(output_channel[input_channel.samples()], typename TestFixture::output_sample_type());

    // verify extra output channel is still empty
    auto extra_channel = output_container.channel(1);
    for (auto sample_iter = extra_channel.begin(); sample_iter != extra_channel.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample_type());
    }
}

} // namespace test
} // namespace ratl
