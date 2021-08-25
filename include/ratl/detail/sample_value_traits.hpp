#ifndef _ratl_detail_sample_value_traits_
#define _ratl_detail_sample_value_traits_

// C++ Standard Library includes
#include <array>
#include <type_traits>

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/sample_value_traits.hpp>
#include <ratl/types.hpp>

namespace ratl
{
namespace detail
{
// is_valid_sample_value_type_impl

template<class SampleValueType>
struct is_valid_sample_value_type_impl : public std::false_type
{
};
template<>
struct is_valid_sample_value_type_impl<int16_t> : public std::true_type
{
};
template<>
struct is_valid_sample_value_type_impl<int24_t> : public std::true_type
{
};
template<>
struct is_valid_sample_value_type_impl<int32_t> : public std::true_type
{
};
template<>
struct is_valid_sample_value_type_impl<float32_t> : public std::true_type
{
};

// is_valid_sample_value_type

template<class SampleValueType>
struct is_valid_sample_value_type : public detail::is_valid_sample_value_type_impl<SampleValueType>
{
};

// is_valid_sample_value_type_v

template<class SampleValueType>
static constexpr bool is_valid_sample_value_type_v = is_valid_sample_value_type<SampleValueType>::value;

// network_sample_value_underlying_type

template<class SampleValueType>
struct network_sample_value_underlying_type;

template<>
struct network_sample_value_underlying_type<int16_t>
{
    using type = std::uint16_t;
};

template<>
struct network_sample_value_underlying_type<int24_t>
{
    using type = ratl::uint24_t;
};

template<>
struct network_sample_value_underlying_type<int32_t>
{
    using type = std::uint32_t;
};

template<>
struct network_sample_value_underlying_type<float32_t>
{
    using type = std::uint32_t;
};

// network_sample_value_underlying_type_t

template<class SampleValueType>
using network_sample_value_underlying_type_t = typename network_sample_value_underlying_type<SampleValueType>::type;

// network_sample_value_type

template<class SampleValueType>
struct network_sample_value_type
{
    static_assert(is_valid_sample_value_type_v<SampleValueType>, "sample_type is not a valid sample type");

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

        network_sample_value_underlying_type_t<SampleValueType> storage_;
    };
};

// network_sample_value_type_t

template<class SampleValueType>
using network_sample_value_type_t = typename network_sample_value_type<SampleValueType>::type;

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_sample_value_traits_
