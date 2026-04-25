set(SRC_ROOT "${ENGINE_ROOT}/CrySystem")

# Exclude Win32-only files that have no Vita equivalent
set(EXCLUDE_SOURCES
    "${SRC_ROOT}/DllMain.cpp"          # DLL entry point — not needed (static lib)
    "${SRC_ROOT}/SystemWin32.cpp"      # replaced by VitaSystem.cpp
    "${SRC_ROOT}/DebugCallStack.cpp"   # Windows SEH stack walker
    "${SRC_ROOT}/CPUDetect.cpp"        # CPUID — ARM has no CPUID instruction
)

file(GLOB ALL_SOURCES "${SRC_ROOT}/*.cpp")
list(REMOVE_ITEM ALL_SOURCES ${EXCLUDE_SOURCES})

add_library(cry_system STATIC ${ALL_SOURCES})

target_include_directories(cry_system PRIVATE
    "${SRC_ROOT}"
    "${ENGINE_ROOT}/CryCommon"
    "${CMAKE_SOURCE_DIR}/Platform/PSVita"
    "${CMAKE_SOURCE_DIR}/Shims"
)

target_link_libraries(cry_system PRIVATE vita_shims)
