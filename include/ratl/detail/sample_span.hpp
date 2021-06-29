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
inline bool operator==(
    const SampleSpan<Sample, false, Iterator>& a, const SampleSpan<Sample, false, Iterator>& b) noexcept;

template<class Sample, template<class, bool> class Iterator>
class SampleSpan<Sample, false, Iterator>
{
public:
    using sample_traits = detail::SampleTraits<Sample>;
    using sample = typename sample_traits::sample;
    using const_sample = typename sample_traits::const_sample;
    using sample_pointer = typename sample_traits::pointer;
    using sample_const_pointer = typename sample_traits::const_pointer;
    using sample_reference = typename sample_traits::reference;
    using sample_const_reference = typename sample_traits::const_reference;

    using size_type = detail::types::size_type;
    using difference_type = detail::types::difference_type;

    using value_type = sample;
    using pointer = sample_pointer;
    using const_pointer = sample_const_pointer;
    using reference = sample_reference;
    using const_reference = sample_const_reference;

    using iterator = Iterator<sample, false>;
    using const_iterator = Iterator<const_sample, false>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    sample_pointer data_ = nullptr;
    size_type samples_ = 0;
    size_type stride_ = 0;

public:
    SampleSpan() noexcept = default;

    SampleSpan(sample_pointer data, size_type samples, size_type stride) noexcept :
        data_(data), samples_(samples), stride_(stride)
    {
    }

    SampleSpan(unsigned char* data, size_type samples, size_type stride) noexcept :
        SampleSpan(reinterpret_cast<sample_pointer>(data), samples, stride)
    {
    }

    SampleSpan(const SampleSpan& other) noexcept = default;

    SampleSpan(const SampleSpan<Sample, true, Iterator>& other) noexcept :
        data_(other.data_), samples_(other.samples_), stride_(1)
    {
    }

    SampleSpan& operator=(const SampleSpan& other) noexcept = default;

    SampleSpan& operator=(const SampleSpan<Sample, true, Iterator>& other) noexcept
    {
        ChannelSpan(other).swap(*this);
        return *this;
    }

    inline void swap(SampleSpan& other) noexcept;

    inline sample_pointer data() noexcept
    {
        return data_;
    }

    inline const sample_pointer data() const noexcept
    {
        return data_;
    }

    inline size_type stride() const noexcept
    {
        return stride_;
    }

    inline bool empty() const noexcept
    {
        return samples_ == 0;
    }

    inline reference operator[](size_type n)
    {
        return data_[n * stride_];
    }

    inline const_reference operator[](size_type n) const
    {
        return data_[n * stride_];
    }

    inline reference at(size_type n);

    inline const_reference at(size_type n) const;

    inline reference front()
    {
        return *data_;
    }

    inline const_reference front() const
    {
        return *data_;
    }

    inline reference back()
    {
        return *(data_ + (samples_ * (stride_ - 1)));
    }

    inline const_reference back() const
    {
        return *(data_ + (samples_ * (stride_ - 1)));
    }

    // iterators
    inline iterator begin() noexcept
    {
        return iterator(data_, stride_);
    }

    inline const_iterator begin() const noexcept
    {
        return const_iterator(data_, stride_);
    }

    inline iterator end() noexcept
    {
        return iterator(data_ + (samples_ * stride_), stride_);
    }

    inline const_iterator end() const noexcept
    {
        return const_iterator(data_ + (samples_ * stride_), stride_);
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

    friend bool operator==<>(const SampleSpan& a, const SampleSpan& b) noexcept;

    friend inline bool operator!=(const SampleSpan& a, const SampleSpan& b) noexcept
    {
        return !(a == b);
    }

protected:
    inline size_type samples() const noexcept
    {
        return samples_;
    }
};

template<class Sample, template<class, bool> class Iterator>
inline void SampleSpan<Sample, false, Iterator>::swap(SampleSpan& other) noexcept
{
    std::swap(samples_, other.samples_);
    std::swap(stride_, other.stride_);
    std::swap(data_, other.data_);
}

template<class Sample, template<class, bool> class Iterator>
inline typename SampleSpan<Sample, false, Iterator>::reference SampleSpan<Sample, false, Iterator>::at(size_type n)
{
    if (n >= samples_)
    {
        throw std::out_of_range("ChannelSpan");
    }
    return data_[n];
}

template<class Sample, template<class, bool> class Iterator>
inline typename SampleSpan<Sample, false, Iterator>::const_reference SampleSpan<Sample, false, Iterator>::at(
    size_type n) const
{
    if (n >= samples_)
    {
        throw std::out_of_range("ChannelSpan");
    }
    return data_[n];
}

template<class Sample, template<class, bool> class Iterator>
inline bool operator==(
    const SampleSpan<Sample, false, Iterator>& a, const SampleSpan<Sample, false, Iterator>& b) noexcept
{
    if ((a.samples_ == b.samples_) && (a.stride_ == b.stride_))
    {
        if (a.data_ == b.data_)
        {
            return true;
        }
        if (a.stride_ == 1)
        {
            if (std::equal(a.data_, a.data_ + a.samples_, b.data_))
            {
                return true;
            }
        }
        else
        {
            if (std::equal(a.begin(), a.end(), b.begin()))
            {
                return true;
            }
        }
    }
    return false;
}

template<class Sample, template<class, bool> class Iterator>
inline bool operator==(
    const SampleSpan<Sample, true, Iterator>& a, const SampleSpan<Sample, true, Iterator>& b) noexcept;

template<class Sample, template<class, bool> class Iterator>
class SampleSpan<Sample, true, Iterator>
{
public:
    using sample_traits = detail::SampleTraits<Sample>;
    using sample = typename sample_traits::sample;
    using const_sample = typename sample_traits::const_sample;
    using sample_pointer = typename sample_traits::pointer;
    using sample_const_pointer = typename sample_traits::const_pointer;
    using sample_reference = typename sample_traits::reference;
    using sample_const_reference = typename sample_traits::const_reference;

    using size_type = detail::types::size_type;
    using difference_type = detail::types::difference_type;

    using value_type = sample;
    using pointer = sample_pointer;
    using const_pointer = sample_const_pointer;
    using reference = sample_reference;
    using const_reference = sample_const_reference;

    using iterator = Iterator<sample, true>;
    using const_iterator = Iterator<const_sample, true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    sample_pointer data_ = nullptr;
    size_type samples_ = 0;

public:
    SampleSpan() noexcept = default;

    SampleSpan(sample_pointer data, size_type samples) noexcept : data_(data), samples_(samples) {}

    SampleSpan(unsigned char* data, size_type samples) noexcept :
        SampleSpan(reinterpret_cast<sample_pointer>(data), samples)
    {
    }

    SampleSpan(const SampleSpan& other) noexcept = default;

    SampleSpan& operator=(const SampleSpan& other) noexcept = default;

    inline void swap(SampleSpan& other) noexcept;

    inline sample_pointer data() noexcept
    {
        return data_;
    }

    inline const sample_pointer data() const noexcept
    {
        return data_;
    }

    inline size_type stride() const noexcept
    {
        return 1;
    }

    inline bool empty() const noexcept
    {
        return samples_ == 0;
    }

    inline reference operator[](size_type n) noexcept
    {
        return data_[n];
    }

    inline const_reference operator[](size_type n) const noexcept
    {
        return data_[n];
    }

    inline reference at(size_type n);

    inline const_reference at(size_type n) const;

    inline reference front() noexcept
    {
        return *data_;
    }

    inline const_reference front() const noexcept
    {
        return *data_;
    }

    inline reference back() noexcept
    {
        return *(data_ - 1);
    }

    inline const_reference back() const noexcept
    {
        return *(data_ - 1);
    }

    // iterators
    inline iterator begin() noexcept
    {
        return iterator(data_);
    }

    inline const_iterator begin() const noexcept
    {
        return const_iterator(data_);
    }

    inline iterator end() noexcept
    {
        return iterator(data_ + samples_);
    }

    inline const_iterator end() const noexcept
    {
        return const_iterator(data_ + samples_);
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

    friend bool operator==<>(const SampleSpan& a, const SampleSpan& b) noexcept;

    friend inline bool operator!=(const SampleSpan& a, const SampleSpan& b) noexcept
    {
        return !(a == b);
    }

protected:
    explicit SampleSpan(size_type samples) noexcept : data_(nullptr), samples_(samples) {}

    inline void set_pointer(sample_pointer begin) noexcept
    {
        data_ = begin;
    }

    inline size_type samples() const noexcept
    {
        return samples_;
    }
};

template<class Sample, template<class, bool> class Iterator>
inline void SampleSpan<Sample, true, Iterator>::swap(SampleSpan& other) noexcept
{
    std::swap(data_, other.data_);
    std::swap(samples_, other.samples_);
}

template<class Sample, template<class, bool> class Iterator>
inline typename SampleSpan<Sample, true, Iterator>::reference SampleSpan<Sample, true, Iterator>::at(size_type n)
{
    if (n >= samples_)
    {
        throw std::out_of_range("FrameSpan");
    }
    return data_[n];
}

template<class Sample, template<class, bool> class Iterator>
inline typename SampleSpan<Sample, true, Iterator>::const_reference SampleSpan<Sample, true, Iterator>::at(
    size_type n) const
{
    if (n >= samples_)
    {
        throw std::out_of_range("FrameSpan");
    }
    return data_[n];
}

template<class Sample, template<class, bool> class Iterator>
inline bool operator==(
    const SampleSpan<Sample, true, Iterator>& a, const SampleSpan<Sample, true, Iterator>& b) noexcept
{
    if (a.samples_ == b.samples_)
    {
        if (a.data_ == b.data_)
        {
            return true;
        }
        if (std::equal(a.data_, a.data_ + a.samples_, b.data_))
        {
            return true;
        }
    }
    return false;
}

} // namespace ratl

#endif // _ratl_sample_span_
