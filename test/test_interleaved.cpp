/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

// ratl test includes
#include "test_utils.hpp"

namespace ratl
{
namespace test
{
// DefaultConstructor

template<typename InterleavedWrapper>
class DefaultConstructor : public ::testing::Test
{
    using container_type = typename InterleavedWrapper::container_type;

    container_type container_;

public:
    container_type& container()
    {
        return container_;
    }
};

TYPED_TEST_SUITE(DefaultConstructor, PossibleInterleaved, );

TYPED_TEST(DefaultConstructor, DataNull)
{
    EXPECT_EQ(this->container().data(), nullptr);
}

TYPED_TEST(DefaultConstructor, Empty)
{
    EXPECT_TRUE(this->container().empty());
}

TYPED_TEST(DefaultConstructor, ChannelsZero)
{
    EXPECT_EQ(this->container().channels(), 0);
}

TYPED_TEST(DefaultConstructor, FramesZero)
{
    EXPECT_EQ(this->container().frames(), 0);
}

TYPED_TEST(DefaultConstructor, SamplesZero)
{
    EXPECT_EQ(this->container().samples(), 0);
}

TYPED_TEST(DefaultConstructor, AtBeginThrow)
{
    EXPECT_THROW(this->container().at(0), std::out_of_range);
}

TYPED_TEST(DefaultConstructor, IteratorEqual)
{
    EXPECT_EQ(this->container().begin(), this->container().end());
}

TYPED_TEST(DefaultConstructor, ReverseIteratorEqual)
{
    EXPECT_EQ(this->container().rbegin(), this->container().rend());
}

TYPED_TEST(DefaultConstructor, ConstIteratorEqual)
{
    EXPECT_EQ(this->container().cbegin(), this->container().cend());
}

TYPED_TEST(DefaultConstructor, ConstReverseIteratorEqual)
{
    EXPECT_EQ(this->container().crbegin(), this->container().crend());
}

// ZeroConstructor

template<typename InterleavedWrapper>
class ZeroConstructor : public ZeroFramesAndChannelsTestBase<InterleavedWrapper>
{
};

TYPED_TEST_SUITE(ZeroConstructor, PossibleInterleaved, );

TYPED_TEST(ZeroConstructor, DataNull)
{
    EXPECT_EQ(this->container().data(), nullptr);
}

TYPED_TEST(ZeroConstructor, Empty)
{
    EXPECT_TRUE(this->container().empty());
}

TYPED_TEST(ZeroConstructor, ChannelsZero)
{
    EXPECT_EQ(this->container().channels(), 0);
}

TYPED_TEST(ZeroConstructor, FramesZero)
{
    EXPECT_EQ(this->container().frames(), 0);
}

TYPED_TEST(ZeroConstructor, SamplesZero)
{
    EXPECT_EQ(this->container().samples(), 0);
}

TYPED_TEST(ZeroConstructor, AtBeginThrow)
{
    EXPECT_THROW(this->container().at(0), std::out_of_range);
}

TYPED_TEST(ZeroConstructor, IteratorEqual)
{
    EXPECT_EQ(this->container().begin(), this->container().end());
}

TYPED_TEST(ZeroConstructor, ReverseIteratorEqual)
{
    EXPECT_EQ(this->container().rbegin(), this->container().rend());
}

TYPED_TEST(ZeroConstructor, ConstIteratorEqual)
{
    EXPECT_EQ(this->container().cbegin(), this->container().cend());
}

TYPED_TEST(ZeroConstructor, ConstReverseIteratorEqual)
{
    EXPECT_EQ(this->container().crbegin(), this->container().crend());
}

// ZeroChannelsConstructor

template<typename InterleavedWrapper>
class ZeroChannelsConstructor : public ZeroChannelsTestBase<InterleavedWrapper>
{
};

TYPED_TEST_SUITE(ZeroChannelsConstructor, PossibleInterleaved, );

TYPED_TEST(ZeroChannelsConstructor, DataNull)
{
    EXPECT_EQ(this->container().data(), nullptr);
}

TYPED_TEST(ZeroChannelsConstructor, Empty)
{
    EXPECT_TRUE(this->container().empty());
}

TYPED_TEST(ZeroChannelsConstructor, ChannelsZero)
{
    EXPECT_EQ(this->container().channels(), 0);
}

TYPED_TEST(ZeroChannelsConstructor, Frames)
{
    EXPECT_EQ(this->container().frames(), this->configured_frames());
}

TYPED_TEST(ZeroChannelsConstructor, SamplesZero)
{
    EXPECT_EQ(this->container().samples(), 0);
}

TYPED_TEST(ZeroChannelsConstructor, AtBeginNoThrow)
{
    EXPECT_NO_THROW(this->container().at(0));
}

TYPED_TEST(ZeroChannelsConstructor, AtEndThrow)
{
    EXPECT_THROW(this->container().at(this->configured_frames()), std::out_of_range);
}

TYPED_TEST(ZeroChannelsConstructor, IteratorEqual)
{
    EXPECT_EQ(this->container().begin(), this->container().end());
}

TYPED_TEST(ZeroChannelsConstructor, ReverseIteratorEqual)
{
    EXPECT_EQ(this->container().rbegin(), this->container().rend());
}

TYPED_TEST(ZeroChannelsConstructor, ConstIteratorEqual)
{
    EXPECT_EQ(this->container().cbegin(), this->container().cend());
}

TYPED_TEST(ZeroChannelsConstructor, ConstReverseIteratorEqual)
{
    EXPECT_EQ(this->container().crbegin(), this->container().crend());
}

// ZeroFramesConstructor

template<typename InterleavedWrapper>
class ZeroFramesConstructor : public ZeroFramesTestBase<InterleavedWrapper>
{
};

TYPED_TEST_SUITE(ZeroFramesConstructor, PossibleInterleaved, );

TYPED_TEST(ZeroFramesConstructor, DataNull)
{
    EXPECT_EQ(this->container().data(), nullptr);
}

TYPED_TEST(ZeroFramesConstructor, Empty)
{
    EXPECT_TRUE(this->container().empty());
}

TYPED_TEST(ZeroFramesConstructor, Channels)
{
    EXPECT_EQ(this->container().channels(), this->configured_channels());
}

TYPED_TEST(ZeroFramesConstructor, FramesZero)
{
    EXPECT_EQ(this->container().frames(), 0);
}

TYPED_TEST(ZeroFramesConstructor, SamplesZero)
{
    EXPECT_EQ(this->container().samples(), 0);
}

TYPED_TEST(ZeroFramesConstructor, AtBeginThrow)
{
    EXPECT_THROW(this->container().at(0), std::out_of_range);
}

TYPED_TEST(ZeroFramesConstructor, IteratorEqual)
{
    EXPECT_EQ(this->container().begin(), this->container().end());
}

TYPED_TEST(ZeroFramesConstructor, ReverseIteratorEqual)
{
    EXPECT_EQ(this->container().rbegin(), this->container().rend());
}

TYPED_TEST(ZeroFramesConstructor, ConstIteratorEqual)
{
    EXPECT_EQ(this->container().cbegin(), this->container().cend());
}

TYPED_TEST(ZeroFramesConstructor, ConstReverseIteratorEqual)
{
    EXPECT_EQ(this->container().crbegin(), this->container().crend());
}

// TypedTypical

template<typename InterleavedWrapper>
class TypedTypical : public TypicalTestBase<InterleavedWrapper>
{
};

TYPED_TEST_SUITE(TypedTypical, PossibleInterleaved, );

TYPED_TEST(TypedTypical, DataNotNull)
{
    EXPECT_NE(this->container().data(), nullptr);
}

TYPED_TEST(TypedTypical, NotEmpty)
{
    EXPECT_FALSE(this->container().empty());
}

TYPED_TEST(TypedTypical, Channels)
{
    EXPECT_EQ(this->container().channels(), this->configured_channels());
}

TYPED_TEST(TypedTypical, Frames)
{
    EXPECT_EQ(this->container().frames(), this->configured_frames());
}

TYPED_TEST(TypedTypical, Samples)
{
    EXPECT_EQ(this->container().samples(), this->configured_samples());
}

TYPED_TEST(TypedTypical, ZeroInitialised)
{
    auto data = this->container().data();
    for (size_t i = 0; i < this->container().samples(); ++i)
    {
        EXPECT_EQ(data[i].get(), typename TestFixture::sample_value_type());
    }
}

TYPED_TEST(TypedTypical, FrameArrayAccessEquality)
{
    for (size_t i = 0; i < this->configured_frames(); ++i)
    {
        EXPECT_TRUE(IsContainerEqual(this->container().frame(i), this->container()[i]));
    }
}

TYPED_TEST(TypedTypical, FrameAtEquality)
{
    for (size_t i = 0; i < this->configured_frames(); ++i)
    {
        EXPECT_TRUE(IsContainerEqual(this->container().frame(i), this->container().at(i)));
    }
}

TYPED_TEST(TypedTypical, AtEndThrow)
{
    EXPECT_THROW(this->container().at(this->configured_frames()), std::out_of_range);
}

TYPED_TEST(TypedTypical, FrameFrontEquality)
{
    EXPECT_TRUE(IsContainerEqual(this->container().frame(0), this->container().front()));
}

TYPED_TEST(TypedTypical, FrameBackEquality)
{
    EXPECT_TRUE(IsContainerEqual(this->container().frame(this->configured_frames() - 1), this->container().back()));
}

TYPED_TEST(TypedTypical, IteratorBaseData)
{
    EXPECT_EQ(this->container().begin().base(), this->container().data());
}

TYPED_TEST(TypedTypical, IteratorPlus)
{
    EXPECT_EQ(this->container().begin() + this->configured_frames(), this->container().end());
}

TYPED_TEST(TypedTypical, IteratorPlusOverwrite)
{
    auto iter = this->container().begin();
    iter = iter + this->configured_frames();
    EXPECT_EQ(iter, this->container().end());
}

TYPED_TEST(TypedTypical, IteratorPlusEquals)
{
    auto iter = this->container().begin();
    iter += this->configured_frames();
    EXPECT_EQ(iter, this->container().end());
}

TYPED_TEST(TypedTypical, IteratorMinus)
{
    EXPECT_EQ(this->container().end() - this->configured_frames(), this->container().begin());
}

TYPED_TEST(TypedTypical, IteratorMinusOverwrite)
{
    auto iter = this->container().end();
    iter = iter - this->configured_frames();
    EXPECT_EQ(iter, this->container().begin());
}

TYPED_TEST(TypedTypical, IteratorMinusEquals)
{
    auto iter = this->container().end();
    iter -= this->configured_frames();
    EXPECT_EQ(iter, this->container().begin());
}

TYPED_TEST(TypedTypical, IteratorDistance)
{
    EXPECT_EQ(
        static_cast<std::size_t>(std::distance(this->container().begin(), this->container().end())),
        this->configured_frames());
}

TYPED_TEST(TypedTypical, IteratorIncrement)
{
    auto iter = this->container().begin();
    for (size_t i = 0; i < this->configured_frames(); ++i)
    {
        iter++;
    }
    EXPECT_EQ(iter, this->container().end());
}

TYPED_TEST(TypedTypical, IteratorOffsetIteratorIncrement)
{
    auto iter = this->container().begin();
    for (size_t i = 0; i < this->configured_frames(); ++i)
    {
        EXPECT_EQ(this->container().begin() + i, iter++);
    }
}

TYPED_TEST(TypedTypical, IteratorIncrementBaseData)
{
    auto iter = this->container().begin();
    for (size_t i = 0; i < this->configured_frames(); ++i)
    {
        EXPECT_EQ((iter++).base(), &this->container().data()[i * this->configured_channels()]);
    }
}

TYPED_TEST(TypedTypical, IteratorDereferenceBeginFront)
{
    EXPECT_TRUE(IsContainerEqual(*this->container().begin(), this->container().front()));
}

TYPED_TEST(TypedTypical, IteratorDereferenceEndBack)
{
    EXPECT_TRUE(IsContainerEqual(*(this->container().end() - 1), this->container().back()));
}

TYPED_TEST(TypedTypical, IteratorOffsetDereferenceFrame)
{
    for (size_t i = 0; i < this->configured_frames(); ++i)
    {
        EXPECT_TRUE(IsContainerEqual(*(this->container().begin() + i), this->container().frame(i)));
    }
}

TYPED_TEST(TypedTypical, IteratorIncrementDereferenceFrame)
{
    auto iter = this->container().begin();
    for (size_t i = 0; i < this->configured_frames(); ++i)
    {
        EXPECT_TRUE(IsContainerEqual(*iter++, this->container().frame(i)));
    }
    EXPECT_EQ(iter, this->container().end());
}

TYPED_TEST(TypedTypical, IteratorArrayAccessFrame)
{
    for (size_t i = 0; i < this->configured_frames(); ++i)
    {
        EXPECT_TRUE(IsContainerEqual(this->container().begin()[i], this->container().frame(i)));
    }
}

TYPED_TEST(TypedTypical, ReverseIteratorPlus)
{
    EXPECT_EQ(this->container().rbegin() + this->configured_frames(), this->container().rend());
}

TYPED_TEST(TypedTypical, ReverseIteratorMinus)
{
    EXPECT_EQ(this->container().rend() - this->configured_frames(), this->container().rbegin());
}

TYPED_TEST(TypedTypical, ConstIteratorPlus)
{
    EXPECT_EQ(this->container().cbegin() + this->configured_frames(), this->container().cend());
}

TYPED_TEST(TypedTypical, ConstIteratorMinus)
{
    EXPECT_EQ(this->container().cend() - this->configured_frames(), this->container().cbegin());
}

TYPED_TEST(TypedTypical, ConstReverseIteratorPlus)
{
    EXPECT_EQ(this->container().crbegin() + this->configured_frames(), this->container().crend());
}

TYPED_TEST(TypedTypical, ConstReverseIteratorMinus)
{
    EXPECT_EQ(this->container().crend() - this->configured_frames(), this->container().crbegin());
}

TYPED_TEST(TypedTypical, RangeBasedForLoopFrame)
{
    int i = 0;
    for (const auto& frame : this->container())
    {
        EXPECT_TRUE(IsContainerEqual(frame, this->container().frame(i++)));
    }
    EXPECT_EQ(i, this->configured_frames());
}

// Equality

template<typename InterleavedWrapper>
class Equality : public TypicalTestBase<InterleavedWrapper>
{
};

TYPED_TEST_SUITE(Equality, PossibleInterleaved, );

TYPED_TEST(Equality, DefaultEqual)
{
    EXPECT_TRUE(IsContainerContentEqual(
        TestFixture::generate_default_wrapper().container(), TestFixture::generate_default_wrapper().container()));
}

TYPED_TEST(Equality, TypicalEqual)
{
    EXPECT_TRUE(IsContainerContentEqual(
        TestFixture::generate_configured_wrapper().container(),
        TestFixture::generate_configured_wrapper().container()));
}

TYPED_TEST(Equality, DefaultZeroConstructorEqual)
{
    EXPECT_TRUE(IsContainerContentEqual(
        TestFixture::generate_default_wrapper().container(), TestFixture::generate_wrapper(0, 0).container()));
}

TYPED_TEST(Equality, DefaultZeroChannelsConstructorNotEqual)
{
    EXPECT_TRUE(IsContainerContentNotEqual(
        TestFixture::generate_default_wrapper().container(),
        TestFixture::generate_wrapper(0, TEST_RATL_TYPICAL_FRAMES).container()));
}

TYPED_TEST(Equality, DefaultZeroFramesConstructorNotEqual)
{
    EXPECT_TRUE(IsContainerContentNotEqual(
        TestFixture::generate_default_wrapper().container(),
        TestFixture::generate_wrapper(TEST_RATL_TYPICAL_CHANNELS, 0).container()));
}

TYPED_TEST(Equality, DefaultTypicalNotEqual)
{
    EXPECT_TRUE(IsContainerContentNotEqual(
        TestFixture::generate_default_wrapper().container(), TestFixture::generate_configured_wrapper().container()));
}

TYPED_TEST(Equality, TypicalAllWriteNotEqual)
{
    auto typical = TestFixture::generate_configured_wrapper();
    std::fill_n(typical.storage().data(), typical.container().samples(), TestFixture::make_sample(1000000));
    EXPECT_TRUE(
        IsContainerContentNotEqual(typical.container(), TestFixture::generate_configured_wrapper().container()));
}

TYPED_TEST(Equality, TypicalAllWriteBackEqual)
{
    auto typical = TestFixture::generate_configured_wrapper();
    std::fill_n(typical.storage().data(), typical.container().samples(), TestFixture::make_sample(1000000));
    std::fill_n(typical.storage().data(), typical.container().samples(), TestFixture::make_sample(0));
    EXPECT_TRUE(IsContainerContentEqual(typical.container(), TestFixture::generate_configured_wrapper().container()));
}

TYPED_TEST(Equality, TypicalOneWriteNotEqual)
{
    auto typical = TestFixture::generate_configured_wrapper();
    size_t middle_index = typical.container().samples() / 2;
    typical.storage().data()[middle_index] = TestFixture::make_sample(1000000);
    EXPECT_TRUE(
        IsContainerContentNotEqual(typical.container(), TestFixture::generate_configured_wrapper().container()));
}

TYPED_TEST(Equality, TypicalOneWriteBackEqual)
{
    auto typical = TestFixture::generate_configured_wrapper();
    auto middle_index = typical.container().samples() / 2;
    typical.storage().data()[middle_index] = TestFixture::make_sample(1000000);
    typical.storage().data()[middle_index] = TestFixture::make_sample(0);
    EXPECT_TRUE(IsContainerContentEqual(typical.container(), TestFixture::generate_configured_wrapper().container()));
}

// Copy

template<typename InterleavedWrapper>
class Copy : public CopyTestBase<InterleavedWrapper>
{
};

TYPED_TEST_SUITE(Copy, PossibleInterleaved, );

TYPED_TEST(Copy, CopyConstructor)
{
    auto copy = this->container();
    EXPECT_TRUE(IsContainerEqual(copy, this->container()));
}

TYPED_TEST(Copy, CopyAssignmentOperator)
{
    typename TestFixture::container_type copy;
    copy = this->container();
    EXPECT_TRUE(IsContainerEqual(copy, this->container()));
}

// BufferMove

template<typename InterleavedWrapper>
class BufferMove : public MoveTestBase<InterleavedWrapper>
{
};

TYPED_TEST_SUITE(BufferMove, PossibleInterleavedBuffers, );

TYPED_TEST(BufferMove, MoveConstructor)
{
    auto copy = this->container();
    auto move = std::move(this->container());
    EXPECT_TRUE(IsContainerEqual(move, copy));
}

TYPED_TEST(BufferMove, MoveAssignmentOperator)
{
    auto copy = this->container();
    typename TestFixture::container_type move;
    move = std::move(this->container());
    EXPECT_TRUE(IsContainerEqual(move, copy));
}

// SpanMove

template<typename InterleavedWrapper>
class SpanMove : public CopyTestBase<InterleavedWrapper>
{
};

TYPED_TEST_SUITE(SpanMove, PossibleInterleavedSpans, );

TYPED_TEST(SpanMove, MoveConstructor)
{
    auto copy = this->container();
    auto move = std::move(this->container());
    EXPECT_TRUE(IsContainerEqual(move, copy));
}

TYPED_TEST(SpanMove, MoveAssignmentOperator)
{
    auto copy = this->container();
    typename TestFixture::container_type move;
    move = std::move(this->container());
    EXPECT_TRUE(IsContainerEqual(move, copy));
}

// Swap

template<typename InterleavedWrapper>
class Swap : public TypicalTestBase<InterleavedWrapper>
{
    using super_type = typename Swap::TypicalTestBase;
    using wrapper_type = typename super_type::wrapper_type;
    using container_type = typename super_type::container_type;

public:
    static wrapper_type generate_swap_wrapper()
    {
        return super_type::generate_wrapper(
            super_type::configured_channels() + 10, super_type::configured_frames() + 50);
    }
};

TYPED_TEST_SUITE(Swap, PossibleInterleaved, );

TYPED_TEST(Swap, SwapFree)
{
    auto swap = this->generate_swap_wrapper();
    std::swap(this->container(), swap.container());
    EXPECT_TRUE(IsContainerContentEqual(this->container(), this->generate_swap_wrapper().container()));
    EXPECT_TRUE(IsContainerContentEqual(swap.container(), this->generate_configured_wrapper().container()));
}

TYPED_TEST(Swap, SwapMember)
{
    auto swap = this->generate_swap_wrapper();
    swap.container().swap(this->container());
    EXPECT_TRUE(IsContainerContentEqual(this->container(), this->generate_swap_wrapper().container()));
    EXPECT_TRUE(IsContainerContentEqual(swap.container(), this->generate_configured_wrapper().container()));
}

// allocator

template<typename InterleavedWrapper>
class Allocate : public TypicalTestBase<InterleavedWrapper>
{
public:
    using super_type = typename Allocate::TypicalTestBase;
    using container_type = typename super_type::container_type;
    using sample_type = typename super_type::sample_type;

    class allocator : public ratl::allocator<sample_type>
    {
    public:
        using super_type = ratl::allocator<sample_type>;

        using value_type = typename super_type::value_type;
        using size_type = typename super_type::size_type;
        using difference_type = typename super_type::difference_type;
#if !defined(RATL_CPP_VERSION_HAS_CPP20)
        using pointer = typename super_type::pointer;
        using const_pointer = typename super_type::const_pointer;
        using reference = typename super_type::reference;
        using const_reference = typename super_type::const_reference;
#endif

        using propagate_on_container_copy_assignment = std::false_type;
        using propagate_on_container_move_assignment = std::true_type;
        using propagate_on_container_swap = std::true_type;

        allocator() : n_(0), p_() {}

        virtual ~allocator()
        {
            EXPECT_EQ(n_, 0);
            EXPECT_EQ(p_, nullptr);
        }

        allocator(const allocator& other) : allocator()
        {
            EXPECT_EQ(other.n_, 0);
            EXPECT_EQ(other.p_, nullptr);
        }

        allocator& operator=(const allocator&) = delete;

        allocator(allocator&& other) : n_(other.n_), p_(other.p_)
        {
            other.n_ = 0;
            other.p_ = nullptr;
        }

        allocator& operator=(allocator&& other)
        {
            std::swap(n_, other.n_);
            std::swap(p_, other.p_);
            return *this;
        }

        virtual sample_type* allocate(size_type n)
        {
            EXPECT_EQ(n_, 0);
            EXPECT_EQ(p_, nullptr);

            n_ = n;
            p_ = super_type::allocate(n_);
            return p_;
        }

        virtual void deallocate(sample_type* p, size_type n)
        {
            EXPECT_EQ(p, p_);
            EXPECT_EQ(n, n_);

            super_type::deallocate(p_, n_);
            n_ = 0;
            p_ = nullptr;
        }

        allocator select_on_container_copy_construction() const
        {
            return allocator();
        }

        bool operator==(const allocator& other) const
        {
            return ((n_ == other.n_) && (p_ == other.p_));
        }

        bool operator!=(const allocator& other) const
        {
            return !(*this == other);
        }

    private:
        size_type n_;
        sample_type* p_;
    };
};

TYPED_TEST_SUITE(Allocate, PossibleInterleavedBuffers, );

TYPED_TEST(Allocate, TypicalAllocate)
{
    using container_type = typename TestFixture::container_type;
    container_type(this->container().channels(), this->container().frames(), typename TestFixture::allocator());
}

TYPED_TEST(Allocate, CopyConstructor)
{
    using container_type = typename TestFixture::container_type;
    container_type container_allocated(
        this->container().channels(), this->container().frames(), typename TestFixture::allocator());
    container_type(container_allocated, typename TestFixture::allocator());
}

TYPED_TEST(Allocate, CopyAssignmentOperator)
{
    using container_type = typename TestFixture::container_type;
    container_type container_allocated_1(
        this->container().channels(), this->container().frames(), typename TestFixture::allocator());
    container_type container_allocated_2(
        this->container().channels(), this->container().frames(), typename TestFixture::allocator());
    container_allocated_1 = container_allocated_2;
}

TYPED_TEST(Allocate, MoveConstructorOperator)
{
    using container_type = typename TestFixture::container_type;
    container_type container_allocated(
        this->container().channels(), this->container().frames(), typename TestFixture::allocator());
    container_type(container_allocated, typename TestFixture::allocator());
}

TYPED_TEST(Allocate, MoveAssignmentOperator)
{
    using container_type = typename TestFixture::container_type;
    container_type container_allocated(
        this->container().channels(), this->container().frames(), typename TestFixture::allocator());
    container_allocated =
        container_type(this->container().channels(), this->container().frames(), typename TestFixture::allocator());
}

TYPED_TEST(Allocate, Swap)
{
    using container_type = typename TestFixture::container_type;
    container_type container_allocated_1(
        this->container().channels(), this->container().frames(), typename TestFixture::allocator());
    container_type container_allocated_2(
        this->container().channels(), this->container().frames(), typename TestFixture::allocator());
    std::swap(container_allocated_1, container_allocated_2);
}

// ReadWrite

template<typename InterleavedWrapper>
class RawReadWrite : public TypicalTestBase<InterleavedWrapper>
{
};

TYPED_TEST_SUITE(RawReadWrite, PossibleNonConstInterleaved, );

TYPED_TEST(RawReadWrite, Write)
{
    auto iter = std::fill_n(this->container().data(), this->container().samples(), TestFixture::make_sample(1));
    EXPECT_EQ(iter, this->container().data() + this->container().samples());
}

TYPED_TEST(RawReadWrite, Read)
{
    std::fill_n(this->container().data(), this->container().samples(), TestFixture::make_sample(1));

    for (size_t i = 0; i < this->container().samples(); ++i)
    {
        EXPECT_EQ(this->container().data()[i].get(), TestFixture::make_sample(1).get());
    }
}

} // namespace test
} // namespace ratl
