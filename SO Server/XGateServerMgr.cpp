//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 8
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#include "XGateServerMgr.h"
#include "XGateServer.h"

#include "..\\XCommon\\JXConnectSocket.h"
#include "..\\XCommon\\JXPacket.h"
#include "..\\XCommon\\JXPacketQueue.h"


XGateServerMgr::XGateServerMgr()
{
	m_pGateServer = NULL;
}

XGateServerMgr::~XGateServerMgr()
{
	if( m_pGateServer ) delete [] m_pGateServer;
}


void XGateServerMgr::Create( UI32 uiMaxGateServer )
{
	if( m_pGateServer ) delete [] m_pGateServer;

	m_pGateServer = new XGateServer[ uiMaxGateServer ];

	for( UI32 i = 0; i < uiMaxGateServer; ++i ) {
		
		m_pGateServer[ i ].m_usServerID = i + 1;

		AddToFree( &m_pGateServer[ i ] );
	}

}

XGateServer* XGateServerMgr::AddServer( JXSocket *pSocket )
{
	XGateServer *pServer = (XGateServer *)PushBack();

	if( pServer == NULL ) return NULL;

	pServer->SetSocket( pSocket );


	JXPacketQueue *pPacketQueue = NULL;
	
	pPacketQueue = pSocket->GetQueue();
	pPacketQueue->Clear();

	return pServer;
}

void XGateServerMgr::DelServer( XGateServer *pServer )
{
	Remove( (JXObject *)pServer );
}

// 전체 게이트 서버로 패킷을 전송한다
void XGateServerMgr::SendPacket( JXPacket *pPacket )
{

	BEGIN_JXLIST_LOOP( local_uiTemp, this, XGateServer, local_pGateServer )

		local_pGateServer->GetSocket()->SendPacket( pPacket );

	END_JXLIST_LOOP
}

void XGateServerMgr::SendPacket( char *pPacket, int len )
{

	BEGIN_JXLIST_LOOP( local_uiTemp, this, XGateServer, local_pGateServer )

		local_pGateServer->GetSocket()->SendPacket( pPacket, len );

	END_JXLIST_LOOP
}

// 특정 코드의 게이트 서버로 패킷을 전송한다
void XGateServerMgr::SendPacket( UI16 usServerID, JXPacket *pPacket )
{
	BEGIN_JXLIST_LOOP( local_uiTemp, this, XGateServer, local_pGateServer )

		if( local_pGateServer->m_usServerID == usServerID ) {

			local_pGateServer->GetSocket()->SendPacket( pPacket );

			break;
		}

	END_JXLIST_LOOP
}

void XGateServerMgr::SendPacket( UI16 usServerID, char *pPacket, int len )
{
	BEGIN_JXLIST_LOOP( local_uiTemp, this, XGateServer, local_pGateServer )

		if( local_pGateServer->m_usServerID == usServerID ) {

			local_pGateServer->GetSocket()->SendPacket( pPacket, len );

			break;
		}

	END_JXLIST_LOOP
}

