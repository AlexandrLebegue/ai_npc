# Header-only shim library — no sources to compile.
# Engine modules link against vita_shims to get the include path.
add_library(vita_shims INTERFACE)
target_include_directories(vita_shims INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
