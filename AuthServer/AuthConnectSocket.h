

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: AuthConnectSocket.h

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

//////////////////////////////////////////////////////////////////////

#ifndef __AUTH_CONNECT_SOCKET_H_INCLUDED__
#define __AUTH_CONNECT_SOCKET_H_INCLUDED__

//////////////////////////////////////////////////////////////////////

#include "AuthSocket.h"

//////////////////////////////////////////////////////////////////////

class CAuthConnectSocket : public CAuthSocket
{
public : 
	CAuthConnectSocket();
	virtual ~CAuthConnectSocket();

private : 
	static unsigned int WINAPI	Run_Connect_Thread(void* vpThis);
	unsigned int ConnectThread();

protected : 
	virtual void OnDisConnect() {};

public : 
	void	Create(char* pIP, WORD wPort);
	BOOL	Connect();	
	void	StopConnectThread();
	void	StartConnectThread();
	char*	SetPacket(char* pPacket, void* pData, int nSize);	

protected : 
	char	m_szIP[16];
	WORD	m_wPort;

protected : 
	CRITICAL_SECTION m_csQueueLock;

	void EnterQueueLock()	{ ::EnterCriticalSection(&m_csQueueLock); }
	void LeaveQueueLock()	{ ::LeaveCriticalSection(&m_csQueueLock); }

private : 
	BOOL			m_bConnectThreadRun;
	unsigned int	m_uiConnectThreadID;	
	HANDLE			m_hExitConnectThread;

	HANDLE			m_hEvent[2];	// 0 - Break connect thread event.
									// 1 - Close socket event.
};

//////////////////////////////////////////////////////////////////////

#endif // __AUTH_CONNECT_SOCKET_H_INCLUDED__

//////////////////////////////////////////////////////////////////////