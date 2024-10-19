#ifndef _XMAP_H
#define _XMAP_H

#include "..\\XCommon\\JXObject.h"

class XCell;
class XPlayer;
class XChatServer;
class JXPacket;

class XMap : public JXObject
{
public:
	XMap();
	virtual ~XMap();

	void InitMap( XChatServer *pServer, UI16 sizeX, UI16 sizeY );

	void AddPlayer( UI16 x, UI16 y, XPlayer *pPlayer );
	void DelPlayer( UI16 x, UI16 y, XPlayer *pPlayer );

	void SendPacket( XPlayer *pPlayer, JXPacket *pPacket );

public:
	UI16	m_usSizeX;
	UI16	m_usSizeY;

	XCell	*m_pCell;
};


#endif

