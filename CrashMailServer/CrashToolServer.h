
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: CrashToolServer.h

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


/////////////////////////////////////////////////////////////////////

#ifndef __CRASH_TOOL_SERVER_H_INCLUDED__
#define __CRASH_TOOL_SERVER_H_INCLUDED__

/////////////////////////////////////////////////////////////////////

#include <winsock2.h>
#include <windows.h>
#include "..\\CrashMailProtocol\\CrashMailProtocol.h"
#include "..\\XCommon\\JXPacket.h"

class CAdoExecutor;
class JXConnectSocket;
class JXGateSocket;
class JXSocketMgr;
class JXSocket;
class JXPacketQueue;
class JXAcceptSocketList;
class XClientMgr;
class XClient;

/////////////////////////////////////////////////////////////////////

class CCrashToolServer
{	
public : 
	CCrashToolServer();
	~CCrashToolServer();

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

	void SendCrashInfoList(ON_P_RESP_SEARCHCRASHINFO_LIST	pCrashList, 
						   ON_P_RESP_SEARCHCRASHINFO	    pCrashInfo,
						   int								nCount, 
						   XClient*							pClient);

	void SendCrashStatisticsInfo(ON_P_RESP_CRASHSTATISTICSINFO pCrashStatistics,
								 XClient*					   pClient);

private : 
	CAdoExecutor*		m_pAdoExecutor;

private : 
	JXGateSocket		*m_pGateSocketClient;
	JXAcceptSocketList	*m_pAcceptListClient;
	JXSocketMgr			*m_pConnectListClient;
	XClientMgr			*m_pClientMgr;

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
	JXPacket			m_LocalPacket;
	DWORD				m_dwLocalCmd;
	char				m_szLocalBuf[CRASH_SERVER_RECV_BUF_SIZE];
};

/////////////////////////////////////////////////////////////////////

#endif // __CRASH_USER_SERVER_H_INCLUDED__

/////////////////////////////////////////////////////////////////////