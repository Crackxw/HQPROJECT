#include "SOMain.h"
#include "SODefine.h"
#include "SOWorld.h"
#include "SOVillage.h"

#include "SODatabase.h"
#include "SOVillage_HospitalStructure.h"


#define DISCOUNT_WHAFT_MY			90
#define DISCOUNT_WHAFT_ENEMY		100


SOVillage_Wharf::SOVillage_Wharf()
{
	m_uiVillageCode = 0;

	m_lpVillage   = NULL;
	lpstWharfData =	NULL;
}

SOVillage_Wharf::~SOVillage_Wharf()
{
	Free();
}

BOOL SOVillage_Wharf::Init( UI16 uiVillageCode, DWORD dwMaxPlayer, SOVillage* lpVillage )
{
	m_VillageCode	= uiVillageCode;
	m_lpVillage     = lpVillage;

	// �� ������ �εΰ� �ִٸ� �ε��� ����Ÿ�� �����Ѵ�.
	lpstWharfData = new stWharfData;
	ZeroMemory(lpstWharfData, sizeof(stWharfData));

	for(SI32 i = 0; i < m_lpVillage->clWorld->clTicketParser.m_siTicketNum; i++)
	{
		// ����ϴ� ������ �̸��� �̶�� 
		if(m_lpVillage->clWorld->clTicketParser.m_lpTicketHeader[i].uiStartVillageCode == m_VillageCode)
		{
			CopyMemory(&lpstWharfData->stOTicketList[lpstWharfData->uiTotalTicketNum], 
					  &m_lpVillage->clWorld->clTicketParser.m_lpTicketHeader[i], sizeof(TicketHeader));

			lpstWharfData->stTicketList[lpstWharfData->uiTotalTicketNum].m_siTicketKind = 
					  m_lpVillage->clWorld->clTicketParser.m_lpTicketHeader[i].siTicketCode;
			
			lpstWharfData->uiTotalTicketNum++;
		}
	}
	lpstWharfData->WharfUserManager.Create(SOVILLAGE_MAX_WHARF_PLAYER_NUM);
	lpstWharfData->WharfWaitRoomUserManager.Create(SOVILLAGE_MAX_WHARFWAITROOM_PLAYER_NUM);

	SOVillage_BaseStructure::Init( uiVillageCode, dwMaxPlayer );

	return TRUE;
}

VOID SOVillage_Wharf::Free()
{
	m_uiVillageCode	=	0;
	m_VillageCode	=	0;

	m_lpVillage = NULL;

	if(lpstWharfData)
	{
		delete lpstWharfData;
		lpstWharfData	=	NULL;
	}

	SOVillage_BaseStructure::Free();	
}

BOOL SOVillage_Wharf::WharfEnter(SOPlayer *pPlayer)
{
	// �����̳� �ſ뵵�� �������� �˻�
	if((pPlayer->IFollower.GetFollowerParameterLevel(0) < 6) && (pPlayer->GetTradeGrade() < 6))
	{
		pPlayer->SendFieldMsg(ON_RESPONSE_WHARF_ENTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		return TRUE;
	}

	// �εο� ���� �ִ��� �˻�
	if(lpstWharfData->WharfUserManager.m_NowUseDataCount >= SOVILLAGE_MAX_WHARF_PLAYER_NUM)
	{
		pPlayer->SendFieldMsg(ON_RESPONSE_WHARF_ENTER, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
	else
	{
		SI32		DiscountPercent;

		DiscountPercent = 0;

		// �εη� �̵�
		lpstWharfData->WharfUserManager.Add(pPlayer->uiAccount);
		pPlayer->SendFieldMsg(ON_RESPONSE_WHARF_ENTER, SO_SFM_ONLYME, LPVOID(ON_RET_OK));

		// ���� ���� ����̸�
		if(m_lpVillage->uiGuildID != 0)
		{
			if((m_lpVillage->uiGuildID == pPlayer->uiGuildID) && (pPlayer->siClassInGuild != m_lpVillage->clWorld->clIGuildClass.GetJWJCode()))
			{
				DiscountPercent += DISCOUNT_WHAFT_MY;
			}
			if(m_lpVillage->clWorld->clIGuild.IsHostilityRelation(m_lpVillage->uiGuildID, pPlayer->uiGuildID) == TRUE)
			{
				DiscountPercent -= DISCOUNT_WHAFT_ENEMY;
			}
			if(m_lpVillage->clWorld->clIGuild.IsEnemyRelationWar(m_lpVillage->uiGuildID, pPlayer->uiGuildID) == TRUE)
			{
				DiscountPercent -= DISCOUNT_WHAFT_ENEMY;			
			}
		}

		// ���� �εο��� �Ĵ� Ticket ����
		pPlayer->SendFieldMsg(ON_RESPONSE_WHARF_CHANGETICKETLIST, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(lpstWharfData->uiTotalTicketNum), LPVOID(lpstWharfData->stTicketList), LPVOID(DiscountPercent));
	}
	return TRUE;
}

BOOL SOVillage_Wharf::WharfBuyTicket(SOPlayer *pPlayer, SI32 Kind)
{
	__int64				iMoney;
	SI32				TempCount;
	SI32				TempSelectCount;
	SI32				DiscountPercent;
	MONEY				TempTicketPrice;
	MONEY				VillageProfit;

	// Ticket�� �˻�
	TempSelectCount = -1;
	for(TempCount = 0; TempCount < (SI32)lpstWharfData->uiTotalTicketNum; TempCount++)
	{
		if(lpstWharfData->stTicketList[TempCount].m_siTicketKind == Kind)
		{
			TempSelectCount = TempCount;
			break;
		}
	}

	if(TempSelectCount != -1)
	{
		// DC �� �˻�
		DiscountPercent = 0;
		if(m_lpVillage->uiGuildID != 0)
		{
			if((m_lpVillage->uiGuildID == pPlayer->uiGuildID) && (pPlayer->siClassInGuild != m_lpVillage->clWorld->clIGuildClass.GetJWJCode()))
			{
				DiscountPercent += DISCOUNT_WHAFT_MY;
			}
			if(m_lpVillage->clWorld->clIGuild.IsHostilityRelation(m_lpVillage->uiGuildID, pPlayer->uiGuildID) == TRUE)
			{
				DiscountPercent -= DISCOUNT_WHAFT_ENEMY;
			}
			if(m_lpVillage->clWorld->clIGuild.IsEnemyRelationWar(m_lpVillage->uiGuildID, pPlayer->uiGuildID) == TRUE)
			{
				DiscountPercent -= DISCOUNT_WHAFT_ENEMY;
			}
		}

		// ���� �Ŵ��� �˻�
		TempTicketPrice = (MONEY)lpstWharfData->stOTicketList[TempSelectCount].siMoney;
		TempTicketPrice = TempTicketPrice - ((TempTicketPrice * DiscountPercent) / 100);
		
		iMoney = pPlayer->siMoney;
		if((iMoney - TempTicketPrice) < 0)
		{
			pPlayer->SendFieldMsg(ON_RESPONSE_WHARF_BUYTICKET, SO_SFM_ONLYME, LPVOID(ON_RET_NO), NULL);
		}
		else
		{
			// Ticket�� ��
			DBIKBuyTicket		TempDBIKBuyTicket;

			VillageProfit = MONEY(TempTicketPrice*ON_CAPITAL_RATE_TICKET);

			TempDBIKBuyTicket.uiVillageCode   = m_lpVillage->uiVillageCode;
			TempDBIKBuyTicket.DBAccount       = pPlayer->stAccountInfo.siDBAccount;
			TempDBIKBuyTicket.uiSlot          = pPlayer->stAccountInfo.uiSlot;
			TempDBIKBuyTicket.siTicketKind    = lpstWharfData->stTicketList[TempSelectCount].m_siTicketKind;
			TempDBIKBuyTicket.siMoney         = TempTicketPrice;
			TempDBIKBuyTicket.siVillageProfit = VillageProfit;
			m_lpVillage->clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_BUYTICKET, LPVOID(&TempDBIKBuyTicket), sizeof(TempDBIKBuyTicket));

			pPlayer->DecreaseMoney(TempTicketPrice, FALSE);
			m_lpVillage->IncreaseCapital(VillageProfit, ON_PROFIT_TICKET, FALSE);
			pPlayer->stTicketInfo.siTicketKind = lpstWharfData->stTicketList[TempSelectCount].m_siTicketKind;

			// Ticket�� ��ٴ� Message����
			iMoney = pPlayer->siMoney;
			pPlayer->SendFieldMsg(ON_RESPONSE_WHARF_BUYTICKET, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(&iMoney));

			// ���� �εο��� �Ĵ� Ticket ����
			pPlayer->SendFieldMsg(ON_RESPONSE_WHARF_CHANGETICKETLIST, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(lpstWharfData->uiTotalTicketNum), LPVOID(lpstWharfData->stTicketList), LPVOID(DiscountPercent));
		}
	}

	return TRUE;
}

BOOL SOVillage_Wharf::WharfExit(SOPlayer *pPlayer)
{
	// �εθ� ������.
	lpstWharfData->WharfUserManager.DeleteType(pPlayer->uiAccount);
	pPlayer->SendFieldMsg(ON_RESPONSE_WHARF_EXIT, SO_SFM_ONLYME, LPVOID(ON_RET_OK));

	return TRUE;
}

BOOL SOVillage_Wharf::WharfWaitRoomEnter(SOPlayer *pPlayer)
{
	TicketHeader*		lpTicketHeader;

	// �ε� ���Ƿ� ���� �ִ� Ticket�� �������� �˻�
	lpTicketHeader = m_lpVillage->clWorld->clTicketParser.GetTicket(pPlayer->stTicketInfo.siTicketKind);
	if(lpTicketHeader == NULL)
	{
		pPlayer->SendFieldMsg(ON_RESPONSE_WHARFWAITROOM_ENTER, SO_SFM_ONLYME, LPVOID(ON_RET_NOTUSETICKET));
	}
	else
	{
		if((UI32)lpTicketHeader->uiStartVillageCode != m_VillageCode)
		{
			pPlayer->SendFieldMsg(ON_RESPONSE_WHARFWAITROOM_ENTER, SO_SFM_ONLYME, LPVOID(ON_RET_NOTUSETICKET));
		}
		else
		{
			// �ε� ���ǿ� ����.
			lpstWharfData->WharfWaitRoomUserManager.Add(pPlayer->uiAccount);
			pPlayer->SendFieldMsg(ON_RESPONSE_WHARFWAITROOM_ENTER, SO_SFM_ONLYME, LPVOID(ON_RET_OK));

			pPlayer->uiStructerInPos = STRUCTERPOS_WHARFWAITTINGROOM;
		}
	}

	return TRUE;
}

BOOL SOVillage_Wharf::WharfWaitRoomExit(SOPlayer *pPlayer)
{
	// �εδ��ǿ��� ������.
	lpstWharfData->WharfWaitRoomUserManager.DeleteType(pPlayer->uiAccount);
	pPlayer->SendFieldMsg(ON_RESPONSE_WHARFWAITROOM_EXIT, SO_SFM_ONLYME, LPVOID(ON_RET_OK));

	pPlayer->uiStructerInPos = STRUCTERPOS_NONE;

	return TRUE;
}

