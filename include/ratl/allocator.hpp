#ifndef _ratl_allocator_
#define _ratl_allocator_

// C++ Standard Library includes
#include <memory>

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/xsimd.hpp>

namespace ratl
{
#if defined(RATL_HAS_XSIMD)

template<class Tp>
using Allocator = XSIMD_DEFAULT_ALLOCATOR(Tp);

template<class Allocator, class Tp>
using IsAlignedAllocator = std::is_same<Allocator, XSIMD_DEFAULT_ALLOCATOR(Tp)>;

#else

template<class Tp>
using Allocator = std::allocator<Tp>;

template<class Allocator, class Tp>
using IsAlignedAllocator = std::integral_constant<bool, false>;

#endif

template<class Allocator, class Tp>
static constexpr bool IsAlignedAllocator_v = IsAlignedAllocator<Allocator, Tp>::value;

} // namespace ratl

#endif // _ratl_allocator_
