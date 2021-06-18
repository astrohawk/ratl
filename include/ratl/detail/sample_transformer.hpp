#ifndef _ratl_detail_sample_transform_
#define _ratl_detail_sample_transform_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/convert.hpp>
#include <ratl/detail/batch_dither_generator.hpp>
#include <ratl/detail/config.hpp>

namespace ratl
{
namespace detail
{
template<class InputSample, class OutputSample, class DitherGenerator>
struct SampleTransformerImpl
{
private:
    using InputSampleValueType = typename InputSample::value_type;
    using OuputSampleValueType = typename OutputSample::value_type;

    using Converter = DefaultConverter<InputSample, OutputSample, DitherGenerator>;

public:
    explicit SampleTransformerImpl(DitherGenerator& dither_generator) : converter_{dither_generator} {}

    inline OuputSampleValueType* operator()(
        InputSampleValueType* first, InputSampleValueType* last, OuputSampleValueType* result) const noexcept
    {
        return std::transform(first, last, result, converter_);
    }

    template<typename InputIterator>
    inline OuputSampleValueType* operator()(
        InputIterator first, InputIterator last, OuputSampleValueType* result) const noexcept
    {
        return std::transform(
            first,
            last,
            result,
            [&](const InputSample& input)
            {
                return converter_(input.get());
            });
    }

    template<typename OutputIterator>
    inline OutputIterator operator()(
        InputSampleValueType* first, InputSampleValueType* last, OutputIterator result) const noexcept
    {
        return std::transform(
            first,
            last,
            result,
            [&](const InputSampleValueType& input)
            {
                return OutputSample{converter_(input)};
            });
    }

    template<typename InputIterator, typename OutputIterator>
    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        return std::transform(first, last, result, converter_);
    }

private:
    Converter converter_;
};

template<class Sample, class DitherGenerator>
struct SampleTransformerImpl<Sample, Sample, DitherGenerator>
{
private:
    using SampleValueType = typename Sample::value_type;

public:
    explicit SampleTransformerImpl(DitherGenerator&) {}

    inline SampleValueType* operator()(
        SampleValueType* first, SampleValueType* last, SampleValueType* result) const noexcept
    {
        return std::copy(first, last, result);
    }

    template<typename InputIterator>
    inline SampleValueType* operator()(InputIterator first, InputIterator last, SampleValueType* result) const noexcept
    {
        return std::transform(
            first,
            last,
            result,
            [](const Sample& input)
            {
                return input.get();
            });
    }

    template<typename OutputIterator>
    inline OutputIterator operator()(
        SampleValueType* first, SampleValueType* last, OutputIterator result) const noexcept
    {
        return std::transform(
            first,
            last,
            result,
            [](const SampleValueType& input)
            {
                return Sample{input};
            });
    }

    template<typename InputIterator, typename OutputIterator>
    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        return std::copy(first, last, result);
    }
};

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_sample_transform_
