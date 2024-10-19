//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 8
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#ifndef _XGateServer_H
#define _XGateServer_H

#include "..\\XCommon\\JXObject.h"

class JXPacket;
class JXSocket;

class XGateServer : public JXObject
{
public:
	XGateServer();
	virtual ~XGateServer();

	void				SetSocket( JXSocket *pSocket ){ 
							m_pSocket = pSocket; 
							m_uiNewPlayerNum = 0;
							m_uiLogoutPlayerNum = 0;
						};

	JXSocket*			GetSocket() { return m_pSocket; };

	void				SendPacket( JXPacket *pPacket );

private:
	JXSocket			*m_pSocket;


////////////////////////////

public:
	UI16				m_usServerID;
	BOOL				m_bRDisconnect;


	UI32				m_uiNewPlayerNum;
	UI32				m_uiLogoutPlayerNum;

	UI32				m_uiUserCode[ 300 ];
	char				m_szUserIP[ 300 ][ 20 ];

	UI16				m_usLogoutAccount[ 300 ];
	
};

#endif