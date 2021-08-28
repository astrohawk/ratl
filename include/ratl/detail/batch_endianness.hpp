/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_batch_endianness_
#define _ratl_detail_batch_endianness_

// ratl includes
#include <ratl/detail/batch_cast.hpp>
#include <ratl/detail/batch_sample_fix.hpp>
#include <ratl/detail/batch_traits.hpp>
#include <ratl/detail/config.hpp>

// other includes
#include <type_traits>

#if defined(RATL_HAS_XSIMD)

namespace ratl
{
namespace detail
{
// batch_reverse_endianness_impl

template<typename NetworkSampleValueUnderlyingType>
struct batch_reverse_endianness_impl;

template<>
struct batch_reverse_endianness_impl<uint16_t>
{
#    if XSIMD_X86_INSTR_SET >= XSIMD_X86_SSE3_VERSION
    static inline xsimd::batch<std::uint32_t, 4> reverse(const xsimd::batch<std::uint32_t, 4>& input) noexcept
    {
        static const __m128i mask = _mm_set_epi8(-1, -1, 12, 13, -1, -1, 8, 9, -1, -1, 4, 5, -1, -1, 0, 1);
        return _mm_shuffle_epi8(input, mask);
    }
#    endif

#    if XSIMD_X86_INSTR_SET >= XSIMD_X86_SSE3_VERSION
    static inline xsimd::batch<std::uint16_t, 8> reverse(const xsimd::batch<std::uint16_t, 8>& input) noexcept
    {
        static const __m128i mask = _mm_set_epi8(14, 15, 12, 13, 10, 11, 8, 9, 6, 7, 4, 5, 2, 3, 0, 1);
        return _mm_shuffle_epi8(input, mask);
    }
#    endif

#    if XSIMD_X86_INSTR_SET >= XSIMD_X86_AVX2_VERSION
    static inline xsimd::batch<std::uint16_t, 16> reverse(const xsimd::batch<std::uint16_t, 16>& input) noexcept
    {
        // clang-format off
        static const __m256i mask = _mm256_set_epi8(
            30, 31, 28, 29, 26, 27, 24, 25, 22, 23, 20, 21, 18, 19, 16, 17,
            14, 15, 12, 13, 10, 11,  8,  9,  6,  7,  4,  5,  2,  3,  0,  1);
        // clang-format on
        return _mm256_shuffle_epi8(input, mask);
    }
#    endif

#    if XSIMD_ARM_INSTR_SET >= XSIMD_ARM7_NEON_VERSION
    static inline xsimd::batch<std::uint32_t, 4> reverse(const xsimd::batch<std::uint32_t, 4>& input) noexcept
    {
        return xsimd::bitwise_cast<xsimd::batch<std::uint32_t, 4>>(
            xsimd::batch<std::uint8_t, 16>(vrev16q_u8(xsimd::bitwise_cast<xsimd::batch<std::uint8_t, 16>>(input))));
    }
#    endif

    template<class BatchNetworkSampleValueType>
    static inline BatchNetworkSampleValueType reverse(const BatchNetworkSampleValueType& input) noexcept
    {
        return ((input & 0xff00) >> 8) | ((input & 0x00ff) << 8);
    }
};

template<>
struct batch_reverse_endianness_impl<uint24_t>
{
#    if XSIMD_X86_INSTR_SET >= XSIMD_X86_SSE3_VERSION
    static inline xsimd::batch<std::uint32_t, 4> reverse(const xsimd::batch<std::uint32_t, 4>& input) noexcept
    {
        static const __m128i mask = _mm_set_epi8(-1, 12, 13, 14, -1, 8, 9, 10, -1, 4, 5, 6, -1, 0, 1, 2);
        return _mm_shuffle_epi8(input, mask);
    }
#    endif

#    if XSIMD_X86_INSTR_SET >= XSIMD_X86_AVX2_VERSION
    static inline xsimd::batch<std::uint32_t, 8> reverse(const xsimd::batch<std::uint32_t, 8>& input) noexcept
    {
        // clang-format off
        static const __m256i mask = _mm256_set_epi8(
            -1, 28, 29, 30, -1, 24, 25, 26, -1, 20, 21, 22, -1, 16, 17, 18,
            -1, 12, 13, 14, -1,  8,  9, 10, -1,  4,  5,  6, -1,  0,  1,  2);
        // clang-format on
        return _mm256_shuffle_epi8(input, mask);
    }
#    endif

#    if XSIMD_X86_INSTR_SET >= XSIMD_X86_AVX512_VERSION && defined(XSIMD_AVX512DQ_AVAILABLE)
    static inline xsimd::batch<std::uint32_t, 16> reverse(const xsimd::batch<std::uint32_t, 16>& input) noexcept
    {
        // clang-format off
        static const __m512i mask = _mm512_set_epi8(
            -1, 60, 61, 62, -1, 56, 57, 58, -1, 52, 53, 54, -1, 48, 49, 50,
            -1, 44, 45, 46, -1, 40, 41, 42, -1, 36, 37, 38, -1, 32, 33, 34,
            -1, 28, 29, 30, -1, 24, 25, 26, -1, 20, 21, 22, -1, 16, 17, 18,
            -1, 12, 13, 14, -1,  8,  9, 10, -1,  4,  5,  6, -1,  0,  1,  2);
        // clang-format on
        return _mm512_shuffle_epi8(input, mask);
    }
#    endif

#    if XSIMD_ARM_INSTR_SET >= XSIMD_ARM7_NEON_VERSION
    static inline xsimd::batch<std::uint32_t, 4> reverse(const xsimd::batch<std::uint32_t, 4>& input) noexcept
    {
        return xsimd::bitwise_cast<xsimd::batch<std::uint32_t, 4>>(xsimd::batch<std::uint8_t, 16>(
            vrev32q_u8(xsimd::bitwise_cast<xsimd::batch<std::uint8_t, 16>>(input << 8))));
    }
#    endif

    template<class BatchNetworkSampleValueType>
    static inline BatchNetworkSampleValueType reverse(const BatchNetworkSampleValueType& input) noexcept
    {
        return ((input & 0xff0000) >> 16) | (input & 0x00ff00) | ((input & 0x0000ff) << 16);
    }
};

template<>
struct batch_reverse_endianness_impl<uint32_t>
{
#    if XSIMD_X86_INSTR_SET >= XSIMD_X86_SSE3_VERSION
    static inline xsimd::batch<std::uint32_t, 4> reverse(const xsimd::batch<std::uint32_t, 4>& input) noexcept
    {
        static const __m128i mask = _mm_set_epi8(12, 13, 14, 15, 8, 9, 10, 11, 4, 5, 6, 7, 0, 1, 2, 3);
        return _mm_shuffle_epi8(input, mask);
    }
#    endif

#    if XSIMD_X86_INSTR_SET >= XSIMD_X86_AVX2_VERSION
    static inline xsimd::batch<std::uint32_t, 8> reverse(const xsimd::batch<std::uint32_t, 8>& input) noexcept
    {
        // clang-format off
        static const __m256i mask = _mm256_set_epi8(
            28, 29, 30, 31, 24, 25, 26, 27, 20, 21, 22, 23, 16, 17, 18, 19,
            12, 13, 14, 15,  8,  9, 10, 11,  4,  5,  6,  7,  0,  1,  2,  3);
        // clang-format on
        return _mm256_shuffle_epi8(input, mask);
    }
#    endif

#    if XSIMD_X86_INSTR_SET >= XSIMD_X86_AVX512_VERSION && defined(XSIMD_AVX512DQ_AVAILABLE)
    static inline xsimd::batch<std::uint32_t, 16> reverse(const xsimd::batch<std::uint32_t, 16>& input) noexcept
    {
        // clang-format off
        static const __m512i mask = _mm512_set_epi8(
            60, 61, 62, 63, 56, 57, 58, 59, 52, 53, 54, 55, 48, 49, 50, 51,
            44, 45, 46, 47, 40, 41, 42, 43, 36, 37, 38, 39, 32, 33, 34, 35,
            28, 29, 30, 31, 24, 25, 26, 27, 20, 21, 22, 23, 16, 17, 18, 19,
            12, 13, 14, 15,  8,  9, 10, 11,  4,  5,  6,  7,  0,  1,  2,  3);
        // clang-format on
        return _mm512_shuffle_epi8(input, mask);
    }
#    endif

#    if XSIMD_ARM_INSTR_SET >= XSIMD_ARM7_NEON_VERSION
    static inline xsimd::batch<std::uint32_t, 4> reverse(const xsimd::batch<std::uint32_t, 4>& input) noexcept
    {
        return xsimd::bitwise_cast<xsimd::batch<std::uint32_t, 4>>(
            xsimd::batch<std::uint8_t, 16>(vrev32q_u8(xsimd::bitwise_cast<xsimd::batch<std::uint8_t, 16>>(input))));
    }
#    endif

    template<class BatchNetworkSampleValueType>
    static inline BatchNetworkSampleValueType reverse(const BatchNetworkSampleValueType& input) noexcept
    {
        return ((input & 0xff000000) >> 24) | ((input & 0x00ff0000) >> 8) | ((input & 0x0000ff00) << 8) |
               ((input & 0x000000ff) << 24);
    }
};

// batch_reverse_endianness

template<typename NetworkSampleValueUnderlyingType, typename BatchNetworkSampleValueType>
inline BatchNetworkSampleValueType batch_reverse_endianness(const BatchNetworkSampleValueType& input) noexcept
{
    return batch_reverse_endianness_impl<NetworkSampleValueUnderlyingType>::reverse(input);
}

// batch_host_to_network

template<typename NetworkSampleValueUnderlyingType, typename BatchNetworkSampleValueType>
inline BatchNetworkSampleValueType batch_host_to_network(const BatchNetworkSampleValueType& input) noexcept
{
#    if defined(RATL_CPP_LITTLE_ENDIAN)
    return batch_reverse_endianness<NetworkSampleValueUnderlyingType>(input);
#    else
    return input;
#    endif
}

// batch_network_to_host

template<typename NetworkSampleValueUnderlyingType, typename BatchNetworkSampleValueType>
inline BatchNetworkSampleValueType batch_network_to_host(const BatchNetworkSampleValueType& input) noexcept
{
#    if defined(RATL_CPP_LITTLE_ENDIAN)
    return batch_reverse_endianness<NetworkSampleValueUnderlyingType>(input);
#    else
    return input;
#    endif
}

// batch_sample_to_network_sample

template<typename SampleValueType>
inline batch_network_sample_value_type_t<SampleValueType> batch_sample_to_network_sample(
    const batch_sample_value_type_t<SampleValueType>& input) noexcept
{
    return batch_host_to_network<network_sample_value_underlying_type_t<SampleValueType>>(
        xsimd::bitwise_cast<batch_network_sample_value_type_t<SampleValueType>>(input));
}

// batch_network_sample_to_sample

template<typename SampleValueType>
inline batch_sample_value_type_t<SampleValueType> batch_network_sample_to_sample(
    const batch_network_sample_value_type_t<SampleValueType>& input) noexcept
{
    return batch_fix_negative_samples<SampleValueType>(xsimd::bitwise_cast<batch_sample_value_type_t<SampleValueType>>(
        batch_network_to_host<network_sample_value_underlying_type_t<SampleValueType>>(input)));
}

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_detail_batch_endianness_
