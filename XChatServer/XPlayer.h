//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 7
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


#ifndef _XPLAYER_H
#define _XPLAYER_H

#include "..\\XCommon\\JXObject.h"

class JXSocket;
class XCell;

class XPlayer : public JXObject
{
public:
	XPlayer();
	virtual ~XPlayer();

	void			Init();

	void			SetSocket( JXSocket *pSocket );
	JXSocket*		GetSocket();

private:
	JXSocket		*m_pSocket;

public:
	UI32			m_uiAccount;
	char			m_szID[ 32 ];
	char			m_szIP[ 16 ];

	UI16			m_usMapCode;
	XCell			*m_pCell;


};

#endif