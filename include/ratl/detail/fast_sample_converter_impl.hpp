/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_fast_sample_converter_impl_
#define _ratl_detail_fast_sample_converter_impl_

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
// fast_sample_converter_impl

template<typename InputSampleType, typename OutputSampleType, typename DitherGenerator>
struct fast_sample_converter_impl;

template<typename SampleValueType, typename DitherGenerator>
struct fast_sample_converter_impl<sample<SampleValueType>, sample<SampleValueType>, DitherGenerator>
{
    static const SampleValueType& convert(const SampleValueType& input, DitherGenerator&) noexcept
    {
        return input;
    }
};

template<typename DitherGenerator>
struct fast_sample_converter_impl<sample<int16_t>, sample<int24_t>, DitherGenerator>
{
    static int24_t convert(int32_t input, DitherGenerator&) noexcept
    {
        return static_cast<int24_t>(input << 8);
    }
};

template<typename DitherGenerator>
struct fast_sample_converter_impl<sample<int16_t>, sample<int32_t>, DitherGenerator>
{
    static int32_t convert(int32_t input, DitherGenerator&) noexcept
    {
        return input << 16;
    }
};

template<typename DitherGenerator>
struct fast_sample_converter_impl<sample<int24_t>, sample<int16_t>, DitherGenerator>
{
    static constexpr std::size_t total_shift = 8;
    static constexpr std::size_t pre_dither_shift =
        DitherGenerator::int16_bits > 0 ? DitherGenerator::int16_bits - total_shift : 0;
    static constexpr std::size_t post_dither_shift = total_shift + pre_dither_shift;

    static int16_t convert(int32_t input, DitherGenerator& dither_gen) noexcept
    {
        return static_cast<int24_t>(((input << pre_dither_shift) + dither_gen.generate_int16()) >> post_dither_shift);
    }
};

template<typename DitherGenerator>
struct fast_sample_converter_impl<sample<int24_t>, sample<int32_t>, DitherGenerator>
{
    static int32_t convert(int32_t input, DitherGenerator&) noexcept
    {
        return input << 8;
    }
};

template<typename DitherGenerator>
struct fast_sample_converter_impl<sample<int32_t>, sample<int16_t>, DitherGenerator>
{
    static constexpr std::size_t total_shift = 16;
    static constexpr std::size_t pre_dither_shift = total_shift - DitherGenerator::int16_bits;
    static constexpr std::size_t post_dither_shift = total_shift - pre_dither_shift;

    static int16_t convert(int32_t input, DitherGenerator& dither_gen) noexcept
    {
        return static_cast<int16_t>(((input >> pre_dither_shift) + dither_gen.generate_int16()) >> post_dither_shift);
    }
};

template<typename DitherGenerator>
struct fast_sample_converter_impl<sample<int32_t>, sample<int24_t>, DitherGenerator>
{
    static int24_t convert(int32_t input, DitherGenerator&) noexcept
    {
        return static_cast<int24_t>(input >> 8);
    }
};

template<typename SampleValueType, typename DitherGenerator>
struct fast_sample_converter_impl<sample<SampleValueType>, sample<float32_t>, DitherGenerator>
{
    static constexpr float32_t scaler = float_convert_traits<SampleValueType>::divisor;

    static inline float32_t convert(SampleValueType input, DitherGenerator&) noexcept
    {
        return static_cast<float32_t>(input) * scaler;
    }
};

template<typename SampleValueType, typename DitherGenerator>
struct fast_sample_converter_impl<sample<float32_t>, sample<SampleValueType>, DitherGenerator>
{
private:
    static constexpr float32_t positive_scaler =
        static_cast<float32_t>(sample_limits<SampleValueType>::max) - DitherGenerator::float32_max;
    static constexpr float32_t negative_scaler =
        -static_cast<float32_t>(sample_limits<SampleValueType>::min) - DitherGenerator::float32_max;

public:
    static inline int32_t convert(float32_t input, DitherGenerator& dither_gen) noexcept
    {
        return round_float32_to_int32_fast(
            (input * (input < 0 ? negative_scaler : positive_scaler)) + dither_gen.generate_float32());
    }
};

// This is required as sample_converter<float32_t, sample_type> is more specific than
// sample_converter<sample_type, sample_type>

template<typename DitherGenerator>
struct fast_sample_converter_impl<sample<float32_t>, sample<float32_t>, DitherGenerator>
{
    static inline float32_t convert(float32_t input, DitherGenerator&) noexcept
    {
        return input;
    }
};

template<typename InputSampleType, typename OutputSampleType, typename DitherGenerator>
struct fast_sample_converter_impl<sample<InputSampleType>, network_sample<OutputSampleType>, DitherGenerator>
{
    static inline network_sample_value_type_t<OutputSampleType> convert(
        InputSampleType input, DitherGenerator& dither_gen) noexcept
    {
        return fast_sample_converter_impl<sample<OutputSampleType>, network_sample<OutputSampleType>, DitherGenerator>::
            convert(
                fast_sample_converter_impl<sample<InputSampleType>, sample<OutputSampleType>, DitherGenerator>::convert(
                    input, dither_gen),
                dither_gen);
    }
};

template<typename SampleValueType, typename DitherGenerator>
struct fast_sample_converter_impl<sample<SampleValueType>, network_sample<SampleValueType>, DitherGenerator>
{
    static inline network_sample_value_type_t<SampleValueType> convert(
        SampleValueType sample, DitherGenerator&) noexcept
    {
        return sample_to_network_sample<SampleValueType>(sample);
    }
};

template<typename InputSampleType, typename OutputSampleType, typename DitherGenerator>
struct fast_sample_converter_impl<network_sample<InputSampleType>, sample<OutputSampleType>, DitherGenerator>
{
    static inline OutputSampleType convert(
        network_sample_value_type_t<InputSampleType> input, DitherGenerator& dither_gen) noexcept
    {
        return fast_sample_converter_impl<sample<InputSampleType>, sample<OutputSampleType>, DitherGenerator>::convert(
            fast_sample_converter_impl<network_sample<InputSampleType>, sample<InputSampleType>, DitherGenerator>::
                convert(input, dither_gen),
            dither_gen);
    }
};

template<typename SampleValueType, typename DitherGenerator>
struct fast_sample_converter_impl<network_sample<SampleValueType>, sample<SampleValueType>, DitherGenerator>
{
    static inline SampleValueType convert(network_sample_value_type_t<SampleValueType> input, DitherGenerator&) noexcept
    {
        return network_sample_to_sample<SampleValueType>(input);
    }
};

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_fast_sample_converter_impl_
