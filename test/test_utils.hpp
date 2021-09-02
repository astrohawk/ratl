/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_test_utils_
#define _ratl_test_utils_

// ratl includes
#include <ratl/ratl.hpp>

// other includes
#include <gtest/gtest.h>

#define TEST_RATL_TYPICAL_CHANNELS 32
#define TEST_RATL_TYPICAL_FRAMES 480

namespace ratl
{
namespace test
{
using PossibleSampleValueTypes = ::testing::Types<int16_t, int24_t, int32_t, float32_t>;

using PossibleIntSampleValueTypes = ::testing::Types<int16_t, int24_t, int32_t>;

template<typename InputSampleValueType, typename OutputSampleValueType>
struct SampleValueTypeCombination
{
    using input_sample_value_type = InputSampleValueType;
    using output_sample_value_type = OutputSampleValueType;
};

using PossibleSampleValueTypeCombinations = ::testing::Types<
    SampleValueTypeCombination<int16_t, int16_t>,
    SampleValueTypeCombination<int16_t, int24_t>,
    SampleValueTypeCombination<int16_t, int32_t>,
    SampleValueTypeCombination<int16_t, float32_t>,
    SampleValueTypeCombination<int24_t, int16_t>,
    SampleValueTypeCombination<int24_t, int24_t>,
    SampleValueTypeCombination<int24_t, int32_t>,
    SampleValueTypeCombination<int24_t, float32_t>,
    SampleValueTypeCombination<int32_t, int16_t>,
    SampleValueTypeCombination<int32_t, int24_t>,
    SampleValueTypeCombination<int32_t, int32_t>,
    SampleValueTypeCombination<int32_t, float32_t>,
    SampleValueTypeCombination<float32_t, int16_t>,
    SampleValueTypeCombination<float32_t, int24_t>,
    SampleValueTypeCombination<float32_t, int32_t>,
    SampleValueTypeCombination<float32_t, float32_t>>;

using PossibleIntInputSampleValueTypeCombinations = ::testing::Types<
    /*SampleValueTypeCombination<int16_t, int16_t>,
    SampleValueTypeCombination<int16_t, int24_t>,
    SampleValueTypeCombination<int16_t, int32_t>,
    SampleValueTypeCombination<int16_t, float32_t>,*/
    SampleValueTypeCombination<int24_t, int16_t>/*,
    SampleValueTypeCombination<int24_t, int24_t>,
    SampleValueTypeCombination<int24_t, int32_t>,
    SampleValueTypeCombination<int24_t, float32_t>,
    SampleValueTypeCombination<int32_t, int16_t>,
    SampleValueTypeCombination<int32_t, int24_t>,
    SampleValueTypeCombination<int32_t, int32_t>,
    SampleValueTypeCombination<int32_t, float32_t>*/>;

template<template<typename> class ContainerType, typename SampleValueType, size_t Channels, size_t Frames>
class TestBase : public ::testing::Test
{
protected:
    using container_type = ContainerType<SampleValueType>;
    using sample_type = typename container_type::sample_type;

    TestBase() : container_(generate()) {}

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
        return container_type();
    }

    static container_type generate()
    {
        return container_type(Channels, Frames);
    }

    container_type container_;
};

template<template<typename> class BufferType, typename SampleValueType>
class TypicalBase : public TestBase<BufferType, SampleValueType, TEST_RATL_TYPICAL_CHANNELS, TEST_RATL_TYPICAL_FRAMES>
{
};

template<template<typename> class BufferType, typename SampleValueType>
class ZeroBase : public TestBase<BufferType, SampleValueType, 0, 0>
{
};

template<template<typename> class BufferType, typename SampleValueType>
class ZeroFramesBase : public TestBase<BufferType, SampleValueType, TEST_RATL_TYPICAL_CHANNELS, 0>
{
};

template<template<typename> class BufferType, typename SampleValueType>
class SingleFrameBase : public TestBase<BufferType, SampleValueType, TEST_RATL_TYPICAL_CHANNELS, 1>
{
};

template<template<typename> class BufferType, typename SampleValueType>
class ZeroChannelsBase : public TestBase<BufferType, SampleValueType, 0, TEST_RATL_TYPICAL_FRAMES>
{
};

template<template<typename> class BufferType, typename SampleValueType>
class SingleChannelBase : public TestBase<BufferType, SampleValueType, 1, TEST_RATL_TYPICAL_FRAMES>
{
};

template<template<typename> class BufferType, typename SampleValueType>
class CopyBase : public TypicalBase<BufferType, SampleValueType>
{
protected:
    virtual ~CopyBase()
    {
        EXPECT_EQ(this->container_, this->generate());
    }
};

template<template<typename> class BufferType, typename SampleValueType>
class MoveBase : public TypicalBase<BufferType, SampleValueType>
{
protected:
    virtual ~MoveBase()
    {
        EXPECT_EQ(this->container_, this->generate_empty());
    }
};

template<typename SampleValueType>
using TypicalInterleavedBase = TypicalBase<ratl::interleaved, SampleValueType>;

template<typename SampleValueType>
using ZeroInterleavedBase = ZeroBase<ratl::interleaved, SampleValueType>;

template<typename SampleValueType>
using ZeroFramesInterleavedBase = ZeroFramesBase<ratl::interleaved, SampleValueType>;

template<typename SampleValueType>
using SingleFrameInterleavedBase = SingleFrameBase<ratl::interleaved, SampleValueType>;

template<typename SampleValueType>
using ZeroChannelsInterleavedBase = ZeroChannelsBase<ratl::interleaved, SampleValueType>;

template<typename SampleValueType>
using SingleChannelInterleavedBase = SingleChannelBase<ratl::interleaved, SampleValueType>;

template<typename SampleValueType>
using CopyInterleavedBase = CopyBase<ratl::interleaved, SampleValueType>;

template<typename SampleValueType>
using MoveInterleavedBase = MoveBase<ratl::interleaved, SampleValueType>;

template<typename SampleValueType>
using TypicalNoninterleavedBase = TypicalBase<ratl::noninterleaved, SampleValueType>;

template<typename SampleValueType>
using ZeroNoninterleavedBase = ZeroBase<ratl::noninterleaved, SampleValueType>;

template<typename SampleValueType>
using ZeroFramesNoninterleavedBase = ZeroFramesBase<ratl::noninterleaved, SampleValueType>;

template<typename SampleValueType>
using SingleFrameNoninterleavedBase = SingleFrameBase<ratl::noninterleaved, SampleValueType>;

template<typename SampleValueType>
using ZeroChannelsNoninterleavedBase = ZeroChannelsBase<ratl::noninterleaved, SampleValueType>;

template<typename SampleValueType>
using SingleChannelNoninterleavedBase = SingleChannelBase<ratl::noninterleaved, SampleValueType>;

template<typename SampleValueType>
using CopyNoninterleavedBase = CopyBase<ratl::noninterleaved, SampleValueType>;

template<typename SampleValueType>
using MoveNoninterleavedBase = MoveBase<ratl::noninterleaved, SampleValueType>;

} // namespace test
} // namespace ratl

#endif
