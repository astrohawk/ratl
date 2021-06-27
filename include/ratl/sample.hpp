#ifndef _ratl_sample_
#define _ratl_sample_

// C++ Standard Library includes
#include <ostream>
#include <type_traits>

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/type_traits.hpp>

namespace ratl
{
template<class SampleType>
class Sample final
{
public:
    using sample_type = detail::SampleTypeDecay_t<SampleType>;

    using value_type = sample_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    Sample() noexcept = default;

    constexpr Sample(const Sample&) noexcept = default;

    constexpr Sample(Sample&&) noexcept = default;

    constexpr Sample& operator=(const Sample&) noexcept = default;

    constexpr Sample& operator=(Sample&&) noexcept = default;

    ~Sample() = default;

    template<class Up, class = typename std::enable_if<std::is_convertible<Up, value_type>::value>::type>
    constexpr explicit Sample(const Up& other) noexcept : sample_(other)
    {
    }

    template<class Up, class = typename std::enable_if<std::is_convertible<Up, value_type>::value>::type>
    constexpr Sample& operator=(const Up& other) noexcept
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

    inline bool operator==(const Sample& other) const noexcept
    {
        return sample_ == other.sample_;
    }

    inline bool operator!=(const Sample& other) const noexcept
    {
        return sample_ != other.sample_;
    }

    inline bool operator<(const Sample& other) const noexcept
    {
        return sample_ < other.sample_;
    }

    inline bool operator<=(const Sample& other) const noexcept
    {
        return sample_ <= other.sample_;
    }

    inline bool operator>(const Sample& other) const noexcept
    {
        return sample_ > other.sample_;
    }

    inline bool operator>=(const Sample& other) const noexcept
    {
        return sample_ >= other.sample_;
    }

#else

    friend inline constexpr bool operator==(const Sample& x, const Sample& y) noexcept
    {
        return x.sample_ == y.sample_;
    }

    friend inline constexpr bool operator!=(const Sample& x, const Sample& y) noexcept
    {
        return x.sample_ != y.sample_;
    }

    friend inline constexpr bool operator<(const Sample& x, const Sample& y) noexcept
    {
        return x.sample_ < y.sample_;
    }

    friend inline constexpr bool operator<=(const Sample& x, const Sample& y) noexcept
    {
        return x.sample_ <= y.sample_;
    }

    friend inline constexpr bool operator>(const Sample& x, const Sample& y) noexcept
    {
        return x.sample_ > y.sample_;
    }

    friend inline constexpr bool operator>=(const Sample& x, const Sample& y) noexcept
    {
        return x.sample_ >= y.sample_;
    }

#endif

    friend inline std::ostream& operator<<(std::ostream& os, const Sample& sample)
    {
        return os << sample.sample_;
    }

    // not private as gcc requires memcpyable types to not have any private members
    value_type sample_;
};

static_assert(std::is_trivial<Sample<int16_t>>::value, "Sample<int16_t> is not a trivial type");
static_assert(
    std::is_trivially_default_constructible<Sample<int16_t>>::value,
    "Sample<int16_t> is not a trivially default constructible type");
static_assert(std::is_trivially_copyable<Sample<int16_t>>::value, "Sample<int16_t> is not a trivially copyable type");
static_assert(std::is_standard_layout<Sample<int16_t>>::value, "Sample<int16_t> is not a standard layout type");
static_assert(std::is_trivial<Sample<int24_t>>::value, "Sample<int24_t> is not a trivial type");
static_assert(
    std::is_trivially_default_constructible<Sample<int24_t>>::value,
    "Sample<int24_t> is not a trivially default constructible type");
static_assert(std::is_trivially_copyable<Sample<int24_t>>::value, "Sample<int24_t> is not a trivially copyable type");
static_assert(std::is_standard_layout<Sample<int24_t>>::value, "Sample<int24_t> is not a standard layout type");
static_assert(std::is_trivial<Sample<int32_t>>::value, "Sample<int32_t> is not a trivial type");
static_assert(
    std::is_trivially_default_constructible<Sample<int32_t>>::value,
    "Sample<int32_t> is not a trivially default constructible type");
static_assert(std::is_trivially_copyable<Sample<int32_t>>::value, "Sample<int32_t> is not a trivially copyable type");
static_assert(std::is_standard_layout<Sample<int32_t>>::value, "Sample<int32_t> is not a standard layout type");
static_assert(std::is_trivial<Sample<float32_t>>::value, "Sample<float32_t> is not a trivial type");
static_assert(
    std::is_trivially_default_constructible<Sample<float32_t>>::value,
    "Sample<float32_t> is not a trivially default constructible type");
static_assert(
    std::is_trivially_copyable<Sample<float32_t>>::value, "Sample<float32_t> is not a trivially copyable type");
static_assert(std::is_standard_layout<Sample<float32_t>>::value, "Sample<float32_t> is not a standard layout type");

using SampleInt16 = Sample<int16_t>;
using SampleInt24 = Sample<int24_t>;
using SampleInt32 = Sample<int32_t>;
using SampleFloat32 = Sample<float32_t>;

} // namespace ratl

#endif // _ratl_sample_
