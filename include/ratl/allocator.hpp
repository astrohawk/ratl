/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_allocator_
#define _ratl_allocator_

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/xsimd.hpp>

// other includes
#include <memory>

namespace ratl
{
#if defined(RATL_HAS_XSIMD)

template<class Tp>
using allocator = XSIMD_DEFAULT_ALLOCATOR(Tp);

#else

template<class Tp>
using allocator = std::allocator<Tp>;

#endif

} // namespace ratl

#endif // _ratl_allocator_
