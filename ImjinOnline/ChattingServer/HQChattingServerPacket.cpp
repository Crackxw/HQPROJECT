/************************************************************************************************
	파일명 : HQChattingServerPacket.cpp
	담당자 : 이윤석

	Chatting Server와 주고받는 Message Class
************************************************************************************************/
#include <GSL.h>
//#include <winsock2.h>
//#include <stdio.h>
//#include <conio.h>
//#include "Directives.h"
#include <winbase.h>


#include "HQChattingServerClientSocket.h"


HQPacket::HQPacket()
{
	ClearPacket();
}

HQPacket::~HQPacket()
{
}

void HQPacket::RecvPacket( char *pSrc )
{
	memcpy( &m_usSize, pSrc, 2 );
	//m_usSize += 2;
	
	memcpy( m_pPacket, pSrc, m_usSize );
		
	m_usReadPos = 2;
}

void HQPacket::ClearPacket()
{
	m_usReadPos = 2;
	m_usSize = 2;
	memset( m_pPacket, 0, 2 );	
}

void HQPacket::CompletePacket()
{
	UI16 size = m_usSize;
	memcpy( m_pPacket, &size, 2 );
}

void HQPacket::WriteValue( void *pSrc, int size )
{
	memcpy( &m_pPacket[ m_usSize ], pSrc, size );

	m_usSize += size;
}

void HQPacket::WriteString( char *pSrc, int maxsize )
{
	
	UI16 len = strlen( pSrc ) + 1;

	if( len > maxsize ) len = maxsize;

	memcpy( &m_pPacket[ m_usSize ], &len, 2 );
	m_usSize += 2;

	memcpy( &m_pPacket[ m_usSize ], pSrc, len );
	m_usSize += len;

	m_pPacket[ m_usSize - 1 ] = NULL;

}

BOOL HQPacket::ReadValue( void *pDest, int size )
{

	if( m_usReadPos + size > m_usSize ) return FALSE;

	memcpy( pDest, &m_pPacket[ m_usReadPos ], size );
	m_usReadPos += size;

	return TRUE;

}

BOOL HQPacket::ReadString( char *pDest, int maxsize )
{

	UI16 len;

	memcpy( &len, &m_pPacket[ m_usReadPos ], 2 );
	m_usReadPos += 2;

	if( len == 0 ) return FALSE;

	if( len > maxsize ) len = maxsize;

	if( m_usReadPos + len > m_usSize ) return FALSE;

	memcpy( pDest, &m_pPacket[ m_usReadPos ], len );
	m_usReadPos += len;

	pDest[ len - 1 ] = NULL;

	return TRUE;
}
