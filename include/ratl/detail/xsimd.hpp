/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_xsimd_
#define _ratl_detail_xsimd_

// other includes
#if defined(RATL_HAS_XSIMD)
#    if defined(RATL_CPP_COMPILER_MSVC)
#        pragma warning(push, 0)
#    endif
#    include <xsimd/xsimd.hpp>
#    if defined(RATL_CPP_COMPILER_MSVC)
#        pragma warning(pop)
#    endif
#endif

#endif // _ratl_detail_xsimd_
