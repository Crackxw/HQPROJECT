//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완		
// 최종 수정일 : 2002. 7. 2	
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#include "stdio.h"

#include "JXConnectSocket.h"

unsigned __stdcall JXConnectSocket_ThreadFunc( LPVOID param )
{
	JXConnectSocket *pConnectSocket = (JXConnectSocket *)param;

	HANDLE	hEvents[ 2 ];

	hEvents[ 0 ] = pConnectSocket->m_hTryConnectEvent;
	hEvents[ 1 ] = pConnectSocket->m_hExitEvent;

	DWORD dwRet;

	while( 1 ) {

		dwRet = WaitForMultipleObjects( 2, hEvents, FALSE, INFINITE );

		if( dwRet == WAIT_OBJECT_0 ) { 

			if( pConnectSocket->m_bConnect == TRUE ) continue;
			
			if( pConnectSocket->Connect() == TRUE ) {
				pConnectSocket->m_bConnect = TRUE;
			}

		} else if( dwRet == WAIT_OBJECT_0 + 1 ) { 
			break; 
		}  
	} 

	printf("Connect Socket.. End Thread... \n");

	return 0; 
}

unsigned __stdcall JXConnectSocket_Recv_ThreadFunc( LPVOID param )
{

	printf("Start JXConnectSocket_Recv_ThreadFunc.. \n");

	JXConnectSocket *pSocket = (JXConnectSocket *)param;

	while( pSocket->m_bThreadRun ) {

		pSocket->NBRecv();

		Sleep( 10 );
	}

	SetEvent( pSocket->m_hEvent );

	printf("Connect Socket Recv.. End Thread... \n");

	return 0;

}

unsigned __stdcall JXConnectSocket_Send_ThreadFunc( LPVOID param )
{

	printf("Start JXConnectSocket_Send_ThreadFunc3.. \n");

	JXConnectSocket *pSocket = (JXConnectSocket *)param;

	while( pSocket->m_bThreadRun ) {

		pSocket->NBSend();

		Sleep( 10 );
	}

	SetEvent( pSocket->m_hEvent );

	printf("Connect Socket Send.. End Thread... \n");

	return 0;

}

JXConnectSocket::JXConnectSocket()
{
	m_pszIP[ 0 ] = NULL;

	m_uiThreadID = 0;

	m_hTryConnectEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
	m_hExitEvent = CreateEvent( NULL, FALSE, FALSE, NULL );

	m_bThreadRun = FALSE;
	m_uiThread2ID = 0;

	m_hEvent = CreateEvent( NULL, FALSE, FALSE, NULL );

}

JXConnectSocket::~JXConnectSocket()
{

	SetEvent( m_hExitEvent );

	StopThread();

	CloseHandle( m_hTryConnectEvent );
	CloseHandle( m_hExitEvent );
	CloseHandle( m_hEvent );
}

void JXConnectSocket::Create( char *pszIP, UI16 usPort, SI32 siRecvBufSize, SI32 siSendBufSize, SI32 siPacketQueueSize, BOOL bConnThreadRun /* = TRUE*/ )
{

	CreateSocket( siRecvBufSize, siSendBufSize, siPacketQueueSize );

	memset( &m_sockaddr, 0, sizeof( m_sockaddr ) );

	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_addr.s_addr = inet_addr( pszIP );
	m_sockaddr.sin_port = htons( usPort );

	strcpy( m_pszIP, pszIP );

	m_usPort = usPort;

	if(bConnThreadRun == TRUE)
	{	
		HANDLE hThread = (HANDLE) _beginthreadex( NULL, 0, JXConnectSocket_ThreadFunc, (LPVOID)this, 0, &m_uiThreadID );
		CloseHandle( hThread );
	}
}

void JXConnectSocket::StartThread()
{
	if( m_bThreadRun == TRUE ) StopThread();

	m_bThreadRun = TRUE;

	HANDLE hThread = (HANDLE) _beginthreadex( NULL, 0, JXConnectSocket_Recv_ThreadFunc, (LPVOID)this, 0, &m_uiThread2ID );
	CloseHandle( hThread );

	hThread = (HANDLE) _beginthreadex( NULL, 0, JXConnectSocket_Send_ThreadFunc, (LPVOID)this, 0, &m_uiThread2ID );
	CloseHandle( hThread );


}

void JXConnectSocket::StopThread()
{
	if( m_bThreadRun == FALSE ) return;

	m_bThreadRun = FALSE;

	WaitForSingleObject( m_hEvent, INFINITE );	
}

void JXConnectSocket::TryConnect()
{
	SetEvent( m_hTryConnectEvent );
}


BOOL JXConnectSocket::Connect()
{
	if( m_pszIP[ 0 ] == NULL ) return FALSE;

	if( m_bConnect == TRUE ) Disconnect();

	m_socket = socket( AF_INET, SOCK_STREAM, 0 );

	if( m_socket == INVALID_SOCKET ) return FALSE;
/*
	unsigned long ul = 1;
	int nRet = ioctlsocket( m_socket, FIONBIO, &ul );

	if( nRet == SOCKET_ERROR ) {
		closesocket( m_socket );
		return FALSE;
	}
*/
	int err = connect( m_socket, (SOCKADDR *)&m_sockaddr, sizeof( m_sockaddr ) );

	if( err == SOCKET_ERROR ) {
		
		
		closesocket( m_socket );
		return FALSE;
	}

	SetSocket( m_socket, m_sockaddr );

	return TRUE;
}


BOOL JXConnectSocket::Connect(const char* pIP, UI16 usPort)
{
	if( m_pszIP[ 0 ] == NULL ) return FALSE;

	if( m_bConnect == TRUE ) Disconnect();

	m_socket = socket( AF_INET, SOCK_STREAM, 0 );

	if( m_socket == INVALID_SOCKET ) return FALSE;

	memset( &m_sockaddr, 0, sizeof( m_sockaddr ) );

	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_addr.s_addr = inet_addr( pIP );
	m_sockaddr.sin_port = htons( usPort );

	int err = connect( m_socket, (SOCKADDR *)&m_sockaddr, sizeof( m_sockaddr ) );

	if( err == SOCKET_ERROR ) {	
		
		closesocket( m_socket );
		return FALSE;
	}

	SetSocket( m_socket, m_sockaddr );

	return TRUE;
}

