//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 7
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


#ifndef _XGAMESERVER_H
#define _XGAMESERVER_H

#include "..\\XCommon\\JXObject.h"

class JXSocket;
class JXPacket;

class XGameServer : public JXObject
{
public:
	XGameServer();
	virtual ~XGameServer();

	void			Init() {};

	void			SetSocket( JXSocket *pSocket ) { m_pSocket = pSocket; };
	JXSocket*		GetSocket() { return m_pSocket; };

	void			SendPacket( JXPacket *pPacket );

private:
	JXSocket		*m_pSocket;


};


#endif