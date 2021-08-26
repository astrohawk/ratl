#
# Copyright (c) 2018-2021 Hamish Cook
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.
#

if (${CMAKE_SYSTEM_NAME} STREQUAL Linux)
    find_package(ALSA)

    if (ALSA_FOUND)
        add_executable(alsa_playback
                ${CMAKE_CURRENT_LIST_DIR}/alsa_playback.cpp)

        target_link_libraries(alsa_playback
                ratl
                ALSA::ALSA)
    endif ()
endif ()
