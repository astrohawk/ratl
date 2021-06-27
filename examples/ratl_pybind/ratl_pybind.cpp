
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <ratl/ratl.hpp>

namespace py = pybind11;

double sine(double position)
{
    return std::sin(2 * M_PI * position);
}

double square(double position)
{
    return std::fmod(position, 1.0) >= 0.5 ? 1.0 : -1.0;
}

double triangle(double position)
{
    return 1 - std::fabs(2 * ((2 * std::fmod(position, 1.0)) - 1));
}

double sawtooth(double position)
{
    return 2 * (std::fmod(position, 1.0) - 1);
}

static constexpr std::size_t FrameSize = 65536;
static constexpr std::size_t SampleRate = 44100;
static constexpr std::size_t Frequency = 1000;
static constexpr double Amplitude = 0.8;

PYBIND11_MODULE(ratl_pybind, m)
{
    static constexpr auto tone = sine;

    m.def(
        "triangular_pdf",
        []()
        {
            ratl::detail::TriangularDitherGenerator gen;
            std::vector<double> samples(1000000);
            for (auto& sample : samples)
            {
                sample = gen.generateFloat32();
            }
            return samples;
        });
    m.def(
        "gaussian_pdf",
        []()
        {
            ratl::detail::ShapedDitherGenerator gen;
            std::vector<double> samples(1000000);
            for (auto& sample : samples)
            {
                sample = gen.generateFloat32();
            }
            return samples;
        });
    m.def(
        "samplerate",
        []()
        {
            return SampleRate;
        });
    m.def(
        "num_samples",
        []()
        {
            return FrameSize;
        });
    m.def(
        "reference_double_tone",
        []()
        {
            std::vector<double> samples(FrameSize);
            double samples_per_cycle = static_cast<double>(SampleRate) / static_cast<double>(Frequency);
            for (std::size_t i = 0; i < samples.size(); ++i)
            {
                samples[i] = Amplitude * tone(static_cast<double>(i) / samples_per_cycle);
            }

            return samples;
        });
    m.def(
        "reference_int16_tone",
        []()
        {
            ratl::Interleaved<ratl::float32_t> input(1, FrameSize);
            double samples_per_cycle = static_cast<double>(SampleRate) / static_cast<double>(Frequency);
            for (std::size_t i = 0; i < input.frames(); ++i)
            {
                input.channel(0)[i].get() =
                    static_cast<float>(Amplitude * tone(static_cast<double>(i) / samples_per_cycle));
            }

            ratl::Interleaved<ratl::int32_t> temp1(1, FrameSize);
            ratl::transform(input.begin(), input.end(), temp1.begin());

            ratl::Interleaved<ratl::int16_t> temp2(1, FrameSize);
            ratl::transform(temp1.begin(), temp1.end(), temp2.begin());

            ratl::Interleaved<ratl::float32_t> output(1, FrameSize);
            ratl::transform(temp2.begin(), temp2.end(), output.begin());

            std::vector<float> samples(FrameSize);
            for (std::size_t i = 0; i < samples.size(); ++i)
            {
                samples[i] = output.channel(0)[i].get();
            }
            return samples;
        });
    m.def(
        "dither_int16_tone",
        []()
        {
            ratl::DitherGenerator dither_generator;

            ratl::Interleaved<ratl::float32_t> input(1, FrameSize);
            double samples_per_cycle = static_cast<double>(SampleRate) / static_cast<double>(Frequency);
            for (std::size_t i = 0; i < input.frames(); ++i)
            {
                input.channel(0)[i].get() =
                    static_cast<float>(Amplitude * tone(static_cast<double>(i) / samples_per_cycle));
            }

            ratl::Interleaved<ratl::int32_t> temp1(1, FrameSize);
            ratl::transform(input.begin(), input.end(), temp1.begin(), dither_generator);

            ratl::Interleaved<ratl::int16_t> temp2(1, FrameSize);
            ratl::transform(temp1.begin(), temp1.end(), temp2.begin(), dither_generator);

            ratl::Interleaved<ratl::float32_t> output(1, FrameSize);
            ratl::transform(temp2.begin(), temp2.end(), output.begin(), dither_generator);

            std::vector<float> samples(FrameSize);
            for (std::size_t i = 0; i < samples.size(); ++i)
            {
                samples[i] = output.channel(0)[i].get();
            }
            return samples;
        });
}