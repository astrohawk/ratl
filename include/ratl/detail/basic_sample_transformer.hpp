#ifndef _ratl_detail_basic_sample_transformer_
#define _ratl_detail_basic_sample_transformer_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/convert.hpp>
#include <ratl/detail/config.hpp>

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
struct BasicSampleTransformer
{
private:
    using SampleConverter = SampleConverterT<InputSample, OutputSample, DitherGenerator>;

public:
    explicit BasicSampleTransformer(DitherGenerator& dither_generator) : sample_converter_{dither_generator} {}

    template<typename InputIterator, typename OutputIterator>
    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
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

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_basic_sample_transformer_
