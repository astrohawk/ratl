/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_sample_span_
#define _ratl_detail_sample_span_

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/frame_iterator.hpp>
#include <ratl/detail/sample_traits.hpp>
#include <ratl/sample.hpp>

// other includes
#include <algorithm>
#include <cassert>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>

namespace ratl
{
template<
    typename SampleType,
    typename SampleTraits,
    typename Contiguous,
    template<typename, typename, typename>
    class Iterator>
class sample_span;

template<typename SampleType, typename SampleTraits, template<typename, typename, typename> class Iterator>
class sample_span<SampleType, SampleTraits, std::false_type, Iterator>
{
    static_assert(
        std::is_same<typename SampleTraits::sample_type, SampleType>::value,
        "sample_type in SampleTraits must be the same type as SampleType");

    using sample_traits = SampleTraits;
    using const_sample_traits = detail::const_sample_traits_t<sample_traits>;

public:
    using sample_type = typename sample_traits::sample_type;
    using const_sample_type = typename sample_traits::const_sample_type;
    using sample_pointer = typename sample_traits::pointer;
    using const_sample_pointer = typename sample_traits::const_pointer;

    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using value_type = sample_type;
    using pointer = sample_pointer;
    using const_pointer = const_sample_pointer;
    using reference = typename sample_traits::reference;
    using const_reference = typename sample_traits::const_reference;

    using iterator = Iterator<sample_type, sample_traits, std::false_type>;
    using const_iterator = Iterator<const_sample_type, const_sample_traits, std::false_type>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    sample_pointer start_;
    size_type samples_;
    size_type stride_;

    sample_span() noexcept : start_(), samples_(), stride_() {}

    sample_span(sample_pointer data, size_type samples) noexcept : start_(data), samples_(samples), stride_(1) {}

    sample_span(sample_pointer data, size_type samples, size_type stride) noexcept :
        start_(data), samples_(samples), stride_(stride)
    {
    }

    sample_span(const sample_span&) noexcept = default;

    sample_span(const sample_span<SampleType, SampleTraits, std::true_type, Iterator>& other) noexcept :
        start_(other.start_), samples_(other.samples_), stride_(1)
    {
    }

    sample_span& operator=(const sample_span&) noexcept = default;

    sample_span& operator=(const sample_span<SampleType, SampleTraits, std::true_type, Iterator>& other) noexcept
    {
        sample_span(other).swap(*this);
        return *this;
    }

    inline void swap(sample_span& other) noexcept;

    inline sample_pointer data() const noexcept
    {
        return start_;
    }

    inline size_type samples() const noexcept
    {
        return samples_;
    }

    inline size_type stride() const noexcept
    {
        return stride_;
    }

    inline bool empty() const noexcept
    {
        return samples() == 0;
    }

    inline reference operator[](size_type n)
    {
        return data()[n * stride()];
    }

    inline const_reference operator[](size_type n) const
    {
        return data()[n * stride()];
    }

    inline reference front()
    {
        return *data();
    }

    inline const_reference front() const
    {
        return *data();
    }

    inline reference back()
    {
        return *(data() + (samples() * (stride() - 1)));
    }

    inline const_reference back() const
    {
        return *(data() + (samples() * (stride() - 1)));
    }

    // iterators
    inline iterator begin() noexcept
    {
        return iterator(data(), stride());
    }

    inline const_iterator begin() const noexcept
    {
        return const_iterator(data(), stride());
    }

    inline iterator end() noexcept
    {
        return iterator(data() + (samples() * stride()), stride());
    }

    inline const_iterator end() const noexcept
    {
        return const_iterator(data() + (samples() * stride()), stride());
    }

    // reverse iterators
    inline reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }

    inline const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    inline reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }

    inline const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    // const iterators
    inline const_iterator cbegin() const noexcept
    {
        return begin();
    }

    inline const_iterator cend() const noexcept
    {
        return end();
    }

    inline const_reverse_iterator crbegin() const noexcept
    {
        return rbegin();
    }

    inline const_reverse_iterator crend() const noexcept
    {
        return rend();
    }
};

template<typename SampleType, typename SampleTraits, template<typename, typename, typename> class Iterator>
inline void sample_span<SampleType, SampleTraits, std::false_type, Iterator>::swap(sample_span& other) noexcept
{
    std::swap(start_, other.start_);
    std::swap(samples_, other.samples_);
    std::swap(stride_, other.stride_);
}

template<typename SampleType, typename SampleTraits, template<typename, typename, typename> class Iterator>
class sample_span<SampleType, SampleTraits, std::true_type, Iterator>
{
    static_assert(
        std::is_same<typename SampleTraits::sample_type, SampleType>::value,
        "sample_type in SampleTraits must be the same type as SampleType");

    using sample_traits = SampleTraits;
    using const_sample_traits = detail::const_sample_traits_t<sample_traits>;

public:
    using sample_type = typename sample_traits::sample_type;
    using const_sample_type = typename sample_traits::const_sample_type;
    using sample_pointer = typename sample_traits::pointer;
    using const_sample_pointer = typename sample_traits::const_pointer;

    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using value_type = sample_type;
    using pointer = sample_pointer;
    using const_pointer = const_sample_pointer;
    using reference = typename sample_traits::reference;
    using const_reference = typename sample_traits::const_reference;

    using iterator = Iterator<sample_type, sample_traits, std::true_type>;
    using const_iterator = Iterator<const_sample_type, const_sample_traits, std::true_type>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    sample_pointer start_;
    size_type samples_;

    sample_span() noexcept : start_(), samples_() {}

    sample_span(size_type samples) noexcept : start_(), samples_(samples) {}

    sample_span(sample_pointer data, size_type samples) noexcept : start_(data), samples_(samples) {}

    sample_span(const sample_span&) noexcept = default;

    sample_span& operator=(const sample_span&) noexcept = default;

    inline void swap(sample_span& other) noexcept;

    inline sample_pointer data() const noexcept
    {
        return start_;
    }

    inline size_type samples() const noexcept
    {
        return samples_;
    }

    inline size_type stride() const noexcept
    {
        return 1;
    }

    inline bool empty() const noexcept
    {
        return samples() == 0;
    }

    inline reference operator[](size_type n) noexcept
    {
        return data()[n];
    }

    inline const_reference operator[](size_type n) const noexcept
    {
        return data()[n];
    }

    inline reference front() noexcept
    {
        return *data();
    }

    inline const_reference front() const noexcept
    {
        return *data();
    }

    inline reference back() noexcept
    {
        return *(data() - 1);
    }

    inline const_reference back() const noexcept
    {
        return *(data() - 1);
    }

    // iterators
    inline iterator begin() noexcept
    {
        return iterator(data());
    }

    inline const_iterator begin() const noexcept
    {
        return const_iterator(data());
    }

    inline iterator end() noexcept
    {
        return iterator(data() + samples());
    }

    inline const_iterator end() const noexcept
    {
        return const_iterator(data() + samples());
    }

    // reverse iterators
    inline reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }

    inline const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    inline reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }

    inline const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    // const iterators
    inline const_iterator cbegin() const noexcept
    {
        return begin();
    }

    inline const_iterator cend() const noexcept
    {
        return end();
    }

    inline const_reverse_iterator crbegin() const noexcept
    {
        return rbegin();
    }

    inline const_reverse_iterator crend() const noexcept
    {
        return rend();
    }
};

template<typename SampleType, typename SampleTraits, template<typename, typename, typename> class Iterator>
inline void sample_span<SampleType, SampleTraits, std::true_type, Iterator>::swap(sample_span& other) noexcept
{
    std::swap(start_, other.start_);
    std::swap(samples_, other.samples_);
}

} // namespace ratl

#endif // _ratl_detail_sample_span_
