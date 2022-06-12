/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

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
    template<typename, typename, typename>
    class SampleConverterImpl,
    typename InputSampleType,
    typename OutputSampleType,
    typename DitherGenerator>
class basic_sample_converter
{
    using sample_converter_impl = SampleConverterImpl<InputSampleType, OutputSampleType, DitherGenerator>;

public:
    explicit basic_sample_converter(DitherGenerator& dither_gen) : dither_gen_(dither_gen) {}

#if defined(RATL_HAS_XSIMD)
    inline batch_sample_type_t<OutputSampleType> operator()(
        const batch_sample_type_t<InputSampleType>& input) const noexcept
    {
        return sample_converter_impl::batch_convert(input, dither_gen_);
    }
#endif

    inline OutputSampleType operator()(const InputSampleType& input) const noexcept
    {
        return OutputSampleType(sample_converter_impl::convert(input.get(), dither_gen_));
    }

private:
    std::reference_wrapper<DitherGenerator> dither_gen_;
};

template<typename InputSampleType, typename OutputSampleType, typename DitherGenerator>
using reference_sample_converter = basic_sample_converter<
    detail::batch_reference_sample_converter_impl,
    InputSampleType,
    OutputSampleType,
    DitherGenerator>;

template<typename InputSampleType, typename OutputSampleType, typename DitherGenerator>
using fast_sample_converter = basic_sample_converter<
    detail::batch_fast_sample_converter_impl,
    InputSampleType,
    OutputSampleType,
    DitherGenerator>;

template<typename InputSampleType, typename OutputSampleType, typename DitherGenerator>
using default_sample_converter = fast_sample_converter<InputSampleType, OutputSampleType, DitherGenerator>;

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_sample_converter_
