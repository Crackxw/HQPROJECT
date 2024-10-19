#ifndef _XCELL_H
#define _XCELL_H

#include "..\\XCommon\\JXObject.h"
#include "..\\XCommon\\JXList.h"

class JXPacket;

class XPlayer;

class XCell : public JXObject
{
public:
	XCell();
	virtual ~XCell();

	void AddPlayer( XPlayer *pPlayer );
	void DelPlayer( XPlayer *pPlayer );

	void SendPacket( JXPacket *pPacket );
	void SendPacketExceptOne( JXPacket *pPacket, XPlayer *pPlayer );

public:
	JXList		*m_pPlayerPtrList;

	UI16		m_x;
	UI16		m_y;
};


#endif

