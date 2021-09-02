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

template<typename SampleValueType>
class DefaultConstructor : public ::testing::Test
{
protected:
    ratl::interleaved<SampleValueType> container_;
};

TYPED_TEST_SUITE(DefaultConstructor, PossibleSampleValueTypes, );

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

template<typename SampleValueType>
class ZeroConstructor : public ZeroInterleavedBase<SampleValueType>
{
};

TYPED_TEST_SUITE(ZeroConstructor, PossibleSampleValueTypes, );

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

template<typename SampleValueType>
class ZeroChannelsConstructor : public ZeroChannelsInterleavedBase<SampleValueType>
{
};

TYPED_TEST_SUITE(ZeroChannelsConstructor, PossibleSampleValueTypes, );

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

TYPED_TEST(ZeroChannelsConstructor, AtBeginNoThrow)
{
    EXPECT_NO_THROW(this->container_.at(0));
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

template<typename SampleValueType>
class ZeroFramesConstructor : public ZeroFramesInterleavedBase<SampleValueType>
{
};

TYPED_TEST_SUITE(ZeroFramesConstructor, PossibleSampleValueTypes, );

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

TYPED_TEST(ZeroFramesConstructor, AtBeginThrow)
{
    EXPECT_THROW(this->container_.at(0), std::out_of_range);
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

template<typename SampleValueType>
class TypedTypical : public TypicalInterleavedBase<SampleValueType>
{
};

TYPED_TEST_SUITE(TypedTypical, PossibleSampleValueTypes, );

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
    for (size_t i = 0; i < this->frames(); ++i)
    {
        EXPECT_EQ(this->container_.frame(i), this->container_[i]);
    }
}

TYPED_TEST(TypedTypical, FrameAtEquality)
{
    for (size_t i = 0; i < this->frames(); ++i)
    {
        EXPECT_EQ(this->container_.frame(i), this->container_.at(i));
    }
}

TYPED_TEST(TypedTypical, AtEndThrow)
{
    EXPECT_THROW(this->container_.at(this->frames()), std::out_of_range);
}

TYPED_TEST(TypedTypical, FrameFrontEquality)
{
    EXPECT_EQ(this->container_.frame(0), this->container_.front());
}

TYPED_TEST(TypedTypical, FrameBackEquality)
{
    EXPECT_EQ(this->container_.frame(this->frames() - 1), this->container_.back());
}

TYPED_TEST(TypedTypical, IteratorBaseData)
{
    EXPECT_EQ(this->container_.begin().base(), this->container_.data());
}

TYPED_TEST(TypedTypical, IteratorPlus)
{
    EXPECT_EQ(this->container_.begin() + this->frames(), this->container_.end());
}

TYPED_TEST(TypedTypical, IteratorPlusOverwrite)
{
    auto iter = this->container_.begin();
    iter = iter + this->frames();
    EXPECT_EQ(iter, this->container_.end());
}

TYPED_TEST(TypedTypical, IteratorPlusEquals)
{
    auto iter = this->container_.begin();
    iter += this->frames();
    EXPECT_EQ(iter, this->container_.end());
}

TYPED_TEST(TypedTypical, IteratorMinus)
{
    EXPECT_EQ(this->container_.end() - this->frames(), this->container_.begin());
}

TYPED_TEST(TypedTypical, IteratorMinusOverwrite)
{
    auto iter = this->container_.end();
    iter = iter - this->frames();
    EXPECT_EQ(iter, this->container_.begin());
}

TYPED_TEST(TypedTypical, IteratorMinusEquals)
{
    auto iter = this->container_.end();
    iter -= this->frames();
    EXPECT_EQ(iter, this->container_.begin());
}

TYPED_TEST(TypedTypical, IteratorDistance)
{
    EXPECT_EQ(
        static_cast<std::size_t>(std::distance(this->container_.begin(), this->container_.end())), this->frames());
}

TYPED_TEST(TypedTypical, IteratorIncrement)
{
    auto iter = this->container_.begin();
    for (size_t i = 0; i < this->frames(); ++i)
    {
        iter++;
    }
    EXPECT_EQ(iter, this->container_.end());
}

TYPED_TEST(TypedTypical, IteratorOffsetIteratorIncrement)
{
    auto iter = this->container_.begin();
    for (size_t i = 0; i < this->frames(); ++i)
    {
        EXPECT_EQ(this->container_.begin() + i, iter++);
    }
}

TYPED_TEST(TypedTypical, IteratorIncrementBaseData)
{
    auto iter = this->container_.begin();
    for (size_t i = 0; i < this->frames(); ++i)
    {
        EXPECT_EQ((iter++).base(), &this->container_.data()[i * this->channels()]);
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

TYPED_TEST(TypedTypical, IteratorOffsetDereferenceFrame)
{
    for (size_t i = 0; i < this->frames(); ++i)
    {
        EXPECT_EQ(*(this->container_.begin() + i), this->container_.frame(i));
    }
}

TYPED_TEST(TypedTypical, IteratorIncrementDereferenceFrame)
{
    auto iter = this->container_.begin();
    for (size_t i = 0; i < this->frames(); ++i)
    {
        EXPECT_EQ(*iter++, this->container_.frame(i));
    }
    EXPECT_EQ(iter, this->container_.end());
}

TYPED_TEST(TypedTypical, IteratorArrayAccessFrame)
{
    for (size_t i = 0; i < this->frames(); ++i)
    {
        EXPECT_EQ(this->container_.begin()[i], this->container_.frame(i));
    }
}

TYPED_TEST(TypedTypical, ReverseIteratorPlus)
{
    EXPECT_EQ(this->container_.rbegin() + this->frames(), this->container_.rend());
}

TYPED_TEST(TypedTypical, ReverseIteratorMinus)
{
    EXPECT_EQ(this->container_.rend() - this->frames(), this->container_.rbegin());
}

TYPED_TEST(TypedTypical, ConstIteratorPlus)
{
    EXPECT_EQ(this->container_.cbegin() + this->frames(), this->container_.cend());
}

TYPED_TEST(TypedTypical, ConstIteratorMinus)
{
    EXPECT_EQ(this->container_.cend() - this->frames(), this->container_.cbegin());
}

TYPED_TEST(TypedTypical, ConstReverseIteratorPlus)
{
    EXPECT_EQ(this->container_.crbegin() + this->frames(), this->container_.crend());
}

TYPED_TEST(TypedTypical, ConstReverseIteratorMinus)
{
    EXPECT_EQ(this->container_.crend() - this->frames(), this->container_.crbegin());
}

TYPED_TEST(TypedTypical, RangeBasedForLoopFrame)
{
    int i = 0;
    for (const auto& frame : this->container_)
    {
        EXPECT_EQ(frame, this->container_.frame(i++));
    }
    EXPECT_EQ(i, this->frames());
}

// Equality

template<typename SampleValueType>
class Equality : public ::testing::Test
{
protected:
    using sample_value_type = SampleValueType;

    static ratl::interleaved<sample_value_type> generate_default()
    {
        return ratl::interleaved<sample_value_type>();
    }

    static ratl::interleaved<sample_value_type> generate_typical()
    {
        return ratl::interleaved<sample_value_type>(TEST_RATL_TYPICAL_CHANNELS, TEST_RATL_TYPICAL_FRAMES);
    }
};

TYPED_TEST_SUITE(Equality, PossibleSampleValueTypes, );

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
    EXPECT_EQ(this->generate_default(), (ratl::interleaved<typename TestFixture::sample_value_type>(0, 0)));
}

TYPED_TEST(Equality, DefaultZeroChannelsConstructorNotEqual)
{
    EXPECT_NE(
        this->generate_default(),
        (ratl::interleaved<typename TestFixture::sample_value_type>(0, TEST_RATL_TYPICAL_FRAMES)));
}

TYPED_TEST(Equality, DefaultZeroFramesConstructorNotEqual)
{
    EXPECT_NE(
        this->generate_default(),
        (ratl::interleaved<typename TestFixture::sample_value_type>(TEST_RATL_TYPICAL_CHANNELS, 0)));
}

TYPED_TEST(Equality, DefaultTypicalNotEqual)
{
    EXPECT_NE(this->generate_default(), this->generate_typical());
}

TYPED_TEST(Equality, TypicalAllWriteNotEqual)
{
    auto typical = this->generate_typical();
    std::fill_n(typical.data(), typical.samples(), static_cast<typename TestFixture::sample_value_type>(1));
    EXPECT_NE(typical, this->generate_typical());
}

TYPED_TEST(Equality, TypicalAllWriteBackEqual)
{
    auto typical = this->generate_typical();
    std::fill_n(typical.data(), typical.samples(), static_cast<typename TestFixture::sample_value_type>(1));
    std::fill_n(typical.data(), typical.samples(), static_cast<typename TestFixture::sample_value_type>(0));
    EXPECT_EQ(typical, this->generate_typical());
}

TYPED_TEST(Equality, TypicalOneWriteNotEqual)
{
    auto typical = this->generate_typical();
    size_t middle_sample = (TEST_RATL_TYPICAL_CHANNELS * TEST_RATL_TYPICAL_FRAMES) / 2;
    typical.data()[middle_sample] = static_cast<typename TestFixture::sample_value_type>(1);
    EXPECT_NE(typical, this->generate_typical());
}

TYPED_TEST(Equality, TypicalOneWriteBackEqual)
{
    auto typical = this->generate_typical();
    size_t middle_sample = (TEST_RATL_TYPICAL_CHANNELS * TEST_RATL_TYPICAL_FRAMES) / 2;
    typical.data()[middle_sample] = static_cast<typename TestFixture::sample_value_type>(1);
    typical.data()[middle_sample] = static_cast<typename TestFixture::sample_value_type>(0);
    EXPECT_EQ(typical, this->generate_typical());
}

// CopyConstructor

template<typename SampleValueType>
class CopyConstructor : public CopyInterleavedBase<SampleValueType>
{
protected:
    using super_type = TypicalInterleavedBase<SampleValueType>;
    using interleaved_type = typename super_type::container_type;
};

TYPED_TEST_SUITE(CopyConstructor, PossibleSampleValueTypes, );

TYPED_TEST(CopyConstructor, CopyConstructor)
{
    using interleaved_type = typename TestFixture::interleaved_type;
    interleaved_type interleaved_copy = this->container_;
    EXPECT_EQ(interleaved_copy, this->generate());
}

// CopyAssignmentOperator

template<typename SampleValueType>
class CopyAssignmentOperator : public CopyInterleavedBase<SampleValueType>
{
protected:
    using super_type = TypicalInterleavedBase<SampleValueType>;
    using interleaved_type = typename super_type::container_type;
};

TYPED_TEST_SUITE(CopyAssignmentOperator, PossibleSampleValueTypes, );

TYPED_TEST(CopyAssignmentOperator, CopyAssignmentOperator)
{
    using interleaved_type = typename TestFixture::interleaved_type;
    interleaved_type interleaved_copy;
    interleaved_copy = this->container_;
    EXPECT_EQ(interleaved_copy, this->generate());
}

// MoveConstructor

template<typename SampleValueType>
class MoveConstructor : public MoveInterleavedBase<SampleValueType>
{
protected:
    using super_type = TypicalInterleavedBase<SampleValueType>;
    using interleaved_type = typename super_type::container_type;
};

TYPED_TEST_SUITE(MoveConstructor, PossibleSampleValueTypes, );

TYPED_TEST(MoveConstructor, MoveConstructor)
{
    using interleaved_type = typename TestFixture::interleaved_type;
    interleaved_type interleaved_move = std::move(this->container_);
    EXPECT_EQ(interleaved_move, this->generate());
}

// MoveAssignmentOperator

template<typename SampleValueType>
class MoveAssignmentOperator : public MoveInterleavedBase<SampleValueType>
{
protected:
    using super_type = TypicalInterleavedBase<SampleValueType>;
    using interleaved_type = typename super_type::container_type;
};

TYPED_TEST_SUITE(MoveAssignmentOperator, PossibleSampleValueTypes, );

TYPED_TEST(MoveAssignmentOperator, MoveAssignmentOperator)
{
    using interleaved_type = typename TestFixture::interleaved_type;
    interleaved_type interleaved_move;
    interleaved_move = std::move(this->container_);
    EXPECT_EQ(interleaved_move, this->generate());
}

// Swap

template<typename SampleValueType>
class Swap : public TypicalInterleavedBase<SampleValueType>
{
protected:
    using super_type = TypicalInterleavedBase<SampleValueType>;
    using interleaved_type = typename super_type::container_type;

    static interleaved_type generate_swap()
    {
        return interleaved_type(super_type::generate().channels() + 10, super_type::generate().frames() + 50);
    }
};

TYPED_TEST_SUITE(Swap, PossibleSampleValueTypes, );

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

template<typename SampleValueType>
class Allocate : public TypicalInterleavedBase<SampleValueType>
{
protected:
    using super_type = TypicalInterleavedBase<SampleValueType>;
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

    using interleaved_type = ratl::basic_interleaved<sample_type, allocator>;
};

TYPED_TEST_SUITE(Allocate, PossibleSampleValueTypes, );

TYPED_TEST(Allocate, TypicalAllocate)
{
    using interleaved_type = typename TestFixture::interleaved_type;
    interleaved_type(this->channels(), this->frames(), typename TestFixture::allocator());
}

TYPED_TEST(Allocate, CopyConstructor)
{
    using interleaved_type = typename TestFixture::interleaved_type;
    interleaved_type interleaved_allocated(this->channels(), this->frames(), typename TestFixture::allocator());
    interleaved_type(interleaved_allocated, typename TestFixture::allocator());
}

TYPED_TEST(Allocate, CopyAssignmentOperator)
{
    using interleaved_type = typename TestFixture::interleaved_type;
    interleaved_type interleaved_allocated_1(this->channels(), this->frames(), typename TestFixture::allocator());
    interleaved_type interleaved_allocated_2(this->channels(), this->frames(), typename TestFixture::allocator());
    interleaved_allocated_1 = interleaved_allocated_2;
}

TYPED_TEST(Allocate, MoveConstructorOperator)
{
    using interleaved_type = typename TestFixture::interleaved_type;
    interleaved_type interleaved_allocated(this->channels(), this->frames(), typename TestFixture::allocator());
    interleaved_type(interleaved_allocated, typename TestFixture::allocator());
}

TYPED_TEST(Allocate, MoveAssignmentOperator)
{
    using interleaved_type = typename TestFixture::interleaved_type;
    interleaved_type interleaved_allocated(this->channels(), this->frames(), typename TestFixture::allocator());
    interleaved_allocated = interleaved_type(this->channels(), this->frames(), typename TestFixture::allocator());
}

TYPED_TEST(Allocate, Swap)
{
    using interleaved_type = typename TestFixture::interleaved_type;
    interleaved_type interleaved_allocated_1(this->channels(), this->frames(), typename TestFixture::allocator());
    interleaved_type interleaved_allocated_2(this->channels(), this->frames(), typename TestFixture::allocator());
    std::swap(interleaved_allocated_1, interleaved_allocated_2);
}

// ReadWrite

template<typename SampleValueType>
class RawReadWrite : public TypicalInterleavedBase<SampleValueType>
{
protected:
    using sample_value_type = SampleValueType;
};

TYPED_TEST_SUITE(RawReadWrite, PossibleSampleValueTypes, );

TYPED_TEST(RawReadWrite, Write)
{
    auto iter = std::fill_n(
        this->container_.data(), this->container_.samples(), static_cast<typename TestFixture::sample_value_type>(1));
    EXPECT_EQ(iter, this->container_.data() + this->container_.samples());
}

TYPED_TEST(RawReadWrite, Read)
{
    std::fill_n(
        this->container_.data(), this->container_.samples(), static_cast<typename TestFixture::sample_value_type>(1));

    for (size_t i = 0; i < this->container_.samples(); ++i)
    {
        EXPECT_EQ(this->container_.data()[i].get(), static_cast<typename TestFixture::sample_value_type>(1));
    }
}

} // namespace test
} // namespace ratl
