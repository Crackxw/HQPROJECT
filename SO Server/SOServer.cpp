#include "SOMain.h"
#include "SOWorld.h"
#include "SOServer.h"

#include "XGateServerMgr.h"
#include "XGateServer.h"
#include "XPlayerMgr.h"
#include "XPlayer.h"
#include "XMonitor.h"

#include "SOPlayer.h"

#include "..\\XCommon\\JXPacketQueue.h"
#include "..\\XCommon\\JXSocketMgr.h"
#include "..\\XCommon\\JXSocket.h"
#include "..\\XCommon\\JXConnectSocket.h"

#include "..\\XCommon\\XMonitorCmd.h"

#include "..\\XCommon\\XPacketCmd.h"
#include "..\\ServerMonitor\\SMSProtocol.h"

//--------------------------------------------
#include "..\\JWCommon\\JWListen.h"
#include "..\\JWCommon\\JWIOCPServer.h"


// CAuthServerProc
#include "../kojimodo/utility/Winsock.h"
#include "AuthServerProc.h"





//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// SOServer()曖 儅撩濠. 賅萇 滲熱菟擊 蟾晦�倆炾� 撲薑и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOServer::SOServer(VOID)
{	
	// 1. DB 蟾晦�� 
	//---------------------------------------------------------------------------------------------------------
	dbinit();

	// 2. 孺樓 蟾晦�� 
	//---------------------------------------------------------------------------------------------------------
//	WORD wVersionRequested;
//	WSADATA wsaData;	
//	int err;	
// 
//	wVersionRequested = MAKEWORD( 2, 2 );
//	err = WSAStartup( wVersionRequested, &wsaData );
//	 
//	if ( err != 0 ) { 
//		printf("WSAStartup Failed [%d]\n",GetLastError());
//	} 
// 
//	if ( LOBYTE( wsaData.wVersion ) != 2 ||
//		 HIBYTE( wsaData.wVersion ) != 2 ) {
//
//		printf("WSAStartup Failed - Version Error\n" );
//		WSACleanup(); 		
//	}

	
	// 3. 衛濛 �飛橉� ル衛и棻.
	//---------------------------------------------------------------------------------------------------------
	OpenConsole();
	
	
	// 4. 唳я纂 �倣狨� 碳楝螞棻 
	//---------------------------------------------------------------------------------------------------------
	////LoadExpData();


	// 5. 等檜顫 漆檜蝶 儅撩 
	//---------------------------------------------------------------------------------------------------------
	printf("---Database Init...---\r");	
	
	clDatabase		= new SODatabase();

	clDatabase->createQueue();
	clDatabase->ConnectDatabase();	
	
	clLoginDatabase = new SODatabase();	

	clLoginDatabase->ConnectDatabase();
		
	clItemDatabase = new SODatabase();
	
	clItemDatabase->ConnectDatabase2();

	// DB蘭葬 聽 籀葬 噙溯萄蒂 儅撩 
	clDatabase->createQueueThread();



	printf("---\tDatabase Completed\t---\n");
	

	// 6. SOWorld諦 SOGate 贗溯蝶 儅撩 (賅萇 翱骯婁 蕾樓  籀葬)
	//---------------------------------------------------------------------------------------------------------
	printf("---World Init...---\r");
		
	clWorld		= new SOWorld(this);	

	printf("---\tWorld Completed[Player:%d][World:%d]\t\t---\n",sizeof(SOPlayer),sizeof(SOWorld) );

	printf("---Gate Init...---\r");
	try{	
		clGate		= new SOGate(clWorld, clDatabase, this);
	}
	catch(std::bad_alloc){
		assert(0);
	}

	printf("---\tGate Completed\t\t---\n");

	clWorld->clGate	= clGate;

	//-----------------------------------------------------------------------------------
	// JW Server Engine 蟾晦��


	// Ы溯檜橫 葬蝶お 儅撩 塽 蟾晦��
	try{	
		m_pXPlayerMgr = new XPlayerMgr;
	}
	catch(std::bad_alloc){
		assert(0);
	}
	m_pXPlayerMgr->Create( ON_MAX_CONNECTION );


	// 偌羹 儅撩 
	try{	
		m_pListen = new JWListen;
	}
	catch(std::bad_alloc){
		assert(0);
	}

	try{	
		m_pIOCPServer = new JWIOCPServer;
	}
	catch(std::bad_alloc){
		assert(0);
	}

	// Server偌羹 蟾晦��( 識 蕾樓璋濠, 模鰍 渡 爾頂晦 幗ぷ 觼晦, 嫡晦 幗ぷ 觼晦 )
	m_pIOCPServer->InitServer( ON_MAX_CONNECTION, 60000, 60000 );

	// Listen偌羹 蟾晦��( Acceptл熱蒂 ��轎ж晦嬪и Server偌羹 ん檣攪, んお廓�� )
	m_pListen->Init( m_pIOCPServer, 8000 );

	//-----------------------------------------------------------------------------------

	
	// 億煎遴 XPlayer諦 World縑 氈朝 SOPlayer蒂 翱唸..  
	JWObject *itor = m_pXPlayerMgr->m_pFreeFirst->GetNext();		
	UI32	 i;
	for( i = 0; i < m_pXPlayerMgr->m_uiFreeNumber; ++i ) {

		((XPlayer *)(itor))->m_usAccount = i + 1;
		((XPlayer *)(itor))->m_pSOPlayer = (SOPlayer *)clWorld->clPlayer[ i ];
		((XPlayer *)(itor))->m_pServer = this;
		clWorld->clPlayer[ i ]->m_pXPlayer = (XPlayer *)itor;

		itor = itor->GetNext();
	}


	// 葬蝦 んお蒂 翮堅 蕾樓擊 晦棻萼棻
	m_pListen->Start();

	// 賅棲攪葭 憮幗諦曖 鱔褐	
	try{
		m_pMonitorSocket = new JXConnectSocket;
	}
	catch(std::bad_alloc){
		assert(0);
	}
	try{
		m_pMonitor = new XMonitor;
	}
	catch(std::bad_alloc){
		assert(0);
	}

	m_pMonitorSocket->Create( "127.0.0.1", 12002, 100000, 100000, 100000 );
	m_pMonitor->SetSocket( m_pMonitorSocket );

	m_pMonitorSocket->StartThread();


    // 8. 譆謙瞳檣 蟾晦�� 
	//---------------------------------------------------------------------------------------------------------
	Init();


	


	//====================================================
	// AUTHENTICATION: 檣隸憮幗縑 憮幗 葬撢擊 蹂羶 и棻 
	//====================================================
	
	
	
	CAuthServerProc AuthServer(0); // 0擎 蹂羶�� 鼠и渠晦蒂 曖嘐 и棻 
	

	char responseKind;
	AuthServer.RequestReset( g_GameServerInfoParser.GetWorldID(), 
							 &responseKind );

	
	

	// 憮幗 衛濛 衛除 晦煙 
//	WriteInfo("..\\ConnectStatus.txt","Server Started");		
	writeInfoToFile("ConnectStatus.txt","Server Started");		
	return;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// SOServer曖 模資濠
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOServer::~SOServer()
{

	if( clGate ) {
		
		delete	clGate;		
		clGate	= NULL;
		
	}

	if( clWorld ) {

		delete	clWorld;				
		clWorld  = NULL;

	}

	if( clDatabase ){

		
		delete	clDatabase;
		delete  clLoginDatabase;
		delete  clItemDatabase;
		
		clDatabase		= NULL;		
		clLoginDatabase = NULL;


	}

	delete m_pListen;
	delete m_pIOCPServer;

	delete m_pMonitorSocket;
	delete m_pMonitor;
	
	delete m_pXPlayerMgr;

	// 孺樓 謙猿 
	WSACleanup();
	
	// DB 謙猿 
	dbexit();

	// 謙猿 �飛橉� 轎溘и棻.
	CloseConsole();

	return;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 蟾晦�� и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID SOServer::Init()
{
	iLoopTime		 = 0;	
	iLoopCount		 = 0;

	bShutDownOk		 = FALSE;		// 樣棻遴 鼻鷓陛 嬴棲棻.		
	bConsoleLock	 = TRUE;		// 酈爾萄 塊 鼻鷓

	dwCurrentFPS	 = 0;
	dwMaxCurrentFPS	 = 0;
	dwMinCurrentFPS	 = ULONG_MAX;

	dwWorldCurrentFPS		= 0;
	dwWorldMaxCurrentFPS	= 0;
	dwWorldMinCurrentFPS	= ULONG_MAX;

	dwSmsLastTime			= timeGetTime();
}




//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 衛濛 �飛橉� 轎溘и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID SOServer::OpenConsole(VOID)
{
	printf("----------------------------------------------------------------------------\n");
	printf("                                          Online Gersang Game Server Ver 0.1\n");
	printf("----------------------------------------------------------------------------\n");
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 謙猿 �飛橉� 轎溘и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID SOServer::CloseConsole(VOID)
{
	printf("----------------------------------------------------------------------------\n");
	printf(" Server Shut down...........................................................\n");
	printf("----------------------------------------------------------------------------\n");
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// SOServer曖 詭檣 л熱
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32 SOServer::Poll()
{
	
	SI32	siReturnCode			= 2; // 啗樓 瑞Щ蒂 給紫煙 撲薑и棻.
	SI32	siFrameLoop				= 0;
	DWORD	dwLastTick				= 0;
	DWORD	dwCount					= 0;
	DWORD   dwLatestNumUserClock    = 0;
	UI32	uiCurrentClock			= 0;
	UI32	uiWorldCurrentClock		= 0;
	DWORD	dwLastAcceptClock		= 0;

	g_forceShutdown = FALSE;

	FLOAT	fInterval				= 0;

	DWORD	jdwFrameCount			= 0;
	
	DWORD	jdwLastFrameCount		= 0;
	DWORD	jdwLastWriteCPS			= 0;
	DWORD	jdwLastGetDBCPS			= 0;

	DWORD	jdwLastTick				= GetTickCount();

	while( 1 )
	{

		jdwFrameCount++;


		fInterval = GetTickCount() - jdwLastTick;


		if( fInterval > 5000 ) {
			
			jdwLastTick += 5000;

			fFPS = ( jdwFrameCount - jdwLastFrameCount ) * 1000 / fInterval;
			fWriteCPS = ( clDatabase->m_countWrite - jdwLastWriteCPS ) * 1000 / fInterval;
			fGetDBCPS = ( clDatabase->m_countGetDB - jdwLastGetDBCPS ) * 1000 / fInterval;

			jdwLastFrameCount = jdwFrameCount;
			jdwLastWriteCPS = clDatabase->m_countWrite;
			jdwLastGetDBCPS = clDatabase->m_countGetDB;
		
		}


		// Loop Timer 衛濛, ⑷營 衛陝 晦煙 			
		StartMilliTimer( iLoopS, iLoopMS );   		
		uiCurrentClock = timeGetTime();	 		

		Sleep( 10 );

		// 憮幗 謙猿 鼻鷓 塭賊 瑞Щ蒂 謙猿ж堅 釭除棻				
		if(bShutDownOk)
		{			
			siReturnCode = 0;
			break;
		}
				

		// 30蟾縑 и廓噶 ⑷營 譆渠 嶸盪熱蒂 盪濰и棻 				
		if( (uiCurrentClock - dwLatestNumUserClock) > 30000 )
		{
			dwLatestNumUserClock = uiCurrentClock;			
			clDatabase->writeDBQuery( SO_DB_INFOKIND_CONCURRENTUSER, &clGate->siTotalConnectPlayer, 4 );		
			
			g_GameServerInfoParser.SetConcurrent( clGate->siTotalConnectPlayer );
		}


		// 5 蟾縑 и廓噶  Щ溯歜 瑞Щ蒂 偵褐и棻 				
		if( (uiCurrentClock - dwLastTick)  > 5000)
		{
			dwLastTick  = uiCurrentClock;
			siFrameLoop = dwCount / 5;
			dwCount     = 0;			 
			SendHeartBeat();
		} else {
			++dwCount;
		}

		UI32	 i;

		

		m_pIOCPServer->UpdateServer( m_pXPlayerMgr );



		//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
		// Ы溯檜橫 ぬ韃 Щ煎衛盪..
		//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收


		for( i = 0; i < ON_MAX_CONNECTION; ++i ) {
			ProcPacketPlayer( clWorld->clPlayer[ i ]->m_pXPlayer );
		}


		//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
		// 錯萄曖 賅萇 翱骯婁 酈爾萄 殮溘 籀葬 	
		uiWorldCurrentClock		=	timeGetTime();
		
		siReturnCode = clWorld->Poll();			

		ProcPacketMonitor( m_pMonitor );

		siReturnCode = ProcessConsole();
		
		// World Щ溯歜擊 掘и棻			
		dwWorldCurrentFPS	=	1000 / max(1, GETTIMEGAP(uiWorldCurrentClock, timeGetTime()));
		if(dwWorldCurrentFPS > dwWorldMaxCurrentFPS)		dwWorldMaxCurrentFPS		=	dwWorldCurrentFPS;
		if(dwWorldCurrentFPS < dwWorldMinCurrentFPS)		dwWorldMinCurrentFPS		=	dwWorldCurrentFPS;


		// 爾婦л縑 氈朝 蘭葬蒂 綠遺 
		clDatabase->emptiedDepository();

		
		// 鬼薯煎 憮幗蒂 謙猿 п撿 ж朝唳辦 			
		if( g_forceShutdown == TRUE )
		{
			for( i = 1; i < ON_MAX_CONNECTION+1; ++i )
			{
				if( clWorld->pPlayerOnAccount[i] ) clWorld->pPlayerOnAccount[i]->LogOut();
			}

			clDatabase->emptiedDepository();

			printf("Waiting for settlement while closing the server forcibly\n");
			
			writeInfoToFile("DBError.txt", "Waiting for settlement while closing the server forcibly");
			
			while( clDatabase->getGateQueueSize() > 0 || clDatabase->getSubQueueSize() > 0 )
			{
				Sleep( 30000 );							
			}				
			
			writeInfoToFile("DBError.txt", "Succeded to manage all queries from the DB");
			getch();
			exit(-1);			
		}


		// Щ溯歜擊 掘и棻
		dwCurrentFPS	=	1000 / max(1, GETTIMEGAP(uiCurrentClock, timeGetTime()));

		if(dwCurrentFPS > dwMaxCurrentFPS)		dwMaxCurrentFPS		=	dwCurrentFPS;
		if(dwCurrentFPS < dwMinCurrentFPS)		dwMinCurrentFPS		=	dwCurrentFPS;

		// 瑞Щ蒂 и廓 紫朝等 勘葬朝 衛除 難薑 				
		iCalcTime = CheckMilliTimer(iLoopS,iLoopMS);	// Loop 衛除 啗骯 
		iLoopTime+= iCalcTime;							

		// 瑞Щ蒂 謙猿и棻.
		if(siReturnCode != 2) break;
	}

	return siReturnCode;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 酈籀葬 и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32 SOServer::ProcessConsole(VOID)
{
	char	cChar;

	while(kbhit())
	{
		cChar=getch();

		if(!bConsoleLock)
		{
			switch(toupper(cChar))	// 渠僥濠煎 夥脯憮 籀葬и棻.
			{
			// 憮幗蒂 謙猿и棻.
			case '\x1b':
				g_bShutdown = TRUE;
				clWorld->LogOutAllPlayer();
				return 0;


			// 賅萇 Ы溯檜橫蒂 煎斜嬴醒 衛鑑棻
			case '*':
				LogOutPlayerAll();
				break;


			// 酈爾萄蒂 濡斬棻.
			case 'L':
				printf( "Keyboard Locked\n" );
				bConsoleLock = TRUE;
				break;

			// ⑷營 蕾樓醞檣 餌辨濠熱蒂 �飛橦� 爾罹遽棻.
			case 'P':				
				printf( "Total Connected User : Max : [%u] Cur : [%u]\n", clGate->siMaxConnectedPlayer, clGate->siTotalConnectPlayer );
				break;


			// ⑷營 Щ溯歜 熱蒂 爾罹遽棻.
			case 'R':				
				printf( "Max FPS : [%u], Min FPS : [%u], Cur FPS : [%u]\n", dwMaxCurrentFPS, dwMinCurrentFPS, dwCurrentFPS);
				break;


			case '1':
				printf( "fps:[%.2f], DB Query Write cps:[%.2f], DB Query & Response cps:[%.2f]\n ", fFPS, fWriteCPS, fGetDBCPS );
				printf( "fps:[%.2f], DB Query Write cps:[%.2f], DB Query & Response cps:[%.2f]\n ", fFPS, fWriteCPS, fGetDBCPS );
				printf( "fps:[%.2f], DB Query Write cps:[%.2f], DB Query & Response cps:[%.2f]\n ", fFPS, fWriteCPS, fGetDBCPS );
				break;

/*			case 'C':
				{
					printf("憮幗曖 衛除擊 滲唳м棲棻.");
					clWorld->bChangeTime = true;
					printf("衛蒂 殮溘ж撮蹂\n");
					cChar = getch();
					clWorld->pChangeTime->tm_hour = atoi(&cChar);
					
					printf("碟擊 殮溘ж撮蹂\n");
					cChar = getch();
					clWorld->pChangeTime->tm_min = atoi(&cChar);					
				}
				break;
*/
			default:
				printf("........... Message : Unknown Control Command.\n");
			}
		}
		else
		{
			switch(toupper(cChar))
			{
			case 'U':
				printf("Keyboard UnLocked\n");
				bConsoleLock = FALSE;
				break;

			// Server曖 Loop 鼻鷓蒂 憲嬴獄棻.
			case 'I':	
				printf("# Loop Rate:%03.5f\r",ServerLoad());
				break;			
			
			}
		}
	}

	return 2;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 憮幗曖 鱔啗 薑爾蒂 噹棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
/*
VOID SOServer::WriteInfo(char *FileName, const char *Content,...)
{

	char	Buf[1024];
	char	DateBuf[128];
	char	TimeBuf[128];
	int		Count;
	FILE	*fp;
	va_list	arglist;
	va_start(arglist,Content);
	Count = vsprintf(Buf, Content, arglist);
	fp	=	fopen(FileName, "at");
	_strtime( TimeBuf );
	_strdate( DateBuf );
	fprintf(fp,"[%s %s] %s\n",DateBuf,TimeBuf,Buf);
	printf("[%s %s] %s\n",DateBuf,TimeBuf,Buf);
	fclose(fp);
	va_end(arglist);
}
*/
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 夥檜傘葬 だ橾煎 煎斜蒂 陴曹棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID SOServer::WriteBinary( char *FileName, const char *pContent, int iSize )
{
	FILE	*fp;

	if( (fp = fopen(FileName, "ab")) == NULL )		return;
	fwrite( pContent, sizeof(char), iSize, fp );
	fclose( fp );
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//  Ы溯檜橫 ぬ韃擊 籀葬и棻
//	檜薯諫 2002-7-10
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
void SOServer::ProcPacketPlayer( XPlayer *pXPlayer )
{

	if( pXPlayer->IsOnAction() ) CompleteLogOut( pXPlayer );
	else pXPlayer->m_pSOPlayer->Poll();
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	億煎遴 Ы溯檜橫 蕾樓 籀葬	
//	檜薯諫 2002-7-10
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
void SOServer::NewPlayer( XPlayer *pXPlayer )
{

	//printf("New Player..\n");

	SOPlayer *pSOPlayer;

	if( pXPlayer == NULL ) {
		printf( "Failed!\n" );
		return;
	}

	pSOPlayer = pXPlayer->m_pSOPlayer;

	pSOPlayer->Create( this );

	pSOPlayer->m_lastTick = GetTickCount();

	pSOPlayer->m_mainState.set( USER_IN_AUTH );


	OnEncryptionKey msg;

	msg.dwMsgType = ON_NOTICE_ENCRYPTION_KEY;
	msg.cKey1 = pXPlayer->m_cKey1;
	msg.cKey2 = pXPlayer->m_cKey2;

	memcpy(msg.dwRoundKey, pXPlayer->m_dwRoundKey, sizeof(DWORD) * 32);
	
	pSOPlayer->Write( &msg, sizeof( msg ) );

	return;
}




//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	Ы溯檜橫 煎斜 嬴醒 ( 跡煙縑憮 煎斜 嬴醒 й Ы溯檜橫蒂 瓊嬴憮 煎斜 嬴醒 衛鑑棻 )
//	檜薯諫 2002-7-10
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
void SOServer::LogOutPlayer( UI16 usAccount )
{

	UI32 i;
	XPlayer *pXPlayer;

	JWLIST_LOOP( i, m_pXPlayerMgr, XPlayer, pXPlayer )
	{
		
		if( pXPlayer->m_usAccount == usAccount ) {
			LogOutPlayer( pXPlayer );
			break;
		}
	}
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	Ы溯檜橫 煎斜 嬴醒 ( 褒薯煎 и Ы溯檜橫曖 煎斜嬴醒 籀葬蒂 熱чи棻 )
//	檜薯諫 2002-7-10
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
void SOServer::LogOutPlayer( XPlayer *pXPlayer )
{
	pXPlayer->SetAction();
}

void SOServer::CompleteLogOut( XPlayer *pXPlayer )
{
	SOPlayer *pPlayer = (SOPlayer *)( pXPlayer->m_pSOPlayer );
	
	pPlayer->m_mainState.logout();

	if( pPlayer->m_mainState.match( USER_MUST_LOGOUT_IN_GAME ) ) {

		InterlockedDecrement( (long *)&clGate->siTotalConnectPlayer );

		pPlayer->m_mainState.set( USER_WAIT_FOR_LOGOUT );

	} 

	if( pPlayer->m_mainState.match( USER_WAIT_FOR_LOGOUT ) == TRUE ) {

		pPlayer->m_mainState.set( NONE_STATE );
		m_pIOCPServer->CompleteLogOut( (JWSocket *)(pXPlayer->GetFriend()) );

	}
	
}

void SOServer::LogOutPlayerAll()
{
	UI32 i;

	XPlayer *pXPlayer;

	m_pListen->Close();

	JWLIST_LOOP( i, m_pXPlayerMgr, XPlayer, pXPlayer )
	{
		LogOutPlayer( pXPlayer );
	}

}

void SOServer::__LogOutPlayer( XPlayer *pXPlayer )
{
	SOPlayer *pSOPlayer = pXPlayer->m_pSOPlayer;

	if(pSOPlayer->stAccountInfo.pVillage !=  NULL)
	{
		if(pSOPlayer->stAccountInfo.pVillage->m_bVillageInLimit == TRUE)
		{
			pSOPlayer->stAccountInfo.pVillage->m_bVillageInLimit = FALSE;

			pSOPlayer->stAccountInfo.pVillage->DeletCharName(pSOPlayer->szID);

//			if(strcmp(pSOPlayer->stAccountInfo.pVillage->m_szCharName,pSOPlayer->szID) == 0)
//			{
//				strcpy(pSOPlayer->stAccountInfo.pVillage->m_szCharName,"");
//			}
		}
	}
	
	// 檜 嶸盪蒂 啪歜寰縑憮 薯剪 и棻 
	pSOPlayer->removeSelfInGame();
	
	// ん虜馬,嶸盪曖 辨煽菟薑爾,⑷營 嬪纂,絲擊 DB縑 盪濰и棻 	
	pSOPlayer->saveSatietyToDB();
	pSOPlayer->saveAllFollowerDataToDB();
	pSOPlayer->saveLogoutInfoToDB();
	pSOPlayer->saveLogoutStartToDB();
	
	//奢撩衛除縑虜 盪濰擊 и棻.
	if(pSOPlayer->IsFieldAttackUnit() == TRUE)
		pSOPlayer->saveLogountWarInfoToDB();
	
	// 嶸盪 薑爾 蟾晦�� 
	pSOPlayer->InitEnv();
	pSOPlayer->uiAccount = pXPlayer->m_usAccount;

	// 嶸盪蒂 煎斜 嬴醒 鼻鷓煎 虜萇棻 
	clWorld->pPlayerOnAccount[ pXPlayer->m_usAccount ] = NULL;
	pSOPlayer->uiState = ON_STATE_LOGOUT;
}


void SOServer::ProcPacketMonitor( XMonitor *pMonitor )
{
	BOOL bConnect;
	JXPacketQueue *pPacketQueue = NULL;

	pPacketQueue = pMonitor->GetSocket()->GetQueue();

	pMonitor->GetSocket()->IsConnect( &bConnect );

	if( bConnect == FALSE ) {

		pPacketQueue->Clear();

		m_pMonitorSocket->TryConnect();

		return;
	}

	UI32	uiNumPacket = pPacketQueue->GetNumPacket();

	UI16	usCmd;
//	UI16	usAccount;
//	SI32	siPacketSize;
//	UI32	uiTemp;
	static  char	buf[ 512 ];

	for( UI32 i = 0; i < uiNumPacket; ++i ) {
		
		if( pPacketQueue->Dequeue( &local_packetE ) == FALSE ) return;

		local_packetE.ReadValue( &usCmd, sizeof( usCmd ) );

		switch( usCmd ) {
		case MONITOR_QUERY_NOTICE:
			local_packetE.ReadValue( buf, 256 );
			clWorld->clCommand.Notice( buf );
			break;

		case MONITOR_QUERY_LOGOUT:
			LogOutPlayerAll();
			break;

		///////////////////////////////////////////////////////////
		// SMS 婦溼
		//
		case SS_SMS_REQ_MSG : 
			{
				DWORD dwSmsCurrentTime = timeGetTime();
				if(dwSmsLastTime < (dwSmsCurrentTime - 5000))
				{
					SMS_REQ_MSG sSMS;
					memset(&sSMS, 0, sizeof(SMS_REQ_MSG));

					local_packetE.ReadValue(&sSMS, sizeof(SMS_REQ_MSG));

					clWorld->clCommand.SendSMSMsg(&sSMS);

					dwSmsLastTime = dwSmsCurrentTime;
				}
			}
			break;
		///////////////////////////////////////////////////////////
		}

	}

}

void SOServer::SendHeartBeat()
{
	UI16 uiCmd;
	UI16 uiNum = (UI16)( clGate->siTotalConnectPlayer );

	DWORD pid = GetCurrentProcessId();
	int gatequeuesize = clDatabase->getGateQueueSize();
	int subqueuesize = clDatabase->getSubQueueSize();

	uiCmd = MONITORCMD_CURRENT_CONNECT;

	local_packetD.ClearPacket();
	local_packetD.WriteValue( &uiCmd, sizeof( uiCmd ) );
	local_packetD.WriteValue( &pid, sizeof( pid ) );
	local_packetD.WriteValue( &uiNum, sizeof( uiNum ) );
	local_packetD.CompletePacket();

	m_pMonitor->GetSocket()->SendPacket( &local_packetD );

	uiCmd = MONITORCMD_CURRENT_QUEUE;

	local_packetD.ClearPacket();
	local_packetD.WriteValue( &uiCmd, sizeof( uiCmd ) );
	local_packetD.WriteValue( &gatequeuesize, sizeof( gatequeuesize ) );
	local_packetD.WriteValue( &subqueuesize, sizeof( subqueuesize ) );
	local_packetD.CompletePacket();

	m_pMonitor->GetSocket()->SendPacket( &local_packetD );


}

