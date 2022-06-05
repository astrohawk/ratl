/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_test_utils_
#define _ratl_test_utils_

// ratl includes
#include "test_predicates.hpp"
#include "test_type_utils.hpp"

#include <ratl/ratl.hpp>

// other includes
#include <gtest/gtest.h>

#define TEST_RATL_TYPICAL_CHANNELS 31
#define TEST_RATL_TYPICAL_FRAMES 479

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

using PossibleSampleValueTypeCombinations =
    testing_types_combine_t<SampleValueTypeCombination, PossibleSampleValueTypes, PossibleSampleValueTypes>;

using PossibleIntInputSampleValueTypeCombinations =
    testing_types_combine_t<SampleValueTypeCombination, PossibleIntSampleValueTypes, PossibleSampleValueTypes>;

template<typename ContainerType>
class BufferWrapper
{
public:
    using container_type = ContainerType;
    using storage_type = ContainerType;

private:
    using size_type = typename container_type::size_type;

    container_type container_;

public:
    BufferWrapper() : container_() {}

    BufferWrapper(size_type channels, size_type frames) : container_(channels, frames) {}

    ~BufferWrapper() = default;

    BufferWrapper(const BufferWrapper&) = delete;
    BufferWrapper& operator=(const BufferWrapper&) = delete;
    BufferWrapper(BufferWrapper&&) noexcept = default;
    BufferWrapper& operator=(BufferWrapper&&) noexcept = default;

    container_type& container()
    {
        return container_;
    }

    storage_type& storage()
    {
        return container_;
    }
};

template<typename ContainerType>
class SpanWrapper
{
public:
    using container_type = ContainerType;

private:
    using base_sample_type = std::remove_cv_t<typename container_type::sample_type>;
    using size_type = typename container_type::size_type;

public:
    using storage_type = std::vector<base_sample_type>;

private:
    storage_type storage_;
    container_type container_;

public:
    SpanWrapper() = default;

    SpanWrapper(size_type channels, size_type frames) :
        storage_(channels * frames), container_(storage_.data(), channels, frames)
    {
    }

    ~SpanWrapper() = default;

    SpanWrapper(const SpanWrapper&) = delete;
    SpanWrapper& operator=(const SpanWrapper&) = delete;
    SpanWrapper(SpanWrapper&&) noexcept = default;
    SpanWrapper& operator=(SpanWrapper&&) noexcept = default;

    container_type& container()
    {
        return container_;
    }

    storage_type& storage()
    {
        return storage_;
    }
};

template<typename SampleValueType>
using PossibleInterleavedContainers = ::testing::Types<
    BufferWrapper<interleaved<SampleValueType>>,
    BufferWrapper<network_interleaved<SampleValueType>>,
    SpanWrapper<interleaved_span<SampleValueType>>,
    SpanWrapper<const_interleaved_span<SampleValueType>>,
    SpanWrapper<network_interleaved_span<SampleValueType>>,
    SpanWrapper<const_network_interleaved_span<SampleValueType>>>;
using PossibleInterleaved = testing_types_combine_t<PossibleInterleavedContainers, PossibleSampleValueTypes>;

template<typename SampleValueType>
using PossibleInterleavedBufferContainers =
    ::testing::Types<BufferWrapper<interleaved<SampleValueType>>, BufferWrapper<network_interleaved<SampleValueType>>>;
using PossibleInterleavedBuffers =
    testing_types_combine_t<PossibleInterleavedBufferContainers, PossibleSampleValueTypes>;

template<typename SampleValueType>
using PossibleInterleavedSpanContainers = ::testing::Types<
    SpanWrapper<interleaved_span<SampleValueType>>,
    SpanWrapper<const_interleaved_span<SampleValueType>>,
    SpanWrapper<network_interleaved_span<SampleValueType>>,
    SpanWrapper<const_network_interleaved_span<SampleValueType>>>;
using PossibleInterleavedSpans = testing_types_combine_t<PossibleInterleavedSpanContainers, PossibleSampleValueTypes>;

template<typename SampleValueType>
using PossibleNonConstInterleavedContainers = ::testing::Types<
    BufferWrapper<interleaved<SampleValueType>>,
    BufferWrapper<network_interleaved<SampleValueType>>,
    SpanWrapper<interleaved_span<SampleValueType>>,
    SpanWrapper<network_interleaved_span<SampleValueType>>>;
using PossibleNonConstInterleaved =
    testing_types_combine_t<PossibleNonConstInterleavedContainers, PossibleSampleValueTypes>;

template<typename SampleValueType>
using PossibleHostInterleavedContainers = ::testing::Types<
    BufferWrapper<interleaved<SampleValueType>>,
    SpanWrapper<interleaved_span<SampleValueType>>,
    SpanWrapper<const_interleaved_span<SampleValueType>>>;
using PossibleHostInterleaved = testing_types_combine_t<PossibleHostInterleavedContainers, PossibleSampleValueTypes>;

template<typename SampleValueType>
using PossibleHostInterleavedBufferContainers = BufferWrapper<interleaved<SampleValueType>>;
using PossibleHostInterleavedBuffers =
    testing_types_combine_t<PossibleHostInterleavedBufferContainers, PossibleSampleValueTypes>;

template<typename SampleValueType>
using PossibleHostInterleavedSpanContainers = ::testing::
    Types<SpanWrapper<interleaved_span<SampleValueType>>, SpanWrapper<const_interleaved_span<SampleValueType>>>;
using PossibleHostInterleavedSpans =
    testing_types_combine_t<PossibleHostInterleavedSpanContainers, PossibleSampleValueTypes>;

template<typename SampleValueType>
using PossibleNonConstHostInterleavedContainers =
    ::testing::Types<BufferWrapper<interleaved<SampleValueType>>, SpanWrapper<interleaved_span<SampleValueType>>>;
using PossibleNonConstHostInterleaved =
    testing_types_combine_t<PossibleNonConstHostInterleavedContainers, PossibleSampleValueTypes>;

template<typename SampleValueType>
using PossibleNetworkInterleavedContainers = ::testing::Types<
    BufferWrapper<network_interleaved<SampleValueType>>,
    SpanWrapper<network_interleaved_span<SampleValueType>>,
    SpanWrapper<const_network_interleaved_span<SampleValueType>>>;
using PossibleNetworkInterleaved =
    testing_types_combine_t<PossibleNetworkInterleavedContainers, PossibleSampleValueTypes>;

template<typename SampleValueType>
using PossibleNetworkInterleavedBufferContainers = BufferWrapper<network_interleaved<SampleValueType>>;
using PossibleNetworkInterleavedBuffers =
    testing_types_combine_t<PossibleNetworkInterleavedBufferContainers, PossibleSampleValueTypes>;

template<typename SampleValueType>
using PossibleNetworkInterleavedSpanContainers = ::testing::Types<
    SpanWrapper<network_interleaved_span<SampleValueType>>,
    SpanWrapper<const_network_interleaved_span<SampleValueType>>>;
using PossibleNetworkInterleavedSpans =
    testing_types_combine_t<PossibleNetworkInterleavedSpanContainers, PossibleSampleValueTypes>;

template<typename SampleValueType>
using PossibleNonConstNetworkInterleavedContainers = ::testing::
    Types<BufferWrapper<network_interleaved<SampleValueType>>, SpanWrapper<network_interleaved_span<SampleValueType>>>;
using PossibleNonConstNetworkInterleaved =
    testing_types_combine_t<PossibleNonConstNetworkInterleavedContainers, PossibleSampleValueTypes>;

template<typename SampleValueType>
using PossibleNoninterleavedContainers = ::testing::Types<
    BufferWrapper<noninterleaved<SampleValueType>>,
    BufferWrapper<network_noninterleaved<SampleValueType>>,
    SpanWrapper<noninterleaved_span<SampleValueType>>,
    SpanWrapper<const_noninterleaved_span<SampleValueType>>,
    SpanWrapper<network_noninterleaved_span<SampleValueType>>,
    SpanWrapper<const_network_noninterleaved_span<SampleValueType>>>;
using PossibleNoninterleaved = testing_types_combine_t<PossibleNoninterleavedContainers, PossibleSampleValueTypes>;

template<typename SampleValueType>
using PossibleNoninterleavedBufferContainers = ::testing::
    Types<BufferWrapper<noninterleaved<SampleValueType>>, BufferWrapper<network_noninterleaved<SampleValueType>>>;
using PossibleNoninterleavedBuffers =
    testing_types_combine_t<PossibleNoninterleavedBufferContainers, PossibleSampleValueTypes>;

template<typename SampleValueType>
using PossibleNoninterleavedSpanContainers = ::testing::Types<
    SpanWrapper<noninterleaved_span<SampleValueType>>,
    SpanWrapper<const_noninterleaved_span<SampleValueType>>,
    SpanWrapper<network_noninterleaved_span<SampleValueType>>,
    SpanWrapper<const_network_noninterleaved_span<SampleValueType>>>;
using PossibleNoninterleavedSpans =
    testing_types_combine_t<PossibleNoninterleavedSpanContainers, PossibleSampleValueTypes>;

template<typename SampleValueType>
using PossibleNonConstNoninterleavedContainers = ::testing::Types<
    BufferWrapper<noninterleaved<SampleValueType>>,
    BufferWrapper<network_noninterleaved<SampleValueType>>,
    SpanWrapper<noninterleaved_span<SampleValueType>>,
    SpanWrapper<network_noninterleaved_span<SampleValueType>>>;
using PossibleNonConstNoninterleaved =
    testing_types_combine_t<PossibleNonConstNoninterleavedContainers, PossibleSampleValueTypes>;

template<typename SampleValueType>
using PossibleHostNoninterleavedContainers = ::testing::Types<
    BufferWrapper<noninterleaved<SampleValueType>>,
    SpanWrapper<noninterleaved_span<SampleValueType>>,
    SpanWrapper<const_noninterleaved_span<SampleValueType>>>;
using PossibleHostNoninterleaved =
    testing_types_combine_t<PossibleHostNoninterleavedContainers, PossibleSampleValueTypes>;

template<typename SampleValueType>
using PossibleHostNoninterleavedBufferContainers = BufferWrapper<noninterleaved<SampleValueType>>;
using PossibleHostNoninterleavedBuffers =
    testing_types_combine_t<PossibleHostNoninterleavedBufferContainers, PossibleSampleValueTypes>;

template<typename SampleValueType>
using PossibleHostNoninterleavedSpanContainers = ::testing::
    Types<SpanWrapper<noninterleaved_span<SampleValueType>>, SpanWrapper<const_noninterleaved_span<SampleValueType>>>;
using PossibleHostNoninterleavedSpans =
    testing_types_combine_t<PossibleHostNoninterleavedSpanContainers, PossibleSampleValueTypes>;

template<typename SampleValueType>
using PossibleNonConstHostNoninterleavedContainers =
    ::testing::Types<BufferWrapper<noninterleaved<SampleValueType>>, SpanWrapper<noninterleaved_span<SampleValueType>>>;
using PossibleNonConstHostNoninterleaved =
    testing_types_combine_t<PossibleNonConstHostNoninterleavedContainers, PossibleSampleValueTypes>;

template<typename SampleValueType>
using PossibleNetworkNoninterleavedContainers = ::testing::Types<
    BufferWrapper<network_noninterleaved<SampleValueType>>,
    SpanWrapper<network_noninterleaved_span<SampleValueType>>,
    SpanWrapper<const_network_noninterleaved_span<SampleValueType>>>;
using PossibleNetworkNoninterleaved =
    testing_types_combine_t<PossibleNetworkNoninterleavedContainers, PossibleSampleValueTypes>;

template<typename SampleValueType>
using PossibleNetworkNoninterleavedBufferContainers = BufferWrapper<network_noninterleaved<SampleValueType>>;
using PossibleNetworkNoninterleavedBuffers =
    testing_types_combine_t<PossibleNetworkNoninterleavedBufferContainers, PossibleSampleValueTypes>;

template<typename SampleValueType>
using PossibleNetworkNoninterleavedSpanContainers = ::testing::Types<
    SpanWrapper<network_noninterleaved_span<SampleValueType>>,
    SpanWrapper<const_network_noninterleaved_span<SampleValueType>>>;
using PossibleNetworkNoninterleavedSpans =
    testing_types_combine_t<PossibleNetworkNoninterleavedSpanContainers, PossibleSampleValueTypes>;

template<typename SampleValueType>
using PossibleNonConstNetworkNoninterleavedContainers = ::testing::Types<
    BufferWrapper<network_noninterleaved<SampleValueType>>,
    SpanWrapper<network_noninterleaved_span<SampleValueType>>>;
using PossibleNonConstNetworkNoninterleaved =
    testing_types_combine_t<PossibleNonConstNetworkNoninterleavedContainers, PossibleSampleValueTypes>;

template<typename InputBufferWrapper, typename OutputBufferWrapper>
struct BufferCombination
{
    using input_wrapper_type = InputBufferWrapper;
    using output_wrapper_type = OutputBufferWrapper;
};

using PossibleInterleavedTransformCombinations = testing_types_concat_t<
    testing_types_combine_t<BufferCombination, PossibleHostInterleaved, PossibleNonConstHostInterleaved>,
    testing_types_combine_t<BufferCombination, PossibleHostInterleaved, PossibleNonConstNetworkInterleaved>,
    testing_types_combine_t<BufferCombination, PossibleNetworkInterleaved, PossibleNonConstHostInterleaved>>;

using PossibleNoninterleavedTransformCombinations = testing_types_concat_t<
    testing_types_combine_t<BufferCombination, PossibleHostNoninterleaved, PossibleNonConstHostNoninterleaved>,
    testing_types_combine_t<BufferCombination, PossibleHostNoninterleaved, PossibleNonConstNetworkNoninterleaved>,
    testing_types_combine_t<BufferCombination, PossibleNetworkNoninterleaved, PossibleNonConstHostNoninterleaved>>;

using PossibleInterleavedToNoninterleavedTransformCombinations = testing_types_concat_t<
    testing_types_combine_t<BufferCombination, PossibleHostInterleaved, PossibleNonConstHostNoninterleaved>,
    testing_types_combine_t<BufferCombination, PossibleHostInterleaved, PossibleNonConstNetworkNoninterleaved>,
    testing_types_combine_t<BufferCombination, PossibleNetworkInterleaved, PossibleNonConstHostNoninterleaved>>;

using PossibleNoninterleavedToInterleavedTransformCombinations = testing_types_concat_t<
    testing_types_combine_t<BufferCombination, PossibleHostNoninterleaved, PossibleNonConstHostInterleaved>,
    testing_types_combine_t<BufferCombination, PossibleHostNoninterleaved, PossibleNonConstNetworkInterleaved>,
    testing_types_combine_t<BufferCombination, PossibleNetworkNoninterleaved, PossibleNonConstHostInterleaved>>;

using SelectedInterleavedTransformCombinations = testing_types_concat_t<
    testing_types_combine_t<BufferCombination, PossibleHostInterleavedBuffers, PossibleHostInterleavedBuffers>>;

using SelectedNoninterleavedTransformCombinations = testing_types_concat_t<
    testing_types_combine_t<BufferCombination, PossibleHostNoninterleavedBuffers, PossibleHostNoninterleavedBuffers>>;

using SelectedInterleavedToNoninterleavedTransformCombinations = testing_types_concat_t<
    testing_types_combine_t<BufferCombination, PossibleHostInterleavedBuffers, PossibleHostNoninterleavedBuffers>>;

using SelectedNoninterleavedToInterleavedTransformCombinations = testing_types_concat_t<
    testing_types_combine_t<BufferCombination, PossibleHostNoninterleavedBuffers, PossibleHostInterleavedBuffers>>;

template<typename BufferWrapper, size_t Channels, size_t Frames>
class TestBase : public ::testing::Test
{
public:
    using wrapper_type = BufferWrapper;
    using container_type = typename wrapper_type::container_type;
    using storage_type = typename wrapper_type::storage_type;
    using sample_type = typename container_type::sample_type;
    using sample_value_type = typename sample_type::value_type;

    using size_type = typename container_type::size_type;

private:
    wrapper_type wrapper_;

public:
    TestBase() : wrapper_(generate_configured_wrapper()) {}

    wrapper_type& wrapper()
    {
        return wrapper_;
    }

    container_type& container()
    {
        return wrapper_.container();
    }

    storage_type& storage()
    {
        return wrapper_.storage();
    }

    static constexpr size_type configured_channels()
    {
        return Channels;
    }

    static constexpr size_type configured_frames()
    {
        return Frames;
    }

    static constexpr size_type configured_samples()
    {
        return Channels * Frames;
    }

    static wrapper_type generate_wrapper(size_type channels, size_type frames)
    {
        return wrapper_type(channels, frames);
    }

    static wrapper_type generate_default_wrapper()
    {
        return wrapper_type();
    }

    static wrapper_type generate_configured_wrapper()
    {
        return generate_wrapper(Channels, Frames);
    }

    static void fill_wrapper(wrapper_type& wrapper)
    {
        auto size = wrapper.container().samples();
        if (size > 0)
        {
            auto increment = sample_limits<int32_t>::max() / size;
            for (size_type i = 0; i < size; ++i)
            {
                wrapper.storage().data()[i] = make_sample(static_cast<int32_t>(increment * i * std::pow(-1, i)));
            }
        }
    }

    static sample_type make_sample(int32_t value)
    {
        return reference_convert<sample_type>(sample<int32_t>(value));
    }
};

template<typename BufferWrapper>
class TypicalTestBase : public TestBase<BufferWrapper, TEST_RATL_TYPICAL_CHANNELS, TEST_RATL_TYPICAL_FRAMES>
{
};

template<typename BufferWrapper>
class ZeroFramesAndChannelsTestBase : public TestBase<BufferWrapper, 0, 0>
{
};

template<typename BufferWrapper>
class ZeroFramesTestBase : public TestBase<BufferWrapper, TEST_RATL_TYPICAL_CHANNELS, 0>
{
};

template<typename BufferWrapper>
class SingleFrameTestBase : public TestBase<BufferWrapper, TEST_RATL_TYPICAL_CHANNELS, 1>
{
};

template<typename BufferWrapper>
class ZeroChannelsTestBase : public TestBase<BufferWrapper, 0, TEST_RATL_TYPICAL_FRAMES>
{
};

template<typename BufferWrapper>
class SingleChannelTestBase : public TestBase<BufferWrapper, 1, TEST_RATL_TYPICAL_FRAMES>
{
};

template<typename BufferWrapper>
class CopyTestBase : public TypicalTestBase<BufferWrapper>
{
public:
    CopyTestBase()
    {
        CopyTestBase::TypicalTestBase::fill_wrapper(this->wrapper());
    }

    ~CopyTestBase()
    {
        auto test_wrapper = this->generate_configured_wrapper();
        CopyTestBase::TypicalTestBase::fill_wrapper(test_wrapper);
        EXPECT_TRUE(IsContainerContentEqual(this->container(), test_wrapper.container()));
    }
};

template<typename BufferWrapper>
class MoveTestBase : public TypicalTestBase<BufferWrapper>
{
public:
    MoveTestBase()
    {
        MoveTestBase::TypicalTestBase::fill_wrapper(this->wrapper());
    }

    ~MoveTestBase()
    {
        EXPECT_TRUE(IsContainerContentEqual(this->container(), this->generate_default_wrapper().container()));
    }
};

template<typename TransformCombination, template<typename> class BaseType>
class TransformTestBase : public BaseType<typename TransformCombination::input_wrapper_type>
{
public:
    using super_type = BaseType<typename TransformCombination::input_wrapper_type>;

    using output_wrapper_type = typename TransformCombination::output_wrapper_type;
    using output_container_type = typename output_wrapper_type::container_type;
    using output_sample_type = typename output_container_type::sample_type;

    TransformTestBase()
    {
        super_type::fill_wrapper(this->wrapper());
    }

    static output_wrapper_type generate_output_wrapper(size_t channels, size_t frames)
    {
        return output_wrapper_type(channels, frames);
    }
};

template<typename TransformCombination>
class TypicalTransformTestBase : public TransformTestBase<TransformCombination, TypicalTestBase>
{
};

template<typename TransformCombination>
class SingleFrameTransformTestBase : public TransformTestBase<TransformCombination, SingleFrameTestBase>
{
};

template<typename TransformCombination>
class SingleChannelTransformTestBase : public TransformTestBase<TransformCombination, SingleChannelTestBase>
{
};

} // namespace test
} // namespace ratl

#endif
