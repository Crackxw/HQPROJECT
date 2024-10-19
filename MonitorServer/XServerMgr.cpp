#include "XServerMgr.h"
#include "XServer.h"

#include "..\\XCommon\\JXSocket.h"
#include "..\\XCommon\\JXPacketQueue.h"


XServerMgr::XServerMgr()
{
	m_pServer = NULL;
}

XServerMgr::~XServerMgr()
{
	if( m_pServer ) delete [] m_pServer;
}

void XServerMgr::Create( UI32 uiMaxServer )
{
	if( m_pServer ) delete [] m_pServer;

	m_pServer = new XServer[ uiMaxServer ];

	for( UI32 i = 0; i < uiMaxServer; ++i ) {
		
//		m_pServer[ i ].m_uiID = i;
		m_pServer[ i ].Init();

		AddToFree( &m_pServer[ i ] );
	}

}

XServer* XServerMgr::AddServer( JXSocket *pSocket )
{
	XServer *pServer = (XServer *)PushBack();
	JXPacketQueue *pPacketQueue = NULL;
	
	if( pServer == NULL ) return NULL;

	pServer->Init();

	pServer->SetSocket( pSocket );

	pPacketQueue = pSocket->GetQueue();
	pPacketQueue->Clear();

	return pServer;
}

void XServerMgr::DelServer( XServer *pServer )
{
	pServer->Init();
	Remove( (JXObject *)pServer );
}

void XServerMgr::SendPacket( JXPacket *pPacket )
{
	BEGIN_JXLIST_LOOP( local_uiTemp, this, XServer, local_pServer )

		local_pServer->GetSocket()->SendPacket( pPacket );

	END_JXLIST_LOOP
}

void XServerMgr::SendPacket( char *pPacket, int len )
{

	BEGIN_JXLIST_LOOP( local_uiTemp, this, XServer, local_pServer )

		local_pServer->GetSocket()->SendPacket( pPacket, len );

	END_JXLIST_LOOP
}
