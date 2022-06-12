/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_noninterleaved_span_
#define _ratl_noninterleaved_span_

// ratl includes
#include <ratl/channel_span.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/detail/noninterleaved_iterator.hpp>
#include <ratl/detail/operator_arrow_proxy.hpp>
#include <ratl/detail/sample_traits.hpp>
#include <ratl/frame_span.hpp>
#include <ratl/network_sample.hpp>
#include <ratl/noninterleaved.hpp>
#include <ratl/sample.hpp>

// other includes
#include <iterator>
#include <memory>
#include <type_traits>

namespace ratl
{
template<typename SampleType, typename SampleTraits>
class basic_noninterleaved_span
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

    using channel_type = basic_channel_span<sample_type, sample_traits, std::true_type>;
    using const_channel_type = basic_channel_span<const_sample_type, const_sample_traits, std::true_type>;
    using frame_type = basic_frame_span<sample_type, sample_traits, std::false_type>;
    using const_frame_type = basic_frame_span<const_sample_type, const_sample_traits, std::false_type>;

    using char_pointer = std::conditional_t<std::is_const<sample_type>::value, const unsigned char*, unsigned char*>;

    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using value_type = channel_type;
    using pointer = detail::operator_arrow_proxy<channel_type>;
    using const_pointer = const detail::operator_arrow_proxy<channel_type>;
    using reference = channel_type;
    using const_reference = const_channel_type;

    using iterator = detail::noninterleaved_iterator<sample_type, sample_traits>;
    using const_iterator = detail::noninterleaved_iterator<const_sample_type, const_sample_traits>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    sample_pointer start_;
    size_type channels_;
    size_type frames_;

public:
    basic_noninterleaved_span() noexcept : start_(), channels_(), frames_() {}

    basic_noninterleaved_span(sample_pointer data, size_type channels, size_type frames) noexcept :
        start_(data), channels_(channels), frames_(frames)
    {
    }

    basic_noninterleaved_span(char_pointer data, size_type channels, size_type frames) noexcept :
        start_(reinterpret_cast<sample_pointer>(data)), channels_(channels), frames_(frames)
    {
    }

    basic_noninterleaved_span(const basic_noninterleaved_span& other) noexcept :
        start_(other.data()), channels_(other.channels()), frames_(other.frames())
    {
    }

    template<
        typename Sample,
        typename Allocator,
        std::enable_if_t<std::is_same<Sample, std::remove_const_t<sample_type>>::value, bool> = true>
    basic_noninterleaved_span(basic_interleaved<Sample, Allocator>& noninterleaved) noexcept :
        start_(noninterleaved.data()), channels_(noninterleaved.channels()), frames_(noninterleaved.frames())
    {
    }

    template<
        typename Sample,
        typename Allocator,
        std::enable_if_t<
            std::is_same<typename detail::sample_traits<Sample>::const_sample_type, sample_type>::value,
            bool> = true>
    basic_noninterleaved_span(const basic_interleaved<Sample, Allocator>& noninterleaved) noexcept :
        start_(noninterleaved.data()), channels_(noninterleaved.channels()), frames_(noninterleaved.frames())
    {
    }

    basic_noninterleaved_span& operator=(const basic_noninterleaved_span& other) noexcept = default;

    inline void swap(basic_noninterleaved_span& other) noexcept;

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

template<typename SampleType, typename SampleTraits>
inline void basic_noninterleaved_span<SampleType, SampleTraits>::swap(basic_noninterleaved_span& other) noexcept
{
    std::swap(start_, other.start_);
    std::swap(channels_, other.channels_);
    std::swap(frames_, other.frames_);
}

template<typename SampleType, typename SampleTraits>
inline typename basic_noninterleaved_span<SampleType, SampleTraits>::reference basic_noninterleaved_span<
    SampleType,
    SampleTraits>::at(size_type n)
{
    if (n >= channels())
    {
        throw std::out_of_range("noninterleaved");
    }
    return (*this)[n];
}

template<typename SampleType, typename SampleTraits>
inline typename basic_noninterleaved_span<SampleType, SampleTraits>::const_reference basic_noninterleaved_span<
    SampleType,
    SampleTraits>::at(size_type n) const
{
    if (n >= channels())
    {
        throw std::out_of_range("noninterleaved");
    }
    return (*this)[n];
}

template<typename SampleValueType>
using noninterleaved_span =
    basic_noninterleaved_span<sample<SampleValueType>, detail::sample_traits<sample<SampleValueType>>>;

template<typename SampleValueType>
using const_noninterleaved_span = basic_noninterleaved_span<
    typename detail::sample_traits<sample<SampleValueType>>::const_sample_type,
    detail::const_sample_traits_t<detail::sample_traits<sample<SampleValueType>>>>;

template<typename SampleValueType>
using network_noninterleaved_span =
    basic_noninterleaved_span<network_sample<SampleValueType>, detail::sample_traits<network_sample<SampleValueType>>>;

template<typename SampleValueType>
using const_network_noninterleaved_span = basic_noninterleaved_span<
    typename detail::sample_traits<network_sample<SampleValueType>>::const_sample_type,
    detail::const_sample_traits_t<detail::sample_traits<network_sample<SampleValueType>>>>;

} // namespace ratl

#endif // _ratl_noninterleaved_span_
