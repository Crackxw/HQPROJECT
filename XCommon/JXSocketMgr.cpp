//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완		
// 최종 수정일 : 2002. 7. 2	
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#include "JXSocketMgr.h"	
#include "JXPacket.h"		
#include "JXSocket.h"		

#include <process.h>		
#include <conio.h>			

unsigned __stdcall JXSocketMgr_Recv_ThreadFunc( LPVOID param )
{

	JXSocketMgr *pSocketMgr = (JXSocketMgr *)param;
	HANDLE event;
	DWORD dwRet;

	while( 1 ) {

		event = pSocketMgr->m_hStopEvent;

		dwRet = WaitForSingleObject( event, 10 );

		if( dwRet == WAIT_OBJECT_0 ) break;

		pSocketMgr->Recv();

	}

	printf("Socket Mgr.. End Thread... \n");

	SetEvent( pSocketMgr->m_hExitEvent );

	return 0;
}

unsigned __stdcall JXSocketMgr_Send_ThreadFunc( LPVOID param )
{

	JXSocketMgr *pSocketMgr = (JXSocketMgr *)param;
	HANDLE event;
	DWORD dwRet;

	while( 1 ) {

		event = pSocketMgr->m_hStopEvent;

		dwRet = WaitForSingleObject( event, 10 );

		if( dwRet == WAIT_OBJECT_0 ) break;


		pSocketMgr->Send();

	}

	printf("Socket Mgr.. End Thread... \n");

	SetEvent( pSocketMgr->m_hExitEvent );

	return 0;
}



JXSocketMgr::JXSocketMgr()
{
	FD_ZERO( &m_fdset );

	m_timeout.tv_sec = 0;
	m_timeout.tv_usec = 0;

	m_pSocket = NULL;

	m_bThreadRun = FALSE;
	m_uiThreadID = 0;

	m_hStopEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	m_hExitEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
}

JXSocketMgr::~JXSocketMgr()
{

	if( m_bThreadRun == TRUE ) {
		
		m_bThreadRun = FALSE;

		Wait( INFINITE );
		SetEvent( m_hStopEvent );
		Release();

		printf("Wait For SocketMgr Exit Event..");
		
		WaitForSingleObject( m_hExitEvent, INFINITE );

	}

	CloseHandle( m_hStopEvent );
	CloseHandle( m_hExitEvent );

	if( m_pSocket ) delete [] m_pSocket;

	printf( "SocketMgr Destory..\n" );
}

void JXSocketMgr::Create( SI32 siMaxNumber, SI32 siRecvBufSize, SI32 siSendBufSize, SI32 siPacketQueueSize )
{
	Wait( INFINITE );

	if( m_pSocket ) delete [] m_pSocket;

	m_pSocket = new JXSocket[ siMaxNumber ];

	for( SI32 i = 0; i < siMaxNumber; ++i ) {
		m_pSocket[ i ].Create( siRecvBufSize, siSendBufSize, siPacketQueueSize );
		AddToFree( &m_pSocket[ i ] );
	}

	m_bThreadRun = FALSE;

	Release();
}

void JXSocketMgr::StartThread()
{
	Wait( INFINITE );

	if( m_bThreadRun == TRUE ) {
		Release();
		return;
	}

	m_bThreadRun = TRUE;
	
	ResetEvent( m_hStopEvent );

	HANDLE hThread = (HANDLE)_beginthreadex( NULL, 0, JXSocketMgr_Recv_ThreadFunc, (LPVOID)this, 0, &m_uiThreadID );
	CloseHandle( hThread );

	hThread = (HANDLE)_beginthreadex( NULL, 0, JXSocketMgr_Send_ThreadFunc, (LPVOID)this, 0, &m_uiThreadID );
	CloseHandle( hThread );


	Release();
}

void JXSocketMgr::StopThread()
{
	Wait( INFINITE );

	if( m_bThreadRun == FALSE ) {
		Release();
		return;
	}

	m_bThreadRun = FALSE;

	SetEvent( m_hStopEvent );

	Release();

	WaitForSingleObject( m_hExitEvent, INFINITE );	

}

void JXSocketMgr::AddSocket( SOCKET socket, SOCKADDR_IN sockaddr, JXPacket *pPacket )
{
	Wait( INFINITE );

	JXSocket *pSocket;
	
	pSocket = (JXSocket *)PushBack();
		
	pSocket->SetSocket( socket, sockaddr );
	
	printf("AddSocket:%d\n", socket );

	FD_SET( socket, &m_fdset );

	pSocket->SendPacket( pPacket );

	Release();
}

void JXSocketMgr::AddSocket( SOCKET socket, SOCKADDR_IN sockaddr )
{
	Wait( INFINITE );

	((JXSocket *)PushBack())->SetSocket( socket, sockaddr );

	FD_SET( socket, &m_fdset );

	Release();
}

void JXSocketMgr::DelSocket( JXSocket *pSocket )
{
	Wait( INFINITE );

	FD_CLR( pSocket->m_socket, &m_fdset );
	
	Remove( (JXObject *)pSocket );
	
	Release();
}

void JXSocketMgr::Send()
{

	Wait( INFINITE );

	__send();

	Release();

}

void JXSocketMgr::Recv()
{

	Wait( INFINITE );

	__recv();
	
	Release();

}

void JXSocketMgr::__recv()
{

	local_readfds = m_fdset;
	local_errfds = m_fdset;

	select( 0, &local_readfds, NULL, &local_errfds, &m_timeout );

	UI32	i;
	JXSocket *pSocket;

	BEGIN_JXLIST_LOOP( i, this, JXSocket, pSocket );
		 
		if( FD_ISSET( pSocket->m_socket, &local_readfds ) ) {
		
			pSocket->Recv();

			if( pSocket->m_bConnect == FALSE ) FD_CLR( pSocket->m_socket, &m_fdset );

		} else 

		if( FD_ISSET( pSocket->m_socket, &local_errfds ) ) {

			FD_CLR( pSocket->m_socket, &m_fdset );

			pSocket->Disconnect();
		} 

	END_JXLIST_LOOP
}

void JXSocketMgr::__send()
{

	local_writefds = m_fdset;
	local_errfds = m_fdset;

	select( 0, NULL, &local_writefds, &local_errfds, &m_timeout );

	UI32	i;
	JXSocket *pSocket;

	BEGIN_JXLIST_LOOP( i, this, JXSocket, pSocket );


		if( FD_ISSET( pSocket->m_socket, &local_writefds ) ) {

			 pSocket->Send();

			 if( pSocket->m_bConnect == FALSE ) FD_CLR( pSocket->m_socket, &m_fdset );

		} else

		if( FD_ISSET( pSocket->m_socket, &local_errfds ) ) {
		
			FD_CLR( pSocket->m_socket, &m_fdset );

			pSocket->Disconnect();

		}

	END_JXLIST_LOOP

}

void JXSocketMgr::SendPacket( JXPacket *pPacket )
{
	Wait( INFINITE );

	UI32	i;
	JXSocket *pSocket;

	BEGIN_JXLIST_LOOP( i, this, JXSocket, pSocket )
	
		pSocket->SendPacket( pPacket );

	END_JXLIST_LOOP

	Release();
}
