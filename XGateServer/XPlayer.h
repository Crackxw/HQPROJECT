//������������������������������������������������������������������������������
// �ۼ���      : ������			
// ���� ������ : 2002. 7. 7
//������������������������������������������������������������������������������


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
	UI16			m_usAccount;				// 0�̸� ��ī��Ʈ�� ��û �ؾ� �ȴ�. 
	BOOL			m_bRequestAccount;			// ��ī��Ʈ�� ��û �ߴ°�
	BOOL			m_bSendLogOutToGameServer;

	char			m_szIP[ 16 ];

};

#endif