// Latest Update 2002/6/26, 10:13 

#include "SOMain.h"
#include <onlinebattlemsg.h>
#include "SOBaseChar.h"
#include "SOPlayer.h"
#include "Game.h"
#include "Element.h"
#include "Battle.h"
#include "BattleMgr.h"
//-----------------------------------------------------------------------------------------------------------------------
//	Name : CBattleMgr()
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
CBattleMgr::CBattleMgr()
{	
	for(int i=0; i < MAX_BATTLE; ++i)
	{
		m_BattleList[i] = new CBattle();
		if(m_BattleList[i] == NULL)
		{
			printf("Not enough Memory!\n");
		}
	}	

	m_numBattle = 0;
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : ~CBattleMgr()
//	Desc :  
//-----------------------------------------------------------------------------------------------------------------------
CBattleMgr::~CBattleMgr()
{	
	for(int i=0; i < MAX_BATTLE; ++i)
	{
		if(m_BattleList[i] != NULL) 		
			delete m_BattleList[i];		
	}
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : GetBattle()
//	Desc :  
//-----------------------------------------------------------------------------------------------------------------------
CBattle* CBattleMgr::GetBattle(int key)
{
	if(key < 0 || key >= MAX_BATTLE) return NULL;
	return m_BattleList[key];
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : CreateBattle()
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
int CBattleMgr::CreateBattle( UI16 uiPartyID )
{
	for(int i=0; i < MAX_BATTLE; ++i)
	{		
		if(GetBattle(i)->GetUsed() == FALSE)
		{			
			GetBattle(i)->InitBattleProperty();			
			GetBattle(i)->SetKey(i);			
			GetBattle(i)->SetUsed(TRUE);
			GetBattle(i)->SetPartyID( uiPartyID );

			
			m_numBattle++;
			//printf("%d 전투가 생성 되었습니다. (총 %d 개)\n", i, m_numBattle);
			
			return i;
		}
	}

	return -1;
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : DestoryBattle()
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
void CBattleMgr::DestroyBattle(int key)
{
	if(key < 0 || key >= MAX_BATTLE) return;
	
	GetBattle(key)->CleanupBattle();
	GetBattle(key)->SetUsed(FALSE);
	
	m_numBattle--;	
	//printf("%d 전투가 삭제 되었습니다. (총 %d 개)\n", key, m_numBattle);	
}




//-----------------------------------------------------------------------------------------------------------------------
//	Name : ActionPoll()
//	Desc : 매 루프 마다 생성된 전투들을 Action을 수행한다 
//-----------------------------------------------------------------------------------------------------------------------
void CBattleMgr::ActionPoll()
{
	for(int i=0; i < MAX_BATTLE; ++i)
	{
		if(GetBattle(i)->GetUsed() == TRUE)
		{
			GetBattle(i)->Action();
		}
	}
}

