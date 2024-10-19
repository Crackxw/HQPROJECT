//������������������������������������������������������������������������������	
// �ۼ���      : ������				
// ���� ������ : 2002. 6. 20		
//������������������������������������������������������������������������������	

#include "JXPacketQueue.h"		
#include "JXPacket.h"			

JXPacketQueue::JXPacketQueue()	
{
	m_pQueueBuf = NULL;	
	
	m_siBufSize = 0;	

	m_siBeginPos = 0;	
	m_siEndPos = 0;		
	m_siDataLen = 0;	
	m_siNumPacket = 0 ;

	InitializeCriticalSection( &m_cs );
}

JXPacketQueue::~JXPacketQueue()
{
	DeleteCriticalSection( &m_cs );

	if( m_pQueueBuf ) delete [] m_pQueueBuf;
}

void JXPacketQueue::Create( SI32 bufsize )
{
	EnterCriticalSection( &m_cs );

	if( m_pQueueBuf ) delete [] m_pQueueBuf;

	m_pQueueBuf = new char[ bufsize ];

	m_siBufSize = bufsize;
	
	m_siBeginPos = 0;
	m_siEndPos = 0;
	m_siDataLen = 0;

	LeaveCriticalSection( &m_cs );
}

void JXPacketQueue::Clear()
{
	EnterCriticalSection( &m_cs );

	m_siBeginPos = 0;	
	m_siEndPos = 0;		
	m_siDataLen = 0;	

	m_siNumPacket = 0 ;

	LeaveCriticalSection( &m_cs );
}

//������������������������������������������������������������������������������
// ��Ŷ�� ť�� ���� �ִ´�
//������������������������������������������������������������������������������

BOOL JXPacketQueue::Enqueue( JXPacket *pPacket )
{
	return Enqueue( pPacket->GetPacket() );
}


BOOL JXPacketQueue::Enqueue( char *pPacket )
{

	EnterCriticalSection( &m_cs );

	// local_siLenA : �� ��Ŷ ���� 
	// local_siLenB : ������ ���� ��� ��Ŷ�� �� �κ� ����
	// local_siLenC : ������ ���� ��� ��Ŷ�� �� �κ� ����
	
	UI16	usLen;
	memcpy( &usLen, pPacket, 2 );

	local_siLenA = usLen;

	// ���ۿ� ���� ������ ������ FALSE�� ��ȯ�Ѵ�
	if( local_siLenA > m_siBufSize - m_siDataLen ) {

		LeaveCriticalSection( &m_cs );
		
		return FALSE;	
	}

	if( m_siEndPos + local_siLenA > m_siBufSize ) {
		// ������ ���� �ؾ��ϸ�,
		
		local_siLenB = m_siBufSize - m_siEndPos;
		local_siLenC = local_siLenA - local_siLenB;

		memcpy( &m_pQueueBuf[ m_siEndPos ], pPacket, local_siLenB );

		pPacket += local_siLenB;

		memcpy( &m_pQueueBuf[ 0 ], pPacket, local_siLenC );

		m_siEndPos = local_siLenC;

	} else {
		// �ѹ��� ���� �����ϸ�,

		memcpy( &m_pQueueBuf[ m_siEndPos ], pPacket, local_siLenA );

		m_siEndPos += local_siLenA;

		if( m_siEndPos == m_siBufSize ) m_siEndPos = 0;
	}

	m_siDataLen += local_siLenA;

	++m_siNumPacket;

	LeaveCriticalSection( &m_cs );

	return TRUE;
}

//������������������������������������������������������������������������������
// ��Ŷ�� ť���� �����´�	
//������������������������������������������������������������������������������

BOOL JXPacketQueue::Dequeue( JXPacket *pPacket )
{

	EnterCriticalSection( &m_cs );

	BOOL bRet;
	char buf[ JXPACKET_MAX_SIZE ];

	bRet = __dequeue( (char *)buf );

	if( bRet ) pPacket->SetPacket( (char *)buf );

	LeaveCriticalSection( &m_cs );

	return bRet;
}

BOOL JXPacketQueue::Dequeue( char *pPacket )
{
	EnterCriticalSection( &m_cs );

	BOOL bRet = __dequeue( pPacket );

	LeaveCriticalSection( &m_cs );

	return bRet;
}

BOOL JXPacketQueue::__dequeue( char *pPacket )
{

	// ť�� ��� ������ FALSE�� ��ȯ�Ѵ� 
	if( m_siDataLen == 0 ) {
		return FALSE; 
	}

	UI16 usPacketSize;

	// ��Ŷ ũ�� �о� ���� 
	if( m_siBeginPos == m_siBufSize - 1 ) {
		// ��Ŷ ũ�� �����Ͱ� �и� �Ǿ� �ִ�
		
		*( (char*)&usPacketSize ) = m_pQueueBuf[ m_siBeginPos ];
		*( (char*)&usPacketSize + 1 ) = m_pQueueBuf[ 0 ];

	} else {
		// ��Ŷ ũ�� �����͸� �ѹ��� ���� �� �ִ�
		
		memcpy( &usPacketSize, &m_pQueueBuf[ m_siBeginPos ], 2 );
	}

	if( m_siBeginPos + (SI32)usPacketSize > m_siBufSize ) {
		// ������ �о�� �Ѵٸ�,

		local_siLenB = m_siBufSize - m_siBeginPos;
		local_siLenC = (SI32)usPacketSize - local_siLenB;

		memcpy( &pPacket[ 0 ], &m_pQueueBuf[ m_siBeginPos ], local_siLenB );	
		memcpy( &pPacket[ local_siLenB ], &m_pQueueBuf[ 0 ], local_siLenC );	

		m_siBeginPos = local_siLenC; 

	} else {
		// �ѹ��� ���� �� �ִٸ�,

		memcpy( pPacket, &m_pQueueBuf[ m_siBeginPos ], usPacketSize );
		
		m_siBeginPos += (SI32)usPacketSize;

		if( m_siBeginPos == m_siBufSize ) m_siBeginPos = 0;
	}

	m_siDataLen -= (SI32)usPacketSize;

	--m_siNumPacket;

	return TRUE;

}
