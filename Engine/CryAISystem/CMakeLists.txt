set(SRC_ROOT "${ENGINE_ROOT}/CryAISystem")

file(GLOB ALL_SOURCES "${SRC_ROOT}/*.cpp")

add_library(cry_ai STATIC ${ALL_SOURCES})

target_include_directories(cry_ai PRIVATE
    "${SRC_ROOT}"
    "${ENGINE_ROOT}/CryCommon"
    "${CMAKE_SOURCE_DIR}/Platform/PSVita"
    "${CMAKE_SOURCE_DIR}/Shims"
)

target_link_libraries(cry_ai PRIVATE vita_shims)
