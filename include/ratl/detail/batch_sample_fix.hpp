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
// batch_fix_negative_samples

template<class SampleValueType>
typename std::enable_if<has_batch_type_v<SampleValueType>, batch_sample_value_type_t<SampleValueType>>::type
batch_fix_negative_samples(const batch_sample_value_type_t<SampleValueType>& input) noexcept
{
    return input;
}

template<class SampleValueType>
typename std::enable_if<!has_batch_type_v<SampleValueType>, batch_sample_value_type_t<SampleValueType>>::type
batch_fix_negative_samples(const batch_sample_value_type_t<SampleValueType>& input) noexcept
{
    static constexpr std::size_t shift =
        (sizeof(typename batch_sample_value_type_t<SampleValueType>::value_type) - sizeof(SampleValueType)) * CHAR_BIT;
    return (input << shift) >> shift;
}

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_batch_sample_fix_
