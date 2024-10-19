
#include "JWRecvBuffer.h"

JWRecvBuffer::JWRecvBuffer()
{
	m_pRecvBuf = NULL;

	m_pBasePtr = NULL;
	m_pBeginPtr = NULL;
	m_pCompletePtr = NULL;
	m_pLastPtr = NULL;
	m_pEndPtr = NULL;

	m_siNumPacket = 0;

	m_bRemovable = FALSE;

	InitializeCriticalSection( &m_cs );

}

JWRecvBuffer::~JWRecvBuffer()
{

	if( m_pRecvBuf ) delete [] m_pRecvBuf;

	DeleteCriticalSection( &m_cs );

}

void JWRecvBuffer::ClearBuffer()
{

	EnterCriticalSection( &m_cs );	

	m_pBeginPtr = m_pBasePtr;

	m_pLastPtr = m_pBasePtr;

	m_pCompletePtr = m_pBasePtr;

	m_siNumPacket = 0;

	m_bRemovable = FALSE;

	LeaveCriticalSection( &m_cs );
}

void JWRecvBuffer::CreateRecvBuffer( SI32 size )
{

	EnterCriticalSection( &m_cs );	

	if( m_pRecvBuf ) delete [] m_pRecvBuf;

	m_pRecvBuf = new char[ JWPACKET_MAX_SIZE + size ];

	m_pBasePtr = m_pRecvBuf + JWPACKET_MAX_SIZE;
	m_pEndPtr = m_pBasePtr + size;
	m_pLastPtr = m_pBasePtr;
	m_pCompletePtr = m_pBasePtr;
	
	m_siNumPacket = 0;
	
	m_bRemovable = FALSE;

	LeaveCriticalSection( &m_cs );	

}

void JWRecvBuffer::GetRecvParam( char **pRecvPtr, SI32 *psiFreeSize )
{
	
	EnterCriticalSection( &m_cs );	
 
	*pRecvPtr = m_pLastPtr;

	if( m_pBeginPtr == m_pLastPtr && m_siNumPacket > 0  ) {

		OutputDebugString( "읽기버퍼 꽉참\n" );

		*psiFreeSize = 0;

	} else if( m_pLastPtr < m_pBeginPtr ) {

		*psiFreeSize = (SI32)( m_pBeginPtr - m_pLastPtr );

	} else {

		*psiFreeSize = (SI32)( m_pEndPtr - m_pLastPtr );
	}

	LeaveCriticalSection( &m_cs );
}


BOOL JWRecvBuffer::Completion( SI32 siRecvdBytes )
{	

	EnterCriticalSection( &m_cs );	

	local_siNumPacket = 0;

	local_pCheckPtr = m_pCompletePtr;

	m_pLastPtr += siRecvdBytes;

	while( local_pCheckPtr < m_pLastPtr ) {

		local_pInterestPtr = local_pCheckPtr;

		if( m_pLastPtr - local_pCheckPtr < 2 ) {

			if( m_pLastPtr == m_pEndPtr ) {

				m_pCompletePtr = m_pBasePtr - 1;

				*m_pCompletePtr = *local_pCheckPtr;

				m_pLastPtr = m_pBasePtr;
			}
			else
			{
				m_pCompletePtr	=	local_pInterestPtr;
			}

			goto end;
		}

		// 메시지 크기 읽어 오기
		local_usDataSize = *(UI16 *)local_pCheckPtr;

		if( local_usDataSize < 2 ) {
			
			printf( "MIN SIZE ERROR --------------------\n" );

			OutputDebugString( "최소패킷크기오류!\n" );

			LeaveCriticalSection( &m_cs );

			return FALSE;
		}

		if( local_usDataSize > JWPACKET_MAX_SIZE ) {
			// 메시지 최대 크기 보다 큰 값이면, 

			printf( "MAX SIZE ERROR --------------------\n" );

			OutputDebugString( "최대패킷크기초과!\n" );

			LeaveCriticalSection( &m_cs );

			return FALSE;
		}

		local_pCheckPtr += local_usDataSize;

		++local_siNumPacket;
	} 


	// m_pCompletePtr 설정

	if( local_pCheckPtr == m_pLastPtr ) {
		// 완성일 경우
		
		if( local_pCheckPtr == m_pEndPtr ) m_pCompletePtr = m_pLastPtr = m_pBasePtr;
		else m_pCompletePtr = local_pCheckPtr;

	} else {
		// 미완성일 경우

		if( m_pLastPtr == m_pEndPtr ) {

			local_siTemp = m_pEndPtr - local_pInterestPtr;

			m_pCompletePtr = m_pBasePtr - local_siTemp;

			memcpy( m_pCompletePtr, local_pInterestPtr, local_siTemp );

			m_pLastPtr = m_pBasePtr;

		} else {

			m_pCompletePtr = local_pInterestPtr;
		}

		--local_siNumPacket;
	}

end:

	m_siNumPacket += local_siNumPacket;

	LeaveCriticalSection( &m_cs );

	return TRUE;

}


char* JWRecvBuffer::GetFirstPacket()
{

	EnterCriticalSection( &m_cs );	

	if( m_siNumPacket == 0 ) {

		LeaveCriticalSection( &m_cs );

		return NULL;
	}

	UI16 usDataSize;

	if( m_pBeginPtr == m_pEndPtr - 1 ) {

		*( m_pBasePtr - 1 ) = *m_pBeginPtr;

		m_pBeginPtr = m_pBasePtr - 1;
	
	} else {

		usDataSize = *(UI16 *)m_pBeginPtr;

		if( m_pBeginPtr + usDataSize > m_pEndPtr ) {

			local_siTemp = (SI32)( m_pEndPtr - m_pBeginPtr );

			memcpy( m_pBasePtr - local_siTemp, m_pBeginPtr, local_siTemp );

			m_pBeginPtr = m_pBasePtr - local_siTemp;

		}

	}

	m_bRemovable = TRUE;

	LeaveCriticalSection( &m_cs );

	return m_pBeginPtr;
}

void JWRecvBuffer::RemoveFirstPacket()
{

	UI16 usDataSize;

	if( m_bRemovable == FALSE ) return;

	EnterCriticalSection( &m_cs );

	--m_siNumPacket;

	usDataSize = *(UI16 *)m_pBeginPtr;
	
	m_pBeginPtr += usDataSize;

	if( m_pBeginPtr == m_pEndPtr ) m_pBeginPtr = m_pBasePtr;

	LeaveCriticalSection( &m_cs );

}

