#include	<GSL.h>

#include "cltXsprQueue.h"


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: ������
//------------------------------------------------------------------------------------------------------------
cltXsprQueue::cltXsprQueue( SI32 siSize )
{
	m_pQueue	=	new strXsprQueueData[siSize];
	m_siLen		=	0;
	m_siMax		=	siSize;

	// ����ȭ ��ü �ʱ�ȭ
	InitializeCriticalSection( &m_cs );
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: �Ҹ���
//------------------------------------------------------------------------------------------------------------
cltXsprQueue::~cltXsprQueue()
{
	delete []m_pQueue;

	DeleteCriticalSection( &m_cs );
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc:
//------------------------------------------------------------------------------------------------------------
BOOL	cltXsprQueue::Peek()
{
	if( m_siLen > 0 )	return	TRUE;
	else				return	FALSE;
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc:
//------------------------------------------------------------------------------------------------------------
BOOL	cltXsprQueue::Enqueue( strXsprQueueData stData )
{
	EnterCriticalSection( &m_cs );

	if( m_siLen >= m_siMax ) { LeaveCriticalSection( &m_cs ); return FALSE; }

	// enqueue
	memcpy( m_pQueue + m_siLen, &stData, sizeof(strXsprQueueData) );

	m_siLen++;

	LeaveCriticalSection( &m_cs );

	return	TRUE;
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc:
//------------------------------------------------------------------------------------------------------------
BOOL	cltXsprQueue::Dequeue( strXsprQueueData &stData )
{
	EnterCriticalSection( &m_cs );

	if( m_siLen <= 0 ) { LeaveCriticalSection( &m_cs ); return FALSE; }

	// dequeue
	memcpy( &stData, m_pQueue, sizeof(strXsprQueueData) );
	memmove( m_pQueue, m_pQueue + 1, sizeof(strXsprQueueData) * (m_siLen-1) );

	m_siLen--;

	LeaveCriticalSection( &m_cs );

	return	TRUE;
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc:
//------------------------------------------------------------------------------------------------------------
SI32	cltXsprQueue::GetLength()
{
	return m_siLen;
}


//------------------------------------------------------------------------------------------------------------
// Name:
// Desc: pXspr�� ���� �����͸� ������ dequeue�Ѵ�.
//------------------------------------------------------------------------------------------------------------
BOOL	cltXsprQueue::StrongDequeue( XSPR *pXspr, strXsprQueueData &stData )
{
	EnterCriticalSection( &m_cs );

	for( SI32 i = 0; i < m_siLen; i++ )
	{		
		// pXspr�� key�� �Ͽ� data�� ã�´�.
		if( m_pQueue[i].pXspr == pXspr )
		{
			// pXspr�� ���� data�� ���� dequeue�Ѵ�.
			memcpy( &stData, m_pQueue + i, sizeof(strXsprQueueData) );
			memmove( m_pQueue + i, m_pQueue + i + 1, sizeof(strXsprQueueData) * (m_siLen - (i+1)) );

			m_siLen--;

			LeaveCriticalSection( &m_cs );
			return TRUE;
		}
	}

	LeaveCriticalSection( &m_cs );
	return FALSE;
}