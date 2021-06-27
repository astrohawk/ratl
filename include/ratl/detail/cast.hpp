#ifndef _ratl_detail_cast_
#define _ratl_detail_cast_

// C++ Standard Library includes
#include <type_traits>
#if defined(RATL_CPP_VERSION_HAS_CPP20)
#    include <bit>
#endif

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
#if defined(RATL_CPP_VERSION_HAS_CPP20)
    return std::bit_cast<NetworkSampleValueUnderlyingType_t<SampleType>>(input);
#else
    NetworkSampleValueUnderlyingType_t<SampleType> output;
    std::memcpy(&output, &input, sizeof(SampleType));
    return output;
#endif
}

// networkUnderlyingToSampleCast

template<class SampleType>
inline SampleType networkUnderlyingToSampleCast(NetworkSampleValueUnderlyingType_t<SampleType> input) noexcept
{
#if defined(RATL_CPP_VERSION_HAS_CPP20)
    return std::bit_cast<SampleType>(input);
#else
    SampleType output;
    std::memcpy(&output, &input, sizeof(SampleType));
    return output;
#endif
}

// networkToNetworkUnderlyingCast

template<class SampleType>
inline NetworkSampleValueUnderlyingType_t<SampleType> networkToNetworkUnderlyingCast(
    NetworkSampleValueType_t<SampleType> input) noexcept
{
#if defined(RATL_CPP_VERSION_HAS_CPP20)
    return std::bit_cast<NetworkSampleValueUnderlyingType_t<SampleType>>(input);
#else
    NetworkSampleValueUnderlyingType_t<SampleType> output;
    std::memcpy(&output, &input, sizeof(SampleType));
    return output;
#endif
}

// networkUnderlyingToNetworkCast

template<class SampleType>
inline NetworkSampleValueType_t<SampleType> networkUnderlyingToNetworkCast(
    NetworkSampleValueUnderlyingType_t<SampleType> input) noexcept
{
#if defined(RATL_CPP_VERSION_HAS_CPP20)
    return std::bit_cast<NetworkSampleValueType_t<SampleType>>(input);
#else
    NetworkSampleValueType_t<SampleType> output;
    std::memcpy(&output, &input, sizeof(SampleType));
    return output;
#endif
}

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_cast_
