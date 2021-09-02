/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_cast_
#define _ratl_detail_cast_

// ratl includes
#include <ratl/detail/sample_value_traits.hpp>

// other includes
#include <cassert>
#include <cstring>
#include <type_traits>
#if defined(RATL_CPP_VERSION_HAS_CPP20)
#    include <bit>
#endif

namespace ratl
{
namespace detail
{
template<typename OutputType, typename InputType>
inline typename std::enable_if_t<
    sizeof(OutputType) == sizeof(InputType) && std::is_trivially_copyable<InputType>::value &&
        std::is_trivially_copyable<OutputType>::value,
    OutputType>
bit_cast(const InputType& input) noexcept
{
    static_assert(
        std::is_trivially_constructible<OutputType>::value, "destination type must be trivially constructible");

#if defined(RATL_CPP_VERSION_HAS_CPP20)
    return std::bit_cast<OutputType>(input);
#else
    OutputType output;
    std::memcpy(&output, &input, sizeof(OutputType));
    return output;
#endif
}

// narrowing_cast

template<typename OutputType>
struct narrowing_cast_impl;

template<>
struct narrowing_cast_impl<int16_t>
{
    static inline int16_t cast(int16_t input) noexcept
    {
        return input;
    }

    static inline int16_t cast(int24_t input) noexcept
    {
        return static_cast<int16_t>(input);
    }

    static inline int16_t cast(int32_t input) noexcept
    {
        return static_cast<int16_t>(input);
    }
};

template<>
struct narrowing_cast_impl<uint16_t>
{
    static inline uint16_t cast(uint16_t input) noexcept
    {
        return input;
    }

    static inline uint16_t cast(uint24_t input) noexcept
    {
        return static_cast<uint16_t>(input);
    }

    static inline uint16_t cast(uint32_t input) noexcept
    {
        return static_cast<uint16_t>(input);
    }
};

template<>
struct narrowing_cast_impl<int24_t>
{
    static inline int24_t cast(int24_t input) noexcept
    {
        return input;
    }

    static inline int24_t cast(int32_t input) noexcept
    {
#if defined(RATL_USE_INT24_MEMCPY_NARROWING_CAST)
        // memcpy is required instead of just casting input to output as msvc is very bad at optimising the constexpr
        // int24_t operator=(int32_t). This is solved in C++20 as we can use `if (std::is_constant_evaluated())` to do
        // the constexpr shifting if in a constexpr context and use memcpy otherwise.
        int24_t temp;
        std::memcpy(&temp, &input, sizeof(int24_t));
        return temp;
#else
        return static_cast<int24_t>(input);
#endif
    }
};

template<>
struct narrowing_cast_impl<uint24_t>
{
    static inline uint24_t cast(uint24_t input) noexcept
    {
        return input;
    }

    static inline uint24_t cast(uint32_t input) noexcept
    {
#if defined(RATL_USE_INT24_MEMCPY_NARROWING_CAST)
        // memcpy is required instead of just casting input to output as msvc is very bad at optimising the constexpr
        // int24_t operator=(int32_t). This is solved in C++20 as we can use `if (std::is_constant_evaluated())` to do
        // the constexpr shifting if in a constexpr context and use memcpy otherwise.
        uint24_t temp;
        std::memcpy(&temp, &input, sizeof(uint24_t));
        return temp;
#else
        return static_cast<uint24_t>(input);
#endif
    }
};

template<>
struct narrowing_cast_impl<int32_t>
{
    static inline int32_t cast(int32_t input) noexcept
    {
        return input;
    }
};

template<>
struct narrowing_cast_impl<uint32_t>
{
    static inline uint32_t cast(uint32_t input) noexcept
    {
        return input;
    }
};

template<>
struct narrowing_cast_impl<float32_t>
{
    static inline float32_t cast(float32_t input) noexcept
    {
        return input;
    }
};

template<typename OutputType, typename InputType>
inline OutputType narrowing_cast(InputType input) noexcept
{
    auto output = narrowing_cast_impl<OutputType>::cast(input);
    assert(static_cast<InputType>(output) == input);
    return output;
}

// sample_to_network_underlying_cast

template<typename SampleValueType>
inline network_sample_value_underlying_type_t<SampleValueType> sample_to_network_underlying_cast(
    SampleValueType input) noexcept
{
    return bit_cast<network_sample_value_underlying_type_t<SampleValueType>>(input);
}

// network_underlying_to_sample_cast

template<typename SampleValueType>
inline SampleValueType network_underlying_to_sample_cast(
    network_sample_value_underlying_type_t<SampleValueType> input) noexcept
{
    return bit_cast<SampleValueType>(input);
}

// network_to_network_underlying_cast

template<typename SampleValueType>
inline network_sample_value_underlying_type_t<SampleValueType> network_to_network_underlying_cast(
    network_sample_value_type_t<SampleValueType> input) noexcept
{
    return bit_cast<network_sample_value_underlying_type_t<SampleValueType>>(input);
}

// network_underlying_to_network_cast

template<typename SampleValueType>
inline network_sample_value_type_t<SampleValueType> network_underlying_to_network_cast(
    network_sample_value_underlying_type_t<SampleValueType> input) noexcept
{
    return bit_cast<network_sample_value_type_t<SampleValueType>>(input);
}

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_cast_
