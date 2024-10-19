#include <stdio.h>
#include <conio.h>

#include "..\\XCommon\\JXConnectSocket.h"	
#include "..\\XCommon\\JXGateSocket.h"	
#include "..\\XCommon\\JXSocketMgr.h"	
#include "..\\XCommon\\JXSocket.h"		
#include "..\\XCommon\\JXPacket.h"		
#include "..\\XCommon\\JXPacketQueue.h"	
#include "..\\XCommon\\JXAcceptSocketList.h"
#include "..\\XCommon\\XMonitorCmd.h"

#include "..\\ServerMonitor\\MonitorProtocol.h"
#include "..\\ServerMonitor\\SMSProtocol.h"

#include "MasterServer.h"

#include "XClientMgr.h"
#include "XServerMgr.h"

#include "XClient.h"
#include "XServer.h"

#include "CDBMgr.h"

MasterServer::MasterServer() : 
CJOGersangConfig()
{

	m_tickServer = 0;

	
	//Accept 해줌
	m_pGateSocketClient = new JXGateSocket;
	
	//Accept 리스트 가지고 있음
	m_pAcceptListClient = new JXAcceptSocketList;

	//Accept 리스트를 옮겨 놓을 리스트(여기는 각 유저의 소켓 정보만 가지고 있음) 그래서 connection 리스트라고 하는 거징..
	m_pConnectListClient = new JXSocketMgr;
	m_pClientMgr = new XClientMgr;		//유저는 일반적으로 소켓 정보와 유저 정보들을 가지고 있는데, connection 리스트에서 소켓 정보를 가지고 있고, 여기서는 유저 정보를 가지고 있음 이렇게 하는 이유는 라이브러리화가 주된 생각이지 싶음


	//위에서 Accept 리스트를 가지고 있고, Accpet 리스트를 옮기는 이유는 blocking 을 좀 더 줄이기 위해서다.
	//일반적으로 바로 connection 리스트에 넣지만, 그렇게 하면 Accept 할때나, disconnect 할때 blocking이 걸리므로, 그 횟수를 줄이기 위해서다.
	//리스트를 가지고 있다가 리스트를 그대로 넣으면 한번에 많은 blocking 을 줄일 수 있으므로..

	m_pConnectServerSocket = new JXConnectSocket;		//각 Monitoring 서버에 접속하기 위해서..
	
	m_pServerMgr = new XServerMgr;		//Monitoring 서버 객체의 속성들을 가지고 있음

	m_pClientToRemove = NULL;
	m_siNumClientToRemove = 0;

	m_pConnectServerSocket = NULL;

	m_usernum = 0;
	
	m_dwRefreshDB = 0;
}

MasterServer::~MasterServer()
{
	
	if( m_pConnectServerSocket ) 
	{
		delete [] m_pConnectServerSocket;
	}

	delete m_pGateSocketClient;
	delete m_pAcceptListClient;
	delete m_pConnectListClient;
	delete m_pClientMgr;

	delete m_pServerMgr;

//	m_pSMSServer->StopThread();
//	delete m_pSMSServer;

}

BOOL MasterServer::Init()
{

	// Input..

	SI32 siMaxClient = 30;
	SI32 siMaxServer = 100;


	if( m_pClientToRemove ) 
	{
		delete [] m_pClientToRemove;
	}

	m_pClientToRemove = new XClient*[ siMaxClient ];
	m_siNumClientToRemove = 0;

	// 플레이어 관련 초기화 
	m_pAcceptListClient->Create( siMaxClient, 30000, 30000, 30000 );
	m_pGateSocketClient->Create( MASTER_SERVER_PORT, m_pAcceptListClient );
	m_pClientMgr->Create( siMaxClient );

	// 서버 관련 초기화
	m_pServerMgr->Create( siMaxServer );

	if( m_pConnectServerSocket ) delete [] m_pConnectServerSocket;

	m_pConnectServerSocket = new JXConnectSocket[ siMaxServer ];


	// ODBC를 초기화하고 DB에 접속한다.[1/18/2005]
	if(CDBMgr::GetInstance()->Init())
		CDBMgr::GetInstance()->ConnectDB();
	
	return TRUE;
}

void MasterServer::Start()
{

	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf(" 거상 온라인 모니터링 마스터 서버 Ver 0.1\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");

	
	BOOL bRetVal = FALSE;

#ifdef __CONFIG_FILE_LOAD__
	bRetVal = LoadConfigFile();

	if( bRetVal == FALSE )
	{
		printf( "Gersang.ini file error!\n" );
		exit( -2 );
	}
#else
	bRetVal = LoadFile();

	if( bRetVal == FALSE )
	{
		printf( "serverlist.txt file error!\n" );
		exit( -2 );
	}
#endif

	ReadUserDB();

	// 클라이언트
	m_pGateSocketClient->StartThread();
	m_pConnectListClient->StartThread();

	//
	int i, j, k = 0;
	for( i = 0; i < m_siServerGroupNum; ++i ) {
		for( j = 0; j < m_siServerNum; ++j ) {

			printf( "connect.. %s %s [%s]\n", m_szServerGroupName[ i ], m_szServerName[ i ],m_szServerIP[ i ][ j ] );

			m_pConnectServerSocket[ k ].StartThread();
			m_pConnectServerSocket[ k ].Create( m_szServerIP[ i ][ j ], MONITOR_SERVER_PORT, 30000, 30000, 30000 );
			m_pConnectServerSocket[ k ].TryConnect();

			m_pServerMgr->AddServer( &m_pConnectServerSocket[ k ] )->m_servercode = MAKESERVERCODE( i, j );

			k++;
		}
	}


	///////////////////////////////////////////////////////////
	// SMS Server 관련
//	m_pSMSServer = new CSMSServer(m_pServerMgr);

//	m_pSMSServer->Create();
//	m_pSMSServer->Start();
//	m_pSMSServer->StartThread();
	///////////////////////////////////////////////////////////

	m_bServerRun = TRUE;
	
}

void MasterServer::Run()
{
	int k;


	UI32			i;
	SI32			j;
	JXSocket		*pSocket;
	
	XClient			*pClient;
	XServer			*pServer;
	
	JXPacketQueue	*pPacketQueue = NULL;

	while( m_bServerRun ) {

		Sleep(10);

		// 접속을 받아들인 신규 소켓 리스트를 본 리스트로 옮겨온다

		// (플레이어)
		m_pAcceptListClient->Wait( INFINITE );
		m_pConnectListClient->Wait( INFINITE );

		BEGIN_JXLIST_LOOP( i, m_pAcceptListClient, JXSocket, pSocket ) 

			printf("AcceptList -> ConnectList : Socket Addr[ %x ]\n", pSocket );
			
			m_pClientMgr->AddClient( pSocket );
			
			FD_SET( pSocket->m_socket, &m_pConnectListClient->m_fdset );
		
		END_JXLIST_LOOP

		m_pConnectListClient->PushFront( m_pAcceptListClient );


		// 접속 끊기

		for( j = 0; j < m_siNumClientToRemove; ++j ) {
			pClient = m_pClientToRemove[ j ];
			
			if( pClient->IsValid() == FALSE ) continue;

			// 모니터링 서버로 diconnect 보내기 
			local_usCmd = MMC_CLIENT_DISCONNECT;
			local_packet.ClearPacket();
			local_packet.WriteValue( &local_usCmd, 2 );
			local_packet.WriteValue( &pClient->m_uiID, 2 );
			local_packet.CompletePacket();

			m_pServerMgr->SendPacket( &local_packet );
			///////////////////////////////////

			if( pClient->m_bLoginOK == TRUE ) {
				local_usCmd = MMC_REPORT_USER_EXIT;
				local_packet.ClearPacket();
				local_packet.WriteValue( &local_usCmd, 2 );
				local_packet.WriteValue( &pClient->m_user.id, 32 );
				local_packet.WriteValue( &pClient->m_user.name, 32 );
				local_packet.CompletePacket();

				m_pClientMgr->SendPacket( &local_packet );			
			}

			FD_CLR( pClient->GetSocket()->m_socket, &m_pConnectListClient->m_fdset );
			
			pClient->GetSocket()->Disconnect();
			
			m_pConnectListClient->Remove( (JXObject *)(pClient->GetSocket()) );
			
			m_pClientMgr->DelClient( pClient );

		}
		m_siNumClientToRemove = 0;
		
		m_pConnectListClient->Release();
		m_pAcceptListClient->Release();



		// 서버 접속 여부
		if( GetTickCount() - m_tickServer > 3000 ) {
			k = 0;
			for( i = 0; i < m_siServerGroupNum; ++i ) {
				for( j = 0; j < m_siServerNum; ++j ) {
					if( m_pConnectServerSocket[ k ].m_bConnect == FALSE )
					{
						printf( "%s %s is Disconnected! Connecting..\n", m_szServerGroupName[ i ], m_szServerName[ j ] );
						m_pConnectServerSocket[ k ].TryConnect();

						CDBMgr::GetInstance()->SendQuary(FALSE, i); //서버상태를 DB에 쿼리한다.
					}
					k++;
				}
			}
			m_tickServer = GetTickCount();
		}

        //db에 서버상태를 전송한다.
		if( GetTickCount() - m_dwRefreshDB > 1000*60)
		{
			k = 0;
			for( i = 0; i < m_siServerGroupNum; ++i ) 
			{
				for( j = 0; j < m_siServerNum; ++j ) 
				{
					if( m_pConnectServerSocket[ k ].m_bConnect == TRUE )
						CDBMgr::GetInstance()->SendQuary(TRUE, i); //서버상태를 DB에 쿼리한다.
					
					k++;
				}
			}
			m_dwRefreshDB = GetTickCount();
		}

		// 클라이언트 프로세스
		BEGIN_JXLIST_LOOP( i, m_pClientMgr, XClient, pClient ) 

			ProcPacketClient( pClient );

		END_JXLIST_LOOP

		// 서버 프로세스
		BEGIN_JXLIST_LOOP( i, m_pServerMgr, XServer, pServer ) 

			ProcPacketServer( pServer );

		END_JXLIST_LOOP

		// 콘솔처리
		ProcessConsole();

	}

}

void MasterServer::Stop()
{

	m_bServerRun = FALSE;

	printf( "서버를 정지합니다\n" );

}

void MasterServer::ProcessConsole()
{
	char ch;

	if( kbhit() ) {

		ch = getch();

		switch( ch ) {
		case 27:	// ESC
			Stop();
			break;

		case '1':
			break;

		case '2':
			break;

		}

	}

}


// 클라이언트 패킷 처리..
void MasterServer::ProcPacketClient( XClient *pClient )
{


	int k;
	BOOL bConnect;
	JXPacketQueue *pPacketQueue = NULL;

	char id[ 32 ];
	char pass[ 32 ];
	char name[ 32 ];
	char result;

    // 접속 여부 알아오기
	pClient->GetSocket()->IsConnect( &bConnect );

	// 접속이 종료된 상태이면, 로그아웃 처리를 한다
	if( bConnect == FALSE ) {
		LogOutClient( pClient );
		return;
	}

	// 큐 포인터 얻어옴
	pPacketQueue = pClient->GetSocket()->GetQueue();

    // 큐에 있는 패킷 개수 얻어옴
	UI32 uiNumPacket = pPacketQueue->GetNumPacket();

    // 패킷 개수 만큼 반복
	for( UI32 i = 0; i < uiNumPacket; ++i ) {

		// 패킷 얻어옴
		if( pPacketQueue->Dequeue( &local_packet ) == FALSE ) return;

		// 패킷에서 커맨드 읽음
		local_packet.ReadValue( &local_usCmd, sizeof( local_usCmd ) );


		// 로그인 되지 않은 경우
		if( pClient->m_bLoginOK == FALSE ) {

			if( local_usCmd == MMC_QUERY_LOGIN ) {

				local_packet.ReadValue( id, 32 );
				local_packet.ReadValue( pass, 32 );

				printf("Login id:[%s][%s]\n", id, pass );

				BOOL bValidID = FALSE;
				BOOL bPassOK = FALSE;
				BOOL bConnOK = TRUE;

				// 아이디 및 패스워드 체크
				for( i = 0; i < m_usernum; ++i ) {

					if( _stricmp( id, m_user[ i ].id ) == 0 ) {
						bValidID = TRUE;
					
						if( strcmp( pass, m_user[ i ].password ) == 0 ) {
							bPassOK = TRUE;
							break;
						}
					}
				}

				UI32			ii;
				XClient			*pTClient;

				// 이미 접속해 있는지 여부
				BEGIN_JXLIST_LOOP( ii, m_pClientMgr, XClient, pTClient ) 
			
						if( _stricmp( pTClient->m_user.id, id ) == 0 ) bConnOK = FALSE;
	
				END_JXLIST_LOOP


				if( bValidID == FALSE ) result = 1;
				else if( bPassOK == FALSE ) result = 2;
				else if( bConnOK == FALSE ) result = 3;
				else {

					result = 0;

					pClient->m_bLoginOK = TRUE;
					strcpy( pClient->m_user.id,  m_user[ i ].id );
					strcpy( pClient->m_user.password,  m_user[ i ].password );
					strcpy( pClient->m_user.name,  m_user[ i ].name );
					pClient->m_user.level = m_user[ i ].level;


					// 모니터링 서버로 connect 보내기 
					local_usCmd = MMC_CLIENT_CONNECT;
					local_packet.ClearPacket();
					local_packet.WriteValue( &local_usCmd, 2 );
					local_packet.WriteValue( &pClient->m_uiID, 2 );
					local_packet.CompletePacket();

					m_pServerMgr->SendPacket( &local_packet );
					///////////////////////////////////

					local_usCmd = MMC_REPORT_USER_ENTER;
					local_packet.ClearPacket();
					local_packet.WriteValue( &local_usCmd, 2 );
					local_packet.WriteValue( &pClient->m_user.id, 32 );
					local_packet.WriteValue( &pClient->m_user.name, 32 );
					local_packet.CompletePacket();

					m_pClientMgr->SendPacket( &local_packet );
				}
				
				if( result != 0 ) LogOutClient( pClient );

				printf( "result: %d\n", result );

				local_usCmd = MMC_REPORT_LOGIN;

				local_packet.ClearPacket();
				local_packet.WriteValue( &local_usCmd, 2 );
				local_packet.WriteValue( &result, 1 );
				local_packet.CompletePacket();

				pClient->GetSocket()->SendPacket( &local_packet );

			} else continue;
		}


		// 패킷 처리..
		switch( local_usCmd ) {
		case MMC_QUERY_SERVER_STRUCT:							// 월드의 서버 정보 요청에 대한 응답 
			local_usCmd = MMC_REPORT_SERVER_STRUCT;
			local_packet.ClearPacket();
			local_packet.WriteValue( &local_usCmd, 2 );
			local_packet.WriteValue( &m_siServerNum, 4 );
			for( k = 0; k < m_siServerNum; ++k ) {
				local_packet.WriteValue( m_szServerName[ k ], 32 );
			}
			local_packet.CompletePacket();

			pClient->GetSocket()->SendPacket( &local_packet );

			break;

		case MMC_QUERY_WORLD_LIST:								// 월드 정보 요청에 대한 응답
			local_usCmd = MMC_REPORT_WORLD_LIST;
			local_packet.ClearPacket();
			local_packet.WriteValue( &local_usCmd, 2 );
			local_packet.WriteValue( &m_siServerGroupNum, 4 );
			for( k = 0; k < m_siServerGroupNum; ++k ) {
				local_packet.WriteValue( m_szServerGroupName[ k ], 32 );
			}
			local_packet.CompletePacket();

			pClient->GetSocket()->SendPacket( &local_packet );

			break;

		case MMC_QUERY_USERLIST:								// 등록된 사용자 리스트 
			local_usCmd = MMC_REPORT_USERLIST;
			
			local_packet.ClearPacket();
			local_packet.WriteValue( &local_usCmd, 2 );
			local_packet.WriteValue( &m_usernum, 4 );

			for( k = 0; k < m_usernum; ++k ) {
				local_packet.WriteValue( m_user[ k ].id, 32 );
				local_packet.WriteValue( m_user[ k ].name, 32 );
			}

			local_packet.CompletePacket();

			pClient->GetSocket()->SendPacket( &local_packet );

			break;

		case MMC_QUERY_CONNECTUSERLIST:							// 접속중인 사용자 리스트
			{
				UI32			i;
				XClient			*pTClient;
			
				local_usCmd = MMC_REPORT_CONNECTUSERLIST;
				SI32	n = m_pClientMgr->GetNumberNodes();

				local_packet.ClearPacket();
				local_packet.WriteValue( &local_usCmd, 2 );
				local_packet.WriteValue( &n, 4 );

				BEGIN_JXLIST_LOOP( i, m_pClientMgr, XClient, pTClient ) 
			
					local_packet.WriteValue( pTClient->m_user.id, 32 );
					local_packet.WriteValue( pTClient->m_user.name, 32 );
	
				END_JXLIST_LOOP

				local_packet.CompletePacket();

				pClient->GetSocket()->SendPacket( &local_packet );
			}
			break;

		case MMC_QUERY_ADDUSER:									// 사용자 추가
			local_packet.ReadValue( id, 32 );
			local_packet.ReadValue( pass, 32 );
			local_packet.ReadValue( name, 32 );

			result = 0;
			for( k = 0; k < m_usernum; ++k ) {
				if( _stricmp( m_user[ k ].id, id ) == 0 ) {
					result = 1;
					break;
				}
			}

			if( result == 0 ) {
				strcpy( m_user[ m_usernum ].id, id );
				strcpy( m_user[ m_usernum ].password, pass );
				strcpy( m_user[ m_usernum ].name, name );
				m_usernum++;

				WriteUserDB();
			}

			local_usCmd = MMC_REPORT_ADDUSER;

			local_packet.ClearPacket();
			local_packet.WriteValue( &local_usCmd, 2 );
			local_packet.WriteValue( &result, 1 );
			local_packet.CompletePacket();

			pClient->GetSocket()->SendPacket( &local_packet );

			break;
 
		case MMC_QUERY_DELUSER:									// 사용자 삭제
			local_packet.ReadValue( id, 32 );
			break;


		case MMC_QUERY_CHAT:
			{
				char buf[ 1024 ];
				local_packet.ReadString( buf, 1024 );

				local_usCmd = MMC_REPORT_CHAT;
				local_packet.ClearPacket();
				local_packet.WriteValue( &local_usCmd, 2 );
				local_packet.WriteValue( pClient->m_user.id, 32 );
				local_packet.WriteValue( pClient->m_user.name, 32 );
				local_packet.WriteString( buf, 1024 );
				local_packet.CompletePacket();

				m_pClientMgr->SendPacket( &local_packet );
			}
			break;
		case MMC_QUERY_SMS:
			{
				char szMsg[80];

				local_packet.ReadValue(&szMsg,80);
							
				SMS_REQ_MSG sSMS;
				memset(&sSMS, 0, sizeof(SMS_REQ_MSG));

				//m_LocalPacket.ReadValue(&sSMS, sizeof(SMS_REQ_MSG));
				//sSMS.dwCallCount = ntohs(sSMS.dwCallCount);

				strncpy(sSMS.szCallMessage,szMsg,79);
				strcpy(sSMS.szOrgPhone,"00000000000");

				UI16 cmd = SS_SMS_REQ_MSG;
				local_packetB.ClearPacket();
				local_packetB.WriteValue( &cmd, 2 );
				local_packetB.WriteValue( &sSMS, sizeof(SMS_REQ_MSG));					
				local_packetB.CompletePacket();

				m_pServerMgr->SendPacket(&local_packetB);

			}
			break;
		case MMC_CLIENT_TO_SERVER:
			{
				UI16 sn;
				UI16 sc[ 1000 ];
				UI16 size;

				local_packet.ReadValue( &sn, 2 );

				for( int i = 0; i < sn; ++i ) {
					local_packet.ReadValue( &sc[ i ], 2 );
				}

				local_packet.ReadValue( &size, 2 );
				local_packet.ReadValue( local_buf, size );

				UI16 cmd = MMC_MASTER_TO_MONITOR;
				local_packetB.ClearPacket();
				local_packetB.WriteValue( &cmd, 2 );
				local_packetB.WriteValue( &pClient->m_uiID, 2 );
				local_packetB.WriteValue( &size, 2 );
				local_packetB.WriteValue( local_buf, size );
				local_packetB.CompletePacket();

				for( i = 0; i < sn; ++i )
				{
					m_pServerMgr->SendPacket( sc[ i ], &local_packetB );
				}
			}
			break;
		}
			
	}

}

// 서버 패킷 처리..
void MasterServer::ProcPacketServer( XServer *pServer )
{

	BOOL bConnect;
	JXPacketQueue *pPacketQueue = NULL;

    // 접속 여부 알아오기
	pServer->GetSocket()->IsConnect( &bConnect );

	// 접속이 종료된 상태이면, 로그아웃 처리를 한다
	/*
	if( bConnect == FALSE ) {
		LogOutServer( pServer );
		return;
	}
	*/

	// 큐 포인터 얻어옴
	pPacketQueue = pServer->GetSocket()->GetQueue();

    // 큐에 있는 패킷 개수 얻어옴
	UI32 uiNumPacket = pPacketQueue->GetNumPacket();

    // 패킷 개수 만큼 반복
	for( UI32 i = 0; i < uiNumPacket; ++i ) {

		// 패킷 얻어옴
		if( pPacketQueue->Dequeue( &local_packet ) == FALSE ) return;

		//kurt9322@hotmail.com

		// 패킷에서 커맨드 읽음
		local_packet.ReadValue( &local_usCmd, sizeof( local_usCmd ) );

		// 패킷 처리..
		switch( local_usCmd ) {
		case MMC_MONITOR_TO_MASTER_CLIENT:
			{

				UI16 size;
				UI16 uiID;

				local_packet.ReadValue( &uiID, 2 );
				local_packet.ReadValue( &size, 2 );
				local_packet.ReadValue( local_buf, size );

				local_usCmd = MMC_SERVER_TO_CLIENT;
				local_packetB.ClearPacket();
				local_packetB.WriteValue( &local_usCmd, 2 );
				local_packetB.WriteValue( &pServer->m_servercode, 2 );
				local_packetB.WriteValue( local_buf, size );
				local_packetB.CompletePacket();

				m_pClientMgr->SendPacket( uiID, &local_packetB );

				printf("MMC_MONITOR_TO_MASTER_CLIENT : %d\n", uiID );
			}
			break;
		case MMC_MONITOR_TO_MASTER_ALLCLIENT:
			{
				UI16 size;
				

				local_packet.ReadValue( &size, 2 );
				local_packet.ReadValue( local_buf, size );

				local_usCmd = MMC_SERVER_TO_CLIENT;
				local_packetB.ClearPacket();
				local_packetB.WriteValue( &local_usCmd, 2 );
				local_packetB.WriteValue( &pServer->m_servercode, 2 );
				local_packetB.WriteValue( local_buf, size );
				local_packetB.CompletePacket();

				WORD wTemp = 0;
				memcpy(&wTemp, &local_buf[2], sizeof(WORD));
				if(wTemp == 5)
				{
					int i = pServer->m_servercode / 10;
					CDBMgr::GetInstance()->SendQuary(FALSE, i);
				}

				m_pClientMgr->SendPacket( &local_packetB );
			}
			break;
		///////////////////////////////////////////////////////////
		// SMS 관련
//		case SS_SMS_RESP_MSG : 
//			{
//				SMS_RESP_MSG sSMS;
//				memset(&sSMS, 0, sizeof(SMS_RESP_MSG));

//				local_packet.ReadValue(&sSMS, sizeof(SMS_RESP_MSG));
//				m_pSMSServer->SendSMSMsgResult(&sSMS);
//			}
//			break;
		///////////////////////////////////////////////////////////
		}

	}

}

void MasterServer::LogOutClient( XClient *pClient )
{
	m_pClientToRemove[ m_siNumClientToRemove++ ] = pClient;
}

void MasterServer::LogOutServer( XServer *pServer )
{
	m_pServerToRemove[ m_siNumServerToRemove++ ] = pServer;
} 


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=
//	Name : 이민수
//  Date : 2004.01.16 PM
//
//	Desc : 서버 정보를 Config에서 읽는다.
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=
BOOL MasterServer::LoadConfigFile()
{
	m_siServerGroupNum = 1;
	m_siServerNum = 1;

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	SetConfigFileName(GERSANG_SERVER_CONFIG_FILE);
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	int nGroupCount = GetMasterGroupCount();
	if	(nGroupCount < 1)	return FALSE;

	m_siServerGroupNum = nGroupCount;

	for(int n = 0; n < nGroupCount ; n++)
	{
		strcpy(m_szServerType[n], GetMasterServerType(n));
		strcpy(m_szServerName[n], GetMasterServerGroupName(n));
		strcpy(m_szServerGroupName[n], GetMasterServerName(n));

		int nServerCount = GetMasterServerCount(n);
		if	(nServerCount < 1)	return FALSE;		

		for(int i = 0 ; i < nServerCount ; i++)
		{			
			strcpy(m_szServerIP[n][i], GetMasterServerIP(n, i));
		}

		m_siServerNum = m_siServerNum < nServerCount ? m_siServerNum : nServerCount;
	}

	return TRUE;
}

BOOL MasterServer::LoadFile()
{

	m_siServerGroupNum = 0;
	m_siServerNum = 0;

	int		tagcode;
	int		i;

	HANDLE	hFile = NULL;
	BOOL	bResult;
	DWORD	dwNumberOfBytesRead;
	char	ch;

	BOOL	bString;

	char	buf[ 2 ][ 256 ];

	buf[ 0 ][ 0 ] = NULL;
	buf[ 1 ][ 0 ] = NULL;
	
	bString = FALSE;

	int		currBufCode = 0;
	int		currBufPos = 0;

	int		state	=	0;			// 0: 구분자 상태		1: 의미 문장	2: 줄바꿈

	hFile = CreateFile( "serverlist.txt", GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	if( hFile == INVALID_HANDLE_VALUE ) return FALSE;

	while( 1 ) {

		bResult = ReadFile( hFile, &ch, 1, &dwNumberOfBytesRead, NULL );

		if( bResult == FALSE || dwNumberOfBytesRead == 0 ) break;

		if( (ch == 9 || ch == ' ' || ch == '=') && !bString )  {
			if( state == 1 ) {
				state = 0;

				buf[ currBufCode ][ currBufPos ] = NULL;

				currBufCode++;
				currBufPos = 0;
			
			}
		} else 
		if( ch == 13 || ch == 10 ) {
			if( state == 2 ) continue;

			if( state == 1 ) {
				buf[ currBufCode ][ currBufPos ] = NULL;				
			}

			/////////////////

			if( strcmp( buf[ 0 ], "[server group]" ) == 0 ) {
				printf("read server group..\n");
				tagcode = 0;
				
			} else if( strcmp( buf[ 0 ], "[server info]" ) == 0 ) {
				printf("read server info..\n");
				if( tagcode == 0 ) {
					tagcode = 1;
				}
				
				m_siServerGroupNum++;

			} else if( tagcode == 0 ) {
		
				strcpy( m_szServerType[ m_siServerNum ], buf[ 0 ] );
				strcpy( m_szServerName[ m_siServerNum ], buf[ 1 ] );

				printf( "%s, %s\n", buf[ 0 ], buf[ 1 ] );

				m_siServerNum++;

			} else if( tagcode == 1 ) {
				
				if( strcmp( buf[ 0 ], "server_name" ) == 0 ) {
					strcpy( m_szServerGroupName[ m_siServerGroupNum - 1 ], buf[ 1 ] );
				} else {

					for( i = 0; i < m_siServerNum; ++i ) {

						if( strcmp( buf[ 0 ], m_szServerType[ i ] ) == 0 ) {
							strcpy( m_szServerIP[ m_siServerGroupNum - 1 ][ i ], buf[ 1 ] );
							break;
						}

					}
				}

			}

			//////////////////

			state = 2;

			currBufCode = 0;
			currBufPos = 0;

			buf[ 0 ][ 0 ] = NULL;
			buf[ 1 ][ 0 ] = NULL;

			bString = FALSE;

		} else {

			if( currBufCode < 2 ) {
				buf[ currBufCode ][ currBufPos++ ] = ch;
			}

			if( ch == '[' || ch == '\"' ) bString = TRUE;

			state = 1;
		}
	}

	CloseHandle( hFile );

	return TRUE;
}

void MasterServer::ReadUserDB()
{

	HANDLE	hFile = NULL;
	BOOL	bResult;
	DWORD	dwNumberOfBytesRead;

	XUser	user;

	strcpy( m_user[ 0 ].id, "administrator" );
	strcpy( m_user[ 0 ].password, "duddnjsgltkfkdgo" );
	strcpy( m_user[ 0 ].name, "Administrator" );
	m_user[ 0 ].level = 1;

	m_usernum = 1;

	hFile = CreateFile( "userdb", GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	if( hFile == INVALID_HANDLE_VALUE ) return;	

	while( 1 ) {

		bResult = ReadFile( hFile, user.id, 32, &dwNumberOfBytesRead, NULL );
		if( bResult == FALSE || dwNumberOfBytesRead != 32 ) break;

		bResult = ReadFile( hFile, user.password, 32, &dwNumberOfBytesRead, NULL );
		if( bResult == FALSE || dwNumberOfBytesRead != 32 ) break;

		bResult = ReadFile( hFile, user.name, 32, &dwNumberOfBytesRead, NULL );
		if( bResult == FALSE || dwNumberOfBytesRead != 32 ) break;

		bResult = ReadFile( hFile, &user.level, 1, &dwNumberOfBytesRead, NULL );
		if( bResult == FALSE || dwNumberOfBytesRead != 1 ) break;

		m_user[ m_usernum++ ] = user;
	}

	CloseHandle( hFile );
}

void MasterServer::WriteUserDB()
{

	HANDLE	hFile = NULL;
	BOOL	bResult;
	DWORD	dwNumberOfBytesWritten;

	hFile = CreateFile( "userdb", GENERIC_WRITE, FILE_SHARE_READ, NULL, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	if( hFile == INVALID_HANDLE_VALUE ) return;

	for( int i = 1; i < m_usernum; ++i ) {

		bResult = WriteFile( hFile, m_user[ i ].id, 32, &dwNumberOfBytesWritten, NULL );
		if( bResult == FALSE || dwNumberOfBytesWritten != 32 ) break;

		bResult = WriteFile( hFile, m_user[ i ].password, 32, &dwNumberOfBytesWritten, NULL );
		if( bResult == FALSE || dwNumberOfBytesWritten != 32 ) break;

		bResult = WriteFile( hFile, m_user[ i ].name, 32, &dwNumberOfBytesWritten, NULL );
		if( bResult == FALSE || dwNumberOfBytesWritten != 32 ) break;

		bResult = WriteFile( hFile, &m_user[ i ].level, 1, &dwNumberOfBytesWritten, NULL );
		if( bResult == FALSE || dwNumberOfBytesWritten != 1 ) break;

	}

	CloseHandle( hFile );

}


void MasterServer::GetIP(SOCKET s, char *szIP)
{
	if(s == INVALID_SOCKET) szIP = NULL;

	int nLen = sizeof(sockaddr_in); 
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockaddr_in));
	
	getpeername(s, (SOCKADDR*)&sockAddr, &nLen); //IP를 얻어온다.
	
	memcpy(szIP, &sockAddr.sin_addr.s_addr, 16);
}

int MasterServer::FindServerIndex(SOCKET s)
{
	if(s == INVALID_SOCKET) return -1;

	int i, j;
	char IP[16] = {0,};
	GetIP(s, IP);//소켓 번호로 아이피를 얻어온다.
	
	if(strlen(IP) > 0)
	{
		for( i = 0; i < m_siServerGroupNum; ++i )
		{
			for( j = 0; j < m_siServerNum; ++j ) 
			{
				if(strcmp(m_szServerIP[i][j], IP) == 0)
				{
					break;
				}
			}
		}
	}

//	i = wServerCode / 10;
//	j = wServerCode % 10;
	
	return i;
}