
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: SMS Server.h

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


/////////////////////////////////////////////////////////////////////

#ifndef __SMS_SERVER_H_INCLUDED__
#define __SMS_SERVER_H_INCLUDED__

/////////////////////////////////////////////////////////////////////

#include <winsock2.h>
#include <windows.h>
#include "..\\XCommon\\JXPacket.h"
#include "..\\JOCommon\\LogWriter.h"
#include "..\\ServerMonitor\\SMSProtocol.h"


class JXConnectSocket;
class JXGateSocket;
class JXSocketMgr;
class JXSocket;
class JXPacketQueue;
class JXAcceptSocketList;
class XClientMgr;
class XClient;
class XServerMgr;

/////////////////////////////////////////////////////////////////////

class CSMSServer
{	
public : 
	CSMSServer(XServerMgr* pServerMgr,XClientMgr* pClientMgr);
	~CSMSServer();

private : 
	static unsigned int WINAPI	Run_Process(void* vpThis);
	unsigned int Process();

	void Enter()	{ ::EnterCriticalSection(&m_csLock); }
	void Leave()	{ ::LeaveCriticalSection(&m_csLock); }
	
public : 
	void Create();
	void Start();

	void StartThread();
	void StopThread();

	void ProcessNetwork();
	void ProcPacketClient(XClient* pClient);

	void SendConfirmMessage(XClient* pClient);
	void GetIPString(SOCKET Socket, char* IP);	
	void SendSMSMsgResult(SMS_RESP_MSG* pSMS);

private : 
	JXGateSocket		*m_pGateSocketClient;
	JXAcceptSocketList	*m_pAcceptListClient;
	JXSocketMgr			*m_pConnectListClient;
	XClientMgr			*m_pClientMgr;
	XServerMgr			*m_pServerMgr;
	XClientMgr			*m_pMoniterClientMgr;


	XClient				**m_pClientToRemove;
	int					m_siNumClientToRemove;

	CRITICAL_SECTION	m_csLock;

private : 
	BOOL				m_bThreadRun;

	BOOL				m_bProcessThreadRun;
	unsigned int		m_uiProcessThreadID;
	HANDLE				m_hProcessBreakEvent;
	HANDLE				m_hProcessExitEvent;

	WORD				m_wListenPort;

private : 
	CLogWriter			m_LogWriter;

private : 
	JXPacket			m_LocalPacket;
	JXPacket			m_LocalPacketA;
	UI16				m_dwLocalCmd;
	char				m_szLocalBuf[SMS_SERVER_RECV_BUF_SIZE];
};

/////////////////////////////////////////////////////////////////////

#endif // __SMS_SERVER_H_INCLUDED__

/////////////////////////////////////////////////////////////////////                    