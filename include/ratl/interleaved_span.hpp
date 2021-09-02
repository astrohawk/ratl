/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_interleaved_span_
#define _ratl_interleaved_span_

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

// other includes
#include <iterator>
#include <memory>
#include <type_traits>

namespace ratl
{
template<typename SampleType, typename SampleTraits>
class basic_interleaved_span
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

    using channel_type = basic_channel_span<sample_type, sample_traits, std::false_type>;
    using const_channel_type = basic_channel_span<const_sample_type, const_sample_traits, std::false_type>;
    using frame_type = basic_frame_span<sample_type, sample_traits, std::true_type>;
    using const_frame_type = basic_frame_span<const_sample_type, const_sample_traits, std::true_type>;

    using char_pointer = std::conditional_t<std::is_const<sample_type>::value, const unsigned char*, unsigned char*>;

    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using value_type = frame_type;
    using pointer = detail::operator_arrow_proxy<frame_type>;
    using const_pointer = const detail::operator_arrow_proxy<frame_type>;
    using reference = frame_type;
    using const_reference = const_frame_type;

    using iterator = detail::interleaved_iterator<sample_type, sample_traits>;
    using const_iterator = detail::interleaved_iterator<const_sample_type, const_sample_traits>;
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
        class Sample,
        typename Allocator,
        std::enable_if_t<std::is_same<Sample, std::remove_const_t<sample_type>>::value, bool> = true>
    basic_interleaved_span(basic_interleaved<Sample, Allocator>& interleaved) noexcept :
        start_(interleaved.data()), channels_(interleaved.channels()), frames_(interleaved.frames())
    {
    }

    template<
        class Sample,
        typename Allocator,
        std::enable_if_t<
            std::is_same<typename detail::sample_traits<Sample>::const_sample_type, sample_type>::value,
            bool> = true>
    basic_interleaved_span(const basic_interleaved<Sample, Allocator>& interleaved) noexcept :
        start_(interleaved.data()), channels_(interleaved.channels()), frames_(interleaved.frames())
    {
    }

    basic_interleaved_span& operator=(const basic_interleaved_span& other) noexcept = default;

    inline void swap(basic_interleaved_span& other) noexcept;

    inline sample_pointer data() const noexcept
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

template<typename SampleType, typename SampleTraits>
inline void basic_interleaved_span<SampleType, SampleTraits>::swap(basic_interleaved_span& other) noexcept
{
    std::swap(start_, other.start_);
    std::swap(channels_, other.channels_);
    std::swap(frames_, other.frames_);
}

template<typename SampleType, typename SampleTraits>
inline typename basic_interleaved_span<SampleType, SampleTraits>::reference basic_interleaved_span<
    SampleType,
    SampleTraits>::at(size_type n)
{
    if (n >= frames())
    {
        throw std::out_of_range("interleaved_span");
    }
    return (*this)[n];
}

template<typename SampleType, typename SampleTraits>
inline typename basic_interleaved_span<SampleType, SampleTraits>::const_reference basic_interleaved_span<
    SampleType,
    SampleTraits>::at(size_type n) const
{
    if (n >= frames())
    {
        throw std::out_of_range("interleaved_span");
    }
    return (*this)[n];
}

template<
    typename SampleTypeA,
    typename SampleTraitsA,
    typename SampleTypeB,
    typename SampleTraitsB,
    std::enable_if_t<
        std::is_same<typename SampleTraitsA::const_sample_type, typename SampleTraitsB::const_sample_type>::value,
        bool> = true>
inline bool operator==(
    const basic_interleaved_span<SampleTypeA, SampleTraitsA>& a,
    const basic_interleaved_span<SampleTypeB, SampleTraitsB>& b) noexcept
{
    return (a.channels() == b.channels()) && (a.frames() == b.frames()) &&
           std::equal(a.data(), a.data() + a.samples(), b.data());
}

template<
    typename SampleTypeA,
    typename SampleTraitsA,
    typename SampleTypeB,
    typename SampleTraitsB,
    std::enable_if_t<
        std::is_same<typename SampleTraitsA::const_sample_type, typename SampleTraitsB::const_sample_type>::value,
        bool> = true>
inline bool operator!=(
    const basic_interleaved_span<SampleTypeA, SampleTraitsA>& a,
    const basic_interleaved_span<SampleTypeB, SampleTraitsB>& b) noexcept
{
    return !(a == b);
}

template<typename SampleValueType>
using interleaved_span =
    basic_interleaved_span<sample<SampleValueType>, detail::sample_traits<sample<SampleValueType>>>;

template<typename SampleValueType>
using const_interleaved_span = basic_interleaved_span<
    typename detail::sample_traits<sample<SampleValueType>>::const_sample_type,
    detail::const_sample_traits_t<detail::sample_traits<sample<SampleValueType>>>>;

template<typename SampleValueType>
using network_interleaved_span =
    basic_interleaved_span<network_sample<SampleValueType>, detail::sample_traits<network_sample<SampleValueType>>>;

template<typename SampleValueType>
using const_network_interleaved_span = basic_interleaved_span<
    typename detail::sample_traits<network_sample<SampleValueType>>::const_sample_type,
    detail::const_sample_traits_t<detail::sample_traits<network_sample<SampleValueType>>>>;

} // namespace ratl

#endif // _ratl_interleaved_span_
