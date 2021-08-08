#ifndef _ratl_detail_sample_transformer_
#define _ratl_detail_sample_transformer_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/detail/batch_creator.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/detail/sample_converter.hpp>

namespace ratl
{
namespace detail
{
template<
    template<class, class, class>
    class SampleConverterT,
    class InputSample,
    class OutputSample,
    class DitherGenerator>
class BasicSampleTransformer
{
private:
#if defined(RATL_HAS_XSIMD)
    using InputBatchCreator = BatchCreator<InputSample>;
    using OutputBatchCreator = BatchCreator<OutputSample>;

    using InputBatchType = typename InputBatchCreator::batch_type;
    using OutputBatchType = typename OutputBatchCreator::batch_type;

    static constexpr std::size_t BatchSize = InputBatchType::size;
    static_assert(BatchSize == OutputBatchType::size, "Input and output batch size are not equal");
#endif

    using SampleConverter = SampleConverterT<InputSample, OutputSample, DitherGenerator>;

public:
    explicit BasicSampleTransformer(DitherGenerator& dither_generator) : sample_converter_{dither_generator} {}

    template<typename InputIterator, typename OutputIterator>
    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
#if defined(RATL_HAS_XSIMD)
        std::size_t distance = std::distance(first, last);
        std::size_t vec_distance = distance - distance % BatchSize;
        InputIterator vec_last = first + vec_distance;
        for (; first != vec_last; first += BatchSize, result += BatchSize)
        {
            auto input = InputBatchCreator::load_unaligned(first);
            auto output = convert(input, sample_converter_);
            OutputBatchCreator::store_unaligned(output, result);
        }
#endif
        return std::transform(first, last, result, sample_converter_);
    }

private:
    SampleConverter sample_converter_;
};

template<template<class, class, class> class SampleConverterT, class Sample, class DitherGenerator>
struct BasicSampleTransformer<SampleConverterT, Sample, Sample, DitherGenerator>
{
    explicit BasicSampleTransformer(DitherGenerator&) {}

    template<typename InputIterator, typename OutputIterator>
    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        return std::copy(first, last, result);
    }
};

template<class InputSample, class OutputSample, class DitherGenerator>
using ReferenceSampleTransformer =
    BasicSampleTransformer<ReferenceSampleConverter, InputSample, OutputSample, DitherGenerator>;

template<class InputSample, class OutputSample, class DitherGenerator>
using FastSampleTransformer = BasicSampleTransformer<FastSampleConverter, InputSample, OutputSample, DitherGenerator>;

template<class InputSample, class OutputSample, class DitherGenerator>
using DefaultSampleTransformer =
    BasicSampleTransformer<DefaultSampleConverter, InputSample, OutputSample, DitherGenerator>;

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_sample_transformer_
