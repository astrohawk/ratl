#ifndef _ratl_batch_traits_
#define _ratl_batch_traits_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/detail/batch_base_traits.hpp>
#include <ratl/network_sample.hpp>
#include <ratl/sample.hpp>

#if defined(RATL_HAS_XSIMD)

namespace ratl
{
namespace detail
{
// BatchSampleType

template<class Sample>
struct BatchSampleType;

template<class SampleType>
struct BatchSampleType<Sample<SampleType>>
{
    using type = BatchSampleValueType_t<SampleType>;
};

template<class SampleType>
struct BatchSampleType<NetworkSample<SampleType>>
{
    using type = BatchNetworkSampleValueType_t<SampleType>;
};

template<class Sample>
using BatchSampleType_t = typename BatchSampleType<Sample>::type;

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_batch_traits_
