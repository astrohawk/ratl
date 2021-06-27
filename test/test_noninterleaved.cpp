
#include "test_utils.hpp"

namespace ratl
{
namespace test
{
// DefaultConstructor

template<class SampleType>
class DefaultConstructor : public ::testing::Test
{
protected:
    ratl::Noninterleaved<SampleType> buffer_;
};

TYPED_TEST_SUITE(DefaultConstructor, PossibleSampleTypes, );

TYPED_TEST(DefaultConstructor, DataNull)
{
    EXPECT_EQ(this->buffer_.data(), nullptr);
}

TYPED_TEST(DefaultConstructor, Empty)
{
    EXPECT_TRUE(this->buffer_.empty());
}

TYPED_TEST(DefaultConstructor, ChannelsZero)
{
    EXPECT_EQ(this->buffer_.channels(), 0);
}

TYPED_TEST(DefaultConstructor, FramesZero)
{
    EXPECT_EQ(this->buffer_.frames(), 0);
}

TYPED_TEST(DefaultConstructor, SamplesZero)
{
    EXPECT_EQ(this->buffer_.samples(), 0);
}

TYPED_TEST(DefaultConstructor, AtBeginThrow)
{
    EXPECT_THROW(this->buffer_.at(0), std::out_of_range);
}

TYPED_TEST(DefaultConstructor, IteratorEqual)
{
    EXPECT_EQ(this->buffer_.begin(), this->buffer_.end());
}

TYPED_TEST(DefaultConstructor, ReverseIteratorEqual)
{
    EXPECT_EQ(this->buffer_.rbegin(), this->buffer_.rend());
}

TYPED_TEST(DefaultConstructor, ConstIteratorEqual)
{
    EXPECT_EQ(this->buffer_.cbegin(), this->buffer_.cend());
}

TYPED_TEST(DefaultConstructor, ConstReverseIteratorEqual)
{
    EXPECT_EQ(this->buffer_.crbegin(), this->buffer_.crend());
}

// ZeroConstructor

template<class SampleType>
class ZeroConstructor : public ZeroNoninterleavedBase<SampleType>
{
};

TYPED_TEST_SUITE(ZeroConstructor, PossibleSampleTypes, );

TYPED_TEST(ZeroConstructor, DataNull)
{
    EXPECT_EQ(this->buffer_.data(), nullptr);
}

TYPED_TEST(ZeroConstructor, Empty)
{
    EXPECT_TRUE(this->buffer_.empty());
}

TYPED_TEST(ZeroConstructor, ChannelsZero)
{
    EXPECT_EQ(this->buffer_.channels(), 0);
}

TYPED_TEST(ZeroConstructor, FramesZero)
{
    EXPECT_EQ(this->buffer_.frames(), 0);
}

TYPED_TEST(ZeroConstructor, SamplesZero)
{
    EXPECT_EQ(this->buffer_.samples(), 0);
}

TYPED_TEST(ZeroConstructor, AtBeginThrow)
{
    EXPECT_THROW(this->buffer_.at(0), std::out_of_range);
}

TYPED_TEST(ZeroConstructor, IteratorEqual)
{
    EXPECT_EQ(this->buffer_.begin(), this->buffer_.end());
}

TYPED_TEST(ZeroConstructor, ReverseIteratorEqual)
{
    EXPECT_EQ(this->buffer_.rbegin(), this->buffer_.rend());
}

TYPED_TEST(ZeroConstructor, ConstIteratorEqual)
{
    EXPECT_EQ(this->buffer_.cbegin(), this->buffer_.cend());
}

TYPED_TEST(ZeroConstructor, ConstReverseIteratorEqual)
{
    EXPECT_EQ(this->buffer_.crbegin(), this->buffer_.crend());
}

// ZeroChannelsConstructor

template<class SampleType>
class ZeroChannelsConstructor : public ZeroChannelsNoninterleavedBase<SampleType>
{
};

TYPED_TEST_SUITE(ZeroChannelsConstructor, PossibleSampleTypes, );

TYPED_TEST(ZeroChannelsConstructor, DataNull)
{
    EXPECT_EQ(this->buffer_.data(), nullptr);
}

TYPED_TEST(ZeroChannelsConstructor, Empty)
{
    EXPECT_TRUE(this->buffer_.empty());
}

TYPED_TEST(ZeroChannelsConstructor, ChannelsZero)
{
    EXPECT_EQ(this->buffer_.channels(), 0);
}

TYPED_TEST(ZeroChannelsConstructor, Frames)
{
    EXPECT_EQ(this->buffer_.frames(), this->frames());
}

TYPED_TEST(ZeroChannelsConstructor, SamplesZero)
{
    EXPECT_EQ(this->buffer_.samples(), 0);
}

TYPED_TEST(ZeroChannelsConstructor, AtBeginThrow)
{
    EXPECT_THROW(this->buffer_.at(0), std::out_of_range);
}

TYPED_TEST(ZeroChannelsConstructor, AtEndThrow)
{
    EXPECT_THROW(this->buffer_.at(this->frames()), std::out_of_range);
}

TYPED_TEST(ZeroChannelsConstructor, IteratorEqual)
{
    EXPECT_EQ(this->buffer_.begin(), this->buffer_.end());
}

TYPED_TEST(ZeroChannelsConstructor, ReverseIteratorEqual)
{
    EXPECT_EQ(this->buffer_.rbegin(), this->buffer_.rend());
}

TYPED_TEST(ZeroChannelsConstructor, ConstIteratorEqual)
{
    EXPECT_EQ(this->buffer_.cbegin(), this->buffer_.cend());
}

TYPED_TEST(ZeroChannelsConstructor, ConstReverseIteratorEqual)
{
    EXPECT_EQ(this->buffer_.crbegin(), this->buffer_.crend());
}

// ZeroFramesConstructor

template<class SampleType>
class ZeroFramesConstructor : public ZeroFramesNoninterleavedBase<SampleType>
{
};

TYPED_TEST_SUITE(ZeroFramesConstructor, PossibleSampleTypes, );

TYPED_TEST(ZeroFramesConstructor, DataNull)
{
    EXPECT_EQ(this->buffer_.data(), nullptr);
}

TYPED_TEST(ZeroFramesConstructor, Empty)
{
    EXPECT_TRUE(this->buffer_.empty());
}

TYPED_TEST(ZeroFramesConstructor, Channels)
{
    EXPECT_EQ(this->buffer_.channels(), this->channels());
}

TYPED_TEST(ZeroFramesConstructor, FramesZero)
{
    EXPECT_EQ(this->buffer_.frames(), 0);
}

TYPED_TEST(ZeroFramesConstructor, SamplesZero)
{
    EXPECT_EQ(this->buffer_.samples(), 0);
}

TYPED_TEST(ZeroFramesConstructor, AtBeginNoThrow)
{
    EXPECT_NO_THROW(this->buffer_.at(0));
}

TYPED_TEST(ZeroFramesConstructor, IteratorEqual)
{
    EXPECT_EQ(this->buffer_.begin(), this->buffer_.end());
}

TYPED_TEST(ZeroFramesConstructor, ReverseIteratorEqual)
{
    EXPECT_EQ(this->buffer_.rbegin(), this->buffer_.rend());
}

TYPED_TEST(ZeroFramesConstructor, ConstIteratorEqual)
{
    EXPECT_EQ(this->buffer_.cbegin(), this->buffer_.cend());
}

TYPED_TEST(ZeroFramesConstructor, ConstReverseIteratorEqual)
{
    EXPECT_EQ(this->buffer_.crbegin(), this->buffer_.crend());
}

// TypedTypical

template<class SampleType>
class TypedTypical : public TypicalNoninterleavedBase<SampleType>
{
};

TYPED_TEST_SUITE(TypedTypical, PossibleSampleTypes, );

TYPED_TEST(TypedTypical, DataNotNull)
{
    EXPECT_NE(this->buffer_.data(), nullptr);
}

TYPED_TEST(TypedTypical, NotEmpty)
{
    EXPECT_FALSE(this->buffer_.empty());
}

TYPED_TEST(TypedTypical, Channels)
{
    EXPECT_EQ(this->buffer_.channels(), this->channels());
}

TYPED_TEST(TypedTypical, Frames)
{
    EXPECT_EQ(this->buffer_.frames(), this->frames());
}

TYPED_TEST(TypedTypical, Samples)
{
    EXPECT_EQ(this->buffer_.samples(), this->samples());
}

TYPED_TEST(TypedTypical, ZeroInitialised)
{
    auto data = this->buffer_.data();
    for (size_t i = 0; i < this->buffer_.samples(); ++i)
    {
        EXPECT_EQ(data[i].get(), 0);
    }
}

TYPED_TEST(TypedTypical, FrameArrayAccessEquality)
{
    for (size_t i = 0; i < this->channels(); ++i)
    {
        EXPECT_EQ(this->buffer_.channel(i), this->buffer_[i]);
    }
}

TYPED_TEST(TypedTypical, FrameAtEquality)
{
    for (size_t i = 0; i < this->channels(); ++i)
    {
        EXPECT_EQ(this->buffer_.channel(i), this->buffer_.at(i));
    }
}

TYPED_TEST(TypedTypical, AtEndThrow)
{
    EXPECT_THROW(this->buffer_.at(this->channels()), std::out_of_range);
}

TYPED_TEST(TypedTypical, ChannelFrontEquality)
{
    EXPECT_EQ(this->buffer_.channel(0), this->buffer_.front());
}

TYPED_TEST(TypedTypical, ChannelBackEquality)
{
    EXPECT_EQ(this->buffer_.channel(this->channels() - 1), this->buffer_.back());
}

TYPED_TEST(TypedTypical, IteratorBaseData)
{
    EXPECT_EQ(this->buffer_.begin().base(), this->buffer_.data());
}

TYPED_TEST(TypedTypical, IteratorPlus)
{
    EXPECT_EQ(this->buffer_.begin() + this->channels(), this->buffer_.end());
}

TYPED_TEST(TypedTypical, IteratorPlusOverwrite)
{
    auto iter = this->buffer_.begin();
    iter = iter + this->channels();
    EXPECT_EQ(iter, this->buffer_.end());
}

TYPED_TEST(TypedTypical, IteratorPlusEquals)
{
    auto iter = this->buffer_.begin();
    iter += this->channels();
    EXPECT_EQ(iter, this->buffer_.end());
}

TYPED_TEST(TypedTypical, IteratorMinus)
{
    EXPECT_EQ(this->buffer_.end() - this->channels(), this->buffer_.begin());
}

TYPED_TEST(TypedTypical, IteratorMinusOverwrite)
{
    auto iter = this->buffer_.end();
    iter = iter - this->channels();
    EXPECT_EQ(iter, this->buffer_.begin());
}

TYPED_TEST(TypedTypical, IteratorMinusEquals)
{
    auto iter = this->buffer_.end();
    iter -= this->channels();
    EXPECT_EQ(iter, this->buffer_.begin());
}

TYPED_TEST(TypedTypical, IteratorDistance)
{
    EXPECT_EQ(std::distance(this->buffer_.begin(), this->buffer_.end()), this->channels());
}

TYPED_TEST(TypedTypical, IteratorIncrement)
{
    auto iter = this->buffer_.begin();
    for (size_t i = 0; i < this->channels(); ++i)
    {
        iter++;
    }
    EXPECT_EQ(iter, this->buffer_.end());
}

TYPED_TEST(TypedTypical, IteratorOffsetIteratorIncrement)
{
    auto iter = this->buffer_.begin();
    for (size_t i = 0; i < this->channels(); ++i)
    {
        EXPECT_EQ(this->buffer_.begin() + i, iter++);
    }
}

TYPED_TEST(TypedTypical, IteratorIncrementBaseData)
{
    auto iter = this->buffer_.begin();
    for (size_t i = 0; i < this->channels(); ++i)
    {
        EXPECT_EQ((iter++).base(), &this->buffer_.data()[i * this->frames()]);
    }
}

TYPED_TEST(TypedTypical, IteratorDereferenceBeginFront)
{
    EXPECT_EQ(*this->buffer_.begin(), this->buffer_.front());
}

TYPED_TEST(TypedTypical, IteratorDereferenceEndBack)
{
    EXPECT_EQ(*(this->buffer_.end() - 1), this->buffer_.back());
}

TYPED_TEST(TypedTypical, IteratorOffsetDereferenceChannel)
{
    for (size_t i = 0; i < this->channels(); ++i)
    {
        EXPECT_EQ(*(this->buffer_.begin() + i), this->buffer_.channel(i));
    }
}

TYPED_TEST(TypedTypical, IteratorIncrementDereferenceChannel)
{
    auto iter = this->buffer_.begin();
    for (size_t i = 0; i < this->channels(); ++i)
    {
        EXPECT_EQ(*iter++, this->buffer_.channel(i));
    }
    EXPECT_EQ(iter, this->buffer_.end());
}

TYPED_TEST(TypedTypical, IteratorArrayAccessFrame)
{
    for (size_t i = 0; i < this->channels(); ++i)
    {
        EXPECT_EQ(this->buffer_.begin()[i], this->buffer_.channel(i));
    }
}

TYPED_TEST(TypedTypical, ReverseIteratorPlus)
{
    EXPECT_EQ(this->buffer_.rbegin() + this->channels(), this->buffer_.rend());
}

TYPED_TEST(TypedTypical, ReverseIteratorMinus)
{
    EXPECT_EQ(this->buffer_.rend() - this->channels(), this->buffer_.rbegin());
}

TYPED_TEST(TypedTypical, ConstIteratorPlus)
{
    EXPECT_EQ(this->buffer_.cbegin() + this->channels(), this->buffer_.cend());
}

TYPED_TEST(TypedTypical, ConstIteratorMinus)
{
    EXPECT_EQ(this->buffer_.cend() - this->channels(), this->buffer_.cbegin());
}

TYPED_TEST(TypedTypical, ConstReverseIteratorPlus)
{
    EXPECT_EQ(this->buffer_.crbegin() + this->channels(), this->buffer_.crend());
}

TYPED_TEST(TypedTypical, ConstReverseIteratorMinus)
{
    EXPECT_EQ(this->buffer_.crend() - this->channels(), this->buffer_.crbegin());
}

TYPED_TEST(TypedTypical, RangeBasedForLoopFrame)
{
    int i = 0;
    for (const auto& channel : this->buffer_)
    {
        EXPECT_EQ(channel, this->buffer_.channel(i++));
    }
    EXPECT_EQ(i, this->channels());
}

// Equality

template<class SampleType>
class Equality : public ::testing::Test
{
protected:
    using sample_type = SampleType;

    static ratl::Noninterleaved<SampleType> generate_default()
    {
        return ratl::Noninterleaved<SampleType>{};
    }

    static ratl::Noninterleaved<SampleType> generate_typical()
    {
        return ratl::Noninterleaved<SampleType>{TEST_RATL_TYPICAL_CHANNELS, TEST_RATL_TYPICAL_FRAMES};
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
    EXPECT_EQ(this->generate_default(), (ratl::Noninterleaved<typename TestFixture::sample_type>{0, 0}));
}

TYPED_TEST(Equality, DefaultZeroChannelsConstructorNotEqual)
{
    EXPECT_NE(
        this->generate_default(),
        (ratl::Noninterleaved<typename TestFixture::sample_type>{0, TEST_RATL_TYPICAL_FRAMES}));
}

TYPED_TEST(Equality, DefaultZeroFramesConstructorNotEqual)
{
    EXPECT_NE(
        this->generate_default(),
        (ratl::Noninterleaved<typename TestFixture::sample_type>{TEST_RATL_TYPICAL_CHANNELS, 0}));
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

template<class SampleType>
class CopyConstructor : public CopyNoninterleavedBase<SampleType>
{
protected:
    using super_type = TypicalNoninterleavedBase<SampleType>;
    using sample_type = typename super_type::sample_type;
};

TYPED_TEST_SUITE(CopyConstructor, PossibleSampleTypes, );

TYPED_TEST(CopyConstructor, CopyConstructor)
{
    ratl::Noninterleaved<typename TestFixture::sample_type> interleaved_copy = this->buffer_;
    EXPECT_EQ(interleaved_copy, this->generate());
}

// CopyAssignmentOperator

template<class SampleType>
class CopyAssignmentOperator : public CopyNoninterleavedBase<SampleType>
{
protected:
    using super_type = TypicalNoninterleavedBase<SampleType>;
    using sample_type = typename super_type::sample_type;
};

TYPED_TEST_SUITE(CopyAssignmentOperator, PossibleSampleTypes, );

TYPED_TEST(CopyAssignmentOperator, CopyAssignmentOperator)
{
    ratl::Noninterleaved<typename TestFixture::sample_type> interleaved_copy;
    interleaved_copy = this->buffer_;
    EXPECT_EQ(interleaved_copy, this->generate());
}

// MoveConstructor

template<class SampleType>
class MoveConstructor : public MoveNoninterleavedBase<SampleType>
{
protected:
    using super_type = TypicalNoninterleavedBase<SampleType>;
    using sample_type = typename super_type::sample_type;
};

TYPED_TEST_SUITE(MoveConstructor, PossibleSampleTypes, );

TYPED_TEST(MoveConstructor, MoveConstructor)
{
    ratl::Noninterleaved<typename TestFixture::sample_type> interleaved_copy = std::move(this->buffer_);
    EXPECT_EQ(interleaved_copy, this->generate());
}

// MoveAssignmentOperator

template<class SampleType>
class MoveAssignmentOperator : public MoveNoninterleavedBase<SampleType>
{
protected:
    using super_type = TypicalNoninterleavedBase<SampleType>;
    using sample_type = typename super_type::sample_type;
};

TYPED_TEST_SUITE(MoveAssignmentOperator, PossibleSampleTypes, );

TYPED_TEST(MoveAssignmentOperator, MoveAssignmentOperator)
{
    ratl::Noninterleaved<typename TestFixture::sample_type> interleaved_copy;
    interleaved_copy = std::move(this->buffer_);
    EXPECT_EQ(interleaved_copy, this->generate());
}

// Swap

template<class SampleType>
class Swap : public TypicalNoninterleavedBase<SampleType>
{
protected:
    using super_type = TypicalNoninterleavedBase<SampleType>;
    using sample_type = typename super_type::sample_type;

    static ratl::Noninterleaved<sample_type> generate_swap()
    {
        return ratl::Noninterleaved<sample_type>{
            super_type::generate().channels() + 10, super_type::generate().frames() + 50};
    }
};

TYPED_TEST_SUITE(Swap, PossibleSampleTypes, );

TYPED_TEST(Swap, SwapFree)
{
    auto interleaved_swap = this->generate_swap();
    std::swap(this->buffer_, interleaved_swap);
    EXPECT_EQ(this->buffer_, this->generate_swap());
    EXPECT_EQ(interleaved_swap, this->generate());
}

TYPED_TEST(Swap, SwapMember)
{
    auto interleaved_swap = this->generate_swap();
    interleaved_swap.swap(this->buffer_);
    EXPECT_EQ(this->buffer_, this->generate_swap());
    EXPECT_EQ(interleaved_swap, this->generate());
}

// Allocator

template<class SampleType>
class Allocate : public TypicalNoninterleavedBase<SampleType>
{
protected:
    using super_type = TypicalNoninterleavedBase<SampleType>;
    using sample_type = typename super_type::sample_type;
    using sample = ratl::Sample<sample_type>;

    class Allocator : public ratl::Allocator<sample>
    {
    public:
        using super_type = ratl::Allocator<sample>;

        using size_type = typename super_type::size_type;
        using difference_type = typename super_type::difference_type;
        using pointer = typename super_type::pointer;
        using const_pointer = typename super_type::const_pointer;
        using reference = typename super_type::reference;
        using const_reference = typename super_type::const_reference;
        using value_type = typename super_type::value_type;

        using propagate_on_container_copy_assignment = std::false_type;
        using propagate_on_container_move_assignment = std::true_type;
        using propagate_on_container_swap = std::true_type;

        Allocator() : n_{0}, p_{nullptr} {}

        virtual ~Allocator()
        {
            EXPECT_EQ(n_, 0);
            EXPECT_EQ(p_, nullptr);
        }

        Allocator(const Allocator& other) : Allocator{}
        {
            EXPECT_EQ(other.n_, 0);
            EXPECT_EQ(other.p_, nullptr);
        }

        Allocator& operator=(const Allocator&) = delete;

        Allocator(Allocator&& other) : n_{other.n_}, p_{other.p_}
        {
            other.n_ = 0;
            other.p_ = nullptr;
        }

        Allocator& operator=(Allocator&& other)
        {
            std::swap(n_, other.n_);
            std::swap(p_, other.p_);
            return *this;
        }

        virtual pointer allocate(size_type n)
        {
            EXPECT_EQ(n_, 0);
            EXPECT_EQ(p_, nullptr);

            n_ = n;
            p_ = super_type::allocate(n_);
            return p_;
        }

        virtual void deallocate(pointer p, size_type n)
        {
            EXPECT_EQ(p, p_);
            EXPECT_EQ(n, n_);

            super_type::deallocate(p_, n_);
            n_ = 0;
            p_ = nullptr;
        }

        Allocator select_on_container_copy_construction() const
        {
            return Allocator{};
        }

        bool operator==(const Allocator& other) const
        {
            return ((n_ == other.n_) && (p_ == other.p_));
        }

        bool operator!=(const Allocator& other) const
        {
            return !(*this == other);
        }

    private:
        size_type n_;
        pointer p_;
    };

    using Noninterleaved = ratl::BasicNoninterleaved<sample, Allocator>;
};

TYPED_TEST_SUITE(Allocate, PossibleSampleTypes, );

TYPED_TEST(Allocate, TypicalAllocate)
{
    using Noninterleaved = typename TestFixture::Noninterleaved;
    Noninterleaved(this->channels(), this->frames(), typename TestFixture::Allocator{});
}

TYPED_TEST(Allocate, CopyConstructor)
{
    using Noninterleaved = typename TestFixture::Noninterleaved;
    Noninterleaved interleaved_allocated(this->channels(), this->frames(), typename TestFixture::Allocator{});
    Noninterleaved(interleaved_allocated, typename TestFixture::Allocator{});
}

TYPED_TEST(Allocate, CopyAssignmentOperator)
{
    using Noninterleaved = typename TestFixture::Noninterleaved;
    Noninterleaved interleaved_allocated_1(this->channels(), this->frames(), typename TestFixture::Allocator{});
    Noninterleaved interleaved_allocated_2(this->channels(), this->frames(), typename TestFixture::Allocator{});
    interleaved_allocated_1 = interleaved_allocated_2;
}

TYPED_TEST(Allocate, MoveConstructorOperator)
{
    using Noninterleaved = typename TestFixture::Noninterleaved;
    Noninterleaved interleaved_allocated(this->channels(), this->frames(), typename TestFixture::Allocator{});
    Noninterleaved(interleaved_allocated, typename TestFixture::Allocator{});
}

TYPED_TEST(Allocate, MoveAssignmentOperator)
{
    using Noninterleaved = typename TestFixture::Noninterleaved;
    Noninterleaved interleaved_allocated(this->channels(), this->frames(), typename TestFixture::Allocator{});
    interleaved_allocated = Noninterleaved(this->channels(), this->frames(), typename TestFixture::Allocator{});
}

TYPED_TEST(Allocate, Swap)
{
    using Noninterleaved = typename TestFixture::Noninterleaved;
    Noninterleaved interleaved_allocated_1(this->channels(), this->frames(), typename TestFixture::Allocator{});
    Noninterleaved interleaved_allocated_2(this->channels(), this->frames(), typename TestFixture::Allocator{});
    std::swap(interleaved_allocated_1, interleaved_allocated_2);
}

// ReadWrite

template<class SampleType>
class RawReadWrite : public TypicalNoninterleavedBase<SampleType>
{
};

TYPED_TEST_SUITE(RawReadWrite, PossibleSampleTypes, );

TYPED_TEST(RawReadWrite, Write)
{
    auto iter = std::fill_n(this->buffer_.data(), this->buffer_.samples(), 1);
    EXPECT_EQ(iter, this->buffer_.data() + this->buffer_.samples());
}

TYPED_TEST(RawReadWrite, Read)
{
    std::fill_n(this->buffer_.data(), this->buffer_.samples(), 1);

    for (size_t i = 0; i < this->buffer_.samples(); ++i)
    {
        EXPECT_EQ(this->buffer_.data()[i].get(), 1);
    }
}

} // namespace test
} // namespace ratl
