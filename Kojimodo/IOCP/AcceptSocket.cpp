#include <winsock2.h>
#include <stdio.h>
#include "AcceptSocket.h"



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc :  �� �� �� 
//--------------------------------------------------------------------------------------------
CAcceptSocket::CAcceptSocket( DWORD completion_key,
							  int header_size,
							  int recvbuffer_size, 
							  int sendbuffer_size, 
							  int recvqueue_size,
							  int sendqueue_size ):

m_sockCompletionKey( completion_key  ),
HEADER_SIZE		   ( header_size     ),
MAX_RECVBUFFER	   ( recvbuffer_size ),
MAX_SENDBUFFER	   ( sendbuffer_size ), 
MAX_RECVQUEUE	   ( recvqueue_size  ),
MAX_SENDQUEUE	   ( sendqueue_size  )

{

	// �ۼ��� ���� ���� 
	m_recvBuffer = new char[MAX_RECVBUFFER];
	if(m_recvBuffer == NULL)
	{
		printf("Not enough memory!\n");
	}

	m_sendBuffer = new char[MAX_SENDBUFFER];
	if(m_sendBuffer == NULL)
	{
		printf("Not enough memory!\n");
	}


	// �ۼ��� ť ���� 
	m_recvQueue = new char[MAX_RECVQUEUE];
	if(m_recvQueue == NULL)
	{
		printf("Not enough memory!\n");
	}

	m_sendQueue = new char[MAX_SENDQUEUE];
	if(m_sendQueue == NULL)
	{
		printf("Not enough memory!\n");
	}		


	// ũ��Ƽ�� ���� ���� 
	InitializeCriticalSection(&m_csRecv);
	InitializeCriticalSection(&m_csSend);

	
	initAcceptSocket();	
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �� �� �� 
//--------------------------------------------------------------------------------------------
CAcceptSocket::~CAcceptSocket()
{
	// �ۼ��� ���� ���� 
	if(m_recvBuffer != NULL) delete m_recvBuffer;	
	if(m_sendBuffer != NULL) delete m_sendBuffer;	
	

	// �ۼ��� ť ���� 
	if(m_recvQueue  != NULL) delete m_recvQueue;
	if(m_sendQueue  != NULL) delete m_sendQueue;


	// ũ��Ƽ�� ���� ���� 
	DeleteCriticalSection(&m_csRecv);
	DeleteCriticalSection(&m_csSend);
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : AcceptSocket�� ���� 
//--------------------------------------------------------------------------------------------
void CAcceptSocket::SockOpen(SOCKET socket, SOCKADDR_IN *sockAddrIn, DWORD orderKey)
{
	setSocket(socket);
	setSockAddrIn(sockAddrIn);
	setSockOrderKey(orderKey);	

	setSockOpen(TRUE);

	_CallBack_SockOpen();
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : AcceptSocket�� ������ �ݴ´� 
//--------------------------------------------------------------------------------------------
void CAcceptSocket::SockClose()
{
	_CallBack_SockClose();

	if( getSocket() && !isSockClosing() ) ::closesocket(getSocket());		

	initAcceptSocket();	
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : AcceptSocket�� �ݴ� �۾��� ���� �Ѵ� 
//--------------------------------------------------------------------------------------------
void CAcceptSocket::SockClosing()
{	
	if( getSocket() ) ::closesocket(getSocket());

	setSockClosing(TRUE);
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : AcceptSocket ���� �������� ��� �ʱ�ȭ �Ѵ� 
//--------------------------------------------------------------------------------------------
void CAcceptSocket::initAcceptSocket()
{	
	initializeSocketInfo();	
	initializeRSBuffer();
	initializeRSQueue();
	initializeRSOverlapped();
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� ������ �ʱ�ȭ�Ѵ� 
//--------------------------------------------------------------------------------------------
void CAcceptSocket::initializeSocketInfo()
{			
	setSocket(0);	
	setSockAddrIn(0);	
	setSockOrderKey(0);		
	setPacketRecvedTime(0);	

	setSockClosing(FALSE);
	setSockOpen(FALSE);
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �ۼ��� ���۸� �ʱ�ȭ�Ѵ� 
//--------------------------------------------------------------------------------------------
void CAcceptSocket::initializeRSBuffer()
{
	// ���� �ʱ�ȭ 
	memset(m_recvBuffer, 0, MAX_RECVBUFFER);	
	m_wsaRecvBuffer.buf = m_recvBuffer;
	m_wsaRecvBuffer.len = MAX_RECVBUFFER;	

	memset(m_sendBuffer, 0, MAX_SENDBUFFER);
	m_wsaSendBuffer.buf = m_sendBuffer;
	m_wsaSendBuffer.len = MAX_SENDBUFFER;			
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �ۼ��� ť�� �ʱ�ȭ�Ѵ� 
//--------------------------------------------------------------------------------------------
void CAcceptSocket::initializeRSQueue()
{
	// ť �ʱ�ȭ 
	memset(m_recvQueue,  0, MAX_RECVQUEUE);
	memset(m_sendQueue,  0, MAX_SENDQUEUE);	
	m_recvQueuePos = 0;
	m_sendQueuePos = 0;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ������ ����ü�� �ʱ�ȭ�Ѵ� 
//--------------------------------------------------------------------------------------------
void CAcceptSocket::initializeRSOverlapped()
{
	// ������ ����ü �ʱ�ȭ 	
	memset(&m_recvOverlapped, 0, sizeof(m_recvOverlapped));
	m_recvOverlapped.transferMode = 0; 
	
	memset(&m_sendOverlapped, 0, sizeof(m_sendOverlapped));
	m_sendOverlapped.transferMode = 1;
}

		


//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �������κ����� ����Ÿ������ ��� �Ѵ� 
//--------------------------------------------------------------------------------------------
int CAcceptSocket::Recv(int numberOfBytesTransferred)
{	
	if( numberOfBytesTransferred > 0 ) {
		if( writeToRecvQueue(NULL, numberOfBytesTransferred) < 0 ) return -1;
	}

	setPacketRecvedTime(timeGetTime());


	DWORD	flags = 0;
	DWORD	recvedSize = 0;
	if(SOCKET_ERROR == WSARecv( m_socket, &m_wsaRecvBuffer, 1, 
							    &recvedSize, &flags, (OVERLAPPED*)&m_recvOverlapped, NULL))			
	{
		if(WSAGetLastError()!=WSA_IO_PENDING)						
		{ 			
			printf("WSARecv() fail!\n");			
			return -1;
		}					
	}

	return 0;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �۽� ť�� �ִ� ����Ÿ�� �ϰ� �۽��Ѵ� 
//--------------------------------------------------------------------------------------------
int CAcceptSocket::Send()
{
	EnterCriticalSection(&m_csSend);

	if(m_sendQueuePos > 0)
	{
		int size = 0;		
		if( SOCKET_ERROR == (size = send(m_socket, m_sendQueue, m_sendQueuePos, 0)) )
		{
			if(WSAGetLastError() != WSAEWOULDBLOCK)
			{					
				printf("send() fail!\n");

				LeaveCriticalSection(&m_csSend);
				return -1;
			}
		}
		else
		{		
			m_sendQueuePos -= size;

			if( m_sendQueuePos > 0 )
			{
				memmove(m_sendQueue, m_sendQueue+size, m_sendQueuePos);	
			}

			LeaveCriticalSection(&m_csSend);
			return size;
		}	
	}

	LeaveCriticalSection(&m_csSend);
	return 0;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ����ť�� ����Ÿ�� �ִ´� 
//--------------------------------------------------------------------------------------------
int CAcceptSocket::writeToRecvQueue(char* data, int size)
{
	if(data == NULL && size > 0) data = m_recvBuffer;


	if(size > 0)
	{
		EnterCriticalSection(&m_csRecv);
		if(m_recvQueuePos + size <= MAX_RECVQUEUE)	
		{		 				
			memcpy(m_recvQueue + m_recvQueuePos, data, size);
			m_recvQueuePos += size;		
			
			LeaveCriticalSection(&m_csRecv);
			return size;
		}
		else
		{	
			printf("RecvQueue Overflow!\n");	
			
			LeaveCriticalSection(&m_csRecv);
			return -1;
		}	
	}

	return 0;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �۽�ť�� ����Ÿ�� �ִ´� 
//--------------------------------------------------------------------------------------------
int CAcceptSocket::writeToSendQueue(char* header, char* data, int dataSize)
{
	if( header != NULL && data != NULL && dataSize > 0)
	{
		EnterCriticalSection(&m_csSend);

		int packetSize = HEADER_SIZE + dataSize;
		
		/* PACKET SIZE */
		if( packetSize > MAX_SENDBUFFER )
		{
			printf("Send PacketSize Over Max!\n");
			
			LeaveCriticalSection(&m_csSend);
			return -1;
		}


		if(m_sendQueuePos + packetSize <= MAX_SENDQUEUE)	
		{		 			
			
			/* HEADER COPY */
			memcpy(m_sendQueue + m_sendQueuePos, header, HEADER_SIZE);
			m_sendQueuePos += HEADER_SIZE;


			/* DATA COPY */
			memcpy(m_sendQueue + m_sendQueuePos, data, dataSize);
			m_sendQueuePos += dataSize;							


			LeaveCriticalSection(&m_csSend);
			
			return packetSize;
		}
		else
		{	
			printf("SendQueue Overflow!\n");	
			printf("%s\n",m_sockIP);
			//writeInfoToFile("ConnectStatus.txt",m_sockIP);
			FILE *fp;

			fp = fopen("sinbal.txt","at");

			fprintf(fp,"SendQueue Overflow!\n");
			fprintf(fp,"%s\n",m_sockIP);

			fclose(fp);

			LeaveCriticalSection(&m_csSend);
			return -1;
		}	
	}

	return 0;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �ֱ������� ó���ؾ� �� ���� �۾��� ���� �Ѵ� 
//--------------------------------------------------------------------------------------------
int CAcceptSocket::processAcceptSock()
{		
	if( isSockOpen() && isSockClosing() == FALSE )
	{
		
		if( processRecvQueue() == FALSE )  /* ���� ť ó�� */
			return -1;		
	
		
		if( Send() < 0 ) /* �۽� ó�� */					
			return -1;
		

		if( _CallBack_ProcessAcceptSock() == FALSE ) 
			return -1;
		
		return 1;
	}

	return 0;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� ť�� �ִ� ����Ÿ�� ��Ŷ ������ ó�� �Ѵ� 
//--------------------------------------------------------------------------------------------
bool CAcceptSocket::processRecvQueue()
{
	EnterCriticalSection(&m_csRecv);

	while(m_recvQueuePos >= HEADER_SIZE)	
	{					 			
		int			   packetSize = getPacketSize(m_recvQueue);	  // ��Ŷũ�� 				
		int			   dataSize	  = packetSize  - HEADER_SIZE;	  // ����Ÿ ũ�� 			
		char*		   data       = m_recvQueue + HEADER_SIZE;	  // ����Ÿ�� ���� ������ 
	

		/* PACKET SIZE */
		if( packetSize > MAX_RECVBUFFER )
		{
			printf("Recv PacketSize Over Max!\n");
			
			LeaveCriticalSection(&m_csRecv);
			return FALSE;
		}


		if(m_recvQueuePos >= packetSize)
		{		

			// üũ�� �� 
			if( getChecksum(m_recvQueue) == calcChecksum(data, dataSize) ) 
			{						
				processPacket(m_recvQueue, data, dataSize);				
			}
			else
			{
				char str[200];
				wsprintf(str, "[%s:%d] checksum error!\n", getSockIP(), getSocket());
				OutputDebugString(str);
				
				LeaveCriticalSection(&m_csRecv);				
				return FALSE;
			}


			m_recvQueuePos -= packetSize;					
			
			if(m_recvQueuePos > 0)
			memmove(m_recvQueue, m_recvQueue+packetSize, m_recvQueuePos);							
		}
		else 
			break;
	}		

	LeaveCriticalSection(&m_csRecv);
	return TRUE;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ������ ���� �����Ѵ� 
//--------------------------------------------------------------------------------------------
void CAcceptSocket::setSocket(SOCKET socket)
{
	m_socket = socket;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ������ ������ ���� ���´� 
//--------------------------------------------------------------------------------------------
SOCKET CAcceptSocket::getSocket()
{
	return m_socket;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� ���� ����ü�� ���� �Ѵ� 
//--------------------------------------------------------------------------------------------
void CAcceptSocket::setSockAddrIn(SOCKADDR_IN* sockAddrIn)
{
	memset (&m_sockAddrIn, 0, sizeof(SOCKADDR_IN));
	memset (m_sockIP, 0, sizeof(m_sockIP));

	if(sockAddrIn != NULL)
	{
		memcpy(&m_sockAddrIn, sockAddrIn, sizeof(SOCKADDR_IN));		
		strncpy(m_sockIP, inet_ntoa(m_sockAddrIn.sin_addr), sizeof(m_sockIP));	
	}
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �� ������ ���� ����ü�� ���´� 
//--------------------------------------------------------------------------------------------
const SOCKADDR_IN* CAcceptSocket::getSockAddrIn() const
{
	return &m_sockAddrIn;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �� ������ IP�� ���´� 
//--------------------------------------------------------------------------------------------
const char*	CAcceptSocket::getSockIP() const
{
	return m_sockIP;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �� ������ �Ϸ� Ű�� ���´�  
//--------------------------------------------------------------------------------------------
DWORD CAcceptSocket::getSockCompletionKey()
{
	return m_sockCompletionKey;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �� ������ ���� Ű�� �����Ѵ� 
//--------------------------------------------------------------------------------------------
void CAcceptSocket::setSockOrderKey(DWORD orderKey)
{
	m_sockOrderKey = orderKey;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �� ������ ���� Ű�� ���´� 
//--------------------------------------------------------------------------------------------
DWORD CAcceptSocket::getSockOrderKey()
{
	return m_sockOrderKey;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �� ������ ���� ���¸� �����Ѵ� 
//--------------------------------------------------------------------------------------------
void CAcceptSocket::setSockOpen(bool which)
{
	m_bSockOpen = which;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �� ������ ���� ���¸� ���´� 
//--------------------------------------------------------------------------------------------
bool CAcceptSocket::isSockOpen()
{
	return m_bSockOpen;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �� ������ '�ݴ��۾���' ���¸� �˾ƿ´� 
//--------------------------------------------------------------------------------------------
bool CAcceptSocket::isSockClosing()
{
	//========================================================================
	// Completion Key�� �ߺ� ���� ���� ������ SockClose�� �ݵ�� 
	// IOCompletion Thread������ ȣ�� �Ǿ�� �Ѵ� 
	//========================================================================
	return m_bSockClosing;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �� ������ '�ݴ��۾���' ���¸� �����Ѵ�
//--------------------------------------------------------------------------------------------
void CAcceptSocket::setSockClosing(bool which)
{
	m_bSockClosing = which;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ��Ŷ�� ���� ���� �ð��� ���� �Ѵ� 
//--------------------------------------------------------------------------------------------
void CAcceptSocket::setPacketRecvedTime(DWORD recvedTime)
{
	m_packetRecvedTime = recvedTime;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �ֱٿ� ��Ŷ�� ���� �ް� ���� ����� �ð��� ��� �´� 
//--------------------------------------------------------------------------------------------
DWORD CAcceptSocket::getPacketElapseTime()
{
	return (timeGetTime() - m_packetRecvedTime);
}