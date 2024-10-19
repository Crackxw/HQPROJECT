#include "XCell.h"
#include "XPlayer.h"

#include "../JWCommon/JWList.h"
#include "../JWCommon/JWSocket.h"
#include "../JWCommon/JWPacket.h"
#include "../JWCommon/JWSendBuffer.h"

XCell::XCell()
{
	m_pPlayerPtrList = new JWList;
	m_x = m_y = 0;
}

XCell::~XCell()
{
	delete m_pPlayerPtrList;
}

void XCell::AddPlayer( XPlayer *pPlayer )
{

	JWObjectPtr *p = (JWObjectPtr *)(m_pPlayerPtrList->PushFront());

	if( p == NULL ) return;

	p->SetObject( (JWObject *)pPlayer );

	pPlayer->m_pCell = this;
}

void XCell::DelPlayer( XPlayer *pPlayer )
{
	UI32 i;
	JWObjectPtr *p;
	XPlayer *player;
	
	BOOL b = FALSE;

	JWLIST_LOOP( i, m_pPlayerPtrList, JWObjectPtr, p )
	{
		player = (XPlayer *)( p->GetObject() );
		if( player == pPlayer ) { b = TRUE; break; }
	}

	if( b ) {

		m_pPlayerPtrList->Remove( p );

		pPlayer->m_pCell = NULL;
	}

}


void XCell::SendPacket( JWPacket *pPacket )
{

	UI32 i;
	JWObjectPtr *p;
	XPlayer *player;

	JWLIST_LOOP( i, m_pPlayerPtrList, JWObjectPtr, p )
	{
		player = (XPlayer *)( p->GetObject() );
		player->GetSocket()->GetSendBuffer()->Write( pPacket->GetPacket() );
	}

}

void XCell::SendPacketExceptOne( JWPacket *pPacket, XPlayer *pPlayer )
{

	UI32 i;
	JWObjectPtr *p;
	XPlayer *player;

	JWLIST_LOOP( i, m_pPlayerPtrList, JWObjectPtr, p )
	{
		player = (XPlayer *)( p->GetObject() );
		
		if( player != pPlayer ) player->GetSocket()->GetSendBuffer()->Write( pPacket->GetPacket() );
	}

}

