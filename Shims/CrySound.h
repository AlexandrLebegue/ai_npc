// CrySound.h — stub for PS Vita (no FMOD SDK)
// Provides the minimum types that ISound.h references from CrySound.h.
#pragma once
#ifndef _CRY_SOUND_H_
#define _CRY_SOUND_H_

// CS_REVERB_PROPERTIES — FMOD EAX reverb preset structure (stubbed for Vita)
typedef struct {
    int    Environment;
    float  EnvSize;
    float  EnvDiffusion;
    int    Room;
    int    RoomHF;
    int    RoomLF;
    float  DecayTime;
    float  DecayHFRatio;
    float  DecayLFRatio;
    int    Reflections;
    float  ReflectionsDelay;
    float  ReflectionsPan[3];
    int    Reverb;
    float  ReverbDelay;
    float  ReverbPan[3];
    float  EchoTime;
    float  EchoDepth;
    float  ModulationTime;
    float  ModulationDepth;
    float  AirAbsorptionHF;
    float  HFReference;
    float  LFReference;
    float  RoomRolloffFactor;
    float  Diffusion;
    float  Density;
    unsigned int Flags;
} CS_REVERB_PROPERTIES;

// EAX preset placeholders
#ifndef CS_PRESET_OFF
#  define CS_PRESET_OFF  {0,1.0f,0.00f,-10000,-10000,-10000,0.1f,0.1f,0.1f,-10000,0.0f,{0.0f,0.0f,0.0f},-10000,0.0f,{0.0f,0.0f,0.0f},0.075f,0.0f,0.04f,0.0f,0.0f,1000.0f,20.0f,0.0f,0.0f,0.0f,0}
#endif

#endif // _CRY_SOUND_H_
