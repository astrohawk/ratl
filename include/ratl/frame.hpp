#ifndef _ratl_frame_
#define _ratl_frame_

// C++ Standard Library includes
#include <iterator>
#include <memory>
#include <type_traits>

// ratl includes
#include <ratl/allocator.hpp>
#include <ratl/detail/channel_iterator.hpp>
#include <ratl/detail/sample_span.hpp>
#include <ratl/network_sample.hpp>
#include <ratl/sample.hpp>

namespace ratl
{
template<class Sample, class Allocator = ratl::Allocator<Sample>>
class BasicFrame
{
    using data_impl_type = SampleSpan<Sample, true, detail::FrameIterator>;

    using allocator_type = Allocator;
    using alloc_traits = std::allocator_traits<allocator_type>;

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
    allocator_type alloc_;
    data_impl_type data_;

public:
    static_assert(
        std::is_same<sample, typename allocator_type::value_type>::value,
        "Allocator::value_type must be same type as Sample");
    static_assert(
        std::is_same<sample_pointer, typename alloc_traits::pointer>::value,
        "Allocator::pointer must be same type as Sample*");

    BasicFrame() noexcept(std::is_nothrow_default_constructible<allocator_type>::value) = default;

    explicit BasicFrame(const allocator_type& alloc) noexcept : alloc_(alloc) {}

    explicit BasicFrame(size_type channels);

    BasicFrame(size_type channels, const allocator_type& alloc);

    BasicFrame(const BasicFrame& other);

    BasicFrame(const BasicFrame& other, const allocator_type& alloc);

    BasicFrame(BasicFrame&& other) noexcept = default;

#if defined(RATL_CPP_VERSION_HAS_CPP17)
    BasicFrame(BasicFrame&& other, const allocator_type& alloc) noexcept(alloc_traits::is_always_equal::value) :
        BasicFrame(other, alloc, alloc_traits::is_always_equal)
    {
    }
#else
    BasicFrame(BasicFrame&& other, const allocator_type& alloc) : BasicFrame(other, alloc, std::false_type{}) {}
#endif

    ~BasicFrame()
    {
        if (data() != nullptr)
        {
            deallocate();
        }
    }

    BasicFrame& operator=(const BasicFrame& other);

    BasicFrame& operator=(BasicFrame&& other) noexcept(alloc_traits::propagate_on_container_move_assignment::value);

    allocator_type get_allocator() const noexcept
    {
        return alloc_;
    }

    void swap(BasicFrame& other);

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
    BasicFrame(BasicFrame&& other, const allocator_type& alloc, std::true_type) noexcept :
        alloc_(alloc), data_(std::move(other.data_))
    {
    }

    BasicFrame(BasicFrame&& other, const allocator_type& alloc, std::false_type);

    void allocate()
    {
        // we don't need to default construct the samples as sample types are trivially default constructable
        data_.start_ = alloc_.allocate(channels());
    }

    void deallocate()
    {
        alloc_.deallocate(data(), channels());
    }

    void copy_assign_alloc(const BasicFrame& other)
    {
        copy_assign_alloc(other, typename alloc_traits::propagate_on_container_copy_assignment{});
    }

    void copy_assign_alloc(const BasicFrame& other, std::true_type)
    {
        if (alloc_ != other.alloc_)
        {
            deallocate();
        }
        alloc_ = other.alloc_;
    }

    void copy_assign_alloc(const BasicFrame&, std::false_type) {}

    void move_assign(BasicFrame& other, std::true_type) noexcept;

    void move_assign(BasicFrame& other, std::false_type);

    void move_assign_alloc(BasicFrame& other) noexcept
    {
        move_assign_alloc(other, typename alloc_traits::propagate_on_container_move_assignment{});
    }

    void move_assign_alloc(BasicFrame& other, std::true_type) noexcept
    {
        alloc_ = std::move(other.alloc_);
    }

    void move_assign_alloc(BasicFrame&, std::false_type) noexcept {}

    void swap_alloc(BasicFrame& other, std::true_type) noexcept
    {
        std::swap(alloc_, other.alloc_);
    }

    void swap_alloc(BasicFrame&, std::false_type) noexcept {}
};

template<class Sample, class Allocator>
BasicFrame<Sample, Allocator>::BasicFrame(size_type channels) : data_(channels)
{
    if (channels() > 0)
    {
        allocate();
        std::fill_n(data(), channels(), sample{});
    }
}

template<class Sample, class Allocator>
BasicFrame<Sample, Allocator>::BasicFrame(size_type channels, const allocator_type& alloc) :
    alloc_(alloc), data_(channels)
{
    if (channels() > 0)
    {
        allocate();
        std::fill_n(data(), channels(), sample{});
    }
}

template<class Sample, class Allocator>
BasicFrame<Sample, Allocator>::BasicFrame(const BasicFrame& other) :
    alloc_(alloc_traits::select_on_container_copy_construction(other.alloc_)), data_(other.channels())
{
    if (channels() > 0)
    {
        allocate();
        std::copy_n(other.data(), other.channels(), data());
    }
}

template<class Sample, class Allocator>
BasicFrame<Sample, Allocator>::BasicFrame(const BasicFrame& other, const allocator_type& alloc) :
    alloc_(alloc), data_(other.channels())
{
    if (channels() > 0)
    {
        allocate();
        std::copy_n(other.data(), other.channels(), data());
    }
}

template<class Sample, class Allocator>
BasicFrame<Sample, Allocator>::BasicFrame(BasicFrame&& other, const allocator_type& alloc, std::false_type) :
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

template<class Sample, class Allocator>
BasicFrame<Sample, Allocator>& BasicFrame<Sample, Allocator>::operator=(const BasicFrame& other)
{
    if (this != &other)
    {
        copy_assign_alloc(other);
        BasicFrame(other).swap(*this);
    }
    return *this;
}

template<class Sample, class Allocator>
BasicFrame<Sample, Allocator>& BasicFrame<Sample, Allocator>::operator=(BasicFrame&& other) noexcept(
    alloc_traits::propagate_on_container_move_assignment::value)
{
    move_assign(other, typename alloc_traits::propagate_on_container_move_assignment());
    return *this;
}

template<class Sample, class Allocator>
void BasicFrame<Sample, Allocator>::move_assign(BasicFrame& other, std::true_type) noexcept
{
    if (data() != nullptr)
    {
        deallocate();
    }
    move_assign_alloc(other);
    data_.move(other.data_);
}

template<class Sample, class Allocator>
void BasicFrame<Sample, Allocator>::move_assign(BasicFrame& other, std::false_type)
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

template<class Sample, class Allocator>
void BasicFrame<Sample, Allocator>::swap(BasicFrame& other)
{
    swap_alloc(other, typename alloc_traits::propagate_on_container_swap());
    data_.swap(other.data_);
}

template<class Sample, class Allocator>
inline typename BasicFrame<Sample, Allocator>::reference BasicFrame<Sample, Allocator>::at(size_type n)
{
    if (n >= channels())
    {
        throw std::out_of_range("Frame");
    }
    return (*this)[n];
}

template<class Sample, class Allocator>
inline typename BasicFrame<Sample, Allocator>::const_reference BasicFrame<Sample, Allocator>::at(size_type n) const
{
    if (n >= channels())
    {
        throw std::out_of_range("Frame");
    }
    return (*this)[n];
}

template<class SampleType>
using Frame = BasicFrame<Sample<SampleType>>;

template<class SampleType>
using NetworkFrame = BasicFrame<NetworkSample<SampleType>>;

} // namespace ratl

#endif // _ratl_frame_
