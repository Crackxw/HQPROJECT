#include "SOMain.h"
#include "SOWorld.h"
#include "SOPlayer.h"
#include "ParametaBox.h"

//��������������������������������������������������������������������������������������������
//ON_IMPERIUM_USE
//��������������������������������������������������������������������������������������������
//�������� �Ծ����� SpendItem�� ON_ITEM_SPECIAL_IMPERIUM�� �߻��Ѵ�.
//�̿� ���� �������� ON_IMPERIUM_USE�� Ŭ���̾�Ʈ�� �����Ѵ�.
//��������������������������������������������������������������������������������������������


//��������������������������������������������������������������������������������������������
//ON_REQUEST_FIELDATTACK_READY 
//��������������������������������������������������������������������������������������������
//���� ���ְ� ������ �ο򿡼� ���⸦ �߻��Ѵٴ� �޼����� ������.
//��������������������������������������������������������������������������������������������
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

	//�´� �÷��̾�.
	pTargetPlayer =	pMyWorld->GetPlayer(pOnReqFieldAttackRdyMsg->uiUniqueID);

	//���� �ð��ΰ�
	if( pMyWorld->IsAttackVillageTimeZone() == FALSE )
	{
		dwResponse = ON_RET_NOT_ATTACK_TIME;
		goto END;
	}

	if(pTargetPlayer != NULL)
	{   //���� ����ΰ�
		if( pMyWorld->clIGuild.IsEnemyRelationWar( uiGuildID, pTargetPlayer->uiGuildID ) == FALSE )
		{
			dwResponse = ON_RET_NOT_ENEMYGUILD;
			goto END;
		}
		
		//���� ���� �ΰ�
		if(pTargetPlayer->IsFieldAttackUnit() == FALSE)	
		{
			dwResponse = ON_RET_NOT_ATTACK_UNITE;
			goto END;
		}

		//��� �ֳ�
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
	//���� �ð��� ���Ѵ�.
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
		//�ڽ��� ������ ������ ��� ����鿡�� ������.
		SendFieldMsg(ON_RESPONSE_FIELDATTACK_READY, SO_SFM_MEANDNEARCHAR,
			         LPVOID(dwResponse), LPVOID(uiAccount),                   
					 LPVOID(pOnReqFieldAttackRdyMsg->uiUniqueID),
					 LPVOID(pFollower->uiCharID), LPVOID(dwTempTime), LPVOID(bIsLong));
	}
/*	else
	{
		//������ ������Ű�� ���Ұ�� �ڱ� �ڽſ��Ը� ���� �޼����� ������.
		SendFieldMsg(ON_RESPONSE_FIELDATTACK_READY, SO_SFM_ONLYME,
			         LPVOID(dwResponse), LPVOID(uiAccount),                   
					 LPVOID(pOnReqFieldAttackRdyMsg->uiUniqueID),
					 LPVOID(pFollower->uiCharID), LPVOID(dwTempTime), LPVOID(bIsLong));
	}
	*/
}


//��������������������������������������������������������������������������������������������
//ON_REQUEST_FIELDATTACK_ACTION
//��������������������������������������������������������������������������������������������
//���� ���ְ��� �ο򿡼� ���⿡ �¾Ҵٴ� �޼����� ������.
//��������������������������������������������������������������������������������������������
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
		//�´� ����� ������������ Ȯ���Ѵ�.
		if(!pTargetPlayer->IsFieldAttackUnit())	return;
		if(!pTargetPlayer->IsFieldAlive())	return;
	}
	
	//������ ����� ������������ Ȯ���Ѵ�.
	if(IsFieldAttackUnit() == FALSE) return;
	
	pTargetFollower = pTargetPlayer->IFollower.GetFollower(0);
	if(pTargetFollower == NULL) return;

	//���� ���������� ó�����ش�. 
	if(FieldAttackDead()) return;
	if(pTargetPlayer->FieldAttackDead()) return;

	//�����ڵ带 ���´�
	SI16 siUnitCode = CSiegeUnitTable::GetInstance()->GetKindUnitCode(uiKind);
	if(siUnitCode < 0 || siUnitCode == FIELD_BATTLE_RAMPART) 
	{
		dwResponse = ON_RET_NO;
		return;
	}
	
	//���� ������ ���
	DWORD dwNowTick = GetTickCount();
	DWORD dwTempTime = dwNowTick - m_dwAttackDelay;
	if(dwTempTime < CSiegeUnitTable::GetInstance()->GetDelay(siUnitCode))
	{
		dwTempTime = CSiegeUnitTable::GetInstance()->GetDelay(siUnitCode) - dwTempTime;
		dwResponse = ON_RET_NOT_ATTACK_DELAY_TIME;
		return;
	}
	m_dwAttackDelay = dwNowTick; //���� �ð��� �������ش�.

	//�������� ����Ѵ�.
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
	
	//�߰��� ������ ��Ʈ�Ǿ��ٸ�.
	if(siAttackPower >= siRand)
	{	
		 //���� �ַ��� �������� 1.5���� �������� �ش�.
		 float fTempDamage = (float)siDamage * 1.5;
		 siDamage	  = (SI16)fTempDamage;
		 siAttackType = ON_RET_ATTACK_POWER;
	}
	//�´� �ΰ��� �߰��� �� �����Ͽ���.
	else if(siDefencePower < siRand)
	{
		siDamage = 1;
		siAttackType = ON_RET_DEFENCE_POWER;
	}

	pTargetPlayer->DecreaseFieldHp(siDamage);
	
	if(pTargetPlayer->GetFieldHp() <= 0) m_siKillNum++;

	if(dwResponse == ON_RET_OK)
	{
		//�������� �����ֱ� ���� ������鿡�� ������.
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
		//���÷��� ������ ó��
		UI16 uiDir = 0;
		int i = 0;
		UI16 uiTargetID[9];
		memset(uiTargetID, 0, sizeof(UI16) * 9);
		
		if(siUnitCode == FIELD_BATTLE_SPEAR) //â���ΰ��
		{
			//������ �˾ƿ´�.
			uiDir = GetDir(siX, siY, pTargetPlayer->siX, pTargetPlayer->siY);

			//���÷��� �������� �޴� ���� �ε����� �˾ƿ´�.
			GetSpTargetIDOFSpear(uiDir, pTargetPlayer->siX, pTargetPlayer->siY, uiTargetID);
			
		}
		else if(siUnitCode == FIELD_BATTLE_SIEGE) //�߼����ΰ��
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
			if(pPlayer->IsFieldAttackUnit() == FALSE) continue; //���� �������� �˻�
			if(uiGuildID == pPlayer->uiGuildID) continue; //���� ������� �˻�
			if(pMyWorld->clIGuild.IsEnemyRelationWar( uiGuildID, pPlayer->uiGuildID) == FALSE ) continue;

			
			pPlayer->DecreaseFieldHp(siDamage);

			pPlayer->SendFieldMsg(ON_RESPONSE_FIELDATTACK_ACTION, SO_SFM_MEANDNEARCHAR, 
								  LPVOID(uiAccount), LPVOID(siDamage), LPVOID(m_bFieldAlive),
								  LPVOID(pPlayer->uiAccount), LPVOID(dwResponse), LPVOID(siAttackType));
		}	
	}
}

//��������������������������������������������������������������������������������������������
//ON_REQUEST_FIELDATTACK_DEAD
//��������������������������������������������������������������������������������������������
//���� ���ְ��� �ο򿡼� �׾��ٴ� �޼����� ������.
//��������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvFieldAttackDead()
{
	OnReqFieldAttackDeadMsg*		pOnReqFieldAttackDeadMsg;
	pOnReqFieldAttackDeadMsg	=	(OnReqFieldAttackDeadMsg*)cMsg;
	
	OnlineFollower*					pFollower = IFollower.GetFollower(0);
	if(pFollower == NULL) return;
	if(IsFieldAttackUnit() == FALSE) return;

	if(m_bCaptainUnit == FALSE)
	{
		//�������� ���ϱ� ���� �ð��� �����Ѵ�.
		m_dwDeadTime = pMyWorld->dwtimeGetTime;
		//�г�Ƽ�� �޴� �ð��� �����Ѵ�. ��� 30�� 
		m_dwDeadSec  = 30000;
		m_bFieldAlive = FALSE;
	}
	else
	{
		//�������� ���ϱ� ���� �ð��� �����Ѵ�.
		m_dwDeadTime = pMyWorld->dwtimeGetTime;
		//�г�Ƽ�� �޴� �ð��� �����Ѵ�. ���� 1��
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

	if(m_bPlayerVillageInLimit) //���� �������̽����� �����ɷ� ó��
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

//��������������������������������������������������������������������������������������������
//ON_REQUEST_LONGDISTANCE_VILLAGEATTACK_READY
//��������������������������������������������������������������������������������������������
//���Ÿ� ���������� ������ ������ �� ���⸦ �߻��Ѵٴ� �޼����� ������.
//���� ������ �ٰŸ��� ���Ÿ��� �������� : 
//�ٰŸ��� ��� ������ �ݰ��� �����ϰ� ���Ÿ��� ��� ����ȭ�� ����� �ϴ� ������ �ѷ� ������.
//��������������������������������������������������������������������������������������������
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

	// �� �޽����� �ޱ⿡ ��ȿ�� �ð����� �˾ƿ´�.	
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

	// ���� ������ ����� �ִ��� �˻��Ѵ�.
	if( pVillage->uiGuildID == 0 )																		
	{
		dwResponse = ON_RET_NOT_VILLAGE_CODE;
		goto END;
	}

	// �÷��̾��� ��ܰ� �� ������ ����� ���������� �˻��Ѵ�.
	if( pMyWorld->clIGuild.IsEnemyRelationWar( uiGuildID, pVillage->uiGuildID ) == FALSE )
	{
		dwResponse = ON_RET_NOT_ENEMYGUILD;
		goto END;
	}

	//���� �ð��� ���Ѵ�.
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
	//�� ���ǵ��� ���� ��Ų�ٸ� �ڽ��� ������ ������ ��� ����鿡�� ������.
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

//��������������������������������������������������������������������������������������������
//ON_REQUEST_LONGDISTANCE_VILLAGEATTACK_ACTION
//��������������������������������������������������������������������������������������������
//���Ÿ� ���������� ������ ������ �� ���Ⱑ �¾Ҵٴ� �޼����� ó�����ش�.
//������ �ı��Ǿ������� �ϵ� ó���Ѵ�.
//��������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvLongDistanceAttackVillageAction()
{
	OnReqLongAttackVillageActionMsg		*pOnReqLongAttackVillageActionMsg;
	SOVillage							*pVillage;
	BOOL                                 bIsLong = FALSE;

	pOnReqLongAttackVillageActionMsg	=	(OnReqLongAttackVillageActionMsg*)cMsg;
	
	//������ ������ �ð��ΰ� üũ..
	if( pMyWorld->IsAttackVillageTimeZone() == FALSE ) return;	
	if((pVillage = pMyWorld->GetVillageByCode( pOnReqLongAttackVillageActionMsg->uiVillageCode)) == NULL ) return;	
	if( pVillage->GetVillageDefence() <= 0) return;
	// ���� ������ ����� �ִ��� �˻��Ѵ�.
	if( pVillage->uiGuildID == 0 ) return;
	// �÷��̾��� ��ܰ� �� ������ ����� ���������� �˻��Ѵ�.
	if( pMyWorld->clIGuild.IsEnemyRelationWar( uiGuildID, pVillage->uiGuildID ) == FALSE ) return;	

	int nUnitCode = CSiegeUnitTable::GetInstance()->GetKindUnitCode(uiKind);
	if(nUnitCode < 0) return;
	
	if(nUnitCode == FIELD_BATTLE_ARROW || nUnitCode == FIELD_BATTLE_SIEGE)
		bIsLong = TRUE;
	
	//������ �����Ѵ�.
	if(pVillage->AttackDamage(this, pOnReqLongAttackVillageActionMsg->siAttackType, bIsLong) == TRUE)
	{
		if(pVillage->uiGuildID != 0)
			pMyWorld->clIGuild.DelVillage(pVillage->uiGuildID, pVillage, FALSE);
	}

	/*
	//������ ��� ���� �ı��Ǿ��� �ľ� ���.
	if(pVillage->LongDistanceAttack(this,pOnReqLongAttackVillageActionMsg->siAttackType) == TRUE)
	{
		// ������ ������ �ı��Ǿ���.
		if(pVillage->uiGuildID != 0)
			pMyWorld->clIGuild.DelVillage(pVillage->uiGuildID, pVillage, FALSE);
	}	
	*/
}

//��������������������������������������������������������������������������������������������
//ON_REQUEST_ATTACK_VILLAGE
//��������������������������������������������������������������������������������������������
//���������� ������ �����Ҷ� ������ ������.������ �¾Ҵ�.
//��������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvAttackVillage()
{
	OnReqAttackVillageMsg		*pAttackVillageMsg;
	SOVillage					*pVillage;	
	BOOL                         bIsLong = FALSE;

	// �� �޽����� �ޱ⿡ ��ȿ�� �ð����� �˾ƿ´�.	
	if( pMyWorld->IsAttackVillageTimeZone() == FALSE )													return;		
	pAttackVillageMsg	=	(OnReqAttackVillageMsg*)cMsg;

	if( ( pVillage = pMyWorld->GetVillageByCode( pAttackVillageMsg->uiVillageCode ) ) == NULL )			return;	
	if( pVillage->GetVillageDefence() <= 0) return;

	// ���� ������ ����� �ִ��� �˻��Ѵ�.
	if( pVillage->uiGuildID == 0 )																		return;

	// �÷��̾��� ��ܰ� �� ������ ����� ���������� �˻��Ѵ�.
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
		// ������ ������ �ı��Ǿ���.
		if(pVillage->uiGuildID != 0)
			pMyWorld->clIGuild.DelVillage(pVillage->uiGuildID, pVillage, FALSE);
	}
	*/
}
//��������������������������������������������������������������������������������������������
//ON_REQUEST_WAR_VILLAGE_DEFENCE
//��������������������������������������������������������������������������������������������
// ������ �ϱ����� ���� ������ ����.
// ������ ������ �� ���´� �ƴϰ� �ʵ� �Ӽ��� ������ �����鼭 ������ �����͸� ������ �ִ´�.
// �ʵ�->���� �������̽�.
//��������������������������������������������������������������������������������������������
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

	stAccountInfo.pVillage	=	pMyWorld->GetVillageByCode(pOnReqWarVillageDefence->uiVillageCode);				// ������ ã�´�.
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

	if(siUnitCode != FIELD_BATTLE_SWORD) //�˺��� �����ϴ�.
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

	//�� �������� �Ѹ��� �÷��̾ ���� �������̽��� �� ������ �ִ�.
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

	// ������ Guild ID�� ���Ѵ�.
	uiVillageGuildID		=	stAccountInfo.pVillage->GetGuildNID();

	// �� ������ ����̸� �� �� ����.
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

	// ����â�� �ݴ´�.
	CloseTrade();

	// Ÿ �������� ��Ƽ ��û���̰ų� 
	// �ٸ� ������ ��Ƽ ��û�� �޾Ҵٸ� �̸� ����Ѵ�.
	CloseRequestingJoinParty();

	//�����������̽��� ���� �ִٴ� �÷��׸� ����.
//	pVillage->m_bVillageInLimit = TRUE;
	m_bPlayerVillageInLimit     = TRUE;
	m_siVilligeCode = pOnReqWarVillageDefence->uiVillageCode;	

	//Ŭ���̾�Ʈ�� �����ֱ� ���ؼ� ���� AttackType�� ������ �ð��� ���.
	if(pVillage->m_dwPrevAttackdelay > 0)
		siLimitTime = (SI32)pVillage->m_dwSkillDelayTime - (pMyWorld->dwtimeGetTime - pVillage->m_dwPrevAttackdelay);

	dwResponse	=	ON_RET_OK;

END:
	// ������ �� �� ����.
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

//��������������������������������������������������������������������������������������������
// ON_REQUEST_WAR_VILLAGE_IN
//��������������������������������������������������������������������������������������������
// �����������̽����� ������ ����.
// �ʵ�Ӽ����� �����Ӽ����� �ٲ��.
// ���� �������̽� -> ����
//��������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvReqWarVillageIn()
{
	OnReqWarVillageIn*			pOnReqWarVillageIn;
	SOVillage*					pVillage	=	NULL;

	pOnReqWarVillageIn	=	(OnReqWarVillageIn*)cMsg;

	stAccountInfo.pVillage	=	pMyWorld->GetVillageByCode(pOnReqWarVillageIn->uiVillageCode);				// ������ ã�´�.
	pVillage				=	pMyWorld->GetVillageByCode(pOnReqWarVillageIn->uiVillageCode);				// ������ ã�´�.

	pVillage->m_bVillageInLimit = FALSE;
	m_bPlayerVillageInLimit		= FALSE;
	m_siVilligeCode = -1;
//	strcpy(pVillage->m_szCharName,"");

	//�ʿ��� �����ش�.
	if(DelIDOnMap( 6 ) == FALSE)
		writeInfoToFile("DelIDOnMap Failed.txt", "5 [%u] [%d] [%d] [%d]", uiAccount, uiMapID, siX, siY);
//			pMyWorld->clServer->WriteInfo("..\\DelIDOnMap Failed.txt", "5 [%u] [%d] [%d] [%d]", uiAccount, uiMapID, siX, siY);

	EnterVillage();		
			
	//������ �˸��� ������ ������ ������.
//	SendFieldMsg(ON_RESPONSE_WAR_VILLAGE_IN, SO_SFM_ONLYME,LPVOID(pVillage));

	//����������� ������ ���� ������ ����.
//	SendFieldMsg(ON_RESPONSE_WAR_VILLAGE_IN, SO_SFM_ONLYNEARCHAR);	
}

//���� �������̽����� ���´�.
VOID SOPlayer::RecvReqWarVillageDefenceStop()
{
	OnReqWarVillageIn     *pVillageIn = (OnReqWarVillageIn*)cMsg;

	SOVillage *pVillage	= NULL;
	pVillage = pMyWorld->GetVillageByCode(pVillageIn->uiVillageCode);				// ������ ã�´�.

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

//��������������������������������������������������������������������������������������������
//ON_REQUEST_VILLAGE_ATTACK_TYPE_CHECK
//��������������������������������������������������������������������������������������������
//���� �������̽��� ����� ����Ҽ� �ִ��� �˾ƿ´�.
//��������������������������������������������������������������������������������������������
/*
VOID	SOPlayer::RecvVillageAttackTypeCheck()
{
	OnReqVillageAttackTypeCheck*		pOnReqVillageAttackTypeCheck;
	SOVillage*							pVillage;
	DWORD								dwResponse = ON_RET_OK;

	pOnReqVillageAttackTypeCheck =		(OnReqVillageAttackTypeCheck*)cMsg;
	pVillage = pMyWorld->GetVillageByCode(pOnReqVillageAttackTypeCheck->uiVillageCode);

	// siClassInGuild ��� ������.-_-����..
	// ����̳� ����� ���� üũ.
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


//��������������������������������������������������������������������������������������������
//ON_REQUEST_VILLAGE_PLAYER_OIL_ATTACK
//��������������������������������������������������������������������������������������������
//���� �������̽� �⸧ ���ݸ޼����� ���� ������ ó���Ѵ�.
//��������������������������������������������������������������������������������������������
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


//��������������������������������������������������������������������������������������������
//ON_REQUEST_VILLAGE_PLAYER_STONE_ATTACK
//��������������������������������������������������������������������������������������������
//���� �������̽� �� ���ݸ޼����� ���� ������ ó���Ѵ�.
//��������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������
//ON_REQEUST_VILLAGE_DEFENCE
//��������������������������������������������������������������������������������������������
//���� �������̽� ���� ���� ��� ���� ������ ó���Ѵ�.
//��������������������������������������������������������������������������������������������
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

	//������ ������ false
	if(pVillage == NULL)				               goto END;
	if(pVillage->vdMaxDefence == pVillage->vdDefence)  goto END;

	//����̳� ������� �ƴϸ� false
/*	if(siClassInGuild != pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI) && 
		siClassInGuild != pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DHS) &&
		strcmp(szID,pVillage->szHNSID) != 0) goto END;
	if(pVillage->GetVillageAttackType()			!=			ON_VILLAGE_ATTACK_NONE)	goto END;
	if(pVillage->m_bAttackTypeDelay				==							  TRUE) goto END;
	if(pVillage->GetDefenceSoldier()			<=						  0)		goto END;
	if(pVillage->vdMaxDefence == pVillage->vdDefence						)		goto END;


	//���� ���� ������ 10����� ���� ������ �ø��� �ִ�.
	siDefenceSoldierToDefenceUp = (SI32)pVillage->GetDefenceSoldier() * 10;
*/

	siUnitCode = CSiegeUnitTable::GetInstance()->GetKindUnitCode(uiKind);
	if(siUnitCode < 0)
	{
		dwResponse = ON_RET_DEFENSE_NO;
		goto END;
	}

	if(siUnitCode != FIELD_BATTLE_SWORD) 
	{   //�˺��� �ƴϸ� �����Ҽ� ����...
		dwResponse = ON_RET_DEFENSE_NO;
		goto END;
	}

	mnMoney = 1000;
	if(EnoughMoney(mnMoney) ==	FALSE)			
	{	//���� üũ
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
	
	//������ ������ �ø���.
	pVillage->IncreaseVillageDefence(vdDefenceUp);
	
	//�������� ���δ�.
	DecreaseMoney(mnMoney);

/*
	if(vdDefenceUp < siDefenceSoldierToDefenceUp)
	{
		//���� ������ �ø��� ���� ���� ���Ѵ�.
		mnMoney = (MONEY)vdDefenceUp;
		//���� ������� �˾ƿ´�.
		if(EnoughMoney(mnMoney)	==	FALSE)			
		{	
			dwResponse = ON_RET_DEFENSE_NOT_MONEY;
			goto END;
		}
		//���� ������ �ø���.
		pVillage->IncreaseVillageDefence(vdDefenceUp);
		
		//�������� ���δ�.
		DecreaseMoney(mnMoney);
		
	}
	else // vdDefenceUp >= siDefenceSoldierToDefenceUp
	{
		//�ø� ���� ������ ���´�.
		vdDefenceUp = (VDEF)siDefenceSoldierToDefenceUp;
		//���� ������ �ø��� ���� ���� ���Ѵ�.
		mnMoney = (MONEY)vdDefenceUp;
		//���� ������� �˾ƿ´�.
		if(EnoughMoney(mnMoney)	==	FALSE)		
		{
			dwResponse = ON_RET_DEFENSE_NOT_MONEY;
			goto END;
		}
		//���� ������ �ø���.
		pVillage->IncreaseVillageDefence(vdDefenceUp);
		//�������� ���δ�.
		DecreaseMoney(mnMoney);
	}
*/
	//���� �������¸� ���� ������� �����.
	

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



//��������������������������������������������������������������������������������������������
//ON_REQUEST_INCREASE_GUILD_UNIT
//��������������������������������������������������������������������������������������������
//����� �������� ���Ѵ�.
//���� �ȿ����� ����.�κ��丮�� �ƴ� ����� cltGuild::cltGuildUnit Ŭ������ �����Ѵ�.
//������ �α���.����� ����ǰ ���� ����ؼ� �����Ҽ� �ִ�.
//��������������������������������������������������������������������������������������������
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
	
	
	//������ ���̵� ���´�.
//	uiItemID = pMyWorld->m_FieldBattleParser.GetItemID(pOnReqIncreaseGuildUnit->siKind);
//	pItemHeader = pMyWorld->clItem.GetItem(uiItemID);

	//���� * 1�δ��� ���޷� 
//	siSupply = pItemHeader->m_pImperium->m_siSupply * pOnReqIncreaseGuildUnit->siCount;
	//1�δ� * pItemHeader->m_pImperium->m_siPrice * pItemHeader->m_pImperium->m_siPopulation
//	mnMoney =  (MONEY)(pItemHeader->m_pImperium->m_siPrice * pItemHeader->m_pImperium->m_siPopulation * pOnReqIncreaseGuildUnit->siCount);


//	if(pclGuild->GetGuildSupply() < siSupply)
//	{
//		dwResponse = ON_RET_NOT_ENOUGH_SUPPLY;
//		goto END;
//	}

	//������ �δ��� ���� ����Ѵ�.
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
	
	//�Ϸ翡 �����Ҽ� �ִ� �ִ� ���귮�� �Ѿ��� üũ.	
	if(!pVillage->CheckDecreasePopulation((UI32)nTempPopu * pOnReqIncreaseGuildUnit->siCount))
	{
		dwResponse = ON_RET_NOT_ENOUGH_POPULATION;
		goto END;
	}

	//������ �ּ� �α����� �����Ѵ�.
//	if(pVillage->LimitDecreasePopulation((UI32)(pItemHeader->m_pImperium->m_siPopulation * pOnReqIncreaseGuildUnit->siCount)) == FALSE)
	if(!pVillage->LimitDecreasePopulation((UI32)nTempPopu * pOnReqIncreaseGuildUnit->siCount))
	{
		dwResponse = ON_RET_NOT_LIMIT_POPULATION;
		goto END;
	}

	//����� ����ǰ ����.
//	pclGuild->DecreaseGuildSupply((SI32)siSupply);
	
	//������ ���� ���� ����.
	DecreaseMoney(mnMoney, FALSE);
	
	//������ ���� ������ ���� ��Ų��.
	siTotalGuildUnitCount = pclGuild->m_GuildUnit.IncreaseGuildUnit(siUnitCode, pOnReqIncreaseGuildUnit->siCount);

	//������ �α����� ���ҽ�Ų��.
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
	//������ �α����� ���̰� ���� ���̰� ����ǰ�� ���δ�.
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



//��������������������������������������������������������������������������������������������
//ON_REQUEST_DECREASE_GUILD_UNIT
//��������������������������������������������������������������������������������������������
//����� ����� ����� �������� 
//cltGuild::cltGuildUnitŬ������ �����͸� �κ��丮�� ���������� ��ȯ�Ѵ�. (����)
//��������������������������������������������������������������������������������������������
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

	//�������� ������.
	UI16 uiItemID = pMyWorld->m_FieldBattleParser.GetItemID(pOnReqDecreaseGuildUnit->siKind);
	pItemHeader = pMyWorld->clItem.GetItem(uiItemID);

	//�������� ���� ������ ��ġ�� �˻����� �κ��丮�� ��������ش�.
	for(SI16 i = 0 ;i < ON_MAX_FOLLOWER_NUM ; i++)
	{
		if(IFollower.IsValidFollower(i) == TRUE)
		{
			 pAddItemData = IFollower.GetItemByID(i, uiItemID); 

			 if(pAddItemData)	//�ش� �������� ������ �ִ�.
			 {
                //AddItemNoQuery ����.
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

					//�������� ���� �������� �Ѱ���.
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
			 else	//������ ���� �ʴ�.
			 {
				 //
				 siTempItemSlot = IFollower.FindEmptyItemBuffer(i);
				 if((siTempItemSlot != -1) && (siTempItemSlot != 0xffff))
				 {
					//��ĭ�� ã�Ƽ� ������ �ִٸ� �߰�.
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

						//�������� ���� �������� �Ѱ���.
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



//��������������������������������������������������������������������������������������������
//ON_REQUEST_LIST_GUILD_UNIT
//��������������������������������������������������������������������������������������������
//����� ���������� ����Ʈ�� ���´�. 
//��ο� ��� �������̽����� ���. 
//cltGuild::cltGuildUnit�� ������ ���´�.
//��������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvGetGuildUnitList()
{
	cltGuild*				    pclGuild;
	OnReqGetGuildUnitList*		pOnReqGetGuildUnitList;
	UI32						uiTempPopulation = 0;

	pOnReqGetGuildUnitList		=	(OnReqGetGuildUnitList*)cMsg;

	if((pclGuild =	pMyWorld->clIGuild.GetGuildByID(uiGuildID)) == NULL )	return;

	//���� �����õ� ����ϱ� ������ �����Ҽ� �ִ� ������ �α����� �����ش�.
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
//��������������������������������������������������������������������������������������������
//ON_REQUEST_INCREASE_GUILD_SUPPLY
//��������������������������������������������������������������������������������������������
//����� ������ �ִ� ����ǰ�� ���� ��Ų��.
//cltGuild::m_siGuildSupply�� �����Ѵ�.
//��������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvIncreaseGuildSupply()
{
	cltGuild*					 pclGuild;
	MONEY						 uiMoney = 0;
	DWORD						 dwResponse = ON_RET_OK;
	SI32						 siSupplyPrice = 125;		//����ǰ�� ����.

	OnReqIncreaseGuildSupply* pOnReqIncreaseGuildSupply = (OnReqIncreaseGuildSupply*)cMsg;

	if((pclGuild =	pMyWorld->clIGuild.GetGuildByID(uiGuildID)) == NULL )			dwResponse = ON_RET_NO;
	if(GetMoney() < (siSupplyPrice * pOnReqIncreaseGuildSupply->siSupplyQuantity))	dwResponse = ON_RET_NO;

	//Guild ����ǰ ������ ���� ��Ų��.
	pclGuild->IncreaseGuildSupply(pOnReqIncreaseGuildSupply->siSupplyQuantity);
	//�ش� ��ǰ�� ���� ���´�.
	uiMoney = siSupplyPrice * pOnReqIncreaseGuildSupply->siSupplyQuantity;
	DecreaseMoney(uiMoney,FALSE);

	//Guild����ǰ�� �� ���� �� �������� �� ���� ��� ����.
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



//��������������������������������������������������������������������������������������������
//ON_REQUEST_DECREASE_GUILD_SUPPLY
//��������������������������������������������������������������������������������������������
//���޼����� ����� ����ǰ�� ��������.
//���޼����� ������ Ŭ���ؼ� ����� ����ǰ�� ��������.
//���޼����� ����ǰ�� ���� ����� ����ǰ�� ����.�����߿��� �����ϴ�.
//cltGuild::m_siGuildSupply ����.
//SOPlayer::m_siSupplyCharge ����.
//��������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvDecreaseGuildSupply()
{
	cltGuild*					pclGuild;
	MONEY						uiMoney				 = 0;
	DWORD						dwResponse			 = ON_RET_OK;
	SOVillage*					pVillage			 =	NULL;
	SI32						siTempSupplyQuantity = 0;
	

	OnReqDecreaseGuildSupply* pOnReqDecreaseGuildSupply = (OnReqDecreaseGuildSupply*)cMsg;

	pVillage = pMyWorld->GetVillageByCode(pOnReqDecreaseGuildSupply->siVillageCode);

	//������ ���̵�� �ڽ��� �����̵� ���� �ؾ��Ѵ�.
	if(pVillage->uiGuildID != uiGuildID)											dwResponse = ON_RET_NO;
	if((pclGuild = pMyWorld->clIGuild.GetGuildByID(uiGuildID)) == NULL )			dwResponse = ON_RET_NO;
	if(pclGuild->GetGuildSupply() == 0)												dwResponse = ON_RET_NO;
	if(pMyWorld->clIGuild.GetGuildWarCheck(uiGuildID) == FALSE)						dwResponse = ON_RET_NO;


	if(dwResponse == ON_RET_OK)
	{
		siTempSupplyQuantity = pOnReqDecreaseGuildSupply->siSupplyQuantity;
		
		if(pclGuild->GetGuildSupply() > siTempSupplyQuantity) 
		{
			//Guild ����ǰ ������ ���� ��Ų��.
			pclGuild->DecreaseGuildSupply(siTempSupplyQuantity);
			//���޼������Ը� �ѱ�� �ִ� ������ ���ް�����.
			SetFieldSupplyCharge(siTempSupplyQuantity);
		}
		else
		{
			siTempSupplyQuantity = pclGuild->GetGuildSupply();

			pclGuild->DecreaseGuildSupply(siTempSupplyQuantity);
			SetFieldSupplyCharge(siTempSupplyQuantity);
		}

	}

	//���� Guild����ǰ �� �������� �� ����ǰ ��� ����.
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



//��������������������������������������������������������������������������������������������
//ON_REQUEST_GET_SUPPLY_AT_GUILD_UNIT
//��������������������������������������������������������������������������������������������
//���������� ���޼����κ��� ����ǰ�� ���´�.
//���������� ���޼����� Ŭ�� ���޼����� ���� �ϰ� �ִ� ����ǰ�� ���������� ��������.
//���������� ������ ������ �ʱⰪ���� �������� ����ϰ�
//Ŭ���̾�Ʈ�� ������ ����ǰ�� ������ ������� Ŭ���̾�Ʈ���� ó���Ѵ�.
//���޼���(pTargetPlayer)�� SOPlayer::m_siSupplyCharge ����.
//��������������������������������������������������������������������������������������������
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

		//���� ����� �Ҽ��� Ȯ���ϸ�.
		if(uiGuildID == pTargetPlayer->uiGuildID)
		{
			//���� ������ �´°��� Ȯ���Ѵ�.
			if(pTargetPlayer->IsFieldAttackUnit() == FALSE)								return;
			if(IsFieldAttackUnit()				  == FALSE)								return;
			//������ ������ ���޼����� �´°��� Ȯ���Ѵ�.
			if(pMyWorld->m_FieldBattleParser.GetUnitKind(pTargetPlayer->uiKind) != FIELD_BATTLE_SUPPLY)	return;
			//�ڽ��� ���޼����̸鼭 ���޼������� ���� �Ҽ� ����.
			if(pMyWorld->m_FieldBattleParser.GetUnitKind(uiKind)		 == FIELD_BATTLE_SUPPLY)	return;
			//���޼����� ������ �ִ� ���ް������� �����Ǿ� �ִ����� Ȯ���Ѵ�.
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
				LPVOID(siResSupplyAmount),						// Player�� ������ Supply��.
				LPVOID(pOnReqGetSupplyAtGuildUnit->uiTargetID), // TargetPlayerID
				LPVOID(pTargetPlayer->GetFieldSupplyCharge()));	// TargetPlayer�� ������ �ִ� Supply��.
		}
	}
}



//��������������������������������������������������������������������������������������������
//ON_REQUEST_LIST_GUILD_SUPPLY
//��������������������������������������������������������������������������������������������
//����� ������ �ִ� ����ǰ�� ������ ���´�.
//������ ���������������̽����� ȣ�������� ����ǰ�� ��ܿ� ����ǹǷ� 
//cltGuild::m_siGuildSupply�� ���´�.
//��������������������������������������������������������������������������������������������
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



//��������������������������������������������������������������������������������������������
//ON_REQUEST_INCREASE_VILLAGE_DEFENCE_SOLDIER
//��������������������������������������������������������������������������������������������
//������ ���� ���� ���� ��Ų��.
//���� ���� �������̽����� �������ְ� ������ �Բ� ����������
//����������		cltGuild::cltGuildUnit ��
//���� ���񺴼���	SOVillage::m_siDefenceSoldier �� ����ȴ�.
//���� ������ ������ų�� ����� �������� ���ҽ�Ű�� ������ �α����� ����.
//���� ������ �� �������� �����Ѵ�.
//��������������������������������������������������������������������������������������������
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
	
	//Guild ����ǰ�� �� ������ ����  �������� �A���� ��� ����.
	if(pVillage)
	{
		UI16 uiItemID = pMyWorld->m_FieldBattleParser.GetItemID(FIELD_BATTLE_DEFENSE);
		pItemHeader = pMyWorld->clItem.GetItem(uiItemID);


		//���� ���񺴼��� ���� ��Ų��.
		//���� * ���� Hp
		siDefenceSoldier = (SI32)(pOnReqVillageDefenceSoldier->siDefenceSoldierCount*pItemHeader->m_pImperium->m_siHp);
		
		//����� ����ǰ���� ���� ��Ų��.
		//���� * 1�δ�� ���޷�.
		siSupply = (SI32)(pItemHeader->m_pImperium->m_siSupply*pOnReqVillageDefenceSoldier->siDefenceSoldierCount);

		if(pclGuild->GetGuildSupply() < siSupply)
		{
			dwResponse = ON_RET_NOT_ENOUGH_SUPPLY;
			goto END;
		}

		mnMoney = (MONEY)(pItemHeader->m_pImperium->m_siPrice * pItemHeader->m_pImperium->m_siPopulation * pOnReqVillageDefenceSoldier->siDefenceSoldierCount);

		//���� ���ҽ�ĭ��.
		if(EnoughMoney(mnMoney) == FALSE)
		{
			dwResponse = ON_RET_NOT_ENOUGH_MONEY;
			goto END;
		}
		//�Ϸ翡 �����Ҽ� �ִ� �ִ� ���귮�� �Ѿ��� üũ.
		if(pVillage->CheckDecreasePopulation((UI32)(pOnReqVillageDefenceSoldier->siDefenceSoldierCount * pItemHeader->m_pImperium->m_siPopulation)) == FALSE)
		{
			dwResponse = ON_RET_NOT_ENOUGH_POPULATION;
			goto END;
		}
		//������ �ּ� �α����� �����Ѵ�.
		if(pVillage->LimitDecreasePopulation((UI32)(pOnReqVillageDefenceSoldier->siDefenceSoldierCount * pItemHeader->m_pImperium->m_siPopulation)) == FALSE)
		{
			dwResponse = ON_RET_NOT_LIMIT_POPULATION;
			goto END;
		}

		//��� ����ǰ���� ���δ�.
		pclGuild->DecreaseGuildSupply(siSupply);
		//����� ������ �ִ� ���� ���δ�.
		DecreaseMoney(mnMoney,FALSE);
		//���� �α��� ���ҽ�Ų��.
		pVillage->DecreasePopulation((UI32)(pOnReqVillageDefenceSoldier->siDefenceSoldierCount * pItemHeader->m_pImperium->m_siPopulation));
		//���� ���� ���� ���� ��Ų��.
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



//��������������������������������������������������������������������������������������������
//ON_REQUEST_GET_VILLAGE_DEFENCE_SOLDIER
//��������������������������������������������������������������������������������������������
//���� ���� ���� ���´�.
//SOVillage::m_siDefenceSoldier
//��������������������������������������������������������������������������������������������
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

//�÷��̾�� ���� ���ش�.
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

		//���� ����� �Ҽ��� Ȯ���ϸ�.
		if(uiGuildID == pTargetPlayer->uiGuildID)
		{
			//���� ������ �´°��� Ȯ���Ѵ�.
			if(pTargetPlayer->IsFieldAttackUnit() == FALSE)	return;
			if(IsFieldAttackUnit()				  == FALSE)	return;
			
			if(FieldAttackDead() || pTargetPlayer->FieldAttackDead()) return;

			//�ڽ��� ���޼����̸鼭 ���޼������� ���� �Ҽ� ����.
//			if(pMyWorld->m_FieldBattleParser.GetUnitKind(pTargetPlayer->uiKind) == FIELD_BATTLE_SUPPLY)	return;
//			if(CSiegeUnitTable::GetInstance()->GetKindUnitCode(pTargetPlayer->uiKind) == FIELD_BATTLE_SUPPLY) return;
			//���޼����� ������ �ִ� ���ް������� �����Ǿ� �ִ����� Ȯ���Ѵ�.
			int nMp = GetFieldMP();			
			if(nMp <= 0 ) return;

			int nHealAmount = pTargetPlayer->GetFieldMaxHP() - pTargetPlayer->GetFieldHp();
		
//			if(nTargetHP < 0 || nTargetHP >= pTargetPlayer->GetFieldMaxHP()) return; 

			if(nHealAmount <= 0) return;
			
			//�����پ��� ����Ѵ�.
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
				         LPVOID(siHealAmount),						    // Player�� �����ؾ�
				         LPVOID(GetFieldMP()),                           // ���� ������ 
				         LPVOID(pOnReqGetSupplyAtGuildUnit->uiTargetID), // TargetPlayerID
				         LPVOID(pTargetPlayer->GetFieldHp()));           // TargetPlayer�� ���� hp

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
//��������������������������������������������������������������������������������������������
//ON_REQUEST_GUILD_SET_RELATION
//��������������������������������������������������������������������������������������������
//��ܰ��� ���� ���踦 �����Ѵ�.
//cltGuild::m_siGuildRelation,cltGuild::m_siGuildRelationEnemyTime �� �����Ѵ�.
//��ܰ��� ���踦 ���� ����.���� ���� ���� ������ ���ʴ� ���� �����̴�.
//��������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvGuildSetRelation()
{
	OnReqGuildSetRelationMsg	*pGuildSetRelationMsg;
	BOOL						bSuccess = FALSE;
	SI32						siRelation;
	BOOL						bRelation = FALSE;
	
	// ������� üũ�Ѵ�.
	if(uiGuildID == 0)	goto	END;

	if(siClassInGuild != pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))	goto	END;

	pGuildSetRelationMsg	=	(OnReqGuildSetRelationMsg*)cMsg;

	siRelation				=	pMyWorld->clIGuild.ResponseDefineToRelationDefine(pGuildSetRelationMsg->dwResponse);
	
	//���� ��ܰ��� ���븦 �����Ѵ�.
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



//��������������������������������������������������������������������������������������������
//ON_REQUEST_GUILD_FRIELDLY_MESSAGE_OFFER
//��������������������������������������������������������������������������������������������
//ȭģ���� �޼����� ������.
//ȭģ���� �޼����� ������ ����� �޼����� 
//ȭģ���� �޴� ����� cltGuild::m_szGuildfriendlyMessage�� ����ȴ�.
//��������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvFriendlyMessageOffer()
{	
	OnReqGuildFriendlyMessageOffer*	TempOnReqGuildFriendlyMessageOffer;
	DWORD							dwResponse;

	TempOnReqGuildFriendlyMessageOffer		=	( OnReqGuildFriendlyMessageOffer* )cMsg;

	//�����ϴ� ������ üũ�Ѵ�.
/*	SYSTEMTIME  sysTime;
	GetSystemTime(&sysTime);
	if(sysTime.wDayOfWeek == 2|| sysTime.wDayOfWeek == 4)
	{
		dwResponse = ON_RET_NO;
		SendFieldMsg(ON_RESPONSE_GUILD_FRIELDLY_MESSAGE_OFFER, SO_SFM_ONLYME, LPVOID(dwResponse));
		return;
	}
*/
	//�����ϴ� ���Ͽ� �ð��̸� ȭģ���Ǹ� �Ҽ�����.
	if(pMyWorld->m_WarTimeParser.IsWarCheck(pMyWorld->pRealTime->tm_wday, pMyWorld->pRealTime->tm_hour)) return;
//	if(pMyWorld->m_WarTimeParser.IsWarTime(pMyWorld->pRealTime->tm_hour)) return;
	
	if(pMyWorld->clIGuild.SetFriendlyMessage(uiGuildID,TempOnReqGuildFriendlyMessageOffer->szGuildID,TempOnReqGuildFriendlyMessageOffer->szMessage) == FALSE)
		dwResponse = ON_RET_NO;
	else 
		dwResponse = ON_RET_OK;	
	
	SendFieldMsg(ON_RESPONSE_GUILD_FRIELDLY_MESSAGE_OFFER, SO_SFM_ONLYME, LPVOID(dwResponse));
}



//��������������������������������������������������������������������������������������������
//ON_REQUEST_GUILD_FRIENDLY_MESSAGE_ACCEPT
//��������������������������������������������������������������������������������������������
//ȭģ���Ǹ� ���� ��ܿ��� ȭģ���Ǹ� ����Ѵ�.
//��ܰ��� ���� �ʱ�ȭ �ȴ�.
//��������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvFriendlyMessageAccept()
{
	OnReqGuildFriendlyMessageAccept*	TempOnReqGuildFriendlyMessageAccept;

	TempOnReqGuildFriendlyMessageAccept		=	( OnReqGuildFriendlyMessageAccept* )cMsg;

	pMyWorld->clIGuild.SetFriendlyMessageAccept(uiGuildID,TempOnReqGuildFriendlyMessageAccept->szGuildID);

	SendFieldMsg(ON_RESPONSE_GUILD_FRIENDLY_MESSAGE_ACCEPT, SO_SFM_ONLYME);
} 



//��������������������������������������������������������������������������������������������
//ON_REQUEST_GUILD_FRIENDLY_MESSAGE_CANCEL
//��������������������������������������������������������������������������������������������
//ȭģ���Ǹ� ���� ��ܿ��� ȭģ���Ǹ� �����Ѵ�.
//ȭģ���Ǹ� ���� ����� msg�� �����ȴ�.
//���� ����� �״�� ���� �ִ�.
//��������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvFreidnlyMessageCancel()
{
	OnReqGuildFreindlyMessageCancel*	TempOnReqGuildFreindlyMessageCancel;

	TempOnReqGuildFreindlyMessageCancel		=	( OnReqGuildFreindlyMessageCancel* )cMsg;

	pMyWorld->clIGuild.SetFriendlyMessageCancel(uiGuildID,TempOnReqGuildFreindlyMessageCancel->szGuildID);

	SendFieldMsg(ON_RESPONSE_GUILD_FRIENDLY_MESSAGE_CANCEL, SO_SFM_ONLYME);
}



//��������������������������������������������������������������������������������������������
//ON_REQUEST_GUILD_VILLAGEATTACK_RESULT
//��������������������������������������������������������������������������������������������
//������ ������ ����� ���� ��� ���� �Ҽ� �ִ� ������ ���� ������ ��û�Ѵ�.
//cltGuild::m_stGuildResult ����Ǿ� �ִ� ��.
//�� ���� ���� �ı��� �ش� ��ܿ� ����ȴ�.
//��������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvVillageAttackResult()
{
	OnReqVillageAttackResult*			pOnReqVillageAttackResult;
	cltGuild*							pclGuild;
	DWORD								dwResponse = ON_RET_OK;
	SI16								siVillageCount = 0;

	pOnReqVillageAttackResult		=	(OnReqVillageAttackResult*)cMsg;

	//�ڱ��� GuildID	= uiGuildID
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

//��������������������������������������������������������������������������������������������
//ON_REQUEST_GUILD_CHOICE_PLAYER
//��������������������������������������������������������������������������������������������
//����� ���� ���� ������ �ش� ����� �÷��̾�� �Ѱ��ش�.
//cltGuild::m_stGuildResult�� �ش��ϴ� �÷��̾ ��������ش�.
//��������������������������������������������������������������������������������������������
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
		//��濡�� �˸���.
		SendFieldMsg(ON_RESPONSE_GUILD_CHOICE_PLAYER,SO_SFM_ONLYME,LPVOID(dwResponse),
			LPVOID(pOnReqGuildChoicePlayer->siVillageCode),LPVOID(pOnReqGuildChoicePlayer->szUserName));
		//���ڱ��� �޴� �÷��̾�� �˸���.
		pOpponentPlayer->SendFieldMsg(ON_RESPONSE_GUILD_CHOICE_PLAYER,SO_SFM_ONLYME,LPVOID(dwResponse),
			LPVOID(pOnReqGuildChoicePlayer->siVillageCode),LPVOID(pOnReqGuildChoicePlayer->szUserName));
		break;
	case ON_RET_NO:
		//���н� ��濡�Ը� �˸���.
		SendFieldMsg(ON_RESPONSE_GUILD_CHOICE_PLAYER,SO_SFM_ONLYME,LPVOID(dwResponse),LPVOID(pOnReqGuildChoicePlayer->siVillageCode)
			,LPVOID(pOnReqGuildChoicePlayer->szUserName));
		break;
	}	
}

//��������������������������������������������������������������������������������������������
//ON_REQUEST_VILLAGE_INVESTMENT_POWER
//��������������������������������������������������������������������������������������������
//�÷��̾ ���ڸ� �� �� ���������� �ִ� �����ΰ� ������ �����.
//��������������������������������������������������������������������������������������������
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

	//��� ���ڱ����� �ִ��� �˻�.
	for(SI16 i = 0; i < ON_MAX_GUILD_RESULT; i++)
	{
		if(pclGuild->m_stGuildResult[i].GetVillageCode() == pOnReqInvestmentPower->siVillageCode)
		{
			if(strcmp(pclGuild->m_stGuildResult[i].GetCharName(),pOnReqInvestmentPower->szUserName) == 0)
			{
				dwResponse = ON_RET_OK;
				siPercent  = pclGuild->m_stGuildResult[i].GetPercent();
				//���� ���ڽ� ���� ������ Ǯ���ִ� �÷��׸� On���ش�.
				m_bInvestmentPower = TRUE;
			}
		}
	}

	SendFieldMsg(ON_RESPONSE_VILLAGE_INVESTMENT_POWER,SO_SFM_ONLYME,LPVOID(dwResponse),LPVOID(siPercent));
}

//��������������������������������������������������������������������������������������������
// ON_REQUEST_WAR_WARP
//��������������������������������������������������������������������������������������������
// ���������� �Ϲ� ������ ���� ��Ű�� �޼���.
// Response�� ���� Warp�� �̿��Ѵ�.
// ON_RESPONSE_WAR_WARP�� ������� �ʴ´�.
//��������������������������������������������������������������������������������������������
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


//��������������������������������������������������������������������������������������������
// ON_REQUEST_NORMAL_CHAR
//��������������������������������������������������������������������������������������������
// ���� ������ ���¿��� �Ϲ��������� �����Ѵ�.
// �ڱ� �ڽ��� �Ϲ��������� �����ϴ� �޼���.
//��������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvNormalChar()
{
	OnReqNormalChar*		pOnReqNormalChar;
	OnlineFollower*			pFollower = IFollower.GetFollower(0);

	pOnReqNormalChar	=	(OnReqNormalChar*)cMsg;	

	ChangeNormalChar();
}

//������ �������� �����Ѵ�.
VOID SOPlayer::RecvChangeUnit()
{
	OnReqChangeUnit        *pReqChangeUnit;
	pReqChangeUnit = (OnReqChangeUnit*)cMsg;

	SI16 siRes = ChangeUnit(pReqChangeUnit->siChangeKind, siClassInGuild);
	if(siRes != ON_CHANGE_OK)
		SendFieldMsg(ON_RESPONSE_CHANGE_WAR, SO_SFM_ONLYME, LPVOID(0), LPVOID(0), 
		             LPVOID(0), LPVOID(0), LPVOID(0),LPVOID(siRes));
	
}

//��������������������������������������������������������������������������������������������
// ���� ���� �Ϲ� �Լ��� 
//��������������������������������������������������������������������������������������������
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
//���޼����� ��� �������� �����´�
SI32 SOPlayer::GetFieldMP()
{
	return m_siFieldMP;
}
//���޼����ΰ�� �������� �����Ѵ�.
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


// ����� ��ü �Ǿ��� �� ĳ���Ͱ� ������ �ִ� ���� ���þ������� �����Ѵ�.
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

//������������ ������ �Ѵ�.[2005/01/14] - �ֺ���
UI16 SOPlayer::ChangeUnit(UI16 uiCode, UI16 uiQuantity)
{
	if(uiCode < 0) return ON_CHANGE_FAILED;
	if( pMyWorld->IsAttackVillageTimeZone() == FALSE )  return ON_CHANGE_FAILED;
	
	if(m_bIsWarUnit) return ON_CHANGE_FAILED;

	SI16 siUnitCode = CSiegeUnitTable::GetInstance()->GetKindUnitCode(uiCode);
	if(siUnitCode < 0) return ON_CHANGE_FAILED;
	
	OnlineFollower *pFollower = IFollower.GetFollower(0);
	_ImportantParameta* pParam  = &pFollower->ParametaBox.IP;

	//��ܿ� ���ԵǾ� �ִ��� �˻��Ѵ�.
	cltGuild *pclGuild;
	pclGuild = pMyWorld->clIGuild.GetGuildByID(uiGuildID);
	if(pclGuild == NULL) return ON_CHANGE_NOT_GUILD;
    
	//������ �Ҽ� �ִ� ������� �˻��Ѵ�.
	if(pclGuild->IsWar() == FALSE) return ON_CHANGE_NOT_GUILD;
	
	//���������� �˻�....
	if(uiQuantity == pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ))
		return ON_CHANGE_NOT_GUILD;
	
	//������ 10���� ������ ������ ����
	UI16 uiLevel = pParam->GetLevel();
	if(uiLevel < 10) 
		return ON_CHANGE_ENOUGH_LEVEL;
	

	//�����Ҽ� �ִ� ������ �˾ƿ´�.
	int nChangeLevel = CSiegeUnitTable::GetInstance()->GetLevel(siUnitCode);
	//������ ������ ���� ����
	if(uiLevel < nChangeLevel)
		return ON_CHANGE_ENOUGH_LEVEL;

	
	//������ ������ ������ ���� �ִ��� �˻�.
	SI16 siUnitCount = pclGuild->m_GuildUnit.GetGuildUnit(siUnitCode);
	if(siUnitCount <= 0) return ON_CHANGE_ENOUGH_KIND;
	
	SI16 siNeedCount = 0;	
	
	//���� ���� ���δ�.
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

	//�����ϴµ� �ʿ��� ���
	int nChangeMoney = CSiegeUnitTable::GetInstance()->GetChangePrice(siUnitCode);
	nChangeMoney = nChangeMoney * siNeedCount;
	
	//���� �Ҽ� �ִ� ���� ������ ����
	if(GetMoney() < nChangeMoney) 
		return ON_CHANGE_ENOUGH_MONEY;
	

//-----------------------------------------------------------------
//  ĳ���͸� �������ִ� �κ�.....

	//���� ��� ��ŭ ���� ����.
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
	
	//�������� ���� �������� �Ѱ���.
	pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_DECREASE_GUILD_UNIT, 
		                               LPVOID(&DecreaseGuildUnit), 
									   sizeof(DBIKDecreaseGuildUnit));

	//������ �Ӽ��� �����Ѵ�.
	SetPrevKind(uiKind);
	uiKind = uiCode;
	pFollower->uiCharID = uiCode;
	
	//���� �������� �����Ѵ�.
	if(uiQuantity == pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI)||
	   uiQuantity == pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS))
		SetCaptainUnit(TRUE);
	else
		SetCaptainUnit(FALSE);

	//ü���� �����Ѵ�
	int BP = GetFightingPower();
	SI16 siHP = CSiegeUnitTable::GetInstance()->GetHP(siUnitCode, BP);
	
	//����� ��� 2���� ü�����ִ´�.
	if(uiQuantity == pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI)||
	   uiQuantity == pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS)) siHP *= 2; 
	
	SetFieldHp(siHP);
	SetFieldMaxHP(siHP);
	//���� �����ΰ�� �������� �����Ѵ�.
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
	ImperumUse.siMp		       = siMP;        // �Ϲ� ������ ���� �������� 0���� �Ѵ�.
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
		case 1: //��
			{
				for(i = 0; i < nCount; i++)
				{
					siEX = siSX - i;
					siEY = siEY + i;
					
					puiIDs[i] = pMyWorld->clISOMap.GetCharUniID(uiMapID, siEX, siEY, ON_MAP_CHARID_PLAYER);
				}
			}
			break;

		case 2: //�ϼ�
			{				
				for(i = 0; i < nCount; i++)
				{
					siEY = siEY;
					siEX = siSX - i;					
					
					puiIDs[i] = pMyWorld->clISOMap.GetCharUniID(uiMapID, siEX, siEY, ON_MAP_CHARID_PLAYER);
				}
			}
			break;

		case 3: //��
			{
				for(i = 0; i < nCount; i++)
				{
					siEX = siSX - i;
					siEY = siEY - i;
					
					puiIDs[i] = pMyWorld->clISOMap.GetCharUniID(uiMapID, siEX, siEY, ON_MAP_CHARID_PLAYER);
				}
			}
			break;

		case 4: //����
			{
				for(i = 0; i < 2; i++)
				{
					siEX = siSX;
					siEY = siEY + i;
					
					puiIDs[i] = pMyWorld->clISOMap.GetCharUniID(uiMapID, siEX, siEY, ON_MAP_CHARID_PLAYER);
				}
			}
			break;

		case 6: //�ϵ�
			{
				for(i = 0; i < nCount; i++)
				{
					siEX = siSX;
					siEY = siEY - i;
					
					puiIDs[i] = pMyWorld->clISOMap.GetCharUniID(uiMapID, siEX, siEY, ON_MAP_CHARID_PLAYER);
				}
			}
			break;

		case 7: //��
			{
				for(i = 0; i < nCount; i++)
				{
					siEX = siSX + i;
					siEY = siEY + i;
					
					puiIDs[i] = pMyWorld->clISOMap.GetCharUniID(uiMapID, siEX, siEY, ON_MAP_CHARID_PLAYER);
				}
			}
			break;

		case 8: //����
			{
				for(i = 0; i < nCount; i++)
				{
					siEX = siSX + i;
					siEY = siEY;
					
					puiIDs[i] = pMyWorld->clISOMap.GetCharUniID(uiMapID, siEX, siEY, ON_MAP_CHARID_PLAYER);
				}
			}
			break;

		case 9: //��
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

	if(m_bPlayerVillageInLimit) //���� �������̽����� �����ɷ� ó��
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

	//�̸� ������ ���� �ڽ��� ���� ĳ������ kind�� �����Ѵ�.
	uiKind				= m_uiPrevKind;
	pFollower->uiCharID = m_uiPrevKind;


	//���� ���� �����͵��� �ʱ�ȭ.
	m_bFieldAlive = TRUE;
	m_dwDeadTime  = pMyWorld->dwtimeGetTime;
	m_dwDeadSec	  = 1000 * 30;

	SetFieldHp(0);
	SetPrevKind(GetPrevKind());
	SetCaptainUnit(FALSE);
	SetFieldMP(0);
	SetFieldMaxHP(0);
	SetFieldMaxMP(0);

	if(m_bPlayerVillageInLimit) //���� �������̽����� �����ɷ� ó��
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
	
	//���� ���������� �������� ���..
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

		if(m_bPlayerVillageInLimit) //���� �������̽����� �����ɷ� ó��
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
	SI32  siBestIndex = pVillage->m_Government->GetOldBestInvestIndex(); //������ �ε����� ���´�.

	if(pOnGiveUp->uiResponse == ON_GUILD_GIVEUP_OK)
	{   //��� ���踦 ����� �ٲ��ش�.
		if(pMyWorld->clIGuild.GiveUp(uiGuildID))
		{   //������ �ε����� ���ͼ� ���ڱ��� 10%�� ��´�....			
			mnMoney = pVillage->m_Government->DelInvestMoney(siBestIndex, 1, TRUE);

			//��ܿ��鿡�� ����� ������ �����ߴٰ� �˷��ش�.
			pclGuild->SendGiveUpMsg();

			//��ü ������ ���ش�...
			pMyWorld->OfficialAnnouncement(ON_RET_ON_GUILD_GIVEUP, LPARAM(pclGuild->GetGuildName()), LPARAM(0));

		}
	}
	else
	{
		pOnGiveUp->uiVillageCode = 0;
		pOnGiveUp->uiResponse = ON_GUILD_GIVEUP_NORMAL;
	}

	pclGuild->SetGiveUpFlag(pOnGiveUp->uiResponse);
	
	//��� ������ �����Ѵ�.
	DBIKWarGiveUp           warGiveUp;
	memset(&warGiveUp, 0, sizeof(DBIKWarGiveUp));

	warGiveUp.uiGuildID     = uiGuildID;
	warGiveUp.uiVillageCode = pOnGiveUp->uiVillageCode;
	warGiveUp.mnMoney       = mnMoney;
	warGiveUp.siFlag        = pOnGiveUp->uiResponse;
	pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_SET_WARGIVEUP, &warGiveUp, sizeof(DBIKWarGiveUp));
	
}