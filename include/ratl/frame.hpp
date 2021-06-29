#ifndef _ratl_frame_
#define _ratl_frame_

// C++ Standard Library includes
#include <iterator>
#include <memory>
#include <type_traits>

// ratl includes
#include <ratl/allocator.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/frame_span.hpp>

namespace ratl
{
template<class Sample, class Allocator = ratl::Allocator<Sample>>
class BasicFrame : public BasicFrameSpan<Sample, true>
{
private:
    using super_type = BasicFrameSpan<Sample>;

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

    BasicFrame() noexcept(std::is_nothrow_default_constructible<allocator_type>::value) = default;

    explicit BasicFrame(const allocator_type& alloc) noexcept(
        std::is_nothrow_copy_constructible<allocator_type>::value) :
        alloc_(alloc)
    {
    }

    explicit BasicFrame(size_type channels);

    BasicFrame(size_type channels, const allocator_type& alloc);

    BasicFrame(const BasicFrame& other);

    BasicFrame(const BasicFrame& other, const allocator_type& alloc);

    BasicFrame(BasicFrame&& other) noexcept(std::is_nothrow_move_constructible<allocator_type>::value) :
        super_type(std::move(other)), alloc_(std::move(other.alloc_))
    {
        other.clear_contents();
    }

    BasicFrame(BasicFrame&& other, const allocator_type& alloc) : super_type(std::move(other)), alloc_(alloc)
    {
        other.clear_contents();
    }

    ~BasicFrame()
    {
        if (this->data() != nullptr)
        {
            deallocate();
        }
    }

    BasicFrame& operator=(const BasicFrame& other);

    BasicFrame& operator=(BasicFrame&& other) noexcept(alloc_traits::propagate_on_container_move_assignment::value&&
                                                           std::is_nothrow_move_assignable<allocator_type>::value);

    allocator_type get_allocator() const noexcept
    {
        return this->alloc_;
    }

    void swap(BasicFrame& other);

private:
    void allocate()
    {
        // we don't need to default construct the samples as sample types are trivially default constructible
        super_type::set_pointer(alloc_.allocate(this->channels()));
    }

    void deallocate()
    {
        alloc_.deallocate(this->data(), this->channels());
    }

    void copy_assign_alloc(const BasicFrame& other)
    {
        copy_assign_alloc(
            other, std::integral_constant<bool, alloc_traits::propagate_on_container_copy_assignment::value>());
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

    void move_assign(BasicFrame& other, std::true_type) noexcept(
        std::is_nothrow_move_assignable<allocator_type>::value);

    void move_assign(BasicFrame& other, std::false_type);

    void move_assign_alloc(BasicFrame& other) noexcept(
        !alloc_traits::propagate_on_container_move_assignment::value ||
        std::is_nothrow_move_assignable<allocator_type>::value)
    {
        move_assign_alloc(
            other, std::integral_constant<bool, alloc_traits::propagate_on_container_move_assignment::value>());
    }

    void move_assign_alloc(BasicFrame& other, std::true_type) noexcept(
        std::is_nothrow_move_assignable<allocator_type>::value)
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
BasicFrame<Sample, Allocator>::BasicFrame(size_type channels) : super_type(channels)
{
    if (this->channels() > 0)
    {
        allocate();
        std::fill_n(this->data(), this->channels(), sample{});
    }
}

template<class Sample, class Allocator>
BasicFrame<Sample, Allocator>::BasicFrame(size_type channels, const allocator_type& alloc) :
    super_type(channels), alloc_(alloc)
{
    if (this->channels() > 0)
    {
        allocate();
        std::fill_n(this->data(), this->channels(), sample{});
    }
}

template<class Sample, class Allocator>
BasicFrame<Sample, Allocator>::BasicFrame(const BasicFrame& other) :
    super_type(other), alloc_(alloc_traits::select_on_container_copy_construction(other.alloc_))
{
    if (this->channels() > 0)
    {
        allocate();
        std::copy_n(other.data(), other.channels(), this->data());
    }
}

template<class Sample, class Allocator>
BasicFrame<Sample, Allocator>::BasicFrame(const BasicFrame& other, const allocator_type& alloc) :
    super_type(other), alloc_(alloc)
{
    if (this->channels() > 0)
    {
        allocate();
        std::copy_n(other.data(), other.channels(), this->data());
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
    alloc_traits::propagate_on_container_move_assignment::value&&
        std::is_nothrow_move_assignable<allocator_type>::value)
{
    move_assign(other, std::integral_constant<bool, alloc_traits::propagate_on_container_move_assignment::value>());
    other.clear_contents();
    return *this;
}

template<class Sample, class Allocator>
void BasicFrame<Sample, Allocator>::swap(BasicFrame& other)
{
    super_type::swap(other);
    swap_alloc(other, std::integral_constant<bool, alloc_traits::propagate_on_container_swap::value>());
}

template<class Sample, class Allocator>
void BasicFrame<Sample, Allocator>::move_assign(BasicFrame& other, std::true_type) noexcept(
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
void BasicFrame<Sample, Allocator>::move_assign(BasicFrame& other, std::false_type)
{
    if (alloc_ != other.alloc_)
    {
        if (this->data() != nullptr)
        {
            deallocate();
        }
        super_type(other).swap(*this);
        if (this->channels() > 0)
        {
            allocate();
            std::copy_n(other.data(), other.channels(), this->data());
        }
    }
    else
    {
        move_assign(other, std::true_type());
    }
}

template<class SampleType>
using Frame = BasicFrame<Sample<SampleType>>;

template<class SampleType>
using NetworkFrame = BasicFrame<NetworkSample<SampleType>>;

} // namespace ratl

#endif // _ratl_frame_
