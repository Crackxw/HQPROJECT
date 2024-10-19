//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━	
// 작성자      : 이제완					
// 최종 수정일 : 2002. 7. 7				
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━	

#include <stdio.h>
#include <conio.h>

#include "..\\XCommon\\JXConnectSocket.h"			
#include "..\\XCommon\\JXGateSocket.h"				
#include "..\\XCommon\\JXSocketMgr.h"				
#include "..\\XCommon\\JXSocket.h"					
#include "..\\XCommon\\JXPacket.h"					
#include "..\\XCommon\\JXPacketQueue.h"				
#include "..\\XCommon\\JXAcceptSocketList.h"		
#include "..\\XCommon\\JXWorkManager.h"				

#include "..\\XCommon\\XPacketCmd.h"				

#include "XMonitor.h"

#include "XPlayerMgr.h"
#include "XGameServerMgr.h"

#include "XPlayer.h"
#include "XGameServer.h"
#include "XControlServer.h"

#include "XDBProc.h"
#include "XGateServer.h"

XGateServer::XGateServer()
{

	// 플레이어
	m_pGateSocketPlayer = new JXGateSocket;	
	m_pAcceptListPlayer = new JXAcceptSocketList;
	m_pConnectListPlayer = new JXSocketMgr;

	m_pPlayerMgr = new XPlayerMgr;
	m_pPlayerMgr->m_pGateServer = this;
	
	// 게임서버

	m_pGameServerSocket = new JXConnectSocket;
	m_pGameServer = new XGameServer;
	

	// 컨트롤 서버 소켓 
	m_pControlServerSocket = new JXConnectSocket;
	m_pControlServer = new XControlServer;

	// 객체와 소켓 연결
	m_pControlServer->SetSocket( m_pControlServerSocket );


	m_bServerRun = FALSE;

	for( int i = 0; i < 10000; ++i ) {
		m_pPlayerOnAccount[ i ] = NULL;
	}

	m_pPlayerToRemove = NULL;
	m_siNumPlayerToRemove = 0;

}

XGateServer::~XGateServer()
{	
	if( m_pPlayerToRemove ) delete [] m_pPlayerToRemove;
	
	delete m_pGateSocketPlayer;
	delete m_pAcceptListPlayer;
	delete m_pConnectListPlayer;
	delete m_pPlayerMgr;

	delete m_pGameServerSocket;
	delete m_pGameServer;

	delete m_pControlServerSocket;
	delete m_pControlServer;

	delete m_pMonitorSocket;
	delete m_pMonitor;

}

BOOL XGateServer::Init()
{

	char szIP[ 256 ];

	FILE *fp;

	if( ( fp = fopen( "GameServerIP.txt", "rt") ) == NULL )	{

		printf( "\'GameServerIP.txt\' read error..\n" );
		getch();
		return FALSE;
	}

	fgets( szIP, 256, fp );

	fclose( fp ); 

	SI32 siMaxPlayer = 4000;

	if( m_pPlayerToRemove ) delete [] m_pPlayerToRemove;

	m_pPlayerToRemove = new XPlayer*[ siMaxPlayer ];
	m_siNumPlayerToRemove = 0;

	// 플레이어 관련 초기화 
	m_pAcceptListPlayer->Create( siMaxPlayer, 80000, 80000, 80000 );
	m_pGateSocketPlayer->Create( 8000, m_pAcceptListPlayer );
	m_pPlayerMgr->Create( siMaxPlayer );


	// 게임서버 관련 초기화
	m_pGameServerSocket->Create( szIP, 22220, 10000000, 10000000, 10000000 );
	m_pGameServer->SetSocket( m_pGameServerSocket );


	// 모니터링 서버와의 통신	
	m_pMonitorSocket = new JXConnectSocket;
	m_pMonitor = new XMonitor;

	m_pMonitorSocket->Create( "127.0.0.1", 12002, 100000, 100000, 100000 );
	m_pMonitor->SetSocket( m_pMonitorSocket );
/*
	printf( "모니터링 서버로 접속 시도 중.. " );
	while( m_pMonitorSocket->Connect() == FALSE ) printf( "." );
	printf( "[성공]\n" );
*/
	m_pMonitorSocket->StartThread();


	return TRUE;
}


void XGateServer::Start()
{
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf(" Gersang Online GateServer Ver 0.1\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");


	// 게임서버

	printf( "Trying to Connect GameServer..." );
	while( m_pGameServerSocket->Connect() == FALSE ) printf( "." );
	printf( "[Success]\n" );

	m_pGameServerSocket->StartThread();

	// 플레이어
	m_pGateSocketPlayer->StartThread();
	m_pConnectListPlayer->StartThread();

	m_bServerRun = TRUE;
}

void XGateServer::Stop()
{
	m_bServerRun = FALSE;

	printf( "Server Stop\n" );

}

void XGateServer::Run()
{

	UI32			i;
	SI32			j;
	JXSocket		*pSocket;
	
	XPlayer			*pPlayer;
	
	JXPacketQueue	*pPacketQueue = NULL;

	DWORD			m_tick, m_tick1;

	m_tick1 = GetTickCount();

	while( m_bServerRun ) {

		// 접속을 받아들인 신규 소켓 리스트를 본 리스트로 옮겨온다

		Sleep( 10 );

		// (플레이어)
		m_pAcceptListPlayer->Wait( INFINITE );
		m_pConnectListPlayer->Wait( INFINITE );

		// 접속 끊기

		for( j = 0; j < m_siNumPlayerToRemove; ++j ) {
			pPlayer = m_pPlayerToRemove[ j ];

			if( pPlayer->IsValid() == FALSE ) continue;

			FD_CLR( pPlayer->GetSocket()->m_socket, &m_pConnectListPlayer->m_fdset );

			pPlayer->GetSocket()->Disconnect();

			if( pPlayer->m_bSendLogOutToGameServer ) SendLogOut( pPlayer->m_usAccount );

			m_pConnectListPlayer->Remove( (JXObject *)(pPlayer->GetSocket()) );
			
			m_pPlayerMgr->DelPlayer( pPlayer );

			pPlayer->m_usAccount = 0;

		}
		m_siNumPlayerToRemove = 0;

		
		BEGIN_JXLIST_LOOP( i, m_pAcceptListPlayer, JXSocket, pSocket ) 

			printf("AcceptList -> ConnectList : Socket Addr[ %x ]\n", pSocket );			
			
			m_pPlayerMgr->AddPlayer( pSocket );
			
			FD_SET( pSocket->m_socket, &m_pConnectListPlayer->m_fdset );
		
		END_JXLIST_LOOP

		m_pConnectListPlayer->PushFront( m_pAcceptListPlayer );


		m_pConnectListPlayer->Release();
		m_pAcceptListPlayer->Release();

		
		// 플레이어 프로세스
		BEGIN_JXLIST_LOOP( i, m_pPlayerMgr, XPlayer, pPlayer ) 
			
			ProcPacketPlayer( pPlayer );

		END_JXLIST_LOOP

		// 게임서버 프로세스 

		ProcPacketGameServer( m_pGameServer );

		// 콘솔처리
		ProcessConsole();

		// 모니터 서버
		ProcPacketMonitor( m_pMonitor );

		m_tick = GetTickCount();

		if( m_tick - m_tick1 >= 5000 ) {
			m_tick1 = m_tick;
			SendHeartBeat();
		}

	}

}

void XGateServer::ProcessConsole()
{

	char ch;

	if( kbhit() ) {

		ch = getch();

		switch( ch ) {
		case 27:	// ESC
			Stop();
			break;

		case '1':
			printf( "Current Connections [%d][%d]\n", 
				m_pConnectListPlayer->GetNumberNodes(),
				m_pPlayerMgr->GetNumberNodes() );
			break;

		case '2':
		// 버퍼 크기 표시
			printf( "Send[%d], Recv[%d], Queue[%d]\n", 
				m_pGameServerSocket->GetSendDataSize(),  m_pGameServerSocket->GetRecvDataSize(),
				m_pGameServerSocket->GetQueue()->GetDataSize() );
			break;

		}

	}

}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 게임 서버 패킷 처리
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL XGateServer::ProcPacketGameServer( XGameServer *pGameServer )
{

	BOOL bConnect;
	JXPacketQueue *pPacketQueue = NULL;
	
	pGameServer->GetSocket()->IsConnect( &bConnect );

	if( bConnect == FALSE ) {

//		m_pGateSocketPlayer->m_bAcceptable = FALSE;

		writeInfoToFile( TRUE, "JXSocketERR.txt", "게임 서버와 접속 끊어짐!!\n" );

		Sleep( 2000 );

		LogOutGameServer( pGameServer );

		return FALSE;
	}

	pPacketQueue = pGameServer->GetSocket()->GetQueue();

	UI32 uiNumPacket = pPacketQueue->GetNumPacket();

	for( UI32 i = 0; i < uiNumPacket; ++i ) {

		if( pPacketQueue->Dequeue( &local_packetA ) == FALSE ) return TRUE;

		local_packetA.ReadValue( &local_uiCmd, sizeof( local_uiCmd ) );

		switch( local_uiCmd ) {
	
		case JXCMD_REPORT_PLAYERACCOUNT: 
			// Account번호를 받아서 셋팅							
			{
				local_packetA.ReadValue( &local_uiID, 4 );			
				local_packetA.ReadValue( &local_usAccount, 2 );		

				m_pPlayerOnAccount[ local_usAccount ] = m_pPlayerMgr->SetAccount( local_uiID, local_usAccount );

				printf("[%d] Recv Account Number.... [%d]\n", local_uiID, local_usAccount );			

			}
			break;

		case JXCMD_LOGOUT_PLAYER:
			// 로그 아웃 처리
			{
				local_packetA.ReadValue( &local_usAccount, 2 );
				LogOutPlayer( local_usAccount );

			}
			break;

		case JXCMD_SENDPACKET_TO_PLAYER:
			// 특정 유저에게 패킷 전달
			{
				local_packetA.ReadValue( &local_usAccount, 2 );
				local_packetA.ReadValue( &local_siPacketSize, 4 );
				local_packetA.ReadValue( local_buf, local_siPacketSize );

				m_pPlayerMgr->SendPacket( local_usAccount, local_buf, local_siPacketSize );

			}
			break;

		case JXCMD_SENDPACKET_TO_PLAYERGROUP:
			// 다수의 유저 그룹에게 패킷 전달
			{
				local_packetA.ReadValue( &local_siSendNum, 4 );
				local_packetA.ReadValue( local_usAccounts, local_siSendNum * 2 );
				local_packetA.ReadValue( &local_siPacketSize, 4 );
				local_packetA.ReadValue( local_buf, local_siPacketSize );

				local_packetB.SetPacket( local_buf );

				m_pPlayerMgr->SendPacket( local_siSendNum, local_usAccounts, &local_packetB );
			}
			break;

		case JXCMD_SENDPACKET_TO_PLAYERALL:
			// 모든 유저에게 패킷 전달
			{
				local_packetA.ReadValue( &local_siPacketSize, 4 );
				local_packetA.ReadValue( local_buf, local_siPacketSize );

				m_pPlayerMgr->SendPacket( local_buf, local_siPacketSize );
			}
			break;
		}
	}

	return TRUE;

}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 플레이어 패킷 처리
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void XGateServer::ProcPacketPlayer( XPlayer *pPlayer )
{
 
	BOOL bConnect;
	JXPacketQueue *pPacketQueue = NULL;

	pPlayer->GetSocket()->IsConnect( &bConnect );

	if( bConnect == FALSE ) {
		
		if( pPlayer->m_usAccount != 0 ) pPlayer->m_bSendLogOutToGameServer = TRUE;
		LogOutPlayer( pPlayer );
		return;
	}

	DWORD dwCurrTickCount = GetTickCount();

	if( dwCurrTickCount - pPlayer->GetSocket()->m_dwLastTickCount > 180 * 1000 &&
		dwCurrTickCount - pPlayer->GetSocket()->m_dwLastTickCount < 300 * 1000 ) {
		printf( "허트 비트 시간 초과! \n" );
		if( pPlayer->m_usAccount != 0 ) pPlayer->m_bSendLogOutToGameServer = TRUE;
		LogOutPlayer( pPlayer );
		return;
	}
	
	// 어카운트가 없으면, 게임 서버에 어카운트를 요청한다 
	if( pPlayer->m_usAccount == 0 ) {

		if( pPlayer->m_bRequestAccount ) return;

		char szip[ 20 ];
/*
		SOCKADDR_IN		saddr;
		int addrlen;

		addrlen = sizeof( saddr );
		getpeername( pPlayer->GetSocket()->GetSocket(), (SOCKADDR *)&saddr, &addrlen );

		strcpy( szip, inet_ntoa( saddr.sin_addr ) );
*/

		memcpy( szip, pPlayer->m_szIP, 16 );
		szip[ 15 ] = NULL;

		local_uiCmd = JXCMD_REQUEST_PLAYERACCOUNT;

		local_packetA.ClearPacket();
		local_packetA.WriteValue( &local_uiCmd, sizeof( local_uiCmd ) );
		local_packetA.WriteValue( &pPlayer->m_uiID, 4 );
		local_packetA.WriteValue( szip, 20 );
		local_packetA.CompletePacket();

		m_pGameServer->SendPacket( &local_packetA );

		pPlayer->m_bRequestAccount = TRUE;

		printf("[%d] Request Account....\n", pPlayer->m_uiID );

		return;
	}

	pPacketQueue = pPlayer->GetSocket()->GetQueue();

	UI32 uiNumPacket = pPacketQueue->GetNumPacket();

	for( UI32 i = 0; i < uiNumPacket; ++i ) {

		if( pPacketQueue->Dequeue( &local_packetA ) == FALSE ) return;

		// 게임 서버로 보낼 패킷 만들기 
		local_uiCmd = JXCMD_SENDPACKET_TO_GAMESERVER;

		local_packetB.ClearPacket();
		local_packetB.WriteValue( &local_uiCmd, sizeof( local_uiCmd ) );
		local_packetB.WriteValue( &pPlayer->m_usAccount, 2 );
		local_packetB.WriteValue( &local_packetA.m_siDataSize, 4 );
		local_packetB.WriteValue( local_packetA.GetPacket(), local_packetA.GetPacketSize() );
		local_packetB.CompletePacket();
	
		// 게임 서버로 패킷 전달 
		m_pGameServer->SendPacket( &local_packetB );	

		//printf("게임서버로 패킷 전달 Account[%d], Size[%d]\n", pPlayer->m_usAccount, local_packetA.GetPacketSize() );

/*
		//////////////////// debug //////////////////////////////////////////////////////
		if( strcmp( pPlayer->m_szIP, "211.62.119.141" ) == 0 ) {

			local_packetA.ReadValue( &local_uiCmd, 2 );

			sprintf( local_buf, " Send Packet To GameServer: account:%d, cmd:%d\n", 
				pPlayer->m_usAccount,
				local_uiCmd ); 

			OutputDebugString( local_buf );
		}

		/////////////////////////////////////////////////////////////////////////////
*/
	}

}


BOOL XGateServer::ProcPacketControlServer( XControlServer *pControlServer )
{

	BOOL bConnect;
	JXPacketQueue *pPacketQueue = NULL;

	pControlServer->GetSocket()->IsConnect( &bConnect );

	pPacketQueue = pControlServer->GetSocket()->GetQueue();

	if( pPacketQueue->Dequeue( &local_packetA ) == FALSE ) return TRUE;

	local_packetA.ReadValue( &local_uiCmd, sizeof( local_uiCmd ) );

	switch( local_uiCmd ) {
	case 0:;
	};

	printf( "패킷도착 : %x\n", local_uiCmd );

	return TRUE;

}


// Log Out...........


void XGateServer::LogOutGameServer( XGameServer *pGameServer )
{
	printf("LogOut Gameserver..\n");

	m_pAcceptListPlayer->Wait( INFINITE );
	m_pConnectListPlayer->Wait( INFINITE );
/*
	UI32 i;
	XPlayer *pPlayer;

	m_siNumPlayerToRemove = 0;

	BEGIN_JXLIST_LOOP( i, m_pPlayerMgr, XPlayer, pPlayer )
			
		LogOutPlayer( pPlayer );

	END_JXLIST_LOOP

	for( int j = 0; j < m_siNumPlayerToRemove; ++j ) {

		pPlayer = m_pPlayerToRemove[ j ];

		if( pPlayer->IsValid() == FALSE ) continue;

		FD_CLR( pPlayer->GetSocket()->m_socket, &m_pConnectListPlayer->m_fdset );

		pPlayer->GetSocket()->Disconnect();

		m_pConnectListPlayer->Remove( (JXObject *)(pPlayer->GetSocket()) );
			
		m_pPlayerMgr->DelPlayer( pPlayer );

	}
	m_siNumPlayerToRemove = 0;
*/
	JXPacketQueue *pPacketQueue = pGameServer->GetSocket()->GetQueue();
	pPacketQueue->Clear();

	//m_pGameServer->TryConnect();

	printf( "Trying to Connect GameServer..." );
	while( m_pGameServerSocket->Connect() == FALSE ) printf( "." );
	printf( "[Success]\n" );

	m_pConnectListPlayer->Release();
	m_pAcceptListPlayer->Release();
}


void XGateServer::LogOutPlayer( UI32 uiLocalID )
{

	UI32	i;
	XPlayer *pPlayer;

	BEGIN_JXLIST_LOOP( i, m_pPlayerMgr, XPlayer, pPlayer )

		if( pPlayer->m_uiID == uiLocalID ) {
			
			LogOutPlayer( pPlayer );
			break;
		}

	END_JXLIST_LOOP

}



void XGateServer::LogOutPlayer( UI16 usAccount )
{
	if( m_pPlayerOnAccount[ usAccount ] ) {
	
		LogOutPlayer( m_pPlayerOnAccount[ usAccount ] );
	}

}

void XGateServer::LogOutPlayer( XPlayer *pPlayer )
{

	printf("LogOut Player.. Account:%d XPlayer Addr:%x\n", pPlayer->m_usAccount, pPlayer );

	m_pPlayerOnAccount[ pPlayer->m_usAccount ] = NULL;

	m_pPlayerToRemove[ m_siNumPlayerToRemove++ ] = pPlayer;

	return;

}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 게임서버로 플레이어 로그 아웃 요청 메시지 보냄 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void XGateServer::SendLogOut( UI16 usAccount )
{
	if( usAccount != 0 ) {

		printf("로그 아웃 메시지 보냄Account[%d]\n", usAccount );

		local_uiCmd = JXCMD_LOGOUT_PLAYER;

		local_packetA.ClearPacket();
		local_packetA.WriteValue( &local_uiCmd, sizeof( local_uiCmd ) );
		local_packetA.WriteValue( &usAccount, 2 );
		local_packetA.CompletePacket();

		m_pGameServer->SendPacket( &local_packetA );

	}

}


void XGateServer::ProcPacketMonitor( XMonitor *pMonitor )
{
	if( pMonitor == NULL ) return;

	BOOL bConnect;
	JXPacketQueue *pPacketQueue = NULL;

	pPacketQueue = pMonitor->GetSocket()->GetQueue();

	pMonitor->GetSocket()->IsConnect( &bConnect );

	if( bConnect == FALSE ) {

		pPacketQueue->Clear();

		m_pMonitorSocket->TryConnect();
/*
		printf( "모니터링 서버로 접속 시도 중.. " );
		while( m_pMonitorSocket->Connect() == FALSE ) printf( "." );
		printf( "[성공]\n" );
*/
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

	}

}

void XGateServer::SendHeartBeat()
{
	
	UI16 uiCmd;

	DWORD pid = GetCurrentProcessId();

	uiCmd = 0x0110;

	local_packetD.ClearPacket();
	local_packetD.WriteValue( &uiCmd, sizeof( uiCmd ) );
	local_packetD.WriteValue( &pid, sizeof( pid ) );
	local_packetD.CompletePacket();

	m_pMonitor->GetSocket()->SendPacket( &local_packetD );

}


