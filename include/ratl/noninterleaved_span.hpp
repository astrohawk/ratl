#ifndef _ratl_noninterleaved_span_
#define _ratl_noninterleaved_span_

// C++ Standard Library includes
#include <iterator>
#include <memory>
#include <type_traits>

// ratl includes
#include <ratl/channel_span.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/detail/noninterleaved_iterator.hpp>
#include <ratl/detail/operator_arrow_proxy.hpp>
#include <ratl/frame_span.hpp>
#include <ratl/network_sample.hpp>
#include <ratl/sample.hpp>

namespace ratl
{
template<class Sample>
class BasicNoninterleavedSpan;

template<class Sample>
inline bool operator==(const BasicNoninterleavedSpan<Sample>& a, const BasicNoninterleavedSpan<Sample>& b) noexcept;

template<class Sample>
class BasicNoninterleavedSpan
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

    using frame_type = BasicFrameSpan<sample, false>;
    using const_frame_type = BasicFrameSpan<const_sample, false>;
    using channel_type = BasicChannelSpan<sample, true>;
    using const_channel_type = BasicChannelSpan<const_sample, true>;

    using size_type = typename sample_traits::size_type;
    using difference_type = typename sample_traits::difference_type;

    using value_type = channel_type;
    using pointer = detail::operator_arrow_proxy<value_type>;
    using const_pointer = const detail::operator_arrow_proxy<value_type>;
    using reference = channel_type;
    using const_reference = const_channel_type;

    using iterator = detail::NoninterleavedIterator<sample>;
    using const_iterator = detail::NoninterleavedIterator<const_sample>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    sample_pointer data_ = nullptr;
    size_type channels_ = 0;
    size_type frames_ = 0;

public:
    BasicNoninterleavedSpan() noexcept = default;

    BasicNoninterleavedSpan(sample_pointer data, size_type channels, size_type frames) noexcept :
        data_(data), channels_(channels), frames_(frames)
    {
    }

    BasicNoninterleavedSpan(unsigned char* data, size_type channels, size_type frames) noexcept :
        BasicInterleavedSpan(reinterpret_cast<sample_pointer>(data), channels, frames)
    {
    }

    BasicNoninterleavedSpan(const BasicNoninterleavedSpan& other) noexcept :
        data_(other.data_), channels_(other.channels_), frames_(other.frames_)
    {
    }

    BasicNoninterleavedSpan& operator=(const BasicNoninterleavedSpan& other) noexcept
    {
        BasicNoninterleavedSpan(other).swap(*this);
        return *this;
    }

    inline void swap(BasicNoninterleavedSpan& other) noexcept;

    inline sample_pointer data() noexcept
    {
        return data_;
    }

    inline const sample_pointer data() const noexcept
    {
        return data_;
    }

    inline size_type channels() const noexcept
    {
        return channels_;
    }

    inline size_type frames() const noexcept
    {
        return frames_;
    }

    inline size_type samples() const noexcept
    {
        return channels_ * frames_;
    }

    inline bool empty() const noexcept
    {
        return (channels_ == 0) || (frames_ == 0);
    }

    inline frame_type frame(size_type n)
    {
        return frame_type(data_ + n, channels_, frames_);
    }

    inline const_frame_type frame(size_type n) const
    {
        return const_frame_type(data_ + n, channels_, frames_);
    }

    inline channel_type channel(size_type n)
    {
        return channel_type(data_ + (n * frames_), frames_);
    }

    inline const_channel_type channel(size_type n) const
    {
        return const_channel_type(data_ + (n * frames_), frames_);
    }

    inline reference operator[](size_type n) noexcept
    {
        return this->channel(n);
    }

    inline const_reference operator[](size_type n) const noexcept
    {
        return this->channel(n);
    }

    inline reference at(size_type n);

    inline const_reference at(size_type n) const;

    inline reference front() noexcept
    {
        return reference(data_, frames_);
    }

    inline const_reference front() const noexcept
    {
        return const_reference(data_, frames_);
    }

    inline reference back() noexcept
    {
        return reference(data_ + ((channels_ - 1) * frames_), frames_);
    }

    inline const_reference back() const noexcept
    {
        return reference(data_ + ((channels_ - 1) * frames_), frames_);
    }

    // iterators
    inline iterator begin() noexcept
    {
        return iterator(data_, frames_);
    }

    inline const_iterator begin() const noexcept
    {
        return const_iterator(data_, frames_);
    }

    inline iterator end() noexcept
    {
        return iterator(data_ + (channels_ * frames_), frames_);
    }

    inline const_iterator end() const noexcept
    {
        return const_iterator(data_ + (channels_ * frames_), frames_);
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

    friend bool operator==<>(const BasicNoninterleavedSpan& a, const BasicNoninterleavedSpan& b) noexcept;

    friend inline bool operator!=(const BasicNoninterleavedSpan& a, const BasicNoninterleavedSpan& b) noexcept
    {
        return !(a == b);
    }

protected:
    inline void set_pointer(sample_pointer begin) noexcept
    {
        data_ = begin;
    }

    inline void clear_contents() noexcept
    {
        data_ = nullptr;
        channels_ = 0;
        frames_ = 0;
    }
};

template<class Sample>
inline void BasicNoninterleavedSpan<Sample>::swap(BasicNoninterleavedSpan& other) noexcept
{
    std::swap(data_, other.data_);
    std::swap(channels_, other.channels_);
    std::swap(frames_, other.frames_);
}

template<class Sample>
inline typename BasicNoninterleavedSpan<Sample>::reference BasicNoninterleavedSpan<Sample>::at(size_type n)
{
    if (n >= channels_)
    {
        throw std::out_of_range("Noninterleaved");
    }
    return this->channel(n);
}

template<class Sample>
inline typename BasicNoninterleavedSpan<Sample>::const_reference BasicNoninterleavedSpan<Sample>::at(size_type n) const
{
    if (n >= channels_)
    {
        throw std::out_of_range("Noninterleaved");
    }
    return this->channel(n);
}

template<class Sample>
inline bool operator==(const BasicNoninterleavedSpan<Sample>& a, const BasicNoninterleavedSpan<Sample>& b) noexcept
{
    if ((a.channels_ == b.channels_) && (a.frames_ == b.frames_))
    {
        if (a.data_ == b.data_)
        {
            return true;
        }
        if (std::equal(a.data_, a.data_ + a.samples(), b.data_))
        {
            return true;
        }
    }
    return false;
}

template<class SampleType>
using NoninterleavedSpan = BasicNoninterleavedSpan<Sample<SampleType>>;

template<class SampleType>
using NetworkNoninterleavedSpan = BasicNoninterleavedSpan<NetworkSample<SampleType>>;

} // namespace ratl

#endif // _ratl_noninterleaved_span_
