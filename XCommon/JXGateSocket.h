//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완		
// 최종 수정일 : 2002. 7. 2	
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#ifndef _JXGATESOCKET_H
#define _JXGATESOCKET_H

#include "jxdef.h"

class JXPacket;
class JXSocketMgr;
class JXAcceptSocketList;

class JXGateSocket
{
friend unsigned __stdcall JXGateSocket_ThreadFunc( LPVOID param );

public:
	JXGateSocket();
	~JXGateSocket();

public:
	BOOL				Create( UI16 usPort, JXAcceptSocketList *pAcceptSocketList, 
								SI32 siSocketRecvBuf = 0, SI32 siSocketSendBuf = 0, DWORD dwAcceptInterval = 0 );

	void				Accept();

	void				StartThread();
	void				StopThread();

	void				OpenGate();
	void				CloseGate();

public:
	BOOL				m_bAcceptable;

private:
	BOOL				m_bOpenGate;

private:
	// Thread 관련
	BOOL				m_bThreadRun;
	UI32				m_uiThreadID;
	HANDLE				m_hStopEvent;
	HANDLE				m_hExitEvent;

private:
	JXAcceptSocketList *m_pAcceptSocketList;

private:
	SOCKET				m_socket;
	SOCKADDR_IN			m_sockaddr;

	SI32				m_siAddrlen;

	fd_set				m_fdset;
	timeval				m_timeout;

	SI32				m_siSocketRecvBuf;
	SI32				m_siSocketSendBuf;

	DWORD				m_dwAcceptInterval;

private:
	SOCKET				local_socket;
	SOCKADDR_IN			local_sockaddr;

	fd_set				local_fdset;

};

#endif

