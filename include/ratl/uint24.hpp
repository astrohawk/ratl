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
#define RATL_UINT24_MIN static_cast<uint24_t>(0)
#define RATL_UINT24_MAX static_cast<uint24_t>(16777215)

namespace ratl
{
class uint24_t final
{
public:
    uint24_t() noexcept = default;

    constexpr uint24_t(const uint24_t&) noexcept = default;
    constexpr uint24_t& operator=(const uint24_t&) noexcept = default;

    template<
        typename Tp,
        std::enable_if_t<std::is_convertible<Tp, std::uint32_t>::value && sizeof(Tp) >= sizeof(std::uint32_t), bool> =
            true>
    inline explicit constexpr uint24_t(const Tp& other) noexcept;
    template<
        typename Tp,
        std::enable_if_t<std::is_convertible<Tp, std::uint32_t>::value && sizeof(Tp) >= sizeof(std::uint32_t), bool> =
            true>
    inline constexpr uint24_t& operator=(const Tp& other) noexcept;

    ~uint24_t() = default;

    inline constexpr operator int32_t() const noexcept;

    // not private as gcc requires memcpyable types to not have any private members
    uint8_t storage_[3];
};

template<
    typename Tp,
    std::enable_if_t<std::is_convertible<Tp, std::uint32_t>::value && sizeof(Tp) >= sizeof(std::uint32_t), bool>>
inline constexpr uint24_t::uint24_t(const Tp& other) noexcept : storage_{}
{
#if defined(RATL_USE_INT24_MEMCPY_CONVERT)
    if (std::is_constant_evaluated())
#endif
    {
        storage_[0] = static_cast<uint8_t>(other);
        storage_[1] = static_cast<uint8_t>(other >> 8);
        storage_[2] = static_cast<uint8_t>(other >> 16);
    }
#if defined(RATL_USE_INT24_MEMCPY_CONVERT)
    else
    {
        std::memcpy(&storage_, &other, 3);
    }
#endif
}

template<
    typename Tp,
    std::enable_if_t<std::is_convertible<Tp, std::uint32_t>::value && sizeof(Tp) >= sizeof(std::uint32_t), bool>>
inline constexpr uint24_t& uint24_t::operator=(const Tp& other) noexcept
{
#if defined(RATL_USE_INT24_MEMCPY_CONVERT)
    if (std::is_constant_evaluated())
#endif
    {
        storage_[0] = static_cast<uint8_t>(other);
        storage_[1] = static_cast<uint8_t>(other >> 8);
        storage_[2] = static_cast<uint8_t>(other >> 16);
    }
#if defined(RATL_USE_INT24_MEMCPY_CONVERT)
    else
    {
        std::memcpy(&storage_, &other, 3);
    }
#endif
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
