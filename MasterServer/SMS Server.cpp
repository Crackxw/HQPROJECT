
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: SMS Server.h.cpp

Author: 이민수 <nshangukin@joyon.com>
Currently Managed by: 이민수

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


//////////////////////////////////////////////////////////////////////

#include "SMS Server.h"
#include <assert.h>
#include "XClient.h"


#include "..\\XCommon\\JXConnectSocket.h"	
#include "..\\XCommon\\JXGateSocket.h"	
#include "..\\XCommon\\JXSocketMgr.h"	
#include "..\\XCommon\\JXSocket.h"
#include "..\\XCommon\\JXPacketQueue.h"	
#include "..\\XCommon\\JXAcceptSocketList.h"

#include "..\\ServerMonitor\\MonitorProtocol.h"
#include "XClientMgr.h"
#include "XServerMgr.h"

//////////////////////////////////////////////////////////////////////

CSMSServer::CSMSServer(XServerMgr* pServerMgr,XClientMgr* pClientMgr) : 
m_pClientToRemove(NULL), 
m_siNumClientToRemove(0),
m_bProcessThreadRun(FALSE), 
m_hProcessExitEvent(NULL), 
m_hProcessBreakEvent(NULL), 
m_wListenPort(0), 
m_pServerMgr(pServerMgr),
m_pMoniterClientMgr(pClientMgr)
{
	::InitializeCriticalSection(&m_csLock);
	
	m_hProcessBreakEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hProcessExitEvent  = CreateEvent(NULL, FALSE, FALSE, NULL);

	m_pGateSocketClient		= new JXGateSocket;
	m_pAcceptListClient		= new JXAcceptSocketList;
	m_pConnectListClient	= new JXSocketMgr;
	m_pClientMgr			= new XClientMgr;	
}

//////////////////////////////////////////////////////////////////////

CSMSServer::~CSMSServer()
{
	delete m_pGateSocketClient;
	delete m_pAcceptListClient;
	delete m_pConnectListClient;
	delete m_pClientMgr;
	delete[] m_pClientToRemove;	

	::DeleteCriticalSection(&m_csLock);
}

//////////////////////////////////////////////////////////////////////

unsigned int CSMSServer::Run_Process(void* vpThis)
{
	unsigned int uiRetVal = 1;

	try{
		uiRetVal = reinterpret_cast<CSMSServer*>(vpThis)->Process();
		return uiRetVal;
	}
	catch(...){
		assert(0);
		return uiRetVal;
	}
}

//////////////////////////////////////////////////////////////////////

unsigned int CSMSServer::Process()
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

void CSMSServer::StartThread()
{
	StopThread();

	HANDLE hThread = NULL;

	hThread = (HANDLE)_beginthreadex(NULL, 0, Run_Process, 
		reinterpret_cast<CSMSServer*>(this), 0, &m_uiProcessThreadID);

	if(hThread != NULL)
	{
		m_bProcessThreadRun = TRUE;
		CloseHandle(hThread);
		printf("[Port : %d] SMS server process thread start!!\n", m_wListenPort);
	}
	else
	{
		printf("[Port : %d] SMS server process thread could not start. because thread create failed. [Error code : %d]\n", m_wListenPort, GetLastError());
	}
}

//////////////////////////////////////////////////////////////////////

void CSMSServer::StopThread()
{
	if(m_bProcessThreadRun == FALSE) return;

	SetEvent(m_hProcessBreakEvent);
	m_bProcessThreadRun = FALSE;

	printf("[Port : %d]process thread ending...!!\n", m_wListenPort);

	WaitForSingleObject(m_hProcessExitEvent, INFINITE);

	printf("[Port : %d] SMS server process thread end...!!\n", m_wListenPort);
}

//////////////////////////////////////////////////////////////////////

void CSMSServer::Create()
{
	int iMaxClient = 10;
	m_pClientToRemove = new XClient*[iMaxClient];
	m_siNumClientToRemove = 0;

	m_wListenPort = SMS_SERVER_PORT;

	m_pAcceptListClient->Create( iMaxClient, 
		SMS_SERVER_RECV_BUF_SIZE, 
		SMS_SERVER_SEND_BUF_SIZE, 
		SMS_SERVER_QUEUE_SIZE);
	
	m_pGateSocketClient->Create( SMS_SERVER_PORT, m_pAcceptListClient );
	m_pClientMgr->Create( iMaxClient);	
}

//////////////////////////////////////////////////////////////////////

void CSMSServer::Start()
{
	m_pGateSocketClient->StartThread();
	m_pConnectListClient->StartThread();

	m_bThreadRun = TRUE;
}

//////////////////////////////////////////////////////////////////////

void CSMSServer::ProcessNetwork()
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

		printf("[SMS] AcceptList -> ConnectList : Socket Addr[ %x ]\n", pSocket );		
		
		m_pClientMgr->AddClient( pSocket, TRUE );
		
		FD_SET( pSocket->m_socket, &m_pConnectListClient->m_fdset );
	
	END_JXLIST_LOOP

	m_pConnectListClient->PushFront( m_pAcceptListClient );

	// 접속 끊기

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
		
		if(m_pClientMgr->GetNumberNodes() > 1)
		{
			if(i < m_pClientMgr->GetNumberNodes()-1)
			{
				m_pClientToRemove[m_siNumClientToRemove++] = pClient;				
			}
		}
		
		if(m_pClientMgr->GetNumberNodes()-1 == i)
			ProcPacketClient(pClient);

	END_JXLIST_LOOP
}

//////////////////////////////////////////////////////////////////////

void CSMSServer::SendSMSMsgResult(SMS_RESP_MSG* pSMS)
{	
	int				 i = 0;
	UI16 cmd = DELIVER_RESP_MSG;
	m_LocalPacketA.ClearPacket();
	m_LocalPacketA.WriteValue( &cmd, 2 );
	m_LocalPacketA.WriteValue( pSMS, sizeof(SMS_RESP_MSG));					
	m_LocalPacketA.CompletePacket();

	m_pClientMgr->SendPacket(&m_LocalPacketA);
}

//////////////////////////////////////////////////////////////////////

void CSMSServer::ProcPacketClient(XClient* pClient)
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


		m_dwLocalCmd = ntohs(m_dwLocalCmd);

		switch(m_dwLocalCmd) 
		{
			case DELIVER_REQ_MSG : 
				{
					SMS_REQ_MSG sSMS;
					memset(&sSMS, 0, sizeof(SMS_REQ_MSG));

					m_LocalPacket.ReadValue(&sSMS, sizeof(SMS_REQ_MSG));

					sSMS.dwCallCount = ntohs(sSMS.dwCallCount);

					UI16 cmd = SS_SMS_REQ_MSG;
					m_LocalPacketA.ClearPacket();
					m_LocalPacketA.WriteValue( &cmd, 2 );
					m_LocalPacketA.WriteValue( &sSMS, sizeof(SMS_REQ_MSG));					
					m_LocalPacketA.CompletePacket();

					//Monitor Server 에 패킷을 보낸다.
					m_pServerMgr->SendPacket(&m_LocalPacketA);
					printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
					printf("SMS Message receive[%s]!\n", sSMS.szCallMessage);



					
					MonitorSmsReq	stSmsReq;
					sprintf(stSmsReq.szOrgPhone,"%s",sSMS.szOrgPhone);
					sprintf(stSmsReq.szCallMessage,"%s",sSMS.szCallMessage);

					cmd = MMC_REPORT_SMS;
					m_LocalPacketA.ClearPacket();
					m_LocalPacketA.WriteValue( &cmd, 2 );
					m_LocalPacketA.WriteValue( &stSmsReq, sizeof(MonitorSmsReq));					
					m_LocalPacketA.CompletePacket();

					m_pMoniterClientMgr->SendPacket(&m_LocalPacketA);
					

					/////////////////////////////////////////////////////////
					// SMS Server에 결과 전송
					SMS_RESP_MSG sSMSResp;
					memset(&sSMSResp, 0, sizeof(SMS_RESP_MSG));
					sSMSResp.dwCallCount = htons(sSMS.dwCallCount);
					sSMSResp.lResult = htonl(0);
					strncpy(sSMSResp.szUserCode, sSMS.szUserCode, 32);

					UI16 usCmd = DELIVER_RESP_MSG;		
					m_LocalPacketA.ClearPacket();

					usCmd = htons(usCmd);

					m_LocalPacketA.WriteValue(&usCmd,2);
					m_LocalPacketA.WriteValue(&sSMSResp,sizeof(SMS_RESP_MSG));
					m_LocalPacketA.CompletePacket();

					pClient->GetSocket()->SendPacket(&m_LocalPacketA);

					printf("SMS result message send!\n");
					printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
					/////////////////////////////////////////////////////////
				}
				break;
			default : 
				{
					m_LogWriter.PrintF("SMS Server Error.Log", 
						"[SMS SERVER] Unknown message type : %d", 
						m_dwLocalCmd);

					pClient->GetSocket()->Disconnect();
				}
				break;
		}
	}
}

//////////////////////////////////////////////////////////////////////

void CSMSServer::SendConfirmMessage(XClient* pClient)
{
//	if(pClient->m_bReady == FALSE)
//	{	
//		int nTemp = 0;
//		pClient->SendPacket((char*)&nTemp, sizeof(int));
//		pClient->m_bReady = TRUE;
//	}
}

//////////////////////////////////////////////////////////////////////

void CSMSServer::GetIPString(SOCKET Socket, char* IP)
{
	sockaddr_in addr;

	memset(&addr,0,sizeof(sockaddr));
	int len = sizeof(sockaddr);
	int nResult = getpeername (Socket,(sockaddr*)&addr,&len);	
	if(nResult == 0)
		memcpy( IP, inet_ntoa( addr.sin_addr ), 16 );
}

//////////////////////////////////////////////////////////////////////