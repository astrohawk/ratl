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
    using sample_type = typename sample_traits::sample_type;
    using sample_value_type = typename sample_traits::value_type;
    using sample_value_pointer = typename sample_traits::value_pointer;
    using sample_pointer = typename sample_traits::pointer;
    using sample_const_pointer = typename sample_traits::const_pointer;
    using sample_reference = typename sample_traits::reference;
    using sample_const_reference = typename sample_traits::const_reference;

    using size_type = typename sample_traits::size_type;
    using difference_type = typename sample_traits::difference_type;

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
    sample_value_pointer data_ = nullptr;
    size_type samples_ = 0;
    size_type stride_ = 0;

public:
    SampleSpan() noexcept = default;

    SampleSpan(size_type samples, size_type stride, sample_value_pointer data) noexcept :
        data_(data), samples_(samples), stride_(stride)
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

    void swap(SampleSpan& other) noexcept;

    bool operator==(const SampleSpan& other) const noexcept;

    bool operator!=(const SampleSpan& other) const noexcept
    {
        return !(*this == other);
    }

    sample_value_pointer data() noexcept
    {
        return data_;
    }

    const sample_value_pointer data() const noexcept
    {
        return data_;
    }

    size_type stride() const noexcept
    {
        return stride_;
    }

    bool empty() const noexcept
    {
        return samples_ == 0;
    }

    reference operator[](size_type n)
    {
        return reinterpret_cast<reference>(data_[n * stride_]);
    }

    const_reference operator[](size_type n) const
    {
        return reinterpret_cast<const_reference>(data_[n * stride_]);
    }

    reference at(size_type n);

    const_reference at(size_type n) const;

    reference front()
    {
        return reinterpret_cast<reference>(*data_);
    }

    const_reference front() const
    {
        return reinterpret_cast<const_reference>(*data_);
    }

    reference back()
    {
        return reinterpret_cast<reference>(*(data_ + (samples_ * (stride_ - 1))));
    }

    const_reference back() const
    {
        return reinterpret_cast<const_reference>(*(data_ + (samples_ * (stride_ - 1))));
    }

    // iterators
    iterator begin() noexcept
    {
        return iterator(stride_, data_);
    }

    const_iterator begin() const noexcept
    {
        return const_iterator(stride_, data_);
    }

    iterator end() noexcept
    {
        return iterator(stride_, data_ + (samples_ * stride_));
    }

    const_iterator end() const noexcept
    {
        return const_iterator(stride_, data_ + (samples_ * stride_));
    }

    // reverse iterators
    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    // const iterators
    const_iterator cbegin() const noexcept
    {
        return begin();
    }

    const_iterator cend() const noexcept
    {
        return end();
    }

    const_reverse_iterator crbegin() const noexcept
    {
        return rbegin();
    }

    const_reverse_iterator crend() const noexcept
    {
        return rend();
    }

protected:
    size_type samples() const noexcept
    {
        return samples_;
    }
};

template<class Sample, template<class, bool> class Iterator>
void SampleSpan<Sample, false, Iterator>::swap(SampleSpan& other) noexcept
{
    std::swap(samples_, other.samples_);
    std::swap(stride_, other.stride_);
    std::swap(data_, other.data_);
}

template<class Sample, template<class, bool> class Iterator>
bool SampleSpan<Sample, false, Iterator>::operator==(const SampleSpan& other) const noexcept
{
    if ((samples_ == other.samples_) && (stride_ == other.stride_))
    {
        if (data_ == other.data_)
        {
            return true;
        }
        if (stride_ == 1)
        {
            if (std::equal(data_, data_ + samples_, other.data_))
            {
                return true;
            }
        }
        else
        {
            if (std::equal(begin(), end(), other.begin()))
            {
                return true;
            }
        }
    }
    return false;
}

template<class Sample, template<class, bool> class Iterator>
typename SampleSpan<Sample, false, Iterator>::reference SampleSpan<Sample, false, Iterator>::at(size_type n)
{
    if (n >= samples_)
    {
        throw std::out_of_range("ChannelSpan");
    }
    return data_[n];
}

template<class Sample, template<class, bool> class Iterator>
typename SampleSpan<Sample, false, Iterator>::const_reference SampleSpan<Sample, false, Iterator>::at(size_type n) const
{
    if (n >= samples_)
    {
        throw std::out_of_range("ChannelSpan");
    }
    return data_[n];
}

template<class Sample, template<class, bool> class Iterator>
class SampleSpan<Sample, true, Iterator>
{
protected:
    using sample_traits = detail::SampleTraits<Sample>;
    using sample = typename sample_traits::sample;
    using const_sample = typename sample_traits::const_sample;
    using sample_type = typename sample_traits::sample_type;
    using sample_value_type = typename sample_traits::value_type;
    using sample_value_pointer = typename sample_traits::value_pointer;
    using sample_pointer = typename sample_traits::pointer;
    using sample_const_pointer = typename sample_traits::const_pointer;
    using sample_reference = typename sample_traits::reference;
    using sample_const_reference = typename sample_traits::const_reference;

public:
    using size_type = typename sample_traits::size_type;
    using difference_type = typename sample_traits::difference_type;

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
    sample_value_pointer data_ = nullptr;
    size_type samples_ = 0;

public:
    SampleSpan() noexcept = default;

    SampleSpan(size_type samples, sample_value_pointer data) noexcept : data_(data), samples_(samples) {}

    SampleSpan(const SampleSpan& other) noexcept = default;

    SampleSpan& operator=(const SampleSpan& other) noexcept = default;

    void swap(SampleSpan& other) noexcept;

    bool operator==(const SampleSpan& other) const noexcept;

    bool operator!=(const SampleSpan& other) const noexcept
    {
        return !(*this == other);
    }

    sample_value_pointer data() noexcept
    {
        return data_;
    }

    const sample_value_pointer data() const noexcept
    {
        return data_;
    }

    size_type stride() const noexcept
    {
        return 1;
    }

    bool empty() const noexcept
    {
        return samples_ == 0;
    }

    reference operator[](size_type n) noexcept
    {
        return reinterpret_cast<reference>(data_[n]);
    }

    const_reference operator[](size_type n) const noexcept
    {
        return reinterpret_cast<const_reference>(data_[n]);
    }

    reference at(size_type n);

    const_reference at(size_type n) const;

    reference front() noexcept
    {
        return reinterpret_cast<reference>(*data_);
    }

    const_reference front() const noexcept
    {
        return reinterpret_cast<const_reference>(*data_);
    }

    reference back() noexcept
    {
        return reinterpret_cast<reference>(*(data_ - 1));
    }

    const_reference back() const noexcept
    {
        return reinterpret_cast<const_reference>(*(data_ - 1));
    }

    // iterators
    iterator begin() noexcept
    {
        return iterator(data_);
    }

    const_iterator begin() const noexcept
    {
        return const_iterator(data_);
    }

    iterator end() noexcept
    {
        return iterator(data_ + samples_);
    }

    const_iterator end() const noexcept
    {
        return const_iterator(data_ + samples_);
    }

    // reverse iterators
    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    // const iterators
    const_iterator cbegin() const noexcept
    {
        return begin();
    }

    const_iterator cend() const noexcept
    {
        return end();
    }

    const_reverse_iterator crbegin() const noexcept
    {
        return rbegin();
    }

    const_reverse_iterator crend() const noexcept
    {
        return rend();
    }

protected:
    size_type samples() const noexcept
    {
        return samples_;
    }
};

template<class Sample, template<class, bool> class Iterator>
bool SampleSpan<Sample, true, Iterator>::operator==(const SampleSpan& other) const noexcept
{
    if (samples_ == other.samples_)
    {
        if (data_ == other.data_)
        {
            return true;
        }
        if (std::equal(data_, data_ + samples_, other.data_))
        {
            return true;
        }
    }
    return false;
}

template<class Sample, template<class, bool> class Iterator>
void SampleSpan<Sample, true, Iterator>::swap(SampleSpan& other) noexcept
{
    std::swap(data_, other.data_);
    std::swap(samples_, other.samples_);
}

template<class Sample, template<class, bool> class Iterator>
typename SampleSpan<Sample, true, Iterator>::reference SampleSpan<Sample, true, Iterator>::at(size_type n)
{
    if (n >= samples_)
    {
        throw std::out_of_range("FrameSpan");
    }
    return data_[n];
}

template<class Sample, template<class, bool> class Iterator>
typename SampleSpan<Sample, true, Iterator>::const_reference SampleSpan<Sample, true, Iterator>::at(size_type n) const
{
    if (n >= samples_)
    {
        throw std::out_of_range("FrameSpan");
    }
    return data_[n];
}

} // namespace ratl

#endif // _ratl_sample_span_
