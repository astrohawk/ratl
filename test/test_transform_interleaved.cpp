
#include "test_utils.hpp"

namespace ratl
{
template<class SampleTypeCombination, template<typename InputSampleType> class InterleavedBaseType>
class TransformInterleavedBase : public InterleavedBaseType<typename SampleTypeCombination::InputSampleType>
{
protected:
    using super_type = InterleavedBaseType<typename SampleTypeCombination::InputSampleType>;

    using InputSampleType = typename super_type::sample_type;
    using input_sample = typename super_type::sample;
    using input_buffer = typename super_type::buffer;

    using OutputSampleType = typename SampleTypeCombination::OutputSampleType;
    using output_sample = ratl::Sample<OutputSampleType>;
    using output_buffer = ratl::BasicInterleaved<output_sample>;

    TransformInterleavedBase()
    {
        for (size_t i = 0; i < super_type::buffer_.samples(); ++i)
        {
            super_type::buffer_.data()[i] = ratl::convert<input_sample>(ratl::Sample<OutputSampleType>{i + 1}).get();
        }
    }

    static output_buffer generate_output(size_t channels, size_t frames)
    {
        return output_buffer{channels, frames};
    }
};

template<class SampleTypeCombination>
class TransformTypicalInterleavedBase : public TransformInterleavedBase<SampleTypeCombination, TypicalInterleavedBase>
{
};

template<class SampleTypeCombination>
class TransformSingleFrameInterleavedBase :
    public TransformInterleavedBase<SampleTypeCombination, SingleFrameInterleavedBase>
{
};

template<class SampleTypeCombination>
class TransformSingleChannelInterleavedBase :
    public TransformInterleavedBase<SampleTypeCombination, SingleChannelInterleavedBase>
{
};

template<class SampleTypeCombination>
class TransformInterleaved : public TransformTypicalInterleavedBase<SampleTypeCombination>
{
};

TYPED_TEST_SUITE(TransformInterleaved, PossibleSampleTypeCombinations, );

TYPED_TEST(TransformInterleaved, Typical)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels(), input_buffer.frames());

    auto output_end = ratl::transform(input_buffer.begin(), input_buffer.end(), output_buffer.begin());
    EXPECT_EQ(output_end, output_buffer.end());

    // verify all samples have been copied correctly
    for (size_t frame_num = 0; frame_num < input_buffer.frames(); ++frame_num)
    {
        for (size_t channel_num = 0; channel_num < input_buffer.channels(); ++channel_num)
        {
            EXPECT_EQ(input_buffer[frame_num][channel_num], output_buffer[frame_num][channel_num]);
        }
    }
}

TYPED_TEST(TransformInterleaved, MoreOutputFrames)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels(), input_buffer.frames() + 1);

    auto output_end = ratl::transform(input_buffer.begin(), input_buffer.end(), output_buffer.begin());
    EXPECT_EQ(output_end, output_buffer.end() - 1);

    // verify all samples have been copied correctly
    for (size_t frame_num = 0; frame_num < input_buffer.frames(); ++frame_num)
    {
        for (size_t channel_num = 0; channel_num < input_buffer.channels(); ++channel_num)
        {
            EXPECT_EQ(input_buffer[frame_num][channel_num], output_buffer[frame_num][channel_num]);
        }
    }

    // verify extra output frame is still empty
    auto extra_frame = output_buffer.frame(input_buffer.frames());
    for (auto sample_iter = extra_frame.begin(); sample_iter != extra_frame.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample{});
    }
}

TYPED_TEST(TransformInterleaved, MoreOutputChannels)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels() + 1, input_buffer.frames());

    auto output_end = ratl::transform(input_buffer.begin(), input_buffer.end(), output_buffer.begin());
    EXPECT_EQ(output_end, output_buffer.end());

    // verify all samples have been copied correctly
    for (size_t frame_num = 0; frame_num < input_buffer.frames(); ++frame_num)
    {
        for (size_t channel_num = 0; channel_num < input_buffer.channels(); ++channel_num)
        {
            EXPECT_EQ(input_buffer[frame_num][channel_num], output_buffer[frame_num][channel_num]);
        }
    }

    // verify extra output channel is still empty
    auto extra_channel = output_buffer.channel(input_buffer.channels());
    for (auto sample_iter = extra_channel.begin(); sample_iter != extra_channel.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample{});
    }
}

TYPED_TEST(TransformInterleaved, MoreOutputFramesMoreOutputChannels)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels() + 1, input_buffer.frames() + 1);

    auto output_end = ratl::transform(input_buffer.begin(), input_buffer.end(), output_buffer.begin());
    EXPECT_EQ(output_end, output_buffer.end() - 1);

    // verify all samples have been copied correctly
    for (size_t frame_num = 0; frame_num < input_buffer.frames(); ++frame_num)
    {
        for (size_t channel_num = 0; channel_num < input_buffer.channels(); ++channel_num)
        {
            EXPECT_EQ(input_buffer[frame_num][channel_num], output_buffer[frame_num][channel_num]);
        }
    }

    // verify extra output channel is still empty
    auto extra_channel = output_buffer.channel(input_buffer.channels());
    for (auto sample_iter = extra_channel.begin(); sample_iter != extra_channel.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample{});
    }

    // verify extra output frame is still empty
    auto extra_frame = output_buffer.frame(input_buffer.frames());
    for (auto sample_iter = extra_frame.begin(); sample_iter != extra_frame.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample{});
    }
}

TYPED_TEST(TransformInterleaved, ConstInputIterators)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels(), input_buffer.frames());

    // const input iterators
    const auto input_begin = input_buffer.begin();
    const auto input_end = input_buffer.end();

    auto output_end = ratl::transform(input_begin, input_end, output_buffer.begin());
    EXPECT_EQ(output_end, output_buffer.end());

    // verify all samples have been copied correctly
    for (size_t frame_num = 0; frame_num < input_buffer.frames(); ++frame_num)
    {
        for (size_t channel_num = 0; channel_num < input_buffer.channels(); ++channel_num)
        {
            EXPECT_EQ(input_buffer[frame_num][channel_num], output_buffer[frame_num][channel_num]);
        }
    }
}

template<class SampleTypeCombination>
class TransformFrame : public TransformTypicalInterleavedBase<SampleTypeCombination>
{
};

TYPED_TEST_SUITE(TransformFrame, PossibleSampleTypeCombinations, );

TYPED_TEST(TransformFrame, Typical)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels(), input_buffer.frames());

    auto input_frame = input_buffer.frame(0);
    auto output_frame = output_buffer.frame(0);

    auto output_end = ratl::transform(input_frame.begin(), input_frame.end(), output_frame.begin());
    EXPECT_EQ(output_end, output_frame.end());

    // verify all channels have been copied correctly
    for (size_t channel_num = 0; channel_num < input_frame.channels(); ++channel_num)
    {
        EXPECT_EQ(input_frame[channel_num], output_frame[channel_num]);
    }

    // verify all other output frames are still empty
    for (auto frame_iter = output_buffer.begin() + 1; frame_iter != output_buffer.end(); ++frame_iter)
    {
        for (auto sample_iter = frame_iter->begin(); sample_iter != frame_iter->end(); ++sample_iter)
        {
            EXPECT_EQ(*sample_iter, typename TestFixture::output_sample{});
        }
    }
}

TYPED_TEST(TransformFrame, MoreOutputFrames)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels(), input_buffer.frames() + 1);

    auto input_frame = input_buffer.frame(0);
    auto output_frame = output_buffer.frame(0);

    auto output_end = ratl::transform(input_frame.begin(), input_frame.end(), output_frame.begin());
    EXPECT_EQ(output_end, output_frame.end());

    // verify all channels have been copied correctly
    for (size_t channel_num = 0; channel_num < input_frame.channels(); ++channel_num)
    {
        EXPECT_EQ(input_frame[channel_num], output_frame[channel_num]);
    }

    // verify all other output frames are still empty
    for (auto frame_iter = output_buffer.begin() + 1; frame_iter != output_buffer.end(); ++frame_iter)
    {
        for (auto sample_iter = frame_iter->begin(); sample_iter != frame_iter->end(); ++sample_iter)
        {
            EXPECT_EQ(*sample_iter, typename TestFixture::output_sample{});
        }
    }
}

TYPED_TEST(TransformFrame, MoreOutputChannels)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels() + 1, input_buffer.frames());

    auto input_frame = input_buffer.frame(0);
    auto output_frame = output_buffer.frame(0);

    auto output_end = ratl::transform(input_frame.begin(), input_frame.end(), output_frame.begin());
    EXPECT_EQ(output_end, output_frame.end() - 1);

    // verify all channels have been copied correctly
    for (size_t channel_num = 0; channel_num < input_frame.channels(); ++channel_num)
    {
        EXPECT_EQ(input_frame[channel_num], output_frame[channel_num]);
    }

    // verify all other output frames are still empty
    for (auto frame_iter = output_buffer.begin() + 1; frame_iter != output_buffer.end(); ++frame_iter)
    {
        for (auto sample_iter = frame_iter->begin(); sample_iter != frame_iter->end(); ++sample_iter)
        {
            EXPECT_EQ(*sample_iter, typename TestFixture::output_sample{});
        }
    }

    // verify extra output channel is still empty
    EXPECT_EQ(output_frame[input_frame.channels()], typename TestFixture::output_sample{});
}

TYPED_TEST(TransformFrame, MoreOutputFramesMoreOutputChannels)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels() + 1, input_buffer.frames() + 1);

    auto input_frame = input_buffer.frame(0);
    auto output_frame = output_buffer.frame(0);

    auto output_end = ratl::transform(input_frame.begin(), input_frame.end(), output_frame.begin());
    EXPECT_EQ(output_end, output_frame.end() - 1);

    // verify all channels have been copied correctly
    for (size_t channel_num = 0; channel_num < input_frame.channels(); ++channel_num)
    {
        EXPECT_EQ(input_frame[channel_num], output_frame[channel_num]);
    }

    // verify all other output frames are still empty
    for (auto frame_iter = output_buffer.begin() + 1; frame_iter != output_buffer.end(); ++frame_iter)
    {
        for (auto sample_iter = frame_iter->begin(); sample_iter != frame_iter->end(); ++sample_iter)
        {
            EXPECT_EQ(*sample_iter, typename TestFixture::output_sample{});
        }
    }

    // verify extra output channel is still empty
    EXPECT_EQ(output_frame[input_frame.channels()], typename TestFixture::output_sample{});
}

TYPED_TEST(TransformFrame, ConstInputIterators)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels(), input_buffer.frames());

    auto input_frame = input_buffer.frame(0);
    auto output_frame = output_buffer.frame(0);

    // const input iterators
    const auto input_begin = input_frame.begin();
    const auto input_end = input_frame.end();

    auto output_end = ratl::transform(input_begin, input_end, output_frame.begin());
    EXPECT_EQ(output_end, output_frame.end());

    // verify all channels have been copied correctly
    for (size_t channel_num = 0; channel_num < input_frame.channels(); ++channel_num)
    {
        EXPECT_EQ(input_frame[channel_num], output_frame[channel_num]);
    }

    // verify all other output frames are still empty
    for (auto frame_iter = output_buffer.begin() + 1; frame_iter != output_buffer.end(); ++frame_iter)
    {
        for (auto sample_iter = frame_iter->begin(); sample_iter != frame_iter->end(); ++sample_iter)
        {
            EXPECT_EQ(*sample_iter, typename TestFixture::output_sample{});
        }
    }
}

template<class SampleTypeCombination>
class TransformChannel : public TransformTypicalInterleavedBase<SampleTypeCombination>
{
};

TYPED_TEST_SUITE(TransformChannel, PossibleSampleTypeCombinations, );

TYPED_TEST(TransformChannel, Typical)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels(), input_buffer.frames());

    auto input_channel = input_buffer.channel(0);
    auto output_channel = output_buffer.channel(0);

    auto output_end = ratl::transform(input_channel.begin(), input_channel.end(), output_channel.begin());
    EXPECT_EQ(output_end, output_channel.end());

    // verify all samples have been copied correctly
    for (size_t sample_num = 0; sample_num < input_channel.samples(); ++sample_num)
    {
        EXPECT_EQ(input_channel[sample_num], output_channel[sample_num]);
    }

    // verify all other output channels are still empty
    for (auto frame_iter = output_buffer.begin(); frame_iter != output_buffer.end(); ++frame_iter)
    {
        for (auto sample_iter = frame_iter->begin() + 1; sample_iter != frame_iter->end(); ++sample_iter)
        {
            EXPECT_EQ(*sample_iter, typename TestFixture::output_sample{});
        }
    }
}

TYPED_TEST(TransformChannel, MoreOutputFrames)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels(), input_buffer.frames() + 1);

    auto input_channel = input_buffer.channel(0);
    auto output_channel = output_buffer.channel(0);

    auto output_end = ratl::transform(input_channel.begin(), input_channel.end(), output_channel.begin());
    EXPECT_EQ(output_end, output_channel.end() - 1);

    // verify all samples have been copied correctly
    for (size_t sample_num = 0; sample_num < input_channel.samples(); ++sample_num)
    {
        EXPECT_EQ(input_channel[sample_num], output_channel[sample_num]);
    }

    // verify all other output channels are still empty
    for (auto frame_iter = output_buffer.begin(); frame_iter != output_buffer.end(); ++frame_iter)
    {
        for (auto sample_iter = frame_iter->begin() + 1; sample_iter != frame_iter->end(); ++sample_iter)
        {
            EXPECT_EQ(*sample_iter, typename TestFixture::output_sample{});
        }
    }

    // verify extra output frame is still empty
    EXPECT_EQ(output_channel[input_channel.samples()], typename TestFixture::output_sample{});
}

TYPED_TEST(TransformChannel, MoreOutputChannels)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels() + 1, input_buffer.frames());

    auto input_channel = input_buffer.channel(0);
    auto output_channel = output_buffer.channel(0);

    auto output_end = ratl::transform(input_channel.begin(), input_channel.end(), output_channel.begin());
    EXPECT_EQ(output_end, output_channel.end());

    // verify all samples have been copied correctly
    for (size_t sample_num = 0; sample_num < input_channel.samples(); ++sample_num)
    {
        EXPECT_EQ(input_channel[sample_num], output_channel[sample_num]);
    }

    // verify all other output channels are still empty
    for (auto frame_iter = output_buffer.begin(); frame_iter != output_buffer.end(); ++frame_iter)
    {
        for (auto sample_iter = frame_iter->begin() + 1; sample_iter != frame_iter->end(); ++sample_iter)
        {
            EXPECT_EQ(*sample_iter, typename TestFixture::output_sample{});
        }
    }
}

TYPED_TEST(TransformChannel, MoreOutputFramesMoreOutputChannels)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels() + 1, input_buffer.frames() + 1);

    auto input_channel = input_buffer.channel(0);
    auto output_channel = output_buffer.channel(0);

    auto output_end = ratl::transform(input_channel.begin(), input_channel.end(), output_channel.begin());
    EXPECT_EQ(output_end, output_channel.end() - 1);

    // verify all samples have been copied correctly
    for (size_t sample_num = 0; sample_num < input_channel.samples(); ++sample_num)
    {
        EXPECT_EQ(input_channel[sample_num], output_channel[sample_num]);
    }

    // verify all other output channels are still empty
    for (auto frame_iter = output_buffer.begin(); frame_iter != output_buffer.end(); ++frame_iter)
    {
        for (auto sample_iter = frame_iter->begin() + 1; sample_iter != frame_iter->end(); ++sample_iter)
        {
            EXPECT_EQ(*sample_iter, typename TestFixture::output_sample{});
        }
    }

    // verify extra output frame is still empty
    EXPECT_EQ(output_channel[input_channel.samples()], typename TestFixture::output_sample{});
}

TYPED_TEST(TransformChannel, ConstInputIterators)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels(), input_buffer.frames());

    auto input_channel = input_buffer.channel(0);
    auto output_channel = output_buffer.channel(0);

    // const input iterators
    const auto input_begin = input_channel.begin();
    const auto input_end = input_channel.end();

    auto output_end = ratl::transform(input_begin, input_end, output_channel.begin());
    EXPECT_EQ(output_end, output_channel.end());

    // verify all samples have been copied correctly
    for (size_t sample_num = 0; sample_num < input_channel.samples(); ++sample_num)
    {
        EXPECT_EQ(input_channel[sample_num], output_channel[sample_num]);
    }

    // verify all other output channels are still empty
    for (auto frame_iter = output_buffer.begin(); frame_iter != output_buffer.end(); ++frame_iter)
    {
        for (auto sample_iter = frame_iter->begin() + 1; sample_iter != frame_iter->end(); ++sample_iter)
        {
            EXPECT_EQ(*sample_iter, typename TestFixture::output_sample{});
        }
    }
}

template<class SampleTypeCombination>
class TransformInterleavedSingleFrame : public TransformSingleFrameInterleavedBase<SampleTypeCombination>
{
};

TYPED_TEST_SUITE(TransformInterleavedSingleFrame, PossibleSampleTypeCombinations, );

TYPED_TEST(TransformInterleavedSingleFrame, Typical)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels(), input_buffer.frames());

    auto output_end = ratl::transform(input_buffer.begin(), input_buffer.end(), output_buffer.begin());
    EXPECT_EQ(output_end, output_buffer.end());

    // verify all samples have been copied correctly
    for (size_t channel_num = 0; channel_num < input_buffer.channels(); ++channel_num)
    {
        EXPECT_EQ(input_buffer[0][channel_num], output_buffer[0][channel_num]);
    }
}

TYPED_TEST(TransformInterleavedSingleFrame, MoreOutputFrames)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels(), input_buffer.frames() + 1);

    auto output_end = ratl::transform(input_buffer.begin(), input_buffer.end(), output_buffer.begin());
    EXPECT_EQ(output_end, output_buffer.end() - 1);

    // verify all samples have been copied correctly
    for (size_t channel_num = 0; channel_num < input_buffer.channels(); ++channel_num)
    {
        EXPECT_EQ(input_buffer[0][channel_num], output_buffer[0][channel_num]);
    }

    // verify extra output frame is still empty
    auto extra_frame = output_buffer.frame(1);
    for (auto sample_iter = extra_frame.begin(); sample_iter != extra_frame.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample{});
    }
}

TYPED_TEST(TransformInterleavedSingleFrame, MoreOutputChannels)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels() + 1, input_buffer.frames());

    auto output_end = ratl::transform(input_buffer.begin(), input_buffer.end(), output_buffer.begin());
    EXPECT_EQ(output_end, output_buffer.end());

    // verify all samples have been copied correctly
    for (size_t channel_num = 0; channel_num < input_buffer.channels(); ++channel_num)
    {
        EXPECT_EQ(input_buffer[0][channel_num], output_buffer[0][channel_num]);
    }

    // verify extra output channel is still empty
    EXPECT_EQ(output_buffer[0][input_buffer.channels()], typename TestFixture::output_sample{});
}

TYPED_TEST(TransformInterleavedSingleFrame, MoreOutputFramesMoreOutputChannels)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels() + 1, input_buffer.frames() + 1);

    auto output_end = ratl::transform(input_buffer.begin(), input_buffer.end(), output_buffer.begin());
    EXPECT_EQ(output_end, output_buffer.end() - 1);

    // verify all samples have been copied correctly
    for (size_t channel_num = 0; channel_num < input_buffer.channels(); ++channel_num)
    {
        EXPECT_EQ(input_buffer[0][channel_num], output_buffer[0][channel_num]);
    }

    // verify extra output frame is still empty
    auto extra_frame = output_buffer.frame(1);
    for (auto sample_iter = extra_frame.begin(); sample_iter != extra_frame.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample{});
    }

    // verify extra output channel is still empty
    EXPECT_EQ(output_buffer[0][input_buffer.channels()], typename TestFixture::output_sample{});
}

template<class SampleTypeCombination>
class TransformFrameSingleFrame : public TransformSingleFrameInterleavedBase<SampleTypeCombination>
{
};

TYPED_TEST_SUITE(TransformFrameSingleFrame, PossibleSampleTypeCombinations, );

TYPED_TEST(TransformFrameSingleFrame, Typical)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels(), input_buffer.frames());

    auto input_frame = input_buffer.frame(0);
    auto output_frame = output_buffer.frame(0);

    auto output_end = ratl::transform(input_frame.begin(), input_frame.end(), output_frame.begin());
    EXPECT_EQ(output_end, output_frame.end());

    // verify all channels have been copied correctly
    for (size_t channel_num = 0; channel_num < input_frame.channels(); ++channel_num)
    {
        EXPECT_EQ(input_frame[channel_num], output_frame[channel_num]);
    }
}

TYPED_TEST(TransformFrameSingleFrame, MoreOutputFrames)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels(), input_buffer.frames() + 1);

    auto input_frame = input_buffer.frame(0);
    auto output_frame = output_buffer.frame(0);

    auto output_end = ratl::transform(input_frame.begin(), input_frame.end(), output_frame.begin());
    EXPECT_EQ(output_end, output_frame.end());

    // verify all channels have been copied correctly
    for (size_t channel_num = 0; channel_num < input_frame.channels(); ++channel_num)
    {
        EXPECT_EQ(input_frame[channel_num], output_frame[channel_num]);
    }

    // verify extra output frame is still empty
    auto extra_frame = output_buffer.frame(1);
    for (auto sample_iter = extra_frame.begin(); sample_iter != extra_frame.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample{});
    }
}

TYPED_TEST(TransformFrameSingleFrame, MoreOutputChannels)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels() + 1, input_buffer.frames());

    auto input_frame = input_buffer.frame(0);
    auto output_frame = output_buffer.frame(0);

    auto output_end = ratl::transform(input_frame.begin(), input_frame.end(), output_frame.begin());
    EXPECT_EQ(output_end, output_frame.end() - 1);

    // verify all channels have been copied correctly
    for (size_t channel_num = 0; channel_num < input_frame.channels(); ++channel_num)
    {
        EXPECT_EQ(input_frame[channel_num], output_frame[channel_num]);
    }

    // verify all other output frames are still empty
    for (auto frame_iter = output_buffer.begin() + 1; frame_iter != output_buffer.end(); ++frame_iter)
    {
        for (auto sample_iter = frame_iter->begin(); sample_iter != frame_iter->end(); ++sample_iter)
        {
            EXPECT_EQ(*sample_iter, typename TestFixture::output_sample{});
        }
    }

    // verify extra output channel is still empty
    EXPECT_EQ(output_frame[input_frame.channels()], typename TestFixture::output_sample{});
}

TYPED_TEST(TransformFrameSingleFrame, MoreOutputFramesMoreOutputChannels)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels() + 1, input_buffer.frames() + 1);

    auto input_frame = input_buffer.frame(0);
    auto output_frame = output_buffer.frame(0);

    auto output_end = ratl::transform(input_frame.begin(), input_frame.end(), output_frame.begin());
    EXPECT_EQ(output_end, output_frame.end() - 1);

    // verify all channels have been copied correctly
    for (size_t channel_num = 0; channel_num < input_frame.channels(); ++channel_num)
    {
        EXPECT_EQ(input_frame[channel_num], output_frame[channel_num]);
    }

    // verify extra output frame is still empty
    auto extra_frame = output_buffer.frame(1);
    for (auto sample_iter = extra_frame.begin(); sample_iter != extra_frame.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample{});
    }

    // verify extra output channel is still empty
    EXPECT_EQ(output_frame[input_frame.channels()], typename TestFixture::output_sample{});
}

template<class SampleTypeCombination>
class TransformChannelSingleFrame : public TransformSingleFrameInterleavedBase<SampleTypeCombination>
{
};

TYPED_TEST_SUITE(TransformChannelSingleFrame, PossibleSampleTypeCombinations, );

TYPED_TEST(TransformChannelSingleFrame, Typical)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels(), input_buffer.frames());

    auto input_channel = input_buffer.channel(0);
    auto output_channel = output_buffer.channel(0);

    auto output_end = ratl::transform(input_channel.begin(), input_channel.end(), output_channel.begin());
    EXPECT_EQ(output_end, output_channel.end());

    // verify all samples have been copied correctly
    EXPECT_EQ(input_channel[0], output_channel[0]);

    // verify all other output channels are still empty
    for (auto channel_iter = output_channel.begin() + 1; channel_iter != output_channel.end(); ++channel_iter)
    {
        EXPECT_EQ(*channel_iter, typename TestFixture::output_sample{});
    }
}

TYPED_TEST(TransformChannelSingleFrame, MoreOutputFrames)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels(), input_buffer.frames() + 1);

    auto input_channel = input_buffer.channel(0);
    auto output_channel = output_buffer.channel(0);

    auto output_end = ratl::transform(input_channel.begin(), input_channel.end(), output_channel.begin());
    EXPECT_EQ(output_end, output_channel.end() - 1);

    // verify all samples have been copied correctly
    EXPECT_EQ(input_channel[0], output_channel[0]);

    // verify all other output channels are still empty
    for (auto channel_iter = output_channel.begin() + 1; channel_iter != output_channel.end(); ++channel_iter)
    {
        EXPECT_EQ(*channel_iter, typename TestFixture::output_sample{});
    }

    // verify extra output frame is still empty
    auto extra_frame = output_buffer.frame(1);
    for (auto sample_iter = extra_frame.begin(); sample_iter != extra_frame.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample{});
    }
}

TYPED_TEST(TransformChannelSingleFrame, MoreOutputChannels)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels() + 1, input_buffer.frames());

    auto input_channel = input_buffer.channel(0);
    auto output_channel = output_buffer.channel(0);

    auto output_end = ratl::transform(input_channel.begin(), input_channel.end(), output_channel.begin());
    EXPECT_EQ(output_end, output_channel.end());

    // verify all samples have been copied correctly
    EXPECT_EQ(input_channel[0], output_channel[0]);

    // verify all other output channels are still empty
    for (auto channel_iter = output_channel.begin() + 1; channel_iter != output_channel.end(); ++channel_iter)
    {
        EXPECT_EQ(*channel_iter, typename TestFixture::output_sample{});
    }
}

TYPED_TEST(TransformChannelSingleFrame, MoreOutputFramesMoreOutputChannels)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels() + 1, input_buffer.frames() + 1);

    auto input_channel = input_buffer.channel(0);
    auto output_channel = output_buffer.channel(0);

    auto output_end = ratl::transform(input_channel.begin(), input_channel.end(), output_channel.begin());
    EXPECT_EQ(output_end, output_channel.end() - 1);

    // verify all samples have been copied correctly
    EXPECT_EQ(input_channel[0], output_channel[0]);

    // verify all other output channels are still empty
    for (auto channel_iter = output_channel.begin() + 1; channel_iter != output_channel.end(); ++channel_iter)
    {
        EXPECT_EQ(*channel_iter, typename TestFixture::output_sample{});
    }

    // verify extra output frame is still empty
    auto extra_frame = output_buffer.frame(1);
    for (auto sample_iter = extra_frame.begin(); sample_iter != extra_frame.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample{});
    }
}

template<class SampleTypeCombination>
class TransformInterleavedSingleChannel : public TransformSingleChannelInterleavedBase<SampleTypeCombination>
{
};

TYPED_TEST_SUITE(TransformInterleavedSingleChannel, PossibleSampleTypeCombinations, );

TYPED_TEST(TransformInterleavedSingleChannel, Typical)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels(), input_buffer.frames());

    auto output_end = ratl::transform(input_buffer.begin(), input_buffer.end(), output_buffer.begin());
    EXPECT_EQ(output_end, output_buffer.end());

    // verify all samples have been copied correctly
    for (size_t frame_num = 0; frame_num < input_buffer.frames(); ++frame_num)
    {
        EXPECT_EQ(input_buffer[frame_num][0], output_buffer[frame_num][0]);
    }
}

TYPED_TEST(TransformInterleavedSingleChannel, MoreOutputFrames)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels(), input_buffer.frames() + 1);

    auto output_end = ratl::transform(input_buffer.begin(), input_buffer.end(), output_buffer.begin());
    EXPECT_EQ(output_end, output_buffer.end() - 1);

    // verify all samples have been copied correctly
    for (size_t frame_num = 0; frame_num < input_buffer.frames(); ++frame_num)
    {
        EXPECT_EQ(input_buffer[frame_num][0], output_buffer[frame_num][0]);
    }

    // verify extra output frame is still empty
    EXPECT_EQ(output_buffer[input_buffer.frames()][0], typename TestFixture::output_sample{});
}

TYPED_TEST(TransformInterleavedSingleChannel, MoreOutputChannels)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels() + 1, input_buffer.frames());

    auto output_end = ratl::transform(input_buffer.begin(), input_buffer.end(), output_buffer.begin());
    EXPECT_EQ(output_end, output_buffer.end());

    // verify all samples have been copied correctly
    for (size_t frame_num = 0; frame_num < input_buffer.frames(); ++frame_num)
    {
        EXPECT_EQ(input_buffer[frame_num][0], output_buffer[frame_num][0]);
    }

    // verify extra output channel is still empty
    auto extra_channel = output_buffer.channel(1);
    for (auto sample_iter = extra_channel.begin(); sample_iter != extra_channel.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample{});
    }
}

TYPED_TEST(TransformInterleavedSingleChannel, MoreOutputFramesMoreOutputChannels)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels() + 1, input_buffer.frames() + 1);

    auto output_end = ratl::transform(input_buffer.begin(), input_buffer.end(), output_buffer.begin());
    EXPECT_EQ(output_end, output_buffer.end() - 1);

    // verify all samples have been copied correctly
    for (size_t frame_num = 0; frame_num < input_buffer.frames(); ++frame_num)
    {
        EXPECT_EQ(input_buffer[frame_num][0], output_buffer[frame_num][0]);
    }

    // verify extra output frame is still empty
    EXPECT_EQ(output_buffer[input_buffer.frames()][0], typename TestFixture::output_sample{});

    // verify extra output channel is still empty
    auto extra_channel = output_buffer.channel(1);
    for (auto sample_iter = extra_channel.begin(); sample_iter != extra_channel.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample{});
    }
}

template<class SampleTypeCombination>
class TransformFrameSingleChannel : public TransformSingleChannelInterleavedBase<SampleTypeCombination>
{
};

TYPED_TEST_SUITE(TransformFrameSingleChannel, PossibleSampleTypeCombinations, );

TYPED_TEST(TransformFrameSingleChannel, Typical)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels(), input_buffer.frames());

    auto input_frame = input_buffer.frame(0);
    auto output_frame = output_buffer.frame(0);

    auto output_end = ratl::transform(input_frame.begin(), input_frame.end(), output_frame.begin());
    EXPECT_EQ(output_end, output_frame.end());

    // verify Sample have been copied correctly
    EXPECT_EQ(input_frame[0], output_frame[0]);

    // verify all other output frames are still empty
    for (auto frame_iter = output_buffer.begin() + 1; frame_iter != output_buffer.end(); ++frame_iter)
    {
        EXPECT_EQ((*frame_iter)[0], typename TestFixture::output_sample{});
    }
}

TYPED_TEST(TransformFrameSingleChannel, MoreOutputFrames)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels(), input_buffer.frames() + 1);

    auto input_frame = input_buffer.frame(0);
    auto output_frame = output_buffer.frame(0);

    auto output_end = ratl::transform(input_frame.begin(), input_frame.end(), output_frame.begin());
    EXPECT_EQ(output_end, output_frame.end());

    // verify Sample have been copied correctly
    EXPECT_EQ(input_frame[0], output_frame[0]);

    // verify all other output frames are still empty
    for (auto frame_iter = output_buffer.begin() + 1; frame_iter != output_buffer.end(); ++frame_iter)
    {
        EXPECT_EQ((*frame_iter)[0], typename TestFixture::output_sample{});
    }
}

TYPED_TEST(TransformFrameSingleChannel, MoreOutputChannels)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels() + 1, input_buffer.frames());

    auto input_frame = input_buffer.frame(0);
    auto output_frame = output_buffer.frame(0);

    auto output_end = ratl::transform(input_frame.begin(), input_frame.end(), output_frame.begin());
    EXPECT_EQ(output_end, output_frame.end() - 1);

    // verify Sample have been copied correctly
    EXPECT_EQ(input_frame[0], output_frame[0]);

    // verify all other output frames are still empty
    for (auto frame_iter = output_buffer.begin() + 1; frame_iter != output_buffer.end(); ++frame_iter)
    {
        EXPECT_EQ((*frame_iter)[0], typename TestFixture::output_sample{});
    }

    // verify extra output channel is still empty
    auto extra_channel = output_buffer.channel(1);
    for (auto sample_iter = extra_channel.begin(); sample_iter != extra_channel.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample{});
    }
}

TYPED_TEST(TransformFrameSingleChannel, MoreOutputFramesMoreOutputChannels)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels() + 1, input_buffer.frames() + 1);

    auto input_frame = input_buffer.frame(0);
    auto output_frame = output_buffer.frame(0);

    auto output_end = ratl::transform(input_frame.begin(), input_frame.end(), output_frame.begin());
    EXPECT_EQ(output_end, output_frame.end() - 1);

    // verify Sample have been copied correctly
    EXPECT_EQ(input_frame[0], output_frame[0]);

    // verify all other output frames are still empty
    for (auto frame_iter = output_buffer.begin() + 1; frame_iter != output_buffer.end(); ++frame_iter)
    {
        EXPECT_EQ((*frame_iter)[0], typename TestFixture::output_sample{});
    }

    // verify extra output channel is still empty
    auto extra_channel = output_buffer.channel(1);
    for (auto sample_iter = extra_channel.begin(); sample_iter != extra_channel.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample{});
    }
}

template<class SampleTypeCombination>
class TransformChannelSingleChannel : public TransformSingleChannelInterleavedBase<SampleTypeCombination>
{
};

TYPED_TEST_SUITE(TransformChannelSingleChannel, PossibleSampleTypeCombinations, );

TYPED_TEST(TransformChannelSingleChannel, Typical)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels(), input_buffer.frames());

    auto input_channel = input_buffer.channel(0);
    auto output_channel = output_buffer.channel(0);

    auto output_end = ratl::transform(input_channel.begin(), input_channel.end(), output_channel.begin());
    EXPECT_EQ(output_end, output_channel.end());

    // verify all samples have been copied correctly
    for (size_t sample_num = 0; sample_num < input_channel.samples(); ++sample_num)
    {
        EXPECT_EQ(input_channel[sample_num], output_channel[sample_num]);
    }
}

TYPED_TEST(TransformChannelSingleChannel, MoreOutputFrames)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels(), input_buffer.frames() + 1);

    auto input_channel = input_buffer.channel(0);
    auto output_channel = output_buffer.channel(0);

    auto output_end = ratl::transform(input_channel.begin(), input_channel.end(), output_channel.begin());
    EXPECT_EQ(output_end, output_channel.end() - 1);

    // verify all samples have been copied correctly
    for (size_t sample_num = 0; sample_num < input_channel.samples(); ++sample_num)
    {
        EXPECT_EQ(input_channel[sample_num], output_channel[sample_num]);
    }

    // verify extra output frame is still empty
    EXPECT_EQ(output_channel[input_channel.samples()], typename TestFixture::output_sample{});
}

TYPED_TEST(TransformChannelSingleChannel, MoreOutputChannels)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels() + 1, input_buffer.frames());

    auto input_channel = input_buffer.channel(0);
    auto output_channel = output_buffer.channel(0);

    auto output_end = ratl::transform(input_channel.begin(), input_channel.end(), output_channel.begin());
    EXPECT_EQ(output_end, output_channel.end());

    // verify all samples have been copied correctly
    for (size_t sample_num = 0; sample_num < input_channel.samples(); ++sample_num)
    {
        EXPECT_EQ(input_channel[sample_num], output_channel[sample_num]);
    }

    // verify extra output channel is still empty
    auto extra_channel = output_buffer.channel(1);
    for (auto sample_iter = extra_channel.begin(); sample_iter != extra_channel.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample{});
    }
}

TYPED_TEST(TransformChannelSingleChannel, MoreOutputFramesMoreOutputChannels)
{
    const auto& input_buffer = TestFixture::buffer_;
    auto output_buffer = TestFixture::generate_output(input_buffer.channels() + 1, input_buffer.frames() + 1);

    auto input_channel = input_buffer.channel(0);
    auto output_channel = output_buffer.channel(0);

    auto output_end = ratl::transform(input_channel.begin(), input_channel.end(), output_channel.begin());
    EXPECT_EQ(output_end, output_channel.end() - 1);

    // verify all samples have been copied correctly
    for (size_t sample_num = 0; sample_num < input_channel.samples(); ++sample_num)
    {
        EXPECT_EQ(input_channel[sample_num], output_channel[sample_num]);
    }

    // verify extra output frame is still empty
    EXPECT_EQ(output_channel[input_channel.samples()], typename TestFixture::output_sample{});

    // verify extra output channel is still empty
    auto extra_channel = output_buffer.channel(1);
    for (auto sample_iter = extra_channel.begin(); sample_iter != extra_channel.end(); ++sample_iter)
    {
        EXPECT_EQ(*sample_iter, typename TestFixture::output_sample{});
    }
}

} // namespace ratl
