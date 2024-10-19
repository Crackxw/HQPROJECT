// GetSendParam �� Completion�� �ݵ�� ¦�� �̷��� �Ѵ� 

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

	// local_siLenA : �� ��Ŷ ���� 
	// local_siLenB : ������ ���� ��� ��Ŷ�� �� �κ� ����
	// local_siLenC : ������ ���� ��� ��Ŷ�� �� �κ� ����
	
	local_siLenA = (SI32)( *(UI16 *)pPacket );

	// ���ۿ� ���� ������ ������ FALSE�� ��ȯ�Ѵ�
	if( local_siLenA > m_siBufSize - m_siDataLen ) 
	{
		char szTemp[1024] = {0,};
		sprintf(szTemp, "[������ۿ� ���� �뷮 ����!!][���� ������: %d][���� ������:%d][���� ������:%d]\n",
			    m_siBufSize, m_siBufSize - m_siDataLen, local_siLenA);

		OutputDebugString(szTemp);
//		OutputDebugString( "������ۿ� ���� �뷮 ����\n" );

		LeaveCriticalSection( &m_cs );
		
		return FALSE;	
	}

	if( m_siEndPos + local_siLenA > m_siBufSize ) {
		// ������ ���� �ؾ��ϸ�,
		
		local_siLenB = m_siBufSize - m_siEndPos;
		local_siLenC = local_siLenA - local_siLenB;

		memcpy( m_pSendBuf + m_siEndPos, pPacket, local_siLenB );

		pPacket += local_siLenB;

		memcpy( m_pSendBuf, pPacket, local_siLenC );

		m_siEndPos = local_siLenC;

	} else {
		// �ѹ��� ���� �����ϸ�,

		memcpy( m_pSendBuf + m_siEndPos, pPacket, local_siLenA );

		m_siEndPos += local_siLenA;

		if( m_siEndPos == m_siBufSize ) m_siEndPos = 0;
	}

	m_siDataLen += local_siLenA;
	
	LeaveCriticalSection( &m_cs );

	return TRUE;
}

