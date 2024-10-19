/************************************************************************************************
	���ϸ� : HQChattingServerClientSocket.cpp
	����� : ������

	Chatting Server�� ����
************************************************************************************************/
#include <GSL.h>
#include <winsock2.h>
#include <winbase.h>

#include <stdio.h>
#include <conio.h>

#include <clGame.h>
#include "Directives.h"
#include "HQChattingServerClientSocket.h"

extern _clGame* pGame;


HQClientSocket::HQClientSocket()
{
	m_socket = -1;
	m_bConnect = FALSE;

	m_pRecvBuf = NULL;
	m_pSendBuf = NULL;

	ZeroMemory(IPAddress, sizeof(IPAddress));

	m_Event = WSACreateEvent();
}

HQClientSocket::~HQClientSocket()
{
	Close();

	if( m_pRecvBuf ) delete [] m_pRecvBuf;
	if( m_pSendBuf ) delete [] m_pSendBuf;

	WSACloseEvent(m_Event);
}

void HQClientSocket::Create( UI32 uiRecvBufSize, UI32 uiSendBufSize )
{
	if( m_pRecvBuf ) delete [] m_pRecvBuf;
	if( m_pSendBuf ) delete [] m_pSendBuf;

	m_pRecvBuf = new char[ uiRecvBufSize ];
	m_pSendBuf = new char[ uiSendBufSize ];

	m_uiRecvBufSize = uiRecvBufSize;
	m_uiSendBufSize = uiSendBufSize;

	ClearRecvBuffer();
	ClearSendBuffer();

	m_fIsMap		= 0;
	m_NowMapMapIndex= 0;
	m_NowMapXPos	= 0;
	m_NowMapYPos	= 0;
	m_fLogInSucess	= FALSE;
	m_ReconnectFlag	= FALSE;
	m_GMGrade		= 0;
	m_ReconnectTickCount = 0;

	ZeroMemory(m_LogInID, 32);
}


BOOL HQClientSocket::Connect( const char *pszIP, UI16 usPort )
{
	int err;
	if( pszIP == NULL )		return FALSE;

	strcpy((char*)IPAddress, pszIP);
	Port = usPort;

	memset( &m_sockaddr, 0, sizeof( m_sockaddr ) );

	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_addr.s_addr = inet_addr( pszIP );
	m_sockaddr.sin_port = htons( usPort );

	m_socket = socket( AF_INET, SOCK_STREAM, 0 );

	if( m_socket == INVALID_SOCKET )
	{
		m_ReconnectFlag = TRUE;
		return FALSE;
	}

	err = connect( m_socket, (SOCKADDR *)&m_sockaddr, sizeof( m_sockaddr ) );

	if( err == SOCKET_ERROR )
	{
		m_ReconnectFlag = TRUE;
		closesocket( m_socket );
		int error = WSAGetLastError();
		return FALSE;
	}

	m_bConnect = TRUE;

	ClearRecvBuffer();
	ClearSendBuffer();

//	WSAEventSelect(m_socket, m_Event, FD_READ | FD_CLOSE);

	m_LastTickCount = GetTickCount();

	return TRUE;
}

BOOL HQClientSocket::DataProc(void)
{
	WSANETWORKEVENTS		TempEvent;
	WSAEventSelect( m_socket, m_Event, FD_READ | FD_CLOSE );

	if(m_ReconnectFlag)
	{
		if(Reconnect())
			m_ReconnectFlag = FALSE;
	}

	if(WSAEnumNetworkEvents(m_socket, m_Event, &TempEvent) != 0)
	{
		return FALSE;
	}

	if((TempEvent.lNetworkEvents & FD_READ) == FD_READ)
	{
		Recv();
	}

	if((TempEvent.lNetworkEvents & FD_CLOSE) == FD_CLOSE)
	{
		m_ReconnectFlag = TRUE;
		if(Reconnect())
			m_ReconnectFlag = FALSE;
	}

	if((GetTickCount() - m_LastTickCount) > 30000)
		HeartBeat();

	return TRUE;
}


void HQClientSocket::Close()
{
	closesocket( m_socket );

	m_socket = NULL;
	m_bConnect = FALSE;

	m_fIsMap		= 0;
	m_NowMapMapIndex= 0;
	m_NowMapXPos	= 0;
	m_NowMapYPos	= 0;
	m_fLogInSucess	= FALSE;
	m_ReconnectFlag	= FALSE;

	ZeroMemory(m_LogInID, 32);
	m_LogInDBAccount = 0;

	ClearRecvBuffer();
	ClearSendBuffer();
}

BOOL HQClientSocket::Reconnect()
{
	int	errorCode = WSAGetLastError();

	// �ʱ�ȭ
	closesocket( m_socket );

	m_socket		= NULL;
	m_bConnect		= FALSE;
	m_fLogInSucess	= FALSE;
	m_fIsMap		= 0;
	m_NowMapMapIndex= 0;
	m_NowMapXPos	= 0;
	m_NowMapYPos	= 0;
	ClearRecvBuffer();
	ClearSendBuffer();

	// 30�ʸ��� �翬���� �õ��Ѵ�.
	if((GetTickCount() - m_ReconnectTickCount) < 30000)
		return FALSE;

	m_ReconnectTickCount = GetTickCount();

	// �翬��
	if(Connect((char*)IPAddress, Port))
	{
		MessageSend_LogIn((char*)m_LogInID, m_LogInDBAccount, m_GMGrade);
		return TRUE;
	}
	else
		return FALSE;
}

BOOL HQClientSocket::MessageSend_LogIn(char* lpID, DWORD DBAccount, UI16 uiGrade)
{
	if(lpID[0] == NULL)		return FALSE;

	HQPacket Packet;
	UI16 CMD;
	char TempBuffer[32];

	ZeroMemory(TempBuffer, 32);
	strncpy((char*)TempBuffer, lpID, 10);
	Packet.ClearPacket();
	CMD = REQUEST_LOGIN;
	Packet.WriteValue(&CMD,       2);			// Packet ID
	Packet.WriteValue(&DBAccount, 4);			// IP
	Packet.WriteValue(&uiGrade, 2);				// GMGrade
	Packet.WriteString((char*)TempBuffer, 32);	// ID
	Packet.CompletePacket();
	WritePacket(&Packet);
	Send();

	if(lpID != m_LogInID)
	{
		ZeroMemory(m_LogInID, 32);
		strcpy((char*)m_LogInID, lpID);
	}
	m_LogInDBAccount = DBAccount;
	m_GMGrade = uiGrade;

	return TRUE;
}

BOOL HQClientSocket::MessageSend_SendChatting(char* lpChatting)
{
	HQPacket			Packet;
	UI16				CMD;

	if(m_fLogInSucess == FALSE)		return FALSE;

	Packet.ClearPacket();
	CMD = REQUEST_CHATMESSAGE;
	Packet.WriteValue(&CMD,        2);						// Packet ID
	Packet.WriteString(lpChatting, 256);					// Chatting
	Packet.CompletePacket();
	WritePacket(&Packet);
	Send();

	return TRUE;
}

BOOL HQClientSocket::MessageSend_SendWhisper(char* lpID, char* lpChatting)
{
	HQPacket			Packet;
	UI16				CMD;

	if(m_fLogInSucess == FALSE) return FALSE;

	Packet.ClearPacket();
	CMD = REQUEST_WHISPER;
	Packet.WriteValue(&CMD,        2);						// Packet ID
	Packet.WriteString(lpID,       32);						// ID
	Packet.WriteString(lpChatting, 256);					// Chatting
	Packet.CompletePacket();
	WritePacket(&Packet);
	Send();

	return TRUE;
}

BOOL HQClientSocket::MessageSend_MoveSell(UI16 uiMapCode, UI16 uiXPos, UI16 uiYPos)
{
	HQPacket			Packet;
	UI16				CMD;

	if(m_fLogInSucess == FALSE) return FALSE;

	uiXPos /= 20;
	uiYPos /= 20;
	if((m_fIsMap != 1) || (m_NowMapMapIndex != uiMapCode) || (m_NowMapXPos != uiXPos) || (m_NowMapYPos != uiYPos))
	{
		Packet.ClearPacket();
		CMD = REQUEST_MOVECELL;
		Packet.WriteValue(&CMD,       2);						// Packet ID
		Packet.WriteValue(&uiMapCode, 2);						// Map Code
		Packet.WriteValue(&uiXPos,    2);						// X Pos
		Packet.WriteValue(&uiYPos,    2);						// Y Pos
		Packet.CompletePacket();
		WritePacket(&Packet);
		Send();

		m_fIsMap         = 1;
		m_NowMapMapIndex = uiMapCode;
		m_NowMapXPos     = uiXPos;
		m_NowMapYPos     = uiYPos;
	}

	return TRUE;
}

BOOL HQClientSocket::MessageSend_NotIsMap(void)
{
	HQPacket			Packet;
	UI16				CMD;

	if(m_fLogInSucess == FALSE) return FALSE;

	if(m_fIsMap != 2)
	{
		Packet.ClearPacket();
		CMD = REQUEST_OUTWORLD;
		Packet.WriteValue(&CMD, 2);						// Packet ID
		Packet.CompletePacket();
		WritePacket(&Packet);
		Send();
	}

	m_fIsMap = 2;

	return TRUE;
}

BOOL HQClientSocket::HeartBeat(void)
{
	HQPacket			Packet;
	UI16				CMD;

	Packet.ClearPacket();
	CMD = HEARTBEAT;
	Packet.WriteValue(&CMD, 2);			// Packet ID
	Packet.CompletePacket();
	WritePacket(&Packet);
	Send();

	return TRUE;
}

void HQClientSocket::ClearRecvBuffer()
{
	m_bRecvDataSize = FALSE;
	m_uiRecvBytes = 0;
	m_usRecvBytesToRead = 0;
}

void HQClientSocket::ClearSendBuffer()
{
	m_startPtr = 0;
	m_dataLen = 0;
}

BOOL HQClientSocket::Recv()
{
	// startPos : ���� ��ġ
	// currentPos : ���� ��ġ  
	// len : ����Ʈ �� ����
	//
	// m_bRecvDataSize        : ������ ����� �о����� ����
	// m_uiRecvBytes          : ���� ������ ũ�� 
	// m_usRecvBytesToRead    : �о���� ������ ũ��
	// 

	SI32 startPos = 0;								// ���ۿ��� ��Ŷ�� ���� ��ġ
	SI32 currentPos = 0;							// ���ۿ��� ���� ��ġ

	int len;

	len = ::recv( m_socket, (char *)( m_pRecvBuf + m_uiRecvBytes ), m_uiRecvBufSize - m_uiRecvBytes, 0 );

	if( len <= 0 ) 
	{
		m_bConnect = FALSE;
		m_ReconnectFlag = TRUE;
		return FALSE;									
	}

	currentPos = m_uiRecvBytes;

	len += m_uiRecvBytes;					// ( ���ۿ� ���� �ִ� �ڷ��� ����Ʈ + ���� ���� �ڷ��� ����Ʈ )

	while( currentPos < len ) {

		if( m_bRecvDataSize == FALSE ) {
			// ��Ŷ ������ ����� ���� �ʾҴٸ�,
			
			if( len - startPos < 2 ) {
				m_uiRecvBytes = len - startPos;
				return TRUE;
			};

			memcpy( &m_usRecvBytesToRead, &m_pRecvBuf[ startPos ], 2 );

			m_bRecvDataSize = TRUE;

			currentPos = startPos + 2;

			m_usRecvBytesToRead -= 2;

			continue;

		} else {
			// ��Ŷ ������ �κ��� �д´�				

			if( m_usRecvBytesToRead > len - currentPos ){
				// �����͸� �ѹ��� ��� ���� �� ���� ��,

				m_usRecvBytesToRead -= ( len - currentPos );

				break;

			} else {
				// �����͸� �ѹ��� ��� ���� �� ���� ��, 

				currentPos += m_usRecvBytesToRead;

				pGame->ChattingPacketProc( (char *)&m_pRecvBuf[ startPos ] );
				
				startPos = currentPos;

				m_bRecvDataSize = FALSE;

				continue;
			}

		}

	}

	if( startPos > 0 ){ 
		// �� ���� �� �� �̻��� ��Ŷ�� ������ �����, 
		// �� ������ �̿ϼ� ��Ŷ�� ���ۿ��� �� ������ ���� 
		
		m_uiRecvBytes = len - startPos;
		
		if( m_uiRecvBytes > 0 ) memcpy( m_pRecvBuf, &m_pRecvBuf[ startPos ], m_uiRecvBytes );

	} else {

		m_uiRecvBytes = len;
	}

	return TRUE;
}

BOOL HQClientSocket::Send()
{
	if(!m_bConnect)		return FALSE;
	if(m_dataLen == 0)	
	{
		m_bConnect = FALSE;
		m_ReconnectFlag = TRUE;
		return FALSE;
	}

	int bytesSent;	
	
	int lenA, lenB;
	int endPtr = m_startPtr + m_dataLen;

	if( m_uiSendBufSize < (unsigned)endPtr ){

		lenA = m_uiSendBufSize - m_startPtr;
		lenB = m_dataLen - lenA;

		// �� �κ����� ����� �����ؾ� �ϴ� ���

		bytesSent = ::send( m_socket, (const char *)&m_pSendBuf[ m_startPtr ], lenA, 0 );

		if( bytesSent == SOCKET_ERROR ) { 
			// ����.. ���� ���� 
			m_bConnect = FALSE;
			m_ReconnectFlag = TRUE;
			return FALSE; 
		} 

		if( bytesSent < lenA ) { 
			// �� ������ �� ������, 
			m_dataLen -= bytesSent; 
			m_startPtr += bytesSent; 
			if( (unsigned)m_startPtr >= m_uiSendBufSize ) m_startPtr -= m_uiSendBufSize; 
			return FALSE; 
		} 
 
		bytesSent = ::send( m_socket, (const char *)m_pSendBuf, lenB, 0 ); 
 
		if( bytesSent == SOCKET_ERROR ) {
			m_bConnect = FALSE;
			m_ReconnectFlag = TRUE;			
			return FALSE; 
		} 
		
		if( bytesSent < lenB ) { 
			m_dataLen -= ( lenA + bytesSent ); 
			m_startPtr += ( lenA + bytesSent ); 
			if( (unsigned)m_startPtr >= m_uiSendBufSize ) m_startPtr -= m_uiSendBufSize; 
			return FALSE; 
		} 

		m_dataLen = 0; 
		m_startPtr = 0; 
 
	} else { 

		// �ѹ��� ���� ������ ���
		bytesSent = ::send( m_socket, (const char *)&m_pSendBuf[ m_startPtr ], m_dataLen, 0 );

		if( bytesSent == SOCKET_ERROR ){
			m_bConnect = FALSE;
			m_ReconnectFlag = TRUE;			
			return FALSE;
		}

		if( bytesSent < m_dataLen ){
			// �� ������ �� ������,
			m_dataLen -= bytesSent;
			m_startPtr += bytesSent;

			if( (unsigned)m_startPtr >= m_uiSendBufSize )m_startPtr -= m_uiSendBufSize;

			return FALSE;
		}

		m_dataLen = 0;
		m_startPtr = 0;
	}

	m_LastTickCount = GetTickCount();
	return TRUE;
}

BOOL HQClientSocket::WritePacket( HQPacket *pPacket )
{
	return WritePacket( pPacket->GetPacket(), pPacket->GetPacketSize() );
}

BOOL HQClientSocket::WritePacket( char *pPacket, int len )
{
	int endPtr;
	int lenB;

	// ���� ������ ũ�⺸�� ���� �����Ͱ� ũ�� FALSE ��ȯ
	if( (unsigned)len > m_uiSendBufSize - m_dataLen ) return FALSE;

	// �������� ������ ��ġ�� ���Ѵ�
	endPtr = m_startPtr + m_dataLen;

	if( (unsigned)endPtr >= m_uiSendBufSize ){
		endPtr -= m_uiSendBufSize;
		memcpy( &m_pSendBuf[ endPtr ], pPacket, len );
		m_dataLen += len;
		return TRUE;
	}

	// ���� �� �κ��� ũ�� 
	lenB = m_uiSendBufSize - endPtr;
	
	if( len > lenB ) {
		// ���� �� �κ� ���� ���� �����Ͱ� ũ�� �ѷ� ������ �ִ´�

		memcpy( &m_pSendBuf[ endPtr ], pPacket, lenB );
		memcpy( m_pSendBuf, pPacket + lenB, len - lenB );

		m_dataLen += len;

	} else {
		// �ѹ��� ���� �� ������ �ִ´�
		
		memcpy( &m_pSendBuf[ endPtr ], pPacket, len );
		m_dataLen += len;

	}

	return TRUE;
}
