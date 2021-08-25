/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_round_
#define _ratl_detail_round_

// ratl includes
#include <ratl/detail/cast.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/types.hpp>

// defines
#if (defined(RATL_CPP_ARCH_X86) || defined(RATL_CPP_ARCH_X86_64)) && defined(__SSE__)
#    define RATL_ROUND_USE_SSE_INTRINSICS
#endif
#if (defined(RATL_CPP_ARCH_ARM) || defined(RATL_CPP_ARCH_AARCH64)) && defined(__ARM_NEON)
#    define RATL_ROUND_USE_NEON_INTRINSICS
#endif

#if defined(RATL_ROUND_USE_SSE_INTRINSICS) || defined(RATL_ROUND_USE_NEON_INTRINSICS)
#    include <ratl/detail/intrin.hpp>
#endif

namespace ratl
{
namespace detail
{
inline int32_t round_float32_to_int32_magic(float32_t input) noexcept
{
    static constexpr double magic_double_delta = (1.5e-8);
    static constexpr double magic_double = 6755399441055744.0;
#if defined(RATL_CPP_LITTLE_ENDIAN)
    static constexpr int magic_round_index = 0;
#else
    static constexpr int magic_round_index = 1;
#endif

    double d = input;
    d += magic_double_delta;
    d += magic_double;
    return bit_cast<std::array<int32_t, 2>>(d)[magic_round_index];
}

inline int32_t round_float32_to_int32(float32_t input) noexcept
{
#if defined(RATL_ROUND_USE_SSE_INTRINSICS)
    return _mm_cvt_ss2si(_mm_set_ss(input));
#elif defined(RATL_ROUND_USE_NEON_INTRINSICS) && defined(RATL_CPP_ARCH_AARCH64)
    return vgetq_lane_s32(vcvtnq_s32_f32(vdupq_n_f32(input)), 0);
#elif defined(RATL_CPP_COMPILER_CLANG) || defined(RATL_CPP_COMPILER_GCC)
    return static_cast<int32_t>(__builtin_lrint(input));
#else
    return static_cast<int32_t>(std::lrint(input));
#endif
}

inline int32_t round_float32_to_int32_fast(float32_t input) noexcept
{
#if defined(RATL_ROUND_USE_SSE_INTRINSICS)
    return _mm_cvt_ss2si(_mm_set_ss(input));
#elif defined(RATL_ROUND_USE_NEON_INTRINSICS) && defined(RATL_CPP_ARCH_AARCH64)
    return vgetq_lane_s32(vcvtnq_s32_f32(vdupq_n_f32(input)), 0);
#else
    return static_cast<int32_t>(round_float32_to_int32_magic(input));
#endif
}
} // namespace detail
} // namespace ratl

#endif // _ratl_detail_round_
