#ifndef _ratl_noninterleaved_
#define _ratl_noninterleaved_

// C++ Standard Library includes
#include <iterator>
#include <memory>
#include <type_traits>

// ratl includes
#include <ratl/allocator.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/noninterleaved_span.hpp>

namespace ratl
{
template<class Sample, class Allocator = ratl::Allocator<Sample>>
class BasicNoninterleaved : public BasicNoninterleavedSpan<Sample>
{
private:
    using super_type = BasicNoninterleavedSpan<Sample>;

    using allocator_type = Allocator;
    using alloc_traits = std::allocator_traits<allocator_type>;

public:
    using sample_traits = typename super_type::sample_traits;
    using sample = typename sample_traits::sample;
    using const_sample = typename sample_traits::const_sample;
    using sample_type = typename super_type::sample_type;
    using sample_value_type = typename super_type::sample_value_type;
    using sample_value_pointer = typename super_type::sample_value_pointer;
    using sample_pointer = typename super_type::sample_pointer;
    using sample_const_pointer = typename super_type::sample_const_pointer;
    using sample_reference = typename super_type::sample_reference;
    using sample_const_reference = typename super_type::sample_const_reference;

    using frame_type = typename super_type::frame_type;
    using const_frame_type = typename super_type::const_frame_type;
    using channel_type = typename super_type::channel_type;
    using const_channel_type = typename super_type::const_channel_type;

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

    BasicNoninterleaved() noexcept(std::is_nothrow_default_constructible<allocator_type>::value) {}

    BasicNoninterleaved(const allocator_type& alloc) noexcept(
        std::is_nothrow_copy_constructible<allocator_type>::value) :
        alloc_(alloc)
    {
    }

    BasicNoninterleaved(size_type channels, size_type frames);

    BasicNoninterleaved(size_type channels, size_type frames, const allocator_type& alloc);

    BasicNoninterleaved(const BasicNoninterleaved& other);

    BasicNoninterleaved(const BasicNoninterleaved& other, const allocator_type& alloc);

    BasicNoninterleaved(BasicNoninterleaved&& other) noexcept(
        std::is_nothrow_move_constructible<allocator_type>::value) :
        super_type(std::move(other)), alloc_(std::move(other.alloc_))
    {
        other.clear_contents();
    }

    BasicNoninterleaved(BasicNoninterleaved&& other, const allocator_type& alloc) :
        super_type(std::move(other)), alloc_(alloc)
    {
        other.clear_contents();
    }

    ~BasicNoninterleaved()
    {
        if (this->data() != nullptr)
        {
            deallocate();
        }
    }

    BasicNoninterleaved& operator=(const BasicNoninterleaved& other);

    BasicNoninterleaved& operator=(BasicNoninterleaved&& other) noexcept(
        alloc_traits::propagate_on_container_move_assignment::value&&
            std::is_nothrow_move_assignable<allocator_type>::value);

    allocator_type get_allocator() const noexcept
    {
        return this->alloc_;
    }

    void swap(BasicNoninterleaved& other);

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

    void copy_assign_alloc(const BasicNoninterleaved& other)
    {
        copy_assign_alloc(
            other, std::integral_constant<bool, alloc_traits::propagate_on_container_copy_assignment::value>());
    }

    void copy_assign_alloc(const BasicNoninterleaved& other, std::true_type)
    {
        if (alloc_ != other.alloc_)
        {
            deallocate();
        }
        alloc_ = other.alloc_;
    }

    void copy_assign_alloc(const BasicNoninterleaved&, std::false_type) {}

    void move_assign(BasicNoninterleaved& other, std::true_type) noexcept(
        std::is_nothrow_move_assignable<allocator_type>::value);

    void move_assign(BasicNoninterleaved& other, std::false_type);

    void move_assign_alloc(BasicNoninterleaved& other) noexcept(
        !alloc_traits::propagate_on_container_move_assignment::value ||
        std::is_nothrow_move_assignable<allocator_type>::value)
    {
        move_assign_alloc(
            other, std::integral_constant<bool, alloc_traits::propagate_on_container_move_assignment::value>());
    }

    void move_assign_alloc(BasicNoninterleaved& other, std::true_type) noexcept(
        std::is_nothrow_move_assignable<allocator_type>::value)
    {
        alloc_ = std::move(other.alloc_);
    }

    void move_assign_alloc(BasicNoninterleaved&, std::false_type) noexcept {}

    void swap_alloc(BasicNoninterleaved& other, std::true_type) noexcept
    {
        std::swap(alloc_, other.alloc_);
    }

    void swap_alloc(BasicNoninterleaved&, std::false_type) noexcept {}
};

template<class Sample, class Allocator>
BasicNoninterleaved<Sample, Allocator>::BasicNoninterleaved(size_type channels, size_type frames) :
    super_type(nullptr, channels, frames)
{
    if (this->samples() > 0)
    {
        allocate();
        std::fill_n(this->data(), this->samples(), sample_value_type{});
    }
}

template<class Sample, class Allocator>
BasicNoninterleaved<Sample, Allocator>::BasicNoninterleaved(
    size_type channels, size_type frames, const allocator_type& alloc) :
    super_type(nullptr, channels, frames), alloc_(alloc)
{
    if (this->samples() > 0)
    {
        allocate();
        std::fill_n(this->data(), this->samples(), sample_value_type{});
    }
}

template<class Sample, class Allocator>
BasicNoninterleaved<Sample, Allocator>::BasicNoninterleaved(const BasicNoninterleaved& other) :
    super_type(other), alloc_(alloc_traits::select_on_container_copy_construction(other.alloc_))
{
    if (this->samples() > 0)
    {
        allocate();
        std::copy_n(other.data(), other.samples(), this->data());
    }
}

template<class Sample, class Allocator>
BasicNoninterleaved<Sample, Allocator>::BasicNoninterleaved(
    const BasicNoninterleaved& other, const allocator_type& alloc) :
    super_type(other), alloc_(alloc)
{
    if (this->samples() > 0)
    {
        allocate();
        std::copy_n(other.data(), other.samples(), this->data());
    }
}

template<class Sample, class Allocator>
BasicNoninterleaved<Sample, Allocator>& BasicNoninterleaved<Sample, Allocator>::operator=(
    const BasicNoninterleaved& other)
{
    if (this != &other)
    {
        copy_assign_alloc(other);
        BasicNoninterleaved(other).swap(*this);
    }
    return *this;
}

template<class Sample, class Allocator>
BasicNoninterleaved<Sample, Allocator>& BasicNoninterleaved<Sample, Allocator>::operator=(
    BasicNoninterleaved&& other) noexcept(alloc_traits::propagate_on_container_move_assignment::value&&
                                              std::is_nothrow_move_assignable<allocator_type>::value)
{
    move_assign(other, std::integral_constant<bool, alloc_traits::propagate_on_container_move_assignment::value>());
    other.clear_contents();
    return *this;
}

template<class Sample, class Allocator>
void BasicNoninterleaved<Sample, Allocator>::swap(BasicNoninterleaved& other)
{
    super_type::swap(other);
    swap_alloc(other, std::integral_constant<bool, alloc_traits::propagate_on_container_swap::value>());
}

template<class Sample, class Allocator>
void BasicNoninterleaved<Sample, Allocator>::move_assign(BasicNoninterleaved& other, std::true_type) noexcept(
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
void BasicNoninterleaved<Sample, Allocator>::move_assign(BasicNoninterleaved& other, std::false_type)
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
using Noninterleaved = BasicNoninterleaved<Sample<SampleType>>;

template<class SampleType>
using NetworkNoninterleaved = BasicNoninterleaved<NetworkSample<SampleType>>;

} // namespace ratl

#endif // _ratl_noninterleaved_
