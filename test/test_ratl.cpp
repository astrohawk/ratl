
#include <array>
#include <cassert>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <ratl/ratl.hpp>
#include <sstream>
#include <timer.hpp>
#include <vector>

#define NUM_TESTS 10000
//#define NUM_TESTS 100

#define CHANNELS 16
#define FRAMES 512

template<class Tp>
static std::string bytes_string(const Tp& bytes)
{
    const auto* array = reinterpret_cast<const uint8_t*>(&bytes);

    std::stringstream stream;
    auto separator = "";
    for (std::size_t i = 0; i < sizeof(Tp); ++i)
    {
        stream << separator << std::hex << std::setfill('0') << std::setw(2) << +array[i];
        separator = " ";
    }
    return std::move(stream).str();
}

template<class SampleType>
auto generate_empty_interleaved(size_t channels, size_t frames)
{
    using sample_traits = ratl::detail::sample_traits<sample>;
    using sample = typename sample_traits::sample;
    using interleaved = ratl::basic_interleaved<sample>;

    return interleaved(channels, frames);
}

template<class SampleValueType, class = typename std::enable_if<!std::is_floating_point<sample_type>::value>::type>
std::uniform_int_distribution<int32_t> generate_distribution()
{
    using sample_limits = ratl::sample_limits<sample_type>;
    return std::uniform_int_distribution<int32_t>(sample_limits::min, sample_limits::max);
}

template<class SampleValueType, class = typename std::enable_if<std::is_floating_point<sample_type>::value>::type>
std::uniform_real_distribution<float32_t> generate_distribution()
{
    using sample_limits = ratl::sample_limits<sample_type>;
    return std::uniform_real_distribution<float32_t>(sample_limits::min, sample_limits::max);
}

template<class SampleType>
auto generate_random_interleaved(size_t channels, size_t frames)
{
    using sample_traits = ratl::detail::sample_traits<sample>;
    using sample = typename sample_traits::sample;
    using sample_type = typename sample_traits::sample_type;

    std::random_device random;
    std::default_random_engine generator{random()};
    auto distribution = generate_distribution<sample_type>();

    auto out = generate_empty_interleaved<sample>(channels, frames);
    for (auto f : out)
    {
        for (auto& s : f)
        {
            ratl::sample<sample_type> temp{distribution(generator)};
            s = ratl::convert<sample>(temp);
        }
    }
    return out;
}

#if 0


typedef std::chrono::microseconds duration_type;
#    define DURATION_SUFFIX "us"

std::vector<std::pair<size_t, size_t>> g_samples;


float* array_copy(const float* array, size_t channels, size_t frames)
{
    size_t samples = channels * frames;
    float* array_copy = new float[samples];
    memcpy(array_copy, array, samples * sizeof(float));
    return array_copy;
}

ratl::interleaved<float> interleaved_copy(const ratl::interleaved<float>& interleaved)
{
    ratl::interleaved<float> interleaved_copy = interleaved;
    return interleaved_copy;
}


duration_type test_array()
{
    size_t channels = CHANNELS;
    size_t frames = FRAMES;
    size_t samples = channels * frames;
    float* array = new float[samples];

    for (int frame = 0; frame < FRAMES; ++frame)
    {
        for (int channel = 0; channel < CHANNELS; ++channel)
        {
            size_t sample = (frame * CHANNELS) + channel;
            array[sample] = 1;
        }
    }

    auto duration = high_resolution_timer([array, channels, frames] {
        for (const auto& sample_index : g_samples)
        {
            float* copy = array_copy(array, channels, frames);

            size_t sample = (sample_index.second * CHANNELS) + sample_index.first;
            float val = copy[sample];
            if (val != 1)
            {
                throw -1;
            }

            delete[] copy;
        }
    });
    return std::chrono::duration_cast<duration_type>(duration);
}

duration_type test_interleaved()
{
    ratl::interleaved<float> interleaved(CHANNELS, FRAMES);

    for (auto frame : interleaved)
    {
        for (auto& sample : frame)
        {
            sample = 1;
        }
    }

    auto duration = high_resolution_timer([&interleaved] {
        for (const auto& sample_index : g_samples)
        {
            ratl::interleaved<float> copy = interleaved_copy(interleaved);

            float val = copy[sample_index.second][sample_index.first];
            if (val != 1)
            {
                throw -1;
            }
        }
    });
    return std::chrono::duration_cast<duration_type>(duration);
}

int main()
{
    std::random_device g_rd;
    std::mt19937 g_gen(g_rd());

    std::uniform_int_distribution<size_t> channel_distribution(0, CHANNELS-1);
    std::uniform_int_distribution<size_t> frame_distribution(0, FRAMES-1);

    g_samples.resize(NUM_TESTS);
    for (int i = 0; i < NUM_TESTS; ++i)
    {
        g_samples[i] = std::make_pair(channel_distribution(g_gen), frame_distribution(g_gen));
    }

    std::cout << test_array().count() << " " DURATION_SUFFIX << std::endl;
    std::cout << test_interleaved().count() << " " DURATION_SUFFIX << std::endl;
}

#    elsif 0

int main()
{
    {
        // generator

        ratl::interleaved<uint32_t> interleaved = generate_interleaved();
        verify_interleaved(interleaved);
    }

    {
        // view

        ratl::interleaved<uint32_t> interleaved = generate_interleaved();

        ratl::interleaved_span<uint32_t> view(interleaved);
        assert(interleaved.frames() == FRAMES);
        assert(interleaved.channels() == CHANNELS);

        int i = 0;
        for (const auto& frame : view)
        {
            for (const auto& sample : frame)
            {
                assert(sample == i++);
            }
        }
    }

    {
        // copy constructor

        ratl::interleaved<uint32_t> interleaved = generate_interleaved();

        ratl::interleaved<uint32_t> interleaved_test = interleaved;
        verify_interleaved(interleaved_test);
        verify_interleaved(interleaved_test);
    }

    {
        // move constructor

        ratl::interleaved<uint32_t> interleaved = generate_interleaved();

        ratl::interleaved<uint32_t> interleaved_test = std::move(interleaved);
        assert(interleaved.frames() == 0);
        assert(interleaved.channels() == 0);
        verify_interleaved(interleaved_test);
    }

    {
        // copy assignment operator

        ratl::interleaved<uint32_t> interleaved = generate_interleaved();

        ratl::interleaved<uint32_t> interleaved_test;
        assert(interleaved_test.frames() == 0);
        assert(interleaved_test.channels() == 0);

        interleaved_test = interleaved;
        verify_interleaved(interleaved);
        verify_interleaved(interleaved_test);
    }

    {
        // move assignment operator

        ratl::interleaved<uint32_t> interleaved = generate_interleaved();

        ratl::interleaved<uint32_t> interleaved_test;
        assert(interleaved_test.frames() == 0);
        assert(interleaved_test.channels() == 0);

        interleaved_test = std::move(interleaved);
        assert(interleaved.frames() == 0);
        assert(interleaved.channels() == 0);
        verify_interleaved(interleaved_test);
    }

    {
        ratl::interleaved<uint32_t> interleaved = generate_interleaved();

        ratl::channel_span<uint32_t> channel1 = interleaved.channel(1);

        std::array<uint32_t, FRAMES> test_channel;
        ratl::channel_span<uint32_t> test_view(1, FRAMES, test_channel.data());

        std::copy(channel1.begin(), channel1.end(), test_view.begin());

        for (const auto& sample : test_view)
        {
            std::cout << sample << std::endl;
        }
    }

    {
        ratl::interleaved<uint32_t> interleaved = generate_interleaved();

        auto iter = interleaved.begin();

        std::cout << (*iter)[1] << std::endl;
        std::cout << iter->at(1) << std::endl;

        ++iter;

        std::cout << (*iter)[0] << std::endl;
        std::cout << iter->at(0) << std::endl;

        *iter = interleaved.back();
    }

    {
        ratl::interleaved<uint32_t> interleaved = generate_interleaved();

        auto iter = interleaved.begin();
        for (const auto& sample : *iter)
        {
            std::cout << sample << std::endl;
        }

        iter++;
        for (const auto& sample : *iter)
        {
            std::cout << sample << std::endl;
        }
    }
}

#elif 0

#    define FLOAT_TO_INT std::lrint
//#define FLOAT_TO_INT std::lround
//#define FLOAT_TO_INT static_cast<int16_t>

static constexpr float32_t scalar = 32767.f;
static constexpr float32_t divisor = (1.f / 32767.f);

inline int16_t test_float_convert(const float32_t& sample) noexcept
{
    if (sample < ratl::sample_limits<float32_t>::min())
    {
        return ratl::sample_limits<int16_t>::min();
    }
    if (sample >= ratl::sample_limits<float32_t>::max())
    {
        return std::numeric_limits<int16_t>::max();
    }
    return FLOAT_TO_INT(sample * scalar);
}

void float_to_int()
{
    std::chrono::high_resolution_clock::duration lib_duration;
    std::chrono::high_resolution_clock::duration test_duration;

    volatile float32_t value = 0.8;
    volatile int16_t sample;

    for (int i = 0; i < 100; ++i)
    {
        lib_duration += high_resolution_timer(
            [&]
            {
                for (long long j = 0; j < NUM_TESTS; ++j)
                {
                    float32_t v = value;
                    sample = ratl::detail::sample_convert<int16_t>(v);
                }
            });
        assert(sample == 26214);

        // ----------

        test_duration += high_resolution_timer(
            [&]
            {
                for (long long j = 0; j < NUM_TESTS; ++j)
                {
                    float32_t v = value;
                    sample = test_float_convert(v);
                }
            });
        assert(sample == 26214);
    }

    auto ratio = static_cast<float32_t>(lib_duration.count()) / static_cast<float32_t>(test_duration.count());

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(lib_duration).count() << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(test_duration).count() << std::endl;
    std::cout << ratio << std::endl;
}

inline float32_t test_int_convert(const int16_t& sample) noexcept
{
    return static_cast<float32_t>(sample) * divisor;
}

void int_to_float()
{
    std::chrono::high_resolution_clock::duration lib_duration;
    std::chrono::high_resolution_clock::duration test_duration;

    volatile int16_t value = 26214;
    volatile float32_t sample;

    for (int i = 0; i < 100; ++i)
    {
        lib_duration += high_resolution_timer(
            [&]
            {
                for (long long j = 0; j < NUM_TESTS; ++j)
                {
                    int16_t v = value;
                    sample = ratl::detail::sample_convert<float32_t>(v);
                }
            });

        // ----------

        test_duration += high_resolution_timer(
            [&]
            {
                for (long long j = 0; j < NUM_TESTS; ++j)
                {
                    int16_t v = value;
                    sample = test_int_convert(v);
                }
            });
        int16_t v = value;
        assert(sample == ratl::detail::sample_convert<float32_t>(v));
    }

    auto ratio = static_cast<float32_t>(lib_duration.count()) / static_cast<float32_t>(test_duration.count());

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(lib_duration).count() << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(test_duration).count() << std::endl;
    std::cout << ratio << std::endl;
}

int main()
{
    float_to_int();
    int_to_float();
}

#elif 0

template<class F, class... T, typename = decltype(std::declval<F>()(std::declval<T>()...))>
std::true_type supports_test(const F&, const T&...);
std::false_type supports_test(...);

template<class>
struct supports;
template<class F, class... T>
struct supports<F(T...)> : decltype(supports_test(std::declval<F>(), std::declval<T>()...))
{
};

template<class Tp, class Up>
struct supports_equal_to_impl
{
    template<class TpTest, class UpTest>
    static auto equal_to_test(const TpTest* t, const UpTest* u) -> decltype(*t == *u, char(0))
    {
    }

    static std::array<char, 2> equal_to_test(...) {}

    static const bool value = (sizeof(equal_to_test((Tp*)0, (Up*)0)) == 1);
};

template<class Tp, class Up>
struct supports_equal_to : public std::integral_constant<bool, supports_equal_to_impl<Tp, Up>::value>
{
};

template<class Tp, class Up>
struct supports_less_than_impl
{
    template<class TpTest, class UpTest>
    static auto less_than_test(const TpTest* t, const UpTest* u) -> decltype(*t <= *u, char(0))
    {
    }

    static std::array<char, 2> less_than_test(...) {}

    static const bool value = (sizeof(less_than_test((Tp*)0, (Up*)0)) == 1);
};

template<class Tp, class Up>
struct supports_less_than : public std::integral_constant<bool, supports_less_than_impl<Tp, Up>::value>
{
};

int main()
{
#    if 0
    {
        uint8_t i = 1;
        auto result = i + 0x80000000;
        std::cout << result << std::endl;
    }
    {
        uint16_t i = 1;
        auto result = i + 0x80000000;
        std::cout << result << std::endl;
    }
    {
        uint24_t i = 1;
        auto result = i + 0x80000000;
        std::cout << result << std::endl;
    }
    {
        uint32_t i = 1;
        auto result = i + 0x80000000;
        std::cout << result << std::endl;
    }
#    endif

#    if 0

    if (static_cast<int16_t>(0) == static_cast<int16_t>(0)) {}
    if (static_cast<int16_t>(0) == static_cast<int24_t>(0)) {}
    if (static_cast<int16_t>(0) == static_cast<int32_t>(0)) {}
    if (static_cast<int24_t>(0) == static_cast<int16_t>(0)) {}
    if (static_cast<int24_t>(0) == static_cast<int24_t>(0)) {}
    if (static_cast<int24_t>(0) == static_cast<int32_t>(0)) {}
    if (static_cast<int32_t>(0) == static_cast<int16_t>(0)) {}
    if (static_cast<int32_t>(0) == static_cast<int24_t>(0)) {}
    if (static_cast<int32_t>(0) == static_cast<int32_t>(0)) {}

    if (ratl::sample<int16_t>(0) == ratl::sample<int16_t>(0)) {}
    if (ratl::sample<int16_t>(0) == ratl::sample<int24_t>(0)) {}
    if (ratl::sample<int16_t>(0) == ratl::sample<int32_t>(0)) {}
    if (ratl::sample<int24_t>(0) == ratl::sample<int16_t>(0)) {}
    if (ratl::sample<int24_t>(0) == ratl::sample<int24_t>(0)) {}
    if (ratl::sample<int24_t>(0) == ratl::sample<int32_t>(0)) {}
    if (ratl::sample<int32_t>(1) == ratl::sample<int16_t>(0)) {}
    if (ratl::sample<int32_t>(0) == ratl::sample<int24_t>(0)) {}
    if (ratl::sample<int32_t>(0) == ratl::sample<int32_t>(0)) {}

    if (static_cast<int16_t>(0) == ratl::sample<int16_t>(0)) {}
    if (static_cast<int16_t>(0) == ratl::sample<int24_t>(0)) {}
    if (static_cast<int16_t>(0) == ratl::sample<int32_t>(0)) {}
    if (static_cast<int24_t>(0) == ratl::sample<int16_t>(0)) {}
    if (static_cast<int24_t>(0) == ratl::sample<int24_t>(0)) {}
    if (static_cast<int24_t>(0) == ratl::sample<int32_t>(0)) {}
    if (static_cast<int32_t>(0) == ratl::sample<int16_t>(0)) {}
    if (static_cast<int32_t>(0) == ratl::sample<int24_t>(0)) {}
    if (static_cast<int32_t>(0) == ratl::sample<int32_t>(0)) {}

    if (ratl::sample<int16_t>(0) == static_cast<int16_t>(0)) {}
    if (ratl::sample<int16_t>(0) == static_cast<int24_t>(0)) {}
    if (ratl::sample<int16_t>(0) == static_cast<int32_t>(0)) {}
    if (ratl::sample<int24_t>(0) == static_cast<int16_t>(0)) {}
    if (ratl::sample<int24_t>(0) == static_cast<int24_t>(0)) {}
    if (ratl::sample<int24_t>(0) == static_cast<int32_t>(0)) {}
    if (ratl::sample<int32_t>(1) == static_cast<int16_t>(0)) {}
    if (ratl::sample<int32_t>(0) == static_cast<int24_t>(0)) {}
    if (ratl::sample<int32_t>(0) == static_cast<int32_t>(0)) {}

#    endif

#    if 0
    std::cout << supports_equal_to<int16_t, int16_t>::value << std::endl;
    std::cout << supports_equal_to<int16_t, int24_t>::value << std::endl;
    std::cout << supports_equal_to<int16_t, int32_t>::value << std::endl;
    std::cout << supports_equal_to<int24_t, int16_t>::value << std::endl;
    std::cout << supports_equal_to<int24_t, int24_t>::value << std::endl;
    std::cout << supports_equal_to<int24_t, int32_t>::value << std::endl;
    std::cout << supports_equal_to<int32_t, int16_t>::value << std::endl;
    std::cout << supports_equal_to<int32_t, int24_t>::value << std::endl;
    std::cout << supports_equal_to<int32_t, int32_t>::value << std::endl;
    std::cout << std::endl;
    std::cout << supports_equal_to<ratl::sample<int16_t>, ratl::sample<int16_t>>::value << std::endl;
    std::cout << supports_equal_to<ratl::sample<int16_t>, ratl::sample<int24_t>>::value << std::endl;
    std::cout << supports_equal_to<ratl::sample<int16_t>, ratl::sample<int32_t>>::value << std::endl;
    std::cout << supports_equal_to<ratl::sample<int24_t>, ratl::sample<int16_t>>::value << std::endl;
    std::cout << supports_equal_to<ratl::sample<int24_t>, ratl::sample<int24_t>>::value << std::endl;
    std::cout << supports_equal_to<ratl::sample<int24_t>, ratl::sample<int32_t>>::value << std::endl;
    std::cout << supports_equal_to<ratl::sample<int32_t>, ratl::sample<int16_t>>::value << std::endl;
    std::cout << supports_equal_to<ratl::sample<int32_t>, ratl::sample<int24_t>>::value << std::endl;
    std::cout << supports_equal_to<ratl::sample<int32_t>, ratl::sample<int32_t>>::value << std::endl;
    std::cout << std::endl;
    std::cout << supports_equal_to<int16_t, ratl::sample<int16_t>>::value << std::endl;
    std::cout << supports_equal_to<int16_t, ratl::sample<int24_t>>::value << std::endl;
    std::cout << supports_equal_to<int16_t, ratl::sample<int32_t>>::value << std::endl;
    std::cout << supports_equal_to<int24_t, ratl::sample<int16_t>>::value << std::endl;
    std::cout << supports_equal_to<int24_t, ratl::sample<int24_t>>::value << std::endl;
    std::cout << supports_equal_to<int24_t, ratl::sample<int32_t>>::value << std::endl;
    std::cout << supports_equal_to<int32_t, ratl::sample<int16_t>>::value << std::endl;
    std::cout << supports_equal_to<int32_t, ratl::sample<int24_t>>::value << std::endl;
    std::cout << supports_equal_to<int32_t, ratl::sample<int32_t>>::value << std::endl;
    std::cout << std::endl;
    std::cout << supports_equal_to<ratl::sample<int16_t>, int16_t>::value << std::endl;
    std::cout << supports_equal_to<ratl::sample<int16_t>, int24_t>::value << std::endl;
    std::cout << supports_equal_to<ratl::sample<int16_t>, int32_t>::value << std::endl;
    std::cout << supports_equal_to<ratl::sample<int24_t>, int16_t>::value << std::endl;
    std::cout << supports_equal_to<ratl::sample<int24_t>, int24_t>::value << std::endl;
    std::cout << supports_equal_to<ratl::sample<int24_t>, int32_t>::value << std::endl;
    std::cout << supports_equal_to<ratl::sample<int32_t>, int16_t>::value << std::endl;
    std::cout << supports_equal_to<ratl::sample<int32_t>, int24_t>::value << std::endl;
    std::cout << supports_equal_to<ratl::sample<int32_t>, int32_t>::value << std::endl;
#    endif

#    if 0

    if (static_cast<int16_t>(1) > static_cast<int16_t>(0)) {}
    if (static_cast<int16_t>(1) > static_cast<int24_t>(0)) {}
    if (static_cast<int16_t>(1) > static_cast<int32_t>(0)) {}
    if (static_cast<int24_t>(1) > static_cast<int16_t>(0)) {}
    if (static_cast<int24_t>(1) > static_cast<int24_t>(0)) {}
    if (static_cast<int24_t>(1) > static_cast<int32_t>(0)) {}
    if (static_cast<int32_t>(1) > static_cast<int16_t>(0)) {}
    if (static_cast<int32_t>(1) > static_cast<int24_t>(0)) {}
    if (static_cast<int32_t>(1) > static_cast<int32_t>(0)) {}

    if (ratl::sample<int16_t>(1) > ratl::sample<int16_t>(0)) {}
    if (ratl::sample<int16_t>(1) > ratl::sample<int24_t>(0)) {}
    if (ratl::sample<int16_t>(1) > ratl::sample<int32_t>(0)) {}
    if (ratl::sample<int24_t>(1) > ratl::sample<int16_t>(0)) {}
    if (ratl::sample<int24_t>(1) > ratl::sample<int24_t>(0)) {}
    if (ratl::sample<int24_t>(1) > ratl::sample<int32_t>(0)) {}
    if (ratl::sample<int32_t>(1) > ratl::sample<int16_t>(0)) {}
    if (ratl::sample<int32_t>(1) > ratl::sample<int24_t>(0)) {}
    if (ratl::sample<int32_t>(1) > ratl::sample<int32_t>(0)) {}

    if (static_cast<int16_t>(1) > ratl::sample<int16_t>(0)) {}
    if (static_cast<int16_t>(1) > ratl::sample<int24_t>(0)) {}
    if (static_cast<int16_t>(1) > ratl::sample<int32_t>(0)) {}
    if (static_cast<int24_t>(1) > ratl::sample<int16_t>(0)) {}
    if (static_cast<int24_t>(1) > ratl::sample<int24_t>(0)) {}
    if (static_cast<int24_t>(1) > ratl::sample<int32_t>(0)) {}
    if (static_cast<int32_t>(1) > ratl::sample<int16_t>(0)) {}
    if (static_cast<int32_t>(1) > ratl::sample<int24_t>(0)) {}
    if (static_cast<int32_t>(1) > ratl::sample<int32_t>(0)) {}

    if (ratl::sample<int16_t>(1) > static_cast<int16_t>(0)) {}
    if (ratl::sample<int16_t>(1) > static_cast<int24_t>(0)) {}
    if (ratl::sample<int16_t>(1) > static_cast<int32_t>(0)) {}
    if (ratl::sample<uint24_t>(1) > static_cast<int16_t>(0)) {}
    if (ratl::sample<int24_t>(1) > static_cast<int24_t>(0)) {}
    if (ratl::sample<int24_t>(1) > static_cast<int32_t>(0)) {}
    if (ratl::sample<int32_t>(1) > static_cast<int16_t>(0)) {}
    if (ratl::sample<int32_t>(1) > static_cast<int24_t>(0)) {}
    if (ratl::sample<int32_t>(1) > static_cast<int32_t>(0)) {}

#    endif

#    if 0

    std::cout << std::is_convertible<int16_t, int24_t>::value << std::endl;
    std::cout << std::is_convertible<int24_t, int24_t>::value << std::endl;
    std::cout << std::is_convertible<int32_t, int24_t>::value << std::endl;

    std::cout << std::is_convertible<ratl::sample<int16_t>, int24_t>::value << std::endl;
    std::cout << std::is_convertible<ratl::sample<int24_t>, int24_t>::value << std::endl;
    std::cout << std::is_convertible<ratl::sample<int32_t>, int24_t>::value << std::endl;

    std::cout << std::endl;

    std::cout << std::is_same<decltype(int24_t(0) == int32_t(0)), bool>::value << std::endl;
    std::cout << std::is_same<decltype(int24_t(0) == int32_t(0)), bool>::value << std::endl;
    std::cout << std::is_same<decltype(int24_t(0) == int32_t(0)), bool>::value << std::endl;

    std::cout << std::is_same<decltype(ratl::sample<int16_t>(0) == int24_t(0)), bool>::value << std::endl;
    //std::cout << std::is_same<decltype(ratl::sample<int24_t>(0) == int24_t(0)), bool>::value << std::endl;
    std::cout << std::is_same<decltype(ratl::sample<int32_t>(0) == int24_t(0)), bool>::value << std::endl;

#    endif

#    if 0
    std::cout << std::is_convertible<int16_t, int16_t>::value << std::endl;
    std::cout << std::is_convertible<int16_t, int24_t>::value << std::endl;
    std::cout << std::is_convertible<int16_t, int32_t>::value << std::endl;
    std::cout << std::is_convertible<int24_t, int16_t>::value << std::endl;
    std::cout << std::is_convertible<int24_t, int24_t>::value << std::endl;
    std::cout << std::is_convertible<int24_t, int32_t>::value << std::endl;
    std::cout << std::is_convertible<int32_t, int16_t>::value << std::endl;
    std::cout << std::is_convertible<int32_t, int24_t>::value << std::endl;
    std::cout << std::is_convertible<int32_t, int32_t>::value << std::endl;
#    endif

    {
        int24_t i = RATL_INT24_MAX;
        std::cout << i << std::endl;
        i++;
        std::cout << i << std::endl;
        i++;
        std::cout << i << std::endl;
    }

    {
        int16_t i = 1;
        std::cout << i << std::endl;
        std::cout << i++ << std::endl;
        std::cout << ++i << std::endl;
        std::cout << i << std::endl;
    }
    {
        int24_t i = 1;
        std::cout << i << std::endl;
        std::cout << i++ << std::endl;
        std::cout << ++i << std::endl;
        std::cout << i << std::endl;
    }
}

#elif 0

int main()
{
#    if 0
    {
        std::vector<int16_t> test(20);

        auto begin_plus = test.begin() + 1;
        auto plus_begin = 1 + test.begin();
        std::cout << (begin_plus == plus_begin) << std::endl;

        auto end_minus = test.end() - 1;

        std::cout << (test.end() - test.begin()) << std::endl;
        std::cout << (test.begin() - test.end()) << std::endl;
    }

    {
        ratl::interleaved<int16_t> test(2, 10);

        auto begin_plus = test.begin() + 1;
        auto plus_begin = 1 + test.begin();
        std::cout << (begin_plus == plus_begin) << std::endl;

        auto end_minus = test.end() - 1;

        std::cout << (test.end() - test.begin()) << std::endl;
        std::cout << (test.begin() - test.end()) << std::endl;
    }

    {
        ratl::interleaved<int16_t> interleaved(2, 10);

        ratl::channel_span<int16_t> test = interleaved.channel(0);

        auto begin_plus = test.begin() + 1;
        auto plus_begin = 1 + test.begin();
        std::cout << (begin_plus == plus_begin) << std::endl;

        auto end_minus = test.end() - 1;

        std::cout << (test.end() - test.begin()) << std::endl;
        std::cout << (test.begin() - test.end()) << std::endl;
    }

    {
        ratl::interleaved<int16_t> interleaved(2, 10);

        ratl::frame_span<int16_t> test = interleaved.frame(0);

        auto begin_plus = test.begin() + 1;
        auto plus_begin = 1 + test.begin();
        std::cout << (begin_plus == plus_begin) << std::endl;

        auto end_minus = test.end() - 1;

        std::cout << (test.end() - test.begin()) << std::endl;
        std::cout << (test.begin() - test.end()) << std::endl;
    }
#    endif

#    if 0
    ratl::interleaved<int16_t> interleaved16(2, 10);
    for (auto frame : interleaved16)
    {
        for (auto& sample : frame)
        {
            sample = 256;
        }
    }

    ratl::interleaved<int24_t> interleaved24(2, 10);

    std::copy(interleaved16.begin(), interleaved16.end(), interleaved24.begin());
    for (const auto& frame : interleaved24)
    {
        for (const auto& sample : frame)
        {
            std::cout << sample.get() << std::endl;
        }
    }
#    endif
}

#elif 0

template<class Tp>
void print_convert(const Tp& sample)
{
    auto initial_int = ratl::sample<Tp>(sample);
    std::cout << (sample < 0 ? "" : " ") << initial_int.get();

    std::cout << " => ";

    auto convert_float = ratl::convert<ratl::sample<float32_t>>(initial_int);
    std::cout << (sample < 0 ? "" : " ") << std::setprecision(100) << convert_float.get();

    std::cout << " => ";

    auto final_int = ratl::convert<ratl::sample<Tp>>(convert_float);
    std::cout << (sample < 0 ? "" : " ") << final_int.get();

    std::cout << std::endl;
}

template<class Tp>
void print_tofloat(const Tp& sample)
{
    std::cout << std::setprecision(100) << (sample * ratl::detail::float_convert_traits<Tp>::multiplier) << std::endl;
}

int main()
{
#    if 1
    print_convert<int16_t>(0x7ffe);
    print_convert<int16_t>(0x7fff);
    print_convert<int16_t>(0x8000);
    print_convert<int16_t>(0x8001);

    print_convert<int24_t>(0x7ffffe);
    print_convert<int24_t>(0x7fffff);
    print_convert<int24_t>(0x800000);
    print_convert<int24_t>(0x800001);

    print_convert<int32_t>(0x7ffffffe);
    print_convert<int32_t>(0x7fffffff);
    print_convert<int32_t>(0x80000000);
    print_convert<int32_t>(0x80000001);
#    endif

    // print_tofloat<int16_t>(0x7ffe);
    print_tofloat<int16_t>(0x7fff);
    print_tofloat<int16_t>(0x8000);
    // print_tofloat<int16_t>(0x8001);

    // print_tofloat<int24_t>(0x7ffffe);
    print_tofloat<int24_t>(0x7fffff);
    print_tofloat<int24_t>(0x800000);
    // print_tofloat<int24_t>(0x800001);

    // print_tofloat<int32_t>(0x7ffffffe);
    print_tofloat<int32_t>(0x7fffffff);
    print_tofloat<int32_t>(0x80000000);
    // print_tofloat<int32_t>(0x80000001);

    // ratl::sample_decay_t<bool> b;

    static_assert(ratl::is_sample<ratl::sample<int32_t>>::value, "ratl::sample<int32_t>");
    static_assert(ratl::is_sample<ratl::network_sample<int32_t>>::value, "ratl::network_sample<int32_t>");
    static_assert(!ratl::is_sample<std::vector<int32_t>>::value, "std::vector<int32_t>");
}

#elif 1

template<class InputSampleType, class OutputSampleType>
static void verify_samples(const ratl::sample<InputSampleType>& in, const ratl::sample<OutputSampleType>& out)
{
    auto in_raw = static_cast<int32_t>(in.get());
    auto out_raw = static_cast<int32_t>(out.get());
    auto in_narrow = ratl::convert<ratl::sample<int16_t>>(in);
    auto out_narrow = ratl::convert<ratl::sample<int16_t>>(out);
    //    assert(in_narrow == out_narrow);
    if (out_narrow.get() < in_narrow.get() || out_narrow.get() > in_narrow.get())
    {
        std::cout << in_narrow << " (" << in_raw << ", " << bytes_string(in) << ") != " << out_narrow << " (" << out_raw
                  << ", " << bytes_string(out) << ")" << std::endl;
    }
    //    auto separator = out_narrow.get() < in_narrow.get() - 2 || out_narrow.get() > in_narrow.get() + 2 ? "!=" :
    //    "=="; std::cout << in_narrow << " (" << in_raw << ", " << bytes_string(in) << ") " << separator << " " <<
    //    out_narrow << " (" << out_raw << ", " << bytes_string(out) << ")" << std::endl;
}

template<class SampleValueType>
static void verify_samples(const ratl::sample<sample_type>& in, const ratl::sample<sample_type>& out)
{
    //    assert(in.get() == out.get());
    if (in != out)
    {
        std::cout << in << " (" << bytes_string(in) << ") != " << out << " (" << bytes_string(out) << ")" << std::endl;
    }
    //    auto separator = in != out ? "!=" : "==";
    //    std::cout << in << " (" << bytes_string(in) << ") " << separator << " " << out << " (" << bytes_string(out) <<
    //    ")" << std::endl;
}

template<class InputSampleType, class OutputSampleType>
static void verify_samples(const ratl::sample<InputSampleType>& in, const ratl::network_sample<OutputSampleType>& out)
{
    return verify_samples(in, ratl::convert<ratl::sample<OutputSampleType>>(out));
}

template<class InputSampleType, class OutputSampleType>
static void verify_samples(const ratl::network_sample<InputSampleType>& in, const ratl::sample<OutputSampleType>& out)
{
    return verify_samples(ratl::convert<ratl::sample<InputSampleType>>(in), out);
}

template<class SampleIn, class SampleOut, class Duration>
Duration get_average_transform_time(size_t channels, size_t frames)
{
    const auto input_interleaved = generate_random_interleaved<SampleIn>(channels, frames);

    auto results = std::array<Duration, NUM_TESTS>();
    std::generate(
        results.begin(),
        results.end(),
        [&input_interleaved, channels, frames]()
        {
            auto output_interleaved = generate_empty_interleaved<SampleOut>(channels, frames);

            const auto& input = input_interleaved;
            auto& output = output_interleaved;

            decltype(output.begin()) ouput_end;
            auto duration = high_resolution_timer(
                [&]
                {
                    ouput_end = ratl::transform(input.begin(), input.end(), output.begin());
                    //			  for (std::size_t channel_index = 0; channel_index < input.channels(); ++channel_index)
                    //            {
                    //                ratl::transform(input.channel(channel_index).begin(),
                    //                input.channel(channel_index).end(), output.channel(channel_index).begin());
                    //            }
                });

            assert(ouput_end == output.end());
            for (size_t i = 0; i < frames; ++i)
            {
                for (size_t j = 0; j < channels; ++j)
                {
                    verify_samples(input[i][j], output[i][j]);
                }
            }

            return std::chrono::duration_cast<Duration>(duration);
        });

    auto sum = Duration(0);
    for (const auto& result : results)
    {
        // std::cout << result.count() << " us" << std::endl;
        sum += result;
    }
    return sum / results.size();
}

int main()
{
    size_t channels = 32;
    size_t frames = 480;
    //    size_t channels = 1;
    //    size_t frames = 8;

    using Duration = std::chrono::nanoseconds;

    // clang-format off

#    if 0

    auto duration_string = "us";

    std::cout << "int16   -> int16   = " << get_average_transform_time<ratl::sample<int16_t>, ratl::sample<int16_t>, Duration>(channels, frames).count() << " " << duration_string << std::endl;
    std::cout << "int24   -> int24   = " << get_average_transform_time<ratl::sample<int24_t>, ratl::sample<int24_t>, Duration>(channels, frames).count() << " " << duration_string << std::endl;
    std::cout << "int32   -> int32   = " << get_average_transform_time<ratl::sample<int32_t>, ratl::sample<int32_t>, Duration>(channels, frames).count() << " " << duration_string << std::endl;
    std::cout << "float32 -> float32 = " << get_average_transform_time<ratl::sample<float32_t>, ratl::sample<float32_t>, Duration>(channels, frames).count() << " " << duration_string << std::endl;
    std::cout << "----------------" << std::endl;
    std::cout << "int16   -> float32 = " << get_average_transform_time<ratl::sample<int16_t>, ratl::sample<float32_t>, Duration>(channels, frames).count() << " " << duration_string << std::endl;
    std::cout << "int24   -> float32 = " << get_average_transform_time<ratl::sample<int24_t>, ratl::sample<float32_t>, Duration>(channels, frames).count() << " " << duration_string << std::endl;
    std::cout << "int32   -> float32 = " << get_average_transform_time<ratl::sample<int32_t>, ratl::sample<float32_t>, Duration>(channels, frames).count() << " " << duration_string << std::endl;
    std::cout << "float32 -> int16   = " << get_average_transform_time<ratl::sample<float32_t>, ratl::sample<int16_t>, Duration>(channels, frames).count() << " " << duration_string << std::endl;
    std::cout << "float32 -> int24   = " << get_average_transform_time<ratl::sample<float32_t>, ratl::sample<int24_t>, Duration>(channels, frames).count() << " " << duration_string << std::endl;
    std::cout << "float32 -> int32   = " << get_average_transform_time<ratl::sample<float32_t>, ratl::sample<int32_t>, Duration>(channels, frames).count() << " " << duration_string << std::endl;
    std::cout << "----------------" << std::endl;
    std::cout << "int16   -> network int16   = " << get_average_transform_time<ratl::sample<int16_t>, ratl::network_sample<int16_t>, Duration>(channels, frames).count() << " " << duration_string << std::endl;
    std::cout << "int24   -> network int24   = " << get_average_transform_time<ratl::sample<int24_t>, ratl::network_sample<int24_t>, Duration>(channels, frames).count() << " " << duration_string << std::endl;
    std::cout << "int32   -> network int32   = " << get_average_transform_time<ratl::sample<int32_t>, ratl::network_sample<int32_t>, Duration>(channels, frames).count() << " " << duration_string << std::endl;
    std::cout << "float32 -> network float32 = " << get_average_transform_time<ratl::sample<float32_t>, ratl::network_sample<float32_t>, Duration>(channels, frames).count() << " " << duration_string << std::endl;
    std::cout << "network int16   -> int16   = " << get_average_transform_time<ratl::network_sample<int16_t>, ratl::sample<int16_t>, Duration>(channels, frames).count() << " " << duration_string << std::endl;
    std::cout << "network int24   -> int24   = " << get_average_transform_time<ratl::network_sample<int24_t>, ratl::sample<int24_t>, Duration>(channels, frames).count() << " " << duration_string << std::endl;
    std::cout << "network int32   -> int32   = " << get_average_transform_time<ratl::network_sample<int32_t>, ratl::sample<int32_t>, Duration>(channels, frames).count() << " " << duration_string << std::endl;
    std::cout << "network float32 -> float32 = " << get_average_transform_time<ratl::network_sample<float32_t>, ratl::sample<float32_t>, Duration>(channels, frames).count() << " " << duration_string << std::endl;
    std::cout << "----------------" << std::endl;
    std::cout << "float32       -> network int16 = " << get_average_transform_time<ratl::sample<float32_t>, ratl::network_sample<int16_t>, Duration>(channels, frames).count() << " " << duration_string << std::endl;
    std::cout << "float32       -> network int24 = " << get_average_transform_time<ratl::sample<float32_t>, ratl::network_sample<int24_t>, Duration>(channels, frames).count() << " " << duration_string << std::endl;
    std::cout << "float32       -> network int32 = " << get_average_transform_time<ratl::sample<float32_t>, ratl::network_sample<int32_t>, Duration>(channels, frames).count() << " " << duration_string << std::endl;
    std::cout << "network int16 -> float32       = " << get_average_transform_time<ratl::network_sample<int16_t>, ratl::sample<float32_t>, Duration>(channels, frames).count() << " " << duration_string << std::endl;
    std::cout << "network int24 -> float32       = " << get_average_transform_time<ratl::network_sample<int24_t>, ratl::sample<float32_t>, Duration>(channels, frames).count() << " " << duration_string << std::endl;
    std::cout << "network int32 -> float32       = " << get_average_transform_time<ratl::network_sample<int32_t>, ratl::sample<float32_t>, Duration>(channels, frames).count() << " " << duration_string << std::endl;

#    elif 1

//    std::cout << "int16,host      => int16,host      = " << std::setw(6) << get_average_transform_time<ratl::sample<int16_t>, ratl::sample<int16_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "int16,host      => int24,host      = " << std::setw(6) << get_average_transform_time<ratl::sample<int16_t>, ratl::sample<int24_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "int16,host      => int32,host      = " << std::setw(6) << get_average_transform_time<ratl::sample<int16_t>, ratl::sample<int32_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "int16,host      => float32,host    = " << std::setw(6) << get_average_transform_time<ratl::sample<int16_t>, ratl::sample<float32_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//
//    std::cout << "int24,host      => int16,host      = " << std::setw(6) << get_average_transform_time<ratl::sample<int24_t>, ratl::sample<int16_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "int24,host      => int24,host      = " << std::setw(6) << get_average_transform_time<ratl::sample<int24_t>, ratl::sample<int24_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "int24,host      => int32,host      = " << std::setw(6) << get_average_transform_time<ratl::sample<int24_t>, ratl::sample<int32_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "int24,host      => float32,host    = " << std::setw(6) << get_average_transform_time<ratl::sample<int24_t>, ratl::sample<float32_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//
//    std::cout << "int32,host      => int16,host      = " << std::setw(6) << get_average_transform_time<ratl::sample<int32_t>, ratl::sample<int16_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "int32,host      => int24,host      = " << std::setw(6) << get_average_transform_time<ratl::sample<int32_t>, ratl::sample<int24_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "int32,host      => int32,host      = " << std::setw(6) << get_average_transform_time<ratl::sample<int32_t>, ratl::sample<int32_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "int32,host      => float32,host    = " << std::setw(6) << get_average_transform_time<ratl::sample<int32_t>, ratl::sample<float32_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//
//    std::cout << "float32,host    => int16,host      = " << std::setw(6) << get_average_transform_time<ratl::sample<float32_t>, ratl::sample<int16_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "float32,host    => int24,host      = " << std::setw(6) << get_average_transform_time<ratl::sample<float32_t>, ratl::sample<int24_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "float32,host    => int32,host      = " << std::setw(6) << get_average_transform_time<ratl::sample<float32_t>, ratl::sample<int32_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "float32,host    => float32,host    = " << std::setw(6) << get_average_transform_time<ratl::sample<float32_t>, ratl::sample<float32_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//
//    std::cout << "int16,network   => int16,host      = " << std::setw(6) << get_average_transform_time<ratl::network_sample<int16_t>, ratl::sample<int16_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "int16,network   => int24,host      = " << std::setw(6) << get_average_transform_time<ratl::network_sample<int16_t>, ratl::sample<int24_t>, Duration>(channels, frames).count() << "ns" << std::endl;
    std::cout << "int16,network   => int32,host      = " << std::setw(6)
              << get_average_transform_time<ratl::network_sample<int16_t>, ratl::sample<int32_t>, Duration>(channels,
                                                                                                           frames).count()
              << "ns" << std::endl;
//    std::cout << "int16,network   => float32,host    = " << std::setw(6) << get_average_transform_time<ratl::network_sample<int16_t>, ratl::sample<float32_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//
//    std::cout << "int24,network   => int16,host      = " << std::setw(6) << get_average_transform_time<ratl::network_sample<int24_t>, ratl::sample<int16_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "int24,network   => int24,host      = " << std::setw(6) << get_average_transform_time<ratl::network_sample<int24_t>, ratl::sample<int24_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "int24,network   => int32,host      = " << std::setw(6) << get_average_transform_time<ratl::network_sample<int24_t>, ratl::sample<int32_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "int24,network   => float32,host    = " << std::setw(6) << get_average_transform_time<ratl::network_sample<int24_t>, ratl::sample<float32_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//
//    std::cout << "int32,network   => int16,host      = " << std::setw(6) << get_average_transform_time<ratl::network_sample<int32_t>, ratl::sample<int16_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "int32,network   => int24,host      = " << std::setw(6) << get_average_transform_time<ratl::network_sample<int32_t>, ratl::sample<int24_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "int32,network   => int32,host      = " << std::setw(6) << get_average_transform_time<ratl::network_sample<int32_t>, ratl::sample<int32_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "int32,network   => float32,host    = " << std::setw(6) << get_average_transform_time<ratl::network_sample<int32_t>, ratl::sample<float32_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//
//    std::cout << "float32,network => int16,host      = " << std::setw(6) << get_average_transform_time<ratl::network_sample<float32_t>, ratl::sample<int16_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "float32,network => int24,host      = " << std::setw(6) << get_average_transform_time<ratl::network_sample<float32_t>, ratl::sample<int24_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "float32,network => int32,host      = " << std::setw(6) << get_average_transform_time<ratl::network_sample<float32_t>, ratl::sample<int32_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "float32,network => float32,host    = " << std::setw(6) << get_average_transform_time<ratl::network_sample<float32_t>, ratl::sample<float32_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//
//    std::cout << "int16,host      => int16,network   = " << std::setw(6) << get_average_transform_time<ratl::sample<int16_t>, ratl::network_sample<int16_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "int16,host      => int24,network   = " << std::setw(6) << get_average_transform_time<ratl::sample<int16_t>, ratl::network_sample<int24_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "int16,host      => int32,network   = " << std::setw(6) << get_average_transform_time<ratl::sample<int16_t>, ratl::network_sample<int32_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "int16,host      => float32,network = " << std::setw(6) << get_average_transform_time<ratl::sample<int16_t>, ratl::network_sample<float32_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//
//    std::cout << "int24,host      => int16,network   = " << std::setw(6) << get_average_transform_time<ratl::sample<int24_t>, ratl::network_sample<int16_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "int24,host      => int24,network   = " << std::setw(6) << get_average_transform_time<ratl::sample<int24_t>, ratl::network_sample<int24_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "int24,host      => int32,network   = " << std::setw(6) << get_average_transform_time<ratl::sample<int24_t>, ratl::network_sample<int32_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "int24,host      => float32,network = " << std::setw(6) << get_average_transform_time<ratl::sample<int24_t>, ratl::network_sample<float32_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//
//    std::cout << "int32,host      => int16,network   = " << std::setw(6) << get_average_transform_time<ratl::sample<int32_t>, ratl::network_sample<int16_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "int32,host      => int24,network   = " << std::setw(6) << get_average_transform_time<ratl::sample<int32_t>, ratl::network_sample<int24_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "int32,host      => int32,network   = " << std::setw(6) << get_average_transform_time<ratl::sample<int32_t>, ratl::network_sample<int32_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "int32,host      => float32,network = " << std::setw(6) << get_average_transform_time<ratl::sample<int32_t>, ratl::network_sample<float32_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//
//    std::cout << "float32,host    => int16,network   = " << std::setw(6) << get_average_transform_time<ratl::sample<float32_t>, ratl::network_sample<int16_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "float32,host    => int24,network   = " << std::setw(6) << get_average_transform_time<ratl::sample<float32_t>, ratl::network_sample<int24_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "float32,host    => int32,network   = " << std::setw(6) << get_average_transform_time<ratl::sample<float32_t>, ratl::network_sample<int32_t>, Duration>(channels, frames).count() << "ns" << std::endl;
//    std::cout << "float32,host    => float32,network = " << std::setw(6) << get_average_transform_time<ratl::sample<float32_t>, ratl::network_sample<float32_t>, Duration>(channels, frames).count() << "ns" << std::endl;

#    elif 0

    std::cout << "int16,false,int16,false" << std::endl;
    for (int i = 0; i < 10; ++i)
    {
        std::cout << get_average_transform_time<ratl::sample<int16_t>, ratl::sample<int16_t>, Duration>(channels, frames).count() << std::endl;
    }

    std::cout << "int16,false,int24,false" << std::endl;
    for (int i = 0; i < 10; ++i)
    {
        std::cout << get_average_transform_time<ratl::sample<int16_t>, ratl::sample<int24_t>, Duration>(channels, frames).count() << std::endl;
    }

    std::cout << "int16,false,int32,false" << std::endl;
    for (int i = 0; i < 10; ++i)
    {
        std::cout << get_average_transform_time<ratl::sample<int16_t>, ratl::sample<int32_t>, Duration>(channels, frames).count() << std::endl;
    }

#    else

    std::cout << "float32,host => int32,host = " << get_average_transform_time<ratl::sample<float32_t>, ratl::sample<int32_t>, Duration>(channels, frames).count() << "ns" << std::endl;

#    endif

    // clang-format on
}

#elif 0

int main()
{
    auto interleaved1 = generate_random_interleaved<ratl::network_sample<int24_t>>(2, 8);
    auto interleaved2 = generate_random_interleaved<ratl::sample<int32_t>>(3, 10);
    // auto interleaved2 = ratl::interleaved<int32_t>(3, 10);

    auto output = ratl::interleaved<float32_t>(4, 11);

    auto interleaved1_frames = std::min(interleaved1.frames(), output.frames());
    auto interleaved1_channels = std::min(interleaved1.channels(), output.channels());
    {
        auto interleaved1_iter = interleaved1.begin();
        auto interleaved1_end = interleaved1_iter + interleaved1_frames;
        auto output_iter = output.begin();
        while (interleaved1_iter != interleaved1_end)
        {
            auto frame1_begin = interleaved1_iter->begin();
            ratl::transform(frame1_begin, frame1_begin + interleaved1_channels, output_iter->begin());
            ++interleaved1_iter;
            ++output_iter;
        }
    }
    auto interleaved2_frames = std::min(interleaved2.frames(), output.frames());
    auto interleaved2_channels =
        std::min(interleaved1_channels + interleaved2.channels(), output.channels()) - interleaved1_channels;
    {
        auto interleaved2_iter = interleaved2.begin();
        auto interleaved2_end = interleaved2_iter + interleaved2_frames;
        auto output_iter = output.begin();
        while (interleaved2_iter != interleaved2_end)
        {
            auto frame2_begin = interleaved2_iter->begin();
            ratl::transform(
                frame2_begin, frame2_begin + interleaved2_channels, output_iter->begin() + interleaved1_channels);
            ++interleaved2_iter;
            ++output_iter;
        }
    }

    for (auto frame : output)
    {
        for (auto& sample : frame)
        {
            std::cout << sample.get() << std::endl;
        }
    }
}

#elif 1

int main()
{
    std::vector<int> v;
    v.end();
}

#endif
