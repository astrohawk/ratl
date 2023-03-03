/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_batch_fast_sample_converter_impl_
#define _ratl_detail_batch_fast_sample_converter_impl_

// ratl includes
#include <ratl/detail/batch_cast.hpp>
#include <ratl/detail/batch_endianness.hpp>
#include <ratl/detail/batch_round.hpp>
#include <ratl/detail/batch_traits.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/detail/convert_traits.hpp>
#include <ratl/detail/fast_sample_converter_impl.hpp>

// other includes
#include <cmath>
#include <type_traits>

namespace ratl
{
namespace detail
{
#if defined(RATL_HAS_XSIMD)

// base_batch_fast_sample_converter_impl

template<typename InputSampleType, typename OutputSampleType, typename DitherGenerator, typename = void>
struct base_batch_fast_sample_converter_impl;

template<typename SampleValueType, typename DitherGenerator>
struct base_batch_fast_sample_converter_impl<sample<SampleValueType>, sample<SampleValueType>, DitherGenerator>
{
    static inline const batch_sample_value_type_t<SampleValueType>& batch_convert(
        const batch_sample_value_type_t<SampleValueType>& input, DitherGenerator&) noexcept
    {
        return input;
    }
};

template<typename DitherGenerator>
struct base_batch_fast_sample_converter_impl<sample<int16_t>, sample<int24_t>, DitherGenerator>
{
    static inline batch_sample_value_type_t<int24_t> batch_convert(
        const batch_sample_value_type_t<int16_t>& input, DitherGenerator&) noexcept
    {
        return batch_sample_cast<int24_t>(input) << 8;
    }
};

template<typename DitherGenerator>
struct base_batch_fast_sample_converter_impl<sample<int16_t>, sample<int32_t>, DitherGenerator>
{
    static inline batch_sample_value_type_t<int32_t> batch_convert(
        const batch_sample_value_type_t<int16_t>& input, DitherGenerator&) noexcept
    {
        return batch_sample_cast<int32_t>(input) << 16;
    }
};

template<typename DitherGenerator>
struct base_batch_fast_sample_converter_impl<sample<int24_t>, sample<int16_t>, DitherGenerator>
{
private:
    static constexpr std::size_t total_shift = 8;
    static constexpr std::size_t pre_dither_shift =
        DitherGenerator::int16_bits > 0 ? DitherGenerator::int16_bits - total_shift : 0;
    static constexpr std::size_t post_dither_shift = total_shift + pre_dither_shift;

public:
    static inline batch_sample_value_type_t<int16_t> batch_convert(
        const batch_sample_value_type_t<int24_t>& input, DitherGenerator& dither_gen) noexcept
    {
        return batch_sample_cast<int16_t>(
            ((input << pre_dither_shift) + dither_gen.generate_batch_int16()) >> post_dither_shift);
    }
};

#    if !defined(RATL_CPP_VERSION_HAS_CPP17)
template<typename DitherGenerator>
constexpr std::size_t
    base_batch_fast_sample_converter_impl<sample<int24_t>, sample<int16_t>, DitherGenerator>::total_shift;
template<typename DitherGenerator>
constexpr std::size_t
    base_batch_fast_sample_converter_impl<sample<int24_t>, sample<int16_t>, DitherGenerator>::pre_dither_shift;
template<typename DitherGenerator>
constexpr std::size_t
    base_batch_fast_sample_converter_impl<sample<int24_t>, sample<int16_t>, DitherGenerator>::post_dither_shift;
#    endif

template<typename DitherGenerator>
struct base_batch_fast_sample_converter_impl<sample<int24_t>, sample<int32_t>, DitherGenerator>
{
    static inline batch_sample_value_type_t<int32_t> batch_convert(
        const batch_sample_value_type_t<int24_t>& input, DitherGenerator&) noexcept
    {
        return batch_sample_cast<int24_t>(input << 8);
    }
};

template<typename DitherGenerator>
struct base_batch_fast_sample_converter_impl<sample<int32_t>, sample<int16_t>, DitherGenerator>
{
private:
    static constexpr std::size_t total_shift = 16;
    static constexpr std::size_t pre_dither_shift = total_shift - DitherGenerator::int16_bits;
    static constexpr std::size_t post_dither_shift = total_shift - pre_dither_shift;

public:
    static inline batch_sample_value_type_t<int16_t> batch_convert(
        const batch_sample_value_type_t<int32_t>& input, DitherGenerator& dither_gen) noexcept
    {
        return batch_sample_cast<int16_t>(
            ((input >> pre_dither_shift) + dither_gen.generate_batch_int16()) >> post_dither_shift);
    }
};

#    if !defined(RATL_CPP_VERSION_HAS_CPP17)
template<typename DitherGenerator>
constexpr std::size_t
    base_batch_fast_sample_converter_impl<sample<int32_t>, sample<int16_t>, DitherGenerator>::total_shift;
template<typename DitherGenerator>
constexpr std::size_t
    base_batch_fast_sample_converter_impl<sample<int32_t>, sample<int16_t>, DitherGenerator>::pre_dither_shift;
template<typename DitherGenerator>
constexpr std::size_t
    base_batch_fast_sample_converter_impl<sample<int32_t>, sample<int16_t>, DitherGenerator>::post_dither_shift;
#    endif

template<typename DitherGenerator>
struct base_batch_fast_sample_converter_impl<sample<int32_t>, sample<int24_t>, DitherGenerator>
{
    static inline batch_sample_value_type_t<int24_t> batch_convert(
        const batch_sample_value_type_t<int32_t>& input, DitherGenerator&) noexcept
    {
        return batch_sample_cast<int24_t>(input >> 8);
    }
};

template<typename SampleValueType, typename DitherGenerator>
struct base_batch_fast_sample_converter_impl<
    sample<SampleValueType>,
    sample<float32_t>,
    DitherGenerator,
    std::enable_if_t<sample_limits<SampleValueType>::is_integer>>
{
private:
    static constexpr float32_t scaler = asymmetric_float_convert_traits<SampleValueType>::int_to_float_scaler;

public:
    static inline batch_sample_value_type_t<float32_t> batch_convert(
        const batch_sample_value_type_t<SampleValueType>& input, DitherGenerator&) noexcept
    {
        return xsimd::to_float(batch_sample_cast<int32_t>(input)) * scaler;
    }
};

#    if !defined(RATL_CPP_VERSION_HAS_CPP17)
template<typename SampleValueType, typename DitherGenerator>
constexpr float32_t base_batch_fast_sample_converter_impl<
    sample<SampleValueType>,
    sample<float32_t>,
    DitherGenerator,
    std::enable_if_t<sample_limits<SampleValueType>::is_integer>>::scaler;
#    endif

template<typename SampleValueType, typename DitherGenerator>
struct base_batch_fast_sample_converter_impl<
    sample<float32_t>,
    sample<SampleValueType>,
    DitherGenerator,
    std::enable_if_t<sample_limits<SampleValueType>::is_integer>>
{
private:
    static constexpr float32_t scaler =
        asymmetric_float_convert_traits<SampleValueType>::float_to_int_scaler - DitherGenerator::float32_max;

public:
    static inline batch_sample_value_type_t<SampleValueType> batch_convert(
        batch_sample_value_type_t<float32_t> input, DitherGenerator& dither_gen) noexcept
    {
        return batch_sample_cast<SampleValueType>(
            batch_round<int32_t>((input * scaler) + dither_gen.generate_batch_float32()));
    }
};

#    if !defined(RATL_CPP_VERSION_HAS_CPP17)
template<typename SampleValueType, typename DitherGenerator>
constexpr float32_t base_batch_fast_sample_converter_impl<
    sample<float32_t>,
    sample<SampleValueType>,
    DitherGenerator,
    std::enable_if_t<sample_limits<SampleValueType>::is_integer>>::scaler;
#    endif

template<typename InputSampleType, typename OutputSampleType, typename DitherGenerator>
struct base_batch_fast_sample_converter_impl<sample<InputSampleType>, network_sample<OutputSampleType>, DitherGenerator>
{
    static inline batch_network_sample_value_type_t<OutputSampleType> batch_convert(
        const batch_sample_value_type_t<InputSampleType>& input, DitherGenerator& dither_gen) noexcept
    {
        return base_batch_fast_sample_converter_impl<
            sample<OutputSampleType>,
            network_sample<OutputSampleType>,
            DitherGenerator>::
            batch_convert(
                base_batch_fast_sample_converter_impl<
                    sample<InputSampleType>,
                    sample<OutputSampleType>,
                    DitherGenerator>::batch_convert(input, dither_gen),
                dither_gen);
    }
};

template<typename SampleValueType, typename DitherGenerator>
struct base_batch_fast_sample_converter_impl<sample<SampleValueType>, network_sample<SampleValueType>, DitherGenerator>
{
    static inline batch_network_sample_value_type_t<SampleValueType> batch_convert(
        const batch_sample_value_type_t<SampleValueType>& input, DitherGenerator&) noexcept
    {
        return batch_sample_to_network_sample<SampleValueType>(input);
    }
};

template<typename InputSampleType, typename OutputSampleType, typename DitherGenerator>
struct base_batch_fast_sample_converter_impl<network_sample<InputSampleType>, sample<OutputSampleType>, DitherGenerator>
{
    static inline batch_sample_value_type_t<OutputSampleType> batch_convert(
        const batch_network_sample_value_type_t<InputSampleType>& input, DitherGenerator& dither_gen) noexcept
    {
        return base_batch_fast_sample_converter_impl<
            sample<InputSampleType>,
            sample<OutputSampleType>,
            DitherGenerator>::
            batch_convert(
                base_batch_fast_sample_converter_impl<
                    network_sample<InputSampleType>,
                    sample<InputSampleType>,
                    DitherGenerator>::batch_convert(input, dither_gen),
                dither_gen);
    }
};

template<typename SampleValueType, typename DitherGenerator>
struct base_batch_fast_sample_converter_impl<network_sample<SampleValueType>, sample<SampleValueType>, DitherGenerator>
{
    static inline batch_sample_value_type_t<SampleValueType> batch_convert(
        const batch_network_sample_value_type_t<SampleValueType>& input, DitherGenerator&) noexcept
    {
        return batch_network_sample_to_sample<SampleValueType>(input);
    }
};

// batch_fast_sample_converter_impl

template<typename InputSampleType, typename OutputSampleType, typename DitherGenerator>
struct batch_fast_sample_converter_impl :
    base_batch_fast_sample_converter_impl<InputSampleType, OutputSampleType, DitherGenerator>,
    fast_sample_converter_impl<InputSampleType, OutputSampleType, DitherGenerator>
{
};

#else

template<typename InputSampleType, typename OutputSampleType, typename DitherGenerator>
using batch_fast_sample_converter_impl = fast_sample_converter_impl<InputSampleType, OutputSampleType, DitherGenerator>;

#endif

} // namespace detail
} // namespace ratl

#endif // _ratl_detail_batch_fast_sample_converter_impl_
