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
//	Desc : �÷��̾� ����Ʈ�� �ʱ�ȭ �Ѵ� 
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
//	Desc : ������ ���� �������� �ʱ�ȭ �Ѵ� 
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
//	Desc : ������ �÷��̾ �߰��Ѵ� 
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
			//printf("%d ������ �÷��̾ �߰� �Ǿ����ϴ�. (�� %d:%d ��)\n", GetKey(), m_numPlayer, m_numUser);

			return TRUE;
		}
	}

	return FALSE;
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : RemovePlayer()
//	Desc : �������� �÷��̾ �����Ѵ� 
//-----------------------------------------------------------------------------------------------------------------------
int CBattle::RemovePlayer(BaseChar* player)
{
	for(int i=0; i < MAX_PLAYER; ++i)
	{		
		if(GetPlayer(i) == player)
		{													
			m_numPlayer--;
			if(ISUSER(GetPlayer(i)->uiAccount))	m_numUser--;											
			//printf("%d ������ �÷��̾ ���� �Ǿ����ϴ�. (�� %d:%d ��)\n", GetKey(), m_numPlayer, m_numUser);			
						
			SetPlayer(i, NULL);						
			
			// ������ ������ �ƹ��� ���� �ʾ��� ��� 
			if(m_numUser == 0) 
				return -1;	// ���� ������ �˸��� 
			

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
//	Desc : �ش� �ε����� �÷��̾ ��´� 
//-----------------------------------------------------------------------------------------------------------------------
BaseChar* CBattle::GetPlayer(int index)
{
	if(index < 0 || index >= MAX_PLAYER) return NULL;
	return m_PlayerList[index];
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : GetFollower()
//	Desc : �ش� �ε����� �뺴������ �뺴�� ��´� 
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
//	Desc : �ش� �뺴�� �Ķ���� ������ ��´� 
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
//	Desc : ������ ������ �����鿡�� ���� ���� ������ ���´� 
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
			// 1. �÷��̾� ������ ���� �Ѵ� 			
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


			// 2. �÷��̾ ���� �뺴���� ������ �����Ѵ� 
			//--------------------------------------------------------------------
			GetBattleFollowerInfo(&OIBPI[count], i);

			++count;
		}		
	}



	return 32 + (sizeof(OnImjinBattlePlayerInfo) * packet->numPlayer);		
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : GetBattleFollowerInfo()
//	Desc : ������ �����ϴ� �뺴���� ������ ���´� 
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
//	Desc : ���� ������ ��ο��� ������ 
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
			// ������ ������ ����  
			packet.userMoney = ((SOPlayer*)GetPlayer(i))->siMoney;		
			((SOPlayer*)GetPlayer(i))->Write(&packet, size);			
		}		
	}		
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : SendToAll()
//	Desc : ������ ������ ��� �������� ����Ÿ ���� 
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
//	Desc : ������ �����Ҽ� �ִ����� �˾ƺ��� 
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
		// ���������� ������ �߰��ǰ� ���� 5���Ŀ� �����Ѵ� (���⸦ ������ �ణ ���� �����Ѵ�)
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
//	Desc : ������ ���۽�Ų�� 
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
//	Desc : ������ ���� ��ų�� �����ִ� ������ ���͸� �ʵ�� ���� ������ 
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
//	Desc : ������ �����ϱ�����, ������ ������ ���͸� �غ��Ų�� 
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
							