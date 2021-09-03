/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_int24_
#define _ratl_int24_

// ratl includes
#include <ratl/detail/config.hpp>

// other includes
#include <cstdint>
#include <cstring>
#include <limits>
#include <type_traits>

// defines
#define RATL_INT24_DIGITS 23
#define RATL_INT24_MIN static_cast<int24_t>(-8388608)
#define RATL_INT24_MAX static_cast<int24_t>(8388607)

namespace ratl
{
class int24_t final
{
public:
    int24_t() noexcept = default;

    constexpr int24_t(const int24_t&) noexcept = default;
    constexpr int24_t& operator=(const int24_t&) noexcept = default;

    static constexpr auto i = std::numeric_limits<int>::max();

    template<
        typename Tp,
        std::enable_if_t<std::is_convertible<Tp, std::int32_t>::value && sizeof(Tp) >= sizeof(std::int32_t), bool> =
            true>
    inline explicit constexpr int24_t(const Tp& other) noexcept;
    template<
        typename Tp,
        std::enable_if_t<std::is_convertible<Tp, std::int32_t>::value && sizeof(Tp) >= sizeof(std::int32_t), bool> =
            true>
    inline constexpr int24_t& operator=(const Tp& other) noexcept;

    ~int24_t() = default;

    inline constexpr operator int32_t() const noexcept;

    // not private as gcc requires memcpyable types to not have any private members
    uint8_t storage_[3];
};

template<
    typename Tp,
    std::enable_if_t<std::is_convertible<Tp, std::int32_t>::value && sizeof(Tp) >= sizeof(std::int32_t), bool>>
inline constexpr int24_t::int24_t(const Tp& other) noexcept : storage_{}
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
    std::enable_if_t<std::is_convertible<Tp, std::int32_t>::value && sizeof(Tp) >= sizeof(std::int32_t), bool>>
inline constexpr int24_t& int24_t::operator=(const Tp& other) noexcept
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

inline constexpr int24_t::operator int32_t() const noexcept
{
#if defined(RATL_CPP_COMPILER_GCC)
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#    pragma GCC diagnostic ignored "-Warray-bounds"
#endif
    return static_cast<int32_t>(
        (static_cast<uint32_t>(storage_[0])) | (static_cast<uint32_t>(storage_[1]) << 8) |
        (static_cast<uint32_t>(static_cast<int32_t>(static_cast<int8_t>(storage_[2]))) << 16));
#if defined(RATL_CPP_COMPILER_GCC)
#    pragma GCC diagnostic pop
#endif
}

template<typename Up>
inline constexpr int24_t& operator+=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) + val;
    return x;
}

template<typename Up>
inline constexpr int24_t& operator-=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) - val;
    return x;
}

template<typename Up>
inline constexpr int24_t& operator*=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) * val;
    return x;
}

template<typename Up>
inline constexpr int24_t& operator/=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) / val;
    return x;
}

template<typename Up>
inline constexpr int24_t& operator%=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) % val;
    return x;
}

template<typename Up>
inline constexpr int24_t& operator&=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) & val;
    return x;
}

template<typename Up>
inline constexpr int24_t& operator|=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) | val;
    return x;
}

template<typename Up>
inline constexpr int24_t& operator^=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) ^ val;
    return x;
}

template<typename Up>
inline constexpr int24_t& operator<<=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) << val;
    return x;
}

template<typename Up>
inline constexpr int24_t& operator>>=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) >> val;
    return x;
}

inline constexpr int24_t& operator++(int24_t& x) noexcept
{
    int32_t tmp = x;
    x = ++tmp;
    return x;
}

inline constexpr int24_t& operator--(int24_t& x) noexcept
{
    int32_t tmp = x;
    x = --tmp;
    return x;
}

inline constexpr int24_t operator++(int24_t& x, int) noexcept
{
    int24_t tmp = x;
    ++x;
    return tmp;
}

inline constexpr int24_t operator--(int24_t& x, int) noexcept
{
    int24_t tmp = x;
    --x;
    return tmp;
}

} // namespace ratl

#endif // _ratl_int24_
