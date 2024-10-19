#ifndef _XCLIENTMGR_H
#define _XCLIENTMGR_H

#include "..\\XCommon\\JXList.h"

class JXPacket;
class JXSocket;
class XClient;

class XClientMgr : public JXList
{
public:
	XClientMgr();
	virtual ~XClientMgr();

public:
	void			Create( UI32 uiMaXClient );
	XClient*		AddClient( JXSocket *pSocket );
	void			DelClient( XClient *pPlayer );

	void			SendPacket( JXPacket *pPacket );
	void			SendPacket( char *pPacket, int len );

public:


private:
	XClient			*m_pClient;

private:
	UI32			local_uiTemp;
	XClient			*local_pClient;
};


#endif
