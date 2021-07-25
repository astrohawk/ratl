#ifndef _ratl_detail_intrin_
#define _ratl_detail_intrin_

// lib-aud-cpp includes
#include <ratl/detail/config.hpp>

#if defined(RATL_CPP_ARCH_X86) || defined(RATL_CPP_ARCH_X86_64)
#    include <immintrin.h>
#elif defined(RATL_CPP_ARCH_ARM) || defined(RATL_CPP_ARCH_AARCH64)
#    include <arm_neon.h>
#else
#    error Unsupported architecture
#endif

#endif // _ratl_detail_intrin_
