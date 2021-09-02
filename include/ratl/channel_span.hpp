/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_channel_span_
#define _ratl_channel_span_

// ratl includes
#include <ratl/channel.hpp>
#include <ratl/detail/channel_iterator.hpp>
#include <ratl/detail/sample_span.hpp>
#include <ratl/network_sample.hpp>
#include <ratl/sample.hpp>

namespace ratl
{
template<typename SampleType, typename SampleTraits, typename Contiguous = std::false_type>
class basic_channel_span
{
    using data_impl_type = sample_span<SampleType, SampleTraits, Contiguous, detail::channel_iterator>;

public:
    using sample_type = typename data_impl_type::sample_type;
    using const_sample_type = typename data_impl_type::const_sample_type;
    using sample_pointer = typename data_impl_type::sample_pointer;
    using const_sample_pointer = typename data_impl_type::const_sample_pointer;

    using char_pointer = std::conditional_t<std::is_const<sample_type>::value, const unsigned char*, unsigned char*>;

    using size_type = typename data_impl_type::size_type;
    using difference_type = typename data_impl_type::difference_type;

    using value_type = typename data_impl_type::value_type;
    using pointer = typename data_impl_type::pointer;
    using const_pointer = typename data_impl_type::const_pointer;
    using reference = typename data_impl_type::reference;
    using const_reference = typename data_impl_type::const_reference;

    using iterator = typename data_impl_type::iterator;
    using const_iterator = typename data_impl_type::const_iterator;
    using reverse_iterator = typename data_impl_type::reverse_iterator;
    using const_reverse_iterator = typename data_impl_type::const_reverse_iterator;

private:
    data_impl_type data_;

public:
    basic_channel_span() noexcept = default;

    basic_channel_span(const basic_channel_span&) noexcept = default;

    template<
        typename DummyContiguous = Contiguous,
        std::enable_if_t<std::is_same<DummyContiguous, std::false_type>::value, bool> = true>
    basic_channel_span(sample_pointer data, size_type samples, size_type stride) noexcept : data_(data, samples, stride)
    {
    }

    template<
        typename DummyContiguous = Contiguous,
        std::enable_if_t<std::is_same<DummyContiguous, std::true_type>::value, bool> = true>
    basic_channel_span(sample_pointer data, size_type samples) noexcept : data_(data, samples)
    {
    }

    template<
        typename DummyContiguous = Contiguous,
        std::enable_if_t<std::is_same<DummyContiguous, std::false_type>::value, bool> = true>
    basic_channel_span(char_pointer data, size_type samples, size_type stride) noexcept :
        data_(reinterpret_cast<sample_pointer>(data), samples, stride)
    {
    }

    template<
        typename DummyContiguous = Contiguous,
        std::enable_if_t<std::is_same<DummyContiguous, std::true_type>::value, bool> = true>
    basic_channel_span(char_pointer data, size_type samples) noexcept :
        data_(reinterpret_cast<sample_pointer>(data), samples)
    {
    }

    template<
        class Sample,
        typename Allocator,
        std::enable_if_t<std::is_same<Sample, std::remove_const_t<sample_type>>::value, bool> = true>
    basic_channel_span(basic_channel<Sample, Allocator>& channel) noexcept : data_(channel.data(), channel.samples())
    {
    }

    template<
        class Sample,
        typename Allocator,
        std::enable_if_t<
            std::is_same<typename detail::sample_traits<Sample>::const_sample_type, sample_type>::value,
            bool> = true>
    basic_channel_span(const basic_channel<Sample, Allocator>& channel) noexcept :
        data_(channel.data(), channel.samples())
    {
    }

    basic_channel_span& operator=(const basic_channel_span&) noexcept = default;

    void swap(basic_channel_span& other)
    {
        std::swap(data_, other.data_);
    }

    inline sample_pointer data() const noexcept
    {
        return data_.data();
    }

    inline size_type samples() const noexcept
    {
        return data_.samples();
    }

    inline size_type stride() const noexcept
    {
        return data_.stride();
    }

    inline bool empty() const noexcept
    {
        return data_.empty();
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
        return data_.front();
    }

    inline const_reference front() const noexcept
    {
        return data_.front();
    }

    inline reference back() noexcept
    {
        return data_.back();
    }

    inline const_reference back() const noexcept
    {
        return data_.back();
    }

    // iterators
    inline iterator begin() noexcept
    {
        return data_.begin();
    }

    inline const_iterator begin() const noexcept
    {
        return data_.begin();
    }

    inline iterator end() noexcept
    {
        return data_.end();
    }

    inline const_iterator end() const noexcept
    {
        return data_.end();
    }

    // reverse iterators
    inline reverse_iterator rbegin() noexcept
    {
        return data_.rbegin();
    }

    inline const_reverse_iterator rbegin() const noexcept
    {
        return data_.rbegin();
    }

    inline reverse_iterator rend() noexcept
    {
        return data_.rend();
    }

    inline const_reverse_iterator rend() const noexcept
    {
        return data_.rend();
    }

    // const iterators
    inline const_iterator cbegin() const noexcept
    {
        return data_.cbegin();
    }

    inline const_iterator cend() const noexcept
    {
        return data_.cend();
    }

    inline const_reverse_iterator crbegin() const noexcept
    {
        return data_.crbegin();
    }

    inline const_reverse_iterator crend() const noexcept
    {
        return data_.crend();
    }
};

template<typename SampleType, typename SampleTraits, typename Contiguous>
inline typename basic_channel_span<SampleType, SampleTraits, Contiguous>::reference basic_channel_span<
    SampleType,
    SampleTraits,
    Contiguous>::at(size_type n)
{
    if (n >= samples())
    {
        throw std::out_of_range("channel_span");
    }
    return (*this)[n];
}

template<typename SampleType, typename SampleTraits, typename Contiguous>
inline typename basic_channel_span<SampleType, SampleTraits, Contiguous>::const_reference basic_channel_span<
    SampleType,
    SampleTraits,
    Contiguous>::at(size_type n) const
{
    if (n >= samples())
    {
        throw std::out_of_range("channel_span");
    }
    return (*this)[n];
}

template<
    typename SampleTypeA,
    typename SampleTraitsA,
    typename ContiguousA,
    typename SampleTypeB,
    typename SampleTraitsB,
    typename ContiguousB,
    std::enable_if_t<
        std::is_same<typename SampleTraitsA::const_sample_type, typename SampleTraitsB::const_sample_type>::value,
        bool> = true>
inline bool operator==(
    const basic_channel_span<SampleTypeA, SampleTraitsA, ContiguousA>& a,
    const basic_channel_span<SampleTypeB, SampleTraitsB, ContiguousB>& b) noexcept
{
    return (a.samples() == b.samples()) && std::equal(a.begin(), a.end(), b.begin());
}

template<
    typename SampleTypeA,
    typename SampleTraitsA,
    typename ContiguousA,
    typename SampleTypeB,
    typename SampleTraitsB,
    typename ContiguousB,
    std::enable_if_t<
        std::is_same<typename SampleTraitsA::const_sample_type, typename SampleTraitsB::const_sample_type>::value,
        bool> = true>
inline bool operator!=(
    const basic_channel_span<SampleTypeA, SampleTraitsA, ContiguousA>& a,
    const basic_channel_span<SampleTypeB, SampleTraitsB, ContiguousB>& b) noexcept
{
    return !(a == b);
}

template<typename SampleValueType>
using channel_span = basic_channel_span<sample<SampleValueType>, detail::sample_traits<sample<SampleValueType>>>;

template<typename SampleValueType>
using const_channel_span = basic_channel_span<
    typename detail::sample_traits<sample<SampleValueType>>::const_sample_type,
    detail::const_sample_traits_t<detail::sample_traits<sample<SampleValueType>>>>;

template<typename SampleValueType>
using network_channel_span =
    basic_channel_span<network_sample<SampleValueType>, detail::sample_traits<network_sample<SampleValueType>>>;

template<typename SampleValueType>
using const_network_channel_span = basic_channel_span<
    typename detail::sample_traits<network_sample<SampleValueType>>::const_sample_type,
    detail::const_sample_traits_t<detail::sample_traits<network_sample<SampleValueType>>>>;

} // namespace ratl

#endif // _ratl_channel_span_
