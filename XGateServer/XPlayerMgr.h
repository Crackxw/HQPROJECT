//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 7
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


#ifndef _XPLAYERMGR_H
#define _XPLAYERMGR_H

#include "..\\XCommon\\JXList.h"

class JXPacket;
class JXSocket;
class XPlayer;
class XGateServer;

class XPlayerMgr : public JXList
{
public:
	XPlayerMgr();
	virtual ~XPlayerMgr();

public:
	void			Create( UI32 uiMaxPlayer );
	XPlayer*		AddPlayer( JXSocket *pSocket );
	void			DelPlayer( XPlayer *pPlayer );
	
	XPlayer*		SetAccount( UI32 uiID, UI16 usAccount );

	void			SendPacket( JXPacket *pPacket );
	void			SendPacket( char *pPacket, int len );

	void			SendPacket( UI32 uiID, JXPacket *pPacket );

	void			SendPacket( UI16 usAccount, JXPacket *pPacket );
	void			SendPacket( UI16 usAccount, char *pPacket, int len );
	
	void			SendPacket( SI32 siSendNum, UI16 *usAccounts, JXPacket *pPacket );

public:
	XGateServer		*m_pGateServer;

private:
	XPlayer			*m_pPlayer;

private:
	UI32			local_uiTemp;
	XPlayer			*local_pPlayer;
};


#endif
