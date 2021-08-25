/**
 * Copyright (c) 2018-2021 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_dither_generator_
#define _ratl_dither_generator_

// ratl includes
#include <ratl/detail/batch_dither_generator.hpp>
#include <ratl/detail/config.hpp>

// other includes
#include <cstddef>
#include <cstdint>

namespace ratl
{
using dither_generator = detail::batch_triangular_dither_generator;

}

#endif // _ratl_dither_generator_
