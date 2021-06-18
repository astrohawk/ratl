
if (${CMAKE_SYSTEM_NAME} STREQUAL Linux)
    find_package(ALSA)

    add_executable(alsa_playback
            alsa_playback.cpp
            )

    target_link_libraries(alsa_playback
            ratl
            ALSA::ALSA
            )
endif ()
