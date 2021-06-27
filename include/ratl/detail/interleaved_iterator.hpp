#ifndef _ratl_detail_interleaved_iterator_
#define _ratl_detail_interleaved_iterator_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/operator_arrow_proxy.hpp>
#include <ratl/detail/types.hpp>
#include <ratl/frame_span.hpp>

namespace ratl
{
namespace detail
{
template<class Sample>
class InterleavedIterator
{
private:
    using sample_traits = SampleTraits<Sample>;
    using sample = typename sample_traits::sample;
    using sample_pointer = typename sample_traits::pointer;
    using sample_reference = typename sample_traits::reference;

    using size_type = detail::types::size_type;

    using frame_type = BasicFrameSpan<sample, true>;

public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = frame_type;
    using difference_type = detail::types::difference_type;
    using pointer = detail::operator_arrow_proxy<frame_type>;
    using reference = frame_type;

private:
    sample_pointer data_ = nullptr;
    size_type channels_ = 0;

public:
    InterleavedIterator() noexcept = default;

    InterleavedIterator(sample_pointer data, size_type channels) noexcept : data_(data), channels_(channels) {}

    InterleavedIterator(const InterleavedIterator& other) noexcept = default;

    InterleavedIterator& operator=(const InterleavedIterator& other) noexcept = default;

    inline size_type channels() const noexcept
    {
        return channels_;
    }

    inline reference operator*() const noexcept
    {
        return reference(data_, channels_);
    }

    inline pointer operator->() const noexcept
    {
        return pointer(reference(data_, channels_));
    }

    inline reference operator[](difference_type n) const noexcept
    {
        return reference(data_ + (n * channels_), channels_);
    }

    inline InterleavedIterator& operator++() noexcept
    {
        data_ += channels_;
        return *this;
    }

    inline InterleavedIterator operator++(int) noexcept
    {
        auto&& tmp = InterleavedIterator(*this);
        ++(*this);
        return tmp;
    }

    inline InterleavedIterator& operator--() noexcept
    {
        data_ -= channels_;
        return *this;
    }

    inline InterleavedIterator operator--(int) noexcept
    {
        auto&& tmp = InterleavedIterator(*this);
        --(*this);
        return tmp;
    }

    inline InterleavedIterator operator+(difference_type n) const noexcept
    {
        auto&& w = InterleavedIterator(*this);
        w += n;
        return w;
    }

    inline InterleavedIterator& operator+=(difference_type n) noexcept
    {
        data_ += static_cast<difference_type>(n * channels_);
        return *this;
    }

    inline InterleavedIterator operator-(difference_type n) const noexcept
    {
        return *this + (-n);
    }

    inline InterleavedIterator& operator-=(difference_type n) noexcept
    {
        *this += -n;
        return *this;
    }

#if defined(RATL_CPP_VERSION_HAS_CPP20)

    inline bool operator==(const InterleavedIterator& other) const noexcept
    {
        return data_ == other.data_;
    }

    inline bool operator<(const InterleavedIterator& other) const noexcept
    {
        return data_ < other.data_;
    }

    inline bool operator<=>(const InterleavedIterator& other) const noexcept = default;

#else

    friend inline bool operator==(const InterleavedIterator& x, const InterleavedIterator& y) noexcept
    {
        return x.data_ == y.data_;
    }

    friend inline bool operator!=(const InterleavedIterator& x, const InterleavedIterator& y) noexcept
    {
        return !(x == y);
    }

    friend inline bool operator<(const InterleavedIterator& x, const InterleavedIterator& y) noexcept
    {
        return x.data_ < y.data_;
    }

    friend inline bool operator<=(const InterleavedIterator& x, const InterleavedIterator& y) noexcept
    {
        return !(x > y);
    }

    friend inline bool operator>(const InterleavedIterator& x, const InterleavedIterator& y) noexcept
    {
        return y < x;
    }

    friend inline bool operator>=(const InterleavedIterator& x, const InterleavedIterator& y) noexcept
    {
        return !(x < y);
    }

#endif

    friend inline InterleavedIterator operator+(typename InterleavedIterator::difference_type n, InterleavedIterator x)
    {
        x += n;
        return x;
    }

    friend inline typename InterleavedIterator::difference_type operator-(
        const InterleavedIterator& x, const InterleavedIterator& y)
    {
        return (x.data_ - y.data_) / static_cast<typename InterleavedIterator::difference_type>(x.channels_);
    }

    inline sample_pointer base() const noexcept
    {
        return data_;
    }
};

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_interleaved_iterator_
