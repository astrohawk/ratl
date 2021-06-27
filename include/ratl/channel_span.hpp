#ifndef _ratl_channel_span_
#define _ratl_channel_span_

// C++ Standard Library includes

// ratl includes
#include <ratl/detail/channel_iterator.hpp>
#include <ratl/detail/sample_span.hpp>
#include <ratl/network_sample.hpp>
#include <ratl/sample.hpp>

namespace ratl
{
template<class Sample, bool Contiguous = false>
class BasicChannelSpan : public SampleSpan<Sample, Contiguous, detail::ChannelIterator>
{
    using super_type = SampleSpan<Sample, Contiguous, detail::ChannelIterator>;

public:
    using sample_traits = typename super_type::sample_traits;
    using sample = typename super_type::sample;
    using const_sample = typename super_type::const_sample;
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

    using super_type::samples;
};

template<class SampleType, bool Contiguous = false>
using ChannelSpan = BasicChannelSpan<Sample<SampleType>, Contiguous>;

template<class SampleType, bool Contiguous = false>
using ConstChannelSpan = BasicChannelSpan<typename detail::SampleTraits<Sample<SampleType>>::const_sample, Contiguous>;

template<class SampleType, bool Contiguous = false>
using NetworkChannelSpan = BasicChannelSpan<NetworkSample<SampleType>, Contiguous>;

template<class SampleType, bool Contiguous = false>
using ConstNetworkChannelSpan =
    BasicChannelSpan<typename detail::SampleTraits<NetworkSample<SampleType>>::const_sample, Contiguous>;

} // namespace ratl

#endif // _ratl_channel_span_
