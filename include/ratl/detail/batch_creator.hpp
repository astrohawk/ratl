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
    template<typename Iterator, std::size_t... I>
    static inline batch_type load_unaligned_impl(Iterator input, std::index_sequence<I...>) noexcept
    {
        return batch_type{input[I].get()...};
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
        for (std::size_t i = 0; i < BatchSize; ++i)
        {
            output[i].get() = static_cast<SampleType>(input[i]);
        }
    }
};

template<class SampleType>
struct BaseBatchCreator<NetworkSample<SampleType>>
{
    using batch_type = BatchNetworkSampleValueType_t<SampleType>;

private:
    template<typename Iterator, std::size_t... I>
    static inline batch_type load_unaligned_impl(Iterator input, std::index_sequence<I...>) noexcept
    {
        return batch_type{networkToNetworkUnderlyingCast<SampleType>(input[I].get())...};
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
        for (std::size_t i = 0; i < BatchSize; ++i)
        {
            output[i].get() = networkUnderlyingToNetworkCast<SampleType>(
                static_cast<NetworkSampleValueUnderlyingType_t<SampleType>>(input[i]));
        }
    }
};

template<class Sample>
struct BatchCreator : public BaseBatchCreator<Sample>
{
};

// The load_unaligned and store_unaligned overloads for the specific sample types technically rely on undefined
// behaviour when performing the reinterpret_cast but this is a price we are currently willing to pay in order to get
// the corresponding performance boost

template<>
struct BatchCreator<Sample<int16_t>> : public BaseBatchCreator<Sample<int16_t>>
{
    using base = typename BatchCreator::BaseBatchCreator;
    using batch_type = base::batch_type;

    using base::load_unaligned;
    using base::store_unaligned;

    static inline batch_type load_unaligned(Sample<int16_t>* input) noexcept
    {
        return batch_type(reinterpret_cast<int16_t*>(input), xsimd::unaligned_mode{});
    }

    static inline batch_type load_unaligned(const Sample<int16_t>* input) noexcept
    {
        return batch_type(reinterpret_cast<const int16_t*>(input), xsimd::unaligned_mode{});
    }

    static inline void store_unaligned(const batch_type& input, Sample<int16_t>* output) noexcept
    {
        input.store_unaligned(reinterpret_cast<int16_t*>(output));
    }
};

template<>
struct BatchCreator<Sample<int32_t>> : public BaseBatchCreator<Sample<int32_t>>
{
    using base = typename BatchCreator::BaseBatchCreator;
    using batch_type = base::batch_type;

    using base::load_unaligned;
    using base::store_unaligned;

    static inline batch_type load_unaligned(Sample<int32_t>* input) noexcept
    {
        return batch_type(reinterpret_cast<int32_t*>(input), xsimd::unaligned_mode{});
    }

    static inline batch_type load_unaligned(const Sample<int32_t>* input) noexcept
    {
        return batch_type(reinterpret_cast<const int32_t*>(input), xsimd::unaligned_mode{});
    }

    static inline void store_unaligned(const batch_type& input, Sample<int32_t>* output) noexcept
    {
        input.store_unaligned(reinterpret_cast<int32_t*>(output));
    }
};

template<>
struct BatchCreator<Sample<float32_t>> : public BaseBatchCreator<Sample<float32_t>>
{
    using base = typename BatchCreator::BaseBatchCreator;
    using batch_type = base::batch_type;

    using base::load_unaligned;
    using base::store_unaligned;

    static inline batch_type load_unaligned(Sample<float32_t>* input) noexcept
    {
        return batch_type(reinterpret_cast<float32_t*>(input), xsimd::unaligned_mode{});
    }

    static inline batch_type load_unaligned(const Sample<float32_t>* input) noexcept
    {
        return batch_type(reinterpret_cast<const float32_t*>(input), xsimd::unaligned_mode{});
    }

    static inline void store_unaligned(const batch_type& input, Sample<float32_t>* output) noexcept
    {
        input.store_unaligned(reinterpret_cast<float32_t*>(output));
    }
};

template<>
struct BatchCreator<NetworkSample<int16_t>> : public BaseBatchCreator<NetworkSample<int16_t>>
{
    using base = typename BatchCreator::BaseBatchCreator;
    using batch_type = base::batch_type;

    using base::load_unaligned;
    using base::store_unaligned;

    static inline batch_type load_unaligned(NetworkSample<int16_t>* input) noexcept
    {
        return batch_type(
            reinterpret_cast<NetworkSampleValueUnderlyingType_t<int16_t>*>(input), xsimd::unaligned_mode{});
    }

    static inline batch_type load_unaligned(const NetworkSample<int16_t>* input) noexcept
    {
        return batch_type(
            reinterpret_cast<const NetworkSampleValueUnderlyingType_t<int16_t>*>(input), xsimd::unaligned_mode{});
    }

    static inline void store_unaligned(const batch_type& input, NetworkSample<int16_t>* output) noexcept
    {
        input.store_unaligned(reinterpret_cast<NetworkSampleValueUnderlyingType_t<int16_t>*>(output));
    }
};

template<>
struct BatchCreator<NetworkSample<int32_t>> : public BaseBatchCreator<NetworkSample<int32_t>>
{
    using base = typename BatchCreator::BaseBatchCreator;
    using batch_type = base::batch_type;

    using base::load_unaligned;
    using base::store_unaligned;

    static inline batch_type load_unaligned(NetworkSample<int32_t>* input) noexcept
    {
        return batch_type(
            reinterpret_cast<NetworkSampleValueUnderlyingType_t<int32_t>*>(input), xsimd::unaligned_mode{});
    }

    static inline batch_type load_unaligned(const NetworkSample<int32_t>* input) noexcept
    {
        return batch_type(
            reinterpret_cast<const NetworkSampleValueUnderlyingType_t<int32_t>*>(input), xsimd::unaligned_mode{});
    }

    static inline void store_unaligned(const batch_type& input, NetworkSample<int32_t>* output) noexcept
    {
        input.store_unaligned(reinterpret_cast<NetworkSampleValueUnderlyingType_t<int32_t>*>(output));
    }
};

template<>
struct BatchCreator<NetworkSample<float32_t>> : public BaseBatchCreator<NetworkSample<float32_t>>
{
    using base = typename BatchCreator::BaseBatchCreator;
    using batch_type = base::batch_type;

    using base::load_unaligned;
    using base::store_unaligned;

    static inline batch_type load_unaligned(NetworkSample<float32_t>* input) noexcept
    {
        return batch_type(
            reinterpret_cast<NetworkSampleValueUnderlyingType_t<float32_t>*>(input), xsimd::unaligned_mode{});
    }

    static inline batch_type load_unaligned(const NetworkSample<float32_t>* input) noexcept
    {
        return batch_type(
            reinterpret_cast<const NetworkSampleValueUnderlyingType_t<float32_t>*>(input), xsimd::unaligned_mode{});
    }

    static inline void store_unaligned(const batch_type& input, NetworkSample<float32_t>* output) noexcept
    {
        input.store_unaligned(reinterpret_cast<NetworkSampleValueUnderlyingType_t<float32_t>*>(output));
    }
};

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_detail_batch_creator_
