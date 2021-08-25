#ifndef _ratl_detail_batch_round_
#define _ratl_detail_batch_round_

// C++ Standard Library includes

// ratl includes
#include <ratl/detail/xsimd.hpp>

#if defined(RATL_HAS_XSIMD)

namespace ratl
{
namespace detail
{
#    if XSIMD_X86_INSTR_SET >= XSIMD_X86_SSE2_VERSION
inline xsimd::batch<int32_t, 4> batch_round_float32_to_int32(const xsimd::batch<float, 4>& input)
{
    return _mm_cvtps_epi32(input);
}
#    endif

#    if XSIMD_ARM_INSTR_SET >= XSIMD_ARM8_32_NEON_VERSION
inline xsimd::batch<int32_t, 4> batch_round_float32_to_int32(const xsimd::batch<float, 4>& input)
{
    return vcvtnq_s32_f32(input);
}
#    elif XSIMD_ARM_INSTR_SET >= XSIMD_ARM7_NEON_VERSION
inline xsimd::batch<int32_t, 4> batch_round_float32_to_int32(const xsimd::batch<float, 4>& input)
{
    static const float32x4_t zero_val_dup = vdupq_n_f32(0.0f);
    static const float32x4_t point5_val_dup = vdupq_n_f32(0.5f);
    static const float32x4_t minus_point5_val_dup = vdupq_n_f32(-0.5f);

    const uint32x4_t mask = vcltq_f32(input, zero_val_dup);
    const float32x4_t round = vbslq_f32(mask, minus_point5_val_dup, point5_val_dup);
    return vcvtq_s32_f32(vaddq_f32(x, round));
}
#    endif

#    if XSIMD_X86_INSTR_SET >= XSIMD_X86_AVX_VERSION
inline xsimd::batch<int32_t, 8> batch_round_float32_to_int32(const xsimd::batch<float, 8>& input)
{
    return _mm256_cvtps_epi32(input);
}
#    endif

#    if XSIMD_X86_INSTR_SET >= XSIMD_X86_AVX512_VERSION && defined(XSIMD_AVX512DQ_AVAILABLE)
inline xsimd::batch<int32_t, 16> batch_round_float32_to_int32(const xsimd::batch<float, 16>& input)
{
    return _mm512_cvtps_epi32(input);
}
#    endif
} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_detail_batch_round_
