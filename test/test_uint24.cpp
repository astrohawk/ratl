/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

// ratl includes
#include <ratl/uint24.hpp>

// other includes
#include <gtest/gtest.h>

namespace ratl
{
namespace test
{
static_assert(sizeof(uint24_t) == 3, "uint24_t is not 3 bytes");
static_assert(sizeof(uint24_t[2]) == 6, "int24_t is not packed correctly");
static_assert(std::is_trivial<uint24_t>::value, "uint24_t is not a trivial type");
static_assert(std::is_trivially_copyable<uint24_t>::value, "uint24_t is not a trivially copyable type");
static_assert(std::is_standard_layout<uint24_t>::value, "uint24_t is not a standard layout type");

// Type tests

TEST(ReturnTypes, Plus)
{
    ::testing::StaticAssertTypeEq<decltype(ratl::uint24_t(1) + 1), decltype(uint16_t(1) + 1)>();
    ::testing::StaticAssertTypeEq<decltype(ratl::uint24_t(1) + 0x80000000), decltype(uint16_t(1) + 0x80000000)>();
}

TEST(ReturnTypes, Minus)
{
    ::testing::StaticAssertTypeEq<decltype(ratl::uint24_t(1) - 1), decltype(uint16_t(1) - 1)>();
    ::testing::StaticAssertTypeEq<decltype(ratl::uint24_t(1) - 2), decltype(uint16_t(1) - 2)>();
    ::testing::StaticAssertTypeEq<decltype(ratl::uint24_t(1) - 0x7fffffff), decltype(uint16_t(1) - 0x7fffffff)>();
    ::testing::StaticAssertTypeEq<decltype(ratl::uint24_t(1) - 0x80000000), decltype(uint16_t(1) - 0x80000000)>();
}

// 0 <-> 0

TEST(CompareZeroZero, Equal)
{
    EXPECT_TRUE(ratl::uint24_t(0) == ratl::uint24_t(0));
}

TEST(CompareZeroZero, NotEqual)
{
    EXPECT_FALSE(ratl::uint24_t(0) != ratl::uint24_t(0));
}

TEST(CompareZeroZero, LessThan)
{
    EXPECT_FALSE(ratl::uint24_t(0) < ratl::uint24_t(0));
}

TEST(CompareZeroZero, LessThanEqual)
{
    EXPECT_TRUE(ratl::uint24_t(0) <= ratl::uint24_t(0));
}

TEST(CompareZeroZero, GreaterThan)
{
    EXPECT_FALSE(ratl::uint24_t(0) > ratl::uint24_t(0));
}

TEST(CompareZeroZero, GreaterThanEqual)
{
    EXPECT_TRUE(ratl::uint24_t(0) >= ratl::uint24_t(0));
}

TEST(CompareZeroZero, EqualInt)
{
    EXPECT_TRUE(ratl::uint24_t(0) == 0);
}

TEST(CompareZeroZero, NotEqualInt)
{
    EXPECT_FALSE(ratl::uint24_t(0) != 0);
}

TEST(CompareZeroZero, LessThanInt)
{
    EXPECT_FALSE(ratl::uint24_t(0) < 0);
}

TEST(CompareZeroZero, LessThanEqualInt)
{
    EXPECT_TRUE(ratl::uint24_t(0) <= 0);
}

TEST(CompareZeroZero, GreaterThanInt)
{
    EXPECT_FALSE(ratl::uint24_t(0) > 0);
}

TEST(CompareZeroZero, GreaterThanEqualInt)
{
    EXPECT_TRUE(ratl::uint24_t(0) >= 0);
}

// 1 <-> 1

TEST(ComparePos1Pos1, Equal)
{
    EXPECT_TRUE(ratl::uint24_t(1) == ratl::uint24_t(1));
}

TEST(ComparePos1Pos1, NotEqual)
{
    EXPECT_FALSE(ratl::uint24_t(1) != ratl::uint24_t(1));
}

TEST(ComparePos1Pos1, LessThan)
{
    EXPECT_FALSE(ratl::uint24_t(1) < ratl::uint24_t(1));
}

TEST(ComparePos1Pos1, LessThanEqual)
{
    EXPECT_TRUE(ratl::uint24_t(1) <= ratl::uint24_t(1));
}

TEST(ComparePos1Pos1, GreaterThan)
{
    EXPECT_FALSE(ratl::uint24_t(1) > ratl::uint24_t(1));
}

TEST(ComparePos1Pos1, GreaterThanEqual)
{
    EXPECT_TRUE(ratl::uint24_t(1) >= ratl::uint24_t(1));
}

TEST(ComparePos1Pos1, EqualInt)
{
    EXPECT_TRUE(ratl::uint24_t(1) == 1);
}

TEST(ComparePos1Pos1, NotEqualInt)
{
    EXPECT_FALSE(ratl::uint24_t(1) != 1);
}

TEST(ComparePos1Pos1, LessThanInt)
{
    EXPECT_FALSE(ratl::uint24_t(1) < 1);
}

TEST(ComparePos1Pos1, LessThanEqualInt)
{
    EXPECT_TRUE(ratl::uint24_t(1) <= 1);
}

TEST(ComparePos1Pos1, GreaterThanInt)
{
    EXPECT_FALSE(ratl::uint24_t(1) > 1);
}

TEST(ComparePos1Pos1, GreaterThanEqualInt)
{
    EXPECT_TRUE(ratl::uint24_t(1) >= 1);
}

// 256 <-> 256

TEST(ComparePos256Pos256, Equal)
{
    EXPECT_TRUE(ratl::uint24_t(256) == ratl::uint24_t(256));
}

TEST(ComparePos256Pos256, NotEqual)
{
    EXPECT_FALSE(ratl::uint24_t(256) != ratl::uint24_t(256));
}

TEST(ComparePos256Pos256, LessThan)
{
    EXPECT_FALSE(ratl::uint24_t(256) < ratl::uint24_t(256));
}

TEST(ComparePos256Pos256, LessThanEqual)
{
    EXPECT_TRUE(ratl::uint24_t(256) <= ratl::uint24_t(256));
}

TEST(ComparePos256Pos256, GreaterThan)
{
    EXPECT_FALSE(ratl::uint24_t(256) > ratl::uint24_t(256));
}

TEST(ComparePos256Pos256, GreaterThanEqual)
{
    EXPECT_TRUE(ratl::uint24_t(256) >= ratl::uint24_t(256));
}

TEST(ComparePos256Pos256, EqualInt)
{
    EXPECT_TRUE(ratl::uint24_t(256) == 256);
}

TEST(ComparePos256Pos256, NotEqualInt)
{
    EXPECT_FALSE(ratl::uint24_t(256) != 256);
}

TEST(ComparePos256Pos256, LessThanInt)
{
    EXPECT_FALSE(ratl::uint24_t(256) < 256);
}

TEST(ComparePos256Pos256, LessThanEqualInt)
{
    EXPECT_TRUE(ratl::uint24_t(256) <= 256);
}

TEST(ComparePos256Pos256, GreaterThanInt)
{
    EXPECT_FALSE(ratl::uint24_t(256) > 256);
}

TEST(ComparePos256Pos256, GreaterThanEqualInt)
{
    EXPECT_TRUE(ratl::uint24_t(256) >= 256);
}

// 1 <-> 0

TEST(ComparePos1Zero, Equal)
{
    EXPECT_FALSE(ratl::uint24_t(1) == ratl::uint24_t(0));
}

TEST(ComparePos1Zero, NotEqual)
{
    EXPECT_TRUE(ratl::uint24_t(1) != ratl::uint24_t(0));
}

TEST(ComparePos1Zero, LessThan)
{
    EXPECT_FALSE(ratl::uint24_t(1) < ratl::uint24_t(0));
}

TEST(ComparePos1Zero, LessThanEqual)
{
    EXPECT_FALSE(ratl::uint24_t(1) <= ratl::uint24_t(0));
}

TEST(ComparePos1Zero, GreaterThan)
{
    EXPECT_TRUE(ratl::uint24_t(1) > ratl::uint24_t(0));
}

TEST(ComparePos1Zero, GreaterThanEqual)
{
    EXPECT_TRUE(ratl::uint24_t(1) >= ratl::uint24_t(0));
}

TEST(ComparePos1Zero, EqualInt)
{
    EXPECT_FALSE(ratl::uint24_t(1) == 0);
}

TEST(ComparePos1Zero, NotEqualInt)
{
    EXPECT_TRUE(ratl::uint24_t(1) != 0);
}

TEST(ComparePos1Zero, LessThanInt)
{
    EXPECT_FALSE(ratl::uint24_t(1) < 0);
}

TEST(ComparePos1Zero, LessThanEqualInt)
{
    EXPECT_FALSE(ratl::uint24_t(1) <= 0);
}

TEST(ComparePos1Zero, GreaterThanInt)
{
    EXPECT_TRUE(ratl::uint24_t(1) > 0);
}

TEST(ComparePos1Zero, GreaterThanEqualInt)
{
    EXPECT_TRUE(ratl::uint24_t(1) >= 0);
}

// 0 <-> 1

TEST(CompareZeroPos1, Equal)
{
    EXPECT_FALSE(ratl::uint24_t(0) == ratl::uint24_t(1));
}

TEST(CompareZeroPos1, NotEqual)
{
    EXPECT_TRUE(ratl::uint24_t(0) != ratl::uint24_t(1));
}

TEST(CompareZeroPos1, LessThan)
{
    EXPECT_TRUE(ratl::uint24_t(0) < ratl::uint24_t(1));
}

TEST(CompareZeroPos1, LessThanEqual)
{
    EXPECT_TRUE(ratl::uint24_t(0) <= ratl::uint24_t(1));
}

TEST(CompareZeroPos1, GreaterThan)
{
    EXPECT_FALSE(ratl::uint24_t(0) > ratl::uint24_t(1));
}

TEST(CompareZeroPos1, GreaterThanEqual)
{
    EXPECT_FALSE(ratl::uint24_t(0) >= ratl::uint24_t(1));
}

TEST(CompareZeroPos1, EqualInt)
{
    EXPECT_FALSE(ratl::uint24_t(0) == 1);
}

TEST(CompareZeroPos1, NotEqualInt)
{
    EXPECT_TRUE(ratl::uint24_t(0) != 1);
}

TEST(CompareZeroPos1, LessThanInt)
{
    EXPECT_TRUE(ratl::uint24_t(0) < 1);
}

TEST(CompareZeroPos1, LessThanEqualInt)
{
    EXPECT_TRUE(ratl::uint24_t(0) <= 1);
}

TEST(CompareZeroPos1, GreaterThanInt)
{
    EXPECT_FALSE(ratl::uint24_t(0) > 1);
}

TEST(CompareZeroPos1, GreaterThanEqualInt)
{
    EXPECT_FALSE(ratl::uint24_t(0) >= 1);
}

} // namespace test
} // namespace ratl