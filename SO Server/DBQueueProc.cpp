#include <windows.h>
#include "_DBQueryDefine.h"
#include "DBQueue.h"
#include "DBQueueProc.h"



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ������ 
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
//	Desc : �Ҹ��� 
//--------------------------------------------------------------------------------------------
CDBQueueProc::~CDBQueueProc()
{
	Shutdown();

	delete temp_query;

	DeleteCriticalSection(&m_csQueue);
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ť ó������ ����� SQL Process�� ���� �Ѵ� 
//--------------------------------------------------------------------------------------------
void CDBQueueProc::setSQLProc(HQSQLDB* pSQL)
{
	m_pSQL = pSQL;
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ó������ ���ϴ� ������ ������ ť�� ���� �Ѵ� 
//--------------------------------------------------------------------------------------------
void CDBQueueProc::setNeighbor(CDBQueueProc* pNeighbor)
{
	m_pNeighbor = pNeighbor;
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ť�� ��Ŷ�� ���� �ִ´� 
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
//	Desc : ť���� ���ʴ�� �о�� ó�� �Ѵ� 
//--------------------------------------------------------------------------------------------
void CDBQueueProc::queueProc()
{
	EnterCriticalSection(&m_csQueue);

		
	char* data = NULL;
	while( data = read() )
	{	
						
		// ���� üũ 
		if( isAbleQuery(data) ) 
		{

			memcpy(temp_query, data, ((DBQUERY_HEADER*)data)->packetSize);
			
			LeaveCriticalSection(&m_csQueue);
				
			
			// ���� ���� 
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
//	Desc : ������ ó���� �ٸ� ť���� �ѱ�� 
//--------------------------------------------------------------------------------------------
bool CDBQueueProc::moveQueryToNeighbor(char* data)
{
	if( m_pNeighbor == NULL ) return FALSE;

	
	if( m_pNeighbor->isAbleQuery(data) ) 
	{
		m_pNeighbor->write( data, ((DBQUERY_HEADER*)data)->packetSize );
		return TRUE;
	}

	// �ٸ� ť���� ó���� �ѱ�� 
	return m_pNeighbor->moveQueryToNeighbor(data);
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ťó�� ������ ���� 
//--------------------------------------------------------------------------------------------
void CDBQueueProc::Shutdown()
{
	m_bShutdown = TRUE;
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ťó�� �������� ���� ���θ� �˾ƿ´� 
//--------------------------------------------------------------------------------------------
bool CDBQueueProc::isShutdown()
{
	return m_bShutdown;
}