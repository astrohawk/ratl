/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_type_traits_
#define _ratl_detail_type_traits_

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
#if defined(RATL_CPP_VERSION_HAS_CPP17)
template<typename... Ts>
using void_t = std::void_t<Ts...>;
#else
template<typename...>
struct void_impl
{
    using type = void;
};

/*
 * std::void_t reimplementation for C++14.
 * Supports CWG 1558
 */
template<typename... Ts>
using void_t = typename detail::void_impl<Ts...>::type;
#endif

// is_complete

template<typename Tp, typename = void>
struct is_complete : std::false_type
{
};

template<typename Tp>
struct is_complete<Tp, decltype(void(sizeof(Tp)))> : std::true_type
{
};

// is_complete_v

template<typename Tp>
static constexpr bool is_complete_v = is_complete<Tp>::value;

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_type_traits_
