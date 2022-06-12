/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_channel_iterator_
#define _ratl_detail_channel_iterator_

// ratl includes
#include <ratl/detail/sample_iterator.hpp>

namespace ratl
{
namespace detail
{
struct channel_iterator_tag
{
};

template<typename SampleType, typename SampleTraits, typename Contiguous = std::false_type>
using channel_iterator = sample_iterator<channel_iterator_tag, SampleType, SampleTraits, Contiguous>;

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_channel_iterator_
