//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 6. 20	
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#include "JXPacket.h"	
#include "JXSocket.h"	
#include "JXPacketQueue.h"	



JXSocket::JXSocket()	
{
	m_socket = INVALID_SOCKET;
	m_bConnect = FALSE;
	m_bOdering = FALSE;

	m_pRecvBuf = NULL;
	m_pSendBuf = NULL;

	m_pPacketQueue = NULL;

	InitializeCriticalSection( &m_cs );
}

JXSocket::~JXSocket()		
{
	EnterCriticalSection( &m_cs );

	__disconnect();

	if( m_pRecvBuf ) delete [] m_pRecvBuf;
	if( m_pSendBuf ) delete [] m_pSendBuf;

	if( m_pPacketQueue ) delete m_pPacketQueue;

	LeaveCriticalSection( &m_cs );	

	DeleteCriticalSection( &m_cs );
}

void JXSocket::Create( SI32 siRecvBufSize, SI32 siSendBufSize, SI32 siPacketQueueSize )
{

	EnterCriticalSection( &m_cs );

	if( m_pRecvBuf ) delete [] m_pRecvBuf;
	if( m_pSendBuf ) delete [] m_pSendBuf;

	m_pRecvBuf = new char[ siRecvBufSize ];
	m_pSendBuf = new char[ siSendBufSize ];

	m_siRecvBufSize = siRecvBufSize;
	m_siSendBufSize = siSendBufSize;

	__clearRecvBuffer();
	__clearSendBuffer();

	if( m_pPacketQueue ) delete m_pPacketQueue;

	m_pPacketQueue = new JXPacketQueue;

	m_pPacketQueue->Create( siPacketQueueSize );

	LeaveCriticalSection( &m_cs );
}


void JXSocket::IsConnect( BOOL *pbRet )
{
	EnterCriticalSection( &m_cs );

	*pbRet = m_bConnect;

	LeaveCriticalSection( &m_cs );
}


void JXSocket::Disconnect()
{
	EnterCriticalSection( &m_cs );

	__disconnect();

	LeaveCriticalSection( &m_cs );
}

void JXSocket::__disconnect()
{
	if( m_bConnect == FALSE ) return;

	printf("Disconnect!! [%x]\n", this );
	
	closesocket( m_socket );

	m_bConnect = FALSE;

	__clearRecvBuffer();
	__clearSendBuffer();

	m_pPacketQueue->Clear();
}

void JXSocket::SetSocket( SOCKET socket, SOCKADDR_IN sockaddr )
{

	EnterCriticalSection( &m_cs );

	m_socket = socket;
	m_sockaddr = sockaddr;

	__clearRecvBuffer();
	__clearSendBuffer();

	m_pPacketQueue->Clear();

	m_bConnect = TRUE;

	m_dwLastTickCount = GetTickCount();

	LeaveCriticalSection( &m_cs );

}

void JXSocket::__clearRecvBuffer()
{
	m_bRecvPacketSize = FALSE;
	m_siRecvDataLen = 0;
	m_usRecvBytesToRead = 0;
}

void JXSocket::__clearSendBuffer()
{
	m_siSendStartPos = 0;
	m_siSendDataLen = 0;
}

void JXSocket::__recv()
{

	// siStartPos : 버퍼에서 패킷의 시작 위치			
	// siCurrentPos : 버퍼에서 현재 위치		
	// len : 바이트 총 길이			
	//
	// m_bRecvPacketSize        : 데이터 사이즈를 읽었는지 여부
	// m_siRecvDataLen          : 읽은 데이터 크기 
	// m_usRecvBytesToRead		: 읽어야할 데이터 크기
	//  

	if( m_bConnect == FALSE )
	{
		return;
	}

	SI32 siStartPos = 0;								// 버퍼에서 패킷의 시작 위치
	SI32 siCurrentPos = 0;								// 버퍼에서 현재 위치

	int len;
	len = ::recv( m_socket, (char *)( m_pRecvBuf + m_siRecvDataLen ), m_siRecvBufSize - m_siRecvDataLen, 0 );
	
	if( len == SOCKET_ERROR ) {
		// 접속 해제
		local_dwError = GetLastError();

		if( local_dwError == WSAEWOULDBLOCK ) { return; }

		printf( "Recv Error!! [%d]\n", local_dwError );

//		writeInfoToFile( TRUE, "JXSocketERR.txt", "받기 에러!! [%d]\n", local_dwError );

		__disconnect();
		
		return;
	}

	if( len == 0 ) return;

	m_dwLastTickCount = GetTickCount();

	siCurrentPos = m_siRecvDataLen;

	len += m_siRecvDataLen;							// ( 버퍼에 남아 있는 자료의 바이트 + 전송 받은 자료의 바이트 )

	while( siCurrentPos < len ) {

		if( m_bRecvPacketSize == FALSE ) {				
			// 패킷 데이터 사이즈를 읽지 않았다면,
			
			if( len - siStartPos < 2 ) break;

			memcpy( &m_usRecvBytesToRead, &m_pRecvBuf[ siStartPos ], 2 );

			if(m_bOdering == TRUE)
			{
				m_usRecvBytesToRead = ntohs(m_usRecvBytesToRead);
			}			

			// 메시지 최대 크기 보다 크다면,
			if( m_usRecvBytesToRead > JXPACKET_MAX_SIZE ) {
				
				UI16 usTemp;

				memcpy( &usTemp, &m_pRecvBuf[ siStartPos + 15 ], 4 );
/*
//				writeInfoToFile( TRUE, "JXSocketERR.txt", "[%d][%d]Disconnected becuase of bigger package than the most allowed message size!!\n", 
					m_usRecvBytesToRead, usTemp );
*/
				__disconnect();

				return;
			}

			siCurrentPos = siStartPos + 2;
			m_usRecvBytesToRead -= 2;

			m_bRecvPacketSize = TRUE;

			continue;

		} else {
			// 패킷 데이터 부분을 읽는다				

			if( m_usRecvBytesToRead > (UI16)( len - siCurrentPos ) ) {
				// 데이터를 한번에 모두 읽을 수 없을 때,

				m_usRecvBytesToRead -= (UI16)( len - siCurrentPos );

				break;

			} else {
				// 데이터를 한번에 모두 읽을 수 있을 때, 

				siCurrentPos += (SI32)m_usRecvBytesToRead;

				m_pPacketQueue->Enqueue( &m_pRecvBuf[ siStartPos ] );
				
				siStartPos = siCurrentPos;

				m_bRecvPacketSize = FALSE;

				continue;
			}

		}

	}

	m_siRecvDataLen = len - siStartPos;

	if( siStartPos > 0 && m_siRecvDataLen > 0 ){													
		// 한 번에 두 개 이상의 패킷이 도착한 경우라면,					
		// 맨 마지막 미완성 패킷을 버퍼에서 맨 앞으로 당긴다			
		
		memcpy( m_pRecvBuf, &m_pRecvBuf[ siStartPos ], m_siRecvDataLen );
	}

}

void JXSocket::__send()
{

	if( m_bConnect == FALSE || m_siSendDataLen == 0 ) return;

	int bytesSent;	
	
	int lenA, lenB;

	if( m_siSendStartPos + m_siSendDataLen > m_siSendBufSize ){

		lenA = m_siSendBufSize - m_siSendStartPos;
		lenB = m_siSendDataLen - lenA;

		// 두 부분으로 나누어서 전송해야 하는 경우

		bytesSent = ::send( m_socket, (const char *)&m_pSendBuf[ m_siSendStartPos ], lenA, 0 );

		if( bytesSent == SOCKET_ERROR ) { 
			// 에러.. 접속 종료 
			local_dwError = GetLastError();

			if( local_dwError == WSAEWOULDBLOCK ) return;

			printf("send error!! [%d]\n", local_dwError );

//			writeInfoToFile( TRUE, "JXSocketERR.txt", "보내기에러!! [%d]\n", local_dwError );

//			__disconnect();

			return;
		} 

		if( bytesSent < lenA ) { 
			// 다 보내지 못 했으면, 
			m_siSendDataLen -= bytesSent; 
			m_siSendStartPos += bytesSent; 
			if( m_siSendStartPos >= m_siSendBufSize ) m_siSendStartPos -= m_siSendBufSize; 

			return;
		} 
 
		bytesSent = ::send( m_socket, (const char *)m_pSendBuf, lenB, 0 ); 
 
		if( bytesSent == SOCKET_ERROR ) {

			local_dwError = GetLastError();

			if( local_dwError == WSAEWOULDBLOCK ) return;

			printf("send error!! [%d]\n", local_dwError );

//			writeInfoToFile( TRUE, "JXSocketERR.txt", "보내기에러!! [%d]\n", local_dwError );

//			__disconnect();

			return;
		} 

		if( bytesSent < lenB ) { 

			m_siSendDataLen -= ( lenA + bytesSent ); 
			m_siSendStartPos += ( lenA + bytesSent ); 
			if( m_siSendStartPos >= m_siSendBufSize ) m_siSendStartPos -= m_siSendBufSize; 
			
			return;
		} 

		m_siSendDataLen = 0; 
		m_siSendStartPos = 0; 
 
	} else { 

		// 한번에 전송 가능한 경우
		bytesSent = ::send( m_socket, (const char *)&m_pSendBuf[ m_siSendStartPos ], m_siSendDataLen, 0 );

		if( bytesSent == SOCKET_ERROR ) {

			local_dwError = GetLastError();

			if( local_dwError == WSAEWOULDBLOCK ) return;

			printf("send error!! [%d]\n", local_dwError );

//			writeInfoToFile( TRUE, "JXSocketERR.txt", "보내기에러!! [%d]\n", local_dwError );

//			__disconnect();			

			return;
		}

		if( bytesSent < m_siSendDataLen ) {
			// 다 보내지 못 했으면,
			m_siSendDataLen -= bytesSent;
			m_siSendStartPos += bytesSent;

			if( m_siSendStartPos >= m_siSendBufSize ) m_siSendStartPos -= m_siSendBufSize;

			return;
		}

		m_siSendDataLen = 0;
		m_siSendStartPos = 0;
	}

}

void JXSocket::Send()
{
	EnterCriticalSection( &m_cs );

	__send();

	LeaveCriticalSection( &m_cs );
}

void JXSocket::Recv()
{
	EnterCriticalSection( &m_cs );

	__recv();
 
	LeaveCriticalSection( &m_cs );
}

void JXSocket::SendPacket( JXPacket *pPacket )
{
	SendPacket( pPacket->GetPacket(), pPacket->GetPacketSize() );
}

void JXSocket::SendPacket( char *pPacket, int len )
{

	EnterCriticalSection( &m_cs );

	if( m_bConnect == FALSE ) {
		
		LeaveCriticalSection( &m_cs );

		return;
	}

	int endPtr;
	int lenB;

	// 받은 데이터 보다 남은 버퍼의 크기가 커야된다
	while( len > m_siSendBufSize - m_siSendDataLen ) {
		printf("not enough write buffer...\n");

//		writeInfoToFile( TRUE, "JXSocketERR.txt", "Lack writing-buffer!\n" );
		
		m_bConnect = FALSE;
		
		LeaveCriticalSection( &m_cs );

		return;
	}

	// 데이터의 마지막 위치를 구한다
	endPtr = m_siSendStartPos + m_siSendDataLen;

	if( endPtr >= m_siSendBufSize ) { 
		endPtr -= m_siSendBufSize;
		memcpy( &m_pSendBuf[ endPtr ], pPacket, len );
		m_siSendDataLen += len;

		LeaveCriticalSection( &m_cs );
		return;
	}

	// 남은 뒷 부분의 크기 
	lenB = m_siSendBufSize - endPtr;
	
	if( len > lenB ) {
		// 남은 뒷 부분 보다 받은 데이터가 크면 둘로 나눠서 넣는다

		memcpy( &m_pSendBuf[ endPtr ], pPacket, lenB );
		memcpy( m_pSendBuf, pPacket + lenB, len - lenB );

		m_siSendDataLen += len;

	} else {
		// 한번에 넣을 수 있으면 넣는다
		
		memcpy( &m_pSendBuf[ endPtr ], pPacket, len );
		m_siSendDataLen += len;

	}

	LeaveCriticalSection( &m_cs );
}


void JXSocket::NBRecv()
{
	EnterCriticalSection( &m_cs );
	
	if( m_bConnect == FALSE ) {
		LeaveCriticalSection( &m_cs );
		return;
	}

	timeval	timeout; 
	timeout.tv_sec = 0; 
	timeout.tv_usec = 0; 

	fd_set fdset, readfds; 
	
	FD_ZERO( &fdset );  
	FD_SET( m_socket, &fdset );	 

	readfds = fdset;
	
	select( 0, &readfds, 0, 0, &timeout ); 

	if( FD_ISSET( m_socket, &readfds ) ) __recv(); 

	LeaveCriticalSection( &m_cs );

}

void JXSocket::NBSend()
{

	EnterCriticalSection( &m_cs );

	if( m_bConnect == FALSE ) {
		LeaveCriticalSection( &m_cs );
		return;
	}

	timeval	timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	fd_set	fdset, writefds, errfds;	

	FD_ZERO( &fdset );
	FD_SET( m_socket, &fdset );	

	writefds = errfds = fdset;

	select( 0, 0, &writefds, &errfds, &timeout );

	if( FD_ISSET( m_socket, &writefds ) ) __send();

	LeaveCriticalSection( &m_cs );

}
