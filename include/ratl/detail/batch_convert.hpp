#ifndef _ratl_detail_batch_convert_
#define _ratl_detail_batch_convert_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/detail/batch_traits.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/network_sample.hpp>
#include <ratl/sample.hpp>

#if defined(RATL_HAS_XSIMD)

namespace ratl
{
namespace detail
{
template<class BatchSampleConverter, class InputSample, class OutputSample, class DitherGenerator>
class BatchConverterImpl;

template<class BatchSampleConverter, class InputSampleType, class OutputSampleType, class DitherGenerator>
class BatchConverterImpl<BatchSampleConverter, Sample<InputSampleType>, Sample<OutputSampleType>, DitherGenerator>
{
private:
    using InputSample = Sample<InputSampleType>;
    using OutputSample = Sample<OutputSampleType>;

    using InputBatchSampleValueType = BatchSampleType_t<InputSample>;
    using OutputBatchSampleValueType = BatchSampleType_t<OutputSample>;

    using BatchSampleToSampleConverter = typename BatchSampleConverter::
        template BatchSampleToSampleConverter<InputSampleType, OutputSampleType, DitherGenerator>;

public:
    explicit BatchConverterImpl(DitherGenerator& dither_generator) : dither_generator_{dither_generator} {}

    inline OutputBatchSampleValueType operator()(const InputBatchSampleValueType& input) const noexcept
    {
        return BatchSampleToSampleConverter::convert(input, dither_generator_);
    }

private:
    DitherGenerator& dither_generator_;
};

template<class BatchSampleConverter, class InputSampleType, class OutputSampleType, class DitherGenerator>
class BatchConverterImpl<
    BatchSampleConverter,
    Sample<InputSampleType>,
    NetworkSample<OutputSampleType>,
    DitherGenerator>
{
private:
    using InputSample = Sample<InputSampleType>;
    using OutputSample = NetworkSample<OutputSampleType>;

    using InputBatchSampleValueType = BatchSampleType_t<InputSample>;
    using OutputBatchSampleValueType = BatchSampleType_t<OutputSample>;

    using BatchSampleToNetworkSampleConverter = typename BatchSampleConverter::
        template BatchSampleToNetworkSampleConverter<InputSampleType, OutputSampleType, DitherGenerator>;

public:
    explicit BatchConverterImpl(DitherGenerator& dither_generator) : dither_generator_{dither_generator} {}

    inline OutputBatchSampleValueType operator()(const InputBatchSampleValueType& input) const noexcept
    {
        return BatchSampleToNetworkSampleConverter::convert(input, dither_generator_);
    }

private:
    DitherGenerator& dither_generator_;
};

template<class BatchSampleConverter, class InputSampleType, class OutputSampleType, class DitherGenerator>
class BatchConverterImpl<
    BatchSampleConverter,
    NetworkSample<InputSampleType>,
    Sample<OutputSampleType>,
    DitherGenerator>
{
private:
    using InputSample = NetworkSample<InputSampleType>;
    using OutputSample = Sample<OutputSampleType>;

    using InputBatchSampleValueType = BatchSampleType_t<InputSample>;
    using OutputBatchSampleValueType = BatchSampleType_t<OutputSample>;

    using BatchNetworkSampleToSampleConverter = typename BatchSampleConverter::
        template BatchNetworkSampleToSampleConverter<InputSampleType, OutputSampleType, DitherGenerator>;

public:
    explicit BatchConverterImpl(DitherGenerator& dither_generator) : dither_generator_{dither_generator} {}

    inline OutputBatchSampleValueType operator()(const InputBatchSampleValueType& input) const noexcept
    {
        return BatchNetworkSampleToSampleConverter::convert(input, dither_generator_);
    }

private:
    DitherGenerator& dither_generator_;
};

template<class BatchSampleConverter, class InputSample, class OutputSample, class DitherGenerator>
using DefaultBatchConverter = BatchConverterImpl<
    BatchSampleConverter,
    std::remove_cv_t<std::remove_reference_t<InputSample>>,
    std::remove_cv_t<std::remove_reference_t<OutputSample>>,
    DitherGenerator>;

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_detail_batch_convert_
