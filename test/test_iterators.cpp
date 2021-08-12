
#include <gtest/gtest.h>
#include <ratl/ratl.hpp>

namespace ratl
{
namespace test
{
TEST(RandomAccess, TestTag)
{
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::InterleavedIterator<Sample<int16_t>>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::InterleavedIterator<Sample<int24_t>>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::InterleavedIterator<Sample<int32_t>>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::InterleavedIterator<Sample<float32_t>>>::iterator_category,
        std::random_access_iterator_tag>();

    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::NoninterleavedIterator<Sample<int16_t>>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::NoninterleavedIterator<Sample<int24_t>>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::NoninterleavedIterator<Sample<int32_t>>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::NoninterleavedIterator<Sample<float32_t>>>::iterator_category,
        std::random_access_iterator_tag>();

    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::ChannelIterator<Sample<int16_t>, false>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::ChannelIterator<Sample<int24_t>, false>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::ChannelIterator<Sample<int32_t>, false>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::ChannelIterator<Sample<float32_t>, false>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::ChannelIterator<Sample<int16_t>, true>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::ChannelIterator<Sample<int24_t>, true>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::ChannelIterator<Sample<int32_t>, true>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::ChannelIterator<Sample<float32_t>, true>>::iterator_category,
        std::random_access_iterator_tag>();

    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::ChannelIterator<Sample<int16_t>, false>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::ChannelIterator<Sample<int24_t>, false>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::ChannelIterator<Sample<int32_t>, false>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::ChannelIterator<Sample<float32_t>, false>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::ChannelIterator<Sample<int16_t>, true>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::ChannelIterator<Sample<int24_t>, true>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::ChannelIterator<Sample<int32_t>, true>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::ChannelIterator<Sample<float32_t>, true>>::iterator_category,
        std::random_access_iterator_tag>();
}

#if defined(RATL_CPP_VERSION_HAS_CPP20)
static_assert(std::random_access_iterator<detail::InterleavedIterator<Sample<int16_t>>>);
static_assert(std::random_access_iterator<detail::InterleavedIterator<Sample<int24_t>>>);
static_assert(std::random_access_iterator<detail::InterleavedIterator<Sample<int32_t>>>);
static_assert(std::random_access_iterator<detail::InterleavedIterator<Sample<float32_t>>>);

static_assert(std::random_access_iterator<detail::NoninterleavedIterator<Sample<int16_t>>>);
static_assert(std::random_access_iterator<detail::NoninterleavedIterator<Sample<int24_t>>>);
static_assert(std::random_access_iterator<detail::NoninterleavedIterator<Sample<int32_t>>>);
static_assert(std::random_access_iterator<detail::NoninterleavedIterator<Sample<float32_t>>>);

static_assert(std::random_access_iterator<detail::ChannelIterator<Sample<int16_t>, false>>);
static_assert(std::random_access_iterator<detail::ChannelIterator<Sample<int24_t>, false>>);
static_assert(std::random_access_iterator<detail::ChannelIterator<Sample<int32_t>, false>>);
static_assert(std::random_access_iterator<detail::ChannelIterator<Sample<float32_t>, false>>);
static_assert(std::contiguous_iterator<detail::ChannelIterator<Sample<int16_t>, true>>);
static_assert(std::contiguous_iterator<detail::ChannelIterator<Sample<int24_t>, true>>);
static_assert(std::contiguous_iterator<detail::ChannelIterator<Sample<int32_t>, true>>);
static_assert(std::contiguous_iterator<detail::ChannelIterator<Sample<float32_t>, true>>);

static_assert(std::random_access_iterator<detail::FrameIterator<Sample<int16_t>, false>>);
static_assert(std::random_access_iterator<detail::FrameIterator<Sample<int24_t>, false>>);
static_assert(std::random_access_iterator<detail::FrameIterator<Sample<int32_t>, false>>);
static_assert(std::random_access_iterator<detail::FrameIterator<Sample<float32_t>, false>>);
static_assert(std::contiguous_iterator<detail::FrameIterator<Sample<int16_t>, true>>);
static_assert(std::contiguous_iterator<detail::FrameIterator<Sample<int24_t>, true>>);
static_assert(std::contiguous_iterator<detail::FrameIterator<Sample<int32_t>, true>>);
static_assert(std::contiguous_iterator<detail::FrameIterator<Sample<float32_t>, true>>);
#endif
} // namespace test
} // namespace ratl