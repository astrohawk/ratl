#ifndef _ratl_convert_
#define _ratl_convert_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/dither_generator.hpp>
#include <ratl/network_sample.hpp>
#include <ratl/sample.hpp>
#include <ratl/sample_converter.hpp>

namespace ratl
{
namespace detail
{
template<class SampleConverter, class InputSample, class OutputSample, class DitherGenerator>
class DefaultConverterImpl;

template<class SampleConverter, class InputSampleType, class OutputSampleType, class DitherGenerator>
class DefaultConverterImpl<SampleConverter, Sample<InputSampleType>, Sample<OutputSampleType>, DitherGenerator>
{
private:
    using InputSample = Sample<InputSampleType>;
    using OutputSample = Sample<OutputSampleType>;

    using SampleToSampleConverter =
        typename SampleConverter::template SampleToSampleConverter<InputSampleType, OutputSampleType, DitherGenerator>;

public:
    explicit DefaultConverterImpl(DitherGenerator& dither_generator) : dither_generator_{dither_generator} {}

    inline OutputSample operator()(const InputSample& input) const noexcept
    {
        return OutputSample{SampleToSampleConverter::convert(input.get(), dither_generator_)};
    }

private:
    DitherGenerator& dither_generator_;
};

template<class SampleConverter, class InputSampleType, class OutputSampleType, class DitherGenerator>
class DefaultConverterImpl<SampleConverter, Sample<InputSampleType>, NetworkSample<OutputSampleType>, DitherGenerator>
{
private:
    using InputSample = Sample<InputSampleType>;
    using OutputSample = NetworkSample<OutputSampleType>;

    using SampleToNetworkSampleConverter = typename SampleConverter::
        template SampleToNetworkSampleConverter<InputSampleType, OutputSampleType, DitherGenerator>;

public:
    explicit DefaultConverterImpl(DitherGenerator& dither_generator) : dither_generator_{dither_generator} {}

    inline OutputSample operator()(const InputSample& input) const noexcept
    {
        return OutputSample{SampleToNetworkSampleConverter::convert(input.get(), dither_generator_)};
    }

private:
    DitherGenerator& dither_generator_;
};

template<class SampleConverter, class InputSampleType, class OutputSampleType, class DitherGenerator>
class DefaultConverterImpl<SampleConverter, NetworkSample<InputSampleType>, Sample<OutputSampleType>, DitherGenerator>
{
private:
    using InputSample = NetworkSample<InputSampleType>;
    using OutputSample = Sample<OutputSampleType>;

    using NetworkSampleToSampleConverter = typename SampleConverter::
        template NetworkSampleToSampleConverter<InputSampleType, OutputSampleType, DitherGenerator>;

public:
    explicit DefaultConverterImpl(DitherGenerator& dither_generator) : dither_generator_{dither_generator} {}

    inline OutputSample operator()(const InputSample& input) const noexcept
    {
        return OutputSample{NetworkSampleToSampleConverter::convert(input.get(), dither_generator_)};
    }

private:
    DitherGenerator& dither_generator_;
};

// DefaultConverter alias

template<class SampleConverter, class InputSample, class OutputSample, class DitherGenerator>
using DefaultConverter = DefaultConverterImpl<
    SampleConverter,
    std::remove_cv_t<std::remove_reference_t<InputSample>>,
    std::remove_cv_t<std::remove_reference_t<OutputSample>>,
    DitherGenerator>;

} // namespace detail

// convert functions

template<class InputSample, class Converter>
inline auto convert(const InputSample& sample, Converter converter) -> decltype(converter(sample))
{
    return converter(sample);
}

template<class OutputSample, class InputSample>
inline OutputSample convert(const InputSample& sample, DitherGenerator& dither_generator) noexcept
{
    using Converter = detail::DefaultConverter<DefaultSampleConverter, InputSample, OutputSample, DitherGenerator>;
    return convert(sample, Converter{dither_generator});
}

template<class OutputSample, class InputSample>
inline OutputSample convert(const InputSample& sample) noexcept
{
    using ConverterDitherGenerator = detail::NullDitherGenerator;
    using Converter =
        detail::DefaultConverter<DefaultSampleConverter, InputSample, OutputSample, ConverterDitherGenerator>;
    ConverterDitherGenerator dither_generator{};
    return convert(sample, Converter{dither_generator});
}

} // namespace ratl

#endif // _ratl_convert_
