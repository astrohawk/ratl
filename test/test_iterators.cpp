
#include <gtest/gtest.h>
#include <ratl/ratl.hpp>

namespace ratl
{
namespace test
{
TEST(RandomAccess, TestTag)
{
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::interleaved_iterator<sample<int16_t>>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::interleaved_iterator<sample<int24_t>>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::interleaved_iterator<sample<int32_t>>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::interleaved_iterator<sample<float32_t>>>::iterator_category,
        std::random_access_iterator_tag>();

    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::noninterleaved_iterator<sample<int16_t>>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::noninterleaved_iterator<sample<int24_t>>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::noninterleaved_iterator<sample<int32_t>>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::noninterleaved_iterator<sample<float32_t>>>::iterator_category,
        std::random_access_iterator_tag>();

    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::channel_iterator<sample<int16_t>, false>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::channel_iterator<sample<int24_t>, false>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::channel_iterator<sample<int32_t>, false>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::channel_iterator<sample<float32_t>, false>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::channel_iterator<sample<int16_t>, true>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::channel_iterator<sample<int24_t>, true>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::channel_iterator<sample<int32_t>, true>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::channel_iterator<sample<float32_t>, true>>::iterator_category,
        std::random_access_iterator_tag>();

    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::channel_iterator<sample<int16_t>, false>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::channel_iterator<sample<int24_t>, false>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::channel_iterator<sample<int32_t>, false>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::channel_iterator<sample<float32_t>, false>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::channel_iterator<sample<int16_t>, true>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::channel_iterator<sample<int24_t>, true>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::channel_iterator<sample<int32_t>, true>>::iterator_category,
        std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<
        std::iterator_traits<detail::channel_iterator<sample<float32_t>, true>>::iterator_category,
        std::random_access_iterator_tag>();
}

#if defined(RATL_CPP_VERSION_HAS_CPP20)
static_assert(std::random_access_iterator<detail::interleaved_iterator<sample<int16_t>>>);
static_assert(std::random_access_iterator<detail::interleaved_iterator<sample<int24_t>>>);
static_assert(std::random_access_iterator<detail::interleaved_iterator<sample<int32_t>>>);
static_assert(std::random_access_iterator<detail::interleaved_iterator<sample<float32_t>>>);

static_assert(std::random_access_iterator<detail::noninterleaved_iterator<sample<int16_t>>>);
static_assert(std::random_access_iterator<detail::noninterleaved_iterator<sample<int24_t>>>);
static_assert(std::random_access_iterator<detail::noninterleaved_iterator<sample<int32_t>>>);
static_assert(std::random_access_iterator<detail::noninterleaved_iterator<sample<float32_t>>>);

static_assert(std::random_access_iterator<detail::channel_iterator<sample<int16_t>, false>>);
static_assert(std::random_access_iterator<detail::channel_iterator<sample<int24_t>, false>>);
static_assert(std::random_access_iterator<detail::channel_iterator<sample<int32_t>, false>>);
static_assert(std::random_access_iterator<detail::channel_iterator<sample<float32_t>, false>>);
static_assert(std::contiguous_iterator<detail::channel_iterator<sample<int16_t>, true>>);
static_assert(std::contiguous_iterator<detail::channel_iterator<sample<int24_t>, true>>);
static_assert(std::contiguous_iterator<detail::channel_iterator<sample<int32_t>, true>>);
static_assert(std::contiguous_iterator<detail::channel_iterator<sample<float32_t>, true>>);

static_assert(std::random_access_iterator<detail::frame_iterator<sample<int16_t>, false>>);
static_assert(std::random_access_iterator<detail::frame_iterator<sample<int24_t>, false>>);
static_assert(std::random_access_iterator<detail::frame_iterator<sample<int32_t>, false>>);
static_assert(std::random_access_iterator<detail::frame_iterator<sample<float32_t>, false>>);
static_assert(std::contiguous_iterator<detail::frame_iterator<sample<int16_t>, true>>);
static_assert(std::contiguous_iterator<detail::frame_iterator<sample<int24_t>, true>>);
static_assert(std::contiguous_iterator<detail::frame_iterator<sample<int32_t>, true>>);
static_assert(std::contiguous_iterator<detail::frame_iterator<sample<float32_t>, true>>);
#endif
} // namespace test
} // namespace ratl