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

    template<std::size_t... I>
    static inline batch_type load_unaligned_impl(SampleType* input, std::index_sequence<I...>) noexcept
    {
        return batch_type{input[I]...};
    }

public:
    template<typename Iterator>
    static inline batch_type load_unaligned(Iterator input) noexcept
    {
        return load_unaligned_impl(input, std::make_index_sequence<BatchSize>{});
    }

    static inline batch_type load_unaligned(SampleType* input) noexcept
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

    static inline void store_unaligned(const batch_type& input, SampleType* output) noexcept
    {
        for (std::size_t i = 0; i < BatchSize; ++i)
        {
            output[i] = static_cast<SampleType>(input[i]);
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

    template<std::size_t... I>
    static inline batch_type load_unaligned_impl(
        NetworkSampleValueType_t<SampleType>* input, std::index_sequence<I...>) noexcept
    {
        return batch_type{networkToNetworkUnderlyingCast<SampleType>(input[I])...};
    }

public:
    template<typename Iterator>
    static inline batch_type load_unaligned(Iterator input) noexcept
    {
        return load_unaligned_impl(input, std::make_index_sequence<BatchSize>{});
    }

    static inline batch_type load_unaligned(NetworkSampleValueType_t<SampleType>* input) noexcept
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

    static inline void store_unaligned(const batch_type& input, NetworkSampleValueType_t<SampleType>* output) noexcept
    {
        for (std::size_t i = 0; i < BatchSize; ++i)
        {
            output[i] = networkUnderlyingToNetworkCast<SampleType>(
                static_cast<NetworkSampleValueUnderlyingType_t<SampleType>>(input[i]));
        }
    }
};

template<class Sample>
struct BatchCreator : public BaseBatchCreator<Sample>
{
};

template<>
struct BatchCreator<Sample<int32_t>> : public BaseBatchCreator<Sample<int32_t>>
{
    using base = typename BatchCreator::BaseBatchCreator;
    using batch_type = base::batch_type;

    using base::load_unaligned;
    using base::store_unaligned;

    static inline batch_type load_unaligned(int32_t* input) noexcept
    {
        return batch_type(input, xsimd::unaligned_mode{});
    }

    static inline void store_unaligned(const batch_type& input, int32_t* output) noexcept
    {
        input.store_unaligned(output);
    }
};

template<>
struct BatchCreator<Sample<float32_t>> : public BaseBatchCreator<Sample<float32_t>>
{
    using base = typename BatchCreator::BaseBatchCreator;
    using batch_type = base::batch_type;

    using base::load_unaligned;
    using base::store_unaligned;

    static inline batch_type load_unaligned(float32_t* input) noexcept
    {
        return batch_type(input, xsimd::unaligned_mode{});
    }

    static inline void store_unaligned(const batch_type& input, float32_t* output) noexcept
    {
        input.store_unaligned(output);
    }
};

template<>
struct BatchCreator<NetworkSample<int32_t>> : public BaseBatchCreator<NetworkSample<int32_t>>
{
    using base = typename BatchCreator::BaseBatchCreator;
    using batch_type = base::batch_type;

    using base::load_unaligned;
    using base::store_unaligned;

    static inline batch_type load_unaligned(NetworkSampleValueType_t<int32_t>* input) noexcept
    {
        return batch_type(reinterpret_cast<const uint32_t*>(input), xsimd::unaligned_mode{});
    }

    static inline void store_unaligned(const batch_type& input, NetworkSampleValueType_t<int32_t>* output) noexcept
    {
        input.store_unaligned(reinterpret_cast<uint32_t*>(output));
    }
};

template<>
struct BatchCreator<NetworkSample<float32_t>> : public BaseBatchCreator<NetworkSample<float32_t>>
{
    using base = typename BatchCreator::BaseBatchCreator;
    using batch_type = base::batch_type;

    using base::load_unaligned;
    using base::store_unaligned;

    static inline batch_type load_unaligned(NetworkSampleValueType_t<float32_t>* input) noexcept
    {
        return batch_type(reinterpret_cast<const uint32_t*>(input), xsimd::unaligned_mode{});
    }

    static inline void store_unaligned(const batch_type& input, NetworkSampleValueType_t<float32_t>* output) noexcept
    {
        input.store_unaligned(reinterpret_cast<uint32_t*>(output));
    }
};

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_detail_batch_creator_
