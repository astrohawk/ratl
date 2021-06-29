#ifndef _ratl_channel_
#define _ratl_channel_

// C++ Standard Library includes
#include <iterator>
#include <memory>
#include <type_traits>

// ratl includes
#include <ratl/allocator.hpp>
#include <ratl/channel_span.hpp>
#include <ratl/detail/config.hpp>

namespace ratl
{
template<class Sample, class Allocator = ratl::Allocator<Sample>>
class BasicChannel : public BasicChannelSpan<Sample, true>
{
private:
    using super_type = BasicChannelSpan<Sample>;

    using allocator_type = Allocator;
    using alloc_traits = std::allocator_traits<allocator_type>;

public:
    using sample_traits = typename super_type::sample_traits;
    using sample = typename sample_traits::sample;
    using const_sample = typename sample_traits::const_sample;
    using sample_pointer = typename super_type::sample_pointer;
    using sample_const_pointer = typename super_type::sample_const_pointer;
    using sample_reference = typename super_type::sample_reference;
    using sample_const_reference = typename super_type::sample_const_reference;

    using size_type = typename super_type::size_type;
    using difference_type = typename super_type::difference_type;

    using value_type = typename super_type::value_type;
    using reference = typename super_type::reference;
    using const_reference = typename super_type::const_reference;
    using pointer = typename super_type::pointer;
    using const_pointer = typename super_type::const_pointer;

    using iterator = typename super_type::iterator;
    using const_iterator = typename super_type::const_iterator;
    using reverse_iterator = typename super_type::reverse_iterator;
    using const_reverse_iterator = typename super_type::const_reverse_iterator;

private:
    allocator_type alloc_;

public:
    static_assert(
        std::is_same<sample, typename allocator_type::value_type>::value,
        "Allocator::value_type must be same type as Sample");
    static_assert(
        std::is_same<sample_pointer, typename alloc_traits::pointer>::value,
        "Allocator::pointer must be same type as Sample*");

    BasicChannel() noexcept(std::is_nothrow_default_constructible<allocator_type>::value) = default;

    explicit BasicChannel(const allocator_type& alloc) noexcept(
        std::is_nothrow_copy_constructible<allocator_type>::value) :
        alloc_(alloc)
    {
    }

    explicit BasicChannel(size_type samples);

    BasicChannel(size_type samples, const allocator_type& alloc);

    BasicChannel(const BasicChannel& other);

    BasicChannel(const BasicChannel& other, const allocator_type& alloc);

    BasicChannel(BasicChannel&& other) noexcept(std::is_nothrow_move_constructible<allocator_type>::value) :
        super_type(std::move(other)), alloc_(std::move(other.alloc_))
    {
        other.clear_contents();
    }

    BasicChannel(BasicChannel&& other, const allocator_type& alloc) : super_type(std::move(other)), alloc_(alloc)
    {
        other.clear_contents();
    }

    ~BasicChannel()
    {
        if (this->data() != nullptr)
        {
            deallocate();
        }
    }

    BasicChannel& operator=(const BasicChannel& other);

    BasicChannel& operator=(BasicChannel&& other) noexcept(alloc_traits::propagate_on_container_move_assignment::value&&
                                                               std::is_nothrow_move_assignable<allocator_type>::value);

    allocator_type get_allocator() const noexcept
    {
        return this->alloc_;
    }

    void swap(BasicChannel& other);

private:
    void allocate()
    {
        // we don't need to default construct the samples as sample types are trivially default constructible
        super_type::set_pointer(alloc_.allocate(this->samples()));
    }

    void deallocate()
    {
        alloc_.deallocate(this->data(), this->samples());
    }

    void copy_assign_alloc(const BasicChannel& other)
    {
        copy_assign_alloc(
            other, std::integral_constant<bool, alloc_traits::propagate_on_container_copy_assignment::value>());
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

    void move_assign(BasicChannel& other, std::true_type) noexcept(
        std::is_nothrow_move_assignable<allocator_type>::value);

    void move_assign(BasicChannel& other, std::false_type);

    void move_assign_alloc(BasicChannel& other) noexcept(
        !alloc_traits::propagate_on_container_move_assignment::value ||
        std::is_nothrow_move_assignable<allocator_type>::value)
    {
        move_assign_alloc(
            other, std::integral_constant<bool, alloc_traits::propagate_on_container_move_assignment::value>());
    }

    void move_assign_alloc(BasicChannel& other, std::true_type) noexcept(
        std::is_nothrow_move_assignable<allocator_type>::value)
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
BasicChannel<Sample, Allocator>::BasicChannel(size_type samples) : super_type(samples)
{
    if (this->samples() > 0)
    {
        allocate();
        std::fill_n(this->data(), this->samples(), sample{});
    }
}

template<class Sample, class Allocator>
BasicChannel<Sample, Allocator>::BasicChannel(size_type samples, const allocator_type& alloc) :
    super_type(samples), alloc_(alloc)
{
    if (this->samples() > 0)
    {
        allocate();
        std::fill_n(this->data(), this->samples(), sample{});
    }
}

template<class Sample, class Allocator>
BasicChannel<Sample, Allocator>::BasicChannel(const BasicChannel& other) :
    super_type(other), alloc_(alloc_traits::select_on_container_copy_construction(other.alloc_))
{
    if (this->samples() > 0)
    {
        allocate();
        std::copy_n(other.data(), other.samples(), this->data());
    }
}

template<class Sample, class Allocator>
BasicChannel<Sample, Allocator>::BasicChannel(const BasicChannel& other, const allocator_type& alloc) :
    super_type(other), alloc_(alloc)
{
    if (this->samples() > 0)
    {
        allocate();
        std::copy_n(other.data(), other.samples(), this->data());
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
    alloc_traits::propagate_on_container_move_assignment::value&&
        std::is_nothrow_move_assignable<allocator_type>::value)
{
    move_assign(other, std::integral_constant<bool, alloc_traits::propagate_on_container_move_assignment::value>());
    other.clear_contents();
    return *this;
}

template<class Sample, class Allocator>
void BasicChannel<Sample, Allocator>::swap(BasicChannel& other)
{
    super_type::swap(other);
    swap_alloc(other, std::integral_constant<bool, alloc_traits::propagate_on_container_swap::value>());
}

template<class Sample, class Allocator>
void BasicChannel<Sample, Allocator>::move_assign(BasicChannel& other, std::true_type) noexcept(
    std::is_nothrow_move_assignable<allocator_type>::value)
{
    if (this->data() != nullptr)
    {
        deallocate();
    }
    move_assign_alloc(other);
    super_type(std::move(other)).swap(*this);
}

template<class Sample, class Allocator>
void BasicChannel<Sample, Allocator>::move_assign(BasicChannel& other, std::false_type)
{
    if (alloc_ != other.alloc_)
    {
        if (this->data() != nullptr)
        {
            deallocate();
        }
        super_type(other).swap(*this);
        if (this->samples() > 0)
        {
            allocate();
            std::copy_n(other.data(), other.samples(), this->data());
        }
    }
    else
    {
        move_assign(other, std::true_type());
    }
}

template<class SampleType>
using Channel = BasicChannel<Sample<SampleType>>;

template<class SampleType>
using NetworkChannel = BasicChannel<NetworkSample<SampleType>>;

} // namespace ratl

#endif // _ratl_channel_
