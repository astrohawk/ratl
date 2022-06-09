# Ratl (Real-time Audio Template Library)

`ratl` (pronounced rattle) is a C++ header-only library that is designed to help
with the processing of real-time audio
data. It does this by providing clear and unambiguous types to hold and
represent audio data as well as fast and easy
ways to convert audio data between these various types.

## Features

Support for:

1. 16 bit, 24 bit, and 32 bit integer, and 32 bit floating point samples
1. Host byte order and network byte order samples
1. Interleaved and non-interleaved audio buffers
1. Optional sample dithering

## Usage

Converting a 2 channel, 128 frame interleaved buffer of host-order 32-bit floats
to network-order 16-bit integers, with
dithering:

```cpp
std::size_t channels = 2;
std::size_t frames = 128;
ratl::interleaved<ratl::float32_t> input = generate_tone(channels, frames);
ratl::network_interleaved<ratl::int16_t> output(channels, frames);
ratl::dither_generator dither_gen;
ratl::transform(input.begin(), input.end(), output.begin(), dither_gen);
```

Converting a 4 channel, 32 frame externally-allocated interleaved buffer of
network-order 24-bit integers to a non-interleaved
buffer of host-order 32-bit floats:

```cpp
std::size_t channels = 4;
std::size_t frames = 32;
const unsigned char* data = read(sizeof(ratl::int24_t) * channels * frames);
ratl::const_network_interleaved_span<ratl::int24_t> input(data, channels, frames);
ratl::noninterleaved<ratl::float32_t> output(channels, frames);
ratl::transform(input.begin(), input.end(), output.begin());
```

## Supported Platforms

### Operating Systems

1. Linux
1. MacOS
1. Windows

### Compilers

Ratl uses C++14 and is therefore limited to a compiler that has support for
this.

1. gcc 5.0+
1. clang 3.4+
1. MSVC 2015+

## Getting Started

### CMake

You can add ratl as a dependency to your cmake library via the `ratl::ratl`
cmake target.

#### Tests

To build the ratl tests, set the `RATL_BUILD_TESTING` option to ON

#### Benchmarks

To build the ratl benchmarks, set the `RATL_BUILD_BENCHMARKS` option to ON

#### Examples

To build the ratl examples, set the `RATL_BUILD_EXAMPLES` option to ON

### SIMD

Ratl has the ability to explicitly use SIMD instructions if it has access to the
[`xsimd`](https://github.com/xtensor-stack/xsimd) library. There are two methods
for pulling in `xsimd`:

1. Via the git submodule in the `third_party` directory. If this method is used,
   you will need to set the cmake option `RATL_XSIMD_SOURCE` to `ON`.
2. Via a top-level project. If `ratl` itself is a dependency of a top-level
   project, `xsimd` can be added in as a third party dependency alongside
   `ratl`. In this case, the `xsimd` cmake target will automatically be detected
   and used. The one thing to note is that, due to a large refactor of `xsimd`
   that actually removed some required functionality, the `xsimd` git repository
   must be on the `7.x` branch.
