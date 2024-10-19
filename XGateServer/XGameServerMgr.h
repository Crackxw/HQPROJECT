//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 7
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


#ifndef _XGAMESERVERMGR_H
#define _XGAMESERVERMGR_H

#include "..\\XCommon\\JXList.h"

class XGameServer;
class JXSocket;
class JXPacket;

class XGameServerMgr : public JXList
{
public:
	XGameServerMgr();
	virtual ~XGameServerMgr();

public:
	void				Create( UI32 uiMaxGameServer );
	XGameServer*		AddGameServer( JXSocket *pSocket );
	void				SendPacket( JXPacket *pPacket );

private:
	XGameServer			*m_pGameServer;

};

#endif