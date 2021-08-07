#ifndef _ratl_detail_batch_basic_sample_converter_
#define _ratl_detail_batch_basic_sample_converter_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/detail/basic_sample_converter.hpp>
#include <ratl/detail/batch_traits.hpp>
#include <ratl/detail/config.hpp>

namespace ratl
{
namespace detail
{
#if defined(RATL_HAS_XSIMD)

template<
    template<class, class, class>
    class BatchSampleConverterImplT,
    class InputSample,
    class OutputSample,
    class DitherGenerator>
class BatchBasicSampleConverter
{
private:
    using BatchSampleConverterImpl = BatchSampleConverterImplT<InputSample, OutputSample, DitherGenerator>;
    using SampleConverter = BasicSampleConverter<BatchSampleConverterImplT, InputSample, OutputSample, DitherGenerator>;

public:
    explicit BatchBasicSampleConverter(DitherGenerator& dither_generator) : dither_generator_{dither_generator} {}

    inline BatchSampleType_t<OutputSample> operator()(const BatchSampleType_t<InputSample>& input) const noexcept
    {
        return BatchSampleConverterImpl::batch_convert(input, dither_generator_);
    }

    inline OutputSample operator()(const InputSample& input) const noexcept
    {
        return SampleConverter{dither_generator_}(input);
    }

private:
    std::reference_wrapper<DitherGenerator> dither_generator_;
};

#else

template<
    template<class, class, class>
    class SampleConverterImplT,
    class InputSample,
    class OutputSample,
    class DitherGenerator>
using BatchBasicSampleConverter =
    BasicSampleConverter<SampleConverterImplT, InputSample, OutputSample, DitherGenerator>;

#endif

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_batch_basic_sample_converter_
