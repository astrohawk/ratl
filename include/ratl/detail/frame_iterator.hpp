#ifndef _ratl_detail_frame_iterator_
#define _ratl_detail_frame_iterator_

// C++ Standard Library includes

// ratl includes
#include <ratl/detail/sample_iterator.hpp>

namespace ratl
{
namespace detail
{
struct FrameIteratorTag
{
};

template<class Sample, bool Contiguous = false>
using FrameIterator = SampleIterator<FrameIteratorTag, Sample, Contiguous>;

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_frame_iterator_
