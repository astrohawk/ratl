#ifndef _ratl_detail_sample_transform_
#define _ratl_detail_sample_transform_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/convert.hpp>
#include <ratl/detail/config.hpp>

namespace ratl
{
namespace detail
{
template<class SampleConverter, class InputSample, class OutputSample, class DitherGenerator>
struct SampleTransformerImpl
{
private:
    using Converter = DefaultConverter<SampleConverter, InputSample, OutputSample, DitherGenerator>;

public:
    explicit SampleTransformerImpl(DitherGenerator& dither_generator) : converter_{dither_generator} {}

    template<typename InputIterator, typename OutputIterator>
    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        return std::transform(first, last, result, converter_);
    }

private:
    Converter converter_;
};

template<class SampleConverter, class Sample, class DitherGenerator>
struct SampleTransformerImpl<SampleConverter, Sample, Sample, DitherGenerator>
{
    explicit SampleTransformerImpl(DitherGenerator&) {}

    template<typename InputIterator, typename OutputIterator>
    inline OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) const noexcept
    {
        return std::copy(first, last, result);
    }
};

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_sample_transform_
