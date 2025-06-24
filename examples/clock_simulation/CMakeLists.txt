#
# Copyright (c) 2018-2022 Hamish Cook
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.
#

if (TARGET pybind11::pybind11)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)

    pybind11_add_module(ratl_chrono ${CMAKE_CURRENT_LIST_DIR}/ratl_chrono.cpp)
    target_link_libraries(ratl_chrono PRIVATE
            ratl::ratl)
endif ()
