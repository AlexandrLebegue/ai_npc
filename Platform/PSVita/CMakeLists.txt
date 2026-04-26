add_library(vita_platform STATIC
    VitaSystem.cpp
    VitaInput.cpp
    VitaRenderer.cpp
)

target_include_directories(vita_platform PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${ENGINE_ROOT}/CryCommon"
    "${CMAKE_SOURCE_DIR}/Shims"
)

target_compile_definitions(vita_platform PUBLIC VITA __vita__ __psp2__)
