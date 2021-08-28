/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_uint24_
#define _ratl_uint24_

// ratl includes
#include <ratl/detail/config.hpp>

// other includes
#include <cstdint>
#include <type_traits>

// defines
#define RATL_UINT24_DIGITS 24
#define RATL_UINT24_MIN 0
#define RATL_UINT24_MAX 16777215

namespace ratl
{
class uint24_t final
{
public:
    uint24_t() noexcept = default;

    constexpr uint24_t(const uint24_t&) noexcept = default;

    constexpr uint24_t(uint24_t&&) noexcept = default;

    constexpr uint24_t& operator=(const uint24_t&) noexcept = default;

    constexpr uint24_t& operator=(uint24_t&&) noexcept = default;

    ~uint24_t() = default;

    inline constexpr uint24_t(const uint32_t& other) noexcept :
        storage_{static_cast<uint8_t>(other), static_cast<uint8_t>(other >> 8), static_cast<uint8_t>(other >> 16)}
    {
    }

    inline constexpr uint24_t& operator=(const uint32_t& other) noexcept;

    inline constexpr operator int32_t() const noexcept;

    // not private as gcc requires memcpyable types to not have any private members
    uint8_t storage_[3];
};

inline constexpr uint24_t& uint24_t::operator=(const uint32_t& other) noexcept
{
    storage_[0] = static_cast<uint8_t>(other);
    storage_[1] = static_cast<uint8_t>(other >> 8);
    storage_[2] = static_cast<uint8_t>(other >> 16);
    return *this;
}

inline constexpr uint24_t::operator int32_t() const noexcept
{
    return static_cast<int32_t>(
        (static_cast<uint32_t>(storage_[0])) | (static_cast<uint32_t>(storage_[1]) << 8) |
        (static_cast<uint32_t>(storage_[2]) << 16));
}

template<typename Up>
inline constexpr uint24_t& operator+=(uint24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) + val;
    return x;
}

template<typename Up>
inline constexpr uint24_t& operator-=(uint24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) - val;
    return x;
}

template<typename Up>
inline constexpr uint24_t& operator*=(uint24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) * val;
    return x;
}

template<typename Up>
inline constexpr uint24_t& operator/=(uint24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) / val;
    return x;
}

template<typename Up>
inline constexpr uint24_t& operator%=(uint24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) % val;
    return x;
}

template<typename Up>
inline constexpr uint24_t& operator&=(uint24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) & val;
    return x;
}

template<typename Up>
inline constexpr uint24_t& operator|=(uint24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) | val;
    return x;
}

template<typename Up>
inline constexpr uint24_t& operator^=(uint24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) ^ val;
    return x;
}

template<typename Up>
inline constexpr uint24_t& operator<<=(uint24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) << val;
    return x;
}

template<typename Up>
inline constexpr uint24_t& operator>>=(uint24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) >> val;
    return x;
}

inline constexpr uint24_t& operator++(uint24_t& x) noexcept
{
    int32_t tmp = x;
    x = ++tmp;
    return x;
}

inline constexpr uint24_t& operator--(uint24_t& x) noexcept
{
    int32_t tmp = x;
    x = --tmp;
    return x;
}

inline constexpr uint24_t operator++(uint24_t& x, int) noexcept
{
    uint24_t tmp = x;
    ++x;
    return tmp;
}

inline constexpr uint24_t operator--(uint24_t& x, int) noexcept
{
    uint24_t tmp = x;
    --x;
    return tmp;
}

} // namespace ratl

#endif // _ratl_uint24_
