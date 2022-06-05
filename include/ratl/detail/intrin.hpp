/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_intrin_
#define _ratl_detail_intrin_

// ratl includes
#include <ratl/detail/config.hpp>

#if defined(RATL_CPP_ARCH_X86) || defined(RATL_CPP_ARCH_X86_64)
#    include <immintrin.h>
#elif defined(RATL_CPP_ARCH_ARM) || defined(RATL_CPP_ARCH_AARCH64)
#    include <arm_neon.h>
#else
#    error Unsupported architecture
#endif

#endif // _ratl_detail_intrin_
