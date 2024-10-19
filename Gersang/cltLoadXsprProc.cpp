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
// Desc: 생성자
//------------------------------------------------------------------------------------------------------------
cltLoadXsprProc::cltLoadXsprProc()
{
//	m_pMemMgr	=	new MemMgr(30, 2000);

	// Work Queue, Complete Queue 생성
	m_pWorkQueue			=	new cltXsprQueue( MAX_WORKQUEUE );
	m_pWorkCompleteQueue	=	new cltXsprQueue( MAX_WORKCOMPLETEQUEUE );
	m_pFreeCompleteQueue	=	new cltXsprQueue( MAX_FREECOMPLETEQUEUE );
	m_pFreeList				=	new cltFreeXsprList( MAX_FREELIST );

	// 이벤트 객체 생성
	m_hEvent	=	CreateEvent( NULL, FALSE, FALSE, NULL );

	// 쓰레드 생성
	UI32 threadID;
	HANDLE hThread = (HANDLE) _beginthreadex( NULL, 0, loadxspr_worker, (LPVOID)this, 0, &threadID );
	CloseHandle( hThread );

	Start();
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: 소멸자
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
// Desc: 쓰레드를 시작한다.
//------------------------------------------------------------------------------------------------------------
VOID	cltLoadXsprProc::Start()
{
	m_siAction	=	LOADXSPR::ACTION_START;
	SetEvent( m_hEvent );
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: 쓰레드를 종료한다.
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
// Desc: Work Queue에 데이터가 들어있는지 들여다본다.
//------------------------------------------------------------------------------------------------------------

BOOL	cltLoadXsprProc::PeekWorkQueue()
{
	return	m_pWorkQueue->Peek();
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: Complete Queue에 데이터가 들어있는지 들여다본다.
//------------------------------------------------------------------------------------------------------------
BOOL	cltLoadXsprProc::PeekCompleteQueue()
{
	return	m_pWorkCompleteQueue->Peek();
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: Free Queue에 데이터가 들어있는지 들여다본다.
//------------------------------------------------------------------------------------------------------------
BOOL	cltLoadXsprProc::PeekFreeQueue()
{
	return	m_pFreeCompleteQueue->Peek();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enqueue
//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: Work Queue에 데이터를 하나 추가한다.
//------------------------------------------------------------------------------------------------------------
BOOL	cltLoadXsprProc::EnqueueWorkQueue( strXsprQueueData stData )
{
	return	m_pWorkQueue->Enqueue( stData );
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: Complete Queue에 데이터를 하나 추가한다.
//------------------------------------------------------------------------------------------------------------
BOOL	cltLoadXsprProc::EnqueueCompleteQueue( strXsprQueueData stData )
{
	return	m_pWorkCompleteQueue->Enqueue( stData );
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: Free Queue에 데이터를 하나 추가한다.
//------------------------------------------------------------------------------------------------------------
BOOL	cltLoadXsprProc::EnqueueFreeQueue( strXsprQueueData stData )
{
	return	m_pFreeCompleteQueue->Enqueue( stData );
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dequeue
//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: Work Queue에서 데이터를 하나 꺼낸다.
//------------------------------------------------------------------------------------------------------------
BOOL	cltLoadXsprProc::DequeueWorkQueue( strXsprQueueData &stData )
{
	return	m_pWorkQueue->Dequeue( stData );
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: Complete Queue에서 데이터를 하나 꺼낸다.
//------------------------------------------------------------------------------------------------------------
BOOL	cltLoadXsprProc::DequeueCompleteQueue( strXsprQueueData &stData )
{
	return	m_pWorkCompleteQueue->Dequeue( stData );
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: Free Queue에서 데이터를 하나 꺼낸다.
//------------------------------------------------------------------------------------------------------------
BOOL	cltLoadXsprProc::DequeueFreeQueue( strXsprQueueData &stData )
{
	return	m_pFreeCompleteQueue->Dequeue( stData );
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: pXspr을 가지고 있는 데이터를 work queue에서 강제로 빼낸다.
//------------------------------------------------------------------------------------------------------------
BOOL	cltLoadXsprProc::StrongDequeueWorkQueue( XSPR *pXspr, strXsprQueueData &stData )
{
	return	m_pWorkQueue->StrongDequeue( pXspr, stData );
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: pXspr을 가지고 있는 데이터를 complete queue에서 강제로 빼낸다.
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
		// 로드해야할 Xspr들을 로드한다.
		if( PeekWorkQueue() )
		{
//			Sleep(200);
			strXsprQueueData	stData;

			// Work queue에서 첫 데이터를 얻어온다.
			DequeueWorkQueue( stData );

			// 그 데이터로 Xspr을 메모리에 로드한다.
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

			// WorkCompleteQueue는 main thread에서 쓰지 않으므로 당분간 enqueue하지 않는다.
			//while( !EnqueueCompleteQueue(stData) )	Sleep(10);
		}

		///////////////////////////////////////
		// 해제해야할 Xspr들을 해제한다.
		strXsprQueueData	*pstItem;
		m_pFreeList->ResetPos();
		while( (pstItem = m_pFreeList->GetNextData()) != NULL )
		{
			strXsprQueueData	stDummy;

			// 메모리에 로드 완료된 것은 그냥 해제시킨다.
			// work queue에서 로드하려고 대기중이면 제거하고 해제시킨다.
			if( pstItem->pXspr->bLoadComplete || m_pWorkQueue->StrongDequeue(pstItem->pXspr, stDummy) )
			{
//				m_pMemMgr->Free(*pstItem->pXspr);
				clGrp.FreeXspr(*pstItem->pXspr);

				// 해제된 것은 free queue에 넣고 free list에서 뺀다. (순서 바꾸지 말것)
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