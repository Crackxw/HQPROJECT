
#include "JWFQueueMISO.h"

JWFQueueMISO::JWFQueueMISO()
{

	m_pBuf = NULL;

	m_siBufSize = 0;
	m_siNumItem = 0;

	InitializeCriticalSection( &m_cs );	
}

JWFQueueMISO::~JWFQueueMISO()
{

	if( m_pBuf ) delete [] m_pBuf;

	DeleteCriticalSection( &m_cs );	
}

void JWFQueueMISO::ClearQueue()
{
	EnterCriticalSection( &m_cs );	

	m_siBeginPos = 0;
	m_siEndPos = 0;	
	m_siNumItem = 0;

	LeaveCriticalSection( &m_cs );
}

void JWFQueueMISO::CreateQueue( SI32 size )
{
	EnterCriticalSection( &m_cs );	

	if( m_pBuf ) delete [] m_pBuf;

	m_pBuf = new DWORD[ size ];

	m_siBufSize = size;
	
	m_siBeginPos = 0;
	m_siEndPos = 0;
	m_siNumItem = 0;
	
	LeaveCriticalSection( &m_cs );	
}

BOOL JWFQueueMISO::Enqueue( DWORD dwItem )
{
	EnterCriticalSection( &m_cs );		

	if( m_siBufSize == m_siNumItem ) {

		LeaveCriticalSection( &m_cs );	

		return FALSE;
	}

	m_pBuf[ m_siEndPos ] = dwItem;

	m_siEndPos++;

	if( m_siEndPos == m_siBufSize ) m_siEndPos = 0;

	InterlockedIncrement( (long *)&m_siNumItem );
	
	LeaveCriticalSection( &m_cs );

	return TRUE;
}

BOOL JWFQueueMISO::Dequeue( DWORD *pOut_dwItem )
{
	if( m_siNumItem == 0 ) return FALSE;

	*pOut_dwItem = m_pBuf[ m_siBeginPos ];

	m_siBeginPos++;

	if( m_siBeginPos == m_siBufSize ) m_siBeginPos = 0;

	InterlockedDecrement( (long *)&m_siNumItem );

	return TRUE;
}


