#include "XCell.h"
#include "XChatServer.h"
#include "XPlayer.h"

#include "..\\XCommon\\JXSocket.h"

XCell::XCell()
{
	m_pPlayerPtrList = new JXList;
	m_x = m_y = 0;
}

XCell::~XCell()
{
	delete m_pPlayerPtrList;
}

void XCell::AddPlayer( XPlayer *pPlayer )
{

	JXObjectPtr *p = (JXObjectPtr *)(m_pPlayerPtrList->PushFront());

	if( p == NULL ) return;

	p->SetObject( (JXObject *)pPlayer );

	pPlayer->m_pCell = this;
}

void XCell::DelPlayer( XPlayer *pPlayer )
{
	UI32 i;
	JXObjectPtr *p;
	XPlayer *player;
	BOOL b = FALSE;

	BEGIN_JXLIST_LOOP( i, m_pPlayerPtrList, JXObjectPtr, p )

		player = (XPlayer *)( p->GetObject() );
		if( player == pPlayer ) { b = TRUE; break; }
			
	END_JXLIST_LOOP

	if( b ) {
		m_pPlayerPtrList->Remove( p );

		pPlayer->m_pCell = NULL;
	}

}


void XCell::SendPacket( JXPacket *pPacket )
{

	UI32 i;
	JXObjectPtr *p;
	XPlayer *player;

	BEGIN_JXLIST_LOOP( i, m_pPlayerPtrList, JXObjectPtr, p )

		player = (XPlayer *)( p->GetObject() );
		player->GetSocket()->SendPacket( pPacket );
			
	END_JXLIST_LOOP

}

void XCell::SendPacketExceptOne( JXPacket *pPacket, XPlayer *pPlayer )
{

	UI32 i;
	JXObjectPtr *p;
	XPlayer *player;

	BEGIN_JXLIST_LOOP( i, m_pPlayerPtrList, JXObjectPtr, p )


		player = (XPlayer *)( p->GetObject() );
		
		if( player != pPlayer ) player->GetSocket()->SendPacket( pPacket );
			
	END_JXLIST_LOOP
}

