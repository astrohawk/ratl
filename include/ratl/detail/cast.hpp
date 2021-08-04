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
// sampleToNetworkCast

template<class Output, class Input>
inline Output bitwiseCast(const Input& input) noexcept
{
#if defined(RATL_CPP_VERSION_HAS_CPP20)
    return std::bit_cast<Output>(input);
#elif defined(RATL_CPP_COMPILER_CLANG) || defined(RATL_CPP_COMPILER_GCC)
    Output output;
    __builtin_memcpy(&output, &input, sizeof(Output));
    return output;
#else
    Output output;
    std::memcpy(&output, &input, sizeof(Output));
    return output;
#endif
}

// sampleToNetworkUnderlyingCast

template<class SampleType>
inline NetworkSampleValueUnderlyingType_t<SampleType> sampleToNetworkUnderlyingCast(SampleType input) noexcept
{
    return bitwiseCast<NetworkSampleValueUnderlyingType_t<SampleType>>(input);
}

// networkUnderlyingToSampleCast

template<class SampleType>
inline SampleType networkUnderlyingToSampleCast(NetworkSampleValueUnderlyingType_t<SampleType> input) noexcept
{
    return bitwiseCast<SampleType>(input);
}

// networkToNetworkUnderlyingCast

template<class SampleType>
inline NetworkSampleValueUnderlyingType_t<SampleType> networkToNetworkUnderlyingCast(
    NetworkSampleValueType_t<SampleType> input) noexcept
{
    return bitwiseCast<NetworkSampleValueUnderlyingType_t<SampleType>>(input);
}

// networkUnderlyingToNetworkCast

template<class SampleType>
inline NetworkSampleValueType_t<SampleType> networkUnderlyingToNetworkCast(
    NetworkSampleValueUnderlyingType_t<SampleType> input) noexcept
{
    return bitwiseCast<NetworkSampleValueType_t<SampleType>>(input);
}

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_cast_
