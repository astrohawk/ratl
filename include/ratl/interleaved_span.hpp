#ifndef _ratl_interleaved_span_
#define _ratl_interleaved_span_

// C++ Standard Library includes
#include <iterator>
#include <memory>
#include <type_traits>

// ratl includes
#include <ratl/channel_span.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/detail/interleaved_iterator.hpp>
#include <ratl/detail/operator_arrow_proxy.hpp>
#include <ratl/frame_span.hpp>
#include <ratl/network_sample.hpp>
#include <ratl/sample.hpp>

namespace ratl
{
template<class Sample>
class BasicInterleavedSpan
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

    using frame_type = BasicFrameSpan<sample, true>;
    using const_frame_type = BasicFrameSpan<const_sample, true>;
    using channel_type = BasicChannelSpan<sample, false>;
    using const_channel_type = BasicChannelSpan<const_sample, false>;

    using size_type = typename sample_traits::size_type;
    using difference_type = typename sample_traits::difference_type;

    using value_type = frame_type;
    using pointer = detail::operator_arrow_proxy<value_type>;
    using const_pointer = const detail::operator_arrow_proxy<value_type>;
    using reference = frame_type;
    using const_reference = const_frame_type;

    using iterator = detail::InterleavedIterator<sample>;
    using const_iterator = detail::InterleavedIterator<const_sample>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    sample_value_pointer data_ = nullptr;
    size_type channels_ = 0;
    size_type frames_ = 0;

public:
    BasicInterleavedSpan() noexcept = default;

    BasicInterleavedSpan(size_type channels, size_type frames, sample_value_pointer data) noexcept :
        data_(data), channels_(channels), frames_(frames)
    {
    }

    BasicInterleavedSpan(const BasicInterleavedSpan& other) noexcept :
        data_(other.data_), channels_(other.channels_), frames_(other.frames_)
    {
    }

    BasicInterleavedSpan& operator=(const BasicInterleavedSpan& other) noexcept
    {
        BasicInterleavedSpan(other).swap(*this);
        return *this;
    }

    void swap(BasicInterleavedSpan& other) noexcept;

    bool operator==(const BasicInterleavedSpan& other) const noexcept;

    bool operator!=(const BasicInterleavedSpan& other) const noexcept
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

    size_type channels() const noexcept
    {
        return channels_;
    }

    size_type frames() const noexcept
    {
        return frames_;
    }

    size_type samples() const noexcept
    {
        return channels_ * frames_;
    }

    bool empty() const noexcept
    {
        return (channels_ == 0) || (frames_ == 0);
    }

    frame_type frame(size_type n)
    {
        return frame_type(channels_, data_ + (n * channels_));
    }

    const_frame_type frame(size_type n) const
    {
        return const_frame_type(channels_, data_ + (n * channels_));
    }

    channel_type channel(size_type n)
    {
        return channel_type(frames_, channels_, data_ + n);
    }

    const_channel_type channel(size_type n) const
    {
        return const_channel_type(frames_, channels_, data_ + n);
    }

    reference operator[](size_type n) noexcept
    {
        return this->frame(n);
    }

    const_reference operator[](size_type n) const noexcept
    {
        return this->frame(n);
    }

    reference at(size_type n);

    const_reference at(size_type n) const;

    reference front() noexcept
    {
        return reference(channels_, data_);
    }

    const_reference front() const noexcept
    {
        return const_reference(channels_, data_);
    }

    reference back() noexcept
    {
        return reference(channels_, data_ + (channels_ * (frames_ - 1)));
    }

    const_reference back() const noexcept
    {
        return const_reference(channels_, data_ + (channels_ * (frames_ - 1)));
    }

    // iterators
    iterator begin() noexcept
    {
        return iterator(channels_, data_);
    }

    const_iterator begin() const noexcept
    {
        return const_iterator(channels_, data_);
    }

    iterator end() noexcept
    {
        return iterator(channels_, data_ + (channels_ * frames_));
    }

    const_iterator end() const noexcept
    {
        return const_iterator(channels_, data_ + (channels_ * frames_));
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
    void set_pointer(sample_value_pointer begin) noexcept
    {
        data_ = begin;
    }

    void clear_contents() noexcept
    {
        data_ = nullptr;
        channels_ = 0;
        frames_ = 0;
    }
};

template<class Sample>
void BasicInterleavedSpan<Sample>::swap(BasicInterleavedSpan& other) noexcept
{
    std::swap(data_, other.data_);
    std::swap(channels_, other.channels_);
    std::swap(frames_, other.frames_);
}

template<class Sample>
bool BasicInterleavedSpan<Sample>::operator==(const BasicInterleavedSpan& other) const noexcept
{
    if ((channels_ == other.channels_) && (frames_ == other.frames_))
    {
        if (data_ == other.data_)
        {
            return true;
        }
        if (std::equal(data_, data_ + samples(), other.data_))
        {
            return true;
        }
    }
    return false;
}

template<class Sample>
typename BasicInterleavedSpan<Sample>::reference BasicInterleavedSpan<Sample>::at(size_type n)
{
    if (n >= frames_)
    {
        throw std::out_of_range("Interleaved");
    }
    return this->frame(n);
}

template<class Sample>
typename BasicInterleavedSpan<Sample>::const_reference BasicInterleavedSpan<Sample>::at(size_type n) const
{
    if (n >= frames_)
    {
        throw std::out_of_range("Interleaved");
    }
    return this->frame(n);
}

template<class SampleType>
using InterleavedSpan = BasicInterleavedSpan<Sample<SampleType>>;

template<class SampleType>
using NetworkInterleavedSpan = BasicInterleavedSpan<NetworkSample<SampleType>>;

} // namespace ratl

#endif // _ratl_interleaved_span_
