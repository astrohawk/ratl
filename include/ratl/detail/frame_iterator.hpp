#ifndef _ratl_detail_frame_iterator_
#define _ratl_detail_frame_iterator_

// C++ Standard Library includes

// ratl includes
#include <ratl/detail/sample_iterator.hpp>

namespace ratl
{
namespace detail
{
struct frame_iterator_tag
{
};

template<class SampleType, bool Contiguous = false>
using frame_iterator = sample_iterator<frame_iterator_tag, SampleType, Contiguous>;

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_frame_iterator_
