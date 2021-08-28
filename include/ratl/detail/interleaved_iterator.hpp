/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_interleaved_iterator_
#define _ratl_detail_interleaved_iterator_

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/operator_arrow_proxy.hpp>
#include <ratl/frame_span.hpp>

// other includes
#include <type_traits>

namespace ratl
{
namespace detail
{
template<typename SampleTraits>
class interleaved_iterator
{
    using sample_traits = SampleTraits;
    using sample_type = typename sample_traits::sample_type;
    using sample_pointer = typename sample_traits::pointer;

    using frame_type = basic_frame_span<SampleTraits, true>;

    using size_type = std::size_t;

    using iter_traits = std::iterator_traits<sample_pointer>;

public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = frame_type;
    using difference_type = typename iter_traits::difference_type;
    using pointer = detail::operator_arrow_proxy<frame_type>;
    using reference = frame_type;

private:
    sample_pointer data_ = nullptr;
    size_type channels_ = 0;

public:
    interleaved_iterator() noexcept = default;

    interleaved_iterator(sample_pointer data, size_type channels) noexcept : data_(data), channels_(channels) {}

    interleaved_iterator(const interleaved_iterator& other) noexcept = default;

    interleaved_iterator& operator=(const interleaved_iterator& other) noexcept = default;

    inline size_type channels() const noexcept
    {
        return channels_;
    }

    inline reference operator*() const noexcept
    {
        return reference(data_, channels_);
    }

    inline pointer operator->() const noexcept
    {
        return pointer(reference(data_, channels_));
    }

    inline reference operator[](difference_type n) const noexcept
    {
        return reference(data_ + (n * channels_), channels_);
    }

    inline interleaved_iterator& operator++() noexcept
    {
        data_ += channels_;
        return *this;
    }

    inline interleaved_iterator operator++(int) noexcept
    {
        auto&& tmp = interleaved_iterator(*this);
        ++(*this);
        return tmp;
    }

    inline interleaved_iterator& operator--() noexcept
    {
        data_ -= channels_;
        return *this;
    }

    inline interleaved_iterator operator--(int) noexcept
    {
        auto&& tmp = interleaved_iterator(*this);
        --(*this);
        return tmp;
    }

    inline interleaved_iterator operator+(difference_type n) const noexcept
    {
        auto&& w = interleaved_iterator(*this);
        w += n;
        return w;
    }

    inline interleaved_iterator& operator+=(difference_type n) noexcept
    {
        data_ += static_cast<difference_type>(n * channels_);
        return *this;
    }

    inline interleaved_iterator operator-(difference_type n) const noexcept
    {
        return *this + (-n);
    }

    inline interleaved_iterator& operator-=(difference_type n) noexcept
    {
        *this += -n;
        return *this;
    }

#if defined(RATL_CPP_VERSION_HAS_CPP20)

    inline bool operator==(const interleaved_iterator& other) const noexcept
    {
        return data_ == other.data_;
    }

    inline bool operator<(const interleaved_iterator& other) const noexcept
    {
        return data_ < other.data_;
    }

    inline auto operator<=>(const interleaved_iterator& other) const noexcept = default;

#else

    friend inline bool operator==(const interleaved_iterator& x, const interleaved_iterator& y) noexcept
    {
        return x.data_ == y.data_;
    }

    friend inline bool operator!=(const interleaved_iterator& x, const interleaved_iterator& y) noexcept
    {
        return !(x == y);
    }

    friend inline bool operator<(const interleaved_iterator& x, const interleaved_iterator& y) noexcept
    {
        return x.data_ < y.data_;
    }

    friend inline bool operator<=(const interleaved_iterator& x, const interleaved_iterator& y) noexcept
    {
        return !(x > y);
    }

    friend inline bool operator>(const interleaved_iterator& x, const interleaved_iterator& y) noexcept
    {
        return y < x;
    }

    friend inline bool operator>=(const interleaved_iterator& x, const interleaved_iterator& y) noexcept
    {
        return !(x < y);
    }

#endif

    friend inline interleaved_iterator operator+(
        typename interleaved_iterator::difference_type n, interleaved_iterator x)
    {
        x += n;
        return x;
    }

    friend inline typename interleaved_iterator::difference_type operator-(
        const interleaved_iterator& x, const interleaved_iterator& y)
    {
        return (x.data_ - y.data_) / static_cast<typename interleaved_iterator::difference_type>(x.channels_);
    }

    inline sample_pointer base() const noexcept
    {
        return data_;
    }
};

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_interleaved_iterator_
