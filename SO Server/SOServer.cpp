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





//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// SOServer()�� ������. ��� �������� �ʱ�ȭ�ϰ� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOServer::SOServer(VOID)
{	
	// 1. DB �ʱ�ȭ 
	//---------------------------------------------------------------------------------------------------------
	dbinit();

	// 2. ���� �ʱ�ȭ 
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

	
	// 3. ���� ȭ���� ǥ���Ѵ�.
	//---------------------------------------------------------------------------------------------------------
	OpenConsole();
	
	
	// 4. ����ġ ȭ���� �ҷ��´� 
	//---------------------------------------------------------------------------------------------------------
	////LoadExpData();


	// 5. ����Ÿ ���̽� ���� 
	//---------------------------------------------------------------------------------------------------------
	printf("---Database Init...---\r");	
	
	clDatabase		= new SODatabase();

	clDatabase->createQueue();
	clDatabase->ConnectDatabase();	
	
	clLoginDatabase = new SODatabase();	

	clLoginDatabase->ConnectDatabase();
		
	clItemDatabase = new SODatabase();
	
	clItemDatabase->ConnectDatabase2();

	// DB���� ť ó�� �����带 ���� 
	clDatabase->createQueueThread();



	printf("---\tDatabase Completed\t---\n");
	

	// 6. SOWorld�� SOGate Ŭ���� ���� (��� ����� ����  ó��)
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
	// JW Server Engine �ʱ�ȭ


	// �÷��̾� ����Ʈ ���� �� �ʱ�ȭ
	try{	
		m_pXPlayerMgr = new XPlayerMgr;
	}
	catch(std::bad_alloc){
		assert(0);
	}
	m_pXPlayerMgr->Create( ON_MAX_CONNECTION );


	// ��ü ���� 
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

	// Server��ü �ʱ�ȭ( �� ���Ӽ���, ���� �� ������ ���� ũ��, �ޱ� ���� ũ�� )
	m_pIOCPServer->InitServer( ON_MAX_CONNECTION, 60000, 60000 );

	// Listen��ü �ʱ�ȭ( Accept�Լ��� ȣ���ϱ����� Server��ü ������, ��Ʈ��ȣ )
	m_pListen->Init( m_pIOCPServer, 8000 );

	//-----------------------------------------------------------------------------------

	
	// ���ο� XPlayer�� World�� �ִ� SOPlayer�� ����..  
	JWObject *itor = m_pXPlayerMgr->m_pFreeFirst->GetNext();		
	UI32	 i;
	for( i = 0; i < m_pXPlayerMgr->m_uiFreeNumber; ++i ) {

		((XPlayer *)(itor))->m_usAccount = i + 1;
		((XPlayer *)(itor))->m_pSOPlayer = (SOPlayer *)clWorld->clPlayer[ i ];
		((XPlayer *)(itor))->m_pServer = this;
		clWorld->clPlayer[ i ]->m_pXPlayer = (XPlayer *)itor;

		itor = itor->GetNext();
	}


	// ���� ��Ʈ�� ���� ������ ��ٸ���
	m_pListen->Start();

	// ����͸� �������� ���	
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


    // 8. �������� �ʱ�ȭ 
	//---------------------------------------------------------------------------------------------------------
	Init();


	


	//====================================================
	// AUTHENTICATION: ���������� ���� ������ ��û �Ѵ� 
	//====================================================
	
	
	
	CAuthServerProc AuthServer(0); // 0�� ��û�� ���Ѵ�⸦ �ǹ� �Ѵ� 
	

	char responseKind;
	AuthServer.RequestReset( g_GameServerInfoParser.GetWorldID(), 
							 &responseKind );

	
	

	// ���� ���� �ð� ��� 
//	WriteInfo("..\\ConnectStatus.txt","Server Started");		
	writeInfoToFile("ConnectStatus.txt","Server Started");		
	return;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// SOServer�� �Ҹ���
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

	// ���� ���� 
	WSACleanup();
	
	// DB ���� 
	dbexit();

	// ���� ȭ���� ����Ѵ�.
	CloseConsole();

	return;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// �ʱ�ȭ �Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID SOServer::Init()
{
	iLoopTime		 = 0;	
	iLoopCount		 = 0;

	bShutDownOk		 = FALSE;		// �˴ٿ� ���°� �ƴϴ�.		
	bConsoleLock	 = TRUE;		// Ű���� �� ����

	dwCurrentFPS	 = 0;
	dwMaxCurrentFPS	 = 0;
	dwMinCurrentFPS	 = ULONG_MAX;

	dwWorldCurrentFPS		= 0;
	dwWorldMaxCurrentFPS	= 0;
	dwWorldMinCurrentFPS	= ULONG_MAX;

	dwSmsLastTime			= timeGetTime();
}




//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// ���� ȭ���� ����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID SOServer::OpenConsole(VOID)
{
	printf("----------------------------------------------------------------------------\n");
	printf("                                          Online Gersang Game Server Ver 0.1\n");
	printf("----------------------------------------------------------------------------\n");
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// ���� ȭ���� ����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID SOServer::CloseConsole(VOID)
{
	printf("----------------------------------------------------------------------------\n");
	printf(" Server Shut down...........................................................\n");
	printf("----------------------------------------------------------------------------\n");
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// SOServer�� ���� �Լ�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32 SOServer::Poll()
{
	
	SI32	siReturnCode			= 2; // ��� ������ ������ �����Ѵ�.
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


		// Loop Timer ����, ���� �ð� ��� 			
		StartMilliTimer( iLoopS, iLoopMS );   		
		uiCurrentClock = timeGetTime();	 		

		Sleep( 10 );

		// ���� ���� ���� ��� ������ �����ϰ� ������				
		if(bShutDownOk)
		{			
			siReturnCode = 0;
			break;
		}
				

		// 30�ʿ� �ѹ��� ���� �ִ� �������� �����Ѵ� 				
		if( (uiCurrentClock - dwLatestNumUserClock) > 30000 )
		{
			dwLatestNumUserClock = uiCurrentClock;			
			clDatabase->writeDBQuery( SO_DB_INFOKIND_CONCURRENTUSER, &clGate->siTotalConnectPlayer, 4 );		
			
			g_GameServerInfoParser.SetConcurrent( clGate->siTotalConnectPlayer );
		}


		// 5 �ʿ� �ѹ���  ������ ������ �����Ѵ� 				
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



		//����������������������������������������������������������������������������
		// �÷��̾� ��Ŷ ���ν���..
		//����������������������������������������������������������������������������


		for( i = 0; i < ON_MAX_CONNECTION; ++i ) {
			ProcPacketPlayer( clWorld->clPlayer[ i ]->m_pXPlayer );
		}


		//����������������������������������������������������������������������������
		// ������ ��� ����� Ű���� �Է� ó�� 	
		uiWorldCurrentClock		=	timeGetTime();
		
		siReturnCode = clWorld->Poll();			

		ProcPacketMonitor( m_pMonitor );

		siReturnCode = ProcessConsole();
		
		// World �������� ���Ѵ�			
		dwWorldCurrentFPS	=	1000 / max(1, GETTIMEGAP(uiWorldCurrentClock, timeGetTime()));
		if(dwWorldCurrentFPS > dwWorldMaxCurrentFPS)		dwWorldMaxCurrentFPS		=	dwWorldCurrentFPS;
		if(dwWorldCurrentFPS < dwWorldMinCurrentFPS)		dwWorldMinCurrentFPS		=	dwWorldCurrentFPS;


		// �����Կ� �ִ� ������ ��� 
		clDatabase->emptiedDepository();

		
		// ������ ������ ���� �ؾ� �ϴ°�� 			
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


		// �������� ���Ѵ�
		dwCurrentFPS	=	1000 / max(1, GETTIMEGAP(uiCurrentClock, timeGetTime()));

		if(dwCurrentFPS > dwMaxCurrentFPS)		dwMaxCurrentFPS		=	dwCurrentFPS;
		if(dwCurrentFPS < dwMinCurrentFPS)		dwMinCurrentFPS		=	dwCurrentFPS;

		// ������ �ѹ� ���µ� �ɸ��� �ð� ���� 				
		iCalcTime = CheckMilliTimer(iLoopS,iLoopMS);	// Loop �ð� ��� 
		iLoopTime+= iCalcTime;							

		// ������ �����Ѵ�.
		if(siReturnCode != 2) break;
	}

	return siReturnCode;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// Űó�� �Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32 SOServer::ProcessConsole(VOID)
{
	char	cChar;

	while(kbhit())
	{
		cChar=getch();

		if(!bConsoleLock)
		{
			switch(toupper(cChar))	// �빮�ڷ� �ٲ㼭 ó���Ѵ�.
			{
			// ������ �����Ѵ�.
			case '\x1b':
				g_bShutdown = TRUE;
				clWorld->LogOutAllPlayer();
				return 0;


			// ��� �÷��̾ �α׾ƿ� ��Ų��
			case '*':
				LogOutPlayerAll();
				break;


			// Ű���带 ��ٴ�.
			case 'L':
				printf( "Keyboard Locked\n" );
				bConsoleLock = TRUE;
				break;

			// ���� �������� ����ڼ��� ȭ�鿡 �����ش�.
			case 'P':				
				printf( "Total Connected User : Max : [%u] Cur : [%u]\n", clGate->siMaxConnectedPlayer, clGate->siTotalConnectPlayer );
				break;


			// ���� ������ ���� �����ش�.
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
					printf("������ �ð��� �����մϴ�.");
					clWorld->bChangeTime = true;
					printf("�ø� �Է��ϼ���\n");
					cChar = getch();
					clWorld->pChangeTime->tm_hour = atoi(&cChar);
					
					printf("���� �Է��ϼ���\n");
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

			// Server�� Loop ���¸� �˾ƺ���.
			case 'I':	
				printf("# Loop Rate:%03.5f\r",ServerLoad());
				break;			
			
			}
		}
	}

	return 2;
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// ������ ��� ������ ����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// ���̳ʸ� ���Ϸ� �α׸� �����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID SOServer::WriteBinary( char *FileName, const char *pContent, int iSize )
{
	FILE	*fp;

	if( (fp = fopen(FileName, "ab")) == NULL )		return;
	fwrite( pContent, sizeof(char), iSize, fp );
	fclose( fp );
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//  �÷��̾� ��Ŷ�� ó���Ѵ�
//	������ 2002-7-10
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
void SOServer::ProcPacketPlayer( XPlayer *pXPlayer )
{

	if( pXPlayer->IsOnAction() ) CompleteLogOut( pXPlayer );
	else pXPlayer->m_pSOPlayer->Poll();
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	���ο� �÷��̾� ���� ó��	
//	������ 2002-7-10
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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




//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	�÷��̾� �α� �ƿ� ( ��Ͽ��� �α� �ƿ� �� �÷��̾ ã�Ƽ� �α� �ƿ� ��Ų�� )
//	������ 2002-7-10
//������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	�÷��̾� �α� �ƿ� ( ������ �� �÷��̾��� �α׾ƿ� ó���� �����Ѵ� )
//	������ 2002-7-10
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
	
	// �� ������ ���Ӿȿ��� ���� �Ѵ� 
	pSOPlayer->removeSelfInGame();
	
	// ������,������ �뺴������,���� ��ġ,���� DB�� �����Ѵ� 	
	pSOPlayer->saveSatietyToDB();
	pSOPlayer->saveAllFollowerDataToDB();
	pSOPlayer->saveLogoutInfoToDB();
	pSOPlayer->saveLogoutStartToDB();
	
	//�����ð����� ������ �Ѵ�.
	if(pSOPlayer->IsFieldAttackUnit() == TRUE)
		pSOPlayer->saveLogountWarInfoToDB();
	
	// ���� ���� �ʱ�ȭ 
	pSOPlayer->InitEnv();
	pSOPlayer->uiAccount = pXPlayer->m_usAccount;

	// ������ �α� �ƿ� ���·� ����� 
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
		// SMS ����
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

