//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 7
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#include "XPlayer.h"
#include "..\\XCommon\\JXSocket.h"

XPlayer::XPlayer()
{
	SetSocket( NULL );
}

XPlayer::~XPlayer()
{

}

JXSocket* XPlayer::GetSocket() 
{ 
	return m_pSocket; 
}

void XPlayer::Init()
{
	m_usAccount = 0;
	m_bRequestAccount = FALSE;
	m_bSendLogOutToGameServer = FALSE;
}

void XPlayer::SetAccount( UI16 usAccount )
{
	if( m_pSocket == NULL ) return;

	m_usAccount = usAccount;

	m_bSendLogOutToGameServer = FALSE;

	char buf[ 512 ];
/*
	/// debug ////////////////////////

	if( strcmp( m_szIP, "211.62.119.141" ) == 0 ) {
	
		sprintf( buf, "Set Account...%d\n", m_usAccount );

		OutputDebugString( buf );
	}

	///////////////////////////////////
*/

}

void XPlayer::SetSocket( JXSocket *pSocket )
{ 

	m_usAccount = 0; 
	m_bRequestAccount = FALSE;
	m_pSocket = pSocket; 
	m_bSendLogOutToGameServer = FALSE;

	SOCKADDR_IN		saddr;
	int addrlen;

	addrlen = sizeof( saddr );
	
	if( pSocket ) {

		getpeername( pSocket->GetSocket(), (SOCKADDR *)&saddr, &addrlen );

		strcpy( m_szIP, inet_ntoa( saddr.sin_addr ) );
	}

}




