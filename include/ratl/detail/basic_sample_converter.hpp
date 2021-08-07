#ifndef _ratl_detail_basic_sample_converter_
#define _ratl_detail_basic_sample_converter_

// ratl includes
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

    inline OutputSample operator()(const InputSample& input) const noexcept
    {
        return OutputSample{SampleConverterImpl::convert(input.get(), dither_generator_)};
    }

private:
    std::reference_wrapper<DitherGenerator> dither_generator_;
};

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_basic_sample_converter_
