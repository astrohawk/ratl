#ifndef _ratl_batch_sample_fix_
#define _ratl_batch_sample_fix_

// C++ Standard Library includes
#include <type_traits>

// ratl includes
#include <ratl/detail/batch_traits.hpp>
#include <ratl/detail/config.hpp>

#if defined(RATL_HAS_XSIMD)

namespace ratl
{
namespace detail
{
// batchFixNegativeSamples

template<class SampleType>
struct BatchFixNegativeSamplesImpl
{
    static const BatchSampleValueType_t<SampleType>& fix(const BatchSampleValueType_t<SampleType>& input) noexcept
    {
        return input;
    }
};

template<>
struct BatchFixNegativeSamplesImpl<int24_t>
{
    static BatchSampleValueType_t<int24_t> fix(const BatchSampleValueType_t<int24_t>& input) noexcept
    {
        auto cmp = (input & 0x800000) != 0;
        BatchSampleValueType_t<int24_t> negative_input = input | static_cast<int32_t>(0xff000000);
        return xsimd::select(cmp, negative_input, input);
    }
};

template<class SampleType>
BatchSampleValueType_t<SampleType> batchFixNegativeSamples(const BatchSampleValueType_t<SampleType>& input) noexcept
{
    return BatchFixNegativeSamplesImpl<SampleType>::fix(input);
}

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_batch_sample_fix_
