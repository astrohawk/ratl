#ifndef _ratl_sample_traits_
#define _ratl_sample_traits_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/detail/config.hpp>
#include <ratl/detail/type_traits.hpp>

namespace ratl
{
namespace detail
{
template<class Sample, class = void>
struct IsSampleImpl : public std::false_type
{
};
template<class Sample>
struct IsSampleImpl<Sample, void_t<typename Sample::sample_type>> : public std::true_type
{
};

// IsSample

template<class Sample>
struct IsSample : public detail::IsSampleImpl<Sample>
{
};

// IsSample_v

template<class Sample>
static constexpr bool IsSample_v = IsSample<Sample>::value;

// SampleTraits

template<class Sample>
struct SampleTraits
{
    static_assert(IsSample_v<Sample>, "Sample is not a valid Sample type");

    /*
     * Sample and const_sample
     * The type of the Sample this trait represents,
     * e.g. ratl::Sample<XXX> or ratl::NetworkSample<XXX>
     */
    using sample = Sample;
    using const_sample = const sample;

    /*
     * sample_type
     * The underlying type that this Sample is representing,
     * i.e. int16_t, int24_t, int32_t, float32_t
     */
    using sample_type = typename sample::sample_type;

    /*
     * value_type
     * The type of the underlying representation in memory. This will be the same
     * as sample_type for ratl::Sample, but will be an opaque type for
     * ratl::NetworkSample.
     */
    using underlying_type = typename sample::value_type;

    /*
     * pointer, const_pointer, reference and const_reference
     * A (const) pointer/reference to the Sample.
     */
    using pointer = sample*;
    using const_pointer = const_sample*;
    using reference = sample&;
    using const_reference = const_sample&;
};

} // namespace detail
} // namespace ratl

#endif // _ratl_sample_traits_
