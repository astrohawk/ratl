# Ratl (Real-time Audio Template Library)

`ratl` (pronounced rattle) is a C++ header-only library that is designed to help with the processing of real-time audio
data. It does this by providing clear and unambiguous types to hold and represent audio data as well as fast and easy
ways to convert audio data between these various types.

## Features

Support for:
1. 16 bit, 24 bit, and 32 bit integer, and 32 bit floating point samples
1. Host byte order and network byte order samples
1. Interleaved and non-interleaved audio buffers
1. Optional sample dithering

## Getting Started

## Usage

Converting a 4 channel, 32 frame interleaved buffer of host-order 32-bit floats to network-order 16-bit integers, with
dithering:
```cpp
std::size_t channels = 4;
std::size_t frames = 32;
ratl::Interleaved<ratl::float32_t> input = generate_tone(channels, frames);
ratl::NetworkInterleaved<ratl::int16_t> output(channels, frames);
ratl::DitherGenerator dither_generator;
ratl::transform(input.begin(), input.end(), output.begin(), dither_generator);
```

Converting a 4 channel, 32 frame pre-allocated interleaved buffer of network-order 24-bit integers to a non-interleaved
buffer of host-order 32-bit floats:
```cpp
std::size_t channels = 4;
std::size_t frames = 32;
const unsigned char* data = read(sizeof(ratl::int24_t) * channels * frames);
ratl::ConstNetworkInterleavedSpan<ratl::int24_t> input(data, channels, frames);
ratl::Noninterleaved<ratl::float32_t> output(channels, frames);
for (std::size_t i = 0; i < channels; ++i)
{
    ratl::transform(input.channel(i).begin(), input.channel(i).end(), output.channel(i).begin());
}
```

## Types

### Samples

`ratl` has support for host byte order and network byte order 16-bit, 24-bit, and 32-bit integer, and 32-bit floating point samples.

### Buffers

#### Channel Buffer

#### Frame Buffer

#### Interleaved Buffer

#### Non-interleaved buffer
