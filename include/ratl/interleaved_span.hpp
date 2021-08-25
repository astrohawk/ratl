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
#include <ratl/frame_span.hpp>
#include <ratl/interleaved.hpp>
#include <ratl/network_sample.hpp>
#include <ratl/sample.hpp>

namespace ratl
{
template<class SampleType>
class basic_interleaved_span
{
    using sample_traits = detail::sample_traits<SampleType>;

public:
    using sample_type = typename sample_traits::sample_type;
    using const_sample_type = typename sample_traits::const_sample_type;
    using sample_pointer = typename sample_traits::pointer;
    using const_sample_pointer = typename sample_traits::const_pointer;
    using sample_reference = typename sample_traits::reference;
    using const_sample_reference = typename sample_traits::const_reference;

    using channel_type = basic_channel_span<sample_type, false>;
    using const_channel_type = basic_channel_span<const_sample_type, false>;
    using frame_type = basic_frame_span<sample_type, true>;
    using const_frame_type = basic_frame_span<const_sample_type, true>;

    using char_pointer = std::conditional_t<std::is_const<sample_type>::value, const unsigned char*, unsigned char*>;

    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using value_type = frame_type;
    using pointer = detail::operator_arrow_proxy<value_type>;
    using const_pointer = const detail::operator_arrow_proxy<value_type>;
    using reference = frame_type;
    using const_reference = const_frame_type;

    using iterator = detail::interleaved_iterator<sample_type>;
    using const_iterator = detail::interleaved_iterator<const_sample_type>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    sample_pointer start_;
    size_type channels_;
    size_type frames_;

public:
    basic_interleaved_span() noexcept : start_(), channels_(), frames_() {}

    basic_interleaved_span(sample_pointer data, size_type channels, size_type frames) noexcept :
        start_(data), channels_(channels), frames_(frames)
    {
    }

    basic_interleaved_span(char_pointer data, size_type channels, size_type frames) noexcept :
        start_(reinterpret_cast<sample_pointer>(data)), channels_(channels), frames_(frames)
    {
    }

    basic_interleaved_span(const basic_interleaved_span& other) noexcept :
        start_(other.data()), channels_(other.channels()), frames_(other.frames())
    {
    }

    template<
        class SampleArg,
        class Allocator,
        std::enable_if_t<std::is_same<SampleArg, std::remove_const_t<sample_type>>::value, bool> = true>
    basic_interleaved_span(basic_interleaved<SampleArg, Allocator>& interleaved) noexcept :
        start_(interleaved.data()), channels_(interleaved.channels()), frames_(interleaved.frames())
    {
    }

    template<
        class SampleArg,
        class Allocator,
        std::enable_if_t<
            std::is_same<typename detail::sample_traits<SampleArg>::const_sample, sample_type>::value,
            bool> = true>
    basic_interleaved_span(const basic_interleaved<SampleArg, Allocator>& interleaved) noexcept :
        start_(interleaved.data()), channels_(interleaved.channels()), frames_(interleaved.frames())
    {
    }

    basic_interleaved_span& operator=(const basic_interleaved_span& other) noexcept = default;

    inline void swap(basic_interleaved_span& other) noexcept;

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

template<class SampleType>
inline void basic_interleaved_span<SampleType>::swap(basic_interleaved_span& other) noexcept
{
    std::swap(start_, other.start_);
    std::swap(channels_, other.channels_);
    std::swap(frames_, other.frames_);
}

template<class SampleType>
inline typename basic_interleaved_span<SampleType>::reference basic_interleaved_span<SampleType>::at(size_type n)
{
    if (n >= frames())
    {
        throw std::out_of_range("interleaved_span");
    }
    return (*this)[n];
}

template<class SampleType>
inline typename basic_interleaved_span<SampleType>::const_reference basic_interleaved_span<SampleType>::at(
    size_type n) const
{
    if (n >= frames())
    {
        throw std::out_of_range("interleaved_span");
    }
    return (*this)[n];
}

template<class SampleType>
inline bool operator==(
    const basic_interleaved_span<SampleType>& a, const basic_interleaved_span<SampleType>& b) noexcept
{
    return (a.channels() == b.channels()) && (a.frames() == b.frames()) &&
           std::equal(a.data(), a.data() + a.samples(), b.data());
}

template<class SampleType>
inline bool operator!=(
    const basic_interleaved_span<SampleType>& a, const basic_interleaved_span<SampleType>& b) noexcept
{
    return !(a == b);
}

template<class SampleValueType>
using interleaved_span = basic_interleaved_span<sample<SampleValueType>>;

template<class SampleValueType>
using const_interleaved_span =
    basic_interleaved_span<typename detail::sample_traits<sample<SampleValueType>>::const_sample>;

template<class SampleValueType>
using network_interleaved_span = basic_interleaved_span<network_sample<SampleValueType>>;

template<class SampleValueType>
using const_network_interleaved_span =
    basic_interleaved_span<typename detail::sample_traits<network_sample<SampleValueType>>::const_sample>;

} // namespace ratl

#endif // _ratl_interleaved_span_
