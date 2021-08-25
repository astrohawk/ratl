
#include <gtest/gtest.h>
#include <ratl/ratl.hpp>

#define TEST_RATL_TYPICAL_CHANNELS 32
#define TEST_RATL_TYPICAL_FRAMES 480

namespace ratl
{
namespace test
{
using PossibleSampleTypes = ::testing::Types<int16_t, int24_t, int32_t, float32_t>;

using PossibleIntSampleTypes = ::testing::Types<int16_t, int24_t, int32_t>;

template<class InputSampleType, class OutputSampleType>
struct SampleTypeCombination
{
    using input_sample_type = InputSampleType;
    using output_sample_type = OutputSampleType;
};

using PossibleSampleTypeCombinations = ::testing::Types<
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
    SampleTypeCombination<float32_t, float32_t>>;

using PossibleIntInputSampleTypeCombinations = ::testing::Types<
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
    SampleTypeCombination<int32_t, float32_t>>;

template<template<class> class ContainerType, class SampleValueType, size_t Channels, size_t Frames>
class TestBase : public ::testing::Test
{
protected:
    using container_type = ContainerType<SampleValueType>;
    using sample_type = typename container_type::sample_type;

    TestBase() : container_{generate()} {}

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

    static container_type generate_empty()
    {
        return container_type{};
    }

    static container_type generate()
    {
        return container_type{Channels, Frames};
    }

    container_type container_;
};

template<template<class> class BufferType, class SampleValueType>
class TypicalBase : public TestBase<BufferType, SampleValueType, TEST_RATL_TYPICAL_CHANNELS, TEST_RATL_TYPICAL_FRAMES>
{
};

template<template<class> class BufferType, class SampleValueType>
class ZeroBase : public TestBase<BufferType, SampleValueType, 0, 0>
{
};

template<template<class> class BufferType, class SampleValueType>
class ZeroFramesBase : public TestBase<BufferType, SampleValueType, TEST_RATL_TYPICAL_CHANNELS, 0>
{
};

template<template<class> class BufferType, class SampleValueType>
class SingleFrameBase : public TestBase<BufferType, SampleValueType, TEST_RATL_TYPICAL_CHANNELS, 1>
{
};

template<template<class> class BufferType, class SampleValueType>
class ZeroChannelsBase : public TestBase<BufferType, SampleValueType, 0, TEST_RATL_TYPICAL_FRAMES>
{
};

template<template<class> class BufferType, class SampleValueType>
class SingleChannelBase : public TestBase<BufferType, SampleValueType, 1, TEST_RATL_TYPICAL_FRAMES>
{
};

template<template<class> class BufferType, class SampleValueType>
class CopyBase : public TypicalBase<BufferType, SampleValueType>
{
protected:
    virtual ~CopyBase()
    {
        EXPECT_EQ(this->container_, this->generate());
    }
};

template<template<class> class BufferType, class SampleValueType>
class MoveBase : public TypicalBase<BufferType, SampleValueType>
{
protected:
    virtual ~MoveBase()
    {
        EXPECT_EQ(this->container_, this->generate_empty());
    }
};

template<class SampleValueType>
using TypicalInterleavedBase = TypicalBase<ratl::interleaved, SampleValueType>;

template<class SampleValueType>
using ZeroInterleavedBase = ZeroBase<ratl::interleaved, SampleValueType>;

template<class SampleValueType>
using ZeroFramesInterleavedBase = ZeroFramesBase<ratl::interleaved, SampleValueType>;

template<class SampleValueType>
using SingleFrameInterleavedBase = SingleFrameBase<ratl::interleaved, SampleValueType>;

template<class SampleValueType>
using ZeroChannelsInterleavedBase = ZeroChannelsBase<ratl::interleaved, SampleValueType>;

template<class SampleValueType>
using SingleChannelInterleavedBase = SingleChannelBase<ratl::interleaved, SampleValueType>;

template<class SampleValueType>
using CopyInterleavedBase = CopyBase<ratl::interleaved, SampleValueType>;

template<class SampleValueType>
using MoveInterleavedBase = MoveBase<ratl::interleaved, SampleValueType>;

template<class SampleValueType>
using TypicalNoninterleavedBase = TypicalBase<ratl::noninterleaved, SampleValueType>;

template<class SampleValueType>
using ZeroNoninterleavedBase = ZeroBase<ratl::noninterleaved, SampleValueType>;

template<class SampleValueType>
using ZeroFramesNoninterleavedBase = ZeroFramesBase<ratl::noninterleaved, SampleValueType>;

template<class SampleValueType>
using SingleFrameNoninterleavedBase = SingleFrameBase<ratl::noninterleaved, SampleValueType>;

template<class SampleValueType>
using ZeroChannelsNoninterleavedBase = ZeroChannelsBase<ratl::noninterleaved, SampleValueType>;

template<class SampleValueType>
using SingleChannelNoninterleavedBase = SingleChannelBase<ratl::noninterleaved, SampleValueType>;

template<class SampleValueType>
using CopyNoninterleavedBase = CopyBase<ratl::noninterleaved, SampleValueType>;

template<class SampleValueType>
using MoveNoninterleavedBase = MoveBase<ratl::noninterleaved, SampleValueType>;

} // namespace test
} // namespace ratl
