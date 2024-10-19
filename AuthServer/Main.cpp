#include <winsock2.h>
#include <conio.h>
#include <stdio.h>
#include <process.h>
#include <time.h>

#define  DBNTWIN32
#include <sqlfront.h>
#include <sqldb.h>


// Global
#include "_WorkDefine.h"
#include "../kojimodo/utility/Singleton.h"
#include "../kojimodo/iocp/AcceptSocket.h"
#include "../kojimodo/iocp/ListenSocket.h"


// CGameServerMgr
#include "GameServer.h"
#include "GameServerMgr.h"

// WorkMgr
#include "WorkMgr.h"

// AuthProc
#include "AuthProc.h"


// AccountDBProc
#include "AccountDBProc.h"


// CAuthInfoParser
#include "../kojimodo/utility/Singleton.h"
#include "AuthInfoParser.h"

// CJOChinaAuthserverConfig
#include "JOChinaAuthServerConfig.h"


/*

Ver2.31 (03.7.26)
1. 25일 나간 테스트 서버에 맞추어 테스트를 하기위해 
월드 아이디가 99가 아니면 강제로 0으로 설정하는 코드 추가 

Ver2.3 (03.7.22)
1. SPHQ_OnLoginEnd를 인증서버에 실행 하도록 변경 
2. 월드 리셋 부분을 좀더 안정적으로 변경 

*/

#define			VERSION			2.310000




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 이 호스트의 아이피를 얻어온다 
//--------------------------------------------------------------------------------------------
void getHostIP(char* serverIP)
{
	char				hostName[100];
	memset(hostName, 0, sizeof(hostName));

	struct hostent *	pHostEnt;						
	
	BYTE				h1, h2, h3, h4;				    

	
	if( gethostname(hostName,sizeof(hostName)) == 0 )
	{
		pHostEnt = gethostbyname(hostName);         
		
		h1=(*pHostEnt).h_addr_list[0][0];         
		h2=(*pHostEnt).h_addr_list[0][1];
		h3=(*pHostEnt).h_addr_list[0][2];
		h4=(*pHostEnt).h_addr_list[0][3];
		
		sprintf(serverIP,"%i.%i.%i.%i",h1,h2,h3,h4);
	}
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : Global 객체를 생성한다 
//--------------------------------------------------------------------------------------------
void NewGlobalObject()
{
	char serverIP[16];
	getHostIP(serverIP);
	
	// 반드시 다른 Global 변수 보다 먼저 초기화 해야 한다
	new CAuthInfoParser   ( "AuthInfo.txt" );			 
	
	new CGameServerMgr	  ( serverIP, CAuthInfoParser::P()->getAuthServer_Port(), 
									  CAuthInfoParser::P()->getGameServer_Num() );		

	new CAccountDBWorkMgr ( CAuthInfoParser::P()->getWorkQueue_Size() );					
	new CAuthWorkMgr	  ( CAuthInfoParser::P()->getWorkQueue_Size() );
	
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : Global 객체를 초기화 한다 
//--------------------------------------------------------------------------------------------
void InitiGlobalObject()
{	
	CGameServerMgr::P()->CreateSocketList();
}

BOOL IsGashClientUsed(int& nGashThreadCount)
{
	CJOChinaAuthServerConfig Config;
	Config.SetConfigFileName(GERSANG_GASH_CLIENT_INFO_FILE);
	char szNationCode[128] = {0,};
	strcpy(szNationCode, Config.GetNationCode());

	strcpy( szNationCode, strlwr(szNationCode));
	if(strcmp(szNationCode, "china") == 0 ||
	   strcmp(szNationCode, "cn") == 0)
	{
		Config.SetConfigFileName(GERSANG_GASH_CLIENT_INFO_FILE);
		int nGashMaxSocket = Config.GetMaxConnectionSocketCount();
		
		nGashThreadCount = nGashMaxSocket > 25 ? 25 : nGashMaxSocket;

		return TRUE;
	}

	return FALSE;
}
	


//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 모든 스레드를 생성 한다 
//--------------------------------------------------------------------------------------------
void CreateAllThread()
{
	unsigned int threadID;
	HANDLE		 hThread[6];	
	hThread[0] = (HANDLE)_beginthreadex( NULL, 0, CListenSocket::_ConnectThread,	  (void*)CGameServerMgr::P(), 0, &threadID);		
	hThread[1] = (HANDLE)_beginthreadex( NULL, 0, CListenSocket::_IOCompletionThread, (void*)CGameServerMgr::P(), 0, &threadID);			
	hThread[2] = (HANDLE)_beginthreadex( NULL, 0, CAccountDBProc::_AccountDBProcThread,		  NULL, 0, &threadID);		
	
	int nGashThreadCount = 1;

	if(IsGashClientUsed(nGashThreadCount))
	{
		for(int n = 0 ; n < nGashThreadCount ; n++)
		{
			hThread[3] = (HANDLE)_beginthreadex( NULL, 0, CAuthProc::_AuthProcThread,				  NULL, 0, &threadID);		
		}

		printf(" [Gash thread count : %d]\n", nGashThreadCount*3);
	}
	else
	{	
		hThread[3] = (HANDLE)_beginthreadex( NULL, 0, CAuthProc::_AuthProcThread,				  NULL, 0, &threadID);		
		hThread[4] = (HANDLE)_beginthreadex( NULL, 0, CAuthProc::_AuthProcThread,				  NULL, 0, &threadID);		
		hThread[5] = (HANDLE)_beginthreadex( NULL, 0, CAuthProc::_AuthProcThread,				  NULL, 0, &threadID);		
	}
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 메인 처리 
//--------------------------------------------------------------------------------------------
void MainProcess()
{
	bool bRun = TRUE;	
	while(bRun)
	{
		Sleep(10);
		
		CGameServerMgr::P()->ProcessPoll();

		if(kbhit())
		{
			switch(getch())
			{
				case '\x1b':
					{						
						CGameServerMgr::P()->Shutdown();
						CAccountDBWorkMgr::P()->Shutdown();
						CAuthWorkMgr::P()->Shutdown();

						bRun = FALSE;

						Sleep(5000);
					}
					break;
			}
		}
	}
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : Global 객체를 해제 한다 
//--------------------------------------------------------------------------------------------
void EndGlobalObject()
{
	CGameServerMgr::P()->ReleaseSocketList();	
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : Global 객체를 삭제 한다 
//--------------------------------------------------------------------------------------------
void DeleteGlobalObject()
{	
	delete CGameServerMgr::P();
	delete CAccountDBWorkMgr::P();
	delete CAuthWorkMgr::P();
	delete CAuthInfoParser::P();
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc :  main()
//--------------------------------------------------------------------------------------------
void main(int argc, char* argv[], char* envp[])
{		

	/* 서버가 실행된 시간을 기록 한다 */
	
	FILE* fp;		
	if( (fp = fopen("AuthServerLog.txt","at")) != NULL )		
	{
		char time[128];
		char date[128];

		_strtime( time );
		_strdate( date );

		fprintf(fp,"[%s %s] AuthServer Ver.%f worked\n", date, time, VERSION);

		fclose(fp);
	}
	


	/* 실행인자 파싱 */
	
	if( argc > 1 )
	{
		if( strcmp(argv[1], "/ver") == 0 )
		{
			printf("- Gersang AuthServer Version %f\n", VERSION);
		
			getch();
		}

		else if( strcmp(argv[1], "/help") == 0 )
		{
			printf("- Command Help\n");
			printf("/help		show authserver command help\n");
			printf("/ver		show authserver version\n\n");
						

			printf("- Key Help(at running server)\n");
			printf("'ESC'		server quit\n");
			getch();
		}
		else
		{
			printf("- Invalid argument\n");

			getch();
		}
	}

	else
	{
		printf("----------------------------------------------------------------------------\n");
		printf("                                     Online Gersang Auth Server Ver %f\n", VERSION);
		printf("----------------------------------------------------------------------------\n");

		WSADATA	WSAData;								
		WSAStartup(0x0202, &WSAData);
		

		dbinit();

		dbsetlogintime(30000);
		dbsettime(30000);
		dbsetmaxprocs(100);


		/* Global 객체 생성 */
		NewGlobalObject();
		

		/* Global 객체 초기화 */
		InitiGlobalObject();
		

		/* 모든 스레드 생성 */
		CreateAllThread();
		

		/* 메인 처리 */
		MainProcess();	
		

		/* Global 객체 해제 */ 
		EndGlobalObject();	
		

		/* Global 객체 삭제 */
		DeleteGlobalObject();


		dbexit();

		WSACleanup();
	}
}


