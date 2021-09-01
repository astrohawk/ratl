/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_batch_creator_
#define _ratl_detail_batch_creator_

// ratl includes
#include <ratl/detail/batch_traits.hpp>
#include <ratl/detail/config.hpp>
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
template<typename SampleType>
class base_batch_creator;

template<typename SampleValueType>
class base_batch_creator<sample<SampleValueType>>
{
public:
    using batch_type = batch_sample_value_type_t<SampleValueType>;

    template<typename Iterator>
    static inline batch_type load(Iterator input) noexcept
    {
        return load_impl(input, std::make_index_sequence<batch_size>());
    }

    template<typename Iterator>
    static inline void store(const batch_type& input, Iterator output) noexcept
    {
        store_impl(input, output, std::make_index_sequence<batch_size>());
    }

private:
    template<typename Iterator, std::size_t... I>
    static inline batch_type load_impl(Iterator input, std::index_sequence<I...>) noexcept
    {
        return batch_type(convert_input(input[I].get())...);
    }

    static inline typename batch_type::value_type convert_input(SampleValueType input)
    {
        return input;
    }

    template<typename Iterator, std::size_t... I>
    static inline void store_impl(const batch_type& input, Iterator output, std::index_sequence<I...>) noexcept
    {
#    if defined(RATL_CPP_VERSION_HAS_CPP17)
        (store_element<I>(input, output), ...);
#    else
        int fake[] = {(store_element<I>(input, output), 0)...};
        (void)fake;
#    endif
    }

    template<std::size_t I, typename Iterator>
    static inline void store_element(const batch_type& input, Iterator output) noexcept
    {
        output[I].get() = convert_output(input[I]);
    }

    static inline SampleValueType convert_output(typename batch_type::value_type input)
    {
        return narrowing_cast<SampleValueType>(input);
    }
};

template<typename SampleValueType>
class base_batch_creator<network_sample<SampleValueType>>
{
public:
    using batch_type = batch_network_sample_value_type_t<SampleValueType>;

    template<typename Iterator>
    static inline batch_type load(Iterator input) noexcept
    {
        return load_impl(input, std::make_index_sequence<batch_size>());
    }

    template<typename Iterator>
    static inline void store(const batch_type& input, Iterator output) noexcept
    {
        store_impl(input, output, std::make_index_sequence<batch_size>());
    }

private:
    template<typename Iterator, std::size_t... I>
    static inline batch_type load_impl(Iterator input, std::index_sequence<I...>) noexcept
    {
        return batch_type(convert_input(input[I].get())...);
    }

    static inline typename batch_type::value_type convert_input(network_sample_value_type_t<SampleValueType> input)
    {
        return network_to_network_underlying_cast<SampleValueType>(input);
    }

    template<typename Iterator, std::size_t... I>
    static inline void store_impl(const batch_type& input, Iterator output, std::index_sequence<I...>) noexcept
    {
#    if defined(RATL_CPP_VERSION_HAS_CPP17)
        (store_element<I>(input, output), ...);
#    else
        int fake[] = {(store_element<I>(input, output), 0)...};
        (void)fake;
#    endif
    }

    template<std::size_t I, typename Iterator>
    static inline void store_element(const batch_type& input, Iterator output) noexcept
    {
        output[I].get() = convert_output(input[I]);
    }

    static inline network_sample_value_type_t<SampleValueType> convert_output(typename batch_type::value_type input)
    {
        return network_underlying_to_network_cast<SampleValueType>(
            narrowing_cast<network_sample_value_underlying_type_t<SampleValueType>>(input));
    }
};

template<typename SampleType, typename = void>
struct batch_creator : public base_batch_creator<SampleType>
{
};

// The load and store overloads for the specific sample types technically rely on undefined
// behaviour when performing the reinterpret_cast but this is a risk we are currently willing to take in order to get
// the corresponding performance boost

template<typename SampleValueType>
class batch_creator<sample<SampleValueType>, typename std::enable_if<has_batch_type_v<SampleValueType>>::type> :
    public base_batch_creator<sample<SampleValueType>>
{
    using base = typename batch_creator::base_batch_creator;

public:
    using batch_type = typename base::batch_type;

    using base::load;
    using base::store;

    static inline batch_type load(sample<SampleValueType>* input) noexcept
    {
        return batch_type(reinterpret_cast<SampleValueType*>(input), xsimd::unaligned_mode());
    }

    static inline batch_type load(const sample<SampleValueType>* input) noexcept
    {
        return batch_type(reinterpret_cast<const SampleValueType*>(input), xsimd::unaligned_mode());
    }

    static inline void store(const batch_type& input, sample<SampleValueType>* output) noexcept
    {
        input.store_unaligned(reinterpret_cast<SampleValueType*>(output));
    }
};

template<typename SampleValueType>
class batch_creator<
    network_sample<SampleValueType>,
    typename std::enable_if<has_batch_type_v<network_sample_value_underlying_type_t<SampleValueType>>>::type> :
    public base_batch_creator<network_sample<SampleValueType>>
{
    using base = typename batch_creator::base_batch_creator;

public:
    using batch_type = typename base::batch_type;

    using base::load;
    using base::store;

    static inline batch_type load(network_sample<SampleValueType>* input) noexcept
    {
        return batch_type(
            reinterpret_cast<network_sample_value_underlying_type_t<SampleValueType>*>(input), xsimd::unaligned_mode());
    }

    static inline batch_type load(const network_sample<SampleValueType>* input) noexcept
    {
        return batch_type(
            reinterpret_cast<const network_sample_value_underlying_type_t<SampleValueType>*>(input),
            xsimd::unaligned_mode());
    }

    static inline void store(const batch_type& input, network_sample<SampleValueType>* output) noexcept
    {
        input.store_unaligned(reinterpret_cast<network_sample_value_underlying_type_t<SampleValueType>*>(output));
    }
};

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_detail_batch_creator_
