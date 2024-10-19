
#ifndef _XPLAYERMGR_H
#define _XPLAYERMGR_H

#include "../JWCommon/JWList.h"

class JWPacket;
class JWSocket;
class XPlayer;

class XPlayerMgr : public JWList
{
public:
	XPlayerMgr();
	virtual ~XPlayerMgr();

public:
	void			Create( UI32 uiMaxPlayer );

	void			SendPacket( JWPacket *pPacket );
	void			SendPacket( char *pPacket, int len );

	BOOL			SendPacket( char *szID, JWPacket *pPacket );

	void			SendPacket( UI32 uiAccount, JWPacket *pPacket );

private:
	XPlayer			*m_pPlayer;

private:
	UI32			local_uiTemp;
	XPlayer			*local_pPlayer;
};


#endif
