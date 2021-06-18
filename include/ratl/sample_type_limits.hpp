#ifndef _ratl_sample_type_limits_
#define _ratl_sample_type_limits_

// C++ Standard Library includes
#include <limits>
#include <type_traits>

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/type_traits.hpp>
#include <ratl/detail/types.hpp>

namespace ratl
{
namespace detail
{
// general case (int16 and int32)

template<class SampleType>
struct SampleTypeLimitsImpl
{
    using value_type = SampleType;

    static_assert(std::numeric_limits<value_type>::is_specialized, "SampleType is not specialised");

    static constexpr int digits = std::numeric_limits<value_type>::digits;
    static constexpr bool is_integer = std::numeric_limits<value_type>::is_integer;

    static constexpr value_type min = std::numeric_limits<value_type>::min();
    static constexpr value_type max = std::numeric_limits<value_type>::max();
};

template<class SampleType>
constexpr int SampleTypeLimitsImpl<SampleType>::digits;
template<class SampleType>
constexpr bool SampleTypeLimitsImpl<SampleType>::is_integer;
template<class SampleType>
constexpr typename SampleTypeLimitsImpl<SampleType>::value_type SampleTypeLimitsImpl<SampleType>::min;
template<class SampleType>
constexpr typename SampleTypeLimitsImpl<SampleType>::value_type SampleTypeLimitsImpl<SampleType>::max;

// int24

template<>
struct SampleTypeLimitsImpl<int24_t>
{
    using value_type = int24_t;

    static constexpr int digits = RATL_INT24_DIGITS;
    static constexpr bool is_integer = true;

    static constexpr value_type min = RATL_INT24_MIN;
    static constexpr value_type max = RATL_INT24_MAX;
};

constexpr int SampleTypeLimitsImpl<int24_t>::digits;
constexpr bool SampleTypeLimitsImpl<int24_t>::is_integer;
constexpr typename SampleTypeLimitsImpl<int24_t>::value_type SampleTypeLimitsImpl<int24_t>::min;
constexpr typename SampleTypeLimitsImpl<int24_t>::value_type SampleTypeLimitsImpl<int24_t>::max;

// float32

template<>
struct SampleTypeLimitsImpl<float32_t>
{
    using value_type = float32_t;

    static constexpr int digits = std::numeric_limits<float32_t>::digits;
    static constexpr bool is_integer = std::numeric_limits<float32_t>::is_integer;

    static constexpr value_type min = -1.f;
    static constexpr value_type max = 1.f;
};

constexpr int SampleTypeLimitsImpl<float32_t>::digits;
constexpr bool SampleTypeLimitsImpl<float32_t>::is_integer;
constexpr typename SampleTypeLimitsImpl<float32_t>::value_type SampleTypeLimitsImpl<float32_t>::min;
constexpr typename SampleTypeLimitsImpl<float32_t>::value_type SampleTypeLimitsImpl<float32_t>::max;

} // namespace detail

template<class SampleType>
class SampleTypeLimits : private detail::SampleTypeLimitsImpl<detail::SampleTypeDecay_t<SampleType>>
{
    using super_type = typename detail::SampleTypeLimitsImpl<detail::SampleTypeDecay_t<SampleType>>;
    using value_type = typename super_type::value_type;

public:
    static constexpr int digits = super_type::digits;
    static constexpr bool is_integer = super_type::is_integer;

    static constexpr value_type min = super_type::min;
    static constexpr value_type max = super_type::max;
};

template<class SampleType>
constexpr int SampleTypeLimits<SampleType>::digits;
template<class SampleType>
constexpr bool SampleTypeLimits<SampleType>::is_integer;
template<class SampleType>
constexpr typename SampleTypeLimits<SampleType>::value_type SampleTypeLimits<SampleType>::min;
template<class SampleType>
constexpr typename SampleTypeLimits<SampleType>::value_type SampleTypeLimits<SampleType>::max;

} // namespace ratl

#endif // _ratl_sample_type_limits_
