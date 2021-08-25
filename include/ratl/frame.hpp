/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_frame_
#define _ratl_frame_

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
template<class SampleType, class Allocator = ratl::allocator<SampleType>>
class basic_frame
{
    using data_impl_type = sample_span<SampleType, true, detail::frame_iterator>;

    using allocator_type = Allocator;
    using alloc_traits = std::allocator_traits<allocator_type>;
    using sample_traits = typename data_impl_type::sample_traits;

public:
    using sample_type = typename sample_traits::sample_type;
    using const_sample_type = typename sample_traits::const_sample_type;
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
    allocator_type alloc_;
    data_impl_type data_;

public:
    static_assert(
        std::is_same<sample_type, typename allocator_type::value_type>::value,
        "allocator::value_type must be same type as sample");
    static_assert(
        std::is_same<sample_pointer, typename alloc_traits::pointer>::value,
        "allocator::pointer must be same type as sample*");

    basic_frame() noexcept(std::is_nothrow_default_constructible<allocator_type>::value) = default;

    explicit basic_frame(const allocator_type& alloc) noexcept : alloc_(alloc) {}

    explicit basic_frame(size_type channels);

    basic_frame(size_type channels, const allocator_type& alloc);

    basic_frame(const basic_frame& other);

    basic_frame(const basic_frame& other, const allocator_type& alloc);

    basic_frame(basic_frame&& other) noexcept = default;

#if defined(RATL_CPP_VERSION_HAS_CPP17)
    basic_frame(basic_frame&& other, const allocator_type& alloc) noexcept(alloc_traits::is_always_equal::value) :
        basic_frame(other, alloc, alloc_traits::is_always_equal)
    {
    }
#else
    basic_frame(basic_frame&& other, const allocator_type& alloc) : basic_frame(other, alloc, std::false_type{}) {}
#endif

    ~basic_frame()
    {
        if (data() != nullptr)
        {
            deallocate();
        }
    }

    basic_frame& operator=(const basic_frame& other);

    basic_frame& operator=(basic_frame&& other) noexcept(alloc_traits::propagate_on_container_move_assignment::value);

    allocator_type get_allocator() const noexcept
    {
        return alloc_;
    }

    void swap(basic_frame& other);

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

private:
    basic_frame(basic_frame&& other, const allocator_type& alloc, std::true_type) noexcept :
        alloc_(alloc), data_(std::move(other.data_))
    {
    }

    basic_frame(basic_frame&& other, const allocator_type& alloc, std::false_type);

    void allocate()
    {
        // we don't need to default construct the samples as sample types are trivially default constructable
        data_.start_ = alloc_.allocate(channels());
    }

    void deallocate()
    {
        alloc_.deallocate(data(), channels());
    }

    void copy_assign_alloc(const basic_frame& other)
    {
        copy_assign_alloc(other, typename alloc_traits::propagate_on_container_copy_assignment{});
    }

    void copy_assign_alloc(const basic_frame& other, std::true_type)
    {
        if (alloc_ != other.alloc_)
        {
            deallocate();
        }
        alloc_ = other.alloc_;
    }

    void copy_assign_alloc(const basic_frame&, std::false_type) {}

    void move_assign(basic_frame& other, std::true_type) noexcept;

    void move_assign(basic_frame& other, std::false_type);

    void move_assign_alloc(basic_frame& other) noexcept
    {
        move_assign_alloc(other, typename alloc_traits::propagate_on_container_move_assignment{});
    }

    void move_assign_alloc(basic_frame& other, std::true_type) noexcept
    {
        alloc_ = std::move(other.alloc_);
    }

    void move_assign_alloc(basic_frame&, std::false_type) noexcept {}

    void swap_alloc(basic_frame& other, std::true_type) noexcept
    {
        std::swap(alloc_, other.alloc_);
    }

    void swap_alloc(basic_frame&, std::false_type) noexcept {}
};

template<class SampleType, class Allocator>
basic_frame<SampleType, Allocator>::basic_frame(size_type channels) : data_(channels)
{
    if (channels() > 0)
    {
        allocate();
        std::fill_n(data(), channels(), sample_type{});
    }
}

template<class SampleType, class Allocator>
basic_frame<SampleType, Allocator>::basic_frame(size_type channels, const allocator_type& alloc) :
    alloc_(alloc), data_(channels)
{
    if (channels() > 0)
    {
        allocate();
        std::fill_n(data(), channels(), sample_type{});
    }
}

template<class SampleType, class Allocator>
basic_frame<SampleType, Allocator>::basic_frame(const basic_frame& other) :
    alloc_(alloc_traits::select_on_container_copy_construction(other.alloc_)), data_(other.channels())
{
    if (channels() > 0)
    {
        allocate();
        std::copy_n(other.data(), other.channels(), data());
    }
}

template<class SampleType, class Allocator>
basic_frame<SampleType, Allocator>::basic_frame(const basic_frame& other, const allocator_type& alloc) :
    alloc_(alloc), data_(other.channels())
{
    if (channels() > 0)
    {
        allocate();
        std::copy_n(other.data(), other.channels(), data());
    }
}

template<class SampleType, class Allocator>
basic_frame<SampleType, Allocator>::basic_frame(basic_frame&& other, const allocator_type& alloc, std::false_type) :
    alloc_(alloc)
{
    if (other.alloc_ == alloc)
    {
        data_.swap(other.data_);
    }
    else if (!other.empty())
    {
        allocate();
        std::copy_n(other.data(), other.channels(), data());
    }
}

template<class SampleType, class Allocator>
basic_frame<SampleType, Allocator>& basic_frame<SampleType, Allocator>::operator=(const basic_frame& other)
{
    if (this != &other)
    {
        copy_assign_alloc(other);
        basic_frame(other).swap(*this);
    }
    return *this;
}

template<class SampleType, class Allocator>
basic_frame<SampleType, Allocator>& basic_frame<SampleType, Allocator>::operator=(basic_frame&& other) noexcept(
    alloc_traits::propagate_on_container_move_assignment::value)
{
    move_assign(other, typename alloc_traits::propagate_on_container_move_assignment());
    return *this;
}

template<class SampleType, class Allocator>
void basic_frame<SampleType, Allocator>::move_assign(basic_frame& other, std::true_type) noexcept
{
    if (data() != nullptr)
    {
        deallocate();
    }
    move_assign_alloc(other);
    data_.move(other.data_);
}

template<class SampleType, class Allocator>
void basic_frame<SampleType, Allocator>::move_assign(basic_frame& other, std::false_type)
{
    if (alloc_ != other.alloc_)
    {
        if (data() != nullptr)
        {
            deallocate();
        }
        data_.swap(other.data_);
        if (channels() > 0)
        {
            allocate();
            std::copy_n(other.data(), other.channels(), data());
        }
    }
    else
    {
        move_assign(other, std::true_type());
    }
}

template<class SampleType, class Allocator>
void basic_frame<SampleType, Allocator>::swap(basic_frame& other)
{
    swap_alloc(other, typename alloc_traits::propagate_on_container_swap());
    data_.swap(other.data_);
}

template<class SampleType, class Allocator>
inline typename basic_frame<SampleType, Allocator>::reference basic_frame<SampleType, Allocator>::at(size_type n)
{
    if (n >= channels())
    {
        throw std::out_of_range("frame");
    }
    return (*this)[n];
}

template<class SampleType, class Allocator>
inline typename basic_frame<SampleType, Allocator>::const_reference basic_frame<SampleType, Allocator>::at(
    size_type n) const
{
    if (n >= channels())
    {
        throw std::out_of_range("frame");
    }
    return (*this)[n];
}

template<class SampleValueType>
using frame = basic_frame<sample<SampleValueType>>;

template<class SampleValueType>
using network_frame = basic_frame<network_sample<SampleValueType>>;

} // namespace ratl

#endif // _ratl_frame_
