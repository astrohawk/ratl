#ifndef _ratl_int24_
#define _ratl_int24_

#include <ratl/detail/config.hpp>

// C++ Standard Library includes
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

    constexpr inline int24_t(const int32_t& other) noexcept :
        storage_{static_cast<uint8_t>(other), static_cast<uint8_t>(other >> 8), static_cast<uint8_t>(other >> 16)}
    {
    }

    constexpr inline int24_t& operator=(const int32_t& other) noexcept
    {
        storage_[0] = static_cast<uint8_t>(other);
        storage_[1] = static_cast<uint8_t>(other >> 8);
        storage_[2] = static_cast<uint8_t>(other >> 16);
        return *this;
    }

    constexpr inline operator int32_t() const noexcept
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

    // not private as gcc requires memcpyable types to not have any private members
    uint8_t storage_[3];
};

// Assignment operators

template<class Up>
constexpr inline int24_t& operator+=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) + val;
    return x;
}

template<class Up>
constexpr inline int24_t& operator-=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) - val;
    return x;
}

template<class Up>
constexpr inline int24_t& operator*=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) * val;
    return x;
}

template<class Up>
constexpr inline int24_t& operator/=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) / val;
    return x;
}

template<class Up>
constexpr inline int24_t& operator%=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) % val;
    return x;
}

template<class Up>
constexpr inline int24_t& operator&=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) & val;
    return x;
}

template<class Up>
constexpr inline int24_t& operator|=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) | val;
    return x;
}

template<class Up>
constexpr inline int24_t& operator^=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) ^ val;
    return x;
}

template<class Up>
constexpr inline int24_t& operator<<=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) << val;
    return x;
}

template<class Up>
constexpr inline int24_t& operator>>=(int24_t& x, const Up& val) noexcept
{
    x = static_cast<int32_t>(x) >> val;
    return x;
}

// Increment/decrement operators

constexpr inline int24_t& operator++(int24_t& x) noexcept
{
    int32_t tmp = x;
    x = ++tmp;
    return x;
}

constexpr inline int24_t& operator--(int24_t& x) noexcept
{
    int32_t tmp = x;
    x = --tmp;
    return x;
}

constexpr inline int24_t operator++(int24_t& x, int) noexcept
{
    int24_t tmp = x;
    ++x;
    return tmp;
}

constexpr inline int24_t operator--(int24_t& x, int) noexcept
{
    int24_t tmp = x;
    --x;
    return tmp;
}

static_assert(sizeof(int24_t) == 3, "int24_t is not 3 bytes");
static_assert(sizeof(int24_t[2]) == 6, "int24_t is not packed correctly");
static_assert(std::is_trivial<int24_t>::value, "int24_t is not a trivial type");
static_assert(std::is_trivially_copyable<int24_t>::value, "int24_t is not a trivially copyable type");
static_assert(std::is_standard_layout<int24_t>::value, "int24_t is not a standard layout type");
} // namespace ratl

#endif // _ratl_int24_
