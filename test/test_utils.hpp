
#include <gtest/gtest.h>
#include <ratl/ratl.hpp>

#define TEST_RATL_TYPICAL_CHANNELS 32
#define TEST_RATL_TYPICAL_FRAMES 480

namespace ratl
{
typedef ::testing::Types<int16_t, int24_t, int32_t, float32_t> PossibleSampleTypes;

template<class InputSampleTypeArg, class OutputSampleTypeArg>
struct SampleTypeCombination
{
    using InputSampleType = InputSampleTypeArg;
    using OutputSampleType = OutputSampleTypeArg;
};
typedef ::testing::Types<
    SampleTypeCombination<int16_t, int16_t>,
    SampleTypeCombination<int16_t, int24_t>,
    SampleTypeCombination<int16_t, int32_t>,
    SampleTypeCombination<int16_t, float32_t>,
    SampleTypeCombination<int24_t, int16_t>,
    SampleTypeCombination<int24_t, int24_t>,
    SampleTypeCombination<int24_t, int32_t>,
    SampleTypeCombination<int24_t, float32_t>,
    SampleTypeCombination<int32_t, int16_t>,
    SampleTypeCombination<int32_t, int24_t>,
    SampleTypeCombination<int32_t, int32_t>,
    SampleTypeCombination<int32_t, float32_t>,
    SampleTypeCombination<float32_t, int16_t>,
    SampleTypeCombination<float32_t, int24_t>,
    SampleTypeCombination<float32_t, int32_t>,
    SampleTypeCombination<float32_t, float32_t>>
    PossibleSampleTypeCombinations;
typedef ::testing::Types<
    SampleTypeCombination<int16_t, int16_t>,
    SampleTypeCombination<int16_t, int24_t>,
    SampleTypeCombination<int16_t, int32_t>,
    SampleTypeCombination<int16_t, float32_t>,
    SampleTypeCombination<int24_t, int16_t>,
    SampleTypeCombination<int24_t, int24_t>,
    SampleTypeCombination<int24_t, int32_t>,
    SampleTypeCombination<int24_t, float32_t>,
    SampleTypeCombination<int32_t, int16_t>,
    SampleTypeCombination<int32_t, int24_t>,
    SampleTypeCombination<int32_t, int32_t>,
    SampleTypeCombination<int32_t, float32_t>>
    PossibleIntInputSampleTypeCombinations;

template<template<class> class BufferType, class SampleType, size_t Channels, size_t Frames>
class InterleavedBase : public ::testing::Test
{
protected:
    using sample_type = SampleType;
    using buffer = BufferType<sample_type>;
    using sample = typename buffer::sample;

    InterleavedBase() : buffer_{generate()} {}

    static constexpr size_t channels()
    {
        return Channels;
    }

    static constexpr size_t frames()
    {
        return Frames;
    }

    static constexpr size_t samples()
    {
        return Channels * Frames;
    }

    static buffer generate_empty()
    {
        return buffer{};
    }

    static buffer generate()
    {
        return buffer{Channels, Frames};
    }

    buffer buffer_;
};

template<template<class> class BufferType, class SampleType>
class TypicalBase : public InterleavedBase<BufferType, SampleType, TEST_RATL_TYPICAL_CHANNELS, TEST_RATL_TYPICAL_FRAMES>
{
};

template<template<class> class BufferType, class SampleType>
class ZeroBase : public InterleavedBase<BufferType, SampleType, 0, 0>
{
};

template<template<class> class BufferType, class SampleType>
class ZeroFramesBase : public InterleavedBase<BufferType, SampleType, TEST_RATL_TYPICAL_CHANNELS, 0>
{
};

template<template<class> class BufferType, class SampleType>
class SingleFrameBase : public InterleavedBase<BufferType, SampleType, TEST_RATL_TYPICAL_CHANNELS, 1>
{
};

template<template<class> class BufferType, class SampleType>
class ZeroChannelsBase : public InterleavedBase<BufferType, SampleType, 0, TEST_RATL_TYPICAL_FRAMES>
{
};

template<template<class> class BufferType, class SampleType>
class SingleChannelBase : public InterleavedBase<BufferType, SampleType, 1, TEST_RATL_TYPICAL_FRAMES>
{
};

template<template<class> class BufferType, class SampleType>
class CopyBase : public TypicalBase<BufferType, SampleType>
{
protected:
    virtual ~CopyBase()
    {
        EXPECT_EQ(this->buffer_, this->generate());
    }
};

template<template<class> class BufferType, class SampleType>
class MoveBase : public TypicalBase<BufferType, SampleType>
{
protected:
    virtual ~MoveBase()
    {
        EXPECT_EQ(this->buffer_, this->generate_empty());
    }
};

template<class SampleType>
using TypicalInterleavedBase = TypicalBase<ratl::Interleaved, SampleType>;

template<class SampleType>
using ZeroInterleavedBase = ZeroBase<ratl::Interleaved, SampleType>;

template<class SampleType>
using ZeroFramesInterleavedBase = ZeroFramesBase<ratl::Interleaved, SampleType>;

template<class SampleType>
using SingleFrameInterleavedBase = SingleFrameBase<ratl::Interleaved, SampleType>;

template<class SampleType>
using ZeroChannelsInterleavedBase = ZeroChannelsBase<ratl::Interleaved, SampleType>;

template<class SampleType>
using SingleChannelInterleavedBase = SingleChannelBase<ratl::Interleaved, SampleType>;

template<class SampleType>
using CopyInterleavedBase = CopyBase<ratl::Interleaved, SampleType>;

template<class SampleType>
using MoveInterleavedBase = MoveBase<ratl::Interleaved, SampleType>;

template<class SampleType>
using TypicalNoninterleavedBase = TypicalBase<ratl::Noninterleaved, SampleType>;

template<class SampleType>
using ZeroNoninterleavedBase = ZeroBase<ratl::Noninterleaved, SampleType>;

template<class SampleType>
using ZeroFramesNoninterleavedBase = ZeroFramesBase<ratl::Noninterleaved, SampleType>;

template<class SampleType>
using SingleFrameNoninterleavedBase = SingleFrameBase<ratl::Noninterleaved, SampleType>;

template<class SampleType>
using ZeroChannelsNoninterleavedBase = ZeroChannelsBase<ratl::Noninterleaved, SampleType>;

template<class SampleType>
using SingleChannelNoninterleavedBase = SingleChannelBase<ratl::Noninterleaved, SampleType>;

template<class SampleType>
using CopyNoninterleavedBase = CopyBase<ratl::Noninterleaved, SampleType>;

template<class SampleType>
using MoveNoninterleavedBase = MoveBase<ratl::Noninterleaved, SampleType>;

} // namespace ratl