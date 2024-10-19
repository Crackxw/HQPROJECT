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

	// ������ �����Ѵ�.
	StartWinsock();

	// Ŭ���̾�Ʈ�� �����.
	pClients = new CClients[MAX_CONNECTION];
	assert(pClients);

	// IOCP�� �غ��Ѵ�.
	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, NULL);

	// ���Ͽ��� �����͸� �о�´�.
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
// ���� ����....
//////////////////////////////////////////////////////////////////////////////
BOOL VSManager::Process()
{
	//////////////////////////////////////////////////////////////////////////////
	// ���������� �غ��Ѵ�.
	//////////////////////////////////////////////////////////////////////////////
	InitSocket(TCP, NULL, PORT_NUMBER_VERSION_SERVER);
	Bind();
	Listen(50);

	StartWorker();
	StartAccept();

	//////////////////////////////////////////////////////////////////////////////
	// 'ESC'Ű�� �Էµ� �� ���� ������ ����.
	//////////////////////////////////////////////////////////////////////////////
	while(ConsoleProcess())
	{
//		AcceptClient();
	}


	//////////////////////////////////////////////////////////////////////////////
	// ���������� �����Ѵ�.
	//////////////////////////////////////////////////////////////////////////////
	ReleaseSocket();

	// Accept �����尡 ������ ������ ��ٸ���.
	WaitForSingleObject(m_hAcceptThread, 10000);

	// ��Ŀ�����带 �����Ѵ�.
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
	case '\x1b':	// 'ESC'�̸� ����.
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
			printf("���� �����ڼ��� %d���̴�.\n", count);
		}
		break;
	case 'L':
		{
			// ���Ͽ��� �����͸� �о�´�.
			LoadDataFromScriptFile();
		}
		break;
	default:
		break;
	}
	return bNotExist;
}



//////////////////////////////////////////////////////////////////////////////
// ��Ŀ�����带 �����Ѵ�.
//////////////////////////////////////////////////////////////////////////////
void VSManager::StartWorker()
{
	SYSTEM_INFO	SystemInfo;
	GetSystemInfo(&SystemInfo);
	UINT uiCount = (SystemInfo.dwNumberOfProcessors)*2;

	// ������ �������� ���� �ּ� 3�̻����� �Ѵ�.
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
// �����带 Ŭ������ ���Խ�Ű������ �Լ�... 
//////////////////////////////////////////////////////////////////////////////
inline unsigned __stdcall VSManager::WorkerProc(void* lpArg)
{
		VSManager* pVSWorker = NULL;
		pVSWorker=static_cast<VSManager *>(lpArg);
		pVSWorker->WorkerThread();
		return 0;
}


//////////////////////////////////////////////////////////////////////////////
// ACCEPT�����带 �����Ѵ�.
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
// �����带 Ŭ������ ���Խ�Ű������ �Լ�... 
//////////////////////////////////////////////////////////////////////////////
inline unsigned __stdcall VSManager::AcceptProc(void* lpArg)
{
		VSManager* pVSAccept = NULL;
		pVSAccept = static_cast<VSManager *>(lpArg);
		pVSAccept->AcceptThread();
		return 0;
}

//////////////////////////////////////////////////////////////////////////////
// Ŭ���̾�Ʈ ó��.
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
			// ���� �߸��Ǿ���.
			printf("�߸��Ǿ���?\n");
			CSocket::ViewErrMsg(WSAGetLastError());

			// �α׾ƿ� ��Ű��.
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
			// ������ ������.
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
		else if(pOv->IsEnd())	// ������ ����.
		{
			break;	// while�� ����.
		}
		else
		{
		}
	} // while(true)

	printf("��Ŀ ������[%d] ����\n", (int)pClient);
}



//////////////////////////////////////////////////////////////////////////////
// �� Ŭ���̾�Ʈ Ŭ������ ���´�.
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
// Ŭ���̾�Ʈ�� ������ �޴´�.
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

		// �� Ŭ���̾�Ʈ������ ã�´�.
		clientIndex = GetEmptyClient();
		
		if(clientIndex != CLIENT_FULL)
		{
			printf("====================================================================\n");
			printf("�α��� Index[%d], Socket[%d], IP:%s\n", clientIndex, newsocket, inet_ntoa(m_sockaddr.sin_addr));

			// Ŭ���̾�Ʈ�� ������ �Ҵ��Ѵ�.
			pClients[clientIndex].RegisterClient(newsocket, &m_sockaddr);

			LeaveCriticalSection(&m_AcceptCS);

			// �Ҵ�� ������ IOCP�� ����Ѵ�.
			CreateIoCompletionPort((HANDLE)newsocket, m_hIOCP, (DWORD)&pClients[clientIndex], 0);
			
			// ���ú� ��û
			pClients[clientIndex].RequestRecv();
		}
		else
		{
			LeaveCriticalSection(&m_AcceptCS);
			closesocket(newsocket);
		}
	}
	printf("Accept ������ ����\n");
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
#define ADDRESS_FRONT_SERVER "ADDRESS_FRONT_SERVER"	// robypark 2004/10/5 16:26 ����Ʈ ���� �ּ� ��ū
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

	// robypark 2004/10/5 16:27 ����Ʈ ���� �ּ� ��ū �߰�
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
