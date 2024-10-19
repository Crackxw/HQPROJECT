#include "XMap.h"
#include "XCell.h"
#include "XChatServer.h"
#include "XPlayer.h"
#include "..\\XCommon\\JXPacket.h"

XMap::XMap()
{
	m_usSizeX = 0;
	m_usSizeY = 0;

	m_pCell = NULL;
}

XMap::~XMap()
{

	if( m_pCell ) delete [] m_pCell;

}

void XMap::InitMap( XChatServer *pServer, UI16 sizeX, UI16 sizeY )
{
	int s = sizeX * sizeY;

	if( m_pCell ) delete [] m_pCell;

	m_pCell = new XCell[ s ];

	for( int i = 0; i < s; ++i ) {
		m_pCell[ i ].m_pPlayerPtrList->SetParentList( pServer->m_pPlayerPtrList );
		m_pCell[ i ].m_x = i % sizeX;
		m_pCell[ i ].m_y = i / sizeX;
	}

	m_usSizeX = sizeX;
	m_usSizeY = sizeY;

}

void XMap::AddPlayer( UI16 x, UI16 y, XPlayer *pPlayer )
{
	if( x > m_usSizeX - 2 || y > m_usSizeY - 2 || m_pCell == NULL ) return;

	m_pCell[ y * m_usSizeX + x ].AddPlayer( pPlayer );

}

void XMap::DelPlayer( UI16 x, UI16 y, XPlayer *pPlayer )
{
	if( x > m_usSizeX - 2 || y > m_usSizeY - 2 || m_pCell == NULL ) return;

	m_pCell[ y * m_usSizeX + x ].DelPlayer( pPlayer );
}

void XMap::SendPacket( XPlayer *pPlayer, JXPacket *pPacket )
{
	if( pPlayer->m_pCell == NULL ) return;

	UI16 x = pPlayer->m_pCell->m_x;
	UI16 y = pPlayer->m_pCell->m_y;

	if( x == 0 || y == 0 ||
		x > m_usSizeX - 2 || y > m_usSizeY - 2 ) return;

	UI16 ofs = ( y - 1 ) * m_usSizeX + x - 1;

	m_pCell[ ofs ].SendPacket( pPacket );
	m_pCell[ ofs + 1 ].SendPacket( pPacket );
	m_pCell[ ofs + 2 ].SendPacket( pPacket );

	ofs += m_usSizeX;

	m_pCell[ ofs ].SendPacket( pPacket );
	m_pCell[ ofs + 1 ].SendPacketExceptOne( pPacket, pPlayer );
	m_pCell[ ofs + 2 ].SendPacket( pPacket );

	ofs += m_usSizeX;

	m_pCell[ ofs ].SendPacket( pPacket );
	m_pCell[ ofs + 1 ].SendPacket( pPacket );
	m_pCell[ ofs + 2 ].SendPacket( pPacket );
}
