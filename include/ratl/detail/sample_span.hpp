#ifndef _ratl_sample_span_
#define _ratl_sample_span_

// C++ Standard Library includes
#include <algorithm>
#include <cassert>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/frame_iterator.hpp>
#include <ratl/detail/types.hpp>
#include <ratl/sample.hpp>

namespace ratl
{
template<class Sample, bool Contiguous, template<class, bool> class Iterator>
class SampleSpan;

template<class Sample, template<class, bool> class Iterator>
class SampleSpan<Sample, false, Iterator>
{
public:
    using sample_traits = detail::SampleTraits<Sample>;
    using sample = typename sample_traits::sample;
    using const_sample = typename sample_traits::const_sample;
    using sample_pointer = typename sample_traits::pointer;
    using const_sample_pointer = typename sample_traits::const_pointer;
    using sample_reference = typename sample_traits::reference;
    using const_sample_reference = typename sample_traits::const_reference;

    using size_type = detail::types::size_type;
    using difference_type = detail::types::difference_type;

    using value_type = sample;
    using pointer = sample_pointer;
    using const_pointer = const_sample_pointer;
    using reference = sample_reference;
    using const_reference = const_sample_reference;

    using iterator = Iterator<sample, false>;
    using const_iterator = Iterator<const_sample, false>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    sample_pointer start_;
    size_type samples_;
    size_type stride_;

    SampleSpan() noexcept : start_(), samples_(), stride_() {}

    SampleSpan(sample_pointer data, size_type samples) noexcept : start_(data), samples_(samples), stride_(1) {}

    SampleSpan(sample_pointer data, size_type samples, size_type stride) noexcept :
        start_(data), samples_(samples), stride_(stride)
    {
    }

    SampleSpan(const SampleSpan&) noexcept = default;

    SampleSpan(const SampleSpan<Sample, true, Iterator>& other) noexcept :
        start_(other.start_), samples_(other.samples_), stride_(1)
    {
    }

    SampleSpan& operator=(const SampleSpan&) noexcept = default;

    SampleSpan& operator=(const SampleSpan<Sample, true, Iterator>& other) noexcept
    {
        ChannelSpan(other).swap(*this);
        return *this;
    }

    inline void swap(SampleSpan& other) noexcept;

    inline sample_pointer data() noexcept
    {
        return start_;
    }

    inline const sample_pointer data() const noexcept
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

template<class Sample, template<class, bool> class Iterator>
inline void SampleSpan<Sample, false, Iterator>::swap(SampleSpan& other) noexcept
{
    std::swap(start_, other.start_);
    std::swap(samples_, other.samples_);
    std::swap(stride_, other.stride_);
}

template<class Sample, template<class, bool> class Iterator>
class SampleSpan<Sample, true, Iterator>
{
public:
    using sample_traits = detail::SampleTraits<Sample>;
    using sample = typename sample_traits::sample;
    using const_sample = typename sample_traits::const_sample;
    using sample_pointer = typename sample_traits::pointer;
    using const_sample_pointer = typename sample_traits::const_pointer;
    using sample_reference = typename sample_traits::reference;
    using const_sample_reference = typename sample_traits::const_reference;

    using size_type = detail::types::size_type;
    using difference_type = detail::types::difference_type;

    using value_type = sample;
    using pointer = sample_pointer;
    using const_pointer = const_sample_pointer;
    using reference = sample_reference;
    using const_reference = const_sample_reference;

    using iterator = Iterator<sample, true>;
    using const_iterator = Iterator<const_sample, true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    sample_pointer start_;
    size_type samples_;

    SampleSpan() noexcept : start_(), samples_() {}

    SampleSpan(size_type samples) noexcept : start_(), samples_(samples) {}

    SampleSpan(sample_pointer data, size_type samples) noexcept : start_(data), samples_(samples) {}

    SampleSpan(const SampleSpan&) noexcept = default;

    SampleSpan& operator=(const SampleSpan&) noexcept = default;

    inline void swap(SampleSpan& other) noexcept;

    inline sample_pointer data() noexcept
    {
        return start_;
    }

    inline const sample_pointer data() const noexcept
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

template<class Sample, template<class, bool> class Iterator>
inline void SampleSpan<Sample, true, Iterator>::swap(SampleSpan& other) noexcept
{
    std::swap(start_, other.start_);
    std::swap(samples_, other.samples_);
}

} // namespace ratl

#endif // _ratl_sample_span_
