#ifndef _ratl_detail_round_
#define _ratl_detail_round_

// C++ Standard Library includes

// ratl includes
#include <ratl/detail/cast.hpp>
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
inline int32_t roundFloat32ToInt32Magic(float32_t input) noexcept
{
    static constexpr double MagicDoubleDelta = (1.5e-8);
    static constexpr double MagicDouble = 6755399441055744.0;
#if defined(RATL_CPP_LITTLE_ENDIAN)
    static constexpr int MagicRoundIndex = 0;
#else
    static constexpr int MagicRoundIndex = 1;
#endif

    double d = input;
    d += MagicDoubleDelta;
    d += MagicDouble;
    return detail::bitwiseCast<std::array<int32_t, 2>>(d)[MagicRoundIndex];
}

inline int32_t roundFloat32ToInt32(float32_t input) noexcept
{
#if defined(RATL_ROUND_USE_SSE_INTRINSICS)
    return _mm_cvt_ss2si(_mm_set_ss(input));
#elif defined(RATL_ROUND_USE_NEON_INTRINSICS) && defined(RATL_CPP_ARCH_AARCH64)
    return vgetq_lane_s32(vcvtnq_s32_f32(vdupq_n_f32(input)), 0);
//#elif defined(RATL_ROUND_USE_NEON_INTRINSICS) && defined(RATL_CPP_ARCH_ARM)
//    return static_cast<int32_t>(vgetq_lane_f32(vreinterpretq_f32_m128(_mm_round_ps(a, _MM_FROUND_CUR_DIRECTION)), 0));
#elif defined(RATL_CPP_COMPILER_CLANG) || defined(RATL_CPP_COMPILER_GCC)
    return static_cast<int32_t>(__builtin_lrint(input));
#else
    return static_cast<int32_t>(std::lrint(input));
#endif
}

inline int32_t roundFloat32ToInt32Fast(float32_t input) noexcept
{
#if defined(RATL_ROUND_USE_SSE_INTRINSICS)
    return _mm_cvt_ss2si(_mm_set_ss(input));
#elif defined(RATL_ROUND_USE_NEON_INTRINSICS) && defined(RATL_CPP_ARCH_AARCH64)
    return vgetq_lane_s32(vcvtnq_s32_f32(vdupq_n_f32(input)), 0);
#else
    return static_cast<int32_t>(roundFloat32ToInt32Magic(input));
#endif
}
} // namespace detail
} // namespace ratl

#endif // _ratl_detail_round_
