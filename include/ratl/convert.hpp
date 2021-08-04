#ifndef _ratl_convert_
#define _ratl_convert_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/reference_sample_converter.hpp>
#include <ratl/dither_generator.hpp>
#include <ratl/network_sample.hpp>
#include <ratl/sample.hpp>

namespace ratl
{
namespace detail
{
template<class InputSample, class OutputSample, class DitherGenerator>
class DefaultConverterImpl;

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
class DefaultConverterImpl<Sample<InputSampleType>, Sample<OutputSampleType>, DitherGenerator>
{
private:
    using InputSample = Sample<InputSampleType>;
    using OutputSample = Sample<OutputSampleType>;

    using SampleConverter = detail::ReferenceSampleConverter<InputSampleType, OutputSampleType, DitherGenerator>;

public:
    explicit DefaultConverterImpl(DitherGenerator& dither_generator) : dither_generator_{dither_generator} {}

    inline OutputSample operator()(const InputSample& input) const noexcept
    {
        return OutputSample{SampleConverter::convert(input.get(), dither_generator_)};
    }

private:
    DitherGenerator& dither_generator_;
};

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
class DefaultConverterImpl<Sample<InputSampleType>, NetworkSample<OutputSampleType>, DitherGenerator>
{
private:
    using InputSample = Sample<InputSampleType>;
    using OutputSample = NetworkSample<OutputSampleType>;

    using SampleConverter =
        detail::ReferenceSampleToNetworkConverter<InputSampleType, OutputSampleType, DitherGenerator>;

public:
    explicit DefaultConverterImpl(DitherGenerator& dither_generator) : dither_generator_{dither_generator} {}

    inline OutputSample operator()(const InputSample& input) const noexcept
    {
        return OutputSample{SampleConverter::convert(input.get(), dither_generator_)};
    }

private:
    DitherGenerator& dither_generator_;
};

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
class DefaultConverterImpl<NetworkSample<InputSampleType>, Sample<OutputSampleType>, DitherGenerator>
{
private:
    using InputSample = NetworkSample<InputSampleType>;
    using OutputSample = Sample<OutputSampleType>;

    using SampleConverter =
        detail::ReferenceNetworkToSampleConverter<InputSampleType, OutputSampleType, DitherGenerator>;

public:
    explicit DefaultConverterImpl(DitherGenerator& dither_generator) : dither_generator_{dither_generator} {}

    inline OutputSample operator()(const InputSample& input) const noexcept
    {
        return OutputSample{SampleConverter::convert(input.get(), dither_generator_)};
    }

private:
    DitherGenerator& dither_generator_;
};

// DefaultConverter alias

template<class InputSample, class OutputSample, class DitherGenerator>
using DefaultConverter = DefaultConverterImpl<
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
    using Converter = detail::DefaultConverter<InputSample, OutputSample, DitherGenerator>;
    return convert(sample, Converter{dither_generator});
}

template<class OutputSample, class InputSample>
inline OutputSample convert(const InputSample& sample) noexcept
{
    using ConverterDitherGenerator = detail::NullDitherGenerator;
    using Converter = detail::DefaultConverter<InputSample, OutputSample, ConverterDitherGenerator>;
    ConverterDitherGenerator dither_generator{};
    return convert(sample, Converter{dither_generator});
}

} // namespace ratl

#endif // _ratl_convert_
