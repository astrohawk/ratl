/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_batch_alignment_
#define _ratl_detail_batch_alignment_

// ratl includes
#include <ratl/detail/batch_traits.hpp>
#include <ratl/detail/config.hpp>
#include <ratl/detail/sample_iterator.hpp>
#include <ratl/detail/utility.hpp>

// other includes
#include <type_traits>

#if defined(RATL_HAS_XSIMD)

namespace ratl
{
namespace detail
{
template<typename BatchType>
static constexpr std::size_t batch_alignment = xsimd::simd_batch_traits<BatchType>::align;

struct batch_alignment_mode
{
    struct aligned
    {
    };

    struct unaligned
    {
    };

    struct noncontiguous
    {
    };

    struct unalignable
    {
    };
};

template<typename SampleType, typename BatchType, typename = void>
class batch_alignment_dispatcher
{
public:
    template<
        typename IteratorTag,
        typename IteratorSampleType,
        typename IteratorSampleTraits,
        typename IteratorContiguous,
        typename Fn>
    static inline decltype(auto)
    dispatch(sample_iterator<IteratorTag, IteratorSampleType, IteratorSampleTraits, IteratorContiguous>, Fn&& fn) noexcept(
        // clang-format off
        noexcept(detail::invoke(std::forward<Fn>(fn), batch_alignment_mode::unalignable()))
        // clang-format on
    )
    {
        return detail::invoke(std::forward<Fn>(fn), batch_alignment_mode::unalignable());
    }
};

template<typename SampleType, typename BatchType>
class batch_alignment_dispatcher<
    SampleType,
    BatchType,
    std::enable_if_t<is_native_batch_type<SampleType, BatchType>::value>>
{
    static inline bool is_aligned(const SampleType* ptr) noexcept
    {
        return reinterpret_cast<std::uintptr_t>(ptr) % batch_alignment<BatchType> == 0;
    }

public:
    template<typename IteratorTag, typename IteratorSampleType, typename IteratorSampleTraits, typename Fn>
    static inline decltype(auto)
    dispatch(sample_iterator<IteratorTag, IteratorSampleType, IteratorSampleTraits, std::true_type> iterator, Fn&& fn) noexcept(
        // clang-format off
        noexcept(detail::invoke(std::forward<Fn>(fn), batch_alignment_mode::aligned())) &&
        noexcept(detail::invoke(std::forward<Fn>(fn), batch_alignment_mode::unaligned()))
        // clang-format on
    )
    {
        if (is_aligned(iterator.base()))
        {
            return detail::invoke(std::forward<Fn>(fn), batch_alignment_mode::aligned());
        }
        else
        {
            return detail::invoke(std::forward<Fn>(fn), batch_alignment_mode::unaligned());
        }
    }

    template<typename IteratorTag, typename IteratorSampleType, typename IteratorSampleTraits, typename Fn>
    static inline decltype(auto)
    dispatch(sample_iterator<IteratorTag, IteratorSampleType, IteratorSampleTraits, std::false_type>, Fn&& fn) noexcept(
        // clang-format off
        noexcept(detail::invoke(std::forward<Fn>(fn), batch_alignment_mode::noncontiguous()))
        // clang-format on
    )
    {
        return detail::invoke(std::forward<Fn>(fn), batch_alignment_mode::noncontiguous());
    }
};

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_detail_batch_alignment_
