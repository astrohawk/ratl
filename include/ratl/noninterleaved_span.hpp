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
#include <ratl/detail/types.hpp>
#include <ratl/frame_span.hpp>
#include <ratl/network_sample.hpp>
#include <ratl/noninterleaved.hpp>
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
    using sample_pointer = typename sample_traits::pointer;
    using const_sample_pointer = typename sample_traits::const_pointer;
    using sample_reference = typename sample_traits::reference;
    using const_sample_reference = typename sample_traits::const_reference;

    using channel_type = BasicChannelSpan<sample, true>;
    using const_channel_type = BasicChannelSpan<const_sample, true>;
    using frame_type = BasicFrameSpan<sample, false>;
    using const_frame_type = BasicFrameSpan<const_sample, false>;

    using char_pointer = std::conditional_t<std::is_const<sample>::value, const unsigned char*, unsigned char*>;

    using size_type = detail::types::size_type;
    using difference_type = detail::types::difference_type;

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
    sample_pointer start_;
    size_type channels_;
    size_type frames_;

public:
    BasicNoninterleavedSpan() noexcept : start_(), channels_(), frames_() {}

    BasicNoninterleavedSpan(sample_pointer data, size_type channels, size_type frames) noexcept :
        start_(data), channels_(channels), frames_(frames)
    {
    }

    BasicNoninterleavedSpan(char_pointer data, size_type channels, size_type frames) noexcept :
        start_(reinterpret_cast<sample_pointer>(data)), channels_(channels), frames_(frames)
    {
    }

    BasicNoninterleavedSpan(const BasicNoninterleavedSpan& other) noexcept :
        start_(other.data()), channels_(other.channels()), frames_(other.frames())
    {
    }

    template<
        class SampleArg,
        class Allocator,
        std::enable_if_t<std::is_same<SampleArg, std::remove_const_t<Sample>>::value, bool> = true>
    BasicNoninterleavedSpan(BasicInterleaved<SampleArg, Allocator>& noninterleaved) noexcept :
        start_(noninterleaved.data()), channels_(noninterleaved.channels()), frames_(noninterleaved.frames())
    {
    }

    template<
        class SampleArg,
        class Allocator,
        std::enable_if_t<std::is_same<typename detail::SampleTraits<SampleArg>::const_sample, Sample>::value, bool> =
            true>
    BasicNoninterleavedSpan(const BasicInterleaved<SampleArg, Allocator>& noninterleaved) noexcept :
        start_(noninterleaved.data()), channels_(noninterleaved.channels()), frames_(noninterleaved.frames())
    {
    }

    BasicNoninterleavedSpan& operator=(const BasicNoninterleavedSpan& other) noexcept = default;

    inline void swap(BasicNoninterleavedSpan& other) noexcept;

    inline sample_pointer data() noexcept
    {
        return start_;
    }

    inline const sample_pointer data() const noexcept
    {
        return start_;
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
        return channels() * frames();
    }

    inline bool empty() const noexcept
    {
        return (channels() == 0) || (frames() == 0);
    }

    inline frame_type frame(size_type n)
    {
        return frame_type(start_ + n, channels(), frames());
    }

    inline const_frame_type frame(size_type n) const
    {
        return const_frame_type(start_ + n, channels(), frames());
    }

    inline channel_type channel(size_type n)
    {
        return channel_type(start_ + (n * frames()), frames());
    }

    inline const_channel_type channel(size_type n) const
    {
        return const_channel_type(start_ + (n * frames()), frames());
    }

    inline reference operator[](size_type n) noexcept
    {
        return channel(n);
    }

    inline const_reference operator[](size_type n) const noexcept
    {
        return channel(n);
    }

    inline reference at(size_type n);

    inline const_reference at(size_type n) const;

    inline reference front() noexcept
    {
        return reference(start_, frames());
    }

    inline const_reference front() const noexcept
    {
        return const_reference(start_, frames());
    }

    inline reference back() noexcept
    {
        return reference(start_ + ((channels() - 1) * frames()), frames());
    }

    inline const_reference back() const noexcept
    {
        return reference(start_ + ((channels() - 1) * frames()), frames());
    }

    // iterators
    inline iterator begin() noexcept
    {
        return iterator(start_, frames());
    }

    inline const_iterator begin() const noexcept
    {
        return const_iterator(start_, frames());
    }

    inline iterator end() noexcept
    {
        return iterator(start_ + (channels() * frames()), frames());
    }

    inline const_iterator end() const noexcept
    {
        return const_iterator(start_ + (channels() * frames()), frames());
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

template<class Sample>
inline void BasicNoninterleavedSpan<Sample>::swap(BasicNoninterleavedSpan& other) noexcept
{
    std::swap(start_, other.start_);
    std::swap(channels_, other.channels_);
    std::swap(frames_, other.frames_);
}

template<class Sample>
inline typename BasicNoninterleavedSpan<Sample>::reference BasicNoninterleavedSpan<Sample>::at(size_type n)
{
    if (n >= channels())
    {
        throw std::out_of_range("Noninterleaved");
    }
    return (*this)[n];
}

template<class Sample>
inline typename BasicNoninterleavedSpan<Sample>::const_reference BasicNoninterleavedSpan<Sample>::at(size_type n) const
{
    if (n >= channels())
    {
        throw std::out_of_range("Noninterleaved");
    }
    return (*this)[n];
}

template<class Sample>
inline bool operator==(const BasicNoninterleavedSpan<Sample>& a, const BasicNoninterleavedSpan<Sample>& b) noexcept
{
    return (a.channels() == b.channels()) && (a.frames() == b.frames()) &&
           std::equal(a.data(), a.data() + a.samples(), b.data());
}

template<class Sample>
inline bool operator!=(const BasicNoninterleavedSpan<Sample>& a, const BasicNoninterleavedSpan<Sample>& b) noexcept
{
    return !(a == b);
}

template<class SampleType>
using NoninterleavedSpan = BasicNoninterleavedSpan<Sample<SampleType>>;

template<class SampleType>
using ConstNoninterleavedSpan =
    BasicNoninterleavedSpan<typename detail::SampleTraits<Sample<SampleType>>::const_sample>;

template<class SampleType>
using NetworkNoninterleavedSpan = BasicNoninterleavedSpan<NetworkSample<SampleType>>;

template<class SampleType>
using ConstNetworkNoninterleavedSpan =
    BasicNoninterleavedSpan<typename detail::SampleTraits<NetworkSample<SampleType>>::const_sample>;

} // namespace ratl

#endif // _ratl_noninterleaved_span_
