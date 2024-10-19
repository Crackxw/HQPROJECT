// MySocket.h: interface for the CMySocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSOCKET_H__65F8CD89_C02B_4D28_908C_9DBBEEDDCFB9__INCLUDED_)
#define AFX_MYSOCKET_H__65F8CD89_C02B_4D28_908C_9DBBEEDDCFB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_SENDBUFFER  8192
#define MAX_RECVBUFFER  8192

#define MAX_IP          256

class CMyPacket;



enum SOCKET_STATE
{
	SOCKET_NOT_CONNECTED = 0,
	SOCKET_TRY_CONNECT,
	SOCKET_CONNECTED
	
};

class CMyPacketQueue;


class CMySocket  
{
public:
	void GetRecvData(char *,int size);	
	int GetRecvSize();

	
	int GetSendSize();
	
	int GetSendBufferSpace();
	int GetRecvBufferSpace();

	void Socket();
	CMySocket();
	virtual ~CMySocket();



	int Init();
	int Init(SOCKET socket,SOCKADDR_IN *pAddr,BOOL bConnect);

	int SetSocket(SOCKET socket);

	int SetAddress(const char *szIP,unsigned short usPort);
	char *GetAddress() {return m_szIP;};
	char *GetAddressInDottedFormat(char *);

	int Bind();
	int Listen();
	int Accept(SOCKADDR_IN *pAddr);

	int Connect();

	int Send();
	int Recv();

	int Select();

	
	void ClearSendBuffer();
	void ClearRecvBuffer();

	int PutPacket(char *pscData,int siLen);
	int PutPacket(CMyPacket *pPacket);

	CMyPacket *PopPacket();

	void SetTemp(DWORD pTemp) {m_dwTemp = pTemp;};
	DWORD GetTemp() {return m_dwTemp;};

	void Close();

	HANDLE  GetEventHandle() {return m_hEvent;};
	HANDLE  GetExitEventHandle() {return m_hExitEvent;};
	HANDLE  GetThreadHandle() {return m_hThread;};

	BOOL    IsConnected() {return m_bConnect;};


	

	HANDLE      m_hThread;
	DWORD       m_dwThreadID;


	
private:

	SOCKET			m_socket;
	SOCKADDR_IN		m_sockaddr;

	HANDLE		m_hEvent;
	HANDLE      m_hExitEvent;

	BOOL        m_bConnect;
	BOOL        m_bRecvSize;
	

	CRITICAL_SECTION m_crit;


	unsigned short  m_usPort;
	char m_szIP[MAX_IP];


	
	
	int  m_siSendEnd;
	int  m_siSendStart;
	BOOL m_bSendFull;


	WORD  m_swByteToRecv;
	int  m_siRecvEnd;
	int  m_siRecvStart;
	BOOL m_bRecvFull;

	int  m_dwTemp;


	

	char m_pscSendBuffer[MAX_SENDBUFFER];
	char m_pscRecvBuffer[MAX_RECVBUFFER];

	CMyPacketQueue *m_pPacketQueue;
};

char *GetIPbyDomainName(const char *name,char *ip);
char *GetMyIP(char *ip);

	



#endif // !defined(AFX_MYSOCKET_H__65F8CD89_C02B_4D28_908C_9DBBEEDDCFB9__INCLUDED_)
