//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 7
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#include "XPlayerMgr.h"
#include "XPlayer.h"
#include "XGateServer.h"

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
		
		m_pPlayer[ i ].m_uiID = i;
		m_pPlayer[ i ].Init();

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

XPlayer* XPlayerMgr::SetAccount( UI32 uiID, UI16 usAccount )
{

	BEGIN_JXLIST_LOOP( local_uiTemp, this, XPlayer, local_pPlayer )

		if( local_pPlayer->m_uiID == uiID ) {
			local_pPlayer->SetAccount( usAccount );
			return local_pPlayer;
		}

	END_JXLIST_LOOP

	return NULL;
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


void XPlayerMgr::SendPacket( UI32 uiID, JXPacket *pPacket )
{
	BEGIN_JXLIST_LOOP( local_uiTemp, this, XPlayer, local_pPlayer )

		if( local_pPlayer->m_uiID == uiID ) {
			local_pPlayer->GetSocket()->SendPacket( pPacket );
			break;
		}

	END_JXLIST_LOOP

}

void XPlayerMgr::SendPacket( UI16 usAccount, char *pPacket, int len )
{

	if( m_pGateServer->m_pPlayerOnAccount[ usAccount ] ) {

		m_pGateServer->m_pPlayerOnAccount[ usAccount ]->GetSocket()->SendPacket( pPacket, len );
	}

}

void XPlayerMgr::SendPacket( UI16 usAccount, JXPacket *pPacket )
{

	if( m_pGateServer->m_pPlayerOnAccount[ usAccount ] ) {

		m_pGateServer->m_pPlayerOnAccount[ usAccount ]->GetSocket()->SendPacket( pPacket );
	}

}

void XPlayerMgr::SendPacket( SI32 siSendNum, UI16 *usAccounts, JXPacket *pPacket )
{

	for( int i = 0; i < siSendNum; ++i ) {
		SendPacket( usAccounts[ i ], pPacket );
	}

}

