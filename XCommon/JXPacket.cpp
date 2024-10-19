//������������������������������������������������������������������������������
// �ۼ���      : ������				
// ���� ������ : 2002. 6. 20		
//������������������������������������������������������������������������������


#include <TCHAR.H>

#include "jxdef.h"
#include "JXPacket.h"



JXPacket::JXPacket()
{
}

JXPacket::~JXPacket()
{
}

//������������������������������������������������������������������������������
// �� �� : �ϼ��� ��Ŷ�� �Ѱܹ޾� ���ۿ� �����Ѵ�
//������������������������������������������������������������������������������

void JXPacket::SetPacket( char *pSrc )
{
	// ��Ŷ ũ��( �� �� 2byte )
	memcpy( &local_usLen, pSrc, 2 );

	m_siDataSize = (SI32)local_usLen;

	// ��Ŷ ������
	memcpy( m_pPacketBuf, pSrc, m_siDataSize );
		
	m_siReadPos = 2;

}


//������������������������������������������������������������������������������
// �� �� : ���۸� ������ ����
//������������������������������������������������������������������������������

void JXPacket::ClearPacket()
{
	m_siReadPos = 2;
	m_siDataSize = 2;
	memset( m_pPacketBuf, 0, 2 );	
}

//������������������������������������������������������������������������������
// �� �� : ���۸� ������ ����
//������������������������������������������������������������������������������

void JXPacket::ClearZeroPacket()
{
	m_siReadPos		= 0;
	m_siDataSize	= 0;
	memset(m_pPacketBuf, 0, sizeof(m_pPacketBuf));
}

//������������������������������������������������������������������������������
// �� �� : ��Ŷ�� �ϼ��Ѵ�
//������������������������������������������������������������������������������

void JXPacket::CompletePacket()
{
	local_usLen = (UI16)m_siDataSize;
	memcpy( m_pPacketBuf, &local_usLen, 2 );
}

void JXPacket::WriteValue( void *pSrc, int size )
{
	memcpy( &m_pPacketBuf[ m_siDataSize ], pSrc, size );

	m_siDataSize += size;
}


void JXPacket::WriteString( char *pSrc, int maxsize )
{

	
	local_siLen = strlen( pSrc ) + 1;

	if( local_siLen > maxsize ) local_siLen = maxsize;

	local_usLen = (UI16)local_siLen;

	memcpy( &m_pPacketBuf[ m_siDataSize ], &local_usLen, 2 );
	m_siDataSize += 2;

	memcpy( &m_pPacketBuf[ m_siDataSize ], pSrc, local_siLen );
	m_siDataSize += local_siLen;

	m_pPacketBuf[ m_siDataSize - 1 ] = NULL;

}

void JXPacket::WriteString2( char *pSrc, int siLen,int maxsize )
{
	local_siLen = siLen;
	
	if( local_siLen > maxsize ) local_siLen = maxsize;

	local_usLen = (UI16)local_siLen;

	memcpy( &m_pPacketBuf[ m_siDataSize ], &local_usLen, 2 );
	m_siDataSize += 2;

	memcpy( &m_pPacketBuf[ m_siDataSize ], pSrc, local_siLen );
	m_siDataSize += local_siLen;

	m_pPacketBuf[ m_siDataSize - 1 ] = NULL;
	

}



BOOL JXPacket::ReadValue( void *pDest, int size )
{

	if( m_siReadPos + size > m_siDataSize ) return FALSE;

	memcpy( pDest, &m_pPacketBuf[ m_siReadPos ], size );
	m_siReadPos += size;

	return TRUE;

}


BOOL JXPacket::ReadString( char *pDest, int maxsize )
{

	memcpy( &local_usLen, &m_pPacketBuf[ m_siReadPos ], 2 );
	m_siReadPos += 2;

	local_siLen = (SI32)local_usLen;

	if( local_siLen == 0 ) return FALSE;

	if( local_siLen > maxsize ) {

		memcpy( pDest, &m_pPacketBuf[ m_siReadPos ], maxsize );
		pDest[ maxsize - 1 ] = NULL;

	} else {
		
		memcpy( pDest, &m_pPacketBuf[ m_siReadPos ], local_siLen );
		pDest[ local_siLen - 1 ] = NULL;
	}

	m_siReadPos += local_siLen;

	return TRUE;

}
SI32 JXPacket::ReadString2( char *pDest, int maxsize )
{
	SI32 ret;

	memcpy( &local_usLen, &m_pPacketBuf[ m_siReadPos ], 2 );
	m_siReadPos += 2;

	local_siLen = (SI32)local_usLen;
	ret = local_siLen;

	if( local_siLen == 0 ) return 0;

	if( local_siLen > maxsize ) {

		memcpy( pDest, &m_pPacketBuf[ m_siReadPos ], maxsize );
		pDest[ maxsize - 1 ] = NULL;

	} else {
		
		memcpy( pDest, &m_pPacketBuf[ m_siReadPos ], local_siLen );
		pDest[ local_siLen - 1 ] = NULL;
	}

	m_siReadPos += local_siLen;

	return ret;

}

