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
template<class SampleType>
struct is_sample_impl : public std::false_type
{
};

template<class SampleValueType>
struct is_sample_impl<sample<SampleValueType>> : public std::true_type
{
};

template<class SampleValueType>
struct is_sample_impl<network_sample<SampleValueType>> : public std::true_type
{
};

// is_sample

template<class SampleType>
struct is_sample : public detail::is_sample_impl<std::remove_cv_t<SampleType>>
{
};

// is_sample_v

template<class SampleType>
static constexpr bool is_sample_v = is_sample<SampleType>::value;

// sample_traits

template<class SampleType>
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
     * sample_type
     * The underlying type that this sample is representing,
     * i.e. int16_t, int24_t, int32_t, float32_t
     */
    using sample_value_type = typename SampleType::sample_value_type;

    /*
     * value_type
     * The type of the underlying representation in memory. This will be the same
     * as sample_type for ratl::sample, but will be an opaque type for
     * ratl::network_sample.
     */
    using underlying_type = typename sample_type::value_type;

    /*
     * pointer, const_pointer, reference and const_reference
     * A (const) pointer/reference to the sample.
     */
    using pointer = sample_type*;
    using const_pointer = const_sample_type*;
    using reference = sample_type&;
    using const_reference = const_sample_type&;
};

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_sample_traits_
