#include "XChatServer.h"

#include "chatprotocol.h"

#include <stdio.h>
#include <conio.h>

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

	m_pRoomMgr			= new XRoomMgr;
	m_pUserMgr			= new XUserMgr;

}


XChatServer::~XChatServer()
{
	delete m_iocpserver;
	delete m_listen;

	delete m_pRoomMgr;
	delete m_pUserMgr;
}


void XChatServer::Init( UI16 usPort )
{

	m_usport = usPort;

	SI32 siMaxUser = 5000;
	SI32 siMaxRoom = 300;

	m_pUserMgr->Create( siMaxUser );

	m_pRoomMgr->Create( this, siMaxRoom );

	////////////////////////////////////////////
	// iocp 서버 초기화 , listen 
	m_iocpserver->InitServer( siMaxUser, 10000, 10000 );
	m_listen->Init( m_iocpserver, m_usport );

	m_listen->Start();
}

void XChatServer::Start()
{
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n" );
	printf(" Gersang Web Chatting Server ver 1.0)\n" );
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n" );
}

void XChatServer::Run()
{

	UI32 i;
	XUser *pUser;

	DWORD dwCurrTick;

	m_dwTick = GetTickCount();

	BOOL bRun = TRUE;

	while( bRun ) {

		Sleep( 10 );

		dwCurrTick = GetTickCount();

		if( dwCurrTick - m_dwTick > 10000 ) {
			m_dwTick = dwCurrTick;

			printf( "---------------------------------------------------------------------\n" );
			printf( " Number of Connected socket : %d\n", m_pUserMgr->GetNumberOfNodes() );
			printf( " Number of Room : %d\n", m_pRoomMgr->GetNumRoom() );
			printf( "---------------------------------------------------------------------\n" );
		}


		m_iocpserver->UpdateServer( m_pUserMgr );

		JWLIST_LOOP( i, m_pUserMgr, XUser, pUser )
		{
			// 패킷 처리
			ProcUserPacket( pUser );
		}

		// console
		if( kbhit() ) {

			switch( getch() )
			{
			case 27:
				bRun = FALSE;
				break;

			case '1':
				printf( "Current Connection [%d]\n", m_pUserMgr->GetNumberOfNodes() );
				break;

			default:
				printf( "key pressed\n" );
				break;

			}

		}

	}

}

void XChatServer::LogOutUser( XUser *pUser )
{

	XRoom *pRoom = pUser->GetRoom();
			
	if( pRoom != NULL ) pRoom->DelUser( pUser, EXITCODE_DISCONNECT );

	m_iocpserver->CompleteLogOut( (JWSocket *)(pUser->GetFriend()) );
}

void XChatServer::ProcUserPacket( XUser *pUser )
{

	if( pUser->IsOnAction() ) {
		LogOutUser( pUser );
		return;
	}


	JWSocket	 *pSocket = (JWSocket *)( pUser->GetFriend() );

	char	     *pPacket = pSocket->GetRecvBuffer()->GetFirstPacket();

	if( pPacket == NULL ) return;

	printf( "Recved Packet\n" );

	sPacketHeader	*pPacketHeader = (sPacketHeader *)pPacket;

	printf( "command: %d\n", pPacketHeader->command );

	switch( pPacketHeader->command ) {

	case REQUEST_LOGIN:
		{
			sRequest_LogIn *pRecvMsg = (sRequest_LogIn *)pPacket;

			pUser->LogIn( pRecvMsg );

			sResponse_LogIn sendMsg;

			sendMsg.packetHeader.command = RESPONSE_LOGIN;
			sendMsg.packetHeader.size = sizeof( sendMsg );

			sendMsg.result = RESULT_SUCCESS;

			pUser->SendPacket( (char *)&sendMsg );
		}
		break;

	case REQUEST_LOGOUT:
		{

		}
		break;

	case REQUEST_ROOMLIST:
		{
			m_pRoomMgr->SendRoomList( pUser );
		}
		break;

	case REQUEST_MAKE_ROOM:
		{			
			sRequest_MakeRoom *pRecvMsg = (sRequest_MakeRoom *)pPacket;

			sResponse_MakeRoom sendMsg;

			sendMsg.packetHeader.command = RESPONSE_MAKE_ROOM;
			sendMsg.packetHeader.size = sizeof( sendMsg );


			sendMsg.result = m_pRoomMgr->MakeRoom( pRecvMsg, pUser, &sendMsg.usRoomNum );

			pUser->SendPacket( (char *)&sendMsg );

		}
		break;

	case REQUEST_JOIN_ROOM:
		{
			sRequest_JoinRoom *pRecvMsg = (sRequest_JoinRoom *)pPacket;

			sResponse_JoinRoom sendMsg;

			sendMsg.packetHeader.command = RESPONSE_JOIN_ROOM;
			sendMsg.packetHeader.size = sizeof( sendMsg );

			sendMsg.result = m_pRoomMgr->JoinRoom( pRecvMsg, pUser );

			pUser->SendPacket( (char *)&sendMsg );

		}
		break;

	case REQUEST_EXIT_ROOM:
		{

			XRoom *pRoom = pUser->GetRoom();
			
			if( pRoom == NULL ) break;

			sResponse_ExitRoom sendMsg;

			sendMsg.packetHeader.command = RESPONSE_EXIT_ROOM;
			sendMsg.packetHeader.size = sizeof( sendMsg );

			sendMsg.result = pRoom->DelUser( pUser, EXITCODE_USERREQUEST );
			
			pUser->SendPacket( (char *)&sendMsg );

		}
		break;

	case REQUEST_USERLIST:
		{
			XRoom *pRoom = pUser->GetRoom();
			
			if( pRoom == NULL ) break;

			pRoom->SendUserList( pUser );
		}
		break;

	case REQUEST_CHATMSG:
		{
			sRequest_ChatMsg *pRecvMsg = (sRequest_ChatMsg *)pPacket;
			
			XRoom *pRoom = pUser->GetRoom();
			
			if( pRoom == NULL ) break;

			pRoom->SendChatMsg( pRecvMsg, pUser );
		}
		break;

	case REQUEST_WHISPER:
		{
			sRequest_Whisper *pRecvMsg = (sRequest_Whisper *)pPacket;

			sResponse_Whisper *pSendMsg = (sResponse_Whisper *)local_buf;

			pSendMsg->packetHeader.command = RESPONSE_WHISPER;
			pSendMsg->packetHeader.size = sizeof( sResponse_Whisper ) + pRecvMsg->usStrLength;

			XRoom *pRoom = pUser->GetRoom();
			
			if( pRoom == NULL ) break;

			
			pSendMsg->usStrLength = pRecvMsg->usStrLength;
			
			memcpy( (char *)( pSendMsg ) + sizeof( sResponse_Whisper ),
				    (char *)( pRecvMsg ) + sizeof( sRequest_Whisper ),
					pRecvMsg->usStrLength );

			memcpy( pSendMsg->szReceiverID, pRecvMsg->szReceiverID, MAX_USER_ID );

			pSendMsg->result = pRoom->SendWhisper( pRecvMsg, pUser );

			pUser->SendPacket( (char *)pSendMsg );

		}
		break;


	case REQUEST_CHANGE_ROOMINFO:
		{
			sRequest_ChangeRoomInfo *pRecvMsg = (sRequest_ChangeRoomInfo *)pPacket;

			sResponse_ChangeRoomInfo sendMsg;

			sendMsg.packetHeader.command = RESPONSE_CHANGE_ROOMINFO;
			sendMsg.packetHeader.size = sizeof( sendMsg );

			XRoom *pRoom = pUser->GetRoom();
			
			if( pRoom == NULL ) break;

			sendMsg.result = pRoom->ChangeRoomInfo( pRecvMsg, pUser );

			pUser->SendPacket( (char *)&sendMsg );

		}
		break;

	case REQUEST_CHANGE_ROOMMASTER:
		{
			sRequest_ChangeRoomMaster *pRecvMsg = (sRequest_ChangeRoomMaster *)pPacket;

			sResponse_ChangeRoomMaster sendMsg;

			sendMsg.packetHeader.command = RESPONSE_CHANGE_ROOMMASTER;
			sendMsg.packetHeader.size = sizeof( sendMsg );

			XRoom *pRoom = pUser->GetRoom();
			
			if( pRoom == NULL ) break;

			sendMsg.result = pRoom->ChangeRoomMaster( pRecvMsg, pUser );

			pUser->SendPacket( (char *)&sendMsg );
			
		}
		break;

	case REQUEST_KICK_USER_OUT:
		{
			sRequest_KickUserOut *pRecvMsg = (sRequest_KickUserOut *)pPacket;

			sResponse_KickUserOut sendMsg;

			sendMsg.packetHeader.command = RESPONSE_KICK_USER_OUT;
			sendMsg.packetHeader.size = sizeof( sendMsg );

			XRoom *pRoom = pUser->GetRoom();
			
			if( pRoom == NULL ) break;

			sendMsg.result = pRoom->KickUserOut( pRecvMsg, pUser );

			pUser->SendPacket( (char *)&sendMsg );

		}
		break;

	}

	pSocket->GetRecvBuffer()->RemoveFirstPacket();

}

