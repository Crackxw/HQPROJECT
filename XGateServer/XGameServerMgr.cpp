//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 7
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


#include "XGameServerMgr.h"
#include "XGameServer.h"

#include "..\\XCommon\\JXSocket.h"
#include "..\\XCommon\\JXPacketQueue.h"

XGameServerMgr::XGameServerMgr()
{
	m_pGameServer = NULL;
}

XGameServerMgr::~XGameServerMgr()
{
	if( m_pGameServer ) delete [] m_pGameServer;
}

void XGameServerMgr::Create( UI32 uiMaxGameServer )
{
	if( m_pGameServer ) delete [] m_pGameServer;

	m_pGameServer = new XGameServer[ uiMaxGameServer ];

	for( UI32 i = 0; i < uiMaxGameServer; ++i )
		AddToFree( &m_pGameServer[ i ] );

}

XGameServer* XGameServerMgr::AddGameServer( JXSocket *pSocket )
{
	XGameServer *pGameServer;
	JXPacketQueue *pPacketQueue = NULL;

	pGameServer = (XGameServer *)PushBack();
	
	if( pGameServer == NULL ) return NULL;

	pGameServer->Init();

	pGameServer->SetSocket( pSocket );

	pPacketQueue = pSocket->GetQueue();
	pPacketQueue->Clear();

	return pGameServer;
}

void XGameServerMgr::SendPacket( JXPacket *pPacket )
{
	UI32 i;
	XGameServer *pGameServer;

	BEGIN_JXLIST_LOOP( i, this, XGameServer, pGameServer )

		pGameServer->SendPacket( pPacket );

	END_JXLIST_LOOP
}