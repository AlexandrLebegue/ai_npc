set(SRC_ROOT "${ENGINE_ROOT}/CryScriptSystem")

file(GLOB ALL_SOURCES "${SRC_ROOT}/*.cpp")

add_library(cry_script STATIC ${ALL_SOURCES})

target_include_directories(cry_script PRIVATE
    "${SRC_ROOT}"
    "${ENGINE_ROOT}/CryCommon"
    "${CMAKE_SOURCE_DIR}/Platform/PSVita"
    "${CMAKE_SOURCE_DIR}/Shims"
)

target_link_libraries(cry_script PRIVATE vita_shims)
