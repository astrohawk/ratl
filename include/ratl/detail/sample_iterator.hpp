/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_sample_iterator_
#define _ratl_detail_sample_iterator_

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/sample_traits.hpp>
#include <ratl/sample.hpp>

// other includes
#include <type_traits>

namespace ratl
{
namespace detail
{
template<typename Tag, typename SampleType, typename PointerTraits, bool Contiguous>
class sample_iterator;

template<typename Tag, typename SampleType, typename PointerTraits>
class sample_iterator<Tag, SampleType, PointerTraits, false>
{
    using sample_traits = detail::sample_traits<SampleType>;
    using sample_type = typename sample_traits::sample_type;
    using sample_pointer = detail::sample_pointer_select_t<sample_type, PointerTraits>;

    using iter_traits = std::iterator_traits<sample_pointer>;

    using size_type = std::size_t;

public:
    using iterator_category = typename iter_traits::iterator_category;
    using value_type = typename iter_traits::value_type;
    using difference_type = typename iter_traits::difference_type;
    using pointer = typename iter_traits::pointer;
    using reference = typename iter_traits::reference;

private:
    sample_pointer data_ = nullptr;
    size_type stride_ = 0;

public:
    sample_iterator() noexcept = default;

    sample_iterator(sample_pointer data, size_type stride) noexcept : data_(data), stride_(stride) {}

    sample_iterator(const sample_iterator& other) noexcept = default;

    sample_iterator& operator=(const sample_iterator& other) noexcept = default;

    inline size_type stride() const noexcept
    {
        return stride_;
    }

    inline reference operator*() const noexcept
    {
        return *data_;
    }

    inline pointer operator->() const noexcept
    {
        return data_;
    }

    inline reference operator[](difference_type n) const noexcept
    {
        return data_[n * stride_];
    }

    inline sample_iterator& operator++() noexcept
    {
        data_ += stride_;
        return *this;
    }

    inline sample_iterator operator++(int) noexcept
    {
        auto&& tmp = sample_iterator(*this);
        ++(*this);
        return tmp;
    }

    inline sample_iterator& operator--() noexcept
    {
        data_ -= stride_;
        return *this;
    }

    inline sample_iterator operator--(int) noexcept
    {
        auto&& tmp = sample_iterator(*this);
        --(*this);
        return tmp;
    }

    inline sample_iterator operator+(difference_type n) const noexcept
    {
        auto&& w = sample_iterator(*this);
        w += n;
        return w;
    }

    inline sample_iterator& operator+=(difference_type n) noexcept
    {
        data_ += static_cast<difference_type>(n * stride_);
        return *this;
    }

    inline sample_iterator operator-(difference_type n) const noexcept
    {
        return *this + (-n);
    }

    inline sample_iterator& operator-=(difference_type n) noexcept
    {
        *this += -n;
        return *this;
    }

#if defined(RATL_CPP_VERSION_HAS_CPP20)

    inline bool operator==(const sample_iterator& other) const noexcept
    {
        return data_ == other.data_;
    }

    inline bool operator<(const sample_iterator& other) const noexcept
    {
        return data_ < other.data_;
    }

    inline auto operator<=>(const sample_iterator& other) const noexcept = default;

#else

    friend inline bool operator==(const sample_iterator& x, const sample_iterator& y) noexcept
    {
        return x.data_ == y.data_;
    }

    friend inline bool operator!=(const sample_iterator& x, const sample_iterator& y) noexcept
    {
        return x.data_ != y.data_;
    }

    friend inline bool operator<(const sample_iterator& x, const sample_iterator& y) noexcept
    {
        return x.data_ < y.data_;
    }

    friend inline bool operator<=(const sample_iterator& x, const sample_iterator& y) noexcept
    {
        return x.data_ <= y.data_;
    }

    friend inline bool operator>(const sample_iterator& x, const sample_iterator& y) noexcept
    {
        return x.data_ > y.data_;
    }

    friend inline bool operator>=(const sample_iterator& x, const sample_iterator& y) noexcept
    {
        return x.data_ >= y.data_;
    }

#endif

    friend inline sample_iterator operator+(typename sample_iterator::difference_type n, sample_iterator x)
    {
        x += n;
        return x;
    }

    friend inline typename sample_iterator::difference_type operator-(
        const sample_iterator& x, const sample_iterator& y)
    {
        return (x.data_ - y.data_) / static_cast<typename sample_iterator::difference_type>(x.stride_);
    }

    inline pointer base() const noexcept
    {
        return data_;
    }
};

template<typename Tag, typename SampleType, typename PointerTraits>
class sample_iterator<Tag, SampleType, PointerTraits, true>
{
    using sample_traits = detail::sample_traits<SampleType>;
    using sample_type = typename sample_traits::sample_type;
    using sample_pointer = detail::sample_pointer_select_t<sample_type, PointerTraits>;

    using iter_traits = std::iterator_traits<sample_pointer>;

    using size_type = std::size_t;

public:
#if defined(RATL_CPP_VERSION_HAS_CPP20)
    using iterator_concept = std::contiguous_iterator_tag;
    using element_type = sample_type;
#endif
    using iterator_category = typename iter_traits::iterator_category;
    using value_type = typename iter_traits::value_type;
    using difference_type = typename iter_traits::difference_type;
    using pointer = typename iter_traits::pointer;
    using reference = typename iter_traits::reference;

private:
    sample_pointer data_ = nullptr;

public:
    sample_iterator() noexcept = default;

    explicit sample_iterator(sample_pointer data) noexcept : data_(data) {}

    sample_iterator(const sample_iterator& other) noexcept = default;

    sample_iterator& operator=(const sample_iterator& other) noexcept = default;

    inline size_type stride() const noexcept
    {
        return 1;
    }

    inline reference operator*() const noexcept
    {
        return *data_;
    }

    inline pointer operator->() const noexcept
    {
        return data_;
    }

    inline reference operator[](difference_type n) const noexcept
    {
        return data_[n];
    }

    inline sample_iterator& operator++() noexcept
    {
        ++data_;
        return *this;
    }

    inline sample_iterator operator++(int) noexcept
    {
        auto&& tmp = sample_iterator(*this);
        ++(*this);
        return tmp;
    }

    inline sample_iterator& operator--() noexcept
    {
        --data_;
        return *this;
    }

    inline sample_iterator operator--(int) noexcept
    {
        auto&& tmp = sample_iterator(*this);
        --(*this);
        return tmp;
    }

    inline sample_iterator operator+(difference_type n) const noexcept
    {
        auto&& w = sample_iterator(*this);
        w += n;
        return w;
    }

    inline sample_iterator& operator+=(difference_type n) noexcept
    {
        data_ += n;
        return *this;
    }

    inline sample_iterator operator-(difference_type n) const noexcept
    {
        return *this + (-n);
    }

    inline sample_iterator& operator-=(difference_type n) noexcept
    {
        *this += -n;
        return *this;
    }

#if defined(RATL_CPP_VERSION_HAS_CPP20)

    inline auto operator<=>(const sample_iterator& other) const noexcept = default;

#else

    friend inline bool operator==(const sample_iterator& x, const sample_iterator& y)
    {
        return x.data_ == y.data_;
    }

    friend inline bool operator!=(const sample_iterator& x, const sample_iterator& y)
    {
        return !(x == y);
    }

    friend inline bool operator<(const sample_iterator& x, const sample_iterator& y)
    {
        return x.data_ < y.data_;
    }

    friend inline bool operator<=(const sample_iterator& x, const sample_iterator& y)
    {
        return !(x > y);
    }

    friend inline bool operator>(const sample_iterator& x, const sample_iterator& y)
    {
        return y < x;
    }

    friend inline bool operator>=(const sample_iterator& x, const sample_iterator& y)
    {
        return !(x < y);
    }

#endif

    friend inline sample_iterator operator+(typename sample_iterator::difference_type n, sample_iterator x)
    {
        x += n;
        return x;
    }

    friend inline typename sample_iterator::difference_type operator-(
        const sample_iterator& x, const sample_iterator& y)
    {
        return x.data_ - y.data_;
    }

    inline pointer base() const noexcept
    {
        return data_;
    }
};

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_sample_iterator_
