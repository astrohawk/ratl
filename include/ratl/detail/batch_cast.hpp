/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_batch_cast_
#define _ratl_detail_batch_cast_

// ratl includes
#include <ratl/detail/batch_value_traits.hpp>

// other includes
#include <type_traits>

#if defined(RATL_HAS_XSIMD)

namespace ratl
{
namespace detail
{
// batch_sample_cast

template<typename OutputSampleType, typename InputBatch>
inline batch_sample_value_type_t<OutputSampleType> batch_sample_cast(const InputBatch& input) noexcept
{
    return xsimd::batch_cast<typename batch_sample_value_type_t<OutputSampleType>::value_type>(input);
}

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_detail_batch_cast_
