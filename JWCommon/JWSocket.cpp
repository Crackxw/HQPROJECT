#include "JWSocket.h"
#include "JWRecvBuffer.h"
#include "JWSendBuffer.h"

JWSocket::JWSocket()
{
	m_pRecvBuffer = NULL;
	m_pSendBuffer = NULL;

	m_socket = INVALID_SOCKET;
}

JWSocket::~JWSocket()
{

	if( m_pRecvBuffer ) delete m_pRecvBuffer;
	if( m_pSendBuffer ) delete m_pSendBuffer;
	
}

void JWSocket::CreateSocket( SI32 siRecvBufSize, SI32 siSendBufSize )
{
	
	if( m_pRecvBuffer ) delete m_pRecvBuffer;

	m_pRecvBuffer = new JWRecvBuffer;

	if( m_pSendBuffer ) delete m_pSendBuffer;

	m_pSendBuffer = new JWSendBuffer;

	m_pRecvBuffer->CreateRecvBuffer( siRecvBufSize );
	m_pSendBuffer->CreateSendBuffer( siSendBufSize );

	m_socket = INVALID_SOCKET;
}

BOOL JWSocket::SetSocket( SOCKET socket, SOCKADDR_IN sockaddr )
{

	if( m_socket == INVALID_SOCKET ) {

		m_socket = socket;
		m_sockaddr = sockaddr;

		m_pRecvBuffer->ClearBuffer();
		m_pSendBuffer->ClearBuffer();

		return TRUE;
	}

	return FALSE;
}

BOOL JWSocket::CloseSocket()
{

	if( m_socket != INVALID_SOCKET ) {
		
		printf( "JWSocket::closeSocket!!\n" );

		closesocket( m_socket );

		m_pRecvBuffer->ClearBuffer();
		m_pSendBuffer->ClearBuffer();

		m_socket = INVALID_SOCKET;

		return TRUE;
	}

	return FALSE;
}
