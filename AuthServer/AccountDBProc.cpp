#include <winsock2.h>
#include <stdio.h>

// CMSSQL
#define  DBNTWIN32
#include <sqlfront.h>
#include <sqldb.h>
#include "../kojimodo/utility/MSSQL.h"


// CWorkQueue
#include "../kojimodo/utility/Queue.h"
#include "WorkQueue.h"


// CGameServer
#include "../kojimodo/iocp/AcceptSocket.h"
#include "GameServer.h"


// CGameServerMgr
#include "../kojimodo/iocp/ListenSocket.h"
#include "../kojimodo/utility/Singleton.h"
#include "GameServerMgr.h"


// CWorkMgr
#include "WorkMgr.h"


// CAccountDBProc
#include "AccountDBProc.h"


// CAuthInfoParser
#include "../kojimodo/utility/Singleton.h"
#include "AuthInfoParser.h"


// CAuthorizer
#include "Authorizer.h"
#include "Gash.h"
#include "Ras.h"




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 생 성 자 
//--------------------------------------------------------------------------------------------
CAccountDBProc::CAccountDBProc()
{
	m_queryBuffer = new char[ WORK::MAX_PACKETSIZE ];
	m_AccountDB	  = new CMSSQL();

	m_pAuthorizer = CAuthInfoParser::P()->createAuthorizer();


	m_AccountDB->Connect( CAuthInfoParser::P()->getAccountDB_IP()    ,
						  CAuthInfoParser::P()->getAccountDB_Name()  ,
						  CAuthInfoParser::P()->getAccountDB_ID()	 ,
						  CAuthInfoParser::P()->getAccountDB_PW()      );					
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 소 멸 자 
//--------------------------------------------------------------------------------------------
CAccountDBProc::~CAccountDBProc()
{
	
	if( m_AccountDB ) {

		m_AccountDB->Dissconnect();
		delete m_AccountDB;
	}

	if( m_pAuthorizer ) {

		delete m_pAuthorizer;
	}

	if( m_queryBuffer ) {

		delete m_queryBuffer;
	}	
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 주기적인 작업 처리 
//--------------------------------------------------------------------------------------------
void CAccountDBProc::processPoll()
{
	_dword		lastRecoverTime = 0;

	while( !CAccountDBWorkMgr::P()->isShutdown() )
	{
		Sleep(10);
		
		processQuery();

		if( timeGetTime() - lastRecoverTime  >= 300000 )
		{
			autoRecovery();

			lastRecoverTime = timeGetTime();
		}
	}
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
void CAccountDBProc::processQuery()
{	
	
	//-- packetSize 
	int	packetSize = CAccountDBWorkMgr::P()->getQuery(m_queryBuffer, WORK::MAX_PACKETSIZE);
	

	if( packetSize > 0 )
	{
		
		//-- header 
		WORK::sPacketHeader* header = (WORK::sPacketHeader*)m_queryBuffer;
		

		//-- data, dataSize 
		char*	data	 = m_queryBuffer + sizeof(WORK::sPacketHeader);
		int	    dataSize = packetSize    - sizeof(WORK::sPacketHeader);
	
	
		//-- command 
		DWORD	command;
		memcpy(&command, data, sizeof(command));	


		using namespace LOGIN;

		switch( command )
		{					
			case AUTH::REQUEST_RESET:	
				printf("REQUEST_RESET\n");
				recvRequestReset( header, (AUTH::sRequestReset*)data );				
				break;

		}


	}
	
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc :  
//--------------------------------------------------------------------------------------------
void CAccountDBProc::autoRecovery()
{
	printf("-- AutoRecovery...............Start\n");

	int result = LOGIN::UNKNOWN_FAIL;
	

	char		id		  [LOGIN::ID_MAX_LENGTH];
	char		password  [LOGIN::PW_MAX_LENGTH];
	char		ip		  [16];
	
	_dword		dbAccount = 0;
	_dword		idNumber  = 0;



	/* 최대 동시에 6000명의 복구를 할수 있다 */
	
	_dword		successList[6000];
	int			successCount = 0;



	if( m_AccountDB->SQLRun("EXEC SPHQ_OnGetListOfAuthFailList") )
	{			
		
		if(m_AccountDB->DBResults() == SUCCEED)
		{

			m_AccountDB->DBBind(1, NTBSTRINGBIND,			(DBCHAR)0, (BYTE*)id);
			m_AccountDB->DBBind(2, NTBSTRINGBIND,			(DBCHAR)0, (BYTE*)password);			
			m_AccountDB->DBBind(3, NTBSTRINGBIND,			(DBCHAR)0, (BYTE*)ip);			
			m_AccountDB->DBBind(4, INTBIND,					(DBINT) 0, (BYTE*)&dbAccount);			
			

			while( m_AccountDB->DBNextRow() != NO_MORE_ROWS )
			{		
				if( successCount < 6000 )
				{
					result = m_pAuthorizer->reqLogout( id, password, ip, &idNumber );

					if( result == LOGIN::OK )
					{
						successList[successCount++] = dbAccount;					
					}
				}
			}
			
			result = LOGIN::OK;

		}

		m_AccountDB->clearCMDBuf();
		m_AccountDB->clearRows();

	}	

	for( int i=0; i < successCount; ++i )
	{
		result = DB_ForceLogout( successList[i] );
	}

	printf("-- AutoRecovery(%d)...............End\n", successCount);
}










//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
void CAccountDBProc::recvRequestReset( WORK::sPacketHeader* header, AUTH::sRequestReset* data )
{
	printf("-- ResetWorld...............Start\n");

	
	int result = LOGIN::UNKNOWN_FAIL;
	

	char		id		  [LOGIN::ID_MAX_LENGTH];
	char		password  [LOGIN::PW_MAX_LENGTH];
	char		ip		  [16];
	
	_dword		dbAccount = 0;
	_dword		idNumber  = 0;


	
	/* 최대 동시에 6000명의 복구를 할수 있다 */

	_dword		successList[6000];
	int			successCount = 0;
		
	
	// 성공 할때까지 시도 한다 
	while( m_AccountDB->SQLRun("EXEC SPHQ_OnGetListForReset %d", data->world_id) == FALSE )
	{
		;
	}
	
		
	if(m_AccountDB->DBResults() == SUCCEED)
	{

		m_AccountDB->DBBind(1, NTBSTRINGBIND,			(DBCHAR)0, (BYTE*)id);
		m_AccountDB->DBBind(2, NTBSTRINGBIND,			(DBCHAR)0, (BYTE*)password);			
		m_AccountDB->DBBind(3, NTBSTRINGBIND,			(DBCHAR)0, (BYTE*)ip);			
		m_AccountDB->DBBind(4, INTBIND,					(DBINT) 0, (BYTE*)&dbAccount);			
		
		

		while( m_AccountDB->DBNextRow() != NO_MORE_ROWS )
		{						
			if( successCount < 6000 )
			{
				result = m_pAuthorizer->reqLogout( id, password, ip, &idNumber );

				if( result == LOGIN::OK )
				{
					successList[successCount++] = dbAccount;					
				}
			}
		}
			
		result = LOGIN::OK;
	}

	m_AccountDB->clearCMDBuf();
	m_AccountDB->clearRows();

	
	

	for( int i=0; i < successCount; ++i )
	{
		result = DB_ForceLogout( successList[i] );
	}


	printf("-- ResetWorld(%d)...............End\n", successCount);

	
	sendResponseReset( header->gameServerUniqID, header->userUniqID, LOGIN::OK );
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
void CAccountDBProc::sendResponseReset  ( AUTH::sUniqueID gameServerUniqID, 
										  AUTH::sUniqueID userUniqID, 
										  int responseKind )
{
								  
	AUTH::sResponseLogin	packet;

	packet.command			=	AUTH::RESPONSE_RESET;
	packet.responseKind		=	responseKind;	

	CGameServerMgr::P()->writeToSendQueue(gameServerUniqID , userUniqID, (char*)&packet, sizeof(packet));
}
















//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : DB에 로그아웃을 요청 한다 
//--------------------------------------------------------------------------------------------
int CAccountDBProc::DB_ForceLogout(_dword dbAccount)
{
	printf("-- ForceLogout %d...............Start\n", dbAccount);

	int result = LOGIN::UNKNOWN_FAIL;
	

	if( m_AccountDB->SQLRun("EXEC SPHQ_OnLogoutByForce %d", dbAccount) )
	{				
		
		if(m_AccountDB->DBResults() == SUCCEED)
		{		
			
			while( m_AccountDB->DBNextRow() != NO_MORE_ROWS );	
			
			result = LOGIN::OK;

		}

		m_AccountDB->clearCMDBuf();
		m_AccountDB->clearRows();
	}
	

	printf("-- ForceLogout %d...............End\n", dbAccount);

	return result;
}













