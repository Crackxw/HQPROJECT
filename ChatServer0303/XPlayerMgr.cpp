
#include "XPlayerMgr.h"
#include "XPlayer.h"
#include "XChatServer.h"

#include "../JWCommon/JWSocket.h"
#include "../JWCommon/JWPacket.h"
#include "../JWCommon/JWSendBuffer.h"


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
		
		NewObject( &m_pPlayer[ i ] );
	}

}

void XPlayerMgr::SendPacket( JWPacket *pPacket )
{
	JWLIST_LOOP( local_uiTemp, this, XPlayer, local_pPlayer )
	{

		local_pPlayer->GetSocket()->GetSendBuffer()->Write( pPacket->GetPacket() );
	}
}

void XPlayerMgr::SendPacket( char *pPacket, int len )
{

	JWLIST_LOOP( local_uiTemp, this, XPlayer, local_pPlayer )
	{

		local_pPlayer->GetSocket()->GetSendBuffer()->Write( pPacket );

	}
}


void XPlayerMgr::SendPacket( UI32 uiAccount, JWPacket *pPacket )
{

	JWLIST_LOOP( local_uiTemp, this, XPlayer, local_pPlayer )
	{

		if( local_pPlayer->m_uiAccount == uiAccount ) {
			local_pPlayer->GetSocket()->GetSendBuffer()->Write( pPacket->GetPacket() );
			break;
		}
	}
}

BOOL XPlayerMgr::SendPacket( char *szID, JWPacket *pPacket )
{
	BOOL ret = FALSE;

	JWLIST_LOOP( local_uiTemp, this, XPlayer, local_pPlayer )
	{

		if( strcmp( local_pPlayer->m_szID, szID ) == 0 ) {
			local_pPlayer->GetSocket()->GetSendBuffer()->Write( pPacket->GetPacket() );
			ret = TRUE;
			break;
		}
	}

	return ret;
}

