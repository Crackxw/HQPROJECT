#include <winsock2.h>
#include <stdio.h>

// CWorkQueue
#include "../kojimodo/utility/Queue.h"
#include "WorkQueue.h"


// CWorkMgr
#include "WorkMgr.h"


// CGameServer
#include "../kojimodo/iocp/AcceptSocket.h"
#include "GameServer.h"


// CGameServerMgr
#include "../kojimodo/iocp/ListenSocket.h"
#include "../kojimodo/utility/Singleton.h"
#include "GameServerMgr.h"


// CMSSQL
#define  DBNTWIN32
#include <sqlfront.h>
#include <sqldb.h>
#include "../kojimodo/utility/MSSQL.h"


// CAuthProc
#include "AuthProc.h"


// CAuthInfoParser
#include "../kojimodo/utility/Singleton.h"
#include "AuthInfoParser.h"


// CAuthorizer
#include "Authorizer.h"
#include "Gash.h"


//
#include "_WorkDefine.h"

// Gash Client
#include "GashClient.h"
#include "JOChinaAuthServerConfig.h"




#include <time.h>
void WRITE_LOG(char* A, char* B)
{
	FILE* fp;	
	if( (fp = fopen(A,"at")) != NULL ) 
	{
		char time[128]; 
		char date[128]; 

		_strtime( time ); 
		_strdate( date ); 

		fprintf(fp,"[%s %s] %s\n", date, time, B); 

		fclose(fp); 
	} 
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �� �� �� 
//--------------------------------------------------------------------------------------------
CAuthProc::CAuthProc() : 
m_bGashClientUsed(FALSE)
{
	IsGashClientUsed();

	m_queryBuffer = new char[ WORK::MAX_PACKETSIZE ];	
	m_AccountDB	  = new CMSSQL();
	// Gash client use.
	if(m_bGashClientUsed == TRUE)
	{
		m_pGashClient = new CGashClient(static_cast<CAuthCallback*>(this));
		m_pGashClient->DBConnect(CAuthInfoParser::P()->getAccountDB_IP(), 
								 CAuthInfoParser::P()->getAccountDB_Name(), 
								 CAuthInfoParser::P()->getAccountDB_ID(), 
								 CAuthInfoParser::P()->getAccountDB_PW());
		printf("Gash client start...!!\n");
	}
	// COM+ Interface use.
	else
	{
		m_pAuthorizer = CAuthInfoParser::P()->createAuthorizer();
	}

	m_AccountDB->Connect( CAuthInfoParser::P()->getAccountDB_IP()    ,
						  CAuthInfoParser::P()->getAccountDB_Name()  ,
						  CAuthInfoParser::P()->getAccountDB_ID()	 ,
						  CAuthInfoParser::P()->getAccountDB_PW()      );	
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �� �� �� 
//--------------------------------------------------------------------------------------------
CAuthProc::~CAuthProc()
{
	if( m_queryBuffer ){

		delete m_queryBuffer;
	}

	if(m_bGashClientUsed == TRUE)
	{
		if(m_pGashClient != NULL)
		{
			delete m_pGashClient;
			m_pGashClient = NULL;
		}
	}
	else
	{	
		if( m_pAuthorizer )
		{
			delete m_pAuthorizer;
			m_pAuthorizer = NULL;
		}
	}

	if( m_AccountDB ) {

		m_AccountDB->Dissconnect();

		delete m_AccountDB;
	}

}

//--------------------------------------------------------------------------------------------
//	Name : �̹μ�
//  Date : 2004. 02. 06
//	Desc : Gash client�� �̿��Ұ������� �˻��Ѵ�.
//--------------------------------------------------------------------------------------------
void CAuthProc::IsGashClientUsed()
{
	CJOChinaAuthServerConfig Config;
	Config.SetConfigFileName(GERSANG_GASH_CLIENT_INFO_FILE);
	char szNationCode[128] = {0,};
	strcpy(szNationCode, Config.GetNationCode());

	strcpy( szNationCode, strlwr(szNationCode));
	if(strcmp(szNationCode, "china") == 0 ||
	   strcmp(szNationCode, "cn") == 0)
	{
		m_bGashClientUsed = TRUE;
	}
	else
	{
		m_bGashClientUsed = FALSE;
	}
}

//--------------------------------------------------------------------------------------------
//	Name : �̹μ�
//  Date : 2004. 02. 06
//	Desc : Gash client �ν��Ͻ��� ȣ���� Callback �Լ�.
//--------------------------------------------------------------------------------------------
void CAuthProc::CallbackSendResponseLogin(AUTH::sUniqueID gameServerUniqID, 
										  AUTH::sUniqueID userUniqID, 
										  int responseKind, 
										  DWORD dbAccount)
{
	sendResponseLogin(gameServerUniqID, userUniqID, responseKind, dbAccount);
}


//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �ֱ����� �۾��� ó�� 
//--------------------------------------------------------------------------------------------
void CAuthProc::processPoll()
{
	while( !CAuthWorkMgr::P()->isShutdown() )
	{
		Sleep(10);

		processQuery();
	}
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ��û�� ó�� �Ѵ� 
//--------------------------------------------------------------------------------------------
void CAuthProc::processQuery()
{	

	//-- packetSize 
	int	packetSize = CAuthWorkMgr::P()->getQuery(m_queryBuffer, WORK::MAX_PACKETSIZE);
	

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
			
			case REQUEST_LOGIN:	
				// Gash client use.
				if(m_bGashClientUsed == TRUE)
				{
					m_pGashClient->PushCmdQueueAuthorization(header,(AUTH::sRequestLogin*)data);
					Sleep(100);

					printf("[%x Process]\n", this);
				}
				// Gash COM+ Interface use.
				else
				{
					recvRequestLogin( header, (AUTH::sRequestLogin*)data );
				}
				break;

			case REQUEST_LOGOUT:
				// Gash client use.
				if(m_bGashClientUsed == TRUE)
				{
					m_pGashClient->PushCmdQueueAccounting(header, (AUTH::sRequestLogout*)data);				
				}
				// Gash COM+ Interface use.
				else
				{
					recvRequestLogout( header, (AUTH::sRequestLogout*)data );
				}
				break;

			case AUTH::REQUEST_LOGIN_END:
				recvRequestLoginEnd( header, (AUTH::sRequestLoginEnd*)data );				
				break;

			case AUTH::REPORT_HEARTBEAT:
				break;


		}

	}
}
















/*

	1) �α��� ������ ���� ���� 

		1. Authorizer�� id, password �� Ȯ�ο�û �Ͽ� ��ȿ�� �������� �Ǵ� �Ѵ� 
		  
		( ex> m_pAuthorizer->reqLogin( data->id, data->password, data->ip ) )

	
		2. ��ȿ�� �����ϰ�� DB�� '�α��� ��' �ܰ�� ���¸� ���� �Ѵ� 
		
		( ex> DB_Login(data->id, data->password, data->ip, &dbAccount) )

  
		3. ���� �ϴµ� ���� �Ͽ��ٸ� Authorizer�� ����� ��û, id�� ������� ���·� ���� �Ѵ� 

		( ex> m_pAuthorizer->reqRecCash(data->id, data->password, data->ip) )


		***** Authorizer�� �ι� ��û�ϴ� ���� *****

		�� ��� ���ڷ� ������ id, password�� � �˻�� ��ġ�� �ʾұ� ������ 
		�ݵ�� DB_Login�� �ϱ����� ���� Authorizer�� id�� ��ȿ���� Ȯ�� �Ͽ��� �Ѵ�
		( DB_Login�� ���� ���̵��� ��� ������ ���̵� �����ϵ��� ������� �ִ� )

 
		Authorizer�� ����� DB_Login ���� ���� ���� �ʴ� ������ ����� �ٷ� ������ �ٿ�� ��� 
		�� ������ ���¸� ������ �ټ� ���� �����̴� 		 
	

	2)  
  
*/









//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �α����� ��û �ϴ� ��� 
//--------------------------------------------------------------------------------------------
void CAuthProc::recvRequestLogin( WORK::sPacketHeader* header, AUTH::sRequestLogin* data )
{
	// �׽�Ʈ�� ���� �߰� 
	int world_id =  data->world_id;
	if(world_id != 99) world_id = 0;


	printf("-- Login %s(%s)......Start\n", data->id, data->ip);
	

	int    result		 =  LOGIN::UNKNOWN_FAIL;

	_dword  dbAccount	 =  0;
	_dword  idNumber     =  0;
	
	
	//-- authenication request 	
	result = m_pAuthorizer->reqLogin( data->id, data->password, data->ip, &idNumber );

	
	if( result == LOGIN::OK ) 
	{
		
		//-- check account and state change(to LOGIN_START) 
		result = DB_Login(data->id, data->password, data->ip, world_id, idNumber, &dbAccount);
		

		if( result == LOGIN::OK )
		{

			//-- record to cash request 
			result = m_pAuthorizer->reqRecCash(data->id, data->password, data->ip, &idNumber );									
		}
	}	

	
	
	printf("-- Login %s(%s):[%d]......End\n", data->id, data->ip, result);



	sendResponseLogin( header->gameServerUniqID, header->userUniqID, result, dbAccount);
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �α׾ƿ��� ��û �ϴ� ��� 
//--------------------------------------------------------------------------------------------
void CAuthProc::recvRequestLogout( WORK::sPacketHeader* header, AUTH::sRequestLogout* data )
{

	printf("-- Logout %s(%s)......Start\n", data->id, data->ip);

	
	_dword  idNumber = 0;


	int result = m_pAuthorizer->reqLogout( data->id, data->password, data->ip, &idNumber );


	if( result == LOGIN::OK )
	{
		result = DB_Logout( data->dbAccount );
	}

	printf("-- Logout %s(%s):[%d]......End\n", data->id, data->ip, result);
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �α��� �ϷḦ ��û �ϴ� ��� 
//--------------------------------------------------------------------------------------------
void CAuthProc::recvRequestLoginEnd( WORK::sPacketHeader* header, AUTH::sRequestLoginEnd* data )
{
	
	printf("-- LoginEnd %d......Start\n", data->dbAccount);
	

	int    result		 =  LOGIN::UNKNOWN_FAIL;
	
				
	// �α����� �Ϸ� �Ѵ� 
	result = DB_LoginEnd(data->dbAccount);
	

		
	printf("-- LoginEnd %d:[%d]......End\n", data->dbAccount, result);



	sendResponseLoginEnd( header->gameServerUniqID, header->userUniqID, result );
}







//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �α��� ������ �۽� �Ѵ� 
//--------------------------------------------------------------------------------------------
void CAuthProc::sendResponseLogin  ( AUTH::sUniqueID gameServerUniqID, 
									 AUTH::sUniqueID userUniqID, 
									 int responseKind, _dword dbAccount )
{
								  
	AUTH::sResponseLogin	packet;

	packet.command			=	LOGIN::RESPONSE_LOGIN;
	packet.responseKind		=	responseKind;
	packet.dbAccount		=   dbAccount;

	CGameServerMgr::P()->writeToSendQueue(gameServerUniqID , userUniqID, (char*)&packet, sizeof(packet));
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �α��� �Ϸ� ������ �۽� �Ѵ� 
//--------------------------------------------------------------------------------------------
void CAuthProc::sendResponseLoginEnd  ( AUTH::sUniqueID gameServerUniqID, 
										AUTH::sUniqueID userUniqID, 
										int responseKind )
{
								  
	AUTH::sResponseLoginEnd	packet;

	packet.command			=	AUTH::RESPONSE_LOGIN_END;
	packet.responseKind		=	responseKind;
	
	CGameServerMgr::P()->writeToSendQueue(gameServerUniqID , userUniqID, (char*)&packet, sizeof(packet));
}







//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : DB�� �α����� ��û �Ѵ� 
//--------------------------------------------------------------------------------------------
int CAuthProc::DB_Login(char* id, char* password, char* ip, _word world_id, _dword idNumber, _dword* dbAccount)
{

	int result = LOGIN::UNKNOWN_FAIL;
	
	// (03.6.4 ���� �系�׽�Ʈ�� ���ν���)
//	if( m_AccountDB->SQLRun("EXEC SPHQ_OnLoginStart '%s','%s','%s', %d, %d", id, password, ip, world_id, idNumber) ) 

	// (03.6.4 �������� ���ν���)
	if( m_AccountDB->SQLRun("EXEC SPHQ_OnLoginStart '%s','%s','%d','%s'", id, password, world_id, ip) )
	{			
		
		if(m_AccountDB->DBResults() == SUCCEED)
		{

			m_AccountDB->DBBind(1, INTBIND,			(DBINT)0, (BYTE*)&result);			
			m_AccountDB->DBBind(2, INTBIND,		    (DBINT)0, (BYTE*)dbAccount);
			
			while( m_AccountDB->DBNextRow() != NO_MORE_ROWS );		
			
		}

		m_AccountDB->clearCMDBuf();
		m_AccountDB->clearRows();

	}


	// ���� ��ȭ�� ������ ���� ��� �α׸� ����� 
	if( result == LOGIN::FAIL_CHANGE_STATE ) {

		char str[100]; wsprintf(str,"SPHQ_OnLoginStart %s", id); WRITE_LOG("StateChangeError.txt", str);		
	}
	
	return result;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : DB�� �α׾ƿ��� ��û �Ѵ� 
//--------------------------------------------------------------------------------------------
int CAuthProc::DB_Logout(_dword dbAccount)
{

	int result = LOGIN::UNKNOWN_FAIL;
	

	if( m_AccountDB->SQLRun("EXEC SPHQ_OnLogoutEnd %d", dbAccount) )
	{				
		
		if(m_AccountDB->DBResults() == SUCCEED)
		{		
			m_AccountDB->DBBind(1, INTBIND,			(DBINT)0, (BYTE*)&result);

			while( m_AccountDB->DBNextRow() != NO_MORE_ROWS );							
		}

		m_AccountDB->clearCMDBuf();
		m_AccountDB->clearRows();

	}
	

	// ���� ��ȭ�� ������ ���� ��� �α׸� ����� 
	if( result == LOGIN::FAIL_CHANGE_STATE ) {

		char str[100]; wsprintf(str,"SPHQ_OnLogoutEnd %d", dbAccount); WRITE_LOG("StateChangeError.txt", str);	
	}

	return result;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : DB�� �α��� �ϷḦ ��û �Ѵ� 
//--------------------------------------------------------------------------------------------
int CAuthProc::DB_LoginEnd(_dword dbAccount)
{

	int result = LOGIN::UNKNOWN_FAIL;
	

	if( m_AccountDB->SQLRun("EXEC SPHQ_OnLoginEnd %d", dbAccount) )
	{				
		
		if(m_AccountDB->DBResults() == SUCCEED)
		{		
			m_AccountDB->DBBind(1, INTBIND,			(DBINT)0, (BYTE*)&result);
			
			while( m_AccountDB->DBNextRow() != NO_MORE_ROWS );						
		}

		m_AccountDB->clearCMDBuf();
		m_AccountDB->clearRows();

	}
	

	// ���� ��ȭ�� ������ ���� ��� �α׸� ����� 
	if( result == LOGIN::FAIL_CHANGE_STATE ) {

		char str[100]; wsprintf(str,"SPHQ_OnLoginEnd %d", dbAccount); WRITE_LOG("StateChangeError.txt", str);	
	}

	return result;
}