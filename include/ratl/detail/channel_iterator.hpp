#ifndef _ratl_detail_channel_iterator_
#define _ratl_detail_channel_iterator_

// C++ Standard Library includes

// ratl includes
#include <ratl/detail/sample_iterator.hpp>

namespace ratl
{
namespace detail
{
struct ChannelIteratorTag
{
};

template<class Sample, bool Contiguous = false>
using ChannelIterator = SampleIterator<ChannelIteratorTag, Sample, Contiguous>;

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_channel_iterator_
