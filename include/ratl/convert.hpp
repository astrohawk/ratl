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

template<typename InputSampleType, typename SampleConverter>
inline auto convert(const InputSampleType& sample, SampleConverter sample_converter)
    -> decltype(sample_converter(sample))
{
    return sample_converter(sample);
}

namespace detail
{
template<
    template<typename, typename, typename>
    class SampleConverter,
    typename OutputSampleType,
    typename InputSampleType>
inline OutputSampleType convert_impl(const InputSampleType& sample) noexcept
{
    using converter_dither_generator = detail::null_dither_generator;
    using sample_converter = SampleConverter<InputSampleType, OutputSampleType, converter_dither_generator>;
    converter_dither_generator dither_gen;
    return ratl::convert(sample, sample_converter(dither_gen));
}

template<
    template<typename, typename, typename>
    class SampleConverter,
    typename OutputSampleType,
    typename InputSampleType>
inline OutputSampleType convert_impl(const InputSampleType& sample, dither_generator& dither_gen) noexcept
{
    using sample_converter = SampleConverter<InputSampleType, OutputSampleType, dither_generator>;
    return ratl::convert(sample, sample_converter(dither_gen));
}
} // namespace detail

// convert

template<typename OutputSampleType, typename InputSampleType>
inline std::remove_cv_t<OutputSampleType> convert(const InputSampleType& sample) noexcept
{
    return detail::convert_impl<
        detail::default_sample_converter,
        std::remove_cv_t<OutputSampleType>,
        std::remove_cv_t<InputSampleType>>(sample);
}

template<typename OutputSampleType, typename InputSampleType>
inline std::remove_cv_t<OutputSampleType> convert(const InputSampleType& sample, dither_generator& dither_gen) noexcept
{
    return detail::convert_impl<
        detail::default_sample_converter,
        std::remove_cv_t<OutputSampleType>,
        std::remove_cv_t<InputSampleType>>(sample, dither_gen);
}

// reference_convert

template<typename OutputSampleType, typename InputSampleType>
inline std::remove_cv_t<OutputSampleType> reference_convert(const InputSampleType& sample) noexcept
{
    return detail::convert_impl<
        detail::reference_sample_converter,
        std::remove_cv_t<OutputSampleType>,
        std::remove_cv_t<InputSampleType>>(sample);
}

template<typename OutputSampleType, typename InputSampleType>
inline std::remove_cv_t<OutputSampleType> reference_convert(
    const InputSampleType& sample, dither_generator& dither_gen) noexcept
{
    return detail::convert_impl<
        detail::reference_sample_converter,
        std::remove_cv_t<OutputSampleType>,
        std::remove_cv_t<InputSampleType>>(sample, dither_gen);
}

// fast_convert

template<typename OutputSampleType, typename InputSampleType>
inline std::remove_cv_t<OutputSampleType> fast_convert(const InputSampleType& sample) noexcept
{
    return detail::convert_impl<
        detail::fast_sample_converter,
        std::remove_cv_t<OutputSampleType>,
        std::remove_cv_t<InputSampleType>>(sample);
}

template<typename OutputSampleType, typename InputSampleType>
inline std::remove_cv_t<OutputSampleType> fast_convert(
    const InputSampleType& sample, dither_generator& dither_gen) noexcept
{
    return detail::convert_impl<
        detail::fast_sample_converter,
        std::remove_cv_t<OutputSampleType>,
        std::remove_cv_t<InputSampleType>>(sample, dither_gen);
}

} // namespace ratl

#endif // _ratl_convert_
