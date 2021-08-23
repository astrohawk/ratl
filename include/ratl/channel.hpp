#ifndef _ratl_channel_
#define _ratl_channel_

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
class BasicChannel
{
    using data_impl_type = SampleSpan<Sample, true, detail::ChannelIterator>;

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

    BasicChannel() noexcept(std::is_nothrow_default_constructible<allocator_type>::value) = default;

    explicit BasicChannel(const allocator_type& alloc) noexcept : alloc_(alloc) {}

    explicit BasicChannel(size_type samples);

    BasicChannel(size_type samples, const allocator_type& alloc);

    BasicChannel(const BasicChannel& other);

    BasicChannel(const BasicChannel& other, const allocator_type& alloc);

    BasicChannel(BasicChannel&& other) noexcept = default;

#if defined(RATL_CPP_VERSION_HAS_CPP17)
    BasicChannel(BasicChannel&& other, const allocator_type& alloc) noexcept(alloc_traits::is_always_equal::value) :
        BasicChannel(other, alloc, alloc_traits::is_always_equal)
    {
    }
#else
    BasicChannel(BasicChannel&& other, const allocator_type& alloc) : BasicChannel(other, alloc, std::false_type{}) {}
#endif

    ~BasicChannel()
    {
        if (data() != nullptr)
        {
            deallocate();
        }
    }

    BasicChannel& operator=(const BasicChannel& other);

    BasicChannel& operator=(BasicChannel&& other) noexcept(alloc_traits::propagate_on_container_move_assignment::value);

    allocator_type get_allocator() const noexcept
    {
        return alloc_;
    }

    void swap(BasicChannel& other);

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
    BasicChannel(BasicChannel&& other, const allocator_type& alloc, std::true_type) noexcept :
        alloc_(alloc), data_(std::move(other.data_))
    {
    }

    BasicChannel(BasicChannel&& other, const allocator_type& alloc, std::false_type);

    void allocate()
    {
        // we don't need to default construct the samples as sample types are trivially default constructable
        data_.start_ = alloc_.allocate(samples());
    }

    void deallocate()
    {
        alloc_.deallocate(data(), samples());
    }

    void copy_assign_alloc(const BasicChannel& other)
    {
        copy_assign_alloc(other, typename alloc_traits::propagate_on_container_copy_assignment{});
    }

    void copy_assign_alloc(const BasicChannel& other, std::true_type)
    {
        if (alloc_ != other.alloc_)
        {
            deallocate();
        }
        alloc_ = other.alloc_;
    }

    void copy_assign_alloc(const BasicChannel&, std::false_type) {}

    void move_assign(BasicChannel& other, std::true_type) noexcept;

    void move_assign(BasicChannel& other, std::false_type);

    void move_assign_alloc(BasicChannel& other) noexcept
    {
        move_assign_alloc(other, typename alloc_traits::propagate_on_container_move_assignment{});
    }

    void move_assign_alloc(BasicChannel& other, std::true_type) noexcept
    {
        alloc_ = std::move(other.alloc_);
    }

    void move_assign_alloc(BasicChannel&, std::false_type) noexcept {}

    void swap_alloc(BasicChannel& other, std::true_type) noexcept
    {
        std::swap(alloc_, other.alloc_);
    }

    void swap_alloc(BasicChannel&, std::false_type) noexcept {}
};

template<class Sample, class Allocator>
BasicChannel<Sample, Allocator>::BasicChannel(size_type samples) : data_(samples)
{
    if (samples() > 0)
    {
        allocate();
        std::fill_n(data(), samples(), sample{});
    }
}

template<class Sample, class Allocator>
BasicChannel<Sample, Allocator>::BasicChannel(size_type samples, const allocator_type& alloc) :
    alloc_(alloc), data_(samples)
{
    if (samples() > 0)
    {
        allocate();
        std::fill_n(data(), samples(), sample{});
    }
}

template<class Sample, class Allocator>
BasicChannel<Sample, Allocator>::BasicChannel(const BasicChannel& other) :
    alloc_(alloc_traits::select_on_container_copy_construction(other.alloc_)), data_(other.samples())
{
    if (samples() > 0)
    {
        allocate();
        std::copy_n(other.data(), other.samples(), data());
    }
}

template<class Sample, class Allocator>
BasicChannel<Sample, Allocator>::BasicChannel(const BasicChannel& other, const allocator_type& alloc) :
    alloc_(alloc), data_(other.samples())
{
    if (samples() > 0)
    {
        allocate();
        std::copy_n(other.data(), other.samples(), data());
    }
}

template<class Sample, class Allocator>
BasicChannel<Sample, Allocator>::BasicChannel(BasicChannel&& other, const allocator_type& alloc, std::false_type) :
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
BasicChannel<Sample, Allocator>& BasicChannel<Sample, Allocator>::operator=(const BasicChannel& other)
{
    if (this != &other)
    {
        copy_assign_alloc(other);
        BasicChannel(other).swap(*this);
    }
    return *this;
}

template<class Sample, class Allocator>
BasicChannel<Sample, Allocator>& BasicChannel<Sample, Allocator>::operator=(BasicChannel&& other) noexcept(
    alloc_traits::propagate_on_container_move_assignment::value)
{
    move_assign(other, typename alloc_traits::propagate_on_container_move_assignment());
    return *this;
}

template<class Sample, class Allocator>
void BasicChannel<Sample, Allocator>::move_assign(BasicChannel& other, std::true_type) noexcept
{
    if (data() != nullptr)
    {
        deallocate();
    }
    move_assign_alloc(other);
    data_.move(other.data_);
}

template<class Sample, class Allocator>
void BasicChannel<Sample, Allocator>::move_assign(BasicChannel& other, std::false_type)
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
void BasicChannel<Sample, Allocator>::swap(BasicChannel& other)
{
    swap_alloc(other, typename alloc_traits::propagate_on_container_swap());
    data_.swap(other.data_);
}

template<class Sample, class Allocator>
inline typename BasicChannel<Sample, Allocator>::reference BasicChannel<Sample, Allocator>::at(size_type n)
{
    if (n >= samples())
    {
        throw std::out_of_range("Channel");
    }
    return (*this)[n];
}

template<class Sample, class Allocator>
inline typename BasicChannel<Sample, Allocator>::const_reference BasicChannel<Sample, Allocator>::at(size_type n) const
{
    if (n >= samples())
    {
        throw std::out_of_range("Channel");
    }
    return (*this)[n];
}

template<class SampleType>
using Channel = BasicChannel<Sample<SampleType>>;

template<class SampleType>
using NetworkChannel = BasicChannel<NetworkSample<SampleType>>;

} // namespace ratl

#endif // _ratl_channel_
