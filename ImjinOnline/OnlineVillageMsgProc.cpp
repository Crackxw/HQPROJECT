#include <GSL.h>

#include "OnlineGlobal.h"
#include "OnlineMsgProc.h"

extern	_clGame* pGame;


UI16 OnlineMsgProc::ResCure(BYTE *pMsg)
{
	OnResHospitalCureSolderOK*		TemplpOnResHospitalCureSolderOK;
	IOnlineFollower*				lpIOnlineFollower;
	OnlineFollower*					lpOnlineFollower;

	OnResHospitalCureSolder* TemplpOnResHospitalCureSolder = (OnResHospitalCureSolder*)pMsg;
	if(TemplpOnResHospitalCureSolder == NULL)	return 0;

	if(TemplpOnResHospitalCureSolder->dwResponse == ON_RET_AUTOCUREOK)		// ������ �ڵ�ġ��
	{
		TemplpOnResHospitalCureSolderOK = (OnResHospitalCureSolderOK*)TemplpOnResHospitalCureSolder;

		lpIOnlineFollower = _pWorld->pMyData->GetFollowersInterface();
		lpOnlineFollower  = lpIOnlineFollower->GetFollower(TemplpOnResHospitalCureSolderOK->dwCureSolderSlot);
		lpOnlineFollower->ParametaBox.IP.SetLife(TemplpOnResHospitalCureSolderOK->siLife);
		lpOnlineFollower->ParametaBox.IP.SetMana(TemplpOnResHospitalCureSolderOK->siMana);	
	}
	else if(TemplpOnResHospitalCureSolder->dwResponse == ON_RET_OK)			// �ǿ����� ��û�� ���� ġ��
	{
		OnlineVillageStructureHospital *pStructure = (OnlineVillageStructureHospital *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
		if(!pStructure)		return 0;
		TemplpOnResHospitalCureSolderOK = (OnResHospitalCureSolderOK*)TemplpOnResHospitalCureSolder;
		
		// ĳ���� Data ����
		lpIOnlineFollower = _pWorld->pMyData->GetFollowersInterface();
		lpOnlineFollower  = lpIOnlineFollower->GetFollower(TemplpOnResHospitalCureSolderOK->dwCureSolderSlot);
		lpOnlineFollower->ParametaBox.IP.SetLife(TemplpOnResHospitalCureSolderOK->siLife);
		lpOnlineFollower->ParametaBox.IP.SetMana(TemplpOnResHospitalCureSolderOK->siMana);
		
		// �ڽ��� Money ����
		_pWorld->pMyData->SetMoney(TemplpOnResHospitalCureSolderOK->Money);
		
		// Server�� ���ο� List ��û
		_pWorld->pOnlineClient->SendRequestHospitalGetCureList();
		pStructure->m_NowWork = HOSPITAL_NOWWORK_CHECK_POOL_RETURNWAIT1;
		pStructure->m_Say = 1;
		
		// Sound
		PushEffect(EFFECT_ON_HEAL, 0, 0);
		
		if(_pWorld->pOnlineVillage->GetNation() == ON_COUNTRY_KOREA || _pWorld->pOnlineVillage->GetNation() == ON_COUNTRY_CHINA)
			PushEffect(EFFECT_HOSPITAL40, 0, 0);
		else if(_pWorld->pOnlineVillage->GetNation() == ON_COUNTRY_TAIWAN)
			PushEffect(EFFECT_HOSPITAL40T, 0, 0);
		else if(_pWorld->pOnlineVillage->GetNation() == ON_COUNTRY_JAPAN)
			PushEffect(EFFECT_HOSPITAL40J, 0, 0);
	}
	else
	{
		OnlineVillageStructureHospital *pStructure = (OnlineVillageStructureHospital *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
		if(!pStructure)		return 0;
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, _pWorld->pOnlineText->Get(ON_TEXT_HOSPITAL), _pWorld->pOnlineText->Get(ON_TEXT_HOSPITAL_HEALFAIL));
		pStructure->m_NowWork         = HOSPITAL_NOWWORK_MAIN_POOL;
		pStructure->m_NowSelectSolder = -1;
	}
	
	return 0;
}

UI16 OnlineMsgProc::ResCureAll(BYTE *pMsg)
{
	SI32							TempCount;
	OnResHospitalCureAllSolderOK*	TemplpOnResHospitalCureAllSolderOK;
	IOnlineFollower*				lpIOnlineFollower;
	OnlineFollower*					lpOnlineFollower;

	OnResHospitalCureAllSolder*	TemplpOnResHospitalCureAllSolder = (OnResHospitalCureAllSolder*)pMsg;
	if(TemplpOnResHospitalCureAllSolder == NULL)	return 0;

	if(TemplpOnResHospitalCureAllSolder->dwResponse == ON_RET_AUTOCUREOK)
	{
		// ������ ġ���Ѵ�.
		TemplpOnResHospitalCureAllSolderOK = (OnResHospitalCureAllSolderOK*)TemplpOnResHospitalCureAllSolder;

		lpIOnlineFollower = _pWorld->pMyData->GetFollowersInterface();
		for(TempCount = 0; TempCount < TemplpOnResHospitalCureAllSolderOK->CureSolderCount; TempCount++)
		{
			lpOnlineFollower  = lpIOnlineFollower->GetFollower(TemplpOnResHospitalCureAllSolderOK->CureSolder[TempCount].dwCureSolderSlot);

			lpOnlineFollower->ParametaBox.IP.SetLife(TemplpOnResHospitalCureAllSolderOK->CureSolder[TempCount].siLife);
			lpOnlineFollower->ParametaBox.IP.SetMana(TemplpOnResHospitalCureAllSolderOK->CureSolder[TempCount].siMana);
		}				
	}
	else if(TemplpOnResHospitalCureAllSolder->dwResponse == ON_RET_OK)
	{
		OnlineVillageStructureHospital *pStructure = (OnlineVillageStructureHospital *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
		if(!pStructure)		return 0;
		TemplpOnResHospitalCureAllSolderOK = (OnResHospitalCureAllSolderOK*)TemplpOnResHospitalCureAllSolder;
		
		// ĳ���� Data ����
		lpIOnlineFollower = _pWorld->pMyData->GetFollowersInterface();
		for(TempCount = 0; TempCount < TemplpOnResHospitalCureAllSolderOK->CureSolderCount; TempCount++)
		{
			lpOnlineFollower  = lpIOnlineFollower->GetFollower(TemplpOnResHospitalCureAllSolderOK->CureSolder[TempCount].dwCureSolderSlot);
			lpOnlineFollower->ParametaBox.IP.SetLife(TemplpOnResHospitalCureAllSolderOK->CureSolder[TempCount].siLife);
			lpOnlineFollower->ParametaBox.IP.SetMana(TemplpOnResHospitalCureAllSolderOK->CureSolder[TempCount].siMana);
		}
		
		// �ڽ��� Money ����
		_pWorld->pMyData->SetMoney(TemplpOnResHospitalCureAllSolderOK->Money);
		
		// Server�� ���ο� List ��û
		_pWorld->pOnlineClient->SendRequestHospitalGetCureList();
		pStructure->m_NowWork = HOSPITAL_NOWWORK_CHECK_POOL_RETURNWAIT1;
		pStructure->m_Say = 1;
		
		// Sound
		PushEffect(EFFECT_ON_HEAL, 0, 0);
	}
	else
	{
		OnlineVillageStructureHospital *pStructure = (OnlineVillageStructureHospital *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
		if(!pStructure)		return 0;
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, _pWorld->pOnlineText->Get(ON_TEXT_HOSPITAL), _pWorld->pOnlineText->Get(ON_TEXT_HOSPITAL_HEALFAIL));
		pStructure->m_NowWork         = HOSPITAL_NOWWORK_MAIN_POOL;
		pStructure->m_NowSelectSolder = -1;
	}

	return 0;
}

UI16 OnlineMsgProc::GetCureList(BYTE *pMsg)
{
	OnResHospitalGetCureList*	lpOnResHospitalGetCureList;
	OnResHospitalGetCureListOK*	lpOnResHospitalGetCureListOK;
	UI08*						lpSlot;
	MONEY						*lpMoney, *lpRevivalMoney;
	SI32						TempCount;

	lpOnResHospitalGetCureList = (OnResHospitalGetCureList*)pMsg;
	OnlineVillageStructureHospital *pStructure = (OnlineVillageStructureHospital *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;

	ZeroMemory(pStructure->m_CureSolderList, sizeof(SCureSolderData) * MAX_CURESOLDER_LIST);
	pStructure->m_MaxSelectSolder = 0;
	if(lpOnResHospitalGetCureList->dwResponse == ON_RET_OK)
	{
		lpOnResHospitalGetCureListOK = (OnResHospitalGetCureListOK*)lpOnResHospitalGetCureList;
		lpSlot                       = (UI08*)((BYTE*)lpOnResHospitalGetCureListOK + sizeof(OnResHospitalGetCureListOK));
		lpMoney                      = (MONEY*)((BYTE*)lpSlot +  (sizeof(UI08)  * lpOnResHospitalGetCureListOK->dwCureSolderCount));
		lpRevivalMoney               = (MONEY*)((BYTE*)lpMoney + (sizeof(MONEY) * lpOnResHospitalGetCureListOK->dwCureSolderCount));

		pStructure->m_MaxSelectSolder = lpOnResHospitalGetCureListOK->dwCureSolderCount;
		pStructure->m_NowSelectSolder = -1;
		pStructure->m_Discount        = lpOnResHospitalGetCureListOK->dwDiscount;
		for(TempCount = 0; TempCount < pStructure->m_MaxSelectSolder; TempCount++)
		{
			pStructure->m_CureSolderList[TempCount].m_Slot         = lpSlot[TempCount];
			pStructure->m_CureSolderList[TempCount].m_Money        = lpMoney[TempCount];
			pStructure->m_CureSolderList[TempCount].m_RevivalMoney = lpRevivalMoney[TempCount];
		}

		pStructure->FindFollowerData();
		
		if(pStructure->m_NowWork == HOSPITAL_NOWWORK_MAIN_START)
		{
			pStructure->m_NowWork        = HOSPITAL_NOWWORK_MAIN_STARTPOOL;
			pStructure->m_StartTickCount = GetTickCount();
		}
		else
		{
			pStructure->m_NowWork         = HOSPITAL_NOWWORK_MAIN_POOL;
			pStructure->m_NowSelectSolder = -1;
		}
	}
	else
	{
		if(pStructure->m_NowWork == HOSPITAL_NOWWORK_CHECK_POOL_RETURNWAIT1)
		{
			pStructure->m_NowWork         = HOSPITAL_NOWWORK_MAIN_POOL;
			pStructure->m_NowSelectSolder = -1;
		}
	}
	return 0;
}

UI16 OnlineMsgProc::WharfEnter(BYTE *pMsg)
{
	OnResWharf_Enter*lpOnResWharf_Enter = (OnResWharf_Enter*)pMsg;
	OnlineVillageStructureWharf *pStructure = (OnlineVillageStructureWharf *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;

	switch(lpOnResWharf_Enter->dwResponse)
	{
	case ON_RET_OK:
		pStructure->siTicketKind     = lpOnResWharf_Enter->siTicketKind;
		pStructure->m_NowWork        = WHARF_NOWWORK_MAIN_STARTPOOL;
		pStructure->m_StartTickCount = GetTickCount();
		break;
		
	case ON_RET_NO:
	default:
		pStructure->m_NowWork        = WHARF_NOWWORK_EXIT_POOL;
		break;
	}
	return 0;
}

UI16 OnlineMsgProc::WharfBuyTicket(BYTE *pMsg)
{
	OnResWharf_BuyTicket* lpOnResWharf_BuyTicket = (OnResWharf_BuyTicket*)pMsg;
	OnlineVillageStructureWharf *pStructure = (OnlineVillageStructureWharf *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;

	if(lpOnResWharf_BuyTicket->dwResponse == ON_RET_OK)
	{
		lpOnResWharf_BuyTicket = (OnResWharf_BuyTicket*)lpOnResWharf_BuyTicket;
		
		pStructure->siTicketKind      = lpOnResWharf_BuyTicket->siTicketKind;
		
		// �ڽ��� Money ����
		_pWorld->pMyData->SetMoney(lpOnResWharf_BuyTicket->NowMoney);
		
		// Next Menu
		if(pStructure->CheckTicketUse())
		{
			// ���� Ticket�� �� �εδ��ǿ��� ���������
			pStructure->m_NowWork         = WHARF_NOWWORK_MAIN_POOL;
			pStructure->m_NowSelectTicket = -1;
			
			_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, _pWorld->pOnlineText->Get(ON_TEXT_WHARF), _pWorld->pOnlineText->Get(ON_TEXT_WHARF_SHIPTICKETNO1));
		}
		else
		{
			// �̵�
			pStructure->m_NowWork  = WHARF_NOWWORK_MAIN_END;
			pStructure->m_NextWork = WHARF_NOWWORK_WAITROOM_START;
			pStructure->m_NowSelectTicket = -1;
			
			_pWorld->pOnlineClient->SendRequestWharfExit();
		}
	}
	else
	{
		// Next Menu
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, _pWorld->pOnlineText->Get(ON_TEXT_WHARF), _pWorld->pOnlineText->Get(ON_TEXT_WHARF_TICKETBUYFAIL));
		pStructure->m_NowWork         = WHARF_NOWWORK_MAIN_POOL;
		pStructure->m_NowSelectTicket = -1;
	}
	return 0;
}

UI16 OnlineMsgProc::WharfExit(BYTE *pMsg)
{
	OnResWharf_Exit* pOnResWharf_Exit = (OnResWharf_Exit*)pMsg;
	OnlineVillageStructureWharf *pStructure = (OnlineVillageStructureWharf *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;

	pStructure->m_NowWork        = WHARF_NOWWORK_MAIN_ENDPOOL;
	pStructure->m_StartTickCount = GetTickCount();
	return 0;
}

UI16 OnlineMsgProc::WharfWaitroomEnter(BYTE *pMsg)
{
	OnResWharfWaitRoom_Enter *lpOnResWharfWaitRoom_Enter = (OnResWharfWaitRoom_Enter*)pMsg;
	OnlineVillageStructureWharf *pStructure = (OnlineVillageStructureWharf *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;

	switch(lpOnResWharfWaitRoom_Enter->dwResponse)
	{
	case ON_RET_OK:
		pStructure->siTicketKind      = lpOnResWharfWaitRoom_Enter->siTicketKind;
		
		pStructure->m_NowWork        = WHARF_NOWWORK_WAITROOM_STARTPOOL;
		pStructure->m_StartTickCount = GetTickCount();
		break;
		
	case ON_RET_NOTUSETICKET:
		pStructure->m_NowWork        = WHARF_NOWWORK_MAIN_START;
		_pWorld->pOnlineClient->SendRequestWharfEnter();
		break;
		
	case ON_RET_NO:
	default:
		pStructure->m_NowWork = WHARF_NOWWORK_EXIT_POOL;
	}
	return 0;
}

UI16 OnlineMsgProc::WharfWaitroomExit(BYTE *pMsg)
{
	OnResWharfWaitRoom_Exit* pOnResWharfWaitRoom_Exit = (OnResWharfWaitRoom_Exit*)pMsg;
	OnlineVillageStructureWharf *pStructure = (OnlineVillageStructureWharf *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;

	pStructure->m_NowWork        = WHARF_NOWWORK_WAITROOM_ENDPOOL;
	pStructure->m_StartTickCount = GetTickCount();
	return 0;
}

UI16 OnlineMsgProc::WharfTicketList(BYTE *pMsg)
{
	OnResWharf_ChangeTicketList*	lpOnResWharf_ChangeTicketList;
	BYTE*							lpBuffer;

	lpOnResWharf_ChangeTicketList = (OnResWharf_ChangeTicketList*)pMsg;
	lpBuffer                      = (BYTE*)((BYTE*)lpOnResWharf_ChangeTicketList + sizeof(OnResWharf_ChangeTicketList));
	OnlineVillageStructureWharf *pStructure = (OnlineVillageStructureWharf *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;

	if(lpOnResWharf_ChangeTicketList->dwMsgType == ON_RESPONSE_WHARF_CHANGETICKETLIST)
	{
		pStructure->m_MaxSelectTicket = lpOnResWharf_ChangeTicketList->siTicketCount;
		pStructure->m_DiscountPercent = lpOnResWharf_ChangeTicketList->siDiscountPercent;
		CopyMemory(pStructure->m_TicketList, lpBuffer, sizeof(OnTicketInfo) * pStructure->m_MaxSelectTicket);
	}
	return 0;
}

UI16 OnlineMsgProc::BoardShip(BYTE *pMsg)
{
	OnBoardShipMsg				*pBoardShipMsg;
	OnlineReceiveFieldItemInfo	*pRecvFieldItemInfo;
	SI32						i;
	BOOL						bFindMyShip;

	OnCharBaseInfo*				lpOnCharBaseInfo;
	OnBaseBoothItem				*lpOnBaseBoothItem, *pTempOnBaseBoothItem;
	SellItemInfo				TempSellItemInfo;
	OnCharPlayerInfo*			lpOnCharPlayerInfo;
	OnCharMonsterInfo*			lpOnCharMonsterInfo;
	OnCharShipInfo*				lpOnCharShipInfo;

	// �谡 �����Ѵٴ� �޽����� �޾Ҵ�.
	// �׷��� �ʵ�� ���ư��� �Ѵ�.
	
	// �� ĳ���͸� �߰� ��Ų��.
	pBoardShipMsg	=	(OnBoardShipMsg*)pMsg;

	if( pBoardShipMsg == NULL )	return 0;

	// Map��ġ�� �����Ѵ�.
	// ĳ���� �� �������� �ʿ� �����Ѵ�.
	lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)pBoardShipMsg + sizeof(OnBoardShipMsg));
	bFindMyShip		 = FALSE;

	for(i = 0; i < pBoardShipMsg->uiAroundCharNum; i++)
	{	
		if(ISPLAYER(lpOnCharBaseInfo->uiAccount))
		{
			lpOnCharPlayerInfo = (OnCharPlayerInfo*)lpOnCharBaseInfo;

			if(lpOnCharPlayerInfo->siPrivateMarketItemNum >= 0) lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharPlayerInfo) + (lpOnCharPlayerInfo->siPrivateMarketItemNum * sizeof(OnBaseBoothItem)));
			else                                                lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharPlayerInfo));
		}
		else if(ISMONSTER(lpOnCharBaseInfo->uiAccount))
		{
			lpOnCharMonsterInfo = (OnCharMonsterInfo*)lpOnCharBaseInfo;

			lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharMonsterInfo));
		}
		else if(ISSHIP(lpOnCharBaseInfo->uiAccount))
		{
			lpOnCharShipInfo = (OnCharShipInfo*)lpOnCharBaseInfo;

			if(pBoardShipMsg->uiAccount == lpOnCharShipInfo->uiAccount)
			{			
				// ȭ�鿡 ��Ŀ���� ���� ĳ�����̴�.						
				bFindMyShip		=	TRUE;
				_pWorld->pOnlineMap->SetXY(lpOnCharShipInfo->siX, lpOnCharShipInfo->siY);
				
			}
			lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharShipInfo));
		}
	}	

	if(bFindMyShip == FALSE)
		clGrp.Error("", "[%s]", _pWorld->pOnlineText->Get(ON_TEXT_CANNOT_FIND_MYSHIP));

	// ĳ���� �� �������� �ʿ� �����Ѵ�.
	lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)pBoardShipMsg + sizeof(OnBoardShipMsg));

	CHAR	TempBuffer[128];
	DWORD	IP;

	for(i = 0; i < pBoardShipMsg->uiAroundCharNum; i++)
	{						
		if(ISPLAYER(lpOnCharBaseInfo->uiAccount))
		{
			lpOnCharPlayerInfo = (OnCharPlayerInfo*)lpOnCharBaseInfo;

			ZeroMemory(TempBuffer, 128);
			if(_pWorld->pMyData->GetMyCharUniqueID() == lpOnCharPlayerInfo->uiAccount)
			{
				CopyMemory(TempBuffer, _pWorld->pMyData->GetMyCharName(), ON_ID_LENGTH);
				IP = lpOnCharPlayerInfo->siDBAccount;

				_pWorld->pIOnlineChar->Init(lpOnCharPlayerInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharPlayerInfo->uiKind, lpOnCharPlayerInfo->siX, lpOnCharPlayerInfo->siY, IP, 0, lpOnCharPlayerInfo->siFightingPower, _pWorld,(UI16*)lpOnCharPlayerInfo->uiEquipWeapon);
				_pWorld->pIOnlineChar->SetEquipItem(lpOnCharPlayerInfo->uiAccount, (UI16*)lpOnCharPlayerInfo->uiEquipWeapon);
				
				// ��� ó��
				if(_pWorld->pMyData->GetGuildName())
				{
					_pWorld->pIOnlineChar->SetGuildData(lpOnCharPlayerInfo->uiAccount, _pWorld->pMyData->GetGuildName(), _pWorld->pMyData->GetGuildClass(), _pWorld->pMyData->GetGuildFlag());
				}
			}
			else
			{
				IP = lpOnCharPlayerInfo->siDBAccount;

				_pWorld->pIOnlineChar->Init(lpOnCharPlayerInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharPlayerInfo->uiKind, lpOnCharPlayerInfo->siX, lpOnCharPlayerInfo->siY, IP, 0, lpOnCharPlayerInfo->siFightingPower, _pWorld,(UI16*)lpOnCharPlayerInfo->uiEquipWeapon);
				_pWorld->pIOnlineChar->SetEquipItem(lpOnCharPlayerInfo->uiAccount, (UI16*)lpOnCharPlayerInfo->uiEquipWeapon);
			}
			_pWorld->pIOnlineChar->SetGuildData(lpOnCharPlayerInfo->uiAccount, _pWorld->pIOnlineChar->GetGuildName(lpOnCharPlayerInfo->uiAccount), lpOnCharPlayerInfo->siGuildInGrade, _pWorld->pIOnlineChar->GetGuildFlag(lpOnCharPlayerInfo->uiAccount));
			_pWorld->pIOnlineChar->SetMercenary(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->siVMercenaryKind);

			// ���� ����
			if(lpOnCharPlayerInfo->siPrivateMarketItemNum >= 0) 
			{
				_pWorld->pIOnlineChar->SetBoothOpen(lpOnCharPlayerInfo->uiAccount, TRUE);

				lpOnBaseBoothItem		= (OnBaseBoothItem*)((CHAR*)lpOnCharPlayerInfo + sizeof(OnCharPlayerInfo));
				pTempOnBaseBoothItem	= lpOnBaseBoothItem;

				for(SI32 TempCount1 = 0; TempCount1 < lpOnCharPlayerInfo->siPrivateMarketItemNum; TempCount1++)
				{
					ZeroMemory(&TempSellItemInfo, sizeof(SellItemInfo));
					if(pTempOnBaseBoothItem->siType == LIST_ITEM)
						TempSellItemInfo.Item.uiItemID = pTempOnBaseBoothItem->uiProductID;

					else if(pTempOnBaseBoothItem->siType == LIST_FOLLOWER)
						TempSellItemInfo.Follower.uiKind = pTempOnBaseBoothItem->uiProductID;

					TempSellItemInfo.siType = pTempOnBaseBoothItem->siType;
					_pWorld->pIOnlineChar->SetBoothItem(lpOnCharPlayerInfo->uiAccount, pTempOnBaseBoothItem->uiBoothSlot, TempSellItemInfo);
					pTempOnBaseBoothItem++;
				}

				_pWorld->pOnlineBooth->SetBoothSpeech(lpOnCharPlayerInfo->uiAccount, lpOnBaseBoothItem, lpOnCharPlayerInfo->siPrivateMarketItemNum);
			}
			else
			{
				_pWorld->pIOnlineChar->SetBoothOpen(lpOnCharPlayerInfo->uiAccount, FALSE);
			}

			if(lpOnCharPlayerInfo->siPrivateMarketItemNum >= 0) lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharPlayerInfo) + (lpOnCharPlayerInfo->siPrivateMarketItemNum * sizeof(OnBaseBoothItem)));
			else                                                lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharPlayerInfo));
		}
		else if(ISMONSTER(lpOnCharBaseInfo->uiAccount))
		{
			lpOnCharMonsterInfo = (OnCharMonsterInfo*)lpOnCharBaseInfo;

			// �ٸ� ĳ������ ���� ������ �����Ѵ�.						
			ZeroMemory(TempBuffer, 128);
			IP = 0;
			_pWorld->pIOnlineChar->Init(lpOnCharMonsterInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharMonsterInfo->uiKind, lpOnCharMonsterInfo->siX, lpOnCharMonsterInfo->siY, IP, 0.0f, lpOnCharMonsterInfo->siFightingPower, _pWorld,NULL);
			
			lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharMonsterInfo));
		}
		else if(ISSHIP(lpOnCharBaseInfo->uiAccount))
		{
			lpOnCharShipInfo = (OnCharShipInfo*)lpOnCharBaseInfo;

			// �ٸ� ĳ������ ���� ������ �����Ѵ�.						
			ZeroMemory(TempBuffer, 128);
			IP = 0;
			_pWorld->pIOnlineChar->Init(lpOnCharShipInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharShipInfo->uiKind, lpOnCharShipInfo->siX, lpOnCharShipInfo->siY, IP, 0.0f, 0, _pWorld,NULL);

			if(pBoardShipMsg->uiAccount == lpOnCharShipInfo->uiAccount)
			{
				// ���� ��Ŀ���� �ش� ��� �ű��.
				_pWorld->pOnlineMap->SetCurrentFocusCharID(pBoardShipMsg->uiAccount);
			}

			lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharShipInfo));
		}
	}	

	pRecvFieldItemInfo		=	(OnlineReceiveFieldItemInfo*)((CHAR*)lpOnCharBaseInfo);
	for(i = 0; i < 	pBoardShipMsg->uiAroundItemNum; i++)
	{
		// �ʵ忡 �������� �����Ѵ�.
		_pWorld->pOnlineMap->SetID(pRecvFieldItemInfo[i].siX, pRecvFieldItemInfo[i].siY, 1, 1, 0, 0, pRecvFieldItemInfo[i].uiItemID, pRecvFieldItemInfo[i].uiQuantity);
	}								

	// �ʵ�� ���ư��ٰ� �Ѵ�.
	_pWorld->GotoField();


	// ���� ��Ŀ���� �ش� ��� �ű��.
	_pWorld->pOnlineMap->SetCurrentFocusCharID(pBoardShipMsg->uiAccount);				

	// ������ ��ġ�� ���´�.
	{
		SI16 siX, siY, siRX, siRY;

		_pWorld->pIOnlineChar->GetPos(pBoardShipMsg->uiAccount, &siX, &siY);
		_pWorld->pIOnlineChar->GetRPos(pBoardShipMsg->uiAccount, &siRX, &siRY);

//					DWriteStringToFile("�� ���� ��ġ : [%d] [%d] [%d] [%d] [%d]", pBoardShipMsg->uiAccount, siX, siY, siRX, siRY);
	}

	return 0;
}

UI16 OnlineMsgProc::GetOutShip(BYTE *pMsg)
{
	// �踦 Ÿ�� �������� �����Ͽ���.
	// ������ �ش� ������ ����.
	OnGetOutShipMsg		*pGetOutShipMsg;				
	UI16				uiVillageCode;	

	pGetOutShipMsg	=	(OnGetOutShipMsg*)pMsg;

	// ĳ���Ͱ� ������ ����. ������ �� ĳ���Ͱ� �ڱ� ĳ�����̶�� �ڱⰡ ������ �ִ� ��� ĳ������ ���� ������ ������.
	// �׷��� �ʰ� �ٸ� ĳ�����̶�� �� ĳ���͸� ������.				

	// ������ ���� ������ �� �� �ֶǷ� �㰡�� �޾Ҵ�.

	if( pGetOutShipMsg == NULL ) return 0;

	// ������ ��ü�� �����Ѵ�.
	_pWorld->pOnlineVillage	=	new OnlineVillage;				

	// ������ �̵��ؾ� �Ѵ�.
	uiVillageCode		=	pGetOutShipMsg->uiVillageCode;				
	
	_pWorld->pOnlineVillage->Init(uiVillageCode, &pGetOutShipMsg->VillageInfo, _pWorld);				
	_pWorld->SetGamePollStatus( ON_GAMEPOLL_VILLAGE );

	// ��� ĳ���͵��� ���ش�.
	_pWorld->pIOnlineChar->AllFree();			

	// �� �̻� ������ ���� ������ ���ٴ� �޽����� ��ٸ��� �ʵ��� �����Ѵ�.
	_pWorld->pMyData->SetWaitGotoVillageResponse(FALSE);		
	
	// ������ ���ٴ� ������ �ʱ�ȭ �Ѵ�.
	_pWorld->pMyData->SetGotoVillage(FALSE, 0);							

	// ���� ������ �߰��Ѵ�.
	switch(pGetOutShipMsg->VillageInfo.cNationCode)
	{
		case NATION_CODE_KOREA:
			pGetOutShipMsg->VillageInfo.cNationCode = ON_COUNTRY_KOREA;
			break;

		case NATION_CODE_CHINA:
			pGetOutShipMsg->VillageInfo.cNationCode = ON_COUNTRY_CHINA;
			break;

		case NATION_CODE_JAPAN:
			pGetOutShipMsg->VillageInfo.cNationCode = ON_COUNTRY_JAPAN;
			break;

		case NATION_CODE_TAIWAN:
			pGetOutShipMsg->VillageInfo.cNationCode = ON_COUNTRY_TAIWAN;
			break;
	}

	_pWorld->pOnlineVillageDescriptList->SetVillageDataData(pGetOutShipMsg->uiVillageCode, pGetOutShipMsg->VillageInfo.cNationCode
		, (char*)pGetOutShipMsg->VillageInfo.szGuildID
		, pGetOutShipMsg->VillageInfo.siGuildFlag
		, pGetOutShipMsg->VillageInfo.vdVillageDefence
		, pGetOutShipMsg->VillageInfo.vdVillageMaxDefence
		, pGetOutShipMsg->VillageInfo.dwGuildRelation
		, pGetOutShipMsg->VillageInfo.siDefenceSoldier	// ���� ���� �δ��
		, pGetOutShipMsg->VillageInfo.siDefenceSoldier	/*pGetOutShipMsg->VillageInfo.siMaxDefenceSoldier*/	// �ִ� ���� �δ��
		, pGetOutShipMsg->VillageInfo.siAttackType	// ���� ���� Ÿ��
		);

	_pWorld->pOnlineClientChatting->MessageSend_NotIsMap();

	return 0;
}

// ��� ����� ����	
UI16 OnlineMsgProc::ResChangeGuildFlag(BYTE *pMsg)
{
	// actdoll (2004/03/15 22:26) : Critical Error Recovering at Ver.10205
	//	��� ����� �������� �� ���� Ŭ������ �ʱ�ȭ �Ǿ����� ���� �� �� �޽����� ȣ��Ǵ� ��� ������ �߻��Ѵ�.
	//	��� ����� �����Ų ����� ������� �������� �޽����̳� �׷����� ��������, 
	//	�ʵ忡 ���� ��� �� �޽����� �������� 100% �״´�. 
	//	���� �� �κп� �ϴ� ������ġ�� �ɾ���ڴ�.
	//	���� ���� �����ÿ� �� �κп��� ���װ� �ȳ����ų� ��ġ�� ���� Ư���� ������ ���� ��� �� �ּ��� �����ص� ����.
	if( !_pWorld->pOnlineVillage || !_pWorld->pOnlineVillage->pVillageStructManager )	return 0;
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->ResChangeGuildFlag(pMsg);
}

UI16 OnlineMsgProc::OnResChangedVillageDefence(BYTE *pMsg)
{
	OnChangeVillageDefenceMsg*		lpOnChangeVillageDefenceMsg = (OnChangeVillageDefenceMsg*)pMsg;
	VillageHeader*					lpVillageHeader;
	SVillageData*					lpSVillageData;

	lpVillageHeader = _pWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(lpOnChangeVillageDefenceMsg->uiVillageCode);
	lpSVillageData  = _pWorld->pOnlineVillageDescriptList->GetVillageDataData(lpVillageHeader->uiVillageCode);
	if(lpSVillageData)
	{
		if(lpSVillageData->m_fWait == FALSE)
		{
			lpSVillageData->m_vdChangedDefence = lpOnChangeVillageDefenceMsg->vdChangedDefence;
		}
	}

	return 0;
}

UI16 OnlineMsgProc::BankWithdraw(BYTE *pMsg)
{
	OnResBankWithdrawMsg *pOnResBankDrawingMsg = (OnResBankWithdrawMsg*)pMsg;

	if( pOnResBankDrawingMsg->dwResponse == ON_RET_BANK_OK )
	{
		_pWorld->pMyData->SetMoney( pOnResBankDrawingMsg->moMoney );
		_pWorld->pMyData->SetSaving( pOnResBankDrawingMsg->moSaving );
		OnlineVillageStructureBank *pStructure = (OnlineVillageStructureBank *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
		if(!pStructure)		return 0;
		pStructure->SetWithdrawStatus(ON_BANK_WITHDRAW_EXIT);
	}
	else if( pOnResBankDrawingMsg->dwResponse == ON_RET_BANK_NO )
	{	
		OnlineVillageStructureBank *pStructure = (OnlineVillageStructureBank *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
		if(!pStructure)		return 0;
		pStructure->SetWithdrawStatus(ON_BANK_WITHDRAW_ERROR);
	}

	return 0;
}

UI16 OnlineMsgProc::BankSave(BYTE *pMsg)
{
	OnResBankSaveMsg *pOnResBankSaveMsg = (OnResBankSaveMsg*)pMsg;
	if( pOnResBankSaveMsg->dwResponse == ON_RET_BANK_OK )
	{
		_pWorld->pMyData->SetMoney( pOnResBankSaveMsg->moMoney );
		_pWorld->pMyData->SetSaving( pOnResBankSaveMsg->moSaving );
		OnlineVillageStructureBank *pStructure = (OnlineVillageStructureBank *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
		if(!pStructure)		return 0;
		pStructure->SetSaveStatus(ON_BANK_SAVE_EXIT);
	}
	else if( pOnResBankSaveMsg->dwResponse == ON_RET_BANK_NO )
	{
		OnlineVillageStructureBank *pStructure = (OnlineVillageStructureBank *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
		if(!pStructure)		return 0;
		pStructure->SetSaveStatus(ON_BANK_SAVE_ERROR);	
	}
	return 0;
}

UI16 OnlineMsgProc::BarrackBuySolderList(BYTE *pMsg)
{
	OnResBarrackGetList*		lpOnResBarrackGetList;
	OnResBarrackGetListOK*		lpOnResBarrackGetListOK;
	BYTE*						lpSolderData;
	
	lpOnResBarrackGetList = (OnResBarrackGetList*)pMsg;

	OnlineVillageStructureBarrack *pStructure = (OnlineVillageStructureBarrack *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;

	ZeroMemory(pStructure->m_SolderList, sizeof(OnInnBuySolderInfo) * MAX_BUYSOLDER_LIST);
	pStructure->m_MaxSelectSolder = 0;
	if(lpOnResBarrackGetList->dwResponse == ON_RET_OK)
	{
		lpOnResBarrackGetListOK = (OnResBarrackGetListOK*)lpOnResBarrackGetList;
		lpSolderData            = (BYTE*)((BYTE*)lpOnResBarrackGetListOK + sizeof(OnResBarrackGetListOK));

		pStructure->m_MaxSelectSolder = lpOnResBarrackGetListOK->dwBuySolderCount;
		pStructure->m_NowSelectSolder = -1;
		
		pStructure->m_Discount = lpOnResBarrackGetListOK->dwBuySolderDiscountPercent;
		
		CopyMemory(pStructure->m_SolderList, lpSolderData, sizeof(OnInnBuySolderInfo) * pStructure->m_MaxSelectSolder);
	}
	
	pStructure->m_NowWork        = BARRACK_NOWWORK_BUY_STARTPOOL;
	pStructure->m_StartTickCount = GetTickCount();

	return 0;
}

UI16 OnlineMsgProc::BarrackBuySolder(BYTE *pMsg)
{
	OnResBarrackBuySolder*			lpOnResBarrackBuySolder;
	OnResBarrackBuySolderOK*		lpOnResBarrackBuySolderOK;
	
	lpOnResBarrackBuySolder = (OnResBarrackBuySolder*)pMsg;

	OnlineVillageStructureBarrack *pStructure = (OnlineVillageStructureBarrack *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;

	if(lpOnResBarrackBuySolder->dwResponse == ON_RET_OK)
	{
		lpOnResBarrackBuySolderOK = (OnResBarrackBuySolderOK*)lpOnResBarrackBuySolder;
		
		// ĳ���� �߰�
		_pWorld->pMyData->InitFollower(lpOnResBarrackBuySolderOK->Slot, &lpOnResBarrackBuySolderOK->FollowerInfo);

		// �ڽ��� Money ����
		_pWorld->pMyData->SetMoney(lpOnResBarrackBuySolderOK->Money);
		
		// actdoll (2004/01/10 16:30) : ���� ����
		_pWorld->m_pMercenary->Refresh_Slot();
	
		// Next Menu
		pStructure->m_NowWork = BARRACK_NOWWORK_BUY_POOL;
		pStructure->m_NowSelectSolder = -1;
	}
	else
	{
		// Next Menu
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, _pWorld->pOnlineText->Get(ON_TEXT_BARRACK), _pWorld->pOnlineText->Get(ON_TEXT_BARRACK_SOLIDEREMPOLYFAIL));
		pStructure->m_NowWork = BARRACK_NOWWORK_BUY_POOL;
		pStructure->m_NowSelectSolder = -1;
	}
	return 0;
}

UI16 OnlineMsgProc::BarrackDismissSolderList(BYTE *pMsg)
{
	OnResBarrackGetDismissalList*		lpOnResBarrackGetDismissalList;
	OnResBarrackGetDismissalListOK*		lpOnResBarrackGetDismissalListOK;
	BYTE*								lpSolderData;

	lpOnResBarrackGetDismissalList = (OnResBarrackGetDismissalList*)pMsg;
	OnlineVillageStructureBarrack *pStructure = (OnlineVillageStructureBarrack *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;

	ZeroMemory(pStructure->m_DismissalSolderList, sizeof(OnDismissalSolderInfo) * MAX_BUYSOLDER_LIST);
	pStructure->m_DismissalMaxSelectSolder = 0;
	if(lpOnResBarrackGetDismissalList->dwResponse == ON_RET_OK)
	{
		lpOnResBarrackGetDismissalListOK = (OnResBarrackGetDismissalListOK*)lpOnResBarrackGetDismissalList;
		lpSolderData                     = (BYTE*)((BYTE*)lpOnResBarrackGetDismissalListOK + sizeof(OnResBarrackGetDismissalListOK));
		
		pStructure->m_DismissalMaxSelectSolder = lpOnResBarrackGetDismissalListOK->dwDismissalSolderCount;
		pStructure->m_DismissalNowSelectSolder = -1;
		
		CopyMemory(pStructure->m_DismissalSolderList, lpSolderData, sizeof(OnDismissalSolderInfo) * pStructure->m_DismissalMaxSelectSolder);
	}
	
	pStructure->m_NowWork        = BARRACK_NOWWORK_DISMISSAL_POOL;
	pStructure->m_StartTickCount = GetTickCount();

	return 0;
}

UI16 OnlineMsgProc::BarrackDismissSolder(BYTE *pMsg)
{
	OnResBarrackDismissalSolder*		lpOnResBarrackDismissalSolder;
	OnResBarrackDismissalSolderOK*		lpOnResBarrackDismissalSolderOK;

	// ������ ����
	lpOnResBarrackDismissalSolder = (OnResBarrackDismissalSolder*)pMsg;
	OnlineVillageStructureBarrack *pStructure = (OnlineVillageStructureBarrack *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;

	if(lpOnResBarrackDismissalSolder->dwResponse == ON_RET_OK)
	{
		lpOnResBarrackDismissalSolderOK = (OnResBarrackDismissalSolderOK*)lpOnResBarrackDismissalSolder;
		
		// ĳ���� ����
		_pWorld->pMyData->DeleteFollower(lpOnResBarrackDismissalSolderOK->Slot);
		
		// �ڽ��� Money ����
		_pWorld->pMyData->SetMoney(lpOnResBarrackDismissalSolderOK->Money);
		
		// actdoll (2004/01/10 16:30) : ���� ����
		_pWorld->m_pMercenary->Refresh_Slot			();
		_pWorld->m_pMercenary->Set_SelectedSlot		( 0 );	// �ڱ� �������� �����

		// actdoll (2004/03/22 11:42) : 
		_pWorld->pOnlineTradeBook->SetSelect		( 0 );
		
		_pWorld->pOnlineInventory->SetMercenarySlot	( 0 );
		_pWorld->pOnlineInventory->FillItemListBox	();
	
		// Next Menu
		_pWorld->pOnlineClient->SendRequestBarrackDismissalEnter();
		pStructure->m_NowWork                  = BARRACK_NOWWORK_DISMISSAL_GETLIST;
		pStructure->m_DismissalNowSelectSolder = -1;
		
		// Sound
		PushEffect(EFFECT_ON_DISMISS, 0, 0);
	}
	else
	{
		// Next Menu
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, _pWorld->pOnlineText->Get(ON_TEXT_BARRACK), _pWorld->pOnlineText->Get(ON_TEXT_BARRACK_SOLIDERFIREFAIL));
		pStructure->m_NowWork                  = BARRACK_NOWWORK_DISMISSAL_POOL;
		pStructure->m_DismissalNowSelectSolder = -1;
	}
	return 0;
}
// 2�� ���� ����缺�� ��븮��Ʈ
UI16	OnlineMsgProc::GBarrackBuyGeneralExList(BYTE *pMsg)						
{
	OnResGBarrackGetList*		lpOnResGBarrackGetList;
	OnResGBarrackGetListOK*		lpOnResGBarrackGetListOK;
	BYTE*						lpSolderData;

	lpOnResGBarrackGetList = (OnResGBarrackGetList*)pMsg;
	OnlineVillageStructureGBarrack *pStructure = (OnlineVillageStructureGBarrack *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;
	
	ZeroMemory(pStructure->m_BuySolderList, sizeof(OnInnBuySolderInfo) * MAX_BUYGSOLDER_LIST);
	pStructure->m_BuyMaxSelectSolder = 0;
	if(lpOnResGBarrackGetList->dwResponse == ON_RET_OK)
	{
		lpOnResGBarrackGetListOK = (OnResGBarrackGetListOK*)lpOnResGBarrackGetList;
		lpSolderData             = (BYTE*)((BYTE*)lpOnResGBarrackGetListOK + sizeof(OnResGBarrackGetListOK));

		pStructure->m_BuyMaxSelectSolder = lpOnResGBarrackGetListOK->dwBuySolderCount;
		pStructure->m_BuyNowSelectSolder = -1;
		
		pStructure->m_Discount = lpOnResGBarrackGetListOK->dwBuySolderDiscountPercent;
		
		CopyMemory(pStructure->m_BuySolderList, lpSolderData, sizeof(OnInnBuySolderInfo) * pStructure->m_BuyMaxSelectSolder);
	}

	pStructure->m_NowWork        = GBARRACK_NOWWORK_BUY_POOL;

	return 0;
}

// 2�� ���� ����缺�� �������
UI16	OnlineMsgProc::GBarrackBuyGeneralEx(BYTE *pMsg)
{
	OnResGBarrackBuySolder*			lpOnResGBarrackBuySolder;
	OnResGBarrackBuySolderOK*		lpOnResGBarrackBuySolderOK;

	// ������ ����
	lpOnResGBarrackBuySolder = (OnResGBarrackBuySolder*)pMsg;
	OnlineVillageStructureGBarrack *pStructure = (OnlineVillageStructureGBarrack *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;
	
	if(lpOnResGBarrackBuySolder->dwResponse == ON_RET_OK)
	{
		lpOnResGBarrackBuySolderOK = (OnResGBarrackBuySolderOK*)lpOnResGBarrackBuySolder;
		
		// ĳ���� �߰�
		_pWorld->pMyData->InitFollower(lpOnResGBarrackBuySolderOK->Slot, &lpOnResGBarrackBuySolderOK->FollowerInfo);
		
		// �ڽ��� Money ����
		_pWorld->pMyData->SetMoney(lpOnResGBarrackBuySolderOK->Money);
		
		// actdoll (2004/01/10 16:30) : ���� ����
		_pWorld->m_pMercenary->Refresh_Slot();
	
		// Next Menu
		pStructure->m_NowWork            = GBARRACK_NOWWORK_BUY_POOL;
		pStructure->m_BuyNowSelectSolder = -1;
	}
	else
	{
		// Next Menu
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, _pWorld->pOnlineText->Get(ON_TEXT_GBARRACK), _pWorld->pOnlineText->Get(ON_TEXT_BARRACK_SOLIDEREMPOLYFAIL));
		pStructure->m_NowWork            = GBARRACK_NOWWORK_BUY_POOL;
		pStructure->m_BuyNowSelectSolder = -1;
	}
	return 0;
}

// 2�� ���� ����缺�� �ذ���Ʈ
UI16	OnlineMsgProc::GBarrackDismissGeneralExList(BYTE *pMsg)
{
	OnResGBarrackGetDismissalList*		lpOnResGBarrackGetDismissalList;
	OnResGBarrackGetDismissalListOK*	lpOnResGBarrackGetDismissalListOK;
	BYTE*								lpSolderData;

	lpOnResGBarrackGetDismissalList = (OnResGBarrackGetDismissalList*)pMsg;
	OnlineVillageStructureGBarrack *pStructure = (OnlineVillageStructureGBarrack *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;

	ZeroMemory(pStructure->m_DismissalSolderList, sizeof(OnDismissalSolderInfo) * MAX_BUYGSOLDER_LIST);
	pStructure->m_DismissalMaxSelectSolder = 0;
	if(lpOnResGBarrackGetDismissalList->dwResponse == ON_RET_OK)
	{
		lpOnResGBarrackGetDismissalListOK = (OnResGBarrackGetDismissalListOK*)lpOnResGBarrackGetDismissalList;
		lpSolderData                      = (BYTE*)((BYTE*)lpOnResGBarrackGetDismissalListOK + sizeof(OnResGBarrackGetDismissalListOK));
		
		pStructure->m_DismissalMaxSelectSolder = lpOnResGBarrackGetDismissalListOK->dwDismissalSolderCount;
		pStructure->m_DismissalNowSelectSolder = -1;
		
		CopyMemory(pStructure->m_DismissalSolderList, lpSolderData, sizeof(OnDismissalSolderInfo) * pStructure->m_DismissalMaxSelectSolder);
	}
	
	pStructure->m_NowWork        = GBARRACK_NOWWORK_DISMISSAL_POOL;
	return 0;
}

// 2�� ���� ����缺�� �ذ�����
UI16	OnlineMsgProc::GBarrackDismissGeneralEx(BYTE *pMsg)
{
	OnResGBarrackDismissalSolder*		lpOnResGBarrackDismissalSolder;
	OnResGBarrackDismissalSolderOK*		lpOnResGBarrackDismissalSolderOK;

	// ������ ����
	lpOnResGBarrackDismissalSolder = (OnResGBarrackDismissalSolder*)pMsg;
	OnlineVillageStructureGBarrack *pStructure = (OnlineVillageStructureGBarrack *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;

	if(lpOnResGBarrackDismissalSolder->dwResponse == ON_RET_OK)
	{
		lpOnResGBarrackDismissalSolderOK = (OnResGBarrackDismissalSolderOK*)lpOnResGBarrackDismissalSolder;
		
		// ĳ���� �߰�
		_pWorld->pMyData->DeleteFollower(lpOnResGBarrackDismissalSolderOK->Slot);
		
		// �ڽ��� Money ����
		_pWorld->pMyData->SetMoney(lpOnResGBarrackDismissalSolderOK->Money);
		
		// actdoll (2004/01/10 16:30) : ���� ����
		_pWorld->m_pMercenary->Refresh_Slot			();
		_pWorld->m_pMercenary->Set_SelectedSlot		( 0 );	// �ڱ� �������� �����

		// actdoll (2004/03/22 11:42) : 
		_pWorld->pOnlineTradeBook->SetSelect		( 0 );

		_pWorld->pOnlineInventory->SetMercenarySlot	( 0 );
		_pWorld->pOnlineInventory->FillItemListBox	();
	
		// Next Menu
		_pWorld->pOnlineClient->SendRequestGBarrackDismissalGeneralExEnter();
		pStructure->m_NowWork                  = GBARRACK_NOWWORK_DISMISSAL_GETLIST;
		pStructure->m_DismissalNowSelectSolder = -1;
		
		// Sound
		PushEffect(EFFECT_ON_DISMISS, 0, 0);
	}
	else
	{
		// Next Menu
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, _pWorld->pOnlineText->Get(ON_TEXT_GBARRACK), _pWorld->pOnlineText->Get(ON_TEXT_BARRACK_SOLIDERFIREFAIL));
		pStructure->m_NowWork                  = GBARRACK_NOWWORK_DISMISSAL_POOL;
		pStructure->m_DismissalNowSelectSolder = -1;
	}
	return 0;
}


// 2�� ���� ����缺�� ���� ����Ʈ
UI16	OnlineMsgProc::GBarrackChangeGeneralExList(BYTE *pMsg)
{
	OnResGBarrackGetChangeJobList*		lpOnResGBarrackGetChangeJobList;
	OnResGBarrackGetChangeJobListOK*	lpOnResGBarrackGetChangeJobListOK;
	BYTE*								lpSolderData;

	lpOnResGBarrackGetChangeJobList = (OnResGBarrackGetChangeJobList*)pMsg;
	OnlineVillageStructureGBarrack *pStructure = (OnlineVillageStructureGBarrack *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;
	
	ZeroMemory(pStructure->m_ChangeJobSolderList, sizeof(OnChangeJobSolderInfo) * MAX_BUYGSOLDER_LIST);
	pStructure->m_ChangeJobMaxSelectSolder = 0;
	if(lpOnResGBarrackGetChangeJobList->dwResponse == ON_RET_OK)
	{
		lpOnResGBarrackGetChangeJobListOK = (OnResGBarrackGetChangeJobListOK*)lpOnResGBarrackGetChangeJobList;
		lpSolderData                      = (BYTE*)((BYTE*)lpOnResGBarrackGetChangeJobListOK + sizeof(OnResGBarrackGetChangeJobListOK));
		
		pStructure->m_ChangeJobMaxSelectSolder = lpOnResGBarrackGetChangeJobListOK->dwChangeJobSolderCount;
		pStructure->m_ChangeJobNowSelectSolder = -1;
		
		CopyMemory(pStructure->m_ChangeJobSolderList, lpSolderData, sizeof(OnChangeJobSolderInfo) * pStructure->m_ChangeJobMaxSelectSolder);
	}
	
	pStructure->m_NowWork        = GBARRACK_NOWWORK_CHANGEJOB_POOL;
	
	return 0;
}

// 2�� ���� ����缺�� ���� ����
UI16	OnlineMsgProc::GBarrackChangeGeneralEx(BYTE *pMsg)						
{
	OnResGBarrackChangeJobSolder*		lpOnResGBarrackChangeJobSolder;
	OnResGBarrackChangeJobSolderOK*		lpOnResGBarrackChangeJobSolderOK;

	lpOnResGBarrackChangeJobSolder = (OnResGBarrackChangeJobSolder*)pMsg;
	OnlineVillageStructureGBarrack *pStructure = (OnlineVillageStructureGBarrack *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;

	if(lpOnResGBarrackChangeJobSolder->dwResponse == ON_RET_OK)
	{
		lpOnResGBarrackChangeJobSolderOK = (OnResGBarrackChangeJobSolderOK*)lpOnResGBarrackChangeJobSolder;
		
		// ĳ���� ����
		_pWorld->pMyData->UpdateFollower(lpOnResGBarrackChangeJobSolderOK->Slot, &lpOnResGBarrackChangeJobSolderOK->FollowerInfo);

		// actdoll (2004/01/10 16:30) : ���� ����
		_pWorld->m_pMercenary->Refresh_Slot();
	
		// Next Menu
		_pWorld->pOnlineClient->SendRequestGBarrackChangeGeneralExEnter();
		pStructure->m_NowWork                  = GBARRACK_NOWWORK_CHANGEJOB_GETLIST;
		pStructure->m_ChangeJobNowSelectSolder = -1;
	}
	else
	{
		// Next Menu
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, _pWorld->pOnlineText->Get(ON_TEXT_GBARRACK), _pWorld->pOnlineText->Get(ON_TEXT_BARRACK_SOLIDEREMPOLYFAIL));
		pStructure->m_NowWork                  = GBARRACK_NOWWORK_CHANGEJOB_POOL;
		pStructure->m_ChangeJobNowSelectSolder = -1;
	}
	return 0;
}


UI16 OnlineMsgProc::GBarrackBuySolderList(BYTE *pMsg)
{
	OnResGBarrackGetList*		lpOnResGBarrackGetList;
	OnResGBarrackGetListOK*		lpOnResGBarrackGetListOK;
	BYTE*						lpSolderData;

	lpOnResGBarrackGetList = (OnResGBarrackGetList*)pMsg;
	OnlineVillageStructureGBarrack *pStructure = (OnlineVillageStructureGBarrack *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;
	
	ZeroMemory(pStructure->m_BuySolderList, sizeof(OnInnBuySolderInfo) * MAX_BUYGSOLDER_LIST);
	pStructure->m_BuyMaxSelectSolder = 0;
	if(lpOnResGBarrackGetList->dwResponse == ON_RET_OK)
	{
		lpOnResGBarrackGetListOK = (OnResGBarrackGetListOK*)lpOnResGBarrackGetList;
		lpSolderData             = (BYTE*)((BYTE*)lpOnResGBarrackGetListOK + sizeof(OnResGBarrackGetListOK));
		
		pStructure->m_BuyMaxSelectSolder = lpOnResGBarrackGetListOK->dwBuySolderCount;
		pStructure->m_BuyNowSelectSolder = -1;
		
		pStructure->m_Discount = lpOnResGBarrackGetListOK->dwBuySolderDiscountPercent;
		
		CopyMemory(pStructure->m_BuySolderList, lpSolderData, sizeof(OnInnBuySolderInfo) * pStructure->m_BuyMaxSelectSolder);
	}
	
	pStructure->m_NowWork        = GBARRACK_NOWWORK_BUY_STARTPOOL;

	return 0;
}

UI16 OnlineMsgProc::GBarrackBuySolder(BYTE *pMsg)
{
	OnResGBarrackBuySolder*			lpOnResGBarrackBuySolder;
	OnResGBarrackBuySolderOK*		lpOnResGBarrackBuySolderOK;

	// ������ ����
	lpOnResGBarrackBuySolder = (OnResGBarrackBuySolder*)pMsg;
	OnlineVillageStructureGBarrack *pStructure = (OnlineVillageStructureGBarrack *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;
	
	if(lpOnResGBarrackBuySolder->dwResponse == ON_RET_OK)
	{
		lpOnResGBarrackBuySolderOK = (OnResGBarrackBuySolderOK*)lpOnResGBarrackBuySolder;
		
		// ĳ���� �߰�
		_pWorld->pMyData->InitFollower(lpOnResGBarrackBuySolderOK->Slot, &lpOnResGBarrackBuySolderOK->FollowerInfo);
		
		// �ڽ��� Money ����
		_pWorld->pMyData->SetMoney(lpOnResGBarrackBuySolderOK->Money);
		
		// actdoll (2004/01/10 16:30) : ���� ����
		_pWorld->m_pMercenary->Refresh_Slot();
	
		// Next Menu
		pStructure->m_NowWork            = GBARRACK_NOWWORK_BUY_POOL;
		pStructure->m_BuyNowSelectSolder = -1;
	}
	else
	{
		// Next Menu
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, _pWorld->pOnlineText->Get(ON_TEXT_GBARRACK), _pWorld->pOnlineText->Get(ON_TEXT_BARRACK_SOLIDEREMPOLYFAIL));
		pStructure->m_NowWork            = GBARRACK_NOWWORK_BUY_POOL;
		pStructure->m_BuyNowSelectSolder = -1;
	}
	return 0;
}

UI16 OnlineMsgProc::GBarrackDismissSolderList(BYTE *pMsg)
{
	OnResGBarrackGetDismissalList*		lpOnResGBarrackGetDismissalList;
	OnResGBarrackGetDismissalListOK*	lpOnResGBarrackGetDismissalListOK;
	BYTE*								lpSolderData;

	lpOnResGBarrackGetDismissalList = (OnResGBarrackGetDismissalList*)pMsg;
	OnlineVillageStructureGBarrack *pStructure = (OnlineVillageStructureGBarrack *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;

	ZeroMemory(pStructure->m_DismissalSolderList, sizeof(OnDismissalSolderInfo) * MAX_BUYGSOLDER_LIST);
	pStructure->m_DismissalMaxSelectSolder = 0;
	if(lpOnResGBarrackGetDismissalList->dwResponse == ON_RET_OK)
	{
		lpOnResGBarrackGetDismissalListOK = (OnResGBarrackGetDismissalListOK*)lpOnResGBarrackGetDismissalList;
		lpSolderData                      = (BYTE*)((BYTE*)lpOnResGBarrackGetDismissalListOK + sizeof(OnResGBarrackGetDismissalListOK));
		
		pStructure->m_DismissalMaxSelectSolder = lpOnResGBarrackGetDismissalListOK->dwDismissalSolderCount;
		pStructure->m_DismissalNowSelectSolder = -1;
		
		CopyMemory(pStructure->m_DismissalSolderList, lpSolderData, sizeof(OnDismissalSolderInfo) * pStructure->m_DismissalMaxSelectSolder);
	}
	
	pStructure->m_NowWork        = GBARRACK_NOWWORK_DISMISSAL_POOL;
	return 0;
}

UI16 OnlineMsgProc::GBarrackDismissSolder(BYTE *pMsg)
{
	OnResGBarrackDismissalSolder*		lpOnResGBarrackDismissalSolder;
	OnResGBarrackDismissalSolderOK*		lpOnResGBarrackDismissalSolderOK;

	// ������ ����
	lpOnResGBarrackDismissalSolder = (OnResGBarrackDismissalSolder*)pMsg;
	OnlineVillageStructureGBarrack *pStructure = (OnlineVillageStructureGBarrack *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;

	if(lpOnResGBarrackDismissalSolder->dwResponse == ON_RET_OK)
	{
		lpOnResGBarrackDismissalSolderOK = (OnResGBarrackDismissalSolderOK*)lpOnResGBarrackDismissalSolder;
		
		// ĳ���� �߰�
		_pWorld->pMyData->DeleteFollower(lpOnResGBarrackDismissalSolderOK->Slot);
		
		// �ڽ��� Money ����
		_pWorld->pMyData->SetMoney(lpOnResGBarrackDismissalSolderOK->Money);
		
		// actdoll (2004/01/10 16:30) : ���� ����
		_pWorld->m_pMercenary->Refresh_Slot			();
		_pWorld->m_pMercenary->Set_SelectedSlot		( 0 );	// �ڱ� �������� �����

		// actdoll (2004/03/22 11:42) : 
		_pWorld->pOnlineTradeBook->SetSelect		( 0 );

		_pWorld->pOnlineInventory->SetMercenarySlot	( 0 );
		_pWorld->pOnlineInventory->FillItemListBox	();
	
		// Next Menu
		_pWorld->pOnlineClient->SendRequestGBarrackDismissalEnter();
		pStructure->m_NowWork                  = GBARRACK_NOWWORK_DISMISSAL_GETLIST;
		pStructure->m_DismissalNowSelectSolder = -1;
		
		// Sound
		PushEffect(EFFECT_ON_DISMISS, 0, 0);
	}
	else
	{
		// Next Menu
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, _pWorld->pOnlineText->Get(ON_TEXT_GBARRACK), _pWorld->pOnlineText->Get(ON_TEXT_BARRACK_SOLIDERFIREFAIL));
		pStructure->m_NowWork                  = GBARRACK_NOWWORK_DISMISSAL_POOL;
		pStructure->m_DismissalNowSelectSolder = -1;
	}
	return 0;
}

UI16 OnlineMsgProc::GBarrackChangjobSolderList(BYTE *pMsg)
{
	OnResGBarrackGetChangeJobList*		lpOnResGBarrackGetChangeJobList;
	OnResGBarrackGetChangeJobListOK*	lpOnResGBarrackGetChangeJobListOK;
	BYTE*								lpSolderData;

	lpOnResGBarrackGetChangeJobList = (OnResGBarrackGetChangeJobList*)pMsg;
	OnlineVillageStructureGBarrack *pStructure = (OnlineVillageStructureGBarrack *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;
	
	ZeroMemory(pStructure->m_ChangeJobSolderList, sizeof(OnChangeJobSolderInfo) * MAX_BUYGSOLDER_LIST);
	pStructure->m_ChangeJobMaxSelectSolder = 0;
	if(lpOnResGBarrackGetChangeJobList->dwResponse == ON_RET_OK)
	{
		lpOnResGBarrackGetChangeJobListOK = (OnResGBarrackGetChangeJobListOK*)lpOnResGBarrackGetChangeJobList;
		lpSolderData                      = (BYTE*)((BYTE*)lpOnResGBarrackGetChangeJobListOK + sizeof(OnResGBarrackGetChangeJobListOK));
		
		pStructure->m_ChangeJobMaxSelectSolder = lpOnResGBarrackGetChangeJobListOK->dwChangeJobSolderCount;
		pStructure->m_ChangeJobNowSelectSolder = -1;
		
		CopyMemory(pStructure->m_ChangeJobSolderList, lpSolderData, sizeof(OnChangeJobSolderInfo) * pStructure->m_ChangeJobMaxSelectSolder);
	}
	
	pStructure->m_NowWork        = GBARRACK_NOWWORK_CHANGEJOB_POOL;
	pStructure->m_StartTickCount = GetTickCount();
	
	return 0;
}

UI16 OnlineMsgProc::GBarrackChangjobSolder(BYTE *pMsg)
{
	OnResGBarrackChangeJobSolder*		lpOnResGBarrackChangeJobSolder;
	OnResGBarrackChangeJobSolderOK*		lpOnResGBarrackChangeJobSolderOK;

	lpOnResGBarrackChangeJobSolder = (OnResGBarrackChangeJobSolder*)pMsg;
	OnlineVillageStructureGBarrack *pStructure = (OnlineVillageStructureGBarrack *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;

	if(lpOnResGBarrackChangeJobSolder->dwResponse == ON_RET_OK)
	{
		lpOnResGBarrackChangeJobSolderOK = (OnResGBarrackChangeJobSolderOK*)lpOnResGBarrackChangeJobSolder;
		
		// ĳ���� ����
		_pWorld->pMyData->UpdateFollower(lpOnResGBarrackChangeJobSolderOK->Slot, &lpOnResGBarrackChangeJobSolderOK->FollowerInfo);

		// actdoll (2004/01/10 16:30) : ���� ����
		_pWorld->m_pMercenary->Refresh_Slot();
	
		// Next Menu
		_pWorld->pOnlineClient->SendRequestGBarrackChangeJobEnter();
		pStructure->m_NowWork                  = GBARRACK_NOWWORK_CHANGEJOB_GETLIST;
		pStructure->m_ChangeJobNowSelectSolder = -1;
	}
	else
	{
		// Next Menu
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, _pWorld->pOnlineText->Get(ON_TEXT_GBARRACK), _pWorld->pOnlineText->Get(ON_TEXT_BARRACK_SOLIDEREMPOLYFAIL));
		pStructure->m_NowWork                  = GBARRACK_NOWWORK_CHANGEJOB_POOL;
		pStructure->m_ChangeJobNowSelectSolder = -1;
	}
	return 0;
}

UI16 OnlineMsgProc::MBarrackBuyMonsterList(BYTE *pMsg)
{
	OnResMBarrackGetList*		lpOnResBarrackGetList;
	OnResMBarrackGetListOK*		lpOnResBarrackGetListOK;
	BYTE*						lpSolderData;

	lpOnResBarrackGetList = (OnResMBarrackGetList*)pMsg;
	OnlineVillageStructureMBarrack *pStructure = (OnlineVillageStructureMBarrack *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;

	ZeroMemory(pStructure->m_SolderList, sizeof(OnInnBuyMonsterInfo) * MAX_BUYSOLDER_LIST);
	pStructure->m_MaxSelectSolder = 0;
	if(lpOnResBarrackGetList->dwResponse == ON_RET_OK)
	{
		lpOnResBarrackGetListOK = (OnResMBarrackGetListOK*)lpOnResBarrackGetList;
		lpSolderData            = (BYTE*)((BYTE*)lpOnResBarrackGetListOK + sizeof(OnResMBarrackGetListOK));
		
		pStructure->m_MaxSelectSolder = lpOnResBarrackGetListOK->dwBuyMonsterCount;
		pStructure->m_NowSelectSolder = -1;
		
		pStructure->m_Discount = lpOnResBarrackGetListOK->dwBuyMonsterDiscountPercent;
		
		CopyMemory(pStructure->m_SolderList, lpSolderData, sizeof(OnInnBuyMonsterInfo) * pStructure->m_MaxSelectSolder);
	}
	
	pStructure->m_NowWork        = MBARRACK_NOWWORK_BUY_STARTPOOL;
	pStructure->m_StartTickCount = GetTickCount();
	
	return 0;
}

UI16 OnlineMsgProc::MBarrackBuyMonster(BYTE *pMsg)
{
	OnResMBarrackBuyMonster*		lpOnResBarrackBuySolder;
	OnResMBarrackBuyMonsterOK*		lpOnResBarrackBuySolderOK;

	// ������ ����
	lpOnResBarrackBuySolder = (OnResMBarrackBuyMonster*)pMsg;
	OnlineVillageStructureMBarrack *pStructure = (OnlineVillageStructureMBarrack *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;

	if(lpOnResBarrackBuySolder->dwResponse == ON_RET_OK)
	{
		lpOnResBarrackBuySolderOK = (OnResMBarrackBuyMonsterOK*)lpOnResBarrackBuySolder;
		
		// ĳ���� �߰�
		_pWorld->pMyData->InitFollower(lpOnResBarrackBuySolderOK->Slot, &lpOnResBarrackBuySolderOK->FollowerInfo);
		
		// ������ ����
		MyItemData*		lpItemList = _pWorld->pMyData->GetItemList(0);
		CItemHeader*	pItemHeader;
		
		for(SI32 TempCount = 0; TempCount < ON_MYDATA_MAX_ITEM_NUM; TempCount++)
		{
			if(lpItemList[TempCount].uiID != 0)
			{
				pItemHeader = _pWorld->pItemsInfo->GetItem(lpItemList[TempCount].uiID);
				
				if(pItemHeader)
				{
					if((pItemHeader->m_siGroupCode & ON_ITEM_SPECIAL) && (pItemHeader->m_siSpecial == ON_ITEM_SPECIAL_SEALINGBEAD))
					{
						lpItemList[TempCount].uiQuantity -= pStructure->m_DeleteItemCount;
						if(lpItemList[TempCount].uiQuantity == 0)
						{
							lpItemList[TempCount].uiID             = 0;
							lpItemList[TempCount].uiQuantity       = 0;
							lpItemList[TempCount].siPosInInventory = 0;
						}
						break;
					}
				}
			}
		}
		
		// �ڽ��� Money ����
		_pWorld->pMyData->SetMoney(lpOnResBarrackBuySolderOK->Money);
		
		// actdoll (2004/01/10 16:30) : ���� ����
		_pWorld->m_pMercenary->Refresh_Slot();
	
		// Next Menu
		pStructure->m_NowWork         = MBARRACK_NOWWORK_BUY_POOL;
		pStructure->m_NowSelectSolder = -1;
	}
	else
	{
		// Next Menu
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, _pWorld->pOnlineText->Get(ON_TEXT_MBARRACK), _pWorld->pOnlineText->Get(ON_TEXT_BARRACK_SOLIDEREMPOLYFAIL));
		pStructure->m_NowWork         = MBARRACK_NOWWORK_BUY_POOL;
		pStructure->m_NowSelectSolder = -1;
	}
	return 0;
}

UI16 OnlineMsgProc::MBarrackDismissMonsterList(BYTE *pMsg)
{
	OnResMBarrackGetDismissList*		lpOnResBarrackGetDismissalList;
	OnResMBarrackGetDismissListOK*		lpOnResBarrackGetDismissalListOK;
	BYTE*								lpSolderData;

	lpOnResBarrackGetDismissalList = (OnResMBarrackGetDismissList*)pMsg;
	OnlineVillageStructureMBarrack *pStructure = (OnlineVillageStructureMBarrack *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;

	ZeroMemory(pStructure->m_DismissalSolderList, sizeof(OnDismissalSolderInfo) * MAX_BUYSOLDER_LIST);
	pStructure->m_DismissalMaxSelectSolder = 0;
	if(lpOnResBarrackGetDismissalList->dwResponse == ON_RET_OK)
	{
		lpOnResBarrackGetDismissalListOK = (OnResMBarrackGetDismissListOK*)lpOnResBarrackGetDismissalList;
		lpSolderData                     = (BYTE*)((BYTE*)lpOnResBarrackGetDismissalListOK + sizeof(OnResMBarrackGetDismissListOK));
		
		pStructure->m_DismissalMaxSelectSolder = lpOnResBarrackGetDismissalListOK->dwDismissMonsterCount;
		pStructure->m_DismissalNowSelectSolder = -1;
		
		CopyMemory(pStructure->m_DismissalSolderList, lpSolderData, sizeof(OnDismissalSolderInfo) * pStructure->m_DismissalMaxSelectSolder);
	}
	
	pStructure->m_NowWork        = MBARRACK_NOWWORK_DISMISSAL_POOL;
	pStructure->m_StartTickCount = GetTickCount();

	return 0;
}

UI16 OnlineMsgProc::MBarrackDismissMonster(BYTE *pMsg)
{
	OnResMBarrackDismissMonster*		lpOnResBarrackDismissalSolder;
	OnResMBarrackDismissMonsterOK*		lpOnResBarrackDismissalSolderOK;

	// ������ ����
	lpOnResBarrackDismissalSolder = (OnResMBarrackDismissMonster*)pMsg;
	OnlineVillageStructureMBarrack *pStructure = (OnlineVillageStructureMBarrack *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;

	if(lpOnResBarrackDismissalSolder->dwResponse == ON_RET_OK)
	{
		lpOnResBarrackDismissalSolderOK = (OnResMBarrackDismissMonsterOK*)lpOnResBarrackDismissalSolder;
		
		// ĳ���� �߰�
		_pWorld->pMyData->DeleteFollower(lpOnResBarrackDismissalSolderOK->Slot);
		
		// �ڽ��� Money ����
		_pWorld->pMyData->SetMoney(lpOnResBarrackDismissalSolderOK->Money);
		
		// actdoll (2004/01/10 16:30) : ���� ����
		_pWorld->m_pMercenary->Refresh_Slot			();
		_pWorld->m_pMercenary->Set_SelectedSlot		( 0 );	// �ڱ� �������� �����

		// actdoll (2004/03/22 11:42) : 
		_pWorld->pOnlineTradeBook->SetSelect		( 0 );

		_pWorld->pOnlineInventory->SetMercenarySlot	( 0 );
		_pWorld->pOnlineInventory->FillItemListBox	();
	
		// Next Menu
		_pWorld->pOnlineClient->SendRequestMBarrackDismissalEnter();
		pStructure->m_NowWork                  = MBARRACK_NOWWORK_DISMISSAL_GETLIST;
		pStructure->m_DismissalNowSelectSolder = -1;
		
		// Sound
		PushEffect(EFFECT_ON_DISMISS, 0, 0);
	}
	else
	{
		// Next Menu
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, _pWorld->pOnlineText->Get(ON_TEXT_MBARRACK), _pWorld->pOnlineText->Get(ON_TEXT_MBARRACK_SOLIDERFIREFAIL));
		pStructure->m_NowWork                  = MBARRACK_NOWWORK_DISMISSAL_POOL;
		pStructure->m_DismissalNowSelectSolder = -1;
	}
	return 0;
}

UI16 OnlineMsgProc::InnItemList(BYTE *pMsg)
{
	OnResItemListMsg *pOnResItemListMsg = (OnResItemListMsg*)pMsg;
	OnlineVillageStructureInn *pStructure = (OnlineVillageStructureInn *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;
	
	if(!pOnResItemListMsg->uiCount)		
	{
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", _pWorld->pOnlineText->Get(ON_TEXT_INN_NO_ARRIVED_ITEM));
		_pWorld->m_siInnStatus = -1;
		pStructure->m_siMainStatus = ON_INN_STATUS_MAIN_POLL;
		return 0;
	}

	pStructure->GetList()->Clear();
	_pWorld->m_siInnStatus = 1;

	BYTE *pCurrPtr = (BYTE *)pOnResItemListMsg + 6;

	for(int i = 0; i < pOnResItemListMsg->uiCount; i++)
	{
		UI16 TempItemID, TempQuantity;

		memcpy(&TempItemID, pCurrPtr, 2 );
		pCurrPtr += 2;

		memcpy(&TempQuantity, pCurrPtr, 2 );
		pCurrPtr += 2;
		
		ItemInfo TempItemInfo;
		TempItemInfo.uiItemID	= TempItemID;
		TempItemInfo.ItemCount	= TempQuantity;
		
		pStructure->GetList()->AddItem(TempItemInfo, i);
	}

	// ������ ������ ����Ʈ�� �޾����Ƿ� �˸�ǥ�ø� �����Ѵ�.
	_pWorld->m_bReceivedItem = FALSE;

	if(!_pWorld->pOnlineInventory->IsActive())	_pWorld->pOnlineInventory->SetActive(TRUE);
	
	pStructure->m_StartTickCount = GetTickCount();
	pStructure->m_siStatus = ON_INN_STATUS_GET_ITEM_POLL;
	return 0;
}

UI16 OnlineMsgProc::InnTakeoutItem(BYTE *pMsg)
{
	OnResTakeOutItemMsg *pOnResTakeOutItemMsg = (OnResTakeOutItemMsg*)pMsg;
	OnlineVillageStructureInn *pStructure = (OnlineVillageStructureInn *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;

	_pWorld->m_bItemDraging = FALSE;
	pStructure->GetList()->StopDrag();
	
	if(pOnResTakeOutItemMsg->dwResponse == ON_RET_OK)
	{
		// ������ �κ��丮�� �߰�
		MyItemData MyItemData;
		MyItemData.siPosInInventory	=	pStructure->m_siInvenSlot;
		MyItemData.uiID				=	pStructure->m_uiItemID;
		MyItemData.uiQuantity		=	pStructure->m_uiQuantity;				
		_pWorld->pMyData->AddItem(pStructure->m_uiFollowerID, &MyItemData);
		_pWorld->m_pMercenary->Set_SelectedSlot( pStructure->m_uiFollowerID );	// actdoll (2004/01/02 18:42)
		_pWorld->pOnlineInventory->SetMercenarySlot(pStructure->m_uiFollowerID);
		_pWorld->pOnlineInventory->FillItemListBox();
		
		// ���� �κ��丮���� �ű� ������ ����
		pStructure->GetList()->DeleteSlot(pStructure->m_siInnSlot, pStructure->m_uiQuantity);
		_pWorld->m_siReceivedItemCount = pStructure->GetList()->GetItemNum();
		
		pStructure->m_siStatus = ON_INN_STATUS_GET_ITEM_POLL;
	}
	else
	{
		// ������ �ޱ� ����
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", _pWorld->pOnlineText->Get(ON_TEXT_INN_CANNOT_RECEIVE_ITEM));
		pStructure->m_siStatus = ON_INN_STATUS_GET_ITEM_POLL;
	}
	return 0;
}

UI16 OnlineMsgProc::InnSaveItem(BYTE *pMsg)
{
	OnResSaveItemMsg *pOnResSaveItemMsg = (OnResSaveItemMsg*)pMsg;
	OnlineVillageStructureInn *pStructure = (OnlineVillageStructureInn *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;

	_pWorld->pOnlineInventory->DragFail();
	
	if(pOnResSaveItemMsg->dwResponse == ON_RET_OK)
	{
		// �κ��丮���� ����
		MyItemData MyItemData;
		MyItemData.siPosInInventory	=	pStructure->m_siInvenSlot;
		MyItemData.uiID				=	pStructure->m_uiItemID;
		MyItemData.uiQuantity		=	pStructure->m_uiQuantity;
		
		_pWorld->pMyData->DelItem(pStructure->m_uiFollowerID, &MyItemData);
		_pWorld->pOnlineInventory->FillItemListBox();
		
		// ���� �κ��丮�� �ű� ������ �߰�
		ItemInfo TempItemInfo;
		TempItemInfo.uiItemID	= pStructure->m_uiItemID;
		TempItemInfo.ItemCount	= pStructure->m_uiQuantity;
		SI16 siSlot				= pStructure->GetList()->GetEmptySlot(pStructure->m_uiItemID);
		if(pStructure->GetList()->GetSlotInfo(siSlot))
			TempItemInfo.ItemCount	+= pStructure->GetList()->GetSlotInfo(siSlot)->ItemCount;
		pStructure->GetList()->AddItem(TempItemInfo, siSlot);

		// ������ �ΰ�
		_pWorld->pMyData->SetMoney(_pWorld->pMyData->GetMoney() - ON_INN_SEND_ITEM_COMMISSION);

		_pWorld->m_siReceivedItemCount = pStructure->GetList()->GetItemNum();
		pStructure->m_siStatus = ON_INN_STATUS_DEPOSIT_FOLLOWER_POLL;
	}
	else
	{
		// ������ ������ ����
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", _pWorld->pOnlineText->Get(ON_TEXT_INN_CANNOT_SEND_ITEM));
		pStructure->m_siStatus = ON_INN_STATUS_DEPOSIT_FOLLOWER_POLL;
	}
	return 0;
}

UI16 OnlineMsgProc::MarketChangeIteminfo(BYTE *pMsg)
{
	OnChangeItemInfoMsg	*pChangedItemInfo = (OnChangeItemInfoMsg*)pMsg;
	OnlineVillageStructureMarket *pStructure = (OnlineVillageStructureMarket *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;

	pStructure->ModifyData(pChangedItemInfo);
	return 0;
}

UI16 OnlineMsgProc::MarketEnter(BYTE *pMsg)
{
	OnResEnterMarketMsg		*pEnterMarketMsg = (OnResEnterMarketMsg*)pMsg;
	OnlineVillageStructureMarket *pStructure = (OnlineVillageStructureMarket *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;
	
	if(pEnterMarketMsg->dwResponse == ON_RET_MARKET_OK)
	{					
		// ������ ����Ʈ �޶�� �������� ��û�Ѵ�.
		_pWorld->pOnlineClient->SendRequestItemList();
		pStructure->siStatus = ON_MARKET_STATUS_RECEIVE_ITEMLIST;
	}				
	else
	{
		// ���忡 �� �� ����.
		pStructure->bExit =	TRUE; 
	}
	return 0;
}

UI16 OnlineMsgProc::MarketItemList(BYTE *pMsg)
{
	OnResItemListInMarketMsg *pReqItemListInMarketMsg =	(OnResItemListInMarketMsg*)pMsg;
	OnlineVillageStructureMarket *pStructure = (OnlineVillageStructureMarket *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;

	OnlineReceivedItemInfo	*pRecvItemInfo;
	pRecvItemInfo =	(OnlineReceivedItemInfo*)((CHAR*)pReqItemListInMarketMsg + sizeof(OnResItemListInMarketMsg));
	
	// �������� ���� �������� ������ �÷��̾� ���� ����
	if(!_pWorld->pOnlineInventory->IsActive())	_pWorld->pOnlineInventory->SetActive( TRUE );
	pStructure->InsertData((SI32)pReqItemListInMarketMsg->dwItemNum, pRecvItemInfo );
	pStructure->siStatus = ON_MARKET_STATUS_RECEIVE_POLL;
	
	// �ǹ� ������ ���� �ʻ�ȭ �̹��� ����
	switch(_pWorld->pOnlineVillage->pVillageStructManager->GetEnterStructureKind())
	{
	case ON_VIL_ST_KIND_MARKET:		pStructure->siDealerKind = 0;	break;
	case ON_VIL_ST_KIND_WSTORE:		pStructure->siDealerKind = 1;	break;
	case ON_VIL_ST_KIND_FSTORE:		pStructure->siDealerKind = 2;	break;
	case ON_VIL_ST_KIND_DSTORE:		pStructure->siDealerKind = 3;	break;
	case ON_VIL_ST_KIND_RING:		pStructure->siDealerKind = 2;	break;
	default:						pStructure->siDealerKind = 0;	break;
	}
	return 0;
}

UI16 OnlineMsgProc::MarketSellItem(BYTE *pMsg)
{
	OnResSellItemMsg *pOnResSellItemMsg = (OnResSellItemMsg*)pMsg;
	OnlineVillageStructureMarket *pStructure = (OnlineVillageStructureMarket *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;
	
	_pWorld->m_pMercenary->NoAction(FALSE);
	if( pOnResSellItemMsg->dwResponse == ON_RET_OK )
	{
		OnResSellItemOkMsg	*pOnResSellItemOkMsg;
		pOnResSellItemOkMsg = (OnResSellItemOkMsg*)pOnResSellItemMsg;

		SI16 siSellSlot = pStructure->pPlayerLC->GetSlot(pStructure->uiID);

		MyItemData	MyItemData;
		MyItemData.siPosInInventory =	siSellSlot;
		MyItemData.uiID				=	pStructure->uiID;
		MyItemData.uiQuantity		=	_pWorld->pOnlineNumPad->siCount;					
		
		// ���� ����Ÿ���� �������� �����ش�.
		pStructure->pPlayerLC->DeleteSlot(MyItemData.siPosInInventory, _pWorld->pOnlineNumPad->siCount);
		_pWorld->pMyData->DelItem(pStructure->uiCurFollower, &MyItemData );

		// ������ �����Ѵ�.
		ItemInfo TempItemInfo;
		pStructure->pPlayerLC->CopyData(siSellSlot, TempItemInfo);
		UI32 uiSlot = pStructure->pNpcLC->GetSlot(pStructure->uiID);
		if(uiSlot)
		{
			if(pStructure->pNpcLC->GetSlotInfo(uiSlot))
			{
				TempItemInfo.uiSellPrice = pStructure->pNpcLC->GetSlotInfo(uiSlot)->uiSellPrice;
				pStructure->pPlayerLC->AddItem(TempItemInfo, siSellSlot);
			}
		}

		// ���� ���� �ſ��� ���� �Ѵ�.
		_pWorld->pMyData->SetTradeCredit( pOnResSellItemOkMsg->uiTradeCredit, 0 );
		_pWorld->pMyData->SetMoney( pOnResSellItemOkMsg->siMoney );
		
		// ������ �������� �ٽ� �ѹ� ������ �ش�.(���� �ſ뵵�� ����ʿ� ����)
		ItemInfo TempInfo;
		CItemHeader *pItem;
		for( SI16 i=0; i<pStructure->pNpcLC->GetItemNum(); i++ )
		{
			pStructure->pNpcLC->CopyData(i, TempInfo);
			pItem = _pWorld->pItemsInfo->GetItem(TempInfo.uiItemID);
			if(pItem) TempInfo.siColor = _pWorld->pMyData->GetTradeGrade() < pItem->m_uiTradeGrade ? LIST_COLOR_PINK : LIST_COLOR_NOT;
		}

		pStructure->siStatus	=	ON_MARKET_STATUS_RECEIVE_POLL;
		pStructure->siDragItem	=	-1;
		
		_pWorld->pOnlineNumPad->pNumberControl->Initialize();
		
		// �ŷ� OK ���
		pStructure->NowTradesmanTalk = ON_TEXT_ITEMHELP_TRADESMAN_AFTERTRADE;
	}
	else if( pOnResSellItemMsg->dwResponse == ON_RET_NO )																										// ������ �ȱ� ����
	{
		pStructure->siDragItem	=	-1;
		pStructure->siStatus	=	ON_MARKET_STATUS_RECEIVE_POLL;
		
		_pWorld->pOnlineNumPad->pNumberControl->Initialize();
	}
	else
	{
		pStructure->siDragItem	=	-1;
		pStructure->siStatus	=	ON_MARKET_STATUS_RECEIVE_POLL;
		clGrp.Error("VillageStruct-Market", _pWorld->pOnlineText->Get(ON_TEXT_MAKE_UNKNOWMESSAGE), pOnResSellItemMsg->dwResponse);
	}
	return 0;
}

UI16 OnlineMsgProc::MarketBuyItem(BYTE *pMsg)
{
	OnResBuyItemMsg		*pResBuyItemMsg = (OnResBuyItemMsg*)pMsg;
	OnlineVillageStructureMarket *pStructure = (OnlineVillageStructureMarket *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;

	_pWorld->m_pMercenary->NoAction(FALSE);
	
	if( pResBuyItemMsg->dwResponse == ON_RET_OK )						// �������� �������� ��� �ִٴ� �޽���
	{	
		OnResBuyItemOkMsg		*pOnResBuyItemOkMsg;
		const ItemInfo			*pSlot;					
		
		pOnResBuyItemOkMsg =	(OnResBuyItemOkMsg*)pResBuyItemMsg;
		SI16 siSlot 	   =	pStructure->pNpcLC->GetSlot(pStructure->uiID);
		pSlot			   =	pStructure->pNpcLC->GetSlotInfo( siSlot );
		
		// ���� ����Ÿ�� �ʾ� �ش�.
		MyItemData	MyItemData;
		MyItemData.siPosInInventory	=	pStructure->siBuySlot;
		MyItemData.uiID				=	pStructure->uiID;
		MyItemData.uiQuantity		=	pStructure->siBuyItemNum;				
		_pWorld->pMyData->AddItem(pStructure->uiCurFollower, &MyItemData);
		
		// ������ �������� �ٽ� �ѹ� ������ �ش�.(���� �ſ뵵�� ����ʿ� ����)
		ItemInfo TempInfo;
		CItemHeader *pItem;
		for( SI16 i=0; i<pStructure->pNpcLC->GetItemNum(); i++ )
		{
			pStructure->pNpcLC->CopyData(i, TempInfo);
			pItem = _pWorld->pItemsInfo->GetItem(TempInfo.uiItemID);
			if(pItem) TempInfo.siColor = _pWorld->pMyData->GetTradeGrade() < pItem->m_uiTradeGrade ? LIST_COLOR_PINK : LIST_COLOR_NOT;
		}
		
		// ���� �κ��� �ʾ� �ش�.
		ItemInfo TempItemInfo;
		pStructure->pPlayerLC->CopyData(pStructure->siBuySlot, TempItemInfo);
		TempItemInfo.uiItemID		= pStructure->uiID;
		TempItemInfo.ItemCount		+= pStructure->siBuyItemNum;					
		TempItemInfo.uiSellPrice	= pStructure->pNpcLC->GetSlotInfo(pStructure->pNpcLC->GetSlot(pStructure->uiID))->uiSellPrice;
		pStructure->pPlayerLC->AddItem(TempItemInfo, pStructure->siBuySlot);
		
		_pWorld->pOnlineInventory->FillItemListBox();
		
		// ���� ���� �ٽ� ���� �Ѵ�.
		_pWorld->pMyData->SetMoney(pOnResBuyItemOkMsg->siMoney);
		
		// ���� �ſ뵵�� ���� �Ѵ�.
		_pWorld->pMyData->SetTradeCredit( pOnResBuyItemOkMsg->uiTradeCredit, 0 );
		pStructure->pNpcLC->StopDrag();
		
		pStructure->siStatus	=	ON_MARKET_STATUS_RECEIVE_POLL;
		_pWorld->pOnlineNumPad->pNumberControl->Initialize();					
		
		// �ŷ� OK ���
		pStructure->NowTradesmanTalk = ON_TEXT_ITEMHELP_TRADESMAN_AFTERTRADE;
	}
	else if( pResBuyItemMsg->dwResponse == ON_RET_NOMONEY )													// �������� ���� ���ڶ�� �޽���
	{	
		pStructure->pNpcLC->StopDrag();
		_pWorld->pOnlineNumPad->pNumberControl->Initialize();
		
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, _pWorld->pOnlineText->Get(ON_TEXT_MARKETTEXT_ITEMBUY), _pWorld->pOnlineText->Get(ON_TEXT_ERRORNOTMONEY));
		
		pStructure->siStatus	=	ON_MARKET_STATUS_RECEIVE_POLL;				
		pStructure->siDragItem	=	-1;																					// ���� �巡������ �������� ����ġ�� ������.
		
		pStructure->NowTradesmanTalk = ON_TEXT_ITEMHELP_TRADESMAN_NOBUY_NOMONEY;
	}
	else if( pResBuyItemMsg->dwResponse == ON_RET_NOITEM )													// �������� �������� ���ٴ� �޽���
	{
		pStructure->pNpcLC->StopDrag();
		_pWorld->pOnlineNumPad->pNumberControl->Initialize();
		
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, _pWorld->pOnlineText->Get(ON_TEXT_MARKETTEXT_ITEMBUY), _pWorld->pOnlineText->Get(ON_TEXT_ERRORNOTFINDITEM));
		
		pStructure->siStatus	=	ON_MARKET_STATUS_RECEIVE_POLL;
		pStructure->siDragItem	=	-1;																					// ���� �巡������ �������� ����ġ�� ������.
	}
	else if( pResBuyItemMsg->dwResponse == ON_RET_BUYZERO )													// �������� �������� ���ٴ� �޽���
	{
		pStructure->pNpcLC->StopDrag();
		_pWorld->pOnlineNumPad->pNumberControl->Initialize();
		
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, _pWorld->pOnlineText->Get(ON_TEXT_MARKETTEXT_ITEMBUY), _pWorld->pOnlineText->Get(ON_TEXT_ERRORNOTFINDITEM));
		
		pStructure->siStatus	=	ON_MARKET_STATUS_RECEIVE_POLL;
		pStructure->siDragItem	=	-1;																					// ���� �巡������ �������� ����ġ�� ������.
	}
	else
	{
		pStructure->siStatus	=	ON_MARKET_STATUS_RECEIVE_POLL;
		clGrp.Error("VillageStruct-Market", _pWorld->pOnlineText->Get(ON_TEXT_MAKE_UNKNOWMESSAGE), pResBuyItemMsg->dwResponse);	
	}
	return 0;
}

UI16 OnlineMsgProc::MarketLeave(BYTE *pMsg)
{
	OnResLeaveMarketMsg *pLeaveMarketMsg = (OnResLeaveMarketMsg*)pMsg;
	OnlineVillageStructureMarket *pStructure = (OnlineVillageStructureMarket *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;
	
	if( pLeaveMarketMsg->dwResponse == ON_RET_MARKET_OK )
	{
		pStructure->bExit	=	TRUE;
		
		// ��θ� ������Ʈ �Ѵ�.
		pStructure->SaveInfo();
		_pWorld->pOnlineVillageSave->CheckSearchItem(pStructure->GetVillageCode());
		
		if(_pWorld->pOnlineInventory)
		{
			if(_pWorld->pOnlineInventory->IsActive())
			{
				_pWorld->pOnlineInventory->FillItemListBox();
			}
		}
	}
	else 
	{
		pStructure->siStatus = ON_MARKET_STATUS_RECEIVE_POLL;
	}
	return 0;
}

UI16 OnlineMsgProc::MarketMoveItem(BYTE *pMsg)
{
	OnResMoveItemMsg *pMoveItemMsg = (OnResMoveItemMsg*)pMsg;
	OnlineVillageStructureMarket *pStructure = (OnlineVillageStructureMarket *)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)		return 0;
	
	if( pMoveItemMsg->dwResponse == ON_RET_OK )
	{
		OnlineReceivedMyItemInfo*		lpItemData;
		
		// Item �̵�
		lpItemData = _pWorld->pMyData->GetItem(pStructure->uiCurFollower, pStructure->uiMoveItemID, pStructure->siMoveItemYetPosition);
		if(lpItemData) lpItemData->siPosInInventory = pStructure->siMoveItemNewPosition;
		_pWorld->pOnlineInventory->FillItemListBox();		
		
		pStructure->siStatus		=	ON_MARKET_STATUS_RECEIVE_POLL;
		pStructure->siDragItem		= -1;					
	}
	else if( pMoveItemMsg->dwResponse == ON_RET_NO )
	{
		pStructure->pPlayerLC->StopDrag();
		pStructure->siStatus		=	ON_MARKET_STATUS_RECEIVE_POLL;
		pStructure->siDragItem		= -1;					
	}
	return 0;
}

UI16 OnlineMsgProc::GovernEnter(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->GovernEnter(pMsg);
}

UI16 OnlineMsgProc::GovernLeave(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->GovernLeave(pMsg);
}

UI16 OnlineMsgProc::GovernInvestSet(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->GovernInvestSet(pMsg);
}

UI16 OnlineMsgProc::GovernInvestGetout(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->GovernInvestGetout(pMsg);
}

UI16 OnlineMsgProc::GovernInvestMylist(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->GovernInvestMylist(pMsg);
}

UI16 OnlineMsgProc::GovernInvestLeave(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->GovernInvestLeave(pMsg);
}

UI16 OnlineMsgProc::GovernInvestProfitInfo(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->GovernInvestProfitInfo(pMsg);
}

UI16 OnlineMsgProc::GovernDescript(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->GovernDescript(pMsg);
}

UI16 OnlineMsgProc::GovernVillageInfo(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->GovernVillageInfo(pMsg);
}

UI16 OnlineMsgProc::GovernVillageDefenceInfo(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->GovernVillageDefenceInfo(pMsg);
}

UI16 OnlineMsgProc::GovernVillageDefenceSet(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->GovernVillageDefenceSet(pMsg);
}

// ������ ���� ���� ���� ��û �޽��� ����
UI16 OnlineMsgProc::OnResponseIncreaseVillageDefenceSoldier(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	
	if(!pStructure)	
		return 0;

	return pStructure->OnResponseIncreaseVillageDefenceSoldier(pMsg);
}

// ������ ���� ���� �� ���
// ON_RESPONSE_GET_VILLAGE_DEFENCE_SOLDIER
UI16 OnlineMsgProc::OnResponseGetVillageDefenceSoldier(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	
	if(!pStructure)	
		return 0;

	return pStructure->OnResponseGetVillageDefenceSoldier(pMsg);
}

// ������ ��� ����ǰ ���� ���
// ON_RESPONSE_LIST_GUILD_SUPPLY
UI16	OnlineMsgProc::OnResponseListGuildSupply(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = NULL;

	if (NULL != _pWorld->pOnlineVillage)
		pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

//	if(!pStructure)		return 0;
	if (!pStructure)
	{
		if (_pWorld->pOnlineTradeBook->IsActivityBook(ON_BOOK_SIEGEWARFARE))
		{
			return _pWorld->pOnlineTradeBook->OnResponseListGuildSupply(pMsg);
		}

		return 0;
	}

	return pStructure->OnResponseListGuildSupply(pMsg);
}

// robypark 2004/12/2 10:56
// ������ ��� ������ ���� ����Ʈ �� ���� % ����
// ON_RESPONSE_GUILD_VILLAGEATTACK_RESULT
UI16 OnlineMsgProc::OnResponseGuildVillageattackResult(BYTE *pMsg)
{
	// ��� �������̽��� �۵� ���� ���
	if (_pWorld->pOnlineTradeBook->IsActivityBook(ON_BOOK_SIEGEWARFARE))
	{
		return _pWorld->pOnlineTradeBook->OnResponseGuildVillageattackResult(pMsg);
	}

	return 0;
}

// robypark 2004/12/2 13:45
// ������ ���� �Ŀ� ������ ���� ���� ������ Ư�� ĳ���Ϳ� �ֱ� ��û
// ON_RESPONSE_GUILD_CHOICE_PLAYER
UI16 OnlineMsgProc::OnResponseGuildChoicePlayer(BYTE *pMsg)
{
	// ��� �������̽��� �۵� ���� ���
	if (_pWorld->pOnlineTradeBook->IsActivityBook(ON_BOOK_SIEGEWARFARE))
	{
		return _pWorld->pOnlineTradeBook->OnResponseGuildChoicePlayer(pMsg);
	}
	else
	{
		// robypark 2005/1/5 18:5
		// ��ź ������ ���� ĳ���Ϳ��Ե� �޾Ҵٴ� �޽��� �ڽ��� ������ش�.
		OnResGuildChoicePlayer	*pOnResGuildChoicePlayer	= (OnResGuildChoicePlayer *)pMsg;

		if (ON_RET_OK == pOnResGuildChoicePlayer->dwResponse)
		{
			VillageHeader *lpVillageHeader = _pWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(pOnResGuildChoicePlayer->siVillageCode);
			
			if (lpVillageHeader)
			{
				// %s ������ ���� ���ڱ��� ���� �޾ҽ��ϴ�. ���ڱ��� %d �ÿ� �������, �� ���� ���ڱ��� ��� �Ͻʽÿ�.
				_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", 
													_pWorld->pOnlineText->Get(ON_TEXT_MSGBOX_RECEIVE_COMPETENCY), 
													_pWorld->pOnlineText->Get( lpVillageHeader->siVillageName ),
													_pWorld->GetSOptionSiegeWarfare()->siSIEGEWARFARETIME_AllInInvestment_End);
			}
		}
		else
			_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", _pWorld->pOnlineText->Get(ON_TEXT_FAIL_TO_GIVE_A_COMPETENCY));
	}

	return 0;
}

// robypark 2004/12/2 14:15
// ������ ���� �Ŀ� ���� ��ź ����(���� ����)�� ���������� ���� ����
// ON_RESPONSE_VILLAGE_INVESTMENT_POWER
UI16 OnlineMsgProc::OnResponseVillageInvestmentPower(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)
		return 0;

	return pStructure->OnResponseVillageInvestmentPower(pMsg);
}

// ������ ��� ����ǰ ���� ��û ����
// ON_RESPONSE_INCREASE_GUILD_SUPPLY
UI16 OnlineMsgProc::OnResponseIncreaseGuildSupply(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	if(!pStructure)
		return 0;

	return pStructure->OnResponseIncreaseGuildSupply(pMsg);
}

UI16 OnlineMsgProc::GovernGuildList(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->GovernGuildList(pMsg);
}

UI16 OnlineMsgProc::GovernGuildSetRelation(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->GovernGuildSetRelation(pMsg);
}

// robypark 2004/11/25 16:4
// ȭģ���� �޽��� ���� ����
// ON_RESPONSE_GUILD_FRIELDLY_MESSAGE_OFFER
UI16 OnlineMsgProc::OnResponseGuildFrieldlyMessageOffer(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	
	if (!pStructure)
		return 0;

	return pStructure->OnResponseGuildFrieldlyMessageOffer(pMsg);
}

UI16 OnlineMsgProc::GovernGuildDetailInfo(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->GovernGuildDetailInfo(pMsg);
}

UI16 OnlineMsgProc::GovernGuildJoin(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->GovernGuildJoin(pMsg);
}

UI16 OnlineMsgProc::GovernGuildCreateCost(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->GovernGuildCreateCost(pMsg);
}

UI16 OnlineMsgProc::GovernGuildCreate(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->GovernGuildCreate(pMsg);
}


UI16 OnlineMsgProc::GovernGuildDelete(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->GovernGuildDelete(pMsg);
}

UI16 OnlineMsgProc::GovernGuildWaitingMemberList(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->GovernGuildWaitingMemberList(pMsg);
}

UI16 OnlineMsgProc::GovernGuildWaitingMemberJoin(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->GovernGuildWaitingMemberJoin(pMsg);
}

UI16 OnlineMsgProc::GovernGuildMemberList(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = NULL;

	if (NULL != _pWorld->pOnlineVillage)
		pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
//	if(!pStructure)		return 0;
	// ��� �������̽����� ó���ϴ� ���.
	if (!pStructure)
	{
		if (_pWorld->pOnlineTradeBook->IsActivityBook(ON_BOOK_SIEGEWARFARE))
		{
			return _pWorld->pOnlineTradeBook->SetGuildMemberList(pMsg);
		}

		return 0;
	}

	// ��û �ǹ��ȿ��� ó���� ���
	return pStructure->GovernGuildMemberList(pMsg);
}

UI16 OnlineMsgProc::GovernGuildDismiss(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->GovernGuildDismiss(pMsg);
}

UI16 OnlineMsgProc::GovernGuildPromotion(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->GovernGuildPromotion(pMsg);
}

UI16 OnlineMsgProc::GovernGuildDemotion(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->GovernGuildDemotion(pMsg);
}

UI16 OnlineMsgProc::GovernGuildFlagList(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->GovernGuildFlagList(pMsg);
}

UI16 OnlineMsgProc::GovernGuildFlagSet(BYTE *pMsg)
{
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->GovernGuildFlagSet(pMsg);
}

UI16 OnlineMsgProc::MakeFarmInfo(BYTE *pMsg)
{
	OnResFarmInfoMsg *pOnResFarmInfoMsg = (OnResFarmInfoMsg*)pMsg;
	OnlineVillageStructMake	*pMakeList = _pWorld->pOnlineVillage->pMakeList;

	if(!pMakeList)		return 0;

	CHAR	szTempName[32];		
	ZeroMemory( szTempName, sizeof( szTempName ) );
	
	switch( pMakeList->m_uiKind )
	{
	case ON_PLANT_FARM:		sprintf( szTempName, _pWorld->pOnlineText->Get(ON_TEXT_MAKE_NUMBER));			break;
	case ON_PLANT_WEAPON:	sprintf( szTempName, _pWorld->pOnlineText->Get(ON_TEXT_MAKE_NUMBER1));			break;
	case ON_PLANT_RANCH:	sprintf( szTempName, _pWorld->pOnlineText->Get(ON_TEXT_MAKE_NUMBER2));			break;
	case ON_PLANT_FACTORY:	sprintf( szTempName, _pWorld->pOnlineText->Get(ON_TEXT_MAKE_NUMBER3));			break;
	case ON_PLANT_ARMOR:	sprintf( szTempName, _pWorld->pOnlineText->Get(ON_TEXT_MAKE_NUMBER4));			break;
	}			
	
	OnIndustrialComplexInfo	*pOnIndustrialComplexInfo = (OnIndustrialComplexInfo*)(pOnResFarmInfoMsg + 1);
	
	for(UI16 i=0 ; i < ON_MAKE_MAXLIST; i++ )
	{
		// actdoll (2004/08/27 17:03) : ��� ���� �߻�! �� �������� �ٸ��� Ÿ���� �⵵�� �Ѵ�.
		switch( pGame->LanguageCode )
		{
			case ELANGAGECODE_KOREAN:	case ELANGAGECODE_TAIWAN:	case ELANGAGECODE_JAPAN:	case ELANGAGECODE_HONGKONG:	case ELANGAGECODE_CHINA:
				sprintf( pMakeList->m_MakeList[i].m_szFarmName, "%02d%s",i+1, szTempName );	break;
			case ELANGAGECODE_INDONESIA:
				sprintf( pMakeList->m_MakeList[i].m_szFarmName, "%s%02d",szTempName, i+1 );	break;
		}
		ZeroMemory( pMakeList->m_MakeList[i].m_szID, sizeof( pMakeList->m_MakeList[i].m_szID ) );
		
		pMakeList->m_MakeList[i].m_moMaxPay	=	0;
		pMakeList->m_MakeList[i].m_uiSlotNum	=	0;	
		pMakeList->m_MakeList[i].m_siWork		=	0;
	}
	
	for( i=0 ; i < pOnResFarmInfoMsg->uiIndustrialComplexNum; i++ )
	{
		// actdoll (2004/08/27 17:03) : ��� ���� �߻�! �� �������� �ٸ��� Ÿ���� �⵵�� �Ѵ�.
		switch( pGame->LanguageCode )
		{
			case ELANGAGECODE_KOREAN:	case ELANGAGECODE_TAIWAN:	case ELANGAGECODE_JAPAN:	case ELANGAGECODE_HONGKONG:	case ELANGAGECODE_CHINA:
				sprintf( pMakeList->m_MakeList[pOnIndustrialComplexInfo[i].siID].m_szFarmName, "%02d%s", pOnIndustrialComplexInfo[i].siID+1, szTempName );	break;
			case ELANGAGECODE_INDONESIA:
				sprintf( pMakeList->m_MakeList[pOnIndustrialComplexInfo[i].siID].m_szFarmName, "%s%02d", szTempName, pOnIndustrialComplexInfo[i].siID+1 );	break;
		}
		strncpy( pMakeList->m_MakeList[pOnIndustrialComplexInfo[i].siID].m_szID, pOnIndustrialComplexInfo[i].szName, ON_ID_LENGTH);
		
		pMakeList->m_MakeList[pOnIndustrialComplexInfo[i].siID].m_moMaxPay	=	pOnIndustrialComplexInfo[i].m_moMaxPay;	
		pMakeList->m_MakeList[pOnIndustrialComplexInfo[i].siID].m_siWork	=	pOnIndustrialComplexInfo[i].m_uiTotalAmountOfWork;
	}
	
	pMakeList->m_siStatus = ON_MAKE_POLL;
	return 0;
}

UI16 OnlineMsgProc::MakeFarmBuy(BYTE *pMsg)
{
	OnResFarmBuyMsg		*pOnResFarmBuyMsg = (OnResFarmBuyMsg*)pMsg;
	OnlineVillageStructMake	*pMakeList = _pWorld->pOnlineVillage->pMakeList;

	if(!pMakeList)		return 0;

	if( pOnResFarmBuyMsg->dwResponse == ON_RET_OK )
	{
		stPlants *pPlants = _pWorld->pOnlineBuilding->GetPlants( pMakeList->m_szKind );
		// �� ���� �ü��� ��ŭ ����.
		_pWorld->pMyData->SetMoney( _pWorld->pMyData->GetMoney() - pPlants->uiCost );
		strcpy( pMakeList->m_MakeList[ pMakeList->m_siSelect ].m_szID, _pWorld->pMyData->GetMyCharName() );
	}
	else
	{
		_pWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", _pWorld->pOnlineText->Get(ON_TEXT_MAKE_BUYFAIL) );				
	}
	
	pMakeList->m_siStatus = ON_MAKE_POLL;
	return 0;
}

UI16 OnlineMsgProc::FarmWorkCompleted(BYTE *pMsg)
{
	OnlineVillageStructFarm *pStructure = (OnlineVillageStructFarm*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;

	return pStructure->FarmWorkCompleted(pMsg);
}

UI16 OnlineMsgProc::FarmSlotInfo(BYTE *pMsg)
{
	OnlineVillageStructFarm *pStructure = (OnlineVillageStructFarm*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->FarmSlotInfo(pMsg);
}

UI16 OnlineMsgProc::FarmSlotExpand(BYTE *pMsg)
{
	OnlineVillageStructFarm *pStructure = (OnlineVillageStructFarm*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->FarmSlotExpand(pMsg);
}

UI16 OnlineMsgProc::FarmExit(BYTE *pMsg)
{
	OnlineVillageStructFarm *pStructure = (OnlineVillageStructFarm*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->FarmExit(pMsg);
}

UI16 OnlineMsgProc::FarmStructBuild(BYTE *pMsg)
{
	OnlineVillageStructFarm *pStructure = (OnlineVillageStructFarm*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->FarmStructBuild(pMsg);
}

UI16 OnlineMsgProc::FarmStructDestroy(BYTE *pMsg)
{
	OnlineVillageStructFarm *pStructure = (OnlineVillageStructFarm*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->FarmStructDestroy(pMsg);
}

UI16 OnlineMsgProc::FarmStructUpgrade(BYTE *pMsg)
{
	OnlineVillageStructFarm *pStructure = (OnlineVillageStructFarm*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->FarmStructUpgrade(pMsg);
}

UI16 OnlineMsgProc::FarmStructExit(BYTE *pMsg)
{
	OnlineVillageStructFarm *pStructure = (OnlineVillageStructFarm*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->FarmStructExit(pMsg);
}

UI16 OnlineMsgProc::FarmStorageInfo(BYTE *pMsg)
{
	OnlineVillageStructFarm *pStructure = (OnlineVillageStructFarm*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->FarmStorageInfo(pMsg);
}

UI16 OnlineMsgProc::FarmStorageItemInput(BYTE *pMsg)
{
	OnlineVillageStructFarm *pStructure = (OnlineVillageStructFarm*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->FarmStorageItemInput(pMsg);
}

UI16 OnlineMsgProc::FarmStorageItemOutput(BYTE *pMsg)
{
	OnlineVillageStructFarm *pStructure = (OnlineVillageStructFarm*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->FarmStorageItemOutput(pMsg);
}

UI16 OnlineMsgProc::FarmProductInfo(BYTE *pMsg)
{
	OnlineVillageStructFarm *pStructure = (OnlineVillageStructFarm*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->FarmProductInfo(pMsg);
}

UI16 OnlineMsgProc::FarmProduct(BYTE *pMsg)
{
	OnlineVillageStructFarm *pStructure = (OnlineVillageStructFarm*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->FarmProduct(pMsg);
}

UI16 OnlineMsgProc::FarmProductCancel(BYTE *pMsg)
{
	OnlineVillageStructFarm *pStructure = (OnlineVillageStructFarm*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->FarmProductCancel(pMsg);
}

UI16 OnlineMsgProc::FarmProductPayChange(BYTE *pMsg)
{
	OnlineVillageStructFarm *pStructure = (OnlineVillageStructFarm*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->FarmProductPayChange(pMsg);
}

UI16 OnlineMsgProc::FarmWorkStart(BYTE *pMsg)
{
	OnlineVillageStructFarm *pStructure = (OnlineVillageStructFarm*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->FarmWorkStart(pMsg);
}

UI16 OnlineMsgProc::FarmWorkResult(BYTE *pMsg)
{
	OnlineVillageStructFarm *pStructure = (OnlineVillageStructFarm*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if(!pStructure)		return 0;
	return pStructure->FarmWorkResult(pMsg);
}

