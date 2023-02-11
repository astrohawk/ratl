/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_network_sample_
#define _ratl_network_sample_

// ratl includes
#include <ratl/detail/cast.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/detail/sample_value_traits.hpp>

// other includes
#include <ostream>
#include <type_traits>

namespace ratl
{
template<typename SampleValueType>
class network_sample final
{
    static_assert(
        detail::is_valid_sample_value_type<SampleValueType>::value, "SampleValueType is not a valid sample value type");

public:
    using sample_value_type = SampleValueType;

    using value_type = detail::network_sample_value_type_t<sample_value_type>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    network_sample() noexcept = default;

    constexpr network_sample(const network_sample&) noexcept = default;

    constexpr network_sample(network_sample&&) noexcept = default;

    constexpr network_sample& operator=(const network_sample&) noexcept = default;

    constexpr network_sample& operator=(network_sample&&) noexcept = default;

    ~network_sample() = default;

    constexpr explicit network_sample(const value_type& other) noexcept : sample_(other) {}

    constexpr network_sample& operator=(const value_type& other) noexcept
    {
        sample_ = other;
        return *this;
    }

    inline constexpr reference get() noexcept
    {
        return sample_;
    }

    inline constexpr const_reference get() const noexcept
    {
        return sample_;
    }

#if defined(RATL_CPP_VERSION_HAS_CPP20)

    inline constexpr bool operator==(const network_sample& other) const noexcept = default;

    inline constexpr bool operator!=(const network_sample& other) const noexcept = default;

#else

    friend inline constexpr bool operator==(const network_sample& x, const network_sample& y) noexcept
    {
        return x.sample_ == y.sample_;
    }

    friend inline constexpr bool operator!=(const network_sample& x, const network_sample& y) noexcept
    {
        return x.sample_ != y.sample_;
    }

#endif

    friend inline std::ostream& operator<<(std::ostream& os, const network_sample& network_sample)
    {
        return os << detail::network_to_network_underlying_cast<sample_value_type>(network_sample.sample_);
    }

    // not private as gcc requires memcpyable types to not have any private members
    value_type sample_;
};

using network_sample_int16 = network_sample<int16_t>;
using network_sample_int24 = network_sample<int24_t>;
using network_sample_int32 = network_sample<int32_t>;
using network_sample_float32 = network_sample<float32_t>;

} // namespace ratl

#endif // _ratl_network_sample_
