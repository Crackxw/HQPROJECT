#include "SOMain.h"
#include "SOWorld.h"
#include "Game.h"
#include "Element.h"
#include "Battle.h"
#include "BattleMgr.h"
#include "SOBaseChar.h"
#include "GSCDefine.h"




//--------------------------------------------------------------------------------------------
//	Name : PrepareBattle()
//	Desc : 
//--------------------------------------------------------------------------------------------
VOID	BaseChar::PrepareBattle()
{	
	if(DelIDOnMap( 12 ) == FALSE)
	{
		if( ISPLAYER( uiAccount ) == TRUE )
		{
			SOPlayer	*pPlayer;

			pPlayer		=	( SOPlayer* )this;
		
//			pMyWorld->clServer->WriteInfo("..\\DelIDOnMap Failed.txt", "1 [%u] [%d] [%d] [%d] [%d] [%d] [%d]", uiAccount, siStatus, m_battleKey, uiMapID, siX, siY, pPlayer->bSuccessLogin);
			writeInfoToFile("DelIDOnMap Failed.txt", "1 [%u] [%d] [%d] [%d] [%d] [%d] [%d]", uiAccount, siStatus, m_battleKey, uiMapID, siX, siY, pPlayer->bSuccessLogin);
		}
		else
		{
//			pMyWorld->clServer->WriteInfo("..\\DelIDOnMap Failed.txt", "1 [%u] [%d] [%d] [%d] [%d] [%d]", uiAccount, siStatus, m_battleKey, uiMapID, siX, siY);
			writeInfoToFile("DelIDOnMap Failed.txt", "1 [%u] [%d] [%d] [%d] [%d] [%d]", uiAccount, siStatus, m_battleKey, uiMapID, siX, siY);
		}		
	}

	SETSTATUS( siStatus, ON_PLAYER_STATUS_INBATTLE );	
}




//--------------------------------------------------------------------------------------------
//	Name : EndBattle()
//	Desc : 
//--------------------------------------------------------------------------------------------
VOID	BaseChar::EndBattle()
{	
	SetBattleKey(-1);

	SETSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD );	
}



//--------------------------------------------------------------------------------------------
//	Name : CancelBattle()
//	Desc : 
//--------------------------------------------------------------------------------------------
VOID	BaseChar::CancelBattle()
{	
	SetBattleKey(-1);
	SendFieldMsg(ON_RESPONSE_BATTLEPLAYER, SO_SFM_MEANDNEARCHAR, LPVOID(0));
}

//--------------------------------------------------------------------------------------------
//	Name : GetBaseCharFromID()
//	Desc : id(account)에 해당하는 BaseChar 클래스를 얻는다 
//--------------------------------------------------------------------------------------------
BaseChar*  BaseChar::GetBaseCharFromID(int id)
{
	BaseChar* pBaseChar= NULL;
		
	if(ISPLAYER(id))
	{
		pBaseChar = (BaseChar*)pMyWorld->pPlayerOnAccount[id];
	}	
	else if(ISMONSTER(id))
	{
		pBaseChar = (BaseChar*)pMyWorld->clMonster.GetMonster(id);
	}	
	
	return pBaseChar;
}



//--------------------------------------------------------------------------------------------
//	Name : IsAdminPlayer()
//	Desc : id에 해당하는 플레이어가 운영자인가?
//--------------------------------------------------------------------------------------------
bool BaseChar::IsAdminPlayer(int id)
{	
	if( (ISPLAYER(id) == TRUE) )
	{	
		if( ((SOPlayer *)GetBaseCharFromID(id))->USGrade != ON_USERGRADE_NORMAL )
		{		
			return TRUE;
		}		
	}
	
	return FALSE;
}



//--------------------------------------------------------------------------------------------
//	Name : AddToBattle()
//	Desc : 키에 해당하는 전투에 자기 자신을 추가한다 
//--------------------------------------------------------------------------------------------
bool BaseChar::AddToBattle(int battleKey)
{
	SetBattleKey(battleKey);	
	
	if(GetBattle()->AddPlayer(this))
	{
		// 전투에 추가 되었다는것을 클라이언트에 알린다 
		SendFieldMsg(ON_RESPONSE_BATTLEPLAYER, SO_SFM_MEANDNEARCHAR, LPVOID(1));	
		return TRUE;
	}

	return FALSE;
}



//--------------------------------------------------------------------------------------------
//	Name : CheckJustBattle()
//	Desc : PK일 경우 정당한지를 검사해 아닐 경우 페널티를 적용한다
//--------------------------------------------------------------------------------------------
void BaseChar::CheckJustBattle(int enemyID)
{				
	if( ISMONSTER(enemyID) ) return;
    
	BaseChar*	assaulter = this;
	BaseChar*   victim	  = (BaseChar*)pMyWorld->pPlayerOnAccount[enemyID];

	if( assaulter == NULL || victim == NULL ) return;



	// 1. PK를 인정하는 지역이면 페널티를 적용하지 않는다 
	//-----------------------------------------------------------------------------------
	if( pMyWorld->clISOMap.IsWarFieldMap( assaulter->uiMapID ) ) return;
	if( pMyWorld->clISOMap.IsWarFieldMap( victim->uiMapID ) ) return;


	// 2. 전투의 가해자 와 피해자의 전투력을 비교한다 (둘다 유저 일경우)
	//-----------------------------------------------------------------------------------
	if( ISPLAYER(assaulter->uiAccount) && ISPLAYER(victim->uiAccount) )
	{

		// BP: BattlePoint (전투력)

		int assulterBP  = assaulter->IFollower.GetFollowerParameterBattlePower();
		int victimBP    = victim->IFollower.GetFollowerParameterBattlePower();
		

		if( assulterBP > victimBP )
		{
			int differBP = assulterBP - victimBP;
			int cutPoint = differBP * 0.10;



			// 3. 모든 용병(현재는 주인공만) 의 경험치를 전투력 차이의 10% 만큼 삭감 한다 
			//-----------------------------------------------------------------------------------
			for(int i=0; i < 1; ++i)
			{
				if(assaulter->IFollower.GetFollower(i))
				{
					_ImportantParameta* pFollowerIP	= &assaulter->IFollower.GetFollower(i)->ParametaBox.IP;
										
					pFollowerIP->IncreaseExp( -cutPoint );				
				}
			}
		}

	}


}



//--------------------------------------------------------------------------------------------
//	Name : IsInShip()
//	Desc : 
//--------------------------------------------------------------------------------------------
bool BaseChar::IsInShip(int id)
{
	if( ISPLAYER( id ) && 
	    (SOPlayer*)pMyWorld->GetPlayer(id)->uiBoardShipAccount != 0 ) return TRUE; 

	return FALSE;
}



//--------------------------------------------------------------------------------------------
//	Name : PickBattleWithEnemy()
//	Desc : 적에게 전투를 건다
//--------------------------------------------------------------------------------------------
bool BaseChar::PickBattleWithEnemy(int enemyID)
{		
	UI16	uiPartyID = INITIAL_ERROR_VALUE;

	if( GetBattle() != NULL ) return FALSE;	

	
	// 1. 전투를 건 대상이 유효한가?
	//---------------------------------------------------------------------------------------
	BaseChar* pEnemy = GetBaseCharFromID(enemyID);		
	if( pEnemy == NULL ) return FALSE;	


	// -- 내가 필드에 있거나 적이 필드에 있을때에만 전투를 할 수 있다.	
	if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD ) == FALSE  ||
		CHECKSTATUS( pEnemy->siStatus, ON_PLAYER_STATUS_INFIELD ) == FALSE )	return FALSE;		


	// -- 적이나 자신이 배에 타고 있는지 알아 본다 
	if( IsInShip( uiAccount ) || IsInShip( enemyID ) )							return FALSE;
	
	// 나와 상대가 모두 플레이어인 경우 둘중 하나라도 평화모드이면 실패 처리
	if( ISPLAYER(uiAccount) && ISPLAYER(enemyID) )
	{
		if( GetAttackMode() == FALSE || pEnemy->GetAttackMode() == FALSE )		return FALSE;
	}

	// -- 적이 운영자인 경우 
	//if( IsAdminPlayer(uiEnemyID) ) return FALSE;	




	// 2. 적이 이미 전투 대기 중이라면 적이 대기중인 전투에 참여하고 그렇지 않으면 
	//    새로운 전투를 생성한다 
	//---------------------------------------------------------------------------------------
	if( pEnemy->GetBattle() )
	{				
		// 심의 기간 동안 추가. 양정모 2003/07/29 10:21
		
		// 무도장이 아니면 사용자는 만들어진 전투에 참여할 수 없다.
//		if( ( pMyWorld->clISOMap.IsWarFieldMap( uiMapID ) == FALSE ) && ( ISPLAYER( uiAccount ) == TRUE ) )			return	FALSE;

		// 이벤트(토너먼트)중에는 만들어진 전투에 참여할 수 없다. (1:1만 가능)
		if( IsEvent() )		return FALSE;

//		if( pEnemy->GetBattle()->GetPartyID() == 0 )		return	FALSE;

		
		// 사용자일 경우에는 파티가 있고, 기존 전투에 참가할 수 있는 파티원인 경우에만
		// 들어갈 수 있다.
		if( pMyWorld->clISOMap.IsWarFieldMap( uiMapID ) == TRUE )
		{
			// 무도장이다.
			// 무도장은 파티에 관계없이 모든 유저가 전투에 참가할 수 있다.

		}
		else
		{
			// 무도장이 아니다.
			// 무도장이 아니면 파티원인 경우에만 전투에 참가할 수 있다.

			if( ISPLAYER( uiAccount ) == TRUE )
			{
				SOPlayer	*pPlayer	=	( SOPlayer* ) this;

				if( pEnemy->GetBattle()->GetPartyID() == INITIAL_ERROR_VALUE )		return	FALSE;

				if( pPlayer->GetPartyId() == INITIAL_ERROR_VALUE )					return	FALSE;
				
				if( pEnemy->GetBattle()->GetPartyID() != 
					pPlayer->GetPartyId() )											return	FALSE;

			}		
		}

		if(!pEnemy->GetBattle()->IsJoinable())				return	FALSE;
		if(!AddToBattle(pEnemy->GetBattleKey()))			return	FALSE;				
	}
	else
	{ 						
		if( ISPLAYER( uiAccount ) == TRUE )
		{
			SOPlayer	*pPlayer	=	( SOPlayer* ) this;

			uiPartyID	=	pPlayer->GetPartyId();
		}

		int key = GetBattleMgr()->CreateBattle( uiPartyID );				
		if(key == -1) return FALSE;
				
		if(!AddToBattle(key)) return FALSE;				
		pEnemy->AddToBattle(key);				
	}			


	// 3. PK일 경우 그 정당성을 알아보고 페널티를 적용한다 
    //---------------------------------------------------------------------------------------		
	CheckJustBattle(enemyID);

	return TRUE;
}
