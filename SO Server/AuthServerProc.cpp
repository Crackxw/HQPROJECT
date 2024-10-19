#include <winsock2.h>
#include <stdio.h>


// CAuthServerProc
#include "../kojimodo/utility/Winsock.h"
#include "AuthServerProc.h"


// _AuthDefine
#include "../AuthServer/_AuthDefine.h"


// CMSSQL
#define  DBNTWIN32
#include <sqlfront.h>
#include <sqldb.h>
#include "../kojimodo/utility/MSSQL.h"


// etc..
#include "SOMain.h"








//--------------------------------------------------------------------------------------------
// Name : 
// Desc : �� �� �� 
//--------------------------------------------------------------------------------------------
CAuthServerProc::CAuthServerProc(int wait_time):			 
CWinsock(sizeof(AUTH::sPacketHeader), wait_time)
{
	
	m_GameDB = new CMSSQL();

	m_GameDB->Connect( g_GameServerInfoParser.GetDB1IPString(),  
					   g_GameServerInfoParser.GetDBName1String(), 
					   g_GameServerInfoParser.GetDBUserString(), 
					   "dhs1fk2dls3rj4tkd" );
		

	// ���������� ���� �Ѵ� 
	Connect();	
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : �� �� �� 
//--------------------------------------------------------------------------------------------
CAuthServerProc::~CAuthServerProc()
{
	if( m_GameDB ) {

		m_GameDB->Dissconnect();

		delete m_GameDB;
	}
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : ���������� ���̴� ��Ŷ�� ũ�⸦ ��� �´� 
//--------------------------------------------------------------------------------------------
int CAuthServerProc::getPacketSize(char* header) 
{
	return ((AUTH::sPacketHeader*)header)->packetSize;
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : ���� ���� ��Ŷ�� ��� üũ���� ��� �´� 
//--------------------------------------------------------------------------------------------
int	CAuthServerProc::getChecksum(char* header)
{
	return ((AUTH::sPacketHeader*)header)->checksum;
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : üũ���� ����Ͽ� ���� �Ѵ� 
//--------------------------------------------------------------------------------------------
int CAuthServerProc::calcChecksum(char* data, int size) 
{
	char checksum = 0;
	for(int i=0; i < size; i++)
	{
		checksum ^= data[i];
	}

	return checksum;
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : ���� ������ ���� �Ѵ� 
//--------------------------------------------------------------------------------------------
bool CAuthServerProc::Connect()
{
	return CWinsock::Connect( g_GameServerInfoParser.GetAuthServerIP(), 20002 );	
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : ��Ŷ�� ����� ����(�۽���)�� ����ũ ���̵� �����Ͽ� �۽� �Ѵ� 
//--------------------------------------------------------------------------------------------
bool CAuthServerProc::Send(AUTH::sUniqueID userUniqID, char* data, int dataSize)
{
	AUTH::sPacketHeader header;
			
	header.packetSize  = HEADER_SIZE + dataSize;		
	header.checksum    = calcChecksum(data, dataSize);
	header.userUniqID  = userUniqID;

	return CWinsock::Send( (char*)&header, data, dataSize );
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : ��Ŷ�� �۽� �Ѵ� 
//--------------------------------------------------------------------------------------------
bool CAuthServerProc::Send(char* data, int dataSize)
{
	AUTH::sPacketHeader header;
			
	header.packetSize  = HEADER_SIZE + dataSize;		
	header.checksum    = calcChecksum(data, dataSize);
	
	
	// DWORD�� �ִ� ũ��� ����

	header.userUniqID.arrayIndex	= -1; 
	header.userUniqID.orderKey		= -1; 


	return CWinsock::Send( (char*)&header, data, dataSize );
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �α����� ��û �Ѵ� 
//--------------------------------------------------------------------------------------------	
bool CAuthServerProc::RequestLogin( AUTH::sUniqueID userUniqID, char* id, char* password, char* ip, 
									char* responseKind, DWORD* dbAccount )

{
	
	printf("-- Login %s(%s)......Start\n", id, ip);


	// ���ڸ� �ݵ�� �ʱ�ȭ �����ش� 
	*responseKind  = LOGIN::UNKNOWN_FAIL;
	*dbAccount	   = 0;

		
		
	
	AUTH::sRequestLogin		reqPacket;		
	reqPacket.command  = LOGIN::REQUEST_LOGIN;		
	

	// id, password, ip �� ���̴� ��Ʈ���� �ִ� ���̸� �Ѿ�� �ȵȴ� 

	memcpy( reqPacket.id		, id,		LOGIN::ID_MAX_LENGTH );	
	memcpy( reqPacket.password	, password, LOGIN::PW_MAX_LENGTH );	
	memcpy( reqPacket.ip		, ip,		16 );
	
	reqPacket.world_id = g_GameServerInfoParser.GetWorldID();




	AUTH::sResponseLogin	resPacket;
	AUTH::sPacketHeader		resHeader;
	
	while(TRUE)
	{

		if( Send(userUniqID, (char*)&reqPacket, sizeof(reqPacket)) ) 
		{


			if( Recv((char*)&resHeader, (char*)&resPacket, sizeof(resPacket)) ) 
			{


				if( resPacket.command	  == LOGIN::RESPONSE_LOGIN &&
					resHeader.userUniqID  == userUniqID ) {
											
					
					// ���� ������ DB ��ī��Ʈ�� ���� ��´� 
					*responseKind = resPacket.responseKind;
					*dbAccount	  = resPacket.dbAccount;
								


					printf("-- Login %s(%s)[%d]......End\n", id, ip, *responseKind);
					return TRUE;
				}				

			}

		}
	
				
		if( getLastError() == CWinsock::OVER_WAITTIME ) {

			Connect();
			break;
		}
		
		Connect();				
	}

	
	printf("-- Login %s(%s)[%d]......End\n", id, ip, *responseKind);
	return FALSE;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �α׾ƿ��� ��û �Ѵ� 
//--------------------------------------------------------------------------------------------	
bool CAuthServerProc::RequestLogout( char* id, char* password, char* ip, DWORD dbAccount )
{

	printf("-- Logout %s(%s).....Start\n", id, ip);

	
				
	AUTH::sRequestLogout		reqPacket;


		
	reqPacket.command	 =	 LOGIN::REQUEST_LOGOUT;
	reqPacket.dbAccount  =	 dbAccount;
	
	
	// id, password, ip �� ���̴� ��Ʈ���� �ִ� ���̸� �Ѿ�� �ȵȴ�

	memcpy( reqPacket.id		, id,		LOGIN::ID_MAX_LENGTH );	
	memcpy( reqPacket.password	, password, LOGIN::PW_MAX_LENGTH );	
	memcpy( reqPacket.ip		, ip,		16 );
	
	while( TRUE )
	{
		
		if( Send((char*)&reqPacket, sizeof(reqPacket)) )
		{		
			printf("-- Logout %s(%s)[%d]......End\n", id, ip, 1);
			return true;
		}

	
		if( getLastError() == CWinsock::OVER_WAITTIME ) {

			Connect();
			break;
		}
		
		
		Connect();
						
	}


	printf("-- Logout %s(%s)[%d].....End\n", id, ip, -99);
	return FALSE;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� ������ ��û �Ѵ� 
//--------------------------------------------------------------------------------------------	
bool CAuthServerProc::RequestReset( int world_id, char* responseKind )
{
	
	printf("-- ResetWorld.........Start\n");


	// ���ڸ� �ݵ�� �ʱ�ȭ ���� �ش� 
	*responseKind = LOGIN::UNKNOWN_FAIL;



	
			
	AUTH::sRequestReset		reqPacket;
	
	reqPacket.command	 =	AUTH::REQUEST_RESET;
	reqPacket.world_id	 =	world_id;
	
	
	

	AUTH::sResponseReset	resPacket;

	while( TRUE )
	{

		if( Send((char*)&reqPacket, sizeof(reqPacket)) )
		{		
			
			if( Recv(NULL, (char*)&resPacket, sizeof(resPacket)) )
			{

				if( resPacket.command == AUTH::RESPONSE_RESET ) {
					

					// ���� ������ ���� �޴´� 
					*responseKind = resPacket.responseKind;																						

					
					printf("-- ResetWorld(%d).........End\n", *responseKind);
					return TRUE;
				}			
			}			
		}	


		if( getLastError() == CWinsock::OVER_WAITTIME ) {

			Connect();
			break;
		}
		
		
		Connect();							
	}


	printf("-- ResetWorld(%d).........End\n", *responseKind);
	return FALSE;

}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ��Ʈ ��Ʈ�� ���� �Ѵ� 
//--------------------------------------------------------------------------------------------	
bool CAuthServerProc::ReportHeartbeat()
{
	
	AUTH::sReportHeartbeat	repPacket;		
	
	repPacket.command =	AUTH::REPORT_HEARTBEAT;
	

	while(TRUE)
	{	
		if( Send((char*)&repPacket, sizeof(repPacket)) )
		{
			return TRUE;
		}	
		

		if( getLastError() == CWinsock::OVER_WAITTIME ) {

			Connect();
			break;
		}
				
		Connect();		
	}

	return FALSE;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �α��� �ϷḦ ��û �Ѵ� 
//--------------------------------------------------------------------------------------------	
bool CAuthServerProc::RequestLoginEnd( AUTH::sUniqueID userUniqID, DWORD dbAccount, char* responseKind )
									
{
	
	printf("-- LoginEnd %d......Start\n", dbAccount);


	// ���ڸ� �ݵ�� �ʱ�ȭ �����ش� 
	*responseKind  = LOGIN::UNKNOWN_FAIL;
	

		
			
	AUTH::sRequestLoginEnd		reqPacket;		
	
	reqPacket.command   = AUTH::REQUEST_LOGIN_END;		
	reqPacket.dbAccount = dbAccount;

	

	AUTH::sResponseLoginEnd	resPacket;
	AUTH::sPacketHeader		resHeader;
	
	while(TRUE)
	{

		if( Send(userUniqID, (char*)&reqPacket, sizeof(reqPacket)) ) 
		{


			if( Recv((char*)&resHeader, (char*)&resPacket, sizeof(resPacket)) ) 
			{


				if( resPacket.command	  == AUTH::RESPONSE_LOGIN_END &&
					resHeader.userUniqID  == userUniqID ) {
											
					
					// ���� ������ ���� ��´� 
					*responseKind = resPacket.responseKind;
					
													
					printf("-- LoginEnd %d[%d]......End\n", dbAccount, *responseKind);
					return TRUE;
				}				

			}

		}
	
				
		if( getLastError() == CWinsock::OVER_WAITTIME ) {

			Connect();
			break;
		}
		
		Connect();				
	}

	
	printf("-- LoginEnd %d[%d]......End\n", dbAccount, *responseKind);

	return FALSE;
}




/*
//--------------------------------------------------------------------------------------------
// Name : 
// Desc : DB�� �α��� ���¸� ���� �Ѵ� 
//--------------------------------------------------------------------------------------------
int CAuthServerProc::DB_LoginEnd( DWORD dbAccount )
{
	printf("--DB_LoginEnd.....Start\n");

	int result = LOGIN::UNKNOWN_FAIL;
	

	if( m_GameDB->SQLRun("EXEC SPHQ_OnLoginEnd %d, %d", dbAccount, g_GameServerInfoParser.GetWorldID()) )
	{			
		
		if(m_GameDB->DBResults() == SUCCEED)
		{

			while( m_GameDB->DBNextRow() != NO_MORE_ROWS );		

			result = LOGIN::OK;
		}

		m_GameDB->clearCMDBuf();
		m_GameDB->clearRows();

	}
	
	printf("--DB_LoginEnd(%d).....End\n", result);
	return result;
}
*/