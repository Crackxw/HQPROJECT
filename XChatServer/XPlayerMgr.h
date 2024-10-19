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

class XPlayerMgr : public JXList
{
public:
	XPlayerMgr();
	virtual ~XPlayerMgr();

public:
	void			Create( UI32 uiMaxPlayer );
	XPlayer*		AddPlayer( JXSocket *pSocket );
	void			DelPlayer( XPlayer *pPlayer );

	void			SendPacket( JXPacket *pPacket );
	void			SendPacket( char *pPacket, int len );

	BOOL			SendPacket( char *szID, JXPacket *pPacket );

	void			SendPacket( UI32 uiAccount, JXPacket *pPacket );

private:
	XPlayer			*m_pPlayer;

private:
	UI32			local_uiTemp;
	XPlayer			*local_pPlayer;
};


#endif
