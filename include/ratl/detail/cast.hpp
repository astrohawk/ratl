#ifndef _ratl_detail_cast_
#define _ratl_detail_cast_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/detail/type_traits.hpp>

namespace ratl
{
namespace detail
{
// sampleToNetworkUnderlyingCast

template<class SampleType>
inline NetworkSampleValueUnderlyingType_t<SampleType> sampleToNetworkUnderlyingCast(SampleType input) noexcept
{
    NetworkSampleValueUnderlyingType_t<SampleType> output;
    std::memcpy(&output, &input, sizeof(SampleType));
    return output;
}

// networkUnderlyingToSampleCast

template<class SampleType>
inline SampleType networkUnderlyingToSampleCast(NetworkSampleValueUnderlyingType_t<SampleType> input) noexcept
{
    SampleType output;
    std::memcpy(&output, &input, sizeof(SampleType));
    return output;
}

// networkToNetworkUnderlyingCast

template<class SampleType>
inline NetworkSampleValueUnderlyingType_t<SampleType> networkToNetworkUnderlyingCast(
    NetworkSampleValueType_t<SampleType> input) noexcept
{
    NetworkSampleValueUnderlyingType_t<SampleType> output;
    std::memcpy(&output, &input, sizeof(SampleType));
    return output;
}

// networkUnderlyingToNetworkCast

template<class SampleType>
inline NetworkSampleValueType_t<SampleType> networkUnderlyingToNetworkCast(
    NetworkSampleValueUnderlyingType_t<SampleType> input) noexcept
{
    NetworkSampleValueType_t<SampleType> output;
    std::memcpy(&output, &input, sizeof(SampleType));
    return output;
}

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_cast_
