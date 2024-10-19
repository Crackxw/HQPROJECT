//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━	
// 작성자      : 이제완				
// 최종 수정일 : 2002. 6. 20		
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━	

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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 패킷을 큐에 집어 넣는다
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

BOOL JXPacketQueue::Enqueue( JXPacket *pPacket )
{
	return Enqueue( pPacket->GetPacket() );
}


BOOL JXPacketQueue::Enqueue( char *pPacket )
{

	EnterCriticalSection( &m_cs );

	// local_siLenA : 쓸 패킷 길이 
	// local_siLenB : 나눠서 넣을 경우 패킷의 앞 부분 길이
	// local_siLenC : 나눠서 넣을 경우 패킷의 뒷 부분 길이
	
	UI16	usLen;
	memcpy( &usLen, pPacket, 2 );

	local_siLenA = usLen;

	// 버퍼에 여유 공간이 없으면 FALSE를 반환한다
	if( local_siLenA > m_siBufSize - m_siDataLen ) {

		LeaveCriticalSection( &m_cs );
		
		return FALSE;	
	}

	if( m_siEndPos + local_siLenA > m_siBufSize ) {
		// 나눠서 복사 해야하면,
		
		local_siLenB = m_siBufSize - m_siEndPos;
		local_siLenC = local_siLenA - local_siLenB;

		memcpy( &m_pQueueBuf[ m_siEndPos ], pPacket, local_siLenB );

		pPacket += local_siLenB;

		memcpy( &m_pQueueBuf[ 0 ], pPacket, local_siLenC );

		m_siEndPos = local_siLenC;

	} else {
		// 한번에 복사 가능하면,

		memcpy( &m_pQueueBuf[ m_siEndPos ], pPacket, local_siLenA );

		m_siEndPos += local_siLenA;

		if( m_siEndPos == m_siBufSize ) m_siEndPos = 0;
	}

	m_siDataLen += local_siLenA;

	++m_siNumPacket;

	LeaveCriticalSection( &m_cs );

	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 패킷을 큐에서 꺼내온다	
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

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

	// 큐가 비어 있으면 FALSE를 반환한다 
	if( m_siDataLen == 0 ) {
		return FALSE; 
	}

	UI16 usPacketSize;

	// 패킷 크기 읽어 오기 
	if( m_siBeginPos == m_siBufSize - 1 ) {
		// 패킷 크기 데이터가 분리 되어 있다
		
		*( (char*)&usPacketSize ) = m_pQueueBuf[ m_siBeginPos ];
		*( (char*)&usPacketSize + 1 ) = m_pQueueBuf[ 0 ];

	} else {
		// 패킷 크기 데이터를 한번에 읽을 수 있다
		
		memcpy( &usPacketSize, &m_pQueueBuf[ m_siBeginPos ], 2 );
	}

	if( m_siBeginPos + (SI32)usPacketSize > m_siBufSize ) {
		// 나눠서 읽어야 한다면,

		local_siLenB = m_siBufSize - m_siBeginPos;
		local_siLenC = (SI32)usPacketSize - local_siLenB;

		memcpy( &pPacket[ 0 ], &m_pQueueBuf[ m_siBeginPos ], local_siLenB );	
		memcpy( &pPacket[ local_siLenB ], &m_pQueueBuf[ 0 ], local_siLenC );	

		m_siBeginPos = local_siLenC; 

	} else {
		// 한번에 읽을 수 있다면,

		memcpy( pPacket, &m_pQueueBuf[ m_siBeginPos ], usPacketSize );
		
		m_siBeginPos += (SI32)usPacketSize;

		if( m_siBeginPos == m_siBufSize ) m_siBeginPos = 0;
	}

	m_siDataLen -= (SI32)usPacketSize;

	--m_siNumPacket;

	return TRUE;

}
