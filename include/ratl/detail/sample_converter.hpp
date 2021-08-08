#ifndef _ratl_detail_sample_converter_
#define _ratl_detail_sample_converter_

// ratl includes
#include <ratl/detail/batch_fast_sample_converter_impl.hpp>
#include <ratl/detail/batch_reference_sample_converter_impl.hpp>
#include <ratl/detail/batch_traits.hpp>
#include <ratl/detail/config.hpp>

namespace ratl
{
namespace detail
{
template<
    template<class, class, class>
    class SampleConverterImplT,
    class InputSample,
    class OutputSample,
    class DitherGenerator>
class BasicSampleConverter
{
private:
    using SampleConverterImpl = SampleConverterImplT<InputSample, OutputSample, DitherGenerator>;

public:
    explicit BasicSampleConverter(DitherGenerator& dither_generator) : dither_generator_{dither_generator} {}

#if defined(RATL_HAS_XSIMD)
    inline BatchSampleType_t<OutputSample> operator()(const BatchSampleType_t<InputSample>& input) const noexcept
    {
        return SampleConverterImpl::batch_convert(input, dither_generator_);
    }
#endif

    inline OutputSample operator()(const InputSample& input) const noexcept
    {
        return OutputSample{SampleConverterImpl::convert(input.get(), dither_generator_)};
    }

private:
    std::reference_wrapper<DitherGenerator> dither_generator_;
};

template<class InputSample, class OutputSample, class DitherGenerator>
using ReferenceSampleConverter =
    BasicSampleConverter<detail::BatchReferenceSampleConverterImpl, InputSample, OutputSample, DitherGenerator>;

template<class InputSample, class OutputSample, class DitherGenerator>
using FastSampleConverter =
    BasicSampleConverter<detail::BatchFastSampleConverterImpl, InputSample, OutputSample, DitherGenerator>;

template<class InputSample, class OutputSample, class DitherGenerator>
using DefaultSampleConverter = FastSampleConverter<InputSample, OutputSample, DitherGenerator>;

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_sample_converter_
