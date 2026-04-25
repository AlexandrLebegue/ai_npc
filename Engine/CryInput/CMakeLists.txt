set(SRC_ROOT "${ENGINE_ROOT}/CryInput")

# Exclude Win32-specific device files (keyboard/mouse replaced by VitaInput)
set(EXCLUDE_SOURCES
    "${SRC_ROOT}/XKeyboard.cpp"
    "${SRC_ROOT}/XMouse.cpp"
    "${SRC_ROOT}/XDebugKeyboard.cpp"
    "${SRC_ROOT}/DllMain.cpp"
)

file(GLOB ALL_SOURCES "${SRC_ROOT}/*.cpp")
list(REMOVE_ITEM ALL_SOURCES ${EXCLUDE_SOURCES})

add_library(cry_input STATIC ${ALL_SOURCES})

target_include_directories(cry_input PRIVATE
    "${SRC_ROOT}"
    "${ENGINE_ROOT}/CryCommon"
    "${CMAKE_SOURCE_DIR}/Platform/PSVita"
    "${CMAKE_SOURCE_DIR}/Shims"
)

target_link_libraries(cry_input PRIVATE vita_shims vita_platform)
