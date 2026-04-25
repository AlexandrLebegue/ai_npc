set(SRC_ROOT "${ENGINE_ROOT}/CryAnimation")

file(GLOB ALL_SOURCES "${SRC_ROOT}/*.cpp")

add_library(cry_animation STATIC ${ALL_SOURCES})

target_include_directories(cry_animation PRIVATE
    "${SRC_ROOT}"
    "${ENGINE_ROOT}/CryCommon"
    "${CMAKE_SOURCE_DIR}/Platform/PSVita"
    "${CMAKE_SOURCE_DIR}/Shims"
)

target_link_libraries(cry_animation PRIVATE vita_shims)
