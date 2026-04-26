cmake_minimum_required(VERSION 3.16)

# ============================================================
#  Far Cry 1 — PS Vita Port
#  Toolchain: vitaSDK  (arm-vita-eabi-g++)
# ============================================================

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake"
        CACHE PATH "vitaSDK toolchain")
  else()
    message(FATAL_ERROR
      "VITASDK environment variable not set.\n"
      "Install vitaSDK: https://vitasdk.org/  then  export VITASDK=/usr/local/vitasdk")
  endif()
endif()

project(FarCry_Vita CXX C ASM)

include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)

# ── VPK metadata ─────────────────────────────────────────────
set(VITA_APP_NAME  "Far Cry")
set(VITA_TITLEID   "FCRY00001")
set(VITA_VERSION   "01.00")

# ── Engine source root ────────────────────────────────────────
set(ENGINE_ROOT "${CMAKE_SOURCE_DIR}/farcry")

# ── Global compile definitions ────────────────────────────────
add_compile_definitions(
    VITA                      # our master Vita guard
    __vita__
    __psp2__                  # vitaSDK internal name
    # NOTE: do NOT define LINUX/LINUX32 — causes type redefinitions with PSVitaSpecific.h
    _CPU_ARM
    NOT_USE_CRY_MEMORY_MANAGER
    _MT
    # Silence STLPORT threading
    _NOTHREADS
    _STLP_NO_THREADS
    # PHYSICS_EXPORTS and RENDER_EXPORTS are set per-module only
)

# ── Include paths shared by all targets ───────────────────────
include_directories(
    "${ENGINE_ROOT}/CryCommon"
    # STLPORT excluded on Vita — vitaSDK provides its own stdlib
    "${CMAKE_SOURCE_DIR}/Platform/PSVita"
    "${CMAKE_SOURCE_DIR}/Shims"
    "$ENV{VITASDK}/arm-vita-eabi/include"
)

# ── Compiler flags ────────────────────────────────────────────
set(VITA_CXX_FLAGS
    "-march=armv7-a -mfpu=neon -mfloat-abi=hard"
    "-fno-rtti -fno-exceptions"
    "-fpermissive"           # MSVC→GCC: allow non-conformant template lookup
    "-Wno-deprecated -Wno-write-strings -Wno-multichar"
    "-Wno-error"             # treat all remaining warnings as non-fatal
    "-O2"
)
string(REPLACE ";" " " VITA_CXX_FLAGS "${VITA_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${VITA_CXX_FLAGS}")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${VITA_CXX_FLAGS}")

# ============================================================
#  Subdirectories (each builds one static lib for Vita)
# ============================================================
add_subdirectory(Platform/PSVita)
add_subdirectory(Shims)
add_subdirectory(Engine/CrySystem)
add_subdirectory(Engine/CryPhysics)
add_subdirectory(Engine/CryEntitySystem)
add_subdirectory(Engine/CryAnimation)
add_subdirectory(Engine/CryAISystem)
add_subdirectory(Engine/CrySoundSystem)
add_subdirectory(Engine/CryInput)
add_subdirectory(Engine/Cry3DEngine)
add_subdirectory(Engine/XRenderOGL)
add_subdirectory(Engine/CryGame)
add_subdirectory(Engine/CryScriptSystem)
add_subdirectory(Engine/CryFont)
# CryMovie excluded — TCB spline template code requires extensive MSVC→GCC porting
# add_subdirectory(Engine/CryMovie)
# CryNetwork excluded — multiplayer not supported on Vita
# add_subdirectory(Engine/CryNetwork)

# ============================================================
#  Main executable
# ============================================================
file(GLOB_RECURSE MAIN_SOURCES
    "${ENGINE_ROOT}/FARCRY/Main.cpp"
)

add_executable(FarCry ${MAIN_SOURCES})

target_link_libraries(FarCry
    # Port layers
    vita_platform
    vita_shims

    # Engine modules (link order matters)
    cry_game
    cry_script
    cry_entity
    cry_animation
    cry_ai
    cry_sound
    cry_input
    cry_3dengine
    cry_render_ogl
    cry_physics
    cry_font
    # cry_movie — cutscene system excluded on Vita
    # cry_network — multiplayer excluded on Vita
    cry_system

    # vitaSDK system libraries
    SceLibKernel_stub
    SceDisplay_stub
    SceGxm_stub
    SceCtrl_stub
    SceTouchForDriver_stub
    SceAudio_stub
    SceAudioIn_stub
    SceIofilemgr_stub
    SceNet_stub
    SceNetCtl_stub
    ScePower_stub
    SceRtc_stub
    SceSysmodule_stub
    SceCommonDialog_stub
    SceAppMgr_stub

    # Third-party (bundled via vdpm)
    vitaGL
    SceShaccCg_stub
    mathneon
    z
    m
)

# ── Package as VPK ────────────────────────────────────────────
vita_create_self(FarCry.self FarCry)
vita_create_vpk(FarCry.vpk ${VITA_TITLEID} FarCry.self
    VERSION ${VITA_VERSION}
    NAME    ${VITA_APP_NAME}
    FILE    "${CMAKE_SOURCE_DIR}/assets/vita/sce_sys/icon0.png"  sce_sys/icon0.png
    FILE    "${CMAKE_SOURCE_DIR}/assets/vita/sce_sys/livearea/contents/bg.png"
            sce_sys/livearea/contents/bg.png
    FILE    "${CMAKE_SOURCE_DIR}/assets/vita/sce_sys/livearea/contents/startup.png"
            sce_sys/livearea/contents/startup.png
    FILE    "${CMAKE_SOURCE_DIR}/assets/vita/sce_sys/livearea/contents/template.xml"
            sce_sys/livearea/contents/template.xml
)
