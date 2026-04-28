// Vita stubs for CSystem methods defined in SystemWin32.cpp on Windows
#include "StdAfx.h"
#include "System.h"

// gDLLHandle is defined inside #ifdef WIN32 in DllMain.cpp — provide it here for Vita
HMODULE gDLLHandle = NULL;

void CSystem::SetAffinity()
{
    // On Vita all 4 cores are available; no affinity API needed
}

void CSystem::DumpMemoryUsageStatistics()
{
    // No-op on Vita
}

const char* CSystem::GetUserName()
{
    return "VitaPlayer";
}

void CSystem::TickMemStats(MemStatsPurposeEnum /*nPurpose*/)
{
    // No-op on Vita
}

int CSystem::DumpMMStats(bool /*log*/)
{
    return 0;
}

void CSystem::DebugStats(bool /*checkpoint*/, bool /*leaks*/)
{
    // No-op on Vita
}

void CSystem::DumpWinHeaps()
{
    // No-op on Vita
}

bool CSystem::GetSSFileInfo(const char* /*inszFileName*/, char* outszInfo, const DWORD indwBufferSize)
{
    if (outszInfo && indwBufferSize > 0)
        outszInfo[0] = '\0';
    return false;
}

int CSystem::AutoDetectRenderer(char* Vendor, char* Device)
{
    if (Vendor) strcpy(Vendor, "CryRenderOGL");
    if (Device) strcpy(Device, "CryRenderOGL");
    return 1;
}

void CSystem::Error(const char* szCommand, ...)
{
    va_list args;
    va_start(args, szCommand);
    char buf[512];
    vsnprintf(buf, sizeof(buf)-1, szCommand, args);
    va_end(args);
    buf[sizeof(buf)-1] = '\0';
    if (m_pLog)
        m_pLog->LogError("%s", buf);
}
