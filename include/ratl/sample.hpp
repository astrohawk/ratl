/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_sample_
#define _ratl_sample_

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/sample_value_traits.hpp>

// other includes
#include <ostream>
#include <type_traits>

namespace ratl
{
template<typename SampleValueType>
class sample final
{
    static_assert(
        detail::is_valid_sample_value_type_v<SampleValueType>, "SampleValueType is not a valid sample value type");

public:
    using sample_value_type = SampleValueType;

    using value_type = sample_value_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    sample() noexcept = default;

    constexpr sample(const sample&) noexcept = default;

    constexpr sample(sample&&) noexcept = default;

    constexpr sample& operator=(const sample&) noexcept = default;

    constexpr sample& operator=(sample&&) noexcept = default;

    ~sample() = default;

    template<typename Up, std::enable_if_t<std::is_convertible<Up, value_type>::value, bool> = true>
    constexpr explicit sample(const Up& other) noexcept : sample_(other)
    {
    }

    template<typename Up, std::enable_if_t<std::is_convertible<Up, value_type>::value, bool> = true>
    constexpr sample& operator=(const Up& other) noexcept
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

    inline constexpr auto operator<=>(const sample& other) const noexcept = default;

#else

    friend inline constexpr bool operator==(const sample& x, const sample& y) noexcept
    {
        return x.sample_ == y.sample_;
    }

    friend inline constexpr bool operator!=(const sample& x, const sample& y) noexcept
    {
        return x.sample_ != y.sample_;
    }

    friend inline constexpr bool operator<(const sample& x, const sample& y) noexcept
    {
        return x.sample_ < y.sample_;
    }

    friend inline constexpr bool operator<=(const sample& x, const sample& y) noexcept
    {
        return x.sample_ <= y.sample_;
    }

    friend inline constexpr bool operator>(const sample& x, const sample& y) noexcept
    {
        return x.sample_ > y.sample_;
    }

    friend inline constexpr bool operator>=(const sample& x, const sample& y) noexcept
    {
        return x.sample_ >= y.sample_;
    }

#endif

    friend inline std::ostream& operator<<(std::ostream& os, const sample& sample)
    {
        return os << sample.sample_;
    }

    // not private as gcc requires memcpyable types to not have any private members
    value_type sample_;
};

using sample_int16 = sample<int16_t>;
using sample_int24 = sample<int24_t>;
using sample_int32 = sample<int32_t>;
using sample_float32 = sample<float32_t>;

} // namespace ratl

#endif // _ratl_sample_
