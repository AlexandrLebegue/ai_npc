set(SRC_ROOT "${ENGINE_ROOT}/CryPhysics")

file(GLOB ALL_SOURCES "${SRC_ROOT}/*.cpp")

add_library(cry_physics STATIC ${ALL_SOURCES})

target_include_directories(cry_physics PRIVATE
    "${SRC_ROOT}"
    "${ENGINE_ROOT}/CryCommon"
    "${CMAKE_SOURCE_DIR}/Platform/PSVita"
    "${CMAKE_SOURCE_DIR}/Shims"
)

target_link_libraries(cry_physics PRIVATE vita_shims)

target_compile_definitions(cry_physics PRIVATE PHYSICS_EXPORTS)
