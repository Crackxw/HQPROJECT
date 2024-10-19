#include <windows.h>
#include "_DBQueryDefine.h"
#include "DBQueue.h"
#include "DBQueueProc.h"



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 생성자 
//--------------------------------------------------------------------------------------------
CDBQueueProc::CDBQueueProc(int a, int b): CDBQueue(a,b)
{
	temp_query	= new char[ PACKET_MAX_SIZE ];
	memset( temp_query, 0, sizeof(temp_query) );

	m_pNeighbor		= NULL;
	m_pSQL			= NULL;
	m_bShutdown		= FALSE;

	InitializeCriticalSection(&m_csQueue);
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 소멸자 
//--------------------------------------------------------------------------------------------
CDBQueueProc::~CDBQueueProc()
{
	Shutdown();

	delete temp_query;

	DeleteCriticalSection(&m_csQueue);
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 큐 처리에서 사용할 SQL Process를 설정 한다 
//--------------------------------------------------------------------------------------------
void CDBQueueProc::setSQLProc(HQSQLDB* pSQL)
{
	m_pSQL = pSQL;
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 처리하지 못하는 쿼리를 전달할 큐를 설정 한다 
//--------------------------------------------------------------------------------------------
void CDBQueueProc::setNeighbor(CDBQueueProc* pNeighbor)
{
	m_pNeighbor = pNeighbor;
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 큐에 패킷을 집어 넣는다 
//--------------------------------------------------------------------------------------------
int CDBQueueProc::write(char* data, int size)
{
	EnterCriticalSection(&m_csQueue);

	int result = CDBQueue::write(data, size);

	LeaveCriticalSection(&m_csQueue);

	return result;
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 큐에서 차례대로 읽어와 처리 한다 
//--------------------------------------------------------------------------------------------
void CDBQueueProc::queueProc()
{
	EnterCriticalSection(&m_csQueue);

		
	char* data = NULL;
	while( data = read() )
	{	
						
		// 쿼리 체크 
		if( isAbleQuery(data) ) 
		{

			memcpy(temp_query, data, ((DBQUERY_HEADER*)data)->packetSize);
			
			LeaveCriticalSection(&m_csQueue);
				
			
			// 쿼리 수행 
			//----------------------------------------------------------------
			setDBInfo( ((DBQUERY_PACKET*)temp_query)->header.command, 
					   ((DBQUERY_PACKET*)temp_query)->data );			
			//----------------------------------------------------------------
			
			EnterCriticalSection(&m_csQueue);			

		}
		else
			moveQueryToNeighbor(data);
	}		

	LeaveCriticalSection(&m_csQueue);	
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 쿼리의 처리를 다른 큐에게 넘긴다 
//--------------------------------------------------------------------------------------------
bool CDBQueueProc::moveQueryToNeighbor(char* data)
{
	if( m_pNeighbor == NULL ) return FALSE;

	
	if( m_pNeighbor->isAbleQuery(data) ) 
	{
		m_pNeighbor->write( data, ((DBQUERY_HEADER*)data)->packetSize );
		return TRUE;
	}

	// 다른 큐에게 처리를 넘긴다 
	return m_pNeighbor->moveQueryToNeighbor(data);
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 큐처리 쓰레드 종료 
//--------------------------------------------------------------------------------------------
void CDBQueueProc::Shutdown()
{
	m_bShutdown = TRUE;
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 큐처리 쓰레드의 종료 여부를 알아온다 
//--------------------------------------------------------------------------------------------
bool CDBQueueProc::isShutdown()
{
	return m_bShutdown;
}