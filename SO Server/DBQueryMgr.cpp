#include <windows.h>
#include "_DBQueryDefine.h"
#include "DBQueue.h"
#include "DBQueueProc.h"
#include "DBGateQueue.h"
#include "DBSubQueue.h"
#include "DBQueryMgr.h"



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 생성자
//--------------------------------------------------------------------------------------------
CDBQueryMgr::CDBQueryMgr()
{				
	memset( m_depository,  0, sizeof(m_depository) );
	m_depositoryPos = 0;	

	m_pGateQueue = NULL;
	m_pSubQueue	 = NULL;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 소멸자
//--------------------------------------------------------------------------------------------
CDBQueryMgr::~CDBQueryMgr()
{	
	if( m_pGateQueue ) delete m_pGateQueue;
	if( m_pSubQueue  ) delete m_pSubQueue;
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 큐를 생성 한다 
//--------------------------------------------------------------------------------------------
void CDBQueryMgr::createQueue()
{
	m_pGateQueue = new CDBGateQueue( MAX_QUEUE, MAX_PACKET );
	m_pSubQueue  = new CDBSubQueue ( MAX_QUEUE, MAX_PACKET );

	m_pGateQueue->setNeighbor( m_pSubQueue );	
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 보관함을 모두 비운다 
//--------------------------------------------------------------------------------------------
void CDBQueryMgr::emptiedDepository()
{
	if(m_depositoryPos > 0)
	{		
		if( m_pGateQueue->write( m_depository, m_depositoryPos ) > -1 )					
		{			
			m_depositoryPos = 0;			
		}	
		else
		{
			
			printf(" DB query gate queue han been fulled.\n" );			
		
			while( m_pGateQueue->write( m_depository, m_depositoryPos ) == -1 )
			{
				Sleep(60000); 							
			}

			m_depositoryPos = 0;		
			
			printf(" DB query write successed \n" );			
		}
	}
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 보관함에 쿼리를 보관한다 
//--------------------------------------------------------------------------------------------
void CDBQueryMgr::writeToDepository( char* header, int headerSize,
  									 char* data,   int dataSize )
{
	int size = headerSize + dataSize;
	
	if(size > 0)
	{			 
		if(m_depositoryPos + size <= MAX_QUEUE)	
		{		 				
			if( header )
			{
				memcpy(m_depository + m_depositoryPos, header, headerSize);
				m_depositoryPos += headerSize;		
			}

			if( data )
			{
				memcpy(m_depository + m_depositoryPos, data, dataSize);
				m_depositoryPos += dataSize;		
			}
		}
		else
		{					
			printf(" DB 쿼리 보관함이 가득 찼습니다 \n" );
			getch();

			exit(-1);
		}
	}
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 보관함에 쿼리를 보관한다 
//--------------------------------------------------------------------------------------------
void CDBQueryMgr::createQueueThread()
{
	unsigned int threadID;
	_beginthreadex(NULL, 0, CDBQueueProc::_DBQueueProcThread, (void*)m_pGateQueue, 0, &threadID);	
	_beginthreadex(NULL, 0, CDBQueueProc::_DBQueueProcThread, (void*)m_pSubQueue,  0, &threadID);
}
