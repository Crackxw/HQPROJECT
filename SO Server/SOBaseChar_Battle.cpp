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
//	Desc : id(account)�� �ش��ϴ� BaseChar Ŭ������ ��´� 
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
//	Desc : id�� �ش��ϴ� �÷��̾ ����ΰ�?
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
//	Desc : Ű�� �ش��ϴ� ������ �ڱ� �ڽ��� �߰��Ѵ� 
//--------------------------------------------------------------------------------------------
bool BaseChar::AddToBattle(int battleKey)
{
	SetBattleKey(battleKey);	
	
	if(GetBattle()->AddPlayer(this))
	{
		// ������ �߰� �Ǿ��ٴ°��� Ŭ���̾�Ʈ�� �˸��� 
		SendFieldMsg(ON_RESPONSE_BATTLEPLAYER, SO_SFM_MEANDNEARCHAR, LPVOID(1));	
		return TRUE;
	}

	return FALSE;
}



//--------------------------------------------------------------------------------------------
//	Name : CheckJustBattle()
//	Desc : PK�� ��� ���������� �˻��� �ƴ� ��� ���Ƽ�� �����Ѵ�
//--------------------------------------------------------------------------------------------
void BaseChar::CheckJustBattle(int enemyID)
{				
	if( ISMONSTER(enemyID) ) return;
    
	BaseChar*	assaulter = this;
	BaseChar*   victim	  = (BaseChar*)pMyWorld->pPlayerOnAccount[enemyID];

	if( assaulter == NULL || victim == NULL ) return;



	// 1. PK�� �����ϴ� �����̸� ���Ƽ�� �������� �ʴ´� 
	//-----------------------------------------------------------------------------------
	if( pMyWorld->clISOMap.IsWarFieldMap( assaulter->uiMapID ) ) return;
	if( pMyWorld->clISOMap.IsWarFieldMap( victim->uiMapID ) ) return;


	// 2. ������ ������ �� �������� �������� ���Ѵ� (�Ѵ� ���� �ϰ��)
	//-----------------------------------------------------------------------------------
	if( ISPLAYER(assaulter->uiAccount) && ISPLAYER(victim->uiAccount) )
	{

		// BP: BattlePoint (������)

		int assulterBP  = assaulter->IFollower.GetFollowerParameterBattlePower();
		int victimBP    = victim->IFollower.GetFollowerParameterBattlePower();
		

		if( assulterBP > victimBP )
		{
			int differBP = assulterBP - victimBP;
			int cutPoint = differBP * 0.10;



			// 3. ��� �뺴(����� ���ΰ���) �� ����ġ�� ������ ������ 10% ��ŭ �谨 �Ѵ� 
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
//	Desc : ������ ������ �Ǵ�
//--------------------------------------------------------------------------------------------
bool BaseChar::PickBattleWithEnemy(int enemyID)
{		
	UI16	uiPartyID = INITIAL_ERROR_VALUE;

	if( GetBattle() != NULL ) return FALSE;	

	
	// 1. ������ �� ����� ��ȿ�Ѱ�?
	//---------------------------------------------------------------------------------------
	BaseChar* pEnemy = GetBaseCharFromID(enemyID);		
	if( pEnemy == NULL ) return FALSE;	


	// -- ���� �ʵ忡 �ְų� ���� �ʵ忡 ���������� ������ �� �� �ִ�.	
	if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD ) == FALSE  ||
		CHECKSTATUS( pEnemy->siStatus, ON_PLAYER_STATUS_INFIELD ) == FALSE )	return FALSE;		


	// -- ���̳� �ڽ��� �迡 Ÿ�� �ִ��� �˾� ���� 
	if( IsInShip( uiAccount ) || IsInShip( enemyID ) )							return FALSE;
	
	// ���� ��밡 ��� �÷��̾��� ��� ���� �ϳ��� ��ȭ����̸� ���� ó��
	if( ISPLAYER(uiAccount) && ISPLAYER(enemyID) )
	{
		if( GetAttackMode() == FALSE || pEnemy->GetAttackMode() == FALSE )		return FALSE;
	}

	// -- ���� ����� ��� 
	//if( IsAdminPlayer(uiEnemyID) ) return FALSE;	




	// 2. ���� �̹� ���� ��� ���̶�� ���� ������� ������ �����ϰ� �׷��� ������ 
	//    ���ο� ������ �����Ѵ� 
	//---------------------------------------------------------------------------------------
	if( pEnemy->GetBattle() )
	{				
		// ���� �Ⱓ ���� �߰�. ������ 2003/07/29 10:21
		
		// �������� �ƴϸ� ����ڴ� ������� ������ ������ �� ����.
//		if( ( pMyWorld->clISOMap.IsWarFieldMap( uiMapID ) == FALSE ) && ( ISPLAYER( uiAccount ) == TRUE ) )			return	FALSE;

		// �̺�Ʈ(��ʸ�Ʈ)�߿��� ������� ������ ������ �� ����. (1:1�� ����)
		if( IsEvent() )		return FALSE;

//		if( pEnemy->GetBattle()->GetPartyID() == 0 )		return	FALSE;

		
		// ������� ��쿡�� ��Ƽ�� �ְ�, ���� ������ ������ �� �ִ� ��Ƽ���� ��쿡��
		// �� �� �ִ�.
		if( pMyWorld->clISOMap.IsWarFieldMap( uiMapID ) == TRUE )
		{
			// �������̴�.
			// �������� ��Ƽ�� ������� ��� ������ ������ ������ �� �ִ�.

		}
		else
		{
			// �������� �ƴϴ�.
			// �������� �ƴϸ� ��Ƽ���� ��쿡�� ������ ������ �� �ִ�.

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


	// 3. PK�� ��� �� ���缺�� �˾ƺ��� ���Ƽ�� �����Ѵ� 
    //---------------------------------------------------------------------------------------		
	CheckJustBattle(enemyID);

	return TRUE;
}
