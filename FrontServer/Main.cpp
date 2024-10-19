#include <winsock2.h>
#include <conio.h>
#include <stdio.h>
#include <process.h>
#include <time.h>


// Global
#include "../kojimodo/utility/Singleton.h"
#include "../kojimodo/iocp/AcceptSocket.h"
#include "../kojimodo/iocp/ListenSocket.h"


// CUserMgr
#include "User.h"
#include "UserMgr.h"


// CGameServerMgr
#include "GameServer.h"
#include "GameServerMgr.h"


// WorldInfoMgr
#include "WorldInfoMgr.h"


// GameInfoParser
#include "GameInfoParser.h"


// CWorldInfoParser
#include "WorldInfoParser.h"


//
#include "_FrontDefine.h"



#define			VERSION			2.000000





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
	

	
	new CGameInfoParser	 ( "Notice.txt", "GameInfo.txt" );
	
	
	new CUserMgr		 ( serverIP, CGameInfoParser::P()->getUserPort(), 
									 CGameInfoParser::P()->getNumUser() );


	new CGameServerMgr   ( serverIP, CGameInfoParser::P()->getGameServerPort(), FRONT::MAX_WORLD );
	

	// 반드시 CWorldInfoMgr를 생성 하고 CWorldInfoParser를 생성 해야 한다 
	new CWorldInfoMgr	 ();
	new CWorldInfoParser ( "WorldsInfo.txt" );
	
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : Global 객체를 초기화 한다 
//--------------------------------------------------------------------------------------------
void InitiGlobalObject()
{
	CUserMgr::P()->CreateSocketList();
	CGameServerMgr::P()->CreateSocketList();
}

	


//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 모든 스레드를 생성 한다 
//--------------------------------------------------------------------------------------------
void CreateAllThread()
{
	unsigned int threadID;
	HANDLE		 hThread[4];	
	hThread[0] = (HANDLE)_beginthreadex( NULL, 0, CListenSocket::_ConnectThread,	  (void*)CUserMgr::P(), 0, &threadID);		
	hThread[1] = (HANDLE)_beginthreadex( NULL, 0, CListenSocket::_IOCompletionThread, (void*)CUserMgr::P(), 0, &threadID);		
	hThread[2] = (HANDLE)_beginthreadex( NULL, 0, CListenSocket::_ConnectThread,	  (void*)CGameServerMgr::P(), 0, &threadID);		
	hThread[3] = (HANDLE)_beginthreadex( NULL, 0, CListenSocket::_IOCompletionThread, (void*)CGameServerMgr::P(), 0, &threadID);		
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

		CUserMgr::P()->ProcessPoll();
		CGameServerMgr::P()->ProcessPoll();

		if(kbhit())
		{
			switch(getch())
			{
				case 'u':
					{
						CGameInfoParser::P()->LoadGameInfo("GameInfo.txt");
						CGameInfoParser::P()->LoadNotice("Notice.txt");
					}
					break;


				case '\x1b':
					{								
						CUserMgr::P()->Shutdown();
						CGameServerMgr::P()->Shutdown();

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
	CUserMgr::P()->ReleaseSocketList();
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : Global 객체를 삭제 한다 
//--------------------------------------------------------------------------------------------
void DeleteGlobalObject()
{	
	delete CWorldInfoParser::P();
	delete CGameInfoParser::P();

	delete CWorldInfoMgr::P();
	delete CGameServerMgr::P();
	delete CUserMgr::P();
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc :  main()
//--------------------------------------------------------------------------------------------
void main(int argc, char* argv[], char* envp[])
{
	
	/* 서버가 실행된 시간을 기록 한다 */
	
	FILE* fp;		
	if( (fp = fopen("FrontServerLog.txt","at")) != NULL )		
	{
		char time[128];
		char date[128];

		_strtime( time );
		_strdate( date );

		fprintf(fp,"[%s %s] FrontServer Ver.%f worked\n", date, time, VERSION);

		fclose(fp);
	}
	


	/* 실행인자 파싱 */
	
	if( argc > 1 )
	{
		if( strcmp(argv[1], "/ver") == 0 )
		{
			printf("- FrontServer Version %f\n", VERSION);
		
			getch();
		}

		else if( strcmp(argv[1], "/help") == 0 )
		{
			printf("- Command Help\n");
			printf("/help		show frontserver command help\n");
			printf("/ver		show frontserver version\n\n");
						

			printf("- Key Help(at running server)\n");
			printf("'ESC'		frontserver quit\n");
			printf("'U'			update notice and version\n");
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
		printf("                                    Online Gersang Front Server Ver %f\n", VERSION);
		printf("----------------------------------------------------------------------------\n");


		WSADATA	WSAData;								
		WSAStartup(0x0202, &WSAData);
		

		
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



		WSACleanup();
	}
}


