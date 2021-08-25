#ifndef _ratl_dither_generator_
#define _ratl_dither_generator_

// C++ Standard Library includes
#include <cstddef>
#include <cstdint>

// ratl includes
#include <ratl/detail/batch_dither_generator.hpp>
#include <ratl/detail/config.hpp>

namespace ratl
{
using dither_generator = detail::batch_triangular_dither_generator;

}

#endif // _ratl_dither_generator_
