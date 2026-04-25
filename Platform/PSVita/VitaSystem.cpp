////////////////////////////////////////////////////////////////////////////
//  Far Cry 1 — PS Vita Port
//  VitaSystem.cpp  —  System initialisation and entry point
////////////////////////////////////////////////////////////////////////////

#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/modulemgr.h>
#include <psp2/sysmodule.h>
#include <psp2/power.h>
#include <psp2/ctrl.h>
#include <psp2/touch.h>
#include <psp2/display.h>
#include <psp2/gxm.h>
#include <stdio.h>
#include <stdlib.h>

// ── vitaGL ───────────────────────────────────────────────────────────────
#include <vitaGL.h>

// ── Engine interfaces (from CryCommon) ───────────────────────────────────
#include <ISystem.h>
#include <ILog.h>

// Vita requires the game to declare its heap size
int _newlib_heap_size_user = 192 * 1024 * 1024;  // 192 MB heap

// ── Forward declarations from engine main ────────────────────────────────
extern int FarCryMain(int argc, char** argv);

// ── Vita system modules to load ──────────────────────────────────────────
static const SceSysmoduleModuleId g_vita_modules[] = {
    SCE_SYSMODULE_NET,
    SCE_SYSMODULE_HTTP,
    SCE_SYSMODULE_HTTPS,
    SCE_SYSMODULE_LIBSSL,
    SCE_SYSMODULE_RUDP,
};

static void vita_load_modules()
{
    for (size_t i = 0; i < sizeof(g_vita_modules) / sizeof(g_vita_modules[0]); ++i)
        sceSysmoduleLoadModule(g_vita_modules[i]);
}

static void vita_init_gxm()
{
    // vitaGL handles GXM initialisation when vglInit is called
    // Parameters: (vram_threshold_kb) — give vitaGL 64 MB of VRAM
    vglInit(64 * 1024);
}

// ── Main entry point ─────────────────────────────────────────────────────
int main(int argc, char* argv[])
{
    // Boost CPU + GPU clocks to maximum for best performance
    scePowerSetArmClockFrequency(444);
    scePowerSetBusClockFrequency(222);
    scePowerSetGpuClockFrequency(222);
    scePowerSetGpuXbarClockFrequency(166);

    vita_load_modules();
    vita_init_gxm();

    // Enable both analogue pads and triggers
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG_WIDE);
    sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START);
    sceTouchSetSamplingState(SCE_TOUCH_PORT_BACK,  SCE_TOUCH_SAMPLING_STATE_START);

    // Hand over to the engine
    int ret = FarCryMain(argc, argv);

    vglEnd();
    sceKernelExitProcess(ret);
    return ret;
}

// ── Platform utilities used by CrySystem ─────────────────────────────────

extern "C"
{

// Vita equivalent of QueryPerformanceCounter / QueryPerformanceFrequency
unsigned long long VitaGetTimeMicroseconds()
{
    SceRtcTick tick;
    sceRtcGetCurrentTick(&tick);
    return tick.tick;
}

// Sleep wrapper
void VitaSleep(unsigned int milliseconds)
{
    sceKernelDelayThread(milliseconds * 1000);
}

// Shared-library stubs (engine tries to LoadLibrary / GetProcAddress on Win32).
// On Vita everything is statically linked, so we stub these out.
void* VitaLoadLibrary(const char* /*path*/)  { return (void*)1; }
void  VitaFreeLibrary(void* /*handle*/)      {}
void* VitaGetProcAddress(void* /*handle*/, const char* /*name*/) { return NULL; }

} // extern "C"
