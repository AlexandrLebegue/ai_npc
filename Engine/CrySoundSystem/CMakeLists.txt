set(SRC_ROOT "${ENGINE_ROOT}/CrySoundSystem")

# Exclude FMOD and audio-codec files not available on Vita
# (DummySoundSystem is used instead; music system excluded)
set(EXCLUDE_SOURCES
    "${SRC_ROOT}/DllMain.cpp"
    "${SRC_ROOT}/SoundSystem.cpp"
    "${SRC_ROOT}/SoundBuffer.cpp"
    "${SRC_ROOT}/Sound.cpp"
    "${SRC_ROOT}/MusicSystem.cpp"
    "${SRC_ROOT}/MusicPattern.cpp"
    "${SRC_ROOT}/MusicPatternInstance.cpp"
    "${SRC_ROOT}/OGGDecoder.cpp"
)

file(GLOB ALL_SOURCES "${SRC_ROOT}/*.cpp")
list(REMOVE_ITEM ALL_SOURCES ${EXCLUDE_SOURCES})

add_library(cry_sound STATIC ${ALL_SOURCES})

target_include_directories(cry_sound PRIVATE
    "${SRC_ROOT}"
    "${ENGINE_ROOT}/CryCommon"
    "${CMAKE_SOURCE_DIR}/Platform/PSVita"
    "${CMAKE_SOURCE_DIR}/Shims"
)

target_link_libraries(cry_sound PRIVATE vita_shims SceAudio_stub)
