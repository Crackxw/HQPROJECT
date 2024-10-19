#include "SOMain.h"
#include "SOServer.h"

#ifdef _GAMEGUARD
#include "CSAuth.h"
#endif

// CFrontServerProc
#include "../kojimodo/utility/Winsock.h"
#include "FrontServerProc.h"
#include "../FrontServer/_FrontGameServerDefine.h"


bool				g_forceShutdown = FALSE;
bool				g_bShutdown     = FALSE;
HANDLE				g_hFrontThread	= NULL;
					
SOGameServerParser	g_GameServerInfoParser;



CRITICAL_SECTION			g_csLog;			// Logfile처리를 위한 CS 
// AuthProc
#include "AuthProc.h"

#include "SOWorld.h"
#include "SOServer.h"
#include "SOPlayer.h"
#include "XPlayer.h"

#include "..\\JWCommon\\JWSocket.h"
#include "..\\JWCommon\\JWRecvBuffer.h"
#include "..\\AuthServer\\_AuthDefine.h"
#include "..\\AuthServer\\LoginProc.h"

// Crash log.
#include "..\\SCCommon\\JODebug.h"




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 프론트 서버와의 통신을 처리하는 쓰레드 
//--------------------------------------------------------------------------------------------
unsigned int __stdcall	_FrontServerThread(void* pArg)
{
	
	DWORD				dwLastSendTime = 0;

	
	CFrontServerProc	Front;


	while( !g_bShutdown ) 
	{
		Sleep(10);	
		
			
		if( timeGetTime() - dwLastSendTime >= 30000 )
		{
			
			/* 동시 접속자수 보고 */

			Front.ReportConcurrent( g_GameServerInfoParser.GetWorldID(), 
									g_GameServerInfoParser.GetConcurrent() );

			dwLastSendTime = timeGetTime();				
		}							
			
	}

	return 0;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 프론트 서버 처리를 시작 한다 
//--------------------------------------------------------------------------------------------
void  FrontProcStart()
{
	unsigned int threadID;

	g_hFrontThread  = (HANDLE)_beginthreadex( NULL, 0, 
		_FrontServerThread,	 NULL, CREATE_SUSPENDED, &threadID);	
}

//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 서버 로그 디렉토리 생성
//  Author : 유재휘 2004.1.6
//--------------------------------------------------------------------------------------------
void createSvrDir()
{

	if (CreateDirectory(szDirPath, NULL) == 0 )
	{ 
		DWORD dw = GetLastError(); 
		if ( dw == ERROR_ALREADY_EXISTS )
		{
			printf("Already exist svrlog directory.\n"); 
		}
		else
		{
			printf("Couldn't create new svrlog directory[%u].\n",dw); 
		}
	}

	InitializeCriticalSection( &g_csLog );
}


//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 서버 로그 남기기 수정
//  Author : 유재휘 2004.1.6
//--------------------------------------------------------------------------------------------
void	writeInfoToFile(char *FileName, char *Content, ...)
{
	char	Buf[1024];
	char	FilePath[1024];
	char	DateBuf[128];
	char	TimeBuf[128];
	int		Count;
	FILE	*fp;
	struct	tm *today;

	char	thismonth[128];
	time_t	ltime;

	EnterCriticalSection(&g_csLog);

	va_list	arglist;
	va_start(arglist,Content);

	// Use strftime to build a customized time string.
	time(&ltime);
	today = localtime( &ltime );
	strftime( thismonth, 128, "%Y%m", today );

//	printf("[%s %s] %s\n",DateBuf,TimeBuf,Buf);
	sprintf( FilePath, "%s%s %s", szDirPath, thismonth, FileName);

	Count = wvsprintf(Buf, Content, arglist);
	fp	=	fopen(FilePath, "at");

	_strtime( TimeBuf );
	_strdate( DateBuf );
	fprintf(fp,"[%s %s] %s\n",DateBuf,TimeBuf,Buf);
	printf("[%s %s] %s\n",DateBuf,TimeBuf,Buf);
	fclose(fp);
	va_end(arglist);

	LeaveCriticalSection(&g_csLog);
}

//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : Main
//--------------------------------------------------------------------------------------------
SI32 main(SI32 argc, char *argv[])
{

	/////////////////////////////////////////////////////////
	// 이민수	/	2004. 02. 18
	//
	// Game server가 Memory crash 발생시 
	// Crash 주소를 찾아 파일 로그를 기록한다.
	// 검사항목
	// EXCEPTION ACCESS VIOLATION, EXCEPTION ARRAY BOUNDS EXCEEDED,
	// EXCEPTION_FLT_DIVIDE_BY_ZERO...등등
#ifdef __ENABLE_CRASH_FILE__
	InstallCrashHandler("Game server", "Crash[Game server]", "TEST_SERVER", CRASH_CLIENT);
#endif
	/////////////////////////////////////////////////////////
	
	WORD wVersionRequested;
	WSADATA wsaData;	
	int err;	
 
	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
	 
	if ( err != 0 ) { 
		printf("WSAStartup Failed [%d]\n",GetLastError());
	} 
 
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		 HIBYTE( wsaData.wVersion ) != 2 ) {

		printf("WSAStartup Failed - Version Error\n" );
		WSACleanup(); 		
	}

	createSvrDir();

	// 게임 서버의 정보를 읽어 온다 

	BOOL bRetVal = FALSE;

#ifdef __CONFIG_FILE_LOAD__
	bRetVal = g_GameServerInfoParser.Load();

	if(bRetVal == FALSE)
	{
		printf("Gersang.ini Read Failed!\n");
		getch(); return 0;
	}

#else
	bRetVal = g_GameServerInfoParser.Load("..\\GameServerInfo.txt");

	if(bRetVal == FALSE)
	{
		printf("GameServerInfo.txt Read Failed!\n");
		getch(); return 0;
	}
#endif	
	
	printf("GameServerInfo.txt Read Success!\n");
	

	DeleteFile( "ErrorCode.txt" );
 
#ifdef _GAMEGUARD
	if(g_GameServerInfoParser.IsGameGuard() == TRUE)
	{
		LoadAuthTable("..\\CSAuth.tab");
		LoadAuthIndex("..\\CSAuth.idx");
	}
#endif
	
	// SOServer 클레스
	SOServer	*clServer;			
	
	cAuthProc	AuthProc;
	cLoginProc  LoginProc;


	// Return Value 를 2로 초기화 ( 0: Shutdown, 1: Restart, 2: No ERROR )
	SI32 siReturnCode = 2;		

	while(siReturnCode > 0)
	{
		FrontProcStart();

		// Server 클레스 생성
		clServer		= new SOServer;

		AuthProc.SetServer( clServer );
		AuthProc.Start();

		LoginProc.SetServer( clServer );
		LoginProc.Start();

		ResumeThread(g_hFrontThread);

		printf("----------------------------------------------------------------------------\n");
		printf(" Online Gersang has been initialized successfully.\n");
		printf("----------------------------------------------------------------------------\n");

		
		// Server 클레스의 모든 연산 처리
		if( siReturnCode == 2 )		siReturnCode	= clServer->Poll();
		

		// Server 클레스 제거.
		delete clServer;
	}

	if(g_hFrontThread != NULL)
		CloseHandle(g_hFrontThread);

	printf("\nPress any key...\n");

	while(!kbhit());
	
	return 0;
}

