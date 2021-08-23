#ifndef _ratl_frame_span_
#define _ratl_frame_span_

// C++ Standard Library includes

// ratl includes
#include <ratl/detail/frame_iterator.hpp>
#include <ratl/detail/sample_span.hpp>
#include <ratl/frame.hpp>
#include <ratl/network_sample.hpp>
#include <ratl/sample.hpp>

namespace ratl
{
template<class Sample, bool Contiguous = false>
class BasicFrameSpan
{
    using data_impl_type = SampleSpan<Sample, Contiguous, detail::FrameIterator>;

public:
    using sample_traits = typename data_impl_type::sample_traits;
    using sample = typename sample_traits::sample;
    using const_sample = typename sample_traits::const_sample;
    using sample_pointer = typename data_impl_type::sample_pointer;
    using const_sample_pointer = typename data_impl_type::const_sample_pointer;
    using sample_reference = typename data_impl_type::sample_reference;
    using const_sample_reference = typename data_impl_type::const_sample_reference;

    using size_type = typename data_impl_type::size_type;
    using difference_type = typename data_impl_type::difference_type;

    using value_type = typename data_impl_type::value_type;
    using reference = typename data_impl_type::reference;
    using const_reference = typename data_impl_type::const_reference;
    using pointer = typename data_impl_type::pointer;
    using const_pointer = typename data_impl_type::const_pointer;

    using iterator = typename data_impl_type::iterator;
    using const_iterator = typename data_impl_type::const_iterator;
    using reverse_iterator = typename data_impl_type::reverse_iterator;
    using const_reverse_iterator = typename data_impl_type::const_reverse_iterator;

private:
    data_impl_type data_;

public:
    BasicFrameSpan() noexcept = default;

    BasicFrameSpan(const BasicFrameSpan&) noexcept = default;

    template<bool DummyContiguous = Contiguous, std::enable_if_t<DummyContiguous == false, bool> = true>
    BasicFrameSpan(sample_pointer data, size_type channels, size_type stride) noexcept : data_(data, channels, stride)
    {
    }

    template<bool DummyContiguous = Contiguous, std::enable_if_t<DummyContiguous == true, bool> = true>
    BasicFrameSpan(sample_pointer data, size_type channels) noexcept : data_(data, channels)
    {
    }

    template<bool DummyContiguous = Contiguous, std::enable_if_t<DummyContiguous == false, bool> = true>
    BasicFrameSpan(unsigned char* data, size_type channels, size_type stride) noexcept :
        data_(reinterpret_cast<sample_pointer>(data), channels, stride)
    {
    }

    template<bool DummyContiguous = Contiguous, std::enable_if_t<DummyContiguous == true, bool> = true>
    BasicFrameSpan(unsigned char* data, size_type channels) noexcept :
        data_(reinterpret_cast<sample_pointer>(data), channels)
    {
    }

    template<
        class SampleArg,
        class Allocator,
        std::enable_if_t<std::is_same<SampleArg, std::remove_const_t<Sample>>::value, bool> = true>
    BasicFrameSpan(BasicFrame<SampleArg, Allocator>& frame) noexcept : data_(frame.data(), frame.channels())
    {
    }

    template<
        class SampleArg,
        class Allocator,
        std::enable_if_t<std::is_same<typename detail::SampleTraits<SampleArg>::const_sample, Sample>::value, bool> =
            true>
    BasicFrameSpan(const BasicFrame<SampleArg, Allocator>& frame) noexcept : data_(frame.data(), frame.channels())
    {
    }

    BasicFrameSpan& operator=(const BasicFrameSpan&) noexcept = default;

    void swap(BasicFrameSpan& other)
    {
        std::swap(data_, other.data_);
    }

    inline sample_pointer data() noexcept
    {
        return data_.data();
    }

    inline const sample_pointer data() const noexcept
    {
        return data_.data();
    }

    inline size_type channels() const noexcept
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

template<class Sample, bool Contiguous>
inline typename BasicFrameSpan<Sample, Contiguous>::reference BasicFrameSpan<Sample, Contiguous>::at(size_type n)
{
    if (n >= channels())
    {
        throw std::out_of_range("FrameSpan");
    }
    return (*this)[n];
}

template<class Sample, bool Contiguous>
inline typename BasicFrameSpan<Sample, Contiguous>::const_reference BasicFrameSpan<Sample, Contiguous>::at(
    size_type n) const
{
    if (n >= channels())
    {
        throw std::out_of_range("FrameSpan");
    }
    return (*this)[n];
}

template<class Sample, bool ContiguousA, bool ContiguousB>
inline bool operator==(
    const BasicFrameSpan<Sample, ContiguousA>& a, const BasicFrameSpan<Sample, ContiguousB>& b) noexcept
{
    return (a.channels() == b.channels()) && std::equal(a.begin(), a.end(), b.begin());
}

template<class Sample, bool ContiguousA, bool ContiguousB>
inline bool operator!=(
    const BasicFrameSpan<Sample, ContiguousA>& a, const BasicFrameSpan<Sample, ContiguousB>& b) noexcept
{
    return !(a == b);
}

template<class SampleType, bool Contiguous = false>
using FrameSpan = BasicFrameSpan<Sample<SampleType>, Contiguous>;

template<class SampleType, bool Contiguous = false>
using ConstFrameSpan = BasicFrameSpan<typename detail::SampleTraits<Sample<SampleType>>::const_sample, Contiguous>;

template<class SampleType, bool Contiguous = false>
using NetworkFrameSpan = BasicFrameSpan<NetworkSample<SampleType>, Contiguous>;

template<class SampleType, bool Contiguous = false>
using ConstNetworkFrameSpan =
    BasicFrameSpan<typename detail::SampleTraits<NetworkSample<SampleType>>::const_sample, Contiguous>;

} // namespace ratl

#endif // _ratl_frame_span_
