#ifndef _ratl_detail_cast_
#define _ratl_detail_cast_

// C++ Standard Library includes
#include <cstring>
#include <type_traits>
#if defined(RATL_CPP_VERSION_HAS_CPP20)
#    include <bit>
#endif

// ratl includes
#include <ratl/detail/sample_value_traits.hpp>

namespace ratl
{
namespace detail
{
template<class OutputType, class InputType>
inline typename std::enable_if_t<
    sizeof(OutputType) == sizeof(InputType) && std::is_trivially_copyable<InputType>::value &&
        std::is_trivially_copyable<OutputType>::value,
    OutputType>
bit_cast(const InputType& input) noexcept
{
    static_assert(
        std::is_trivially_constructible<OutputType>::value, "destination type must be trivially constructible");

#if defined(RATL_CPP_VERSION_HAS_CPP20)
    return std::bit_cast<OutputType>(input);
#else
    OutputType output;
#    if defined(RATL_CPP_COMPILER_CLANG) || defined(RATL_CPP_COMPILER_GCC)
    __builtin_memcpy(&output, &input, sizeof(OutputType));
#    else
    std::memcpy(&output, &input, sizeof(OutputType));
#    endif
    return output;
#endif
}

// sample_to_network_underlying_cast

template<class SampleValueType>
inline network_sample_value_underlying_type_t<SampleValueType> sample_to_network_underlying_cast(
    SampleValueType input) noexcept
{
    return bit_cast<network_sample_value_underlying_type_t<SampleValueType>>(input);
}

// network_underlying_to_sample_cast

template<class SampleValueType>
inline SampleValueType network_underlying_to_sample_cast(
    network_sample_value_underlying_type_t<SampleValueType> input) noexcept
{
    return bit_cast<SampleValueType>(input);
}

// network_to_network_underlying_cast

template<class SampleValueType>
inline network_sample_value_underlying_type_t<SampleValueType> network_to_network_underlying_cast(
    network_sample_value_type_t<SampleValueType> input) noexcept
{
    return bit_cast<network_sample_value_underlying_type_t<SampleValueType>>(input);
}

// network_underlying_to_network_cast

template<class SampleValueType>
inline network_sample_value_type_t<SampleValueType> network_underlying_to_network_cast(
    network_sample_value_underlying_type_t<SampleValueType> input) noexcept
{
    return bit_cast<network_sample_value_type_t<SampleValueType>>(input);
}

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_cast_
