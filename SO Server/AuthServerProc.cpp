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
// Desc : 생 성 자 
//--------------------------------------------------------------------------------------------
CAuthServerProc::CAuthServerProc(int wait_time):			 
CWinsock(sizeof(AUTH::sPacketHeader), wait_time)
{
	
	m_GameDB = new CMSSQL();

	m_GameDB->Connect( g_GameServerInfoParser.GetDB1IPString(),  
					   g_GameServerInfoParser.GetDBName1String(), 
					   g_GameServerInfoParser.GetDBUserString(), 
					   "dhs1fk2dls3rj4tkd" );
		

	// 인증서버에 연결 한다 
	Connect();	
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 소 멸 자 
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
// Desc : 인증서버와 쓰이는 패킷의 크기를 얻어 온다 
//--------------------------------------------------------------------------------------------
int CAuthServerProc::getPacketSize(char* header) 
{
	return ((AUTH::sPacketHeader*)header)->packetSize;
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 수신 받은 패킷에 담긴 체크섬을 얻어 온다 
//--------------------------------------------------------------------------------------------
int	CAuthServerProc::getChecksum(char* header)
{
	return ((AUTH::sPacketHeader*)header)->checksum;
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 체크섬을 계산하여 리턴 한다 
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
// Desc : 인증 서버와 연결 한다 
//--------------------------------------------------------------------------------------------
bool CAuthServerProc::Connect()
{
	return CWinsock::Connect( g_GameServerInfoParser.GetAuthServerIP(), 20002 );	
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 패킷의 헤더에 유저(송신자)의 유니크 아이디를 포함하여 송신 한다 
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
// Desc : 패킷을 송신 한다 
//--------------------------------------------------------------------------------------------
bool CAuthServerProc::Send(char* data, int dataSize)
{
	AUTH::sPacketHeader header;
			
	header.packetSize  = HEADER_SIZE + dataSize;		
	header.checksum    = calcChecksum(data, dataSize);
	
	
	// DWORD의 최대 크기와 같다

	header.userUniqID.arrayIndex	= -1; 
	header.userUniqID.orderKey		= -1; 


	return CWinsock::Send( (char*)&header, data, dataSize );
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 로그인을 요청 한다 
//--------------------------------------------------------------------------------------------	
bool CAuthServerProc::RequestLogin( AUTH::sUniqueID userUniqID, char* id, char* password, char* ip, 
									char* responseKind, DWORD* dbAccount )

{
	
	printf("-- Login %s(%s)......Start\n", id, ip);


	// 인자를 반드시 초기화 시켜준다 
	*responseKind  = LOGIN::UNKNOWN_FAIL;
	*dbAccount	   = 0;

		
		
	
	AUTH::sRequestLogin		reqPacket;		
	reqPacket.command  = LOGIN::REQUEST_LOGIN;		
	

	// id, password, ip 의 길이는 스트링의 최대 길이를 넘어서는 안된다 

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
											
					
					// 응답 종류와 DB 어카운트를 응답 밥는다 
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
//	Desc : 로그아웃을 요청 한다 
//--------------------------------------------------------------------------------------------	
bool CAuthServerProc::RequestLogout( char* id, char* password, char* ip, DWORD dbAccount )
{

	printf("-- Logout %s(%s).....Start\n", id, ip);

	
				
	AUTH::sRequestLogout		reqPacket;


		
	reqPacket.command	 =	 LOGIN::REQUEST_LOGOUT;
	reqPacket.dbAccount  =	 dbAccount;
	
	
	// id, password, ip 의 길이는 스트링의 최대 길이를 넘어서는 안된다

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
//	Desc : 서버 리셋을 요청 한다 
//--------------------------------------------------------------------------------------------	
bool CAuthServerProc::RequestReset( int world_id, char* responseKind )
{
	
	printf("-- ResetWorld.........Start\n");


	// 인자를 반드시 초기화 시켜 준다 
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
					

					// 응답 종류를 응답 받는다 
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
//	Desc : 허트 비트를 보고 한다 
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
//	Desc : 로그인 완료를 요청 한다 
//--------------------------------------------------------------------------------------------	
bool CAuthServerProc::RequestLoginEnd( AUTH::sUniqueID userUniqID, DWORD dbAccount, char* responseKind )
									
{
	
	printf("-- LoginEnd %d......Start\n", dbAccount);


	// 인자를 반드시 초기화 시켜준다 
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
											
					
					// 응답 종류를 응답 밥는다 
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
// Desc : DB에 로그인 상태를 설정 한다 
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