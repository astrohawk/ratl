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
#include <ratl/detail/sample_traits.hpp>
#include <ratl/detail/types.hpp>
#include <ratl/frame_span.hpp>
#include <ratl/interleaved.hpp>
#include <ratl/network_sample.hpp>
#include <ratl/sample.hpp>

namespace ratl
{
template<class Sample>
class BasicInterleavedSpan;

template<class Sample>
inline bool operator==(const BasicInterleavedSpan<Sample>& a, const BasicInterleavedSpan<Sample>& b) noexcept;

template<class Sample>
class BasicInterleavedSpan
{
public:
    using sample_traits = detail::SampleTraits<Sample>;
    using sample = typename sample_traits::sample;
    using const_sample = typename sample_traits::const_sample;
    using sample_pointer = typename sample_traits::pointer;
    using const_sample_pointer = typename sample_traits::const_pointer;
    using sample_reference = typename sample_traits::reference;
    using const_sample_reference = typename sample_traits::const_reference;

    using channel_type = BasicChannelSpan<sample, false>;
    using const_channel_type = BasicChannelSpan<const_sample, false>;
    using frame_type = BasicFrameSpan<sample, true>;
    using const_frame_type = BasicFrameSpan<const_sample, true>;

    using char_pointer = std::conditional_t<std::is_const<sample>::value, const unsigned char*, unsigned char*>;

    using size_type = detail::types::size_type;
    using difference_type = detail::types::difference_type;

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
    sample_pointer start_;
    size_type channels_;
    size_type frames_;

public:
    BasicInterleavedSpan() noexcept : start_(), channels_(), frames_() {}

    BasicInterleavedSpan(sample_pointer data, size_type channels, size_type frames) noexcept :
        start_(data), channels_(channels), frames_(frames)
    {
    }

    BasicInterleavedSpan(char_pointer data, size_type channels, size_type frames) noexcept :
        start_(reinterpret_cast<sample_pointer>(data)), channels_(channels), frames_(frames)
    {
    }

    BasicInterleavedSpan(const BasicInterleavedSpan& other) noexcept :
        start_(other.data()), channels_(other.channels()), frames_(other.frames())
    {
    }

    template<
        class SampleArg,
        class Allocator,
        std::enable_if_t<std::is_same<SampleArg, std::remove_const_t<Sample>>::value, bool> = true>
    BasicInterleavedSpan(BasicInterleaved<SampleArg, Allocator>& interleaved) noexcept :
        start_(interleaved.data()), channels_(interleaved.channels()), frames_(interleaved.frames())
    {
    }

    template<
        class SampleArg,
        class Allocator,
        std::enable_if_t<std::is_same<typename detail::SampleTraits<SampleArg>::const_sample, Sample>::value, bool> =
            true>
    BasicInterleavedSpan(const BasicInterleaved<SampleArg, Allocator>& interleaved) noexcept :
        start_(interleaved.data()), channels_(interleaved.channels()), frames_(interleaved.frames())
    {
    }

    BasicInterleavedSpan& operator=(const BasicInterleavedSpan& other) noexcept = default;

    inline void swap(BasicInterleavedSpan& other) noexcept;

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
        return frame_type(data() + (n * channels()), channels());
    }

    inline const_frame_type frame(size_type n) const
    {
        return const_frame_type(data() + (n * channels()), channels());
    }

    inline channel_type channel(size_type n)
    {
        return channel_type(data() + n, frames(), channels());
    }

    inline const_channel_type channel(size_type n) const
    {
        return const_channel_type(data() + n, frames(), channels());
    }

    inline reference operator[](size_type n) noexcept
    {
        return frame(n);
    }

    inline const_reference operator[](size_type n) const noexcept
    {
        return frame(n);
    }

    inline reference at(size_type n);

    inline const_reference at(size_type n) const;

    inline reference front() noexcept
    {
        return reference(data(), channels());
    }

    inline const_reference front() const noexcept
    {
        return const_reference(data(), channels());
    }

    inline reference back() noexcept
    {
        return reference(data() + (channels() * (frames() - 1)), channels());
    }

    inline const_reference back() const noexcept
    {
        return const_reference(data() + (channels() * (frames() - 1)), channels());
    }

    // iterators
    inline iterator begin() noexcept
    {
        return iterator(data(), channels());
    }

    inline const_iterator begin() const noexcept
    {
        return const_iterator(data(), channels());
    }

    inline iterator end() noexcept
    {
        return iterator(data() + (channels() * frames()), channels());
    }

    inline const_iterator end() const noexcept
    {
        return const_iterator(data() + (channels() * frames()), channels());
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
inline void BasicInterleavedSpan<Sample>::swap(BasicInterleavedSpan& other) noexcept
{
    std::swap(start_, other.start_);
    std::swap(channels_, other.channels_);
    std::swap(frames_, other.frames_);
}

template<class Sample>
inline typename BasicInterleavedSpan<Sample>::reference BasicInterleavedSpan<Sample>::at(size_type n)
{
    if (n >= frames())
    {
        throw std::out_of_range("InterleavedSpan");
    }
    return (*this)[n];
}

template<class Sample>
inline typename BasicInterleavedSpan<Sample>::const_reference BasicInterleavedSpan<Sample>::at(size_type n) const
{
    if (n >= frames())
    {
        throw std::out_of_range("InterleavedSpan");
    }
    return (*this)[n];
}

template<class Sample>
inline bool operator==(const BasicInterleavedSpan<Sample>& a, const BasicInterleavedSpan<Sample>& b) noexcept
{
    return (a.channels() == b.channels()) && (a.frames() == b.frames()) &&
           std::equal(a.data(), a.data() + a.samples(), b.data());
}

template<class Sample>
inline bool operator!=(const BasicInterleavedSpan<Sample>& a, const BasicInterleavedSpan<Sample>& b) noexcept
{
    return !(a == b);
}

template<class SampleType>
using InterleavedSpan = BasicInterleavedSpan<Sample<SampleType>>;

template<class SampleType>
using ConstInterleavedSpan = BasicInterleavedSpan<typename detail::SampleTraits<Sample<SampleType>>::const_sample>;

template<class SampleType>
using NetworkInterleavedSpan = BasicInterleavedSpan<NetworkSample<SampleType>>;

template<class SampleType>
using ConstNetworkInterleavedSpan =
    BasicInterleavedSpan<typename detail::SampleTraits<NetworkSample<SampleType>>::const_sample>;

} // namespace ratl

#endif // _ratl_interleaved_span_
