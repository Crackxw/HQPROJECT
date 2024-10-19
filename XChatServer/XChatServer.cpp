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

#include "packetcmd.h"
#include "XPlayerMgr.h"
#include "XPlayer.h"
#include "XMap.h"
#include "XCell.h"
#include "XMonitor.h"

#include "XChatServer.h"

XChatServer::XChatServer()
{

	// 플레이어
	m_pGateSocketPlayer = new JXGateSocket;	
	m_pAcceptListPlayer = new JXAcceptSocketList;
	m_pConnectListPlayer = new JXSocketMgr;

	m_pPlayerPtrList = new JXList;

	m_pPlayerMgr = new XPlayerMgr;
	
	m_bServerRun = FALSE;

	m_pPlayerToRemove = NULL;
	m_siNumPlayerToRemove = 0;

	m_pPlayerPtr = NULL;
	m_pMap = NULL;
}

XChatServer::~XChatServer()
{	
	if( m_pPlayerToRemove ) delete [] m_pPlayerToRemove;
	
	delete m_pPlayerPtrList;

	if( m_pPlayerPtr ) delete [] m_pPlayerPtr;

	if( m_pMap ) delete [] m_pMap;

	delete m_pPlayerMgr;	

	delete m_pGateSocketPlayer;
	delete m_pAcceptListPlayer;
	delete m_pConnectListPlayer;

	delete m_pMonitorSocket;
	delete m_pMonitor;
	
}

BOOL XChatServer::Init()
{

	int i;
	SI32 siMaxPlayer = 5000;

	// 맵 초기화
	if( m_pMap ) delete [] m_pMap;

	m_pMap = new XMap[ 64 ];

	m_pMap[ 0 ].InitMap( this, 120, 120 );

	for( i = 1; i < 64; ++i ) {

		m_pMap[ i ].InitMap( this, 40, 40 );
	}

	if( m_pPlayerToRemove ) delete [] m_pPlayerToRemove;

	m_pPlayerToRemove = new XPlayer*[ siMaxPlayer ];
	m_siNumPlayerToRemove = 0;

	if( m_pPlayerPtr ) delete [] m_pPlayerPtr;

	m_pPlayerPtr = new JXObjectPtr[ siMaxPlayer ]; 

	// 플레이어 관련 초기화 
	m_pAcceptListPlayer->Create( siMaxPlayer, 15000, 15000, 15000 );
	m_pGateSocketPlayer->Create( 8500, m_pAcceptListPlayer );
	m_pPlayerMgr->Create( siMaxPlayer );	

	// 플레이어 포인터 리스트 초기화

	for( i = 0; i < siMaxPlayer; ++i ) {

		m_pPlayerPtrList->AddToFree( &m_pPlayerPtr[ i ] );
	}

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


void XChatServer::Start()
{

	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf(" Gersang Online Chatting Server Ver 0.1\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");

	// 플레이어
	m_pGateSocketPlayer->StartThread();
	m_pConnectListPlayer->StartThread();

	m_bServerRun = TRUE;
}

void XChatServer::Stop()
{
	m_bServerRun = FALSE;

	printf( "서버를 정지합니다\n" );

}

void XChatServer::Run()
{

	UI32			i;
	SI32			j;
	JXSocket		*pSocket;
	
	XPlayer			*pPlayer;
	
	JXPacketQueue	*pPacketQueue = NULL;

	DWORD			m_tick;
	DWORD			m_tick2, m_tick3;

	m_tick2 = m_tick3 = GetTickCount();

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

			m_pConnectListPlayer->Remove( (JXObject *)(pPlayer->GetSocket()) );
			
			m_pPlayerMgr->DelPlayer( pPlayer );

			pPlayer->m_uiAccount = 0;

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

		ProcPacketMonitor( m_pMonitor );

		// 콘솔처리
		ProcessConsole();
	
		m_tick = GetTickCount();

		if( m_tick - m_tick2 >= 5000 ) {
			m_tick2 = m_tick;
			SendHeartBeat();
		}

		if( m_tick - m_tick3 >= 20000 ) {
			m_tick3 = m_tick;
			SendHeartBeatToPlayers();
		}

	}

}

void XChatServer::ProcessConsole()
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

		}

	}

}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 플레이어 패킷 처리
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void XChatServer::ProcPacketPlayer( XPlayer *pPlayer )
{
 
	UI32 i, j;
	UI16 result;

	BOOL bConnect;
	JXPacketQueue *pPacketQueue = NULL;

	pPlayer->GetSocket()->IsConnect( &bConnect );

	if( bConnect == FALSE ) {
		
		LogOutPlayer( pPlayer );
		return;
	}

	DWORD dwCurrTickCount = GetTickCount();
/*
	if( dwCurrTickCount - pPlayer->GetSocket()->m_dwLastTickCount > 180 * 1000 &&
		dwCurrTickCount - pPlayer->GetSocket()->m_dwLastTickCount < 300 * 1000 ) {
		printf( "허트 비트 시간 초과! \n" );
		LogOutPlayer( pPlayer );
		return;
	}
*/
	pPacketQueue = pPlayer->GetSocket()->GetQueue();

	UI32 uiNumPacket = pPacketQueue->GetNumPacket();

	for( i = 0; i < uiNumPacket; ++i ) {

		if( pPacketQueue->Dequeue( &local_packetA ) == FALSE ) return;

		local_packetA.ReadValue( &local_usCmd, 2 );

		if( pPlayer->m_uiAccount == 0 ) {
			if( local_usCmd == REQUEST_LOGIN ) {

				UI32 uiAccount;
				char szID[ 32 ];
				XPlayer *player;

				memset( szID, 0, 32 );

				local_packetA.ReadValue( &uiAccount, 4 );
				local_packetA.ReadString( szID, 32 );

				printf( "Login: [%s][%d]\n", szID, uiAccount );

				// 접속되어 있는지 검사 

				BEGIN_JXLIST_LOOP( j, m_pPlayerMgr, XPlayer, player )

					if( player->m_uiAccount == uiAccount ) {
						player->m_szID[ 0 ] = 0;
						player->m_uiAccount = 0;
						LogOutPlayer( player );
						break;
					}
			
				END_JXLIST_LOOP

				// 로그인 처리
				pPlayer->m_uiAccount = uiAccount;
				memcpy( pPlayer->m_szID, szID, 32 );


				// 응답전송
				local_usCmd = REPORT_LOGIN;
				result = 0;
				local_packetB.ClearPacket();
				local_packetB.WriteValue( &local_usCmd, 2 );
				local_packetB.WriteValue( &result, 2 );
				local_packetB.CompletePacket();

				pPlayer->GetSocket()->SendPacket( &local_packetB );

			}
			return;

		}

		switch( local_usCmd ) {
	
		case REQUEST_MOVECELL:
			// 셀이동
			{
				UI16 usMapCode;
				UI16 usCellX;
				UI16 usCellY;

				local_packetA.ReadValue( &usMapCode, 2 );
				local_packetA.ReadValue( &usCellX, 2 );
				local_packetA.ReadValue( &usCellY, 2 );
/*
				printf( "Move : [%s][%d] Map: %d, CellX: %d, CellY: %d\n ", 
					pPlayer->m_szID, pPlayer->m_uiAccount, usMapCode, usCellX, usCellY );
*/
				if( usMapCode > 63 ) break;

				// 기존 셀에서 빠져 나온다 
				if( pPlayer->m_pCell ) {

					pPlayer->m_pCell->DelPlayer( pPlayer );
				}
				
				// 새로운 셀에 추가한다
				m_pMap[ usMapCode ].AddPlayer( usCellX + 1, usCellY + 1, pPlayer );
				
				pPlayer->m_usMapCode = usMapCode;
				
			}
			break;

		case REQUEST_OUTWORLD:
			{

				if( pPlayer->m_pCell ) {
					
					pPlayer->m_pCell->DelPlayer( pPlayer );
				}
			}
			break;

		case REQUEST_CHATMESSAGE:
			{
				if( pPlayer->m_usMapCode > 63 ) break;

				// 메시지 읽기 
				local_packetA.ReadString( local_buf, 256 );


				// 보낼 패킷 만들기
				local_usCmd = CHATMESSAGE;

				local_packetB.ClearPacket();
				local_packetB.WriteValue( &local_usCmd, 2 );
				local_packetB.WriteValue( &pPlayer->m_uiAccount, 4 );
				local_packetB.WriteString( pPlayer->m_szID, 32 );
				local_packetB.WriteString( local_buf, 256 );
				local_packetB.CompletePacket();

				m_pMap[ pPlayer->m_usMapCode ].SendPacket( pPlayer, &local_packetB );

			}
			break;

		case REQUEST_WHISPER:
			{
				char szID[ 32 ];
				BOOL ret;

				local_packetA.ReadString( szID, 32 );
				local_packetA.ReadString( local_buf, 256 );
				

				// 보낼 패킷 만들기
				local_usCmd = WHISPER;

				local_packetB.ClearPacket();
				local_packetB.WriteValue( &local_usCmd, 2 );
				local_packetB.WriteString( pPlayer->m_szID, 32 );
				local_packetB.WriteString( local_buf, 256 );
				local_packetB.CompletePacket();
				
				ret = m_pPlayerMgr->SendPacket( szID, &local_packetB );

				// 귓속말 결과 패킷 만들기

				if( ret ) result = 0;
				else result = 1;

				local_usCmd = REPORT_WHISPER;

				local_packetB.ClearPacket();
				local_packetB.WriteValue( &local_usCmd, 2 );
				local_packetB.WriteValue( &result, 2 );
				local_packetB.WriteString( szID, 32 );
				local_packetB.WriteString( local_buf, 256 );

				local_packetB.CompletePacket();
				
				pPlayer->GetSocket()->SendPacket( &local_packetB );


			}
			break;
		}

	}

}


void XChatServer::LogOutPlayer( UI32 uiAccount )
{

	UI32	i;
	XPlayer *pPlayer;

	BEGIN_JXLIST_LOOP( i, m_pPlayerMgr, XPlayer, pPlayer )

		if( pPlayer->m_uiAccount == uiAccount ) {
			
			LogOutPlayer( pPlayer );
			break;
		}

	END_JXLIST_LOOP

}


void XChatServer::LogOutPlayer( XPlayer *pPlayer )
{

	printf("LogOut Player.. Account:%d XPlayer Addr:%x\n", pPlayer->m_uiAccount, pPlayer );

	m_pPlayerToRemove[ m_siNumPlayerToRemove++ ] = pPlayer;

	// 기존 셀에서 빠져 나온다 
	if( pPlayer->m_pCell ) {

		pPlayer->m_pCell->DelPlayer( pPlayer );
	}

	return;

}

void XChatServer::ProcPacketMonitor( XMonitor *pMonitor )
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

void XChatServer::SendHeartBeat()
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

void XChatServer::SendHeartBeatToPlayers()
{

	UI16 usCmd;

	usCmd = HEARTBEAT;

	local_packetD.ClearPacket();
	local_packetD.WriteValue( &usCmd, 2 );
	local_packetD.CompletePacket();

	m_pPlayerMgr->SendPacket( &local_packetD );

}