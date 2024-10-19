
#include "JWListen.h"
#include "JWServer.h"

unsigned __stdcall listen_run( LPVOID param )
{

	JWListen		*pClass = (JWListen *)param;
	DWORD			dwRet;

	SI32			err;

	SOCKET			listenSocket;
	SOCKET			newSocket;
	
	SOCKADDR_IN		sockaddr;
	SI32			siAddrlen;

	siAddrlen = sizeof( SOCKADDR );

	WSANETWORKEVENTS NetworkEvents;	

	HANDLE hEvents[ 2 ];

	hEvents[ 0 ] = pClass->m_hActionEvent;
	hEvents[ 1 ] = WSACreateEvent();
	

	while( 1 ) {

		dwRet = WSAWaitForMultipleEvents( 2, hEvents, FALSE, WSA_INFINITE, FALSE );

		dwRet = dwRet - WSA_WAIT_EVENT_0;


		if( dwRet == 0 ) {
			// Action Event 처리
			
			if( pClass->m_uiActionCode == JWLISTEN_ACT_START ) {
				
				if( pClass->m_uiState == JWLISTEN_STAT_RUN ) continue;

				if( pClass->m_uiState == JWLISTEN_STAT_CLOSE ) {
					// 소켓이 닫힌 상태라면,

					listenSocket = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );					

					if( listenSocket == INVALID_SOCKET ) continue;

					err = bind( listenSocket, (SOCKADDR *)&pClass->m_sockaddr, sizeof( pClass->m_sockaddr ) );

					if( err == SOCKET_ERROR ) {

						closesocket( listenSocket );

						continue;
					}

					WSAEventSelect( listenSocket, hEvents[ 1 ], FD_ACCEPT | FD_CLOSE );

					err = listen( listenSocket, 200 );

					if( err == SOCKET_ERROR ) {

						closesocket( listenSocket );

						continue;
					}

					pClass->m_uiState = JWLISTEN_STAT_RUN;	

				} else if( pClass->m_uiState == JWLISTEN_STAT_STOP ) {

					pClass->m_uiState = JWLISTEN_STAT_RUN;	
				}


			} else if( pClass->m_uiActionCode == JWLISTEN_ACT_STOP ) {

					pClass->m_uiState = JWLISTEN_STAT_STOP;

			} else if( pClass->m_uiActionCode == JWLISTEN_ACT_CLOSE ) {

					closesocket( listenSocket );

					listenSocket = INVALID_SOCKET;

					pClass->m_uiState = JWLISTEN_STAT_CLOSE;
			}

		} else if( dwRet == 1 ) {

			WSAEnumNetworkEvents( listenSocket, hEvents[ 1 ], &NetworkEvents );

			if( NetworkEvents.lNetworkEvents & FD_ACCEPT ) {

				if( NetworkEvents.iErrorCode[ FD_ACCEPT_BIT ] != 0 ) {

					continue;
				}

				newSocket = WSAAccept( listenSocket, (SOCKADDR *)&sockaddr, &siAddrlen, NULL, 0 );

				if( newSocket == INVALID_SOCKET ) {
					continue;
				}

				if( pClass->m_uiState != JWLISTEN_STAT_RUN ) {

					closesocket( newSocket );

					continue;
				}

				// 연결을 받아들인다
				if( pClass->m_pServer->Accept( newSocket, sockaddr ) == FALSE ) {

					closesocket( newSocket );

					continue;
				}

			}

		}

	} // while( 1 )
	
}



JWListen::JWListen()
{

	m_hActionEvent = CreateEvent( NULL, FALSE, FALSE, NULL );

	m_bInitOk = FALSE;

	m_uiState = 0;
	m_uiActionCode = 0;

}

JWListen::~JWListen()
{
	
	CloseHandle( m_hActionEvent );
}


void JWListen::Init( JWServer *pServer, UI16 usPort ) 
{

	if( m_bInitOk ) return;

	m_pServer = pServer;

	UI32 threadID;

	HANDLE hThread = (HANDLE) _beginthreadex( NULL, 0, listen_run, (LPVOID)this, 0, &threadID );
	CloseHandle( hThread );

	SetListenPort( usPort );

	m_uiState = JWLISTEN_STAT_CLOSE;

	m_bInitOk = TRUE;
}

void JWListen::SetListenPort( UI16 usPort )
{

	memset( &m_sockaddr, 0, sizeof( m_sockaddr ) );

	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_addr.s_addr = htonl( INADDR_ANY );
	m_sockaddr.sin_port = htons( usPort );

}

void JWListen::Start()
{
	if( m_bInitOk == FALSE ) return;

	m_uiActionCode = JWLISTEN_ACT_START;

	SetEvent( m_hActionEvent );

}

void JWListen::Stop()
{
	if( m_bInitOk == FALSE ) return;

	m_uiActionCode = JWLISTEN_ACT_STOP;

	SetEvent( m_hActionEvent );

}

void JWListen::Close()
{
	if( m_bInitOk == FALSE ) return;

	m_uiActionCode = JWLISTEN_ACT_CLOSE;

	SetEvent( m_hActionEvent );

}

