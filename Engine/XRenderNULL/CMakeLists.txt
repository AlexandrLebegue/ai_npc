set(SRC_ROOT "${ENGINE_ROOT}/RenderDll/XRenderNULL")
set(COMMON_ROOT "${ENGINE_ROOT}/RenderDll/Common")

file(GLOB NULL_SOURCES    "${SRC_ROOT}/*.cpp")
file(GLOB COMMON_SOURCES  "${COMMON_ROOT}/*.cpp")

# Exclude DLL entry points and files that pull in heavy Win32/GL deps
list(FILTER NULL_SOURCES    EXCLUDE REGEX ".*DllMain\\.cpp$")
list(FILTER COMMON_SOURCES  EXCLUDE REGEX ".*DllMain\\.cpp$")
# Exclude sub-directories that have their own heavy dependencies
list(FILTER COMMON_SOURCES  EXCLUDE REGEX ".*(NvTriStrip|Textures/Image/jpeg6)\\.cpp$")

add_library(cry_render_ogl STATIC
    ${NULL_SOURCES}
    ${COMMON_SOURCES}
)

target_include_directories(cry_render_ogl PRIVATE
    "${SRC_ROOT}"
    "${COMMON_ROOT}"
    "${ENGINE_ROOT}/RenderDll"
    "${ENGINE_ROOT}/CryCommon"
    "${CMAKE_SOURCE_DIR}/Platform/PSVita"
    "${CMAKE_SOURCE_DIR}/Shims"
)

target_link_libraries(cry_render_ogl PRIVATE
    vita_shims
    vitaGL
    SceGxm_stub
)

target_compile_definitions(cry_render_ogl PRIVATE RENDER_EXPORTS)
