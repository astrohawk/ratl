/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_interleaved_
#define _ratl_interleaved_

// ratl includes
#include <ratl/allocator.hpp>
#include <ratl/channel_span.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/detail/interleaved_iterator.hpp>
#include <ratl/detail/operator_arrow_proxy.hpp>
#include <ratl/detail/sample_traits.hpp>
#include <ratl/frame_span.hpp>
#include <ratl/network_sample.hpp>
#include <ratl/sample.hpp>

// other includes
#include <iterator>
#include <memory>
#include <type_traits>

namespace ratl
{
template<class SampleType, class Allocator = ratl::allocator<SampleType>>
class basic_interleaved
{
    using allocator_type = Allocator;
    using alloc_traits = std::allocator_traits<allocator_type>;
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
    struct data_impl
    {
        sample_pointer start_;
        size_type channels_;
        size_type frames_;

        data_impl() noexcept : start_(), channels_(), frames_() {}

        data_impl(size_type channels, size_type frames) : start_(), channels_(channels), frames_(frames) {}

        data_impl(const data_impl& other) noexcept = default;

        data_impl(data_impl&& other) noexcept : start_(other.start_), channels_(other.channels_), frames_(other.frames_)
        {
            other.start_ = nullptr;
            other.channels_ = 0;
            other.frames_ = 0;
        }

        void copy(const data_impl& other) noexcept
        {
            start_ = other.start_;
            channels_ = other.channels_;
            frames_ = other.frames_;
        }

        void move(data_impl& other) noexcept
        {
            data_impl tmp(std::move(other));
            swap(tmp);
        }

        void swap(data_impl& other) noexcept
        {
            std::swap(start_, other.start_);
            std::swap(channels_, other.channels_);
            std::swap(frames_, other.frames_);
        }
    };

    allocator_type alloc_;
    data_impl data_;

public:
    static_assert(
        std::is_same<sample_type, typename allocator_type::value_type>::value,
        "allocator::value_type must be same type as sample");
    static_assert(
        std::is_same<sample_pointer, typename alloc_traits::pointer>::value,
        "allocator::pointer must be same type as sample*");

    basic_interleaved() noexcept(std::is_nothrow_default_constructible<allocator_type>::value) = default;

    explicit basic_interleaved(const allocator_type& alloc) noexcept : alloc_(alloc) {}

    basic_interleaved(size_type channels, size_type frames);

    basic_interleaved(size_type channels, size_type frames, const allocator_type& alloc);

    basic_interleaved(const basic_interleaved& other);

    basic_interleaved(const basic_interleaved& other, const allocator_type& alloc);

    basic_interleaved(basic_interleaved&& other) noexcept = default;

#if defined(RATL_CPP_VERSION_HAS_CPP17)
    basic_interleaved(basic_interleaved&& other, const allocator_type& alloc) noexcept(
        alloc_traits::is_always_equal::value) :
        basic_interleaved(other, alloc, typename alloc_traits::is_always_equal())
    {
    }
#else
    basic_interleaved(basic_interleaved&& other, const allocator_type& alloc) :
        basic_interleaved(other, alloc, std::false_type())
    {
    }
#endif

    ~basic_interleaved()
    {
        if (data() != nullptr)
        {
            deallocate();
        }
    }

    basic_interleaved& operator=(const basic_interleaved& other);

    basic_interleaved& operator=(basic_interleaved&& other) noexcept(
        alloc_traits::propagate_on_container_move_assignment::value);

    allocator_type get_allocator() const noexcept
    {
        return alloc_;
    }

    void swap(basic_interleaved& other);

    inline sample_pointer data() noexcept
    {
        return data_.start_;
    }

    inline const sample_pointer data() const noexcept
    {
        return data_.start_;
    }

    inline size_type channels() const noexcept
    {
        return data_.channels_;
    }

    inline size_type frames() const noexcept
    {
        return data_.frames_;
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

private:
    basic_interleaved(basic_interleaved&& other, const allocator_type& alloc, std::true_type) noexcept :
        alloc_(alloc), data_(std::move(other.data_))
    {
    }

    basic_interleaved(basic_interleaved&& other, const allocator_type& alloc, std::false_type);

    void allocate()
    {
        // we don't need to default construct the samples as sample types are trivially default constructable
        data_.start_ = alloc_.allocate(samples());
    }

    void deallocate()
    {
        alloc_.deallocate(data(), samples());
    }

    void copy_assign_alloc(const basic_interleaved& other)
    {
        copy_assign_alloc(other, typename alloc_traits::propagate_on_container_copy_assignment());
    }

    void copy_assign_alloc(const basic_interleaved& other, std::true_type)
    {
        if (alloc_ != other.alloc_)
        {
            deallocate();
        }
        alloc_ = other.alloc_;
    }

    void copy_assign_alloc(const basic_interleaved&, std::false_type) {}

    void move_assign(basic_interleaved& other, std::true_type) noexcept;

    void move_assign(basic_interleaved& other, std::false_type);

    void move_assign_alloc(basic_interleaved& other) noexcept
    {
        move_assign_alloc(other, typename alloc_traits::propagate_on_container_move_assignment());
    }

    void move_assign_alloc(basic_interleaved& other, std::true_type) noexcept
    {
        alloc_ = std::move(other.alloc_);
    }

    void move_assign_alloc(basic_interleaved&, std::false_type) noexcept {}

    void swap_alloc(basic_interleaved& other, std::true_type) noexcept
    {
        std::swap(alloc_, other.alloc_);
    }

    void swap_alloc(basic_interleaved&, std::false_type) noexcept {}
};

template<class SampleType, class Allocator>
basic_interleaved<SampleType, Allocator>::basic_interleaved(size_type channels, size_type frames) :
    data_(channels, frames)
{
    if (!empty())
    {
        allocate();
        std::fill_n(data(), samples(), sample_type());
    }
}

template<class SampleType, class Allocator>
basic_interleaved<SampleType, Allocator>::basic_interleaved(
    size_type channels, size_type frames, const allocator_type& alloc) :
    alloc_(alloc), data_(channels, frames)
{
    if (!empty())
    {
        allocate();
        std::fill_n(data(), samples(), sample_type());
    }
}

template<class SampleType, class Allocator>
basic_interleaved<SampleType, Allocator>::basic_interleaved(const basic_interleaved& other) :
    alloc_(alloc_traits::select_on_container_copy_construction(other.alloc_)), data_(other.channels(), other.frames())
{
    if (!empty())
    {
        allocate();
        std::copy_n(other.data(), other.samples(), data());
    }
}

template<class SampleType, class Allocator>
basic_interleaved<SampleType, Allocator>::basic_interleaved(
    const basic_interleaved& other, const allocator_type& alloc) :
    alloc_(alloc), data_(other.channels(), other.frames())
{
    if (!empty())
    {
        allocate();
        std::copy_n(other.data(), other.samples(), data());
    }
}

template<class SampleType, class Allocator>
basic_interleaved<SampleType, Allocator>::basic_interleaved(
    basic_interleaved&& other, const allocator_type& alloc, std::false_type) :
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

template<class SampleType, class Allocator>
basic_interleaved<SampleType, Allocator>& basic_interleaved<SampleType, Allocator>::operator=(
    const basic_interleaved& other)
{
    if (this != &other)
    {
        copy_assign_alloc(other);
        basic_interleaved(other).swap(*this);
    }
    return *this;
}

template<class SampleType, class Allocator>
basic_interleaved<SampleType, Allocator>& basic_interleaved<SampleType, Allocator>::operator=(
    basic_interleaved&& other) noexcept(alloc_traits::propagate_on_container_move_assignment::value)
{
    move_assign(other, typename alloc_traits::propagate_on_container_move_assignment());
    return *this;
}

template<class SampleType, class Allocator>
void basic_interleaved<SampleType, Allocator>::move_assign(basic_interleaved& other, std::true_type) noexcept
{
    if (data() != nullptr)
    {
        deallocate();
    }
    move_assign_alloc(other);
    data_.move(other.data_);
}

template<class SampleType, class Allocator>
void basic_interleaved<SampleType, Allocator>::move_assign(basic_interleaved& other, std::false_type)
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

template<class SampleType, class Allocator>
void basic_interleaved<SampleType, Allocator>::swap(basic_interleaved& other)
{
    swap_alloc(other, typename alloc_traits::propagate_on_container_swap());
    data_.swap(other.data_);
}

template<class SampleType, class Allocator>
inline typename basic_interleaved<SampleType, Allocator>::reference basic_interleaved<SampleType, Allocator>::at(
    size_type n)
{
    if (n >= frames())
    {
        throw std::out_of_range("interleaved");
    }
    return (*this)[n];
}

template<class SampleType, class Allocator>
inline typename basic_interleaved<SampleType, Allocator>::const_reference basic_interleaved<SampleType, Allocator>::at(
    size_type n) const
{
    if (n >= frames())
    {
        throw std::out_of_range("interleaved");
    }
    return (*this)[n];
}

template<class SampleType, class Allocator>
inline bool operator==(
    const basic_interleaved<SampleType, Allocator>& a, const basic_interleaved<SampleType, Allocator>& b) noexcept
{
    return (a.channels() == b.channels()) && (a.frames() == b.frames()) &&
           std::equal(a.data(), a.data() + a.samples(), b.data());
}

template<class SampleType, class Allocator>
inline bool operator!=(
    const basic_interleaved<SampleType, Allocator>& a, const basic_interleaved<SampleType, Allocator>& b) noexcept
{
    return !(a == b);
}

template<class SampleValueType>
using interleaved = basic_interleaved<sample<SampleValueType>>;

template<class SampleValueType>
using network_interleaved = basic_interleaved<network_sample<SampleValueType>>;

} // namespace ratl

#endif // _ratl_interleaved_
