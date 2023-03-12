/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_bench_utils_
#define _ratl_bench_utils_

// ratl includes
#include <ratl/ratl.hpp>

// other includes
#include <benchmark/benchmark.h>
#include <random>

namespace ratl
{
namespace utils
{
template<typename InputType>
InputType generateRandomInput(std::size_t num_channels, std::size_t num_frames)
{
    std::random_device random_device;
    std::default_random_engine random_engine(random_device());
    std::uniform_real_distribution<float32_t> uniform_dist(-0.8f, 0.8f);

    interleaved<float32_t> float_container(num_channels, num_frames);
    for (auto frame : float_container)
    {
        for (auto& input : frame)
        {
            input = sample<float32_t>(uniform_dist(random_engine));
        }
    }

    InputType input(num_channels, num_frames);
    transform(float_container.begin(), float_container.end(), input.begin());
    return input;
}
} // namespace utils
} // namespace ratl

#endif // _ratl_bench_utils_