#include <GSL.h>
#include "VSManager.h"
#include "assert.h"

void main(void)
{
	GSMemoryInit(GS_DEBUG_WINDOWS);

	VSManager *pVSMgr = new VSManager;
	assert(pVSMgr);
	VirtualLock(pVSMgr, sizeof(VSManager));

	pVSMgr->Process();

	delete pVSMgr;
}