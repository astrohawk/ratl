/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_reference_sample_converter_impl_
#define _ratl_detail_reference_sample_converter_impl_

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/convert_traits.hpp>
#include <ratl/detail/endianness.hpp>
#include <ratl/detail/round.hpp>
#include <ratl/network_sample.hpp>
#include <ratl/sample.hpp>
#include <ratl/sample_limits.hpp>

// other includes
#include <cfenv>
#include <cmath>
#include <type_traits>

namespace ratl
{
namespace detail
{
// reference_sample_converter_impl

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
struct reference_sample_converter_impl;

template<class SampleValueType, class DitherGenerator>
struct reference_sample_converter_impl<sample<SampleValueType>, sample<SampleValueType>, DitherGenerator>
{
    static inline SampleValueType convert(SampleValueType input, DitherGenerator&) noexcept
    {
        return input;
    }
};

template<class DitherGenerator>
struct reference_sample_converter_impl<sample<int16_t>, sample<int24_t>, DitherGenerator>
{
    static inline int24_t convert(int32_t input, DitherGenerator&) noexcept
    {
        return static_cast<int24_t>(input << 8);
    }
};

template<class DitherGenerator>
struct reference_sample_converter_impl<sample<int16_t>, sample<int32_t>, DitherGenerator>
{
    static inline int32_t convert(int32_t input, DitherGenerator&) noexcept
    {
        return input << 16;
    }
};

// sample >> 31 results in 0 if sample is positive or 0, and -1 if sample is negative
// this is added to the rounding constant to perform round half away from zero as opposed to just round half up

template<class DitherGenerator>
struct reference_sample_converter_impl<sample<int24_t>, sample<int16_t>, DitherGenerator>
{
    static constexpr int32_t sample_in_max = static_cast<int32_t>(0x007FFF80);
    static constexpr int16_t sample_out_max = sample_limits<int16_t>::max;
    static constexpr int32_t rounding = static_cast<int32_t>(0x80);
    static constexpr std::size_t total_shift = 8;
    static constexpr std::size_t pre_dither_shift =
        DitherGenerator::int16_bits > 0 ? DitherGenerator::int16_bits - total_shift : 0;
    static constexpr std::size_t post_dither_shift = total_shift + pre_dither_shift;

    static inline int16_t convert(int32_t input, DitherGenerator& dither_gen) noexcept
    {
        if (input >= sample_in_max)
        {
            return sample_out_max;
        }
        return static_cast<int16_t>(
            (((input + (rounding + (input >> 31))) << pre_dither_shift) + dither_gen.generate_int16()) >>
            post_dither_shift);
    }
};

template<class DitherGenerator>
constexpr int16_t reference_sample_converter_impl<sample<int24_t>, sample<int16_t>, DitherGenerator>::sample_out_max;

template<class DitherGenerator>
struct reference_sample_converter_impl<sample<int24_t>, sample<int32_t>, DitherGenerator>
{
    static inline int32_t convert(int32_t input, DitherGenerator&) noexcept
    {
        return input << 8;
    }
};

// sample >> 31 results in 0 if sample is positive or 0, and -1 if sample is negative
// this is added to the rounding constant to perform round half away from zero as opposed to just round half up

template<class DitherGenerator>
struct reference_sample_converter_impl<sample<int32_t>, sample<int16_t>, DitherGenerator>
{
    static constexpr int32_t sample_in_max = static_cast<int32_t>(0x7FFF8000);
    static constexpr int16_t sample_out_max = sample_limits<int16_t>::max;
    static constexpr int32_t rounding = static_cast<int32_t>(0x8000);
    static constexpr std::size_t total_shift = 16;
    static constexpr std::size_t pre_dither_shift = total_shift - DitherGenerator::int16_bits;
    static constexpr std::size_t post_dither_shift = total_shift - pre_dither_shift;

    static inline int16_t convert(int32_t input, DitherGenerator& dither_gen) noexcept
    {
        if (input >= sample_in_max)
        {
            return sample_out_max;
        }
        return static_cast<int16_t>(
            (((input + (rounding + (input >> 31))) >> pre_dither_shift) + dither_gen.generate_int16()) >>
            post_dither_shift);
    }
};

template<class DitherGenerator>
constexpr int16_t reference_sample_converter_impl<sample<int32_t>, sample<int16_t>, DitherGenerator>::sample_out_max;

template<class DitherGenerator>
struct reference_sample_converter_impl<sample<int32_t>, sample<int24_t>, DitherGenerator>
{
    static constexpr int32_t sample_in_max = static_cast<int32_t>(0x7FFFFF80);
    static constexpr int24_t sample_out_max = sample_limits<int24_t>::max;
    static constexpr int32_t rounding = static_cast<int32_t>(0x80);

    static inline int24_t convert(int32_t input, DitherGenerator&) noexcept
    {
        if (input >= sample_in_max)
        {
            return sample_out_max;
        }
        return static_cast<int24_t>((input + (rounding + (input >> 31))) >> 8);
    }
};

template<class DitherGenerator>
constexpr int24_t reference_sample_converter_impl<sample<int32_t>, sample<int24_t>, DitherGenerator>::sample_out_max;

template<class SampleValueType, class DitherGenerator>
struct reference_sample_converter_impl<sample<SampleValueType>, sample<float32_t>, DitherGenerator>
{
    static constexpr float32_t scaler = float_convert_traits<SampleValueType>::divisor;

    static inline float32_t convert(SampleValueType input, DitherGenerator&) noexcept
    {
        return static_cast<float32_t>(input) * scaler;
    }
};

// sample >> 31 results in 0 if sample is positive or 0, and -1 if sample is negative
// this is added to the rounding constant to perform round half away from zero as opposed to just round half up

template<class SampleValueType, class DitherGenerator>
struct reference_sample_converter_impl<sample<float32_t>, sample<SampleValueType>, DitherGenerator>
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
    static inline int32_t convert(float32_t input, DitherGenerator& dither_gen) noexcept
    {
        if (RATL_UNLIKELY(input >= sample_in_max))
        {
            return sample_out_max;
        }
        if (RATL_UNLIKELY(input < sample_in_min))
        {
            return sample_out_min;
        }
        return round_float32_to_int32((input * scaler) + dither_gen.generate_float32());
    }
};

template<class SampleValueType, class DitherGenerator>
constexpr SampleValueType
    reference_sample_converter_impl<sample<float32_t>, sample<SampleValueType>, DitherGenerator>::sample_out_max;
template<class SampleValueType, class DitherGenerator>
constexpr SampleValueType
    reference_sample_converter_impl<sample<float32_t>, sample<SampleValueType>, DitherGenerator>::sample_out_min;

// This is required as sample_converter<float32_t, sample_type> is more specific than
// sample_converter<sample_type, sample_type>

template<class DitherGenerator>
struct reference_sample_converter_impl<sample<float32_t>, sample<float32_t>, DitherGenerator>
{
    static inline float32_t convert(float32_t input, DitherGenerator&) noexcept
    {
        return input;
    }
};

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
struct reference_sample_converter_impl<sample<InputSampleType>, network_sample<OutputSampleType>, DitherGenerator>
{
    static inline network_sample_value_type_t<OutputSampleType> convert(
        InputSampleType input, DitherGenerator& dither_gen) noexcept
    {
        return reference_sample_converter_impl<
            sample<OutputSampleType>,
            network_sample<OutputSampleType>,
            DitherGenerator>::
            convert(
                reference_sample_converter_impl<sample<InputSampleType>, sample<OutputSampleType>, DitherGenerator>::
                    convert(input, dither_gen),
                dither_gen);
    }
};

template<class SampleValueType, class DitherGenerator>
struct reference_sample_converter_impl<sample<SampleValueType>, network_sample<SampleValueType>, DitherGenerator>
{
    static inline network_sample_value_type_t<SampleValueType> convert(SampleValueType input, DitherGenerator&) noexcept
    {
        return sample_to_network_sample<SampleValueType>(input);
    }
};

template<class InputSampleType, class OutputSampleType, class DitherGenerator>
struct reference_sample_converter_impl<network_sample<InputSampleType>, sample<OutputSampleType>, DitherGenerator>
{
    static inline OutputSampleType convert(
        network_sample_value_type_t<InputSampleType> input, DitherGenerator& dither_gen) noexcept
    {
        return reference_sample_converter_impl<sample<InputSampleType>, sample<OutputSampleType>, DitherGenerator>::
            convert(
                reference_sample_converter_impl<
                    network_sample<InputSampleType>,
                    sample<InputSampleType>,
                    DitherGenerator>::convert(input, dither_gen),
                dither_gen);
    }
};

template<class SampleValueType, class DitherGenerator>
struct reference_sample_converter_impl<network_sample<SampleValueType>, sample<SampleValueType>, DitherGenerator>
{
    static inline SampleValueType convert(network_sample_value_type_t<SampleValueType> input, DitherGenerator&) noexcept
    {
        return network_sample_to_sample<SampleValueType>(input);
    }
};

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_reference_sample_converter_impl_
