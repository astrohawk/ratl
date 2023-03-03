/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_batch_round_
#define _ratl_detail_batch_round_

// ratl includes
#include <ratl/detail/xsimd.hpp>

// other includes
#include <cstdint>

#if defined(RATL_HAS_XSIMD)

namespace ratl
{
namespace detail
{
// batch_round_impl

template<typename InputSampleValueType, typename OutputSampleValueType>
struct batch_round_impl;

template<>
struct batch_round_impl<float, std::int32_t>
{
#    if XSIMD_X86_INSTR_SET >= XSIMD_X86_SSE2_VERSION
    static inline xsimd::batch<std::int32_t, 4> round(const xsimd::batch<float, 4>& input)
    {
        return _mm_cvtps_epi32(input);
    }
#    endif

#    if XSIMD_ARM_INSTR_SET >= XSIMD_ARM8_64_NEON_VERSION
    static inline xsimd::batch<std::int32_t, 4> round(const xsimd::batch<float, 4>& input)
    {
        return vcvtnq_s32_f32(input);
    }
#    elif XSIMD_ARM_INSTR_SET >= XSIMD_ARM7_NEON_VERSION
#        if defined(RATL_CPP_VERSION_HAS_CPP17)
private:
    inline static const float32x4_t zero_val_dup = vdupq_n_f32(0.0f);
    inline static const float32x4_t point5_val_dup = vdupq_n_f32(0.5f);
    inline static const float32x4_t minus_point5_val_dup = vdupq_n_f32(-0.5f);

public:
#        endif
    static inline xsimd::batch<std::int32_t, 4> round(const xsimd::batch<float, 4>& input)
    {
#        if !defined(RATL_CPP_VERSION_HAS_CPP17)
        static const float32x4_t zero_val_dup = vdupq_n_f32(0.0f);
        static const float32x4_t point5_val_dup = vdupq_n_f32(0.5f);
        static const float32x4_t minus_point5_val_dup = vdupq_n_f32(-0.5f);
#        endif

        const uint32x4_t mask = vcltq_f32(input, zero_val_dup);
        const float32x4_t round = vbslq_f32(mask, minus_point5_val_dup, point5_val_dup);
        return vcvtq_s32_f32(vaddq_f32(input, round));
    }
#    endif

#    if XSIMD_X86_INSTR_SET >= XSIMD_X86_AVX_VERSION
    static inline xsimd::batch<std::int32_t, 8> round(const xsimd::batch<float, 8>& input)
    {
        return _mm256_cvtps_epi32(input);
    }
#    endif

#    if XSIMD_X86_INSTR_SET >= XSIMD_X86_AVX512_VERSION && defined(XSIMD_AVX512DQ_AVAILABLE)
    static inline xsimd::batch<std::int32_t, 16> round(const xsimd::batch<float, 16>& input)
    {
        return _mm512_cvtps_epi32(input);
    }
#    endif
};

// batch_round

template<typename OutputSampleValueType, typename InputSampleValueType, std::size_t BatchSize>
inline xsimd::batch<OutputSampleValueType, BatchSize> batch_round(
    const xsimd::batch<InputSampleValueType, BatchSize>& input) noexcept
{
    return batch_round_impl<InputSampleValueType, OutputSampleValueType>::round(input);
}

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_detail_batch_round_
