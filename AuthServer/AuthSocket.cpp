
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: AuthSocket.h

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

//////////////////////////////////////////////////////////////////////

#include "AuthSocket.h"

//////////////////////////////////////////////////////////////////////

CAuthSocket::CAuthSocket() : 
m_Socket(INVALID_SOCKET),
m_bConnect(FALSE), 
m_dwSendTickCount(0), 
m_dwReponseTimeout(DEFAULT_TIMEOUT_VALUE),
m_bNextSendAble(TRUE), 
m_dwNextSendAbleTime(0), 
m_dwSendAbleChangeTime(DEFAULT_SENDABLE_TIME_CHANGE),
m_pParentCallBack(NULL)
{
	::InitializeCriticalSection(&m_csLock);
}

//////////////////////////////////////////////////////////////////////

CAuthSocket::~CAuthSocket()
{
	Enter();	
	DisConnect(TRUE);
	Leave();
	
	::DeleteCriticalSection(&m_csLock);
}

//////////////////////////////////////////////////////////////////////

void CAuthSocket::SetParent(CParentCallback* pParent)
{
	m_pParentCallBack = pParent;
}

//////////////////////////////////////////////////////////////////////

BOOL CAuthSocket::IsConnect()
{
	Enter();

	BOOL bRetVal = m_bConnect;

	Leave();

	return bRetVal;
}

//////////////////////////////////////////////////////////////////////

void CAuthSocket::DisConnect(BOOL bEnd /* = FALSE*/)
{
	Enter();
	if(m_bConnect == FALSE)
	{
		Leave();
		return;
	}

	printf("Disconnect[%d]!!\n", m_Socket);

	closesocket(m_Socket);
	m_bConnect = FALSE;

	m_bNextSendAble = TRUE;

//	if(m_pParentCallBack != NULL && bEnd == FALSE)
//	{		
//		m_pParentCallBack->CallbackDisConnect();
//	}

	Leave();
}

//////////////////////////////////////////////////////////////////////

void CAuthSocket::SetSocket(SOCKET Socket, SOCKADDR_IN SockAddr)
{
	Enter();
	m_Socket		= Socket;
	m_SocketAddr	= SockAddr;

	m_bConnect		= TRUE;
	Leave();
}

//////////////////////////////////////////////////////////////////////

SOCKET CAuthSocket::GetSocket() const
{
	return m_Socket;
}

//////////////////////////////////////////////////////////////////////

void CAuthSocket::SetTimeout(DWORD dwMilliSecond)
{
	Enter();
	m_dwReponseTimeout = dwMilliSecond;
	m_dwSendAbleChangeTime = dwMilliSecond*2;
	Leave();
}

//////////////////////////////////////////////////////////////////////

BOOL CAuthSocket::IsTimeout() const
{
	return (m_dwReponseTimeout > (GetTickCount()-m_dwSendTickCount)) ? FALSE : TRUE;
}

//////////////////////////////////////////////////////////////////////

void CAuthSocket::SendAbleChangeCheckAndChange()
{
	Enter();
	if(m_bNextSendAble == FALSE)
	{
		if(m_dwSendAbleChangeTime <= (GetTickCount() - m_dwNextSendAbleTime))
		{
			SetNextSendAble();
		}
	}
	Leave();
}

//////////////////////////////////////////////////////////////////////

BOOL CAuthSocket::GetNextSendAble() const
{
	return m_bNextSendAble;
}

//////////////////////////////////////////////////////////////////////

void CAuthSocket::SetNextSendAble()
{
	Enter();
	m_bNextSendAble = TRUE;
	m_dwNextSendAbleTime = 0;
	Leave();
}

//////////////////////////////////////////////////////////////////////

void CAuthSocket::UnSetNextSendAble()
{
	Enter();
	m_bNextSendAble = FALSE;
	m_dwNextSendAbleTime = GetTickCount();
	Leave();
}

//////////////////////////////////////////////////////////////////////

BOOL CAuthSocket::IsSendAble() const
{
	timeval	timeout;

	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	fd_set setfds, errfds;
	FD_ZERO(&setfds);
	FD_SET(m_Socket, &setfds);
	FD_ZERO(&errfds);
	FD_SET(m_Socket, &errfds);

	int nRetVal = select(m_Socket, 0, &setfds, &errfds, &timeout);

	if(nRetVal != SOCKET_ERROR)
	{
		if(FD_ISSET(m_Socket, &setfds) && !FD_ISSET(m_Socket, &errfds))
			return TRUE;
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////

BYTE CAuthSocket::IsRecvAble() const
{
	timeval	timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	fd_set setfds, errfds;
	FD_ZERO(&setfds);
	FD_SET(m_Socket, &setfds);
	FD_ZERO(&errfds);
	FD_SET(m_Socket, &errfds);

	int nRetVal = select(m_Socket, &setfds, 0, &errfds, &timeout);

	if(nRetVal != SOCKET_ERROR)
	{
		if(FD_ISSET(m_Socket, &setfds) && !FD_ISSET(m_Socket, &errfds))
			return 1;
	}
	else
	{
		return 2;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////

BOOL CAuthSocket::Send(const char* pPacket, int nSize)
{
	Enter();
	if(m_bConnect == FALSE || m_Socket == INVALID_SOCKET || nSize <= 0)
	{
		Leave();
		return FALSE;
	}

	if(IsSendAble() == FALSE)
	{
		Leave();
		return FALSE;
	}

	int nSendBytes = 0;
	int nRemainSize = nSize;
	int nSendPos = 0;
	DWORD dwSendTime = 0;

	dwSendTime = GetTickCount();
	while(1)
	{
		if(GetTickCount() - dwSendTime >= m_dwReponseTimeout)
		{
			Leave();
			return FALSE;
		}
		
		nSendBytes = ::send(m_Socket, (const char*)(pPacket+nSendPos), nRemainSize, 0);

		if(nSendBytes == SOCKET_ERROR)
		{
			if(GetLastError() != WSAEWOULDBLOCK)
			{
				Leave();
				return FALSE;
			}
		}

		if(nSendBytes < nRemainSize)
		{
			nSendPos += nSendBytes;
			nRemainSize -= nSendBytes;
		}
		else
		{
			m_dwSendTickCount = GetTickCount();
			Leave();
			return TRUE;
		}
	}

	Leave();
	return FALSE;
}

//////////////////////////////////////////////////////////////////////

BYTE CAuthSocket::Recv(char* pPacket, int nSize)
{	
	if(m_bConnect == FALSE || m_Socket == INVALID_SOCKET || nSize <= 0)
	{
		return GASH_SERVER_UNKNOWN_FAIL;
	}

	Enter();
	int nRecvSize = 0;
	BYTE byRetVal = 0;

	while(1)
	{
		if(IsTimeout() == TRUE)
		{			
			m_LogWriter.PrintF("Reponse Time out.Log", "Time out happen : %d", 
				GetTickCount() - m_dwSendTickCount);

			Leave();
			return GASH_SERVER_REPONSE_TIMEOUT;
		}

		byRetVal = IsRecvAble();

		if(byRetVal == 0)
		{
			Leave();
			return GASH_SERVER_RECV_READY_ENABLE;
		}
		else if(byRetVal == 2)
		{
			m_LogWriter.PrintF("SocketError.Log", "Gash Server connection socket error");
			Leave();
			return GASH_SERVER_UNKNOWN_FAIL;
		}

		nRecvSize = ::recv(m_Socket, pPacket, nSize, 0);
		if(nRecvSize == SOCKET_ERROR)
		{
			if(GetLastError() != WSAEWOULDBLOCK)
			{
				m_LogWriter.PrintF("Receive error[Gash Server].Log", 
					"Receive error!![Error code : %d]", GetLastError());

				DisConnect();
				Leave();
				return GASH_SERVER_UNKNOWN_FAIL;
			}
		}

		if(nRecvSize > 0)
		{
			if(nRecvSize > nSize )
			{
				Leave();
				return GASH_SERVER_UNKNOWN_FAIL;
			}

			printf("The time required : %d ms\n", GetTickCount()-m_dwSendTickCount);
			Leave();
			return GASH_SERVER_REPONSE_OK;
		}
	}

	Leave();
	return GASH_SERVER_UNKNOWN_FAIL;
}
