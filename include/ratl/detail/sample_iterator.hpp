/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_sample_iterator_
#define _ratl_detail_sample_iterator_

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/sample_traits.hpp>
#include <ratl/detail/utility.hpp>
#include <ratl/sample.hpp>

// other includes
#include <type_traits>

namespace ratl
{
namespace detail
{
template<typename Tag, typename SampleType, typename SampleTraits, typename Contiguous>
class sample_iterator;

template<typename Tag, typename SampleType, typename SampleTraits>
class sample_iterator<Tag, SampleType, SampleTraits, std::false_type>
{
    static_assert(
        std::is_same<typename SampleTraits::sample_type, SampleType>::value,
        "sample_type in SampleTraits must be the same type as SampleType");

    using sample_traits = SampleTraits;
    using sample_type = typename sample_traits::sample_type;
    using sample_pointer = typename sample_traits::pointer;

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

    template<
        typename ArgSampleType,
        typename ArgSampleTraits,
        typename = std::enable_if_t<std::is_same<const_sample_traits_t<ArgSampleTraits>, SampleTraits>::value>>
    sample_iterator(const sample_iterator<Tag, ArgSampleType, ArgSampleTraits, std::false_type>& other) noexcept :
        data_(other.base()), stride_(other.stride())
    {
        static_assert(
            std::is_same<typename ArgSampleTraits::sample_type, ArgSampleType>::value,
            "sample_type in SampleTraits must be the same type as SampleType");
    }

    sample_iterator& operator=(const sample_iterator& other) noexcept = default;

    template<
        typename ArgSampleType,
        typename ArgSampleTraits,
        typename = std::enable_if_t<std::is_same<const_sample_traits_t<ArgSampleTraits>, SampleTraits>::value>>
    sample_iterator& operator=(
        const sample_iterator<Tag, ArgSampleType, ArgSampleTraits, std::false_type>& other) noexcept
    {
        static_assert(
            std::is_same<typename ArgSampleTraits::sample_type, ArgSampleType>::value,
            "sample_type in SampleTraits must be the same type as SampleType");
        data_ = other.base();
        stride_ = other.stride();
        return *this;
    }

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

template<typename Tag, typename SampleType, typename SampleTraits>
class sample_iterator<Tag, SampleType, SampleTraits, std::true_type>
{
    static_assert(
        std::is_same<typename SampleTraits::sample_type, SampleType>::value,
        "sample_type in SampleTraits must be the same type as SampleType");

    using sample_traits = SampleTraits;
    using sample_type = typename sample_traits::sample_type;
    using sample_pointer = typename sample_traits::pointer;

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

    template<
        typename ArgSampleType,
        typename ArgSampleTraits,
        typename = std::enable_if_t<std::is_same<const_sample_traits_t<ArgSampleTraits>, SampleTraits>::value>>
    sample_iterator(const sample_iterator<Tag, ArgSampleType, ArgSampleTraits, std::true_type>& other) noexcept :
        data_(other.base())
    {
        static_assert(
            std::is_same<typename ArgSampleTraits::sample_type, ArgSampleType>::value,
            "sample_type in SampleTraits must be the same type as SampleType");
    }

    sample_iterator& operator=(const sample_iterator& other) noexcept = default;

    template<
        typename ArgSampleType,
        typename ArgSampleTraits,
        typename = std::enable_if_t<std::is_same<const_sample_traits_t<ArgSampleTraits>, SampleTraits>::value>>
    sample_iterator& operator=(
        const sample_iterator<Tag, ArgSampleType, ArgSampleTraits, std::true_type>& other) noexcept
    {
        static_assert(
            std::is_same<typename ArgSampleTraits::sample_type, ArgSampleType>::value,
            "sample_type in SampleTraits must be the same type as SampleType");
        data_ = other.base();
        return *this;
    }

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

// sample_iterator_underlying_type

template<typename SampleIterator>
struct sample_iterator_underlying_type;

template<typename IteratorTag, typename IteratorSampleType, typename IteratorSampleTraits, typename IteratorContiguous>
struct sample_iterator_underlying_type<
    sample_iterator<IteratorTag, IteratorSampleType, IteratorSampleTraits, IteratorContiguous>>
{
    using type = sample_underlying_type_t<IteratorSampleType>;
};

// sample_iterator_underlying_type_t

template<typename SampleIterator>
using sample_iterator_underlying_type_t = typename sample_iterator_underlying_type<SampleIterator>::type;

// make_sample_iterator

template<typename IteratorTag, typename IteratorSampleType, typename IteratorSampleTraits>
sample_iterator<IteratorTag, IteratorSampleType, IteratorSampleTraits, std::false_type> make_sample_iterator(
    typename IteratorSampleTraits::pointer data,
    sample_iterator<IteratorTag, IteratorSampleType, IteratorSampleTraits, std::false_type> iterator)
{
    return sample_iterator<IteratorTag, IteratorSampleType, IteratorSampleTraits, std::false_type>(
        data, iterator.stride());
}

template<typename IteratorTag, typename IteratorSampleType, typename IteratorSampleTraits>
sample_iterator<IteratorTag, IteratorSampleType, IteratorSampleTraits, std::true_type> make_sample_iterator(
    typename IteratorSampleTraits::pointer data,
    sample_iterator<IteratorTag, IteratorSampleType, IteratorSampleTraits, std::true_type>)
{
    return sample_iterator<IteratorTag, IteratorSampleType, IteratorSampleTraits, std::true_type>(data);
}

struct sample_iterator_contiguous_dispatcher
{
    template<
        typename IteratorTag,
        typename InputSampleType,
        typename InputSampleTraits,
        typename OutputSampleIterator,
        typename Fn>
    static inline OutputSampleIterator dispatch(
        sample_iterator<IteratorTag, InputSampleType, InputSampleTraits, std::true_type> first,
        sample_iterator<IteratorTag, InputSampleType, InputSampleTraits, std::true_type> last,
        OutputSampleIterator result,
        Fn&& fn) noexcept
    {
        return dispatch_impl(first, last, result, std::forward<Fn>(fn));
    }

    template<
        typename IteratorTag,
        typename InputSampleType,
        typename InputSampleTraits,
        typename OutputSampleIterator,
        typename Fn>
    static inline OutputSampleIterator dispatch(
        sample_iterator<IteratorTag, InputSampleType, InputSampleTraits, std::false_type> first,
        sample_iterator<IteratorTag, InputSampleType, InputSampleTraits, std::false_type> last,
        OutputSampleIterator result,
        Fn&& fn) noexcept
    {
        using contiguous_input_interator =
            sample_iterator<IteratorTag, InputSampleType, InputSampleTraits, std::true_type>;
        if (first.stride() == 1)
        {
            return dispatch_impl(
                contiguous_input_interator(first.base()),
                contiguous_input_interator(last.base()),
                result,
                std::forward<Fn>(fn));
        }
        else
        {
            return dispatch_impl(first, last, result, std::forward<Fn>(fn));
        }
    }

private:
    template<
        typename IteratorTag,
        typename InputIterator,
        typename OutputSampleType,
        typename OutputSampleTraits,
        typename Fn>
    static inline sample_iterator<IteratorTag, OutputSampleType, OutputSampleTraits, std::true_type> dispatch_impl(
        InputIterator first,
        InputIterator last,
        sample_iterator<IteratorTag, OutputSampleType, OutputSampleTraits, std::true_type> result,
        Fn&& fn) noexcept
    {
        return detail::invoke(std::forward<Fn>(fn), first, last, result);
    }

    template<
        typename IteratorTag,
        typename InputIterator,
        typename OutputSampleType,
        typename OutputSampleTraits,
        typename Fn>
    static inline sample_iterator<IteratorTag, OutputSampleType, OutputSampleTraits, std::false_type> dispatch_impl(
        InputIterator first,
        InputIterator last,
        sample_iterator<IteratorTag, OutputSampleType, OutputSampleTraits, std::false_type> result,
        Fn&& fn) noexcept
    {
        using contiguous_output_interator =
            sample_iterator<IteratorTag, OutputSampleType, OutputSampleTraits, std::true_type>;
        using noncontiguous_output_interator =
            sample_iterator<IteratorTag, OutputSampleType, OutputSampleTraits, std::false_type>;
        if (result.stride() == 1)
        {
            return noncontiguous_output_interator(
                detail::invoke(std::forward<Fn>(fn), first, last, contiguous_output_interator(result.base())).base(),
                1);
        }
        else
        {
            return detail::invoke(std::forward<Fn>(fn), first, last, result);
        }
    }
};

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_sample_iterator_
