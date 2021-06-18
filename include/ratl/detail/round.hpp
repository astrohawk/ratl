#ifndef _ratl_detail_round_
#define _ratl_detail_round_

// C++ Standard Library includes

// ratl includes
#include <ratl/types.hpp>

// defines
#if (defined(RATL_CPP_ARCH_X86) || defined(RATL_CPP_ARCH_X86_64)) && defined(__SSE__)
#    define RATL_ROUND_USE_X86_INTRINSICS
#endif
#if defined(RATL_CPP_ARCH_ARM) && defined(__ARM_NEON)
#    define RATL_ROUND_USE_NEON_INTRINSICS
#endif

#if defined(RATL_ROUND_USE_X86_INTRINSICS) || defined(RATL_ROUND_USE_NEON_INTRINSICS)
#    include <ratl/detail/intrin.hpp>
#endif

namespace ratl
{
namespace detail
{
inline int32_t roundFloat32ToInt32(float32_t input) noexcept
{
#if defined(RATL_ROUND_USE_X86_INTRINSICS)
    return _mm_cvt_ss2si(_mm_set_ss(input));
//#elif defined(RATL_ROUND_USE_NEON_INTRINSICS)
//#    if defined(__aarch64__)
//    return vgetq_lane_s32(vcvtnq_s32_f32(vrndiq_f32(vdupq_n_f32(input))), 0);
//#    else
//    return static_cast<int32_t>(vgetq_lane_f32(vreinterpretq_f32_m128(_mm_round_ps(a, _MM_FROUND_CUR_DIRECTION)), 0));
//#    endif
#else
    return static_cast<int32_t>(std::lrint(input));
#endif
}
} // namespace detail
} // namespace ratl

#endif // _ratl_detail_round_
