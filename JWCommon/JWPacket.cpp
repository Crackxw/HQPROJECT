
#include "JWPacket.h"

JWPacket::JWPacket()
{

}

JWPacket::~JWPacket()
{

}


void JWPacket::SetPacket( char *pSrc )
{
	// 패킷 크기( 맨 앞 2byte )
	m_siDataSize = (SI32)( *(UI16 *)pSrc );

	// 패킷 데이터
	memcpy( m_pPacketBuf, pSrc, m_siDataSize );
		
	m_siReadPos = 2;

}



void JWPacket::ClearPacket()
{
	m_siReadPos = 2;
	m_siDataSize = 2;

	*(UI16 *)m_pPacketBuf = 0;
}


void JWPacket::CompletePacket()
{
	*(UI16 *)m_pPacketBuf = (UI16)m_siDataSize;
}

void JWPacket::WriteValue( void *pSrc, int size )
{
	memcpy( m_pPacketBuf + m_siDataSize, pSrc, size );

	m_siDataSize += size;
}


void JWPacket::WriteString( char *pSrc, int maxsize )
{
	
	local_siLen = strlen( pSrc ) + 1;

	if( local_siLen > maxsize ) local_siLen = maxsize;

	*(UI16 *)( m_pPacketBuf + m_siDataSize ) = (UI16)local_siLen;
	m_siDataSize += 2;

	memcpy( m_pPacketBuf + m_siDataSize, pSrc, local_siLen );
	m_siDataSize += local_siLen;

	m_pPacketBuf[ m_siDataSize - 1 ] = NULL;

}


BOOL JWPacket::ReadValue( void *pDest, int size )
{

	memcpy( pDest, m_pPacketBuf + m_siReadPos, size );
	m_siReadPos += size;

	return TRUE;

}


BOOL JWPacket::ReadString( char *pDest, int maxsize )
{

	local_siLen = (SI32)( *(UI16 *)( m_pPacketBuf + m_siReadPos ) );

	m_siReadPos += 2;

	if( local_siLen == 0 ) return FALSE;

	if( local_siLen > maxsize ) {

		memcpy( pDest, m_pPacketBuf + m_siReadPos, maxsize );

		pDest[ maxsize - 1 ] = NULL;
		
	} else {

		memcpy( pDest, m_pPacketBuf + m_siReadPos, local_siLen );

		pDest[ local_siLen - 1 ] = NULL;

	}
		
	m_siReadPos += local_siLen;

	return TRUE;

}
