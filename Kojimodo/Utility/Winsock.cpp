#include <winsock2.h>
#include <mmsystem.h>
#include "Winsock.h"







//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 생 성 자
//--------------------------------------------------------------------------------------------
CWinsock::CWinsock(int header_size, int wait_time, int max_sendQ, int max_recvQ): 
HEADER_SIZE(header_size), WAIT_TIME(wait_time), MAX_SENDQUEUE(max_sendQ), MAX_RECVQUEUE(max_recvQ)

{
	m_sendQueue = new char[MAX_SENDQUEUE];
	m_recvQueue = new char[MAX_RECVQUEUE];


	initialize();
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 소 멸 자
//--------------------------------------------------------------------------------------------
CWinsock::~CWinsock()
{
	Dissconnect();

	if( m_sendQueue ) {

		delete m_sendQueue;
	}

	if( m_recvQueue ) {

		delete m_recvQueue;
	}
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 초기화 작업  
//--------------------------------------------------------------------------------------------
void CWinsock::initialize()
{
	m_socket = INVALID_SOCKET;	


	memset(m_recvQueue,0,MAX_RECVQUEUE);
	m_rcurPos   = 0;
	
	memset(m_sendQueue,0,MAX_SENDQUEUE);
	m_scurPos   = 0;

	
	//////////////////////////////
	setLastError( DONOT_CONNECT );
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 최종적으로 발생한 에러를 설정 한다 
//--------------------------------------------------------------------------------------------
void CWinsock::setLastError(int error)
{	
	m_lastError = error;
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 최종적으로 발생한 에러를 발생 한다 
//--------------------------------------------------------------------------------------------
int CWinsock::getLastError()
{
	return m_lastError;	
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 송신큐의 모든 데이타를 전송 한다 
//--------------------------------------------------------------------------------------------
bool CWinsock::Send()
{
	if( isSendable() ) 
	{
		int size = send(m_socket, m_sendQueue, m_scurPos, 0);
	
		if( size == SOCKET_ERROR )
		{		
			int error  = WSAGetLastError();
			if( error != WSAEWOULDBLOCK)
			{
				//////////////////////////
				setLastError( FAIL_SEND );								
				return false; 
			}
		}

		else if( size > 0 )
		{								
			m_scurPos -= size;
			
			if( m_scurPos == 0 ) {
							
				return true;
			}
						
			memmove(m_sendQueue, m_sendQueue+size, m_scurPos);			
		}					
	}
	
	return false;
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 수신큐에 수신 받은 패킷을 넣는다 
//--------------------------------------------------------------------------------------------
bool CWinsock::Recv()
{
	if( isRecvable() )
	{	
		char buffer[512];
		int size = ::recv(m_socket,buffer,512,0);
		
		if( size == SOCKET_ERROR ) 
		{
			if(WSAGetLastError() != WSAEWOULDBLOCK) 
			{
				//////////////////////////
				setLastError( FAIL_RECV );
				return false;		
			}
		}

		else if(size > 0)
		{								
			if( m_rcurPos + size > MAX_RECVQUEUE )	
			{
				///////////////////////////////////
				setLastError( OVERFLOW_RECVQUEUE );
				return false;													
			}
			
			
			memcpy(m_recvQueue+m_rcurPos, buffer, size);													
			m_rcurPos+=size;									
						
			return true;
		}
	}
			
	return false;
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 송신할 패킷을 송신큐에 넣는다 
//--------------------------------------------------------------------------------------------
bool CWinsock::setSendPacket(char* header, char* data, int dataSize)
{
	if(header && data && dataSize > 0)
	{
		int packetSize = dataSize + HEADER_SIZE;

		if( m_scurPos + packetSize <= MAX_SENDQUEUE )
		{			
			memcpy(m_sendQueue + m_scurPos, header, HEADER_SIZE);
			m_scurPos += HEADER_SIZE;
			
			memcpy(m_sendQueue + m_scurPos, data, dataSize);
			m_scurPos += dataSize;
			
			return true;
		}
		else
		{
			///////////////////////////////////
			setLastError( OVERFLOW_SENDQUEUE );
			return false;
		}
	}
		
	return false;
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 수신 받은 패킷을 얻어 온다
//--------------------------------------------------------------------------------------------
bool CWinsock::getRecvPacket(char* header, char* data, int dataSize)
{			
	if(m_rcurPos >= HEADER_SIZE)	
	{					 			
		int			   packetSize   = getPacketSize(m_recvQueue);	 
		int			   srcDataSize	= packetSize  - HEADER_SIZE;	 
		char*		   srcData		= m_recvQueue + HEADER_SIZE;	 
	
		if(m_rcurPos >= packetSize)
		{					
			if(getChecksum(m_recvQueue) != calcChecksum(srcData, srcDataSize)) 
			{
				///////////////////////////////
				setLastError( ERROR_CHECKSUM );
				return false;						
			}
						
			if( header )  memcpy( header , m_recvQueue , HEADER_SIZE );			
			if( data   )  memcpy( data   , srcData	   , dataSize );
			
			
			m_rcurPos -= packetSize;								
			
			if(m_rcurPos > 0)
			memmove(m_recvQueue, m_recvQueue+packetSize, m_rcurPos);		
			
			
			return true;			
		}
	}		
		
	return false;					
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 패킷을 수신받는다 ( 블락 함수 )
//--------------------------------------------------------------------------------------------
bool CWinsock::Recv(char* header, char* data, int dataSize)
{

	DWORD startTime = timeGetTime();
	

	while( TRUE )
	{
		if( WAIT_TIME != 0 && ((timeGetTime() - startTime) >= WAIT_TIME) )  {

			//////////////////////////////
			setLastError( OVER_WAITTIME );
			break;		
		}

		if( Recv() )
		{
			if( getRecvPacket(header, data, dataSize) ) return true;
			
			else if( getLastError() < 0 ) break;
		}

		else if( getLastError() < 0 ) break;

		Sleep(10);
	}
	
	return false;
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 패킷을 송신 한다 ( 블락 함수 )
//--------------------------------------------------------------------------------------------
bool CWinsock::Send(char* header, char* data, int dataSize)
{	
	if( setSendPacket(header, data, dataSize) )
	{
		DWORD startTime = timeGetTime();
		

		while( m_scurPos > 0 )
		{							
			if( WAIT_TIME != 0 && ((timeGetTime() - startTime) >= WAIT_TIME) ) {
				
				//////////////////////////////
				setLastError( OVER_WAITTIME );
				break;	
			}
			
			if( Send() ) return true;

			else if( getLastError() < 0 )
			{
				break;
			}

			Sleep(10);
		}
	}

	return false;														
}







//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 파일(소켓)열거자를 초기화 한다 
//--------------------------------------------------------------------------------------------
void CWinsock::clearFD_SET()
{
	FD_ZERO(&m_fdError);
	FD_SET(m_socket,&m_fdError);

	FD_ZERO(&m_fdUser);
	FD_SET(m_socket,&m_fdUser);
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 패킷을 송신 할수 있는가?
//--------------------------------------------------------------------------------------------
bool CWinsock::isSendable()	
{
	clearFD_SET();


	if( SOCKET_ERROR != select(m_socket, NULL, &m_fdUser, &m_fdError, &m_timeout) ) 
	{
		if ( FD_ISSET(m_socket, &m_fdUser)  ) return true;

		if ( FD_ISSET(m_socket, &m_fdError) )
		{		
			/////////////////////////////
			setLastError( ERROR_SELECT );
			return false;
		}
	}
		
	return false;
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 패킷을 수신 할수 있는가?
//--------------------------------------------------------------------------------------------
bool CWinsock::isRecvable()				
{
	clearFD_SET();

	
	if( SOCKET_ERROR != select( m_socket, &m_fdUser, NULL, &m_fdError, &m_timeout) ) 
	{
		if ( FD_ISSET(m_socket, &m_fdUser)  ) return true;

		if ( FD_ISSET(m_socket, &m_fdError) )
		{
			/////////////////////////////
			setLastError( ERROR_SELECT );
			return false;
		}
	}
	
	return false;
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 윈속의 하부레이어의 부분을 시작 한다 
//--------------------------------------------------------------------------------------------
void CWinsock::startupWinsock()	
{
	WORD	ver = MAKEWORD(2,2);
	WSADATA	wsaData;

	WSAStartup(ver, &wsaData);
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 해당 IP와 PORT에 연결을 시도 한다 
//--------------------------------------------------------------------------------------------
bool CWinsock::Connect(char* ip, int port)				
{

	Dissconnect();
	

	m_socket = socket(AF_INET, SOCK_STREAM, 0 );	

	if( m_socket != INVALID_SOCKET )
	{				
		sockaddr_in saddr;

		saddr.sin_family		= AF_INET;	
		saddr.sin_port			= htons(port);	
		saddr.sin_addr.s_addr   = inet_addr(ip);

		if( SOCKET_ERROR != connect(m_socket, (SOCKADDR*)&saddr, sizeof(saddr)) ) 
		{
			/////////////////////////
			setLastError( NO_ERROR );
			return true;
		}
	}

	/////////////////////////////
	setLastError( FAIL_CONNECT );
	return false;	
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 현재 연결된 호스트와의 연결을 끊는다 
//--------------------------------------------------------------------------------------------
void CWinsock::Dissconnect()		
{	
	closesocket(m_socket);	

	initialize();
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 윈속의 하부레이어 부분을 종료 한다 
//--------------------------------------------------------------------------------------------
void CWinsock::cleanupWinsock()		
{
	WSACleanup();
}