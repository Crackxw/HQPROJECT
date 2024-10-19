// Latest Update 2002/6/26, 10:13 

#include "SOMain.h"
#include <onlinebattlemsg.h>
#include <Onlineimjin2I.h>
#include "SOBaseChar.h"
#include "SOPlayer.h"
#include "Game.h"
#include "Element.h"
#include "Battle.h"



#define ISUSER(x) ISPLAYER(x)


//-----------------------------------------------------------------------------------------------------------------------
//	Name : CBattle()
//	Desc :
//-----------------------------------------------------------------------------------------------------------------------
CBattle::CBattle()
{
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : ~CBattle()
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
CBattle::~CBattle()
{
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : ResetPlayerList()
//	Desc : 플레이어 리스트를 초기화 한다 
//-----------------------------------------------------------------------------------------------------------------------
void CBattle::ResetPlayerList()
{	
	for(int i=0; i < MAX_PLAYER; ++i) 
	{
		SetPlayer(i, NULL);
	}
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : InitBattleProperty()
//	Desc : 전투의 관련 변수들을 초기화 한다 
//-----------------------------------------------------------------------------------------------------------------------
void CBattle::InitBattleProperty()
{
	SetState(READY);

	ResetPlayerList();
		
	m_numPlayer			 =	0;
	m_numUser			 =	0;
	m_LatestAddUserTime  =	timeGetTime();
	m_bSingleMode		 =	FALSE;	
}





//-----------------------------------------------------------------------------------------------------------------------
//	Name : AddPlayer()
//	Desc : 전투에 플레이어를 추가한다 
//-----------------------------------------------------------------------------------------------------------------------
bool CBattle::AddPlayer(BaseChar* player)
{
	for(int i=0; i < MAX_PLAYER; ++i)
	{		
		if(GetPlayer(i) == NULL)		
		{				
			SetPlayer(i, player);			
			m_LatestAddUserTime = timeGetTime();

			
			m_numPlayer++;					
			if(ISUSER(GetPlayer(i)->uiAccount))	m_numUser++;					
			//printf("%d 전투에 플레이어가 추가 되었습니다. (총 %d:%d 명)\n", GetKey(), m_numPlayer, m_numUser);

			return TRUE;
		}
	}

	return FALSE;
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : RemovePlayer()
//	Desc : 전투에서 플레이어를 제거한다 
//-----------------------------------------------------------------------------------------------------------------------
int CBattle::RemovePlayer(BaseChar* player)
{
	for(int i=0; i < MAX_PLAYER; ++i)
	{		
		if(GetPlayer(i) == player)
		{													
			m_numPlayer--;
			if(ISUSER(GetPlayer(i)->uiAccount))	m_numUser--;											
			//printf("%d 전투에 플레이어가 삭제 되었습니다. (총 %d:%d 명)\n", GetKey(), m_numPlayer, m_numUser);			
						
			SetPlayer(i, NULL);						
			
			// 전투에 유저가 아무도 남지 않았을 경우 
			if(m_numUser == 0) 
				return -1;	// 전투 삭제를 알린다 
			

			return i;
		}
	}

	return -2; 
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : SetPlayer()
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
void CBattle::SetPlayer(int index, BaseChar* player)
{
	m_PlayerList[index] = player;
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : GetPlayer()
//	Desc : 해당 인덱스의 플레이어를 얻는다 
//-----------------------------------------------------------------------------------------------------------------------
BaseChar* CBattle::GetPlayer(int index)
{
	if(index < 0 || index >= MAX_PLAYER) return NULL;
	return m_PlayerList[index];
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : GetFollower()
//	Desc : 해당 인덱스와 용병슬롯의 용병을 얻는다 
//-----------------------------------------------------------------------------------------------------------------------
OnlineFollower* CBattle::GetFollower(int playerindex, int followerindex)
{
	if(GetPlayer(playerindex))
	{
		return GetPlayer(playerindex)->IFollower.GetFollower(followerindex);
	}

	return NULL;
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : GetFollowerParam()
//	Desc : 해당 용병의 파라미터 정보를 얻는다 
//-----------------------------------------------------------------------------------------------------------------------
_ImportantParameta* CBattle::GetFollowerParam(int playerindex, int followerindex)
{
	if(GetFollower(playerindex,followerindex))
	{
		return &GetFollower(playerindex,followerindex)->ParametaBox.IP;
	}

	return NULL;	
}





//-----------------------------------------------------------------------------------------------------------------------
//	Name : GetBattleInfo()
//	Desc : 전투에 참여한 유저들에게 보낼 전투 정보를 얻어온다 
//-----------------------------------------------------------------------------------------------------------------------
int CBattle::GetBattleInfo(char* data)
{		
	REPORT_INITBATTLE*		  packet = (REPORT_INITBATTLE*)data; 	
	OnImjinBattlePlayerInfo*  OIBPI  = packet->OIBPI;
	
	
	packet->command	     = ON_RESPONSE_BATTLE;
	packet->subcommand   = ON_RET_BATTLE_BEGIN;

	packet->randomSeed	 = (WORD)(max(timeGetTime() % 1000, 1));
	packet->isSingleMode = IsSingleMode();
	packet->numPlayer	 = m_numPlayer;
	
	
	//----------------------------------------------------------------------------
	int count = 0;
	for(int i=0; i < MAX_PLAYER; ++i)
	{	
		if(GetPlayer(i) != NULL)
		{
			// 1. 플레이어 정보를 설정 한다 			
			//--------------------------------------------------------------------
			OIBPI[count].siPlayersUniqueID	= GetPlayer(i)->uiAccount;
											
			if(ISUSER(GetPlayer(i)->uiAccount)){			
				OIBPI[count].GuildCode	= ((SOPlayer*)GetPlayer(i))->uiGuildID;				
				OIBPI[count].bPlayer	= TRUE;
			}

			else {
				OIBPI[count].GuildCode	= 0;
				OIBPI[count].bPlayer	= FALSE;
			}


			// 2. 플레이어가 가진 용병들의 정보를 설정한다 
			//--------------------------------------------------------------------
			GetBattleFollowerInfo(&OIBPI[count], i);

			++count;
		}		
	}



	return 32 + (sizeof(OnImjinBattlePlayerInfo) * packet->numPlayer);		
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : GetBattleFollowerInfo()
//	Desc : 전투에 참여하는 용병들의 정보를 얻어온다 
//-----------------------------------------------------------------------------------------------------------------------
void CBattle::GetBattleFollowerInfo( OnImjinBattlePlayerInfo* OIBPI, int playerindex )
{
	for(int i=0; i < MAX_FOLLOWER; ++i)
	{
		OnlineFollower* follower = GetFollower(playerindex, i);
		
		if(follower != NULL)
		{
			memcpy(&OIBPI->Iparameta[i]		, &follower->ParametaBox.IP, sizeof(_ImportantParameta));
			memcpy(&OIBPI->MW[i]			, &follower->Weapon,		 sizeof(MyWeapons));
			memcpy(&OIBPI->uiKind[i]		, &follower->uiCharID,		 sizeof(UI16));					
			memcpy(&OIBPI->szPlayerName[i]	, &follower->Name,			 ON_ID_LENGTH + 1);						
		}
	}			
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : SendBattleInfoToAll()
//	Desc : 전투 정보를 모두에게 보낸다 
//-----------------------------------------------------------------------------------------------------------------------
void CBattle::SendBattleInfoToAll()
{
	REPORT_INITBATTLE		 packet;
	memset(&packet, 0, sizeof(packet));

	int size = GetBattleInfo((char*)&packet);
		
	for(int i=0; i < MAX_PLAYER; ++i)
	{						
		if( GetPlayer(i) && ISUSER(GetPlayer(i)->uiAccount) )
		{	
			// 유저의 소지금 설정  
			packet.userMoney = ((SOPlayer*)GetPlayer(i))->siMoney;		
			((SOPlayer*)GetPlayer(i))->Write(&packet, size);			
		}		
	}		
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : SendToAll()
//	Desc : 전투에 참여한 모든 유저에게 데이타 전송 
//-----------------------------------------------------------------------------------------------------------------------
void CBattle::SendToAll(char* data, int size)
{
	for(int i=0; i < MAX_PLAYER; ++i)
	{
		if(GetPlayer(i) != NULL)
		{
			if( ISUSER(GetPlayer(i)->uiAccount) )
			{
				((SOPlayer*)GetPlayer(i))->Write(data, size);
			}
		}
	}
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : IsSingleMode()
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
bool CBattle::IsSingleMode()
{
	return m_bSingleMode;
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : IsJoinable()
//	Desc : 전투에 참여할수 있는지를 알아본다 
//-----------------------------------------------------------------------------------------------------------------------
bool CBattle::IsJoinable()
{
	if(GetState() == READY && m_numPlayer < MAX_PLAYER) return TRUE;
	return FALSE;
}





//-----------------------------------------------------------------------------------------------------------------------
//	Name : PushData()
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
void CBattle::PushData(char* data, int size)
{
	if( GetState() == ACTION )
	{
		m_inGame.PushData(data, size);
	}
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : Action()
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
void CBattle::Action()
{
	if( GetState() == ACTION )
	{		
		m_inGame.Action();
	}

	else
	{
		// 마지막으로 유저가 추가되고 나서 5초후에 시작한다 (동기를 생각해 약간 일찍 시작한다)
		if( (timeGetTime() - m_LatestAddUserTime) > 4500 )
		{			
			PrepareBattle();
			LaunchBattle();			
		}
	}
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : GetState()
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
int CBattle::GetState()
{
	return m_State;
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : SetState()
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
void CBattle::SetState(int state)
{
	m_State = state;	
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : LaunchBattle()
//	Desc : 전투를 시작시킨다 
//-----------------------------------------------------------------------------------------------------------------------
void CBattle::LaunchBattle()
{		
	m_bSingleMode = (m_numUser == 1) ? TRUE : FALSE;		
	m_inGame.Init(this);					
	SendBattleInfoToAll();

	SetState(ACTION);
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : CleanupBattle()
//	Desc : 전투를 종료 시킬때 남아있는 유저나 몬스터를 필드로 돌려 보낸다 
//-----------------------------------------------------------------------------------------------------------------------
void CBattle::CleanupBattle()
{
	for(int i=0; i < MAX_PLAYER; ++i)
	{
		if(GetPlayer(i))
		{						
			if(GetState() == READY)
				GetPlayer(i)->CancelBattle();

			else if(GetState() == ACTION)
				GetPlayer(i)->EndBattle();
		}
	}
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : PrepareBattle()
//	Desc : 전투를 시작하기전에, 참여한 유저나 몬스터를 준비시킨다 
//-----------------------------------------------------------------------------------------------------------------------
void CBattle::PrepareBattle()
{
	for(int i=0; i < MAX_PLAYER; ++i)
	{
		if(GetPlayer(i))
		{
			GetPlayer(i)->PrepareBattle();
		}
	}
}

VOID	CBattle::SetPartyID( UI16 uiPartyID )
{
	m_uiPartyID		=	uiPartyID;
}

UI16	CBattle::GetPartyID()
{
	return m_uiPartyID;
}
							