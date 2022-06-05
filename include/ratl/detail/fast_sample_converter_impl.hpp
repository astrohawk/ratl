/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_fast_sample_converter_impl_
#define _ratl_detail_fast_sample_converter_impl_

// ratl includes
#include <ratl/detail/cast.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/detail/convert_traits.hpp>
#include <ratl/detail/endianness.hpp>
#include <ratl/detail/round.hpp>
#include <ratl/network_sample.hpp>
#include <ratl/sample.hpp>

// other includes
#include <cfenv>
#include <cmath>
#include <type_traits>

namespace ratl
{
namespace detail
{
// fast_sample_converter_impl

template<typename InputSampleType, typename OutputSampleType, typename DitherGenerator, typename = void>
struct fast_sample_converter_impl;

template<typename SampleValueType, typename DitherGenerator>
struct fast_sample_converter_impl<sample<SampleValueType>, sample<SampleValueType>, DitherGenerator>
{
    static inline const SampleValueType& convert(const SampleValueType& input, DitherGenerator&) noexcept
    {
        return input;
    }
};

template<typename DitherGenerator>
struct fast_sample_converter_impl<sample<int16_t>, sample<int24_t>, DitherGenerator>
{
    static inline int24_t convert(int32_t input, DitherGenerator&) noexcept
    {
        return narrowing_cast<int24_t>(input << 8);
    }
};

template<typename DitherGenerator>
struct fast_sample_converter_impl<sample<int16_t>, sample<int32_t>, DitherGenerator>
{
    static inline int32_t convert(int32_t input, DitherGenerator&) noexcept
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

    static inline int16_t convert(int32_t input, DitherGenerator& dither_gen) noexcept
    {
        return narrowing_cast<int16_t>(
            ((input << pre_dither_shift) + dither_gen.generate_int16()) >> post_dither_shift);
    }
};

template<typename DitherGenerator>
struct fast_sample_converter_impl<sample<int24_t>, sample<int32_t>, DitherGenerator>
{
    static inline int32_t convert(int32_t input, DitherGenerator&) noexcept
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

    static inline int16_t convert(int32_t input, DitherGenerator& dither_gen) noexcept
    {
        return narrowing_cast<int16_t>(
            ((input >> pre_dither_shift) + dither_gen.generate_int16()) >> post_dither_shift);
    }
};

template<typename DitherGenerator>
struct fast_sample_converter_impl<sample<int32_t>, sample<int24_t>, DitherGenerator>
{
    static inline int24_t convert(int32_t input, DitherGenerator&) noexcept
    {
        return narrowing_cast<int24_t>(input >> 8);
    }
};

template<typename SampleValueType, typename DitherGenerator>
struct fast_sample_converter_impl<
    sample<SampleValueType>,
    sample<float32_t>,
    DitherGenerator,
    std::enable_if_t<sample_limits<SampleValueType>::is_integer>>
{
    static constexpr float32_t scaler = asymmetric_float_convert_traits<SampleValueType>::int_to_float_scaler;

    static inline float32_t convert(SampleValueType input, DitherGenerator&) noexcept
    {
        return static_cast<float32_t>(input) * scaler;
    }
};

template<typename SampleValueType, typename DitherGenerator>
struct fast_sample_converter_impl<
    sample<float32_t>,
    sample<SampleValueType>,
    DitherGenerator,
    std::enable_if_t<sample_limits<SampleValueType>::is_integer>>
{
private:
    static constexpr float32_t scaler =
        asymmetric_float_convert_traits<SampleValueType>::float_to_int_scaler - DitherGenerator::float32_max;

public:
    static inline SampleValueType convert(float32_t input, DitherGenerator& dither_gen) noexcept
    {
        return narrowing_cast<SampleValueType>(
            round_float32_to_int32_fast((input * scaler) + dither_gen.generate_float32()));
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
