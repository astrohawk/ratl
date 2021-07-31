#ifndef _ratl_detail_convert_traits_
#define _ratl_detail_convert_traits_

// ratl includes
#include <ratl/sample_type_limits.hpp>

namespace ratl
{
namespace detail
{
// FloatConvertTraits

template<class SampleType>
struct FloatConvertTraits
{
    static constexpr float32_t Multiplier = -static_cast<float32_t>(SampleTypeLimits<SampleType>::min);
    static constexpr float32_t Divisor = 1.f / Multiplier;
};

template<class SampleType>
constexpr float32_t FloatConvertTraits<SampleType>::Multiplier;
template<class SampleType>
constexpr float32_t FloatConvertTraits<SampleType>::Divisor;

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_convert_traits_
