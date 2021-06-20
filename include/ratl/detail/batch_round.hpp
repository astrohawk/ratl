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
inline xsimd::batch<int32_t, 4> batchRoundFloat32ToInt32(const xsimd::batch<float, 4>& x)
{
    return _mm_cvtps_epi32(x);
}
#    endif

#    if XSIMD_X86_INSTR_SET >= XSIMD_X86_AVX_VERSION
inline xsimd::batch<int32_t, 8> batchRoundFloat32ToInt32(const xsimd::batch<float, 8>& x)
{
    return _mm256_cvtps_epi32(x);
}
#    endif

#    if XSIMD_X86_INSTR_SET >= XSIMD_X86_AVX512_VERSION
inline xsimd::batch<int32_t, 16> batchRoundFloat32ToInt32(const xsimd::batch<float, 16>& x)
{
    return _mm512_cvtps_epi32(x);
}
#    endif

#    if XSIMD_ARM_INSTR_SET >= XSIMD_ARM8_32_NEON_VERSION
inline xsimd::batch<int32_t, 4> batchRoundFloat32ToInt32(const xsimd::batch<float, 4>& x)
{
    return vcvtnq_s32_f32(x);
}
#    elif XSIMD_ARM_INSTR_SET >= XSIMD_ARM7_NEON_VERSION
inline xsimd::batch<int32_t, 4> batchRoundFloat32ToInt32(const xsimd::batch<float, 4>& x)
{
    static const float32x4_t zero_val_dup = vdupq_n_f32(0.0f);
    static const float32x4_t point5_val_dup = vdupq_n_f32(0.5f);
    static const float32x4_t minus_point5_val_dup = vdupq_n_f32(-0.5f);

    const uint32x4_t mask = vcltq_f32(x, zero_val_dup);
    const float32x4_t round = vbslq_f32(mask, minus_point5_val_dup, point5_val_dup);
    return vcvtq_s32_f32(vaddq_f32(x, round));
}
#    endif
} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_detail_batch_round_
