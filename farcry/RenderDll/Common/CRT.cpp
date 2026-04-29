#if defined(LINUX) || defined(VITA)
	#include "platform.h"
#else
	#include "Windows.h"
#endif

void CRTFreeData(void *pData)
{
  free(pData);
}

void CRTDeleteArray(void *pData)
{
  delete [] pData;
}