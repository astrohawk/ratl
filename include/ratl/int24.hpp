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
#include <type_traits>

// defines
#define RATL_INT24_DIGITS 23
#define RATL_INT24_MIN -8388608
#define RATL_INT24_MAX 8388607

namespace ratl
{
class int24_t final
{
public:
    int24_t() noexcept = default;

    constexpr int24_t(const int24_t&) noexcept = default;

    constexpr int24_t(int24_t&&) noexcept = default;

    constexpr int24_t& operator=(const int24_t&) noexcept = default;

    constexpr int24_t& operator=(int24_t&&) noexcept = default;

    ~int24_t() = default;

    inline constexpr int24_t(const int32_t& other) noexcept :
        storage_{static_cast<uint8_t>(other), static_cast<uint8_t>(other >> 8), static_cast<uint8_t>(other >> 16)}
    {
    }

    inline constexpr int24_t& operator=(const int32_t& other) noexcept;

    inline constexpr operator int32_t() const noexcept;

    // not private as gcc requires memcpyable types to not have any private members
    uint8_t storage_[3];
};

inline constexpr int24_t& int24_t::operator=(const int32_t& other) noexcept
{
    storage_[0] = static_cast<uint8_t>(other);
    storage_[1] = static_cast<uint8_t>(other >> 8);
    storage_[2] = static_cast<uint8_t>(other >> 16);
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

template<class Up>
inline constexpr int24_t& operator+=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) + val;
    return x;
}

template<class Up>
inline constexpr int24_t& operator-=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) - val;
    return x;
}

template<class Up>
inline constexpr int24_t& operator*=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) * val;
    return x;
}

template<class Up>
inline constexpr int24_t& operator/=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) / val;
    return x;
}

template<class Up>
inline constexpr int24_t& operator%=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) % val;
    return x;
}

template<class Up>
inline constexpr int24_t& operator&=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) & val;
    return x;
}

template<class Up>
inline constexpr int24_t& operator|=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) | val;
    return x;
}

template<class Up>
inline constexpr int24_t& operator^=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) ^ val;
    return x;
}

template<class Up>
inline constexpr int24_t& operator<<=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) << val;
    return x;
}

template<class Up>
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
