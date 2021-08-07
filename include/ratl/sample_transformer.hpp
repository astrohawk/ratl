#ifndef _ratl_sample_transformer_
#define _ratl_sample_transformer_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/detail/batch_basic_sample_transformer.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/sample_converter.hpp>

namespace ratl
{
template<class InputSample, class OutputSample, class DitherGenerator>
using ReferenceSampleTransformer =
    detail::BatchBasicSampleTransformer<ReferenceSampleConverter, InputSample, OutputSample, DitherGenerator>;

template<class InputSample, class OutputSample, class DitherGenerator>
using FastSampleTransformer =
    detail::BatchBasicSampleTransformer<FastSampleConverter, InputSample, OutputSample, DitherGenerator>;

template<class InputSample, class OutputSample, class DitherGenerator>
using DefaultSampleTransformer =
    detail::BatchBasicSampleTransformer<DefaultSampleConverter, InputSample, OutputSample, DitherGenerator>;
} // namespace ratl

#endif // _ratl_sample_transformer_
