// GetSendParam 과 Completion은 반드시 짝을 이루어야 한다 

#include "JWSendBuffer.h"

JWSendBuffer::JWSendBuffer()
{

	m_pSendBuf = NULL;

	m_siBufSize = 0;

	InitializeCriticalSection( &m_cs );

}

JWSendBuffer::~JWSendBuffer()
{

	if( m_pSendBuf ) delete [] m_pSendBuf;

	DeleteCriticalSection( &m_cs );

}


void JWSendBuffer::ClearBuffer()
{

	EnterCriticalSection( &m_cs );	

	m_siBeginPos = 0;	
	m_siEndPos = 0;		
	m_siDataLen = 0;	

	m_bComplete = TRUE;

	LeaveCriticalSection( &m_cs );

}

void JWSendBuffer::CreateSendBuffer( SI32 size )
{

	EnterCriticalSection( &m_cs );	

	if( m_pSendBuf ) delete [] m_pSendBuf;

	m_pSendBuf = new char[ size ];

	m_siBufSize = size;
	
	m_siBeginPos = 0;
	m_siEndPos = 0;
	m_siDataLen = 0;

	m_bComplete = TRUE;
	
	LeaveCriticalSection( &m_cs );

}

BOOL JWSendBuffer::GetSendParam( char **pSendPtr, SI32 *psiDataLen )
{
	EnterCriticalSection( &m_cs );

	if( m_bComplete == FALSE || m_siDataLen == 0 ) {

		*psiDataLen = 0;

		LeaveCriticalSection( &m_cs );

		return FALSE;
	}
	
	*pSendPtr = m_pSendBuf + m_siBeginPos;

	if( m_siBeginPos < m_siEndPos ){
	
		*psiDataLen = m_siDataLen;
	
	} else {

		*psiDataLen = m_siBufSize - m_siBeginPos;
	}

	m_bComplete = FALSE;

	LeaveCriticalSection( &m_cs );

	return TRUE;
}

void JWSendBuffer::Completion( SI32 siSentBytes )
{

	EnterCriticalSection( &m_cs );		

	m_siBeginPos += siSentBytes;
	m_siDataLen -= siSentBytes;

	if( m_siBeginPos == m_siBufSize ) m_siBeginPos = 0;

	m_bComplete = TRUE;


	LeaveCriticalSection( &m_cs );
}


BOOL JWSendBuffer::Write( char *pPacket )
{

	EnterCriticalSection( &m_cs );	

	// local_siLenA : 쓸 패킷 길이 
	// local_siLenB : 나눠서 넣을 경우 패킷의 앞 부분 길이
	// local_siLenC : 나눠서 넣을 경우 패킷의 뒷 부분 길이
	
	local_siLenA = (SI32)( *(UI16 *)pPacket );

	// 버퍼에 여유 공간이 없으면 FALSE를 반환한다
	if( local_siLenA > m_siBufSize - m_siDataLen ) 
	{
		char szTemp[1024] = {0,};
		sprintf(szTemp, "[쓰기버퍼에 여유 용량 없음!!][버퍼 사이즈: %d][남은 사이즈:%d][들어온 사이즈:%d]\n",
			    m_siBufSize, m_siBufSize - m_siDataLen, local_siLenA);

		OutputDebugString(szTemp);
//		OutputDebugString( "쓰기버퍼에 여유 용량 없음\n" );

		LeaveCriticalSection( &m_cs );
		
		return FALSE;	
	}

	if( m_siEndPos + local_siLenA > m_siBufSize ) {
		// 나눠서 복사 해야하면,
		
		local_siLenB = m_siBufSize - m_siEndPos;
		local_siLenC = local_siLenA - local_siLenB;

		memcpy( m_pSendBuf + m_siEndPos, pPacket, local_siLenB );

		pPacket += local_siLenB;

		memcpy( m_pSendBuf, pPacket, local_siLenC );

		m_siEndPos = local_siLenC;

	} else {
		// 한번에 복사 가능하면,

		memcpy( m_pSendBuf + m_siEndPos, pPacket, local_siLenA );

		m_siEndPos += local_siLenA;

		if( m_siEndPos == m_siBufSize ) m_siEndPos = 0;
	}

	m_siDataLen += local_siLenA;
	
	LeaveCriticalSection( &m_cs );

	return TRUE;
}

