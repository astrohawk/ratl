/**
 * Copyright (c) 2018-2021 Hamish Cook
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

template<typename SampleType>
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
    using pointer = sample_type*;
    using const_pointer = const_sample_type*;
    using reference = sample_type&;
    using const_reference = const_sample_type&;

    template<typename OtherSampleType>
    using rebind = sample_traits<OtherSampleType>;
};

// sample_pointer_select

template<typename SampleType, typename PointerTraits>
struct sample_pointer_select :
    std::conditional<
        std::is_const<SampleType>::value,
        typename PointerTraits::const_pointer,
        typename PointerTraits::pointer>
{
};

// sample_pointer_select_t

template<typename SampleType, typename PointerTraits>
using sample_pointer_select_t = typename sample_pointer_select<SampleType, PointerTraits>::type;

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_sample_traits_
