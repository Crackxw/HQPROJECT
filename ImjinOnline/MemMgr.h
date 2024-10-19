#ifndef	_ONLINE_MEMMGR_HEADER
#define _ONLINE_MEMMGR_HEADER

#include "Directives.h"
#include "windows.h"
#include "cltList.h"
#include "cltXsprQueue.h"


struct MemInfo
{
	BYTE*		uiMemPos;
	UI32		uiAllocLength;
	UI32		uiEmptyLength;
	BOOL		bMgrAlloc;
};


class MemMgr
{
private:
	BYTE*					m_pMemBuffer;
	cltList<MemInfo>		m_MemList;
	cltList<int>			m_EmptyList;

public:
	MemMgr(UI32 MBMemSize, UI16 uiListSize);
	~MemMgr();


	BOOL		AllocMem(char* filename, XSPR& xspr);
	BOOL		Free(XSPR& xspr);
	MemInfo*	FindEmptyMemInfo(UI32 uiLength, SI16 &index);
};

#endif
