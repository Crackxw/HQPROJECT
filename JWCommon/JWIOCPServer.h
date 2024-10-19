#ifndef _JWIOCPSERVER_H
#define _JWIOCPSERVER_H

#include "jwbase.h"
#include "JWServer.h"
 
class JWSocket;
class JWList;

class JWIOCPServer : public JWServer
{
friend unsigned __stdcall iocpserver_send( LPVOID param );
friend unsigned __stdcall iocpserver_run( LPVOID param );

public:
	JWIOCPServer();
	virtual ~JWIOCPServer();

public:
	virtual void	InitServer( SI32 maxnum, SI32 siRecvBufSize, SI32 siSendBufSize );

	virtual void	AddClient( JWSocket *pSocket, JWList *pList );
	virtual void	DelClient( JWSocket *pSocket, JWList *pList );

private:
	HANDLE			m_hIOCP;

	WSABUF			m_wsabuf;

};



#endif
