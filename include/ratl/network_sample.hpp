#ifndef _ratl_network_sample_
#define _ratl_network_sample_

// C++ Standard Library includes
#include <ostream>
#include <type_traits>

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/dither_generator.hpp>
#include <ratl/detail/sample_converter.hpp>
#include <ratl/detail/type_traits.hpp>

namespace ratl
{
template<class SampleType>
class NetworkSample final
{
public:
    using sample_type = detail::SampleTypeDecay_t<SampleType>;

    using value_type = detail::NetworkSampleValueType_t<SampleType>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    NetworkSample() noexcept = default;

    constexpr NetworkSample(const NetworkSample&) noexcept = default;

    constexpr NetworkSample(NetworkSample&&) noexcept = default;

    constexpr NetworkSample& operator=(const NetworkSample&) noexcept = default;

    constexpr NetworkSample& operator=(NetworkSample&&) noexcept = default;

    ~NetworkSample() = default;

    constexpr explicit NetworkSample(const value_type& other) noexcept : sample_(other) {}

    NetworkSample& operator=(const value_type& other) noexcept
    {
        sample_ = other;
        return *this;
    }

    reference get() noexcept
    {
        return sample_;
    }

    const_reference get() const noexcept
    {
        return sample_;
    }

    bool operator==(const NetworkSample& other) const noexcept
    {
        return sample_ == other.sample_;
    }

    bool operator!=(const NetworkSample& other) const noexcept
    {
        return sample_ != other.sample_;
    }

    bool operator<(const NetworkSample& other) const noexcept
    {
        return sample_ < other.sample_;
    }

    bool operator<=(const NetworkSample& other) const noexcept
    {
        return sample_ <= other.sample_;
    }

    bool operator>(const NetworkSample& other) const noexcept
    {
        return sample_ > other.sample_;
    }

    bool operator>=(const NetworkSample& other) const noexcept
    {
        return sample_ >= other.sample_;
    }

    friend std::ostream& operator<<(std::ostream& os, const NetworkSample& network_sample)
    {
        detail::NullDitherGenerator dither_generator;
        return os << detail::NetworkToSampleConverter<SampleType, SampleType, detail::NullDitherGenerator>::convert(
                   network_sample.sample_, dither_generator);
    }

    // not private as gcc requires memcpyable types to not have any private members
    value_type sample_;
};

static_assert(std::is_trivial<NetworkSample<int16_t>>::value, "NetworkSample<int16_t> is not a trivial type");
static_assert(
    std::is_trivially_copyable<NetworkSample<int16_t>>::value,
    "NetworkSample<int16_t> is not a trivially copyable type");
static_assert(
    std::is_standard_layout<NetworkSample<int16_t>>::value, "NetworkSample<int16_t> is not a standard layout type");
static_assert(std::is_trivial<NetworkSample<int24_t>>::value, "NetworkSample<int24_t> is not a trivial type");
static_assert(
    std::is_trivially_copyable<NetworkSample<int24_t>>::value,
    "NetworkSample<int24_t> is not a trivially copyable type");
static_assert(
    std::is_standard_layout<NetworkSample<int24_t>>::value, "NetworkSample<int24_t> is not a standard layout type");
static_assert(std::is_trivial<NetworkSample<int32_t>>::value, "NetworkSample<int32_t> is not a trivial type");
static_assert(
    std::is_trivially_copyable<NetworkSample<int32_t>>::value,
    "NetworkSample<int32_t> is not a trivially copyable type");
static_assert(
    std::is_standard_layout<NetworkSample<int32_t>>::value, "NetworkSample<int32_t> is not a standard layout type");
static_assert(std::is_trivial<NetworkSample<float32_t>>::value, "NetworkSample<float32_t> is not a trivial type");
static_assert(
    std::is_trivially_copyable<NetworkSample<float32_t>>::value,
    "NetworkSample<float32_t> is not a trivially copyable type");
static_assert(
    std::is_standard_layout<NetworkSample<float32_t>>::value, "NetworkSample<float32_t> is not a standard layout type");

using NetworkSampleInt16 = NetworkSample<int16_t>;
using NetworkSampleInt24 = NetworkSample<int24_t>;
using NetworkSampleInt32 = NetworkSample<int32_t>;
using NetworkSampleFloat32 = NetworkSample<float32_t>;

} // namespace ratl

#endif // _ratl_network_sample_
