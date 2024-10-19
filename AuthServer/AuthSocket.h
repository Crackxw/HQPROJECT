

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: AuthSocket.cpp

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


//////////////////////////////////////////////////////////////////////

#ifndef __AUTHSOCKET_H_INCLUDED__
#define __AUTHSOCKET_H_INCLUDED__

//////////////////////////////////////////////////////////////////////

#include <winsock2.h>
#include <windows.h>
#include "CmdQueue.h"
#include "ParentCallback.h"
#include "LogWriter.h"

//////////////////////////////////////////////////////////////////////

#define DEFAULT_TIMEOUT_VALUE			20000
#define DEFAULT_SENDABLE_TIME_CHANGE	(20000*2)

//////////////////////////////////////////////////////////////////////

#define GASH_SERVER_REPONSE_OK				0
#define GASH_SERVER_RECV_READY_ENABLE		1				
#define GASH_SERVER_REPONSE_TIMEOUT			2
#define GASH_SERVER_UNKNOWN_FAIL			3

//////////////////////////////////////////////////////////////////////

class CAuthSocket
{
public : 
	CAuthSocket();
	virtual ~CAuthSocket();

public : 
	BOOL	IsConnect();

	void	SetTimeout			(DWORD dwMilliSecond);
	BOOL	GetNextSendAble		(void) const;
	void	SetNextSendAble		(void);
	void	UnSetNextSendAble	(void);	

protected : 
	BOOL	Send(const char* pPacket, int nSize);
	BYTE	Recv(char* pPacket, int nSize);

	void	DisConnect(BOOL bEnd = FALSE);	

	void	SetSocket(SOCKET Socket, SOCKADDR_IN SockAddr);
	SOCKET	GetSocket() const;
	
	BOOL	IsSendAble() const;
	BYTE	IsRecvAble() const;
	BOOL	IsTimeout() const;
	
	void	SendAbleChangeCheckAndChange();

protected : 
	void	SetParent(CParentCallback* pParent);	

protected : 
	void	Enter()		{ ::EnterCriticalSection(&m_csLock); }
	void	Leave()		{ ::LeaveCriticalSection(&m_csLock); }

protected : 
	SOCKET			m_Socket;
	SOCKADDR_IN		m_SocketAddr;

	BOOL			m_bConnect;	
	DWORD			m_dwSendTickCount;
	DWORD			m_dwReponseTimeout;	

protected : 	
	BOOL			m_bNextSendAble;
	DWORD			m_dwNextSendAbleTime;
	DWORD			m_dwSendAbleChangeTime;

protected : 
	CLogWriter		m_LogWriter;

protected : 
	CRITICAL_SECTION	m_csLock;
	
private : 
	CParentCallback* volatile m_pParentCallBack;
};

//////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////