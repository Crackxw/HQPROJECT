#include "SOMain.h"
#include "SODefine.h"
#include "SOWorld.h"
#include "SOVillage.h"

#include "SODatabase.h"
#include "SOVillage_BarrackStructure.h"


SOVillage_Barrack::SOVillage_Barrack()
{
	m_uiVillageCode = 0;

	m_lpVillage = NULL;
}

SOVillage_Barrack::~SOVillage_Barrack()
{
	Free();
}

BOOL SOVillage_Barrack::Init(UI16 uiVillageCode, DWORD dwMaxPlayer, SOVillage* lpVillage)
{
	m_uiVillageCode	=	uiVillageCode;
	m_lpVillage		=	lpVillage;

	SOVillage_BaseStructure::Init(uiVillageCode, dwMaxPlayer);

	return TRUE;
}

VOID SOVillage_Barrack::Free()
{
	m_uiVillageCode = 0;

	m_lpVillage = NULL;

	SOVillage_BaseStructure::Free();	
}

BOOL SOVillage_Barrack::BarrackGetSolderList(SOPlayer *pPlayer)
{
	OnInnBuySolderInfo		TempBuffer[128];
	SI32					TempCount;
	SI32					TempCount1;
	SI32					TempUnitCount;
	CharHead*				lpCharHead;
	SolderListHeader*		lpSolderListHeader;
	SI32					TempDiscountPercent;

	memset(TempBuffer, 0, sizeof(OnInnBuySolderInfo)*128);

	TempDiscountPercent = 0;
	// ���� ���� ����̸�
	if(m_lpVillage->uiGuildID != 0)
	{
		if((m_lpVillage->uiGuildID == pPlayer->uiGuildID) && (pPlayer->siClassInGuild != m_lpVillage->clWorld->clIGuildClass.GetJWJCode()))
		{
			TempDiscountPercent += MAX_GUILDDISCOUNT;
		}
		if(m_lpVillage->clWorld->clIGuild.IsHostilityRelation(m_lpVillage->uiGuildID, pPlayer->uiGuildID) == TRUE)
		{
			TempDiscountPercent -= MAX_GUILDDISCOUNT;
		}
		if(m_lpVillage->clWorld->clIGuild.IsEnemyRelationWar(m_lpVillage->uiGuildID, pPlayer->uiGuildID) == TRUE)
		{
			TempDiscountPercent -= MAX_GUILDDISCOUNT;
		}
	}
			
	// ����Ҽ� �ִ� �뺴�� ����� �����Ѵ�.
	lpSolderListHeader = m_lpVillage->clWorld->clSolderListParser.GetSolderList(STRUCTCODE_BARRACK, m_lpVillage->GetVillageState());
	TempUnitCount      = 0;
	if(lpSolderListHeader)
	{
		for(TempCount = 0; TempCount < m_lpVillage->clWorld->clCharParser.GetCharTotalNum(); TempCount++)
		{
			lpCharHead = m_lpVillage->clWorld->clCharParser.GetCharHeader(TempCount);

			for(TempCount1 = 0; TempCount1 < MAX_SOLDER; TempCount1++)
			{
				if(lpSolderListHeader->siSellSolder[TempCount1] == lpCharHead->uiID)
				{
					TempBuffer[TempUnitCount].siCode               = lpCharHead->uiID;
					TempBuffer[TempUnitCount].siLevel              = lpCharHead->siNeedLv;
					TempBuffer[TempUnitCount].siCredit             = lpCharHead->siNeedCredit;
					TempBuffer[TempUnitCount].uiCharNameAddOption  = 0;
					TempBuffer[TempUnitCount].siMoney              = lpCharHead->siCost - ((lpCharHead->siCost * TempDiscountPercent) / 100);

					TempBuffer[TempUnitCount].SolderInfo.uiKind    = lpCharHead->uiID;
					TempBuffer[TempUnitCount].SolderInfo.uiItemNum = 0;
					TempBuffer[TempUnitCount].SolderInfo.siExp     = 0;
					TempBuffer[TempUnitCount].SolderInfo.siStr     = lpCharHead->Parameta.GetStr();
					TempBuffer[TempUnitCount].SolderInfo.siDex     = lpCharHead->Parameta.GetDex();
					TempBuffer[TempUnitCount].SolderInfo.siVit     = lpCharHead->Parameta.GetVit();
					TempBuffer[TempUnitCount].SolderInfo.siInt     = lpCharHead->Parameta.GetInt();
					TempBuffer[TempUnitCount].SolderInfo.siLife    = lpCharHead->Parameta.GetLife();
					TempBuffer[TempUnitCount].SolderInfo.siMana    = lpCharHead->siManaMax;

					TempUnitCount++;

					TempCount1 = MAX_SOLDER;
					break;
				}
			}
		}
	}

	// Message����
	pPlayer->SendFieldMsg(ON_RESPONSE_BARRACK_GETSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(TempUnitCount), LPVOID(TempBuffer), LPVOID(TempDiscountPercent));

	return TRUE;
}

BOOL SOVillage_Barrack::BarrackBuySolder(SOPlayer *pPlayer, SI32 Code, BYTE* lpName)
{
	BOOL				fFlag;
	MONEY				iMoney;
	MONEY				TempCost;
	SI32				siSlot;
	SI32				TempCount;
	OnFollowerInfo		TempOnFollowerInfo;
	CharHead*			lpCharHead;
	CharHead*			lpCharHead1;
	BYTE				TempBuffer[256];
//	int					siReturn;
	SI32				TempDiscountPercent;
	MONEY				TempEmployPorfit;
	SI32				TempNowFollower;

	memset(TempBuffer, 0, sizeof(TempBuffer));

	TempDiscountPercent = 0;
	// ���� ���� ����̸�
	if(m_lpVillage->uiGuildID != 0)
	{
		if((m_lpVillage->uiGuildID == pPlayer->uiGuildID) && (pPlayer->siClassInGuild != m_lpVillage->clWorld->clIGuildClass.GetJWJCode()))
		{
			TempDiscountPercent += MAX_GUILDDISCOUNT;
		}
		if(m_lpVillage->clWorld->clIGuild.IsHostilityRelation(m_lpVillage->uiGuildID, pPlayer->uiGuildID) == TRUE)
		{
			TempDiscountPercent -= MAX_GUILDDISCOUNT;
		}
		if(m_lpVillage->clWorld->clIGuild.IsEnemyRelationWar(m_lpVillage->uiGuildID, pPlayer->uiGuildID) == TRUE)
		{	
			TempDiscountPercent -= MAX_GUILDDISCOUNT;
		}	
	}

	// ����Ҽ� �ִ� �뺴���� �˻��Ѵ�.
	fFlag = FALSE;
	for(TempCount = 0; TempCount < m_lpVillage->clWorld->clCharParser.GetCharTotalNum(); TempCount++)
	{
		lpCharHead = m_lpVillage->clWorld->clCharParser.GetCharHeader(TempCount);

		if(lpCharHead->uiID == Code)
		{
			fFlag       = TRUE;
			lpCharHead1 = lpCharHead;
			break;
		}
	}

	// ���尡���� Slot�� �˻��Ѵ�.
	TempNowFollower = 0;
	if(fFlag)
	{
		fFlag  = FALSE;
		siSlot = -1;
		for(TempCount = ON_MAX_FOLLOWER_NUM-1; TempCount > 0; TempCount--)
		{
			if(pPlayer->IFollower.IsValidFollower(TempCount) == FALSE)
			{
				fFlag  = TRUE;
				siSlot = TempCount;
			}
			else
			{
				TempNowFollower++;
			}
		}
	}

	// ���� �ɷ�ġ�� �뺴�� �� �̻� �뺴�� ����Ҽ� �ִ��� �˻�
	if(fFlag)
	{
		if(TempNowFollower >= m_lpVillage->clWorld->clMercenaryParser.GetMaxFollowerNum(pPlayer->GetTradeGrade(), pPlayer->IFollower.GetFollowerParameterLevel(0)))
		{
			fFlag = FALSE;
		}
	}

	// ����Ҽ� �ִ� �����̳� �ſ뵵�� �ִ��� �˻�
	if(fFlag)
	{
		if((pPlayer->IFollower.GetFollowerParameterLevel(0) < lpCharHead1->siNeedLv) && (pPlayer->GetTradeGrade() < lpCharHead1->siNeedCredit))
		{
			fFlag = FALSE;
		}
	}

	if(fFlag && (siSlot > 0))
	{
		// ��� �Ҽ� ������ �� ������ ������ �����Ѵ�.
		TempCost = (__int64)lpCharHead1->siCost - (((__int64)lpCharHead1->siCost * TempDiscountPercent) / 100);
		if(((__int64)pPlayer->siMoney - TempCost) >= 0)
		{
			pPlayer->DecreaseMoney(TempCost, FALSE);
			// �뺴 ������� 10%�� ���� �ڻ����� ������.
			TempEmployPorfit = m_lpVillage->IncreaseCapital(MONEY(TempCost*ON_CAPITAL_RATE_EMPLOY),ON_PROFIT_EMPLOY, FALSE);
			iMoney = pPlayer->siMoney;

			// Player�� Data�� �� ������ �߰��Ѵ�.(DB�� ������� �Ѵ�.)
			ZeroMemory(&TempOnFollowerInfo, sizeof(TempOnFollowerInfo));
			TempOnFollowerInfo.uiKind              = Code;
			TempOnFollowerInfo.uiItemNum           = 0;
			TempOnFollowerInfo.siLevel             = 1;
			TempOnFollowerInfo.siExp               = 0;
			TempOnFollowerInfo.siStr               = lpCharHead1->Parameta.GetStr();
			TempOnFollowerInfo.siDex               = lpCharHead1->Parameta.GetDex();
			TempOnFollowerInfo.siVit               = lpCharHead1->Parameta.GetVit();
			TempOnFollowerInfo.siInt               = lpCharHead1->Parameta.GetInt();
			TempOnFollowerInfo.siLife              = lpCharHead1->Parameta.GetLife();
			TempOnFollowerInfo.siMana              = lpCharHead1->siManaMax;
			TempOnFollowerInfo.siBonus             = 0;
			TempOnFollowerInfo.siLiftConstant      = lpCharHead1->siCon;
			TempOnFollowerInfo.siEquipMinDamage    = lpCharHead1->Parameta.EquipMinDamage;
			TempOnFollowerInfo.siEquipMaxDamage    = lpCharHead1->Parameta.EquipMaxDamage;
			TempOnFollowerInfo.siAC                = lpCharHead1->Parameta.GetAC();
			TempOnFollowerInfo.siDamageRegist      = lpCharHead1->Parameta.DamageResist;
			TempOnFollowerInfo.siMagicRegist       = lpCharHead1->Parameta.MagicResist;
			strncpy((char*)TempOnFollowerInfo.Name, (char*)lpName, ON_ID_LENGTH);
			ZeroMemory(TempBuffer, 256);
			CopyMemory(TempBuffer, lpName, ON_ID_LENGTH);

			DBIKBuyMercenary		TempDBIKBuyMercenary;
			
			pPlayer->IFollower.InitFollower(siSlot, &TempOnFollowerInfo, lpCharHead1->siNation, &m_lpVillage->clWorld->clItem);
			ZeroMemory(&TempDBIKBuyMercenary, sizeof(DBIKBuyMercenary));
			TempDBIKBuyMercenary.DBAccount       = pPlayer->stAccountInfo.siDBAccount;
			TempDBIKBuyMercenary.uiSlot          = pPlayer->stAccountInfo.uiSlot;
			TempDBIKBuyMercenary.uiMercenarySlot = siSlot;
			TempDBIKBuyMercenary.siKind          = TempOnFollowerInfo.uiKind;
			TempDBIKBuyMercenary.uiVillageCode	 = m_lpVillage->uiVillageCode;
			TempDBIKBuyMercenary.siMoney         = TempCost;
			TempDBIKBuyMercenary.siEmployProfit  = TempEmployPorfit;
			m_lpVillage->clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_BUYMERCENARY, LPVOID(&TempDBIKBuyMercenary), sizeof(TempDBIKBuyMercenary));

			// Client�� �� ������ ������.
			pPlayer->SendFieldMsg(ON_RESPONSE_BARRACK_BUYSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(&iMoney), LPVOID(siSlot), LPVOID(&TempOnFollowerInfo));
		}
		else
		{
			pPlayer->SendFieldMsg(ON_RESPONSE_BARRACK_BUYSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{
		pPlayer->SendFieldMsg(ON_RESPONSE_BARRACK_BUYSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}

	return TRUE;
}

BOOL SOVillage_Barrack::BarrackGetDismissalSolderList(SOPlayer *pPlayer)
{
	// �뺴 �˻�
	OnDismissalSolderInfo	TempBuffer[128];
	SI32					TempCount;
	SI32					TempUnitCount;
	MONEY					TempMoney;

	memset(TempBuffer, 0, sizeof(OnDismissalSolderInfo)*128);

	// ����Ҽ� �ִ� �뺴�� ����� �����Ѵ�.
	TempUnitCount      = 0;
	for(TempCount = 1; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
	{
		TempMoney = BarrackDismissalSolderMoney(pPlayer, TempCount);

		if(TempMoney != -1)
		{
			TempBuffer[TempUnitCount].siSlot  = TempCount;
			TempBuffer[TempUnitCount].siMoney = TempMoney;

			TempUnitCount++;
		}
	}

	// Message����
	pPlayer->SendFieldMsg(ON_RESPONSE_BARRACK_GETDISMISSALSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(TempUnitCount), LPVOID(TempBuffer));

	return TRUE;
}

BOOL SOVillage_Barrack::BarrackDismissalSolder(SOPlayer *pPlayer, SI32 Slot)
{
	MONEY				TempMoney;
	MONEY				iMoney;
	MONEY				TempEmployProfit;
	int					siReturn;
	
	// �ذ��� �뺴�� �ȼ� �ִ��� �˻�
	if(Slot == 0)                                         return FALSE;
	if(pPlayer->IFollower.IsValidFollower(Slot) == FALSE) return FALSE;

	// ���� ���
	TempMoney = BarrackDismissalSolderMoney(pPlayer, Slot);
	if(TempMoney == -1) return FALSE;

	// ���� �ø���.
	TempMoney = pPlayer->IncreaseMoney(TempMoney, FALSE);
	iMoney    = pPlayer->siMoney;

	// �뺴 ������� 10%�� ���� �ڻ����� ������.
	TempEmployProfit = m_lpVillage->IncreaseCapital(MONEY(TempMoney*ON_CAPITAL_RATE_EMPLOY),ON_PROFIT_EMPLOY, FALSE);

	// Slot�� �����Ѵ�.
	DBIKDeleteMercenary		TempDBIKDeleteMercenary;

	siReturn = 0;
	TempDBIKDeleteMercenary.DBAccount             = pPlayer->stAccountInfo.siDBAccount;
	TempDBIKDeleteMercenary.uiSlot                = pPlayer->stAccountInfo.uiSlot;
	TempDBIKDeleteMercenary.siDeleteMercenarySlot = Slot;
	TempDBIKDeleteMercenary.siVillageCode         = m_lpVillage->uiVillageCode;
	TempDBIKDeleteMercenary.siMoney               = TempMoney;
	TempDBIKDeleteMercenary.siEmployProfit        = TempEmployProfit;
	m_lpVillage->clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_DELETEMERCENARY, LPVOID(&TempDBIKDeleteMercenary), sizeof(TempDBIKDeleteMercenary));
	pPlayer->IFollower.DeleteFollower(Slot);

	pPlayer->SendFieldMsg(ON_RESPONSE_BARRACK_DISMISSALSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(&iMoney), LPVOID(Slot));

	// �ذ�� ���̴� �뺴�� ����
	if(pPlayer->siVMercenarySlot == Slot)
	{
		pPlayer->RecvVMercenaryChange(0);
	}

	return TRUE;
}

MONEY SOVillage_Barrack::BarrackDismissalSolderMoney(SOPlayer *pPlayer, SI32 Slot)
{
	MONEY				TempBuyMoney;
	CharHead*			lpCharHead;
	int					TempCount;

	if(Slot == 0)                                         return -1;
	if(pPlayer->IFollower.IsValidFollower(Slot) == FALSE) return -1;
	
	lpCharHead = NULL;
	for(TempCount = 0; TempCount < m_lpVillage->clWorld->clCharParser.GetCharTotalNum(); TempCount++)
	{
		lpCharHead = m_lpVillage->clWorld->clCharParser.GetCharHeader(TempCount);

		if(lpCharHead->uiID == pPlayer->IFollower.GetFollowerCharID(Slot))
		{
			lpCharHead = lpCharHead;
			break;
		}
	}

	// ���� �Ϲݿ뺴�� �ƴ� ����̰ų� ���� �뺴�̸� ����
	if( lpCharHead->siGeneral != 0 ) return -1;

	TempBuyMoney = (MONEY)lpCharHead->siCost;
	TempBuyMoney = (((TempBuyMoney + ((pPlayer->IFollower.GetFollowerParameterLevel(Slot) - 1) * (TempBuyMoney / 10)))) / 3);

	return TempBuyMoney;
}

BOOL SOVillage_Barrack::GBarrackGetSolderList(SOPlayer *pPlayer)
{
	OnInnBuySolderInfo		TempBuffer[32];
	SI32					TempCount;
	SI32					TempCount1;
	SI32					TempUnitCount;
	CharHead*				lpCharHead;
	SolderListHeader*		lpSolderListHeader;
	SI32					TempDiscountPercent;

	memset(TempBuffer, 0, sizeof(OnInnBuySolderInfo)*32);

	TempDiscountPercent = 0;
	
	//OnInnBuySolderInfo*	 SetBuySolderList(OnInnBuySolderInfo* TempBuffer,SolderListHeader* lpSolderListHeader,SI32& TempUnitCount,SI32& TempDiscountPercent);

	// ����Ҽ� �ִ� �뺴�� ����� �����Ѵ�.
	lpSolderListHeader = m_lpVillage->clWorld->clSolderListParser.GetSolderList(STRUCTCODE_GBARRACK, m_lpVillage->GetVillageState());

	TempUnitCount      = 0;
	
	if(lpSolderListHeader)
	{
		for(TempCount = 0; TempCount < m_lpVillage->clWorld->clCharParser.GetCharTotalNum(); TempCount++)
		{
			lpCharHead = m_lpVillage->clWorld->clCharParser.GetCharHeader(TempCount);

			for(TempCount1 = 0; TempCount1 < MAX_SOLDER; TempCount1++)
			{
				if(lpSolderListHeader->siSellSolder[TempCount1] == lpCharHead->uiID)
				{
					TempBuffer[TempUnitCount].siCode               = lpCharHead->uiID;
					TempBuffer[TempUnitCount].siLevel              = lpCharHead->siNeedLv;
					TempBuffer[TempUnitCount].siCredit             = lpCharHead->siNeedCredit;
					TempBuffer[TempUnitCount].uiCharNameAddOption  = 0;
					TempBuffer[TempUnitCount].siMoney              = lpCharHead->siCost - ((lpCharHead->siCost * TempDiscountPercent) / 100);

					TempBuffer[TempUnitCount].SolderInfo.uiKind    = lpCharHead->uiID;
					TempBuffer[TempUnitCount].SolderInfo.uiItemNum = 0;
					TempBuffer[TempUnitCount].SolderInfo.siExp     = 0;
					TempBuffer[TempUnitCount].SolderInfo.siStr     = lpCharHead->Parameta.GetStr();
					TempBuffer[TempUnitCount].SolderInfo.siDex     = lpCharHead->Parameta.GetDex();
					TempBuffer[TempUnitCount].SolderInfo.siVit     = lpCharHead->Parameta.GetVit();
					TempBuffer[TempUnitCount].SolderInfo.siInt     = lpCharHead->Parameta.GetInt();
					TempBuffer[TempUnitCount].SolderInfo.siLife    = lpCharHead->Parameta.GetLife();
					TempBuffer[TempUnitCount].SolderInfo.siMana    = lpCharHead->siManaMax;

					TempUnitCount++;

					TempCount1 = MAX_SOLDER;
					break;
				}
			}
		}
	}

	// Message����
	pPlayer->SendFieldMsg(ON_RESPONSE_GBARRACK_GETSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(TempUnitCount), LPVOID(TempBuffer), LPVOID(TempDiscountPercent));

	return TRUE;
}

BOOL SOVillage_Barrack::GBarrackBuySolder(SOPlayer *pPlayer, SI32 Code, BYTE* lpName)
{
	BOOL				fFlag;
	MONEY				iMoney;
	MONEY				TempCost;
	MONEY				TempEmployPorfit;
	SI32				siSlot;
	SI32				TempCount;
	SI32				TempCount1;
	OnFollowerInfo		TempOnFollowerInfo;
	CharHead*			lpCharHead;
	CharHead*			lpCharHead1;
	CharHead*			lpCharHead2;
	BYTE				TempBuffer[256];
//	int					siReturn;
	SI32				TempDiscountPercent;
	SI32				TempNowFollower;
	SI32				TempNowGeneralFollower;
	SI32				TempNowGeneralExFollower;

	TempDiscountPercent = 0;
	lpCharHead1			= NULL;
	memset(TempBuffer, 0, sizeof(TempBuffer));

	// ����Ҽ� �ִ� �뺴���� �˻��Ѵ�.
	fFlag = FALSE;
	for(TempCount = 0; TempCount < m_lpVillage->clWorld->clCharParser.GetCharTotalNum(); TempCount++)
	{
		lpCharHead = m_lpVillage->clWorld->clCharParser.GetCharHeader(TempCount);

		if(lpCharHead->uiID == Code)
		{
			fFlag       = TRUE;
			lpCharHead1 = lpCharHead;
			break;
		}
	}

	// ���尡���� Slot�� �˻��Ѵ�.
	TempNowFollower        = 0;
	TempNowGeneralFollower = 0;
	TempNowGeneralExFollower = 0;
	if(fFlag)
	{
		fFlag  = FALSE;
		siSlot = -1;
		for(TempCount = ON_MAX_FOLLOWER_NUM-1; TempCount > 0; TempCount--)
		{
			if(pPlayer->IFollower.IsValidFollower(TempCount) == FALSE)
			{
				fFlag  = TRUE;
				siSlot = TempCount;
			}
			else
			{
				TempNowFollower++;

				// �̹� ���� ����� �ִ��� �˻�
				if(pPlayer->IFollower.GetFollowerParameterKind(TempCount) == lpCharHead1->uiID)
				{
					fFlag = FALSE;
					break;
				}

				// ������� �˻�
				for(TempCount1 = 0; TempCount1 < m_lpVillage->clWorld->clCharParser.GetCharTotalNum(); TempCount1++)
				{
					lpCharHead2 = m_lpVillage->clWorld->clCharParser.GetCharHeader(TempCount1);

					if(lpCharHead2->uiID == pPlayer->IFollower.GetFollowerParameterKind(TempCount))
					{
						if( lpCharHead2->siGeneral == 1 )
						{
							TempNowGeneralFollower++;
						}
						if( lpCharHead2->siGeneral == 3 )
						{
							TempNowGeneralExFollower++;
						}
						break;
					}
				}
			}
		}
	}

	// ���� �ɷ�ġ�� �뺴�� �� �̻� �뺴�� ����Ҽ� �ִ��� �˻�
	if(fFlag)
	{
		if(TempNowFollower >= m_lpVillage->clWorld->clMercenaryParser.GetMaxFollowerNum(pPlayer->GetTradeGrade(), pPlayer->IFollower.GetFollowerParameterLevel(0)))
		{
			fFlag = FALSE;
		}
	}

	// ��� ������ ����� �˻�
	if(TempNowGeneralFollower + TempNowGeneralExFollower >= pPlayer->m_siMaxGeneral)
	{
		fFlag = FALSE;
	}

	// ����Ҽ� �ִ� �����̳� �ſ뵵�� �ִ��� �˻�
	if(fFlag)
	{
		if((pPlayer->IFollower.GetFollowerParameterLevel(0) < lpCharHead1->siNeedLv) && (pPlayer->GetTradeGrade() < lpCharHead1->siNeedCredit))
		{
			fFlag = FALSE;
		}
	}
	SI16 tempID;
	GeneralHeader* pGeneral;
	if(fFlag)
	{
		for(SI16 i = 1 ; i < 12 ; i++)
		{
			tempID = pPlayer->IFollower.GetFollowerParameterKind(i);
			pGeneral = m_lpVillage->clWorld->clGeneralExParser.GetGeneralChar(tempID);
			if(lpCharHead1)
			{
				if(pGeneral != NULL)
				{
					if((lpCharHead1->uiID  == pGeneral->siGeneralBaseCharacterCode)
					|| (lpCharHead1->uiID == pGeneral->siGeneralCharacterCode))
					{
						fFlag = FALSE;
						break;
					}
				}
			}
		}
	}

	if(fFlag && (siSlot > 0))
	{
		// ��� �Ҽ� ������ �� ������ ������ �����Ѵ�.
		TempCost = (__int64)lpCharHead1->siCost - (((__int64)lpCharHead1->siCost * TempDiscountPercent) / 100);
		if(((__int64)pPlayer->siMoney - TempCost) >= 0)
		{
			pPlayer->DecreaseMoney(TempCost, FALSE);
			// �뺴 ������� 10%�� ���� �ڻ����� ������.
			TempEmployPorfit = m_lpVillage->IncreaseCapital(MONEY(TempCost*ON_CAPITAL_RATE_EMPLOY),ON_PROFIT_EMPLOY, FALSE);
			iMoney = pPlayer->siMoney;

			// Player�� Data�� �� ������ �߰��Ѵ�.(DB�� ������� �Ѵ�.)
			ZeroMemory(&TempOnFollowerInfo, sizeof(TempOnFollowerInfo));
			TempOnFollowerInfo.uiKind              = Code;
			TempOnFollowerInfo.uiItemNum           = 0;
			TempOnFollowerInfo.siLevel             = 1;
			TempOnFollowerInfo.siExp               = 0;
			TempOnFollowerInfo.siStr               = lpCharHead1->Parameta.GetStr();
			TempOnFollowerInfo.siDex               = lpCharHead1->Parameta.GetDex();
			TempOnFollowerInfo.siVit               = lpCharHead1->Parameta.GetVit();
			TempOnFollowerInfo.siInt               = lpCharHead1->Parameta.GetInt();
			TempOnFollowerInfo.siLife              = lpCharHead1->Parameta.GetLife();
			TempOnFollowerInfo.siMana              = lpCharHead1->siManaMax;
			TempOnFollowerInfo.siBonus             = 0;
			TempOnFollowerInfo.siLiftConstant      = lpCharHead1->siCon;
			TempOnFollowerInfo.siEquipMinDamage    = lpCharHead1->Parameta.EquipMinDamage;
			TempOnFollowerInfo.siEquipMaxDamage    = lpCharHead1->Parameta.EquipMaxDamage;
			TempOnFollowerInfo.siAC                = lpCharHead1->Parameta.GetAC();
			TempOnFollowerInfo.siDamageRegist      = lpCharHead1->Parameta.DamageResist;
			TempOnFollowerInfo.siMagicRegist       = lpCharHead1->Parameta.MagicResist;
			strncpy((char*)TempOnFollowerInfo.Name, (char*)lpName, ON_ID_LENGTH);
			ZeroMemory(TempBuffer, 256);
			CopyMemory(TempBuffer, lpName, ON_ID_LENGTH);

			DBIKBuyMercenary		TempDBIKBuyMercenary;
			
			pPlayer->IFollower.InitFollower(siSlot, &TempOnFollowerInfo, lpCharHead1->siNation, &m_lpVillage->clWorld->clItem);
			ZeroMemory(&TempDBIKBuyMercenary, sizeof(DBIKBuyMercenary));
			TempDBIKBuyMercenary.DBAccount       = pPlayer->stAccountInfo.siDBAccount;
			TempDBIKBuyMercenary.uiSlot          = pPlayer->stAccountInfo.uiSlot;
			TempDBIKBuyMercenary.uiMercenarySlot = siSlot;
			TempDBIKBuyMercenary.siKind          = TempOnFollowerInfo.uiKind;
			TempDBIKBuyMercenary.uiVillageCode   = m_lpVillage->uiVillageCode;
			TempDBIKBuyMercenary.siMoney         = TempCost;
			TempDBIKBuyMercenary.siEmployProfit  = TempEmployPorfit;
			m_lpVillage->clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_BUYMERCENARY, LPVOID(&TempDBIKBuyMercenary), sizeof(TempDBIKBuyMercenary));

			// Client�� �� ������ ������.
			pPlayer->SendFieldMsg(ON_RESPONSE_GBARRACK_BUYSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(&iMoney), LPVOID(siSlot), LPVOID(&TempOnFollowerInfo));
		}
		else
		{
			pPlayer->SendFieldMsg(ON_RESPONSE_GBARRACK_BUYSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{
		pPlayer->SendFieldMsg(ON_RESPONSE_GBARRACK_BUYSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}

	return TRUE;
}
/*

BOOL SOVillage_Barrack::GetBarrackChangeJobSolder(SOPlayer* pPlayer)
{
	int TempCount, TempCount1;

	for(TempCount = 0 ; TempCount < ON_MAX_FOLLOWER_NUM ; TempCount++)
	{
		if(pPlayer->IFollower.IsValidFollower(TempCount) == TRUE)
		{			
			for(TempCount1 = 0; TempCount1 < m_lpVillage->clWorld->clGeneralParser.siGeneralNum; TempCount1++)
			{
				// �÷��̾ �뺴�߿� ����� �ٲ� �� �ִ� �뺴�� �ִ����� �˻��Ѵ�.
				if(m_lpVillage->clWorld->clGeneralParser.pGeneralHeader[TempCount1].siGeneralBaseCharacterCode 
					== pPlayer->IFollower.GetFollowerParameterKind(TempCount))
				{					
					GetBarrackChangeJobSolder(TempCount1);					
				}
			}
		}
	}

	return FALSE;
}

BOOL SOVillage_Barrack::GetBarrackChangeJobSolder(int TempCount1)
{
	int  TempCount2, TempCount3, TempCount4;

	for(TempCount4 = 0; TempCount4 < MAX_SOLDER; TempCount4++)
	{
		if(lpSolderListHeader->siSellSolder[TempCount4] == 
			m_lpVillage->clWorld->clGeneralParser.pGeneralHeader[TempCount1].siGeneralCharacterCode)
		{
			// ������ ã���� �ִ��� �˻�
			for(TempCount2 = 0; TempCount2 < TempUnitCount; TempCount2++)
			{
				if(TempBuffer[TempCount2].siSlot == TempCount)
				{
					for(TempCount3 = 0; TempCount3 < 4; TempCount3++)
					{
						if(TempBuffer[TempCount2].siJobList[TempCount3] == 0)
						{
							TempBuffer[TempCount2].siJobList[TempCount3]            = m_lpVillage->clWorld->clGeneralParser.pGeneralHeader[TempCount1].siGeneralCharacterCode;
							TempBuffer[TempCount2].siJobListFlag[TempCount3]        = GBarrack_CheckChangeJob(&m_lpVillage->clWorld->clGeneralParser.pGeneralHeader[TempCount1], pPlayer, TempCount);
							TempBuffer[TempCount2].siJobListChangeLevel[TempCount3] = m_lpVillage->clWorld->clGeneralParser.pGeneralHeader[TempCount1].siGeneralChangeNeedCharacterLv;
							break;
						}
					}

					break;
				}
			}

			// ������ ���� ã�� ���ϸ� ���� �߰�
			if(TempCount2 == TempUnitCount)
			{
				TempBuffer[TempUnitCount].siSlot                  = TempCount;
				TempBuffer[TempUnitCount].siJobList[0]            = m_lpVillage->clWorld->clGeneralParser.pGeneralHeader[TempCount1].siGeneralCharacterCode;
				TempBuffer[TempUnitCount].siJobListFlag[0]        = GBarrack_CheckChangeJob(&m_lpVillage->clWorld->clGeneralParser.pGeneralHeader[TempCount1], pPlayer, TempCount);
				TempBuffer[TempUnitCount].siJobListChangeLevel[0] = m_lpVillage->clWorld->clGeneralParser.pGeneralHeader[TempCount1].siGeneralChangeNeedCharacterLv;
				TempBuffer[TempUnitCount].siBonus                 = GBarrack_ChangeJobBonus(pPlayer, TempCount);
				TempUnitCount++;
			}
			break;
		}
	}
}
*/

BOOL SOVillage_Barrack::GBarrackGetChangeJobSolderList(SOPlayer *pPlayer)
{
	OnChangeJobSolderInfo	TempBuffer[32];
	SI32					TempCount;
	SI32					TempCount1;
	SI32					TempCount2;
	SI32					TempCount3;
	SI32					TempCount4;
	SI32					TempUnitCount;
	SolderListHeader*		lpSolderListHeader;
	SI32					TempDiscountPercent;

	memset(TempBuffer, 0, sizeof(OnChangeJobSolderInfo)*32);

	TempDiscountPercent = 0;
	
	// ����Ҽ� �ִ� �뺴�� ����� �����Ѵ�.
	lpSolderListHeader = m_lpVillage->clWorld->clSolderListParser.GetSolderList(STRUCTCODE_GBARRACK, m_lpVillage->GetVillageState());
	TempUnitCount      = 0;
	ZeroMemory(TempBuffer, sizeof(OnChangeJobSolderInfo) * 32);
	if(lpSolderListHeader)
	{
		for(TempCount = 1; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
		{
			if(pPlayer->IFollower.IsValidFollower(TempCount) == TRUE)
			{
				// �� �뺴�� ����� �ٲ�� �ִ��� �˻�
				// m_lpVillage->clWorld->clGeneralParser.siGeneralNum generalList�� �ִ� �� ����.
				for(TempCount1 = 0; TempCount1 < m_lpVillage->clWorld->clGeneralParser.siGeneralNum; TempCount1++)
				{
					// ����� ��ȭ ������ �뺴�� �ִ����� �˻��Ѵ�.
					// siGeneralBaseCharacterCode ����� ���ϱ����� �뺴 Code
					if(m_lpVillage->clWorld->clGeneralParser.pGeneralHeader[TempCount1].siGeneralBaseCharacterCode 
						== pPlayer->IFollower.GetFollowerParameterKind(TempCount))
					{
						//MAX_SOLDER 12
						for(TempCount4 = 0; TempCount4 < MAX_SOLDER; TempCount4++)
						{
							//solderlist �ļ��� ��ϵǾ� �ִ� �뺴���� �˻�.
							if(lpSolderListHeader->siSellSolder[TempCount4] == 
								m_lpVillage->clWorld->clGeneralParser.pGeneralHeader[TempCount1].siGeneralCharacterCode)
							{
								// ������ ã���� �ִ��� �˻�
								for(TempCount2 = 0; TempCount2 < TempUnitCount; TempCount2++)
								{
									if(TempBuffer[TempCount2].siSlot == TempCount)
									{
										for(TempCount3 = 0; TempCount3 < 4; TempCount3++)
										{
											if(TempBuffer[TempCount2].siJobList[TempCount3] == 0)
											{
												TempBuffer[TempCount2].siJobList[TempCount3]            
												= m_lpVillage->clWorld->clGeneralParser.pGeneralHeader[TempCount1].siGeneralCharacterCode;
												TempBuffer[TempCount2].siJobListFlag[TempCount3]        
												= GBarrack_CheckChangeJob(&m_lpVillage->clWorld->clGeneralParser.pGeneralHeader[TempCount1], pPlayer, TempCount);
												TempBuffer[TempCount2].siJobListChangeLevel[TempCount3] 
												= m_lpVillage->clWorld->clGeneralParser.pGeneralHeader[TempCount1].siGeneralChangeNeedCharacterLv;
												break;
											}
										}

										break;
									}
								}

								// ������ ���� ã�� ���ϸ� ���� �߰�
								if(TempCount2 == TempUnitCount)
								{
									TempBuffer[TempUnitCount].siSlot                  = TempCount;
									TempBuffer[TempUnitCount].siJobList[0]            = m_lpVillage->clWorld->clGeneralParser.pGeneralHeader[TempCount1].siGeneralCharacterCode;
									TempBuffer[TempUnitCount].siJobListFlag[0]        = GBarrack_CheckChangeJob(&m_lpVillage->clWorld->clGeneralParser.pGeneralHeader[TempCount1], pPlayer, TempCount);
									TempBuffer[TempUnitCount].siJobListChangeLevel[0] = m_lpVillage->clWorld->clGeneralParser.pGeneralHeader[TempCount1].siGeneralChangeNeedCharacterLv;
									TempBuffer[TempUnitCount].siBonus                 = GBarrack_ChangeJobBonus(pPlayer, TempCount);
									TempUnitCount++;
								}
								break;
							}
						}
					}
				}
			}
		}
	}

	// Message����
	pPlayer->SendFieldMsg(ON_RESPONSE_GBARRACK_GETCHANGEJOBSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(TempUnitCount), LPVOID(TempBuffer), LPVOID(TempDiscountPercent));

	return TRUE;
}

BOOL SOVillage_Barrack::GBarrackChangeJobSolder(SOPlayer *pPlayer, SI32 Slot, SI32 Kind)
{
	BYTE						TempBuffer[256];
//	int							siReturn;
	CharHead*					lpCharHead;
	CharHead*					lpCharHead1;
	CharHead*					lpCharHead2;
	BOOL						fFlag;
	GeneralHeader*				lpGeneralData;
	SI32						TempCount;
	SI32						TempCount1;
	SI32						TempNowGeneralFollower;
	SI32						TempNowGeneralExFollower;
	OnChangeJobFollowerInfo		TempFollowerInfo;
	MONEY						iMoney;

	fFlag = FALSE;

	memset(TempBuffer, 0, sizeof(TempBuffer));

	// ��� ���� �˻�
	lpGeneralData = NULL;
	lpCharHead1   = NULL;
	for(TempCount = 0; TempCount < m_lpVillage->clWorld->clGeneralParser.siGeneralNum; TempCount++)
	{
		//������ �����ϰ� �� �뺴�� pGeneralHeader[TempCount].siGeneralCharacterCode �� ��ġ �ϴ��� �˻�.
		if(m_lpVillage->clWorld->clGeneralParser.pGeneralHeader[TempCount].siGeneralCharacterCode == Kind)
		{
			//�÷��̾ ������ �뺴�� ������ �ִ°�.
			if(m_lpVillage->clWorld->clGeneralParser.pGeneralHeader[TempCount].siGeneralBaseCharacterCode == pPlayer->IFollower.GetFollowerParameterKind(Slot))
			{
				fFlag         = TRUE;
				lpGeneralData = &m_lpVillage->clWorld->clGeneralParser.pGeneralHeader[TempCount];
				break;
			}
		}
	}

	// �켱 �� �뺴�� ��ü�������� �˻�
	if(fFlag)
	{
		if(GBarrack_CheckChangeJob(lpGeneralData, pPlayer, Slot) == FALSE)
		{
			fFlag = FALSE;
		}
	}


	// ���� ����� ������ �ִ��� �˻�
	TempNowGeneralFollower = 0;
	TempNowGeneralExFollower = 0;
	if(fFlag)
	{
		for(TempCount = ON_MAX_FOLLOWER_NUM-1; TempCount > 0; TempCount--)
		{
			if(pPlayer->IFollower.IsValidFollower(TempCount))
			{
				// �̹� ���� ����� �ִ��� �˻�
				if(pPlayer->IFollower.GetFollowerParameterKind(TempCount) == Kind)
				{
					fFlag = FALSE;
					break;
				}

				// ������� �˻�
				for(TempCount1 = 0; TempCount1 < m_lpVillage->clWorld->clCharParser.GetCharTotalNum(); TempCount1++)
				{
					lpCharHead2 = m_lpVillage->clWorld->clCharParser.GetCharHeader(TempCount1);

					if(lpCharHead2->uiID == pPlayer->IFollower.GetFollowerParameterKind(TempCount))
					{
						if( lpCharHead2->siGeneral == 1 )
						{
							TempNowGeneralFollower++;
						}
						if( lpCharHead2->siGeneral == 3)
						{
							TempNowGeneralExFollower++;
						}
						break;
					}
				}
			}
		}
	}

	// ������ �ִ� ������� �ʰ��ϴ��� �˻�
	if(fFlag)
	{
		if(TempNowGeneralFollower + TempNowGeneralExFollower >= pPlayer->m_siMaxGeneral)
		{
			fFlag = FALSE;
		}
	}

	// �뺴���� �˻�
	if(fFlag)
	{
		fFlag = FALSE;
		for(TempCount = 0; TempCount < m_lpVillage->clWorld->clCharParser.GetCharTotalNum(); TempCount++)
		{
			lpCharHead = m_lpVillage->clWorld->clCharParser.GetCharHeader(TempCount);

			if(lpCharHead->uiID == Kind)
			{
				fFlag       = TRUE;
				lpCharHead1 = lpCharHead;
				break;
			}
		}
	}

	SI16 tempID;
	GeneralHeader* pGeneral;
	if(fFlag)
	{
		for(SI16 i = 1 ; i < 12 ; i++)
		{
			tempID = pPlayer->IFollower.GetFollowerParameterKind(i);
			pGeneral = m_lpVillage->clWorld->clGeneralExParser.GetGeneralChar(tempID);
			if(lpCharHead1)
			{
				if(pGeneral != NULL)
				{
					if((lpCharHead1->uiID  == pGeneral->siGeneralBaseCharacterCode)
					|| (lpCharHead1->uiID == pGeneral->siGeneralCharacterCode))
					{
						fFlag = FALSE;
						break;
					}
				}
			}
		}
	}

	// ����
	if(fFlag && (Slot > 0))
	{
		iMoney = 0;

		// Player�� Data�� �� ������ �߰��Ѵ�.(DB�� ������� �Ѵ�.)
		ZeroMemory(&TempFollowerInfo, sizeof(TempFollowerInfo));
		TempFollowerInfo.uiKind              = Kind;
		TempFollowerInfo.siLevel             = 1;
		TempFollowerInfo.siExp               = 0;
		TempFollowerInfo.siStr               = lpCharHead1->Parameta.GetStr();
		TempFollowerInfo.siDex               = lpCharHead1->Parameta.GetDex();
		TempFollowerInfo.siVit               = lpCharHead1->Parameta.GetVit();
		TempFollowerInfo.siInt               = lpCharHead1->Parameta.GetInt();
		TempFollowerInfo.siLife              = lpCharHead1->Parameta.GetLife();
		TempFollowerInfo.siMana              = lpCharHead1->siManaMax;
		TempFollowerInfo.siBonus             = GBarrack_ChangeJobBonus(pPlayer, Slot);
		TempFollowerInfo.siLiftConstant      = lpCharHead1->siCon;
		TempFollowerInfo.siEquipMinDamage    = lpCharHead1->Parameta.EquipMinDamage;
		TempFollowerInfo.siEquipMaxDamage    = lpCharHead1->Parameta.EquipMaxDamage;
		TempFollowerInfo.siAC                = lpCharHead1->Parameta.GetAC();
		TempFollowerInfo.siDamageRegist      = lpCharHead1->Parameta.DamageResist;
		TempFollowerInfo.siMagicRegist       = lpCharHead1->Parameta.MagicResist;
		strncpy((char*)TempFollowerInfo.Name, (char*)pPlayer->IFollower.GetFollowerName(Slot), ON_ID_LENGTH);
		ZeroMemory(TempBuffer, 256);
		CopyMemory(TempBuffer, pPlayer->IFollower.GetFollowerName(Slot), ON_ID_LENGTH);
		pPlayer->IFollower.UpdataFollower(Slot, &TempFollowerInfo, lpCharHead1->siNation, &m_lpVillage->clWorld->clItem);

		DBIKChangeJobMercenary	TempDBIKChangeJobMercenary;

		TempDBIKChangeJobMercenary.DBAccount       = pPlayer->stAccountInfo.siDBAccount;
		TempDBIKChangeJobMercenary.uiSlot          = pPlayer->stAccountInfo.uiSlot;
		TempDBIKChangeJobMercenary.uiMercenarySlot = Slot;
		TempDBIKChangeJobMercenary.siChangeKind    = TempFollowerInfo.uiKind;
		TempDBIKChangeJobMercenary.siBonus         = TempFollowerInfo.siBonus;
		TempDBIKChangeJobMercenary.uiVillageCode   = m_lpVillage->uiVillageCode;
		m_lpVillage->clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_CHANGEJOBMERCENARY, LPVOID(&TempDBIKChangeJobMercenary), sizeof(TempDBIKChangeJobMercenary));

		// Client�� �� ������ ������.
		pPlayer->SendFieldMsg(ON_RESPONSE_GBARRACK_CHANGEJOBSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(&iMoney), LPVOID(Slot), LPVOID(&TempFollowerInfo));
	}
	else
	{
		// ���� ���н�
		pPlayer->SendFieldMsg(ON_RESPONSE_GBARRACK_CHANGEJOBSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}

	return TRUE;
}

BOOL SOVillage_Barrack::GBarrackGetDismissalSolderList(SOPlayer *pPlayer)
{
	// �뺴 �˻�
	OnDismissalSolderInfo	TempBuffer[32];
	SI32					TempCount;
	SI32					TempUnitCount;
	MONEY					TempMoney;

	memset(TempBuffer, 0, sizeof(OnDismissalSolderInfo)*32);

	// ����Ҽ� �ִ� �뺴�� ����� �����Ѵ�.
	TempUnitCount      = 0;
	for(TempCount = 1; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
	{
		// siGeneral(�����) : �Ϲݿ뺴(0), ���(1), ����(2), 2�����(3)
		TempMoney = GBarrackDismissalSolderMoney(pPlayer, TempCount,1);

		if(TempMoney != -1)
		{
			TempBuffer[TempUnitCount].siSlot  = TempCount;
			TempBuffer[TempUnitCount].siMoney = TempMoney;

			TempUnitCount++;
		}
	}

	// Message����
	pPlayer->SendFieldMsg(ON_RESPONSE_GBARRACK_GETDISMISSALSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(TempUnitCount), LPVOID(TempBuffer));

	return TRUE;
}

BOOL SOVillage_Barrack::GBarrackDismissalSolder(SOPlayer *pPlayer, SI32 Slot)
{
	MONEY				TempMoney;
	MONEY				iMoney;
	MONEY				TempEmployProfit;
	int					siReturn;
	
	// �ذ��� �뺴�� �ȼ� �ִ��� �˻�
	if(Slot == 0)                                         return FALSE;
	if(pPlayer->IFollower.IsValidFollower(Slot) == FALSE) return FALSE;

	// ���� ���
	// siGeneral(�����) : �Ϲݿ뺴(0), ���(1), ����(2), 2�����(3)
	TempMoney = GBarrackDismissalSolderMoney(pPlayer, Slot,1);
	if(TempMoney == -1) return FALSE;

	// ���� �ø���.
	TempMoney = pPlayer->IncreaseMoney(TempMoney, FALSE);
	iMoney    = pPlayer->siMoney;

	// �뺴 ������� 10%�� ���� �ڻ����� ������.
	TempEmployProfit = m_lpVillage->IncreaseCapital(MONEY(TempMoney*ON_CAPITAL_RATE_EMPLOY),ON_PROFIT_EMPLOY, FALSE);

	// Slot�� �����Ѵ�.
	DBIKDeleteMercenary		TempDBIKDeleteMercenary;

	siReturn = 0;
	TempDBIKDeleteMercenary.DBAccount             = pPlayer->stAccountInfo.siDBAccount;
	TempDBIKDeleteMercenary.uiSlot                = pPlayer->stAccountInfo.uiSlot;
	TempDBIKDeleteMercenary.siDeleteMercenarySlot = Slot;
	TempDBIKDeleteMercenary.siVillageCode         = m_lpVillage->uiVillageCode;
	TempDBIKDeleteMercenary.siMoney               = TempMoney;
	TempDBIKDeleteMercenary.siEmployProfit        = TempEmployProfit;
	m_lpVillage->clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_DELETEMERCENARY, LPVOID(&TempDBIKDeleteMercenary), sizeof(TempDBIKDeleteMercenary));
	pPlayer->IFollower.DeleteFollower(Slot);

	pPlayer->SendFieldMsg(ON_RESPONSE_GBARRACK_DISMISSALSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(&iMoney), LPVOID(Slot));

	// �ذ�� ���̴� �뺴�� ����
	if(pPlayer->siVMercenarySlot == Slot)
	{
		pPlayer->RecvVMercenaryChange(0);
	}

	return TRUE;
}

//Generallist GeneralExlist ���.
BOOL SOVillage_Barrack::GBarrack_CheckChangeJob(GeneralHeader* lpGeneralData, SOPlayer *pPlayer, SI32 Slot)
{
	if((Slot <= 0) || (Slot >= ON_MAX_FOLLOWER_NUM)) return FALSE;

	// ���� �ڵ� �˻�
	switch(pPlayer->IFollower.GetFollowerParameterNation(0))
	{
		case 1:
			if(m_lpVillage->GetVillageState() != ON_COUNTRY_KOREA)
			{
				return FALSE;
			}
			break;

		case 10:
			if(m_lpVillage->GetVillageState() != ON_COUNTRY_JAPAN)
			{
				return FALSE;
			}
			break;

		case 100:
			if(m_lpVillage->GetVillageState() != ON_COUNTRY_CHINA)
			{
				return FALSE;
			}
			break;

		case 1000:
			if(m_lpVillage->GetVillageState() != ON_COUNTRY_TAIWAN)
			{
				return FALSE;
			}
			break;
	}

	// �뺴�� ������ �Ŵ��� �˻�
	if(pPlayer->IFollower.GetFollowerParameterLevel(Slot) < lpGeneralData->siGeneralChangeNeedCharacterLv) return FALSE;

	// ��ȯ������ ��������
	if(lpGeneralData->siGeneralBaseCharacterCode != pPlayer->IFollower.GetFollowerParameterKind(Slot)) return FALSE;

	return TRUE;
}

SI32 SOVillage_Barrack::GBarrack_ChangeJobBonus(SOPlayer *pPlayer, SI32 Slot)
{
	SI32		TempTotalBonus;

	TempTotalBonus =  0;
	TempTotalBonus += pPlayer->IFollower.GetFollowerBaseParameterSTR(Slot);
	TempTotalBonus += pPlayer->IFollower.GetFollowerBaseParameterDEX(Slot);
	TempTotalBonus += pPlayer->IFollower.GetFollowerBaseParameterVIT(Slot);
	TempTotalBonus += pPlayer->IFollower.GetFollowerBaseParameterINT(Slot);
	TempTotalBonus /= 5;

	return TempTotalBonus;
}

MONEY SOVillage_Barrack::GBarrackDismissalSolderMoney(SOPlayer *pPlayer, SI32 Slot,SI32 siGeneralCode)
{
	MONEY				TempBuyMoney;
	CharHead*			lpCharHead;
	int					TempCount;

	if(Slot == 0)                                         return -1;
	if(pPlayer->IFollower.IsValidFollower(Slot) == FALSE) return -1;
	
	lpCharHead = NULL;
	for(TempCount = 0; TempCount < m_lpVillage->clWorld->clCharParser.GetCharTotalNum(); TempCount++)
	{
		lpCharHead = m_lpVillage->clWorld->clCharParser.GetCharHeader(TempCount);

		if(lpCharHead->uiID == pPlayer->IFollower.GetFollowerCharID(Slot))
		{
			lpCharHead = lpCharHead;
			break;
		}
	}

	// ���� ����� �ƴϸ� ����
	if(lpCharHead->siGeneral != siGeneralCode) return -1;


	TempBuyMoney = (MONEY)lpCharHead->siCost;
	TempBuyMoney = (((TempBuyMoney + ((pPlayer->IFollower.GetFollowerParameterLevel(Slot) - 1) * (TempBuyMoney / 10)))) / 3);

	return TempBuyMoney;
}

BOOL SOVillage_Barrack::GBarrackGetGeneralExList(SOPlayer *pPlayer)
{
	OnInnBuySolderInfo		TempBuffer[32];
	SI32					TempCount;
	SI32					TempCount1;
	SI32					TempUnitCount;
	CharHead*				lpCharHead;
	SolderListHeader*		lpSolderListHeader;
	SI32					TempDiscountPercent;

	TempDiscountPercent = 0;
	memset(TempBuffer, 0, sizeof(OnInnBuySolderInfo)*32);
	
	// ����Ҽ� �ִ� �뺴�� ����� �����Ѵ�.
	lpSolderListHeader = m_lpVillage->clWorld->clSolderListParser.GetSolderList(STRUCTCODE_GEXBARRACK, m_lpVillage->GetVillageState());
	TempUnitCount      = 0;
	if(lpSolderListHeader)
	{
		for(TempCount = 0; TempCount < m_lpVillage->clWorld->clCharParser.GetCharTotalNum(); TempCount++)
		{
			lpCharHead = m_lpVillage->clWorld->clCharParser.GetCharHeader(TempCount);

			for(TempCount1 = 0; TempCount1 < MAX_SOLDER; TempCount1++)
			{
				if(lpSolderListHeader->siSellSolder[TempCount1] == lpCharHead->uiID)
				{
					TempBuffer[TempUnitCount].siCode               = lpCharHead->uiID;
					TempBuffer[TempUnitCount].siLevel              = lpCharHead->siNeedLv;
					TempBuffer[TempUnitCount].siCredit             = lpCharHead->siNeedCredit;
					TempBuffer[TempUnitCount].uiCharNameAddOption  = 0;
					TempBuffer[TempUnitCount].siMoney              = lpCharHead->siCost - ((lpCharHead->siCost * TempDiscountPercent) / 100);

					TempBuffer[TempUnitCount].SolderInfo.uiKind    = lpCharHead->uiID;
					TempBuffer[TempUnitCount].SolderInfo.uiItemNum = 0;
					TempBuffer[TempUnitCount].SolderInfo.siExp     = 0;
					TempBuffer[TempUnitCount].SolderInfo.siStr     = lpCharHead->Parameta.GetStr();
					TempBuffer[TempUnitCount].SolderInfo.siDex     = lpCharHead->Parameta.GetDex();
					TempBuffer[TempUnitCount].SolderInfo.siVit     = lpCharHead->Parameta.GetVit();
					TempBuffer[TempUnitCount].SolderInfo.siInt     = lpCharHead->Parameta.GetInt();
					TempBuffer[TempUnitCount].SolderInfo.siLife    = lpCharHead->Parameta.GetLife();
					TempBuffer[TempUnitCount].SolderInfo.siMana    = lpCharHead->siManaMax;

					TempUnitCount++;

					TempCount1 = MAX_SOLDER;
					break;
				}
			}
		}
	}

	// Message����
	pPlayer->SendFieldMsg(ON_RESPONSE_GBARRACK_GET_GENERAL_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(TempUnitCount), LPVOID(TempBuffer), LPVOID(TempDiscountPercent));

	return TRUE;

}

BOOL SOVillage_Barrack::GBarrackBuyGeneralEx(SOPlayer *pPlayer, SI32 Code, BYTE* lpName)
{
	BOOL				fFlag;
	MONEY				iMoney;
	MONEY				TempCost;
	MONEY				TempEmployPorfit;
	SI32				siSlot;
	SI32				TempCount;
	SI32				TempCount1;
	OnFollowerInfo		TempOnFollowerInfo;
	CharHead*			lpCharHead;
	CharHead*			lpCharHead1;
	CharHead*			lpCharHead2;
	BYTE				TempBuffer[256];
//	int					siReturn;
	SI32				TempDiscountPercent;
	SI32				TempNowFollower;
	SI32				TempNowGeneralFollower;				//1�� ���
	SI32				TempNowGeneralExFollower;			//2�� ���

	TempDiscountPercent = 0;
	lpCharHead1			= NULL;

	memset(TempBuffer, 0, sizeof(TempBuffer));

	// ����Ҽ� �ִ� �뺴���� �˻��Ѵ�.
	// Charlist �� �ִ� �뺴�ΰ�.
	fFlag = FALSE;
	for(TempCount = 0; TempCount < m_lpVillage->clWorld->clCharParser.GetCharTotalNum(); TempCount++)
	{
		lpCharHead = m_lpVillage->clWorld->clCharParser.GetCharHeader(TempCount);

		if(lpCharHead->uiID == Code)
		{
			fFlag       = TRUE;
			lpCharHead1 = lpCharHead;
			break;
		}
	}

	// ���尡���� Slot�� �˻��Ѵ�.
	TempNowFollower        = 0;
	TempNowGeneralFollower = 0;
	TempNowGeneralExFollower = 0;
	if(fFlag)
	{
		fFlag  = FALSE;
		siSlot = -1;
		for(TempCount = ON_MAX_FOLLOWER_NUM-1; TempCount > 0; TempCount--)
		{
			if(pPlayer->IFollower.IsValidFollower(TempCount) == FALSE)
			{
				fFlag  = TRUE;
				siSlot = TempCount;
			}
			else
			{
				TempNowFollower++;

				// �̹� ���� ����� �ִ��� �˻�
				if(pPlayer->IFollower.GetFollowerParameterKind(TempCount) == lpCharHead1->uiID)
				{
					fFlag = FALSE;
					break;
				}
				
				// �Ϲ� ����� 2�� ���� ��� �� �Ѱ��� ĳ���͸� ������ ������ �ִ�.

				// ������� �˻�
				for(TempCount1 = 0; TempCount1 < m_lpVillage->clWorld->clCharParser.GetCharTotalNum(); TempCount1++)
				{
					lpCharHead2 = m_lpVillage->clWorld->clCharParser.GetCharHeader(TempCount1);

					if(lpCharHead2->uiID == pPlayer->IFollower.GetFollowerParameterKind(TempCount))
					{
						if( lpCharHead2->siGeneral == 1 )
						{
							TempNowGeneralFollower++;
						}
						//2�� ����� ���� �����ش�.
						if( lpCharHead2->siGeneral == 3 )
						{
							TempNowGeneralExFollower++;
						}
						break;
					}
				}
			}
		}
	}

/*	if(TempNowGeneralExFollower >= 2)
	{
		fFlag = FALSE;
	}
*/
	// ���� �ɷ�ġ�� �뺴�� �� �̻� �뺴�� ����Ҽ� �ִ��� �˻�
	if(fFlag)
	{
		if(TempNowGeneralFollower + TempNowGeneralExFollower >= m_lpVillage->clWorld->clMercenaryParser.GetMaxFollowerNum(pPlayer->GetTradeGrade(), pPlayer->IFollower.GetFollowerParameterLevel(0)))
		{
			fFlag = FALSE;
		}
	}

	// ��� ������ ����� �˻�
	// pPlayer->m_siMaxGeneral DB�� ����Ǿ� �ִ� ��� ���� ��� ����.
	// ��� ������ 2�� ����� ��.
	if(TempNowGeneralFollower + TempNowGeneralExFollower >= pPlayer->m_siMaxGeneral)
	{
		fFlag = FALSE;
	}

	// ����Ҽ� �ִ� �����̳� �ſ뵵�� �ִ��� �˻�
	if(fFlag)
	{
		if((pPlayer->IFollower.GetFollowerParameterLevel(0) < lpCharHead1->siNeedLv) && (pPlayer->GetTradeGrade() < lpCharHead1->siNeedCredit))
		{
			fFlag = FALSE;
		}
	}

	SI16 tempID;
	GeneralHeader* pGeneral;
	if(fFlag)
	{
		for(SI16 i = 1 ; i < 12 ; i++)
		{
			tempID = pPlayer->IFollower.GetFollowerParameterKind(i);
			pGeneral = m_lpVillage->clWorld->clGeneralExParser.GetGeneralChar(tempID);
			if(lpCharHead1)
			{
				if(pGeneral != NULL)
				{
					if((lpCharHead1->uiID  == pGeneral->siGeneralBaseCharacterCode)
					|| (lpCharHead1->uiID == pGeneral->siGeneralCharacterCode))
					{
						fFlag = FALSE;
						break;
					}
				}
			}
		}
	}
	if(fFlag && (siSlot > 0))
	{
		// ��� �Ҽ� ������ �� ������ ������ �����Ѵ�.
		TempCost = (__int64)lpCharHead1->siCost - (((__int64)lpCharHead1->siCost * TempDiscountPercent) / 100);
		if(((__int64)pPlayer->siMoney - TempCost) >= 0)
		{
			pPlayer->DecreaseMoney(TempCost, FALSE);
			// �뺴 ������� 10%�� ���� �ڻ����� ������.
			TempEmployPorfit = m_lpVillage->IncreaseCapital(MONEY(TempCost*ON_CAPITAL_RATE_EMPLOY),ON_PROFIT_EMPLOY, FALSE);
			iMoney = pPlayer->siMoney;

			// Player�� Data�� �� ������ �߰��Ѵ�.(DB�� ������� �Ѵ�.)
			ZeroMemory(&TempOnFollowerInfo, sizeof(TempOnFollowerInfo));
			TempOnFollowerInfo.uiKind              = Code;
			TempOnFollowerInfo.uiItemNum           = 0;
			TempOnFollowerInfo.siLevel             = 1;
			TempOnFollowerInfo.siExp               = 0;
			TempOnFollowerInfo.siStr               = lpCharHead1->Parameta.GetStr();
			TempOnFollowerInfo.siDex               = lpCharHead1->Parameta.GetDex();
			TempOnFollowerInfo.siVit               = lpCharHead1->Parameta.GetVit();
			TempOnFollowerInfo.siInt               = lpCharHead1->Parameta.GetInt();
			TempOnFollowerInfo.siLife              = lpCharHead1->Parameta.GetLife();
			TempOnFollowerInfo.siMana              = lpCharHead1->siManaMax;
			TempOnFollowerInfo.siBonus             = 0;
			TempOnFollowerInfo.siLiftConstant      = lpCharHead1->siCon;
			TempOnFollowerInfo.siEquipMinDamage    = lpCharHead1->Parameta.EquipMinDamage;
			TempOnFollowerInfo.siEquipMaxDamage    = lpCharHead1->Parameta.EquipMaxDamage;
			TempOnFollowerInfo.siAC                = lpCharHead1->Parameta.GetAC();
			TempOnFollowerInfo.siDamageRegist      = lpCharHead1->Parameta.DamageResist;
			TempOnFollowerInfo.siMagicRegist       = lpCharHead1->Parameta.MagicResist;
			strncpy((char*)TempOnFollowerInfo.Name, (char*)lpName, ON_ID_LENGTH);
			ZeroMemory(TempBuffer, 256);
			CopyMemory(TempBuffer, lpName, ON_ID_LENGTH);

			DBIKBuyMercenary		TempDBIKBuyMercenary;
			
			pPlayer->IFollower.InitFollower(siSlot, &TempOnFollowerInfo, lpCharHead1->siNation, &m_lpVillage->clWorld->clItem);
			ZeroMemory(&TempDBIKBuyMercenary, sizeof(DBIKBuyMercenary));
			TempDBIKBuyMercenary.DBAccount       = pPlayer->stAccountInfo.siDBAccount;
			TempDBIKBuyMercenary.uiSlot          = pPlayer->stAccountInfo.uiSlot;
			TempDBIKBuyMercenary.uiMercenarySlot = siSlot;
			TempDBIKBuyMercenary.siKind          = TempOnFollowerInfo.uiKind;
			TempDBIKBuyMercenary.uiVillageCode   = m_lpVillage->uiVillageCode;
			TempDBIKBuyMercenary.siMoney         = TempCost;
			TempDBIKBuyMercenary.siEmployProfit  = TempEmployPorfit;
			m_lpVillage->clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_BUYMERCENARY, LPVOID(&TempDBIKBuyMercenary), sizeof(TempDBIKBuyMercenary));

			// Client�� �� ������ ������.
			pPlayer->SendFieldMsg(ON_RESPONSE_GBARRACK_BUY_GENERAL, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(&iMoney), LPVOID(siSlot), LPVOID(&TempOnFollowerInfo));
		}
		else
		{
			pPlayer->SendFieldMsg(ON_RESPONSE_GBARRACK_BUY_GENERAL, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
	}
	else
	{
		pPlayer->SendFieldMsg(ON_RESPONSE_GBARRACK_BUY_GENERAL, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}

	return TRUE;

}

BOOL SOVillage_Barrack::GBarrackGetChangeGeneralExList(SOPlayer *pPlayer)
{
	OnChangeJobSolderInfo	TempBuffer[32];
	SI32					TempCount;
	SI32					TempCount1;
	SI32					TempCount2;
	SI32					TempCount3;
	SI32					TempCount4;
	SI32					TempUnitCount;
	SolderListHeader*		lpSolderListHeader;
	SI32					TempDiscountPercent;

	TempDiscountPercent = 0;
	memset(TempBuffer, 0, sizeof(OnChangeJobSolderInfo)*32);
	
	// ����Ҽ� �ִ� �뺴�� ����� �����Ѵ�.
	//lpSolderListHeader solder.txt���� �� ������ �ǹ����� �ȼ� �ִ� �亴�� �����´�. 
	lpSolderListHeader = m_lpVillage->clWorld->clSolderListParser.GetSolderList(STRUCTCODE_GEXBARRACK, m_lpVillage->GetVillageState());
	TempUnitCount      = 0;
	ZeroMemory(TempBuffer, sizeof(OnChangeJobSolderInfo) * 32);
	if(lpSolderListHeader)
	{
		for(TempCount = 1; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
		{
			if(pPlayer->IFollower.IsValidFollower(TempCount) == TRUE)
			{
				// �� �뺴�� ����� �ٲ�� �ִ��� �˻�
				// m_lpVillage->clWorld->clGeneralExParser.siGeneralNum generalList�� �ִ� �� ����.
				for(TempCount1 = 0; TempCount1 < m_lpVillage->clWorld->clGeneralExParser.siGeneralNum; TempCount1++)
				{
					// ����� ��ȭ ������ �뺴�� �ִ����� �˻��Ѵ�.
					// siGeneralBaseCharacterCode ����� ���ϱ����� �뺴 Code
					if(m_lpVillage->clWorld->clGeneralExParser.pGeneralHeader[TempCount1].siGeneralBaseCharacterCode 
						== pPlayer->IFollower.GetFollowerParameterKind(TempCount))
					{
						//MAX_SOLDER 12
						for(TempCount4 = 0; TempCount4 < MAX_SOLDER; TempCount4++)
						{
							//solderlist �ļ��� ��ϵǾ� �ִ� �뺴���� �˻�.
							//pGeneralHeader[TempCount1].siGeneralCharacterCode 2�� ����� �����ϸ� ���� �Ǵ� ID.
							if(lpSolderListHeader->siSellSolder[TempCount4] == 
								m_lpVillage->clWorld->clGeneralExParser.pGeneralHeader[TempCount1].siGeneralCharacterCode)
							{
								// ������ ã���� �ִ��� �˻�
								for(TempCount2 = 0; TempCount2 < TempUnitCount; TempCount2++)
								{
									if(TempBuffer[TempCount2].siSlot == TempCount)
									{
										for(TempCount3 = 0; TempCount3 < 4; TempCount3++)
										{
											if(TempBuffer[TempCount2].siJobList[TempCount3] == 0)
											{
												TempBuffer[TempCount2].siJobList[TempCount3]            
												= m_lpVillage->clWorld->clGeneralExParser.pGeneralHeader[TempCount1].siGeneralCharacterCode;
												TempBuffer[TempCount2].siJobListFlag[TempCount3]        
												= GBarrack_CheckChangeJob(&m_lpVillage->clWorld->clGeneralExParser.pGeneralHeader[TempCount1], pPlayer, TempCount);
												TempBuffer[TempCount2].siJobListChangeLevel[TempCount3] 
												= m_lpVillage->clWorld->clGeneralExParser.pGeneralHeader[TempCount1].siGeneralChangeNeedCharacterLv;
												break;
											}
										}

										break;
									}
								}

								// ������ ���� ã�� ���ϸ� ���� �߰�
								if(TempCount2 == TempUnitCount)
								{
									TempBuffer[TempUnitCount].siSlot                  = TempCount;
									TempBuffer[TempUnitCount].siJobList[0]            
									= m_lpVillage->clWorld->clGeneralExParser.pGeneralHeader[TempCount1].siGeneralCharacterCode;
									TempBuffer[TempUnitCount].siJobListFlag[0]        
									= GBarrack_CheckChangeJob(&m_lpVillage->clWorld->clGeneralExParser.pGeneralHeader[TempCount1], pPlayer, TempCount);
									TempBuffer[TempUnitCount].siJobListChangeLevel[0] 
									= m_lpVillage->clWorld->clGeneralExParser.pGeneralHeader[TempCount1].siGeneralChangeNeedCharacterLv;
									TempBuffer[TempUnitCount].siBonus                 = GBarrackGetChangeGeneralExBonus(pPlayer, TempCount);
									TempUnitCount++;
								}
								break;
							}
						}
					}
				}
			}
		}
	}

	// Message����
	pPlayer->SendFieldMsg(ON_RESPONSE_GBARRACK_GET_CHANGE_GENERAL_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(TempUnitCount), LPVOID(TempBuffer), LPVOID(TempDiscountPercent));

	return TRUE;
}

BOOL	SOVillage_Barrack::GBarrackChangeGeneralEx(SOPlayer *pPlayer,SI32 Slot, SI32 Kind)
{
	BYTE						TempBuffer[256];
//	int							siReturn;
	CharHead*					lpCharHead;
	CharHead*					lpCharHead1;
	CharHead*					lpCharHead2;
	BOOL						fFlag;
	GeneralHeader*				lpGeneralData;
	SI32						TempCount;
	SI32						TempCount1;
	SI32						TempNowGeneralFollower;
	SI32						TempNowGeneralExFollower;
	OnChangeJobFollowerInfo		TempFollowerInfo;
	MONEY						iMoney;

	fFlag = FALSE;
	memset(TempBuffer, 0, sizeof(TempBuffer));

	// ��� ���� �˻�
	lpGeneralData = NULL;
	for(TempCount = 0; TempCount < m_lpVillage->clWorld->clGeneralExParser.siGeneralNum; TempCount++)
	{
		//������ �����ϰ� �� �뺴�� pGeneralHeader[TempCount].siGeneralCharacterCode �� ��ġ �ϴ��� �˻�.
		if(m_lpVillage->clWorld->clGeneralExParser.pGeneralHeader[TempCount].siGeneralCharacterCode == Kind)
		{
			//�÷��̾ ������ �뺴�� ������ �ִ°�.
			if(m_lpVillage->clWorld->clGeneralExParser.pGeneralHeader[TempCount].siGeneralBaseCharacterCode == pPlayer->IFollower.GetFollowerParameterKind(Slot))
			{
				fFlag         = TRUE;
				lpGeneralData = &m_lpVillage->clWorld->clGeneralExParser.pGeneralHeader[TempCount];
				break;
			}
		}
	}

	// �켱 �� �뺴�� ��ü�������� �˻�
	if(fFlag)
	{
		if(GBarrack_CheckChangeJob(lpGeneralData, pPlayer, Slot) == FALSE)
		{
			fFlag = FALSE;
		}
	}



	// ���� ����� ������ �ִ��� �˻�
	TempNowGeneralFollower = 0;
	TempNowGeneralExFollower = 0;
	if(fFlag)
	{
		for(TempCount = ON_MAX_FOLLOWER_NUM-1; TempCount > 0; TempCount--)
		{
			if(pPlayer->IFollower.IsValidFollower(TempCount))
			{
				// �̹� ���� ����� �ִ��� �˻�
				if(pPlayer->IFollower.GetFollowerParameterKind(TempCount) == Kind)
				{
					fFlag = FALSE;
					break;
				}

				// ������� �˻�
				for(TempCount1 = 0; TempCount1 < m_lpVillage->clWorld->clCharParser.GetCharTotalNum(); TempCount1++)
				{
					lpCharHead2 = m_lpVillage->clWorld->clCharParser.GetCharHeader(TempCount1);

					if(lpCharHead2->uiID == pPlayer->IFollower.GetFollowerParameterKind(TempCount))
					{
						if( lpCharHead2->siGeneral == 1 )
						{
							TempNowGeneralFollower++;
						}
						//2�� ���� ��� ���� �˻�.
						if(lpCharHead2->siGeneral == 3 )
						{
							TempNowGeneralExFollower++;
						}
						break;
					}
				}
			}
		}
	}

	
	//2�� ����� 2���̻� ����Ҽ� ����.
/*	if(TempNowGeneralExFollower >= 2)
	{
		fFlag = FALSE;
	}
*/
	// ������ �ִ� ������� �ʰ��ϴ��� �˻�
/*	if(fFlag)
	{
		if(TempNowGeneralFollower + TempNowGeneralExFollower>= pPlayer->m_siMaxGeneral)
		{
			fFlag = FALSE;
		}
	}
*/

	// �뺴���� �˻�
	if(fFlag)
	{
		fFlag = FALSE;
		for(TempCount = 0; TempCount < m_lpVillage->clWorld->clCharParser.GetCharTotalNum(); TempCount++)
		{
			lpCharHead = m_lpVillage->clWorld->clCharParser.GetCharHeader(TempCount);

			if(lpCharHead->uiID == Kind)
			{
				fFlag       = TRUE;
				lpCharHead1 = lpCharHead;
				break;
			}
		}
	}

	// ����
	if(fFlag && (Slot > 0))
	{
		iMoney = 0;

		// Player�� Data�� �� ������ �߰��Ѵ�.(DB�� ������� �Ѵ�.)
		ZeroMemory(&TempFollowerInfo, sizeof(TempFollowerInfo));
		TempFollowerInfo.uiKind              = Kind;
		TempFollowerInfo.siLevel             = 1;
		TempFollowerInfo.siExp               = 0;
		TempFollowerInfo.siStr               = lpCharHead1->Parameta.GetStr();
		TempFollowerInfo.siDex               = lpCharHead1->Parameta.GetDex();
		TempFollowerInfo.siVit               = lpCharHead1->Parameta.GetVit();
		TempFollowerInfo.siInt               = lpCharHead1->Parameta.GetInt();
		TempFollowerInfo.siLife              = lpCharHead1->Parameta.GetLife();
		TempFollowerInfo.siMana              = lpCharHead1->siManaMax;
		TempFollowerInfo.siBonus             = GBarrack_ChangeJobBonus(pPlayer, Slot);
		TempFollowerInfo.siLiftConstant      = lpCharHead1->siCon;
		TempFollowerInfo.siEquipMinDamage    = lpCharHead1->Parameta.EquipMinDamage;
		TempFollowerInfo.siEquipMaxDamage    = lpCharHead1->Parameta.EquipMaxDamage;
		TempFollowerInfo.siAC                = lpCharHead1->Parameta.GetAC();
		TempFollowerInfo.siDamageRegist      = lpCharHead1->Parameta.DamageResist;
		TempFollowerInfo.siMagicRegist       = lpCharHead1->Parameta.MagicResist;
		strncpy((char*)TempFollowerInfo.Name, (char*)pPlayer->IFollower.GetFollowerName(Slot), ON_ID_LENGTH);
		ZeroMemory(TempBuffer, 256);
		CopyMemory(TempBuffer, pPlayer->IFollower.GetFollowerName(Slot), ON_ID_LENGTH);
		pPlayer->IFollower.UpdataFollower(Slot, &TempFollowerInfo, lpCharHead1->siNation, &m_lpVillage->clWorld->clItem);

		DBIKChangeJobMercenary	TempDBIKChangeJobMercenary;

		TempDBIKChangeJobMercenary.DBAccount       = pPlayer->stAccountInfo.siDBAccount;
		TempDBIKChangeJobMercenary.uiSlot          = pPlayer->stAccountInfo.uiSlot;
		TempDBIKChangeJobMercenary.uiMercenarySlot = Slot;
		TempDBIKChangeJobMercenary.siChangeKind    = TempFollowerInfo.uiKind;
		TempDBIKChangeJobMercenary.siBonus         = TempFollowerInfo.siBonus;
		TempDBIKChangeJobMercenary.uiVillageCode   = m_lpVillage->uiVillageCode;
		m_lpVillage->clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_CHANGEJOBMERCENARY, LPVOID(&TempDBIKChangeJobMercenary), sizeof(TempDBIKChangeJobMercenary));

		// Client�� �� ������ ������.
		pPlayer->SendFieldMsg(ON_RESPONSE_GBARRACK_CHANGE_GENERAL, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(&iMoney), LPVOID(Slot), LPVOID(&TempFollowerInfo));
	}
	else
	{
		// ���� ���н�
		pPlayer->SendFieldMsg(ON_RESPONSE_GBARRACK_CHANGE_GENERAL, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
	return TRUE;
}

BOOL	SOVillage_Barrack::GBarrackGetDismissalGeneralExList(SOPlayer *pPlayer)
{
	// �뺴 �˻�
	OnDismissalSolderInfo	TempBuffer[32];
	SI32					TempCount;
	SI32					TempUnitCount;
	MONEY					TempMoney;

	memset(TempBuffer, 0, sizeof(OnDismissalSolderInfo)*32);
	TempUnitCount      = 0;
	for(TempCount = 1; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
	{
		// siGeneral(�����) : �Ϲݿ뺴(0), ���(1), ����(2), 2�����(3)
		TempMoney = GBarrackDismissalSolderMoney(pPlayer, TempCount, 3);

		if(TempMoney != -1)
		{
			TempBuffer[TempUnitCount].siSlot  = TempCount;
			TempBuffer[TempUnitCount].siMoney = TempMoney;

			TempUnitCount++;
		}
	}

	// Message����
	pPlayer->SendFieldMsg(ON_RESPONSE_GBARRACK_GETDISMISSAL_GENERAL_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(TempUnitCount), LPVOID(TempBuffer));

	return TRUE;
}

SI32	SOVillage_Barrack::GBarrackSellDismissalGeneralEx(SOPlayer *pPlayer, SI32 Slot)
{
	MONEY				TempMoney;
	MONEY				iMoney;
	MONEY				TempEmployProfit;
	int					siReturn;
	
	// �ذ��� �뺴�� �ȼ� �ִ��� �˻�
	if(Slot == 0)                                         return FALSE;
	if(pPlayer->IFollower.IsValidFollower(Slot) == FALSE) return FALSE;

	// ���� ���
	// siGeneral(�����) : �Ϲݿ뺴(0), ���(1), ����(2), 2�����(3)
	TempMoney = GBarrackDismissalSolderMoney(pPlayer, Slot, 3);
	if(TempMoney == -1) return FALSE;

	// ���� �ø���.
	TempMoney = pPlayer->IncreaseMoney(TempMoney, FALSE);
	iMoney    = pPlayer->siMoney;

	// �뺴 ������� 10%�� ���� �ڻ����� ������.
	TempEmployProfit = m_lpVillage->IncreaseCapital(MONEY(TempMoney*ON_CAPITAL_RATE_EMPLOY),ON_PROFIT_EMPLOY, FALSE);

	// Slot�� �����Ѵ�.
	DBIKDeleteMercenary		TempDBIKDeleteMercenary;

	siReturn = 0;
	TempDBIKDeleteMercenary.DBAccount             = pPlayer->stAccountInfo.siDBAccount;
	TempDBIKDeleteMercenary.uiSlot                = pPlayer->stAccountInfo.uiSlot;
	TempDBIKDeleteMercenary.siDeleteMercenarySlot = Slot;
	TempDBIKDeleteMercenary.siVillageCode         = m_lpVillage->uiVillageCode;
	TempDBIKDeleteMercenary.siMoney               = TempMoney;
	TempDBIKDeleteMercenary.siEmployProfit        = TempEmployProfit;
	m_lpVillage->clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_DELETEMERCENARY, LPVOID(&TempDBIKDeleteMercenary), sizeof(TempDBIKDeleteMercenary));
	pPlayer->IFollower.DeleteFollower(Slot);

	pPlayer->SendFieldMsg(ON_RESPONSE_GBARRACK_DISMISSAL_GENERAL, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(&iMoney), LPVOID(Slot));

	// �ذ�� ���̴� �뺴�� ����
	if(pPlayer->siVMercenarySlot == Slot)
	{
		pPlayer->RecvVMercenaryChange(0);
	}

	return TRUE;

}

SI32	SOVillage_Barrack::GBarrackGetChangeGeneralExBonus(SOPlayer *pPlayer, SI32 Slot)
{
	SI32		TempTotalBonus;

	TempTotalBonus =  0;
	TempTotalBonus += pPlayer->IFollower.GetFollowerBaseParameterSTR(Slot);
	TempTotalBonus += pPlayer->IFollower.GetFollowerBaseParameterDEX(Slot);
	TempTotalBonus += pPlayer->IFollower.GetFollowerBaseParameterVIT(Slot);
	TempTotalBonus += pPlayer->IFollower.GetFollowerBaseParameterINT(Slot);
	TempTotalBonus /= 10;

	return TempTotalBonus;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Name : 
// Desc : ���üҿ��� ��� ���� ����Ʈ�� �����ش�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL SOVillage_Barrack::MBarrackGetMonsterList( SOPlayer *pPlayer )
{
	OnInnBuyMonsterInfo		TempBuffer[32];
	SI32					TempCount;
	SI32					TempCount1;
	SI32					TempUnitCount;
	CharHead*				lpCharHead;
	SolderListHeader*		lpSolderListHeader;
	SI32					TempDiscountPercent;

	TempDiscountPercent = 0;
	memset(TempBuffer, 0, sizeof(OnInnBuyMonsterInfo)*32);
	
	// ����Ҽ� �ִ� ���� ����� �����Ѵ�.
 	lpSolderListHeader = m_lpVillage->clWorld->clSolderListParser.GetSolderList( STRUCTCODE_MBARRACK, m_lpVillage->GetVillageState() );
	TempUnitCount      = 0;
	if( lpSolderListHeader )
	{
		for( TempCount = 0; TempCount < m_lpVillage->clWorld->clCharParser.GetCharTotalNum(); TempCount++ )
		{
			lpCharHead = m_lpVillage->clWorld->clCharParser.GetCharHeader( TempCount );

			for( TempCount1 = 0; TempCount1 < MAX_SOLDER; TempCount1++ )
			{
				if( lpSolderListHeader->siSellSolder[TempCount1] == lpCharHead->uiID )
				{
					TempBuffer[TempUnitCount].siCode               = lpCharHead->uiID;
					TempBuffer[TempUnitCount].siLevel              = lpCharHead->siNeedLv;
					TempBuffer[TempUnitCount].siCredit             = lpCharHead->siNeedCredit;
					TempBuffer[TempUnitCount].uiCharNameAddOption  = 0;
					TempBuffer[TempUnitCount].siMoney              = lpCharHead->siCost - ((lpCharHead->siCost * TempDiscountPercent) / 100);
					TempBuffer[TempUnitCount].siSealingBeadNum	   = GetRequiredSealingBeadNum( lpCharHead->siNeedLv );

					TempBuffer[TempUnitCount].SolderInfo.uiKind    = lpCharHead->uiID;
					TempBuffer[TempUnitCount].SolderInfo.uiItemNum = 0;
					TempBuffer[TempUnitCount].SolderInfo.siExp     = 0;
					TempBuffer[TempUnitCount].SolderInfo.siStr     = lpCharHead->Parameta.GetStr();
					TempBuffer[TempUnitCount].SolderInfo.siDex     = lpCharHead->Parameta.GetDex();
					TempBuffer[TempUnitCount].SolderInfo.siVit     = lpCharHead->Parameta.GetVit();
					TempBuffer[TempUnitCount].SolderInfo.siInt     = lpCharHead->Parameta.GetInt();
					TempBuffer[TempUnitCount].SolderInfo.siLife    = lpCharHead->Parameta.GetLife();
					TempBuffer[TempUnitCount].SolderInfo.siMana    = lpCharHead->siManaMax;

					TempUnitCount++;

					TempCount1 = MAX_SOLDER;
					break;
				}
			}
		}
	}

	// Message����
	pPlayer->SendFieldMsg( ON_RESPONSE_MBARRACK_GETMONSTERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(TempUnitCount), LPVOID(TempBuffer), LPVOID(TempDiscountPercent) );

	return TRUE;
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Name : 
// Desc : ���üҿ��� ���͸� ����Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL SOVillage_Barrack::MBarrackBuyMonster( SOPlayer *pPlayer, SI32 Code, BYTE* lpName )
{
	BOOL				fFlag;
	MONEY				iMoney;
	MONEY				TempCost;
	MONEY				TempEmployPorfit;
	SI32				siSlot;
	SI32				TempCount;
	SI32				TempCount1;
	OnFollowerInfo		TempOnFollowerInfo;
	CharHead*			lpCharHead;
	CharHead*			lpCharHead1;
	CharHead*			lpCharHead2;
	BYTE				TempBuffer[256];
//	int					siReturn;
	SI32				TempDiscountPercent;
	SI32				TempNowFollower;
	SI32				siMonsterCount;
	CItemHeader			*pItemHeader;
	MyItemData			stMyItemData;
	BOOL				bBeadFound		=	FALSE;
	SI32				siRequiredSealingBeadNum;

	TempDiscountPercent = 0;
	memset(TempBuffer, 0, sizeof(TempBuffer));

	// ����Ҽ� �ִ� �������� �˻��Ѵ�.
	fFlag = FALSE;
	for( TempCount = 0; TempCount < m_lpVillage->clWorld->clCharParser.GetCharTotalNum(); TempCount++ )
	{
		lpCharHead = m_lpVillage->clWorld->clCharParser.GetCharHeader(TempCount);

		if(lpCharHead->uiID == Code)
		{
			fFlag       = TRUE;
			lpCharHead1 = lpCharHead;
			break;
		}
	}

	// ���尡���� Slot�� �˻��Ѵ�.
	TempNowFollower			=	0;
	siMonsterCount			=	0;
	if( fFlag )
	{
		fFlag	=	FALSE;
		siSlot	=	-1;
		for( TempCount = ON_MAX_FOLLOWER_NUM-1; TempCount > 0; TempCount-- )
		{
			if( pPlayer->IFollower.IsValidFollower(TempCount) == FALSE )
			{
				fFlag	=	TRUE;
				siSlot	=	TempCount;
			}
			else
			{
				TempNowFollower++;

				// �ش� ���Կ� �̹� ���� ���Ͱ� �ִ��� �˻�
				/*
				if( pPlayer->IFollower.GetFollowerParameterKind(TempCount) == lpCharHead1->uiID )
				{
					fFlag = FALSE;
					break;
				}
				*/

				// �̹� ������� ���� ���� ī��Ʈ
				for( TempCount1 = 0; TempCount1 < m_lpVillage->clWorld->clCharParser.GetCharTotalNum(); TempCount1++ )
				{
					lpCharHead2 = m_lpVillage->clWorld->clCharParser.GetCharHeader( TempCount1 );

					if( lpCharHead2->uiID == pPlayer->IFollower.GetFollowerParameterKind(TempCount) )
					{
						if( lpCharHead2->siGeneral == 2 )
						{
							siMonsterCount++;
						}
						break;
					}
				}
			}
		}
	}

	// ���� �ɷ�ġ�� �� �̻��� ���͸� ����� �� �ִ��� �˻�
	if( fFlag )
	{
		if( TempNowFollower >= m_lpVillage->clWorld->clMercenaryParser.GetMaxFollowerNum(pPlayer->GetTradeGrade(), pPlayer->IFollower.GetFollowerParameterLevel(0)) )
		{
			fFlag	=	FALSE;
		}
	}

	// ��� ������ ���ͼ��� �ʰ��ϴ��� �˻�
	if( siMonsterCount >= ON_MAXMONSTER )
	{
		fFlag	=	FALSE;
	}

	// ����� �� �ִ� �����̳� �ſ뵵�� �ִ��� �˻�
	if( fFlag )
	{
		if( (pPlayer->IFollower.GetFollowerParameterLevel(0) < lpCharHead1->siNeedLv) && (pPlayer->GetTradeGrade() < lpCharHead1->siNeedCredit) )
		{
			fFlag	=	FALSE;
		}
	}

	// ������ ������ ������ �ִ��� �˻�
	if( fFlag )
	{
		// �÷��̾ "������ ����" �Ӽ��� ���� �������� �����ϰ� �ִ��� �˻��Ͽ� ������ �� ���̵� ���´�.
		for( int i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++ )
		{
			pPlayer->IFollower.GetItem( 0, i, &stMyItemData );								// ���ΰ� �뺴�� i��° �������� ���´�.
			pItemHeader		=	m_lpVillage->clWorld->clItem.GetItem( stMyItemData.uiID );	// ������ ID�� ������ ������ ���´�.

			if( pItemHeader )
			{
				if( (pItemHeader->m_siGroupCode & ON_ITEM_SPECIAL) && (pItemHeader->m_siSpecial == ON_ITEM_SPECIAL_SEALINGBEAD) )
				{
					bBeadFound	=	TRUE;
					break;
				}
			}
		}

		// ������ ������ ã�� ���߰ų� ������ �����ϸ� ���� ��� ����		
		siRequiredSealingBeadNum	=	GetRequiredSealingBeadNum( lpCharHead1->siNeedLv );
		if( !bBeadFound || stMyItemData.uiQuantity < siRequiredSealingBeadNum )
		{			
			fFlag	=	FALSE;
		}
	}

	if( fFlag && (siSlot > 0) )
	{
		// ��� �Ҽ� ������ �� ������ ������ �����Ѵ�.
		TempCost = (__int64)lpCharHead1->siCost - (((__int64)lpCharHead1->siCost * TempDiscountPercent) / 100);
		if(((__int64)pPlayer->siMoney - TempCost) >= 0)
		{
			// ������ ������ �ʿ��� ������ŭ �÷��̾ ���� �������� �����Ѵ�.
			pPlayer->IFollower.DelItem( 0, stMyItemData.uiID, siRequiredSealingBeadNum, &stMyItemData.siPosInInventory );

			pPlayer->DecreaseMoney( TempCost, FALSE );
			// �뺴 ������� 10%�� ���� �ڻ����� ������.
			TempEmployPorfit = m_lpVillage->IncreaseCapital( MONEY(TempCost*ON_CAPITAL_RATE_EMPLOY),ON_PROFIT_EMPLOY, FALSE );
			iMoney = pPlayer->siMoney;

			// Player�� Data�� �� ���͸� �߰��Ѵ�.(DB�� ������� �Ѵ�.)
			ZeroMemory( &TempOnFollowerInfo, sizeof(TempOnFollowerInfo) );
			TempOnFollowerInfo.uiKind              = Code;
			TempOnFollowerInfo.uiItemNum           = 0;
			TempOnFollowerInfo.siLevel             = 1;
			TempOnFollowerInfo.siExp               = 0;
			TempOnFollowerInfo.siStr               = lpCharHead1->Parameta.GetStr();
			TempOnFollowerInfo.siDex               = lpCharHead1->Parameta.GetDex();
			TempOnFollowerInfo.siVit               = lpCharHead1->Parameta.GetVit();
			TempOnFollowerInfo.siInt               = lpCharHead1->Parameta.GetInt();
			TempOnFollowerInfo.siLife              = lpCharHead1->Parameta.GetLife();
			TempOnFollowerInfo.siMana              = lpCharHead1->siManaMax;
			TempOnFollowerInfo.siBonus             = 0;
			TempOnFollowerInfo.siLiftConstant      = lpCharHead1->siCon;
			TempOnFollowerInfo.siEquipMinDamage    = lpCharHead1->Parameta.EquipMinDamage;
			TempOnFollowerInfo.siEquipMaxDamage    = lpCharHead1->Parameta.EquipMaxDamage;
			TempOnFollowerInfo.siAC                = lpCharHead1->Parameta.GetAC();
			TempOnFollowerInfo.siDamageRegist      = lpCharHead1->Parameta.DamageResist;
			TempOnFollowerInfo.siMagicRegist       = lpCharHead1->Parameta.MagicResist;
			strncpy( (char*)TempOnFollowerInfo.Name, (char*)lpName, ON_ID_LENGTH );
			ZeroMemory( TempBuffer, 256 );
			CopyMemory( TempBuffer, lpName, ON_ID_LENGTH );

			DBIKBuyMonster			TempDBIKBuyMonster;
			
			pPlayer->IFollower.InitFollower( siSlot, &TempOnFollowerInfo, lpCharHead1->siNation, &m_lpVillage->clWorld->clItem );
			ZeroMemory( &TempDBIKBuyMonster, sizeof(DBIKBuyMonster) );
			TempDBIKBuyMonster.DBAccount		=	pPlayer->stAccountInfo.siDBAccount;
			TempDBIKBuyMonster.uiSlot			=	pPlayer->stAccountInfo.uiSlot;
			TempDBIKBuyMonster.uiMercenarySlot	=	siSlot;
			TempDBIKBuyMonster.siKind			=	TempOnFollowerInfo.uiKind;
			TempDBIKBuyMonster.uiVillageCode	=	m_lpVillage->uiVillageCode;
			TempDBIKBuyMonster.siMoney			=	TempCost;
			TempDBIKBuyMonster.siEmployProfit	=	TempEmployPorfit;
			TempDBIKBuyMonster.uiItemID			=	pItemHeader->m_uiID;
			TempDBIKBuyMonster.siItemQuantity	=	siRequiredSealingBeadNum;

			m_lpVillage->clWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_BUYMONSTER, LPVOID(&TempDBIKBuyMonster), sizeof(TempDBIKBuyMonster) );

			// Client�� �� ������ ������.
			pPlayer->SendFieldMsg( ON_RESPONSE_MBARRACK_BUYMONSTER, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(&iMoney), LPVOID(siSlot), LPVOID(&TempOnFollowerInfo) );
		}
		else
		{
			pPlayer->SendFieldMsg( ON_RESPONSE_MBARRACK_BUYMONSTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
		}
	}
	else
	{
		pPlayer->SendFieldMsg( ON_RESPONSE_MBARRACK_BUYMONSTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
	}

	return TRUE;
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Name : 
// Desc : ���üҿ��� �ذ� ���� ����Ʈ�� �����ش�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL SOVillage_Barrack::MBarrackGetDismissMonsterList( SOPlayer *pPlayer )
{
	// �뺴 �˻�
	OnDismissalSolderInfo	TempBuffer[32];
	SI32					TempCount;
	SI32					TempUnitCount;
	MONEY					TempMoney;

	memset(TempBuffer, 0, sizeof(OnDismissalSolderInfo)*32);

	// ����Ҽ� �ִ� �뺴�� ����� �����Ѵ�.
	TempUnitCount      = 0;
	for( TempCount = 1; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++ )
	{
		TempMoney = MBarrackDismissMonsterMoney( pPlayer, TempCount );

		if( TempMoney != -1 )
		{
			TempBuffer[TempUnitCount].siSlot  = TempCount;
			TempBuffer[TempUnitCount].siMoney = TempMoney;

			TempUnitCount++;
		}
	}

	// Message����
	pPlayer->SendFieldMsg( ON_RESPONSE_MBARRACK_GETDISMISSMONSTERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(TempUnitCount), LPVOID(TempBuffer) );

	return TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Name : 
// Desc : ���üҿ��� ���͸� �ذ��Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL SOVillage_Barrack::MBarrackDismissMonster( SOPlayer *pPlayer, SI32 Slot )
{
	MONEY				TempMoney;
	MONEY				iMoney;
	MONEY				TempEmployProfit;
	int					siReturn;
	
	// �ذ��� �뺴�� �ȼ� �ִ��� �˻�
	if( Slot == 0 )											return FALSE;
	if( pPlayer->IFollower.IsValidFollower(Slot) == FALSE )	return FALSE;

	// ���� ���
	TempMoney = MBarrackDismissMonsterMoney( pPlayer, Slot );
	if( TempMoney == -1 )	return FALSE;

	// ���� �ø���.
	TempMoney = pPlayer->IncreaseMoney( TempMoney, FALSE );
	iMoney    = pPlayer->siMoney;

	// �뺴 ������� 10%�� ���� �ڻ����� ������.
	TempEmployProfit = m_lpVillage->IncreaseCapital( MONEY(TempMoney*ON_CAPITAL_RATE_EMPLOY),ON_PROFIT_EMPLOY, FALSE );

	// Slot�� �����Ѵ�.
	DBIKDeleteMonster		TempDBIKDeleteMonster;

	siReturn = 0;
	TempDBIKDeleteMonster.DBAccount             = pPlayer->stAccountInfo.siDBAccount;
	TempDBIKDeleteMonster.uiSlot                = pPlayer->stAccountInfo.uiSlot;
	TempDBIKDeleteMonster.siDeleteMercenarySlot = Slot;
	TempDBIKDeleteMonster.siVillageCode         = m_lpVillage->uiVillageCode;
	TempDBIKDeleteMonster.siMoney               = TempMoney;
	TempDBIKDeleteMonster.siEmployProfit        = TempEmployProfit;
	m_lpVillage->clWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_DELETEMONSTER, LPVOID(&TempDBIKDeleteMonster), sizeof(TempDBIKDeleteMonster) );
	pPlayer->IFollower.DeleteFollower(Slot);

	pPlayer->SendFieldMsg( ON_RESPONSE_MBARRACK_DISMISSMONSTER, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(&iMoney), LPVOID(Slot) );

	// �ذ�� ���̴� �뺴�� ����
	if( pPlayer->siVMercenarySlot == Slot )
	{
		pPlayer->RecvVMercenaryChange(0);
	}

	return TRUE;
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Name : 
// Desc : ���üҿ��� �ذ��� ������ ������ ����.
//--------------------------------------------------------------------------------------------------------------------------------------------------------
MONEY SOVillage_Barrack::MBarrackDismissMonsterMoney( SOPlayer *pPlayer, SI32 Slot )
{
	MONEY				TempBuyMoney;
	CharHead*			lpCharHead;
	int					TempCount;

	if( Slot == 0 )											return -1;
	if( pPlayer->IFollower.IsValidFollower(Slot) == FALSE )	return -1;
	
	lpCharHead = NULL;
	for( TempCount = 0; TempCount < m_lpVillage->clWorld->clCharParser.GetCharTotalNum(); TempCount++ )
	{
		lpCharHead = m_lpVillage->clWorld->clCharParser.GetCharHeader( TempCount );

		if( lpCharHead->uiID == pPlayer->IFollower.GetFollowerCharID(Slot) )
		{
			lpCharHead = lpCharHead;
			break;
		}
	}

	// ���� ���Ͱ� �ƴϸ� ����.. �Ϲݿ뺴(0), ���(1), ����(2)
	if( lpCharHead->siGeneral != 2 )	return -1;

	TempBuyMoney = (MONEY)lpCharHead->siCost;
	TempBuyMoney = ( ((TempBuyMoney + ((pPlayer->IFollower.GetFollowerParameterLevel(Slot) - 1) * (TempBuyMoney / 10)))) / 3 );

	return TempBuyMoney;
}

SI32 SOVillage_Barrack::GetRequiredSealingBeadNum( SI16 siNeedLv )
{
	// ������ ���� ���� = ���� ��� ���� ���� / 10 (�Ҽ��� ���� �ø�)
	return	(SI32)( (siNeedLv / 10) + 1 );
}

