#
# Copyright (c) 2018-2022 Hamish Cook
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.
#

if (TARGET pybind11::pybind11)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)

    pybind11_add_module(clock_simulation ${CMAKE_CURRENT_LIST_DIR}/clock_simulation.cpp)
    target_link_libraries(clock_simulation PRIVATE
            ratl::ratl)
endif ()
