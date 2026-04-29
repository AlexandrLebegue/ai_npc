set(SRC_ROOT "${ENGINE_ROOT}/Cry3DEngine")

file(GLOB ALL_SOURCES "${SRC_ROOT}/*.cpp")

add_library(cry_3dengine STATIC ${ALL_SOURCES})

target_include_directories(cry_3dengine PRIVATE
    "${SRC_ROOT}"
    "${ENGINE_ROOT}/CryCommon"
    "${CMAKE_SOURCE_DIR}/Platform/PSVita"
    "${CMAKE_SOURCE_DIR}/Shims"
)

target_link_libraries(cry_3dengine PRIVATE vita_shims)
