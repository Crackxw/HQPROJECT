//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 8
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#ifndef _XGATESERVERMGR_H
#define _XGATESERVERMGR_H

#include "..\\XCommon\\JXList.h"

class JXPacket;
class JXSocket;
class XGateServer;

class XGateServerMgr : public JXList
{
public:
	XGateServerMgr();
	virtual ~XGateServerMgr();

public:
	void			Create( UI32 uiMaxGateServer );
	XGateServer*	AddServer( JXSocket *pSocket );
	void			DelServer( XGateServer *pServer );
	
	void			SendPacket( JXPacket *pPacket );
	void			SendPacket( char *pPacket, int len );
	
	void			SendPacket( UI16 usServerID, JXPacket *pPacket );
	void			SendPacket( UI16 usServerID, char *pPacket, int len );

private:
	XGateServer		*m_pGateServer;

private:
	UI32			local_uiTemp;
	XGateServer		*local_pGateServer;

};


#endif
