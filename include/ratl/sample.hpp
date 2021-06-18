#ifndef _ratl_sample_
#define _ratl_sample_

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
template<class Tp>
class Sample final
{
public:
    using sample_type = detail::SampleTypeDecay_t<Tp>;

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
    Sample& operator=(const Up& other) noexcept
    {
        sample_ = other;
        return *this;
    }

    template<class Up>
    Sample(const Sample<Up>& other) noexcept : sample_(convert<Up>(other.get()))
    {
    }

    template<class Up>
    Sample& operator=(const Sample<Up>& other) noexcept
    {
        sample_ = convert<Up>(other.get());
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

    bool operator==(const Sample& other) const noexcept
    {
        return sample_ == other.sample_;
    }

    bool operator!=(const Sample& other) const noexcept
    {
        return sample_ != other.sample_;
    }

    bool operator<(const Sample& other) const noexcept
    {
        return sample_ < other.sample_;
    }

    bool operator<=(const Sample& other) const noexcept
    {
        return sample_ <= other.sample_;
    }

    bool operator>(const Sample& other) const noexcept
    {
        return sample_ > other.sample_;
    }

    bool operator>=(const Sample& other) const noexcept
    {
        return sample_ >= other.sample_;
    }

    friend std::ostream& operator<<(std::ostream& os, const Sample& sample)
    {
        return os << sample.sample_;
    }

    // not private as gcc requires memcpyable types to not have any private members
    value_type sample_;

private:
    template<class Up>
    static sample_type convert(detail::SampleTypeDecay_t<Up> other)
    {
        detail::NullDitherGenerator dither_generator;
        return detail::SampleConverter<detail::SampleTypeDecay_t<Up>, sample_type, detail::NullDitherGenerator>::
            convert(other, dither_generator);
    }
};

static_assert(std::is_trivial<Sample<int16_t>>::value, "Sample<int16_t> is not a trivial type");
static_assert(std::is_trivially_copyable<Sample<int16_t>>::value, "Sample<int16_t> is not a trivially copyable type");
static_assert(std::is_standard_layout<Sample<int16_t>>::value, "Sample<int16_t> is not a standard layout type");
static_assert(std::is_trivial<Sample<int24_t>>::value, "Sample<int24_t> is not a trivial type");
static_assert(std::is_trivially_copyable<Sample<int24_t>>::value, "Sample<int24_t> is not a trivially copyable type");
static_assert(std::is_standard_layout<Sample<int24_t>>::value, "Sample<int24_t> is not a standard layout type");
static_assert(std::is_trivial<Sample<int32_t>>::value, "Sample<int32_t> is not a trivial type");
static_assert(std::is_trivially_copyable<Sample<int32_t>>::value, "Sample<int32_t> is not a trivially copyable type");
static_assert(std::is_standard_layout<Sample<int32_t>>::value, "Sample<int32_t> is not a standard layout type");
static_assert(std::is_trivial<Sample<float32_t>>::value, "Sample<float32_t> is not a trivial type");
static_assert(
    std::is_trivially_copyable<Sample<float32_t>>::value, "Sample<float32_t> is not a trivially copyable type");
static_assert(std::is_standard_layout<Sample<float32_t>>::value, "Sample<float32_t> is not a standard layout type");

using SampleInt16 = Sample<int16_t>;
using SampleInt24 = Sample<int24_t>;
using SampleInt32 = Sample<int32_t>;
using SampleFloat32 = Sample<float32_t>;

} // namespace ratl

#endif // _ratl_sample_
