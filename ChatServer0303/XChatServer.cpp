#include "XChatServer.h"
#include "XPlayerMgr.h"
#include "XPlayer.h"
#include "XMap.h"
#include "XCell.h"
#include "packetcmd.h"

#include <stdio.h>
#include <conio.h>
#include <time.h>

#include "../JWCommon/JWList.h"

#include "../JWCommon/JWIOCPServer.h"
#include "../JWCommon/JWListen.h"

#include "../JWCommon/JWSocket.h"
#include "../JWCommon/JWRecvBuffer.h"
#include "../JWCommon/JWSendBuffer.h"

XChatServer::XChatServer()
{

	m_iocpserver		= new JWIOCPServer;
	m_listen			= new JWListen;

	m_pPlayerPtrList	= new JWList;
	m_pPlayerMgr		= new XPlayerMgr;

	m_pPlayerPtr		= NULL;
	m_pMap				= NULL;
}


XChatServer::~XChatServer()
{	
}


void XChatServer::Init( UI16 usPort )
{

	m_usport = usPort;

	int i;
	SI32 siMaxPlayer = 10000;

	///////////////////////////////////////////
	// 맵 초기화
	m_pMap = new XMap[ 64 ];

	m_pMap[ 0 ].InitMap( this, 120, 120 );

	for( i = 1; i < 64; ++i ) {

		m_pMap[ i ].InitMap( this, 40, 40 );
	}

	m_pPlayerPtr = new JWObjectPtr[ siMaxPlayer ]; 

	m_pPlayerMgr->Create( siMaxPlayer );

	////////////////////////////////////////////
	// 플레이어 포인터 리스트 초기화

	for( i = 0; i < siMaxPlayer; ++i ) {

		m_pPlayerPtrList->NewObject( &m_pPlayerPtr[ i ] );
	}

	////////////////////////////////////////////
	// iocp 서버 초기화 , listen 
	m_iocpserver->InitServer( siMaxPlayer, 10000, 10000 );
	m_listen->Init( m_iocpserver, m_usport );

	m_listen->Start();
}

void XChatServer::Start()
{
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n" );
	printf(" Gersang Online Chatting Server (2003/3/6)\n" );
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n" );
}

void XChatServer::Run()
{

	UI32 i;
	XPlayer *pPlayer;

	BOOL bRun = TRUE;

	while( bRun ) {

		Sleep( 10 );
		
		m_iocpserver->UpdateServer( m_pPlayerMgr );

		JWLIST_LOOP( i, m_pPlayerMgr, XPlayer, pPlayer )
		{

			// 패킷 처리
			ProcPlayerPacket( pPlayer );

			if( GetTickCount() - pPlayer->m_lastTick > 60000 ) pPlayer->SetAction();

		}

		if( kbhit() ) {

			switch( getch() )
			{
			case 27:
				bRun = FALSE;
				break;

			case '1':
				printf( "Current Connection [%d]\n", m_pPlayerMgr->GetNumberOfNodes() );
				break;

			case '2':
				printf( "Listen Port : %d\n", m_usport );
				break;

			default:
				printf( "key pressed\n" );
				break;

			}

		}

	}

}

void XChatServer::LogOutPlayer( XPlayer *pPlayer )
{

	printf( "LogOut[%s][%d]\n", pPlayer->m_szID, pPlayer->m_uiAccount );

	if( pPlayer->m_pCell ) {

		pPlayer->m_pCell->DelPlayer( pPlayer );
	}

	m_iocpserver->CompleteLogOut( (JWSocket *)(pPlayer->GetFriend()) );
}

void XChatServer::ProcPlayerPacket( XPlayer *pPlayer )
{

	if( pPlayer->IsOnAction() ) {
		LogOutPlayer( pPlayer );
		return;
	}

	UI32 j;
	UI16 result;

	JWSocket	 *pSocket = (JWSocket *)( pPlayer->GetFriend() );

	char	     *pPacket = pSocket->GetRecvBuffer()->GetFirstPacket();

	if( pPacket == NULL ) return;

	pPlayer->m_lastTick = GetTickCount();

	local_packetA.SetPacket( pPacket );

	local_packetA.ReadValue( &local_usCmd, 2 );

	pSocket->GetRecvBuffer()->RemoveFirstPacket();

	switch( local_usCmd ) {

	case REQUEST_LOGIN:
		{

			UI32		uiAccount;
			UI16		uiUserGrade;
			char szID[ 32 ];
			XPlayer *player;

			memset( szID, 0, 32 );

			local_packetA.ReadValue( &uiAccount, 4 );
			local_packetA.ReadValue( &uiUserGrade, 2 );
			local_packetA.ReadString( szID, 32 );

			printf( "Login: [%s][%d]\n", szID, uiAccount );

			// 접속되어 있는지 검사

			JWLIST_LOOP( j, m_pPlayerMgr, XPlayer, player )
			{

				if( player->m_uiAccount == uiAccount )
				{
					printf("Already Connected\n");
					player->m_szID[ 0 ] = 0;
					player->m_uiAccount = 0;
					player->m_uiUserGrade = ON_USERGRADE_NORMAL;
					LogOutPlayer( player );
					break;
				}
				
			}

			// 로그인 처리
			pPlayer->m_uiAccount	= uiAccount;
			pPlayer->m_uiUserGrade	= (UI16)uiUserGrade;
			memcpy( pPlayer->m_szID, szID, 32 );


			// 응답전송
			local_usCmd = REPORT_LOGIN;
			result = 0;
			local_packetB.ClearPacket();
			local_packetB.WriteValue( &local_usCmd, 2 );
			local_packetB.WriteValue( &result, 2 );
			local_packetB.CompletePacket();

			pPlayer->GetSocket()->GetSendBuffer()->Write( local_packetB.GetPacket() );
		}

		break;

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

			WriteNormalChatLog( pPlayer );

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

			WriteWhisperChatLog( pPlayer, GetPlayer(szID) );

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
			
			pPlayer->GetSocket()->GetSendBuffer()->Write( local_packetB.GetPacket() );

		}
		break;
	}
}


bool XChatServer::WriteInfo( char *FileName, const char *Content,... )
{
	char	Buf[1024];
	char	DateBuf[128];
	char	TimeBuf[128];
	int		Count;
	FILE	*fp = NULL;
	va_list	arglist;
	va_start(arglist,Content);
	Count = vsprintf(Buf, Content, arglist);
	fp	=	fopen(FileName, "at");
	if(fp == NULL)
		return false;

	_strtime( TimeBuf );
	_strdate( DateBuf );
	fprintf(fp,"[%s %s] %s\n",DateBuf,TimeBuf,Buf);
	//printf("[%s %s] %s\n",DateBuf,TimeBuf,Buf);
	fclose(fp);
	va_end(arglist);

	return true;
}

//--------------------------------------------------------------------------------------------------------------
// 아이디로 플레이어 객체 포인터를 얻어낸다. 없으면 NULL 리턴
//--------------------------------------------------------------------------------------------------------------
XPlayer*	XChatServer::GetPlayer( char *szID )
{
	XPlayer		*pPlayer;
	UI32		i;

	JWLIST_LOOP( i, m_pPlayerMgr, XPlayer, pPlayer )
	{
		if( strcmp(pPlayer->m_szID, szID) == 0 )
		{
			return pPlayer;
		}
	}

	return NULL;
}

void	XChatServer::GetDateString( char *szDate )
{
	time_t		ltime;
	struct tm	*today;

	time( &ltime );
	today = localtime( &ltime );
	strftime( szDate, 128, "%y-%m-%d", today );
}

void	XChatServer::WriteNormalChatLog( XPlayer *pPlayer )
{
	if( pPlayer->m_uiUserGrade < ON_USERGRADE_ADMIN1 )		return;		// 운영자가 아니면 리턴

	char		szDate[128];
	char		szFileName[128];
	// 운영자 채팅 로그 남기기
	GetDateString( szDate );
	sprintf( szFileName, "%s_%s.txt", szDate, pPlayer->m_szID );
	if(WriteInfo( szFileName, "%s(%d) : %s", pPlayer->m_szID, pPlayer->m_uiUserGrade, local_buf ) == false)
	{
		memset(szFileName, 0, sizeof(szFileName));
		sprintf(szFileName, "ChatError_%s.txt", szDate);
		ChatErrorLog(szFileName, "%s(%d) : %s", pPlayer->m_szID, pPlayer->m_uiUserGrade, local_buf);
	}
}

void	XChatServer::WriteWhisperChatLog( XPlayer *pPlayer, XPlayer *pDestPlayer )
{
	if( !pDestPlayer )										return;		// 귓말 대상이 접속중이지 않으면 리턴

	// 귓말을 시도한 사람이 운영자인 경우 일단 로그를 남긴다.
	if( pPlayer->m_uiUserGrade >= ON_USERGRADE_ADMIN1 )
	{
		char		szDate[128];
		char		szFileName[128];
		GetDateString( szDate );
		sprintf( szFileName, "%s_%s_Whisper.txt", szDate, pPlayer->m_szID );
		if(WriteInfo( szFileName, "%s(%d) To %s : %s", pPlayer->m_szID, pPlayer->m_uiUserGrade, pDestPlayer->m_szID, local_buf ) == false)
		{
			memset(szFileName, 0, sizeof(szFileName));
			sprintf(szFileName, "ChatError_%s.txt", szDate);
			ChatErrorLog(szFileName, "%s(%d) To %s : %s", pPlayer->m_szID, pPlayer->m_uiUserGrade, pDestPlayer->m_szID, local_buf);
		}
	}

	// 운영자가 귓말을 받는 경우에도 그 운영자 아이디를 파일명으로 하여 따로 로그를 남긴다.
	if( pDestPlayer->m_uiUserGrade >= ON_USERGRADE_ADMIN1 )
	{
		char		szDate[128];
		char		szFileName[128];
		GetDateString( szDate );
		sprintf( szFileName, "%s_%s_Whisper.txt", szDate, pDestPlayer->m_szID );
		if(WriteInfo( szFileName, "%s To %s(%d) : %s", pPlayer->m_szID, pDestPlayer->m_szID, pDestPlayer->m_uiUserGrade, local_buf ) == false)
		{
			memset(szFileName, 0, sizeof(szFileName));
			sprintf(szFileName, "ChatError_%s.txt", szDate);
			ChatErrorLog(szFileName, "%s To %s(%d) : %s", pPlayer->m_szID, pDestPlayer->m_szID, pDestPlayer->m_uiUserGrade, local_buf);
		}
	}
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 작성자 : 이민수	/	2004.01.14 PM
//
// 채팅 로그 남기는데 에러발생시 로그를 기록한다.
// 파일 생성 불가능 문자. (\, /, :, *, ?, <, >, ", |)
// 캐릭터 이름중에 위의 문자가 포함 되어 있으면 파일이 생성이 되지 않는다.
void	XChatServer::ChatErrorLog(char* pFileName, const char* pLog, ...)
{
	FILE* pFP = fopen(pFileName, "at");
	if(pFP == NULL) return;	

	if(strlen(pLog) >= 1024)
		return;

	va_list	vl;
	TCHAR	szLog[1024];
	char	DateBuf[128]={0,};
	char	TimeBuf[128]={0,};

	memset(szLog, 0, sizeof(szLog));
	va_start(vl, pLog);
	_vstprintf(szLog, pLog, vl);
	va_end(vl);

	int nLast = strlen(szLog) - 1;
	if(szLog[nLast] == '\n')
		szLog[nLast] = '\0';

	_strtime( TimeBuf );
	_strdate( DateBuf );	

	fprintf(pFP, "[%s %s] %s\r\n", DateBuf, TimeBuf, szLog);
	fflush(pFP);

	fclose(pFP);
}