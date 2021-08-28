/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_transformer_
#define _ratl_detail_transformer_

// ratl includes
#include <ratl/detail/batch_creator.hpp>
#include <ratl/detail/channel_iterator.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/detail/frame_iterator.hpp>
#include <ratl/detail/interleaved_iterator.hpp>
#include <ratl/detail/noninterleaved_iterator.hpp>
#include <ratl/detail/sample_converter.hpp>

// other includes
#include <type_traits>

namespace ratl
{
namespace detail
{
// utility function

template<typename InputIterator, typename OutputIterator, typename BinaryOp>
OutputIterator apply_op(InputIterator first1, InputIterator last1, OutputIterator first2, BinaryOp binary_op)
{
    for (; first1 != last1; ++first1, (void)++first2)
    {
        binary_op(*first1, *first2);
    }
    return first2;
}

#if defined(RATL_HAS_XSIMD)

template<
    template<typename, typename, typename>
    class SampleConverter,
    typename InputSampleType,
    typename OutputSampleType,
    typename DitherGenerator>
class basic_transformer_impl
{
    using input_batch_creator = batch_creator<InputSampleType>;
    using output_batch_creator = batch_creator<OutputSampleType>;

    using input_batch_type = typename input_batch_creator::batch_type;
    using output_batch_type = typename output_batch_creator::batch_type;

    static constexpr std::size_t batch_size = input_batch_type::size;
    static_assert(batch_size == output_batch_type::size, "Input and output batch size are not equal");

    using sample_converter = SampleConverter<InputSampleType, OutputSampleType, DitherGenerator>;

public:
    explicit basic_transformer_impl(DitherGenerator& dither_gen) : sample_converter_(dither_gen) {}

    template<typename input_iterator, typename output_iterator>
    inline output_iterator transform(input_iterator first, input_iterator last, output_iterator result) const noexcept
    {
        std::size_t distance = std::distance(first, last);
        std::size_t vec_distance = distance - distance % batch_size;
        input_iterator vec_last = first + vec_distance;
        for (; first != vec_last; first += batch_size, result += batch_size)
        {
            auto input = input_batch_creator::load(first);
            auto output = convert(input, sample_converter_);
            output_batch_creator::store(output, result);
        }
        return std::transform(first, last, result, sample_converter_);
    }

private:
    sample_converter sample_converter_;
};

#else

template<
    template<typename, typename, typename>
    class SampleConverter,
    typename InputSampleType,
    typename OutputSampleType,
    typename DitherGenerator>
class basic_transformer_impl
{
    using sample_converter = SampleConverter<InputSampleType, OutputSampleType, DitherGenerator>;

public:
    explicit basic_transformer_impl(DitherGenerator& dither_gen) : sample_converter_(dither_gen) {}

    template<typename input_iterator, typename output_iterator>
    inline output_iterator transform(input_iterator first, input_iterator last, output_iterator result) const noexcept
    {
        return std::transform(first, last, result, sample_converter_);
    }

    sample_converter sample_converter_;
};

#endif

template<template<typename, typename, typename> class SampleConverter, typename SampleType, typename DitherGenerator>
struct basic_transformer_impl<SampleConverter, SampleType, SampleType, DitherGenerator>
{
    explicit basic_transformer_impl(DitherGenerator&) {}

    template<typename input_iterator, typename output_iterator>
    inline output_iterator transform(input_iterator first, input_iterator last, output_iterator result) const noexcept
    {
        return std::copy(first, last, result);
    }
};

// basic_transformer class

template<
    template<typename, typename, typename>
    class SampleConverter,
    class InputIterator,
    class OutputIterator,
    typename DitherGenerator>
class basic_transformer;

// transformer for interleaved_iterator

template<
    template<typename, typename, typename>
    class SampleConverter,
    typename InputSampleTraits,
    typename OutputSampleTraits,
    typename DitherGenerator>
class basic_transformer<
    SampleConverter,
    interleaved_iterator<InputSampleTraits>,
    interleaved_iterator<OutputSampleTraits>,
    DitherGenerator>
{
    using input_iterator = interleaved_iterator<InputSampleTraits>;
    using output_iterator = interleaved_iterator<OutputSampleTraits>;

    using base_input_sample_type = std::remove_cv_t<typename InputSampleTraits::sample_type>;
    using base_output_sample_type = std::remove_cv_t<typename OutputSampleTraits::sample_type>;

    using transformer_impl =
        basic_transformer_impl<SampleConverter, base_input_sample_type, base_output_sample_type, DitherGenerator>;

    using input_frame = typename input_iterator::value_type;
    using output_frame = typename output_iterator::value_type;

    using input_frame_iterator = typename input_frame::const_iterator;
    using output_frame_iterator = typename output_frame::iterator;

    using frame_transformer =
        basic_transformer<SampleConverter, input_frame_iterator, output_frame_iterator, DitherGenerator>;

public:
    explicit basic_transformer(DitherGenerator& dither_gen) : dither_gen_(dither_gen) {}

    inline output_iterator operator()(input_iterator first, input_iterator last, output_iterator result) const noexcept
    {
        if (first.channels() == result.channels())
        {
            // Input and output have same number of channels, so blit samples

            return output_iterator(
                transformer_impl(dither_gen_).transform(first.base(), last.base(), result.base()), result.channels());
        }
        else
        {
            // Input and output don't have same number of channels, so must transform frame by frame

            frame_transformer frame_transformer(dither_gen_);
            auto min_channels = std::min(first.channels(), result.channels());
            return detail::apply_op(
                first,
                last,
                result,
                [&frame_transformer, min_channels](input_frame input, output_frame output)
                {
                    auto input_begin = input.cbegin();
                    auto input_end = std::next(input_begin, min_channels);
                    frame_transformer(input_begin, input_end, output.begin());
                });
        }
    }

private:
    std::reference_wrapper<DitherGenerator> dither_gen_;
};

// transformer for noninterleaved_iterator

template<
    template<typename, typename, typename>
    class SampleConverter,
    typename InputSampleTraits,
    typename OutputSampleTraits,
    typename DitherGenerator>
class basic_transformer<
    SampleConverter,
    noninterleaved_iterator<InputSampleTraits>,
    noninterleaved_iterator<OutputSampleTraits>,
    DitherGenerator>
{
    using input_iterator = noninterleaved_iterator<InputSampleTraits>;
    using output_iterator = noninterleaved_iterator<OutputSampleTraits>;

    using base_input_sample_type = std::remove_cv_t<typename InputSampleTraits::sample_type>;
    using base_output_sample_type = std::remove_cv_t<typename OutputSampleTraits::sample_type>;

    using transformer_impl =
        basic_transformer_impl<SampleConverter, base_input_sample_type, base_output_sample_type, DitherGenerator>;

    using input_channel = typename input_iterator::value_type;
    using output_channel = typename output_iterator::value_type;

    using input_channel_iterator = typename input_channel::const_iterator;
    using output_channel_iterator = typename output_channel::iterator;

    using channel_transformer =
        basic_transformer<SampleConverter, input_channel_iterator, output_channel_iterator, DitherGenerator>;

public:
    explicit basic_transformer(DitherGenerator& dither_gen) : dither_gen_(dither_gen) {}

    inline output_iterator operator()(input_iterator first, input_iterator last, output_iterator result) const noexcept
    {
        if (first.frames() == result.frames())
        {
            // Input and output have same number of channels, so blit samples

            return output_iterator(
                transformer_impl(dither_gen_).transform(first.base(), last.base(), result.base()), result.frames());
        }
        else
        {
            // Input and output don't have same number of frames, so must transform channel by channel

            channel_transformer channel_transformer(dither_gen_);
            auto min_frames = std::min(first.frames(), result.frames());
            return detail::apply_op(
                first,
                last,
                result,
                [&channel_transformer, min_frames](input_channel input, output_channel output)
                {
                    auto input_begin = input.cbegin();
                    auto input_end = std::next(input_begin, min_frames);
                    channel_transformer(input_begin, input_end, output.begin());
                });
        }
    }

private:
    std::reference_wrapper<DitherGenerator> dither_gen_;
};

// transformer for frame_iterator

template<
    template<typename, typename, typename>
    class SampleConverter,
    typename InputSampleTraits,
    typename OutputSampleTraits,
    typename DitherGenerator>
class basic_transformer<
    SampleConverter,
    frame_iterator<InputSampleTraits, false>,
    frame_iterator<OutputSampleTraits, false>,
    DitherGenerator>
{
    using input_iterator = frame_iterator<InputSampleTraits, false>;
    using output_iterator = frame_iterator<OutputSampleTraits, false>;

    using base_input_sample_type = std::remove_cv_t<typename InputSampleTraits::sample_type>;
    using base_output_sample_type = std::remove_cv_t<typename OutputSampleTraits::sample_type>;

    using transformer_impl =
        basic_transformer_impl<SampleConverter, base_input_sample_type, base_output_sample_type, DitherGenerator>;

public:
    explicit basic_transformer(DitherGenerator& dither_gen) : transformer_impl_(dither_gen) {}

    inline output_iterator operator()(input_iterator first, input_iterator last, output_iterator result) const noexcept
    {
        if (first.stride() == 1)
        {
            if (result.stride() == 1)
            {
                return output_iterator(transformer_impl_.transform(first.base(), last.base(), result.base()), 1);
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
                return output_iterator(transformer_impl_.transform(first, last, result.base()), 1);
            }
            else
            {
                return transformer_impl_.transform(first, last, result);
            }
        }
    }

private:
    transformer_impl transformer_impl_;
};

template<
    template<typename, typename, typename>
    class SampleConverter,
    typename InputSampleTraits,
    typename OutputSampleTraits,
    typename DitherGenerator>
class basic_transformer<
    SampleConverter,
    frame_iterator<InputSampleTraits, false>,
    frame_iterator<OutputSampleTraits, true>,
    DitherGenerator>
{
    using input_iterator = frame_iterator<InputSampleTraits, false>;
    using output_iterator = frame_iterator<OutputSampleTraits, true>;

    using base_input_sample_type = std::remove_cv_t<typename InputSampleTraits::sample_type>;
    using base_output_sample_type = std::remove_cv_t<typename OutputSampleTraits::sample_type>;

    using transformer_impl =
        basic_transformer_impl<SampleConverter, base_input_sample_type, base_output_sample_type, DitherGenerator>;

public:
    explicit basic_transformer(DitherGenerator& dither_gen) : transformer_impl_(dither_gen) {}

    inline output_iterator operator()(input_iterator first, input_iterator last, output_iterator result) const noexcept
    {
        if (first.stride() == 1)
        {
            return output_iterator(transformer_impl_.transform(first.base(), last.base(), result.base()));
        }
        else
        {
            return output_iterator(transformer_impl_.transform(first, last, result.base()));
        }
    }

private:
    transformer_impl transformer_impl_;
};

template<
    template<typename, typename, typename>
    class SampleConverter,
    typename InputSampleTraits,
    typename OutputSampleTraits,
    typename DitherGenerator>
class basic_transformer<
    SampleConverter,
    frame_iterator<InputSampleTraits, true>,
    frame_iterator<OutputSampleTraits, false>,
    DitherGenerator>
{
    using input_iterator = frame_iterator<InputSampleTraits, true>;
    using output_iterator = frame_iterator<OutputSampleTraits, false>;

    using base_input_sample_type = std::remove_cv_t<typename InputSampleTraits::sample_type>;
    using base_output_sample_type = std::remove_cv_t<typename OutputSampleTraits::sample_type>;

    using transformer_impl =
        basic_transformer_impl<SampleConverter, base_input_sample_type, base_output_sample_type, DitherGenerator>;

public:
    explicit basic_transformer(DitherGenerator& dither_gen) : transformer_impl_(dither_gen) {}

    inline output_iterator operator()(input_iterator first, input_iterator last, output_iterator result) const noexcept
    {
        if (result.stride() == 1)
        {
            return output_iterator(transformer_impl_.transform(first.base(), last.base(), result.base()), 1);
        }
        else
        {
            return transformer_impl_.transform(first.base(), last.base(), result);
        }
    }

private:
    transformer_impl transformer_impl_;
};

template<
    template<typename, typename, typename>
    class SampleConverter,
    typename InputSampleTraits,
    typename OutputSampleTraits,
    typename DitherGenerator>
class basic_transformer<
    SampleConverter,
    frame_iterator<InputSampleTraits, true>,
    frame_iterator<OutputSampleTraits, true>,
    DitherGenerator>
{
    using input_iterator = frame_iterator<InputSampleTraits, true>;
    using output_iterator = frame_iterator<OutputSampleTraits, true>;

    using base_input_sample_type = std::remove_cv_t<typename InputSampleTraits::sample_type>;
    using base_output_sample_type = std::remove_cv_t<typename OutputSampleTraits::sample_type>;

    using transformer_impl =
        basic_transformer_impl<SampleConverter, base_input_sample_type, base_output_sample_type, DitherGenerator>;

public:
    explicit basic_transformer(DitherGenerator& dither_gen) : transformer_impl_(dither_gen) {}

    inline output_iterator operator()(input_iterator first, input_iterator last, output_iterator result) const noexcept
    {
        return output_iterator(transformer_impl_.transform(first.base(), last.base(), result.base()));
    }

private:
    transformer_impl transformer_impl_;
};

// transformer for channel_iterator

template<
    template<typename, typename, typename>
    class SampleConverter,
    typename InputSampleTraits,
    typename OutputSampleTraits,
    typename DitherGenerator>
class basic_transformer<
    SampleConverter,
    channel_iterator<InputSampleTraits, false>,
    channel_iterator<OutputSampleTraits, false>,
    DitherGenerator>
{
    using input_iterator = channel_iterator<InputSampleTraits, false>;
    using output_iterator = channel_iterator<OutputSampleTraits, false>;

    using base_input_sample_type = std::remove_cv_t<typename InputSampleTraits::sample_type>;
    using base_output_sample_type = std::remove_cv_t<typename OutputSampleTraits::sample_type>;

    using transformer_impl =
        basic_transformer_impl<SampleConverter, base_input_sample_type, base_output_sample_type, DitherGenerator>;

public:
    explicit basic_transformer(DitherGenerator& dither_gen) : transformer_impl_(dither_gen) {}

    inline output_iterator operator()(input_iterator first, input_iterator last, output_iterator result) const noexcept
    {
        if (first.stride() == 1)
        {
            if (result.stride() == 1)
            {
                return output_iterator(transformer_impl_.transform(first.base(), last.base(), result.base()), 1);
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
                return output_iterator(transformer_impl_.transform(first, last, result.base()), 1);
            }
            else
            {
                return transformer_impl_.transform(first, last, result);
            }
        }
    }

private:
    transformer_impl transformer_impl_;
};

template<
    template<typename, typename, typename>
    class SampleConverter,
    typename InputSampleTraits,
    typename OutputSampleTraits,
    typename DitherGenerator>
class basic_transformer<
    SampleConverter,
    channel_iterator<InputSampleTraits, false>,
    channel_iterator<OutputSampleTraits, true>,
    DitherGenerator>
{
    using input_iterator = channel_iterator<InputSampleTraits, false>;
    using output_iterator = channel_iterator<OutputSampleTraits, true>;

    using base_input_sample_type = std::remove_cv_t<typename InputSampleTraits::sample_type>;
    using base_output_sample_type = std::remove_cv_t<typename OutputSampleTraits::sample_type>;

    using transformer_impl =
        basic_transformer_impl<SampleConverter, base_input_sample_type, base_output_sample_type, DitherGenerator>;

public:
    explicit basic_transformer(DitherGenerator& dither_gen) : transformer_impl_(dither_gen) {}

    inline output_iterator operator()(input_iterator first, input_iterator last, output_iterator result) const noexcept
    {
        if (first.stride() == 1)
        {
            return output_iterator(transformer_impl_.transform(first.base(), last.base(), result.base()));
        }
        else
        {
            return output_iterator(transformer_impl_.transform(first, last, result.base()));
        }
    }

private:
    transformer_impl transformer_impl_;
};

template<
    template<typename, typename, typename>
    class SampleConverter,
    typename InputSampleTraits,
    typename OutputSampleTraits,
    typename DitherGenerator>
class basic_transformer<
    SampleConverter,
    channel_iterator<InputSampleTraits, true>,
    channel_iterator<OutputSampleTraits, false>,
    DitherGenerator>
{
    using input_iterator = channel_iterator<InputSampleTraits, true>;
    using output_iterator = channel_iterator<OutputSampleTraits, false>;

    using base_input_sample_type = std::remove_cv_t<typename InputSampleTraits::sample_type>;
    using base_output_sample_type = std::remove_cv_t<typename OutputSampleTraits::sample_type>;

    using transformer_impl =
        basic_transformer_impl<SampleConverter, base_input_sample_type, base_output_sample_type, DitherGenerator>;

public:
    explicit basic_transformer(DitherGenerator& dither_gen) : transformer_impl_(dither_gen) {}

    inline output_iterator operator()(input_iterator first, input_iterator last, output_iterator result) const noexcept
    {
        if (result.stride() == 1)
        {
            return output_iterator(transformer_impl_.transform(first.base(), last.base(), result.base()), 1);
        }
        else
        {
            return transformer_impl_.transform(first.base(), last.base(), result);
        }
    }

private:
    transformer_impl transformer_impl_;
};

template<
    template<typename, typename, typename>
    class SampleConverter,
    typename InputSampleTraits,
    typename OutputSampleTraits,
    typename DitherGenerator>
class basic_transformer<
    SampleConverter,
    channel_iterator<InputSampleTraits, true>,
    channel_iterator<OutputSampleTraits, true>,
    DitherGenerator>
{
    using input_iterator = channel_iterator<InputSampleTraits, true>;
    using output_iterator = channel_iterator<OutputSampleTraits, true>;

    using base_input_sample_type = std::remove_cv_t<typename InputSampleTraits::sample_type>;
    using base_output_sample_type = std::remove_cv_t<typename OutputSampleTraits::sample_type>;

    using transformer_impl =
        basic_transformer_impl<SampleConverter, base_input_sample_type, base_output_sample_type, DitherGenerator>;

public:
    explicit basic_transformer(DitherGenerator& dither_gen) : transformer_impl_(dither_gen) {}

    inline output_iterator operator()(input_iterator first, input_iterator last, output_iterator result) const noexcept
    {
        return output_iterator(transformer_impl_.transform(first.base(), last.base(), result.base()));
    }

private:
    transformer_impl transformer_impl_;
};

template<typename InputIterator, typename OutputIterator, typename DitherGenerator>
using reference_transformer =
    basic_transformer<reference_sample_converter, InputIterator, OutputIterator, DitherGenerator>;

template<typename InputIterator, typename OutputIterator, typename DitherGenerator>
using fast_transformer = basic_transformer<fast_sample_converter, InputIterator, OutputIterator, DitherGenerator>;

template<typename InputIterator, typename OutputIterator, typename DitherGenerator>
using default_transformer = basic_transformer<default_sample_converter, InputIterator, OutputIterator, DitherGenerator>;

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_transformer_
