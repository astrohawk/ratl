/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_noninterleaved_iterator_
#define _ratl_detail_noninterleaved_iterator_

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/operator_arrow_proxy.hpp>
#include <ratl/detail/sample_traits.hpp>
#include <ratl/channel_span.hpp>

// other includes
#include <type_traits>

namespace ratl
{
namespace detail
{
template<typename SampleType, typename SampleTraits>
class noninterleaved_iterator
{
    static_assert(
        std::is_same<typename SampleTraits::sample_type, SampleType>::value,
        "sample_type in SampleTraits must be the same type as SampleType");

    using sample_traits = SampleTraits;
    using sample_type = typename sample_traits::sample_type;
    using sample_pointer = typename sample_traits::pointer;

    using channel_type = basic_channel_span<sample_type, sample_traits, std::true_type>;

    using size_type = std::size_t;

    using iter_traits = std::iterator_traits<sample_pointer>;

public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = channel_type;
    using difference_type = typename iter_traits::difference_type;
    using pointer = detail::operator_arrow_proxy<channel_type>;
    using reference = channel_type;

private:
    sample_pointer data_ = nullptr;
    size_type frames_ = 0;

public:
    noninterleaved_iterator() noexcept = default;

    noninterleaved_iterator(sample_pointer data, size_type frames) noexcept : data_(data), frames_(frames) {}

    noninterleaved_iterator(const noninterleaved_iterator& other) noexcept = default;

    noninterleaved_iterator& operator=(const noninterleaved_iterator& other) noexcept = default;

    inline size_type frames() const noexcept
    {
        return frames_;
    }

    inline reference operator*() const noexcept
    {
        return reference(data_, frames_);
    }

    inline pointer operator->() const noexcept
    {
        return pointer(reference(data_, frames_));
    }

    inline reference operator[](difference_type n) const noexcept
    {
        return reference(data_ + (n * frames_), frames_);
    }

    inline noninterleaved_iterator& operator++() noexcept
    {
        data_ += frames_;
        return *this;
    }

    inline noninterleaved_iterator operator++(int) noexcept
    {
        auto&& tmp = noninterleaved_iterator(*this);
        ++(*this);
        return tmp;
    }

    inline noninterleaved_iterator& operator--() noexcept
    {
        data_ -= frames_;
        return *this;
    }

    inline noninterleaved_iterator operator--(int) noexcept
    {
        auto&& tmp = noninterleaved_iterator(*this);
        --(*this);
        return tmp;
    }

    inline noninterleaved_iterator operator+(difference_type n) const noexcept
    {
        auto&& w = noninterleaved_iterator(*this);
        w += n;
        return w;
    }

    inline noninterleaved_iterator& operator+=(difference_type n) noexcept
    {
        data_ += static_cast<difference_type>(n * frames_);
        return *this;
    }

    inline noninterleaved_iterator operator-(difference_type n) const noexcept
    {
        return *this + (-n);
    }

    inline noninterleaved_iterator& operator-=(difference_type n) noexcept
    {
        *this += -n;
        return *this;
    }

#if defined(RATL_CPP_VERSION_HAS_CPP20)

    inline bool operator==(const noninterleaved_iterator& other) const noexcept
    {
        return data_ == other.data_;
    }

    inline bool operator<(const noninterleaved_iterator& other) const noexcept
    {
        return data_ < other.data_;
    }

    inline auto operator<=>(const noninterleaved_iterator& other) const noexcept = default;

#else

    friend inline bool operator==(const noninterleaved_iterator& x, const noninterleaved_iterator& y) noexcept
    {
        return x.data_ == y.data_;
    }

    friend inline bool operator!=(const noninterleaved_iterator& x, const noninterleaved_iterator& y) noexcept
    {
        return !(x == y);
    }

    friend inline bool operator<(const noninterleaved_iterator& x, const noninterleaved_iterator& y) noexcept
    {
        return x.data_ < y.data_;
    }

    friend inline bool operator<=(const noninterleaved_iterator& x, const noninterleaved_iterator& y) noexcept
    {
        return !(x > y);
    }

    friend inline bool operator>(const noninterleaved_iterator& x, const noninterleaved_iterator& y) noexcept
    {
        return y < x;
    }

    friend inline bool operator>=(const noninterleaved_iterator& x, const noninterleaved_iterator& y) noexcept
    {
        return !(x < y);
    }

#endif

    friend inline noninterleaved_iterator operator+(
        typename noninterleaved_iterator::difference_type n, noninterleaved_iterator x)
    {
        x += n;
        return x;
    }

    friend inline typename noninterleaved_iterator::difference_type operator-(
        const noninterleaved_iterator& x, const noninterleaved_iterator& y)
    {
        return (x.data_ - y.data_) / static_cast<typename noninterleaved_iterator::difference_type>(x.frames_);
    }

    inline sample_pointer base() const noexcept
    {
        return data_;
    }
};

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_noninterleaved_iterator_
