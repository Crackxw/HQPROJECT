#include "XUser.h"
#include "XRoom.h"

#include "../JWCommon/JWSocket.h"
#include "../JWCommon/JWSendBuffer.h"

XUser::XUser()
{

}

XUser::~XUser()
{

}

void XUser::Init()
{
	m_bLogIn = FALSE;
	m_pRoom = NULL;

	ZeroMemory( &m_userInfo, sizeof( m_userInfo ) );

}

void XUser::Close()
{
	m_bLogIn = FALSE;
	m_pRoom = NULL;

	ZeroMemory( &m_userInfo, sizeof( m_userInfo ) );
}


UI32 XUser::LogIn( sRequest_LogIn *pLogin )
{
	
	memcpy( m_userInfo.szID, pLogin->szID, MAX_USER_ID ); 
	m_userInfo.szID[ MAX_USER_ID - 1 ] = NULL;

	memcpy( m_userInfo.szName, pLogin->szName, MAX_USER_NAME );
	m_userInfo.szName[ MAX_USER_NAME - 1 ] = NULL;

	printf( "Login..\n" );
	printf( "ID : %s\n", m_userInfo.szID );
	printf( "Name : %s\n", m_userInfo.szName );

	m_bLogIn = TRUE;

	return RESULT_SUCCESS;
}

void XUser::GetUserViewInfo( sUserViewInfo *pUserViewInfo )
{
	memcpy( pUserViewInfo->szID, m_userInfo.szID, MAX_USER_ID );
	memcpy( pUserViewInfo->szName, m_userInfo.szName, MAX_USER_NAME );

	pUserViewInfo->usUserType = m_userInfo.usUserType;
}

void XUser::SendPacket( char *pPacket )
{
	((JWSocket *)GetFriend())->GetSendBuffer()->Write( pPacket );
}


void XUser::SendChatMsg( sRequest_ChatMsg *pChatMsg )
{
	if( m_pRoom == NULL ) return;
	
	m_pRoom->SendChatMsg( pChatMsg, this );
}

