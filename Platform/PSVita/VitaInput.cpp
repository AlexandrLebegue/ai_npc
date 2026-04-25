////////////////////////////////////////////////////////////////////////////
//  Far Cry 1 — PS Vita Port
//  VitaInput.cpp  —  DualShock / touchscreen input bridge
//
//  Maps PS Vita controls to Far Cry's IInput interface.
//  The engine calls IInput::Update() each frame; we fill its key/axis events.
////////////////////////////////////////////////////////////////////////////

#include <psp2/ctrl.h>
#include <psp2/touch.h>
#include <string.h>
#include <stdio.h>

#include <IInput.h>     // CryCommon

// ── Button → key name mapping ─────────────────────────────────────────────
struct VitaButtonMap
{
    unsigned int  vitaButton;
    const char*   cryKeyName;
};

// Default Far Cry keyboard mappings re-bound to Vita buttons
static const VitaButtonMap g_buttonMap[] =
{
    { SCE_CTRL_CROSS,       "xi_a"          },   // Jump / confirm
    { SCE_CTRL_CIRCLE,      "xi_b"          },   // Cancel / crouch
    { SCE_CTRL_SQUARE,      "xi_x"          },   // Reload
    { SCE_CTRL_TRIANGLE,    "xi_y"          },   // Use / interact
    { SCE_CTRL_L1,          "xi_left_shoulder"  },   // Grenade
    { SCE_CTRL_R1,          "xi_right_shoulder" },   // Shoot (primary)
    { SCE_CTRL_L2,          "xi_left_trigger"   },   // Zoom / ADS
    { SCE_CTRL_R2,          "xi_right_trigger"  },   // Shoot (secondary)
    { SCE_CTRL_L3,          "xi_left_thumb"     },   // Sprint
    { SCE_CTRL_R3,          "xi_right_thumb"    },   // Melee
    { SCE_CTRL_DPAD_UP,     "up"            },
    { SCE_CTRL_DPAD_DOWN,   "down"          },
    { SCE_CTRL_DPAD_LEFT,   "left"          },
    { SCE_CTRL_DPAD_RIGHT,  "right"         },
    { SCE_CTRL_START,       "escape"        },   // Pause menu
    { SCE_CTRL_SELECT,      "tab"           },   // Map / inventory
};
static const int g_buttonMapCount = sizeof(g_buttonMap) / sizeof(g_buttonMap[0]);

// ── Analogue axis normalisation ───────────────────────────────────────────
// Vita reports 0-255, dead-zone ≈ 10 %
static float NormaliseAxis(unsigned char raw)
{
    float v = ((float)raw - 128.0f) / 128.0f;
    if (v > -0.1f && v < 0.1f) v = 0.0f;
    return v;
}

// ── Internal state ────────────────────────────────────────────────────────
static SceCtrlData  g_prevPad = {};
static SceCtrlData  g_curPad  = {};

// ── Public API called from the Vita CryInput module ──────────────────────

void VitaInput_Init()
{
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG_WIDE);
    sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START);
    sceTouchSetSamplingState(SCE_TOUCH_PORT_BACK,  SCE_TOUCH_SAMPLING_STATE_START);
    memset(&g_prevPad, 0, sizeof(g_prevPad));
    memset(&g_curPad,  0, sizeof(g_curPad));
    printf("[VitaInput] Initialised\n");
}

void VitaInput_Update(IInput* pInput)
{
    if (!pInput) return;

    g_prevPad = g_curPad;
    sceCtrlReadBufferPositive(0, &g_curPad, 1);

    unsigned int pressed  = g_curPad.buttons  & ~g_prevPad.buttons;
    unsigned int released = g_prevPad.buttons & ~g_curPad.buttons;

    // Button events
    for (int i = 0; i < g_buttonMapCount; ++i)
    {
        unsigned int mask = g_buttonMap[i].vitaButton;
        if (pressed  & mask)
            pInput->PostInputEvent( SInputEvent(eIDT_Gamepad, eIS_Pressed,  g_buttonMap[i].cryKeyName) );
        if (released & mask)
            pInput->PostInputEvent( SInputEvent(eIDT_Gamepad, eIS_Released, g_buttonMap[i].cryKeyName) );
    }

    // Left stick  →  move axes
    float lx = NormaliseAxis(g_curPad.lx);
    float ly = NormaliseAxis(g_curPad.ly);
    if (lx != 0.0f)
        pInput->PostInputEvent( SInputEvent(eIDT_Gamepad, eIS_Changed, "xi_thumblx", lx) );
    if (ly != 0.0f)
        pInput->PostInputEvent( SInputEvent(eIDT_Gamepad, eIS_Changed, "xi_thumbly", ly) );

    // Right stick  →  look axes
    float rx = NormaliseAxis(g_curPad.rx);
    float ry = NormaliseAxis(g_curPad.ry);
    if (rx != 0.0f)
        pInput->PostInputEvent( SInputEvent(eIDT_Gamepad, eIS_Changed, "xi_thumbrx", rx) );
    if (ry != 0.0f)
        pInput->PostInputEvent( SInputEvent(eIDT_Gamepad, eIS_Changed, "xi_thumbry", ry) );
}

void VitaInput_Shutdown()
{
    sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_STOP);
    sceTouchSetSamplingState(SCE_TOUCH_PORT_BACK,  SCE_TOUCH_SAMPLING_STATE_STOP);
}
