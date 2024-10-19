#ifndef _XSERVERMGR_H
#define _XSERVERMGR_H

#include "..\\XCommon\\JXList.h"

class JXPacket;
class JXSocket;
class XServer;

class XServerMgr : public JXList
{
public:
	XServerMgr();
	virtual ~XServerMgr();

public:
	void			Create( UI32 uiMaXServer );
	XServer*		AddServer( JXSocket *pSocket );
	void			DelServer( XServer *pPlayer );

	void			SendPacket( JXPacket *pPacket );
	void			SendPacket( char *pPacket, int len );
	void			SendPacket( WORD servercode, char *pPacket, int len );
	void			SendPacket( WORD servercode, JXPacket *pPacket );

private:
	XServer			*m_pServer;

private:
	UI32			local_uiTemp;
	XServer			*local_pServer;

};

#endif
