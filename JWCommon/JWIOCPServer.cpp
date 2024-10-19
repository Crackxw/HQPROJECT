#include "JWIOCPServer.h"

#include "JWSocket.h"
#include "JWSocketMgr.h"
#include "JWRecvBuffer.h"
#include "JWSendBuffer.h"

#include "JWList.h"

unsigned __stdcall iocpserver_send( LPVOID param )
{

	JWIOCPServer	*pClass = (JWIOCPServer *)param;
	
	WSABUF	wsabuf;

	UI16 i;
	JWSocket *pSocket;

	int ret;

	while( 1 ) {

		Sleep( 10 );

		pClass->m_pConnectedSocketList->Wait();

		JWLIST_LOOP( i, pClass->m_pConnectedSocketList, JWSocket, pSocket )
		{
			if( pSocket->GetSendBuffer()->IsReadyToSend() ) {

				if( pSocket->GetSendBuffer()->GetSendParam( &wsabuf.buf, (SI32 *)&wsabuf.len ) == FALSE ) continue;

				ZeroMemory( &pSocket->m_sendIoData, sizeof( PER_IO_DATA ) );

				pSocket->m_sendIoData.dwOperationType = SEND_POSTED;

				ret = WSASend( pSocket->GetSocket(), &wsabuf, 1, 
					&pSocket->m_sendIoData.dwIoSize, 
					pSocket->m_sendIoData.dwFlags,
					&pSocket->m_sendIoData.overlapped, NULL );

				if( ret == SOCKET_ERROR && ( WSAGetLastError() != ERROR_IO_PENDING ) ) {
					
					pClass->RequestLogOut( pSocket );
				}					

			}

		}

		pClass->m_pConnectedSocketList->Release();
	}

	return 0;

}

unsigned __stdcall iocpserver_run( LPVOID param )
{

	JWIOCPServer	*pClass = (JWIOCPServer *)param;

	BOOL			bSuccess = FALSE;
	DWORD			dwError;
	DWORD			dwIoSize = 0;

	JWSocket		*pSocket = NULL;
	PER_IO_DATA		*pPerIoData = NULL;
	OVERLAPPED		*pOverlapped = NULL;
	
	WSABUF			wsabuf;

	int ret;

	while( 1 ) {

		bSuccess = GetQueuedCompletionStatus( pClass->m_hIOCP, &dwIoSize, (LPDWORD)&pSocket, &pOverlapped, INFINITE );
		
		if( !bSuccess ) {
			dwError = GetLastError();

			if( dwError == 64 && pSocket != NULL ) {
				// 소켓이 이미 닫힌 경우
				
				pClass->RequestLogOut( pSocket );

			}
			continue;
		}

		// 스레드의 강제 종료 명령이 내려진 경우
		if( pSocket == NULL ) break;

		// 클라이언트와의 소켓 연결이 끊어졌으면,
		if( dwIoSize == 0 ) {
			
			pClass->RequestLogOut( pSocket );
			
			continue;
		}

		

		pPerIoData = (PER_IO_DATA *)pOverlapped;

		switch( pPerIoData->dwOperationType ) {
		case RECV_POSTED:
			
			if( pSocket->m_pRecvBuffer->Completion( dwIoSize ) == FALSE ) {

				OutputDebugString( "RecvBuffer Error!!!\n" );

				pClass->RequestLogOut( pSocket );

				continue;
			}

			// 다음 Recv

			pSocket->GetRecvBuffer()->GetRecvParam( &wsabuf.buf, (SI32 *)&wsabuf.len );

			ZeroMemory( &pSocket->m_recvIoData, sizeof( PER_IO_DATA ) );

			pSocket->m_recvIoData.dwOperationType = RECV_POSTED;

			ret = WSARecv( pSocket->GetSocket(), &wsabuf, 1, 
				&pSocket->m_recvIoData.dwIoSize, 
				&pSocket->m_recvIoData.dwFlags,
				&pSocket->m_recvIoData.overlapped, NULL );

			if( ret == SOCKET_ERROR && ( WSAGetLastError() != ERROR_IO_PENDING ) ) {

				OutputDebugString( "WSARecvError!!\n" );

				pClass->RequestLogOut( pSocket );
			}			

			break;

		case SEND_POSTED:

			pSocket->m_pSendBuffer->Completion( dwIoSize );

			break;

		} //switch


	} //while

	return 0;
}


JWIOCPServer::JWIOCPServer()
{
	m_hIOCP = NULL;
}

JWIOCPServer::~JWIOCPServer()
{
	if( m_hIOCP ) CloseHandle( m_hIOCP );
}

void JWIOCPServer::InitServer( SI32 maxnum, SI32 siRecvBufSize, SI32 siSendBufSize )
{

	JWServer::InitServer( maxnum, siRecvBufSize, siSendBufSize );

	m_hIOCP = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );

	UI32 threadID;

	HANDLE hThread = (HANDLE) _beginthreadex( NULL, 0, iocpserver_run, (LPVOID)this, 0, &threadID );
	CloseHandle( hThread );	

	hThread = (HANDLE) _beginthreadex( NULL, 0, iocpserver_send, (LPVOID)this, 0, &threadID );
	CloseHandle( hThread );	

}

void JWIOCPServer::AddClient( JWSocket *pSocket, JWList *pList )
{

	JWObject *pObject;

	pObject = pList->PushBack();

	pSocket->SetFriend( pObject );
	pObject->SetFriend( pSocket );

	pObject->Init();


	CreateIoCompletionPort( (HANDLE) pSocket->GetSocket(), m_hIOCP, (DWORD)pSocket, 0 );

	pSocket->GetRecvBuffer()->GetRecvParam( &m_wsabuf.buf, (SI32 *)&m_wsabuf.len );

	ZeroMemory( &pSocket->m_recvIoData, sizeof( PER_IO_DATA ) );

	pSocket->m_recvIoData.dwOperationType = RECV_POSTED;

	int ret = WSARecv( pSocket->GetSocket(), &m_wsabuf, 1, 
		&pSocket->m_recvIoData.dwIoSize, 
		&pSocket->m_recvIoData.dwFlags, 
		&pSocket->m_recvIoData.overlapped, NULL ); 

	if( ret == SOCKET_ERROR && ( WSAGetLastError() != ERROR_IO_PENDING ) ) {

		OutputDebugString( "WSARecvError!!\n" );

		RequestLogOut( pSocket );
	
	} 	
}

void JWIOCPServer::DelClient( JWSocket *pSocket, JWList *pList )
{

	if( pSocket->GetFriend() == NULL ) return;

	// 소켓에 연결된 클라이언트 객체의 종료 처리를 한다
	pSocket->GetFriend()->Close();

	pList->Remove( pSocket->GetFriend() );

}

