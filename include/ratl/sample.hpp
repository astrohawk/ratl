#ifndef _ratl_sample_
#define _ratl_sample_

// C++ Standard Library includes
#include <ostream>
#include <type_traits>

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/sample_value_traits.hpp>

namespace ratl
{
template<class SampleValueType>
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

    template<class Up, class = typename std::enable_if<std::is_convertible<Up, value_type>::value>::type>
    constexpr explicit sample(const Up& other) noexcept : sample_(other)
    {
    }

    template<class Up, class = typename std::enable_if<std::is_convertible<Up, value_type>::value>::type>
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

    inline bool operator==(const sample& other) const noexcept
    {
        return sample_ == other.sample_;
    }

    inline bool operator!=(const sample& other) const noexcept
    {
        return sample_ != other.sample_;
    }

    inline bool operator<(const sample& other) const noexcept
    {
        return sample_ < other.sample_;
    }

    inline bool operator<=(const sample& other) const noexcept
    {
        return sample_ <= other.sample_;
    }

    inline bool operator>(const sample& other) const noexcept
    {
        return sample_ > other.sample_;
    }

    inline bool operator>=(const sample& other) const noexcept
    {
        return sample_ >= other.sample_;
    }

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

static_assert(std::is_trivial<sample<int16_t>>::value, "sample<int16_t> is not a trivial type");
static_assert(
    std::is_trivially_default_constructible<sample<int16_t>>::value,
    "sample<int16_t> is not a trivially default constructible type");
static_assert(std::is_trivially_copyable<sample<int16_t>>::value, "sample<int16_t> is not a trivially copyable type");
static_assert(std::is_standard_layout<sample<int16_t>>::value, "sample<int16_t> is not a standard layout type");
static_assert(std::is_trivial<sample<int24_t>>::value, "sample<int24_t> is not a trivial type");
static_assert(
    std::is_trivially_default_constructible<sample<int24_t>>::value,
    "sample<int24_t> is not a trivially default constructible type");
static_assert(std::is_trivially_copyable<sample<int24_t>>::value, "sample<int24_t> is not a trivially copyable type");
static_assert(std::is_standard_layout<sample<int24_t>>::value, "sample<int24_t> is not a standard layout type");
static_assert(std::is_trivial<sample<int32_t>>::value, "sample<int32_t> is not a trivial type");
static_assert(
    std::is_trivially_default_constructible<sample<int32_t>>::value,
    "sample<int32_t> is not a trivially default constructible type");
static_assert(std::is_trivially_copyable<sample<int32_t>>::value, "sample<int32_t> is not a trivially copyable type");
static_assert(std::is_standard_layout<sample<int32_t>>::value, "sample<int32_t> is not a standard layout type");
static_assert(std::is_trivial<sample<float32_t>>::value, "sample<float32_t> is not a trivial type");
static_assert(
    std::is_trivially_default_constructible<sample<float32_t>>::value,
    "sample<float32_t> is not a trivially default constructible type");
static_assert(
    std::is_trivially_copyable<sample<float32_t>>::value, "sample<float32_t> is not a trivially copyable type");
static_assert(std::is_standard_layout<sample<float32_t>>::value, "sample<float32_t> is not a standard layout type");

using sample_int16 = sample<int16_t>;
using sample_int24 = sample<int24_t>;
using sample_int32 = sample<int32_t>;
using sample_float32 = sample<float32_t>;

} // namespace ratl

#endif // _ratl_sample_
