#ifndef _XPLAYER_H
#define _XPLAYER_H

#include "../JWCommon/JWObject.h"

enum	UserGrade{ON_USERGRADE_NORMAL, ON_USERGRADE_ADMIN1, ON_USERGRADE_ADMIN2, ON_USERGRADE_ADMIN3, ON_USERGRADE_ADMIN4, ON_USERGRADE_ADMIN5};

class JWSocket;
class XCell;

class XPlayer : public JWObject
{
public:
	XPlayer();
	virtual ~XPlayer();

	virtual void	Init();
	virtual void	Close();
	JWSocket*		GetSocket();

public:
	UI32			m_uiAccount;
	UI16			m_uiUserGrade;
	char			m_szID[ 32 ];
	char			m_szIP[ 16 ];

	UI16			m_usMapCode;
	XCell			*m_pCell;

	DWORD			m_lastTick;

};

#endif