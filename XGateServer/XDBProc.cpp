//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 8
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


#include "XDBProc.h"

#include "..\\XCommon\\JXWorkManager.h"
#include "..\\XCommon\\JXDBController.h"
#include "..\\XCommon\\JXPacket.h"

unsigned __stdcall XDBProc_ThreadFunc( LPVOID param )
{
	XDBProc *pDBProc = (XDBProc *)param;
	DWORD dwRet;

	while( 1 ) {

		dwRet = WaitForSingleObject( pDBProc->m_hStopEvent, 50 );
		
		if( dwRet == WAIT_OBJECT_0 ) break;

		pDBProc->Run();
	}

	printf("XDBProc Thread End..\n" );

	SetEvent( pDBProc->m_hExitEvent );

	return 0;
}

XDBProc::XDBProc()
{

	m_bThreadRun = FALSE;
	m_uiThreadID = 0;

	m_hStopEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
	m_hExitEvent = CreateEvent( NULL, FALSE, FALSE, NULL );

	m_pDBController = NULL;
	m_pWorkManager = NULL;

}

XDBProc::~XDBProc()
{
	StopThread();

	CloseHandle( m_hStopEvent );
	CloseHandle( m_hExitEvent );

	if( m_pDBController ) delete m_pDBController;
}

void XDBProc::Create( JXWorkManager *pWorkManager, 
					 char *pszServerName, char *pszDatabaseName, 
					 char *pszUser, char *pszPassword )
{
	m_pWorkManager = pWorkManager;

	if( m_pDBController ) delete m_pDBController;
	m_pDBController = new JXDBController;

	m_pDBController->Create( pszServerName, pszDatabaseName, pszUser, pszPassword );
	
}

void XDBProc::StartThread()
{
	if( m_bThreadRun == TRUE ) return;

	m_bThreadRun = TRUE;

	HANDLE hThread = (HANDLE) _beginthreadex( NULL, 0, XDBProc_ThreadFunc, (LPVOID)this, 0, &m_uiThreadID );
	CloseHandle( hThread );

}

void XDBProc::StopThread()
{
	if( m_bThreadRun == FALSE ) return;

	m_bThreadRun = FALSE;

	SetEvent( m_hStopEvent );

	WaitForSingleObject( m_hExitEvent, INFINITE );
}

void XDBProc::Run()
{
	// WorkID와 Packet을 받아온다..
	if( m_pWorkManager->GetQuery( &local_dwWorkID, &local_packet ) == FALSE ) return;


}


