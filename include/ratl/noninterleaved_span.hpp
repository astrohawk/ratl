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
    sample_value_pointer data_ = nullptr;
    size_type channels_ = 0;
    size_type frames_ = 0;

public:
    BasicNoninterleavedSpan() noexcept = default;

    BasicNoninterleavedSpan(size_type channels, size_type frames, sample_value_pointer data) noexcept :
        data_(data), channels_(channels), frames_(frames)
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

    void swap(BasicNoninterleavedSpan& other) noexcept;

    bool operator==(const BasicNoninterleavedSpan& other) const noexcept;

    bool operator!=(const BasicNoninterleavedSpan& other) const noexcept
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
        return frame_type(channels_, frames_, data_ + n);
    }

    const_frame_type frame(size_type n) const
    {
        return const_frame_type(channels_, frames_, data_ + n);
    }

    channel_type channel(size_type n)
    {
        return channel_type(frames_, data_ + (n * frames_));
    }

    const_channel_type channel(size_type n) const
    {
        return const_channel_type(frames_, data_ + (n * frames_));
    }

    reference operator[](size_type n) noexcept
    {
        return this->channel(n);
    }

    const_reference operator[](size_type n) const noexcept
    {
        return this->channel(n);
    }

    reference at(size_type n);

    const_reference at(size_type n) const;

    reference front() noexcept
    {
        return reference(frames_, data_);
    }

    const_reference front() const noexcept
    {
        return const_reference(frames_, data_);
    }

    reference back() noexcept
    {
        return reference(frames_, data_ + ((channels_ - 1) * frames_));
    }

    const_reference back() const noexcept
    {
        return reference(frames_, data_ + ((channels_ - 1) * frames_));
    }

    // iterators
    iterator begin() noexcept
    {
        return iterator(frames_, data_);
    }

    const_iterator begin() const noexcept
    {
        return const_iterator(frames_, data_);
    }

    iterator end() noexcept
    {
        return iterator(frames_, data_ + (channels_ * frames_));
    }

    const_iterator end() const noexcept
    {
        return const_iterator(frames_, data_ + (channels_ * frames_));
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
void BasicNoninterleavedSpan<Sample>::swap(BasicNoninterleavedSpan& other) noexcept
{
    std::swap(data_, other.data_);
    std::swap(channels_, other.channels_);
    std::swap(frames_, other.frames_);
}

template<class Sample>
bool BasicNoninterleavedSpan<Sample>::operator==(const BasicNoninterleavedSpan& other) const noexcept
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
typename BasicNoninterleavedSpan<Sample>::reference BasicNoninterleavedSpan<Sample>::at(size_type n)
{
    if (n >= channels_)
    {
        throw std::out_of_range("Noninterleaved");
    }
    return this->channel(n);
}

template<class Sample>
typename BasicNoninterleavedSpan<Sample>::const_reference BasicNoninterleavedSpan<Sample>::at(size_type n) const
{
    if (n >= channels_)
    {
        throw std::out_of_range("Noninterleaved");
    }
    return this->channel(n);
}

template<class SampleType>
using NoninterleavedSpan = BasicNoninterleavedSpan<Sample<SampleType>>;

template<class SampleType>
using NetworkNoninterleavedSpan = BasicNoninterleavedSpan<NetworkSample<SampleType>>;

} // namespace ratl

#endif // _ratl_noninterleaved_span_
