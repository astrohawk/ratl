/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_test_predicates_
#define _ratl_test_predicates_

// ratl includes
#include <ratl/ratl.hpp>

// other includes
#include <gtest/gtest.h>

namespace ratl
{
namespace test
{
template<typename SampleType, typename AllocatorA, typename AllocatorB>
::testing::AssertionResult IsContainerEqual(
    const basic_interleaved<SampleType, AllocatorA>& a, const basic_interleaved<SampleType, AllocatorB>& b) noexcept
{
    if (a == b)
    {
        return testing::AssertionSuccess();
    }
    return testing::AssertionFailure();
}

template<typename SampleType, typename AllocatorA, typename AllocatorB>
::testing::AssertionResult IsContainerNotEqual(
    const basic_interleaved<SampleType, AllocatorA>& a, const basic_interleaved<SampleType, AllocatorB>& b) noexcept
{
    if (a != b)
    {
        return testing::AssertionSuccess();
    }
    return testing::AssertionFailure();
}

template<typename SampleType, typename AllocatorA, typename AllocatorB>
::testing::AssertionResult IsContainerContentEqual(
    const basic_interleaved<SampleType, AllocatorA>& a, const basic_interleaved<SampleType, AllocatorB>& b) noexcept
{
    return IsContainerEqual(a, b);
}

template<typename SampleType, typename AllocatorA, typename AllocatorB>
::testing::AssertionResult IsContainerContentNotEqual(
    const basic_interleaved<SampleType, AllocatorA>& a, const basic_interleaved<SampleType, AllocatorB>& b) noexcept
{
    return IsContainerNotEqual(a, b);
}

template<
    typename SampleTypeA,
    typename SampleTraitsA,
    typename SampleTypeB,
    typename SampleTraitsB,
    std::enable_if_t<
        std::is_same<typename SampleTraitsA::const_sample_type, typename SampleTraitsB::const_sample_type>::value,
        bool> = true>
::testing::AssertionResult IsContainerEqual(
    const basic_interleaved_span<SampleTypeA, SampleTraitsA>& a,
    const basic_interleaved_span<SampleTypeB, SampleTraitsB>& b) noexcept
{
    if ((a.channels() == b.channels()) && (a.frames() == b.frames()) && (a.data() == b.data()))
    {
        return testing::AssertionSuccess();
    }
    return testing::AssertionFailure();
}

template<
    typename SampleTypeA,
    typename SampleTraitsA,
    typename SampleTypeB,
    typename SampleTraitsB,
    std::enable_if_t<
        std::is_same<typename SampleTraitsA::const_sample_type, typename SampleTraitsB::const_sample_type>::value,
        bool> = true>
::testing::AssertionResult IsContainerNotEqual(
    const basic_interleaved_span<SampleTypeA, SampleTraitsA>& a,
    const basic_interleaved_span<SampleTypeB, SampleTraitsB>& b) noexcept
{
    return !IsContainerEqual(a, b);
}

template<
    typename SampleTypeA,
    typename SampleTraitsA,
    typename SampleTypeB,
    typename SampleTraitsB,
    std::enable_if_t<
        std::is_same<typename SampleTraitsA::const_sample_type, typename SampleTraitsB::const_sample_type>::value,
        bool> = true>
::testing::AssertionResult IsContainerContentEqual(
    const basic_interleaved_span<SampleTypeA, SampleTraitsA>& a,
    const basic_interleaved_span<SampleTypeB, SampleTraitsB>& b) noexcept
{
    if ((a.channels() == b.channels()) && (a.frames() == b.frames()) &&
        std::equal(a.data(), a.data() + a.samples(), b.data()))
    {
        return testing::AssertionSuccess();
    }
    return testing::AssertionFailure();
}

template<
    typename SampleTypeA,
    typename SampleTraitsA,
    typename SampleTypeB,
    typename SampleTraitsB,
    std::enable_if_t<
        std::is_same<typename SampleTraitsA::const_sample_type, typename SampleTraitsB::const_sample_type>::value,
        bool> = true>
::testing::AssertionResult IsContainerContentNotEqual(
    const basic_interleaved_span<SampleTypeA, SampleTraitsA>& a,
    const basic_interleaved_span<SampleTypeB, SampleTraitsB>& b) noexcept
{
    return !IsContainerContentEqual(a, b);
}

template<typename SampleType, typename AllocatorA, typename AllocatorB>
::testing::AssertionResult IsContainerEqual(
    const basic_noninterleaved<SampleType, AllocatorA>& a,
    const basic_noninterleaved<SampleType, AllocatorB>& b) noexcept
{
    if (a == b)
    {
        return testing::AssertionSuccess();
    }
    return testing::AssertionFailure();
}

template<typename SampleType, typename AllocatorA, typename AllocatorB>
::testing::AssertionResult IsContainerNotEqual(
    const basic_noninterleaved<SampleType, AllocatorA>& a,
    const basic_noninterleaved<SampleType, AllocatorB>& b) noexcept
{
    if (a != b)
    {
        return testing::AssertionSuccess();
    }
    return testing::AssertionFailure();
}

template<typename SampleType, typename AllocatorA, typename AllocatorB>
::testing::AssertionResult IsContainerContentEqual(
    const basic_noninterleaved<SampleType, AllocatorA>& a,
    const basic_noninterleaved<SampleType, AllocatorB>& b) noexcept
{
    return IsContainerEqual(a, b);
}

template<typename SampleType, typename AllocatorA, typename AllocatorB>
::testing::AssertionResult IsContainerContentNotEqual(
    const basic_noninterleaved<SampleType, AllocatorA>& a,
    const basic_noninterleaved<SampleType, AllocatorB>& b) noexcept
{
    return IsContainerNotEqual(a, b);
}

template<
    typename SampleTypeA,
    typename SampleTraitsA,
    typename SampleTypeB,
    typename SampleTraitsB,
    std::enable_if_t<
        std::is_same<typename SampleTraitsA::const_sample_type, typename SampleTraitsB::const_sample_type>::value,
        bool> = true>
::testing::AssertionResult IsContainerEqual(
    const basic_noninterleaved_span<SampleTypeA, SampleTraitsA>& a,
    const basic_noninterleaved_span<SampleTypeB, SampleTraitsB>& b) noexcept
{
    if ((a.channels() == b.channels()) && (a.frames() == b.frames()) && (a.data() == b.data()))
    {
        return testing::AssertionSuccess();
    }
    return testing::AssertionFailure();
}

template<
    typename SampleTypeA,
    typename SampleTraitsA,
    typename SampleTypeB,
    typename SampleTraitsB,
    std::enable_if_t<
        std::is_same<typename SampleTraitsA::const_sample_type, typename SampleTraitsB::const_sample_type>::value,
        bool> = true>
::testing::AssertionResult IsContainerNotEqual(
    const basic_noninterleaved_span<SampleTypeA, SampleTraitsA>& a,
    const basic_noninterleaved_span<SampleTypeB, SampleTraitsB>& b) noexcept
{
    return !IsContainerEqual(a, b);
}

template<
    typename SampleTypeA,
    typename SampleTraitsA,
    typename SampleTypeB,
    typename SampleTraitsB,
    std::enable_if_t<
        std::is_same<typename SampleTraitsA::const_sample_type, typename SampleTraitsB::const_sample_type>::value,
        bool> = true>
::testing::AssertionResult IsContainerContentEqual(
    const basic_noninterleaved_span<SampleTypeA, SampleTraitsA>& a,
    const basic_noninterleaved_span<SampleTypeB, SampleTraitsB>& b) noexcept
{
    if ((a.channels() == b.channels()) && (a.frames() == b.frames()) &&
        std::equal(a.data(), a.data() + a.samples(), b.data()))
    {
        return testing::AssertionSuccess();
    }
    return testing::AssertionFailure();
}

template<
    typename SampleTypeA,
    typename SampleTraitsA,
    typename SampleTypeB,
    typename SampleTraitsB,
    std::enable_if_t<
        std::is_same<typename SampleTraitsA::const_sample_type, typename SampleTraitsB::const_sample_type>::value,
        bool> = true>
::testing::AssertionResult IsContainerContentNotEqual(
    const basic_noninterleaved_span<SampleTypeA, SampleTraitsA>& a,
    const basic_noninterleaved_span<SampleTypeB, SampleTraitsB>& b) noexcept
{
    return !IsContainerContentEqual(a, b);
}

template<typename SampleType, typename AllocatorA, typename AllocatorB>
::testing::AssertionResult IsContainerEqual(
    const basic_frame<SampleType, AllocatorA>& a, const basic_frame<SampleType, AllocatorB>& b) noexcept
{
    if (a == b)
    {
        return testing::AssertionSuccess();
    }
    return testing::AssertionFailure();
}

template<typename SampleType, typename AllocatorA, typename AllocatorB>
::testing::AssertionResult IsContainerNotEqual(
    const basic_frame<SampleType, AllocatorA>& a, const basic_frame<SampleType, AllocatorB>& b) noexcept
{
    if (a != b)
    {
        return testing::AssertionSuccess();
    }
    return testing::AssertionFailure();
}

template<typename SampleType, typename AllocatorA, typename AllocatorB>
::testing::AssertionResult IsContainerContentEqual(
    const basic_frame<SampleType, AllocatorA>& a, const basic_frame<SampleType, AllocatorB>& b) noexcept
{
    return IsContainerEqual(a, b);
}

template<typename SampleType, typename AllocatorA, typename AllocatorB>
::testing::AssertionResult IsContainerContentNotEqual(
    const basic_frame<SampleType, AllocatorA>& a, const basic_frame<SampleType, AllocatorB>& b) noexcept
{
    return IsContainerNotEqual(a, b);
}

template<
    typename SampleTypeA,
    typename SampleTraitsA,
    typename ContiguousA,
    typename SampleTypeB,
    typename SampleTraitsB,
    typename ContiguousB,
    std::enable_if_t<
        std::is_same<typename SampleTraitsA::const_sample_type, typename SampleTraitsB::const_sample_type>::value,
        bool> = true>
::testing::AssertionResult IsContainerEqual(
    const basic_frame_span<SampleTypeA, SampleTraitsA, ContiguousA>& a,
    const basic_frame_span<SampleTypeB, SampleTraitsB, ContiguousB>& b) noexcept
{
    if ((a.channels() == b.channels()) && std::equal(a.begin(), a.end(), b.begin()))
    {
        return testing::AssertionSuccess();
    }
    return testing::AssertionFailure();
}

template<
    typename SampleTypeA,
    typename SampleTraitsA,
    typename ContiguousA,
    typename SampleTypeB,
    typename SampleTraitsB,
    typename ContiguousB,
    std::enable_if_t<
        std::is_same<typename SampleTraitsA::const_sample_type, typename SampleTraitsB::const_sample_type>::value,
        bool> = true>
::testing::AssertionResult IsContainerNotEqual(
    const basic_frame_span<SampleTypeA, SampleTraitsA, ContiguousA>& a,
    const basic_frame_span<SampleTypeB, SampleTraitsB, ContiguousB>& b) noexcept
{
    return !IsContainerEqual(a, b);
}

template<
    typename SampleTypeA,
    typename SampleTraitsA,
    typename SampleTypeB,
    typename SampleTraitsB,
    std::enable_if_t<
        std::is_same<typename SampleTraitsA::const_sample_type, typename SampleTraitsB::const_sample_type>::value,
        bool> = true>
::testing::AssertionResult IsContainerContentEqual(
    const basic_frame_span<SampleTypeA, SampleTraitsA>& a,
    const basic_frame_span<SampleTypeB, SampleTraitsB>& b) noexcept
{
    if ((a.channels() == b.channels()) && std::equal(a.data(), a.data() + a.channels(), b.data()))
    {
        return testing::AssertionSuccess();
    }
    return testing::AssertionFailure();
}

template<
    typename SampleTypeA,
    typename SampleTraitsA,
    typename SampleTypeB,
    typename SampleTraitsB,
    std::enable_if_t<
        std::is_same<typename SampleTraitsA::const_sample_type, typename SampleTraitsB::const_sample_type>::value,
        bool> = true>
::testing::AssertionResult IsContainerContentNotEqual(
    const basic_frame_span<SampleTypeA, SampleTraitsA>& a,
    const basic_frame_span<SampleTypeB, SampleTraitsB>& b) noexcept
{
    return !IsContainerContentEqual(a, b);
}

template<typename SampleType, typename AllocatorA, typename AllocatorB>
::testing::AssertionResult IsContainerEqual(
    const basic_channel<SampleType, AllocatorA>& a, const basic_channel<SampleType, AllocatorB>& b) noexcept
{
    if (a == b)
    {
        return testing::AssertionSuccess();
    }
    return testing::AssertionFailure();
}

template<typename SampleType, typename AllocatorA, typename AllocatorB>
::testing::AssertionResult IsContainerNotEqual(
    const basic_channel<SampleType, AllocatorA>& a, const basic_channel<SampleType, AllocatorB>& b) noexcept
{
    if (a != b)
    {
        return testing::AssertionSuccess();
    }
    return testing::AssertionFailure();
}

template<typename SampleType, typename AllocatorA, typename AllocatorB>
::testing::AssertionResult IsContainerContentEqual(
    const basic_channel<SampleType, AllocatorA>& a, const basic_channel<SampleType, AllocatorB>& b) noexcept
{
    return IsContainerEqual(a, b);
}

template<typename SampleType, typename AllocatorA, typename AllocatorB>
::testing::AssertionResult IsContainerContentNotEqual(
    const basic_channel<SampleType, AllocatorA>& a, const basic_channel<SampleType, AllocatorB>& b) noexcept
{
    return IsContainerNotEqual(a, b);
}

template<
    typename SampleTypeA,
    typename SampleTraitsA,
    typename ContiguousA,
    typename SampleTypeB,
    typename SampleTraitsB,
    typename ContiguousB,
    std::enable_if_t<
        std::is_same<typename SampleTraitsA::const_sample_type, typename SampleTraitsB::const_sample_type>::value,
        bool> = true>
::testing::AssertionResult IsContainerEqual(
    const basic_channel_span<SampleTypeA, SampleTraitsA, ContiguousA>& a,
    const basic_channel_span<SampleTypeB, SampleTraitsB, ContiguousB>& b) noexcept
{
    if ((a.samples() == b.samples()) && std::equal(a.begin(), a.end(), b.begin()))
    {
        return testing::AssertionSuccess();
    }
    return testing::AssertionFailure();
}

template<
    typename SampleTypeA,
    typename SampleTraitsA,
    typename ContiguousA,
    typename SampleTypeB,
    typename SampleTraitsB,
    typename ContiguousB,
    std::enable_if_t<
        std::is_same<typename SampleTraitsA::const_sample_type, typename SampleTraitsB::const_sample_type>::value,
        bool> = true>
::testing::AssertionResult IsContainerNotEqual(
    const basic_channel_span<SampleTypeA, SampleTraitsA, ContiguousA>& a,
    const basic_channel_span<SampleTypeB, SampleTraitsB, ContiguousB>& b) noexcept
{
    return !IsContainerEqual(a, b);
}

template<
    typename SampleTypeA,
    typename SampleTraitsA,
    typename SampleTypeB,
    typename SampleTraitsB,
    std::enable_if_t<
        std::is_same<typename SampleTraitsA::const_sample_type, typename SampleTraitsB::const_sample_type>::value,
        bool> = true>
::testing::AssertionResult IsContainerContentEqual(
    const basic_channel_span<SampleTypeA, SampleTraitsA>& a,
    const basic_channel_span<SampleTypeB, SampleTraitsB>& b) noexcept
{
    if ((a.samples() == b.samples()) && std::equal(a.data(), a.data() + a.samples(), b.data()))
    {
        return testing::AssertionSuccess();
    }
    return testing::AssertionFailure();
}

template<
    typename SampleTypeA,
    typename SampleTraitsA,
    typename SampleTypeB,
    typename SampleTraitsB,
    std::enable_if_t<
        std::is_same<typename SampleTraitsA::const_sample_type, typename SampleTraitsB::const_sample_type>::value,
        bool> = true>
::testing::AssertionResult IsContainerContentNotEqual(
    const basic_channel_span<SampleTypeA, SampleTraitsA>& a,
    const basic_channel_span<SampleTypeB, SampleTraitsB>& b) noexcept
{
    return !IsContainerContentEqual(a, b);
}

} // namespace test
} // namespace ratl

#endif
