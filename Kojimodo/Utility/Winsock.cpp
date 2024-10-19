#include <winsock2.h>
#include <mmsystem.h>
#include "Winsock.h"







//--------------------------------------------------------------------------------------------
// Name : 
// Desc : �� �� ��
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
// Desc : �� �� ��
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
// Desc : �ʱ�ȭ �۾�  
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
// Desc : ���������� �߻��� ������ ���� �Ѵ� 
//--------------------------------------------------------------------------------------------
void CWinsock::setLastError(int error)
{	
	m_lastError = error;
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : ���������� �߻��� ������ �߻� �Ѵ� 
//--------------------------------------------------------------------------------------------
int CWinsock::getLastError()
{
	return m_lastError;	
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : �۽�ť�� ��� ����Ÿ�� ���� �Ѵ� 
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
// Desc : ����ť�� ���� ���� ��Ŷ�� �ִ´� 
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
// Desc : �۽��� ��Ŷ�� �۽�ť�� �ִ´� 
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
// Desc : ���� ���� ��Ŷ�� ��� �´�
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
// Desc : ��Ŷ�� ���Ź޴´� ( ��� �Լ� )
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
// Desc : ��Ŷ�� �۽� �Ѵ� ( ��� �Լ� )
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
// Desc : ����(����)�����ڸ� �ʱ�ȭ �Ѵ� 
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
// Desc : ��Ŷ�� �۽� �Ҽ� �ִ°�?
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
// Desc : ��Ŷ�� ���� �Ҽ� �ִ°�?
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
// Desc : ������ �Ϻη��̾��� �κ��� ���� �Ѵ� 
//--------------------------------------------------------------------------------------------
void CWinsock::startupWinsock()	
{
	WORD	ver = MAKEWORD(2,2);
	WSADATA	wsaData;

	WSAStartup(ver, &wsaData);
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : �ش� IP�� PORT�� ������ �õ� �Ѵ� 
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
// Desc : ���� ����� ȣ��Ʈ���� ������ ���´� 
//--------------------------------------------------------------------------------------------
void CWinsock::Dissconnect()		
{	
	closesocket(m_socket);	

	initialize();
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : ������ �Ϻη��̾� �κ��� ���� �Ѵ� 
//--------------------------------------------------------------------------------------------
void CWinsock::cleanupWinsock()		
{
	WSACleanup();
}