#ifndef _ratl_batch_sample_transform_
#define _ratl_batch_sample_transform_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/convert.hpp>
#include <ratl/detail/batch_convert.hpp>
#include <ratl/detail/batch_creator.hpp>
#include <ratl/detail/batch_dither_generator.hpp>
#include <ratl/detail/channel_iterator.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/detail/frame_iterator.hpp>
#include <ratl/detail/interleaved_iterator.hpp>
#include <ratl/detail/sample_transformer.hpp>

namespace ratl
{
namespace detail
{
#if defined(RATL_HAS_XSIMD)

template<class InputSample, class OutputSample, class DitherGenerator>
struct BatchSampleTransformerImpl
{
private:
    using InputSampleValueType = typename InputSample::value_type;
    using OuputSampleValueType = typename OutputSample::value_type;

    using InputBatchCreator = BatchCreator<InputSample>;
    using OutputBatchCreator = BatchCreator<OutputSample>;

    using InputBatchType = typename InputBatchCreator::batch_type;
    using OutputBatchType = typename OutputBatchCreator::batch_type;

    static constexpr std::size_t BatchSize = InputBatchType::size;
    static_assert(BatchSize == OutputBatchType::size, "Input and output batch size are not equal");

    using BatchConverter = DefaultBatchConverter<InputSample, OutputSample, DitherGenerator>;
    using SampleTransformer = SampleTransformerImpl<InputSample, OutputSample, DitherGenerator>;

public:
    explicit BatchSampleTransformerImpl(DitherGenerator& dither_generator) : dither_generator_{dither_generator} {}

    template<typename InputIterator, typename OutputIterator>
    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        std::size_t distance = std::distance(first, last);
        std::size_t vec_distance = distance - distance % BatchSize;
        InputIterator vec_last = first + vec_distance;
        for (; first < vec_last; first += BatchSize, result += BatchSize)
        {
            InputBatchType input = InputBatchCreator::load_unaligned(first);
            OutputBatchType output = convert(input, BatchConverter{dither_generator_});
            OutputBatchCreator::store_unaligned(output, result);
        }
        return SampleTransformer{dither_generator_}(first, last, result);
    }

private:
    DitherGenerator& dither_generator_;
};

template<class Sample, class DitherGenerator>
struct BatchSampleTransformerImpl<Sample, Sample, DitherGenerator>
{
private:
    using SampleTransformer = SampleTransformerImpl<Sample, Sample, DitherGenerator>;

public:
    explicit BatchSampleTransformerImpl(DitherGenerator& dither_generator) : sample_transformer_{dither_generator} {}

    template<typename InputIterator, typename OutputIterator>
    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        return sample_transformer_(first, last, result);
    }

private:
    SampleTransformer sample_transformer_;
};

#else

template<class InputSample, class OutputSample, class DitherGenerator>
using BatchSampleTransformerImpl = SampleTransformerImpl<InputSample, OutputSample, DitherGenerator>;

#endif
} // namespace detail
} // namespace ratl

#endif // _ratl_batch_sample_transform_
