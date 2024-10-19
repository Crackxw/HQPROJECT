//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 5	
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#include "JXAcceptSocketList.h"
#include "JXSocket.h"

JXAcceptSocketList::JXAcceptSocketList()
{
	m_pSocket = NULL;
}

JXAcceptSocketList::~JXAcceptSocketList()
{

	Wait( INFINITE );

	if( m_pSocket ) delete [] m_pSocket;

	Release();

	printf("AcceptSocketList Destroy..\n");
}

void JXAcceptSocketList::Create( SI32 siMaxNumber, SI32 siRecvBufSize, SI32 siSendBufSize, SI32 siPacketQueueSize )
{

	Wait( INFINITE );

	if( m_pSocket ) delete [] m_pSocket;

	m_pSocket = new JXSocket[ siMaxNumber ];

	for( SI32 i = 0; i < siMaxNumber; ++i ) {
		m_pSocket[ i ].Create( siRecvBufSize, siSendBufSize, siPacketQueueSize );
		AddToFree( &m_pSocket[ i ] );
	}

	Release();
}

void JXAcceptSocketList::AddSocket( SOCKET socket, SOCKADDR_IN sockaddr, JXPacket *pPacket )
{

	Wait( INFINITE );
	
	local_pSocket = (JXSocket *)PushBack();

	if( local_pSocket == NULL ) {

		closesocket( socket );

		Release();

		return;
	}
		
	local_pSocket->SetSocket( socket, sockaddr );
	
	local_pSocket->SendPacket( pPacket );
	
	Release();

}

void JXAcceptSocketList::AddSocket( SOCKET socket, SOCKADDR_IN sockaddr )
{

	Wait( INFINITE );

	local_pSocket = (JXSocket *)PushBack();

	printf("AcceptSocketList::AddSocket	 [%x]\n", local_pSocket );

	if( local_pSocket == NULL ) {

		closesocket( socket );

		Release();

		return;	
	}

	local_pSocket->SetSocket( socket, sockaddr );

	Release();
}

