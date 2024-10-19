#ifndef _XCLIENT_H
#define _XCLIENT_H

#include "..\\XCommon\\JXObject.h"
#include "MasterServer.h"

class MasterServer;
class JXSocket;

class XClient : public JXObject
{

	friend MasterServer;

public:
	XClient();
	virtual ~XClient();

	void			Init();

	void			SetSocket( JXSocket *pSocket );
	JXSocket*		GetSocket();

	XUser			m_user;
	UI16			m_uiID;

private:
	JXSocket		*m_pSocket;

	BOOL			m_bLoginOK;

};

#endif