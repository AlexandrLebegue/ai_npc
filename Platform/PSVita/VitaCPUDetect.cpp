// Vita stub for x86 CPU detection
#include "StdAfx.h"
#include "CPUDetect.h"

void CCpuFeatures::Detect(void)
{
    // ARM Cortex-A9 quad-core — no x86 CPUID
    m_NumSystemProcessors = 4;
    m_NumAvailProcessors  = 4;
    m_bOS_ISSE            = false;
    m_bOS_ISSE_EXCEPTIONS = false;

    SCpu& cpu = m_Cpu[0];
    memset(&cpu, 0, sizeof(cpu));
    cpu.meVendor = eCVendor_Unknown;
    cpu.meModel  = eCpu_Unknown;
    strcpy(cpu.mVendor,  "ARM");
    strcpy(cpu.mCpuType, "Cortex-A9");
    strcpy(cpu.mFpuType, "VFPv3-D16+NEON");
    cpu.mSpeed           = 444;   // MHz
    cpu.m_SecondsPerCycle = 1.0 / (444.0 * 1e6);
    // NEON ≈ SSE from the engine's point of view
    cpu.mFeatures = CFI_MMX | CFI_SSE;
}
