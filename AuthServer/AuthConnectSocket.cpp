

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: AuthConnectSocket.cpp

Author: 이민수 <nshangukin@joyon.com>
Currently Managed by: 이민수

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

//////////////////////////////////////////////////////////////////////

#include "AuthConnectSocket.h"
#include <assert.h>
#include <stdio.h>
#include <process.h>

//////////////////////////////////////////////////////////////////////

CAuthConnectSocket::CAuthConnectSocket() :
CAuthSocket(), 
m_wPort(0), 
m_bConnectThreadRun(FALSE), 
m_uiConnectThreadID(0),
m_hExitConnectThread(NULL)
{
	::InitializeCriticalSection(&m_csQueueLock);
	m_szIP[0] = 0;
	
	m_hExitConnectThread	= CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hEvent[0]				= CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hEvent[1]				= WSACreateEvent();
}

//////////////////////////////////////////////////////////////////////

CAuthConnectSocket::~CAuthConnectSocket()
{
	if(m_hExitConnectThread != NULL)
		CloseHandle(m_hExitConnectThread);
	if(m_hEvent[0] != NULL)
		CloseHandle(m_hEvent[0]);
	if(m_hEvent[1] != NULL)
		CloseHandle(m_hEvent[1]);

	::DeleteCriticalSection(&m_csQueueLock);
}

//////////////////////////////////////////////////////////////////////

unsigned int CAuthConnectSocket::Run_Connect_Thread(void* vpThis)
{
	unsigned int uiRetVal = 1;

	try{
		uiRetVal = reinterpret_cast<CAuthConnectSocket*>(vpThis)->ConnectThread();
		return uiRetVal;
	}
	catch(...){
		assert(0);
		return uiRetVal;
	}
}

//////////////////////////////////////////////////////////////////////

unsigned int CAuthConnectSocket::ConnectThread()
{
	DWORD dwRetVal;	
	
	WSANETWORKEVENTS Networks;

	while(TRUE)
	{
		dwRetVal = WSAWaitForMultipleEvents(2, m_hEvent, FALSE, 50, FALSE);
		
		if(dwRetVal == WSA_WAIT_EVENT_0)
		{
			break;
		}

		/////////////////////////////////////////////
		// Gash Server가 close 한 소켓 처리.
		if(dwRetVal == WSA_WAIT_EVENT_0+1)
		{
			WSAEnumNetworkEvents(m_Socket, m_hEvent[1], &Networks);
			if(Networks.lNetworkEvents & FD_CLOSE)
			{
				printf("[Socket close by Gash server][Port : %d]\n", m_wPort);

				CAuthSocket::DisConnect();
			}
		}
		/////////////////////////////////////////////

		Connect();
	}

	SetEvent(m_hExitConnectThread);
	printf("Gash Server Connect thread end...IP : %s, Port : %d\n", m_szIP, m_wPort);

	return 0;
}

//////////////////////////////////////////////////////////////////////

void CAuthConnectSocket::StopConnectThread()
{
	if(m_bConnectThreadRun == FALSE) return;

	SetEvent(m_hEvent[0]);
	m_bConnectThreadRun = FALSE;

	WaitForSingleObject(m_hExitConnectThread, INFINITE);
}

//////////////////////////////////////////////////////////////////////

void CAuthConnectSocket::StartConnectThread()
{	
	StopConnectThread();

	HANDLE hThread = NULL;

	m_bConnectThreadRun = TRUE;
	hThread = (HANDLE)_beginthreadex(NULL, 0, Run_Connect_Thread, 
		reinterpret_cast<CAuthConnectSocket*>(this), 0, &m_uiConnectThreadID);

	if(hThread != NULL)
	{
		m_bConnectThreadRun = TRUE;
		CloseHandle(hThread);

		printf("Gash Server Connect thread start. [IP : %s] [Port : %d]\n", m_szIP, m_wPort);
	}
	else
	{
		printf("Gash Server Connect thread could not start. because thread create failed. [IP : %s] [Port : %d] [Error Code : %d]\n", m_szIP, m_wPort, GetLastError());
	}
}

//////////////////////////////////////////////////////////////////////

void CAuthConnectSocket::Create(char* pIP, WORD wPort)
{
	memset(&m_SocketAddr, 0, sizeof(m_SocketAddr));

	m_SocketAddr.sin_family = AF_INET;
	m_SocketAddr.sin_addr.s_addr = inet_addr(pIP);
	m_SocketAddr.sin_port = htons(wPort);

	strncpy(m_szIP, pIP, 16);
	m_wPort = wPort;
}

//////////////////////////////////////////////////////////////////////

BOOL CAuthConnectSocket::Connect()
{	
	if(m_szIP[0] == 0 || m_wPort == 0)
		return FALSE;

	if(m_bConnect == TRUE)
	{
		return FALSE;
	}

	SOCKET Socket = socket(AF_INET, SOCK_STREAM, 0);

	if(Socket == INVALID_SOCKET)
	{
		return FALSE;
	}

	int nRet = connect(Socket, (SOCKADDR*)&m_SocketAddr, sizeof(m_SocketAddr));
	
	if(nRet == SOCKET_ERROR)
	{
		closesocket(Socket);
		return FALSE;
	}

	WSAEventSelect(Socket, m_hEvent[1], FD_CLOSE);
	
	printf("Gash server connect succeed!! [IP : %s][Port : %d]\n", m_szIP, m_wPort);
	SetSocket(Socket, m_SocketAddr);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////

char* CAuthConnectSocket::SetPacket(char* pPacket, void* pData, int nSize)
{
	assert(pPacket != NULL && nSize > 0);

	memcpy(pPacket, (char*)pData, nSize);

	pPacket += nSize;

	return pPacket;
}

//////////////////////////////////////////////////////////////////////

