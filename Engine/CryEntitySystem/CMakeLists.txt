set(SRC_ROOT "${ENGINE_ROOT}/CryEntitySystem")

file(GLOB ALL_SOURCES "${SRC_ROOT}/*.cpp")

add_library(cry_entity STATIC ${ALL_SOURCES})

target_include_directories(cry_entity PRIVATE
    "${SRC_ROOT}"
    "${ENGINE_ROOT}/CryCommon"
    "${CMAKE_SOURCE_DIR}/Platform/PSVita"
    "${CMAKE_SOURCE_DIR}/Shims"
)

target_link_libraries(cry_entity PRIVATE vita_shims)
