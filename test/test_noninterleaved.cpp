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

template<class SampleValueType>
class DefaultConstructor : public ::testing::Test
{
protected:
    ratl::noninterleaved<SampleValueType> container_;
};

TYPED_TEST_SUITE(DefaultConstructor, PossibleSampleTypes, );

TYPED_TEST(DefaultConstructor, DataNull)
{
    EXPECT_EQ(this->container_.data(), nullptr);
}

TYPED_TEST(DefaultConstructor, Empty)
{
    EXPECT_TRUE(this->container_.empty());
}

TYPED_TEST(DefaultConstructor, ChannelsZero)
{
    EXPECT_EQ(this->container_.channels(), 0);
}

TYPED_TEST(DefaultConstructor, FramesZero)
{
    EXPECT_EQ(this->container_.frames(), 0);
}

TYPED_TEST(DefaultConstructor, SamplesZero)
{
    EXPECT_EQ(this->container_.samples(), 0);
}

TYPED_TEST(DefaultConstructor, AtBeginThrow)
{
    EXPECT_THROW(this->container_.at(0), std::out_of_range);
}

TYPED_TEST(DefaultConstructor, IteratorEqual)
{
    EXPECT_EQ(this->container_.begin(), this->container_.end());
}

TYPED_TEST(DefaultConstructor, ReverseIteratorEqual)
{
    EXPECT_EQ(this->container_.rbegin(), this->container_.rend());
}

TYPED_TEST(DefaultConstructor, ConstIteratorEqual)
{
    EXPECT_EQ(this->container_.cbegin(), this->container_.cend());
}

TYPED_TEST(DefaultConstructor, ConstReverseIteratorEqual)
{
    EXPECT_EQ(this->container_.crbegin(), this->container_.crend());
}

// ZeroConstructor

template<class SampleValueType>
class ZeroConstructor : public ZeroNoninterleavedBase<SampleValueType>
{
};

TYPED_TEST_SUITE(ZeroConstructor, PossibleSampleTypes, );

TYPED_TEST(ZeroConstructor, DataNull)
{
    EXPECT_EQ(this->container_.data(), nullptr);
}

TYPED_TEST(ZeroConstructor, Empty)
{
    EXPECT_TRUE(this->container_.empty());
}

TYPED_TEST(ZeroConstructor, ChannelsZero)
{
    EXPECT_EQ(this->container_.channels(), 0);
}

TYPED_TEST(ZeroConstructor, FramesZero)
{
    EXPECT_EQ(this->container_.frames(), 0);
}

TYPED_TEST(ZeroConstructor, SamplesZero)
{
    EXPECT_EQ(this->container_.samples(), 0);
}

TYPED_TEST(ZeroConstructor, AtBeginThrow)
{
    EXPECT_THROW(this->container_.at(0), std::out_of_range);
}

TYPED_TEST(ZeroConstructor, IteratorEqual)
{
    EXPECT_EQ(this->container_.begin(), this->container_.end());
}

TYPED_TEST(ZeroConstructor, ReverseIteratorEqual)
{
    EXPECT_EQ(this->container_.rbegin(), this->container_.rend());
}

TYPED_TEST(ZeroConstructor, ConstIteratorEqual)
{
    EXPECT_EQ(this->container_.cbegin(), this->container_.cend());
}

TYPED_TEST(ZeroConstructor, ConstReverseIteratorEqual)
{
    EXPECT_EQ(this->container_.crbegin(), this->container_.crend());
}

// ZeroChannelsConstructor

template<class SampleValueType>
class ZeroChannelsConstructor : public ZeroChannelsNoninterleavedBase<SampleValueType>
{
};

TYPED_TEST_SUITE(ZeroChannelsConstructor, PossibleSampleTypes, );

TYPED_TEST(ZeroChannelsConstructor, DataNull)
{
    EXPECT_EQ(this->container_.data(), nullptr);
}

TYPED_TEST(ZeroChannelsConstructor, Empty)
{
    EXPECT_TRUE(this->container_.empty());
}

TYPED_TEST(ZeroChannelsConstructor, ChannelsZero)
{
    EXPECT_EQ(this->container_.channels(), 0);
}

TYPED_TEST(ZeroChannelsConstructor, Frames)
{
    EXPECT_EQ(this->container_.frames(), this->frames());
}

TYPED_TEST(ZeroChannelsConstructor, SamplesZero)
{
    EXPECT_EQ(this->container_.samples(), 0);
}

TYPED_TEST(ZeroChannelsConstructor, AtBeginThrow)
{
    EXPECT_THROW(this->container_.at(0), std::out_of_range);
}

TYPED_TEST(ZeroChannelsConstructor, AtEndThrow)
{
    EXPECT_THROW(this->container_.at(this->frames()), std::out_of_range);
}

TYPED_TEST(ZeroChannelsConstructor, IteratorEqual)
{
    EXPECT_EQ(this->container_.begin(), this->container_.end());
}

TYPED_TEST(ZeroChannelsConstructor, ReverseIteratorEqual)
{
    EXPECT_EQ(this->container_.rbegin(), this->container_.rend());
}

TYPED_TEST(ZeroChannelsConstructor, ConstIteratorEqual)
{
    EXPECT_EQ(this->container_.cbegin(), this->container_.cend());
}

TYPED_TEST(ZeroChannelsConstructor, ConstReverseIteratorEqual)
{
    EXPECT_EQ(this->container_.crbegin(), this->container_.crend());
}

// ZeroFramesConstructor

template<class SampleValueType>
class ZeroFramesConstructor : public ZeroFramesNoninterleavedBase<SampleValueType>
{
};

TYPED_TEST_SUITE(ZeroFramesConstructor, PossibleSampleTypes, );

TYPED_TEST(ZeroFramesConstructor, DataNull)
{
    EXPECT_EQ(this->container_.data(), nullptr);
}

TYPED_TEST(ZeroFramesConstructor, Empty)
{
    EXPECT_TRUE(this->container_.empty());
}

TYPED_TEST(ZeroFramesConstructor, Channels)
{
    EXPECT_EQ(this->container_.channels(), this->channels());
}

TYPED_TEST(ZeroFramesConstructor, FramesZero)
{
    EXPECT_EQ(this->container_.frames(), 0);
}

TYPED_TEST(ZeroFramesConstructor, SamplesZero)
{
    EXPECT_EQ(this->container_.samples(), 0);
}

TYPED_TEST(ZeroFramesConstructor, AtBeginNoThrow)
{
    EXPECT_NO_THROW(this->container_.at(0));
}

TYPED_TEST(ZeroFramesConstructor, IteratorEqual)
{
    EXPECT_EQ(this->container_.begin(), this->container_.end());
}

TYPED_TEST(ZeroFramesConstructor, ReverseIteratorEqual)
{
    EXPECT_EQ(this->container_.rbegin(), this->container_.rend());
}

TYPED_TEST(ZeroFramesConstructor, ConstIteratorEqual)
{
    EXPECT_EQ(this->container_.cbegin(), this->container_.cend());
}

TYPED_TEST(ZeroFramesConstructor, ConstReverseIteratorEqual)
{
    EXPECT_EQ(this->container_.crbegin(), this->container_.crend());
}

// TypedTypical

template<class SampleValueType>
class TypedTypical : public TypicalNoninterleavedBase<SampleValueType>
{
};

TYPED_TEST_SUITE(TypedTypical, PossibleSampleTypes, );

TYPED_TEST(TypedTypical, DataNotNull)
{
    EXPECT_NE(this->container_.data(), nullptr);
}

TYPED_TEST(TypedTypical, NotEmpty)
{
    EXPECT_FALSE(this->container_.empty());
}

TYPED_TEST(TypedTypical, Channels)
{
    EXPECT_EQ(this->container_.channels(), this->channels());
}

TYPED_TEST(TypedTypical, Frames)
{
    EXPECT_EQ(this->container_.frames(), this->frames());
}

TYPED_TEST(TypedTypical, Samples)
{
    EXPECT_EQ(this->container_.samples(), this->samples());
}

TYPED_TEST(TypedTypical, ZeroInitialised)
{
    auto data = this->container_.data();
    for (size_t i = 0; i < this->container_.samples(); ++i)
    {
        EXPECT_EQ(data[i].get(), 0);
    }
}

TYPED_TEST(TypedTypical, FrameArrayAccessEquality)
{
    for (size_t i = 0; i < this->channels(); ++i)
    {
        EXPECT_EQ(this->container_.channel(i), this->container_[i]);
    }
}

TYPED_TEST(TypedTypical, FrameAtEquality)
{
    for (size_t i = 0; i < this->channels(); ++i)
    {
        EXPECT_EQ(this->container_.channel(i), this->container_.at(i));
    }
}

TYPED_TEST(TypedTypical, AtEndThrow)
{
    EXPECT_THROW(this->container_.at(this->channels()), std::out_of_range);
}

TYPED_TEST(TypedTypical, ChannelFrontEquality)
{
    EXPECT_EQ(this->container_.channel(0), this->container_.front());
}

TYPED_TEST(TypedTypical, ChannelBackEquality)
{
    EXPECT_EQ(this->container_.channel(this->channels() - 1), this->container_.back());
}

TYPED_TEST(TypedTypical, IteratorBaseData)
{
    EXPECT_EQ(this->container_.begin().base(), this->container_.data());
}

TYPED_TEST(TypedTypical, IteratorPlus)
{
    EXPECT_EQ(this->container_.begin() + this->channels(), this->container_.end());
}

TYPED_TEST(TypedTypical, IteratorPlusOverwrite)
{
    auto iter = this->container_.begin();
    iter = iter + this->channels();
    EXPECT_EQ(iter, this->container_.end());
}

TYPED_TEST(TypedTypical, IteratorPlusEquals)
{
    auto iter = this->container_.begin();
    iter += this->channels();
    EXPECT_EQ(iter, this->container_.end());
}

TYPED_TEST(TypedTypical, IteratorMinus)
{
    EXPECT_EQ(this->container_.end() - this->channels(), this->container_.begin());
}

TYPED_TEST(TypedTypical, IteratorMinusOverwrite)
{
    auto iter = this->container_.end();
    iter = iter - this->channels();
    EXPECT_EQ(iter, this->container_.begin());
}

TYPED_TEST(TypedTypical, IteratorMinusEquals)
{
    auto iter = this->container_.end();
    iter -= this->channels();
    EXPECT_EQ(iter, this->container_.begin());
}

TYPED_TEST(TypedTypical, IteratorDistance)
{
    EXPECT_EQ(std::distance(this->container_.begin(), this->container_.end()), this->channels());
}

TYPED_TEST(TypedTypical, IteratorIncrement)
{
    auto iter = this->container_.begin();
    for (size_t i = 0; i < this->channels(); ++i)
    {
        iter++;
    }
    EXPECT_EQ(iter, this->container_.end());
}

TYPED_TEST(TypedTypical, IteratorOffsetIteratorIncrement)
{
    auto iter = this->container_.begin();
    for (size_t i = 0; i < this->channels(); ++i)
    {
        EXPECT_EQ(this->container_.begin() + i, iter++);
    }
}

TYPED_TEST(TypedTypical, IteratorIncrementBaseData)
{
    auto iter = this->container_.begin();
    for (size_t i = 0; i < this->channels(); ++i)
    {
        EXPECT_EQ((iter++).base(), &this->container_.data()[i * this->frames()]);
    }
}

TYPED_TEST(TypedTypical, IteratorDereferenceBeginFront)
{
    EXPECT_EQ(*this->container_.begin(), this->container_.front());
}

TYPED_TEST(TypedTypical, IteratorDereferenceEndBack)
{
    EXPECT_EQ(*(this->container_.end() - 1), this->container_.back());
}

TYPED_TEST(TypedTypical, IteratorOffsetDereferenceChannel)
{
    for (size_t i = 0; i < this->channels(); ++i)
    {
        EXPECT_EQ(*(this->container_.begin() + i), this->container_.channel(i));
    }
}

TYPED_TEST(TypedTypical, IteratorIncrementDereferenceChannel)
{
    auto iter = this->container_.begin();
    for (size_t i = 0; i < this->channels(); ++i)
    {
        EXPECT_EQ(*iter++, this->container_.channel(i));
    }
    EXPECT_EQ(iter, this->container_.end());
}

TYPED_TEST(TypedTypical, IteratorArrayAccessFrame)
{
    for (size_t i = 0; i < this->channels(); ++i)
    {
        EXPECT_EQ(this->container_.begin()[i], this->container_.channel(i));
    }
}

TYPED_TEST(TypedTypical, ReverseIteratorPlus)
{
    EXPECT_EQ(this->container_.rbegin() + this->channels(), this->container_.rend());
}

TYPED_TEST(TypedTypical, ReverseIteratorMinus)
{
    EXPECT_EQ(this->container_.rend() - this->channels(), this->container_.rbegin());
}

TYPED_TEST(TypedTypical, ConstIteratorPlus)
{
    EXPECT_EQ(this->container_.cbegin() + this->channels(), this->container_.cend());
}

TYPED_TEST(TypedTypical, ConstIteratorMinus)
{
    EXPECT_EQ(this->container_.cend() - this->channels(), this->container_.cbegin());
}

TYPED_TEST(TypedTypical, ConstReverseIteratorPlus)
{
    EXPECT_EQ(this->container_.crbegin() + this->channels(), this->container_.crend());
}

TYPED_TEST(TypedTypical, ConstReverseIteratorMinus)
{
    EXPECT_EQ(this->container_.crend() - this->channels(), this->container_.crbegin());
}

TYPED_TEST(TypedTypical, RangeBasedForLoopFrame)
{
    int i = 0;
    for (const auto& channel : this->container_)
    {
        EXPECT_EQ(channel, this->container_.channel(i++));
    }
    EXPECT_EQ(i, this->channels());
}

// Equality

template<class SampleValueType>
class Equality : public ::testing::Test
{
protected:
    using sample_type = SampleValueType;

    static ratl::noninterleaved<sample_type> generate_default()
    {
        return ratl::noninterleaved<sample_type>{};
    }

    static ratl::noninterleaved<sample_type> generate_typical()
    {
        return ratl::noninterleaved<sample_type>{TEST_RATL_TYPICAL_CHANNELS, TEST_RATL_TYPICAL_FRAMES};
    }
};

TYPED_TEST_SUITE(Equality, PossibleSampleTypes, );

TYPED_TEST(Equality, DefaultEqual)
{
    EXPECT_EQ(this->generate_default(), this->generate_default());
}

TYPED_TEST(Equality, TypicalEqual)
{
    EXPECT_EQ(this->generate_typical(), this->generate_typical());
}

TYPED_TEST(Equality, DefaultZeroConstructorEqual)
{
    EXPECT_EQ(this->generate_default(), (ratl::noninterleaved<typename TestFixture::sample_type>{0, 0}));
}

TYPED_TEST(Equality, DefaultZeroChannelsConstructorNotEqual)
{
    EXPECT_NE(
        this->generate_default(),
        (ratl::noninterleaved<typename TestFixture::sample_type>{0, TEST_RATL_TYPICAL_FRAMES}));
}

TYPED_TEST(Equality, DefaultZeroFramesConstructorNotEqual)
{
    EXPECT_NE(
        this->generate_default(),
        (ratl::noninterleaved<typename TestFixture::sample_type>{TEST_RATL_TYPICAL_CHANNELS, 0}));
}

TYPED_TEST(Equality, DefaultTypicalNotEqual)
{
    EXPECT_NE(this->generate_default(), this->generate_typical());
}

TYPED_TEST(Equality, TypicalAllWriteNotEqual)
{
    auto typical = this->generate_typical();
    std::fill_n(typical.data(), typical.samples(), 1);
    EXPECT_NE(typical, this->generate_typical());
}

TYPED_TEST(Equality, TypicalAllWriteBackEqual)
{
    auto typical = this->generate_typical();
    std::fill_n(typical.data(), typical.samples(), 1);
    std::fill_n(typical.data(), typical.samples(), 0);
    EXPECT_EQ(typical, this->generate_typical());
}

TYPED_TEST(Equality, TypicalOneWriteNotEqual)
{
    auto typical = this->generate_typical();
    size_t middle_sample = (TEST_RATL_TYPICAL_CHANNELS * TEST_RATL_TYPICAL_FRAMES) / 2;
    typical.data()[middle_sample] = 1;
    EXPECT_NE(typical, this->generate_typical());
}

TYPED_TEST(Equality, TypicalOneWriteBackEqual)
{
    auto typical = this->generate_typical();
    size_t middle_sample = (TEST_RATL_TYPICAL_CHANNELS * TEST_RATL_TYPICAL_FRAMES) / 2;
    typical.data()[middle_sample] = 1;
    typical.data()[middle_sample] = 0;
    EXPECT_EQ(typical, this->generate_typical());
}

// CopyConstructor

template<class SampleValueType>
class CopyConstructor : public CopyNoninterleavedBase<SampleValueType>
{
protected:
    using super_type = TypicalNoninterleavedBase<SampleValueType>;
    using noninterleaved_type = typename super_type::container_type;
};

TYPED_TEST_SUITE(CopyConstructor, PossibleSampleTypes, );

TYPED_TEST(CopyConstructor, CopyConstructor)
{
    using noninterleaved_type = typename TestFixture::noninterleaved_type;
    noninterleaved_type noninterleaved_copy = this->container_;
    EXPECT_EQ(noninterleaved_copy, this->generate());
}

// CopyAssignmentOperator

template<class SampleValueType>
class CopyAssignmentOperator : public CopyNoninterleavedBase<SampleValueType>
{
protected:
    using super_type = TypicalNoninterleavedBase<SampleValueType>;
    using noninterleaved_type = typename super_type::container_type;
};

TYPED_TEST_SUITE(CopyAssignmentOperator, PossibleSampleTypes, );

TYPED_TEST(CopyAssignmentOperator, CopyAssignmentOperator)
{
    using noninterleaved_type = typename TestFixture::noninterleaved_type;
    noninterleaved_type noninterleaved_copy;
    noninterleaved_copy = this->container_;
    EXPECT_EQ(noninterleaved_copy, this->generate());
}

// MoveConstructor

template<class SampleValueType>
class MoveConstructor : public MoveNoninterleavedBase<SampleValueType>
{
protected:
    using super_type = TypicalNoninterleavedBase<SampleValueType>;
    using noninterleaved_type = typename super_type::container_type;
};

TYPED_TEST_SUITE(MoveConstructor, PossibleSampleTypes, );

TYPED_TEST(MoveConstructor, MoveConstructor)
{
    using noninterleaved_type = typename TestFixture::noninterleaved_type;
    noninterleaved_type noninterleaved_move = std::move(this->container_);
    EXPECT_EQ(noninterleaved_move, this->generate());
}

// MoveAssignmentOperator

template<class SampleValueType>
class MoveAssignmentOperator : public MoveNoninterleavedBase<SampleValueType>
{
protected:
    using super_type = TypicalNoninterleavedBase<SampleValueType>;
    using noninterleaved_type = typename super_type::container_type;
};

TYPED_TEST_SUITE(MoveAssignmentOperator, PossibleSampleTypes, );

TYPED_TEST(MoveAssignmentOperator, MoveAssignmentOperator)
{
    using noninterleaved_type = typename TestFixture::noninterleaved_type;
    noninterleaved_type noninterleaved_move;
    noninterleaved_move = std::move(this->container_);
    EXPECT_EQ(noninterleaved_move, this->generate());
}

// Swap

template<class SampleValueType>
class Swap : public TypicalNoninterleavedBase<SampleValueType>
{
protected:
    using super_type = TypicalNoninterleavedBase<SampleValueType>;
    using noninterleaved_type = typename super_type::container_type;

    static noninterleaved_type generate_swap()
    {
        return noninterleaved_type{super_type::generate().channels() + 10, super_type::generate().frames() + 50};
    }
};

TYPED_TEST_SUITE(Swap, PossibleSampleTypes, );

TYPED_TEST(Swap, SwapFree)
{
    auto interleaved_swap = this->generate_swap();
    std::swap(this->container_, interleaved_swap);
    EXPECT_EQ(this->container_, this->generate_swap());
    EXPECT_EQ(interleaved_swap, this->generate());
}

TYPED_TEST(Swap, SwapMember)
{
    auto interleaved_swap = this->generate_swap();
    interleaved_swap.swap(this->container_);
    EXPECT_EQ(this->container_, this->generate_swap());
    EXPECT_EQ(interleaved_swap, this->generate());
}

// allocator

template<class SampleValueType>
class Allocate : public TypicalNoninterleavedBase<SampleValueType>
{
protected:
    using super_type = TypicalNoninterleavedBase<SampleValueType>;
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

        allocator() : n_{0}, p_{nullptr} {}

        virtual ~allocator()
        {
            EXPECT_EQ(n_, 0);
            EXPECT_EQ(p_, nullptr);
        }

        allocator(const allocator& other) : allocator{}
        {
            EXPECT_EQ(other.n_, 0);
            EXPECT_EQ(other.p_, nullptr);
        }

        allocator& operator=(const allocator&) = delete;

        allocator(allocator&& other) : n_{other.n_}, p_{other.p_}
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
            return allocator{};
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

    using noninterleaved_type = ratl::basic_noninterleaved<sample_type, allocator>;
};

TYPED_TEST_SUITE(Allocate, PossibleSampleTypes, );

TYPED_TEST(Allocate, TypicalAllocate)
{
    using noninterleaved_type = typename TestFixture::noninterleaved_type;
    noninterleaved_type(this->channels(), this->frames(), typename TestFixture::allocator{});
}

TYPED_TEST(Allocate, CopyConstructor)
{
    using noninterleaved_type = typename TestFixture::noninterleaved_type;
    noninterleaved_type interleaved_allocated(this->channels(), this->frames(), typename TestFixture::allocator{});
    noninterleaved_type(interleaved_allocated, typename TestFixture::allocator{});
}

TYPED_TEST(Allocate, CopyAssignmentOperator)
{
    using noninterleaved_type = typename TestFixture::noninterleaved_type;
    noninterleaved_type interleaved_allocated_1(this->channels(), this->frames(), typename TestFixture::allocator{});
    noninterleaved_type interleaved_allocated_2(this->channels(), this->frames(), typename TestFixture::allocator{});
    interleaved_allocated_1 = interleaved_allocated_2;
}

TYPED_TEST(Allocate, MoveConstructorOperator)
{
    using noninterleaved_type = typename TestFixture::noninterleaved_type;
    noninterleaved_type interleaved_allocated(this->channels(), this->frames(), typename TestFixture::allocator{});
    noninterleaved_type(interleaved_allocated, typename TestFixture::allocator{});
}

TYPED_TEST(Allocate, MoveAssignmentOperator)
{
    using noninterleaved_type = typename TestFixture::noninterleaved_type;
    noninterleaved_type interleaved_allocated(this->channels(), this->frames(), typename TestFixture::allocator{});
    interleaved_allocated = noninterleaved_type(this->channels(), this->frames(), typename TestFixture::allocator{});
}

TYPED_TEST(Allocate, Swap)
{
    using noninterleaved_type = typename TestFixture::noninterleaved_type;
    noninterleaved_type interleaved_allocated_1(this->channels(), this->frames(), typename TestFixture::allocator{});
    noninterleaved_type interleaved_allocated_2(this->channels(), this->frames(), typename TestFixture::allocator{});
    std::swap(interleaved_allocated_1, interleaved_allocated_2);
}

// ReadWrite

template<class SampleValueType>
class RawReadWrite : public TypicalNoninterleavedBase<SampleValueType>
{
};

TYPED_TEST_SUITE(RawReadWrite, PossibleSampleTypes, );

TYPED_TEST(RawReadWrite, Write)
{
    auto iter = std::fill_n(this->container_.data(), this->container_.samples(), 1);
    EXPECT_EQ(iter, this->container_.data() + this->container_.samples());
}

TYPED_TEST(RawReadWrite, Read)
{
    std::fill_n(this->container_.data(), this->container_.samples(), 1);

    for (size_t i = 0; i < this->container_.samples(); ++i)
    {
        EXPECT_EQ(this->container_.data()[i].get(), 1);
    }
}

} // namespace test
} // namespace ratl
