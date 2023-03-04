/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_utility_
#define _ratl_detail_utility_

// ratl includes
#include <ratl/detail/config.hpp>

// other includes
#include <type_traits>
#if defined(RATL_CPP_VERSION_HAS_CPP17)
#    include <functional>
#endif

namespace ratl
{
namespace detail
{
// apply_binary_op function

template<typename InputIterator, typename OutputIterator, typename BinaryOp>
inline OutputIterator apply_binary_op(
    InputIterator first1, InputIterator last1, OutputIterator first2, BinaryOp binary_op)
{
    for (; first1 != last1; ++first1, (void)++first2)
    {
        binary_op(*first1, *first2);
    }
    return first2;
}

// invoke function

#if defined(RATL_CPP_VERSION_HAS_CPP17)
template<class Fn, class... Args>
inline decltype(auto) invoke(Fn&& f, Args&&... args) noexcept(
    noexcept(std::invoke(std::forward<Fn>(f), std::forward<Args>(args)...)))
{
    return std::invoke(std::forward<Fn>(f), std::forward<Args>(args)...);
}
#else
template<class Fn, class... Args>
inline decltype(auto) invoke(Fn&& f, Args&&... args) noexcept(
    noexcept(std::forward<Fn>(f)(std::forward<Args>(args)...)))
{
    return std::forward<Fn>(f)(std::forward<Args>(args)...);
}
#endif
} // namespace detail
} // namespace ratl

#endif // _ratl_detail_utility_
