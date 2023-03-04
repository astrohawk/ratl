/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_batch_creator_
#define _ratl_detail_batch_creator_

// ratl includes
#include <ratl/detail/batch_alignment.hpp>
#include <ratl/detail/batch_value_traits.hpp>
#include <ratl/detail/cast.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/detail/sample_iterator.hpp>
#include <ratl/detail/xsimd.hpp>
#include <ratl/network_sample.hpp>
#include <ratl/sample.hpp>

// other includes
#include <type_traits>

#if defined(RATL_HAS_XSIMD)

namespace ratl
{
namespace detail
{
template<typename SampleType, std::size_t BatchSize = batch_size>
class batch_creator_sample_converter;

template<typename SampleValueType, std::size_t BatchSize>
class batch_creator_sample_converter<sample<SampleValueType>, BatchSize>
{
public:
    using batch_type = batch_sample_value_type_t<SampleValueType, BatchSize>;

    static inline typename batch_type::value_type convert_input(SampleValueType input)
    {
        return input;
    }

    static inline SampleValueType convert_output(typename batch_type::value_type input)
    {
        return narrowing_cast<SampleValueType>(input);
    }
};

template<typename SampleValueType, std::size_t BatchSize>
class batch_creator_sample_converter<network_sample<SampleValueType>, BatchSize>
{
public:
    using batch_type = batch_network_sample_value_type_t<SampleValueType, BatchSize>;

    static inline typename batch_type::value_type convert_input(network_sample_value_type_t<SampleValueType> input)
    {
        return network_to_network_underlying_cast<SampleValueType>(input);
    }

    static inline network_sample_value_type_t<SampleValueType> convert_output(typename batch_type::value_type input)
    {
        return network_underlying_to_network_cast<SampleValueType>(
            narrowing_cast<network_sample_value_underlying_type_t<SampleValueType>>(input));
    }
};

template<typename SampleType, std::size_t BatchSize>
class base_batch_creator
{
    using sample_converter = batch_creator_sample_converter<SampleType, BatchSize>;

public:
    using batch_type = typename sample_converter::batch_type;
    using alignment_dispatcher = batch_alignment_dispatcher<SampleType, batch_type>;

    template<typename SampleIterator, typename AlignmentMode>
    static inline batch_type load(SampleIterator input, AlignmentMode) noexcept
    {
        return load_impl(input, std::make_index_sequence<BatchSize>(), std::make_index_sequence<0>());
    }

    template<typename SampleIterator, typename AlignmentMode>
    static inline batch_type load(SampleIterator input, std::size_t size, AlignmentMode) noexcept
    {
        return partial_load(input, size, std::make_index_sequence<BatchSize>());
    }

    template<typename SampleIterator, typename AlignmentMode>
    static inline void store(const batch_type& input, SampleIterator output, AlignmentMode) noexcept
    {
        store_impl(input, output, std::make_index_sequence<BatchSize>());
    }

    template<typename SampleIterator, typename AlignmentMode>
    static inline void store(const batch_type& input, SampleIterator output, std::size_t size, AlignmentMode) noexcept
    {
        partial_store(input, output, size, std::make_index_sequence<BatchSize>());
    }

private:
    template<typename SampleIterator, std::size_t... I>
    static inline batch_type partial_load(SampleIterator input, std::size_t size, std::index_sequence<I...>) noexcept
    {
        using partial_load_impl_fn = batch_type (*)(SampleIterator);
        static constexpr partial_load_impl_fn partial_load_impl_fns[] = {&partial_load_impl<SampleIterator, I>...};
        return (*partial_load_impl_fns[size])(input);
    }

    template<typename SampleIterator, std::size_t Size>
    static inline batch_type partial_load_impl(SampleIterator input) noexcept
    {
        return load_impl(input, std::make_index_sequence<Size>(), std::make_index_sequence<BatchSize - Size>());
    }

    template<typename SampleIterator, std::size_t... I, std::size_t... J>
    static inline batch_type load_impl(
        SampleIterator input, std::index_sequence<I...>, std::index_sequence<J...>) noexcept
    {
        static_assert(sizeof...(I) + sizeof...(J) == BatchSize, "");
        (void)input; // stops gcc from complaining due to input not being used when I is 0
        return batch_type(sample_converter::convert_input(input[I].get())..., null_input(J)...);
    }

    static inline typename batch_type::value_type null_input(std::size_t)
    {
        return {};
    }

    template<typename SampleIterator, std::size_t... I>
    static inline void partial_store(
        const batch_type& input, SampleIterator output, std::size_t size, std::index_sequence<I...>) noexcept
    {
        using partial_store_impl_fn = void (*)(const batch_type&, SampleIterator);
        static constexpr partial_store_impl_fn partial_store_impl_fns[] = {&partial_store_impl<SampleIterator, I>...};
        return (*partial_store_impl_fns[size])(input, output);
    }

    template<typename SampleIterator, std::size_t Size>
    static inline void partial_store_impl(const batch_type& input, SampleIterator output) noexcept
    {
        store_impl(input, output, std::make_index_sequence<Size>());
    }

    template<typename SampleIterator, std::size_t... I>
    static inline void store_impl(const batch_type& input, SampleIterator output, std::index_sequence<I...>) noexcept
    {
        (void)output; // stops gcc from complaining due to output not being used when I is 0
        alignas(batch_alignment<batch_type>) typename batch_type::value_type buffer[BatchSize];
        input.store_aligned(buffer);
#    if defined(RATL_CPP_VERSION_HAS_CPP17)
        (store_element<I>(buffer, output), ...);
#    else
        int fake[] = {(store_element<I>(buffer, output), 0)..., 0};
        (void)fake;
#    endif
    }

    template<std::size_t I, typename SampleIterator>
    static inline void store_element(const typename batch_type::value_type* buffer, SampleIterator output) noexcept
    {
        output[I].get() = sample_converter::convert_output(buffer[I]);
    }
};

template<typename SampleType, std::size_t BatchSize = batch_size, typename = void>
class batch_creator : public base_batch_creator<SampleType, BatchSize>
{
};

// The load and store overloads for the specific sample types technically rely on undefined behaviour when performing
// the reinterpret_cast but this is a risk we are currently willing to take in order to get the corresponding
// performance boost

template<typename SampleType, std::size_t BatchSize>
class batch_creator<
    SampleType,
    BatchSize,
    std::enable_if_t<has_batch_type<sample_underlying_type_t<SampleType>, BatchSize>::value>> :
    public base_batch_creator<SampleType, BatchSize>
{
    using base = typename batch_creator::base_batch_creator;

public:
    using batch_type = typename base::batch_type;

    template<typename SampleIterator>
    static inline batch_type load(SampleIterator input, batch_alignment_mode::aligned) noexcept
    {
        return batch_type(
            reinterpret_cast<sample_iterator_underlying_type_t<SampleIterator>*>(input.base()), xsimd::aligned_mode());
    }

    template<typename SampleIterator>
    static inline batch_type load(SampleIterator input, batch_alignment_mode::unaligned) noexcept
    {
        return batch_type(
            reinterpret_cast<sample_iterator_underlying_type_t<SampleIterator>*>(input.base()),
            xsimd::unaligned_mode());
    }

    using base::load;

    template<typename SampleIterator>
    static inline void store(const batch_type& input, SampleIterator output, batch_alignment_mode::aligned) noexcept
    {
        input.store_aligned(reinterpret_cast<sample_iterator_underlying_type_t<SampleIterator>*>(output.base()));
    }

    template<typename SampleIterator>
    static inline void store(const batch_type& input, SampleIterator output, batch_alignment_mode::unaligned) noexcept
    {
        input.store_unaligned(reinterpret_cast<sample_iterator_underlying_type_t<SampleIterator>*>(output.base()));
    }

    using base::store;
};

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_detail_batch_creator_
