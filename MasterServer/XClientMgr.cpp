#include "XClientMgr.h"
#include "XClient.h"

#include "..\\XCommon\\JXSocket.h"
#include "..\\XCommon\\JXPacketQueue.h"

XClientMgr::XClientMgr()
{
	m_pClient = NULL;
}

XClientMgr::~XClientMgr()
{
	if( m_pClient ) delete [] m_pClient;
}

void XClientMgr::Create( UI32 uiMaxClient)
{
	if( m_pClient ) delete [] m_pClient;

	m_pClient = new XClient[ uiMaxClient ];

	for( UI32 i = 0; i < uiMaxClient; ++i ) {
		
		m_pClient[ i ].m_uiID = i;
		m_pClient[ i ].Init();		

		AddToFree( &m_pClient[ i ] );
	}

}

XClient* XClientMgr::AddClient( JXSocket *pSocket, BOOL bOrdering )
{
	XClient *pClient = (XClient *)PushBack();
	JXPacketQueue *pPacketQueue = NULL;
	
	if( pClient == NULL ) return NULL;

	pClient->Init();

	pClient->SetSocket( pSocket );

	pPacketQueue = pSocket->GetQueue();
	pPacketQueue->Clear();

	pSocket->m_bOdering = bOrdering;

	return pClient;
}

void XClientMgr::DelClient( XClient *pClient )
{
	pClient->Init();
	Remove( (JXObject *)pClient );
}

void XClientMgr::SendPacket( JXPacket *pPacket )
{

	BEGIN_JXLIST_LOOP( local_uiTemp, this, XClient, local_pClient )

		local_pClient->GetSocket()->SendPacket( pPacket );

	END_JXLIST_LOOP
}

void XClientMgr::SendPacket( char *pPacket, int len )
{

	BEGIN_JXLIST_LOOP( local_uiTemp, this, XClient, local_pClient )

		local_pClient->GetSocket()->SendPacket( pPacket, len );

	END_JXLIST_LOOP
}

void XClientMgr::SendPacket( UI16 uc, JXPacket *pPacket )
{

	BEGIN_JXLIST_LOOP( local_uiTemp, this, XClient, local_pClient )

		if( local_pClient->m_uiID == uc ) {
			local_pClient->GetSocket()->SendPacket( pPacket );
			break;
		}

	END_JXLIST_LOOP
}