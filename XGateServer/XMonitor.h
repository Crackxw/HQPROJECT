#ifndef _XMONITOR_H
#define _XMONITOR_H

#include "..\\XCommon\\JXObject.h"

class XMonitor : public JXObject
{
public:
	XMonitor();
	virtual ~XMonitor();

	void		Init() {};

	void		SetSocket( JXSocket *pSocket ) { m_pSocket = pSocket; };
	JXSocket*	GetSocket() { return m_pSocket; };

public:
	JXSocket	*m_pSocket;
};

#endif