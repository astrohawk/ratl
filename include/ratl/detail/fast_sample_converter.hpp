#ifndef _ratl_detail_sample_converter_
#define _ratl_detail_sample_converter_

// C++ Standard Library includes
#include <cfenv>
#include <cmath>
#include <type_traits>

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/xsimd.hpp>
#include <ratl/sample_type_limits.hpp>

// defines
#if (defined(RATL_CPP_ARCH_X86) || defined(RATL_CPP_ARCH_X86_64)) && defined(__SSE__)
#    define RATL_CPP_RATL_FLOAT_TO_INT_USE_X86_INTRINSICS
#endif

#if defined(RATL_CPP_RATL_FLOAT_TO_INT_USE_X86_INTRINSICS)

#    include <detail/intrin.hpp>

#endif

#define RATL_CPP_RATL_FLOAT_TO_INT_USE_MAGIC_DOUBLE

namespace ratl
{
namespace detail
{
// FloatConvertTraits

template<class SampleType>
struct FloatConvertTraits
{
    static constexpr float32_t Multiplier = static_cast<float32_t>(SampleTypeLimits<SampleType>::max);
    static constexpr float32_t Divisor = 1.f / -static_cast<float32_t>(SampleTypeLimits<SampleType>::min);
};

template<class SampleType>
constexpr float32_t FloatConvertTraits<SampleType>::Multiplier;
template<class SampleType>
constexpr float32_t FloatConvertTraits<SampleType>::Divisor;

// SampleConverter

template<class InputSampleType, class OutputSampleType>
struct SampleConverter;

// intXX_t -> intXX_t

template<class SampleType>
struct SampleConverter<SampleType, SampleType>
{
    static const SampleType& convert(const SampleType& sample) noexcept
    {
        return sample;
    }
};

// int16_t -> intXX_t

template<>
struct SampleConverter<int16_t, int24_t>
{
    static int24_t convert(int32_t sample) noexcept
    {
        return static_cast<int24_t>(sample << 8); // TODO: dithering
    }
};

template<>
struct SampleConverter<int16_t, int32_t>
{
    static int32_t convert(int32_t sample) noexcept
    {
        return sample << 16; // TODO: dithering
    }
};

// int24_t -> intXX_t

template<>
struct SampleConverter<int24_t, int16_t>
{
    static int16_t convert(int32_t sample) noexcept
    {
        if (sample >= static_cast<int32_t>(0x007FFF80))
        {
            return static_cast<int16_t>(SampleTypeLimits<int16_t>::max);
        }
        return static_cast<int16_t>((sample + 0x80) >> 8);
    }
};

template<>
struct SampleConverter<int24_t, int32_t>
{
    static int32_t convert(int32_t sample) noexcept
    {
        return sample << 8; // TODO: dithering
    }
};

// int32_t -> intXX_t

template<>
struct SampleConverter<int32_t, int24_t>
{
    static int24_t convert(int32_t sample) noexcept
    {
        if (sample >= static_cast<int32_t>(0x7FFFFF80))
        {
            return static_cast<int24_t>(SampleTypeLimits<int24_t>::max);
        }
        return static_cast<int24_t>((sample + 0x80) >> 8);
    }
};

template<>
struct SampleConverter<int32_t, int16_t>
{
    static int16_t convert(int32_t sample) noexcept
    {
        if (sample >= static_cast<int32_t>(0x7FFF8000))
        {
            return static_cast<int16_t>(SampleTypeLimits<int16_t>::max);
        }
        return static_cast<int16_t>((sample + 0x8000) >> 16);
    }
};

// intXX_t -> float32_t

template<class SampleType>
struct SampleConverter<SampleType, float32_t>
{
    static float32_t convert(SampleType sample) noexcept
    {
        return static_cast<float32_t>(sample) * FloatConvertTraits<SampleType>::Divisor;
    }
};

// float32_t -> intXX_t

template<class SampleType>
struct SampleConverter<float32_t, SampleType>
{
private:
    static constexpr float64_t MAGIC_DOUBLE = 6755399441055744.0;
    static constexpr float64_t MAGIC_DOUBLE_DELTA = (1.5e-8);
#if defined(RATL_CPP_LITTLE_ENDIAN)
    static constexpr int MAGIC_ROUND_INDEX = 0;
#else
    static constexpr int MAGIC_ROUND_INDEX = 1;
#endif

    static int32_t magicInt32Cast(float64_t sample)
    {
        sample += MAGIC_DOUBLE_DELTA;
        sample += MAGIC_DOUBLE;
        return reinterpret_cast<int32_t*>(&sample)[MAGIC_ROUND_INDEX];
    }

public:
    static int32_t convert(float32_t sample) noexcept
    {
        if RATL_UNLIKELY (sample >= SampleTypeLimits<float32_t>::max)
        {
            return SampleTypeLimits<SampleType>::max;
        }
        if RATL_UNLIKELY (sample < SampleTypeLimits<float32_t>::min)
        {
            return SampleTypeLimits<SampleType>::min;
        }
#if defined(RATL_CPP_RATL_FLOAT_TO_INT_USE_X86_INTRINSICS)
        __m128 temp_m128 = _mm_mul_ss(_mm_set_ss(sample), _mm_loadu_ps(&FloatConvertTraits<SampleType>::Multiplier));
        temp_m128 = _mm_sub_ss(temp_m128, _mm_set_ss(0.5));
        return _mm_cvt_ss2si(temp_m128);
#else
        sample *= FloatConvertTraits<SampleType>::Multiplier;
        sample -= 0.5;
#    if defined(RATL_CPP_RATL_FLOAT_TO_INT_USE_MAGIC_DOUBLE)
        return magicInt32Cast(sample);
#    else
        return std::lround(sample);
#    endif
#endif
    }
};

// float32_t -> float32_t
// This is required as SampleConverter<float32_t, SampleType> is more specific than
// SampleConverter<SampleType, SampleType>

template<>
struct SampleConverter<float32_t, float32_t>
{
    static float32_t convert(float32_t sample) noexcept
    {
        return sample;
    }
};

// SampleToNetworkConverter

template<class InputSampleType, class OutputSampleType>
struct SampleToNetworkConverter;

// int16_t -> intXX_t

template<>
struct SampleToNetworkConverter<int16_t, int16_t>
{
    static NetworkSampleValueType_t<int16_t> convert(int32_t sample) noexcept
    {
#if defined(RATL_CPP_LITTLE_ENDIAN)
        return underlyingToNetworkCast<int16_t>(((sample & 0x0000ff00) >> 8) | ((sample & 0x000000ff) << 8));
#else
        return underlyingToNetworkCast<int16_t>(sample);
#endif
    }
};

template<>
struct SampleToNetworkConverter<int16_t, int24_t>
{
    static NetworkSampleValueType_t<int24_t> convert(int32_t sample) noexcept
    {
#if defined(RATL_CPP_LITTLE_ENDIAN)
        return underlyingToNetworkCast<int24_t>(((sample & 0x0000ff00) >> 8) | ((sample & 0x000000ff) << 8));
#else
        return underlyingToNetworkCast<int24_t>(SampleConverter<int16_t, int24_t>::convert(sample));
#endif
    }
};

template<>
struct SampleToNetworkConverter<int16_t, int32_t>
{
    static NetworkSampleValueType_t<int32_t> convert(int32_t sample) noexcept
    {
#if defined(RATL_CPP_LITTLE_ENDIAN)
        return underlyingToNetworkCast<int32_t>(((sample & 0x0000ff00) >> 8) | ((sample & 0x000000ff) << 8));
#else
        return network_sample_cast<int32_t>(SampleConverter<int16_t, int32_t>::convert(sample));
#endif
    }
};

// int24_t -> intXX_t

template<>
struct SampleToNetworkConverter<int24_t, int16_t>
{
    static NetworkSampleValueType_t<int16_t> convert(int32_t sample) noexcept
    {
#if defined(RATL_CPP_LITTLE_ENDIAN)
        return underlyingToNetworkCast<int16_t>(((sample & 0x00ff0000) >> 16) | (sample & 0x0000ff00));
#else
        return underlyingToNetworkCast<int16_t>(SampleConverter<int24_t, int16_t>::convert(sample));
#endif
    }
};

template<>
struct SampleToNetworkConverter<int24_t, int24_t>
{
    static NetworkSampleValueType_t<int24_t> convert(int32_t sample) noexcept
    {
#if defined(RATL_CPP_LITTLE_ENDIAN)
        return underlyingToNetworkCast<int24_t>(
            ((sample & 0x00ff0000) >> 16) | (sample & 0x0000ff00) | ((sample & 0x000000ff) << 16));
#else
        return underlyingToNetworkCast<int24_t>(sample);
#endif
    }
};

template<>
struct SampleToNetworkConverter<int24_t, int32_t>
{
    static NetworkSampleValueType_t<int32_t> convert(int32_t sample) noexcept
    {
#if defined(RATL_CPP_LITTLE_ENDIAN)
        return underlyingToNetworkCast<int32_t>(
            ((sample & 0x00ff0000) >> 16) | (sample & 0x0000ff00) | ((sample & 0x000000ff) << 16));
#else
        return underlyingToNetworkCast<int32_t>(SampleConverter<int24_t, int32_t>::convert(sample));
#endif
    }
};

// int32_t -> intXX_t

template<>
struct SampleToNetworkConverter<int32_t, int16_t>
{
    static NetworkSampleValueType_t<int16_t> convert(int32_t sample) noexcept
    {
#if defined(RATL_CPP_LITTLE_ENDIAN)
        return underlyingToNetworkCast<int16_t>(((sample & 0xff000000) >> 24) | ((sample & 0x00ff0000) >> 8));
#else
        return underlyingToNetworkCast<int16_t>(SampleConverter<int32_t, int16_t>::convert(sample));
#endif
    }
};

template<>
struct SampleToNetworkConverter<int32_t, int24_t>
{
    static NetworkSampleValueType_t<int24_t> convert(int32_t sample) noexcept
    {
#if defined(RATL_CPP_LITTLE_ENDIAN)
        return underlyingToNetworkCast<int24_t>(
            ((sample & 0xff000000) >> 24) | ((sample & 0x00ff0000) >> 8) | ((sample & 0x0000ff00) << 8));
#else
        return underlyingToNetworkCast<int24_t>(SampleConverter<int32_t, int24_t>::convert(sample));
#endif
    }
};

template<>
struct SampleToNetworkConverter<int32_t, int32_t>
{
    static NetworkSampleValueType_t<int32_t> convert(int32_t sample) noexcept
    {
#if defined(RATL_CPP_LITTLE_ENDIAN)
        return underlyingToNetworkCast<int32_t>(
            ((sample & 0xff000000) >> 24) | ((sample & 0x00ff0000) >> 8) | ((sample & 0x0000ff00) << 8) |
            ((sample & 0x000000ff) << 24));
#else
        return underlyingToNetworkCast<int32_t>(sample);
#endif
    }
};

// float32_t -> float32_t

template<>
struct SampleToNetworkConverter<float32_t, float32_t>
{
    static NetworkSampleValueType_t<float32_t> convert(float32_t sample) noexcept
    {
#if defined(RATL_CPP_LITTLE_ENDIAN)
        auto temp = SampleToNetworkConverter<int32_t, int32_t>::convert(reinterpret_cast<const int32_t&>(sample));
        return underlyingToNetworkCast<float32_t>(reinterpret_cast<const uint32_t&>(temp));
#else
        return underlyingToNetworkCast<float32_t>(reinterpret_cast<const uint32_t&>(sample));
#endif
    }
};

// intXX_t -> float32_t

template<class SampleType>
struct SampleToNetworkConverter<SampleType, float32_t>
{
    static NetworkSampleValueType_t<float32_t> convert(int32_t sample) noexcept
    {
        return SampleToNetworkConverter<float32_t, float32_t>::convert(
            SampleConverter<SampleType, float32_t>::convert(sample));
    }
};

// float32_t -> intXX_t

template<class SampleType>
struct SampleToNetworkConverter<float32_t, SampleType>
{
    static NetworkSampleValueType_t<SampleType> convert(float32_t sample) noexcept
    {
        return SampleToNetworkConverter<SampleType, SampleType>::convert(
            SampleConverter<float32_t, SampleType>::convert(sample));
    }
};

// sample_converter_impl

template<class InputSampleType, class OutputSampleType>
struct NetworkToSampleConverter;

// int16_t -> intXX_t

template<>
struct NetworkToSampleConverter<int16_t, int16_t>
{
    static int16_t convert(NetworkSampleValueType_t<int16_t> sample) noexcept
    {
#if defined(RATL_CPP_LITTLE_ENDIAN)
        uint32_t cast = networkToUnderlyingCast<int16_t>(sample);
        return ((cast & 0x0000ff00) >> 8) | ((cast & 0x000000ff) << 8);
#else
        return networkToUnderlyingCast<int16_t>(sample);
#endif
    }
};

template<>
struct NetworkToSampleConverter<int16_t, int24_t>
{
    static int24_t convert(NetworkSampleValueType_t<int16_t> sample) noexcept
    {
#if defined(RATL_CPP_LITTLE_ENDIAN)
        int32_t cast = networkToUnderlyingCast<int16_t>(sample);
        return (cast & 0x0000ff00) | ((cast & 0x000000ff) << 16);
#else
        return SampleConverter<int16_t, int24_t>::convert(sample_type_cast<int24_t>(sample));
#endif
    }
};

template<>
struct NetworkToSampleConverter<int16_t, int32_t>
{
    static int32_t convert(NetworkSampleValueType_t<int16_t> sample) noexcept
    {
#if defined(RATL_CPP_LITTLE_ENDIAN)
        int32_t cast = networkToUnderlyingCast<int16_t>(sample);
        return ((cast & 0x0000ff00) << 8) | ((cast & 0x000000ff) << 24);
#else
        return SampleConverter<int16_t, int32_t>::convert(sample_type_cast<int32_t>(sample));
#endif
    }
};

// int24_t -> intXX_t

template<>
struct NetworkToSampleConverter<int24_t, int16_t>
{
    static int16_t convert(NetworkSampleValueType_t<int24_t> sample) noexcept
    {
#if defined(RATL_CPP_LITTLE_ENDIAN)
        int32_t cast = networkToUnderlyingCast<int24_t>(sample);
        return ((cast & 0x0000ff00) >> 8) | ((cast & 0x000000ff) << 8);
#else
        return SampleConverter<int24_t, int16_t>::convert(sample_type_cast<int16_t>(sample));
#endif
    }
};

template<>
struct NetworkToSampleConverter<int24_t, int24_t>
{
    static int24_t convert(NetworkSampleValueType_t<int24_t> sample) noexcept
    {
#if defined(RATL_CPP_LITTLE_ENDIAN)
        int32_t cast = networkToUnderlyingCast<int24_t>(sample);
        return ((cast & 0x00ff0000) >> 16) | (cast & 0x0000ff00) | ((cast & 0x000000ff) << 16);
        // return { cast.high(), cast.mid(), cast.low() };
#else
        return networkToUnderlyingCast<int24_t>(sample);
#endif
    }
};

template<>
struct NetworkToSampleConverter<int24_t, int32_t>
{
    static int32_t convert(NetworkSampleValueType_t<int24_t> sample) noexcept
    {
#if defined(RATL_CPP_LITTLE_ENDIAN)
        int32_t cast = networkToUnderlyingCast<int24_t>(sample);
        return ((cast & 0x00ff0000) >> 8) | ((cast & 0x0000ff00) << 8) | ((cast & 0x000000ff) << 24);
#else
        return SampleConverter<int24_t, int32_t>::convert(networkToUnderlyingCast<int32_t>(sample));
#endif
    }
};

// int32_t -> intXX_t

template<>
struct NetworkToSampleConverter<int32_t, int16_t>
{
    static int16_t convert(NetworkSampleValueType_t<int32_t> sample) noexcept
    {
#if defined(RATL_CPP_LITTLE_ENDIAN)
        int32_t cast = networkToUnderlyingCast<int32_t>(sample);
        return ((cast & 0x0000ff00) >> 8) | ((cast & 0x000000ff) << 8);
#else
        return SampleConverter<int32_t, int16_t>::convert(sample_type_cast<int16_t>(sample));
#endif
    }
};

template<>
struct NetworkToSampleConverter<int32_t, int24_t>
{
    static int24_t convert(NetworkSampleValueType_t<int32_t> sample) noexcept
    {
#if defined(RATL_CPP_LITTLE_ENDIAN)
        int32_t cast = networkToUnderlyingCast<int32_t>(sample);
        return ((cast & 0x00ff0000) >> 16) | (cast & 0x0000ff00) | ((cast & 0x000000ff) << 16);
#else
        return SampleConverter<int32_t, int24_t>::convert(sample_type_cast<int24_t>(sample));
#endif
    }
};

template<>
struct NetworkToSampleConverter<int32_t, int32_t>
{
    static int32_t convert(NetworkSampleValueType_t<int32_t> sample) noexcept
    {
#if defined(RATL_CPP_LITTLE_ENDIAN)
        int32_t cast = networkToUnderlyingCast<int32_t>(sample);
        return ((cast & 0xff000000) >> 24) | ((cast & 0x00ff0000) >> 8) | ((cast & 0x0000ff00) << 8) |
               ((cast & 0x000000ff) << 24);
#else
        return sample_type_cast<int32_t>(sample);
#endif
    }
};

// float32_t -> float32_t

template<>
struct NetworkToSampleConverter<float32_t, float32_t>
{
    static float32_t convert(NetworkSampleValueType_t<float32_t> sample) noexcept
    {
#if defined(RATL_CPP_LITTLE_ENDIAN)
        auto temp1 = networkToUnderlyingCast<float32_t>(sample);
        auto temp2 = NetworkToSampleConverter<int32_t, int32_t>::convert(
            reinterpret_cast<const NetworkSampleValueType_t<int32_t>&>(temp1));
        return reinterpret_cast<const float32_t&>(temp2);
#else
        return sample_type_cast<float32_t>(sample);
#endif
    }
};

// intXX_t -> float32_t

template<class SampleType>
struct NetworkToSampleConverter<SampleType, float32_t>
{
    static float32_t convert(NetworkSampleValueType_t<SampleType> sample) noexcept
    {
        return SampleConverter<SampleType, float32_t>::convert(
            NetworkToSampleConverter<SampleType, SampleType>::convert(sample));
    }
};

// float32_t -> intXX_t

template<class SampleType>
struct NetworkToSampleConverter<float32_t, SampleType>
{
    static SampleType convert(NetworkSampleValueType_t<float32_t> sample) noexcept
    {
        return SampleConverter<float32_t, SampleType>::convert(
            NetworkToSampleConverter<float32_t, float32_t>::convert(sample));
    }
};

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_sample_converter_
