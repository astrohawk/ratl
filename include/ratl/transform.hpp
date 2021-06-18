#ifndef _ratl_transform_
#define _ratl_transform_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/detail/batch_sample_transformer.hpp>
#include <ratl/detail/channel_iterator.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/detail/frame_iterator.hpp>
#include <ratl/detail/interleaved_iterator.hpp>
#include <ratl/detail/noninterleaved_iterator.hpp>
#include <ratl/detail/sample_transformer.hpp>
#include <ratl/dither_generator.hpp>

namespace ratl
{
namespace detail
{
// utility function

template<class Iterator1, class Iterator2, class BinaryOp>
Iterator2 apply(Iterator1 first1, Iterator1 last1, Iterator2 first2, BinaryOp binary_op)
{
    for (; first1 != last1; ++first1, (void)++first2)
    {
        binary_op(*first1, *first2);
    }
    return first2;
}

// TransformImpl class

template<class InputIterator, class OutputIterator, class DitherGenerator>
class DefaultTransformerImpl;

// DefaultTransformerImpl for InterleavedIterator

template<class InputSample, class OutputSample, class DitherGenerator>
class DefaultTransformerImpl<InterleavedIterator<InputSample>, InterleavedIterator<OutputSample>, DitherGenerator>
{
private:
    using InputIterator = InterleavedIterator<InputSample>;
    using OutputIterator = InterleavedIterator<OutputSample>;

    using BaseInputSample = std::remove_cv_t<InputSample>;
    using BaseOutputSample = std::remove_cv_t<OutputSample>;

    using SampleTransformer = BatchSampleTransformerImpl<BaseInputSample, BaseOutputSample, DitherGenerator>;

    using InputFrame = typename InputIterator::value_type;
    using OutputFrame = typename OutputIterator::value_type;

    using InputFrameIterator = typename InputFrame::const_iterator;
    using OutputFrameIterator = typename OutputFrame::iterator;

public:
    explicit DefaultTransformerImpl(DitherGenerator& dither_generator) : dither_generator_{dither_generator} {}

    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        if (first.channels() == result.channels())
        {
            // Input and output have same number of channels, so blit samples

            return OutputIterator(
                result.channels(), SampleTransformer{dither_generator_}(first.base(), last.base(), result.base()));
        }
        else
        {
            // Input and output don't have same number of channels, so must transform frame by frame

            DefaultTransformerImpl<InputFrameIterator, OutputFrameIterator, DitherGenerator> transform{
                dither_generator_};
            auto min_channels = std::min(first.channels(), result.channels());
            return detail::apply(
                first,
                last,
                result,
                [&transform, min_channels](InputFrame input, OutputFrame output)
                {
                    auto input_begin = input.cbegin();
                    auto input_end = std::next(input_begin, min_channels);
                    transform(input_begin, input_end, output.begin());
                });
        }
    }

private:
    DitherGenerator& dither_generator_;
};

// DefaultTransformerImpl for NoninterleavedIterator

template<class InputSample, class OutputSample, class DitherGenerator>
class DefaultTransformerImpl<NoninterleavedIterator<InputSample>, NoninterleavedIterator<OutputSample>, DitherGenerator>
{
private:
    using InputIterator = NoninterleavedIterator<InputSample>;
    using OutputIterator = NoninterleavedIterator<OutputSample>;

    using BaseInputSample = std::remove_cv_t<InputSample>;
    using BaseOutputSample = std::remove_cv_t<OutputSample>;

    using SampleTransformer = BatchSampleTransformerImpl<BaseInputSample, BaseOutputSample, DitherGenerator>;

    using InputChannel = typename InputIterator::value_type;
    using OutputChannel = typename OutputIterator::value_type;

    using InputChannelIterator = typename InputChannel::const_iterator;
    using OutputChannelIterator = typename OutputChannel::iterator;

public:
    explicit DefaultTransformerImpl(DitherGenerator& dither_generator) : dither_generator_{dither_generator} {}

    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        if (first.frames() == result.frames())
        {
            // Input and output have same number of channels, so blit samples

            return OutputIterator(
                result.frames(), SampleTransformer{dither_generator_}(first.base(), last.base(), result.base()));
        }
        else
        {
            // Input and output don't have same number of frames, so must transform channel by channel

            DefaultTransformerImpl<InputChannelIterator, OutputChannelIterator, DitherGenerator> transform{
                dither_generator_};
            auto min_frames = std::min(first.frames(), result.frames());
            return detail::apply(
                first,
                last,
                result,
                [&transform, min_frames](InputChannel input, OutputChannel output)
                {
                    auto input_begin = input.cbegin();
                    auto input_end = std::next(input_begin, min_frames);
                    transform(input_begin, input_end, output.begin());
                });
        }
    }

private:
    DitherGenerator& dither_generator_;
};

// DefaultTransformerImpl for FrameIterator

template<class InputSample, class OutputSample, class DitherGenerator>
class DefaultTransformerImpl<FrameIterator<InputSample, false>, FrameIterator<OutputSample, false>, DitherGenerator>
{
private:
    using InputIterator = FrameIterator<InputSample, false>;
    using OutputIterator = FrameIterator<OutputSample, false>;

    using BaseInputSample = std::remove_cv_t<InputSample>;
    using BaseOutputSample = std::remove_cv_t<OutputSample>;

    using SampleTransformer = BatchSampleTransformerImpl<BaseInputSample, BaseOutputSample, DitherGenerator>;

public:
    explicit DefaultTransformerImpl(DitherGenerator& dither_generator) : sample_transformer_{dither_generator} {}

    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        if (first.stride() == 1)
        {
            if (result.stride() == 1)
            {
                return OutputIterator(1, sample_transformer_(first.base(), last.base(), result.base()));
            }
            else
            {
                return sample_transformer_(first.base(), last.base(), result);
            }
        }
        else
        {
            if (result.stride() == 1)
            {
                return OutputIterator(1, sample_transformer_(first, last, result.base()));
            }
            else
            {
                return sample_transformer_(first, last, result);
            }
        }
    }

private:
    SampleTransformer sample_transformer_;
};

template<class InputSample, class OutputSample, class DitherGenerator>
class DefaultTransformerImpl<FrameIterator<InputSample, false>, FrameIterator<OutputSample, true>, DitherGenerator>
{
private:
    using InputIterator = FrameIterator<InputSample, false>;
    using OutputIterator = FrameIterator<OutputSample, true>;

    using BaseInputSample = std::remove_cv_t<InputSample>;
    using BaseOutputSample = std::remove_cv_t<OutputSample>;

    using SampleTransformer = BatchSampleTransformerImpl<BaseInputSample, BaseOutputSample, DitherGenerator>;

public:
    explicit DefaultTransformerImpl(DitherGenerator& dither_generator) : sample_transformer_{dither_generator} {}

    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        if (first.stride() == 1)
        {
            return OutputIterator(sample_transformer_(first.base(), last.base(), result.base()));
        }
        else
        {
            return OutputIterator(sample_transformer_(first, last, result.base()));
        }
    }

private:
    SampleTransformer sample_transformer_;
};

template<class InputSample, class OutputSample, class DitherGenerator>
class DefaultTransformerImpl<FrameIterator<InputSample, true>, FrameIterator<OutputSample, false>, DitherGenerator>
{
private:
    using InputIterator = FrameIterator<InputSample, true>;
    using OutputIterator = FrameIterator<OutputSample, false>;

    using BaseInputSample = std::remove_cv_t<InputSample>;
    using BaseOutputSample = std::remove_cv_t<OutputSample>;

    using SampleTransformer = BatchSampleTransformerImpl<BaseInputSample, BaseOutputSample, DitherGenerator>;

public:
    explicit DefaultTransformerImpl(DitherGenerator& dither_generator) : sample_transformer_{dither_generator} {}

    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        if (result.stride() == 1)
        {
            return OutputIterator(1, sample_transformer_(first.base(), last.base(), result.base()));
        }
        else
        {
            return sample_transformer_(first.base(), last.base(), result);
        }
    }

private:
    SampleTransformer sample_transformer_;
};

template<class InputSample, class OutputSample, class DitherGenerator>
class DefaultTransformerImpl<FrameIterator<InputSample, true>, FrameIterator<OutputSample, true>, DitherGenerator>
{
private:
    using InputIterator = FrameIterator<InputSample, true>;
    using OutputIterator = FrameIterator<OutputSample, true>;

    using BaseInputSample = std::remove_cv_t<InputSample>;
    using BaseOutputSample = std::remove_cv_t<OutputSample>;

    using SampleTransformer = BatchSampleTransformerImpl<BaseInputSample, BaseOutputSample, DitherGenerator>;

public:
    explicit DefaultTransformerImpl(DitherGenerator& dither_generator) : sample_transformer_{dither_generator} {}

    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        return OutputIterator(sample_transformer_(first.base(), last.base(), result.base()));
    }

private:
    SampleTransformer sample_transformer_;
};

// DefaultTransformerImpl for ChannelIterator

template<class InputSample, class OutputSample, class DitherGenerator>
class DefaultTransformerImpl<ChannelIterator<InputSample, false>, ChannelIterator<OutputSample, false>, DitherGenerator>
{
private:
    using InputIterator = ChannelIterator<InputSample, false>;
    using OutputIterator = ChannelIterator<OutputSample, false>;

    using BaseInputSample = std::remove_cv_t<InputSample>;
    using BaseOutputSample = std::remove_cv_t<OutputSample>;

    using SampleTransformer = BatchSampleTransformerImpl<BaseInputSample, BaseOutputSample, DitherGenerator>;

public:
    explicit DefaultTransformerImpl(DitherGenerator& dither_generator) : sample_transformer_{dither_generator} {}

    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        if (first.stride() == 1)
        {
            if (result.stride() == 1)
            {
                return OutputIterator(1, sample_transformer_(first.base(), last.base(), result.base()));
            }
            else
            {
                return sample_transformer_(first.base(), last.base(), result);
            }
        }
        else
        {
            if (result.stride() == 1)
            {
                return OutputIterator(1, sample_transformer_(first, last, result.base()));
            }
            else
            {
                return sample_transformer_(first, last, result);
            }
        }
    }

private:
    SampleTransformer sample_transformer_;
};

template<class InputSample, class OutputSample, class DitherGenerator>
class DefaultTransformerImpl<ChannelIterator<InputSample, false>, ChannelIterator<OutputSample, true>, DitherGenerator>
{
private:
    using InputIterator = ChannelIterator<InputSample, false>;
    using OutputIterator = ChannelIterator<OutputSample, true>;

    using BaseInputSample = std::remove_cv_t<InputSample>;
    using BaseOutputSample = std::remove_cv_t<OutputSample>;

    using SampleTransformer = BatchSampleTransformerImpl<BaseInputSample, BaseOutputSample, DitherGenerator>;

public:
    explicit DefaultTransformerImpl(DitherGenerator& dither_generator) : sample_transformer_{dither_generator} {}

    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        if (first.stride() == 1)
        {
            return OutputIterator(sample_transformer_(first.base(), last.base(), result.base()));
        }
        else
        {
            return OutputIterator(sample_transformer_(first, last, result.base()));
        }
    }

private:
    SampleTransformer sample_transformer_;
};

template<class InputSample, class OutputSample, class DitherGenerator>
class DefaultTransformerImpl<ChannelIterator<InputSample, true>, ChannelIterator<OutputSample, false>, DitherGenerator>
{
private:
    using InputIterator = ChannelIterator<InputSample, true>;
    using OutputIterator = ChannelIterator<OutputSample, false>;

    using BaseInputSample = std::remove_cv_t<InputSample>;
    using BaseOutputSample = std::remove_cv_t<OutputSample>;

    using SampleTransformer = BatchSampleTransformerImpl<BaseInputSample, BaseOutputSample, DitherGenerator>;

public:
    explicit DefaultTransformerImpl(DitherGenerator& dither_generator) : sample_transformer_{dither_generator} {}

    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        if (result.stride() == 1)
        {
            return OutputIterator(1, sample_transformer_(first.base(), last.base(), result.base()));
        }
        else
        {
            return sample_transformer_(first.base(), last.base(), result);
        }
    }

private:
    SampleTransformer sample_transformer_;
};

template<class InputSample, class OutputSample, class DitherGenerator>
class DefaultTransformerImpl<ChannelIterator<InputSample, true>, ChannelIterator<OutputSample, true>, DitherGenerator>
{
private:
    using InputIterator = ChannelIterator<InputSample, true>;
    using OutputIterator = ChannelIterator<OutputSample, true>;

    using BaseInputSample = std::remove_cv_t<InputSample>;
    using BaseOutputSample = std::remove_cv_t<OutputSample>;

    using SampleTransformer = BatchSampleTransformerImpl<BaseInputSample, BaseOutputSample, DitherGenerator>;

public:
    explicit DefaultTransformerImpl(DitherGenerator& dither_generator) : sample_transformer_{dither_generator} {}

    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        return OutputIterator(sample_transformer_(first.base(), last.base(), result.base()));
    }

private:
    SampleTransformer sample_transformer_;
};

// DefaultTransformer alias

template<class InputIterator, class OutputIterator, class DitherGenerator>
using DefaultTransformer = DefaultTransformerImpl<
    std::remove_cv_t<std::remove_reference_t<InputIterator>>,
    std::remove_cv_t<std::remove_reference_t<OutputIterator>>,
    DitherGenerator>;

} // namespace detail

// transform functions

template<class InputIterator, class OutputIterator, class Transformer>
inline OutputIterator transform(InputIterator first, InputIterator last, OutputIterator result, Transformer transformer)
{
    return transformer(first, last, result);
}

template<class InputIterator, class OutputIterator>
inline OutputIterator transform(
    InputIterator first, InputIterator last, OutputIterator result, DitherGenerator& dither_generator)
{
    using Transformer = detail::DefaultTransformer<InputIterator, OutputIterator, DitherGenerator>;
    return transform(first, last, result, Transformer{dither_generator});
}

template<class InputIterator, class OutputIterator>
inline OutputIterator transform(InputIterator first, InputIterator last, OutputIterator result)
{
    using TransformerDitherGenerator = detail::BatchNullDitherGenerator;
    using Transformer = detail::DefaultTransformer<InputIterator, OutputIterator, TransformerDitherGenerator>;
    TransformerDitherGenerator dither_generator{};
    return transform(first, last, result, Transformer{dither_generator});
}

} // namespace ratl

#endif // _ratl_transform_
