#ifndef _ratl_interleaved_
#define _ratl_interleaved_

// C++ Standard Library includes
#include <iterator>
#include <memory>
#include <type_traits>

// ratl includes
#include <ratl/allocator.hpp>
#include <ratl/channel_span.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/detail/interleaved_iterator.hpp>
#include <ratl/detail/operator_arrow_proxy.hpp>
#include <ratl/detail/types.hpp>
#include <ratl/frame_span.hpp>
#include <ratl/network_sample.hpp>
#include <ratl/sample.hpp>

namespace ratl
{
template<class Sample, class Allocator = ratl::Allocator<Sample>>
class BasicInterleaved
{
private:
    using allocator_type = Allocator;
    using alloc_traits = std::allocator_traits<allocator_type>;

public:
    using sample_traits = detail::SampleTraits<Sample>;
    using sample = typename sample_traits::sample;
    using const_sample = typename sample_traits::const_sample;
    using sample_pointer = typename sample_traits::pointer;
    using const_sample_pointer = typename sample_traits::const_pointer;
    using sample_reference = typename sample_traits::reference;
    using const_sample_reference = typename sample_traits::const_reference;

    using channel_type = BasicChannelSpan<sample, false>;
    using const_channel_type = BasicChannelSpan<const_sample, false>;
    using frame_type = BasicFrameSpan<sample, true>;
    using const_frame_type = BasicFrameSpan<const_sample, true>;

    using size_type = detail::types::size_type;
    using difference_type = detail::types::difference_type;

    using value_type = frame_type;
    using pointer = detail::operator_arrow_proxy<value_type>;
    using const_pointer = const detail::operator_arrow_proxy<value_type>;
    using reference = frame_type;
    using const_reference = const_frame_type;

    using iterator = detail::InterleavedIterator<sample>;
    using const_iterator = detail::InterleavedIterator<const_sample>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    struct DataImpl
    {
        sample_pointer start_;
        size_type channels_;
        size_type frames_;

        DataImpl() noexcept : start_(), channels_(), frames_() {}

        DataImpl(size_type channels, size_type frames) : start_(), channels_(channels), frames_(frames) {}

        DataImpl(const DataImpl& other) noexcept = default;

        DataImpl(DataImpl&& other) noexcept : start_(other.start_), channels_(other.channels_), frames_(other.frames_)
        {
            other.start_ = nullptr;
            other.channels_ = 0;
            other.frames_ = 0;
        }

        void copy(const DataImpl& other) noexcept
        {
            start_ = other.start_;
            channels_ = other.channels_;
            frames_ = other.frames_;
        }

        void move(DataImpl& other) noexcept
        {
            DataImpl tmp(std::move(other));
            swap(tmp);
        }

        void swap(DataImpl& other) noexcept
        {
            std::swap(start_, other.start_);
            std::swap(channels_, other.channels_);
            std::swap(frames_, other.frames_);
        }
    };

    allocator_type alloc_;
    DataImpl data_;

public:
    static_assert(
        std::is_same<sample, typename allocator_type::value_type>::value,
        "Allocator::value_type must be same type as Sample");
    static_assert(
        std::is_same<sample_pointer, typename alloc_traits::pointer>::value,
        "Allocator::pointer must be same type as Sample*");

    BasicInterleaved() noexcept(std::is_nothrow_default_constructible<allocator_type>::value) = default;

    explicit BasicInterleaved(const allocator_type& alloc) noexcept : alloc_(alloc) {}

    BasicInterleaved(size_type channels, size_type frames);

    BasicInterleaved(size_type channels, size_type frames, const allocator_type& alloc);

    BasicInterleaved(const BasicInterleaved& other);

    BasicInterleaved(const BasicInterleaved& other, const allocator_type& alloc);

    BasicInterleaved(BasicInterleaved&& other) noexcept = default;

#if defined(RATL_CPP_VERSION_HAS_CPP17)
    BasicInterleaved(BasicInterleaved&& other, const allocator_type& alloc) noexcept(
        alloc_traits::is_always_equal::value) :
        BasicInterleaved(other, alloc, typename alloc_traits::is_always_equal{})
    {
    }
#else
    BasicInterleaved(BasicInterleaved&& other, const allocator_type& alloc) :
        BasicInterleaved(other, alloc, std::false_type{})
    {
    }
#endif

    ~BasicInterleaved()
    {
        if (data() != nullptr)
        {
            deallocate();
        }
    }

    BasicInterleaved& operator=(const BasicInterleaved& other);

    BasicInterleaved& operator=(BasicInterleaved&& other) noexcept(
        alloc_traits::propagate_on_container_move_assignment::value);

    allocator_type get_allocator() const noexcept
    {
        return alloc_;
    }

    void swap(BasicInterleaved& other);

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
    BasicInterleaved(BasicInterleaved&& other, const allocator_type& alloc, std::true_type) noexcept :
        alloc_(alloc), data_(std::move(other.data_))
    {
    }

    BasicInterleaved(BasicInterleaved&& other, const allocator_type& alloc, std::false_type);

    void allocate()
    {
        // we don't need to default construct the samples as sample types are trivially default constructable
        data_.start_ = alloc_.allocate(samples());
    }

    void deallocate()
    {
        alloc_.deallocate(data(), samples());
    }

    void copy_assign_alloc(const BasicInterleaved& other)
    {
        copy_assign_alloc(other, typename alloc_traits::propagate_on_container_copy_assignment{});
    }

    void copy_assign_alloc(const BasicInterleaved& other, std::true_type)
    {
        if (alloc_ != other.alloc_)
        {
            deallocate();
        }
        alloc_ = other.alloc_;
    }

    void copy_assign_alloc(const BasicInterleaved&, std::false_type) {}

    void move_assign(BasicInterleaved& other, std::true_type) noexcept;

    void move_assign(BasicInterleaved& other, std::false_type);

    void move_assign_alloc(BasicInterleaved& other) noexcept
    {
        move_assign_alloc(other, typename alloc_traits::propagate_on_container_move_assignment{});
    }

    void move_assign_alloc(BasicInterleaved& other, std::true_type) noexcept
    {
        alloc_ = std::move(other.alloc_);
    }

    void move_assign_alloc(BasicInterleaved&, std::false_type) noexcept {}

    void swap_alloc(BasicInterleaved& other, std::true_type) noexcept
    {
        std::swap(alloc_, other.alloc_);
    }

    void swap_alloc(BasicInterleaved&, std::false_type) noexcept {}
};

template<class Sample, class Allocator>
BasicInterleaved<Sample, Allocator>::BasicInterleaved(size_type channels, size_type frames) : data_(channels, frames)
{
    if (!empty())
    {
        allocate();
        std::fill_n(data(), samples(), sample{});
    }
}

template<class Sample, class Allocator>
BasicInterleaved<Sample, Allocator>::BasicInterleaved(
    size_type channels, size_type frames, const allocator_type& alloc) :
    alloc_(alloc), data_(channels, frames)
{
    if (!empty())
    {
        allocate();
        std::fill_n(data(), samples(), sample{});
    }
}

template<class Sample, class Allocator>
BasicInterleaved<Sample, Allocator>::BasicInterleaved(const BasicInterleaved& other) :
    alloc_(alloc_traits::select_on_container_copy_construction(other.alloc_)), data_(other.channels(), other.frames())
{
    if (!empty())
    {
        allocate();
        std::copy_n(other.data(), other.samples(), data());
    }
}

template<class Sample, class Allocator>
BasicInterleaved<Sample, Allocator>::BasicInterleaved(const BasicInterleaved& other, const allocator_type& alloc) :
    alloc_(alloc), data_(other.channels(), other.frames())
{
    if (!empty())
    {
        allocate();
        std::copy_n(other.data(), other.samples(), data());
    }
}

template<class Sample, class Allocator>
BasicInterleaved<Sample, Allocator>::BasicInterleaved(
    BasicInterleaved&& other, const allocator_type& alloc, std::false_type) :
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

template<class Sample, class Allocator>
BasicInterleaved<Sample, Allocator>& BasicInterleaved<Sample, Allocator>::operator=(const BasicInterleaved& other)
{
    if (this != &other)
    {
        copy_assign_alloc(other);
        BasicInterleaved(other).swap(*this);
    }
    return *this;
}

template<class Sample, class Allocator>
BasicInterleaved<Sample, Allocator>& BasicInterleaved<Sample, Allocator>::operator=(BasicInterleaved&& other) noexcept(
    alloc_traits::propagate_on_container_move_assignment::value)
{
    move_assign(other, typename alloc_traits::propagate_on_container_move_assignment{});
    return *this;
}

template<class Sample, class Allocator>
void BasicInterleaved<Sample, Allocator>::move_assign(BasicInterleaved& other, std::true_type) noexcept
{
    if (data() != nullptr)
    {
        deallocate();
    }
    move_assign_alloc(other);
    data_.move(other.data_);
}

template<class Sample, class Allocator>
void BasicInterleaved<Sample, Allocator>::move_assign(BasicInterleaved& other, std::false_type)
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

template<class Sample, class Allocator>
void BasicInterleaved<Sample, Allocator>::swap(BasicInterleaved& other)
{
    swap_alloc(other, typename alloc_traits::propagate_on_container_swap{});
    data_.swap(other.data_);
}

template<class Sample, class Allocator>
inline typename BasicInterleaved<Sample, Allocator>::reference BasicInterleaved<Sample, Allocator>::at(size_type n)
{
    if (n >= frames())
    {
        throw std::out_of_range("Interleaved");
    }
    return (*this)[n];
}

template<class Sample, class Allocator>
inline typename BasicInterleaved<Sample, Allocator>::const_reference BasicInterleaved<Sample, Allocator>::at(
    size_type n) const
{
    if (n >= frames())
    {
        throw std::out_of_range("Interleaved");
    }
    return (*this)[n];
}

template<class Sample, class Allocator>
inline bool operator==(
    const BasicInterleaved<Sample, Allocator>& a, const BasicInterleaved<Sample, Allocator>& b) noexcept
{
    return (a.channels() == b.channels()) && (a.frames() == b.frames()) &&
           std::equal(a.data(), a.data() + a.samples(), b.data());
}

template<class Sample, class Allocator>
inline bool operator!=(
    const BasicInterleaved<Sample, Allocator>& a, const BasicInterleaved<Sample, Allocator>& b) noexcept
{
    return !(a == b);
}

template<class SampleType>
using Interleaved = BasicInterleaved<Sample<SampleType>>;

template<class SampleType>
using NetworkInterleaved = BasicInterleaved<NetworkSample<SampleType>>;

} // namespace ratl

#endif // _ratl_interleaved_
