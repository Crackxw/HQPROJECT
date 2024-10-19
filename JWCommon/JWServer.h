
#ifndef _JWSERVER_H
#define _JWSERVER_H

#include "jwbase.h"

class JWSocketMgr;
class JWSocket;
class JWList;
class JWFQueueMISO;

class JWServer 
{
public:
	JWServer();
	virtual ~JWServer();

public:
	virtual void		InitServer( SI32 maxnum, SI32 siRecvBufSize, SI32 siSendBufSize );

	virtual BOOL		Accept( SOCKET socket, SOCKADDR_IN sockaddr );

	virtual void		RequestLogOut( JWSocket *pSocket );
	virtual void		CompleteLogOut( JWSocket *pSocket );

	virtual void		UpdateServer( JWList *pList );

	virtual void		AddClient( JWSocket *pSocket, JWList *pList ) {};
	virtual void		DelClient( JWSocket *pSocket, JWList *pList ) {};

protected:
	JWSocketMgr			*m_pAcceptedSocketList;		
	JWSocketMgr			*m_pConnectedSocketList;	
	JWSocket			*m_pSocket;

	JWFQueueMISO		*m_pDeleteList;

};

#endif
