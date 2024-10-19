//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 7
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


#ifndef _XPLAYER_H
#define _XPLAYER_H

#include "..\\XCommon\\JXObject.h"

class JXSocket;

class XPlayer : public JXObject
{
public:
	XPlayer();
	virtual ~XPlayer();

	void			Init();

	void			SetAccount( UI16 usAccount );

	void			SetSocket( JXSocket *pSocket );
	JXSocket*		GetSocket();

private:
	JXSocket		*m_pSocket;

public:
	UI32			m_uiID;
	UI16			m_usAccount;				// 0이면 어카운트를 요청 해야 된다. 
	BOOL			m_bRequestAccount;			// 어카운트를 요청 했는가
	BOOL			m_bSendLogOutToGameServer;

	char			m_szIP[ 16 ];

};

#endif