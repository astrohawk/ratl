/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_convert_
#define _ratl_convert_

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/sample_converter.hpp>
#include <ratl/dither_generator.hpp>

// other includes
#include <type_traits>

namespace ratl
{
// convert

template<class InputSampleType, class SampleConverter>
inline auto convert(const InputSampleType& sample, SampleConverter sample_converter)
    -> decltype(sample_converter(sample))
{
    return sample_converter(sample);
}

namespace detail
{
template<template<class, class, class> class SampleConverter, class OutputSampleType, class InputSampleType>
inline OutputSampleType convert_impl(const InputSampleType& sample, dither_generator& dither_gen) noexcept
{
    using sample_converter = SampleConverter<InputSampleType, OutputSampleType, dither_generator>;
    return convert(sample, sample_converter(dither_gen));
}

template<template<class, class, class> class SampleConverter, class OutputSampleType, class InputSampleType>
inline OutputSampleType convert_impl(const InputSampleType& sample) noexcept
{
    using converter_dither_generator = detail::null_dither_generator;
    using sample_converter = SampleConverter<InputSampleType, OutputSampleType, converter_dither_generator>;
    converter_dither_generator dither_gen;
    return convert(sample, sample_converter(dither_gen));
}
} // namespace detail

// convert

template<class OutputSampleType, class InputSampleType>
inline OutputSampleType convert(const InputSampleType& sample, dither_generator& dither_gen) noexcept
{
    return detail::convert_impl<detail::default_sample_converter, OutputSampleType, InputSampleType>(
        sample, dither_gen);
}

template<class OutputSampleType, class InputSampleType>
inline OutputSampleType convert(const InputSampleType& sample) noexcept
{
    return detail::convert_impl<detail::default_sample_converter, OutputSampleType, InputSampleType>(sample);
}

// reference_convert

template<class OutputSampleType, class InputSampleType>
inline OutputSampleType reference_convert(const InputSampleType& sample, dither_generator& dither_gen) noexcept
{
    return detail::convert_impl<detail::reference_sample_converter, OutputSampleType, InputSampleType>(
        sample, dither_gen);
}

template<class OutputSampleType, class InputSampleType>
inline OutputSampleType reference_convert(const InputSampleType& sample) noexcept
{
    return detail::convert_impl<detail::reference_sample_converter, OutputSampleType, InputSampleType>(sample);
}

// fast_convert

template<class OutputSampleType, class InputSampleType>
inline OutputSampleType fast_convert(const InputSampleType& sample, dither_generator& dither_gen) noexcept
{
    return detail::convert_impl<detail::fast_sample_converter, OutputSampleType, InputSampleType>(sample, dither_gen);
}

template<class OutputSampleType, class InputSampleType>
inline OutputSampleType fast_convert(const InputSampleType& sample) noexcept
{
    return detail::convert_impl<detail::fast_sample_converter, OutputSampleType, InputSampleType>(sample);
}

} // namespace ratl

#endif // _ratl_convert_
