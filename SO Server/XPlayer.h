//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이 제 완			
// 최종 수정일 : 2002. 7. 8
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#ifndef _XPLAYER_H
#define _XPLAYER_H

#include "..\\JWCommon\\JWObject.h"

class SOPlayer;
class SOServer;
class JWSocket;

class XPlayer : public JWObject
{
public:
	XPlayer();
	virtual ~XPlayer();

public:
	virtual void		Init();
	virtual void		Close();
	void				Write( char *pPacket );
	JWSocket*			GetSocket() { return (JWSocket *)m_pFriend; };			

public:
	SOPlayer			*m_pSOPlayer;					
	SOServer			*m_pServer;

	UI16				m_usAccount;					// 플레이어 어카운트

	char				m_cKey1;						// encryption key
	char				m_cKey2;						// encryption key

	//플레이어의 라운드 키 - 2004-12-07- 최병주
	DWORD               m_dwRoundKey[32];               // encryption Seed Key 
};

#endif


