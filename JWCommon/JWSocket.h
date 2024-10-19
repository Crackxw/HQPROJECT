#ifndef _JWSOCKET_H
#define _JWSOCKET_H

#include "JWObject.h"

#define RECV_POSTED			1
#define SEND_POSTED			2

class JWSocket;
class JWRecvBuffer;
class JWSendBuffer;

struct PER_IO_DATA 
{
	OVERLAPPED	overlapped;
	DWORD		dwOperationType;
	DWORD		dwIoSize;
	DWORD		dwFlags;
};

class JWSocket : public JWObject
{
public:
	JWSocket();
	virtual ~JWSocket();

public:
	void				CreateSocket( SI32 siRecvBufSize, SI32 siSendBufSize ); 

	BOOL				SetSocket( SOCKET socket, SOCKADDR_IN sockaddr );
	BOOL				CloseSocket();

	JWRecvBuffer*		GetRecvBuffer() { return m_pRecvBuffer; };
	JWSendBuffer*		GetSendBuffer() { return m_pSendBuffer; };	
	
	SOCKET				GetSocket() { return m_socket; }

public:
	PER_IO_DATA			m_sendIoData;						// IOCP
	PER_IO_DATA			m_recvIoData;

public:
	SOCKET				m_socket;
	SOCKADDR_IN			m_sockaddr;

	JWRecvBuffer		*m_pRecvBuffer;
	JWSendBuffer		*m_pSendBuffer;
};

#endif

