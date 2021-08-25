/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_endianness_
#define _ratl_detail_endianness_

// ratl includes
#include <ratl/detail/cast.hpp>
#include <ratl/detail/config.hpp>

// other includes
#include <type_traits>

namespace ratl
{
namespace detail
{
// reverse_endianness

inline uint16_t reverse_endianness(uint16_t input) noexcept
{
#if defined(RATL_CPP_COMPILER_MSVC)
    return _byteswap_ushort(input);
#elif defined(RATL_CPP_COMPILER_CLANG) || defined(RATL_CPP_COMPILER_GCC)
    return __builtin_bswap16(input);
#endif
}

inline uint32_t reverse_endianness(uint32_t input) noexcept
{
#if defined(RATL_CPP_COMPILER_MSVC)
    return _byteswap_ulong(input);
#elif defined(RATL_CPP_COMPILER_CLANG) || defined(RATL_CPP_COMPILER_GCC)
    return __builtin_bswap32(input);
#endif
}

inline uint24_t reverse_endianness(uint24_t input) noexcept
{
    return static_cast<uint24_t>(reverse_endianness(static_cast<uint32_t>(input) << 8));
}

// host_to_network

template<class network_sample_value_underlying_type>
inline network_sample_value_underlying_type host_to_network(network_sample_value_underlying_type input) noexcept
{
#if defined(RATL_CPP_LITTLE_ENDIAN)
    return reverse_endianness(input);
#else
    return input;
#endif
}

// network_to_host

template<class network_sample_value_underlying_type>
inline network_sample_value_underlying_type network_to_host(network_sample_value_underlying_type input) noexcept
{
#if defined(RATL_CPP_LITTLE_ENDIAN)
    return reverse_endianness(input);
#else
    return input;
#endif
}

// sample_to_network_sample

template<class SampleValueType>
inline network_sample_value_type_t<SampleValueType> sample_to_network_sample(SampleValueType input) noexcept
{
    return network_underlying_to_network_cast<SampleValueType>(
        host_to_network(sample_to_network_underlying_cast<SampleValueType>(input)));
}

// network_sample_to_sample

template<class SampleValueType>
inline SampleValueType network_sample_to_sample(network_sample_value_type_t<SampleValueType> input) noexcept
{
    return network_underlying_to_sample_cast<SampleValueType>(
        network_to_host(network_to_network_underlying_cast<SampleValueType>(input)));
}

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_endianness_
