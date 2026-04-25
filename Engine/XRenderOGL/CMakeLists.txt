set(SRC_ROOT "${ENGINE_ROOT}/RenderDll/XRenderOGL")
set(COMMON_ROOT "${ENGINE_ROOT}/RenderDll/Common")

file(GLOB OGL_SOURCES     "${SRC_ROOT}/*.cpp")
file(GLOB COMMON_SOURCES  "${COMMON_ROOT}/*.cpp")

# Exclude DLL entry point
list(FILTER OGL_SOURCES    EXCLUDE REGEX ".*DllMain\\.cpp$")
list(FILTER COMMON_SOURCES EXCLUDE REGEX ".*DllMain\\.cpp$")

add_library(cry_render_ogl STATIC
    ${OGL_SOURCES}
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

# vitaGL provides OpenGL ES 2.0 + extensions on top of GXM
target_link_libraries(cry_render_ogl PRIVATE
    vita_shims
    vitaGL
    SceGxm_stub
    SceShaccCg_stub
)
