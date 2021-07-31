#ifndef _ratl_detail_batch_cast_
#define _ratl_detail_batch_cast_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/detail/batch_base_traits.hpp>

#if defined(RATL_HAS_XSIMD)

namespace ratl
{
namespace detail
{
// batchSampleCast

template<class OutputSampleType, class InputBatch>
inline BatchSampleValueType_t<OutputSampleType> batchSampleCast(const InputBatch& input) noexcept
{
    return xsimd::batch_cast<typename BatchSampleValueType_t<OutputSampleType>::value_type>(input);
}

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_detail_batch_cast_
