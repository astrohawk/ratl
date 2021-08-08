#ifndef _ratl_convert_
#define _ratl_convert_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/sample_converter.hpp>
#include <ratl/dither_generator.hpp>

namespace ratl
{
// convert

template<class InputSample, class SampleConverter>
inline auto convert(const InputSample& sample, SampleConverter sample_converter) -> decltype(sample_converter(sample))
{
    return sample_converter(sample);
}

namespace detail
{
template<template<class, class, class> class SampleConverterT, class OutputSample, class InputSample>
inline OutputSample convertImpl(const InputSample& sample, DitherGenerator& dither_generator) noexcept
{
    using SampleConverter = DefaultSampleConverter<InputSample, OutputSample, DitherGenerator>;
    return convert(sample, SampleConverter{dither_generator});
}

template<template<class, class, class> class SampleConverterT, class OutputSample, class InputSample>
inline OutputSample convertImpl(const InputSample& sample) noexcept
{
    using ConverterDitherGenerator = detail::NullDitherGenerator;
    using SampleConverter = SampleConverterT<InputSample, OutputSample, ConverterDitherGenerator>;
    ConverterDitherGenerator dither_generator{};
    return convert(sample, SampleConverter{dither_generator});
}
} // namespace detail

// convert

template<class OutputSample, class InputSample>
inline OutputSample convert(const InputSample& sample, DitherGenerator& dither_generator) noexcept
{
    return detail::convertImpl<detail::DefaultSampleConverter, OutputSample, InputSample>(sample, dither_generator);
}

template<class OutputSample, class InputSample>
inline OutputSample convert(const InputSample& sample) noexcept
{
    return detail::convertImpl<detail::DefaultSampleConverter, OutputSample, InputSample>(sample);
}

// referenceConvert

template<class OutputSample, class InputSample>
inline OutputSample referenceConvert(const InputSample& sample, DitherGenerator& dither_generator) noexcept
{
    return detail::convertImpl<detail::ReferenceSampleConverter, OutputSample, InputSample>(sample, dither_generator);
}

template<class OutputSample, class InputSample>
inline OutputSample referenceConvert(const InputSample& sample) noexcept
{
    return detail::convertImpl<detail::ReferenceSampleConverter, OutputSample, InputSample>(sample);
}

// fastConvert

template<class OutputSample, class InputSample>
inline OutputSample fastConvert(const InputSample& sample, DitherGenerator& dither_generator) noexcept
{
    return detail::convertImpl<detail::FastSampleConverter, OutputSample, InputSample>(sample, dither_generator);
}

template<class OutputSample, class InputSample>
inline OutputSample fastConvert(const InputSample& sample) noexcept
{
    return detail::convertImpl<detail::FastSampleConverter, OutputSample, InputSample>(sample);
}

} // namespace ratl

#endif // _ratl_convert_
