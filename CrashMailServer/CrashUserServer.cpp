
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: CrashUserServer.cpp

Author: 이민수 <nshangukin@joyon.com>
Currently Managed by: 이민수

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


//////////////////////////////////////////////////////////////////////

#include "CrashUserServer.h"
#include <assert.h>
#include "XClient.h"

#include "Defines.h"
#include "AdoExecutor.h"
#include "..\\XCommon\\JXConnectSocket.h"	
#include "..\\XCommon\\JXGateSocket.h"	
#include "..\\XCommon\\JXSocketMgr.h"	
#include "..\\XCommon\\JXSocket.h"
#include "..\\XCommon\\JXPacketQueue.h"	
#include "..\\XCommon\\JXAcceptSocketList.h"
#include "XClientMgr.h"

//////////////////////////////////////////////////////////////////////

CCrashUserServer::CCrashUserServer() : 
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
	m_pAdoExecutor			= new CAdoExecutor;
}

//////////////////////////////////////////////////////////////////////

CCrashUserServer::~CCrashUserServer()
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

unsigned int CCrashUserServer::Run_Process(void* vpThis)
{
	unsigned int uiRetVal = 1;

	try{
		uiRetVal = reinterpret_cast<CCrashUserServer*>(vpThis)->Process();
		return uiRetVal;
	}
	catch(...){
		assert(0);
		return uiRetVal;
	}
}

//////////////////////////////////////////////////////////////////////

unsigned int CCrashUserServer::Process()
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

void CCrashUserServer::StartThread()
{
	StopThread();

	HANDLE hThread = NULL;

	hThread = (HANDLE)_beginthreadex(NULL, 0, Run_Process, 
		reinterpret_cast<CCrashUserServer*>(this), 0, &m_uiProcessThreadID);

	if(hThread != NULL)
	{
		m_bProcessThreadRun = TRUE;
		CloseHandle(hThread);
		printf("[Port : %d] Crash user server process thread start!!\n", m_wListenPort);
	}
	else
	{
		printf("[Port : %d] Crash user server process thread could not start. because thread create failed. [Error code : %d]\n", m_wListenPort, GetLastError());
	}
}

//////////////////////////////////////////////////////////////////////

void CCrashUserServer::StopThread()
{
	if(m_bProcessThreadRun == FALSE) return;

	SetEvent(m_hProcessBreakEvent);
	m_bProcessThreadRun = FALSE;

	printf("[Port : %d]process thread ending...!!\n", m_wListenPort);

	WaitForSingleObject(m_hProcessExitEvent, INFINITE);

	printf("[Port : %d] Crash user server process thread end...!!\n", m_wListenPort);
}

//////////////////////////////////////////////////////////////////////

void CCrashUserServer::Create()
{
	int iMaxClient = 100;
	m_pClientToRemove = new XClient*[iMaxClient];
	m_siNumClientToRemove = 0;

	m_wListenPort = CRASH_MAIL_USER_SERVER_PORT;

	m_pAcceptListClient->Create( iMaxClient, 
		CRASH_SERVER_RECV_BUF_SIZE, 
		CRASH_SERVER_SEND_BUF_SIZE, 
		CRASH_SERVER_QUEUE_SIZE);
	
	m_pGateSocketClient->Create( CRASH_MAIL_USER_SERVER_PORT, m_pAcceptListClient );
	m_pClientMgr->Create( iMaxClient );

	m_pAdoExecutor->Connect();
}

//////////////////////////////////////////////////////////////////////

void CCrashUserServer::Start()
{
	m_pGateSocketClient->StartThread();
	m_pConnectListClient->StartThread();

	m_bThreadRun = TRUE;
}

//////////////////////////////////////////////////////////////////////

void CCrashUserServer::ProcessNetwork()
{
	UI32			i;
	SI32			j;
	JXSocket		*pSocket;
	
	XClient			*pClient;	
	
	JXPacketQueue	*pPacketQueue = NULL;

	// 접속을 받아들인 신규 소켓 리스트를 본 리스트로 옮겨온다
	// (플레이어)
	m_pAcceptListClient->Wait( INFINITE );
	m_pConnectListClient->Wait( INFINITE );


	BEGIN_JXLIST_LOOP( i, m_pAcceptListClient, JXSocket, pSocket ) 

		printf("AcceptList -> ConnectList : Socket Addr[ %x ]\n", pSocket );			
		
		m_pClientMgr->AddClient( pSocket );
		
		FD_SET( pSocket->m_socket, &m_pConnectListClient->m_fdset );
	
	END_JXLIST_LOOP

	m_pConnectListClient->PushFront( m_pAcceptListClient );

	// 접속 끊기

	for( j = 0; j < m_siNumClientToRemove; ++j ) {
		pClient = m_pClientToRemove[ j ];

		if( pClient->IsValid() == FALSE ) continue;

		FD_CLR( pClient->GetSocket()->m_socket, &m_pConnectListClient->m_fdset );

		pClient->GetSocket()->Disconnect();
		pClient->m_bReady = FALSE;

		m_pConnectListClient->Remove( (JXObject *)(pClient->GetSocket()) );
		
		m_pClientMgr->DelClient( pClient );
	}
	m_siNumClientToRemove = 0;
	 
	m_pConnectListClient->Release();
	m_pAcceptListClient->Release();
	
	BEGIN_JXLIST_LOOP( i, m_pClientMgr, XClient, pClient ) 
		SendConfirmMessage(pClient);
		ProcPacketClient(pClient);
	END_JXLIST_LOOP
}

//////////////////////////////////////////////////////////////////////

void CCrashUserServer::ProcPacketClient(XClient* pClient)
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
		if(m_dwLocalCmd == ON_CRASH_MAIL_CLIENT)
		{
			m_LocalPacket.ReadValue( &m_dwLocalCmd, sizeof( m_dwLocalCmd ) );

			switch(m_dwLocalCmd)
			{
				case ON_REQ_CRASH_USER_INFO_SAVE :
					{
						m_LocalPacket.ReadValue(&m_szLocalBuf, sizeof(ON_REQ_CRASHINFO));
						m_pAdoExecutor->Execution(ADO_CRASH_USER_INFO_SAVE, &m_szLocalBuf);
						memset(m_szLocalBuf, 0, sizeof(m_szLocalBuf));
					}
					break;
			}
		}

		m_pClientToRemove[m_siNumClientToRemove++] = pClient;
	}
}

//////////////////////////////////////////////////////////////////////

void CCrashUserServer::SendConfirmMessage(XClient* pClient)
{
	if(pClient->m_bReady == FALSE)
	{	
		int nTemp = 0;
		pClient->SendPacket((char*)&nTemp, sizeof(int));
		pClient->m_bReady = TRUE;
	}
}

//////////////////////////////////////////////////////////////////////

void CCrashUserServer::GetIPString(SOCKET Socket, char* IP)
{
	sockaddr_in addr;

	memset(&addr,0,sizeof(sockaddr));
	int len = sizeof(sockaddr);
	int nResult = getpeername (Socket,(sockaddr*)&addr,&len);	
	if(nResult == 0)
		memcpy( IP, inet_ntoa( addr.sin_addr ), 16 );
}

//////////////////////////////////////////////////////////////////////