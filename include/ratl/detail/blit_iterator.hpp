/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_blit_iterator_
#define _ratl_detail_blit_iterator_

// ratl includes
#include <ratl/detail/sample_iterator.hpp>

namespace ratl
{
namespace detail
{
struct blit_iterator_tag
{
};

template<typename SampleType, typename SampleTraits>
using blit_iterator = sample_iterator<blit_iterator_tag, SampleType, SampleTraits, std::true_type>;

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_blit_iterator_
