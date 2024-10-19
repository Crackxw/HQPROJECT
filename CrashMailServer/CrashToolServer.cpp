
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: CrashToolServer.cpp

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

//////////////////////////////////////////////////////////////////////

#include "CrashToolServer.h"
#include <assert.h>
#include "XClient.h"
#include "..\\CrashMailProtocol\\CrashMailProtocol.h"

#include "AdoExecutor.h"
#include "..\\XCommon\\JXConnectSocket.h"	
#include "..\\XCommon\\JXGateSocket.h"	
#include "..\\XCommon\\JXSocketMgr.h"	
#include "..\\XCommon\\JXSocket.h"
#include "..\\XCommon\\JXPacketQueue.h"	
#include "..\\XCommon\\JXAcceptSocketList.h"
#include "XClientMgr.h"

//////////////////////////////////////////////////////////////////////

CCrashToolServer::CCrashToolServer() : 
m_pAdoExecutor(NULL),
m_bProcessThreadRun(FALSE), 
m_hProcessExitEvent(NULL), 
m_hProcessBreakEvent(NULL), 
m_wListenPort(0)
{
	::InitializeCriticalSection(&m_csLock);
	
	m_hProcessBreakEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hProcessExitEvent  = CreateEvent(NULL, FALSE, FALSE, NULL);

	m_pGateSocketClient		= new JXGateSocket;
	m_pAcceptListClient		= new JXAcceptSocketList;
	m_pConnectListClient	= new JXSocketMgr;
	m_pClientMgr			= new XClientMgr;
	m_pAdoExecutor			= new CAdoExecutor(this);
}

//////////////////////////////////////////////////////////////////////

CCrashToolServer::~CCrashToolServer()
{
	delete m_pGateSocketClient;
	delete m_pAcceptListClient;
	delete m_pConnectListClient;
	delete m_pClientMgr;
	delete[] m_pClientToRemove;
	delete m_pAdoExecutor;

	::DeleteCriticalSection(&m_csLock);
}

//////////////////////////////////////////////////////////////////////

unsigned int CCrashToolServer::Run_Process(void* vpThis)
{
	unsigned int uiRetVal = 1;

	try{
		uiRetVal = reinterpret_cast<CCrashToolServer*>(vpThis)->Process();
		return uiRetVal;
	}
	catch(...){
		assert(0);
		return uiRetVal;
	}
}

//////////////////////////////////////////////////////////////////////

unsigned int CCrashToolServer::Process()
{
	DWORD dwRetVal;

	while(TRUE)
	{
		dwRetVal = WaitForSingleObject(m_hProcessBreakEvent, 50);

		if(dwRetVal == WSA_WAIT_EVENT_0)
		{
			break;
		}

		ProcessNetwork();
	}

	SetEvent(m_hProcessExitEvent);	

	return 0;
}

//////////////////////////////////////////////////////////////////////

void CCrashToolServer::StartThread()
{
	StopThread();

	HANDLE hThread = NULL;

	hThread = (HANDLE)_beginthreadex(NULL, 0, Run_Process, 
		reinterpret_cast<CCrashToolServer*>(this), 0, &m_uiProcessThreadID);

	if(hThread != NULL)
	{
		m_bProcessThreadRun = TRUE;
		CloseHandle(hThread);
		printf("[Port : %d] Crash tool server process thread start!!\n", m_wListenPort);
	}
	else
	{
		printf("[Port : %d] Crash tool server process thread could not start. because thread create failed. [Error code : %d]\n", m_wListenPort, GetLastError());
	}
}

//////////////////////////////////////////////////////////////////////

void CCrashToolServer::StopThread()
{
	if(m_bProcessThreadRun == FALSE) return;

	SetEvent(m_hProcessBreakEvent);
	m_bProcessThreadRun = FALSE;

	WaitForSingleObject(m_hProcessExitEvent, INFINITE);

	printf("[Port : %d] Crash tool server process thread end...!!\n", m_wListenPort);
}

//////////////////////////////////////////////////////////////////////

void CCrashToolServer::Create()
{
	int iMaxClient = 100;
	m_pClientToRemove = new XClient*[iMaxClient];
	m_siNumClientToRemove = 0;

	m_wListenPort = CRASH_MAIL_TOOL_SERVER_PORT;

	m_pAcceptListClient->Create( iMaxClient, 
		CRASH_SERVER_RECV_BUF_SIZE, 
		CRASH_SERVER_SEND_BUF_SIZE, 
		CRASH_SERVER_QUEUE_SIZE);
	
	m_pGateSocketClient->Create( CRASH_MAIL_TOOL_SERVER_PORT, m_pAcceptListClient );
	m_pClientMgr->Create( iMaxClient );

	m_pAdoExecutor->Connect();
}

//////////////////////////////////////////////////////////////////////

void CCrashToolServer::Start()
{
	m_pGateSocketClient->StartThread();
	m_pConnectListClient->StartThread();

	m_bThreadRun = TRUE;
}

//////////////////////////////////////////////////////////////////////

void CCrashToolServer::ProcessNetwork()
{
	UI32			i;
	SI32			j;
	JXSocket		*pSocket;
	
	XClient			*pClient;	
	
	JXPacketQueue	*pPacketQueue = NULL;

	m_pAcceptListClient->Wait( INFINITE );
	m_pConnectListClient->Wait( INFINITE );


	BEGIN_JXLIST_LOOP( i, m_pAcceptListClient, JXSocket, pSocket ) 

		printf("AcceptList -> ConnectList : Socket Addr[ %x ]\n", pSocket );			
		
		m_pClientMgr->AddClient( pSocket );
		
		FD_SET( pSocket->m_socket, &m_pConnectListClient->m_fdset );
	
	END_JXLIST_LOOP

	m_pConnectListClient->PushFront( m_pAcceptListClient );	

	for( j = 0; j < m_siNumClientToRemove; ++j ) {
		pClient = m_pClientToRemove[ j ];

		if( pClient->IsValid() == FALSE ) continue;

		FD_CLR( pClient->GetSocket()->m_socket, &m_pConnectListClient->m_fdset );

		pClient->GetSocket()->Disconnect();

		m_pConnectListClient->Remove( (JXObject *)(pClient->GetSocket()) );
		
		m_pClientMgr->DelClient( pClient );
	}
	m_siNumClientToRemove = 0;
	 
	m_pConnectListClient->Release();
	m_pAcceptListClient->Release();
	
	BEGIN_JXLIST_LOOP( i, m_pClientMgr, XClient, pClient ) 
		ProcPacketClient(pClient);
	END_JXLIST_LOOP
}

//////////////////////////////////////////////////////////////////////

void CCrashToolServer::ProcPacketClient(XClient* pClient)
{
	BOOL bConnect;
	pClient->GetSocket()->IsConnect(&bConnect);
	
	if(bConnect == FALSE)
	{
		 m_pClientToRemove[m_siNumClientToRemove++] = pClient;
		 return;
	}

	JXSocket* pSocket = pClient->GetSocket();

	SOCKET Socket = pSocket->GetSocket();

	JXPacketQueue *pPacketQueue = NULL;
	JXPacket Packet;

	pPacketQueue = pClient->GetSocket()->GetQueue();

	UI32 uiNumPacket = pPacketQueue->GetNumPacket();

	for( UI32 i = 0; i < uiNumPacket; ++i )
	{
		if( pPacketQueue->Dequeue( &m_LocalPacket ) == FALSE )
			return;

		m_LocalPacket.ReadValue( &m_dwLocalCmd, sizeof( m_dwLocalCmd ) );
		
		if(m_dwLocalCmd == ON_CRASH_MAIL_TOOL)
		{
			m_LocalPacket.ReadValue( &m_dwLocalCmd, sizeof( m_dwLocalCmd ) );
			
			switch(m_dwLocalCmd)
			{
				case ON_REQ_CRASH_SEARCH_INFO : 
					{					
						m_LocalPacket.ReadValue(&m_szLocalBuf, sizeof(ON_REQ_SEARCHCRASHINFO));
						m_pAdoExecutor->Execution(ADO_CRASH_TOOL_SEARCH_INFO, &m_szLocalBuf, pClient);
					}
					break;

				case ON_REQ_CRASH_STATISTICSINFO : 
					{
						m_LocalPacket.ReadValue(&m_szLocalBuf, sizeof(ON_REQ_SEARCHCRASHINFO));
						m_pAdoExecutor->Execution(ADO_CRASH_TOOL_STATISTICS_INFO, &m_szLocalBuf, pClient);
					}
					break;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

void CCrashToolServer::SendCrashInfoList(ON_P_RESP_SEARCHCRASHINFO_LIST pCrashList, 
										 ON_P_RESP_SEARCHCRASHINFO	    pCrashInfo,
										 int							nCount, 
										 XClient*					    pClient)
{
	SHORT		snPacketSize = 0;
	DWORD		dwMsgType	 = 0;
	SHORT		snPos		 = 0;
	
	memset(m_szLocalBuf, 0, sizeof(m_szLocalBuf));

	snPacketSize = CRASH_SERVER_HEADER_SIZE+(sizeof(DWORD)*2)+
		sizeof(ON_RESP_SEARCHCRASHINFO_LIST)+(sizeof(ON_RESP_SEARCHCRASHINFO)*nCount);

	memcpy(m_szLocalBuf, &snPacketSize, sizeof(SHORT));
	snPos += sizeof(SHORT);

	dwMsgType = ON_CRASH_MAIL_TOOL;
	memcpy(m_szLocalBuf+snPos, &dwMsgType, sizeof(DWORD));
	snPos += sizeof(DWORD);

	dwMsgType = ON_RESP_CRASH_SEARCH_INFO;
	memcpy(m_szLocalBuf+snPos, &dwMsgType, sizeof(DWORD));
	snPos += sizeof(DWORD);

	memcpy(m_szLocalBuf+snPos, pCrashList, sizeof(ON_RESP_SEARCHCRASHINFO_LIST));
	snPos += sizeof(ON_RESP_SEARCHCRASHINFO_LIST);

	memcpy(m_szLocalBuf+snPos, pCrashInfo, sizeof(ON_RESP_SEARCHCRASHINFO)*nCount);

	pClient->GetSocket()->SendPacket(m_szLocalBuf, snPacketSize);
}

//////////////////////////////////////////////////////////////////////

void CCrashToolServer::SendCrashStatisticsInfo(ON_P_RESP_CRASHSTATISTICSINFO pCrashStatistics,
											   XClient*					     pClient)
{
	SHORT		snPacketSize = 0;
	DWORD		dwMsgType	 = 0;
	SHORT		snPos		 = 0;
	
	memset(m_szLocalBuf, 0, sizeof(m_szLocalBuf));

	snPacketSize = CRASH_SERVER_HEADER_SIZE+(sizeof(DWORD)*2)+
		sizeof(ON_RESP_CRASHSTATISTICSINFO);

	memcpy(m_szLocalBuf, &snPacketSize, sizeof(SHORT));
	snPos += sizeof(SHORT);

	dwMsgType = ON_CRASH_MAIL_TOOL;
	memcpy(m_szLocalBuf+snPos, &dwMsgType, sizeof(DWORD));
	snPos += sizeof(DWORD);

	dwMsgType = ON_RESP_CRASH_STATISTICSINFO;
	memcpy(m_szLocalBuf+snPos, &dwMsgType, sizeof(DWORD));
	snPos += sizeof(DWORD);

	memcpy(m_szLocalBuf+snPos, pCrashStatistics, sizeof(ON_RESP_CRASHSTATISTICSINFO));	

	pClient->GetSocket()->SendPacket(m_szLocalBuf, snPacketSize);
}

//////////////////////////////////////////////////////////////////////

