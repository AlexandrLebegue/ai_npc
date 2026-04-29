set(SRC_ROOT "${ENGINE_ROOT}/CrySystem")

# Exclude Win32-only files that have no Vita equivalent
set(EXCLUDE_SOURCES
    # DllMain.cpp is kept — it contains CreateSystemInterface which we need
    "${SRC_ROOT}/DebugCallStack.cpp"   # Windows SEH stack walker
    "${SRC_ROOT}/getdxver.cpp"         # DirectX version detection — Windows only
    "${SRC_ROOT}/CPUDetect.cpp"        # x86 CPUID — replaced by VitaCPUDetect.cpp
    "${SRC_ROOT}/SystemWin32.cpp"      # Win32 APIs — replaced by VitaSystemWin32.cpp
)

file(GLOB ALL_SOURCES "${SRC_ROOT}/*.cpp")
file(GLOB ALL_C_SOURCES "${SRC_ROOT}/*.c")
file(GLOB XML_SOURCES "${SRC_ROOT}/XML/*.cpp")
file(GLOB EXPAT_SOURCES "${SRC_ROOT}/XML/Expat/xmlparse.c"
                         "${SRC_ROOT}/XML/Expat/xmlrole.c"
                         "${SRC_ROOT}/XML/Expat/xmltok.c")
list(REMOVE_ITEM ALL_SOURCES ${EXCLUDE_SOURCES})

add_library(cry_system STATIC ${ALL_SOURCES} ${ALL_C_SOURCES} ${XML_SOURCES} ${EXPAT_SOURCES})

target_include_directories(cry_system PRIVATE
    "${SRC_ROOT}"
    "${SRC_ROOT}/XML"
    "${ENGINE_ROOT}/CryCommon"
    "${CMAKE_SOURCE_DIR}/Platform/PSVita"
    "${CMAKE_SOURCE_DIR}/Shims"
)

target_link_libraries(cry_system PRIVATE vita_shims)
