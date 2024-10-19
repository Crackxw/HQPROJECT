
/////////////////////////////////////////////////////////////////////

#ifndef __GASH_SERVER_H_INCLUDED__
#define __GASH_SERVER_H_INCLUDED__

/////////////////////////////////////////////////////////////////////

#include "..\\XCommon\\JXConnectSocket.h"	
#include "..\\XCommon\\JXGateSocket.h"	
#include "..\\XCommon\\JXSocketMgr.h"	
#include "..\\XCommon\\JXSocket.h"		
#include "..\\XCommon\\JXPacket.h"		
#include "..\\XCommon\\JXPacketQueue.h"	
#include "..\\XCommon\\JXAcceptSocketList.h"
#include "XClientMgr.h"

/////////////////////////////////////////////////////////////////////

class CGashServer
{	
public : 
	CGashServer();
	~CGashServer();

private : 
	static unsigned int WINAPI	Run_Process(void* vpThis);
	unsigned int Process();

	void Enter()	{ ::EnterCriticalSection(&m_csLock); }
	void Leave()	{ ::LeaveCriticalSection(&m_csLock); }
	
public : 
	void Create(WORD wProt, char szOutputString[256]);
	void Start();

	void StartThread();
	void StopThread();

	void ProcessNetwork();
	void ProcPacketClient(XClient* pClient);

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
	char				m_szOutputString[256];
};

/////////////////////////////////////////////////////////////////////

#endif // __GASH_SERVER_H_INCLUDED__

/////////////////////////////////////////////////////////////////////