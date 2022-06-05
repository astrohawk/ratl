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
struct batch_aligned
{
};

struct batch_unaligned
{
};

struct batch_unknown_alignment
{
};

template<typename SampleType, typename BatchType>
class batch_alignment_checker
{
public:
    template<typename Iterator>
    using alignment_t = std::conditional_t<
        std::is_same<Iterator, SampleType*>::value || std::is_same<Iterator, const SampleType*>::value,
        batch_unknown_alignment,
        batch_unaligned>;

private:
    template<typename Iterator>
    static inline bool is_aligned(Iterator iterator) noexcept
    {
        return reinterpret_cast<std::uintptr_t>(iterator) % xsimd::simd_batch_traits<BatchType>::align == 0;
    }

public:
    template<typename Iterator, typename Fn>
    static inline decltype(auto) unknown_alignment_dispatcher(Iterator iterator, Fn fn) noexcept(
        noexcept(fn(batch_aligned())) || noexcept(fn(batch_unaligned())))
    {
        static_assert(std::is_same<alignment_t<Iterator>, batch_unknown_alignment>::value, "");
        if (is_aligned(iterator))
        {
            return fn(batch_aligned());
        }
        else
        {
            return fn(batch_unaligned());
        }
    }
};

template<typename BatchType, typename Alignment>
class batch_alignment_select;

template<typename BatchType>
class batch_alignment_select<BatchType, batch_aligned>
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
class batch_alignment_select<BatchType, batch_unaligned>
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
