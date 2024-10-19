#ifndef __CDBQUEUE_PROC__
#define __CDBQUEUE_PROC__

#include "SOMain.h"
#include "MSSQLDB.h"



//������������������������������������������������������������������������������������������������������������������������
//
// class : DB ���� ť ó�� Ŭ���� 
//
//������������������������������������������������������������������������������������������������������������������������
class HQSQLDB;
class CDBQueue;
class CDBQueueProc: public CDBQueue
{	

private:
	CRITICAL_SECTION		m_csQueue;						// �� �����忡���� ó���� ���� CS 
	CDBQueueProc*			m_pNeighbor;					// ó������ ���ϴ� ������ ������ ť
	bool					m_bShutdown;					// ������ ���� ���� 
		
	char*					temp_query;						// �ӽ� ���� ���� ����  


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
	

		// DB ���� 
		HQSQLDB			  SQLProc;
		SQLProc.DBConnect( g_GameServerInfoParser.GetDB1IPString(),  g_GameServerInfoParser.GetDBName1String(), 
						   g_GameServerInfoParser.GetDBUserString(), "dhs1fk2dls3rj4tkd" );


		// ť ó������ ����� SQLProc ���� 
		pDBQueueProc->setSQLProc( &SQLProc );

		
		while( !pDBQueueProc->isShutdown() )
		{
			Sleep(10);

			pDBQueueProc->queueProc();		
		}


		// DB ���� 
		SQLProc.DBClose();

		printf(" [CDBQueueProc] DBQueueProcThread Ended.\n");

		return 0;
	}	
	//--------------------------------------------------------------------------------------------------------------


};


#endif


