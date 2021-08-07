#ifndef _ratl_sample_converter_
#define _ratl_sample_converter_

// ratl includes
#include <ratl/detail/batch_basic_sample_converter.hpp>
#include <ratl/detail/batch_fast_sample_converter_impl.hpp>
#include <ratl/detail/batch_reference_sample_converter_impl.hpp>
#include <ratl/detail/config.hpp>

namespace ratl
{
template<class InputSample, class OutputSample, class DitherGenerator>
using ReferenceSampleConverter = detail::
    BatchBasicSampleConverter<detail::BatchReferenceSampleConverterImpl, InputSample, OutputSample, DitherGenerator>;

template<class InputSample, class OutputSample, class DitherGenerator>
using FastSampleConverter =
    detail::BatchBasicSampleConverter<detail::BatchFastSampleConverterImpl, InputSample, OutputSample, DitherGenerator>;

template<class InputSample, class OutputSample, class DitherGenerator>
using DefaultSampleConverter = FastSampleConverter<InputSample, OutputSample, DitherGenerator>;

} // namespace ratl

#endif // _ratl_sample_converter_
