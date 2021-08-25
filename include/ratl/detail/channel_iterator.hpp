#ifndef _ratl_detail_channel_iterator_
#define _ratl_detail_channel_iterator_

// C++ Standard Library includes

// ratl includes
#include <ratl/detail/sample_iterator.hpp>

namespace ratl
{
namespace detail
{
struct channel_iterator_tag
{
};

template<class SampleType, bool Contiguous = false>
using channel_iterator = sample_iterator<channel_iterator_tag, SampleType, Contiguous>;

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_channel_iterator_
