/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_test_type_utils_
#define _ratl_test_type_utils_

// other includes
#include <gtest/gtest.h>

namespace ratl
{
namespace test
{
template<typename... Types>
struct testing_types_concat
{
    using type = ::testing::Types<Types...>;
};

template<>
struct testing_types_concat<>
{
    using type = ::testing::Types<>;
};

template<typename... Types>
struct testing_types_concat<::testing::Types<Types...>>
{
    using type = ::testing::Types<Types...>;
};

template<typename... FirstTypes, typename... SecondTypes, typename... RemainderTypes>
struct testing_types_concat<::testing::Types<FirstTypes...>, ::testing::Types<SecondTypes...>, RemainderTypes...>
{
    using type =
        typename testing_types_concat<::testing::Types<FirstTypes..., SecondTypes...>, RemainderTypes...>::type;
};

template<typename... Types>
using testing_types_concat_t = typename testing_types_concat<Types...>::type;

template<template<typename...> class OuterType, typename... InnerTypes>
struct testing_types_combine;

template<template<typename...> class OuterType>
struct testing_types_combine<OuterType>
{
    using type = OuterType<>;
};

template<template<typename...> class OuterType, typename... FirstInnerTypes>
struct testing_types_combine<OuterType, ::testing::Types<FirstInnerTypes...>>
{
    using type = testing_types_concat_t<OuterType<FirstInnerTypes>...>;
};

template<template<typename...> class OuterType, typename... FirstInnerTypes, typename... RemainderInnerTypes>
struct testing_types_combine<OuterType, ::testing::Types<FirstInnerTypes...>, RemainderInnerTypes...>
{
    template<typename... TempRemainderInnerTypes>
    using temp_type = testing_types_concat_t<OuterType<FirstInnerTypes, TempRemainderInnerTypes...>...>;

    using type = typename testing_types_combine<temp_type, RemainderInnerTypes...>::type;
};

template<template<typename...> class OuterType, typename... InnerTypes>
using testing_types_combine_t = typename testing_types_combine<OuterType, InnerTypes...>::type;

} // namespace test
} // namespace ratl

#endif
