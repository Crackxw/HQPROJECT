
#include "GashServer.h"
#include <assert.h>
#include "XClient.h"

CGashServer::CGashServer() : m_bProcessThreadRun(FALSE), 
m_hProcessExitEvent(NULL), 
m_hProcessBreakEvent(NULL), 
m_wListenPort(0)
{
	::InitializeCriticalSection(&m_csLock);

	memset(m_szOutputString, 0, sizeof(m_szOutputString));
	
	m_hProcessBreakEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hProcessExitEvent  = CreateEvent(NULL, FALSE, FALSE, NULL);

	m_pGateSocketClient		= new JXGateSocket;
	m_pAcceptListClient		= new JXAcceptSocketList;
	m_pConnectListClient	= new JXSocketMgr;
	m_pClientMgr			= new XClientMgr;
}

CGashServer::~CGashServer()
{
	delete m_pGateSocketClient;
	delete m_pAcceptListClient;
	delete m_pConnectListClient;
	delete m_pClientMgr;
	delete[] m_pClientToRemove;

	::DeleteCriticalSection(&m_csLock);
}

unsigned int CGashServer::Run_Process(void* vpThis)
{
	unsigned int uiRetVal = 1;

	try{
		uiRetVal = reinterpret_cast<CGashServer*>(vpThis)->Process();
		return uiRetVal;
	}
	catch(...){
		assert(0);
		return uiRetVal;
	}
}

unsigned int CGashServer::Process()
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

void CGashServer::StartThread()
{
	StopThread();

	HANDLE hThread = NULL;

	hThread = (HANDLE)_beginthreadex(NULL, 0, Run_Process, 
		reinterpret_cast<CGashServer*>(this), 0, &m_uiProcessThreadID);

	if(hThread != NULL)
	{
		m_bProcessThreadRun = TRUE;
		CloseHandle(hThread);
		printf("[Port : %d] Gash server process thread start!!\n", m_wListenPort);
	}
	else
	{
		printf("[Port : %d] Gash server process thread could not start. because thread create failed. [Error code : %d]\n", m_wListenPort, GetLastError());
	}
}

//////////////////////////////////////////////////////////////////////

void CGashServer::StopThread()
{
	if(m_bProcessThreadRun == FALSE) return;

	SetEvent(m_hProcessBreakEvent);
	m_bProcessThreadRun = FALSE;

	WaitForSingleObject(m_hProcessExitEvent, INFINITE);

	printf("[Port : %d] Gash server process thread end...!!\n", m_wListenPort);
}

void CGashServer::Create(WORD wPort, char szOutputString[256])
{
	int iMaxClient = 100;
	m_pClientToRemove = new XClient*[iMaxClient];
	m_siNumClientToRemove = 0;

	m_wListenPort = wPort;
	strncpy(m_szOutputString, szOutputString, 256);

	m_pAcceptListClient->Create( iMaxClient, 1000, 1000, 1000);
	m_pGateSocketClient->Create( wPort, m_pAcceptListClient );
	m_pClientMgr->Create( iMaxClient );
}

void CGashServer::Start()
{
	m_pGateSocketClient->StartThread();
	m_pConnectListClient->StartThread();

	m_bThreadRun = TRUE;
}

void CGashServer::ProcessNetwork()
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

		m_pConnectListClient->Remove( (JXObject *)(pClient->GetSocket()) );
		
		m_pClientMgr->DelClient( pClient );
	}
	m_siNumClientToRemove = 0;
	 
	m_pConnectListClient->Release();
	m_pAcceptListClient->Release();

	// 클라이언트 프로세스
	BEGIN_JXLIST_LOOP( i, m_pClientMgr, XClient, pClient ) 
		ProcPacketClient(pClient);
	END_JXLIST_LOOP
}


// 클라이언트 패킷 처리..
void CGashServer::ProcPacketClient(XClient* pClient)
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

	int nSendBytes = 0;
	int nRemainSize = 0;
	int nSendPos = 0;
	DWORD dwSendTime = 0;
	JXPacketQueue *pPacketQueue = NULL;
	JXPacket Packet;

	pPacketQueue = pClient->GetSocket()->GetQueue();

    // 큐에 있는 패킷 개수 얻어옴
	UI32 uiNumPacket = pPacketQueue->GetNumPacket();

	char szPacket[512] = {0, };	
	int  nOutputStringLen = strlen(m_szOutputString)+1;

	short snResult = 1;
	memcpy(szPacket, &snResult, sizeof(short));
	memcpy(szPacket+sizeof(short), m_szOutputString, nOutputStringLen);
	nRemainSize = nOutputStringLen+2;

    // 패킷 개수 만큼 반복
	for( UI32 i = 0; i < uiNumPacket; ++i )
	{
		Enter();
		while(1)
		{
			nSendBytes = ::send(Socket, (const char*)(szPacket+nSendPos), nRemainSize, 0);

			if(nSendBytes == SOCKET_ERROR)
			{
				if(GetLastError() != WSAEWOULDBLOCK)
				{
					printf("[Port : %d] Send failed!! [Error code : %d]\n", m_wListenPort, GetLastError());
					break;
				}
			}

			if(nSendBytes < nRemainSize)
			{
				nSendPos += nSendBytes;
				nRemainSize -= nSendBytes;
			}
			else
			{
				printf("[Port : %d] Send Ok!!\n", m_wListenPort);
				break;
			}
		}

		Leave();	
	}
}




