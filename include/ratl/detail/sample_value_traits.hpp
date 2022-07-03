/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_sample_value_traits_
#define _ratl_detail_sample_value_traits_

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/sample_value_traits.hpp>
#include <ratl/types.hpp>

// other includes
#include <array>
#include <type_traits>

namespace ratl
{
namespace detail
{
// is_valid_sample_value_type_impl

template<typename SampleValueType>
struct is_valid_sample_value_type_impl : public std::false_type
{
};
template<>
struct is_valid_sample_value_type_impl<int16_t> : public std::true_type
{
};
template<>
struct is_valid_sample_value_type_impl<int24_t> : public std::true_type
{
};
template<>
struct is_valid_sample_value_type_impl<int32_t> : public std::true_type
{
};
template<>
struct is_valid_sample_value_type_impl<float32_t> : public std::true_type
{
};

// is_valid_sample_value_type

template<typename SampleValueType>
struct is_valid_sample_value_type : public detail::is_valid_sample_value_type_impl<SampleValueType>
{
};

// is_valid_sample_value_type_v

#if defined(RATL_CPP_VERSION_HAS_CPP17)
template<typename SampleValueType>
inline constexpr bool is_valid_sample_value_type_v = is_valid_sample_value_type<SampleValueType>::value;
#endif

// network_sample_value_underlying_type

template<typename SampleValueType>
struct network_sample_value_underlying_type;

template<>
struct network_sample_value_underlying_type<int16_t>
{
    using type = std::uint16_t;
};

template<>
struct network_sample_value_underlying_type<int24_t>
{
    using type = ratl::uint24_t;
};

template<>
struct network_sample_value_underlying_type<int32_t>
{
    using type = std::uint32_t;
};

template<>
struct network_sample_value_underlying_type<float32_t>
{
    using type = std::uint32_t;
};

// network_sample_value_underlying_type_t

template<typename SampleValueType>
using network_sample_value_underlying_type_t = typename network_sample_value_underlying_type<SampleValueType>::type;

// network_sample_value_type

template<typename SampleValueType>
struct network_sample_value_type
{
    static_assert(is_valid_sample_value_type<SampleValueType>::value, "sample_type is not a valid sample type");

    // opaque type
    struct type final
    {
#if defined(RATL_CPP_VERSION_HAS_CPP20)
        inline constexpr bool operator==(const type& other) const noexcept = default;

        inline constexpr bool operator!=(const type& other) const noexcept = default;
#else
        friend inline constexpr bool operator==(const type& a, const type& b) noexcept
        {
            return a.storage_ == b.storage_;
        }

        friend inline constexpr bool operator!=(const type& a, const type& b) noexcept
        {
            return a.storage_ != b.storage_;
        }
#endif

        network_sample_value_underlying_type_t<SampleValueType> storage_;
    };
};

// network_sample_value_type_t

template<typename SampleValueType>
using network_sample_value_type_t = typename network_sample_value_type<SampleValueType>::type;

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_sample_value_traits_
