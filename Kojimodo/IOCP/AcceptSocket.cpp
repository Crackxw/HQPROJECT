#include <winsock2.h>
#include <stdio.h>
#include "AcceptSocket.h"



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc :  생 성 자 
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

	// 송수신 버퍼 생성 
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


	// 송수신 큐 생성 
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


	// 크리티컬 섹션 생성 
	InitializeCriticalSection(&m_csRecv);
	InitializeCriticalSection(&m_csSend);

	
	initAcceptSocket();	
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 소 멸 자 
//--------------------------------------------------------------------------------------------
CAcceptSocket::~CAcceptSocket()
{
	// 송수신 버퍼 해제 
	if(m_recvBuffer != NULL) delete m_recvBuffer;	
	if(m_sendBuffer != NULL) delete m_sendBuffer;	
	

	// 송수신 큐 해제 
	if(m_recvQueue  != NULL) delete m_recvQueue;
	if(m_sendQueue  != NULL) delete m_sendQueue;


	// 크리티컬 섹션 해제 
	DeleteCriticalSection(&m_csRecv);
	DeleteCriticalSection(&m_csSend);
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : AcceptSocket를 연다 
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
//	Desc : AcceptSocket를 완전히 닫는다 
//--------------------------------------------------------------------------------------------
void CAcceptSocket::SockClose()
{
	_CallBack_SockClose();

	if( getSocket() && !isSockClosing() ) ::closesocket(getSocket());		

	initAcceptSocket();	
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : AcceptSocket을 닫는 작업을 시작 한다 
//--------------------------------------------------------------------------------------------
void CAcceptSocket::SockClosing()
{	
	if( getSocket() ) ::closesocket(getSocket());

	setSockClosing(TRUE);
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : AcceptSocket 관련 변수들을 모두 초기화 한다 
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
//	Desc : 소켓 정보를 초기화한다 
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
//	Desc : 송수신 버퍼를 초기화한다 
//--------------------------------------------------------------------------------------------
void CAcceptSocket::initializeRSBuffer()
{
	// 버퍼 초기화 
	memset(m_recvBuffer, 0, MAX_RECVBUFFER);	
	m_wsaRecvBuffer.buf = m_recvBuffer;
	m_wsaRecvBuffer.len = MAX_RECVBUFFER;	

	memset(m_sendBuffer, 0, MAX_SENDBUFFER);
	m_wsaSendBuffer.buf = m_sendBuffer;
	m_wsaSendBuffer.len = MAX_SENDBUFFER;			
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 송수신 큐를 초기화한다 
//--------------------------------------------------------------------------------------------
void CAcceptSocket::initializeRSQueue()
{
	// 큐 초기화 
	memset(m_recvQueue,  0, MAX_RECVQUEUE);
	memset(m_sendQueue,  0, MAX_SENDQUEUE);	
	m_recvQueuePos = 0;
	m_sendQueuePos = 0;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 오버랩 구조체를 초기화한다 
//--------------------------------------------------------------------------------------------
void CAcceptSocket::initializeRSOverlapped()
{
	// 오버랩 구조체 초기화 	
	memset(&m_recvOverlapped, 0, sizeof(m_recvOverlapped));
	m_recvOverlapped.transferMode = 0; 
	
	memset(&m_sendOverlapped, 0, sizeof(m_sendOverlapped));
	m_sendOverlapped.transferMode = 1;
}

		


//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 소켓으로부터의 데이타수신을 대기 한다 
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
//	Desc : 송신 큐에 있는 데이타를 일괄 송신한다 
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
//	Desc : 수신큐에 데이타를 넣는다 
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
//	Desc : 송신큐에 데이타를 넣는다 
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
//	Desc : 주기적으로 처리해야 할 소켓 작업을 수행 한다 
//--------------------------------------------------------------------------------------------
int CAcceptSocket::processAcceptSock()
{		
	if( isSockOpen() && isSockClosing() == FALSE )
	{
		
		if( processRecvQueue() == FALSE )  /* 수신 큐 처리 */
			return -1;		
	
		
		if( Send() < 0 ) /* 송신 처리 */					
			return -1;
		

		if( _CallBack_ProcessAcceptSock() == FALSE ) 
			return -1;
		
		return 1;
	}

	return 0;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 수신 큐에 있는 데이타를 패킷 단위로 처리 한다 
//--------------------------------------------------------------------------------------------
bool CAcceptSocket::processRecvQueue()
{
	EnterCriticalSection(&m_csRecv);

	while(m_recvQueuePos >= HEADER_SIZE)	
	{					 			
		int			   packetSize = getPacketSize(m_recvQueue);	  // 패킷크기 				
		int			   dataSize	  = packetSize  - HEADER_SIZE;	  // 데이타 크기 			
		char*		   data       = m_recvQueue + HEADER_SIZE;	  // 데이타의 시작 포인터 
	

		/* PACKET SIZE */
		if( packetSize > MAX_RECVBUFFER )
		{
			printf("Recv PacketSize Over Max!\n");
			
			LeaveCriticalSection(&m_csRecv);
			return FALSE;
		}


		if(m_recvQueuePos >= packetSize)
		{		

			// 체크섬 비교 
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
//	Desc : 소켓의 값을 설정한다 
//--------------------------------------------------------------------------------------------
void CAcceptSocket::setSocket(SOCKET socket)
{
	m_socket = socket;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 설정된 소켓의 값을 얻어온다 
//--------------------------------------------------------------------------------------------
SOCKET CAcceptSocket::getSocket()
{
	return m_socket;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 소켓 정보 구조체를 설정 한다 
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
//	Desc : 이 소켓의 정보 구조체를 얻어온다 
//--------------------------------------------------------------------------------------------
const SOCKADDR_IN* CAcceptSocket::getSockAddrIn() const
{
	return &m_sockAddrIn;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 이 소켓의 IP를 얻어온다 
//--------------------------------------------------------------------------------------------
const char*	CAcceptSocket::getSockIP() const
{
	return m_sockIP;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 이 소켓의 완료 키를 얻어온다  
//--------------------------------------------------------------------------------------------
DWORD CAcceptSocket::getSockCompletionKey()
{
	return m_sockCompletionKey;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 이 소켓의 순서 키를 설정한다 
//--------------------------------------------------------------------------------------------
void CAcceptSocket::setSockOrderKey(DWORD orderKey)
{
	m_sockOrderKey = orderKey;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 이 소켓의 순서 키를 얻어온다 
//--------------------------------------------------------------------------------------------
DWORD CAcceptSocket::getSockOrderKey()
{
	return m_sockOrderKey;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 이 소켓의 열림 상태를 설정한다 
//--------------------------------------------------------------------------------------------
void CAcceptSocket::setSockOpen(bool which)
{
	m_bSockOpen = which;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 이 소켓의 열림 상태를 얻어온다 
//--------------------------------------------------------------------------------------------
bool CAcceptSocket::isSockOpen()
{
	return m_bSockOpen;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 이 소켓의 '닫는작업중' 상태를 알아온다 
//--------------------------------------------------------------------------------------------
bool CAcceptSocket::isSockClosing()
{
	//========================================================================
	// Completion Key의 중복 삭제 위험 때문에 SockClose는 반드시 
	// IOCompletion Thread에서만 호출 되어야 한다 
	//========================================================================
	return m_bSockClosing;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 이 소켓의 '닫는작업중' 상태를 설정한다
//--------------------------------------------------------------------------------------------
void CAcceptSocket::setSockClosing(bool which)
{
	m_bSockClosing = which;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 패킷을 수신 받은 시간을 설정 한다 
//--------------------------------------------------------------------------------------------
void CAcceptSocket::setPacketRecvedTime(DWORD recvedTime)
{
	m_packetRecvedTime = recvedTime;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 최근에 패킷을 수신 받고 나서 경과한 시간을 얻어 온다 
//--------------------------------------------------------------------------------------------
DWORD CAcceptSocket::getPacketElapseTime()
{
	return (timeGetTime() - m_packetRecvedTime);
}