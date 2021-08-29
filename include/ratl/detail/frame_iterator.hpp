/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_frame_iterator_
#define _ratl_detail_frame_iterator_

// ratl includes
#include <ratl/detail/sample_iterator.hpp>

namespace ratl
{
namespace detail
{
struct frame_iterator_tag
{
};

template<typename SampleType, typename SampleTraits, typename Contiguous = std::false_type>
using frame_iterator = sample_iterator<frame_iterator_tag, SampleType, SampleTraits, Contiguous>;

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_frame_iterator_
