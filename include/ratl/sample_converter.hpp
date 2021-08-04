#ifndef _ratl_sample_converter_
#define _ratl_sample_converter_

// ratl includes
#include <ratl/detail/batch_fast_sample_converter.hpp>
#include <ratl/detail/batch_reference_sample_converter.hpp>
#include <ratl/detail/config.hpp>

namespace ratl
{
using ReferenceSampleConverter = detail::BatchReferenceSampleConverter;

using FastSampleConverter = detail::BatchFastSampleConverter;

using DefaultSampleConverter = ReferenceSampleConverter;

} // namespace ratl

#endif // _ratl_sample_converter_
