#include "SOMain.h"
#include "SOWorld.h"
#include "SOPlayer.h"
#include "ParametaBox.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ON_IMPERIUM_USE
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//소집권을 먹었을때 SpendItem의 ON_ITEM_SPECIAL_IMPERIUM가 발생한다.
//이에 대한 응답으로 ON_IMPERIUM_USE을 클라이언트에 전달한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ON_REQUEST_FIELDATTACK_READY 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//공성 유닛과 유닛의 싸움에서 무기를 발사한다는 메세지를 보낸다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvFieldAttackReady()
{
	OnReqFieldAttackRdyMsg*			pOnReqFieldAttackRdyMsg;
	SOPlayer*						pTargetPlayer;
	DWORD							dwResponse = ON_RET_OK;
	OnlineFollower*					pFollower = IFollower.GetFollower(0);
	DWORD                           dwNowTick = GetTickCount();
	DWORD                           dwTempTime = 0;
	int                             nUnitCode = -1;
	BOOL                            bIsLong = FALSE;

	pOnReqFieldAttackRdyMsg		=	(OnReqFieldAttackRdyMsg*)cMsg;

	//맞는 플레이어.
	pTargetPlayer =	pMyWorld->GetPlayer(pOnReqFieldAttackRdyMsg->uiUniqueID);

	//공성 시간인가
	if( pMyWorld->IsAttackVillageTimeZone() == FALSE )
	{
		dwResponse = ON_RET_NOT_ATTACK_TIME;
		goto END;
	}

	if(pTargetPlayer != NULL)
	{   //적대 상단인가
		if( pMyWorld->clIGuild.IsEnemyRelationWar( uiGuildID, pTargetPlayer->uiGuildID ) == FALSE )
		{
			dwResponse = ON_RET_NOT_ENEMYGUILD;
			goto END;
		}
		
		//공성 유닛 인가
		if(pTargetPlayer->IsFieldAttackUnit() == FALSE)	
		{
			dwResponse = ON_RET_NOT_ATTACK_UNITE;
			goto END;
		}

		//사랑 있나
		if(pTargetPlayer->IsFieldAlive()	  == FALSE)
		{
			dwResponse = ON_RET_ATTACK_NO;
			goto END;
		}
	}
	else if(pTargetPlayer == NULL)
	{
		dwResponse = ON_RET_ATTACK_NO;
		goto END;
	}

	if(IsFieldAttackUnit() == FALSE)
	{
		dwResponse = ON_RET_NOT_ATTACK_UNITE;
		goto END;
	}

	nUnitCode = CSiegeUnitTable::GetInstance()->GetKindUnitCode(uiKind);
	if(nUnitCode < 0)
	{
		dwResponse = ON_RET_ATTACK_NO;
		goto END;
	}

	if(nUnitCode == FIELD_BATTLE_ARROW || nUnitCode == FIELD_BATTLE_SIEGE)
		bIsLong = TRUE;
/*
	//현재 시간을 구한다.
	dwTempTime = dwNowTick - m_dwAttackDelay;
	if(dwTempTime < CSiegeUnitTable::GetInstance()->GetDelay(nUnitCode))
	{
		dwTempTime = CSiegeUnitTable::GetInstance()->GetDelay(nUnitCode) - dwTempTime;
		dwResponse = ON_RET_NOT_ATTACK_DELAY_TIME;
		goto END;
	}
	else
		dwTempTime = 0;
*/
END:
	if(dwResponse == ON_RET_OK)
	{
		//자신을 포함한 주의의 모든 사람들에게 보낸다.
		SendFieldMsg(ON_RESPONSE_FIELDATTACK_READY, SO_SFM_MEANDNEARCHAR,
			         LPVOID(dwResponse), LPVOID(uiAccount),                   
					 LPVOID(pOnReqFieldAttackRdyMsg->uiUniqueID),
					 LPVOID(pFollower->uiCharID), LPVOID(dwTempTime), LPVOID(bIsLong));
	}
/*	else
	{
		//조건을 만족시키지 못할경우 자기 자신에게만 실패 메세지를 보낸다.
		SendFieldMsg(ON_RESPONSE_FIELDATTACK_READY, SO_SFM_ONLYME,
			         LPVOID(dwResponse), LPVOID(uiAccount),                   
					 LPVOID(pOnReqFieldAttackRdyMsg->uiUniqueID),
					 LPVOID(pFollower->uiCharID), LPVOID(dwTempTime), LPVOID(bIsLong));
	}
	*/
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ON_REQUEST_FIELDATTACK_ACTION
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//공성 유닛과의 싸움에서 무기에 맞았다는 메세지를 보낸다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvFieldAttackAction()
{
	OnReqFieldAttackActionMsg*		pOnReqFieldAttackActionMsg;
	SOPlayer*						pTargetPlayer;
	DWORD							dwResponse = ON_RET_OK;
	OnlineFollower*					pFollower = IFollower.GetFollower(0);
	OnlineFollower*					pTargetFollower = NULL;
	SI16							siDamage = 0;
	SI16							siAttackType = ON_RET_NONE_POWER;

	pOnReqFieldAttackActionMsg	= (OnReqFieldAttackActionMsg*)cMsg;
	pTargetPlayer				= pMyWorld->GetPlayer(pOnReqFieldAttackActionMsg->uiUniqueID);

	if(pTargetPlayer == NULL) return;

	if(pTargetPlayer != NULL)
	{
		//맞는 사람이 공성유닛인지 확인한다.
		if(!pTargetPlayer->IsFieldAttackUnit())	return;
		if(!pTargetPlayer->IsFieldAlive())	return;
	}
	
	//때리는 사람이 공성유닛인지 확인한다.
	if(IsFieldAttackUnit() == FALSE) return;
	
	pTargetFollower = pTargetPlayer->IFollower.GetFollower(0);
	if(pTargetFollower == NULL) return;

	//죽은 공성유닛은 처리해준다. 
	if(FieldAttackDead()) return;
	if(pTargetPlayer->FieldAttackDead()) return;

	//유닛코드를 얻어온다
	SI16 siUnitCode = CSiegeUnitTable::GetInstance()->GetKindUnitCode(uiKind);
	if(siUnitCode < 0 || siUnitCode == FIELD_BATTLE_RAMPART) 
	{
		dwResponse = ON_RET_NO;
		return;
	}
	
	//공격 딜레이 계산
	DWORD dwNowTick = GetTickCount();
	DWORD dwTempTime = dwNowTick - m_dwAttackDelay;
	if(dwTempTime < CSiegeUnitTable::GetInstance()->GetDelay(siUnitCode))
	{
		dwTempTime = CSiegeUnitTable::GetInstance()->GetDelay(siUnitCode) - dwTempTime;
		dwResponse = ON_RET_NOT_ATTACK_DELAY_TIME;
		return;
	}
	m_dwAttackDelay = dwNowTick; //현재 시간을 설정해준다.

	//데미지를 계산한다.
	siDamage = CSiegeUnitTable::GetInstance()->GetDamage(siUnitCode);	
	if(pOnReqFieldAttackActionMsg->siAttackType == ON_ATTACK_TYPE_NEARCAPTAIN)
	{
		double dTemp = siDamage * 1.2; 
		siDamage = (int)(dTemp + 0.5);
	}
	
	if(GetCaptainUnit())
		siDamage = siDamage * 2;

	SI16 siAttackPower  = pMyWorld->m_AttackPowerParser.GetAttackPercent(GetFightingPower());
	SI16 siDefencePower = 100 - pMyWorld->m_AttackPowerParser.GetDefencePercent(pTargetPlayer->GetFightingPower());

	if((siClassInGuild	==	pMyWorld->clIGuildClass.GetHNDCode()) || (siVMercenarySlot > 0))
	{
		siAttackPower = 5;
		siDefencePower = 95;
	}

	SI16 siRand = rand()%100 + 1;	

	if(pOnReqFieldAttackActionMsg->siAttackType == ON_ATTACK_TYPE_NEARCAPTAIN)
	{
		siDamage *= 1.2;
	}
	
	//견고한 공격이 히트되었다면.
	if(siAttackPower >= siRand)
	{	
		 //원래 주려던 데미지의 1.5배의 데미지를 준다.
		 float fTempDamage = (float)siDamage * 1.5;
		 siDamage	  = (SI16)fTempDamage;
		 siAttackType = ON_RET_ATTACK_POWER;
	}
	//맞는 인간의 견고한 방어가 성공하였다.
	else if(siDefencePower < siRand)
	{
		siDamage = 1;
		siAttackType = ON_RET_DEFENCE_POWER;
	}

	pTargetPlayer->DecreaseFieldHp(siDamage);
	
	if(pTargetPlayer->GetFieldHp() <= 0) m_siKillNum++;

	if(dwResponse == ON_RET_OK)
	{
		//데미지를 던져주기 위해 모든사람들에게 보낸다.
		pTargetPlayer->SendFieldMsg(ON_RESPONSE_FIELDATTACK_ACTION, SO_SFM_MEANDNEARCHAR, 
							LPVOID(uiAccount),
							LPVOID(siDamage),
							LPVOID(m_bFieldAlive),
							LPVOID(pOnReqFieldAttackActionMsg->uiUniqueID),
							LPVOID(dwResponse),
							LPVOID(siAttackType));
	}
	/*
	else
	{
		SendFieldMsg(ON_RESPONSE_FIELDATTACK_ACTION, SO_SFM_ONLYME, 
							LPVOID(uiAccount),
							LPVOID(siDamage),
							LPVOID(m_bFieldAlive),
							LPVOID(pOnReqFieldAttackActionMsg->uiUniqueID),
							LPVOID(dwResponse),
							LPVOID(siAttackType));
	}
	*/
	

	if(siUnitCode == FIELD_BATTLE_SPEAR || siUnitCode == FIELD_BATTLE_SIEGE) 
	{		
		//스플레쉬 데미지 처리
		UI16 uiDir = 0;
		int i = 0;
		UI16 uiTargetID[9];
		memset(uiTargetID, 0, sizeof(UI16) * 9);
		
		if(siUnitCode == FIELD_BATTLE_SPEAR) //창병인경우
		{
			//방향을 알아온다.
			uiDir = GetDir(siX, siY, pTargetPlayer->siX, pTargetPlayer->siY);

			//스플레쉬 데미지를 받는 유저 인덱스를 알아온다.
			GetSpTargetIDOFSpear(uiDir, pTargetPlayer->siX, pTargetPlayer->siY, uiTargetID);
			
		}
		else if(siUnitCode == FIELD_BATTLE_SIEGE) //발석거인경우
		{
			GetSpTargetIDOFSiege( pTargetPlayer->siX - 1, pTargetPlayer->siY - 1, 
								  pTargetPlayer->siX + 1, pTargetPlayer->siY + 1, uiTargetID);
		}

		SOPlayer *pPlayer = NULL;
		for(i = 0; i < 9; i++)
		{
			if(uiTargetID[i] == 0) continue;
			if(pOnReqFieldAttackActionMsg->uiUniqueID == uiTargetID[i]) continue;

			pPlayer = pMyWorld->GetPlayer(uiTargetID[i]);
			if(pPlayer == NULL ) continue;
			if(pPlayer->IsFieldAttackUnit() == FALSE) continue; //공성 유닛인지 검사
			if(uiGuildID == pPlayer->uiGuildID) continue; //적대 상단인지 검사
			if(pMyWorld->clIGuild.IsEnemyRelationWar( uiGuildID, pPlayer->uiGuildID) == FALSE ) continue;

			
			pPlayer->DecreaseFieldHp(siDamage);

			pPlayer->SendFieldMsg(ON_RESPONSE_FIELDATTACK_ACTION, SO_SFM_MEANDNEARCHAR, 
								  LPVOID(uiAccount), LPVOID(siDamage), LPVOID(m_bFieldAlive),
								  LPVOID(pPlayer->uiAccount), LPVOID(dwResponse), LPVOID(siAttackType));
		}	
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ON_REQUEST_FIELDATTACK_DEAD
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//공성 유닛과의 싸움에서 죽었다는 메세지를 보낸다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvFieldAttackDead()
{
	OnReqFieldAttackDeadMsg*		pOnReqFieldAttackDeadMsg;
	pOnReqFieldAttackDeadMsg	=	(OnReqFieldAttackDeadMsg*)cMsg;
	
	OnlineFollower*					pFollower = IFollower.GetFollower(0);
	if(pFollower == NULL) return;
	if(IsFieldAttackUnit() == FALSE) return;

	if(m_bCaptainUnit == FALSE)
	{
		//서버에서 비교하기 위한 시간을 저장한다.
		m_dwDeadTime = pMyWorld->dwtimeGetTime;
		//패널티로 받는 시간을 저장한다. 평민 30초 
		m_dwDeadSec  = 30000;
		m_bFieldAlive = FALSE;
	}
	else
	{
		//서버에서 비교하기 위한 시간을 저장한다.
		m_dwDeadTime = pMyWorld->dwtimeGetTime;
		//패널티로 받는 시간을 저장한다. 대장 1분
		m_dwDeadSec  = 60000;
		m_bFieldAlive = FALSE;
	}

	DBIKChangeCharacter			ChangeCharacter;
							
	ChangeCharacter.siDBAccount				= stAccountInfo.siDBAccount;
	ChangeCharacter.uiDBSlot				= stAccountInfo.uiSlot;
	ChangeCharacter.siDeadTime				= (SI32)m_dwDeadSec;

	uiKind				= m_uiPrevKind;
	pFollower->uiCharID = m_uiPrevKind;

	SetFieldHp(0);
	SetPrevKind(GetPrevKind());
	SetCaptainUnit(FALSE);
	SetFieldMaxHP(0);
	SetFieldMP(0);
	SetFieldMaxMP(0);

	if(m_bPlayerVillageInLimit) //수성 인터페이스에서 나간걸로 처리
	{		
		SOVillage *pVillage	= pMyWorld->GetVillageByCode(m_siVilligeCode);
		m_bPlayerVillageInLimit = FALSE;
		m_siVilligeCode = -1;
		pVillage->DeletCharName(szID);
		SendFieldMsg(ON_RESPONSE_VILLAGE_DEFFNCE_STOP, SO_SFM_ONLYME);
	}

	pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_CHANGECHARACTER, LPVOID(&ChangeCharacter), sizeof(ChangeCharacter));

	SendFieldMsg(ON_CHANGE_CHARACTERCODE, SO_SFM_MEANDNEARCHAR, LPVOID(this),LPVOID(m_uiPrevKind));

}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ON_REQUEST_LONGDISTANCE_VILLAGEATTACK_READY
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//원거리 공성유닛이 마을을 공격할 때 무기를 발사한다는 메세지를 보낸다.
//마을 공격을 근거리와 원거리로 나눈이유 : 
//근거리의 경우 마을의 반격이 가능하고 원거리의 경우 동기화를 맞춰야 하는 이유로 둘로 나눴다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvLongDistanceAttackVillageReady()
{
	OnReqLongAttackVillageRdyMsg*	pOnReqLongAttackVillageRdyMsg;
	SOVillage					*pVillage;	
	OnlineFollower				*pFollower = IFollower.GetFollower(0);
	DWORD                        dwNowTick = GetTickCount();
	DWORD                        dwResponse = ON_RET_OK;
	DWORD                        dwTempTime = 0;
	int                          nUnitCode = -1;
	BOOL                         bIsLong = FALSE;

	// 이 메시지를 받기에 유효한 시간인지 알아온다.	
	if( pMyWorld->IsAttackVillageTimeZone() == FALSE ) 
	{
		dwResponse = ON_RET_NOT_ATTACK_TIME;
		goto END;
	}

	pOnReqLongAttackVillageRdyMsg	=	(OnReqLongAttackVillageRdyMsg*)cMsg;

	if( ( pVillage = pMyWorld->GetVillageByCode( pOnReqLongAttackVillageRdyMsg->uiVillageCode ) ) == NULL )	
	{
		dwResponse = ON_RET_NOT_VILLAGE_CODE;
		goto END;
	}

	// 현재 마을에 상단이 있는지 검사한다.
	if( pVillage->uiGuildID == 0 )																		
	{
		dwResponse = ON_RET_NOT_VILLAGE_CODE;
		goto END;
	}

	// 플레이어의 상단과 이 마을의 상단이 적대적인지 검사한다.
	if( pMyWorld->clIGuild.IsEnemyRelationWar( uiGuildID, pVillage->uiGuildID ) == FALSE )
	{
		dwResponse = ON_RET_NOT_ENEMYGUILD;
		goto END;
	}

	//현재 시간을 구한다.
//	dwTempTime = dwNowTick - m_dwAttackDelay;
	nUnitCode = CSiegeUnitTable::GetInstance()->GetKindUnitCode(uiKind);
	if(nUnitCode < 0) 
	{
		dwResponse = ON_RET_NOT_ATTACK_UNITE;
		goto END;
	}

	if(nUnitCode == FIELD_BATTLE_ARROW || nUnitCode == FIELD_BATTLE_SIEGE)
		bIsLong = TRUE;
/*
	if(dwTempTime < CSiegeUnitTable::GetInstance()->GetDelay(nUnitCode))
	{
		dwTempTime = CSiegeUnitTable::GetInstance()->GetDelay(nUnitCode) - dwTempTime;
		dwResponse = ON_RET_NOT_ATTACK_DELAY_TIME;
		goto END;
	}
	else
		dwTempTime = 0;
	*/
END:
	//위 조건들을 충족 시킨다면 자신을 포함한 주의의 모든 사람들에게 보낸다.
    if(dwResponse == ON_RET_OK)
	{
		SendFieldMsg(ON_RESPONSE_LONGDISTANCE_VILLAGEATTACK_READY, SO_SFM_MEANDNEARCHAR,
		             LPVOID(dwResponse), LPVOID(uiAccount),
				     LPVOID(pOnReqLongAttackVillageRdyMsg->uiVillageCode),
				     LPVOID(pFollower->uiCharID), LPVOID(dwTempTime), LPVOID(bIsLong));
	}
/*	else
	{
		SendFieldMsg(ON_RESPONSE_LONGDISTANCE_VILLAGEATTACK_READY, SO_SFM_ONLYME,
		             LPVOID(dwResponse), LPVOID(uiAccount),
				     LPVOID(pOnReqLongAttackVillageRdyMsg->uiVillageCode),
				     LPVOID(pFollower->uiCharID), LPVOID(dwTempTime), LPVOID(bIsLong));
	}
	*/
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ON_REQUEST_LONGDISTANCE_VILLAGEATTACK_ACTION
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//원거리 공성유닛이 마을을 공격할 때 무기가 맞았다는 메세지를 처리해준다.
//마을이 파괴되었을때의 일도 처리한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvLongDistanceAttackVillageAction()
{
	OnReqLongAttackVillageActionMsg		*pOnReqLongAttackVillageActionMsg;
	SOVillage							*pVillage;
	BOOL                                 bIsLong = FALSE;

	pOnReqLongAttackVillageActionMsg	=	(OnReqLongAttackVillageActionMsg*)cMsg;
	
	//공성이 가능한 시간인가 체크..
	if( pMyWorld->IsAttackVillageTimeZone() == FALSE ) return;	
	if((pVillage = pMyWorld->GetVillageByCode( pOnReqLongAttackVillageActionMsg->uiVillageCode)) == NULL ) return;	
	if( pVillage->GetVillageDefence() <= 0) return;
	// 현재 마을에 상단이 있는지 검사한다.
	if( pVillage->uiGuildID == 0 ) return;
	// 플레이어의 상단과 이 마을의 상단이 적대적인지 검사한다.
	if( pMyWorld->clIGuild.IsEnemyRelationWar( uiGuildID, pVillage->uiGuildID ) == FALSE ) return;	

	int nUnitCode = CSiegeUnitTable::GetInstance()->GetKindUnitCode(uiKind);
	if(nUnitCode < 0) return;
	
	if(nUnitCode == FIELD_BATTLE_ARROW || nUnitCode == FIELD_BATTLE_SIEGE)
		bIsLong = TRUE;
	
	//마을을 공격한다.
	if(pVillage->AttackDamage(this, pOnReqLongAttackVillageActionMsg->siAttackType, bIsLong) == TRUE)
	{
		if(pVillage->uiGuildID != 0)
			pMyWorld->clIGuild.DelVillage(pVillage->uiGuildID, pVillage, FALSE);
	}

	/*
	//데미지 계산 마을 파괴되었나 파악 등등.
	if(pVillage->LongDistanceAttack(this,pOnReqLongAttackVillageActionMsg->siAttackType) == TRUE)
	{
		// 마을이 완전히 파괴되었다.
		if(pVillage->uiGuildID != 0)
			pMyWorld->clIGuild.DelVillage(pVillage->uiGuildID, pVillage, FALSE);
	}	
	*/
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ON_REQUEST_ATTACK_VILLAGE
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//공성유닛이 마을을 공격할때 마을을 때린다.마을이 맞았다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvAttackVillage()
{
	OnReqAttackVillageMsg		*pAttackVillageMsg;
	SOVillage					*pVillage;	
	BOOL                         bIsLong = FALSE;

	// 이 메시지를 받기에 유효한 시간인지 알아온다.	
	if( pMyWorld->IsAttackVillageTimeZone() == FALSE )													return;		
	pAttackVillageMsg	=	(OnReqAttackVillageMsg*)cMsg;

	if( ( pVillage = pMyWorld->GetVillageByCode( pAttackVillageMsg->uiVillageCode ) ) == NULL )			return;	
	if( pVillage->GetVillageDefence() <= 0) return;

	// 현재 마을에 상단이 있는지 검사한다.
	if( pVillage->uiGuildID == 0 )																		return;

	// 플레이어의 상단과 이 마을의 상단이 적대적인지 검사한다.
	if( pMyWorld->clIGuild.IsEnemyRelationWar( uiGuildID, pVillage->uiGuildID ) == FALSE )				return;	

	int nUnitCode = CSiegeUnitTable::GetInstance()->GetKindUnitCode(uiKind);
	if(nUnitCode < 0) return;
	
	if(nUnitCode == FIELD_BATTLE_ARROW || nUnitCode == FIELD_BATTLE_SIEGE)
		bIsLong = TRUE;
	
	if(pVillage->AttackDamage(this, pAttackVillageMsg->siAttackType, bIsLong) == TRUE)
	{
		if(pVillage->uiGuildID != 0)
			pMyWorld->clIGuild.DelVillage(pVillage->uiGuildID, pVillage, FALSE);
	}
	
	/*
	if( pVillage->AttackedByPlayer(this, pAttackVillageMsg->siAttackType) == TRUE )
	{
		// 마을이 완전히 파괴되었다.
		if(pVillage->uiGuildID != 0)
			pMyWorld->clIGuild.DelVillage(pVillage->uiGuildID, pVillage, FALSE);
	}
	*/
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ON_REQUEST_WAR_VILLAGE_DEFENCE
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 수성을 하기위해 마을 안으로 들어간다.
// 실제로 마을에 들어간 상태는 아니고 필드 속성을 가지고 있으면서 마을의 포인터를 가지고 있는다.
// 필드->수성 인터페이스.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvReqWarVillageDefence()
{
	OnReqWarVillageDefence*			 pOnReqWarVillageDefence;
	SOVillage*						 pVillage	=	NULL;
	DWORD							 dwResponse = ON_RET_NO;
	UI32							 uiVillageGuildID;
	SI32							 siLimitTime = 0;
	cltGuild*						 pclGuild	 = NULL;
	SI16                             siUnitCode = -1;

	pOnReqWarVillageDefence	=	(OnReqWarVillageDefence*)cMsg;

	stAccountInfo.pVillage	=	pMyWorld->GetVillageByCode(pOnReqWarVillageDefence->uiVillageCode);				// 마을을 찾는다.
	pVillage				=   pMyWorld->GetVillageByCode(pOnReqWarVillageDefence->uiVillageCode);

	if(pMyWorld->IsAttackVillageTimeZone() == FALSE)
	{
		dwResponse	= ON_RET_NO;
		goto END;
	}

	if(pMyWorld->clIGuild.GetGuildWarCheck(uiGuildID) == FALSE)
	{
		dwResponse	= ON_RET_NO;
		goto END;
	}

	if((pclGuild = pMyWorld->clIGuild.GetGuildByID(uiGuildID)) == NULL )
	{
		dwResponse	= ON_RET_NO;
		goto END;
	}
	
	if(uiGuildID == 0)
	{
		dwResponse	= ON_RET_NO;
		goto END;
	}

	if(uiGuildID != pVillage->uiGuildID)
	{
		dwResponse = ON_RET_NO;
		goto END;
	}

	if(m_bPlayerVillageInLimit)
	{
		dwResponse = ON_RET_NO;
		goto END;
	}

	siUnitCode = CSiegeUnitTable::GetInstance()->GetKindUnitCode(uiKind);
	if(siUnitCode < 0)
	{
		dwResponse	= ON_RET_NO;
		goto END;
	}

	if(siUnitCode != FIELD_BATTLE_SWORD) //검병만 가능하다.
	{
		dwResponse	= ON_RET_NO;
		goto END;
	}
/*
	if(siClassInGuild != pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI) &&
		siClassInGuild != pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DHS) &&
		strcmp(szID,pVillage->szHNSID) != 0)
	{
		dwResponse	= ON_RET_NO;
		goto END;
	}

	//한 마을에는 한명의 플레이어만 수성 인터페이스에 들어가 있을수 있다.
	if(pVillage->m_bVillageInLimit == TRUE)
	{
		dwResponse	=	ON_RET_GUILD_VILLAGE_OFF_LIMITS;
		goto	END;
	}
*/
	if(stAccountInfo.pVillage == NULL)	goto	END;

	if( (stAccountInfo.pVillage->uiMapID != uiMapID) ||
		( (abs(stAccountInfo.pVillage->siVillageX - siX) > 10) && (abs(stAccountInfo.pVillage->siVillageY - siY) > 10) ) )
	{
		writeInfoToFile( "WarpHacking.txt", "DBAccount(%d)  szID(%s)  Player MapIndex(%d) X(%d) Y(%d) Try to Enter village(%s)",
						stAccountInfo.siDBAccount, szID, uiMapID, siX, siY, stAccountInfo.pVillage->GetVillageName() );
		goto	END;
	}

	// 마을의 Guild ID를 구한다.
	uiVillageGuildID		=	stAccountInfo.pVillage->GetGuildNID();

	// 적 마을의 상단이면 들어갈 수 없다.
	if((uiGuildID != 0) && (uiVillageGuildID != 0) && (siClassInGuild != pMyWorld->clIGuildClass.GetJWJCode()))
	{
		if(pMyWorld->clIGuild.IsEnemyGuildBoth(uiGuildID, uiVillageGuildID) == TRUE)		
		{
			dwResponse	=	ON_RET_ENEMY_GUILD_VILLAGE;
			goto	END;
		}
	}

	if(pVillage->SetCharName(szID) < 0)
	{
		goto	END;
	}

	// 교역창을 닫는다.
	CloseTrade();

	// 타 유저에게 파티 신청중이거나 
	// 다른 유저에 파티 신청을 받았다면 이를 취소한다.
	CloseRequestingJoinParty();

	//수성인터페이스에 들어와 있다는 플레그를 세팅.
//	pVillage->m_bVillageInLimit = TRUE;
	m_bPlayerVillageInLimit     = TRUE;
	m_siVilligeCode = pOnReqWarVillageDefence->uiVillageCode;	

	//클라이언트에 던져주기 위해서 수성 AttackType의 딜레이 시간을 계산.
	if(pVillage->m_dwPrevAttackdelay > 0)
		siLimitTime = (SI32)pVillage->m_dwSkillDelayTime - (pMyWorld->dwtimeGetTime - pVillage->m_dwPrevAttackdelay);

	dwResponse	=	ON_RET_OK;

END:
	// 마을에 들어갈 수 없다.
	switch(dwResponse)
	{
	case ON_RET_OK:
		SendFieldMsg(ON_RESPONSE_WAR_VILLAGE_DEFENCE, SO_SFM_MEANDNEARCHAR, LPVOID(dwResponse),LPVOID(siLimitTime),LPVOID(uiAccount));
		break;
	case ON_RET_NO:
	case ON_RET_ENEMY_GUILD_VILLAGE:		
		SendFieldMsg(ON_RESPONSE_WAR_VILLAGE_DEFENCE, SO_SFM_ONLYME, LPVOID(dwResponse),LPVOID(siLimitTime),LPVOID(uiAccount));				
		break;
	case ON_RET_GUILD_VILLAGE_OFF_LIMITS:
		SendFieldMsg(ON_RESPONSE_WAR_VILLAGE_DEFENCE, SO_SFM_ONLYME, LPVOID(dwResponse),LPVOID(siLimitTime),LPVOID(uiAccount));
		break;
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// ON_REQUEST_WAR_VILLAGE_IN
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 수성인터페이스에서 마을로 들어간다.
// 필드속성에서 마을속성으로 바뀐다.
// 수성 인터페이스 -> 마을
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvReqWarVillageIn()
{
	OnReqWarVillageIn*			pOnReqWarVillageIn;
	SOVillage*					pVillage	=	NULL;

	pOnReqWarVillageIn	=	(OnReqWarVillageIn*)cMsg;

	stAccountInfo.pVillage	=	pMyWorld->GetVillageByCode(pOnReqWarVillageIn->uiVillageCode);				// 마을을 찾는다.
	pVillage				=	pMyWorld->GetVillageByCode(pOnReqWarVillageIn->uiVillageCode);				// 마을을 찾는다.

	pVillage->m_bVillageInLimit = FALSE;
	m_bPlayerVillageInLimit		= FALSE;
	m_siVilligeCode = -1;
//	strcpy(pVillage->m_szCharName,"");

	//맵에서 지워준다.
	if(DelIDOnMap( 6 ) == FALSE)
		writeInfoToFile("DelIDOnMap Failed.txt", "5 [%u] [%d] [%d] [%d]", uiAccount, uiMapID, siX, siY);
//			pMyWorld->clServer->WriteInfo("..\\DelIDOnMap Failed.txt", "5 [%u] [%d] [%d] [%d]", uiAccount, uiMapID, siX, siY);

	EnterVillage();		
			
	//나한테 알릴땐 마을의 정보를 보낸다.
//	SendFieldMsg(ON_RESPONSE_WAR_VILLAGE_IN, SO_SFM_ONLYME,LPVOID(pVillage));

	//주위사람한테 보낼땐 마을 정보를 뺀다.
//	SendFieldMsg(ON_RESPONSE_WAR_VILLAGE_IN, SO_SFM_ONLYNEARCHAR);	
}

//수성 인터페이스에서 나온다.
VOID SOPlayer::RecvReqWarVillageDefenceStop()
{
	OnReqWarVillageIn     *pVillageIn = (OnReqWarVillageIn*)cMsg;

	SOVillage *pVillage	= NULL;
	pVillage = pMyWorld->GetVillageByCode(pVillageIn->uiVillageCode);				// 마을을 찾는다.

	DWORD dwRes = ON_RET_NO;

	if(pVillage == NULL)
	{
		SendFieldMsg(ON_RESPONSE_VILLAGE_DEFFNCE_STOP, SO_SFM_ONLYME, LPVOID(dwRes));
		return;
	}

	if(!m_bPlayerVillageInLimit)
	{
		SendFieldMsg(ON_RESPONSE_VILLAGE_DEFFNCE_STOP, SO_SFM_ONLYME, LPVOID(dwRes));
		return;
	}	
	
	if( pVillage->DeletCharName(szID) < 0)
	{
		SendFieldMsg(ON_RESPONSE_VILLAGE_DEFFNCE_STOP, SO_SFM_ONLYME, LPVOID(dwRes));
		return;
	}

	dwRes = ON_RET_OK;
	m_bPlayerVillageInLimit = FALSE;
	m_siVilligeCode = -1;

//	SendFieldMsg(ON_RESPONSE_VILLAGE_DEFFNCE_STOP, SO_SFM_ONLYME, LPVOID(dwRes));
	SendFieldMsg(ON_RESPONSE_VILLAGE_DEFFNCE_STOP, SO_SFM_MEANDNEARCHAR, LPVOID(dwRes));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ON_REQUEST_VILLAGE_ATTACK_TYPE_CHECK
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//수성 인터페이스의 기능을 사용할수 있는지 알아온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
/*
VOID	SOPlayer::RecvVillageAttackTypeCheck()
{
	OnReqVillageAttackTypeCheck*		pOnReqVillageAttackTypeCheck;
	SOVillage*							pVillage;
	DWORD								dwResponse = ON_RET_OK;

	pOnReqVillageAttackTypeCheck =		(OnReqVillageAttackTypeCheck*)cMsg;
	pVillage = pMyWorld->GetVillageByCode(pOnReqVillageAttackTypeCheck->uiVillageCode);

	// siClassInGuild 멤버 변수임.-_-난감..
	// 대방이나 대행수 인지 체크.
	if(siClassInGuild == pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI) || 
		siClassInGuild == pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DHS) ||
		strcmp(szID,pVillage->szHNSID) == 0)
	{		
		if(pVillage != NULL)
		{
			if(pVillage->GetVillageAttackTypeDelayCheck() == FALSE)
			{
				if(pVillage->GetVillageAttackType() == ON_VILLAGE_ATTACK_NONE)
				{
					dwResponse = ON_RET_ATTACK_TYPE_OK;
				}
				else
					dwResponse = ON_RET_ATTACK_TYPE_ACTIVE;
			}
			else
				dwResponse = ON_RET_ATTACK_TYPE_DELAY;
		}
		else 
			dwResponse = ON_RET_ATTACK_TYPE_NO;
	}
	else
		dwResponse = ON_RET_ATTACK_TYPE_NOT_AUTH;

	SendFieldMsg(ON_RESPONSE_VULLAGE_ATTACK_TYPE_CHECK,SO_SFM_ONLYME,
		LPVOID(dwResponse),
		LPVOID(pOnReqVillageAttackTypeCheck->uiVillageCode));
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ON_REQUEST_VILLAGE_PLAYER_OIL_ATTACK
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//수성 인터페이스 기름 공격메세지에 대한 내용을 처리한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvVillagePlayerOilAttack()
{
	OnReqVillagePlayerOilAttack			*pOnReqVillagePlayerOilAttack;
	SOVillage							*pVillage;
	DWORD								dwResponse = ON_RET_OIL_NO;
	MONEY								mnMoney = 15000;
	SI32								siLimitTime = 0;

	pOnReqVillagePlayerOilAttack		=	(OnReqVillagePlayerOilAttack*)cMsg;

	pVillage = 	pMyWorld->GetVillageByCode( pOnReqVillagePlayerOilAttack->uiVillageCode );

	if(pVillage == NULL)															goto END;
	if(pVillage->GetVillageAttackType()			!=			ON_VILLAGE_ATTACK_NONE)	goto END;
	if(pVillage->m_bAttackTypeDelay				==							  TRUE) goto END;				
	if(EnoughMoney(mnMoney)						==							 FALSE)	
	{
		dwResponse = ON_RET_OIL_NOT_MONEY;
		goto END;   
	}
	
	pVillage->SetVillageAttackType(ON_VILLAGE_ATTACK_OIL);
	siLimitTime = (SI32)pVillage->m_dwSkillDelayTime;
	DecreaseMoney(mnMoney);		

	dwResponse = ON_RET_OIL_OK;

END:	 
	if(dwResponse == ON_RET_OIL_OK)
	{
		SendFieldMsg( ON_RESPONSE_VILLAGE_PLAYER_OIL_ATTACK , SO_SFM_MEANDNEARCHAR,
						LPVOID(pOnReqVillagePlayerOilAttack->uiVillageCode),
						LPVOID(dwResponse),
						LPVOID(mnMoney),
						LPVOID(siLimitTime),
						LPVOID(uiAccount));
	}
	else
	{
		SendFieldMsg( ON_RESPONSE_VILLAGE_PLAYER_OIL_ATTACK , SO_SFM_ONLYME,
						LPVOID(pOnReqVillagePlayerOilAttack->uiVillageCode),
						LPVOID(dwResponse),
						LPVOID(mnMoney),
						LPVOID(siLimitTime),
						LPVOID(uiAccount));
	}					
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ON_REQUEST_VILLAGE_PLAYER_STONE_ATTACK
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//수성 인터페이스 돌 공격메세지에 대한 내용을 처리한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvVillagePlayerStoneAttack()
{
	OnReqVillagePlayerStoneAttack		*pOnReqVillagePlayerStoneAttack;
	SOVillage							*pVillage;
	DWORD								dwResponse = ON_RET_STONE_NO;
	MONEY								mnMoney = 25000;
	SI32								siLimitTime = 0;

	pOnReqVillagePlayerStoneAttack		=	(OnReqVillagePlayerStoneAttack*)cMsg;

	pVillage =	pMyWorld->GetVillageByCode( pOnReqVillagePlayerStoneAttack->uiVillageCode );

	if(pVillage == NULL)															goto END;
	if(pVillage->GetVillageAttackType()			!=			ON_VILLAGE_ATTACK_NONE)	goto END;
	if(pVillage->m_bAttackTypeDelay				==							  TRUE) goto END;
	if(EnoughMoney(mnMoney)						==							 FALSE)
	{
		dwResponse = ON_RET_STONE_NOT_MONEY;
		goto END;
	}

	pVillage->SetVillageAttackType(ON_VILLAGE_ATTACK_STONE);

	siLimitTime = (SI32)pVillage->m_dwSkillDelayTime;
	DecreaseMoney(mnMoney);

	dwResponse = ON_RET_STONE_OK;
	
END:
	if(dwResponse == ON_RET_STONE_OK)
	{
		SendFieldMsg( ON_RESPONSE_VILLAGE_PLAYER_STONE_ATTACK, SO_SFM_MEANDNEARCHAR,
						LPVOID(pOnReqVillagePlayerStoneAttack->uiVillageCode),
						LPVOID(dwResponse),
						LPVOID(mnMoney),
						LPVOID(siLimitTime),
						LPVOID(uiAccount));
	}
	else
	{
		SendFieldMsg( ON_RESPONSE_VILLAGE_PLAYER_STONE_ATTACK, SO_SFM_ONLYME,
				LPVOID(pOnReqVillagePlayerStoneAttack->uiVillageCode),
				LPVOID(dwResponse),
				LPVOID(mnMoney),
				LPVOID(siLimitTime),
				LPVOID(uiAccount));
	}
}

*/

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ON_REQEUST_VILLAGE_DEFENCE
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//수성 인터페이스 마을 방어력 향상에 대한 내용을 처리한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvPlayerVillageDefenceUp()
{
	OnReqPlayerVillageDefenceUp*		pOnReqPlayerVillageDefenceUp;
	SOVillage*							pVillage;
	SI32								siDefenceSoldierToDefenceUp = 0;
	VDEF								vdDefenceUp;
	MONEY								mnMoney;
	DWORD								dwResponse = ON_RET_DEFENSE_NO;
	SI32								siLimitTime = 0;
	SI16                                siUnitCode = -1;
	DWORD                               dwNowTick = 0;

	pOnReqPlayerVillageDefenceUp		=	(OnReqPlayerVillageDefenceUp*)cMsg;

	pVillage =	pMyWorld->GetVillageByCode( pOnReqPlayerVillageDefenceUp->uiVillageCode );

	//마을이 없으면 false
	if(pVillage == NULL)				               goto END;
	if(pVillage->vdMaxDefence == pVillage->vdDefence)  goto END;

	//대방이나 대행수가 아니면 false
/*	if(siClassInGuild != pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI) && 
		siClassInGuild != pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DHS) &&
		strcmp(szID,pVillage->szHNSID) != 0) goto END;
	if(pVillage->GetVillageAttackType()			!=			ON_VILLAGE_ATTACK_NONE)	goto END;
	if(pVillage->m_bAttackTypeDelay				==							  TRUE) goto END;
	if(pVillage->GetDefenceSoldier()			<=						  0)		goto END;
	if(pVillage->vdMaxDefence == pVillage->vdDefence						)		goto END;


	//현재 마을 수비병의 10배까지 마을 방어력을 늘릴수 있다.
	siDefenceSoldierToDefenceUp = (SI32)pVillage->GetDefenceSoldier() * 10;
*/

	siUnitCode = CSiegeUnitTable::GetInstance()->GetKindUnitCode(uiKind);
	if(siUnitCode < 0)
	{
		dwResponse = ON_RET_DEFENSE_NO;
		goto END;
	}

	if(siUnitCode != FIELD_BATTLE_SWORD) 
	{   //검병이 아니면 수리할수 없다...
		dwResponse = ON_RET_DEFENSE_NO;
		goto END;
	}

	mnMoney = 1000;
	if(EnoughMoney(mnMoney) ==	FALSE)			
	{	//돈을 체크
		dwResponse = ON_RET_DEFENSE_NOT_MONEY;
		goto END;
	}

	pVillage->SetVillageAttackType(ON_VILLAGE_ATTACK_DEFENCE);
	dwNowTick = GetTickCount();
	if(m_bIsFirstDefence)
	{
		if(dwNowTick - m_dwDefenceTime < pVillage->m_dwSkillDelayTime) 
			goto END;
	}
	
	vdDefenceUp = pVillage->vdMaxDefence - pVillage->vdDefence;
	if(vdDefenceUp > 100)	
		vdDefenceUp = 100;
	
	//마을의 방어력을 올린다.
	pVillage->IncreaseVillageDefence(vdDefenceUp);
	
	//소지금을 줄인다.
	DecreaseMoney(mnMoney);

/*
	if(vdDefenceUp < siDefenceSoldierToDefenceUp)
	{
		//마을 방어력을 올리기 위한 돈을 구한다.
		mnMoney = (MONEY)vdDefenceUp;
		//돈이 충분한지 알아온다.
		if(EnoughMoney(mnMoney)	==	FALSE)			
		{	
			dwResponse = ON_RET_DEFENSE_NOT_MONEY;
			goto END;
		}
		//마을 방어력을 올린다.
		pVillage->IncreaseVillageDefence(vdDefenceUp);
		
		//소지금을 줄인다.
		DecreaseMoney(mnMoney);
		
	}
	else // vdDefenceUp >= siDefenceSoldierToDefenceUp
	{
		//올릴 마을 방어력을 얻어온다.
		vdDefenceUp = (VDEF)siDefenceSoldierToDefenceUp;
		//마을 방어력을 올리기 위한 돈을 구한다.
		mnMoney = (MONEY)vdDefenceUp;
		//돈이 충분한지 알아온다.
		if(EnoughMoney(mnMoney)	==	FALSE)		
		{
			dwResponse = ON_RET_DEFENSE_NOT_MONEY;
			goto END;
		}
		//마을 방어력을 올린다.
		pVillage->IncreaseVillageDefence(vdDefenceUp);
		//소지금을 줄인다.
		DecreaseMoney(mnMoney);
	}
*/
	//마을 공격형태를 방어력 향상으로 만든다.
	

	m_dwDefenceTime = dwNowTick;
	siLimitTime = (SI32)pVillage->m_dwSkillDelayTime; 
	dwResponse = ON_RET_DEFENSE_OK;

	m_bIsFirstDefence = TRUE;
	
END:	
	if(dwResponse == ON_RET_DEFENSE_OK)
	{
		SendFieldMsg( ON_RESPONSE_VILLAGE_DEFENCE, SO_SFM_MEANDNEARCHAR,
						LPVOID(dwResponse),LPVOID(pOnReqPlayerVillageDefenceUp->uiVillageCode),
						LPVOID(mnMoney),LPVOID(vdDefenceUp),LPVOID(siLimitTime),LPVOID(uiAccount));
	}
	else
	{
		SendFieldMsg( ON_RESPONSE_VILLAGE_DEFENCE, SO_SFM_ONLYME,
						LPVOID(dwResponse),LPVOID(pOnReqPlayerVillageDefenceUp->uiVillageCode),
						LPVOID(mnMoney),LPVOID(vdDefenceUp),LPVOID(siLimitTime),LPVOID(uiAccount));
	}
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ON_REQUEST_INCREASE_GUILD_UNIT
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//상단의 소집권을 더한다.
//마을 안에서만 가능.인벤토리의 아닌 상단의 cltGuild::cltGuildUnit 클래스에 저장한다.
//마을의 인구수.상단의 보급품 수에 비례해서 생산할수 있다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvIncreaseGuildUnit()
{
	cltGuild*					 pclGuild;
	SI16						 siTotalGuildUnitCount;
	OnReqIncreaseGuildUnit*		 pOnReqIncreaseGuildUnit;
	MONEY						 mnMoney;
	SOVillage*					 pVillage = NULL;	
	DWORD						 dwResponse = ON_RET_NOT;
	int                          nTempPopu = 0;
	SI16                         siUnitCode = 0;
	SI16                         siUnitCount = 0;

	pOnReqIncreaseGuildUnit		=	(OnReqIncreaseGuildUnit*)cMsg;

	if(pOnReqIncreaseGuildUnit->siCount <= 0)                                                       goto END;
	if(pOnReqIncreaseGuildUnit == NULL)																goto END;
	if((pclGuild = pMyWorld->clIGuild.GetGuildByID(uiGuildID)) == NULL )							goto END;
	if((pVillage = pMyWorld->GetVillageByCode(pOnReqIncreaseGuildUnit->uiVillageCode)) == NULL)		goto END;
	
	siUnitCode = CSiegeUnitTable::GetInstance()->GetKindUnitCode(pOnReqIncreaseGuildUnit->siKind);  
	if(siUnitCode < 0) goto END;

	siUnitCount = pclGuild->m_GuildUnit.GetGuildUnit(siUnitCode);
	if(siUnitCount >= ON_MAX_GUILD_UNIT_COUNT) goto END;
/*	
	if(pOnReqIncreaseGuildUnit->siKind < FIELD_BATTLE_SWORD || 
	   pOnReqIncreaseGuildUnit->siKind > FIELD_BATTLE_SUPPLY)                                       goto END;
*/
//	if(pOnReqIncreaseGuildUnit->siKind	==	FIELD_BATTLE_DEFENSE)									goto END;
	
	
	//아이템 아이디를 얻어온다.
//	uiItemID = pMyWorld->m_FieldBattleParser.GetItemID(pOnReqIncreaseGuildUnit->siKind);
//	pItemHeader = pMyWorld->clItem.GetItem(uiItemID);

	//수비병 * 1부대의 보급량 
//	siSupply = pItemHeader->m_pImperium->m_siSupply * pOnReqIncreaseGuildUnit->siCount;
	//1부대 * pItemHeader->m_pImperium->m_siPrice * pItemHeader->m_pImperium->m_siPopulation
//	mnMoney =  (MONEY)(pItemHeader->m_pImperium->m_siPrice * pItemHeader->m_pImperium->m_siPopulation * pOnReqIncreaseGuildUnit->siCount);


//	if(pclGuild->GetGuildSupply() < siSupply)
//	{
//		dwResponse = ON_RET_NOT_ENOUGH_SUPPLY;
//		goto END;
//	}

	//소집할 부대의 돈을 계산한다.
	mnMoney = CSiegeUnitTable::GetInstance()->GetPrice(siUnitCode);
	if(mnMoney < 0) goto END;

	mnMoney = mnMoney * pOnReqIncreaseGuildUnit->siCount;

	if(EnoughMoney(mnMoney)	==	FALSE)																
	{
		dwResponse = ON_RET_NOT_ENOUGH_MONEY;
		goto END;
	}

	
//	if(pVillage->CheckDecreasePopulation((UI32)(pItemHeader->m_pImperium->m_siPopulation * pOnReqIncreaseGuildUnit->siCount)) == FALSE)
	nTempPopu = CSiegeUnitTable::GetInstance()->GetPopulation(siUnitCode);
	if(nTempPopu < 0) goto END;
	
	//하루에 생산할수 있는 최대 생산량을 넘었나 체크.	
	if(!pVillage->CheckDecreasePopulation((UI32)nTempPopu * pOnReqIncreaseGuildUnit->siCount))
	{
		dwResponse = ON_RET_NOT_ENOUGH_POPULATION;
		goto END;
	}

	//마을의 최소 인구수를 제한한다.
//	if(pVillage->LimitDecreasePopulation((UI32)(pItemHeader->m_pImperium->m_siPopulation * pOnReqIncreaseGuildUnit->siCount)) == FALSE)
	if(!pVillage->LimitDecreasePopulation((UI32)nTempPopu * pOnReqIncreaseGuildUnit->siCount))
	{
		dwResponse = ON_RET_NOT_LIMIT_POPULATION;
		goto END;
	}

	//상단의 보급품 뺀다.
//	pclGuild->DecreaseGuildSupply((SI32)siSupply);
	
	//유저가 가진 돈을 뺀다.
	DecreaseMoney(mnMoney, FALSE);
	
	//모집된 병력 갯수를 증가 시킨다.
	siTotalGuildUnitCount = pclGuild->m_GuildUnit.IncreaseGuildUnit(siUnitCode, pOnReqIncreaseGuildUnit->siCount);

	//마을의 인구수를 감소시킨다.
//	pVillage->DecreasePopulation((UI32)(pItemHeader->m_pImperium->m_siPopulation * pOnReqIncreaseGuildUnit->siCount));
	pVillage->DecreasePopulation((UI32)(nTempPopu * pOnReqIncreaseGuildUnit->siCount));

	DBIKMakeGuildUnit		MakeGuildUnit;
	memset(&MakeGuildUnit, 0, sizeof(DBIKMakeGuildUnit));

	MakeGuildUnit.siDBAccount	=	stAccountInfo.siDBAccount;
	MakeGuildUnit.uiDBSlot		=	stAccountInfo.uiSlot;
	MakeGuildUnit.uiGuildID		=	uiGuildID;
	MakeGuildUnit.uiItemID		=	pOnReqIncreaseGuildUnit->siKind;
	MakeGuildUnit.siQuantity	=	pOnReqIncreaseGuildUnit->siCount;
//	MakeGuildUnit.siSupply		=	siSupply;
//	MakeGuildUnit.siSupply		=	0;
	MakeGuildUnit.mnMoney		=	mnMoney;
	MakeGuildUnit.uiVillageCode	=	pOnReqIncreaseGuildUnit->uiVillageCode;
	MakeGuildUnit.uiPopulation  =   (UI32)(nTempPopu * pOnReqIncreaseGuildUnit->siCount);
//	MakeGuildUnit.uiPopulation	=	(UI32)(pItemHeader->m_pImperium->m_siPopulation * pOnReqIncreaseGuildUnit->siCount);

	pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_INCREASE_GUILD_UNIT, LPVOID(&MakeGuildUnit), sizeof(DBIKMakeGuildUnit));

	dwResponse = ON_RET_OK;

END:
	//마을의 인구수를 줄이고 돈도 줄이고 보급품도 줄인다.
	if(dwResponse == ON_RET_OK)
	{
		SendFieldMsg(ON_RESPONSE_INCREASE_GUILD_UNIT, SO_SFM_ONLYME, 
			LPVOID(pOnReqIncreaseGuildUnit->siKind),
			LPVOID(siTotalGuildUnitCount), 
			LPVOID(0),
			LPVOID(mnMoney),
			LPVOID(dwResponse));
	}
	else
	{
		SendFieldMsg(ON_RESPONSE_INCREASE_GUILD_UNIT, SO_SFM_ONLYME,LPVOID(0),
			LPVOID(0),
			LPVOID(0),
			LPVOID(0),
			LPVOID(dwResponse));
	}
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ON_REQUEST_DECREASE_GUILD_UNIT
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//상단의 대방이 상단의 공성유닛 
//cltGuild::cltGuildUnit클래스의 데이터를 인벤토리의 아이템으로 전환한다. (차출)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvDecreaseGuildUnit()
{
	cltGuild*					 pclGuild;
	SI16						 siTotalGuildUnitCount;
	SI16						 siTempItemSlot;
	OnReqDecreaseGuildUnit*		 pOnReqDecreaseGuildUnit;
	CItemHeader*				 pItemHeader = NULL;
	MyItemData					 *pAddItemData = NULL;
	MyItemData					 *pMyItemData = NULL;

	pOnReqDecreaseGuildUnit		=	(OnReqDecreaseGuildUnit*)cMsg;

	if((pclGuild =	pMyWorld->clIGuild.GetGuildByID(uiGuildID)) == NULL )								return;

	//가져가는 아이템.
	UI16 uiItemID = pMyWorld->m_FieldBattleParser.GetItemID(pOnReqDecreaseGuildUnit->siKind);
	pItemHeader = pMyWorld->clItem.GetItem(uiItemID);

	//아이템이 저장 가능한 위치를 검색한후 인벤토리에 저장시켜준다.
	for(SI16 i = 0 ;i < ON_MAX_FOLLOWER_NUM ; i++)
	{
		if(IFollower.IsValidFollower(i) == TRUE)
		{
			 pAddItemData = IFollower.GetItemByID(i, uiItemID); 

			 if(pAddItemData)	//해당 아이템을 가지고 있다.
			 {
                //AddItemNoQuery 인자.
				//1. MercenaryID 2. ItemID 3. Quantity 4. InventorySlot
				if( (pMyItemData = AddItemNoQuery(i, uiItemID, pOnReqDecreaseGuildUnit->siCount, pAddItemData->siPosInInventory)) != NULL)
				{
					DBIKDecreaseGuildUnit					DecreaseGuildUnit;
					DecreaseGuildUnit.siDBAccount		=	stAccountInfo.siDBAccount;
					DecreaseGuildUnit.uiDBSlot			=	stAccountInfo.uiSlot;	
					DecreaseGuildUnit.uiGuildID			=	uiGuildID;
//					DecreaseGuildUnit.uiItemID			=	pMyItemData->uiID;
					DecreaseGuildUnit.siQuantity		=	pOnReqDecreaseGuildUnit->siCount;
//					DecreaseGuildUnit.uiMercenarySlot	=	i;
//					DecreaseGuildUnit.siInventoryPos	=	pMyItemData->siPosInInventory;

					//가져가고 남은 아이템의 총갯수.
					siTotalGuildUnitCount = pclGuild->m_GuildUnit.DecreaseGuildUnit(pOnReqDecreaseGuildUnit->siKind,pOnReqDecreaseGuildUnit->siCount);


					pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_DECREASE_GUILD_UNIT, LPVOID(&DecreaseGuildUnit), sizeof(DBIKDecreaseGuildUnit));

					//pAddItemData->siPosInInventory = pMyItemData->siPosInInventory;
					//pMyItemData->uiID				=	uiItemID;
					pMyItemData->uiQuantity		=	pOnReqDecreaseGuildUnit->siCount;
					SendFieldMsg(ON_RESPONSE_DECREASE_GUILD_UNIT, SO_SFM_ONLYME,
						LPVOID(ON_RET_OK),
						LPVOID(pOnReqDecreaseGuildUnit->siKind),
						LPVOID(siTotalGuildUnitCount),
						LPVOID(pMyItemData),
						LPVOID(i));
					return;
				}
			 }
			 else	//가지고 있지 않다.
			 {
				 //
				 siTempItemSlot = IFollower.FindEmptyItemBuffer(i);
				 if((siTempItemSlot != -1) && (siTempItemSlot != 0xffff))
				 {
					//빈칸을 찾아서 가지고 있다면 추가.
					if( (pMyItemData = AddItemNoQuery(i,uiItemID, pOnReqDecreaseGuildUnit->siCount, siTempItemSlot)) != NULL) 
					{
						DBIKDecreaseGuildUnit					DecreaseGuildUnit;
						DecreaseGuildUnit.siDBAccount		=	stAccountInfo.siDBAccount;
						DecreaseGuildUnit.uiDBSlot			=	stAccountInfo.uiSlot;	
						DecreaseGuildUnit.uiGuildID			=	uiGuildID;
//						DecreaseGuildUnit.uiItemID			=	pMyItemData->uiID;
						DecreaseGuildUnit.siQuantity		=	pOnReqDecreaseGuildUnit->siCount;
//						DecreaseGuildUnit.uiMercenarySlot	=	i;
//						DecreaseGuildUnit.siInventoryPos	=	pMyItemData->siPosInInventory;

						//가져가고 남은 아이템의 총갯수.
						siTotalGuildUnitCount = pclGuild->m_GuildUnit.DecreaseGuildUnit(pOnReqDecreaseGuildUnit->siKind,pOnReqDecreaseGuildUnit->siCount);


						pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_DECREASE_GUILD_UNIT, LPVOID(&DecreaseGuildUnit), sizeof(DBIKDecreaseGuildUnit));

						MyItemData					stAddItemData;

						stAddItemData.siPosInInventory	=	pMyItemData->siPosInInventory;
						stAddItemData.uiID				=	pMyItemData->uiID;
						stAddItemData.uiQuantity		=	pMyItemData->uiQuantity;
						SendFieldMsg(ON_RESPONSE_DECREASE_GUILD_UNIT, SO_SFM_ONLYME,
							LPVOID(ON_RET_OK),
							LPVOID(pOnReqDecreaseGuildUnit->siKind),
							LPVOID(siTotalGuildUnitCount),
							LPVOID(&stAddItemData),
							LPVOID(i));
						return;
					}
				 }
			 }
		}
	}	

	SendFieldMsg(ON_RESPONSE_DECREASE_GUILD_UNIT, SO_SFM_ONLYME,
		LPVOID(ON_RET_NO));
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ON_REQUEST_LIST_GUILD_UNIT
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//상단의 공성유닛의 리스트를 얻어온다. 
//장부와 상단 인터페이스에서 사용. 
//cltGuild::cltGuildUnit의 내용을 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvGetGuildUnitList()
{
	cltGuild*				    pclGuild;
	OnReqGetGuildUnitList*		pOnReqGetGuildUnitList;
	UI32						uiTempPopulation = 0;

	pOnReqGetGuildUnitList		=	(OnReqGetGuildUnitList*)cMsg;

	if((pclGuild =	pMyWorld->clIGuild.GetGuildByID(uiGuildID)) == NULL )	return;

	//유닛 생성시도 사용하기 때문에 소집할수 있는 마을의 인구수를 보내준다.
	if(stAccountInfo.pVillage)
	{
		UI32 uiMaxPopu = stAccountInfo.pVillage->uiMaxPopulation;
		UI32 uiPopu = stAccountInfo.pVillage->uiPopulation;
		UI32 uiLoadQuantity = uiPopu * 0.3;

		uiTempPopulation = uiPopu - stAccountInfo.pVillage->m_uiLimitMinPopulation;
		if(uiTempPopulation > uiLoadQuantity)
			uiTempPopulation = uiLoadQuantity;

//		uiTempPopulation = uiLoadQuantity - (uiMaxPopu - uiPopu);

//		if(uiPopu - stAccountInfo.pVillage->m_uiLimitMinPopulation < uiTempPopulation)
//			uiTempPopulation = uiTempPopulation - (uiPopu - stAccountInfo.pVillage->m_uiLimitMinPopulation);
		
		/*if(stAccountInfo.pVillage->uiPopulation - stAccountInfo.pVillage->m_uiLimitMinPopulation < uiTempPopulation)
		{
			 uiTempPopulation = uiTempPopulation - (stAccountInfo.pVillage->uiPopulation - stAccountInfo.pVillage->m_uiLimitMinPopulation);
		}
		*/
	}

	SendFieldMsg(ON_RESPONSE_LIST_GUILD_UNIT,SO_SFM_ONLYME, LPVOID(pclGuild), LPVOID(uiTempPopulation));
}


/*
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ON_REQUEST_INCREASE_GUILD_SUPPLY
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//상단이 가지고 있는 보급품을 증가 시킨다.
//cltGuild::m_siGuildSupply에 저장한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvIncreaseGuildSupply()
{
	cltGuild*					 pclGuild;
	MONEY						 uiMoney = 0;
	DWORD						 dwResponse = ON_RET_OK;
	SI32						 siSupplyPrice = 125;		//보급품의 가격.

	OnReqIncreaseGuildSupply* pOnReqIncreaseGuildSupply = (OnReqIncreaseGuildSupply*)cMsg;

	if((pclGuild =	pMyWorld->clIGuild.GetGuildByID(uiGuildID)) == NULL )			dwResponse = ON_RET_NO;
	if(GetMoney() < (siSupplyPrice * pOnReqIncreaseGuildSupply->siSupplyQuantity))	dwResponse = ON_RET_NO;

	//Guild 보급품 갯수를 증가 시킨다.
	pclGuild->IncreaseGuildSupply(pOnReqIncreaseGuildSupply->siSupplyQuantity);
	//해당 제품의 돈을 얻어온다.
	uiMoney = siSupplyPrice * pOnReqIncreaseGuildSupply->siSupplyQuantity;
	DecreaseMoney(uiMoney,FALSE);

	//Guild보급품의 총 갯수 와 유저에게 뺀 돈을 디비에 저장.
	if(dwResponse == ON_RET_OK)
	{
		DBIKMakeGuildSupply					MakeGuildSupply;
		MakeGuildSupply.siDBAccount		=	stAccountInfo.siDBAccount;
		MakeGuildSupply.uiDBSlot		=	stAccountInfo.uiSlot;
		MakeGuildSupply.uiGuildID		=	uiGuildID;
		MakeGuildSupply.siQuantity		=	pOnReqIncreaseGuildSupply->siSupplyQuantity;
		MakeGuildSupply.mnMoney			=	uiMoney;

		pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_INCREASE_SUPPLY, LPVOID(&MakeGuildSupply), sizeof(DBIKMakeGuildSupply));
	}

	SendFieldMsg( ON_RESPONSE_INCREASE_GUILD_SUPPLY, SO_SFM_ONLYME, LPVOID(dwResponse),
		LPVOID(uiMoney),LPVOID(pOnReqIncreaseGuildSupply->siSupplyQuantity));
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ON_REQUEST_DECREASE_GUILD_SUPPLY
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//보급수레가 상단의 보급품을 가져간다.
//보급수레가 마을을 클릭해서 상단의 보급품을 가져간다.
//보급수레의 보급품량 증가 상단의 보급품량 감소.공성중에만 가능하다.
//cltGuild::m_siGuildSupply 감소.
//SOPlayer::m_siSupplyCharge 증가.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvDecreaseGuildSupply()
{
	cltGuild*					pclGuild;
	MONEY						uiMoney				 = 0;
	DWORD						dwResponse			 = ON_RET_OK;
	SOVillage*					pVillage			 =	NULL;
	SI32						siTempSupplyQuantity = 0;
	

	OnReqDecreaseGuildSupply* pOnReqDecreaseGuildSupply = (OnReqDecreaseGuildSupply*)cMsg;

	pVillage = pMyWorld->GetVillageByCode(pOnReqDecreaseGuildSupply->siVillageCode);

	//마을의 아이디와 자신의 길드아이디가 동일 해야한다.
	if(pVillage->uiGuildID != uiGuildID)											dwResponse = ON_RET_NO;
	if((pclGuild = pMyWorld->clIGuild.GetGuildByID(uiGuildID)) == NULL )			dwResponse = ON_RET_NO;
	if(pclGuild->GetGuildSupply() == 0)												dwResponse = ON_RET_NO;
	if(pMyWorld->clIGuild.GetGuildWarCheck(uiGuildID) == FALSE)						dwResponse = ON_RET_NO;


	if(dwResponse == ON_RET_OK)
	{
		siTempSupplyQuantity = pOnReqDecreaseGuildSupply->siSupplyQuantity;
		
		if(pclGuild->GetGuildSupply() > siTempSupplyQuantity) 
		{
			//Guild 보급품 갯수를 감소 시킨다.
			pclGuild->DecreaseGuildSupply(siTempSupplyQuantity);
			//보급수레에게만 넘길수 있는 충전용 보급게이지.
			SetFieldSupplyCharge(siTempSupplyQuantity);
		}
		else
		{
			siTempSupplyQuantity = pclGuild->GetGuildSupply();

			pclGuild->DecreaseGuildSupply(siTempSupplyQuantity);
			SetFieldSupplyCharge(siTempSupplyQuantity);
		}

	}

	//남은 Guild보급품 과 유저에게 간 보급품 디비에 저장.
	if(dwResponse == ON_RET_OK)
	{
		DBIKDecreaseGuildSupply					DecreaseGuildSupply;
		DecreaseGuildSupply.siDBAccount		=	stAccountInfo.siDBAccount;
		DecreaseGuildSupply.uiDBSlot		=	stAccountInfo.uiSlot;
		DecreaseGuildSupply.uiGuildID		=	uiGuildID;
		DecreaseGuildSupply.siQuantity		=	siTempSupplyQuantity;

		pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_DECREASE_SUPPLY, LPVOID(&DecreaseGuildSupply), sizeof(DBIKDecreaseGuildSupply));
	}

	SendFieldMsg( ON_RESPONSE_DECREASE_GUILD_SUPPLY, SO_SFM_ONLYME, LPVOID(dwResponse),
		LPVOID(siTempSupplyQuantity));
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ON_REQUEST_GET_SUPPLY_AT_GUILD_UNIT
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//공성유닛이 보급수레로부터 보급품을 얻어온다.
//공성유닛이 보급수레를 클릭 보급수레가 소지 하고 있는 보급품을 공성유닛이 가져간다.
//서버에서는 유닛이 가져간 초기값만을 기준으로 계산하고
//클라이언트가 가져간 보급품은 서버와 관계없이 클라이언트에서 처리한다.
//보급수레(pTargetPlayer)의 SOPlayer::m_siSupplyCharge 감소.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvGetSupplyAtGuildUnit()
{
	OnReqGetSupplyAtGuildUnit*			pOnReqGetSupplyAtGuildUnit;
	SOPlayer*							pTargetPlayer;
	OnlineFollower*						pFollower = IFollower.GetFollower(0);
	OnlineFollower*						pTargetFollower;
	SI16								siResSupplyAmount;

	pOnReqGetSupplyAtGuildUnit			=	(OnReqGetSupplyAtGuildUnit*)cMsg;

	pTargetPlayer	= pMyWorld->GetPlayer(pOnReqGetSupplyAtGuildUnit->uiTargetID);

	pTargetFollower = pTargetPlayer->IFollower.GetFollower(0);

	if(pTargetPlayer)
	{
		if(uiGuildID == 0)					return;
		if(pTargetPlayer->uiGuildID	== 0)	return;

		//같은 상단의 소속이 확실하면.
		if(uiGuildID == pTargetPlayer->uiGuildID)
		{
			//공성 유닛이 맞는가를 확인한다.
			if(pTargetPlayer->IsFieldAttackUnit() == FALSE)								return;
			if(IsFieldAttackUnit()				  == FALSE)								return;
			//선택한 유닛이 보급수레가 맞는가를 확인한다.
			if(pMyWorld->m_FieldBattleParser.GetUnitKind(pTargetPlayer->uiKind) != FIELD_BATTLE_SUPPLY)	return;
			//자신이 보급수레이면서 보급수레에게 충전 할수 없다.
			if(pMyWorld->m_FieldBattleParser.GetUnitKind(uiKind)		 == FIELD_BATTLE_SUPPLY)	return;
			//보급수레가 가지고 있는 보급게이지가 충전되어 있는지를 확인한다.
			if(pTargetPlayer->GetFieldSupplyCharge() <= 0  )							return;
			
			siResSupplyAmount = pOnReqGetSupplyAtGuildUnit->siRequireSupply;

			if( pTargetPlayer->GetFieldSupplyCharge() >= siResSupplyAmount)
			{
				pTargetPlayer->DecreaseSupplyCharge((SI32)siResSupplyAmount);
			}	
			else
			{
				siResSupplyAmount  = pTargetPlayer->GetFieldSupplyCharge();
				pTargetPlayer->DecreaseSupplyCharge((SI32)pTargetPlayer->GetFieldSupplyCharge());
			}
			
			SendFieldMsg(ON_RESPONSE_GET_SUPPLY_AT_GUILD_UNIT,SO_SFM_MEANDNEARCHAR,
				LPVOID(uiAccount),								// PlayerID
				LPVOID(siResSupplyAmount),						// Player가 가져간 Supply양.
				LPVOID(pOnReqGetSupplyAtGuildUnit->uiTargetID), // TargetPlayerID
				LPVOID(pTargetPlayer->GetFieldSupplyCharge()));	// TargetPlayer가 가지고 있는 Supply양.
		}
	}
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ON_REQUEST_LIST_GUILD_SUPPLY
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//상단이 가지고 있는 보급품의 갯수를 얻어온다.
//마을의 공성관련인터페이스에서 호출하지만 보급품은 상단에 저장되므로 
//cltGuild::m_siGuildSupply를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvGetGuildSupplyList()
{
	OnReqGetGuildSupplyList*	pOnReqGetGuildSupplyList;
	cltGuild*					pclGuild	  =	NULL;
	DWORD						dwResponse    = ON_RET_OK;
	SI32						siGuildSupply =	0; 

	pOnReqGetGuildSupplyList  = (OnReqGetGuildSupplyList*)cMsg;

	if((pclGuild = pMyWorld->clIGuild.GetGuildByID(uiGuildID)) == NULL )		dwResponse = ON_RET_NO;

	siGuildSupply = pclGuild->GetGuildSupply();

	SendFieldMsg(ON_RESPONSE_LIST_GUILD_SUPPLY, SO_SFM_ONLYME, LPVOID(dwResponse),
		LPVOID(siGuildSupply));
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ON_REQUEST_INCREASE_VILLAGE_DEFENCE_SOLDIER
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//마을의 수비병 수를 증가 시킨다.
//마을 공성 인터페이스에서 공성유닛과 수비병을 함께 모집하지만
//공성유닛은		cltGuild::cltGuildUnit 에
//마을 수비병수는	SOVillage::m_siDefenceSoldier 에 저장된다.
//마을 수비병을 증가시킬때 상단의 소집권을 감소시키고 마을의 인구수를 뺀다.
//마을 수비병은 각 마을마다 존재한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvIncreaseVillageDefenceSoldier()
{
	OnReqVillageDefenceSoldier*	pOnReqVillageDefenceSoldier;
	cltGuild*					pclGuild;
	DWORD						dwResponse = ON_RET_OK;
	CItemHeader*				pItemHeader = NULL;
	MONEY						mnMoney = 0;
	SOVillage*					pVillage	  =	NULL;
	SI32						siSupply = 0;
	SI32						siDefenceSoldier = 0;
	

	pOnReqVillageDefenceSoldier = (OnReqVillageDefenceSoldier*)cMsg;

	pVillage = pMyWorld->GetVillageByCode(pOnReqVillageDefenceSoldier->siVillageCode);

	if((pclGuild = pMyWorld->clIGuild.GetGuildByID(uiGuildID)) == NULL )	
	{
		dwResponse = ON_RET_NOT;
		goto END;
	}
	
	//Guild 보급품의 총 갯수를 감소  유저에게 뺸돈을 디비에 저장.
	if(pVillage)
	{
		UI16 uiItemID = pMyWorld->m_FieldBattleParser.GetItemID(FIELD_BATTLE_DEFENSE);
		pItemHeader = pMyWorld->clItem.GetItem(uiItemID);


		//마을 수비병수를 증가 시킨다.
		//수비병 * 수비병 Hp
		siDefenceSoldier = (SI32)(pOnReqVillageDefenceSoldier->siDefenceSoldierCount*pItemHeader->m_pImperium->m_siHp);
		
		//길드의 보급품수를 감소 시킨다.
		//수비병 * 1부대당 보급량.
		siSupply = (SI32)(pItemHeader->m_pImperium->m_siSupply*pOnReqVillageDefenceSoldier->siDefenceSoldierCount);

		if(pclGuild->GetGuildSupply() < siSupply)
		{
			dwResponse = ON_RET_NOT_ENOUGH_SUPPLY;
			goto END;
		}

		mnMoney = (MONEY)(pItemHeader->m_pImperium->m_siPrice * pItemHeader->m_pImperium->m_siPopulation * pOnReqVillageDefenceSoldier->siDefenceSoldierCount);

		//돈을 감소시칸다.
		if(EnoughMoney(mnMoney) == FALSE)
		{
			dwResponse = ON_RET_NOT_ENOUGH_MONEY;
			goto END;
		}
		//하루에 생산할수 있는 최대 생산량을 넘었나 체크.
		if(pVillage->CheckDecreasePopulation((UI32)(pOnReqVillageDefenceSoldier->siDefenceSoldierCount * pItemHeader->m_pImperium->m_siPopulation)) == FALSE)
		{
			dwResponse = ON_RET_NOT_ENOUGH_POPULATION;
			goto END;
		}
		//마을의 최소 인구수를 제한한다.
		if(pVillage->LimitDecreasePopulation((UI32)(pOnReqVillageDefenceSoldier->siDefenceSoldierCount * pItemHeader->m_pImperium->m_siPopulation)) == FALSE)
		{
			dwResponse = ON_RET_NOT_LIMIT_POPULATION;
			goto END;
		}

		//길드 보급품수를 줄인다.
		pclGuild->DecreaseGuildSupply(siSupply);
		//대방이 가지고 있는 돈을 줄인다.
		DecreaseMoney(mnMoney,FALSE);
		//마을 인구를 감소시킨다.
		pVillage->DecreasePopulation((UI32)(pOnReqVillageDefenceSoldier->siDefenceSoldierCount * pItemHeader->m_pImperium->m_siPopulation));
		//마을 수비병 수를 증가 시킨다.
		pVillage->IncreaseDefenceSoldier(siDefenceSoldier);

		DBIKIncreaseSoldierDefence				IncreaseSoldierDefence;
		IncreaseSoldierDefence.siDBAccount		=	stAccountInfo.siDBAccount;
		IncreaseSoldierDefence.uiDBSlot			=	stAccountInfo.uiSlot;
		IncreaseSoldierDefence.uiGuildID		=	uiGuildID;
		IncreaseSoldierDefence.siVillageCode	=	pOnReqVillageDefenceSoldier->siVillageCode;
		IncreaseSoldierDefence.siDefenceSoldier =	siDefenceSoldier;
		IncreaseSoldierDefence.siSupply			=	siSupply;
		IncreaseSoldierDefence.mnMoney			=	mnMoney;
		IncreaseSoldierDefence.uiPopulation		=	(UI32)pOnReqVillageDefenceSoldier->siDefenceSoldierCount * pItemHeader->m_pImperium->m_siPopulation;
	
		pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_INCREASE_DEFENCESOLDIER, LPVOID(&IncreaseSoldierDefence), sizeof(DBIKIncreaseSoldierDefence));
	}

END:
	if(dwResponse == ON_RET_OK)
	{
		SendFieldMsg(ON_RESPONSE_INCREASE_VILLAGE_DEFENCE_SOLDIER,SO_SFM_ONLYME,LPVOID(dwResponse),
			LPVOID(siDefenceSoldier),LPVOID(siSupply),LPVOID(mnMoney));
	}
	else
	{
		SendFieldMsg(ON_RESPONSE_INCREASE_VILLAGE_DEFENCE_SOLDIER,SO_SFM_ONLYME,LPVOID(dwResponse),
			LPVOID(0),LPVOID(0),LPVOID(0));
	}
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ON_REQUEST_GET_VILLAGE_DEFENCE_SOLDIER
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//마을 수비병 수를 얻어온다.
//SOVillage::m_siDefenceSoldier
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvGetVillageDefenceSoldier()
{
	OnReqGetVillageDefenceSoldier*	pOnReqVillageDefenceSoldier;
	SOVillage*						pVillage	  =	NULL;

	pOnReqVillageDefenceSoldier = (OnReqGetVillageDefenceSoldier*)cMsg;

	pVillage = pMyWorld->GetVillageByCode(pOnReqVillageDefenceSoldier->siVillageCode);

	if(pVillage)
	{
		SI32 siDefenceSoldier = pVillage->GetDefenceSoldier();	

		SendFieldMsg(ON_RESPONSE_GET_VILLAGE_DEFENCE_SOLDIER,SO_SFM_ONLYME,LPVOID(siDefenceSoldier));
	}
	else
	{
		SendFieldMsg(ON_RESPONSE_GET_VILLAGE_DEFENCE_SOLDIER,SO_SFM_ONLYME,LPVOID(0));
	}
}
*/

//플레이어에게 힐을 해준다.
VOID SOPlayer::RecvHeal()
{
	OnReqGetSupplyAtGuildUnit*			pOnReqGetSupplyAtGuildUnit;
	SOPlayer*							pTargetPlayer;
	OnlineFollower*						pFollower = IFollower.GetFollower(0);
	OnlineFollower*						pTargetFollower = NULL;
	SI16								siHealAmount;

	pOnReqGetSupplyAtGuildUnit			=	(OnReqGetSupplyAtGuildUnit*)cMsg;
	pTargetPlayer	= pMyWorld->GetPlayer(pOnReqGetSupplyAtGuildUnit->uiTargetID);
	if(pTargetPlayer == NULL) return;

	pTargetFollower = pTargetPlayer->IFollower.GetFollower(0);
	if(pTargetFollower == NULL) return;

	if(pTargetPlayer)
	{
		if(uiGuildID == 0)					return;
		if(pTargetPlayer->uiGuildID	== 0)	return;

		//같은 상단의 소속이 확실하면.
		if(uiGuildID == pTargetPlayer->uiGuildID)
		{
			//공성 유닛이 맞는가를 확인한다.
			if(pTargetPlayer->IsFieldAttackUnit() == FALSE)	return;
			if(IsFieldAttackUnit()				  == FALSE)	return;
			
			if(FieldAttackDead() || pTargetPlayer->FieldAttackDead()) return;

			//자신이 보급수레이면서 보급수레에게 충전 할수 없다.
//			if(pMyWorld->m_FieldBattleParser.GetUnitKind(pTargetPlayer->uiKind) == FIELD_BATTLE_SUPPLY)	return;
//			if(CSiegeUnitTable::GetInstance()->GetKindUnitCode(pTargetPlayer->uiKind) == FIELD_BATTLE_SUPPLY) return;
			//보급수레가 가지고 있는 보급게이지가 충전되어 있는지를 확인한다.
			int nMp = GetFieldMP();			
			if(nMp <= 0 ) return;

			int nHealAmount = pTargetPlayer->GetFieldMaxHP() - pTargetPlayer->GetFieldHp();
		
//			if(nTargetHP < 0 || nTargetHP >= pTargetPlayer->GetFieldMaxHP()) return; 

			if(nHealAmount <= 0) return;
			
			//힐해줄양을 계산한다.
			if(nMp < 100)
			{
				siHealAmount = nMp;
				
				if(siHealAmount > nHealAmount)
					siHealAmount = nHealAmount;
			}
			else
			{
				if(nHealAmount < 100)
					siHealAmount = nHealAmount;
				else
					siHealAmount = 100;
			}
			
			DecreaseFieldMP(siHealAmount);
			pTargetPlayer->IncreaseFieldHp(siHealAmount);


			SendFieldMsg(ON_RESPONSE_GET_SUPPLY_AT_GUILD_UNIT, SO_SFM_MEANDNEARCHAR,
				         LPVOID(siHealAmount),						    // Player가 힐해준양
				         LPVOID(GetFieldMP()),                           // 남은 마나양 
				         LPVOID(pOnReqGetSupplyAtGuildUnit->uiTargetID), // TargetPlayerID
				         LPVOID(pTargetPlayer->GetFieldHp()));           // TargetPlayer의 현재 hp

		}
	}
}

VOID	SOPlayer::RecvChargeMP()
{
	DWORD  dwRes = 1;
	DWORD  dwNowTick = GetTickCount();
	SI16   siUnitCode = -1;

	if(uiGuildID == 0)					                 goto END;

	if(CSiegeUnitTable::GetInstance()->GetKindUnitCode(uiKind) != FIELD_BATTLE_SUPPLY) goto END;
	if( dwNowTick - m_dwChargeTime < 10)            goto END;

	IncreaseFieldMP(10);
	m_dwChargeTime = dwNowTick;
	
	dwRes = 0;

END:
	SendFieldMsg(ON_RESPONSE_CHARGE_MP, SO_SFM_ONLYME, LPVOID(dwRes), LPVOID(GetFieldMP()));
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ON_REQUEST_GUILD_SET_RELATION
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//상단과의 적대 관계를 설정한다.
//cltGuild::m_siGuildRelation,cltGuild::m_siGuildRelationEnemyTime 을 설정한다.
//상단간의 관계를 서로 설정.적대 건쪽 적대 받은쪽 양쪽다 적대 상태이다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvGuildSetRelation()
{
	OnReqGuildSetRelationMsg	*pGuildSetRelationMsg;
	BOOL						bSuccess = FALSE;
	SI32						siRelation;
	BOOL						bRelation = FALSE;
	
	// 대방인지 체크한다.
	if(uiGuildID == 0)	goto	END;

	if(siClassInGuild != pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))	goto	END;

	pGuildSetRelationMsg	=	(OnReqGuildSetRelationMsg*)cMsg;

	siRelation				=	pMyWorld->clIGuild.ResponseDefineToRelationDefine(pGuildSetRelationMsg->dwResponse);
	
	//양쪽 상단간의 적대를 설정한다.
	if(pMyWorld->clIGuild.SetRelation(uiGuildID, pGuildSetRelationMsg->szGuildID, siRelation, FALSE,0) == FALSE)
		goto	END;

	bSuccess	=	TRUE;
END:
	if(bSuccess == TRUE)
	{
		SendFieldMsg(ON_RESPONSE_GUILD_SET_RELATION, SO_SFM_ONLYME, LPVOID(ON_RET_OK),LPVOID(bRelation));	
	}
	else
	{
		SendFieldMsg(ON_RESPONSE_GUILD_SET_RELATION, SO_SFM_ONLYME, LPVOID(ON_RET_NO),LPVOID(bRelation));
	}
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ON_REQUEST_GUILD_FRIELDLY_MESSAGE_OFFER
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//화친제의 메세지를 보낸다.
//화친제의 메세지를 보내는 상단의 메세지가 
//화친제의 받는 상단의 cltGuild::m_szGuildfriendlyMessage에 저장된다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvFriendlyMessageOffer()
{	
	OnReqGuildFriendlyMessageOffer*	TempOnReqGuildFriendlyMessageOffer;
	DWORD							dwResponse;

	TempOnReqGuildFriendlyMessageOffer		=	( OnReqGuildFriendlyMessageOffer* )cMsg;

	//공성하는 날인지 체크한다.
/*	SYSTEMTIME  sysTime;
	GetSystemTime(&sysTime);
	if(sysTime.wDayOfWeek == 2|| sysTime.wDayOfWeek == 4)
	{
		dwResponse = ON_RET_NO;
		SendFieldMsg(ON_RESPONSE_GUILD_FRIELDLY_MESSAGE_OFFER, SO_SFM_ONLYME, LPVOID(dwResponse));
		return;
	}
*/
	//공성하는 요일에 시간이면 화친제의를 할수없다.
	if(pMyWorld->m_WarTimeParser.IsWarCheck(pMyWorld->pRealTime->tm_wday, pMyWorld->pRealTime->tm_hour)) return;
//	if(pMyWorld->m_WarTimeParser.IsWarTime(pMyWorld->pRealTime->tm_hour)) return;
	
	if(pMyWorld->clIGuild.SetFriendlyMessage(uiGuildID,TempOnReqGuildFriendlyMessageOffer->szGuildID,TempOnReqGuildFriendlyMessageOffer->szMessage) == FALSE)
		dwResponse = ON_RET_NO;
	else 
		dwResponse = ON_RET_OK;	
	
	SendFieldMsg(ON_RESPONSE_GUILD_FRIELDLY_MESSAGE_OFFER, SO_SFM_ONLYME, LPVOID(dwResponse));
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ON_REQUEST_GUILD_FRIENDLY_MESSAGE_ACCEPT
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//화친제의를 받은 상단에서 화친제의를 허락한다.
//상단간의 관계 초기화 된다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvFriendlyMessageAccept()
{
	OnReqGuildFriendlyMessageAccept*	TempOnReqGuildFriendlyMessageAccept;

	TempOnReqGuildFriendlyMessageAccept		=	( OnReqGuildFriendlyMessageAccept* )cMsg;

	pMyWorld->clIGuild.SetFriendlyMessageAccept(uiGuildID,TempOnReqGuildFriendlyMessageAccept->szGuildID);

	SendFieldMsg(ON_RESPONSE_GUILD_FRIENDLY_MESSAGE_ACCEPT, SO_SFM_ONLYME);
} 



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ON_REQUEST_GUILD_FRIENDLY_MESSAGE_CANCEL
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//화친제의를 받은 상단에서 화친제의를 거절한다.
//화친제의를 받은 상단의 msg만 삭제된다.
//적대 관계는 그대로 남아 있다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvFreidnlyMessageCancel()
{
	OnReqGuildFreindlyMessageCancel*	TempOnReqGuildFreindlyMessageCancel;

	TempOnReqGuildFreindlyMessageCancel		=	( OnReqGuildFreindlyMessageCancel* )cMsg;

	pMyWorld->clIGuild.SetFriendlyMessageCancel(uiGuildID,TempOnReqGuildFreindlyMessageCancel->szGuildID);

	SendFieldMsg(ON_RESPONSE_GUILD_FRIENDLY_MESSAGE_CANCEL, SO_SFM_ONLYME);
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ON_REQUEST_GUILD_VILLAGEATTACK_RESULT
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//공성이 끝난후 상단의 공성 결과 폭투 할수 있는 권한을 가진 마을을 요청한다.
//cltGuild::m_stGuildResult 저장되어 있는 값.
//이 값은 마을 파괴시 해당 상단에 저장된다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvVillageAttackResult()
{
	OnReqVillageAttackResult*			pOnReqVillageAttackResult;
	cltGuild*							pclGuild;
	DWORD								dwResponse = ON_RET_OK;
	SI16								siVillageCount = 0;

	pOnReqVillageAttackResult		=	(OnReqVillageAttackResult*)cMsg;

	//자기의 GuildID	= uiGuildID
	if((pclGuild = pMyWorld->clIGuild.GetGuildByID(uiGuildID)) == NULL)	dwResponse = ON_RET_NO;

	for(SI16 i = 0; i < ON_MAX_GUILD_RESULT; i++)
	{
		if(pclGuild->m_stGuildResult[i].GetVillageCode() != 0)
		{
			if(pclGuild->m_stGuildResult[i].szCharName[0] == NULL)
				siVillageCount++;
		}
	}

	if(siVillageCount == 0)												dwResponse = ON_RET_NO;

	SendFieldMsg(ON_RESPONSE_GUILD_VILLAGEATTACK_RESULT,SO_SFM_ONLYME,LPVOID(dwResponse),LPVOID(siVillageCount),LPVOID(pclGuild->m_stGuildResult));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ON_REQUEST_GUILD_CHOICE_PLAYER
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//대방이 가진 폭투 권한을 해당 상단의 플레이어에게 넘겨준다.
//cltGuild::m_stGuildResult에 해당하는 플레이어를 저장시켜준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvChoicePlayer()
{
	OnReqGuildChoicePlayer*				pOnReqGuildChoicePlayer;
	SOPlayer*							pOpponentPlayer;
	cltGuild*							pclGuild;
	DWORD								dwResponse = ON_RET_OK;
	SI16								i = 0;

	pOnReqGuildChoicePlayer			=	(OnReqGuildChoicePlayer*)cMsg;
	
	pOnReqGuildChoicePlayer->siVillageCode;

	if((pclGuild = pMyWorld->clIGuild.GetGuildByID(uiGuildID)) == NULL)	
	{
		dwResponse = ON_RET_NO;
		goto END;
	}
	if(( pOpponentPlayer	= pMyWorld->GetPlayer(pOnReqGuildChoicePlayer->szUserName)) == NULL)	
	{
		dwResponse = ON_RET_NO; 
		goto END;
	}

	for( i = 0; i < ON_MAX_GUILD_RESULT ; i++)
	{
		if(pclGuild->m_stGuildResult[i].GetVillageCode() == pOnReqGuildChoicePlayer->siVillageCode)
		{
			if(pclGuild->m_stGuildResult[i].szCharName[0] == NULL)
			{
				pclGuild->m_stGuildResult[i].SetCharName(pOnReqGuildChoicePlayer->szUserName);

				DBIKGuildResult		TempGuildResult;
				TempGuildResult.siVillageCode	=	pOnReqGuildChoicePlayer->siVillageCode;
				TempGuildResult.siPercent		=	pclGuild->m_stGuildResult[i].GetPercent();
				TempGuildResult.uiGuildID		=	uiGuildID;
				strcpy(TempGuildResult.szCharName,pclGuild->m_stGuildResult[i].GetCharName());

				pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_SET_GUILD_RESULT, LPVOID(&TempGuildResult), sizeof(DBIKGuildResult));
			}
			else
			{
				dwResponse = ON_RET_NO;
				goto END;
			}
		}
	}
END:
	
	switch(dwResponse)
	{
	case ON_RET_OK:
		//대방에게 알린다.
		SendFieldMsg(ON_RESPONSE_GUILD_CHOICE_PLAYER,SO_SFM_ONLYME,LPVOID(dwResponse),
			LPVOID(pOnReqGuildChoicePlayer->siVillageCode),LPVOID(pOnReqGuildChoicePlayer->szUserName));
		//투자권을 받는 플레이어에게 알린다.
		pOpponentPlayer->SendFieldMsg(ON_RESPONSE_GUILD_CHOICE_PLAYER,SO_SFM_ONLYME,LPVOID(dwResponse),
			LPVOID(pOnReqGuildChoicePlayer->siVillageCode),LPVOID(pOnReqGuildChoicePlayer->szUserName));
		break;
	case ON_RET_NO:
		//실패시 대방에게만 알린다.
		SendFieldMsg(ON_RESPONSE_GUILD_CHOICE_PLAYER,SO_SFM_ONLYME,LPVOID(dwResponse),LPVOID(pOnReqGuildChoicePlayer->siVillageCode)
			,LPVOID(pOnReqGuildChoicePlayer->szUserName));
		break;
	}	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//ON_REQUEST_VILLAGE_INVESTMENT_POWER
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//플레이어가 투자를 할 때 폭투권한이 있는 상태인가 서버에 물어본다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvInvestmentPower()
{
	OnReqInvestmentPower*				pOnReqInvestmentPower;
	SOVillage*							pVillage	  =	NULL;
	pOnReqInvestmentPower			=	(OnReqInvestmentPower*)cMsg;
	cltGuild*							pclGuild;
	DWORD								dwResponse = ON_RET_NO;
	SI16								siPercent = 0;

	pclGuild = pMyWorld->clIGuild.GetGuildByID(uiGuildID);

	m_bInvestmentPower = FALSE;

	//상단 투자권한이 있는지 검사.
	for(SI16 i = 0; i < ON_MAX_GUILD_RESULT; i++)
	{
		if(pclGuild->m_stGuildResult[i].GetVillageCode() == pOnReqInvestmentPower->siVillageCode)
		{
			if(strcmp(pclGuild->m_stGuildResult[i].GetCharName(),pOnReqInvestmentPower->szUserName) == 0)
			{
				dwResponse = ON_RET_OK;
				siPercent  = pclGuild->m_stGuildResult[i].GetPercent();
				//마을 투자시 제약 조건을 풀어주는 플래그를 On해준다.
				m_bInvestmentPower = TRUE;
			}
		}
	}

	SendFieldMsg(ON_RESPONSE_VILLAGE_INVESTMENT_POWER,SO_SFM_ONLYME,LPVOID(dwResponse),LPVOID(siPercent));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// ON_REQUEST_WAR_WARP
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 공성유닛이 일반 유닛을 워프 시키는 메세지.
// Response는 기존 Warp를 이용한다.
// ON_RESPONSE_WAR_WARP는 사용하지 않는다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvWarWarp()
{
	OnReqWarWarp*					pOnReqWarWarp;
	SOPlayer*						pTargetPlayer  = NULL;

	pOnReqWarWarp	=	(OnReqWarWarp*)cMsg;

	pTargetPlayer	=	pMyWorld->GetPlayer(pOnReqWarWarp->uiUniqueID);

	SI16 MapIndex = pMyWorld->m_WarpParser.GetMapPosition(pTargetPlayer->uiKind);

	if(MapIndex != -1)
		pTargetPlayer->Warp(pMyWorld->m_WarpParser.m_stWarpData[MapIndex].siMapIndex,
		pMyWorld->m_WarpParser.m_stWarpData[MapIndex].siX, pMyWorld->m_WarpParser.m_stWarpData[MapIndex].siY);
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// ON_REQUEST_NORMAL_CHAR
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 공성 유닛인 상태에서 일반유닛으로 변신한다.
// 자기 자신이 일반유닛으로 변신하는 메세지.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvNormalChar()
{
	OnReqNormalChar*		pOnReqNormalChar;
	OnlineFollower*			pFollower = IFollower.GetFollower(0);

	pOnReqNormalChar	=	(OnReqNormalChar*)cMsg;	

	ChangeNormalChar();
}

//공성전 유닛으로 변신한다.
VOID SOPlayer::RecvChangeUnit()
{
	OnReqChangeUnit        *pReqChangeUnit;
	pReqChangeUnit = (OnReqChangeUnit*)cMsg;

	SI16 siRes = ChangeUnit(pReqChangeUnit->siChangeKind, siClassInGuild);
	if(siRes != ON_CHANGE_OK)
		SendFieldMsg(ON_RESPONSE_CHANGE_WAR, SO_SFM_ONLYME, LPVOID(0), LPVOID(0), 
		             LPVOID(0), LPVOID(0), LPVOID(0),LPVOID(siRes));
	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 공성 관련 일반 함수들 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI16 SOPlayer::GetFieldHp()
{
	return m_siFieldHp;
}

void SOPlayer::SetFieldHp(SI16 siFieldHp)
{
	m_siFieldHp = siFieldHp;
}

SI16 SOPlayer::GetFieldMaxHP()
{
	return m_siFieldMaxHP;
}

void SOPlayer::SetFieldMaxHP(SI16 siMaxHP)
{
	if(siMaxHP <= 0) return;
	m_siFieldMaxHP = siMaxHP;
}

void SOPlayer::IncreaseFieldHp(SI16 siFieldHp)
{
	m_siFieldHp += siFieldHp;
}

void SOPlayer::DecreaseFieldHp(SI16 siFieldHp)
{
	if(siFieldHp >= m_siFieldHp)
		m_siFieldHp = 0;
	else
		m_siFieldHp -= siFieldHp;
}

BOOL SOPlayer::IsFieldAlive()
{
	if(m_siFieldHp > 0)	return TRUE;
	else				return FALSE;
}

/*
void SOPlayer::SetFieldSupply(SI32 siFieldSupply)
{
	m_siSupply = siFieldSupply;
}

SI32 SOPlayer::GetFieldSupply()
{
	return m_siSupply;
}

SI32 SOPlayer::GetFieldSupplyCharge()
{
	return m_siSupplyCharge;
}

void SOPlayer::InitFieldSupplyCharge(SI32 siFieldSuppyCharge)
{
	m_siSupplyCharge = siFieldSuppyCharge;
}

void SOPlayer::SetFieldSupplyCharge(SI32 siFieldSuppyCharge)
{
	m_siSupplyCharge += siFieldSuppyCharge;
}

void SOPlayer::DecreaseSupplyCharge(SI32 siDecreaseSupplyCharge)
{
	m_siSupplyCharge -= siDecreaseSupplyCharge;
}
*/
//보급수레인 경우 마나량을 가져온다
SI32 SOPlayer::GetFieldMP()
{
	return m_siFieldMP;
}
//보급수레인경우 마나량을 세팅한다.
void SOPlayer::SetFieldMP(SI32 siMP)
{
	if(siMP < 0) return;
	m_siFieldMP = siMP;
}


SI16 SOPlayer::GetFieldMaxMP()
{
	return m_siFieldMaxMP;
}

void SOPlayer::SetFieldMaxMP(SI32 siMaxMP)
{
	if(siMaxMP <= 0) return;
	m_siFieldMaxMP = siMaxMP;
}

void SOPlayer::IncreaseFieldMP(SI16 siFieldMP)
{
	if(siFieldMP <= 0) return;
	m_siFieldMP = m_siFieldMP + siFieldMP;
	
	if(m_siFieldMP > m_siFieldMaxMP)
		m_siFieldMP = m_siFieldMaxMP;
}

void SOPlayer::DecreaseFieldMP(SI16 siFieldMP)
{
	if(siFieldMP <= 0) return;
	
	if(siFieldMP > m_siFieldHp) 
		m_siFieldMP = 0;
	else
		m_siFieldMP = m_siFieldMP - siFieldMP;
}

void SOPlayer::SetPrevKind(UI16 uiPrevKind)
{
	m_uiPrevKind = uiPrevKind;
}

UI16 SOPlayer::GetPrevKind()
{
	return m_uiPrevKind;
}

void SOPlayer::SetCaptainUnit(BOOL bCaptain)
{
	m_bCaptainUnit = bCaptain;
}

BOOL SOPlayer::GetCaptainUnit()
{
	return m_bCaptainUnit;
}

BOOL SOPlayer::IsFieldAliveWarUnit()
{
	return m_bFieldAlive;
}


// 상단이 해체 되었을 때 캐릭터가 가지고 있는 공성 관련아이템을 삭제한다.
VOID	SOPlayer::SendGuildItemDeleteMessage()
{
//	if(pMyWorld->m_FieldBattleParser.IsFieldAttackUnit(uiKind) == TRUE)
	if(CSiegeUnitTable::GetInstance()->IsFieldAttackUnit(uiKind) == TRUE)
	{
		OnlineFollower*		lpOnlineFollower;

		lpOnlineFollower    = IFollower.GetFollower(0);
		uiKind				= GetPrevKind();
		lpOnlineFollower->uiCharID = GetPrevKind();
		SetFieldHp(0);
		SetPrevKind(GetPrevKind());
		SetCaptainUnit(FALSE);
		SetFieldMP(0);
//		SetFieldSupply(0);
//		InitFieldSupplyCharge(0);
		//OnlineFollower*		lpOnlineFollower;
		
		if (IsInVillage())  
			SendFieldMsg(ON_CHANGED_WAR_END,SO_SFM_ONLYME,LPVOID(GetPrevKind()),LPVOID(this));
		else
			SendFieldMsg(ON_CHANGED_WAR_END,SO_SFM_MEANDNEARCHAR,LPVOID(GetPrevKind()),LPVOID(this));
	}
	else
	{
		if (IsInVillage()) 
			SendFieldMsg(ON_CHANGED_WAR_END, SO_SFM_ONLYME, LPVOID(uiKind), LPVOID(this));
		else
			SendFieldMsg(ON_CHANGED_WAR_END, SO_SFM_ONLYME, LPVOID(uiKind), LPVOID(this));
	}

	for(SI16 i = 0 ;i < ON_MAX_FOLLOWER_NUM ; i++)
	{
		OnlineFollower*	 lpOnlineFollower = IFollower.GetFollower(i);
		
		if(lpOnlineFollower != NULL)
		{
			for(SI16 j = 0; j < FIELD_BATTLE_ATTACK_UNIT_NUM ; j++)
			{
				MyItemData*		pMyItemData;
				SI16 siItemID = (SI16)pMyWorld->m_FieldBattleParser.GetItemID(j);
				
				if((pMyItemData = lpOnlineFollower->GetItem(siItemID)) != NULL)
				{
					DelItemNoQuery(i,pMyItemData->uiID,pMyItemData->uiQuantity,pMyItemData->siPosInInventory);
				}
			}
		}
	}
}

//공성유닛으로 변신을 한다.[2005/01/14] - 최병주
UI16 SOPlayer::ChangeUnit(UI16 uiCode, UI16 uiQuantity)
{
	if(uiCode < 0) return ON_CHANGE_FAILED;
	if( pMyWorld->IsAttackVillageTimeZone() == FALSE )  return ON_CHANGE_FAILED;
	
	if(m_bIsWarUnit) return ON_CHANGE_FAILED;

	SI16 siUnitCode = CSiegeUnitTable::GetInstance()->GetKindUnitCode(uiCode);
	if(siUnitCode < 0) return ON_CHANGE_FAILED;
	
	OnlineFollower *pFollower = IFollower.GetFollower(0);
	_ImportantParameta* pParam  = &pFollower->ParametaBox.IP;

	//상단에 가입되어 있는지 검사한다.
	cltGuild *pclGuild;
	pclGuild = pMyWorld->clIGuild.GetGuildByID(uiGuildID);
	if(pclGuild == NULL) return ON_CHANGE_NOT_GUILD;
    
	//공성을 할수 있는 상단인지 검사한다.
	if(pclGuild->IsWar() == FALSE) return ON_CHANGE_NOT_GUILD;
	
	//지원자인지 검사....
	if(uiQuantity == pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ))
		return ON_CHANGE_NOT_GUILD;
	
	//레벨이 10보다 작으면 무조건 실패
	UI16 uiLevel = pParam->GetLevel();
	if(uiLevel < 10) 
		return ON_CHANGE_ENOUGH_LEVEL;
	

	//변신할수 있는 레벨을 알아온다.
	int nChangeLevel = CSiegeUnitTable::GetInstance()->GetLevel(siUnitCode);
	//레벨이 낮으면 변신 실패
	if(uiLevel < nChangeLevel)
		return ON_CHANGE_ENOUGH_LEVEL;

	
	//변신할 유닛의 개수가 남아 있는지 검사.
	SI16 siUnitCount = pclGuild->m_GuildUnit.GetGuildUnit(siUnitCode);
	if(siUnitCount <= 0) return ON_CHANGE_ENOUGH_KIND;
	
	SI16 siNeedCount = 0;	
	
	//유닛 수를 줄인다.
	if(uiQuantity == pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI)||
	   uiQuantity == pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS)) 
	{
		siNeedCount = 2;
	}
	else 
	{
		siNeedCount = 1;
	}

	if(siUnitCount < siNeedCount) return ON_CHANGE_ENOUGH_KIND;

	//변신하는데 필요한 비용
	int nChangeMoney = CSiegeUnitTable::GetInstance()->GetChangePrice(siUnitCode);
	nChangeMoney = nChangeMoney * siNeedCount;
	
	//변신 할수 있는 돈이 없으면 실패
	if(GetMoney() < nChangeMoney) 
		return ON_CHANGE_ENOUGH_MONEY;
	

//-----------------------------------------------------------------
//  캐릭터를 설정해주는 부분.....

	//변신 비용 만큼 돈을 뺀다.
	DecreaseMoney(nChangeMoney);
	
	pclGuild->m_GuildUnit.DecreaseGuildUnit(siUnitCode, siNeedCount);

	DBIKDecreaseGuildUnit					DecreaseGuildUnit;
	memset(&DecreaseGuildUnit, 0, sizeof(DBIKDecreaseGuildUnit));

	DecreaseGuildUnit.siDBAccount		=	stAccountInfo.siDBAccount;
	DecreaseGuildUnit.uiDBSlot			=	stAccountInfo.uiSlot;	
	DecreaseGuildUnit.uiGuildID			=	uiGuildID;
	DecreaseGuildUnit.uiItemID			=	uiCode;
	DecreaseGuildUnit.siQuantity		=	siNeedCount;//pclGuild->m_GuildUnit.GetGuildUnit(siUnitCode);
//	DecreaseGuildUnit.uiMercenarySlot	=	i;
//	DecreaseGuildUnit.siInventoryPos	=	;
	
	//가져가고 남은 아이템의 총갯수.
	pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_DECREASE_GUILD_UNIT, 
		                               LPVOID(&DecreaseGuildUnit), 
									   sizeof(DBIKDecreaseGuildUnit));

	//유닛의 속성을 변경한다.
	SetPrevKind(uiKind);
	uiKind = uiCode;
	pFollower->uiCharID = uiCode;
	
	//대장 유닛인지 설정한다.
	if(uiQuantity == pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI)||
	   uiQuantity == pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS))
		SetCaptainUnit(TRUE);
	else
		SetCaptainUnit(FALSE);

	//체력을 설정한다
	int BP = GetFightingPower();
	SI16 siHP = CSiegeUnitTable::GetInstance()->GetHP(siUnitCode, BP);
	
	//대방인 경우 2배의 체력을넣는다.
	if(uiQuantity == pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI)||
	   uiQuantity == pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS)) siHP *= 2; 
	
	SetFieldHp(siHP);
	SetFieldMaxHP(siHP);
	//보급 수레인경우 마나량을 설정한다.
	SI16 siMP = 0;
	if(siUnitCode == FIELD_BATTLE_SUPPLY)
	{
		siMP = CSiegeUnitTable::GetInstance()->GetMP(siUnitCode, BP);
		SetFieldMP(siMP);
		SetFieldMaxMP(siMP);
		m_dwChargeTime = GetTickCount();
	}		

	m_bIsWarUnit = TRUE;

	DBIKImperumUse ImperumUse;
	ImperumUse.siDBAccount	   = stAccountInfo.siDBAccount;
	ImperumUse.uiDBSlot        = stAccountInfo.uiSlot;
	ImperumUse.uiNewKind	   = uiCode;
	ImperumUse.bCaptain		   = m_bCaptainUnit;
	ImperumUse.siHp			   = siHP;
	ImperumUse.siMp		       = siMP;        // 일반 유닛은 보급 게이지를 0으로 한다.
//	ImperumUse.mnMoney         = nChangeMoney;

	pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_IMPERIUM_USE, LPVOID(&ImperumUse), sizeof(DBIKImperumUse));

	SendFieldMsg(ON_RESPONSE_CHANGE_WAR, SO_SFM_MEANDNEARCHAR, 
		         LPVOID(uiKind), 
				 LPVOID(m_bCaptainUnit), 
				 LPVOID(siHP), 
				 LPVOID(siMP), 
				 LPVOID(nChangeMoney),
				 LPVOID(ON_CHANGE_OK));

	return ON_CHANGE_OK;
}

UI16 SOPlayer::GetDir(SI32 siSX, SI32 siSY, SI32 siEX, SI32 siEY)
{
	UI16 uiDir = 0;
	if(siSX > siEX)
	{
		if(siSY > siEY) 
			uiDir = 3; //
		else if(siSY == siEY) 
			uiDir = 2; //
		else if(siSY < siEY)
			uiDir = 1;  //
	}
	else if(siSX == siEX)
	{
		if(siSY > siEY) 
			uiDir = 6; //
		else if(siSY < siEY)
			uiDir = 4; //
	}
	else if(siSX < siEX)
	{
		if(siSY > siEY) 
			uiDir = 9; //
		else if(siSY == siEY) 
			uiDir = 8; //
		else if(siSY < siEY)
			uiDir = 7; //
	}

	return uiDir;
}

UI16 SOPlayer::GetSpTargetIDOFSpear(UI16 uiDir, SI32 siSX, SI32 siSY, UI16 *puiIDs)
{
	UI16 uiIDCount = 0;
	if(puiIDs == NULL) return uiIDCount;

	int nCount = 2;
	
	int i = 0;
	SI32 siEX = siSX, siEY = siSY;
	switch(uiDir)
		{
		case 1: //서
			{
				for(i = 0; i < nCount; i++)
				{
					siEX = siSX - i;
					siEY = siEY + i;
					
					puiIDs[i] = pMyWorld->clISOMap.GetCharUniID(uiMapID, siEX, siEY, ON_MAP_CHARID_PLAYER);
				}
			}
			break;

		case 2: //북서
			{				
				for(i = 0; i < nCount; i++)
				{
					siEY = siEY;
					siEX = siSX - i;					
					
					puiIDs[i] = pMyWorld->clISOMap.GetCharUniID(uiMapID, siEX, siEY, ON_MAP_CHARID_PLAYER);
				}
			}
			break;

		case 3: //북
			{
				for(i = 0; i < nCount; i++)
				{
					siEX = siSX - i;
					siEY = siEY - i;
					
					puiIDs[i] = pMyWorld->clISOMap.GetCharUniID(uiMapID, siEX, siEY, ON_MAP_CHARID_PLAYER);
				}
			}
			break;

		case 4: //남서
			{
				for(i = 0; i < 2; i++)
				{
					siEX = siSX;
					siEY = siEY + i;
					
					puiIDs[i] = pMyWorld->clISOMap.GetCharUniID(uiMapID, siEX, siEY, ON_MAP_CHARID_PLAYER);
				}
			}
			break;

		case 6: //북동
			{
				for(i = 0; i < nCount; i++)
				{
					siEX = siSX;
					siEY = siEY - i;
					
					puiIDs[i] = pMyWorld->clISOMap.GetCharUniID(uiMapID, siEX, siEY, ON_MAP_CHARID_PLAYER);
				}
			}
			break;

		case 7: //남
			{
				for(i = 0; i < nCount; i++)
				{
					siEX = siSX + i;
					siEY = siEY + i;
					
					puiIDs[i] = pMyWorld->clISOMap.GetCharUniID(uiMapID, siEX, siEY, ON_MAP_CHARID_PLAYER);
				}
			}
			break;

		case 8: //동남
			{
				for(i = 0; i < nCount; i++)
				{
					siEX = siSX + i;
					siEY = siEY;
					
					puiIDs[i] = pMyWorld->clISOMap.GetCharUniID(uiMapID, siEX, siEY, ON_MAP_CHARID_PLAYER);
				}
			}
			break;

		case 9: //동
			{
				for(i = 0; i < nCount; i++)
				{
					siEX = siSX + i;
					siEY = siEY - i;
					
					puiIDs[i] = pMyWorld->clISOMap.GetCharUniID(uiMapID, siEX, siEY, ON_MAP_CHARID_PLAYER);
				}
			}
			break;
			 
		}

	return uiIDCount;
}

SI32 SOPlayer::GetSpTargetIDOFSiege(SI32 siSX, SI32 siSY, SI32 siEX, SI32 siEY, UI16 *puiIDs)
{
	return pMyWorld->clISOMap.GetCharUniID(uiMapID, siSY, siSY, siEX, siEY, puiIDs, 9, ON_MAP_CHARID_PLAYER);
}

VOID SOPlayer::ChangeNormalChar()
{
	OnlineFollower*			pFollower = IFollower.GetFollower(0);
	if(pFollower == NULL) return;
	if(IsFieldAttackUnit() == FALSE) return;

	DBIKChangeCharacter			ChangeCharacter;
	
	m_dwDeadSec = 30 * 1000;
	ChangeCharacter.siDBAccount				= stAccountInfo.siDBAccount;
	ChangeCharacter.uiDBSlot				= stAccountInfo.uiSlot;
	ChangeCharacter.siDeadTime				= (SI32)m_dwDeadSec;

	uiKind				= m_uiPrevKind;
	pFollower->uiCharID = m_uiPrevKind;

	SetFieldHp(0);
	SetPrevKind(GetPrevKind());
	SetCaptainUnit(FALSE);
	SetFieldMaxHP(0);
	SetFieldMP(0);
	SetFieldMaxMP(0);

	if(m_bPlayerVillageInLimit) //수성 인터페이스에서 나간걸로 처리
	{		
		SOVillage *pVillage	= pMyWorld->GetVillageByCode(m_siVilligeCode);
		m_bPlayerVillageInLimit = FALSE;
		m_siVilligeCode = -1;
		pVillage->DeletCharName(szID);
		SendFieldMsg(ON_RESPONSE_VILLAGE_DEFFNCE_STOP, SO_SFM_ONLYME);
	}

	pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_CHANGECHARACTER, LPVOID(&ChangeCharacter), sizeof(ChangeCharacter));

	
	
/*	DBIKDelteWarCharactor	TempDBIKDelteWarCharactor;
	memset(&TempDBIKDelteWarCharactor, 0, sizeof(DBIKDelteWarCharactor));

	//미리 저장해 놨던 자신의 실제 캐릭터의 kind로 변신한다.
	uiKind				= m_uiPrevKind;
	pFollower->uiCharID = m_uiPrevKind;


	//공성 관련 데이터들을 초기화.
	m_bFieldAlive = TRUE;
	m_dwDeadTime  = pMyWorld->dwtimeGetTime;
	m_dwDeadSec	  = 1000 * 30;

	SetFieldHp(0);
	SetPrevKind(GetPrevKind());
	SetCaptainUnit(FALSE);
	SetFieldMP(0);
	SetFieldMaxHP(0);
	SetFieldMaxMP(0);

	if(m_bPlayerVillageInLimit) //수성 인터페이스에서 나간걸로 처리
	{		
		SOVillage *pVillage	= pMyWorld->GetVillageByCode(m_siVilligeCode);
		m_bPlayerVillageInLimit = FALSE;
		m_siVilligeCode = -1;
		pVillage->DeletCharName(szID);
		SendFieldMsg(ON_RESPONSE_VILLAGE_DEFFNCE_STOP, SO_SFM_ONLYME);
	}
	
	TempDBIKDelteWarCharactor.siDBAccount	=	stAccountInfo.siDBAccount;
	TempDBIKDelteWarCharactor.uiDBSlot		=	stAccountInfo.uiSlot;

	SendFieldMsg(ON_RESPONSE_NORMAL_CHAR, SO_SFM_MEANDNEARCHAR, LPVOID(m_uiPrevKind), LPVOID(uiAccount));

	pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_DELETE_CHARACTOR_WAR, &TempDBIKDelteWarCharactor, sizeof(DBIKDelteWarCharactor));
	*/
}

BOOL SOPlayer::FieldAttackDead()
{
	OnlineFollower*					pFollower = IFollower.GetFollower(0);
	if(pFollower == NULL) return FALSE;
	
	//죽은 공성유닛을 공격했을 경우..
	if(GetFieldHp() <= 0)
	{
		m_dwDeadSec = 1000 * 30;
		DBIKChangeCharacter			ChangeCharacter;
		memset(&ChangeCharacter, 0, sizeof(DBIKChangeCharacter));
							
		ChangeCharacter.siDBAccount				= uiAccount;
		ChangeCharacter.uiDBSlot				= stAccountInfo.uiSlot;
		ChangeCharacter.siDeadTime				= (SI32)m_dwDeadSec;

		uiKind	            = m_uiPrevKind;
		pFollower->uiCharID = m_uiPrevKind;

		SetFieldHp(0);
		SetPrevKind(GetPrevKind());
		SetCaptainUnit(FALSE);
		SetFieldMaxHP(0);
		SetFieldMP(0);
		SetFieldMaxMP(0);

		if(m_bPlayerVillageInLimit) //수성 인터페이스에서 나간걸로 처리
		{		
			SOVillage *pVillage	= pMyWorld->GetVillageByCode(m_siVilligeCode);
			m_bPlayerVillageInLimit = FALSE;
			m_siVilligeCode = -1;
			pVillage->DeletCharName(szID);
			SendFieldMsg(ON_RESPONSE_VILLAGE_DEFFNCE_STOP, SO_SFM_ONLYME);
		}

		pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_CHANGECHARACTER, LPVOID(&ChangeCharacter), sizeof(ChangeCharacter));

		SendFieldMsg(ON_CHANGE_CHARACTERCODE, SO_SFM_MEANDNEARCHAR, LPVOID(this),LPVOID(m_uiPrevKind));
		return TRUE;
	}
	
	return FALSE;
}

VOID SOPlayer::RecvGiveUp()
{
	if(!pMyWorld->m_WarTimeParser.IsWarDay(pMyWorld->pRealTime->tm_wday)) return;
	if(!pMyWorld->m_WarTimeParser.IsWarTime(pMyWorld->pRealTime->tm_hour)) return;

	OnReqGiveUp     *pOnGiveUp = (OnReqGiveUp*)cMsg;
	
	SOVillage   *pVillage = NULL;
	if( ( pVillage = pMyWorld->GetVillageByCode( pOnGiveUp->uiVillageCode ) ) == NULL )	return;	

	cltGuild *pclGuild = NULL;
	pclGuild = pMyWorld->clIGuild.GetGuildByID(uiGuildID);
	if(pclGuild == NULL) return;
	
	MONEY mnMoney     = 0;
	SI32  siBestIndex = pVillage->m_Government->GetOldBestInvestIndex(); //최투의 인덱스를 얻어온다.

	if(pOnGiveUp->uiResponse == ON_GUILD_GIVEUP_OK)
	{   //상단 관계를 적대로 바꿔준다.
		if(pMyWorld->clIGuild.GiveUp(uiGuildID))
		{   //최투의 인덱스를 얻어와서 투자금의 10%를 깍는다....			
			mnMoney = pVillage->m_Government->DelInvestMoney(siBestIndex, 1, TRUE);

			//상단원들에게 대방이 공성을 포기했다고 알려준다.
			pclGuild->SendGiveUpMsg();

			//전체 공지를 해준당...
			pMyWorld->OfficialAnnouncement(ON_RET_ON_GUILD_GIVEUP, LPARAM(pclGuild->GetGuildName()), LPARAM(0));

		}
	}
	else
	{
		pOnGiveUp->uiVillageCode = 0;
		pOnGiveUp->uiResponse = ON_GUILD_GIVEUP_NORMAL;
	}

	pclGuild->SetGiveUpFlag(pOnGiveUp->uiResponse);
	
	//디비에 정보를 전송한다.
	DBIKWarGiveUp           warGiveUp;
	memset(&warGiveUp, 0, sizeof(DBIKWarGiveUp));

	warGiveUp.uiGuildID     = uiGuildID;
	warGiveUp.uiVillageCode = pOnGiveUp->uiVillageCode;
	warGiveUp.mnMoney       = mnMoney;
	warGiveUp.siFlag        = pOnGiveUp->uiResponse;
	pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_SET_WARGIVEUP, &warGiveUp, sizeof(DBIKWarGiveUp));
	
}