set(SRC_ROOT "${ENGINE_ROOT}/CryNetwork")

file(GLOB ALL_SOURCES "${SRC_ROOT}/*.cpp")

add_library(cry_network STATIC ${ALL_SOURCES})

target_include_directories(cry_network PRIVATE
    "${SRC_ROOT}"
    "${ENGINE_ROOT}/CryCommon"
    "${CMAKE_SOURCE_DIR}/Platform/PSVita"
    "${CMAKE_SOURCE_DIR}/Shims"
)

target_link_libraries(cry_network PRIVATE vita_shims)
