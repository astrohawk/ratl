#ifndef _ratl_detail_batch_basic_sample_transformer_
#define _ratl_detail_batch_basic_sample_transformer_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/detail/basic_sample_transformer.hpp>
#include <ratl/detail/batch_creator.hpp>
#include <ratl/detail/config.hpp>

namespace ratl
{
namespace detail
{
#if defined(RATL_HAS_XSIMD)

template<
    template<class, class, class>
    class BatchSampleConverterT,
    class InputSample,
    class OutputSample,
    class DitherGenerator>
class BatchBasicSampleTransformer
{
private:
    using InputBatchCreator = BatchCreator<InputSample>;
    using OutputBatchCreator = BatchCreator<OutputSample>;

    using InputBatchType = typename InputBatchCreator::batch_type;
    using OutputBatchType = typename OutputBatchCreator::batch_type;

    static constexpr std::size_t BatchSize = InputBatchType::size;
    static_assert(BatchSize == OutputBatchType::size, "Input and output batch size are not equal");

    using BatchSampleConverter = BatchSampleConverterT<InputSample, OutputSample, DitherGenerator>;
    using SampleTransformer = BasicSampleTransformer<BatchSampleConverterT, InputSample, OutputSample, DitherGenerator>;

public:
    explicit BatchBasicSampleTransformer(DitherGenerator& dither_generator) : dither_generator_{dither_generator} {}

    template<typename InputIterator, typename OutputIterator>
    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        std::size_t distance = std::distance(first, last);
        std::size_t vec_distance = distance - distance % BatchSize;
        InputIterator vec_last = first + vec_distance;
        auto batch_converter = BatchSampleConverter{dither_generator_};
        for (; first != vec_last; first += BatchSize, result += BatchSize)
        {
            auto input = InputBatchCreator::load_unaligned(first);
            auto output = convert(input, batch_converter);
            OutputBatchCreator::store_unaligned(output, result);
        }
        return SampleTransformer{dither_generator_}(first, last, result);
    }

private:
    std::reference_wrapper<DitherGenerator> dither_generator_;
};

template<template<class, class, class> class BatchSampleConverterT, class Sample, class DitherGenerator>
class BatchBasicSampleTransformer<BatchSampleConverterT, Sample, Sample, DitherGenerator> :
    public BasicSampleTransformer<BatchSampleConverterT, Sample, Sample, DitherGenerator>
{
    using BatchBasicSampleTransformer::BasicSampleTransformer::BasicSampleTransformer;
};

#else

template<
    template<class, class, class>
    class SampleConverterT,
    class InputSample,
    class OutputSample,
    class DitherGenerator>
using BatchBasicSampleTransformer =
    BasicSampleTransformer<SampleConverterT, InputSample, OutputSample, DitherGenerator>;

#endif
} // namespace detail
} // namespace ratl

#endif // _ratl_detail_batch_basic_sample_transformer_
