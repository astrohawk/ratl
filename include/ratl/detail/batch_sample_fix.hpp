#ifndef _ratl_batch_sample_fix_
#define _ratl_batch_sample_fix_

// C++ Standard Library includes
#include <climits>
#include <type_traits>

// ratl includes
#include <ratl/detail/batch_traits.hpp>
#include <ratl/detail/config.hpp>

#if defined(RATL_HAS_XSIMD)

namespace ratl
{
namespace detail
{
// batchFixNegativeSamples

template<class SampleType>
typename std::enable_if<HasBatchType_v<SampleType>, BatchSampleValueType_t<SampleType>>::type batchFixNegativeSamples(
    const BatchSampleValueType_t<SampleType>& input) noexcept
{
    return input;
}

template<class SampleType>
typename std::enable_if<!HasBatchType_v<SampleType>, BatchSampleValueType_t<SampleType>>::type batchFixNegativeSamples(
    const BatchSampleValueType_t<SampleType>& input) noexcept
{
    static constexpr std::size_t Shift =
        (sizeof(typename BatchSampleValueType_t<SampleType>::value_type) - sizeof(SampleType)) * CHAR_BIT;
    return (input << Shift) >> Shift;
}

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_batch_sample_fix_
