////////////////////////////////////////////////////////////////////////////
//  Far Cry 1 — PS Vita Port
//  VitaInput.cpp  —  DualShock / touchscreen → CryInput bridge
//
//  Provides VitaInput_GetState() which CryInput's Update() calls each
//  frame to build SInputEvent objects with the correct engine interface.
////////////////////////////////////////////////////////////////////////////

#include <psp2/ctrl.h>
#include <psp2/touch.h>
#include <string.h>
#include <stdio.h>

// Engine input interface (SInputEvent, KEY_PRESS / KEY_RELEASE)
#include <IInput.h>

// ── Vita D-pad constants (some SDK versions use different names) ──────────
#ifndef SCE_CTRL_DPAD_UP
#  define SCE_CTRL_DPAD_UP     SCE_CTRL_UP
#  define SCE_CTRL_DPAD_DOWN   SCE_CTRL_DOWN
#  define SCE_CTRL_DPAD_LEFT   SCE_CTRL_LEFT
#  define SCE_CTRL_DPAD_RIGHT  SCE_CTRL_RIGHT
#endif

// ── Button → key code mapping ─────────────────────────────────────────────
// We map Vita buttons to XKeys integers that the engine recognizes.
// (XKEY_* constants are defined in CryCommon/XKeys.h)
// For now use raw integers that match the XKEY table in the engine.
// Jump=space(32), Crouch=c(99), Reload=r(114), Use=f(102), etc.
struct VitaButtonMap { unsigned int button; int xkey; const char* name; };
static const VitaButtonMap g_map[] = {
    { SCE_CTRL_CROSS,       32,  "space"   },  // Jump
    { SCE_CTRL_CIRCLE,      99,  "c"       },  // Crouch
    { SCE_CTRL_SQUARE,     114,  "r"       },  // Reload
    { SCE_CTRL_TRIANGLE,   102,  "f"       },  // Use/interact
    { SCE_CTRL_L1,           1,  "lbumper" },  // Grenade
    { SCE_CTRL_R1,          42,  "*"       },  // Fire
    { SCE_CTRL_L2,         304,  "lshift"  },  // ADS / zoom
    { SCE_CTRL_R2,         42,   "*"       },  // Alt-fire
    { SCE_CTRL_L3,         304,  "lshift"  },  // Sprint (hold)
    { SCE_CTRL_R3,          86,  "v"       },  // Melee
    { SCE_CTRL_DPAD_UP,   273,   "up"      },  // weapon wheel
    { SCE_CTRL_DPAD_DOWN, 274,   "down"    },
    { SCE_CTRL_DPAD_LEFT, 276,   "left"    },
    { SCE_CTRL_DPAD_RIGHT,275,   "right"   },
    { SCE_CTRL_START,      27,   "escape"  },  // Pause
    { SCE_CTRL_SELECT,      9,   "tab"     },  // Map
};
static const int MAP_COUNT = (int)(sizeof(g_map)/sizeof(g_map[0]));

// ── Pad state ────────────────────────────────────────────────────────────
static SceCtrlData g_prev = {};
static SceCtrlData g_cur  = {};

// ── Queued input events (engine drains these each frame) ─────────────────
#define MAX_EVENTS 64
static SInputEvent g_events[MAX_EVENTS];
static int         g_eventCount = 0;

static float NormaliseAxis(unsigned char raw)
{
    float v = ((float)raw - 128.0f) / 128.0f;
    return (v > -0.1f && v < 0.1f) ? 0.0f : v;
}

void VitaInput_Init()
{
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG_WIDE);
    sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START);
    sceTouchSetSamplingState(SCE_TOUCH_PORT_BACK,  SCE_TOUCH_SAMPLING_STATE_START);
    memset(&g_prev, 0, sizeof(g_prev));
    memset(&g_cur,  0, sizeof(g_cur));
}

// Called by CryInput's Update() — fill g_events with new events.
void VitaInput_PollFrame()
{
    g_eventCount = 0;
    g_prev = g_cur;
    sceCtrlReadBufferPositive(0, &g_cur, 1);

    unsigned int pressed  = g_cur.buttons  & ~g_prev.buttons;
    unsigned int released = g_prev.buttons & ~g_cur.buttons;

    for (int i = 0; i < MAP_COUNT && g_eventCount < MAX_EVENTS; ++i) {
        if (pressed  & g_map[i].button) {
            SInputEvent& e     = g_events[g_eventCount++];
            e.type             = SInputEvent::KEY_PRESS;
            e.key              = g_map[i].xkey;
            e.keyname          = g_map[i].name;
            e.value            = 1.0f;
            e.timestamp        = g_cur.timeStamp;
            e.moidifiers       = 0;
        }
        if (released & g_map[i].button && g_eventCount < MAX_EVENTS) {
            SInputEvent& e     = g_events[g_eventCount++];
            e.type             = SInputEvent::KEY_RELEASE;
            e.key              = g_map[i].xkey;
            e.keyname          = g_map[i].name;
            e.value            = 0.0f;
            e.timestamp        = g_cur.timeStamp;
            e.moidifiers       = 0;
        }
    }

    // Analogue axes — store as synthetic MOUSE_MOVE for now (engine maps them)
    float lx = NormaliseAxis(g_cur.lx), ly = NormaliseAxis(g_cur.ly);
    float rx = NormaliseAxis(g_cur.rx), ry = NormaliseAxis(g_cur.ry);
    (void)lx; (void)ly; (void)rx; (void)ry;
    // Axes are fed directly to player movement in VitaInputSystem (CryInput override)
}

int         VitaInput_GetEventCount()        { return g_eventCount; }
SInputEvent VitaInput_GetEvent(int i)        { return g_events[i]; }

float VitaInput_GetAxisLX() { return NormaliseAxis(g_cur.lx); }
float VitaInput_GetAxisLY() { return NormaliseAxis(g_cur.ly); }
float VitaInput_GetAxisRX() { return NormaliseAxis(g_cur.rx); }
float VitaInput_GetAxisRY() { return NormaliseAxis(g_cur.ry); }

void VitaInput_Shutdown()
{
    sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_STOP);
    sceTouchSetSamplingState(SCE_TOUCH_PORT_BACK,  SCE_TOUCH_SAMPLING_STATE_STOP);
}
