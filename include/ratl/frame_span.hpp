#ifndef _ratl_frame_span_
#define _ratl_frame_span_

// C++ Standard Library includes

// ratl includes
#include <ratl/detail/frame_iterator.hpp>
#include <ratl/detail/sample_span.hpp>
#include <ratl/network_sample.hpp>
#include <ratl/sample.hpp>

namespace ratl
{
template<class Sample, bool Contiguous = false>
class BasicFrameSpan : public SampleSpan<Sample, Contiguous, detail::FrameIterator>
{
    using super_type = SampleSpan<Sample, Contiguous, detail::FrameIterator>;

public:
    using sample_traits = typename super_type::sample_traits;
    using sample = typename super_type::sample;
    using const_sample = typename super_type::const_sample;
    using sample_type = typename super_type::sample_type;
    using sample_value_type = typename super_type::sample_value_type;
    using sample_value_pointer = typename super_type::sample_value_pointer;
    using sample_pointer = typename super_type::sample_pointer;
    using sample_const_pointer = typename super_type::sample_const_pointer;
    using sample_reference = typename super_type::sample_reference;
    using sample_const_reference = typename super_type::sample_const_reference;

    using size_type = typename super_type::size_type;
    using difference_type = typename super_type::difference_type;

    using value_type = typename super_type::value_type;
    using pointer = typename super_type::pointer;
    using const_pointer = typename super_type::const_pointer;
    using reference = typename super_type::reference;
    using const_reference = typename super_type::const_reference;

    using iterator = typename super_type::iterator;
    using const_iterator = typename super_type::const_iterator;
    using reverse_iterator = typename super_type::reverse_iterator;
    using const_reverse_iterator = typename super_type::const_reverse_iterator;

    using super_type::super_type;

    inline size_type channels() const noexcept
    {
        return super_type::samples();
    }
};

template<class SampleType, bool Contiguous = false>
using FrameSpan = BasicFrameSpan<Sample<SampleType>, Contiguous>;

template<class SampleType, bool Contiguous = false>
using NetworkFrameSpan = BasicFrameSpan<NetworkSample<SampleType>, Contiguous>;

} // namespace ratl

#endif // _ratl_frame_span_
