/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_sample_traits_
#define _ratl_detail_sample_traits_

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/sample_value_traits.hpp>
#include <ratl/network_sample.hpp>
#include <ratl/sample.hpp>

// other includes
#include <type_traits>

namespace ratl
{
namespace detail
{
template<typename SampleType>
struct is_sample_impl : public std::false_type
{
};

template<typename SampleValueType>
struct is_sample_impl<sample<SampleValueType>> : public std::true_type
{
};

template<typename SampleValueType>
struct is_sample_impl<network_sample<SampleValueType>> : public std::true_type
{
};

// is_sample

template<typename SampleType>
struct is_sample : public detail::is_sample_impl<std::remove_cv_t<SampleType>>
{
};

// is_sample_v

template<typename SampleType>
static constexpr bool is_sample_v = is_sample<SampleType>::value;

// sample_traits

template<typename SampleType, typename PointerType = SampleType*, typename ConstPointerType = const SampleType*>
struct sample_traits
{
    static_assert(is_sample_v<SampleType>, "sample is not a valid sample type");

    /*
     * sample and const_sample
     * The type of the sample this trait represents,
     * e.g. ratl::sample<XXX> or ratl::network_sample<XXX>
     */
    using sample_type = SampleType;
    using const_sample_type = const SampleType;

    /*
     * pointer, const_pointer, reference and const_reference
     * A (const) pointer/reference to the sample.
     */
    using pointer = PointerType;
    using const_pointer = ConstPointerType;
    using reference = sample_type&;
    using const_reference = const_sample_type&;
};

template<typename AllocatorTraits>
using sample_traits_from_alloc_traits_t = sample_traits<
    typename AllocatorTraits::value_type,
    typename AllocatorTraits::pointer,
    typename AllocatorTraits::const_pointer>;

template<typename SampleTraits>
using const_sample_traits_t = sample_traits<
    typename SampleTraits::const_sample_type,
    typename SampleTraits::const_pointer,
    typename SampleTraits::const_pointer>;

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_sample_traits_
