
#include "JWServer.h"

#include "JWSocketMgr.h"
#include "JWSocket.h"

#include "JWFQueueMISO.h"
#include <stdexcept>
#include <assert.h>


JWServer::JWServer()
{
	m_pAcceptedSocketList = NULL;
	m_pConnectedSocketList = NULL; 
	m_pSocket = NULL;
	m_pDeleteList = NULL;
}

JWServer::~JWServer()
{

	if( m_pAcceptedSocketList ) delete m_pAcceptedSocketList;
	if( m_pConnectedSocketList ) delete m_pConnectedSocketList;
	if( m_pSocket ) delete [] m_pSocket;

	if( m_pDeleteList ) delete m_pDeleteList;

}

void JWServer::InitServer( SI32 maxnum, SI32 siRecvBufSize, SI32 siSendBufSize )
{

	if( m_pAcceptedSocketList ) delete m_pAcceptedSocketList;

	try{	
		m_pAcceptedSocketList = new JWSocketMgr;
	}
	catch(std::bad_alloc){
		assert(0);
	}

	if( m_pConnectedSocketList ) delete m_pConnectedSocketList;

	try{
		m_pConnectedSocketList = new JWSocketMgr;
	}
	catch(std::bad_alloc){
		assert(0);
	}

	if( m_pSocket ) delete [] m_pSocket;

	try{	
		m_pSocket = new JWSocket[ maxnum ];
	}
	catch(std::bad_alloc){
		assert(0);
	}

	for( int i = 0; i < maxnum; ++i ) {

		// 소켓 초기화
		m_pSocket[ i ].CreateSocket( siRecvBufSize, siSendBufSize );

		m_pAcceptedSocketList->NewObject( (JWObject *)&m_pSocket[ i ] );

	}

	if( m_pDeleteList ) delete m_pDeleteList;

	try{	
		m_pDeleteList = new JWFQueueMISO;
	}
	catch(std::bad_alloc){
		assert(0);
	}

	m_pDeleteList->CreateQueue( maxnum );

}

BOOL JWServer::Accept( SOCKET socket, SOCKADDR_IN sockaddr )
{

	BOOL bRet = FALSE;

	m_pAcceptedSocketList->Wait();

	JWSocket *pSocket;

	pSocket = (JWSocket *)m_pAcceptedSocketList->PushBack();

	if( pSocket != NULL ) {
		
		bRet = pSocket->SetSocket( socket, sockaddr );
	} 

	m_pAcceptedSocketList->Release();

	return bRet;

}

void JWServer::UpdateServer( JWList *pList )
{
	
	UI32 i;
	SI32 j, n;
	BOOL b;

	m_pConnectedSocketList->Wait();
	m_pAcceptedSocketList->Wait();

		JWSocket *pSocket;

		// 삭제

		n = m_pDeleteList->GetNumberOfItems();

		for( j = 0; j < n; ++j ) {

			b = m_pDeleteList->Dequeue( (DWORD *)&pSocket );

			if( b == FALSE || pSocket == NULL || pSocket->GetSocket() == INVALID_SOCKET ) continue;
			
			DelClient( pSocket, pList );

			m_pConnectedSocketList->Remove( (JWObject *)pSocket );

			pSocket->CloseSocket();

		}


		// 추가
		
		JWLIST_LOOP( i, m_pAcceptedSocketList, JWSocket, pSocket )
		{
			AddClient( pSocket, pList );
		}
		
		m_pConnectedSocketList->PushFrontAll( m_pAcceptedSocketList );


	m_pAcceptedSocketList->Release();
	m_pConnectedSocketList->Release();

}


void JWServer::CompleteLogOut( JWSocket *pSocket )
{
	m_pDeleteList->Enqueue( (DWORD)pSocket );
}

void JWServer::RequestLogOut( JWSocket *pSocket )
{
	pSocket->GetFriend()->SetAction();
}


