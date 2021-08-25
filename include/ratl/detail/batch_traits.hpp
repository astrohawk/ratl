#ifndef _ratl_detail_batch_traits_
#define _ratl_detail_batch_traits_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/detail/batch_value_traits.hpp>
#include <ratl/network_sample.hpp>
#include <ratl/sample.hpp>

#if defined(RATL_HAS_XSIMD)

namespace ratl
{
namespace detail
{
// batch_sample_type

template<class SampleType>
struct batch_sample_type;

template<class SampleValueType>
struct batch_sample_type<sample<SampleValueType>>
{
    using type = batch_sample_value_type_t<SampleValueType>;
};

template<class SampleValueType>
struct batch_sample_type<network_sample<SampleValueType>>
{
    using type = batch_network_sample_value_type_t<SampleValueType>;
};

template<class SampleType>
using batch_sample_type_t = typename batch_sample_type<SampleType>::type;

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_detail_batch_traits_
