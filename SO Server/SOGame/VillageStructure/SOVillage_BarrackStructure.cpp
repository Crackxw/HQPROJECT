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
	// 만약 같은 길드이면
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
			
	// 고용할수 있는 용병의 목록을 저장한다.
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

	// Message전송
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
	// 만약 같은 길드이면
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

	// 고용할수 있는 용병인지 검사한다.
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

	// 저장가능한 Slot을 검사한다.
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

	// 현재 능력치로 용병을 더 이상 용병을 고용할수 있는지 검사
	if(fFlag)
	{
		if(TempNowFollower >= m_lpVillage->clWorld->clMercenaryParser.GetMaxFollowerNum(pPlayer->GetTradeGrade(), pPlayer->IFollower.GetFollowerParameterLevel(0)))
		{
			fFlag = FALSE;
		}
	}

	// 고용할수 있는 레벨이나 신용도가 있는지 검사
	if(fFlag)
	{
		if((pPlayer->IFollower.GetFollowerParameterLevel(0) < lpCharHead1->siNeedLv) && (pPlayer->GetTradeGrade() < lpCharHead1->siNeedCredit))
		{
			fFlag = FALSE;
		}
	}

	if(fFlag && (siSlot > 0))
	{
		// 고용 할수 있으면 그 짐꾼의 정보를 저장한다.
		TempCost = (__int64)lpCharHead1->siCost - (((__int64)lpCharHead1->siCost * TempDiscountPercent) / 100);
		if(((__int64)pPlayer->siMoney - TempCost) >= 0)
		{
			pPlayer->DecreaseMoney(TempCost, FALSE);
			// 용병 매출액의 10%를 마을 자산으로 돌린다.
			TempEmployPorfit = m_lpVillage->IncreaseCapital(MONEY(TempCost*ON_CAPITAL_RATE_EMPLOY),ON_PROFIT_EMPLOY, FALSE);
			iMoney = pPlayer->siMoney;

			// Player의 Data에 그 짐꾼을 추가한다.(DB에 저장까지 한다.)
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

			// Client에 그 정보를 보낸다.
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
	// 용병 검사
	OnDismissalSolderInfo	TempBuffer[128];
	SI32					TempCount;
	SI32					TempUnitCount;
	MONEY					TempMoney;

	memset(TempBuffer, 0, sizeof(OnDismissalSolderInfo)*128);

	// 고용할수 있는 용병의 목록을 저장한다.
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

	// Message전송
	pPlayer->SendFieldMsg(ON_RESPONSE_BARRACK_GETDISMISSALSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(TempUnitCount), LPVOID(TempBuffer));

	return TRUE;
}

BOOL SOVillage_Barrack::BarrackDismissalSolder(SOPlayer *pPlayer, SI32 Slot)
{
	MONEY				TempMoney;
	MONEY				iMoney;
	MONEY				TempEmployProfit;
	int					siReturn;
	
	// 해고할 용병을 팔수 있는지 검사
	if(Slot == 0)                                         return FALSE;
	if(pPlayer->IFollower.IsValidFollower(Slot) == FALSE) return FALSE;

	// 가격 계산
	TempMoney = BarrackDismissalSolderMoney(pPlayer, Slot);
	if(TempMoney == -1) return FALSE;

	// 돈을 올린다.
	TempMoney = pPlayer->IncreaseMoney(TempMoney, FALSE);
	iMoney    = pPlayer->siMoney;

	// 용병 매출액의 10%를 마을 자산으로 돌린다.
	TempEmployProfit = m_lpVillage->IncreaseCapital(MONEY(TempMoney*ON_CAPITAL_RATE_EMPLOY),ON_PROFIT_EMPLOY, FALSE);

	// Slot을 삭제한다.
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

	// 해고시 보이는 용병도 삭제
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

	// 만약 일반용병이 아닌 장수이거나 몬스터 용병이면 실패
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

	// 고용할수 있는 용병의 목록을 저장한다.
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

	// Message전송
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

	// 고용할수 있는 용병인지 검사한다.
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

	// 저장가능한 Slot을 검사한다.
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

				// 이미 같은 장수가 있는지 검사
				if(pPlayer->IFollower.GetFollowerParameterKind(TempCount) == lpCharHead1->uiID)
				{
					fFlag = FALSE;
					break;
				}

				// 장수인지 검사
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

	// 현재 능력치로 용병을 더 이상 용병을 고용할수 있는지 검사
	if(fFlag)
	{
		if(TempNowFollower >= m_lpVillage->clWorld->clMercenaryParser.GetMaxFollowerNum(pPlayer->GetTradeGrade(), pPlayer->IFollower.GetFollowerParameterLevel(0)))
		{
			fFlag = FALSE;
		}
	}

	// 고용 가능한 장수수 검사
	if(TempNowGeneralFollower + TempNowGeneralExFollower >= pPlayer->m_siMaxGeneral)
	{
		fFlag = FALSE;
	}

	// 고용할수 있는 레벨이나 신용도가 있는지 검사
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
		// 고용 할수 있으면 그 짐꾼의 정보를 저장한다.
		TempCost = (__int64)lpCharHead1->siCost - (((__int64)lpCharHead1->siCost * TempDiscountPercent) / 100);
		if(((__int64)pPlayer->siMoney - TempCost) >= 0)
		{
			pPlayer->DecreaseMoney(TempCost, FALSE);
			// 용병 매출액의 10%를 마을 자산으로 돌린다.
			TempEmployPorfit = m_lpVillage->IncreaseCapital(MONEY(TempCost*ON_CAPITAL_RATE_EMPLOY),ON_PROFIT_EMPLOY, FALSE);
			iMoney = pPlayer->siMoney;

			// Player의 Data에 그 짐꾼을 추가한다.(DB에 저장까지 한다.)
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

			// Client에 그 정보를 보낸다.
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
				// 플레이어가 용병중에 장수로 바꿀 수 있는 용병이 있는지를 검사한다.
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
			// 이전에 찾은게 있는지 검사
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

			// 이전에 꺼를 찾지 못하면 새로 추가
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
	
	// 고용할수 있는 용병의 목록을 저장한다.
	lpSolderListHeader = m_lpVillage->clWorld->clSolderListParser.GetSolderList(STRUCTCODE_GBARRACK, m_lpVillage->GetVillageState());
	TempUnitCount      = 0;
	ZeroMemory(TempBuffer, sizeof(OnChangeJobSolderInfo) * 32);
	if(lpSolderListHeader)
	{
		for(TempCount = 1; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
		{
			if(pPlayer->IFollower.IsValidFollower(TempCount) == TRUE)
			{
				// 각 용병을 장수로 바꿜수 있는지 검사
				// m_lpVillage->clWorld->clGeneralParser.siGeneralNum generalList에 있는 총 갯수.
				for(TempCount1 = 0; TempCount1 < m_lpVillage->clWorld->clGeneralParser.siGeneralNum; TempCount1++)
				{
					// 장수로 변화 가능한 용병이 있는지를 검사한다.
					// siGeneralBaseCharacterCode 장수로 변하기전의 용병 Code
					if(m_lpVillage->clWorld->clGeneralParser.pGeneralHeader[TempCount1].siGeneralBaseCharacterCode 
						== pPlayer->IFollower.GetFollowerParameterKind(TempCount))
					{
						//MAX_SOLDER 12
						for(TempCount4 = 0; TempCount4 < MAX_SOLDER; TempCount4++)
						{
							//solderlist 파서에 등록되어 있는 용병인지 검사.
							if(lpSolderListHeader->siSellSolder[TempCount4] == 
								m_lpVillage->clWorld->clGeneralParser.pGeneralHeader[TempCount1].siGeneralCharacterCode)
							{
								// 이전에 찾은게 있는지 검사
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

								// 이전에 꺼를 찾지 못하면 새로 추가
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

	// Message전송
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

	// 장수 정보 검사
	lpGeneralData = NULL;
	lpCharHead1   = NULL;
	for(TempCount = 0; TempCount < m_lpVillage->clWorld->clGeneralParser.siGeneralNum; TempCount++)
	{
		//선택한 전직하게 될 용병이 pGeneralHeader[TempCount].siGeneralCharacterCode 와 일치 하는지 검사.
		if(m_lpVillage->clWorld->clGeneralParser.pGeneralHeader[TempCount].siGeneralCharacterCode == Kind)
		{
			//플레이어가 전직할 용병을 가지고 있는가.
			if(m_lpVillage->clWorld->clGeneralParser.pGeneralHeader[TempCount].siGeneralBaseCharacterCode == pPlayer->IFollower.GetFollowerParameterKind(Slot))
			{
				fFlag         = TRUE;
				lpGeneralData = &m_lpVillage->clWorld->clGeneralParser.pGeneralHeader[TempCount];
				break;
			}
		}
	}

	// 우선 그 용병이 교체가능한지 검사
	if(fFlag)
	{
		if(GBarrack_CheckChangeJob(lpGeneralData, pPlayer, Slot) == FALSE)
		{
			fFlag = FALSE;
		}
	}


	// 같은 장수를 데리고 있는지 검사
	TempNowGeneralFollower = 0;
	TempNowGeneralExFollower = 0;
	if(fFlag)
	{
		for(TempCount = ON_MAX_FOLLOWER_NUM-1; TempCount > 0; TempCount--)
		{
			if(pPlayer->IFollower.IsValidFollower(TempCount))
			{
				// 이미 같은 장수가 있는지 검사
				if(pPlayer->IFollower.GetFollowerParameterKind(TempCount) == Kind)
				{
					fFlag = FALSE;
					break;
				}

				// 장수인지 검사
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

	// 가질수 있는 장수수를 초과하는지 검사
	if(fFlag)
	{
		if(TempNowGeneralFollower + TempNowGeneralExFollower >= pPlayer->m_siMaxGeneral)
		{
			fFlag = FALSE;
		}
	}

	// 용병정보 검사
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

	// 전직
	if(fFlag && (Slot > 0))
	{
		iMoney = 0;

		// Player의 Data에 그 짐꾼을 추가한다.(DB에 저장까지 한다.)
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

		// Client에 그 정보를 보낸다.
		pPlayer->SendFieldMsg(ON_RESPONSE_GBARRACK_CHANGEJOBSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(&iMoney), LPVOID(Slot), LPVOID(&TempFollowerInfo));
	}
	else
	{
		// 전직 실패시
		pPlayer->SendFieldMsg(ON_RESPONSE_GBARRACK_CHANGEJOBSOLDER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}

	return TRUE;
}

BOOL SOVillage_Barrack::GBarrackGetDismissalSolderList(SOPlayer *pPlayer)
{
	// 용병 검사
	OnDismissalSolderInfo	TempBuffer[32];
	SI32					TempCount;
	SI32					TempUnitCount;
	MONEY					TempMoney;

	memset(TempBuffer, 0, sizeof(OnDismissalSolderInfo)*32);

	// 고용할수 있는 용병의 목록을 저장한다.
	TempUnitCount      = 0;
	for(TempCount = 1; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
	{
		// siGeneral(장수값) : 일반용병(0), 장수(1), 몬스터(2), 2차장수(3)
		TempMoney = GBarrackDismissalSolderMoney(pPlayer, TempCount,1);

		if(TempMoney != -1)
		{
			TempBuffer[TempUnitCount].siSlot  = TempCount;
			TempBuffer[TempUnitCount].siMoney = TempMoney;

			TempUnitCount++;
		}
	}

	// Message전송
	pPlayer->SendFieldMsg(ON_RESPONSE_GBARRACK_GETDISMISSALSOLDERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(TempUnitCount), LPVOID(TempBuffer));

	return TRUE;
}

BOOL SOVillage_Barrack::GBarrackDismissalSolder(SOPlayer *pPlayer, SI32 Slot)
{
	MONEY				TempMoney;
	MONEY				iMoney;
	MONEY				TempEmployProfit;
	int					siReturn;
	
	// 해고할 용병을 팔수 있는지 검사
	if(Slot == 0)                                         return FALSE;
	if(pPlayer->IFollower.IsValidFollower(Slot) == FALSE) return FALSE;

	// 가격 계산
	// siGeneral(장수값) : 일반용병(0), 장수(1), 몬스터(2), 2차장수(3)
	TempMoney = GBarrackDismissalSolderMoney(pPlayer, Slot,1);
	if(TempMoney == -1) return FALSE;

	// 돈을 올린다.
	TempMoney = pPlayer->IncreaseMoney(TempMoney, FALSE);
	iMoney    = pPlayer->siMoney;

	// 용병 매출액의 10%를 마을 자산으로 돌린다.
	TempEmployProfit = m_lpVillage->IncreaseCapital(MONEY(TempMoney*ON_CAPITAL_RATE_EMPLOY),ON_PROFIT_EMPLOY, FALSE);

	// Slot을 삭제한다.
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

	// 해고시 보이는 용병도 삭제
	if(pPlayer->siVMercenarySlot == Slot)
	{
		pPlayer->RecvVMercenaryChange(0);
	}

	return TRUE;
}

//Generallist GeneralExlist 사용.
BOOL SOVillage_Barrack::GBarrack_CheckChangeJob(GeneralHeader* lpGeneralData, SOPlayer *pPlayer, SI32 Slot)
{
	if((Slot <= 0) || (Slot >= ON_MAX_FOLLOWER_NUM)) return FALSE;

	// 국가 코드 검사
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

	// 용병의 레벨이 돼는지 검사
	if(pPlayer->IFollower.GetFollowerParameterLevel(Slot) < lpGeneralData->siGeneralChangeNeedCharacterLv) return FALSE;

	// 변환가능한 직업인지
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

	// 만약 장수가 아니면 실패
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
	
	// 고용할수 있는 용병의 목록을 저장한다.
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

	// Message전송
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
	SI32				TempNowGeneralFollower;				//1차 장수
	SI32				TempNowGeneralExFollower;			//2차 장수

	TempDiscountPercent = 0;
	lpCharHead1			= NULL;

	memset(TempBuffer, 0, sizeof(TempBuffer));

	// 고용할수 있는 용병인지 검사한다.
	// Charlist 에 있는 용병인가.
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

	// 저장가능한 Slot을 검사한다.
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

				// 이미 같은 장수가 있는지 검사
				if(pPlayer->IFollower.GetFollowerParameterKind(TempCount) == lpCharHead1->uiID)
				{
					fFlag = FALSE;
					break;
				}
				
				// 일반 장수와 2차 전직 장수 중 한개의 캐릭터만 가지고 있을수 있다.

				// 장수인지 검사
				for(TempCount1 = 0; TempCount1 < m_lpVillage->clWorld->clCharParser.GetCharTotalNum(); TempCount1++)
				{
					lpCharHead2 = m_lpVillage->clWorld->clCharParser.GetCharHeader(TempCount1);

					if(lpCharHead2->uiID == pPlayer->IFollower.GetFollowerParameterKind(TempCount))
					{
						if( lpCharHead2->siGeneral == 1 )
						{
							TempNowGeneralFollower++;
						}
						//2차 장수의 수도 더해준다.
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
	// 현재 능력치로 용병을 더 이상 용병을 고용할수 있는지 검사
	if(fFlag)
	{
		if(TempNowGeneralFollower + TempNowGeneralExFollower >= m_lpVillage->clWorld->clMercenaryParser.GetMaxFollowerNum(pPlayer->GetTradeGrade(), pPlayer->IFollower.GetFollowerParameterLevel(0)))
		{
			fFlag = FALSE;
		}
	}

	// 고용 가능한 장수수 검사
	// pPlayer->m_siMaxGeneral DB에 저장되어 있는 고용 가능 장수 정보.
	// 고용 가능한 2차 장수의 수.
	if(TempNowGeneralFollower + TempNowGeneralExFollower >= pPlayer->m_siMaxGeneral)
	{
		fFlag = FALSE;
	}

	// 고용할수 있는 레벨이나 신용도가 있는지 검사
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
		// 고용 할수 있으면 그 짐꾼의 정보를 저장한다.
		TempCost = (__int64)lpCharHead1->siCost - (((__int64)lpCharHead1->siCost * TempDiscountPercent) / 100);
		if(((__int64)pPlayer->siMoney - TempCost) >= 0)
		{
			pPlayer->DecreaseMoney(TempCost, FALSE);
			// 용병 매출액의 10%를 마을 자산으로 돌린다.
			TempEmployPorfit = m_lpVillage->IncreaseCapital(MONEY(TempCost*ON_CAPITAL_RATE_EMPLOY),ON_PROFIT_EMPLOY, FALSE);
			iMoney = pPlayer->siMoney;

			// Player의 Data에 그 짐꾼을 추가한다.(DB에 저장까지 한다.)
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

			// Client에 그 정보를 보낸다.
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
	
	// 고용할수 있는 용병의 목록을 저장한다.
	//lpSolderListHeader solder.txt에서 각 마을의 건물에서 팔수 있는 요병을 가져온다. 
	lpSolderListHeader = m_lpVillage->clWorld->clSolderListParser.GetSolderList(STRUCTCODE_GEXBARRACK, m_lpVillage->GetVillageState());
	TempUnitCount      = 0;
	ZeroMemory(TempBuffer, sizeof(OnChangeJobSolderInfo) * 32);
	if(lpSolderListHeader)
	{
		for(TempCount = 1; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
		{
			if(pPlayer->IFollower.IsValidFollower(TempCount) == TRUE)
			{
				// 각 용병을 장수로 바꿜수 있는지 검사
				// m_lpVillage->clWorld->clGeneralExParser.siGeneralNum generalList에 있는 총 갯수.
				for(TempCount1 = 0; TempCount1 < m_lpVillage->clWorld->clGeneralExParser.siGeneralNum; TempCount1++)
				{
					// 장수로 변화 가능한 용병이 있는지를 검사한다.
					// siGeneralBaseCharacterCode 장수로 변하기전의 용병 Code
					if(m_lpVillage->clWorld->clGeneralExParser.pGeneralHeader[TempCount1].siGeneralBaseCharacterCode 
						== pPlayer->IFollower.GetFollowerParameterKind(TempCount))
					{
						//MAX_SOLDER 12
						for(TempCount4 = 0; TempCount4 < MAX_SOLDER; TempCount4++)
						{
							//solderlist 파서에 등록되어 있는 용병인지 검사.
							//pGeneralHeader[TempCount1].siGeneralCharacterCode 2차 장수로 전직하면 갖게 되는 ID.
							if(lpSolderListHeader->siSellSolder[TempCount4] == 
								m_lpVillage->clWorld->clGeneralExParser.pGeneralHeader[TempCount1].siGeneralCharacterCode)
							{
								// 이전에 찾은게 있는지 검사
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

								// 이전에 꺼를 찾지 못하면 새로 추가
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

	// Message전송
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

	// 장수 정보 검사
	lpGeneralData = NULL;
	for(TempCount = 0; TempCount < m_lpVillage->clWorld->clGeneralExParser.siGeneralNum; TempCount++)
	{
		//선택한 전직하게 될 용병이 pGeneralHeader[TempCount].siGeneralCharacterCode 와 일치 하는지 검사.
		if(m_lpVillage->clWorld->clGeneralExParser.pGeneralHeader[TempCount].siGeneralCharacterCode == Kind)
		{
			//플레이어가 전직할 용병을 가지고 있는가.
			if(m_lpVillage->clWorld->clGeneralExParser.pGeneralHeader[TempCount].siGeneralBaseCharacterCode == pPlayer->IFollower.GetFollowerParameterKind(Slot))
			{
				fFlag         = TRUE;
				lpGeneralData = &m_lpVillage->clWorld->clGeneralExParser.pGeneralHeader[TempCount];
				break;
			}
		}
	}

	// 우선 그 용병이 교체가능한지 검사
	if(fFlag)
	{
		if(GBarrack_CheckChangeJob(lpGeneralData, pPlayer, Slot) == FALSE)
		{
			fFlag = FALSE;
		}
	}



	// 같은 장수를 데리고 있는지 검사
	TempNowGeneralFollower = 0;
	TempNowGeneralExFollower = 0;
	if(fFlag)
	{
		for(TempCount = ON_MAX_FOLLOWER_NUM-1; TempCount > 0; TempCount--)
		{
			if(pPlayer->IFollower.IsValidFollower(TempCount))
			{
				// 이미 같은 장수가 있는지 검사
				if(pPlayer->IFollower.GetFollowerParameterKind(TempCount) == Kind)
				{
					fFlag = FALSE;
					break;
				}

				// 장수인지 검사
				for(TempCount1 = 0; TempCount1 < m_lpVillage->clWorld->clCharParser.GetCharTotalNum(); TempCount1++)
				{
					lpCharHead2 = m_lpVillage->clWorld->clCharParser.GetCharHeader(TempCount1);

					if(lpCharHead2->uiID == pPlayer->IFollower.GetFollowerParameterKind(TempCount))
					{
						if( lpCharHead2->siGeneral == 1 )
						{
							TempNowGeneralFollower++;
						}
						//2차 전직 장수 인지 검사.
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

	
	//2차 장수는 2명이상 고용할수 없다.
/*	if(TempNowGeneralExFollower >= 2)
	{
		fFlag = FALSE;
	}
*/
	// 가질수 있는 장수수를 초과하는지 검사
/*	if(fFlag)
	{
		if(TempNowGeneralFollower + TempNowGeneralExFollower>= pPlayer->m_siMaxGeneral)
		{
			fFlag = FALSE;
		}
	}
*/

	// 용병정보 검사
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

	// 전직
	if(fFlag && (Slot > 0))
	{
		iMoney = 0;

		// Player의 Data에 그 짐꾼을 추가한다.(DB에 저장까지 한다.)
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

		// Client에 그 정보를 보낸다.
		pPlayer->SendFieldMsg(ON_RESPONSE_GBARRACK_CHANGE_GENERAL, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(&iMoney), LPVOID(Slot), LPVOID(&TempFollowerInfo));
	}
	else
	{
		// 전직 실패시
		pPlayer->SendFieldMsg(ON_RESPONSE_GBARRACK_CHANGE_GENERAL, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
	return TRUE;
}

BOOL	SOVillage_Barrack::GBarrackGetDismissalGeneralExList(SOPlayer *pPlayer)
{
	// 용병 검사
	OnDismissalSolderInfo	TempBuffer[32];
	SI32					TempCount;
	SI32					TempUnitCount;
	MONEY					TempMoney;

	memset(TempBuffer, 0, sizeof(OnDismissalSolderInfo)*32);
	TempUnitCount      = 0;
	for(TempCount = 1; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
	{
		// siGeneral(장수값) : 일반용병(0), 장수(1), 몬스터(2), 2차장수(3)
		TempMoney = GBarrackDismissalSolderMoney(pPlayer, TempCount, 3);

		if(TempMoney != -1)
		{
			TempBuffer[TempUnitCount].siSlot  = TempCount;
			TempBuffer[TempUnitCount].siMoney = TempMoney;

			TempUnitCount++;
		}
	}

	// Message전송
	pPlayer->SendFieldMsg(ON_RESPONSE_GBARRACK_GETDISMISSAL_GENERAL_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(TempUnitCount), LPVOID(TempBuffer));

	return TRUE;
}

SI32	SOVillage_Barrack::GBarrackSellDismissalGeneralEx(SOPlayer *pPlayer, SI32 Slot)
{
	MONEY				TempMoney;
	MONEY				iMoney;
	MONEY				TempEmployProfit;
	int					siReturn;
	
	// 해고할 용병을 팔수 있는지 검사
	if(Slot == 0)                                         return FALSE;
	if(pPlayer->IFollower.IsValidFollower(Slot) == FALSE) return FALSE;

	// 가격 계산
	// siGeneral(장수값) : 일반용병(0), 장수(1), 몬스터(2), 2차장수(3)
	TempMoney = GBarrackDismissalSolderMoney(pPlayer, Slot, 3);
	if(TempMoney == -1) return FALSE;

	// 돈을 올린다.
	TempMoney = pPlayer->IncreaseMoney(TempMoney, FALSE);
	iMoney    = pPlayer->siMoney;

	// 용병 매출액의 10%를 마을 자산으로 돌린다.
	TempEmployProfit = m_lpVillage->IncreaseCapital(MONEY(TempMoney*ON_CAPITAL_RATE_EMPLOY),ON_PROFIT_EMPLOY, FALSE);

	// Slot을 삭제한다.
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

	// 해고시 보이는 용병도 삭제
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
// Desc : 조련소에서 고용 몬스터 리스트를 보내준다.
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
	
	// 고용할수 있는 몬스터 목록을 저장한다.
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

	// Message전송
	pPlayer->SendFieldMsg( ON_RESPONSE_MBARRACK_GETMONSTERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(TempUnitCount), LPVOID(TempBuffer), LPVOID(TempDiscountPercent) );

	return TRUE;
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Name : 
// Desc : 조련소에서 몬스터를 고용한다.
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

	// 고용할수 있는 몬스터인지 검사한다.
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

	// 저장가능한 Slot을 검사한다.
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

				// 해당 슬롯에 이미 같은 몬스터가 있는지 검사
				/*
				if( pPlayer->IFollower.GetFollowerParameterKind(TempCount) == lpCharHead1->uiID )
				{
					fFlag = FALSE;
					break;
				}
				*/

				// 이미 고용중인 몬스터 수를 카운트
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

	// 현재 능력치로 더 이상의 몬스터를 고용할 수 있는지 검사
	if( fFlag )
	{
		if( TempNowFollower >= m_lpVillage->clWorld->clMercenaryParser.GetMaxFollowerNum(pPlayer->GetTradeGrade(), pPlayer->IFollower.GetFollowerParameterLevel(0)) )
		{
			fFlag	=	FALSE;
		}
	}

	// 고용 가능한 몬스터수를 초과하는지 검사
	if( siMonsterCount >= ON_MAXMONSTER )
	{
		fFlag	=	FALSE;
	}

	// 고용할 수 있는 레벨이나 신용도가 있는지 검사
	if( fFlag )
	{
		if( (pPlayer->IFollower.GetFollowerParameterLevel(0) < lpCharHead1->siNeedLv) && (pPlayer->GetTradeGrade() < lpCharHead1->siNeedCredit) )
		{
			fFlag	=	FALSE;
		}
	}

	// 봉인의 구슬을 가지고 있는지 검사
	if( fFlag )
	{
		// 플레이어가 "봉인의 구슬" 속성을 가진 아이템을 보유하고 있는지 검사하여 있으면 그 아이디를 얻어온다.
		for( int i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++ )
		{
			pPlayer->IFollower.GetItem( 0, i, &stMyItemData );								// 주인공 용병의 i번째 아이템을 얻어온다.
			pItemHeader		=	m_lpVillage->clWorld->clItem.GetItem( stMyItemData.uiID );	// 아이템 ID로 아이템 정보를 얻어온다.

			if( pItemHeader )
			{
				if( (pItemHeader->m_siGroupCode & ON_ITEM_SPECIAL) && (pItemHeader->m_siSpecial == ON_ITEM_SPECIAL_SEALINGBEAD) )
				{
					bBeadFound	=	TRUE;
					break;
				}
			}
		}

		// 봉인의 구슬을 찾지 못했거나 수량이 부족하면 몬스터 고용 실패		
		siRequiredSealingBeadNum	=	GetRequiredSealingBeadNum( lpCharHead1->siNeedLv );
		if( !bBeadFound || stMyItemData.uiQuantity < siRequiredSealingBeadNum )
		{			
			fFlag	=	FALSE;
		}
	}

	if( fFlag && (siSlot > 0) )
	{
		// 고용 할수 있으면 그 몬스터의 정보를 저장한다.
		TempCost = (__int64)lpCharHead1->siCost - (((__int64)lpCharHead1->siCost * TempDiscountPercent) / 100);
		if(((__int64)pPlayer->siMoney - TempCost) >= 0)
		{
			// 봉인의 구슬을 필요한 개수만큼 플레이어가 가진 수량에서 삭제한다.
			pPlayer->IFollower.DelItem( 0, stMyItemData.uiID, siRequiredSealingBeadNum, &stMyItemData.siPosInInventory );

			pPlayer->DecreaseMoney( TempCost, FALSE );
			// 용병 매출액의 10%를 마을 자산으로 돌린다.
			TempEmployPorfit = m_lpVillage->IncreaseCapital( MONEY(TempCost*ON_CAPITAL_RATE_EMPLOY),ON_PROFIT_EMPLOY, FALSE );
			iMoney = pPlayer->siMoney;

			// Player의 Data에 그 몬스터를 추가한다.(DB에 저장까지 한다.)
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

			// Client에 그 정보를 보낸다.
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
// Desc : 조련소에서 해고 몬스터 리스트를 보내준다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL SOVillage_Barrack::MBarrackGetDismissMonsterList( SOPlayer *pPlayer )
{
	// 용병 검사
	OnDismissalSolderInfo	TempBuffer[32];
	SI32					TempCount;
	SI32					TempUnitCount;
	MONEY					TempMoney;

	memset(TempBuffer, 0, sizeof(OnDismissalSolderInfo)*32);

	// 고용할수 있는 용병의 목록을 저장한다.
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

	// Message전송
	pPlayer->SendFieldMsg( ON_RESPONSE_MBARRACK_GETDISMISSMONSTERLIST, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(TempUnitCount), LPVOID(TempBuffer) );

	return TRUE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Name : 
// Desc : 조련소에서 몬스터를 해고한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL SOVillage_Barrack::MBarrackDismissMonster( SOPlayer *pPlayer, SI32 Slot )
{
	MONEY				TempMoney;
	MONEY				iMoney;
	MONEY				TempEmployProfit;
	int					siReturn;
	
	// 해고할 용병을 팔수 있는지 검사
	if( Slot == 0 )											return FALSE;
	if( pPlayer->IFollower.IsValidFollower(Slot) == FALSE )	return FALSE;

	// 가격 계산
	TempMoney = MBarrackDismissMonsterMoney( pPlayer, Slot );
	if( TempMoney == -1 )	return FALSE;

	// 돈을 올린다.
	TempMoney = pPlayer->IncreaseMoney( TempMoney, FALSE );
	iMoney    = pPlayer->siMoney;

	// 용병 매출액의 10%를 마을 자산으로 돌린다.
	TempEmployProfit = m_lpVillage->IncreaseCapital( MONEY(TempMoney*ON_CAPITAL_RATE_EMPLOY),ON_PROFIT_EMPLOY, FALSE );

	// Slot을 삭제한다.
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

	// 해고시 보이는 용병도 삭제
	if( pPlayer->siVMercenarySlot == Slot )
	{
		pPlayer->RecvVMercenaryChange(0);
	}

	return TRUE;
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Name : 
// Desc : 조련소에서 해고할 몬스터의 가격을 얻어낸다.
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

	// 만약 몬스터가 아니면 실패.. 일반용병(0), 장수(1), 몬스터(2)
	if( lpCharHead->siGeneral != 2 )	return -1;

	TempBuyMoney = (MONEY)lpCharHead->siCost;
	TempBuyMoney = ( ((TempBuyMoney + ((pPlayer->IFollower.GetFollowerParameterLevel(Slot) - 1) * (TempBuyMoney / 10)))) / 3 );

	return TempBuyMoney;
}

SI32 SOVillage_Barrack::GetRequiredSealingBeadNum( SI16 siNeedLv )
{
	// 봉인의 구슬 개수 = 몬스터 고용 가능 레벨 / 10 (소수점 이하 올림)
	return	(SI32)( (siNeedLv / 10) + 1 );
}

