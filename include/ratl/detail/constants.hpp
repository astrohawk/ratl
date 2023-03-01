/**
* Copyright (c) 2018-2021 Hamish Cook
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef _ratl_detail_constants_
#define _ratl_detail_constants_

// ratl includes
#include <ratl/detail/config.hpp>

// other includes
#include <cmath>

namespace ratl
{
namespace detail
{
struct constants
{
    static constexpr double Pi = M_PI;
    static constexpr double Tau = 2 * Pi;
    static constexpr double Sqrt2 = M_SQRT2;
};
}
}

#endif // _ratl_detail_constants_
