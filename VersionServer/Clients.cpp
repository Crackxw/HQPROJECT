// Clients.cpp: implementation of the CClients class.
//
//////////////////////////////////////////////////////////////////////
#include <GSL.h>
#include "Clients.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClients::CClients()
{
	InitializeCriticalSection(&m_cs);

	m_pOv = new XOverlapped;
	assert(m_pOv);

	SetStatus(DISCONNECT);
}

CClients::~CClients()
{
	DeleteCriticalSection(&m_cs);
	delete m_pOv;
}

void CClients::RegisterClient(SOCKET newsocket, SOCKADDR_IN *sockaddr)
{
	SetSocket(newsocket);
	SetSockaddr(sockaddr);
	SetStatus(CONNECT);
}

XOverlapped *CClients::GetOverlapped()
{
	return m_pOv;
}

void CClients::RequestRecv()
{
	// Recv요청을한다.
	DWORD Flags = 0;
	DWORD RecvBytes;

	// 오버랩구조체 초기화.
	m_pOv->Clear();

	// 플레그 변경.
	m_pOv->SetRecv();

	// 리시브 요청
	int ret = WSARecv(GetSocket(), &m_pOv->m_wsaBuf, 1, &RecvBytes, &Flags, (LPWSAOVERLAPPED)m_pOv, NULL);
	if(ret == SOCKET_ERROR)
	{
		ViewErrMsg(WSAGetLastError());
	}
}

void CClients::Logout()
{
	printf("로그아웃 Socket[%d], IP:%s\n", GetSocket(), inet_ntoa(m_sockaddr.sin_addr));
	ReleaseSocket();
	SetSocket(INVALID_SOCKET);
	SetStatus(DISCONNECT);
}

void CClients::SendInformation1()
{
	char cBuf[1024] = "";
	int siIP = rand() % g_stIp.ipcount;
	int sendbytes;

	g_stVer.ip[0] = g_stIp.ip[siIP][0];
	g_stVer.ip[1] = g_stIp.ip[siIP][1];
	g_stVer.ip[2] = g_stIp.ip[siIP][2];
	g_stVer.ip[3] = g_stIp.ip[siIP][3];

	printf("IP : g_stIp.ipcount = [%d], siIP = [%d]\n", g_stIp.ipcount, siIP);
/*	for(int i=0; i < g_stVer.vercount; i++)
	{
		printf("Version [%d] : %d\n", i, g_stVer.ver[i]);
	}
*/
	do
	{
		sendbytes = send(GetSocket(), (char*)&g_stVer, sizeof(g_stVer), 0);
		if(sendbytes == SOCKET_ERROR)
		{
			ViewErrMsg(WSAGetLastError());
			Logout();
			break;
		}
	}while(sendbytes < sizeof(g_stVer));
}

void CClients::SendInformation2()
{
	char cBuf[1024] = "";
	int siIP = rand() % g_stIp2.ipcount;
	int sendbytes;

	memcpy(g_stVer2.MyIP, g_stIp2.ip[siIP], MAX_HOSTLEN);
	memcpy(g_stVer2.MyID, g_stIp2.MyID[siIP], MAX_IDLEN);
	memcpy(g_stVer2.MyPW, g_stIp2.MyPW[siIP], MAX_PWLEN);

	printf("IP : g_stIp2.ipcount = [%d], siIP = [%d]\n", g_stIp2.ipcount, siIP);

	do
	{
		sendbytes = send(GetSocket(), (char*)&g_stVer2, sizeof(g_stVer2), 0);
		if(sendbytes == SOCKET_ERROR)
		{
			ViewErrMsg(WSAGetLastError());
			Logout();
			break;
		}
	}while(sendbytes < sizeof(g_stVer2));
}
/*
g_stVer3
typedef struct {
	FTP_Inf ftpInf;
	FileInf fileInf[MAX_FILE];
} Version3;

g_stVer3.
*/

void CClients::SendInformation3(VersionInf *pVersion)
{
	char cBuf[1024] = "";
	int siIP = rand() % pVersion->ftpInfRealNumber;
	int sendbytes;

	memcpy(g_stVer3.fileInf, pVersion->fileInf, sizeof(pVersion->fileInf));
	memcpy(&g_stVer3.ftpInf, &(pVersion->ftpInf[siIP]), sizeof(FTP_Inf));

	// robypark 2004/10/5 16:34
	// 프론트 서버 주소 정보도 거상 런처에게 전달하도록 추가
	memcpy(g_stVer3.szFrontServerAddress, pVersion->szFrontServerAddress, sizeof(BYTE) * MAX_HOSTLEN);

/*
	memcpy(g_stVer2.MyIP, g_stIp2.ip[siIP], MAX_HOSTLEN);
	memcpy(g_stVer2.MyID, g_stIp2.MyID[siIP], MAX_IDLEN);
	memcpy(g_stVer2.MyPW, g_stIp2.MyPW[siIP], MAX_PWLEN);
*/
	printf("IP : g_stIp3.ipcount = [%d], siIP = [%d]\n", g_stIp2.ipcount, siIP);

	do
	{
		sendbytes = send(GetSocket(), (char*)&g_stVer3, sizeof(g_stVer3), 0);
		if(sendbytes == SOCKET_ERROR)
		{
			ViewErrMsg(WSAGetLastError());
			Logout();
			break;
		}
	}while(sendbytes < sizeof(g_stVer3));
}

void CClients::VerLock()
{
	EnterCriticalSection(&m_cs);
}

void CClients::VerUnlock()
{
	LeaveCriticalSection(&m_cs);
}

bool CClients::isConnect()
{
	if(GetStatus() == CONNECT)
	{
		return true;
	}
	else
	{
		return false;
	}
}
