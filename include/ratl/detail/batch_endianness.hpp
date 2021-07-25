#ifndef _ratl_detail_batch_endianness_
#define _ratl_detail_batch_endianness_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/detail/batch_sample_fix.hpp>
#include <ratl/detail/batch_traits.hpp>
#include <ratl/detail/config.hpp>

#if defined(RATL_HAS_XSIMD)

namespace ratl
{
namespace detail
{
// BatchReverseEndiannessImpl

template<class NetworkSampleValueUnderlyingType>
struct BatchReverseEndiannessImpl;

template<>
struct BatchReverseEndiannessImpl<uint16_t>
{
#    if XSIMD_X86_INSTR_SET >= XSIMD_X86_SSE3_VERSION
    static inline xsimd::batch<uint32_t, 4> reverse(const xsimd::batch<uint32_t, 4>& input) noexcept
    {
        static const __m128i mask = _mm_set_epi8(-1, -1, 12, 13, -1, -1, 8, 9, -1, -1, 4, 5, -1, -1, 0, 1);
        return _mm_shuffle_epi8(input, mask);
    }
#    endif

#    if XSIMD_X86_INSTR_SET >= XSIMD_X86_SSE3_VERSION
    static inline xsimd::batch<uint16_t, 8> reverse(const xsimd::batch<uint16_t, 8>& input) noexcept
    {
        static const __m128i mask = _mm_set_epi8(14, 15, 12, 13, 10, 11, 8, 9, 6, 7, 4, 5, 2, 3, 0, 1);
        return _mm_shuffle_epi8(input, mask);
    }
#    endif

#    if XSIMD_X86_INSTR_SET >= XSIMD_X86_AVX2_VERSION
    static inline xsimd::batch<uint16_t, 16> reverse(const xsimd::batch<uint16_t, 16>& input) noexcept
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
    static inline xsimd::batch<uint32_t, 4> reverse(const xsimd::batch<uint32_t, 4>& input) noexcept
    {
        return xsimd::bitwise_cast<xsimd::batch<uint32_t, 4>>(
            xsimd::batch<uint8_t, 16>(vrev16q_u8(xsimd::bitwise_cast<xsimd::batch<uint8_t, 16>>(input))));
    }
#    endif
};

template<>
struct BatchReverseEndiannessImpl<uint24_t>
{
#    if XSIMD_X86_INSTR_SET >= XSIMD_X86_SSE3_VERSION
    static inline xsimd::batch<uint32_t, 4> reverse(const xsimd::batch<uint32_t, 4>& input) noexcept
    {
        static const __m128i mask = _mm_set_epi8(-1, 12, 13, 14, -1, 8, 9, 10, -1, 4, 5, 6, -1, 0, 1, 2);
        return _mm_shuffle_epi8(input, mask);
    }
#    endif

#    if XSIMD_X86_INSTR_SET >= XSIMD_X86_AVX2_VERSION
    static inline xsimd::batch<uint32_t, 8> reverse(const xsimd::batch<uint32_t, 8>& input) noexcept
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
    static inline xsimd::batch<uint32_t, 16> reverse(const xsimd::batch<uint32_t, 16>& input) noexcept
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
    static inline xsimd::batch<uint32_t, 4> reverse(const xsimd::batch<uint32_t, 4>& input) noexcept
    {
        return xsimd::bitwise_cast<xsimd::batch<uint32_t, 4>>(
            xsimd::batch<uint8_t, 16>(vrev32q_u8(xsimd::bitwise_cast<xsimd::batch<uint8_t, 16>>(input << 8))));
    }
#    endif
};

template<>
struct BatchReverseEndiannessImpl<uint32_t>
{
#    if XSIMD_X86_INSTR_SET >= XSIMD_X86_SSE3_VERSION
    static inline xsimd::batch<uint32_t, 4> reverse(const xsimd::batch<uint32_t, 4>& input) noexcept
    {
        static const __m128i mask = _mm_set_epi8(12, 13, 14, 15, 8, 9, 10, 11, 4, 5, 6, 7, 0, 1, 2, 3);
        return _mm_shuffle_epi8(input, mask);
    }
#    endif

#    if XSIMD_X86_INSTR_SET >= XSIMD_X86_AVX2_VERSION
    static inline xsimd::batch<uint32_t, 8> reverse(const xsimd::batch<uint32_t, 8>& input) noexcept
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
    static inline xsimd::batch<uint32_t, 16> reverse(const xsimd::batch<uint32_t, 16>& input) noexcept
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
    static inline xsimd::batch<uint32_t, 4> reverse(const xsimd::batch<uint32_t, 4>& input) noexcept
    {
        return xsimd::bitwise_cast<xsimd::batch<uint32_t, 4>>(
            xsimd::batch<uint8_t, 16>(vrev32q_u8(xsimd::bitwise_cast<xsimd::batch<uint8_t, 16>>(input))));
    }
#    endif
};

// batchReverseEndianness

template<class NetworkSampleValueUnderlyingType, class BatchNetworkSampleValueType>
inline BatchNetworkSampleValueType batchReverseEndianness(const BatchNetworkSampleValueType& input) noexcept
{
    return BatchReverseEndiannessImpl<NetworkSampleValueUnderlyingType>::reverse(input);
}

// batchHostToNetwork

template<class NetworkSampleValueUnderlyingType, class BatchNetworkSampleValueType>
inline BatchNetworkSampleValueType batchHostToNetwork(const BatchNetworkSampleValueType& input) noexcept
{
#    if defined(RATL_CPP_LITTLE_ENDIAN)
    return batchReverseEndianness<NetworkSampleValueUnderlyingType>(input);
#    else
    return input;
#    endif
}

// batchNetworkToHost

template<class NetworkSampleValueUnderlyingType, class BatchNetworkSampleValueType>
inline BatchNetworkSampleValueType batchNetworkToHost(const BatchNetworkSampleValueType& input) noexcept
{
#    if defined(RATL_CPP_LITTLE_ENDIAN)
    return batchReverseEndianness<NetworkSampleValueUnderlyingType>(input);
#    else
    return input;
#    endif
}

// BatchSampleToNetworkSampleImpl

template<class SampleType>
struct BatchSampleToNetworkSampleImpl
{
    static inline BatchNetworkSampleValueType_t<SampleType> convert(
        const BatchSampleValueType_t<SampleType>& input) noexcept
    {
        return batchHostToNetwork<NetworkSampleValueUnderlyingType_t<SampleType>>(
            xsimd::batch_cast<typename BatchNetworkSampleValueType_t<SampleType>::value_type>(input));
    }
};

template<>
struct BatchSampleToNetworkSampleImpl<float32_t>
{
    static inline BatchNetworkSampleValueType_t<float32_t> convert(
        const BatchSampleValueType_t<float32_t>& input) noexcept
    {
        return batchHostToNetwork<NetworkSampleValueUnderlyingType_t<float32_t>>(
            xsimd::bitwise_cast<BatchNetworkSampleValueType_t<float32_t>>(input));
    }
};

// batchSampleToNetworkSample

template<class SampleType>
inline BatchNetworkSampleValueType_t<SampleType> batchSampleToNetworkSample(
    const BatchSampleValueType_t<SampleType>& input) noexcept
{
    return BatchSampleToNetworkSampleImpl<SampleType>::convert(input);
}

// BatchNetworkSampleToSampleImpl

template<class SampleType>
struct BatchNetworkSampleToSampleImpl
{
    static inline BatchSampleValueType_t<SampleType> convert(
        const BatchNetworkSampleValueType_t<SampleType>& input) noexcept
    {
        return batchFixNegativeSamples<SampleType>(
            xsimd::batch_cast<typename BatchSampleValueType_t<SampleType>::value_type>(
                batchNetworkToHost<NetworkSampleValueUnderlyingType_t<SampleType>>(input)));
    }
};

template<>
struct BatchNetworkSampleToSampleImpl<float32_t>
{
    static inline BatchSampleValueType_t<float32_t> convert(
        const BatchNetworkSampleValueType_t<float32_t>& input) noexcept
    {
        return xsimd::bitwise_cast<BatchSampleValueType_t<float32_t>>(
            batchNetworkToHost<NetworkSampleValueUnderlyingType_t<float32_t>>(input));
    }
};

// batchNetworkSampleToSample

template<class SampleType>
inline BatchSampleValueType_t<SampleType> batchNetworkSampleToSample(
    const BatchNetworkSampleValueType_t<SampleType>& input) noexcept
{
    return BatchNetworkSampleToSampleImpl<SampleType>::convert(input);
}

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_detail_batch_endianness_
