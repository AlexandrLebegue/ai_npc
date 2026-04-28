// CryScriptSystem.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#if !defined(_XBOX)
#if !defined(LINUX) && !defined(VITA)
_ACCESS_POOL;
#endif
#if !defined(LINUX) && !defined(VITA)
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}
#endif
#endif


#include <string>
#include <map>
#include <CrtDebugStats.h>
