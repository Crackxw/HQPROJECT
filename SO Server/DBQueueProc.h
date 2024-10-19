#ifndef __CDBQUEUE_PROC__
#define __CDBQUEUE_PROC__

#include "SOMain.h"
#include "MSSQLDB.h"



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
// class : DB 쿼리 큐 처리 클래스 
//
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class HQSQLDB;
class CDBQueue;
class CDBQueueProc: public CDBQueue
{	

private:
	CRITICAL_SECTION		m_csQueue;						// 각 쓰레드에서의 처리를 위한 CS 
	CDBQueueProc*			m_pNeighbor;					// 처리하지 못하는 쿼리를 전달할 큐
	bool					m_bShutdown;					// 쓰레드 종료 변수 
		
	char*					temp_query;						// 임시 쿼리 보관 변수  


protected:
	HQSQLDB*				m_pSQL;



public:
	CDBQueueProc(int queue_max_size, int packet_max_size);
	~CDBQueueProc();


private:	
	bool			moveQueryToNeighbor(char* data);


protected:
	virtual void	setDBInfo(DWORD command, char* data)	= 0;	
	virtual bool	isAbleQuery(char* data)					= 0;	
	

public:
	void			queueProc();		

	int				write(char* data, int size);		

	void			setNeighbor(CDBQueueProc* pNeighbor);
	void			setSQLProc(HQSQLDB* pSQL);
	
	void			Shutdown();
	bool			isShutdown();




	//--------------------------------------------------------------------------------------------------------------
	inline static unsigned int __stdcall _DBQueueProcThread(void* pArg)
	{				
		printf(" [CDBQueueProc] DBQueueProcThread Start.....\n");
													
		CDBQueueProc*	  pDBQueueProc;				
		pDBQueueProc = static_cast<CDBQueueProc*>(pArg);						
	

		// DB 연결 
		HQSQLDB			  SQLProc;
		SQLProc.DBConnect( g_GameServerInfoParser.GetDB1IPString(),  g_GameServerInfoParser.GetDBName1String(), 
						   g_GameServerInfoParser.GetDBUserString(), "dhs1fk2dls3rj4tkd" );


		// 큐 처리에서 사용할 SQLProc 설정 
		pDBQueueProc->setSQLProc( &SQLProc );

		
		while( !pDBQueueProc->isShutdown() )
		{
			Sleep(10);

			pDBQueueProc->queueProc();		
		}


		// DB 종료 
		SQLProc.DBClose();

		printf(" [CDBQueueProc] DBQueueProcThread Ended.\n");

		return 0;
	}	
	//--------------------------------------------------------------------------------------------------------------


};


#endif


