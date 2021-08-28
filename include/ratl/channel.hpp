/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_channel_
#define _ratl_channel_

// ratl includes
#include <ratl/allocator.hpp>
#include <ratl/detail/channel_iterator.hpp>
#include <ratl/detail/sample_span.hpp>
#include <ratl/network_sample.hpp>
#include <ratl/sample.hpp>

// other includes
#include <iterator>
#include <memory>
#include <type_traits>

namespace ratl
{
template<typename SampleType, typename Allocator = ratl::allocator<SampleType>>
class basic_channel
{
public:
    using allocator_type = Allocator;

    static_assert(
        std::is_same<std::remove_cv_t<SampleType>, SampleType>::value,
        "sample_type must be non-const and non-volatile");
    static_assert(
        std::is_same<typename allocator_type::value_type, SampleType>::value,
        "allocator::value_type must be same type as sample_type");

private:
    using alloc_traits = std::allocator_traits<allocator_type>;
    using sample_traits = detail::sample_traits_from_traits_t<alloc_traits>;
    using const_sample_traits = detail::const_sample_traits_t<sample_traits>;

    using data_impl_type = sample_span<sample_traits, true, detail::channel_iterator>;

public:
    using sample_type = typename data_impl_type::sample_type;
    using const_sample_type = typename data_impl_type::const_sample_type;
    using sample_pointer = typename data_impl_type::sample_pointer;
    using const_sample_pointer = typename data_impl_type::const_sample_pointer;

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
    allocator_type alloc_;
    data_impl_type data_;

public:
    basic_channel() noexcept(std::is_nothrow_default_constructible<allocator_type>::value) = default;

    explicit basic_channel(const allocator_type& alloc) noexcept : alloc_(alloc) {}

    explicit basic_channel(size_type samples);

    basic_channel(size_type samples, const allocator_type& alloc);

    basic_channel(const basic_channel& other);

    basic_channel(const basic_channel& other, const allocator_type& alloc);

    basic_channel(basic_channel&& other) noexcept = default;

#if defined(RATL_CPP_VERSION_HAS_CPP17)
    basic_channel(basic_channel&& other, const allocator_type& alloc) noexcept(alloc_traits::is_always_equal::value) :
        basic_channel(other, alloc, alloc_traits::is_always_equal)
    {
    }
#else
    basic_channel(basic_channel&& other, const allocator_type& alloc) : basic_channel(other, alloc, std::false_type())
    {
    }
#endif

    ~basic_channel()
    {
        if (data() != nullptr)
        {
            deallocate();
        }
    }

    basic_channel& operator=(const basic_channel& other);

    basic_channel& operator=(basic_channel&& other) noexcept(
        alloc_traits::propagate_on_container_move_assignment::value);

    allocator_type get_allocator() const noexcept
    {
        return alloc_;
    }

    void swap(basic_channel& other);

    inline sample_pointer data() noexcept
    {
        return data_.data();
    }

    inline const sample_pointer data() const noexcept
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

private:
    basic_channel(basic_channel&& other, const allocator_type& alloc, std::true_type) noexcept :
        alloc_(alloc), data_(std::move(other.data_))
    {
    }

    basic_channel(basic_channel&& other, const allocator_type& alloc, std::false_type);

    void allocate()
    {
        data_.start_ = alloc_traits::allocate(alloc_, samples());
        // we don't need to default construct the samples as sample types are trivially default constructable
    }

    void deallocate()
    {
        alloc_traits::deallocate(alloc_, data_.start_, samples());
    }

    void copy_assign_alloc(const basic_channel& other)
    {
        copy_assign_alloc(other, typename alloc_traits::propagate_on_container_copy_assignment());
    }

    void copy_assign_alloc(const basic_channel& other, std::true_type)
    {
        if (alloc_ != other.alloc_)
        {
            deallocate();
        }
        alloc_ = other.alloc_;
    }

    void copy_assign_alloc(const basic_channel&, std::false_type) {}

    void move_assign(basic_channel& other, std::true_type) noexcept;

    void move_assign(basic_channel& other, std::false_type);

    void move_assign_alloc(basic_channel& other) noexcept
    {
        move_assign_alloc(other, typename alloc_traits::propagate_on_container_move_assignment());
    }

    void move_assign_alloc(basic_channel& other, std::true_type) noexcept
    {
        alloc_ = std::move(other.alloc_);
    }

    void move_assign_alloc(basic_channel&, std::false_type) noexcept {}

    void swap_alloc(basic_channel& other, std::true_type) noexcept
    {
        std::swap(alloc_, other.alloc_);
    }

    void swap_alloc(basic_channel&, std::false_type) noexcept {}
};

template<typename SampleType, typename Allocator>
basic_channel<SampleType, Allocator>::basic_channel(size_type samples) : data_(samples)
{
    if (samples() > 0)
    {
        allocate();
        std::fill_n(data(), samples(), sample_type());
    }
}

template<typename SampleType, typename Allocator>
basic_channel<SampleType, Allocator>::basic_channel(size_type samples, const allocator_type& alloc) :
    alloc_(alloc), data_(samples)
{
    if (samples() > 0)
    {
        allocate();
        std::fill_n(data(), samples(), sample_type());
    }
}

template<typename SampleType, typename Allocator>
basic_channel<SampleType, Allocator>::basic_channel(const basic_channel& other) :
    alloc_(alloc_traits::select_on_container_copy_construction(other.alloc_)), data_(other.samples())
{
    if (samples() > 0)
    {
        allocate();
        std::copy_n(other.data(), other.samples(), data());
    }
}

template<typename SampleType, typename Allocator>
basic_channel<SampleType, Allocator>::basic_channel(const basic_channel& other, const allocator_type& alloc) :
    alloc_(alloc), data_(other.samples())
{
    if (samples() > 0)
    {
        allocate();
        std::copy_n(other.data(), other.samples(), data());
    }
}

template<typename SampleType, typename Allocator>
basic_channel<SampleType, Allocator>::basic_channel(
    basic_channel&& other, const allocator_type& alloc, std::false_type) :
    alloc_(alloc)
{
    if (other.alloc_ == alloc)
    {
        data_.swap(other.data_);
    }
    else if (!other.empty())
    {
        allocate();
        std::copy_n(other.data(), other.samples(), data());
    }
}

template<typename SampleType, typename Allocator>
basic_channel<SampleType, Allocator>& basic_channel<SampleType, Allocator>::operator=(const basic_channel& other)
{
    if (this != &other)
    {
        copy_assign_alloc(other);
        basic_channel(other).swap(*this);
    }
    return *this;
}

template<typename SampleType, typename Allocator>
basic_channel<SampleType, Allocator>& basic_channel<SampleType, Allocator>::operator=(basic_channel&& other) noexcept(
    alloc_traits::propagate_on_container_move_assignment::value)
{
    move_assign(other, typename alloc_traits::propagate_on_container_move_assignment());
    return *this;
}

template<typename SampleType, typename Allocator>
void basic_channel<SampleType, Allocator>::move_assign(basic_channel& other, std::true_type) noexcept
{
    if (data() != nullptr)
    {
        deallocate();
    }
    move_assign_alloc(other);
    data_.move(other.data_);
}

template<typename SampleType, typename Allocator>
void basic_channel<SampleType, Allocator>::move_assign(basic_channel& other, std::false_type)
{
    if (alloc_ != other.alloc_)
    {
        if (data() != nullptr)
        {
            deallocate();
        }
        data_.swap(other.data_);
        if (samples() > 0)
        {
            allocate();
            std::copy_n(other.data(), other.samples(), data());
        }
    }
    else
    {
        move_assign(other, std::true_type());
    }
}

template<typename SampleType, typename Allocator>
void basic_channel<SampleType, Allocator>::swap(basic_channel& other)
{
    swap_alloc(other, typename alloc_traits::propagate_on_container_swap());
    data_.swap(other.data_);
}

template<typename SampleType, typename Allocator>
inline typename basic_channel<SampleType, Allocator>::reference basic_channel<SampleType, Allocator>::at(size_type n)
{
    if (n >= samples())
    {
        throw std::out_of_range("channel");
    }
    return (*this)[n];
}

template<typename SampleType, typename Allocator>
inline typename basic_channel<SampleType, Allocator>::const_reference basic_channel<SampleType, Allocator>::at(
    size_type n) const
{
    if (n >= samples())
    {
        throw std::out_of_range("channel");
    }
    return (*this)[n];
}

template<typename SampleType, typename AllocatorA, typename AllocatorB>
inline bool operator==(
    const basic_channel<SampleType, AllocatorA>& a, const basic_channel<SampleType, AllocatorB>& b) noexcept
{
    return (a.samples() == b.samples()) && std::equal(a.data(), a.data() + a.samples(), b.data());
}

template<typename SampleType, typename AllocatorA, typename AllocatorB>
inline bool operator!=(
    const basic_channel<SampleType, AllocatorA>& a, const basic_channel<SampleType, AllocatorB>& b) noexcept
{
    return !(a == b);
}

template<typename SampleValueType>
using channel = basic_channel<sample<SampleValueType>>;

template<typename SampleValueType>
using network_channel = basic_channel<network_sample<SampleValueType>>;

} // namespace ratl

#endif // _ratl_channel_
