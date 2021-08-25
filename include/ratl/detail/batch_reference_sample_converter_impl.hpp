/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_batch_reference_sample_converter_impl_
#define _ratl_detail_batch_reference_sample_converter_impl_

// ratl includes
#include <ratl/detail/batch_cast.hpp>
#include <ratl/detail/batch_endianness.hpp>
#include <ratl/detail/batch_round.hpp>
#include <ratl/detail/batch_traits.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/detail/convert_traits.hpp>
#include <ratl/detail/reference_sample_converter_impl.hpp>
#include <ratl/sample_limits.hpp>

// other includes
#include <cmath>
#include <type_traits>

namespace ratl
{
namespace detail
{
#if defined(RATL_HAS_XSIMD)

// base_batch_reference_sample_converter_impl

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
struct base_batch_reference_sample_converter_impl;

template<class SampleValueType, class DitherGenerator>
struct base_batch_reference_sample_converter_impl<sample<SampleValueType>, sample<SampleValueType>, DitherGenerator>
{
    static inline const batch_sample_value_type_t<SampleValueType>& batch_convert(
        const batch_sample_value_type_t<SampleValueType>& input, DitherGenerator&) noexcept
    {
        return input;
    }
};

template<class DitherGenerator>
struct base_batch_reference_sample_converter_impl<sample<int16_t>, sample<int24_t>, DitherGenerator>
{
    static inline batch_sample_value_type_t<int24_t> batch_convert(
        const batch_sample_value_type_t<int16_t>& input, DitherGenerator&) noexcept
    {
        return batch_sample_cast<int24_t>(input) << 8;
    }
};

template<class DitherGenerator>
struct base_batch_reference_sample_converter_impl<sample<int16_t>, sample<int32_t>, DitherGenerator>
{
    static inline batch_sample_value_type_t<int32_t> batch_convert(
        const batch_sample_value_type_t<int16_t>& input, DitherGenerator&) noexcept
    {
        return batch_sample_cast<int32_t>(input) << 16;
    }
};

template<class DitherGenerator>
struct base_batch_reference_sample_converter_impl<sample<int24_t>, sample<int16_t>, DitherGenerator>
{
    static constexpr int32_t sample_in_max = static_cast<int32_t>(0x007FFF80);
    static constexpr int16_t sample_out_max = sample_limits<int16_t>::max;
    static constexpr int32_t rounding = static_cast<int32_t>(0x80);
    static constexpr std::size_t total_shift = 8;
    static constexpr std::size_t pre_dither_shift =
        DitherGenerator::int16_bits > 0 ? DitherGenerator::int16_bits - total_shift : 0;
    static constexpr std::size_t post_dither_shift = total_shift + pre_dither_shift;

    static inline batch_sample_value_type_t<int16_t> batch_convert(
        const batch_sample_value_type_t<int24_t>& input, DitherGenerator& dither_gen) noexcept
    {
        static const batch_sample_value_type_t<int24_t> max(sample_out_max);
        auto cmp = input >= sample_in_max;
        auto temp = (((input + (rounding + (input >> 31))) << pre_dither_shift) + dither_gen.generate_batch_int16()) >>
                    post_dither_shift;
        return batch_sample_cast<int16_t>(xsimd::select(cmp, max, temp));
    }
};

template<class DitherGenerator>
constexpr int32_t
    base_batch_reference_sample_converter_impl<sample<int24_t>, sample<int16_t>, DitherGenerator>::sample_in_max;
template<class DitherGenerator>
constexpr int16_t
    base_batch_reference_sample_converter_impl<sample<int24_t>, sample<int16_t>, DitherGenerator>::sample_out_max;
template<class DitherGenerator>
constexpr int32_t
    base_batch_reference_sample_converter_impl<sample<int24_t>, sample<int16_t>, DitherGenerator>::rounding;
template<class DitherGenerator>
constexpr std::size_t
    base_batch_reference_sample_converter_impl<sample<int24_t>, sample<int16_t>, DitherGenerator>::total_shift;
template<class DitherGenerator>
constexpr std::size_t
    base_batch_reference_sample_converter_impl<sample<int24_t>, sample<int16_t>, DitherGenerator>::pre_dither_shift;
template<class DitherGenerator>
constexpr std::size_t
    base_batch_reference_sample_converter_impl<sample<int24_t>, sample<int16_t>, DitherGenerator>::post_dither_shift;

template<class DitherGenerator>
struct base_batch_reference_sample_converter_impl<sample<int24_t>, sample<int32_t>, DitherGenerator>
{
    static inline batch_sample_value_type_t<int32_t> batch_convert(
        const batch_sample_value_type_t<int24_t>& input, DitherGenerator&) noexcept
    {
        return batch_sample_cast<int24_t>(input << 8);
    }
};

template<class DitherGenerator>
struct base_batch_reference_sample_converter_impl<sample<int32_t>, sample<int16_t>, DitherGenerator>
{
    static constexpr int32_t sample_in_max = static_cast<int32_t>(0x7FFF8000);
    static constexpr int16_t sample_out_max = sample_limits<int16_t>::max;
    static constexpr int32_t rounding = static_cast<int32_t>(0x8000);
    static constexpr std::size_t total_shift = 16;
    static constexpr std::size_t pre_dither_shift = total_shift - DitherGenerator::int16_bits;
    static constexpr std::size_t post_dither_shift = total_shift - pre_dither_shift;

    static inline batch_sample_value_type_t<int16_t> batch_convert(
        const batch_sample_value_type_t<int32_t>& input, DitherGenerator& dither_gen) noexcept
    {
        static const batch_sample_value_type_t<int32_t> max(sample_out_max);
        auto cmp = input >= sample_in_max;
        auto temp = (((input + (rounding + (input >> 31))) >> pre_dither_shift) + dither_gen.generate_batch_int16()) >>
                    post_dither_shift;
        return batch_sample_cast<int16_t>(xsimd::select(cmp, max, temp));
    }
};

template<class DitherGenerator>
constexpr int32_t
    base_batch_reference_sample_converter_impl<sample<int32_t>, sample<int16_t>, DitherGenerator>::sample_in_max;
template<class DitherGenerator>
constexpr int16_t
    base_batch_reference_sample_converter_impl<sample<int32_t>, sample<int16_t>, DitherGenerator>::sample_out_max;
template<class DitherGenerator>
constexpr int32_t
    base_batch_reference_sample_converter_impl<sample<int32_t>, sample<int16_t>, DitherGenerator>::rounding;
template<class DitherGenerator>
constexpr std::size_t
    base_batch_reference_sample_converter_impl<sample<int32_t>, sample<int16_t>, DitherGenerator>::total_shift;
template<class DitherGenerator>
constexpr std::size_t
    base_batch_reference_sample_converter_impl<sample<int32_t>, sample<int16_t>, DitherGenerator>::pre_dither_shift;
template<class DitherGenerator>
constexpr std::size_t
    base_batch_reference_sample_converter_impl<sample<int32_t>, sample<int16_t>, DitherGenerator>::post_dither_shift;

template<class DitherGenerator>
struct base_batch_reference_sample_converter_impl<sample<int32_t>, sample<int24_t>, DitherGenerator>
{
    static constexpr int32_t sample_in_max = static_cast<int32_t>(0x7FFFFF80);
    static constexpr int24_t sample_out_max = sample_limits<int24_t>::max;
    static constexpr int32_t rounding = static_cast<int32_t>(0x80);

    static inline batch_sample_value_type_t<int24_t> batch_convert(
        const batch_sample_value_type_t<int32_t>& input, DitherGenerator&) noexcept
    {
        static const batch_sample_value_type_t<int32_t> max(sample_out_max);
        auto cmp = input >= sample_in_max;
        auto temp = (input + (rounding + (input >> 31))) >> 8;
        return batch_sample_cast<int24_t>(xsimd::select(cmp, max, temp));
    }
};

template<class DitherGenerator>
constexpr int32_t
    base_batch_reference_sample_converter_impl<sample<int32_t>, sample<int24_t>, DitherGenerator>::sample_in_max;
template<class DitherGenerator>
constexpr int24_t
    base_batch_reference_sample_converter_impl<sample<int32_t>, sample<int24_t>, DitherGenerator>::sample_out_max;
template<class DitherGenerator>
constexpr int32_t
    base_batch_reference_sample_converter_impl<sample<int32_t>, sample<int24_t>, DitherGenerator>::rounding;

template<class SampleValueType, class DitherGenerator>
struct base_batch_reference_sample_converter_impl<sample<SampleValueType>, sample<float32_t>, DitherGenerator>
{
    static constexpr float32_t scaler = float_convert_traits<SampleValueType>::divisor;

    static inline batch_sample_value_type_t<float32_t> batch_convert(
        const batch_sample_value_type_t<SampleValueType>& input, DitherGenerator&) noexcept
    {
        return xsimd::to_float(batch_sample_cast<int32_t>(input)) * scaler;
    }
};

template<class SampleValueType, class DitherGenerator>
constexpr float32_t
    base_batch_reference_sample_converter_impl<sample<SampleValueType>, sample<float32_t>, DitherGenerator>::scaler;

template<class SampleValueType, class DitherGenerator>
struct base_batch_reference_sample_converter_impl<sample<float32_t>, sample<SampleValueType>, DitherGenerator>
{
private:
    static constexpr float32_t sample_in_max =
        static_cast<float32_t>(sample_limits<SampleValueType>::max) * float_convert_traits<SampleValueType>::divisor;
    static constexpr SampleValueType sample_out_max = sample_limits<SampleValueType>::max;
    static constexpr float32_t sample_in_min =
        static_cast<float32_t>(sample_limits<SampleValueType>::min) * float_convert_traits<SampleValueType>::divisor;
    static constexpr SampleValueType sample_out_min = sample_limits<SampleValueType>::min;
    static constexpr float32_t scaler =
        float_convert_traits<SampleValueType>::multiplier - DitherGenerator::float32_max;

public:
    static inline batch_sample_value_type_t<SampleValueType> batch_convert(
        batch_sample_value_type_t<float32_t> input, DitherGenerator& dither_gen) noexcept
    {
        static const batch_sample_value_type_t<int32_t> min(sample_out_min);
        static const batch_sample_value_type_t<int32_t> max(sample_out_max);
        auto out = batch_round_float32_to_int32((input * scaler) + dither_gen.generate_batch_float32());
        out = xsimd::select(xsimd::bool_cast(input >= sample_in_max), max, out);
        out = xsimd::select(xsimd::bool_cast(input < sample_in_min), min, out);
        return batch_sample_cast<SampleValueType>(out);
    }
};

template<class SampleValueType, class DitherGenerator>
constexpr float32_t
    base_batch_reference_sample_converter_impl<sample<float32_t>, sample<SampleValueType>, DitherGenerator>::
        sample_in_max;
template<class SampleValueType, class DitherGenerator>
constexpr SampleValueType
    base_batch_reference_sample_converter_impl<sample<float32_t>, sample<SampleValueType>, DitherGenerator>::
        sample_out_max;
template<class SampleValueType, class DitherGenerator>
constexpr float32_t
    base_batch_reference_sample_converter_impl<sample<float32_t>, sample<SampleValueType>, DitherGenerator>::
        sample_in_min;
template<class SampleValueType, class DitherGenerator>
constexpr SampleValueType
    base_batch_reference_sample_converter_impl<sample<float32_t>, sample<SampleValueType>, DitherGenerator>::
        sample_out_min;
template<class SampleValueType, class DitherGenerator>
constexpr float32_t
    base_batch_reference_sample_converter_impl<sample<float32_t>, sample<SampleValueType>, DitherGenerator>::scaler;

template<class DitherGenerator>
struct base_batch_reference_sample_converter_impl<sample<float32_t>, sample<float32_t>, DitherGenerator>
{
    static inline batch_sample_value_type_t<float32_t> batch_convert(
        const batch_sample_value_type_t<float32_t>& input, DitherGenerator&) noexcept
    {
        return input;
    }
};

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
struct base_batch_reference_sample_converter_impl<
    sample<InputSampleType>,
    network_sample<OutputSampleType>,
    DitherGenerator>
{
    static inline batch_network_sample_value_type_t<OutputSampleType> batch_convert(
        const batch_sample_value_type_t<InputSampleType>& input, DitherGenerator& dither_gen) noexcept
    {
        return base_batch_reference_sample_converter_impl<
            sample<OutputSampleType>,
            network_sample<OutputSampleType>,
            DitherGenerator>::
            batch_convert(
                base_batch_reference_sample_converter_impl<
                    sample<InputSampleType>,
                    sample<OutputSampleType>,
                    DitherGenerator>::batch_convert(input, dither_gen),
                dither_gen);
    }
};

template<class SampleValueType, class DitherGenerator>
struct base_batch_reference_sample_converter_impl<
    sample<SampleValueType>,
    network_sample<SampleValueType>,
    DitherGenerator>
{
    static inline batch_network_sample_value_type_t<SampleValueType> batch_convert(
        const batch_sample_value_type_t<SampleValueType>& input, DitherGenerator&) noexcept
    {
        return batch_sample_to_network_sample<SampleValueType>(input);
    }
};

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
struct base_batch_reference_sample_converter_impl<
    network_sample<InputSampleType>,
    sample<OutputSampleType>,
    DitherGenerator>
{
    static inline batch_sample_value_type_t<OutputSampleType> batch_convert(
        const batch_network_sample_value_type_t<InputSampleType>& input, DitherGenerator& dither_gen) noexcept
    {
        return base_batch_reference_sample_converter_impl<
            sample<InputSampleType>,
            sample<OutputSampleType>,
            DitherGenerator>::
            batch_convert(
                base_batch_reference_sample_converter_impl<
                    network_sample<InputSampleType>,
                    sample<InputSampleType>,
                    DitherGenerator>::batch_convert(input, dither_gen),
                dither_gen);
    }
};

template<class SampleValueType, class DitherGenerator>
struct base_batch_reference_sample_converter_impl<
    network_sample<SampleValueType>,
    sample<SampleValueType>,
    DitherGenerator>
{
    static inline batch_sample_value_type_t<SampleValueType> batch_convert(
        const batch_network_sample_value_type_t<SampleValueType>& input, DitherGenerator&) noexcept
    {
        return batch_network_sample_to_sample<SampleValueType>(input);
    }
};

// batch_reference_sample_converter_impl

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
struct batch_reference_sample_converter_impl :
    base_batch_reference_sample_converter_impl<InputSampleType, OutputSampleType, DitherGenerator>,
    reference_sample_converter_impl<InputSampleType, OutputSampleType, DitherGenerator>
{
};

#else

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
using batch_reference_sample_converter_impl =
    reference_sample_converter_impl<InputSampleType, OutputSampleType, DitherGenerator>;

#endif

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_batch_reference_sample_converter_impl_
