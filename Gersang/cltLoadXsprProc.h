#ifndef	_LOADXSPR_H_
#define	_LOADXSPR_H_

#include "Graphics16.h"
#include "cltXsprQueue.h"
#include "cltFreeXsprList.h"
//#include "MemMgr.h"

#define	MAX_WORKQUEUE			1000			// �ε��� xpr���� ������ ��� ť�� ũ��
#define MAX_WORKCOMPLETEQUEUE	1000			// �Ϸ�� xpr���� ������ ��� ť�� ũ��
#define	MAX_FREELIST			3000		// ������ Xspr���� ������ ��� ����Ʈ�� ũ��
#define MAX_FREECOMPLETEQUEUE	3000		// ���� �Ϸ�� xpr���� ������ ��� ť�� ũ��


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
	cltXsprQueue		*m_pFreeCompleteQueue;		// Free �Ϸ�� �͵��� �� ť
	cltFreeXsprList		*m_pFreeList;				// Free �ؾ��� �͵��� ����Ʈ

//	MemMgr				*m_pMemMgr;					// �޸� �Ҵ�� �̹��� �ε��� ���

};

#endif
