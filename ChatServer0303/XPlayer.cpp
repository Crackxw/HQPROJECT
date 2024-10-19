//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 7		
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#include "XPlayer.h"

#include "../JWCommon/JWSocket.h"

XPlayer::XPlayer()
{
	m_pCell = NULL;
	m_usMapCode = 0;
}

XPlayer::~XPlayer()
{

}

JWSocket* XPlayer::GetSocket() 
{ 
	return (JWSocket *)m_pFriend; 
}

void XPlayer::Init()
{
	m_uiAccount = 0;
	m_uiUserGrade = ON_USERGRADE_NORMAL;
	m_usMapCode = 0;
	m_szID[ 0 ] = 0;
	m_pCell = NULL;

	m_lastTick = GetTickCount();

	ResetAction();
}

void XPlayer::Close()
{
	m_uiAccount = 0;
	m_uiUserGrade = ON_USERGRADE_NORMAL;

	ResetAction();
}
