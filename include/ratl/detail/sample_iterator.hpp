#ifndef _ratl_detail_sample_iterator_
#define _ratl_detail_sample_iterator_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/sample_traits.hpp>
#include <ratl/detail/types.hpp>
#include <ratl/sample.hpp>

namespace ratl
{
namespace detail
{
template<class Tag, class Sample, bool Contiguous>
class SampleIterator;

template<class Tag, class Sample>
class SampleIterator<Tag, Sample, false>
{
    using sample_traits = SampleTraits<Sample>;
    using sample = typename sample_traits::sample;
    using sample_pointer = typename sample_traits::pointer;
    using sample_reference = typename sample_traits::reference;

    using size_type = detail::types::size_type;

public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = sample;
    using difference_type = detail::types::difference_type;
    using pointer = sample_pointer;
    using reference = sample_reference;

private:
    pointer data_ = nullptr;
    size_type stride_ = 0;

public:
    SampleIterator() noexcept = default;

    SampleIterator(pointer data, size_type stride) noexcept : data_(data), stride_(stride) {}

    SampleIterator(const SampleIterator& other) noexcept = default;

    SampleIterator& operator=(const SampleIterator& other) noexcept = default;

    inline size_type stride() const noexcept
    {
        return stride_;
    }

    inline reference operator*() const noexcept
    {
        return *data_;
    }

    inline pointer operator->() const noexcept
    {
        return data_;
    }

    inline reference operator[](difference_type n) const noexcept
    {
        return data_[n * stride_];
    }

    inline SampleIterator& operator++() noexcept
    {
        data_ += stride_;
        return *this;
    }

    inline SampleIterator operator++(int) noexcept
    {
        auto&& tmp = SampleIterator(*this);
        ++(*this);
        return tmp;
    }

    inline SampleIterator& operator--() noexcept
    {
        data_ -= stride_;
        return *this;
    }

    inline SampleIterator operator--(int) noexcept
    {
        auto&& tmp = SampleIterator(*this);
        --(*this);
        return tmp;
    }

    inline SampleIterator operator+(difference_type n) const noexcept
    {
        auto&& w = SampleIterator(*this);
        w += n;
        return w;
    }

    inline SampleIterator& operator+=(difference_type n) noexcept
    {
        data_ += static_cast<difference_type>(n * stride_);
        return *this;
    }

    inline SampleIterator operator-(difference_type n) const noexcept
    {
        return *this + (-n);
    }

    inline SampleIterator& operator-=(difference_type n) noexcept
    {
        *this += -n;
        return *this;
    }

    friend inline bool operator==(const SampleIterator& x, const SampleIterator& y)
    {
        return x.data_ == y.data_;
    }

    friend inline bool operator!=(const SampleIterator& x, const SampleIterator& y)
    {
        return !(x == y);
    }

    friend inline bool operator<(const SampleIterator& x, const SampleIterator& y)
    {
        return x.data_ < y.data_;
    }

    friend inline bool operator<=(const SampleIterator& x, const SampleIterator& y)
    {
        return !(x > y);
    }

    friend inline bool operator>(const SampleIterator& x, const SampleIterator& y)
    {
        return y < x;
    }

    friend inline bool operator>=(const SampleIterator& x, const SampleIterator& y)
    {
        return !(x < y);
    }

    friend inline SampleIterator operator+(typename SampleIterator::difference_type n, SampleIterator x)
    {
        x += n;
        return x;
    }

    friend inline typename SampleIterator::difference_type operator-(const SampleIterator& x, const SampleIterator& y)
    {
        return (x.data_ - y.data_) / static_cast<typename SampleIterator::difference_type>(x.stride_);
    }

    inline pointer base() const noexcept
    {
        return data_;
    }
};

template<class Tag, class Sample>
class SampleIterator<Tag, Sample, true>
{
    using sample_traits = SampleTraits<Sample>;
    using sample = typename sample_traits::sample;
    using sample_pointer = typename sample_traits::pointer;
    using sample_reference = typename sample_traits::reference;

    using size_type = detail::types::size_type;

public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = sample;
    using difference_type = detail::types::difference_type;
    using pointer = sample_pointer;
    using reference = sample_reference;

private:
    pointer data_ = nullptr;

public:
    SampleIterator() noexcept = default;

    explicit SampleIterator(pointer data) noexcept : data_(data) {}

    SampleIterator(const SampleIterator& other) noexcept = default;

    SampleIterator& operator=(const SampleIterator& other) noexcept = default;

    inline size_type stride() const noexcept
    {
        return 1;
    }

    inline reference operator*() const noexcept
    {
        return *data_;
    }

    inline pointer operator->() const noexcept
    {
        return data_;
    }

    inline reference operator[](difference_type n) const noexcept
    {
        return data_[n];
    }

    inline SampleIterator& operator++() noexcept
    {
        ++data_;
        return *this;
    }

    inline SampleIterator operator++(int) noexcept
    {
        auto&& tmp = SampleIterator(*this);
        ++(*this);
        return tmp;
    }

    inline SampleIterator& operator--() noexcept
    {
        --data_;
        return *this;
    }

    inline SampleIterator operator--(int) noexcept
    {
        auto&& tmp = SampleIterator(*this);
        --(*this);
        return tmp;
    }

    inline SampleIterator operator+(difference_type n) const noexcept
    {
        auto&& w = SampleIterator(*this);
        w += n;
        return w;
    }

    inline SampleIterator& operator+=(difference_type n) noexcept
    {
        data_ += n;
        return *this;
    }

    inline SampleIterator operator-(difference_type n) const noexcept
    {
        return *this + (-n);
    }

    inline SampleIterator& operator-=(difference_type n) noexcept
    {
        *this += -n;
        return *this;
    }

    friend inline bool operator==(const SampleIterator& x, const SampleIterator& y)
    {
        return x.data_ == y.data_;
    }

    friend inline bool operator!=(const SampleIterator& x, const SampleIterator& y)
    {
        return !(x == y);
    }

    friend inline bool operator<(const SampleIterator& x, const SampleIterator& y)
    {
        return x.data_ < y.data_;
    }

    friend inline bool operator<=(const SampleIterator& x, const SampleIterator& y)
    {
        return !(x > y);
    }

    friend inline bool operator>(const SampleIterator& x, const SampleIterator& y)
    {
        return y < x;
    }

    friend inline bool operator>=(const SampleIterator& x, const SampleIterator& y)
    {
        return !(x < y);
    }

    friend inline SampleIterator operator+(typename SampleIterator::difference_type n, SampleIterator x)
    {
        x += n;
        return x;
    }

    friend inline typename SampleIterator::difference_type operator-(const SampleIterator& x, const SampleIterator& y)
    {
        return x.data_ - y.data_;
    }

    inline pointer base() const noexcept
    {
        return data_;
    }
};

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_sample_iterator_
