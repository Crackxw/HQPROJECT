#include	<GSL.h>

#include "cltLoadXsprProc.h"
#include "directx.h"
#include <Process.h>



//----------------------------------------------------------------------------------------------------------
unsigned __stdcall loadxspr_worker( LPVOID param )
{

	cltLoadXsprProc	*pClass = (cltLoadXsprProc*)param;

	DWORD	dwRet;

	while(1)
	{
		dwRet	=	WaitForSingleObject( pClass->m_hEvent, 200 );

		if( dwRet == WAIT_OBJECT_0 )
		{
			ResetEvent( pClass->m_hEvent );

			if( pClass->m_siAction == LOADXSPR::ACTION_START )
			{
				pClass->m_siState	=	LOADXSPR::STATE_RUNNING;
			}
			else if( pClass->m_siAction == LOADXSPR::ACTION_STOP )
			{
				pClass->m_siState	=	LOADXSPR::STATE_STOPPED;
				break;
			}
		}
		else if( dwRet == WAIT_TIMEOUT && pClass->m_siState == LOADXSPR::STATE_RUNNING )
		{
			pClass->Run();
		}
	}

	return 0;
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: ������
//------------------------------------------------------------------------------------------------------------
cltLoadXsprProc::cltLoadXsprProc()
{
//	m_pMemMgr	=	new MemMgr(30, 2000);

	// Work Queue, Complete Queue ����
	m_pWorkQueue			=	new cltXsprQueue( MAX_WORKQUEUE );
	m_pWorkCompleteQueue	=	new cltXsprQueue( MAX_WORKCOMPLETEQUEUE );
	m_pFreeCompleteQueue	=	new cltXsprQueue( MAX_FREECOMPLETEQUEUE );
	m_pFreeList				=	new cltFreeXsprList( MAX_FREELIST );

	// �̺�Ʈ ��ü ����
	m_hEvent	=	CreateEvent( NULL, FALSE, FALSE, NULL );

	// ������ ����
	UI32 threadID;
	HANDLE hThread = (HANDLE) _beginthreadex( NULL, 0, loadxspr_worker, (LPVOID)this, 0, &threadID );
	CloseHandle( hThread );

	Start();
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: �Ҹ���
//------------------------------------------------------------------------------------------------------------
cltLoadXsprProc::~cltLoadXsprProc()
{
	Stop();

	delete m_pWorkQueue;
	delete m_pWorkCompleteQueue;
	delete m_pFreeCompleteQueue;
	delete m_pFreeList;

	CloseHandle( m_hEvent );

//	delete	m_pMemMgr;
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: �����带 �����Ѵ�.
//------------------------------------------------------------------------------------------------------------
VOID	cltLoadXsprProc::Start()
{
	m_siAction	=	LOADXSPR::ACTION_START;
	SetEvent( m_hEvent );
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: �����带 �����Ѵ�.
//------------------------------------------------------------------------------------------------------------
VOID	cltLoadXsprProc::Stop()
{
	m_siAction	=	LOADXSPR::ACTION_STOP;
	SetEvent( m_hEvent );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Peek Queue
//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: Work Queue�� �����Ͱ� ����ִ��� �鿩�ٺ���.
//------------------------------------------------------------------------------------------------------------

BOOL	cltLoadXsprProc::PeekWorkQueue()
{
	return	m_pWorkQueue->Peek();
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: Complete Queue�� �����Ͱ� ����ִ��� �鿩�ٺ���.
//------------------------------------------------------------------------------------------------------------
BOOL	cltLoadXsprProc::PeekCompleteQueue()
{
	return	m_pWorkCompleteQueue->Peek();
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: Free Queue�� �����Ͱ� ����ִ��� �鿩�ٺ���.
//------------------------------------------------------------------------------------------------------------
BOOL	cltLoadXsprProc::PeekFreeQueue()
{
	return	m_pFreeCompleteQueue->Peek();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enqueue
//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: Work Queue�� �����͸� �ϳ� �߰��Ѵ�.
//------------------------------------------------------------------------------------------------------------
BOOL	cltLoadXsprProc::EnqueueWorkQueue( strXsprQueueData stData )
{
	return	m_pWorkQueue->Enqueue( stData );
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: Complete Queue�� �����͸� �ϳ� �߰��Ѵ�.
//------------------------------------------------------------------------------------------------------------
BOOL	cltLoadXsprProc::EnqueueCompleteQueue( strXsprQueueData stData )
{
	return	m_pWorkCompleteQueue->Enqueue( stData );
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: Free Queue�� �����͸� �ϳ� �߰��Ѵ�.
//------------------------------------------------------------------------------------------------------------
BOOL	cltLoadXsprProc::EnqueueFreeQueue( strXsprQueueData stData )
{
	return	m_pFreeCompleteQueue->Enqueue( stData );
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dequeue
//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: Work Queue���� �����͸� �ϳ� ������.
//------------------------------------------------------------------------------------------------------------
BOOL	cltLoadXsprProc::DequeueWorkQueue( strXsprQueueData &stData )
{
	return	m_pWorkQueue->Dequeue( stData );
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: Complete Queue���� �����͸� �ϳ� ������.
//------------------------------------------------------------------------------------------------------------
BOOL	cltLoadXsprProc::DequeueCompleteQueue( strXsprQueueData &stData )
{
	return	m_pWorkCompleteQueue->Dequeue( stData );
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: Free Queue���� �����͸� �ϳ� ������.
//------------------------------------------------------------------------------------------------------------
BOOL	cltLoadXsprProc::DequeueFreeQueue( strXsprQueueData &stData )
{
	return	m_pFreeCompleteQueue->Dequeue( stData );
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: pXspr�� ������ �ִ� �����͸� work queue���� ������ ������.
//------------------------------------------------------------------------------------------------------------
BOOL	cltLoadXsprProc::StrongDequeueWorkQueue( XSPR *pXspr, strXsprQueueData &stData )
{
	return	m_pWorkQueue->StrongDequeue( pXspr, stData );
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: pXspr�� ������ �ִ� �����͸� complete queue���� ������ ������.
//------------------------------------------------------------------------------------------------------------
BOOL	cltLoadXsprProc::StrongDequeueCompleteQueue( XSPR *pXspr, strXsprQueueData &stData )
{
	return	m_pWorkCompleteQueue->StrongDequeue( pXspr, stData );
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc:
//------------------------------------------------------------------------------------------------------------
VOID	cltLoadXsprProc::Run()
{
	static DWORD	lastSleepTime	=	GetTickCount();
	while( PeekWorkQueue() || (m_pFreeList->GetLength() > 0) )
	{
		///////////////////////////////////////
		// �ε��ؾ��� Xspr���� �ε��Ѵ�.
		if( PeekWorkQueue() )
		{
//			Sleep(200);
			strXsprQueueData	stData;

			// Work queue���� ù �����͸� ���´�.
			DequeueWorkQueue( stData );

			// �� �����ͷ� Xspr�� �޸𸮿� �ε��Ѵ�.
//			m_pMemMgr->AllocMem(stData.szFileName, *stData.pXspr);
			clGrp.LoadXspr(stData.szFileName, *stData.pXspr, FALSE);
			if( GetTickCount() - lastSleepTime > 5 )
			{
				if( GetWorkQueueLength() < MAX_WORKQUEUE )
				{
					if( GetWorkQueueLength() > 200 )	Sleep(20);
					else								Sleep(45);	
				}
				lastSleepTime = GetTickCount();
			}

			// WorkCompleteQueue�� main thread���� ���� �����Ƿ� ��а� enqueue���� �ʴ´�.
			//while( !EnqueueCompleteQueue(stData) )	Sleep(10);
		}

		///////////////////////////////////////
		// �����ؾ��� Xspr���� �����Ѵ�.
		strXsprQueueData	*pstItem;
		m_pFreeList->ResetPos();
		while( (pstItem = m_pFreeList->GetNextData()) != NULL )
		{
			strXsprQueueData	stDummy;

			// �޸𸮿� �ε� �Ϸ�� ���� �׳� ������Ų��.
			// work queue���� �ε��Ϸ��� ������̸� �����ϰ� ������Ų��.
			if( pstItem->pXspr->bLoadComplete || m_pWorkQueue->StrongDequeue(pstItem->pXspr, stDummy) )
			{
//				m_pMemMgr->Free(*pstItem->pXspr);
				clGrp.FreeXspr(*pstItem->pXspr);

				// ������ ���� free queue�� �ְ� free list���� ����. (���� �ٲ��� ����)
				while( !EnqueueFreeQueue(*pstItem) )	Sleep(10);
				m_pFreeList->DeleteXspr( pstItem );
			}
		}
	}
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc:
//------------------------------------------------------------------------------------------------------------
SI32	cltLoadXsprProc::GetWorkQueueLength()
{	
	return	m_pWorkQueue->GetLength();
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc:
//------------------------------------------------------------------------------------------------------------
SI32	cltLoadXsprProc::GetCompleteQueueLength()
{	
	return	m_pWorkCompleteQueue->GetLength();
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc:
//------------------------------------------------------------------------------------------------------------
SI32	cltLoadXsprProc::GetFreeQueueLength()
{	
	return	m_pFreeCompleteQueue->GetLength();
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc:
//------------------------------------------------------------------------------------------------------------
SI32	cltLoadXsprProc::GetFreeListLength()
{	
	return	m_pFreeList->GetLength();
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc:
//------------------------------------------------------------------------------------------------------------
BOOL	cltLoadXsprProc::AddFreeList( strXsprQueueData *pstData )
{	
	return	m_pFreeList->AddXspr( pstData );

}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc:
//------------------------------------------------------------------------------------------------------------
BOOL	cltLoadXsprProc::HasStopped()
{	
	if( m_siState == LOADXSPR::STATE_STOPPED )		return TRUE;
	else											return FALSE;
}

/*
BOOL	cltLoadXsprProc::AllocXsprMem(char* filename, XSPR& xspr)
{
	return m_pMemMgr->AllocMem(filename, xspr);
}

BOOL	cltLoadXsprProc::FreeXspr(XSPR& xspr)
{
	return m_pMemMgr->Free(xspr);
}
*/