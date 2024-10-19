//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 7
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#include "XPlayer.h"
#include "..\\XCommon\\JXSocket.h"

XPlayer::XPlayer()
{
	SetSocket( NULL );
	m_pCell = NULL;
	m_usMapCode = 0;
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
	m_uiAccount = 0;
	m_usMapCode = 0;
	m_pCell = NULL;
}


void XPlayer::SetSocket( JXSocket *pSocket )
{ 

	m_pCell = NULL;
	m_uiAccount = 0; 
	m_pSocket = pSocket; 

	SOCKADDR_IN		saddr;
	int addrlen;

	addrlen = sizeof( saddr );
	
	if( pSocket ) {

		getpeername( pSocket->GetSocket(), (SOCKADDR *)&saddr, &addrlen );

		strcpy( m_szIP, inet_ntoa( saddr.sin_addr ) );
	}

}




