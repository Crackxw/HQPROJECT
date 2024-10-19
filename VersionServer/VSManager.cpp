// VSManager.cpp: implementation of the VSManager class.
//
//////////////////////////////////////////////////////////////////////

#include <GSL.h>
#include "VSDefine.h"
#include "VSManager.h"
#include <stdio.h>
#include <conio.h>
#include <assert.h>
#include <process.h>
#include "SimpleParser.h"

// #define VSERVER_VERSION "1.1"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VSManager::VSManager()
{
	InitializeCriticalSection(&m_AcceptCS);

//	SetWindowText(GetActiveWindow(), VSERVER_VERSION);

	// 윈속을 시작한다.
	StartWinsock();

	// 클라이언트를 만든다.
	pClients = new CClients[MAX_CONNECTION];
	assert(pClients);

	// IOCP를 준비한다.
	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, NULL);

	// 파일에서 데이터를 읽어온다.
	LoadDataFromScriptFile();
}

VSManager::~VSManager()
{
	DeleteCriticalSection(&m_AcceptCS);

	delete[] pClients;
	EndWinsock();
	printf("stop!\n");
}



//////////////////////////////////////////////////////////////////////////////
// 메인 루프....
//////////////////////////////////////////////////////////////////////////////
BOOL VSManager::Process()
{
	//////////////////////////////////////////////////////////////////////////////
	// 리슨소켓을 준비한다.
	//////////////////////////////////////////////////////////////////////////////
	InitSocket(TCP, NULL, PORT_NUMBER_VERSION_SERVER);
	Bind();
	Listen(50);

	StartWorker();
	StartAccept();

	//////////////////////////////////////////////////////////////////////////////
	// 'ESC'키가 입력될 때 까지 루프를 돈다.
	//////////////////////////////////////////////////////////////////////////////
	while(ConsoleProcess())
	{
//		AcceptClient();
	}


	//////////////////////////////////////////////////////////////////////////////
	// 리슨소켓을 해제한다.
	//////////////////////////////////////////////////////////////////////////////
	ReleaseSocket();

	// Accept 쓰레드가 종료할 때까지 기다린다.
	WaitForSingleObject(m_hAcceptThread, 10000);

	// 워커쓰레드를 종료한다.
	DWORD siCount = 0;
	for(siCount = 0; siCount < m_uiWorkerIndex; ++siCount)
	{
		XOverlapped ov;
		ov.Clear();
		ov.SetEnd();
		PostQueuedCompletionStatus(m_hIOCP, 4, siCount, (LPOVERLAPPED)&ov);
	}
	WaitForMultipleObjects(m_uiWorkerIndex, m_hWorkerThread, TRUE, 10000);
	for(siCount = 0; siCount < m_uiWorkerIndex; ++siCount)
	{
		CloseHandle(m_hWorkerThread[siCount]);
	}
	return true;
}

bool VSManager::ConsoleProcess()
{
	bNotExist = true;
	switch(toupper(_getch()))
	{
	case '\x1b':	// 'ESC'이면 종료.
		bNotExist = false;
		break;
	case 'P':
		{
			int i = 0, count = 0;
			for(i = 0; i<MAX_CONNECTION; ++i)
			{
				if(pClients[i].isConnect())
					++count;
			}
			printf("현재 접속자수는 %d명이다.\n", count);
		}
		break;
	case 'L':
		{
			// 파일에서 데이터를 읽어온다.
			LoadDataFromScriptFile();
		}
		break;
	default:
		break;
	}
	return bNotExist;
}



//////////////////////////////////////////////////////////////////////////////
// 워커쓰레드를 실행한다.
//////////////////////////////////////////////////////////////////////////////
void VSManager::StartWorker()
{
	SYSTEM_INFO	SystemInfo;
	GetSystemInfo(&SystemInfo);
	UINT uiCount = (SystemInfo.dwNumberOfProcessors)*2;

	// 생성할 쓰레드의 수를 최소 3이상으로 한다.
	if(uiCount < 3)
		uiCount = 3;
//	uiCount = 1;

	for(m_uiWorkerIndex=0; m_uiWorkerIndex<uiCount; m_uiWorkerIndex++)
	{
		m_hWorkerThread[m_uiWorkerIndex] = (HANDLE)::_beginthreadex( NULL, 0, VSManager::WorkerProc, this, 0, &m_uiWorkerID[m_uiWorkerIndex] );

		if( m_hWorkerThread[m_uiWorkerIndex] == 0 )
			printf("Work thread[%d] start Fail\n", m_uiWorkerIndex);
		else
			printf("Work Thread[%d] Start !\n", m_uiWorkerIndex);
	}
}



//////////////////////////////////////////////////////////////////////////////
// 쓰레드를 클래스에 포함시키기위한 함수... 
//////////////////////////////////////////////////////////////////////////////
inline unsigned __stdcall VSManager::WorkerProc(void* lpArg)
{
		VSManager* pVSWorker = NULL;
		pVSWorker=static_cast<VSManager *>(lpArg);
		pVSWorker->WorkerThread();
		return 0;
}


//////////////////////////////////////////////////////////////////////////////
// ACCEPT쓰레드를 실행한다.
//////////////////////////////////////////////////////////////////////////////
void VSManager::StartAccept()
{
	m_hAcceptThread = (HANDLE)::_beginthreadex( NULL, 0, VSManager::AcceptProc, this, 0, &m_uiAcceptThreadID);
	if( m_hAcceptThread == 0 )
		printf("Accept thread start Fail\n");
	else
		printf("Accept Thread Start !\n");
}



//////////////////////////////////////////////////////////////////////////////
// 쓰레드를 클래스에 포함시키기위한 함수... 
//////////////////////////////////////////////////////////////////////////////
inline unsigned __stdcall VSManager::AcceptProc(void* lpArg)
{
		VSManager* pVSAccept = NULL;
		pVSAccept = static_cast<VSManager *>(lpArg);
		pVSAccept->AcceptThread();
		return 0;
}

//////////////////////////////////////////////////////////////////////////////
// 클라이언트 처리.
//////////////////////////////////////////////////////////////////////////////
void VSManager::WorkerThread()
{
	XOverlapped *pOv = NULL;
	DWORD		dwTransBytes;
	CClients	*pClient;

	BOOL		bRet;
	while(true)
	{
		pClient = NULL;
		bRet = GetQueuedCompletionStatus(m_hIOCP, &dwTransBytes, (DWORD*)&pClient, (OVERLAPPED **)&pOv, INFINITE);

		if(!bRet || !pOv)
		{
			// 뭔가 잘못되었다.
			printf("잘못되었나?\n");
			CSocket::ViewErrMsg(WSAGetLastError());

			// 로그아웃 시키자.
			EnterCriticalSection(&m_AcceptCS);
			if(pClient != NULL)
			{
				pClient->Logout();
			}
			LeaveCriticalSection(&m_AcceptCS);
			continue;
		}

		if(pOv->IsRecv())
		{
			// 정보를 보낸다.
			if(pOv->m_cBuf[0] == '1')
				pClient->SendInformation1();
			else if(pOv->m_cBuf[0] == '2')
				pClient->SendInformation2();
			else if(pOv->m_cBuf[0] == '3')
				pClient->SendInformation3(&m_versionInf);

			pClient->Logout();
//			SendHeartBeat();
			CutAllConnection();
		}
		else if(pOv->IsSend())
		{
		}
		else if(pOv->IsAccept())
		{
		}
		else if(pOv->IsEnd())	// 쓰레드 종료.
		{
			break;	// while문 종료.
		}
		else
		{
		}
	} // while(true)

	printf("워커 쓰레드[%d] 종료\n", (int)pClient);
}



//////////////////////////////////////////////////////////////////////////////
// 빈 클라이언트 클래스를 얻어온다.
//////////////////////////////////////////////////////////////////////////////
int VSManager::GetEmptyClient()
{
	for(int i = 0; i < MAX_CONNECTION; i++)
	{
		if(!pClients[i].isConnect())
		{
			return i;
		}
	}
	return CLIENT_FULL;
}



//////////////////////////////////////////////////////////////////////////////
// 클라이언트의 접속을 받는다.
//////////////////////////////////////////////////////////////////////////////
void VSManager::AcceptThread()
{
	SOCKET	newsocket;
	int		clientIndex;

	while(true)
	{
		newsocket = Accept();
		if(newsocket == INVALID_SOCKET && bNotExist == FALSE)
			break;
		
		EnterCriticalSection(&m_AcceptCS);

		// 빈 클라이언트슬롯을 찾는다.
		clientIndex = GetEmptyClient();
		
		if(clientIndex != CLIENT_FULL)
		{
			printf("====================================================================\n");
			printf("로그인 Index[%d], Socket[%d], IP:%s\n", clientIndex, newsocket, inet_ntoa(m_sockaddr.sin_addr));

			// 클라이언트에 소켓을 할당한다.
			pClients[clientIndex].RegisterClient(newsocket, &m_sockaddr);

			LeaveCriticalSection(&m_AcceptCS);

			// 할당된 소켓을 IOCP에 등록한다.
			CreateIoCompletionPort((HANDLE)newsocket, m_hIOCP, (DWORD)&pClients[clientIndex], 0);
			
			// 리시브 요청
			pClients[clientIndex].RequestRecv();
		}
		else
		{
			LeaveCriticalSection(&m_AcceptCS);
			closesocket(newsocket);
		}
	}
	printf("Accept 쓰레드 종료\n");
}

void VSManager::LoadDataFromScriptFile()
{
	VServerParsing("VServer.txt", &m_versionInf);
	pParser = new ScriptParser;

	assert(pParser);
	delete pParser;

}

void VSManager::SendHeartBeat()
{
	for(int i = 0; i < MAX_CONNECTION; i++)
	{
		if(pClients[i].isConnect())
		{
			if(pClients[i].GetSocket() != INVALID_SOCKET)
			{
				if(pClients[i].Send("1", 1) == SOCKET_ERROR)
					pClients[i].Logout();
			}
			else
			{
				pClients[i].Logout();
			}
		}
	}
}

void VSManager::CutAllConnection()
{
	int count = 0;
	for(int i = 0; i < MAX_CONNECTION; i++)
	{
		if(pClients[i].isConnect())
			count++;
	}

	if(count >= 1024)
	{
		for(int i = 0; i < MAX_CONNECTION; i++)
			{
				pClients[i].Logout();
			}
	}
}

#define FTP_ADDRESS1 "FTP_ADDRESS1"
#define FTP_ID1 "FTP_ID1"
#define FTP_PASSWORD1 "FTP_PASSWORD1"
#define FTP_ADDRESS2 "FTP_ADDRESS2"
#define FTP_ID2 "FTP_ID2"
#define FTP_PASSWORD2 "FTP_PASSWORD2"
#define ADDRESS_FRONT_SERVER "ADDRESS_FRONT_SERVER"	// robypark 2004/10/5 16:26 프론트 서버 주소 토큰
#define VERSION_AUTOPATCH "VERSION_AUTOPATCH"
#define DIRECTORY_AUTOPATCH "DIRECTORY_PATCH_INFO"
#define VERSION_GERSANG "VERSION_GERSANG"
#define DIRECTORY_GERSANG "DIRECTORY_GERSANG_PATCH"
#define PARSE_STRING_LENGTH 100
	
bool VSManager::VServerParsing(char *pFileName, VersionInf *pVersion)
{
	int i;
	SimpleParser *pSimpleParser = new SimpleParser();

	memset(pVersion, 0, sizeof(VersionInf));
	printf("\n*************************************\n");
	printf("Help Message about (%s)\n\n", pFileName);
	pSimpleParser->AddStringSymbol(FTP_ADDRESS1, (char*)pVersion->ftpInf[0].myIP, sizeof(pVersion->ftpInf[0].myIP), true);
	pSimpleParser->AddStringSymbol(FTP_ID1, (char*)pVersion->ftpInf[0].myID, sizeof(pVersion->ftpInf[0].myID), true);
	pSimpleParser->AddStringSymbol(FTP_PASSWORD1, (char*)pVersion->ftpInf[0].myPW, sizeof(pVersion->ftpInf[0].myPW), true);
	pSimpleParser->AddStringSymbol(FTP_ADDRESS2, (char*)pVersion->ftpInf[1].myIP, sizeof(pVersion->ftpInf[1].myIP), true);
	pSimpleParser->AddStringSymbol(FTP_ID2, (char*)pVersion->ftpInf[1].myID, sizeof(pVersion->ftpInf[1].myID), true);
	pSimpleParser->AddStringSymbol(FTP_PASSWORD2, (char*)pVersion->ftpInf[1].myPW, sizeof(pVersion->ftpInf[1].myPW), true);

	// robypark 2004/10/5 16:27 프론트 서버 주소 토큰 추가
	pSimpleParser->AddStringSymbol(ADDRESS_FRONT_SERVER, (char*)pVersion->szFrontServerAddress, sizeof(BYTE) * MAX_HOSTLEN, true);

	pSimpleParser->AddIntSymbol(VERSION_AUTOPATCH, &pVersion->fileInf[AUTOPATCH_FILE].version, true);
	pSimpleParser->AddStringSymbol(DIRECTORY_AUTOPATCH, pVersion->fileInf[AUTOPATCH_FILE].patchPath, sizeof(pVersion->fileInf[AUTOPATCH_FILE].patchPath), true);
	pSimpleParser->AddIntSymbol(VERSION_GERSANG, &pVersion->fileInf[GERSANG_FILE].version, true);
	pSimpleParser->AddStringSymbol(DIRECTORY_GERSANG, pVersion->fileInf[GERSANG_FILE].patchPath, sizeof(pVersion->fileInf[GERSANG_FILE].patchPath), true);
	printf("*************************************\n\n");

	pSimpleParser->Do(pFileName);
	pSimpleParser->OutSymbol();

	delete pSimpleParser;

	for(i = 0; i < MAX_FTP_NUMBER; i++)
	{
		if(pVersion->ftpInf[i].myIP[0] == 0) break;
	}

	pVersion->ftpInfRealNumber = i;

	return(true);
}
