#include "SOMain.h"
#include "SODefine.h"
#include "SOWorld.h"
#include "SOVillage.h"

#include "SODatabase.h"
#include "SOVillage_HospitalStructure.h"


#define DISCOUNT_HOSPITAL_MY			90
#define DISCOUNT_HOSPITAL_ENEMY			100


SOVillage_Hospital::SOVillage_Hospital()
{
	m_uiVillageCode = 0;

	m_lpVillage = NULL;
}

SOVillage_Hospital::~SOVillage_Hospital()
{
	Free();
}

BOOL SOVillage_Hospital::Init(UI16 uiVillageCode, DWORD dwMaxPlayer, SOVillage* lpVillage)
{
	m_uiVillageCode = uiVillageCode;
	m_lpVillage   = lpVillage;

	SOVillage_BaseStructure::Init(uiVillageCode, dwMaxPlayer);

	return TRUE;
}

VOID SOVillage_Hospital::Free()
{
	m_uiVillageCode = 0;

	m_lpVillage = NULL;

	SOVillage_BaseStructure::Free();	
}

BOOL SOVillage_Hospital::HospitalGetCureList(SOPlayer *pPlayer)
{
	SI32					TempCount;
	SI32					TempUnitCount;
	UI08					Slot[ON_MAX_FOLLOWER_NUM];
	MONEY					Money[ON_MAX_FOLLOWER_NUM];
	MONEY					RevivalMoney[ON_MAX_FOLLOWER_NUM];
	SI32					TempMoney;
	SI32					TempDiscount;

	TempDiscount = 0;
	if(m_lpVillage->uiGuildID != 0)
	{
		if((m_lpVillage->uiGuildID == pPlayer->uiGuildID) && (pPlayer->siClassInGuild != m_lpVillage->clWorld->clIGuildClass.GetJWJCode()))
		{
			TempDiscount += DISCOUNT_HOSPITAL_MY;
		}
		if(m_lpVillage->clWorld->clIGuild.IsHostilityRelation(m_lpVillage->uiGuildID, pPlayer->uiGuildID) == TRUE)
		{
			TempDiscount -= DISCOUNT_HOSPITAL_ENEMY;
		}
		if(m_lpVillage->clWorld->clIGuild.IsEnemyRelationWar(m_lpVillage->uiGuildID, pPlayer->uiGuildID) == TRUE)
		{
			TempDiscount -= DISCOUNT_HOSPITAL_ENEMY;
		}
	}

	// ����Ҽ� �ִ� �뺴�� ����� �����Ѵ�.
	TempUnitCount = 0;
	for(TempCount = 0; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
	{
		if(pPlayer->IFollower.IsValidFollower(TempCount))
		{
			TempMoney = HospitalGetCureMoney(pPlayer, TempCount, TempDiscount);
			if(TempMoney > 0)
			{
				Slot[TempUnitCount]         = TempCount;
				Money[TempUnitCount]        = TempMoney;
				RevivalMoney[TempUnitCount] = HospitalGetRevivalMoney(pPlayer, TempCount, TempDiscount);

				TempUnitCount++;
			}
		}
	}

	// Message����
	pPlayer->SendFieldMsg(ON_RESPONSE_HOSPITAL_GETCURELIST, SO_SFM_ONLYME, LPVOID(TempDiscount), LPVOID(TempUnitCount), LPVOID(Slot), LPVOID(Money), LPVOID(RevivalMoney));

	return TRUE;
}

BOOL SOVillage_Hospital::HospitalCureSolder(SOPlayer *pPlayer, UI08 Slot)
{
	MONEY					TempMoney;
	SI32					siLife;
	SI32					siMana;
	SI32					siStartLife;
	SI32					siStartMana;
	SI32					TempSI;
	__int64					iMoney;
	float					TempPercent;
	OnlineFollower*			lpOnlineFollower;
	SI32					TempDiscount;
	DBIKHealAtVillage		TempDBIKHealAtVillage;
	MONEY					TempVillageCapital;

	TempDiscount = 0;
	if(m_lpVillage->uiGuildID != 0)
	{
		if((m_lpVillage->uiGuildID == pPlayer->uiGuildID) && (pPlayer->siClassInGuild != m_lpVillage->clWorld->clIGuildClass.GetJWJCode()))
		{
			TempDiscount += DISCOUNT_HOSPITAL_MY;
		}
		if(m_lpVillage->clWorld->clIGuild.IsHostilityRelation(m_lpVillage->uiGuildID, pPlayer->uiGuildID) == TRUE)
		{
			TempDiscount -= DISCOUNT_HOSPITAL_ENEMY;
		}
		if(m_lpVillage->clWorld->clIGuild.IsEnemyRelationWar(m_lpVillage->uiGuildID, pPlayer->uiGuildID) == TRUE)
		{
			TempDiscount -= DISCOUNT_HOSPITAL_ENEMY;
		}
	}

	// ġ�ᰡ �ʿ��� �뺴���� �˻�
	TempVillageCapital = 0;
	if(pPlayer->IFollower.IsValidFollower(Slot))
	{
		lpOnlineFollower = pPlayer->IFollower.GetFollower(Slot);

		// ġ��
		if(pPlayer->siMoney > 0)
		{
			siStartLife = lpOnlineFollower->ParametaBox.IP.GetLife();
			siStartMana = lpOnlineFollower->ParametaBox.IP.GetMana();

			// ��Ȱ
			TempMoney = HospitalGetRevivalMoney(pPlayer, Slot, TempDiscount);
			if(TempMoney)
			{
				if(pPlayer->siMoney >= TempMoney)
				{
					// ���� ������ HP�� 1�� ��Ȱ
					pPlayer->DecreaseMoney(TempMoney);
					m_lpVillage->IncreaseCapital(MONEY(TempMoney*ON_CAPITAL_RATE_CURE),ON_PROFIT_CURE);
					lpOnlineFollower->ParametaBox.IP.SetLife(1);
					siLife = lpOnlineFollower->ParametaBox.IP.GetLife();
				}
				else
				{
					// ������ Error
					pPlayer->SendFieldMsg(ON_RESPONSE_HOSPITAL_CURESOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
					return TRUE;
				}
			}

			TempMoney = HospitalGetCureMoney(pPlayer, Slot, TempDiscount);
			if((pPlayer->siMoney > 0) && TempMoney)
			{
				// �� ���� ���� ġ�� Percent�� ����
				if(pPlayer->siMoney >= TempMoney)
				{
					TempPercent = 1.0f;
				}
				else
				{
					TempPercent = (float)((double)pPlayer->siMoney / (double)TempMoney);
					TempMoney   = pPlayer->siMoney;
				}

				// ġ��
				if(TempPercent == 1.0f)
				{
					lpOnlineFollower->ParametaBox.IP.SetLife(lpOnlineFollower->ParametaBox.GetMaxLife());
					lpOnlineFollower->ParametaBox.IP.SetMana(lpOnlineFollower->ParametaBox.GetMaxMana(), 9);
				}
				else
				{
					if(lpOnlineFollower->ParametaBox.GetMaxLife() > lpOnlineFollower->ParametaBox.IP.GetLife())
					{
						TempSI = lpOnlineFollower->ParametaBox.GetMaxLife() - lpOnlineFollower->ParametaBox.IP.GetLife();
						TempSI = (SI32)((float)TempSI * TempPercent);
						lpOnlineFollower->ParametaBox.IP.SetLife(lpOnlineFollower->ParametaBox.IP.GetLife() + TempSI);
					}

					if(lpOnlineFollower->ParametaBox.GetMaxMana() > lpOnlineFollower->ParametaBox.IP.GetMana())
					{
						TempSI = lpOnlineFollower->ParametaBox.GetMaxMana() - lpOnlineFollower->ParametaBox.IP.GetMana();
						TempSI = (SI32)((float)TempSI * TempPercent);
						lpOnlineFollower->ParametaBox.IP.SetMana(lpOnlineFollower->ParametaBox.IP.GetMana() + TempSI, 10);
					}
				}

				siLife = lpOnlineFollower->ParametaBox.IP.GetLife();
				siMana = lpOnlineFollower->ParametaBox.IP.GetMana();

				pPlayer->DecreaseMoney(TempMoney, FALSE);
				TempVillageCapital = MONEY(TempMoney*ON_CAPITAL_RATE_CURE);
				m_lpVillage->IncreaseCapital(TempVillageCapital,ON_PROFIT_CURE, FALSE);
				iMoney = pPlayer->siMoney;

				// DB ����
				TempDBIKHealAtVillage.DBAccount        = pPlayer->stAccountInfo.siDBAccount;
				TempDBIKHealAtVillage.uiSlot           = pPlayer->stAccountInfo.uiSlot;
				TempDBIKHealAtVillage.siMoney          = TempMoney;
				TempDBIKHealAtVillage.siVillageCapital = TempVillageCapital;
				TempDBIKHealAtVillage.uiVillageCode    = m_lpVillage->uiVillageCode;
				m_lpVillage->clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_HEALATVILLAGE, LPVOID(&TempDBIKHealAtVillage), sizeof(TempDBIKHealAtVillage));

				// Send Message
				pPlayer->SendFieldMsg(ON_RESPONSE_HOSPITAL_CURESOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(&iMoney), LPVOID(Slot), LPVOID(siLife), LPVOID(siMana));
			}
			else
			{
				// ���� ������ ���� ���¸� ����
				siLife = lpOnlineFollower->ParametaBox.IP.GetLife();
				siMana = lpOnlineFollower->ParametaBox.IP.GetMana();
				iMoney = pPlayer->siMoney;
				pPlayer->SendFieldMsg(ON_RESPONSE_HOSPITAL_CURESOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(&iMoney), LPVOID(Slot), LPVOID(siLife), LPVOID(siMana));
			}
		}
		else
		{
			pPlayer->SendFieldMsg(ON_RESPONSE_HOSPITAL_CURESOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{
		pPlayer->SendFieldMsg(ON_RESPONSE_HOSPITAL_CURESOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}

	return TRUE;
}

BOOL SOVillage_Hospital::HospitalCureAllSolder(SOPlayer *pPlayer)
{
	SI16						CureSolderCount;
	strCureSolder				CureSolder[ON_MAX_FOLLOWER_NUM];
	SI32						CureMoney[ON_MAX_FOLLOWER_NUM];
	SI32						TempCount;
	SI32						TempMoney;
	MONEY						TempMyMoney;
	float						TempPercent;
	OnlineFollower*				lpOnlineFollower;
	SI32						TempSI;
	SI32						TempDiscount;
	DBIKHealAtVillage			TempDBIKHealAtVillage;
	SI32						siStartLife;
	SI32						siStartMana;
	MONEY						TempVillageCapitalAdd;
	MONEY						TempVillageCapital;

	TempDiscount = 0;
	if(m_lpVillage->uiGuildID != 0)
	{
		if((m_lpVillage->uiGuildID == pPlayer->uiGuildID) && (pPlayer->siClassInGuild != m_lpVillage->clWorld->clIGuildClass.GetJWJCode()))
		{
			TempDiscount += DISCOUNT_HOSPITAL_MY;
		}
		if(m_lpVillage->clWorld->clIGuild.IsHostilityRelation(m_lpVillage->uiGuildID, pPlayer->uiGuildID) == TRUE)
		{
			TempDiscount -= DISCOUNT_HOSPITAL_ENEMY;
		}
		if(m_lpVillage->clWorld->clIGuild.IsEnemyRelationWar(m_lpVillage->uiGuildID, pPlayer->uiGuildID) == TRUE)
		{
			TempDiscount -= DISCOUNT_HOSPITAL_ENEMY;
		}
	}

	// ġ���Ҽ� �ִ� �뺴�� ����� �˻��Ѵ�.
	CureSolderCount = 0;
	TempMyMoney     = pPlayer->siMoney;
	for(TempCount = 0; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
	{
		if(pPlayer->IFollower.IsValidFollower(TempCount))
		{
			TempMoney = HospitalGetCureMoney(pPlayer, TempCount, TempDiscount);
			if(TempMoney > 0)
			{
				lpOnlineFollower             = pPlayer->IFollower.GetFollower(TempCount);

				CureSolder[CureSolderCount].dwCureSolderSlot = TempCount;
				CureSolder[CureSolderCount].siLife           = lpOnlineFollower->ParametaBox.IP.GetLife();
				CureSolder[CureSolderCount].siMana           = lpOnlineFollower->ParametaBox.IP.GetMana();
				CureMoney[CureSolderCount]                   = TempMoney;

				CureSolderCount++;
			}
		}
	}

	// �˻��� ����� ġ���Ѵ�.
	TempVillageCapitalAdd = 0;
	for(TempCount = 0; TempCount < CureSolderCount; TempCount++)
	{
		if(TempMyMoney > 0)
		{
			lpOnlineFollower             = pPlayer->IFollower.GetFollower(CureSolder[TempCount].dwCureSolderSlot);

			/*
			// ��Ȱ�� �ʿ����� �˻�
			TempMoney = HospitalGetRevivalMoney(pPlayer, CureSolder[TempCount].dwCureSolderSlot, TempDiscount);
			if(TempMoney)
			{
				if(TempMyMoney >= TempMoney)
				{
					// ���� ������ HP�� 1�� ��Ȱ
					TempMyMoney -= TempMoney;
					lpOnlineFollower->ParametaBox.IP.SetLife(1);
					CureSolder[TempCount].siLife = lpOnlineFollower->ParametaBox.IP.GetLife();

					// DB�� �����Ѵ�.
					SetParam.DBAccount		=	pPlayer->stAccountInfo.siDBAccount;
					SetParam.uiSlot			=	pPlayer->stAccountInfo.uiSlot;
					SetParam.uiFollowerID	=	CureSolder[TempCount].dwCureSolderSlot;
					SetParam.Option			=	SO_DB_SETPARAM_LIFE;
					SetParam.siValue		=	CureSolder[TempCount].siLife;
					clWorld->clDatabase->SetDBInfo(SO_DB_INFOKIND_SET_PARAM, LPVOID(&SetParam), NULL);
				}
			}
			*/

			// ��� �׾������� ���
			//if(lpOnlineFollower->ParametaBox.IP.GetLife() != 0)
			{
				// ��������� ġ��
				TempMoney = HospitalGetCureMoney(pPlayer, CureSolder[TempCount].dwCureSolderSlot, TempDiscount);
				if((TempMyMoney > 0) && TempMoney)
				{
					siStartLife = lpOnlineFollower->ParametaBox.IP.GetLife();
					siStartMana = lpOnlineFollower->ParametaBox.IP.GetMana();

					// �� ���� ���� ġ�� Percent�� ����
					if(TempMyMoney >= TempMoney)
					{
						TempPercent = 1.0f;
					}
					else
					{
						TempPercent = (float)((double)TempMyMoney / (double)TempMoney);
						TempMoney   = (SI32)TempMyMoney;
					}

					// ġ��
					if(TempPercent == 1.0f)
					{
						lpOnlineFollower->ParametaBox.IP.SetLife(lpOnlineFollower->ParametaBox.GetMaxLife());
						lpOnlineFollower->ParametaBox.IP.SetMana(lpOnlineFollower->ParametaBox.GetMaxMana(), 11);
					}
					else
					{
						if(lpOnlineFollower->ParametaBox.GetMaxLife() > lpOnlineFollower->ParametaBox.IP.GetLife())
						{
							TempSI = lpOnlineFollower->ParametaBox.GetMaxLife() - lpOnlineFollower->ParametaBox.IP.GetLife();
							TempSI = (SI32)((float)TempSI * TempPercent);
							lpOnlineFollower->ParametaBox.IP.SetLife(lpOnlineFollower->ParametaBox.IP.GetLife() + TempSI);
						}

						if(lpOnlineFollower->ParametaBox.GetMaxMana() > lpOnlineFollower->ParametaBox.IP.GetMana())
						{
							TempSI = lpOnlineFollower->ParametaBox.GetMaxMana() - lpOnlineFollower->ParametaBox.IP.GetMana();
							TempSI = (SI32)((float)TempSI * TempPercent);
							lpOnlineFollower->ParametaBox.IP.SetMana(lpOnlineFollower->ParametaBox.IP.GetMana() + TempSI, 12);
						}
					}

					CureSolder[TempCount].siLife = lpOnlineFollower->ParametaBox.IP.GetLife();
					CureSolder[TempCount].siMana = lpOnlineFollower->ParametaBox.IP.GetMana();
					TempMyMoney -= TempMoney;

					// DB ����
					TempVillageCapital = MONEY(TempMoney * ON_CAPITAL_RATE_CURE );
					TempVillageCapitalAdd += TempVillageCapital;					

					TempDBIKHealAtVillage.DBAccount        = pPlayer->stAccountInfo.siDBAccount;
					TempDBIKHealAtVillage.uiSlot           = pPlayer->stAccountInfo.uiSlot;
					TempDBIKHealAtVillage.siMoney          = TempMoney;
					TempDBIKHealAtVillage.siVillageCapital = TempVillageCapital;
					TempDBIKHealAtVillage.uiVillageCode    = m_lpVillage->uiVillageCode;
					m_lpVillage->clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_HEALATVILLAGE, LPVOID(&TempDBIKHealAtVillage), sizeof(TempDBIKHealAtVillage));
				}
			}
		}
	}

	// ���� �����Ѵ�.
	TempMyMoney = pPlayer->siMoney - TempMyMoney;
	pPlayer->DecreaseMoney(TempMyMoney, FALSE);
	m_lpVillage->IncreaseCapital(TempVillageCapitalAdd,ON_PROFIT_CURE, FALSE);
	TempMyMoney = pPlayer->siMoney;

	// Message ����
	pPlayer->SendFieldMsg(ON_RESPONSE_HOSPITAL_CUREALLSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(&TempMyMoney), LPVOID(CureSolderCount), LPVOID((strCureSolder*)CureSolder));

	return TRUE;
}

SI32 SOVillage_Hospital::HospitalGetCureMoney(SOPlayer *pPlayer, UI08 Slot, SI32 Discount)
{
	OnlineFollower*		lpOnlineFollower;
	SI32				TempReturn;

	// ����� ������ HP �� 10��, MP�� 20��
	lpOnlineFollower = pPlayer->IFollower.GetFollower(Slot);
	if(lpOnlineFollower)
	{
		if(lpOnlineFollower->ParametaBox.IP.GetLife() < lpOnlineFollower->ParametaBox.GetMaxLife())
		{
			/*
			if(lpOnlineFollower->ParametaBox.IP.GetLife() == 0)
			{
				// ���� ��Ȱ�� �ʿ��Ұ�� ��Ȱ�ϸ� HP1�� �ֹǷ� �װ� �� ���� ���
				TempReturn = (((lpOnlineFollower->ParametaBox.GetMaxLife() - 1) - lpOnlineFollower->ParametaBox.IP.GetLife()) * 1) + ((lpOnlineFollower->ParametaBox.GetMaxMana() - lpOnlineFollower->ParametaBox.IP.GetMana()) * 4);
				TempReturn = TempReturn - ((TempReturn * Discount) / 100);
				return TempReturn;
			}
			else
			{
				TempReturn = ((lpOnlineFollower->ParametaBox.GetMaxLife() - lpOnlineFollower->ParametaBox.IP.GetLife()) * 1) + ((lpOnlineFollower->ParametaBox.GetMaxMana() - lpOnlineFollower->ParametaBox.IP.GetMana()) * 4);
				TempReturn = TempReturn - ((TempReturn * Discount) / 100);
				return TempReturn;
			}
			*/
			TempReturn =  0;
			if(lpOnlineFollower->ParametaBox.GetMaxLife() > lpOnlineFollower->ParametaBox.IP.GetLife()) TempReturn += ((lpOnlineFollower->ParametaBox.GetMaxLife() - lpOnlineFollower->ParametaBox.IP.GetLife()) * 1);
			if(lpOnlineFollower->ParametaBox.GetMaxMana() > lpOnlineFollower->ParametaBox.IP.GetMana()) TempReturn += ((lpOnlineFollower->ParametaBox.GetMaxMana() - lpOnlineFollower->ParametaBox.IP.GetMana()) * 4);

			TempReturn = TempReturn - ((TempReturn * Discount) / 100);
			return TempReturn;
		}
		if(lpOnlineFollower->ParametaBox.IP.GetMana() < lpOnlineFollower->ParametaBox.GetMaxMana())
		{
			TempReturn =  0;
			if(lpOnlineFollower->ParametaBox.GetMaxLife() > lpOnlineFollower->ParametaBox.IP.GetLife()) TempReturn += ((lpOnlineFollower->ParametaBox.GetMaxLife() - lpOnlineFollower->ParametaBox.IP.GetLife()) * 1);
			if(lpOnlineFollower->ParametaBox.GetMaxMana() > lpOnlineFollower->ParametaBox.IP.GetMana()) TempReturn += ((lpOnlineFollower->ParametaBox.GetMaxMana() - lpOnlineFollower->ParametaBox.IP.GetMana()) * 4);

			TempReturn = TempReturn - ((TempReturn * Discount) / 100);
			return TempReturn;
		}
	}

	return 0;
}

SI32 SOVillage_Hospital::HospitalGetRevivalMoney(SOPlayer *pPlayer, UI08 Slot, SI32 Discount)
{
	return 0;			// �һ��� ����

	OnlineFollower*		lpOnlineFollower;
	SI32				TempReturn;

	// ����� ������ Level�� 250��
	lpOnlineFollower = pPlayer->IFollower.GetFollower(Slot);
	if(lpOnlineFollower)
	{
		if(lpOnlineFollower->ParametaBox.IP.GetLife() == 0)
		{
			TempReturn = (lpOnlineFollower->ParametaBox.IP.GetLevel() * 250);

			TempReturn = TempReturn - ((TempReturn * Discount) / 100);
			return TempReturn;
		}
	}

	return 0;
}

