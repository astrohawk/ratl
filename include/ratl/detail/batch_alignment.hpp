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

    struct unknown
    {
    };
};

template<typename SampleType, typename BatchType>
class batch_alignment_checker
{
    template<typename Pointer>
    static inline bool is_aligned(Pointer* ptr) noexcept
    {
        return reinterpret_cast<std::uintptr_t>(ptr) % batch_alignment<BatchType> == 0;
    }

public:
    template<typename Iterator>
    using alignment_mode_t = std::conditional_t<
        std::is_same<Iterator, SampleType*>::value || std::is_same<Iterator, const SampleType*>::value,
        batch_alignment_mode::unknown,
        batch_alignment_mode::unaligned>;

    template<typename Iterator, typename Fn>
    static inline decltype(auto) unknown_alignment_dispatcher(Iterator iterator, Fn&& fn) noexcept(
    // clang-format off
#if defined(RATL_CPP_VERSION_HAS_CPP17)
        noexcept(std::invoke(std::forward<Fn>(fn), batch_alignment_mode::aligned())) &&
        noexcept(std::invoke(std::forward<Fn>(fn), batch_alignment_mode::unaligned()))
#else
        noexcept(std::forward<Fn>(fn)(batch_alignment_mode::aligned())) &&
        noexcept(std::forward<Fn>(fn)(batch_alignment_mode::unaligned()))
#endif
        // clang-format on
    )
    {
        static_assert(std::is_same<alignment_mode_t<Iterator>, batch_alignment_mode::unknown>::value, "");
        if (is_aligned(iterator))
        {
#if defined(RATL_CPP_VERSION_HAS_CPP17)
            return std::invoke(std::forward<Fn>(fn), batch_alignment_mode::aligned());
#else
            return std::forward<Fn>(fn)(batch_alignment_mode::aligned());
#endif
        }
        else
        {
#if defined(RATL_CPP_VERSION_HAS_CPP17)
            return std::invoke(std::forward<Fn>(fn), batch_alignment_mode::unaligned());
#else
            return std::forward<Fn>(fn)(batch_alignment_mode::unaligned());
#endif
        }
    }
};

template<typename BatchType, typename AlignmentMode>
class batch_alignment_select;

template<typename BatchType>
class batch_alignment_select<BatchType, batch_alignment_mode::aligned>
{
public:
    template<typename Input>
    static inline BatchType load(Input* input) noexcept
    {
        return BatchType(input, xsimd::aligned_mode());
    }

    template<typename Output>
    static inline void store(const BatchType& input, Output* output) noexcept
    {
        input.store_aligned(output);
    }
};

template<typename BatchType>
class batch_alignment_select<BatchType, batch_alignment_mode::unaligned>
{
public:
    template<typename Input>
    static inline BatchType load(Input* input) noexcept
    {
        return BatchType(input, xsimd::unaligned_mode());
    }

    template<typename Output>
    static inline void store(const BatchType& input, Output* output) noexcept
    {
        input.store_unaligned(output);
    }
};

} // namespace detail
} // namespace ratl

#endif

#endif // _ratl_detail_batch_alignment_
