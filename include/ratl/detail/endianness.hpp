#ifndef _ratl_detail_endianness_
#define _ratl_detail_endianness_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/detail/cast.hpp>
#include <ratl/detail/config.hpp>

namespace ratl
{
namespace detail
{
// reverseEndianness

inline uint16_t reverseEndianness(uint16_t input) noexcept
{
#if defined(RATL_CPP_COMPILER_MSVC)
    return _byteswap_ushort(input);
#elif defined(RATL_CPP_COMPILER_CLANG) || defined(RATL_CPP_COMPILER_GCC)
    return __builtin_bswap16(input);
#endif
}

inline uint32_t reverseEndianness(uint32_t input) noexcept
{
#if defined(RATL_CPP_COMPILER_MSVC)
    return _byteswap_ulong(input);
#elif defined(RATL_CPP_COMPILER_CLANG) || defined(RATL_CPP_COMPILER_GCC)
    return __builtin_bswap32(input);
#endif
}

inline uint24_t reverseEndianness(uint24_t input) noexcept
{
    return static_cast<uint24_t>(reverseEndianness(static_cast<uint32_t>(input) << 8));
}

// hostToNetwork

template<class NetworkSampleValueUnderlyingType>
inline NetworkSampleValueUnderlyingType hostToNetwork(NetworkSampleValueUnderlyingType input) noexcept
{
#if defined(RATL_CPP_LITTLE_ENDIAN)
    return reverseEndianness(input);
#else
    return input;
#endif
}

// networkToHost

template<class NetworkSampleValueUnderlyingType>
inline NetworkSampleValueUnderlyingType networkToHost(NetworkSampleValueUnderlyingType input) noexcept
{
#if defined(RATL_CPP_LITTLE_ENDIAN)
    return reverseEndianness(input);
#else
    return input;
#endif
}

// sampleToNetworkSample

template<class SampleType>
inline NetworkSampleValueType_t<SampleType> sampleToNetworkSample(SampleType input) noexcept
{
    return networkUnderlyingToNetworkCast<SampleType>(hostToNetwork(sampleToNetworkUnderlyingCast<SampleType>(input)));
}

// networkSampleToSample

template<class SampleType>
inline SampleType networkSampleToSample(NetworkSampleValueType_t<SampleType> input) noexcept
{
    return networkUnderlyingToSampleCast<SampleType>(networkToHost(networkToNetworkUnderlyingCast<SampleType>(input)));
}

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_endianness_
