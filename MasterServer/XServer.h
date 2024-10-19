#ifndef _XSERVER_H
#define _XSERVER_H

#include "..\\XCommon\\JXObject.h"

class JXSocket;

class XServer : public JXObject
{
public:
	XServer();
	virtual ~XServer();

	void			Init();

	void			SetSocket( JXSocket *pSocket );
	JXSocket*		GetSocket();

private:
	JXSocket		*m_pSocket;

public:
	WORD			m_servercode;

};

#endif
