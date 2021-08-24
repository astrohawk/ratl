#ifndef _ratl_detail_transformer_
#define _ratl_detail_transformer_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/detail/batch_creator.hpp>
#include <ratl/detail/channel_iterator.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/detail/frame_iterator.hpp>
#include <ratl/detail/interleaved_iterator.hpp>
#include <ratl/detail/noninterleaved_iterator.hpp>
#include <ratl/detail/sample_converter.hpp>

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

#if defined(RATL_HAS_XSIMD)

template<
    template<class, class, class>
    class SampleConverterT,
    class InputSample,
    class OutputSample,
    class DitherGenerator>
class BasicTransformerImpl
{
private:
    using InputBatchCreator = BatchCreator<InputSample>;
    using OutputBatchCreator = BatchCreator<OutputSample>;

    using InputBatchType = typename InputBatchCreator::batch_type;
    using OutputBatchType = typename OutputBatchCreator::batch_type;

    static constexpr std::size_t BatchSize = InputBatchType::size;
    static_assert(BatchSize == OutputBatchType::size, "Input and output batch size are not equal");

    using SampleConverter = SampleConverterT<InputSample, OutputSample, DitherGenerator>;

public:
    explicit BasicTransformerImpl(DitherGenerator& dither_generator) : sample_converter_{dither_generator} {}

    template<typename InputIterator, typename OutputIterator>
    inline OutputIterator transform(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        std::size_t distance = std::distance(first, last);
        std::size_t vec_distance = distance - distance % BatchSize;
        InputIterator vec_last = first + vec_distance;
        for (; first != vec_last; first += BatchSize, result += BatchSize)
        {
            auto input = InputBatchCreator::load(first);
            auto output = convert(input, sample_converter_);
            OutputBatchCreator::store(output, result);
        }
        return std::transform(first, last, result, sample_converter_);
    }

private:
    SampleConverter sample_converter_;
};

#else

template<
    template<class, class, class>
    class SampleConverterT,
    class InputSample,
    class OutputSample,
    class DitherGenerator>
class BasicTransformerImpl
{
private:
    using SampleConverter = SampleConverterT<InputSample, OutputSample, DitherGenerator>;

public:
    explicit BasicTransformerImpl(DitherGenerator& dither_generator) : sample_converter_{dither_generator} {}

    template<typename InputIterator, typename OutputIterator>
    inline OutputIterator transform(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        return std::transform(first, last, result, sample_converter_);
    }

    SampleConverter sample_converter_;
};

#endif

template<template<class, class, class> class SampleConverterT, class Sample, class DitherGenerator>
struct BasicTransformerImpl<SampleConverterT, Sample, Sample, DitherGenerator>
{
    explicit BasicTransformerImpl(DitherGenerator&) {}

    template<typename InputIterator, typename OutputIterator>
    inline OutputIterator transform(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        return std::copy(first, last, result);
    }
};

// BasicTransformer class

template<
    template<class, class, class>
    class SampleConverterT,
    class InputIterator,
    class OutputIterator,
    class DitherGenerator>
class BasicTransformer;

// Transformer for InterleavedIterator

template<
    template<class, class, class>
    class SampleConverterT,
    class InputSample,
    class OutputSample,
    class DitherGenerator>
class BasicTransformer<
    SampleConverterT,
    InterleavedIterator<InputSample>,
    InterleavedIterator<OutputSample>,
    DitherGenerator>
{
private:
    using InputIterator = InterleavedIterator<InputSample>;
    using OutputIterator = InterleavedIterator<OutputSample>;

    using BaseInputSample = std::remove_cv_t<InputSample>;
    using BaseOutputSample = std::remove_cv_t<OutputSample>;

    using TransformerImpl = BasicTransformerImpl<SampleConverterT, BaseInputSample, BaseOutputSample, DitherGenerator>;

    using InputFrame = typename InputIterator::value_type;
    using OutputFrame = typename OutputIterator::value_type;

    using InputFrameIterator = typename InputFrame::const_iterator;
    using OutputFrameIterator = typename OutputFrame::iterator;

    using FrameTransformer =
        BasicTransformer<SampleConverterT, InputFrameIterator, OutputFrameIterator, DitherGenerator>;

public:
    explicit BasicTransformer(DitherGenerator& dither_generator) : dither_generator_{dither_generator} {}

    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        if (first.channels() == result.channels())
        {
            // Input and output have same number of channels, so blit samples

            return OutputIterator(
                TransformerImpl{dither_generator_}.transform(first.base(), last.base(), result.base()),
                result.channels());
        }
        else
        {
            // Input and output don't have same number of channels, so must transform frame by frame

            FrameTransformer frame_transformer{dither_generator_};
            auto min_channels = std::min(first.channels(), result.channels());
            return detail::apply(
                first,
                last,
                result,
                [&frame_transformer, min_channels](InputFrame input, OutputFrame output)
                {
                    auto input_begin = input.cbegin();
                    auto input_end = std::next(input_begin, min_channels);
                    frame_transformer(input_begin, input_end, output.begin());
                });
        }
    }

private:
    std::reference_wrapper<DitherGenerator> dither_generator_;
};

// Transformer for NoninterleavedIterator

template<
    template<class, class, class>
    class SampleConverterT,
    class InputSample,
    class OutputSample,
    class DitherGenerator>
class BasicTransformer<
    SampleConverterT,
    NoninterleavedIterator<InputSample>,
    NoninterleavedIterator<OutputSample>,
    DitherGenerator>
{
private:
    using InputIterator = NoninterleavedIterator<InputSample>;
    using OutputIterator = NoninterleavedIterator<OutputSample>;

    using BaseInputSample = std::remove_cv_t<InputSample>;
    using BaseOutputSample = std::remove_cv_t<OutputSample>;

    using TransformerImpl = BasicTransformerImpl<SampleConverterT, BaseInputSample, BaseOutputSample, DitherGenerator>;

    using InputChannel = typename InputIterator::value_type;
    using OutputChannel = typename OutputIterator::value_type;

    using InputChannelIterator = typename InputChannel::const_iterator;
    using OutputChannelIterator = typename OutputChannel::iterator;

    using ChannelTransformer =
        BasicTransformer<SampleConverterT, InputChannelIterator, OutputChannelIterator, DitherGenerator>;

public:
    explicit BasicTransformer(DitherGenerator& dither_generator) : dither_generator_{dither_generator} {}

    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        if (first.frames() == result.frames())
        {
            // Input and output have same number of channels, so blit samples

            return OutputIterator(
                TransformerImpl{dither_generator_}.transform(first.base(), last.base(), result.base()),
                result.frames());
        }
        else
        {
            // Input and output don't have same number of frames, so must transform channel by channel

            ChannelTransformer channel_transformer{dither_generator_};
            auto min_frames = std::min(first.frames(), result.frames());
            return detail::apply(
                first,
                last,
                result,
                [&channel_transformer, min_frames](InputChannel input, OutputChannel output)
                {
                    auto input_begin = input.cbegin();
                    auto input_end = std::next(input_begin, min_frames);
                    channel_transformer(input_begin, input_end, output.begin());
                });
        }
    }

private:
    std::reference_wrapper<DitherGenerator> dither_generator_;
};

// Transformer for FrameIterator

template<
    template<class, class, class>
    class SampleConverterT,
    class InputSample,
    class OutputSample,
    class DitherGenerator>
class BasicTransformer<
    SampleConverterT,
    FrameIterator<InputSample, false>,
    FrameIterator<OutputSample, false>,
    DitherGenerator>
{
private:
    using InputIterator = FrameIterator<InputSample, false>;
    using OutputIterator = FrameIterator<OutputSample, false>;

    using BaseInputSample = std::remove_cv_t<InputSample>;
    using BaseOutputSample = std::remove_cv_t<OutputSample>;

    using TransformerImpl = BasicTransformerImpl<SampleConverterT, BaseInputSample, BaseOutputSample, DitherGenerator>;

public:
    explicit BasicTransformer(DitherGenerator& dither_generator) : transformer_impl_{dither_generator} {}

    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        if (first.stride() == 1)
        {
            if (result.stride() == 1)
            {
                return OutputIterator(transformer_impl_.transform(first.base(), last.base(), result.base()), 1);
            }
            else
            {
                return transformer_impl_.transform(first.base(), last.base(), result);
            }
        }
        else
        {
            if (result.stride() == 1)
            {
                return OutputIterator(transformer_impl_.transform(first, last, result.base()), 1);
            }
            else
            {
                return transformer_impl_.transform(first, last, result);
            }
        }
    }

private:
    TransformerImpl transformer_impl_;
};

template<
    template<class, class, class>
    class SampleConverterT,
    class InputSample,
    class OutputSample,
    class DitherGenerator>
class BasicTransformer<
    SampleConverterT,
    FrameIterator<InputSample, false>,
    FrameIterator<OutputSample, true>,
    DitherGenerator>
{
private:
    using InputIterator = FrameIterator<InputSample, false>;
    using OutputIterator = FrameIterator<OutputSample, true>;

    using BaseInputSample = std::remove_cv_t<InputSample>;
    using BaseOutputSample = std::remove_cv_t<OutputSample>;

    using TransformerImpl = BasicTransformerImpl<SampleConverterT, BaseInputSample, BaseOutputSample, DitherGenerator>;

public:
    explicit BasicTransformer(DitherGenerator& dither_generator) : transformer_impl_{dither_generator} {}

    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        if (first.stride() == 1)
        {
            return OutputIterator(transformer_impl_.transform(first.base(), last.base(), result.base()));
        }
        else
        {
            return OutputIterator(transformer_impl_.transform(first, last, result.base()));
        }
    }

private:
    TransformerImpl transformer_impl_;
};

template<
    template<class, class, class>
    class SampleConverterT,
    class InputSample,
    class OutputSample,
    class DitherGenerator>
class BasicTransformer<
    SampleConverterT,
    FrameIterator<InputSample, true>,
    FrameIterator<OutputSample, false>,
    DitherGenerator>
{
private:
    using InputIterator = FrameIterator<InputSample, true>;
    using OutputIterator = FrameIterator<OutputSample, false>;

    using BaseInputSample = std::remove_cv_t<InputSample>;
    using BaseOutputSample = std::remove_cv_t<OutputSample>;

    using TransformerImpl = BasicTransformerImpl<SampleConverterT, BaseInputSample, BaseOutputSample, DitherGenerator>;

public:
    explicit BasicTransformer(DitherGenerator& dither_generator) : transformer_impl_{dither_generator} {}

    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        if (result.stride() == 1)
        {
            return OutputIterator(transformer_impl_.transform(first.base(), last.base(), result.base()), 1);
        }
        else
        {
            return transformer_impl_.transform(first.base(), last.base(), result);
        }
    }

private:
    TransformerImpl transformer_impl_;
};

template<
    template<class, class, class>
    class SampleConverterT,
    class InputSample,
    class OutputSample,
    class DitherGenerator>
class BasicTransformer<
    SampleConverterT,
    FrameIterator<InputSample, true>,
    FrameIterator<OutputSample, true>,
    DitherGenerator>
{
private:
    using InputIterator = FrameIterator<InputSample, true>;
    using OutputIterator = FrameIterator<OutputSample, true>;

    using BaseInputSample = std::remove_cv_t<InputSample>;
    using BaseOutputSample = std::remove_cv_t<OutputSample>;

    using TransformerImpl = BasicTransformerImpl<SampleConverterT, BaseInputSample, BaseOutputSample, DitherGenerator>;

public:
    explicit BasicTransformer(DitherGenerator& dither_generator) : transformer_impl_{dither_generator} {}

    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        return OutputIterator(transformer_impl_.transform(first.base(), last.base(), result.base()));
    }

private:
    TransformerImpl transformer_impl_;
};

// Transformer for ChannelIterator

template<
    template<class, class, class>
    class SampleConverterT,
    class InputSample,
    class OutputSample,
    class DitherGenerator>
class BasicTransformer<
    SampleConverterT,
    ChannelIterator<InputSample, false>,
    ChannelIterator<OutputSample, false>,
    DitherGenerator>
{
private:
    using InputIterator = ChannelIterator<InputSample, false>;
    using OutputIterator = ChannelIterator<OutputSample, false>;

    using BaseInputSample = std::remove_cv_t<InputSample>;
    using BaseOutputSample = std::remove_cv_t<OutputSample>;

    using TransformerImpl = BasicTransformerImpl<SampleConverterT, BaseInputSample, BaseOutputSample, DitherGenerator>;

public:
    explicit BasicTransformer(DitherGenerator& dither_generator) : transformer_impl_{dither_generator} {}

    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        if (first.stride() == 1)
        {
            if (result.stride() == 1)
            {
                return OutputIterator(transformer_impl_.transform(first.base(), last.base(), result.base()), 1);
            }
            else
            {
                return transformer_impl_.transform(first.base(), last.base(), result);
            }
        }
        else
        {
            if (result.stride() == 1)
            {
                return OutputIterator(transformer_impl_.transform(first, last, result.base()), 1);
            }
            else
            {
                return transformer_impl_.transform(first, last, result);
            }
        }
    }

private:
    TransformerImpl transformer_impl_;
};

template<
    template<class, class, class>
    class SampleConverterT,
    class InputSample,
    class OutputSample,
    class DitherGenerator>
class BasicTransformer<
    SampleConverterT,
    ChannelIterator<InputSample, false>,
    ChannelIterator<OutputSample, true>,
    DitherGenerator>
{
private:
    using InputIterator = ChannelIterator<InputSample, false>;
    using OutputIterator = ChannelIterator<OutputSample, true>;

    using BaseInputSample = std::remove_cv_t<InputSample>;
    using BaseOutputSample = std::remove_cv_t<OutputSample>;

    using TransformerImpl = BasicTransformerImpl<SampleConverterT, BaseInputSample, BaseOutputSample, DitherGenerator>;

public:
    explicit BasicTransformer(DitherGenerator& dither_generator) : transformer_impl_{dither_generator} {}

    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        if (first.stride() == 1)
        {
            return OutputIterator(transformer_impl_.transform(first.base(), last.base(), result.base()));
        }
        else
        {
            return OutputIterator(transformer_impl_.transform(first, last, result.base()));
        }
    }

private:
    TransformerImpl transformer_impl_;
};

template<
    template<class, class, class>
    class SampleConverterT,
    class InputSample,
    class OutputSample,
    class DitherGenerator>
class BasicTransformer<
    SampleConverterT,
    ChannelIterator<InputSample, true>,
    ChannelIterator<OutputSample, false>,
    DitherGenerator>
{
private:
    using InputIterator = ChannelIterator<InputSample, true>;
    using OutputIterator = ChannelIterator<OutputSample, false>;

    using BaseInputSample = std::remove_cv_t<InputSample>;
    using BaseOutputSample = std::remove_cv_t<OutputSample>;

    using TransformerImpl = BasicTransformerImpl<SampleConverterT, BaseInputSample, BaseOutputSample, DitherGenerator>;

public:
    explicit BasicTransformer(DitherGenerator& dither_generator) : transformer_impl_{dither_generator} {}

    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        if (result.stride() == 1)
        {
            return OutputIterator(transformer_impl_.transform(first.base(), last.base(), result.base()), 1);
        }
        else
        {
            return transformer_impl_.transform(first.base(), last.base(), result);
        }
    }

private:
    TransformerImpl transformer_impl_;
};

template<
    template<class, class, class>
    class SampleConverterT,
    class InputSample,
    class OutputSample,
    class DitherGenerator>
class BasicTransformer<
    SampleConverterT,
    ChannelIterator<InputSample, true>,
    ChannelIterator<OutputSample, true>,
    DitherGenerator>
{
private:
    using InputIterator = ChannelIterator<InputSample, true>;
    using OutputIterator = ChannelIterator<OutputSample, true>;

    using BaseInputSample = std::remove_cv_t<InputSample>;
    using BaseOutputSample = std::remove_cv_t<OutputSample>;

    using TransformerImpl = BasicTransformerImpl<SampleConverterT, BaseInputSample, BaseOutputSample, DitherGenerator>;

public:
    explicit BasicTransformer(DitherGenerator& dither_generator) : transformer_impl_{dither_generator} {}

    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        return OutputIterator(transformer_impl_.transform(first.base(), last.base(), result.base()));
    }

private:
    TransformerImpl transformer_impl_;
};

template<class InputIterator, class OutputIterator, class DitherGenerator>
using ReferenceTransformer = BasicTransformer<ReferenceSampleConverter, InputIterator, OutputIterator, DitherGenerator>;

template<class InputIterator, class OutputIterator, class DitherGenerator>
using FastTransformer = BasicTransformer<FastSampleConverter, InputIterator, OutputIterator, DitherGenerator>;

template<class InputIterator, class OutputIterator, class DitherGenerator>
using DefaultTransformer = BasicTransformer<DefaultSampleConverter, InputIterator, OutputIterator, DitherGenerator>;

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_transformer_
