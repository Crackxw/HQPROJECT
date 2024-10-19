// MySocket.cpp: implementation of the CMySocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerMonitor.h"
#include "MySocket.h"
#include "MyPacket.h"
#include "MyPacketQueue.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*DWORD WINAPI ConnectThread(LPVOID lpVoid)
{

	CMySocket *pSocket = (CMySocket *)lpVoid;
	
	while (1) {

		if (WaitForSingleObject(pSocket->GetExitEventHandle(),0) == WAIT_OBJECT_0) ExitThread(0);

		switch (pSocket->Connect())
		{
		case -1:
			ExitThread(0);
			break;
		case 0:
			break;
		case 1:
			ExitThread(1);
			break;
		}
	}
	
}

DWORD WINAPI AcceptThread(LPVOID lpVoid)
{

	CMySocket *pSocket = (CMySocket *)lpVoid;
	HANDLE hEvent = pSocket->GetEventHandle();

	DWORD dwTemp;
	CMySocket *pNewSocket;
	SOCKADDR_IN sock;
	
	while (1) {
		dwTemp = pSocket->Accept(&sock);

		if (dwTemp != -1) {

			pNewSocket = new CMySocket();
			pNewSocket->Init(dwTemp,&sock);
			pSocket->SetTemp((DWORD)pNewSocket);
			PulseEvent(hEvent);
			WaitForSingleObject(hEvent,INFINITE);
			
		}
	}
	
}

DWORD WINAPI SendNRecvThread(LPVOID lpVoid)
{

	CMySocket *pSocket = (CMySocket *)lpVoid;
	
	while (1) {

		if (WaitForSingleObject(pSocket->GetExitEventHandle(),0) == WAIT_OBJECT_0) ExitThread(0);

		switch (pSocket->Connect())
		{
		case -1:
			ExitThread(0);
			break;
		case 0:
			break;
		case 1:
			ExitThread(1);
			break;
		}
	}
	
}*/



CMySocket::CMySocket()
{
	m_bConnect = FALSE;
	m_bRecvSize = FALSE;

	

	InitializeCriticalSection( &m_crit );

	m_hEvent = WSACreateEvent();
	m_hThread = NULL;

	m_hExitEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

	Init();

	m_pPacketQueue = new CMyPacketQueue();

	
}

CMySocket::~CMySocket()
{

	Close();

	if (m_pPacketQueue) {
		delete m_pPacketQueue;
		m_pPacketQueue = NULL;
	}

	DeleteCriticalSection( &m_crit );


	CloseHandle(m_hExitEvent);

	WSACloseEvent(m_hEvent);


}

int CMySocket::Init()
{
	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_addr.s_addr = INADDR_ANY;

	m_socket = INVALID_SOCKET;

	ResetEvent(m_hExitEvent);

	ZeroMemory(m_szIP,MAX_IP);
	m_usPort = 0;

	ClearSendBuffer();
    ClearRecvBuffer();
	m_bRecvSize = FALSE;

	

	

	return 1;

}

int CMySocket::Init(SOCKET socket,SOCKADDR_IN *pAddr,BOOL bConnect)
{
	m_sockaddr = *pAddr;
	m_socket = socket;

	

	ResetEvent(m_hExitEvent);

	ZeroMemory(m_szIP,MAX_IP);
	GetAddressInDottedFormat(m_szIP);



	ClearSendBuffer();
    ClearRecvBuffer();

	m_bConnect = bConnect;
	m_bRecvSize = FALSE;

	

	

	return 1;

}


int CMySocket::SetAddress(const char *szIP,unsigned short usPort)
{
	m_usPort = usPort;
	m_sockaddr.sin_port = htons(usPort);

	strcpy(m_szIP,szIP);

	m_sockaddr.sin_addr.s_addr = inet_addr(m_szIP);     
	
	if ( m_sockaddr.sin_addr.s_addr == INADDR_NONE)
	{
		GetIPbyDomainName(szIP,m_szIP);

		m_sockaddr.sin_addr.s_addr = inet_addr(m_szIP);     

		if ( m_sockaddr.sin_addr.s_addr == INADDR_NONE)
		{
			return 0;
		}
	
	}

	return 1;	
}

char *CMySocket::GetAddressInDottedFormat(char *szBuf)
{
	sprintf(szBuf,"%u.%u.%u.%u",m_sockaddr.sin_addr.S_un.S_un_b.s_b1,m_sockaddr.sin_addr.S_un.S_un_b.s_b2,m_sockaddr.sin_addr.S_un.S_un_b.s_b3,m_sockaddr.sin_addr.S_un.S_un_b.s_b4);
	return szBuf;

}

int CMySocket::Bind()
{
	if( bind(m_socket,(struct sockaddr*)&m_sockaddr,sizeof(m_sockaddr)) == SOCKET_ERROR ) 
		return 0;
	return 1;

}

int CMySocket::Listen()	
{

	EnterCriticalSection( &m_crit );
	int ret = listen(m_socket,SOMAXCONN);
	LeaveCriticalSection(&m_crit);
		
	if (ret != 0) return 0;
	return 1;
	

}

int CMySocket::Connect()
{

	if (m_socket == INVALID_SOCKET) {
		Socket();
	}

	EnterCriticalSection( &m_crit );

	int ReturnVal;
	ReturnVal = connect(m_socket, (struct sockaddr*)&m_sockaddr, sizeof(m_sockaddr) );

	LeaveCriticalSection(&m_crit);
	if (ReturnVal != -1) m_bConnect = TRUE;
	
	return ReturnVal;
}

void CMySocket::Close()
{
	if (m_socket != INVALID_SOCKET) {
		WSAEventSelect(m_socket, m_hEvent, 0);
		closesocket(m_socket);
	}


	ClearSendBuffer();
	ClearRecvBuffer();

	if (m_pPacketQueue) m_pPacketQueue->Clear();
	
	
	m_bConnect = FALSE;
	
	m_socket = INVALID_SOCKET;
	m_bRecvSize = FALSE;

	
}

int CMySocket::Send()
{
	if (m_socket == INVALID_SOCKET) return 0;

	int siSendSize;
	int ret;



	

	EnterCriticalSection( &m_crit );

	
	if (m_siSendStart > m_siSendEnd || m_bSendFull)
	{
		siSendSize = MAX_SENDBUFFER - m_siSendStart;		
		ret = send(m_socket,&m_pscSendBuffer[m_siSendStart],siSendSize,0);   
		if (ret == SOCKET_ERROR)
		{
			LeaveCriticalSection(&m_crit);
			Close();
			return 0;
			
		}
		else
		{
			if (ret > 0)
			{
				m_siSendStart += ret;
				m_siSendStart %= MAX_SENDBUFFER;
				m_bSendFull = FALSE;
			}
			
		}
	}

	if (m_siSendStart < m_siSendEnd)
	{
		
		siSendSize = m_siSendEnd - m_siSendStart;		
		ret = send(m_socket,&m_pscSendBuffer[m_siSendStart],siSendSize,0);   
		if (ret == SOCKET_ERROR)
		{
			LeaveCriticalSection(&m_crit);
			Close();
			return 0;
			
		}
		else
		{
			if (ret > 0)
			{
				m_siSendStart += ret;
				m_siSendStart %= MAX_SENDBUFFER;
				m_bSendFull = FALSE;
			}
			
		}

	
	}



	LeaveCriticalSection( &m_crit );
	return 1;

}
int CMySocket::Recv()
{
	int siBufferSpace;

	if (m_socket == INVALID_SOCKET) return 0;

	
	if (m_bRecvFull)
	{
		siBufferSpace = 0;
	}
	else if ( m_siRecvStart <= m_siRecvEnd)
    {
		siBufferSpace = (MAX_RECVBUFFER - m_siRecvEnd);

	}
	else
	{
		siBufferSpace = (m_siRecvStart - m_siRecvEnd);
	}
	
	
	EnterCriticalSection( &m_crit );
	
	if (siBufferSpace) {
		int ret = recv(m_socket,&m_pscRecvBuffer[m_siRecvEnd],siBufferSpace,0);
	
		if (ret == SOCKET_ERROR)
		{
		LeaveCriticalSection(&m_crit);
		Close();
		return 0;
		
		}

		m_siRecvEnd += ret;
		m_siRecvEnd = (m_siRecvEnd % MAX_RECVBUFFER);
		if (m_siRecvEnd == m_siRecvStart) m_bRecvFull = TRUE;
		
	}

	int siRecv;

	while (siRecv = GetRecvSize())
	{

		// 아직 패킷크기를 받지않았으면
		if (!m_bRecvSize)		
		{
			// 2바이트 이상 받았을때
			if (siRecv >= 2)
			{
				// 패킷크기를 얻어온다. 
				GetRecvData((char *)&m_swByteToRecv,2);
				m_swByteToRecv -= 2;
				m_bRecvSize = TRUE;
			}
			// 패킷크기를 받지않았는데 2바이트 이하면 멈춤
			else
			{
				break;
			}

		}

		siRecv = GetRecvSize();

		// 패킷크기를 받았으면
		if (m_bRecvSize)
		{
			// 패킷크기 이상의 데이터를 받았으면 큐에추가
			if (siRecv >= m_swByteToRecv)
			{
				CMyPacket *pPacket = new CMyPacket();
				pPacket->SetLength(m_swByteToRecv);
				GetRecvData(&(pPacket->GetBuffer()[2]),m_swByteToRecv);
				m_pPacketQueue->Enqueue(pPacket);				
				m_bRecvSize = FALSE;				
				m_swByteToRecv = 0;

			}
			else
			{
				break;
			}

		}
		
	}


	
	
	
	
	LeaveCriticalSection( &m_crit );
	return 1;
	
}


int CMySocket::PutPacket(char *pscData,int siLen)
{
	if (m_socket == INVALID_SOCKET) return 0;
	if (m_bConnect == FALSE) return 0;
	if (m_bSendFull) return 0;
	EnterCriticalSection( &m_crit );

	if (siLen > GetSendBufferSpace() )
	{
		LeaveCriticalSection( &m_crit );
		return 0;
	}

	int len1 = MAX_SENDBUFFER - m_siSendEnd;

	if (siLen > len1)
	{
		memcpy(&m_pscSendBuffer[m_siSendEnd],pscData,len1);
		m_siSendEnd += len1;
		m_siSendEnd %= MAX_SENDBUFFER;
		pscData += len1;
		memcpy(&m_pscSendBuffer[m_siSendEnd],pscData,siLen - len1);
		m_siSendEnd += (siLen - len1);
		m_siSendEnd %= MAX_SENDBUFFER;
				
	}
	else
	{
		memcpy(&m_pscSendBuffer[m_siSendEnd],pscData,siLen);
		m_siSendEnd += siLen;
		m_siSendEnd %= MAX_SENDBUFFER;
		

	}

	if (m_siSendEnd == m_siSendStart ) m_bSendFull = TRUE;

	LeaveCriticalSection( &m_crit );
	return 1;

}

int CMySocket::PutPacket(CMyPacket *pPacket)
{
	return PutPacket(pPacket->GetBuffer(),pPacket->GetLength());

}

CMyPacket *CMySocket::PopPacket()
{
	EnterCriticalSection( &m_crit );

	CMyPacket *pPacket = m_pPacketQueue->Dequeue();

	




	LeaveCriticalSection( &m_crit );
	return pPacket;

}



void CMySocket::ClearRecvBuffer()
{
	EnterCriticalSection( &m_crit );

	m_swByteToRecv = 0;
	m_siRecvEnd = 0;
	m_siRecvStart = 0;
	m_bRecvFull = FALSE;

	LeaveCriticalSection( &m_crit );

}

void CMySocket::ClearSendBuffer()
{
	EnterCriticalSection( &m_crit );


	m_siSendEnd = 0;
	m_siSendStart = 0;
	m_bSendFull = FALSE;
	

	LeaveCriticalSection( &m_crit );

}



char *GetIPbyDomainName(const char *name,char *ip)
{
	struct hostent *p;
	struct in_addr *ptr;
	p = gethostbyname(name);
	if (p == NULL) return NULL;
	char **listptr=p->h_addr_list;
	ptr = (struct in_addr *)*listptr;
		strcpy(ip,inet_ntoa(*ptr));
		return ip;

}

char *GetMyIP(char* ip) 
{ 
        char buf[100]; 
        gethostname (buf, 100); 
        struct hostent *p; 
        p=gethostbyname(buf); 
        struct in_addr *ptr; 
        char **listptr=p->h_addr_list; 
        switch(p->h_addrtype) 
        { 
        case PF_INET: 
                while((ptr=(struct in_addr *) *listptr++) != NULL) 
                        strcpy(ip, inet_ntoa(*ptr)); 
                break; 
        } 
		return ip;
} 




void CMySocket::Socket()
{
	if (m_socket != INVALID_SOCKET) Close();
		
	m_socket = socket(AF_INET,SOCK_STREAM,IPPROTO_IP);
	

}

int CMySocket::SetSocket(SOCKET socket)
{
	if (m_socket != INVALID_SOCKET)
	{
		Close();
	}
	m_socket = socket;
	return 1;
}

int CMySocket::Accept(SOCKADDR_IN *pAddr)
{
	int iSize = sizeof(SOCKADDR_IN);

    int ret = accept(m_socket,(SOCKADDR *)pAddr,&iSize);
	return ret;
}

int CMySocket::Select()
{
	timeval	timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	fd_set fread,fwrite,ferror;

	FD_ZERO(&fread);
	

	FD_SET(m_socket,&fread);

	ferror = fwrite = fread;

	select(0,&fread,&fwrite,&ferror,&timeout);

	int ret = 0;
	if (FD_ISSET(m_socket,&fread)) ret += 1;
	if (FD_ISSET(m_socket,&fwrite)) ret += 2;
	if (FD_ISSET(m_socket,&ferror)) ret += 4;

	return ret;
}


int CMySocket::GetSendBufferSpace()
{
	if (m_bSendFull) return 0;

	int ret;

	if (m_siSendStart > m_siSendEnd)
	{
		return (m_siSendStart - m_siSendEnd);
	}
	else
	{
		ret = MAX_SENDBUFFER - m_siSendEnd + m_siSendStart;

		return ret;
	}

}

int CMySocket::GetRecvBufferSpace()
{
	if (m_bRecvFull) return 0;

	if (m_siRecvStart > m_siRecvEnd)
	{
		return (m_siRecvStart - m_siRecvEnd);
	}
	else
	{
		return (MAX_RECVBUFFER - m_siRecvEnd + m_siRecvStart);
	}
	

}

int CMySocket::GetRecvSize()
{
	if (m_bRecvFull)
	{
		return MAX_RECVBUFFER;
	}
	else if (m_siRecvEnd >= m_siRecvStart)
	{
		return (m_siRecvEnd - m_siRecvStart);
	}
	else
	{
		return (MAX_RECVBUFFER - m_siRecvStart + m_siRecvEnd);

	}

}


void CMySocket::GetRecvData(char *pData, int size)
{
	if (size <= 0) return;

    if (m_siRecvStart < m_siRecvEnd)
	{
		memcpy(pData,&m_pscRecvBuffer[m_siRecvStart],size);
	}
	else
	{
		int len = (MAX_RECVBUFFER - m_siRecvStart);
		if (size <= len)
		{
			memcpy(pData,&m_pscRecvBuffer[m_siRecvStart],size);			
		}
		else
		{
			memcpy(pData,&m_pscRecvBuffer[m_siRecvStart],len);
			pData += len;
			memcpy(pData,m_pscRecvBuffer,size - len);			
		}
	}

	m_siRecvStart += size;
	m_siRecvStart %= MAX_RECVBUFFER;
	m_bRecvFull = FALSE;

}

int CMySocket::GetSendSize()
{
	if (m_bSendFull)
	{
		return MAX_SENDBUFFER;
	}
	else if (m_siSendEnd >= m_siSendStart)
	{
		return (m_siSendEnd - m_siSendStart);
	}
	else
	{
		return (MAX_SENDBUFFER - m_siSendStart + m_siSendEnd);

	}

}


/*void CMySocket::GetSendData(char *pData, int size)
{
	if (size <= 0) return;

    if (m_siSendStart < m_siSendEnd)
	{
		memcpy(pData,&m_pscSendBuffer[m_siSendStart],size);
	}
	else
	{
		int len = (MAX_SENDBUFFER - m_siSendStart);
		if (size <= len)
		{
			memcpy(pData,&m_pscSendBuffer[m_siSendStart],size);			
		}
		else
		{
			memcpy(pData,&m_pscSendBuffer[m_siSendStart],len);
			pData += len;
			memcpy(pData,m_pscSendBuffer,size - len);			
		}
	}

	m_siSendStart += size;
	m_siSendStart %= MAX_SENDBUFFER;
	m_bSendFull = FALSE;

}*/

