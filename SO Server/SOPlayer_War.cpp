#include "SOMain.h"
#include "SOWorld.h"
#include "SOPlayer.h"
#include "ParametaBox.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//ON_IMPERIUM_USE
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//模餵掏擊 詳歷擊陽 SpendItem曖 ON_ITEM_SPECIAL_IMPERIUM陛 嫦儅и棻.
//檜縑 渠и 擬港戲煎 ON_IMPERIUM_USE擊 贗塭檜樹お縑 瞪殖и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//ON_REQUEST_FIELDATTACK_READY 
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//奢撩 嶸棉婁 嶸棉曖 諄遺縑憮 鼠晦蒂 嫦餌и棻朝 詭撮雖蒂 爾魚棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

	//蜃朝 Ы溯檜橫.
	pTargetPlayer =	pMyWorld->GetPlayer(pOnReqFieldAttackRdyMsg->uiUniqueID);

	//奢撩 衛除檣陛
	if( pMyWorld->IsAttackVillageTimeZone() == FALSE )
	{
		dwResponse = ON_RET_NOT_ATTACK_TIME;
		goto END;
	}

	if(pTargetPlayer != NULL)
	{   //瞳渠 鼻欽檣陛
		if( pMyWorld->clIGuild.IsEnemyRelationWar( uiGuildID, pTargetPlayer->uiGuildID ) == FALSE )
		{
			dwResponse = ON_RET_NOT_ENEMYGUILD;
			goto END;
		}
		
		//奢撩 嶸棉 檣陛
		if(pTargetPlayer->IsFieldAttackUnit() == FALSE)	
		{
			dwResponse = ON_RET_NOT_ATTACK_UNITE;
			goto END;
		}

		//餌嫌 氈釭
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
	//⑷營 衛除擊 掘и棻.
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
		//濠褐擊 んли 輿曖曖 賅萇 餌塋菟縑啪 爾魚棻.
		SendFieldMsg(ON_RESPONSE_FIELDATTACK_READY, SO_SFM_MEANDNEARCHAR,
			         LPVOID(dwResponse), LPVOID(uiAccount),                   
					 LPVOID(pOnReqFieldAttackRdyMsg->uiUniqueID),
					 LPVOID(pFollower->uiCharID), LPVOID(dwTempTime), LPVOID(bIsLong));
	}
/*	else
	{
		//褻勒擊 虜褶衛酈雖 跤й唳辦 濠晦 濠褐縑啪虜 褒ぬ 詭撮雖蒂 爾魚棻.
		SendFieldMsg(ON_RESPONSE_FIELDATTACK_READY, SO_SFM_ONLYME,
			         LPVOID(dwResponse), LPVOID(uiAccount),                   
					 LPVOID(pOnReqFieldAttackRdyMsg->uiUniqueID),
					 LPVOID(pFollower->uiCharID), LPVOID(dwTempTime), LPVOID(bIsLong));
	}
	*/
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//ON_REQUEST_FIELDATTACK_ACTION
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//奢撩 嶸棉婁曖 諄遺縑憮 鼠晦縑 蜃懊棻朝 詭撮雖蒂 爾魚棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
		//蜃朝 餌塋檜 奢撩嶸棉檣雖 �挫恉挬�.
		if(!pTargetPlayer->IsFieldAttackUnit())	return;
		if(!pTargetPlayer->IsFieldAlive())	return;
	}
	
	//陽葬朝 餌塋檜 奢撩嶸棉檣雖 �挫恉挬�.
	if(IsFieldAttackUnit() == FALSE) return;
	
	pTargetFollower = pTargetPlayer->IFollower.GetFollower(0);
	if(pTargetFollower == NULL) return;

	//避擎 奢撩嶸棉擎 籀葬п遽棻. 
	if(FieldAttackDead()) return;
	if(pTargetPlayer->FieldAttackDead()) return;

	//嶸棉囀萄蒂 橢橫螞棻
	SI16 siUnitCode = CSiegeUnitTable::GetInstance()->GetKindUnitCode(uiKind);
	if(siUnitCode < 0 || siUnitCode == FIELD_BATTLE_RAMPART) 
	{
		dwResponse = ON_RET_NO;
		return;
	}
	
	//奢問 裁溯檜 啗骯
	DWORD dwNowTick = GetTickCount();
	DWORD dwTempTime = dwNowTick - m_dwAttackDelay;
	if(dwTempTime < CSiegeUnitTable::GetInstance()->GetDelay(siUnitCode))
	{
		dwTempTime = CSiegeUnitTable::GetInstance()->GetDelay(siUnitCode) - dwTempTime;
		dwResponse = ON_RET_NOT_ATTACK_DELAY_TIME;
		return;
	}
	m_dwAttackDelay = dwNowTick; //⑷營 衛除擊 撲薑п遽棻.

	//等嘐雖蒂 啗骯и棻.
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
	
	//唯堅и 奢問檜 �鷝挾Ц�棻賊.
	if(siAttackPower >= siRand)
	{	
		 //錳楚 輿溥湍 等嘐雖曖 1.5寡曖 等嘐雖蒂 遽棻.
		 float fTempDamage = (float)siDamage * 1.5;
		 siDamage	  = (SI16)fTempDamage;
		 siAttackType = ON_RET_ATTACK_POWER;
	}
	//蜃朝 檣除曖 唯堅и 寞橫陛 撩奢ж艘棻.
	else if(siDefencePower < siRand)
	{
		siDamage = 1;
		siAttackType = ON_RET_DEFENCE_POWER;
	}

	pTargetPlayer->DecreaseFieldHp(siDamage);
	
	if(pTargetPlayer->GetFieldHp() <= 0) m_siKillNum++;

	if(dwResponse == ON_RET_OK)
	{
		//等嘐雖蒂 湍螳輿晦 嬪п 賅萇餌塋菟縑啪 爾魚棻.
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
		//蝶Ы溯蓮 等嘐雖 籀葬
		UI16 uiDir = 0;
		int i = 0;
		UI16 uiTargetID[9];
		memset(uiTargetID, 0, sizeof(UI16) * 9);
		
		if(siUnitCode == FIELD_BATTLE_SPEAR) //璽煽檣唳辦
		{
			//寞щ擊 憲嬴螞棻.
			uiDir = GetDir(siX, siY, pTargetPlayer->siX, pTargetPlayer->siY);

			//蝶Ы溯蓮 等嘐雖蒂 嫡朝 嶸盪 檣策蝶蒂 憲嬴螞棻.
			GetSpTargetIDOFSpear(uiDir, pTargetPlayer->siX, pTargetPlayer->siY, uiTargetID);
			
		}
		else if(siUnitCode == FIELD_BATTLE_SIEGE) //嫦戮剪檣唳辦
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
			if(pPlayer->IsFieldAttackUnit() == FALSE) continue; //奢撩 嶸棉檣雖 匐餌
			if(uiGuildID == pPlayer->uiGuildID) continue; //瞳渠 鼻欽檣雖 匐餌
			if(pMyWorld->clIGuild.IsEnemyRelationWar( uiGuildID, pPlayer->uiGuildID) == FALSE ) continue;

			
			pPlayer->DecreaseFieldHp(siDamage);

			pPlayer->SendFieldMsg(ON_RESPONSE_FIELDATTACK_ACTION, SO_SFM_MEANDNEARCHAR, 
								  LPVOID(uiAccount), LPVOID(siDamage), LPVOID(m_bFieldAlive),
								  LPVOID(pPlayer->uiAccount), LPVOID(dwResponse), LPVOID(siAttackType));
		}	
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//ON_REQUEST_FIELDATTACK_DEAD
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//奢撩 嶸棉婁曖 諄遺縑憮 避歷棻朝 詭撮雖蒂 爾魚棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvFieldAttackDead()
{
	OnReqFieldAttackDeadMsg*		pOnReqFieldAttackDeadMsg;
	pOnReqFieldAttackDeadMsg	=	(OnReqFieldAttackDeadMsg*)cMsg;
	
	OnlineFollower*					pFollower = IFollower.GetFollower(0);
	if(pFollower == NULL) return;
	if(IsFieldAttackUnit() == FALSE) return;

	if(m_bCaptainUnit == FALSE)
	{
		//憮幗縑憮 綠掖ж晦 嬪и 衛除擊 盪濰и棻.
		m_dwDeadTime = pMyWorld->dwtimeGetTime;
		//ぬ割じ煎 嫡朝 衛除擊 盪濰и棻. ゎ團 30蟾 
		m_dwDeadSec  = 30000;
		m_bFieldAlive = FALSE;
	}
	else
	{
		//憮幗縑憮 綠掖ж晦 嬪и 衛除擊 盪濰и棻.
		m_dwDeadTime = pMyWorld->dwtimeGetTime;
		//ぬ割じ煎 嫡朝 衛除擊 盪濰и棻. 渠濰 1碟
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

	if(m_bPlayerVillageInLimit) //熱撩 檣攪む檜蝶縑憮 釭除勘煎 籀葬
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//ON_REQUEST_LONGDISTANCE_VILLAGEATTACK_READY
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//錳剪葬 奢撩嶸棉檜 葆擊擊 奢問й 陽 鼠晦蒂 嫦餌и棻朝 詭撮雖蒂 爾魚棻.
//葆擊 奢問擊 斬剪葬諦 錳剪葬煎 釭換檜嶸 : 
//斬剪葬曖 唳辦 葆擊曖 奩問檜 陛棟ж堅 錳剪葬曖 唳辦 翕晦�飛� 蜃醮撿 ж朝 檜嶸煎 萃煎 釭斑棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

	// 檜 詭衛雖蒂 嫡晦縑 嶸�褲� 衛除檣雖 憲嬴螞棻.	
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

	// ⑷營 葆擊縑 鼻欽檜 氈朝雖 匐餌и棻.
	if( pVillage->uiGuildID == 0 )																		
	{
		dwResponse = ON_RET_NOT_VILLAGE_CODE;
		goto END;
	}

	// Ы溯檜橫曖 鼻欽婁 檜 葆擊曖 鼻欽檜 瞳渠瞳檣雖 匐餌и棻.
	if( pMyWorld->clIGuild.IsEnemyRelationWar( uiGuildID, pVillage->uiGuildID ) == FALSE )
	{
		dwResponse = ON_RET_NOT_ENEMYGUILD;
		goto END;
	}

	//⑷營 衛除擊 掘и棻.
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
	//嬪 褻勒菟擊 醱褶 衛鑑棻賊 濠褐擊 んли 輿曖曖 賅萇 餌塋菟縑啪 爾魚棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//ON_REQUEST_LONGDISTANCE_VILLAGEATTACK_ACTION
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//錳剪葬 奢撩嶸棉檜 葆擊擊 奢問й 陽 鼠晦陛 蜃懊棻朝 詭撮雖蒂 籀葬п遽棻.
//葆擊檜 だ惚腎歷擊陽曖 橾紫 籀葬и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvLongDistanceAttackVillageAction()
{
	OnReqLongAttackVillageActionMsg		*pOnReqLongAttackVillageActionMsg;
	SOVillage							*pVillage;
	BOOL                                 bIsLong = FALSE;

	pOnReqLongAttackVillageActionMsg	=	(OnReqLongAttackVillageActionMsg*)cMsg;
	
	//奢撩檜 陛棟и 衛除檣陛 羹觼..
	if( pMyWorld->IsAttackVillageTimeZone() == FALSE ) return;	
	if((pVillage = pMyWorld->GetVillageByCode( pOnReqLongAttackVillageActionMsg->uiVillageCode)) == NULL ) return;	
	if( pVillage->GetVillageDefence() <= 0) return;
	// ⑷營 葆擊縑 鼻欽檜 氈朝雖 匐餌и棻.
	if( pVillage->uiGuildID == 0 ) return;
	// Ы溯檜橫曖 鼻欽婁 檜 葆擊曖 鼻欽檜 瞳渠瞳檣雖 匐餌и棻.
	if( pMyWorld->clIGuild.IsEnemyRelationWar( uiGuildID, pVillage->uiGuildID ) == FALSE ) return;	

	int nUnitCode = CSiegeUnitTable::GetInstance()->GetKindUnitCode(uiKind);
	if(nUnitCode < 0) return;
	
	if(nUnitCode == FIELD_BATTLE_ARROW || nUnitCode == FIELD_BATTLE_SIEGE)
		bIsLong = TRUE;
	
	//葆擊擊 奢問и棻.
	if(pVillage->AttackDamage(this, pOnReqLongAttackVillageActionMsg->siAttackType, bIsLong) == TRUE)
	{
		if(pVillage->uiGuildID != 0)
			pMyWorld->clIGuild.DelVillage(pVillage->uiGuildID, pVillage, FALSE);
	}

	/*
	//等嘐雖 啗骯 葆擊 だ惚腎歷釭 だ學 蛔蛔.
	if(pVillage->LongDistanceAttack(this,pOnReqLongAttackVillageActionMsg->siAttackType) == TRUE)
	{
		// 葆擊檜 諫瞪�� だ惚腎歷棻.
		if(pVillage->uiGuildID != 0)
			pMyWorld->clIGuild.DelVillage(pVillage->uiGuildID, pVillage, FALSE);
	}	
	*/
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//ON_REQUEST_ATTACK_VILLAGE
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//奢撩嶸棉檜 葆擊擊 奢問й陽 葆擊擊 陽萼棻.葆擊檜 蜃懊棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvAttackVillage()
{
	OnReqAttackVillageMsg		*pAttackVillageMsg;
	SOVillage					*pVillage;	
	BOOL                         bIsLong = FALSE;

	// 檜 詭衛雖蒂 嫡晦縑 嶸�褲� 衛除檣雖 憲嬴螞棻.	
	if( pMyWorld->IsAttackVillageTimeZone() == FALSE )													return;		
	pAttackVillageMsg	=	(OnReqAttackVillageMsg*)cMsg;

	if( ( pVillage = pMyWorld->GetVillageByCode( pAttackVillageMsg->uiVillageCode ) ) == NULL )			return;	
	if( pVillage->GetVillageDefence() <= 0) return;

	// ⑷營 葆擊縑 鼻欽檜 氈朝雖 匐餌и棻.
	if( pVillage->uiGuildID == 0 )																		return;

	// Ы溯檜橫曖 鼻欽婁 檜 葆擊曖 鼻欽檜 瞳渠瞳檣雖 匐餌и棻.
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
		// 葆擊檜 諫瞪�� だ惚腎歷棻.
		if(pVillage->uiGuildID != 0)
			pMyWorld->clIGuild.DelVillage(pVillage->uiGuildID, pVillage, FALSE);
	}
	*/
}
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//ON_REQUEST_WAR_VILLAGE_DEFENCE
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 熱撩擊 ж晦嬪п 葆擊 寰戲煎 菟橫除棻.
// 褒薯煎 葆擊縑 菟橫除 鼻鷓朝 嬴棲堅 в萄 樓撩擊 陛雖堅 氈戲賊憮 葆擊曖 ん檣攪蒂 陛雖堅 氈朝棻.
// в萄->熱撩 檣攪む檜蝶.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

	stAccountInfo.pVillage	=	pMyWorld->GetVillageByCode(pOnReqWarVillageDefence->uiVillageCode);				// 葆擊擊 瓊朝棻.
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

	if(siUnitCode != FIELD_BATTLE_SWORD) //匐煽虜 陛棟ж棻.
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

	//и 葆擊縑朝 и貲曖 Ы溯檜橫虜 熱撩 檣攪む檜蝶縑 菟橫陛 氈擊熱 氈棻.
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

	// 葆擊曖 Guild ID蒂 掘и棻.
	uiVillageGuildID		=	stAccountInfo.pVillage->GetGuildNID();

	// 瞳 葆擊曖 鼻欽檜賊 菟橫陞 熱 橈棻.
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

	// 掖羲璽擊 殘朝棻.
	CloseTrade();

	// 顫 嶸盪縑啪 だじ 褐羶醞檜剪釭 
	// 棻艇 嶸盪縑 だじ 褐羶擊 嫡懊棻賊 檜蒂 鏃模и棻.
	CloseRequestingJoinParty();

	//熱撩檣攪む檜蝶縑 菟橫諦 氈棻朝 Ы溯斜蒂 撮た.
//	pVillage->m_bVillageInLimit = TRUE;
	m_bPlayerVillageInLimit     = TRUE;
	m_siVilligeCode = pOnReqWarVillageDefence->uiVillageCode;	

	//贗塭檜樹お縑 湍螳輿晦 嬪п憮 熱撩 AttackType曖 裁溯檜 衛除擊 啗骯.
	if(pVillage->m_dwPrevAttackdelay > 0)
		siLimitTime = (SI32)pVillage->m_dwSkillDelayTime - (pMyWorld->dwtimeGetTime - pVillage->m_dwPrevAttackdelay);

	dwResponse	=	ON_RET_OK;

END:
	// 葆擊縑 菟橫陞 熱 橈棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// ON_REQUEST_WAR_VILLAGE_IN
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 熱撩檣攪む檜蝶縑憮 葆擊煎 菟橫除棻.
// в萄樓撩縑憮 葆擊樓撩戲煎 夥莎棻.
// 熱撩 檣攪む檜蝶 -> 葆擊
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvReqWarVillageIn()
{
	OnReqWarVillageIn*			pOnReqWarVillageIn;
	SOVillage*					pVillage	=	NULL;

	pOnReqWarVillageIn	=	(OnReqWarVillageIn*)cMsg;

	stAccountInfo.pVillage	=	pMyWorld->GetVillageByCode(pOnReqWarVillageIn->uiVillageCode);				// 葆擊擊 瓊朝棻.
	pVillage				=	pMyWorld->GetVillageByCode(pOnReqWarVillageIn->uiVillageCode);				// 葆擊擊 瓊朝棻.

	pVillage->m_bVillageInLimit = FALSE;
	m_bPlayerVillageInLimit		= FALSE;
	m_siVilligeCode = -1;
//	strcpy(pVillage->m_szCharName,"");

	//裘縑憮 雖錶遽棻.
	if(DelIDOnMap( 6 ) == FALSE)
		writeInfoToFile("DelIDOnMap Failed.txt", "5 [%u] [%d] [%d] [%d]", uiAccount, uiMapID, siX, siY);
//			pMyWorld->clServer->WriteInfo("..\\DelIDOnMap Failed.txt", "5 [%u] [%d] [%d] [%d]", uiAccount, uiMapID, siX, siY);

	EnterVillage();		
			
	//釭и纔 憲萵隆 葆擊曖 薑爾蒂 爾魚棻.
//	SendFieldMsg(ON_RESPONSE_WAR_VILLAGE_IN, SO_SFM_ONLYME,LPVOID(pVillage));

	//輿嬪餌塋и纔 爾鳥隆 葆擊 薑爾蒂 賓棻.
//	SendFieldMsg(ON_RESPONSE_WAR_VILLAGE_IN, SO_SFM_ONLYNEARCHAR);	
}

//熱撩 檣攪む檜蝶縑憮 釭螞棻.
VOID SOPlayer::RecvReqWarVillageDefenceStop()
{
	OnReqWarVillageIn     *pVillageIn = (OnReqWarVillageIn*)cMsg;

	SOVillage *pVillage	= NULL;
	pVillage = pMyWorld->GetVillageByCode(pVillageIn->uiVillageCode);				// 葆擊擊 瓊朝棻.

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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//ON_REQUEST_VILLAGE_ATTACK_TYPE_CHECK
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//熱撩 檣攪む檜蝶曖 晦棟擊 餌辨й熱 氈朝雖 憲嬴螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
/*
VOID	SOPlayer::RecvVillageAttackTypeCheck()
{
	OnReqVillageAttackTypeCheck*		pOnReqVillageAttackTypeCheck;
	SOVillage*							pVillage;
	DWORD								dwResponse = ON_RET_OK;

	pOnReqVillageAttackTypeCheck =		(OnReqVillageAttackTypeCheck*)cMsg;
	pVillage = pMyWorld->GetVillageByCode(pOnReqVillageAttackTypeCheck->uiVillageCode);

	// siClassInGuild 詹幗 滲熱歜.-_-陪馬..
	// 渠寞檜釭 渠ч熱 檣雖 羹觼.
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//ON_REQUEST_VILLAGE_PLAYER_OIL_ATTACK
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//熱撩 檣攪む檜蝶 晦葷 奢問詭撮雖縑 渠и 頂辨擊 籀葬и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//ON_REQUEST_VILLAGE_PLAYER_STONE_ATTACK
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//熱撩 檣攪む檜蝶 給 奢問詭撮雖縑 渠и 頂辨擊 籀葬и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//ON_REQEUST_VILLAGE_DEFENCE
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//熱撩 檣攪む檜蝶 葆擊 寞橫溘 щ鼻縑 渠и 頂辨擊 籀葬и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

	//葆擊檜 橈戲賊 false
	if(pVillage == NULL)				               goto END;
	if(pVillage->vdMaxDefence == pVillage->vdDefence)  goto END;

	//渠寞檜釭 渠ч熱陛 嬴棲賊 false
/*	if(siClassInGuild != pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI) && 
		siClassInGuild != pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DHS) &&
		strcmp(szID,pVillage->szHNSID) != 0) goto END;
	if(pVillage->GetVillageAttackType()			!=			ON_VILLAGE_ATTACK_NONE)	goto END;
	if(pVillage->m_bAttackTypeDelay				==							  TRUE) goto END;
	if(pVillage->GetDefenceSoldier()			<=						  0)		goto END;
	if(pVillage->vdMaxDefence == pVillage->vdDefence						)		goto END;


	//⑷營 葆擊 熱綠煽曖 10寡梱雖 葆擊 寞橫溘擊 棺萵熱 氈棻.
	siDefenceSoldierToDefenceUp = (SI32)pVillage->GetDefenceSoldier() * 10;
*/

	siUnitCode = CSiegeUnitTable::GetInstance()->GetKindUnitCode(uiKind);
	if(siUnitCode < 0)
	{
		dwResponse = ON_RET_DEFENSE_NO;
		goto END;
	}

	if(siUnitCode != FIELD_BATTLE_SWORD) 
	{   //匐煽檜 嬴棲賊 熱葬й熱 橈棻...
		dwResponse = ON_RET_DEFENSE_NO;
		goto END;
	}

	mnMoney = 1000;
	if(EnoughMoney(mnMoney) ==	FALSE)			
	{	//絲擊 羹觼
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
	
	//葆擊曖 寞橫溘擊 螢萼棻.
	pVillage->IncreaseVillageDefence(vdDefenceUp);
	
	//模雖旎擊 還檣棻.
	DecreaseMoney(mnMoney);

/*
	if(vdDefenceUp < siDefenceSoldierToDefenceUp)
	{
		//葆擊 寞橫溘擊 螢葬晦 嬪и 絲擊 掘и棻.
		mnMoney = (MONEY)vdDefenceUp;
		//絲檜 醱碟и雖 憲嬴螞棻.
		if(EnoughMoney(mnMoney)	==	FALSE)			
		{	
			dwResponse = ON_RET_DEFENSE_NOT_MONEY;
			goto END;
		}
		//葆擊 寞橫溘擊 螢萼棻.
		pVillage->IncreaseVillageDefence(vdDefenceUp);
		
		//模雖旎擊 還檣棻.
		DecreaseMoney(mnMoney);
		
	}
	else // vdDefenceUp >= siDefenceSoldierToDefenceUp
	{
		//螢萵 葆擊 寞橫溘擊 橢橫螞棻.
		vdDefenceUp = (VDEF)siDefenceSoldierToDefenceUp;
		//葆擊 寞橫溘擊 螢葬晦 嬪и 絲擊 掘и棻.
		mnMoney = (MONEY)vdDefenceUp;
		//絲檜 醱碟и雖 憲嬴螞棻.
		if(EnoughMoney(mnMoney)	==	FALSE)		
		{
			dwResponse = ON_RET_DEFENSE_NOT_MONEY;
			goto END;
		}
		//葆擊 寞橫溘擊 螢萼棻.
		pVillage->IncreaseVillageDefence(vdDefenceUp);
		//模雖旎擊 還檣棻.
		DecreaseMoney(mnMoney);
	}
*/
	//葆擊 奢問⑽鷓蒂 寞橫溘 щ鼻戲煎 虜萇棻.
	

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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//ON_REQUEST_INCREASE_GUILD_UNIT
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//鼻欽曖 模餵掏擊 渦и棻.
//葆擊 寰縑憮虜 陛棟.檣漸饜葬曖 嬴棋 鼻欽曖 cltGuild::cltGuildUnit 贗楚蝶縑 盪濰и棻.
//葆擊曖 檣掘熱.鼻欽曖 爾晝ヶ 熱縑 綠滔п憮 儅骯й熱 氈棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
	
	
	//嬴檜蠱 嬴檜蛤蒂 橢橫螞棻.
//	uiItemID = pMyWorld->m_FieldBattleParser.GetItemID(pOnReqIncreaseGuildUnit->siKind);
//	pItemHeader = pMyWorld->clItem.GetItem(uiItemID);

	//熱綠煽 * 1睡渠曖 爾晝榆 
//	siSupply = pItemHeader->m_pImperium->m_siSupply * pOnReqIncreaseGuildUnit->siCount;
	//1睡渠 * pItemHeader->m_pImperium->m_siPrice * pItemHeader->m_pImperium->m_siPopulation
//	mnMoney =  (MONEY)(pItemHeader->m_pImperium->m_siPrice * pItemHeader->m_pImperium->m_siPopulation * pOnReqIncreaseGuildUnit->siCount);


//	if(pclGuild->GetGuildSupply() < siSupply)
//	{
//		dwResponse = ON_RET_NOT_ENOUGH_SUPPLY;
//		goto END;
//	}

	//模餵й 睡渠曖 絲擊 啗骯и棻.
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
	
	//ж瑞縑 儅骯й熱 氈朝 譆渠 儅骯榆擊 剩歷釭 羹觼.	
	if(!pVillage->CheckDecreasePopulation((UI32)nTempPopu * pOnReqIncreaseGuildUnit->siCount))
	{
		dwResponse = ON_RET_NOT_ENOUGH_POPULATION;
		goto END;
	}

	//葆擊曖 譆模 檣掘熱蒂 薯ии棻.
//	if(pVillage->LimitDecreasePopulation((UI32)(pItemHeader->m_pImperium->m_siPopulation * pOnReqIncreaseGuildUnit->siCount)) == FALSE)
	if(!pVillage->LimitDecreasePopulation((UI32)nTempPopu * pOnReqIncreaseGuildUnit->siCount))
	{
		dwResponse = ON_RET_NOT_LIMIT_POPULATION;
		goto END;
	}

	//鼻欽曖 爾晝ヶ 賓棻.
//	pclGuild->DecreaseGuildSupply((SI32)siSupply);
	
	//嶸盪陛 陛霞 絲擊 賓棻.
	DecreaseMoney(mnMoney, FALSE);
	
	//賅餵脹 煽溘 偎熱蒂 隸陛 衛鑑棻.
	siTotalGuildUnitCount = pclGuild->m_GuildUnit.IncreaseGuildUnit(siUnitCode, pOnReqIncreaseGuildUnit->siCount);

	//葆擊曖 檣掘熱蒂 馬模衛鑑棻.
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
	//葆擊曖 檣掘熱蒂 還檜堅 絲紫 還檜堅 爾晝ヶ紫 還檣棻.
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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//ON_REQUEST_DECREASE_GUILD_UNIT
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//鼻欽曖 渠寞檜 鼻欽曖 奢撩嶸棉 
//cltGuild::cltGuildUnit贗楚蝶曖 等檜攪蒂 檣漸饜葬曖 嬴檜蠱戲煎 瞪�納挬�. (離轎)
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

	//陛螳陛朝 嬴檜蠱.
	UI16 uiItemID = pMyWorld->m_FieldBattleParser.GetItemID(pOnReqDecreaseGuildUnit->siKind);
	pItemHeader = pMyWorld->clItem.GetItem(uiItemID);

	//嬴檜蠱檜 盪濰 陛棟и 嬪纂蒂 匐儀и�� 檣漸饜葬縑 盪濰衛麵遽棻.
	for(SI16 i = 0 ;i < ON_MAX_FOLLOWER_NUM ; i++)
	{
		if(IFollower.IsValidFollower(i) == TRUE)
		{
			 pAddItemData = IFollower.GetItemByID(i, uiItemID); 

			 if(pAddItemData)	//п渡 嬴檜蠱擊 陛雖堅 氈棻.
			 {
                //AddItemNoQuery 檣濠.
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

					//陛螳陛堅 陴擎 嬴檜蠱曖 識偎熱.
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
			 else	//陛雖堅 氈雖 彊棻.
			 {
				 //
				 siTempItemSlot = IFollower.FindEmptyItemBuffer(i);
				 if((siTempItemSlot != -1) && (siTempItemSlot != 0xffff))
				 {
					//綴蘊擊 瓊嬴憮 陛雖堅 氈棻賊 蹺陛.
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

						//陛螳陛堅 陴擎 嬴檜蠱曖 識偎熱.
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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//ON_REQUEST_LIST_GUILD_UNIT
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//鼻欽曖 奢撩嶸棉曖 葬蝶お蒂 橢橫螞棻. 
//濰睡諦 鼻欽 檣攪む檜蝶縑憮 餌辨. 
//cltGuild::cltGuildUnit曖 頂辨擊 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvGetGuildUnitList()
{
	cltGuild*				    pclGuild;
	OnReqGetGuildUnitList*		pOnReqGetGuildUnitList;
	UI32						uiTempPopulation = 0;

	pOnReqGetGuildUnitList		=	(OnReqGetGuildUnitList*)cMsg;

	if((pclGuild =	pMyWorld->clIGuild.GetGuildByID(uiGuildID)) == NULL )	return;

	//嶸棉 儅撩衛紫 餌辨ж晦 陽僥縑 模餵й熱 氈朝 葆擊曖 檣掘熱蒂 爾頂遽棻.
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
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//ON_REQUEST_INCREASE_GUILD_SUPPLY
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//鼻欽檜 陛雖堅 氈朝 爾晝ヶ擊 隸陛 衛鑑棻.
//cltGuild::m_siGuildSupply縑 盪濰и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvIncreaseGuildSupply()
{
	cltGuild*					 pclGuild;
	MONEY						 uiMoney = 0;
	DWORD						 dwResponse = ON_RET_OK;
	SI32						 siSupplyPrice = 125;		//爾晝ヶ曖 陛問.

	OnReqIncreaseGuildSupply* pOnReqIncreaseGuildSupply = (OnReqIncreaseGuildSupply*)cMsg;

	if((pclGuild =	pMyWorld->clIGuild.GetGuildByID(uiGuildID)) == NULL )			dwResponse = ON_RET_NO;
	if(GetMoney() < (siSupplyPrice * pOnReqIncreaseGuildSupply->siSupplyQuantity))	dwResponse = ON_RET_NO;

	//Guild 爾晝ヶ 偎熱蒂 隸陛 衛鑑棻.
	pclGuild->IncreaseGuildSupply(pOnReqIncreaseGuildSupply->siSupplyQuantity);
	//п渡 薯ヶ曖 絲擊 橢橫螞棻.
	uiMoney = siSupplyPrice * pOnReqIncreaseGuildSupply->siSupplyQuantity;
	DecreaseMoney(uiMoney,FALSE);

	//Guild爾晝ヶ曖 識 偎熱 諦 嶸盪縑啪 賓 絲擊 蛤綠縑 盪濰.
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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//ON_REQUEST_DECREASE_GUILD_SUPPLY
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//爾晝熱溯陛 鼻欽曖 爾晝ヶ擊 陛螳除棻.
//爾晝熱溯陛 葆擊擊 贗葛п憮 鼻欽曖 爾晝ヶ擊 陛螳除棻.
//爾晝熱溯曖 爾晝ヶ榆 隸陛 鼻欽曖 爾晝ヶ榆 馬模.奢撩醞縑虜 陛棟ж棻.
//cltGuild::m_siGuildSupply 馬模.
//SOPlayer::m_siSupplyCharge 隸陛.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvDecreaseGuildSupply()
{
	cltGuild*					pclGuild;
	MONEY						uiMoney				 = 0;
	DWORD						dwResponse			 = ON_RET_OK;
	SOVillage*					pVillage			 =	NULL;
	SI32						siTempSupplyQuantity = 0;
	

	OnReqDecreaseGuildSupply* pOnReqDecreaseGuildSupply = (OnReqDecreaseGuildSupply*)cMsg;

	pVillage = pMyWorld->GetVillageByCode(pOnReqDecreaseGuildSupply->siVillageCode);

	//葆擊曖 嬴檜蛤諦 濠褐曖 望萄嬴檜蛤陛 翕橾 п撿и棻.
	if(pVillage->uiGuildID != uiGuildID)											dwResponse = ON_RET_NO;
	if((pclGuild = pMyWorld->clIGuild.GetGuildByID(uiGuildID)) == NULL )			dwResponse = ON_RET_NO;
	if(pclGuild->GetGuildSupply() == 0)												dwResponse = ON_RET_NO;
	if(pMyWorld->clIGuild.GetGuildWarCheck(uiGuildID) == FALSE)						dwResponse = ON_RET_NO;


	if(dwResponse == ON_RET_OK)
	{
		siTempSupplyQuantity = pOnReqDecreaseGuildSupply->siSupplyQuantity;
		
		if(pclGuild->GetGuildSupply() > siTempSupplyQuantity) 
		{
			//Guild 爾晝ヶ 偎熱蒂 馬模 衛鑑棻.
			pclGuild->DecreaseGuildSupply(siTempSupplyQuantity);
			//爾晝熱溯縑啪虜 剩望熱 氈朝 醱瞪辨 爾晝啪檜雖.
			SetFieldSupplyCharge(siTempSupplyQuantity);
		}
		else
		{
			siTempSupplyQuantity = pclGuild->GetGuildSupply();

			pclGuild->DecreaseGuildSupply(siTempSupplyQuantity);
			SetFieldSupplyCharge(siTempSupplyQuantity);
		}

	}

	//陴擎 Guild爾晝ヶ 婁 嶸盪縑啪 除 爾晝ヶ 蛤綠縑 盪濰.
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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//ON_REQUEST_GET_SUPPLY_AT_GUILD_UNIT
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//奢撩嶸棉檜 爾晝熱溯煎睡攪 爾晝ヶ擊 橢橫螞棻.
//奢撩嶸棉檜 爾晝熱溯蒂 贗葛 爾晝熱溯陛 模雖 ж堅 氈朝 爾晝ヶ擊 奢撩嶸棉檜 陛螳除棻.
//憮幗縑憮朝 嶸棉檜 陛螳除 蟾晦高虜擊 晦遽戲煎 啗骯ж堅
//贗塭檜樹お陛 陛螳除 爾晝ヶ擎 憮幗諦 婦啗橈檜 贗塭檜樹お縑憮 籀葬и棻.
//爾晝熱溯(pTargetPlayer)曖 SOPlayer::m_siSupplyCharge 馬模.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

		//偽擎 鼻欽曖 模樓檜 �捐Я牉�.
		if(uiGuildID == pTargetPlayer->uiGuildID)
		{
			//奢撩 嶸棉檜 蜃朝陛蒂 �挫恉挬�.
			if(pTargetPlayer->IsFieldAttackUnit() == FALSE)								return;
			if(IsFieldAttackUnit()				  == FALSE)								return;
			//摹鷗и 嶸棉檜 爾晝熱溯陛 蜃朝陛蒂 �挫恉挬�.
			if(pMyWorld->m_FieldBattleParser.GetUnitKind(pTargetPlayer->uiKind) != FIELD_BATTLE_SUPPLY)	return;
			//濠褐檜 爾晝熱溯檜賊憮 爾晝熱溯縑啪 醱瞪 й熱 橈棻.
			if(pMyWorld->m_FieldBattleParser.GetUnitKind(uiKind)		 == FIELD_BATTLE_SUPPLY)	return;
			//爾晝熱溯陛 陛雖堅 氈朝 爾晝啪檜雖陛 醱瞪腎橫 氈朝雖蒂 �挫恉挬�.
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
				LPVOID(siResSupplyAmount),						// Player陛 陛螳除 Supply曄.
				LPVOID(pOnReqGetSupplyAtGuildUnit->uiTargetID), // TargetPlayerID
				LPVOID(pTargetPlayer->GetFieldSupplyCharge()));	// TargetPlayer陛 陛雖堅 氈朝 Supply曄.
		}
	}
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//ON_REQUEST_LIST_GUILD_SUPPLY
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//鼻欽檜 陛雖堅 氈朝 爾晝ヶ曖 偎熱蒂 橢橫螞棻.
//葆擊曖 奢撩婦溼檣攪む檜蝶縑憮 ��轎ж雖虜 爾晝ヶ擎 鼻欽縑 盪濰腎嘎煎 
//cltGuild::m_siGuildSupply蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//ON_REQUEST_INCREASE_VILLAGE_DEFENCE_SOLDIER
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//葆擊曖 熱綠煽 熱蒂 隸陛 衛鑑棻.
//葆擊 奢撩 檣攪む檜蝶縑憮 奢撩嶸棉婁 熱綠煽擊 л眷 賅餵ж雖虜
//奢撩嶸棉擎		cltGuild::cltGuildUnit 縑
//葆擊 熱綠煽熱朝	SOVillage::m_siDefenceSoldier 縑 盪濰脹棻.
//葆擊 熱綠煽擊 隸陛衛鑒陽 鼻欽曖 模餵掏擊 馬模衛酈堅 葆擊曖 檣掘熱蒂 賓棻.
//葆擊 熱綠煽擎 陝 葆擊葆棻 襄營и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
	
	//Guild 爾晝ヶ曖 識 偎熱蒂 馬模  嶸盪縑啪 �A絲擊 蛤綠縑 盪濰.
	if(pVillage)
	{
		UI16 uiItemID = pMyWorld->m_FieldBattleParser.GetItemID(FIELD_BATTLE_DEFENSE);
		pItemHeader = pMyWorld->clItem.GetItem(uiItemID);


		//葆擊 熱綠煽熱蒂 隸陛 衛鑑棻.
		//熱綠煽 * 熱綠煽 Hp
		siDefenceSoldier = (SI32)(pOnReqVillageDefenceSoldier->siDefenceSoldierCount*pItemHeader->m_pImperium->m_siHp);
		
		//望萄曖 爾晝ヶ熱蒂 馬模 衛鑑棻.
		//熱綠煽 * 1睡渠渡 爾晝榆.
		siSupply = (SI32)(pItemHeader->m_pImperium->m_siSupply*pOnReqVillageDefenceSoldier->siDefenceSoldierCount);

		if(pclGuild->GetGuildSupply() < siSupply)
		{
			dwResponse = ON_RET_NOT_ENOUGH_SUPPLY;
			goto END;
		}

		mnMoney = (MONEY)(pItemHeader->m_pImperium->m_siPrice * pItemHeader->m_pImperium->m_siPopulation * pOnReqVillageDefenceSoldier->siDefenceSoldierCount);

		//絲擊 馬模衛蘊棻.
		if(EnoughMoney(mnMoney) == FALSE)
		{
			dwResponse = ON_RET_NOT_ENOUGH_MONEY;
			goto END;
		}
		//ж瑞縑 儅骯й熱 氈朝 譆渠 儅骯榆擊 剩歷釭 羹觼.
		if(pVillage->CheckDecreasePopulation((UI32)(pOnReqVillageDefenceSoldier->siDefenceSoldierCount * pItemHeader->m_pImperium->m_siPopulation)) == FALSE)
		{
			dwResponse = ON_RET_NOT_ENOUGH_POPULATION;
			goto END;
		}
		//葆擊曖 譆模 檣掘熱蒂 薯ии棻.
		if(pVillage->LimitDecreasePopulation((UI32)(pOnReqVillageDefenceSoldier->siDefenceSoldierCount * pItemHeader->m_pImperium->m_siPopulation)) == FALSE)
		{
			dwResponse = ON_RET_NOT_LIMIT_POPULATION;
			goto END;
		}

		//望萄 爾晝ヶ熱蒂 還檣棻.
		pclGuild->DecreaseGuildSupply(siSupply);
		//渠寞檜 陛雖堅 氈朝 絲擊 還檣棻.
		DecreaseMoney(mnMoney,FALSE);
		//葆擊 檣掘蒂 馬模衛鑑棻.
		pVillage->DecreasePopulation((UI32)(pOnReqVillageDefenceSoldier->siDefenceSoldierCount * pItemHeader->m_pImperium->m_siPopulation));
		//葆擊 熱綠煽 熱蒂 隸陛 衛鑑棻.
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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//ON_REQUEST_GET_VILLAGE_DEFENCE_SOLDIER
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//葆擊 熱綠煽 熱蒂 橢橫螞棻.
//SOVillage::m_siDefenceSoldier
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//Ы溯檜橫縑啪 ��擊 п遽棻.
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

		//偽擎 鼻欽曖 模樓檜 �捐Я牉�.
		if(uiGuildID == pTargetPlayer->uiGuildID)
		{
			//奢撩 嶸棉檜 蜃朝陛蒂 �挫恉挬�.
			if(pTargetPlayer->IsFieldAttackUnit() == FALSE)	return;
			if(IsFieldAttackUnit()				  == FALSE)	return;
			
			if(FieldAttackDead() || pTargetPlayer->FieldAttackDead()) return;

			//濠褐檜 爾晝熱溯檜賊憮 爾晝熱溯縑啪 醱瞪 й熱 橈棻.
//			if(pMyWorld->m_FieldBattleParser.GetUnitKind(pTargetPlayer->uiKind) == FIELD_BATTLE_SUPPLY)	return;
//			if(CSiegeUnitTable::GetInstance()->GetKindUnitCode(pTargetPlayer->uiKind) == FIELD_BATTLE_SUPPLY) return;
			//爾晝熱溯陛 陛雖堅 氈朝 爾晝啪檜雖陛 醱瞪腎橫 氈朝雖蒂 �挫恉挬�.
			int nMp = GetFieldMP();			
			if(nMp <= 0 ) return;

			int nHealAmount = pTargetPlayer->GetFieldMaxHP() - pTargetPlayer->GetFieldHp();
		
//			if(nTargetHP < 0 || nTargetHP >= pTargetPlayer->GetFieldMaxHP()) return; 

			if(nHealAmount <= 0) return;
			
			//��п還曄擊 啗骯и棻.
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
				         LPVOID(siHealAmount),						    // Player陛 ��п遽曄
				         LPVOID(GetFieldMP()),                           // 陴擎 葆釭曄 
				         LPVOID(pOnReqGetSupplyAtGuildUnit->uiTargetID), // TargetPlayerID
				         LPVOID(pTargetPlayer->GetFieldHp()));           // TargetPlayer曖 ⑷營 hp

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
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//ON_REQUEST_GUILD_SET_RELATION
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//鼻欽婁曖 瞳渠 婦啗蒂 撲薑и棻.
//cltGuild::m_siGuildRelation,cltGuild::m_siGuildRelationEnemyTime 擊 撲薑и棻.
//鼻欽除曖 婦啗蒂 憮煎 撲薑.瞳渠 勒薹 瞳渠 嫡擎薹 曄薹棻 瞳渠 鼻鷓檜棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvGuildSetRelation()
{
	OnReqGuildSetRelationMsg	*pGuildSetRelationMsg;
	BOOL						bSuccess = FALSE;
	SI32						siRelation;
	BOOL						bRelation = FALSE;
	
	// 渠寞檣雖 羹觼и棻.
	if(uiGuildID == 0)	goto	END;

	if(siClassInGuild != pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))	goto	END;

	pGuildSetRelationMsg	=	(OnReqGuildSetRelationMsg*)cMsg;

	siRelation				=	pMyWorld->clIGuild.ResponseDefineToRelationDefine(pGuildSetRelationMsg->dwResponse);
	
	//曄薹 鼻欽除曖 瞳渠蒂 撲薑и棻.
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



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//ON_REQUEST_GUILD_FRIELDLY_MESSAGE_OFFER
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//�俸�薯曖 詭撮雖蒂 爾魚棻.
//�俸�薯曖 詭撮雖蒂 爾頂朝 鼻欽曖 詭撮雖陛 
//�俸�薯曖 嫡朝 鼻欽曖 cltGuild::m_szGuildfriendlyMessage縑 盪濰脹棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvFriendlyMessageOffer()
{	
	OnReqGuildFriendlyMessageOffer*	TempOnReqGuildFriendlyMessageOffer;
	DWORD							dwResponse;

	TempOnReqGuildFriendlyMessageOffer		=	( OnReqGuildFriendlyMessageOffer* )cMsg;

	//奢撩ж朝 陳檣雖 羹觼и棻.
/*	SYSTEMTIME  sysTime;
	GetSystemTime(&sysTime);
	if(sysTime.wDayOfWeek == 2|| sysTime.wDayOfWeek == 4)
	{
		dwResponse = ON_RET_NO;
		SendFieldMsg(ON_RESPONSE_GUILD_FRIELDLY_MESSAGE_OFFER, SO_SFM_ONLYME, LPVOID(dwResponse));
		return;
	}
*/
	//奢撩ж朝 蹂橾縑 衛除檜賊 �俸�薯曖蒂 й熱橈棻.
	if(pMyWorld->m_WarTimeParser.IsWarCheck(pMyWorld->pRealTime->tm_wday, pMyWorld->pRealTime->tm_hour)) return;
//	if(pMyWorld->m_WarTimeParser.IsWarTime(pMyWorld->pRealTime->tm_hour)) return;
	
	if(pMyWorld->clIGuild.SetFriendlyMessage(uiGuildID,TempOnReqGuildFriendlyMessageOffer->szGuildID,TempOnReqGuildFriendlyMessageOffer->szMessage) == FALSE)
		dwResponse = ON_RET_NO;
	else 
		dwResponse = ON_RET_OK;	
	
	SendFieldMsg(ON_RESPONSE_GUILD_FRIELDLY_MESSAGE_OFFER, SO_SFM_ONLYME, LPVOID(dwResponse));
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//ON_REQUEST_GUILD_FRIENDLY_MESSAGE_ACCEPT
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//�俸�薯曖蒂 嫡擎 鼻欽縑憮 �俸�薯曖蒂 ъ塊и棻.
//鼻欽除曖 婦啗 蟾晦�� 脹棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvFriendlyMessageAccept()
{
	OnReqGuildFriendlyMessageAccept*	TempOnReqGuildFriendlyMessageAccept;

	TempOnReqGuildFriendlyMessageAccept		=	( OnReqGuildFriendlyMessageAccept* )cMsg;

	pMyWorld->clIGuild.SetFriendlyMessageAccept(uiGuildID,TempOnReqGuildFriendlyMessageAccept->szGuildID);

	SendFieldMsg(ON_RESPONSE_GUILD_FRIENDLY_MESSAGE_ACCEPT, SO_SFM_ONLYME);
} 



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//ON_REQUEST_GUILD_FRIENDLY_MESSAGE_CANCEL
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//�俸�薯曖蒂 嫡擎 鼻欽縑憮 �俸�薯曖蒂 剪瞰и棻.
//�俸�薯曖蒂 嫡擎 鼻欽曖 msg虜 餉薯脹棻.
//瞳渠 婦啗朝 斜渠煎 陴嬴 氈棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvFreidnlyMessageCancel()
{
	OnReqGuildFreindlyMessageCancel*	TempOnReqGuildFreindlyMessageCancel;

	TempOnReqGuildFreindlyMessageCancel		=	( OnReqGuildFreindlyMessageCancel* )cMsg;

	pMyWorld->clIGuild.SetFriendlyMessageCancel(uiGuildID,TempOnReqGuildFreindlyMessageCancel->szGuildID);

	SendFieldMsg(ON_RESPONSE_GUILD_FRIENDLY_MESSAGE_CANCEL, SO_SFM_ONLYME);
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//ON_REQUEST_GUILD_VILLAGEATTACK_RESULT
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//奢撩檜 部陪�� 鼻欽曖 奢撩 唸婁 ァ癱 й熱 氈朝 掏и擊 陛霞 葆擊擊 蹂羶и棻.
//cltGuild::m_stGuildResult 盪濰腎橫 氈朝 高.
//檜 高擎 葆擊 だ惚衛 п渡 鼻欽縑 盪濰脹棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvVillageAttackResult()
{
	OnReqVillageAttackResult*			pOnReqVillageAttackResult;
	cltGuild*							pclGuild;
	DWORD								dwResponse = ON_RET_OK;
	SI16								siVillageCount = 0;

	pOnReqVillageAttackResult		=	(OnReqVillageAttackResult*)cMsg;

	//濠晦曖 GuildID	= uiGuildID
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//ON_REQUEST_GUILD_CHOICE_PLAYER
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//渠寞檜 陛霞 ァ癱 掏и擊 п渡 鼻欽曖 Ы溯檜橫縑啪 剩啖遽棻.
//cltGuild::m_stGuildResult縑 п渡ж朝 Ы溯檜橫蒂 盪濰衛麵遽棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
		//渠寞縑啪 憲萼棻.
		SendFieldMsg(ON_RESPONSE_GUILD_CHOICE_PLAYER,SO_SFM_ONLYME,LPVOID(dwResponse),
			LPVOID(pOnReqGuildChoicePlayer->siVillageCode),LPVOID(pOnReqGuildChoicePlayer->szUserName));
		//癱濠掏擊 嫡朝 Ы溯檜橫縑啪 憲萼棻.
		pOpponentPlayer->SendFieldMsg(ON_RESPONSE_GUILD_CHOICE_PLAYER,SO_SFM_ONLYME,LPVOID(dwResponse),
			LPVOID(pOnReqGuildChoicePlayer->siVillageCode),LPVOID(pOnReqGuildChoicePlayer->szUserName));
		break;
	case ON_RET_NO:
		//褒ぬ衛 渠寞縑啪虜 憲萼棻.
		SendFieldMsg(ON_RESPONSE_GUILD_CHOICE_PLAYER,SO_SFM_ONLYME,LPVOID(dwResponse),LPVOID(pOnReqGuildChoicePlayer->siVillageCode)
			,LPVOID(pOnReqGuildChoicePlayer->szUserName));
		break;
	}	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//ON_REQUEST_VILLAGE_INVESTMENT_POWER
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//Ы溯檜橫陛 癱濠蒂 й 陽 ァ癱掏и檜 氈朝 鼻鷓檣陛 憮幗縑 僭橫獄棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

	//鼻欽 癱濠掏и檜 氈朝雖 匐餌.
	for(SI16 i = 0; i < ON_MAX_GUILD_RESULT; i++)
	{
		if(pclGuild->m_stGuildResult[i].GetVillageCode() == pOnReqInvestmentPower->siVillageCode)
		{
			if(strcmp(pclGuild->m_stGuildResult[i].GetCharName(),pOnReqInvestmentPower->szUserName) == 0)
			{
				dwResponse = ON_RET_OK;
				siPercent  = pclGuild->m_stGuildResult[i].GetPercent();
				//葆擊 癱濠衛 薯擒 褻勒擊 ヴ橫輿朝 Ы楚斜蒂 Onп遽棻.
				m_bInvestmentPower = TRUE;
			}
		}
	}

	SendFieldMsg(ON_RESPONSE_VILLAGE_INVESTMENT_POWER,SO_SFM_ONLYME,LPVOID(dwResponse),LPVOID(siPercent));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// ON_REQUEST_WAR_WARP
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 奢撩嶸棉檜 橾奩 嶸棉擊 錶Щ 衛酈朝 詭撮雖.
// Response朝 晦襄 Warp蒂 檜辨и棻.
// ON_RESPONSE_WAR_WARP朝 餌辨ж雖 彊朝棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// ON_REQUEST_NORMAL_CHAR
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 奢撩 嶸棉檣 鼻鷓縑憮 橾奩嶸棉戲煎 滲褐и棻.
// 濠晦 濠褐檜 橾奩嶸棉戲煎 滲褐ж朝 詭撮雖.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvNormalChar()
{
	OnReqNormalChar*		pOnReqNormalChar;
	OnlineFollower*			pFollower = IFollower.GetFollower(0);

	pOnReqNormalChar	=	(OnReqNormalChar*)cMsg;	

	ChangeNormalChar();
}

//奢撩瞪 嶸棉戲煎 滲褐и棻.
VOID SOPlayer::RecvChangeUnit()
{
	OnReqChangeUnit        *pReqChangeUnit;
	pReqChangeUnit = (OnReqChangeUnit*)cMsg;

	SI16 siRes = ChangeUnit(pReqChangeUnit->siChangeKind, siClassInGuild);
	if(siRes != ON_CHANGE_OK)
		SendFieldMsg(ON_RESPONSE_CHANGE_WAR, SO_SFM_ONLYME, LPVOID(0), LPVOID(0), 
		             LPVOID(0), LPVOID(0), LPVOID(0),LPVOID(siRes));
	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 奢撩 婦溼 橾奩 л熱菟 
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
//爾晝熱溯檣 唳辦 葆釭榆擊 陛螳螞棻
SI32 SOPlayer::GetFieldMP()
{
	return m_siFieldMP;
}
//爾晝熱溯檣唳辦 葆釭榆擊 撮たи棻.
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


// 鼻欽檜 п羹 腎歷擊 陽 議葛攪陛 陛雖堅 氈朝 奢撩 婦溼嬴檜蠱擊 餉薯и棻.
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

//奢撩嶸棉戲煎 滲褐擊 и棻.[2005/01/14] - 譆煽輿
UI16 SOPlayer::ChangeUnit(UI16 uiCode, UI16 uiQuantity)
{
	if(uiCode < 0) return ON_CHANGE_FAILED;
	if( pMyWorld->IsAttackVillageTimeZone() == FALSE )  return ON_CHANGE_FAILED;
	
	if(m_bIsWarUnit) return ON_CHANGE_FAILED;

	SI16 siUnitCode = CSiegeUnitTable::GetInstance()->GetKindUnitCode(uiCode);
	if(siUnitCode < 0) return ON_CHANGE_FAILED;
	
	OnlineFollower *pFollower = IFollower.GetFollower(0);
	_ImportantParameta* pParam  = &pFollower->ParametaBox.IP;

	//鼻欽縑 陛殮腎橫 氈朝雖 匐餌и棻.
	cltGuild *pclGuild;
	pclGuild = pMyWorld->clIGuild.GetGuildByID(uiGuildID);
	if(pclGuild == NULL) return ON_CHANGE_NOT_GUILD;
    
	//奢撩擊 й熱 氈朝 鼻欽檣雖 匐餌и棻.
	if(pclGuild->IsWar() == FALSE) return ON_CHANGE_NOT_GUILD;
	
	//雖錳濠檣雖 匐餌....
	if(uiQuantity == pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ))
		return ON_CHANGE_NOT_GUILD;
	
	//溯漣檜 10爾棻 濛戲賊 鼠褻勒 褒ぬ
	UI16 uiLevel = pParam->GetLevel();
	if(uiLevel < 10) 
		return ON_CHANGE_ENOUGH_LEVEL;
	

	//滲褐й熱 氈朝 溯漣擊 憲嬴螞棻.
	int nChangeLevel = CSiegeUnitTable::GetInstance()->GetLevel(siUnitCode);
	//溯漣檜 雪戲賊 滲褐 褒ぬ
	if(uiLevel < nChangeLevel)
		return ON_CHANGE_ENOUGH_LEVEL;

	
	//滲褐й 嶸棉曖 偃熱陛 陴嬴 氈朝雖 匐餌.
	SI16 siUnitCount = pclGuild->m_GuildUnit.GetGuildUnit(siUnitCode);
	if(siUnitCount <= 0) return ON_CHANGE_ENOUGH_KIND;
	
	SI16 siNeedCount = 0;	
	
	//嶸棉 熱蒂 還檣棻.
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

	//滲褐ж朝等 в蹂и 綠辨
	int nChangeMoney = CSiegeUnitTable::GetInstance()->GetChangePrice(siUnitCode);
	nChangeMoney = nChangeMoney * siNeedCount;
	
	//滲褐 й熱 氈朝 絲檜 橈戲賊 褒ぬ
	if(GetMoney() < nChangeMoney) 
		return ON_CHANGE_ENOUGH_MONEY;
	

//-----------------------------------------------------------------
//  議葛攪蒂 撲薑п輿朝 睡碟.....

	//滲褐 綠辨 虜躑 絲擊 賓棻.
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
	
	//陛螳陛堅 陴擎 嬴檜蠱曖 識偎熱.
	pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_DECREASE_GUILD_UNIT, 
		                               LPVOID(&DecreaseGuildUnit), 
									   sizeof(DBIKDecreaseGuildUnit));

	//嶸棉曖 樓撩擊 滲唳и棻.
	SetPrevKind(uiKind);
	uiKind = uiCode;
	pFollower->uiCharID = uiCode;
	
	//渠濰 嶸棉檣雖 撲薑и棻.
	if(uiQuantity == pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI)||
	   uiQuantity == pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS))
		SetCaptainUnit(TRUE);
	else
		SetCaptainUnit(FALSE);

	//羹溘擊 撲薑и棻
	int BP = GetFightingPower();
	SI16 siHP = CSiegeUnitTable::GetInstance()->GetHP(siUnitCode, BP);
	
	//渠寞檣 唳辦 2寡曖 羹溘擊厥朝棻.
	if(uiQuantity == pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI)||
	   uiQuantity == pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS)) siHP *= 2; 
	
	SetFieldHp(siHP);
	SetFieldMaxHP(siHP);
	//爾晝 熱溯檣唳辦 葆釭榆擊 撲薑и棻.
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
	ImperumUse.siMp		       = siMP;        // 橾奩 嶸棉擎 爾晝 啪檜雖蒂 0戲煎 и棻.
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
		case 1: //憮
			{
				for(i = 0; i < nCount; i++)
				{
					siEX = siSX - i;
					siEY = siEY + i;
					
					puiIDs[i] = pMyWorld->clISOMap.GetCharUniID(uiMapID, siEX, siEY, ON_MAP_CHARID_PLAYER);
				}
			}
			break;

		case 2: //磁憮
			{				
				for(i = 0; i < nCount; i++)
				{
					siEY = siEY;
					siEX = siSX - i;					
					
					puiIDs[i] = pMyWorld->clISOMap.GetCharUniID(uiMapID, siEX, siEY, ON_MAP_CHARID_PLAYER);
				}
			}
			break;

		case 3: //磁
			{
				for(i = 0; i < nCount; i++)
				{
					siEX = siSX - i;
					siEY = siEY - i;
					
					puiIDs[i] = pMyWorld->clISOMap.GetCharUniID(uiMapID, siEX, siEY, ON_MAP_CHARID_PLAYER);
				}
			}
			break;

		case 4: //陴憮
			{
				for(i = 0; i < 2; i++)
				{
					siEX = siSX;
					siEY = siEY + i;
					
					puiIDs[i] = pMyWorld->clISOMap.GetCharUniID(uiMapID, siEX, siEY, ON_MAP_CHARID_PLAYER);
				}
			}
			break;

		case 6: //磁翕
			{
				for(i = 0; i < nCount; i++)
				{
					siEX = siSX;
					siEY = siEY - i;
					
					puiIDs[i] = pMyWorld->clISOMap.GetCharUniID(uiMapID, siEX, siEY, ON_MAP_CHARID_PLAYER);
				}
			}
			break;

		case 7: //陴
			{
				for(i = 0; i < nCount; i++)
				{
					siEX = siSX + i;
					siEY = siEY + i;
					
					puiIDs[i] = pMyWorld->clISOMap.GetCharUniID(uiMapID, siEX, siEY, ON_MAP_CHARID_PLAYER);
				}
			}
			break;

		case 8: //翕陴
			{
				for(i = 0; i < nCount; i++)
				{
					siEX = siSX + i;
					siEY = siEY;
					
					puiIDs[i] = pMyWorld->clISOMap.GetCharUniID(uiMapID, siEX, siEY, ON_MAP_CHARID_PLAYER);
				}
			}
			break;

		case 9: //翕
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

	if(m_bPlayerVillageInLimit) //熱撩 檣攪む檜蝶縑憮 釭除勘煎 籀葬
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

	//嘐葬 盪濰п 堡湍 濠褐曖 褒薯 議葛攪曖 kind煎 滲褐и棻.
	uiKind				= m_uiPrevKind;
	pFollower->uiCharID = m_uiPrevKind;


	//奢撩 婦溼 等檜攪菟擊 蟾晦��.
	m_bFieldAlive = TRUE;
	m_dwDeadTime  = pMyWorld->dwtimeGetTime;
	m_dwDeadSec	  = 1000 * 30;

	SetFieldHp(0);
	SetPrevKind(GetPrevKind());
	SetCaptainUnit(FALSE);
	SetFieldMP(0);
	SetFieldMaxHP(0);
	SetFieldMaxMP(0);

	if(m_bPlayerVillageInLimit) //熱撩 檣攪む檜蝶縑憮 釭除勘煎 籀葬
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
	
	//避擎 奢撩嶸棉擊 奢問ц擊 唳辦..
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

		if(m_bPlayerVillageInLimit) //熱撩 檣攪む檜蝶縑憮 釭除勘煎 籀葬
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
	SI32  siBestIndex = pVillage->m_Government->GetOldBestInvestIndex(); //譆癱曖 檣策蝶蒂 橢橫螞棻.

	if(pOnGiveUp->uiResponse == ON_GUILD_GIVEUP_OK)
	{   //鼻欽 婦啗蒂 瞳渠煎 夥脯遽棻.
		if(pMyWorld->clIGuild.GiveUp(uiGuildID))
		{   //譆癱曖 檣策蝶蒂 橢橫諦憮 癱濠旎曖 10%蒂 梧朝棻....			
			mnMoney = pVillage->m_Government->DelInvestMoney(siBestIndex, 1, TRUE);

			//鼻欽錳菟縑啪 渠寞檜 奢撩擊 ん晦ц棻堅 憲溥遽棻.
			pclGuild->SendGiveUpMsg();

			//瞪羹 奢雖蒂 п遽渡...
			pMyWorld->OfficialAnnouncement(ON_RET_ON_GUILD_GIVEUP, LPARAM(pclGuild->GetGuildName()), LPARAM(0));

		}
	}
	else
	{
		pOnGiveUp->uiVillageCode = 0;
		pOnGiveUp->uiResponse = ON_GUILD_GIVEUP_NORMAL;
	}

	pclGuild->SetGiveUpFlag(pOnGiveUp->uiResponse);
	
	//蛤綠縑 薑爾蒂 瞪歎и棻.
	DBIKWarGiveUp           warGiveUp;
	memset(&warGiveUp, 0, sizeof(DBIKWarGiveUp));

	warGiveUp.uiGuildID     = uiGuildID;
	warGiveUp.uiVillageCode = pOnGiveUp->uiVillageCode;
	warGiveUp.mnMoney       = mnMoney;
	warGiveUp.siFlag        = pOnGiveUp->uiResponse;
	pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_SET_WARGIVEUP, &warGiveUp, sizeof(DBIKWarGiveUp));
	
}