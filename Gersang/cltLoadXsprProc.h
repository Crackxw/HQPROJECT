#ifndef	_LOADXSPR_H_
#define	_LOADXSPR_H_

#include "Graphics16.h"
#include "cltXsprQueue.h"
#include "cltFreeXsprList.h"
//#include "MemMgr.h"

#define	MAX_WORKQUEUE			1000			// 로드할 xpr들의 정보를 담는 큐의 크기
#define MAX_WORKCOMPLETEQUEUE	1000			// 완료된 xpr들의 정보를 담는 큐의 크기
#define	MAX_FREELIST			3000		// 해제할 Xspr들의 정보를 담는 리스트의 크기
#define MAX_FREECOMPLETEQUEUE	3000		// 해제 완료된 xpr들의 정보를 담는 큐의 크기


namespace	LOADXSPR
{
	enum
	{
		ACTION_START,
		ACTION_STOP
	};

	enum
	{
		STATE_RUNNING,
		STATE_STOPPED
	};
};


class cltLoadXsprProc
{

public:
	cltLoadXsprProc();
	~cltLoadXsprProc();

public:
	VOID	Start();
	VOID	Stop();
	VOID	Run();

	BOOL	PeekWorkQueue();
	BOOL	PeekCompleteQueue();
	BOOL	PeekFreeQueue();

	BOOL	EnqueueWorkQueue( strXsprQueueData stData );
	BOOL	EnqueueCompleteQueue( strXsprQueueData stData );
	BOOL	EnqueueFreeQueue( strXsprQueueData stData );

	BOOL	DequeueWorkQueue( strXsprQueueData &stData );
	BOOL	DequeueCompleteQueue( strXsprQueueData &stData );
	BOOL	DequeueFreeQueue( strXsprQueueData &stData );

	BOOL	StrongDequeueWorkQueue( XSPR *pXspr, strXsprQueueData &stData );
	BOOL	StrongDequeueCompleteQueue( XSPR *pXspr, strXsprQueueData &stData );
	SI32	GetWorkQueueLength();
	SI32	GetCompleteQueueLength();
	SI32	GetFreeQueueLength();
	SI32	GetFreeListLength();
	SI32	AddFreeList( strXsprQueueData *pstData );
	BOOL	HasStopped();

	BOOL	AllocXsprMem(char* filename, XSPR& xspr);
	BOOL	FreeXspr(XSPR& xspr);

public:
	SI32				m_siState;
	SI32				m_siAction;
	HANDLE				m_hEvent;

private:
	CRITICAL_SECTION	m_csFreeList;
	cltXsprQueue		*m_pWorkQueue;
	cltXsprQueue		*m_pWorkCompleteQueue;
	cltXsprQueue		*m_pFreeCompleteQueue;		// Free 완료된 것들이 들어갈 큐
	cltFreeXsprList		*m_pFreeList;				// Free 해야할 것들의 리스트

//	MemMgr				*m_pMemMgr;					// 메모리 할당과 이미지 로딩을 담당

};

#endif
