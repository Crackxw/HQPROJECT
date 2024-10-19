//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 7
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#include "XPlayerMgr.h"
#include "XPlayer.h"
#include "XChatServer.h"

#include "..\\XCommon\\JXSocket.h"
#include "..\\XCommon\\JXPacketQueue.h"



XPlayerMgr::XPlayerMgr()
{
	m_pPlayer = NULL;
}

XPlayerMgr::~XPlayerMgr()
{
	if( m_pPlayer ) delete [] m_pPlayer;
}

void XPlayerMgr::Create( UI32 uiMaxPlayer )
{
	if( m_pPlayer ) delete [] m_pPlayer;

	m_pPlayer = new XPlayer[ uiMaxPlayer ];

	for( UI32 i = 0; i < uiMaxPlayer; ++i ) {
		
		AddToFree( &m_pPlayer[ i ] );
	}

}

XPlayer* XPlayerMgr::AddPlayer( JXSocket *pSocket )
{
	XPlayer *pPlayer = (XPlayer *)PushBack();
	JXPacketQueue *pPacketQueue = NULL;
	
	if( pPlayer == NULL ) return NULL;

	pPlayer->Init();

	pPlayer->SetSocket( pSocket );

	pPacketQueue = pSocket->GetQueue();
	pPacketQueue->Clear();

	return pPlayer;
}

void XPlayerMgr::DelPlayer( XPlayer *pPlayer )
{
	pPlayer->Init();
	Remove( (JXObject *)pPlayer );
}

void XPlayerMgr::SendPacket( JXPacket *pPacket )
{
	BEGIN_JXLIST_LOOP( local_uiTemp, this, XPlayer, local_pPlayer )

		local_pPlayer->GetSocket()->SendPacket( pPacket );

	END_JXLIST_LOOP
}

void XPlayerMgr::SendPacket( char *pPacket, int len )
{

	BEGIN_JXLIST_LOOP( local_uiTemp, this, XPlayer, local_pPlayer )

		local_pPlayer->GetSocket()->SendPacket( pPacket, len );

	END_JXLIST_LOOP
}


void XPlayerMgr::SendPacket( UI32 uiAccount, JXPacket *pPacket )
{
	BEGIN_JXLIST_LOOP( local_uiTemp, this, XPlayer, local_pPlayer )

		if( local_pPlayer->m_uiAccount == uiAccount ) {
			local_pPlayer->GetSocket()->SendPacket( pPacket );
			break;
		}

	END_JXLIST_LOOP
}

BOOL XPlayerMgr::SendPacket( char *szID, JXPacket *pPacket )
{
	BOOL ret = FALSE;

	BEGIN_JXLIST_LOOP( local_uiTemp, this, XPlayer, local_pPlayer )

		if( strcmp( local_pPlayer->m_szID, szID ) == 0 ) {
			local_pPlayer->GetSocket()->SendPacket( pPacket );
			ret = TRUE;
			break;
		}

	END_JXLIST_LOOP

	return ret;
}
