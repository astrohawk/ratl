#ifndef _ratl_type_traits_
#define _ratl_type_traits_

// C++ Standard Library includes
#include <array>
#include <type_traits>

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/type_traits.hpp>
#include <ratl/detail/types.hpp>
#include <ratl/types.hpp>

namespace ratl
{
namespace detail
{
template<class...>
struct void_impl
{
    using type = void;
};

/*
 * std::void_t reimplementation for C++14.
 * Supports CWG 1558
 */
template<class... Ts>
using void_t = typename detail::void_impl<Ts...>::type;

template<class SampleType>
struct IsSampleTypeImpl : public std::false_type
{
};
template<>
struct IsSampleTypeImpl<int16_t> : public std::true_type
{
};
template<>
struct IsSampleTypeImpl<int24_t> : public std::true_type
{
};
template<>
struct IsSampleTypeImpl<int32_t> : public std::true_type
{
};
template<>
struct IsSampleTypeImpl<float32_t> : public std::true_type
{
};

// SampleTypeDecay

template<class SampleType>
struct SampleTypeDecay
{
    using type = std::remove_cv_t<SampleType>;

    static_assert(detail::IsSampleTypeImpl<type>::value, "SampleType is not a valid Sample type");
};

// SampleTypeDecay_t

template<class SampleType>
using SampleTypeDecay_t = typename SampleTypeDecay<SampleType>::type;

// IsSampleType

template<class SampleType>
struct IsSampleType : public detail::IsSampleTypeImpl<SampleTypeDecay_t<SampleType>>
{
};

// IsSampleType_v

template<class SampleType>
static constexpr bool IsSampleType_v = IsSampleType<SampleType>::value;

// NetworkSampleValueUnderlyingType

template<class SampleType>
struct NetworkSampleValueUnderlyingType;

template<>
struct NetworkSampleValueUnderlyingType<int16_t>
{
    using type = uint16_t;
};

template<>
struct NetworkSampleValueUnderlyingType<int24_t>
{
    using type = uint24_t;
};

template<>
struct NetworkSampleValueUnderlyingType<int32_t>
{
    using type = uint32_t;
};

template<>
struct NetworkSampleValueUnderlyingType<float32_t>
{
    using type = uint32_t;
};

// NetworkSampleValueUnderlyingType_t

template<class SampleType>
using NetworkSampleValueUnderlyingType_t = typename NetworkSampleValueUnderlyingType<SampleType>::type;

// NetworkSampleValueType

template<class SampleType>
struct NetworkSampleValueType
{
    static_assert(IsSampleType_v<SampleType>, "SampleType is not a valid Sample type");

    // opaque type
    class type final
    {
    public:
        constexpr bool operator==(const type& other) const noexcept
        {
            return storage_ == other.storage_;
        }

        constexpr bool operator!=(const type& other) const noexcept
        {
            return storage_ != other.storage_;
        }

        NetworkSampleValueUnderlyingType_t<SampleType> storage_;
    };
};

// NetworkSampleValueType_t

template<class SampleType>
using NetworkSampleValueType_t = typename NetworkSampleValueType<SampleType>::type;

} // namespace detail
} // namespace ratl

#endif // _ratl_type_traits_
