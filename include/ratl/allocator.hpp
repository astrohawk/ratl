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
using allocator = XSIMD_DEFAULT_ALLOCATOR(Tp);

#else

template<class Tp>
using allocator = std::allocator<Tp>;

#endif

} // namespace ratl

#endif // _ratl_allocator_
