#ifndef _ratl_detail_batch_cast_
#define _ratl_detail_batch_cast_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/detail/batch_value_traits.hpp>

#if defined(RATL_HAS_XSIMD)

namespace ratl
{
namespace detail
{
// batch_sample_cast

template<class OutputSampleType, class InputBatch>
inline batch_sample_value_type_t<OutputSampleType> batch_sample_cast(const InputBatch& input) noexcept
{
    return xsimd::batch_cast<typename batch_sample_value_type_t<OutputSampleType>::value_type>(input);
}

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_detail_batch_cast_
