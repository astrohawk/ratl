/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_transform_
#define _ratl_transform_

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/transformer.hpp>
#include <ratl/dither_generator.hpp>

// other includes
#include <type_traits>

namespace ratl
{
// transform

template<typename InputIterator, typename OutputIterator, typename Transformer>
inline OutputIterator transform(InputIterator first, InputIterator last, OutputIterator result, Transformer transformer)
{
    return transformer(first, last, result);
}

namespace detail
{
template<template<typename, typename, typename> class Transformer, typename InputIterator, typename OutputIterator>
inline OutputIterator transform_impl(
    InputIterator first, InputIterator last, OutputIterator result, dither_generator& dither_gen)
{
    using transformer = Transformer<InputIterator, OutputIterator, dither_generator>;
    return ratl::transform(first, last, result, transformer(dither_gen));
}

template<template<typename, typename, typename> class Transformer, typename InputIterator, typename OutputIterator>
inline OutputIterator transform_impl(InputIterator first, InputIterator last, OutputIterator result)
{
    using transformer_dither_generator = batch_null_dither_generator;
    using transformer = Transformer<InputIterator, OutputIterator, transformer_dither_generator>;
    transformer_dither_generator dither_gen;
    return ratl::transform(first, last, result, transformer(dither_gen));
}
} // namespace detail

// transform

template<typename InputIterator, typename OutputIterator>
inline OutputIterator transform(
    InputIterator first, InputIterator last, OutputIterator result, dither_generator& dither_gen)
{
    return detail::transform_impl<detail::default_transformer, InputIterator, OutputIterator>(
        first, last, result, dither_gen);
}

template<typename InputIterator, typename OutputIterator>
inline OutputIterator transform(InputIterator first, InputIterator last, OutputIterator result)
{
    return detail::transform_impl<detail::default_transformer, InputIterator, OutputIterator>(first, last, result);
}

// reference_transform

template<typename InputIterator, typename OutputIterator>
inline OutputIterator reference_transform(
    InputIterator first, InputIterator last, OutputIterator result, dither_generator& dither_gen)
{
    return detail::transform_impl<detail::reference_transformer, InputIterator, OutputIterator>(
        first, last, result, dither_gen);
}

template<typename InputIterator, typename OutputIterator>
inline OutputIterator reference_transform(InputIterator first, InputIterator last, OutputIterator result)
{
    return detail::transform_impl<detail::reference_transformer, InputIterator, OutputIterator>(first, last, result);
}

// fast_transform

template<typename InputIterator, typename OutputIterator>
inline OutputIterator fast_transform(
    InputIterator first, InputIterator last, OutputIterator result, dither_generator& dither_gen)
{
    return detail::transform_impl<detail::fast_transformer, InputIterator, OutputIterator>(
        first, last, result, dither_gen);
}

template<typename InputIterator, typename OutputIterator>
inline OutputIterator fast_transform(InputIterator first, InputIterator last, OutputIterator result)
{
    return detail::transform_impl<detail::fast_transformer, InputIterator, OutputIterator>(first, last, result);
}

} // namespace ratl

#endif // _ratl_transform_
