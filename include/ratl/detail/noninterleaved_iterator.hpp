#ifndef _ratl_detail_noninterleaved_iterator_
#define _ratl_detail_noninterleaved_iterator_

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
class NoninterleavedIterator
{
private:
    using sample_traits = SampleTraits<Sample>;
    using sample = typename sample_traits::sample;
    using sample_pointer = typename sample_traits::pointer;
    using sample_reference = typename sample_traits::reference;

    using size_type = detail::types::size_type;

    using frame_type = BasicChannelSpan<sample, true>;

public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = frame_type;
    using difference_type = detail::types::difference_type;
    using pointer = detail::operator_arrow_proxy<frame_type>;
    using reference = frame_type;

private:
    sample_pointer data_ = nullptr;
    size_type frames_ = 0;

public:
    NoninterleavedIterator() noexcept = default;

    NoninterleavedIterator(sample_pointer data, size_type frames) noexcept : data_(data), frames_(frames) {}

    NoninterleavedIterator(const NoninterleavedIterator& other) noexcept = default;

    NoninterleavedIterator& operator=(const NoninterleavedIterator& other) noexcept = default;

    inline size_type frames() const noexcept
    {
        return frames_;
    }

    inline reference operator*() const noexcept
    {
        return reference(data_, frames_);
    }

    inline pointer operator->() const noexcept
    {
        return pointer(reference(data_, frames_));
    }

    inline reference operator[](difference_type n) const noexcept
    {
        return reference(data_ + (n * frames_), frames_);
    }

    inline NoninterleavedIterator& operator++() noexcept
    {
        data_ += frames_;
        return *this;
    }

    inline NoninterleavedIterator operator++(int) noexcept
    {
        auto&& tmp = NoninterleavedIterator(*this);
        ++(*this);
        return tmp;
    }

    inline NoninterleavedIterator& operator--() noexcept
    {
        data_ -= frames_;
        return *this;
    }

    inline NoninterleavedIterator operator--(int) noexcept
    {
        auto&& tmp = NoninterleavedIterator(*this);
        --(*this);
        return tmp;
    }

    inline NoninterleavedIterator operator+(difference_type n) const noexcept
    {
        auto&& w = NoninterleavedIterator(*this);
        w += n;
        return w;
    }

    inline NoninterleavedIterator& operator+=(difference_type n) noexcept
    {
        data_ += static_cast<difference_type>(n * frames_);
        return *this;
    }

    inline NoninterleavedIterator operator-(difference_type n) const noexcept
    {
        return *this + (-n);
    }

    inline NoninterleavedIterator& operator-=(difference_type n) noexcept
    {
        *this += -n;
        return *this;
    }

    friend inline bool operator==(const NoninterleavedIterator& x, const NoninterleavedIterator& y)
    {
        return x.data_ == y.data_;
    }

    friend inline bool operator!=(const NoninterleavedIterator& x, const NoninterleavedIterator& y)
    {
        return !(x == y);
    }

    friend inline bool operator<(const NoninterleavedIterator& x, const NoninterleavedIterator& y)
    {
        return x.data_ < y.data_;
    }

    friend inline bool operator<=(const NoninterleavedIterator& x, const NoninterleavedIterator& y)
    {
        return !(x > y);
    }

    friend inline bool operator>(const NoninterleavedIterator& x, const NoninterleavedIterator& y)
    {
        return y < x;
    }

    friend inline bool operator>=(const NoninterleavedIterator& x, const NoninterleavedIterator& y)
    {
        return !(x < y);
    }

    friend inline NoninterleavedIterator operator+(
        typename NoninterleavedIterator::difference_type n, NoninterleavedIterator x)
    {
        x += n;
        return x;
    }

    friend inline typename NoninterleavedIterator::difference_type operator-(
        const NoninterleavedIterator& x, const NoninterleavedIterator& y)
    {
        return (x.data_ - y.data_) / static_cast<typename NoninterleavedIterator::difference_type>(x.frames_);
    }

    inline sample_pointer base() const noexcept
    {
        return data_;
    }
};

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_noninterleaved_iterator_
