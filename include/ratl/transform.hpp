#ifndef _ratl_transform_
#define _ratl_transform_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/transformer.hpp>
#include <ratl/dither_generator.hpp>

namespace ratl
{
// transform

template<class InputIterator, class OutputIterator, class Transformer>
inline OutputIterator transform(InputIterator first, InputIterator last, OutputIterator result, Transformer transformer)
{
    return transformer(first, last, result);
}

namespace detail
{
template<template<class, class, class> class TransformerT, class InputIterator, class OutputIterator>
inline OutputIterator transformImpl(
    InputIterator first, InputIterator last, OutputIterator result, DitherGenerator& dither_generator)
{
    using Transformer = TransformerT<InputIterator, OutputIterator, DitherGenerator>;
    return transform(first, last, result, Transformer{dither_generator});
}

template<template<class, class, class> class TransformerT, class InputIterator, class OutputIterator>
inline OutputIterator transformImpl(InputIterator first, InputIterator last, OutputIterator result)
{
    using TransformerDitherGenerator = BatchNullDitherGenerator;
    using Transformer = TransformerT<InputIterator, OutputIterator, TransformerDitherGenerator>;
    TransformerDitherGenerator dither_generator{};
    return transform(first, last, result, Transformer{dither_generator});
}
} // namespace detail

// transform

template<class InputIterator, class OutputIterator>
inline OutputIterator transform(
    InputIterator first, InputIterator last, OutputIterator result, DitherGenerator& dither_generator)
{
    return detail::transformImpl<detail::DefaultTransformer, InputIterator, OutputIterator>(
        first, last, result, dither_generator);
}

template<class InputIterator, class OutputIterator>
inline OutputIterator transform(InputIterator first, InputIterator last, OutputIterator result)
{
    return detail::transformImpl<detail::DefaultTransformer, InputIterator, OutputIterator>(first, last, result);
}

// referenceTransform

template<class InputIterator, class OutputIterator>
inline OutputIterator referenceTransform(
    InputIterator first, InputIterator last, OutputIterator result, DitherGenerator& dither_generator)
{
    return detail::transformImpl<detail::ReferenceTransformer, InputIterator, OutputIterator>(
        first, last, result, dither_generator);
}

template<class InputIterator, class OutputIterator>
inline OutputIterator referenceTransform(InputIterator first, InputIterator last, OutputIterator result)
{
    return detail::transformImpl<detail::ReferenceTransformer, InputIterator, OutputIterator>(first, last, result);
}

// fastTransform

template<class InputIterator, class OutputIterator>
inline OutputIterator fastTransform(
    InputIterator first, InputIterator last, OutputIterator result, DitherGenerator& dither_generator)
{
    return detail::transformImpl<detail::FastTransformer, InputIterator, OutputIterator>(
        first, last, result, dither_generator);
}

template<class InputIterator, class OutputIterator>
inline OutputIterator fastTransform(InputIterator first, InputIterator last, OutputIterator result)
{
    return detail::transformImpl<detail::FastTransformer, InputIterator, OutputIterator>(first, last, result);
}

} // namespace ratl

#endif // _ratl_transform_
