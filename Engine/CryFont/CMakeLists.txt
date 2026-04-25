set(SRC_ROOT "${ENGINE_ROOT}/CryFont")

file(GLOB ALL_SOURCES "${SRC_ROOT}/*.cpp")

add_library(cry_font STATIC ${ALL_SOURCES})

target_include_directories(cry_font PRIVATE
    "${SRC_ROOT}"
    "${ENGINE_ROOT}/CryCommon"
    "${CMAKE_SOURCE_DIR}/Platform/PSVita"
    "${CMAKE_SOURCE_DIR}/Shims"
)

target_link_libraries(cry_font PRIVATE vita_shims)
