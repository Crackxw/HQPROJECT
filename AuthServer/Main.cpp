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
1. 25�� ���� �׽�Ʈ ������ ���߾� �׽�Ʈ�� �ϱ����� 
���� ���̵� 99�� �ƴϸ� ������ 0���� �����ϴ� �ڵ� �߰� 

Ver2.3 (03.7.22)
1. SPHQ_OnLoginEnd�� ���������� ���� �ϵ��� ���� 
2. ���� ���� �κ��� ���� ���������� ���� 

*/

#define			VERSION			2.310000




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �� ȣ��Ʈ�� �����Ǹ� ���´� 
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
//	Desc : Global ��ü�� �����Ѵ� 
//--------------------------------------------------------------------------------------------
void NewGlobalObject()
{
	char serverIP[16];
	getHostIP(serverIP);
	
	// �ݵ�� �ٸ� Global ���� ���� ���� �ʱ�ȭ �ؾ� �Ѵ�
	new CAuthInfoParser   ( "AuthInfo.txt" );			 
	
	new CGameServerMgr	  ( serverIP, CAuthInfoParser::P()->getAuthServer_Port(), 
									  CAuthInfoParser::P()->getGameServer_Num() );		

	new CAccountDBWorkMgr ( CAuthInfoParser::P()->getWorkQueue_Size() );					
	new CAuthWorkMgr	  ( CAuthInfoParser::P()->getWorkQueue_Size() );
	
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : Global ��ü�� �ʱ�ȭ �Ѵ� 
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
//	Desc : ��� �����带 ���� �Ѵ� 
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
//	Desc : ���� ó�� 
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
//	Desc : Global ��ü�� ���� �Ѵ� 
//--------------------------------------------------------------------------------------------
void EndGlobalObject()
{
	CGameServerMgr::P()->ReleaseSocketList();	
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : Global ��ü�� ���� �Ѵ� 
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

	/* ������ ����� �ð��� ��� �Ѵ� */
	
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
	


	/* �������� �Ľ� */
	
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


		/* Global ��ü ���� */
		NewGlobalObject();
		

		/* Global ��ü �ʱ�ȭ */
		InitiGlobalObject();
		

		/* ��� ������ ���� */
		CreateAllThread();
		

		/* ���� ó�� */
		MainProcess();	
		

		/* Global ��ü ���� */ 
		EndGlobalObject();	
		

		/* Global ��ü ���� */
		DeleteGlobalObject();


		dbexit();

		WSACleanup();
	}
}


