//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완		
// 최종 수정일 : 2002. 7. 2	
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#include "jxdef.h"
#include "JXAcceptSocketList.h"
#include "JXGateSocket.h"
#include "JXSocketMgr.h"
#include "JXPacket.h"

unsigned __stdcall JXGateSocket_ThreadFunc( LPVOID param )
{

	JXGateSocket *pGateSocket = (JXGateSocket *)param;
	DWORD dwRet;

	while( 1 ) {
		
		dwRet = WaitForSingleObject( pGateSocket->m_hStopEvent, pGateSocket->m_dwAcceptInterval );

		if( dwRet == WAIT_OBJECT_0 ) break;

		pGateSocket->Accept();

	}

	printf("Gate Socket.. End Thread... \n");

	SetEvent( pGateSocket->m_hExitEvent );

	return 0;
}

JXGateSocket::JXGateSocket()
{
	m_socket = INVALID_SOCKET;

	m_siAddrlen = sizeof( SOCKADDR_IN );

	m_bThreadRun = FALSE;
	m_uiThreadID = 0;

	m_hStopEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
	m_hExitEvent = CreateEvent( NULL, FALSE, FALSE, NULL );

	m_pAcceptSocketList = NULL;

	m_dwAcceptInterval = 100;

	m_siSocketSendBuf = 240 * 1024;
	m_siSocketRecvBuf = 240 * 1024;

	m_bOpenGate = FALSE;

	m_bAcceptable = TRUE;
}

JXGateSocket::~JXGateSocket()
{

	StopThread();

	CloseHandle( m_hStopEvent );
	CloseHandle( m_hExitEvent );

	printf("GateSocket Destroy..\n");
}
 
BOOL JXGateSocket::Create( UI16 usPort, JXAcceptSocketList *pAcceptSocketList, 
						   SI32 siSocketRecvBuf, SI32 siSocketSendBuf, DWORD dwAcceptInterval )
{

	if( dwAcceptInterval != 0 ) m_dwAcceptInterval = dwAcceptInterval;
	
	if( siSocketRecvBuf != 0 ) m_siSocketRecvBuf = siSocketRecvBuf;
	if( siSocketSendBuf != 0 ) m_siSocketSendBuf = siSocketSendBuf;

	m_pAcceptSocketList = pAcceptSocketList;

	memset( &m_sockaddr, 0, sizeof( m_sockaddr ) );

	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_addr.s_addr = htonl( INADDR_ANY );
	m_sockaddr.sin_port = htons( usPort );

	m_socket = socket( AF_INET, SOCK_STREAM, 0 );

	if( m_socket == INVALID_SOCKET ) {
		return FALSE;
	}
/*
	unsigned long ul = 1;
	int nRet = ioctlsocket( m_socket, FIONBIO, &ul );

	if( nRet == SOCKET_ERROR ) {
		return FALSE;
	}
*/
	setsockopt( m_socket, SOL_SOCKET, SO_RCVBUF, (const char *)&m_siSocketRecvBuf, sizeof( siSocketRecvBuf ) );
	setsockopt( m_socket, SOL_SOCKET, SO_SNDBUF, (const char *)&m_siSocketSendBuf, sizeof( siSocketRecvBuf ) );

	int err = bind( m_socket, (SOCKADDR *)&m_sockaddr, sizeof( m_sockaddr ) );

	if( err == SOCKET_ERROR ) {
		return FALSE; 
	}

	listen( m_socket, SOMAXCONN );
	
	FD_ZERO( &m_fdset );
	FD_SET( m_socket, &m_fdset );

	m_timeout.tv_sec = 0;
	m_timeout.tv_usec = 0;

	return TRUE;
}

void JXGateSocket::StartThread()
{
	if( m_bThreadRun == TRUE ) StopThread();

	m_bThreadRun = TRUE;

	HANDLE hThread = (HANDLE) _beginthreadex( NULL, 0, JXGateSocket_ThreadFunc, (LPVOID)this, 0, &m_uiThreadID );
	CloseHandle( hThread );

}

void JXGateSocket::StopThread()
{
	if( m_bThreadRun == FALSE ) return;

	m_bThreadRun = FALSE;

	SetEvent( m_hStopEvent );

	WaitForSingleObject( m_hExitEvent, INFINITE );
}

void JXGateSocket::Accept()
{

	local_fdset = m_fdset;

	select( 0, &local_fdset, NULL, NULL, &m_timeout );

	if( FD_ISSET( m_socket, &local_fdset ) ) {

		local_socket = accept( m_socket, (SOCKADDR *)&local_sockaddr, &m_siAddrlen );
	
		if( local_socket == INVALID_SOCKET ) return;

		if( m_bAcceptable == FALSE ) {
			closesocket( local_socket );
			return;
		}

		m_pAcceptSocketList->AddSocket( local_socket, local_sockaddr );

		printf( "New Connection..\n" );
	}

}


