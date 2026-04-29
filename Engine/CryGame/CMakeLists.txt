set(SRC_ROOT "${ENGINE_ROOT}/CryGame")

file(GLOB ALL_SOURCES "${SRC_ROOT}/*.cpp")

add_library(cry_game STATIC ${ALL_SOURCES})

target_include_directories(cry_game PRIVATE
    "${SRC_ROOT}"
    "${ENGINE_ROOT}/CryCommon"
    "${CMAKE_SOURCE_DIR}/Platform/PSVita"
    "${CMAKE_SOURCE_DIR}/Shims"
)

target_link_libraries(cry_game PRIVATE vita_shims)
