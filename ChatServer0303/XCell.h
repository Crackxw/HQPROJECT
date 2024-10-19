#ifndef _XCELL_H
#define _XCELL_H

#include "../JWCommon/JWObject.h"

class JWPacket;
class JWList;

class XPlayer;

class XCell : public JWObject
{
public:
	XCell();
	virtual ~XCell();

	void AddPlayer( XPlayer *pPlayer );
	void DelPlayer( XPlayer *pPlayer );

	void SendPacket( JWPacket *pPacket );
	void SendPacketExceptOne( JWPacket *pPacket, XPlayer *pPlayer );

public:
	JWList		*m_pPlayerPtrList;

	UI16		m_x;
	UI16		m_y;
};


#endif

