/**
 * Copyright (c) 2018-2022 Hamish Cook
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
namespace detail
{
template<template<typename, typename, typename> class Transformer, typename InputIterator, typename OutputIterator>
inline OutputIterator transform_impl(
    InputIterator first, InputIterator last, OutputIterator result, dither_generator& dither_gen)
{
    using transformer = Transformer<InputIterator, OutputIterator, dither_generator>;
    return transformer(dither_gen)(first, last, result);
}

template<template<typename, typename, typename> class Transformer, typename InputIterator, typename OutputIterator>
inline OutputIterator transform_impl(InputIterator first, InputIterator last, OutputIterator result)
{
    using transformer_dither_generator = batch_null_dither_generator;
    using transformer = Transformer<InputIterator, OutputIterator, transformer_dither_generator>;
    transformer_dither_generator dither_gen;
    return transformer(dither_gen)(first, last, result);
}
} // namespace detail

// transform

template<typename InputIterator, typename OutputIterator, typename... Args>
inline OutputIterator transform(InputIterator first, InputIterator last, OutputIterator result, Args&&... args)
{
    return detail::transform_impl<detail::default_transformer, InputIterator, OutputIterator>(
        first, last, result, std::forward<Args>(args)...);
}

// reference_transform

template<typename InputIterator, typename OutputIterator, typename... Args>
inline OutputIterator reference_transform(
    InputIterator first, InputIterator last, OutputIterator result, Args&&... args)
{
    return detail::transform_impl<detail::reference_transformer, InputIterator, OutputIterator>(
        first, last, result, std::forward<Args>(args)...);
}

// fast_transform

template<typename InputIterator, typename OutputIterator, typename... Args>
inline OutputIterator fast_transform(InputIterator first, InputIterator last, OutputIterator result, Args&&... args)
{
    return detail::transform_impl<detail::fast_transformer, InputIterator, OutputIterator>(
        first, last, result, std::forward<Args>(args)...);
}

} // namespace ratl

#endif // _ratl_transform_
