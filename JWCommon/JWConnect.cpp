#include "JWConnect.h"
#include "JWSocket.h"

#include "JWRecvBuffer.h"
#include "JWSendBuffer.h"

unsigned __stdcall connect_send( LPVOID param )
{

	printf("begin send thread\n");

	JWConnect		*pClass = (JWConnect *)param;
	
	DWORD			dwRet;
	int				err;

	BOOL			bSendable;

	WSABUF			wsabuf;

	while( 1 ) {

		bSendable = FALSE;

		dwRet = WaitForSingleObject( pClass->m_hEventSend, pClass->m_uiSendInterval );

		if( dwRet == WAIT_TIMEOUT ) {

			//if( pClass->m_uiState != JWCONNECT_STATE_ESTABLISHED ) continue;

			bSendable = TRUE;

		} else if( dwRet == WAIT_OBJECT_0 ) {

			ResetEvent( pClass->m_hEventSend );

			if( pClass->m_uiActionCodeSend == JWCONNECT_ACT_SENDIM ) {

				bSendable = TRUE;

			} else if( pClass->m_uiActionCodeSend == JWCONNECT_ACT_EXIT ) break;
			
		}

		if( bSendable ) {

			if( pClass->GetSendBuffer()->IsReadyToSend() ) {

				if( pClass->GetSendBuffer()->GetSendParam( &wsabuf.buf, (SI32 *)&wsabuf.len ) == FALSE ) continue;
/*
				int a = send( pClass->m_socket, wsabuf.buf, wsabuf.len, 0 );

				printf( "send:%d, %d\n", wsabuf.len, a );

				pClass->GetSendBuffer()->Completion( a );
				
*/
				ZeroMemory( &pClass->m_sendIoData, sizeof( PER_IO_DATA ) );
				pClass->m_sendIoData.overlapped.hEvent = pClass->m_hEvents[ 2 ];

				err = WSASend( pClass->m_socket, &wsabuf, 1, 
					&pClass->m_sendIoData.dwIoSize,				
					pClass->m_sendIoData.dwFlags,				
					&pClass->m_sendIoData.overlapped, NULL );	

				if( err == SOCKET_ERROR ) {

					err = WSAGetLastError();
					printf( "%d, send err %d\n", pClass->m_socket, err );
					
				}
			

			}

		}

	}

	
	return 0;
}

unsigned __stdcall connect_run( LPVOID param )
{

	printf("begin run thread\n");

	JWConnect		*pClass = (JWConnect *)param;

	DWORD			dwIoSize = 0;
	DWORD			dwFlags	= 0;

	BOOL			bRun = TRUE;

	while( bRun ) {

		switch( pClass->m_uiState ) {

		case JWCONNECT_STATE_ATTEMPTING:

			bRun = pClass->process_state_attempting();

			break;

		case JWCONNECT_STATE_ESTABLISHED:
			
			bRun = pClass->process_state_established();

			break;

		case JWCONNECT_STATE_CLOSED:

			bRun = pClass->process_state_closed();

			break;
		}
	}

	return 0;
}


BOOL JWConnect::process_state_attempting()
{

	printf( "attempting..\n");

	DWORD	dwRet;
	int		err;

	dwRet = WaitForSingleObject( m_hEvents[ 0 ], m_uiAttemptInterval );

	if( dwRet == WAIT_OBJECT_0 ) {
		// action event

		// reset event
		ResetEvent( m_hEvents[ 0 ] );
		
		if( m_uiActionCode == JWCONNECT_ACT_STOP ||
			m_uiActionCode == JWCONNECT_ACT_CLOSE ) {
			// 접속 시도 종료

			CloseSocket();

			m_uiState = JWCONNECT_STATE_CLOSED;

			return TRUE;

		}

		if( m_uiActionCode == JWCONNECT_ACT_EXIT ) {
			// 쓰레드 종료

			CloseSocket();

			m_uiState = JWCONNECT_STATE_CLOSED;

			return FALSE;

		}

	} else if( dwRet == WAIT_TIMEOUT ) {
		// 접속 시도

		printf( "attempting to connect\n" );

		err = connect( m_socket, (SOCKADDR *)&m_sockaddr, sizeof( m_sockaddr ) );

		if( err == SOCKET_ERROR ) return TRUE;

		// 접속 성공

		printf( "socket:%d\n", m_socket );

		printf( "connect established\n" );

		ResetEvent( m_hEvents[ 1 ] );
		ResetEvent( m_hEvents[ 2 ] );

		GetRecvBuffer()->ClearBuffer();
		GetSendBuffer()->ClearBuffer();

		WSABUF	wsabuf;

		// overlapped 초기화
		ZeroMemory( &m_recvIoData, sizeof( PER_IO_DATA ) );
		m_recvIoData.overlapped.hEvent = m_hEvents[ 1 ];

		GetRecvBuffer()->GetRecvParam( &wsabuf.buf, (SI32 *)&wsabuf.len );

		err = WSARecv( m_socket, &wsabuf, 1, &m_recvIoData.dwIoSize, 
			&m_recvIoData.dwFlags, &m_recvIoData.overlapped, NULL );

		if( err == SOCKET_ERROR && ( WSAGetLastError() != ERROR_IO_PENDING ) ) {

			OutputDebugString( "WSARecvError!!\n" );

			CloseSocket();

			m_uiState = JWCONNECT_STATE_CLOSED;

			return TRUE;
		}

		m_uiState = JWCONNECT_STATE_ESTABLISHED;
	}

	return TRUE;
	
}

BOOL JWConnect::process_state_established()
{
	DWORD	dwIoSize = 0;
	DWORD	dwFlags	= 0;
	
	DWORD	dwRet;
	int		err;

	printf( "established\n" );

	dwRet = WaitForMultipleObjects( 3, m_hEvents, FALSE, INFINITE );

	/////////////////////////
	// action event
	dwRet = WaitForSingleObject( m_hEvents[ 0 ], 0 );

	if( dwRet == WAIT_OBJECT_0 ) {

		printf( "action event\n" );

		// reset event
		ResetEvent( m_hEvents[ 0 ] );

		if( m_uiActionCode == JWCONNECT_ACT_CLOSE ) {

			CloseSocket();

			m_uiState = JWCONNECT_STATE_CLOSED;

			return TRUE;

		} else if( m_uiActionCode == JWCONNECT_ACT_EXIT ) { 

			m_uiState = JWCONNECT_STATE_CLOSED;

			return FALSE;
		}

	}
	
	/////////////////////////
	// recv completion event
	dwRet = WaitForSingleObject( m_hEvents[ 1 ], 0 );

	if( dwRet == WAIT_OBJECT_0 ) {

		printf( "recv event\n" );

		// reset event
		ResetEvent( m_hEvents[ 1 ] );

		WSAGetOverlappedResult( m_socket, (LPWSAOVERLAPPED)&m_recvIoData, 
			&dwIoSize, FALSE, &dwFlags );

		if( dwIoSize == 0 ) {

			CloseSocket();

			m_uiState = JWCONNECT_STATE_CLOSED;

			return TRUE;
		}


		if( GetRecvBuffer()->Completion( dwIoSize ) == FALSE ) {

			CloseSocket();

			m_uiState = JWCONNECT_STATE_CLOSED;

			return TRUE;
		}

		// 다음 recv

		WSABUF	wsabuf;

		// overlapped 초기화
		ZeroMemory( &m_recvIoData, sizeof( PER_IO_DATA ) );
		m_recvIoData.overlapped.hEvent = m_hEvents[ 1 ];

		GetRecvBuffer()->GetRecvParam( &wsabuf.buf, (SI32 *)&wsabuf.len );

		err = WSARecv( m_socket, &wsabuf, 1, &m_recvIoData.dwIoSize, 
			&m_recvIoData.dwFlags, &m_recvIoData.overlapped, NULL );

		if( err == SOCKET_ERROR && ( WSAGetLastError() != ERROR_IO_PENDING ) ) {

			OutputDebugString( "WSARecvError!!\n" );

			CloseSocket();

			m_uiState = JWCONNECT_STATE_CLOSED;

			return TRUE;
		}



	}

	/////////////////////////
	// send completion event
	dwRet = WaitForSingleObject( m_hEvents[ 2 ], 0 );
	
	if( dwRet == WAIT_OBJECT_0 ) {

		printf( "send event\n" );

		// reset event
		ResetEvent( m_hEvents[ 2 ] );

		printf( "send completion\n" );

		WSAGetOverlappedResult( m_socket, (LPWSAOVERLAPPED)&m_sendIoData, 
			&dwIoSize, FALSE, &dwFlags );

		if( dwIoSize == 0 ) {

			CloseSocket();

			m_uiState = JWCONNECT_STATE_CLOSED;

		}

		GetSendBuffer()->Completion( dwIoSize );

	}

	return TRUE;
}


BOOL JWConnect::process_state_closed()
{
	DWORD	dwRet;

	dwRet = WaitForSingleObject( m_hEvents[ 0 ], INFINITE );

	if( dwRet == WAIT_OBJECT_0 ) {

		ResetEvent( m_hEvents[ 0 ] );

		if( m_uiActionCode == JWCONNECT_ACT_ATTEMPT ) {

			m_socket = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );

			if( m_socket == INVALID_SOCKET ) return TRUE;

			m_uiState = JWCONNECT_STATE_ATTEMPTING;

		} else if( m_uiActionCode == JWCONNECT_ACT_EXIT ) {

			return FALSE;
		}

	}

	return TRUE;
}

JWConnect::JWConnect()
{

	m_hEvents[ 0 ] = CreateEvent( NULL, TRUE, FALSE, NULL );
	m_hEvents[ 1 ] = CreateEvent( NULL, TRUE, FALSE, NULL );
	m_hEvents[ 2 ] = CreateEvent( NULL, TRUE, FALSE, NULL );

	m_hEventSend = CreateEvent( NULL, TRUE, FALSE, NULL );

	m_uiActionCode = 0;
	m_uiActionCodeSend = 0;

	m_bInitOk = FALSE;

	m_uiState = JWCONNECT_STATE_CLOSED;

	m_szIP[ 0 ] = NULL;

	m_uiAttemptInterval = 500;
	m_uiSendInterval = 10;

}



JWConnect::~JWConnect()
{

	CloseHandle( m_hEventSend );

	CloseHandle( m_hEvents[ 0 ] );
	CloseHandle( m_hEvents[ 1 ] );
	CloseHandle( m_hEvents[ 2 ] );
}


void JWConnect::InitConnect( char *pszIP, UI16 usPort, SI32 siRecvBufSize, SI32 siSendBufSize )
{
	if( m_bInitOk ) return;

	memcpy( m_szIP, pszIP, 16 );
	m_szIP[ 15 ] = NULL;

	m_usPort = usPort;

	CreateSocket( siRecvBufSize, siSendBufSize );

	memset( &m_sockaddr, 0, sizeof( m_sockaddr ) );

	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_addr.s_addr = inet_addr( m_szIP );
	m_sockaddr.sin_port = htons( m_usPort );

	UI32 threadID;

	HANDLE hThread = (HANDLE) _beginthreadex( NULL, 0, connect_run, (LPVOID)this, 0, &threadID );
	CloseHandle( hThread );	

	hThread = (HANDLE) _beginthreadex( NULL, 0, connect_send, (LPVOID)this, 0, &threadID );
	CloseHandle( hThread );	

	m_bInitOk = TRUE;

}

void JWConnect::Connect()
{
	if( m_bInitOk == FALSE ) return;

	m_uiActionCode = JWCONNECT_ACT_ATTEMPT;

	SetEvent( m_hEvents[ 0 ] );

	printf( "called connect\n" );

}

void JWConnect::Stop()
{
	if( m_bInitOk == FALSE ) return;

	m_uiActionCode = JWCONNECT_ACT_STOP;

	SetEvent( m_hEvents[ 0 ] );

}

void JWConnect::Close()
{
	if( m_bInitOk == FALSE ) return;

	m_uiActionCode = JWCONNECT_ACT_CLOSE;

	SetEvent( m_hEvents[ 0 ] );
}
