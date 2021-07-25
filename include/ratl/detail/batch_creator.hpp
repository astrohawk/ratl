#ifndef _ratl_detail_batch_creator_
#define _ratl_detail_batch_creator_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/detail/batch_traits.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/detail/xsimd.hpp>
#include <ratl/network_sample.hpp>
#include <ratl/sample.hpp>

#if defined(RATL_HAS_XSIMD)

namespace ratl
{
namespace detail
{
template<class Sample>
struct BaseBatchCreator;

template<class SampleType>
struct BaseBatchCreator<Sample<SampleType>>
{
    using batch_type = BatchSampleValueType_t<SampleType>;

private:
    static typename batch_type::value_type convert_input(SampleType input)
    {
        return static_cast<typename batch_type::value_type>(input);
    }

    static SampleType convert_output(typename batch_type::value_type input)
    {
        return static_cast<SampleType>(input);
    }

    template<typename Iterator, std::size_t... I>
    static inline batch_type load_unaligned_impl(Iterator input, std::index_sequence<I...>) noexcept
    {
        return batch_type{convert_input(input[I].get())...};
    }

    template<std::size_t I, typename Iterator>
    static inline void store_unaligned_element(const batch_type& input, Iterator output) noexcept
    {
        output[I].get() = convert_output(input[I]);
    }

    template<typename Iterator, std::size_t... I>
    static inline void store_unaligned_impl(
        const batch_type& input, Iterator output, std::index_sequence<I...>) noexcept
    {
#    if defined(RATL_CPP_VERSION_HAS_CPP17)
        (store_unaligned_element<I>(input, output), ...);
#    else
        int fake[] = {(store_unaligned_element<I>(input, output), 0)...};
        (void)fake;
#    endif
    }

public:
    template<typename Iterator>
    static inline batch_type load_unaligned(Iterator input) noexcept
    {
        return load_unaligned_impl(input, std::make_index_sequence<BatchSize>{});
    }

    template<typename Iterator>
    static inline void store_unaligned(const batch_type& input, Iterator output) noexcept
    {
        store_unaligned_impl(input, output, std::make_index_sequence<BatchSize>{});
    }
};

template<class SampleType>
struct BaseBatchCreator<NetworkSample<SampleType>>
{
    using batch_type = BatchNetworkSampleValueType_t<SampleType>;

private:
    static typename batch_type::value_type convert_input(NetworkSampleValueUnderlyingType_t<SampleType> input)
    {
        return static_cast<typename batch_type::value_type>(input);
    }

    static NetworkSampleValueUnderlyingType_t<SampleType> convert_output(typename batch_type::value_type input)
    {
        return static_cast<NetworkSampleValueUnderlyingType_t<SampleType>>(input);
    }

    template<typename Iterator, std::size_t... I>
    static inline batch_type load_unaligned_impl(Iterator input, std::index_sequence<I...>) noexcept
    {
        return batch_type{convert_input(networkToNetworkUnderlyingCast<SampleType>(input[I].get()))...};
    }

    template<std::size_t I, typename Iterator>
    static inline void store_unaligned_element(const batch_type& input, Iterator output) noexcept
    {
        output[I].get() = networkUnderlyingToNetworkCast<SampleType>(convert_output(input[I]));
    }

    template<typename Iterator, std::size_t... I>
    static inline void store_unaligned_impl(
        const batch_type& input, Iterator output, std::index_sequence<I...>) noexcept
    {
#    if defined(RATL_CPP_VERSION_HAS_CPP17)
        (store_unaligned_element<I>(input, output), ...);
#    else
        int fake[] = {(store_unaligned_element<I>(input, output), 0)...};
        (void)fake;
#    endif
    }

public:
    template<typename Iterator>
    static inline batch_type load_unaligned(Iterator input) noexcept
    {
        return load_unaligned_impl(input, std::make_index_sequence<BatchSize>{});
    }

    template<typename Iterator>
    static inline void store_unaligned(const batch_type& input, Iterator output) noexcept
    {
        store_unaligned_impl(input, output, std::make_index_sequence<BatchSize>{});
    }
};

template<class Sample, class = void>
struct BatchCreator : public BaseBatchCreator<Sample>
{
};

// The load_unaligned and store_unaligned overloads for the specific sample types technically rely on undefined
// behaviour when performing the reinterpret_cast but this is a price we are currently willing to pay in order to get
// the corresponding performance boost

template<class SampleType>
struct BatchCreator<Sample<SampleType>, typename std::enable_if<HasBatchType_v<SampleType>>::type> :
    public BaseBatchCreator<Sample<SampleType>>
{
    using base = typename BatchCreator::BaseBatchCreator;
    using batch_type = typename base::batch_type;

    using base::load_unaligned;
    using base::store_unaligned;

    static inline batch_type load_unaligned(Sample<SampleType>* input) noexcept
    {
        return batch_type(reinterpret_cast<SampleType*>(input), xsimd::unaligned_mode{});
    }

    static inline batch_type load_unaligned(const Sample<SampleType>* input) noexcept
    {
        return batch_type(reinterpret_cast<const SampleType*>(input), xsimd::unaligned_mode{});
    }

    static inline void store_unaligned(const batch_type& input, Sample<SampleType>* output) noexcept
    {
        input.store_unaligned(reinterpret_cast<SampleType*>(output));
    }
};

template<class SampleType>
struct BatchCreator<
    NetworkSample<SampleType>,
    typename std::enable_if<HasBatchType_v<NetworkSampleValueUnderlyingType_t<SampleType>>>::type> :
    public BaseBatchCreator<NetworkSample<SampleType>>
{
    using base = typename BatchCreator::BaseBatchCreator;
    using batch_type = typename base::batch_type;

    using base::load_unaligned;
    using base::store_unaligned;

    static inline batch_type load_unaligned(NetworkSample<SampleType>* input) noexcept
    {
        return batch_type(
            reinterpret_cast<NetworkSampleValueUnderlyingType_t<SampleType>*>(input), xsimd::unaligned_mode{});
    }

    static inline batch_type load_unaligned(const NetworkSample<SampleType>* input) noexcept
    {
        return batch_type(
            reinterpret_cast<const NetworkSampleValueUnderlyingType_t<SampleType>*>(input), xsimd::unaligned_mode{});
    }

    static inline void store_unaligned(const batch_type& input, NetworkSample<SampleType>* output) noexcept
    {
        input.store_unaligned(reinterpret_cast<NetworkSampleValueUnderlyingType_t<SampleType>*>(output));
    }
};

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_detail_batch_creator_
