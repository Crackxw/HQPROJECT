#ifndef _XCLIENT_H
#define _XCLIENT_H

#include "..\\XCommon\\JXObject.h"
#include "..\\XCommon\\JXPacket.h"


struct Monitor_Command_FileTransferStart;

class MonitorServer;
class JXSocket;

class XClient : public JXObject
{

	friend MonitorServer;

public:
	XClient();
	virtual ~XClient();

	void			Init();

	void			SetSocket( JXSocket *pSocket );
	JXSocket*		GetSocket();

	
	void			SendPacket( char *pPacket, int len );

	
public:
	char			local_buf[ 30000 ];
	JXPacket		local_packet;


private:
	JXSocket		*m_pSocket;

	
	

};

#endif