//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 8
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#include "XPlayer.h"

#include "..\\JWCommon\\JWRecvBuffer.h"
#include "..\\JWCommon\\JWSendBuffer.h"
#include "..\\JWCommon\\JWSocket.h"
#include "SOServer.h"

#include "CSeed.h"

XPlayer::XPlayer()
{
}

XPlayer::~XPlayer()
{
}

void XPlayer::Init()
{

	//printf( "XPlayer::Init\n" );

	m_cKey1 = (char)( rand() % 199 ) + 32;
	m_cKey2 = (char)( rand() % 199 ) + 32;

	//플레이어의 라운드키를 세팅한다. 2004-12-07-최병주
	BYTE userKey = rand() % 256;
	CSeed::GetInstance()->SeedEncRoundKey(m_dwRoundKey, &userKey);

	ResetAction();

	m_pServer->NewPlayer( this );
}

void XPlayer::Close()
{
	m_pServer->__LogOutPlayer( this );
}

void XPlayer::Write( char *pPacket )
{
	JWSocket *pSocket = (JWSocket *)m_pFriend;

	if(pSocket->GetSendBuffer()->Write( pPacket ) == FALSE)
	{
		char szTemp[1024] = {0,};
		sprintf(szTemp, "[Packet WriteFaild!!][Account:%d][SocketNum:%d]\n", m_usAccount, pSocket->GetSocket());
		OutputDebugString(szTemp);		
	}
}

