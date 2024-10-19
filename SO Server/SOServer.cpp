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





//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// SOServer()ÀÇ »ý¼ºÀÚ. ¸ðµç º¯¼öµéÀ» ÃÊ±âÈ­ÇÏ°í ¼³Á¤ÇÑ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
SOServer::SOServer(VOID)
{	
	// 1. DB ÃÊ±âÈ­ 
	//---------------------------------------------------------------------------------------------------------
	dbinit();

	// 2. À©¼Ó ÃÊ±âÈ­ 
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

	
	// 3. ½ÃÀÛ È­¸éÀ» Ç¥½ÃÇÑ´Ù.
	//---------------------------------------------------------------------------------------------------------
	OpenConsole();
	
	
	// 4. °æÇèÄ¡ È­ÀÏÀ» ºÒ·¯¿Â´Ù 
	//---------------------------------------------------------------------------------------------------------
	////LoadExpData();


	// 5. µ¥ÀÌÅ¸ º£ÀÌ½º »ý¼º 
	//---------------------------------------------------------------------------------------------------------
	printf("---Database Init...---\r");	
	
	clDatabase		= new SODatabase();

	clDatabase->createQueue();
	clDatabase->ConnectDatabase();	
	
	clLoginDatabase = new SODatabase();	

	clLoginDatabase->ConnectDatabase();
		
	clItemDatabase = new SODatabase();
	
	clItemDatabase->ConnectDatabase2();

	// DBÄõ¸® Å¥ Ã³¸® ¾²·¹µå¸¦ »ý¼º 
	clDatabase->createQueueThread();



	printf("---\tDatabase Completed\t---\n");
	

	// 6. SOWorld¿Í SOGate Å¬·¹½º »ý¼º (¸ðµç ¿¬»ê°ú Á¢¼Ó  Ã³¸®)
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
	// JW Server Engine ÃÊ±âÈ­


	// ÇÃ·¹ÀÌ¾î ¸®½ºÆ® »ý¼º ¹× ÃÊ±âÈ­
	try{	
		m_pXPlayerMgr = new XPlayerMgr;
	}
	catch(std::bad_alloc){
		assert(0);
	}
	m_pXPlayerMgr->Create( ON_MAX_CONNECTION );


	// °´Ã¼ »ý¼º 
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

	// Server°´Ã¼ ÃÊ±âÈ­( ÃÑ Á¢¼Ó¼ýÀÚ, ¼ÒÄÏ ´ç º¸³»±â ¹öÆÛ Å©±â, ¹Þ±â ¹öÆÛ Å©±â )
	m_pIOCPServer->InitServer( ON_MAX_CONNECTION, 60000, 60000 );

	// Listen°´Ã¼ ÃÊ±âÈ­( AcceptÇÔ¼ö¸¦ È£ÃâÇÏ±âÀ§ÇÑ Server°´Ã¼ Æ÷ÀÎÅÍ, Æ÷Æ®¹øÈ£ )
	m_pListen->Init( m_pIOCPServer, 8000 );

	//-----------------------------------------------------------------------------------

	
	// »õ·Î¿î XPlayer¿Í World¿¡ ÀÖ´Â SOPlayer¸¦ ¿¬°á..  
	JWObject *itor = m_pXPlayerMgr->m_pFreeFirst->GetNext();		
	UI32	 i;
	for( i = 0; i < m_pXPlayerMgr->m_uiFreeNumber; ++i ) {

		((XPlayer *)(itor))->m_usAccount = i + 1;
		((XPlayer *)(itor))->m_pSOPlayer = (SOPlayer *)clWorld->clPlayer[ i ];
		((XPlayer *)(itor))->m_pServer = this;
		clWorld->clPlayer[ i ]->m_pXPlayer = (XPlayer *)itor;

		itor = itor->GetNext();
	}


	// ¸®½¼ Æ÷Æ®¸¦ ¿­°í Á¢¼ÓÀ» ±â´Ù¸°´Ù
	m_pListen->Start();

	// ¸ð´ÏÅÍ¸µ ¼­¹ö¿ÍÀÇ Åë½Å	
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


    // 8. ÃÖÁ¾ÀûÀÎ ÃÊ±âÈ­ 
	//---------------------------------------------------------------------------------------------------------
	Init();


	


	//====================================================
	// AUTHENTICATION: ÀÎÁõ¼­¹ö¿¡ ¼­¹ö ¸®¼ÂÀ» ¿äÃ» ÇÑ´Ù 
	//====================================================
	
	
	
	CAuthServerProc AuthServer(0); // 0Àº ¿äÃ»ÈÄ ¹«ÇÑ´ë±â¸¦ ÀÇ¹Ì ÇÑ´Ù 
	

	char responseKind;
	AuthServer.RequestReset( g_GameServerInfoParser.GetWorldID(), 
							 &responseKind );

	
	

	// ¼­¹ö ½ÃÀÛ ½Ã°£ ±â·Ï 
//	WriteInfo("..\\ConnectStatus.txt","Server Started");		
	writeInfoToFile("ConnectStatus.txt","Server Started");		
	return;
}



//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// SOServerÀÇ ¼Ò¸êÀÚ
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
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

	// À©¼Ó Á¾·á 
	WSACleanup();
	
	// DB Á¾·á 
	dbexit();

	// Á¾·á È­¸éÀ» Ãâ·ÂÇÑ´Ù.
	CloseConsole();

	return;
}



//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// ÃÊ±âÈ­ ÇÑ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID SOServer::Init()
{
	iLoopTime		 = 0;	
	iLoopCount		 = 0;

	bShutDownOk		 = FALSE;		// ¼Ë´Ù¿î »óÅÂ°¡ ¾Æ´Ï´Ù.		
	bConsoleLock	 = TRUE;		// Å°º¸µå ¶ô »óÅÂ

	dwCurrentFPS	 = 0;
	dwMaxCurrentFPS	 = 0;
	dwMinCurrentFPS	 = ULONG_MAX;

	dwWorldCurrentFPS		= 0;
	dwWorldMaxCurrentFPS	= 0;
	dwWorldMinCurrentFPS	= ULONG_MAX;

	dwSmsLastTime			= timeGetTime();
}




//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// ½ÃÀÛ È­¸éÀ» Ãâ·ÂÇÑ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID SOServer::OpenConsole(VOID)
{
	printf("----------------------------------------------------------------------------\n");
	printf("                                          Online Gersang Game Server Ver 0.1\n");
	printf("----------------------------------------------------------------------------\n");
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// Á¾·á È­¸éÀ» Ãâ·ÂÇÑ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID SOServer::CloseConsole(VOID)
{
	printf("----------------------------------------------------------------------------\n");
	printf(" Server Shut down...........................................................\n");
	printf("----------------------------------------------------------------------------\n");
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// SOServerÀÇ ¸ÞÀÎ ÇÔ¼ö
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
SI32 SOServer::Poll()
{
	
	SI32	siReturnCode			= 2; // °è¼Ó ·çÇÁ¸¦ µ¹µµ·Ï ¼³Á¤ÇÑ´Ù.
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


		// Loop Timer ½ÃÀÛ, ÇöÀç ½Ã°¢ ±â·Ï 			
		StartMilliTimer( iLoopS, iLoopMS );   		
		uiCurrentClock = timeGetTime();	 		

		Sleep( 10 );

		// ¼­¹ö Á¾·á »óÅÂ ¶ó¸é ·çÇÁ¸¦ Á¾·áÇÏ°í ³ª°£´Ù				
		if(bShutDownOk)
		{			
			siReturnCode = 0;
			break;
		}
				

		// 30ÃÊ¿¡ ÇÑ¹ø¾¿ ÇöÀç ÃÖ´ë À¯Àú¼ö¸¦ ÀúÀåÇÑ´Ù 				
		if( (uiCurrentClock - dwLatestNumUserClock) > 30000 )
		{
			dwLatestNumUserClock = uiCurrentClock;			
			clDatabase->writeDBQuery( SO_DB_INFOKIND_CONCURRENTUSER, &clGate->siTotalConnectPlayer, 4 );		
			
			g_GameServerInfoParser.SetConcurrent( clGate->siTotalConnectPlayer );
		}


		// 5 ÃÊ¿¡ ÇÑ¹ø¾¿  ÇÁ·¹ÀÓ ·çÇÁ¸¦ °»½ÅÇÑ´Ù 				
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



		//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
		// ÇÃ·¹ÀÌ¾î ÆÐÅ¶ ÇÁ·Î½ÃÀú..
		//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬


		for( i = 0; i < ON_MAX_CONNECTION; ++i ) {
			ProcPacketPlayer( clWorld->clPlayer[ i ]->m_pXPlayer );
		}


		//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
		// ¿ùµåÀÇ ¸ðµç ¿¬»ê°ú Å°º¸µå ÀÔ·Â Ã³¸® 	
		uiWorldCurrentClock		=	timeGetTime();
		
		siReturnCode = clWorld->Poll();			

		ProcPacketMonitor( m_pMonitor );

		siReturnCode = ProcessConsole();
		
		// World ÇÁ·¹ÀÓÀ» ±¸ÇÑ´Ù			
		dwWorldCurrentFPS	=	1000 / max(1, GETTIMEGAP(uiWorldCurrentClock, timeGetTime()));
		if(dwWorldCurrentFPS > dwWorldMaxCurrentFPS)		dwWorldMaxCurrentFPS		=	dwWorldCurrentFPS;
		if(dwWorldCurrentFPS < dwWorldMinCurrentFPS)		dwWorldMinCurrentFPS		=	dwWorldCurrentFPS;


		// º¸°üÇÔ¿¡ ÀÖ´Â Äõ¸®¸¦ ºñ¿ò 
		clDatabase->emptiedDepository();

		
		// °­Á¦·Î ¼­¹ö¸¦ Á¾·á ÇØ¾ß ÇÏ´Â°æ¿ì 			
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


		// ÇÁ·¹ÀÓÀ» ±¸ÇÑ´Ù
		dwCurrentFPS	=	1000 / max(1, GETTIMEGAP(uiCurrentClock, timeGetTime()));

		if(dwCurrentFPS > dwMaxCurrentFPS)		dwMaxCurrentFPS		=	dwCurrentFPS;
		if(dwCurrentFPS < dwMinCurrentFPS)		dwMinCurrentFPS		=	dwCurrentFPS;

		// ·çÇÁ¸¦ ÇÑ¹ø µµ´Âµ¥ °É¸®´Â ½Ã°£ ÃøÁ¤ 				
		iCalcTime = CheckMilliTimer(iLoopS,iLoopMS);	// Loop ½Ã°£ °è»ê 
		iLoopTime+= iCalcTime;							

		// ·çÇÁ¸¦ Á¾·áÇÑ´Ù.
		if(siReturnCode != 2) break;
	}

	return siReturnCode;
}



//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// Å°Ã³¸® ÇÑ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
SI32 SOServer::ProcessConsole(VOID)
{
	char	cChar;

	while(kbhit())
	{
		cChar=getch();

		if(!bConsoleLock)
		{
			switch(toupper(cChar))	// ´ë¹®ÀÚ·Î ¹Ù²ã¼­ Ã³¸®ÇÑ´Ù.
			{
			// ¼­¹ö¸¦ Á¾·áÇÑ´Ù.
			case '\x1b':
				g_bShutdown = TRUE;
				clWorld->LogOutAllPlayer();
				return 0;


			// ¸ðµç ÇÃ·¹ÀÌ¾î¸¦ ·Î±×¾Æ¿ô ½ÃÅ²´Ù
			case '*':
				LogOutPlayerAll();
				break;


			// Å°º¸µå¸¦ Àá±Ù´Ù.
			case 'L':
				printf( "Keyboard Locked\n" );
				bConsoleLock = TRUE;
				break;

			// ÇöÀç Á¢¼ÓÁßÀÎ »ç¿ëÀÚ¼ö¸¦ È­¸é¿¡ º¸¿©ÁØ´Ù.
			case 'P':				
				printf( "Total Connected User : Max : [%u] Cur : [%u]\n", clGate->siMaxConnectedPlayer, clGate->siTotalConnectPlayer );
				break;


			// ÇöÀç ÇÁ·¹ÀÓ ¼ö¸¦ º¸¿©ÁØ´Ù.
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
					printf("¼­¹öÀÇ ½Ã°£À» º¯°æÇÕ´Ï´Ù.");
					clWorld->bChangeTime = true;
					printf("½Ã¸¦ ÀÔ·ÂÇÏ¼¼¿ä\n");
					cChar = getch();
					clWorld->pChangeTime->tm_hour = atoi(&cChar);
					
					printf("ºÐÀ» ÀÔ·ÂÇÏ¼¼¿ä\n");
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

			// ServerÀÇ Loop »óÅÂ¸¦ ¾Ë¾Æº»´Ù.
			case 'I':	
				printf("# Loop Rate:%03.5f\r",ServerLoad());
				break;			
			
			}
		}
	}

	return 2;
}



//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// ¼­¹öÀÇ Åë°è Á¤º¸¸¦ ¾´´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
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
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// ¹ÙÀÌ³Ê¸® ÆÄÀÏ·Î ·Î±×¸¦ ³²±ä´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID SOServer::WriteBinary( char *FileName, const char *pContent, int iSize )
{
	FILE	*fp;

	if( (fp = fopen(FileName, "ab")) == NULL )		return;
	fwrite( pContent, sizeof(char), iSize, fp );
	fclose( fp );
}



//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//  ÇÃ·¹ÀÌ¾î ÆÐÅ¶À» Ã³¸®ÇÑ´Ù
//	ÀÌÁ¦¿Ï 2002-7-10
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
void SOServer::ProcPacketPlayer( XPlayer *pXPlayer )
{

	if( pXPlayer->IsOnAction() ) CompleteLogOut( pXPlayer );
	else pXPlayer->m_pSOPlayer->Poll();
}



//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	»õ·Î¿î ÇÃ·¹ÀÌ¾î Á¢¼Ó Ã³¸®	
//	ÀÌÁ¦¿Ï 2002-7-10
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
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




//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	ÇÃ·¹ÀÌ¾î ·Î±× ¾Æ¿ô ( ¸ñ·Ï¿¡¼­ ·Î±× ¾Æ¿ô ÇÒ ÇÃ·¹ÀÌ¾î¸¦ Ã£¾Æ¼­ ·Î±× ¾Æ¿ô ½ÃÅ²´Ù )
//	ÀÌÁ¦¿Ï 2002-7-10
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
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



//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	ÇÃ·¹ÀÌ¾î ·Î±× ¾Æ¿ô ( ½ÇÁ¦·Î ÇÑ ÇÃ·¹ÀÌ¾îÀÇ ·Î±×¾Æ¿ô Ã³¸®¸¦ ¼öÇàÇÑ´Ù )
//	ÀÌÁ¦¿Ï 2002-7-10
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
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
	
	// ÀÌ À¯Àú¸¦ °ÔÀÓ¾È¿¡¼­ Á¦°Å ÇÑ´Ù 
	pSOPlayer->removeSelfInGame();
	
	// Æ÷¸¸°¨,À¯ÀúÀÇ ¿ëº´µéÁ¤º¸,ÇöÀç À§Ä¡,µ·À» DB¿¡ ÀúÀåÇÑ´Ù 	
	pSOPlayer->saveSatietyToDB();
	pSOPlayer->saveAllFollowerDataToDB();
	pSOPlayer->saveLogoutInfoToDB();
	pSOPlayer->saveLogoutStartToDB();
	
	//°ø¼º½Ã°£¿¡¸¸ ÀúÀåÀ» ÇÑ´Ù.
	if(pSOPlayer->IsFieldAttackUnit() == TRUE)
		pSOPlayer->saveLogountWarInfoToDB();
	
	// À¯Àú Á¤º¸ ÃÊ±âÈ­ 
	pSOPlayer->InitEnv();
	pSOPlayer->uiAccount = pXPlayer->m_usAccount;

	// À¯Àú¸¦ ·Î±× ¾Æ¿ô »óÅÂ·Î ¸¸µç´Ù 
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
		// SMS °ü·Ã
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

