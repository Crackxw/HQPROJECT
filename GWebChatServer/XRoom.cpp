
#include "XRoom.h"
#include "XUser.h"
#include "XRoomMgr.h"

#include "../JWCommon/JWList.h"
#include "../JWCommon/JWSocket.h"
#include "../JWCommon/JWSendBuffer.h"

XRoom::XRoom()
{
	m_pRoomMgr = NULL;

	m_pUserPtrList = new JWList;

	m_bOpened = FALSE;
}

XRoom::~XRoom()
{
	delete m_pUserPtrList;
}

void XRoom::CreateRoom( XRoomMgr *pRoomMgr, JWList *pList, UI16 usRoomNum  )
{
	m_pRoomMgr = pRoomMgr;

	m_pUserPtrList->SetSourceList( pList );
	
	m_roomInfo.usRoomNum = usRoomNum;

	m_bOpened = FALSE;
}

UI32 XRoom::MakeRoom( sRequest_MakeRoom *pMakeRoom, XUser *pUser )
{

	UI32 result;

	// room title
	memcpy( m_roomInfo.szRoomTitle, pMakeRoom->szRoomTitle, MAX_ROOM_TITLE );
	m_roomInfo.szRoomPassword[ MAX_ROOM_TITLE - 1 ] = NULL;

	// room attr
	m_roomInfo.roomAttr = pMakeRoom->roomAttr;

	if( m_roomInfo.roomAttr.usRoomType != ROOMTYPE_GENERAL &&
		m_roomInfo.roomAttr.usRoomType != ROOMTYPE_SECRET ) {

		return RESULT_UNKNOWN_FAILED;
	}



	m_roomInfo.roomAttr.usNumJoinedUser = 0;

	// room password
	if( m_roomInfo.roomAttr.usRoomType == ROOMTYPE_SECRET ) {

		memcpy( m_roomInfo.szRoomPassword, pMakeRoom->szRoomPassword, MAX_ROOM_PASSWORD );
		m_roomInfo.szRoomPassword[ MAX_ROOM_PASSWORD - 1 ] = NULL;
	} else {
		m_roomInfo.szRoomPassword[ 0 ] = NULL;
	}

	// room master

	memcpy( m_roomInfo.roomMasterInfo.szID, pUser->m_userInfo.szID, MAX_USER_ID );
	memcpy( m_roomInfo.roomMasterInfo.szName, pUser->m_userInfo.szID, MAX_USER_NAME );

	result = AddUser( pUser );

	if( result == RESULT_SUCCESS ) {
		m_bOpened = TRUE;

		pUser->SetUserType( USERTYPE_ROOMMASTER );
	}

	printf( "Make Room...\n" );
	printf( "Title: %s  Master:%s\n", m_roomInfo.szRoomTitle, pUser->m_userInfo.szID );  

	return result;
}

UI32 XRoom::JoinRoom( sRequest_JoinRoom *pJoinRoom, XUser *pUser )
{

	// 최대 참여인원을 초과 하였나
	if( m_roomInfo.roomAttr.usNumCapacity ==
		m_roomInfo.roomAttr.usNumJoinedUser ) return RESULT_OVER_ROOMCAPACITY;

	// 비밀 방이면, 패스워드 체크
	if( m_roomInfo.roomAttr.usRoomType == ROOMTYPE_SECRET ) {

		m_roomInfo.szRoomPassword[ MAX_ROOM_PASSWORD - 1 ] = NULL;
		pJoinRoom->szRoomPassword[ MAX_ROOM_PASSWORD - 1 ] = NULL;

		if( strcmp( m_roomInfo.szRoomPassword, pJoinRoom->szRoomPassword ) != 0 ) {

			return RESULT_INCORRECT_PASSWORD;
		}
	}

	return AddUser( pUser );
}


UI32 XRoom::AddUser( XUser *pUser )
{

	// 최대 참여인원을 초과 하였나
	if( m_roomInfo.roomAttr.usNumCapacity ==
		m_roomInfo.roomAttr.usNumJoinedUser ) return RESULT_OVER_ROOMCAPACITY;

	if( m_pUserPtrList->CanPush() == FALSE ) {
		return RESULT_UNKNOWN_FAILED;
	}

	XRoom *pRoom = pUser->GetRoom();

	if( pRoom != NULL ) pRoom->DelUser( pUser, EXITCODE_USERREQUEST );

	// 방원들에게 입장 알림
	NotifyUserJoined( pUser );

	JWObjectPtr *p = (JWObjectPtr *)( m_pUserPtrList->PushFront() ); 

	p->SetObject( (JWObject *)pUser );

	pUser->SetRoom( this );

	pUser->SetUserType( USERTYPE_ROOMMEMBER );

	++m_roomInfo.roomAttr.usNumJoinedUser;


	printf( "Add User : %s[%s]\n", pUser->m_userInfo.szID, pUser->m_userInfo.szName );

	return RESULT_SUCCESS;
}

UI32 XRoom::DelUser( XUser *pUser, UI16 usExitCode )
{
	UI32 i;
	XUser *user;
	JWObjectPtr *p;
	
	BOOL b = FALSE;

	JWLIST_LOOP( i, m_pUserPtrList, JWObjectPtr, p )
	{
		user = (XUser *)( p->GetObject() );
		if( user == pUser ) { b = TRUE; break; }
	}

	if( b == FALSE ) return RESULT_UNKNOWN_FAILED;

	m_pUserPtrList->Remove( p );

	pUser->SetRoom( NULL );

	--m_roomInfo.roomAttr.usNumJoinedUser;

	if( m_roomInfo.roomAttr.usNumJoinedUser == 0 ) CloseRoom();
	else {
		NotifyUserExited( pUser, usExitCode );

		// 나가는 사람이 방장이면,
		if( pUser->m_userInfo.usUserType == USERTYPE_ROOMMASTER ) {

			p = (JWObjectPtr *)( m_pUserPtrList->GetActualFirstNode() );
			user = (XUser *)( p->GetObject() );

			user->SetUserType( USERTYPE_ROOMMASTER );

			sNotify_RoomMasterChanged sendMsg;

			sendMsg.packetHeader.command = NOTIFY_ROOMMASTER_CHANGED;
			sendMsg.packetHeader.size = sizeof( sendMsg );

			memcpy( sendMsg.szUserID, user->m_userInfo.szID, MAX_USER_ID );

			SendPacket( (char *)&sendMsg );

		}
	}

	return RESULT_SUCCESS;
}

void XRoom::KickAll()
{

	if( m_roomInfo.roomAttr.usNumJoinedUser == 0 ) return;

	UI32 i;
	XUser *pUser[ 100 ];
	JWObjectPtr *p;

	UI32 count = m_pUserPtrList->GetNumberOfNodes();
	
	JWLIST_LOOP( i, m_pUserPtrList, JWObjectPtr, p )
	{
		pUser[ i ] = (XUser *)( p->GetObject() );
	}

	for( i = 0; i < count; ++i ) {

		m_pUserPtrList->Remove( pUser[ i ] );

		pUser[ i ]->SetRoom( NULL );

	}

	m_roomInfo.roomAttr.usNumJoinedUser = 0;
}

UI32 XRoom::CloseRoom()
{
	return m_pRoomMgr->CloseRoom( this );
}


void XRoom::SendPacket( char *pPacket )
{
	UI32 i;
	XUser *pUser;
	JWObjectPtr *p;

	JWLIST_LOOP( i, m_pUserPtrList, JWObjectPtr, p )
	{
		pUser = (XUser *)( p->GetObject() );
		pUser->SendPacket( pPacket );
		printf( "Send ChatMsg\n" );
	}	
}

void XRoom::SendChatMsg( sRequest_ChatMsg *pChatMsg, XUser *pUser )
{

	sNotify_ChatMsg *pSendMsg = (sNotify_ChatMsg *)m_pBuf;

	pSendMsg->usStrLength = pChatMsg->usStrLength;

	pSendMsg->packetHeader.command = NOTIFY_CHATMSG;
	pSendMsg->packetHeader.size = sizeof( sNotify_ChatMsg ) + pSendMsg->usStrLength;

	pSendMsg->usFontType = pChatMsg->usFontType;
	pSendMsg->usTextSize = pChatMsg->usTextSize;
	pSendMsg->usTextColor = pChatMsg->usTextColor;

	memcpy( pSendMsg->szUserID, pUser->m_userInfo.szID, MAX_USER_ID );

	memcpy( (char *)( pSendMsg ) + sizeof( sNotify_ChatMsg ), 
		    (char *)( pChatMsg ) + sizeof( sRequest_ChatMsg ),
			pChatMsg->usStrLength );

	SendPacket( (char *)pSendMsg );

	printf( "packet size : %d, str size : %d\n", pChatMsg->packetHeader.size, pChatMsg->usStrLength );
	printf( "ChatMsg : %s\n", (char *)( pSendMsg ) + sizeof( sNotify_ChatMsg ) );

}

// pMsg는 유효한 sRequest_Whisper형의 메모리 포인터로 가정한다
// (pMsg를 사용하여 sRequest_Whisper크기 범위에 있는 메모리를 쓰더라도 
// 잘못된 참조 오류를 발생 하지 않는다고 가정한다)
UI32 XRoom::SendWhisper( sRequest_Whisper *pMsg, XUser *pUser )
{
	XUser *pReceiver;

	pMsg->szReceiverID[ MAX_USER_ID - 1 ] = NULL;

	pReceiver = GetUserByID( pMsg->szReceiverID );

	if( pReceiver == NULL ) return RESULT_UNKNOWN_FAILED;

	sNotify_Whisper *pSendMsg = (sNotify_Whisper *)m_pBuf;

	pSendMsg->packetHeader.command = NOTIFY_WHISPER;
	pSendMsg->packetHeader.size = sizeof( sNotify_Whisper ) + pMsg->usStrLength;

	memcpy( pSendMsg->szSenderID, pUser->m_userInfo.szID, MAX_USER_ID );
	
	pSendMsg->usStrLength = pMsg->usStrLength;

	memcpy( ( char * )pSendMsg + sizeof(  sNotify_Whisper ), 
		    ( char * )pMsg + sizeof( sRequest_Whisper ),
			pMsg->usStrLength );


	pReceiver->SendPacket( (char *)pSendMsg );

	return RESULT_SUCCESS;

}

void XRoom::SendUserList( XUser *pUser )
{
	
	UI32 i;
	XUser *user;
	JWObjectPtr *p;

	sResponse_UserList *pSendMsg = (sResponse_UserList *)m_pBuf;

	pSendMsg->usCount = m_pUserPtrList->GetNumberOfNodes();
	pSendMsg->packetHeader.command = RESPONSE_USERLIST;
	pSendMsg->packetHeader.size = sizeof( sResponse_UserList ) + 
		sizeof( sUserViewInfo ) * pSendMsg->usCount;

	sUserViewInfo *pUserViewInfo = (sUserViewInfo *)( m_pBuf + sizeof( sResponse_UserList ) );

	JWLIST_LOOP( i, m_pUserPtrList, JWObjectPtr, p )
	{
		user = (XUser *)( p->GetObject() );

		user->GetUserViewInfo( pUserViewInfo );

		++pUserViewInfo;
	}	

	pUser->SendPacket( (char *)pSendMsg );

}

UI32 XRoom::ChangeRoomInfo( sRequest_ChangeRoomInfo *pMsg, XUser *pUser )
{
	if( pUser->IsRoomMaster() == FALSE ) return RESULT_NOT_ROOMMASTER;
	
	sNotify_RoomInfoChanged sendMsg;

	sendMsg.packetHeader.command = NOTIFY_ROOMINFO_CHANGED;
	sendMsg.packetHeader.size = sizeof( sendMsg );

	sendMsg.roomInfoChangable = pMsg->roomInfoChangable;

	DWORD dwFlag = pMsg->roomInfoChangable.dwFlag;

	if( dwFlag & CHANGE_ROOM_TITLE ) {
	
		pMsg->roomInfoChangable.szRoomTitle[ MAX_ROOM_TITLE - 1 ] = NULL;

		memcpy( m_roomInfo.szRoomTitle, 
			    pMsg->roomInfoChangable.szRoomTitle,
				MAX_ROOM_TITLE );
	}

	if( dwFlag & CHANGE_ROOM_ATTR ) {
		
		m_roomInfo.roomAttr.usNumCapacity = pMsg->roomInfoChangable.roomAttr.usNumCapacity;
		m_roomInfo.roomAttr.usRoomType = pMsg->roomInfoChangable.roomAttr.usRoomType;

	}

	if( dwFlag & CHANGE_ROOM_PASSWORD ) {

	pMsg->roomInfoChangable.szRoomPassword[ MAX_ROOM_PASSWORD - 1 ] = NULL;

		memcpy( m_roomInfo.szRoomPassword, 
			    pMsg->roomInfoChangable.szRoomPassword, 
				MAX_ROOM_PASSWORD );
	}

	SendPacket( (char *)&sendMsg );

	return RESULT_SUCCESS;
}

UI32 XRoom::ChangeRoomMaster( sRequest_ChangeRoomMaster *pMsg, XUser *pUser )
{

	if( pUser->IsRoomMaster() == FALSE ) return RESULT_NOT_ROOMMASTER;

	pMsg->szUserID[ MAX_USER_ID - 1 ] = NULL;

	XUser *user = GetUserByID( pMsg->szUserID );

	// 방에 존재 하지 않는 유저이면,
	if( user == NULL ) return RESULT_UNKNOWN_FAILED;

	user->SetUserType( USERTYPE_ROOMMASTER );
	pUser->SetUserType( USERTYPE_ROOMMEMBER );

	memcpy( m_roomInfo.roomMasterInfo.szID, user->m_userInfo.szID, MAX_USER_ID );
	memcpy( m_roomInfo.roomMasterInfo.szName, user->m_userInfo.szName, MAX_USER_NAME );
	
	sNotify_RoomMasterChanged sendMsg;

	sendMsg.packetHeader.command = NOTIFY_ROOMMASTER_CHANGED;
	sendMsg.packetHeader.size = sizeof( sendMsg );

	memcpy( sendMsg.szUserID, pMsg->szUserID, MAX_USER_ID );

	SendPacket( (char *)&sendMsg );

	return RESULT_SUCCESS;
}

UI32 XRoom::KickUserOut( sRequest_KickUserOut *pMsg, XUser *pUser )
{
	if( pUser->IsRoomMaster() == FALSE ) return RESULT_NOT_ROOMMASTER;
	
	pMsg->szUserID[ MAX_USER_ID - 1 ] = NULL;

	XUser *user = GetUserByID( pMsg->szUserID );

	if( user == NULL ) return RESULT_UNKNOWN_FAILED;

	UI32 result = DelUser( user, EXITCODE_KICKED );

	if( result != RESULT_SUCCESS ) return result;

	sNotify_KickedOut sendMsg;

	sendMsg.packetHeader.command = NOTIFY_KICKEDOUT;
	sendMsg.packetHeader.size = sizeof( sendMsg );

	user->SendPacket( (char *)&sendMsg );

	return RESULT_SUCCESS;

}

void XRoom::NotifyUserJoined( XUser *pUser )
{

	sNotify_UserJoined sendMsg;

	sendMsg.packetHeader.command = NOTIFY_USER_JOINED;
	sendMsg.packetHeader.size = sizeof( sendMsg );

	memcpy( sendMsg.userViewInfo.szID, pUser->m_userInfo.szID, MAX_USER_ID );
	memcpy( sendMsg.userViewInfo.szName, pUser->m_userInfo.szName, MAX_USER_NAME );

	SendPacket( (char *)&sendMsg );
}

void XRoom::NotifyUserExited( XUser *pUser, UI16 usExitCode )
{
	sNotify_UserExited sendMsg;

	sendMsg.packetHeader.command = NOTIFY_USER_EXITED;
	sendMsg.packetHeader.size = sizeof( sendMsg );

	memcpy( sendMsg.szUserID, pUser->m_userInfo.szID, MAX_USER_ID );

	sendMsg.usExitCode = usExitCode;

	SendPacket( (char *)&sendMsg );
}

void XRoom::GetRoomViewInfo( sRoomViewInfo *pRoomViewInfo )
{
	pRoomViewInfo->usRoomNum = m_roomInfo.usRoomNum;
	pRoomViewInfo->roomAttr = m_roomInfo.roomAttr;
	pRoomViewInfo->roomMasterInfo = m_roomInfo.roomMasterInfo;

	memcpy( pRoomViewInfo->szRoomTitle, m_roomInfo.szRoomTitle, MAX_ROOM_TITLE );

	printf( "RoomViewInfo RoomType : %d\n", pRoomViewInfo->roomAttr.usRoomType  );
}

XUser* XRoom::GetUserByID( char *szUserID )
{

	UI32 i;
	XUser *pUser;
	JWObjectPtr *p;

	JWLIST_LOOP( i, m_pUserPtrList, JWObjectPtr, p )
	{
		pUser = (XUser *)( p->GetObject() );

		if( strcmp( pUser->m_userInfo.szID, szUserID ) == 0 ) return pUser;
	}	

	return NULL;

}

XUser* XRoom::GetUserByName( char *szUserName )
{

	UI32 i;
	XUser *pUser;
	JWObjectPtr *p;

	JWLIST_LOOP( i, m_pUserPtrList, JWObjectPtr, p )
	{
		pUser = (XUser *)( p->GetObject() );

		if( strcmp( pUser->m_userInfo.szName, szUserName ) == 0 ) return pUser;
	}	

	return NULL;

}
