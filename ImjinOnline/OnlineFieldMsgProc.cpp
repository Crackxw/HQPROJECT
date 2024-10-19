#include <GSL.h>

#include "OnlineGlobal.h"
#include "OnlineCharStatus.h"
#include "OnlineSmith.h"
#include "OnlineMsgProc.h"
#include "OnlineQuestInterface.h"
#include "OnlineTip.h"

// ���� ���� ������
#include	"SiegeUnitTableMgr.h"

#include	"SiegeWarfareUnitUI.h"
#include	"SiegeWarfareTransformUIWnd.h"

extern _clGame* pGame;

// �̵��߿� �߰��� ������ ĳ���� ����.
UI16 OnlineMsgProc::NewInfoOnMove(BYTE *pMsg)
{
	OnNewInfoOnMoveMsg	*pNewInfoOnMoveMsg = (OnNewInfoOnMoveMsg*)pMsg;
	
	if(pNewInfoOnMoveMsg == NULL)	return 0;

	UI16					*puiCharUniqueID	= NULL;
	OnCharBaseInfo*			lpOnCharBaseInfo	= NULL;
	OnCharPlayerInfo*		lpOnCharPlayerInfo	= NULL;
	BYTE					TempBuffer[128];
	DWORD					IP;

	puiCharUniqueID	 = (UI16*)((CHAR*)pNewInfoOnMoveMsg + sizeof(OnNewInfoOnMoveMsg));
	lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)puiCharUniqueID + sizeof(UI16) * pNewInfoOnMoveMsg->uiDelCharNum);

	for( SI32 i = 0; i < pNewInfoOnMoveMsg->uiAddCharNum; i++)
	{	
		ZeroMemory(TempBuffer, 128);
		IP = 0;
		if(ISPLAYER(lpOnCharBaseInfo->uiAccount))
		{
			lpOnCharPlayerInfo = (OnCharPlayerInfo*)lpOnCharBaseInfo;

			if(_pWorld->pMyData->GetMyCharUniqueID() == lpOnCharPlayerInfo->uiAccount)
			{
				CopyMemory(TempBuffer, _pWorld->pMyData->GetMyCharName(), ON_ID_LENGTH);
				IP = lpOnCharPlayerInfo->siDBAccount;

				_pWorld->pIOnlineChar->Init(lpOnCharPlayerInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharPlayerInfo->uiKind, lpOnCharPlayerInfo->siX, lpOnCharPlayerInfo->siY, IP, 0, lpOnCharPlayerInfo->siFightingPower, _pWorld,(UI16*)lpOnCharPlayerInfo->uiEquipWeapon);
				_pWorld->pIOnlineChar->SetAttackMode(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bAttackMode);
				_pWorld->pIOnlineChar->SetEquipItem(lpOnCharPlayerInfo->uiAccount, (UI16*)lpOnCharPlayerInfo->uiEquipWeapon);
				_pWorld->pIOnlineChar->SetSiegeWarfare_Hp(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->siFieldHp);
				_pWorld->pIOnlineChar->SetCaptain(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->bCaptain);
				_pWorld->pIOnlineChar->SetSiegeWarfare_CurrentSupplyGoodsAmount(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->siSupplyCharge);
				
				// robypark 2004/12/17 17:29
				// �����������̽��� �ִ� ĳ�����ΰ��� ����
				_pWorld->pIOnlineChar->SetInVillageDefenceInterface(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bVillageInLimit);

				// ���� �����ϱ� �������̽��� �ִٰ� ����
				_pWorld->pMyData->SetInVillageDefenceInterface(lpOnCharPlayerInfo->bVillageInLimit);

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
				_pWorld->pIOnlineChar->SetAttackMode(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bAttackMode);
				_pWorld->pIOnlineChar->SetEquipItem(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->uiEquipWeapon);
				_pWorld->pIOnlineChar->SetSiegeWarfare_Hp(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->siFieldHp);
				_pWorld->pIOnlineChar->SetCaptain(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->bCaptain);
				_pWorld->pIOnlineChar->SetSiegeWarfare_CurrentSupplyGoodsAmount(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->siSupplyCharge);

				// robypark 2004/12/17 17:29
				// �����������̽��� �ִ� ĳ�����ΰ��� ����
				_pWorld->pIOnlineChar->SetInVillageDefenceInterface(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bVillageInLimit);

				// ������ ������ �� ���õ� ���� ��� ���
				LPSSiegeUnitTable lpsSiegeUnitTable = _pWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTable(lpOnCharPlayerInfo->uiKind);

				// ������ ������ ���� ���
				if (NULL != lpsSiegeUnitTable)
				{
					// ������ ���� ����� ĳ������ �� ������ ���ٸ�
					if(_pWorld->pIOnlineChar->GetDataType(lpOnCharPlayerInfo->uiAccount) == 1)
					{
						if (_pWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
						{
							// �������� ��û�Ѵ�.
							_pWorld->pOnlineClient->SendPlayerDetailInfo(lpOnCharPlayerInfo->uiAccount);
							_pWorld->pIOnlineChar->SetDataType(lpOnCharPlayerInfo->uiAccount, 2);	// ���� �ٿ�ε� ��� ��...
						}
					}					
				}
			}

			_pWorld->pIOnlineChar->SetGuildData(lpOnCharPlayerInfo->uiAccount, _pWorld->pIOnlineChar->GetGuildName(lpOnCharPlayerInfo->uiAccount), lpOnCharPlayerInfo->siGuildInGrade, _pWorld->pIOnlineChar->GetGuildFlag(lpOnCharPlayerInfo->uiAccount));
			_pWorld->pIOnlineChar->SetMercenary(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->siVMercenaryKind);

			// ���� ����
			if(lpOnCharPlayerInfo->siPrivateMarketItemNum >= 0) 
			{
				OnBaseBoothItem 	*lpOnBaseBoothItem, *pTempOnBaseBoothItem;
				SellItemInfo		TempSellItemInfo;

				_pWorld->pIOnlineChar->SetBoothOpen(lpOnCharPlayerInfo->uiAccount, TRUE);
				lpOnBaseBoothItem = (OnBaseBoothItem*)((CHAR*)lpOnCharPlayerInfo + sizeof(OnCharPlayerInfo));
				pTempOnBaseBoothItem = lpOnBaseBoothItem;
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
			OnCharMonsterInfo *lpOnCharMonsterInfo = (OnCharMonsterInfo*)lpOnCharBaseInfo;			

			// �ٸ� ĳ������ ���� ������ �����Ѵ�.						
			_pWorld->pIOnlineChar->Init(lpOnCharMonsterInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharMonsterInfo->uiKind, lpOnCharMonsterInfo->siX, lpOnCharMonsterInfo->siY, IP, 0.0f, lpOnCharMonsterInfo->siFightingPower, _pWorld,NULL);
			
			lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharMonsterInfo));
		}
		else if(ISSHIP(lpOnCharBaseInfo->uiAccount))
		{
			OnCharShipInfo*	lpOnCharShipInfo = (OnCharShipInfo*)lpOnCharBaseInfo;			

			// �ٸ� ĳ������ ���� ������ �����Ѵ�.						
			_pWorld->pIOnlineChar->Init(lpOnCharShipInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharShipInfo->uiKind, lpOnCharShipInfo->siX, lpOnCharShipInfo->siY, IP, 0.0f, 0, _pWorld,NULL);

			lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharShipInfo));
		}
	}

	OnlineReceiveFieldItemInfo	*pOnlineReceiveFieldItemInfo = (OnlineReceiveFieldItemInfo*)((CHAR*)lpOnCharBaseInfo);
	
	for(i = 0; i < pNewInfoOnMoveMsg->uiDelCharNum; i++)
	{
		if(_pWorld->pMyData->GetMyCharUniqueID() != puiCharUniqueID[i])
		{
			_pWorld->pIOnlineChar->Free(puiCharUniqueID[i]);	
		}
	}

	for(i = 0; i < pNewInfoOnMoveMsg->uiItemNum; i++)		
	{
		_pWorld->pOnlineMap->SetID(pOnlineReceiveFieldItemInfo[i].siX, pOnlineReceiveFieldItemInfo[i].siY, 1, 1, 0, 0, 
							pOnlineReceiveFieldItemInfo[i].uiItemID, pOnlineReceiveFieldItemInfo[i].uiQuantity);
	}

	return 0;
}

// ĳ���� ����
UI16 OnlineMsgProc::OnDelChar(BYTE *pMsg)
{
	OnDelCharMsg	*pDelCharMsg = (OnDelCharMsg*)pMsg;

	if(pDelCharMsg == NULL)		return 0;

	if(_pWorld->pIOnlineChar)			
	{
		SI16 TempSelect, siHeroX, siHeroY;
		_pWorld->pIOnlineChar->GetPos(pDelCharMsg->uiCharID, &siHeroX, &siHeroY);
		MapStoreHeader *pStoreHeader = _pWorld->pOnlineMap->GetMapStoreParser( siHeroX, siHeroY, &TempSelect );

		if(pStoreHeader)
		{
			// ���ǰ����̶��
			_pWorld->pOnlineMap->m_MapBoothInfo.DelBoothData(pStoreHeader->siMapStoreCode);
			// ��ǳ�� ���ֱ�
			_pWorld->pIOnlineChar->SetSpeech(pDelCharMsg->uiCharID, 0);
		}
		_pWorld->pIOnlineChar->Free(pDelCharMsg->uiCharID);
	}	

	return 0;
}

// ĳ���� �߰�
UI16 OnlineMsgProc::OnAddChar(BYTE *pMsg)
{
	OnAddCharMsg*				pAddCharMsg = (OnAddCharMsg*)pMsg;

	if(pAddCharMsg == NULL)		return 0;

	OnCharBaseInfo*			lpOnCharBaseInfo = (OnCharBaseInfo*)((BYTE*)pAddCharMsg + sizeof(OnAddCharMsg));
	OnCharPlayerInfo*		lpOnCharPlayerInfo;
	CHAR					TempBuffer[128];
	DWORD					IP;

	if(_pWorld->pIOnlineChar->IsExistUniqueID(lpOnCharBaseInfo->uiAccount) == FALSE)
	{
		ZeroMemory(TempBuffer, 128);
		IP = 0;
		if(ISPLAYER(lpOnCharBaseInfo->uiAccount))
		{
			lpOnCharPlayerInfo = (OnCharPlayerInfo*)lpOnCharBaseInfo;

			if(_pWorld->pMyData->GetMyCharUniqueID() == lpOnCharPlayerInfo->uiAccount)
			{
				CopyMemory(TempBuffer, _pWorld->pMyData->GetMyCharName(), ON_ID_LENGTH);
				IP = lpOnCharPlayerInfo->siDBAccount;

				_pWorld->pIOnlineChar->Init(lpOnCharPlayerInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharPlayerInfo->uiKind, lpOnCharPlayerInfo->siX, lpOnCharPlayerInfo->siY, IP, 0, lpOnCharPlayerInfo->siFightingPower, _pWorld,(UI16*)lpOnCharPlayerInfo->uiEquipWeapon);
				_pWorld->pIOnlineChar->SetAttackMode(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bAttackMode);
				_pWorld->pIOnlineChar->SetEquipItem(lpOnCharPlayerInfo->uiAccount, (UI16*)lpOnCharPlayerInfo->uiEquipWeapon);
				_pWorld->pIOnlineChar->SetSiegeWarfare_Hp(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->siFieldHp);
				_pWorld->pIOnlineChar->SetCaptain(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->bCaptain);
				_pWorld->pIOnlineChar->SetSiegeWarfare_CurrentSupplyGoodsAmount(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->siSupplyCharge);
				
				// robypark 2004/12/17 17:29
				// �����������̽��� �ִ� ĳ�����ΰ��� ����
				_pWorld->pIOnlineChar->SetInVillageDefenceInterface(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bVillageInLimit);
				_pWorld->pMyData->SetInVillageDefenceInterface(lpOnCharPlayerInfo->bVillageInLimit);

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
				_pWorld->pIOnlineChar->SetAttackMode(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bAttackMode);
				_pWorld->pIOnlineChar->SetEquipItem(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->uiEquipWeapon);
				_pWorld->pIOnlineChar->SetSiegeWarfare_Hp(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->siFieldHp);
				_pWorld->pIOnlineChar->SetCaptain(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->bCaptain);
				_pWorld->pIOnlineChar->SetSiegeWarfare_CurrentSupplyGoodsAmount(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->siSupplyCharge);

				// robypark 2004/12/17 17:29
				// �����������̽��� �ִ� ĳ�����ΰ��� ����
				_pWorld->pIOnlineChar->SetInVillageDefenceInterface(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bVillageInLimit);

				// ������ ������ �� ���õ� ���� ��� ���
				LPSSiegeUnitTable lpsSiegeUnitTable = _pWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTable(lpOnCharPlayerInfo->uiKind);

				// ������ ������ ���� ���
				if (NULL != lpsSiegeUnitTable)
				{
					// ������ ���� ����� ĳ������ �� ������ ���ٸ�
					if(_pWorld->pIOnlineChar->GetDataType(lpOnCharPlayerInfo->uiAccount) == 1)
					{
						if (_pWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
						{
							// �������� ��û�Ѵ�.
							_pWorld->pOnlineClient->SendPlayerDetailInfo(lpOnCharPlayerInfo->uiAccount);
							_pWorld->pIOnlineChar->SetDataType(lpOnCharPlayerInfo->uiAccount, 2);	// ���� �ٿ�ε� ��� ��...
							}
					}
				}
			}

			_pWorld->pIOnlineChar->SetGuildData(lpOnCharPlayerInfo->uiAccount, _pWorld->pIOnlineChar->GetGuildName(lpOnCharPlayerInfo->uiAccount), lpOnCharPlayerInfo->siGuildInGrade, _pWorld->pIOnlineChar->GetGuildFlag(lpOnCharPlayerInfo->uiAccount));
			_pWorld->pIOnlineChar->SetMercenary(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->siVMercenaryKind);

			// ���� ����

			if(lpOnCharPlayerInfo->siPrivateMarketItemNum >= 0) 
			{
				OnBaseBoothItem	*lpOnBaseBoothItem, *pTempOnBaseBoothItem;
				SellItemInfo	TempSellItemInfo;

				_pWorld->pIOnlineChar->SetBoothOpen(lpOnCharPlayerInfo->uiAccount, TRUE);
				lpOnBaseBoothItem = (OnBaseBoothItem*)((CHAR*)lpOnCharPlayerInfo + sizeof(OnCharPlayerInfo));
				pTempOnBaseBoothItem = lpOnBaseBoothItem;
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
		}
		else if(ISMONSTER(lpOnCharBaseInfo->uiAccount))
		{
			OnCharMonsterInfo *lpOnCharMonsterInfo = (OnCharMonsterInfo*)lpOnCharBaseInfo;

			// �ٸ� ĳ������ ���� ������ �����Ѵ�.						
			_pWorld->pIOnlineChar->Init(lpOnCharMonsterInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharMonsterInfo->uiKind, lpOnCharMonsterInfo->siX, lpOnCharMonsterInfo->siY, IP, 0.0f, lpOnCharMonsterInfo->siFightingPower, _pWorld, NULL);
		}
		else if(ISSHIP(lpOnCharBaseInfo->uiAccount))
		{
			OnCharShipInfo *lpOnCharShipInfo = (OnCharShipInfo*)lpOnCharBaseInfo;

			// �ٸ� ĳ������ ���� ������ �����Ѵ�.						
			_pWorld->pIOnlineChar->Init(lpOnCharShipInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharShipInfo->uiKind, lpOnCharShipInfo->siX, lpOnCharShipInfo->siY, IP, 0.0f, 0, _pWorld, NULL);
		}
	}

	return 0;
}

// �ٸ� ĳ������ ��ġ�� �޴´�
UI16 OnlineMsgProc::OnResPosotion(BYTE *pMsg)
{
	OnCharMoveMsg*	pCharMoveMsg;
	UI16*			lpData;	

	pCharMoveMsg = (OnCharMoveMsg*)pMsg;	
	lpData       = (UI16*)((BYTE*)pCharMoveMsg + sizeof(OnCharMoveMsg));	

	
	// �켱 �� ĳ���Ͱ� �ִ��� ���� ������ �� ĳ������ ������ �޶�� ��û�Ѵ�.
	if(_pWorld->pIOnlineChar == NULL)		return 0;

	for(SI32 TempCount = 0; TempCount < pCharMoveMsg->uiMovedCharNum; TempCount++)
	{
		if(ISSHIP(lpData[0]) == TRUE)
		{
			// ���̴�.
//					DWriteStringToFile("Ship X : [%d], Y : [%d]", pCharMoveMsg->uiX, pCharMoveMsg->uiY);
		}

		if(_pWorld->pIOnlineChar->IsExistUniqueID(lpData[0])  == TRUE)
		{	
			// �켱 ��ġ�� �ʹ� ���� ���̰� ���� ������ �ؾ� �ϴ� ��Ȳ���� ����.
			// �� �� ĳ���Ͱ� �ƴϿ��� �Ѵ�.
			if(_pWorld->pIOnlineChar->IsWarp(lpData[0], lpData[1], lpData[2],ON_CHAR_WAPR_DISTANCE) == TRUE)
			{
				if(lpData[0] != _pWorld->pMyData->GetMyCharUniqueID())
				{
					_pWorld->pIOnlineChar->Warp(lpData[0], lpData[1], lpData[2]);
				}
			} 
			else
			{
				// ����ڸ� �� ��ǥ�� �̵� ��Ų��.			
				if(lpData[0] != _pWorld->pMyData->GetMyCharUniqueID())
				{
					_pWorld->pIOnlineChar->AddOrder(lpData[0], ON_ORDER_MOVE, lpData[1], lpData[2]);

					/*
					if(lpData[0] == pMyData->GetMyCharUniqueID())
					{
						pOnlineClientChatting->MessageSend_MoveSell(pOnlineMap->m_MapIndex, lpData[1], lpData[2]);
					}
					*/
				}
			}

			_pWorld->pIOnlineChar->SetWalkingFlag(lpData[0], lpData[3]);
		}			
		else
		{
			// ����� ����
			/*
			// ĳ������ ������ �޶�� ��û�Ѵ�.
			// �켱 �־��� ������ ĳ���͸� �ʱ�ȭ �Ѵ�.
			pIOnlineChar->Init(pCharMoveMsg->uiCharID, "", pCharMoveMsg->uiKind, SI16(pCharMoveMsg->uiX), SI16(pCharMoveMsg->uiY), 0, (float)pCharMoveMsg->uiRank, pCharMoveMsg->siFightPower, this);
			
			// ���� �÷��̾��� �÷��̾��� ������ �䱸�Ѵ�.			
			if(ISPLAYER(pCharMoveMsg->uiCharID) == TRUE)
			{
				RPI.Add(pCharMoveMsg->uiCharID);
			}
			*/
		}

		lpData += 4;
	}

	return 0;
}

// ������ ��� ����.
UI16 OnlineMsgProc::OnResEnterVillage(BYTE *pMsg)
{
	OnResponseMsg *pResponseMsg = (OnResponseMsg*)pMsg;

	if(pResponseMsg == NULL)	return 0;
	
	if(pResponseMsg->dwResponse == ON_RET_OK)
	{
		UI16 uiVillageCode;
		OnResGotoVillageOkMsg *pGotoVillageOkMsg = (OnResGotoVillageOkMsg*)pResponseMsg;	
		
		// ĳ���Ͱ� ������ ����. ������ �� ĳ���Ͱ� �ڱ� ĳ�����̶�� �ڱⰡ ������ �ִ� ��� ĳ������ ���� ������ ������.
		// �׷��� �ʰ� �ٸ� ĳ�����̶�� �� ĳ���͸� ������.
		if(pGotoVillageOkMsg->uiCharID == _pWorld->pMyData->GetMyCharUniqueID())
		{
			// �� ĳ�����̴�.
			// �α� ó��
			OnRecvVillageInfo*		lpOnRecvVillageInfo = (OnRecvVillageInfo*)((UI08*)(pGotoVillageOkMsg) + sizeof(OnResGotoVillageOkMsg));

			// ������ ���� ������ �� �� �ֶǷ� �㰡�� �޾Ҵ�.

			// ������ ��ü�� �����Ѵ�.
			_pWorld->pOnlineVillage	=	new OnlineVillage;
			
			// ������ �̵��ؾ� �Ѵ�.
			uiVillageCode	=	_pWorld->pMyData->GetGotoVillage();						
			
			_pWorld->pOnlineVillage->Init(uiVillageCode, lpOnRecvVillageInfo, _pWorld);
			_pWorld->SetGamePollStatus( ON_GAMEPOLL_VILLAGE );
			
			// ��� ĳ���͵��� ���ش�.
			_pWorld->pIOnlineChar->AllFree();
			
			// ������Ʈ�� ��� �����Ѵ�.
			_pWorld->pIOnlineObject->FreeAll();

			// �� �̻� ������ ���� ������ ���ٴ� �޽����� ��ٸ��� �ʵ��� �����Ѵ�.
			_pWorld->pMyData->SetWaitGotoVillageResponse(FALSE);		
			
			// ������ ���ٴ� ������ �ʱ�ȭ �Ѵ�.
			_pWorld->pMyData->SetGotoVillage(FALSE, 0);			

			_pWorld->pOnlineClientChatting->MessageSend_NotIsMap();

			if(_pWorld->m_VillageInStruct != -1)
			{
				_pWorld->pOnlineVillage->MoveInStruct(_pWorld->GetVillageInStruct(), TRUE, TRUE);
			}
		}
		else
		{
			// �ٸ� ĳ�����̴�.
			_pWorld->pIOnlineChar->Free(pGotoVillageOkMsg->uiCharID);
		}		
	}
	else if(pResponseMsg->dwResponse == ON_RET_NO)
	{
		// ������ �� ���� ����.
		_pWorld->pMyData->SetWaitGotoVillageResponse(FALSE);		
		_pWorld->pMyData->SetGotoVillage(FALSE, 0);
		_pWorld->pOnlineCharUI->SetAction(TRUE);
	}
	else if(pResponseMsg->dwResponse == ON_RET_ENEMY_GUILD_VILLAGE)
	{
		// ������ �� ���� ����.(������� �����̶�)
		_pWorld->pMyData->SetWaitGotoVillageResponse(FALSE);		
		_pWorld->pMyData->SetGotoVillage(FALSE, 0);
		_pWorld->pOnlineCharUI->SetAction(TRUE);
		_pWorld->pIOnlineChar->SetSpeech(_pWorld->pMyData->GetMyCharUniqueID(), _pWorld->pOnlineText->Get(ON_TEXT_ENEMYVILLAGE));
		PushEffect(EFFECT_CANNOTENTERVILLAGE, 0, 0);
	}

	return 0;
}

// ��Ż�� �̵�
UI16 OnlineMsgProc::OnResMoveMap(BYTE *pMsg)
{
	// �ŷ� ���̶�� �ݾ� �ش�.
	_pWorld->pOnlineTrade->m_pMsgBox->Initialize();

	OnResMoveMapMsg* lpOnResMoveMapMsg = (OnResMoveMapMsg*)pMsg;
	if(lpOnResMoveMapMsg == NULL)	return 0;
	
	if(lpOnResMoveMapMsg->dwResponse == ON_RET_OK)
	{
		OnResMoveMapOkMsg*						lpOnResMoveMapOkMsg;
		OnlineReceiveFieldItemInfo*				pReceivedFieldItemInfo;
		PortalHeader*							TempPortal;	
		DWORD									IP;
		SI32									TempCount;
		SI16									siCurMapIndex;
		
		lpOnResMoveMapOkMsg = (OnResMoveMapOkMsg*)lpOnResMoveMapMsg;
		TempPortal          = _pWorld->pOnlineMap->PortalParser.GetPortal(_pWorld->NextMapIndex);
		
		// ��� ĳ���͸� �ʱ�ȭ �Ѵ�.
		_pWorld->pIOnlineChar->AllFree();
		siCurMapIndex = _pWorld->pOnlineMap->m_MapIndex;

		// �� ��ȯ
		OnCharBaseInfo		*lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnResMoveMapOkMsg + sizeof(OnResMoveMapOkMsg));		
		OnCharMonsterInfo	*lpOnCharMonsterInfo;
		OnCharShipInfo		*lpOnCharShipInfo;
		OnCharPlayerInfo	*lpOnCharPlayerInfo;
		
		for(TempCount = 0; TempCount < lpOnResMoveMapOkMsg->uiAroundCharNum; TempCount++)
		{
			// ���ڰŸ����� �Ѿ����� ���ö� ��Ʈ��Ʈ ����
			if(TempCount == (SI16)(lpOnResMoveMapOkMsg->uiAroundCharNum / 2) && siCurMapIndex == 2)
				_pWorld->SendHeartBeat();

			if(ISPLAYER(lpOnCharBaseInfo->uiAccount))
			{
				lpOnCharPlayerInfo = (OnCharPlayerInfo*)lpOnCharBaseInfo;

				if(_pWorld->pMyData->GetMyCharUniqueID() == lpOnCharPlayerInfo->uiAccount)
				{					
					// ���ΰ� ĳ������ ��ġ�� �������� ���� �����Ѵ�.				
					_pWorld->pOnlineMap->ChangeMap(TempPortal->siPortalMoveMapIndex, lpOnCharPlayerInfo->siX, lpOnCharPlayerInfo->siY);

//					_pWorld->pOnlineMap->SetCurrentFocusCharID(_pWorld->pMyData->GetMyCharUniqueID());
//					_pWorld->pOnlineMap->PlayFitMusic();
//					_pWorld->pOnlineClientChatting->MessageSend_MoveSell(TempPortal->siPortalMoveMapIndex, lpOnCharPlayerInfo->siX, lpOnCharPlayerInfo->siY);
				}

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

				lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharShipInfo));
			}
		}
		
		// ĳ���� �߰�
		lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnResMoveMapOkMsg + sizeof(OnResMoveMapOkMsg));

		CHAR					TempBuffer[128];

		for(TempCount = 0; TempCount < lpOnResMoveMapOkMsg->uiAroundCharNum; TempCount++)
		{
			// ���ڰŸ����� �Ѿ����� ���ö� ��Ʈ��Ʈ ����
			if(TempCount == (SI16)(lpOnResMoveMapOkMsg->uiAroundCharNum / 2) && siCurMapIndex == 2)
				_pWorld->SendHeartBeat();

			ZeroMemory(TempBuffer, 128);
			IP = 0;
			if(ISPLAYER(lpOnCharBaseInfo->uiAccount))
			{
				lpOnCharPlayerInfo = (OnCharPlayerInfo*)lpOnCharBaseInfo;

				if(_pWorld->pMyData->GetMyCharUniqueID() == lpOnCharPlayerInfo->uiAccount)
				{
					CopyMemory(TempBuffer, _pWorld->pMyData->GetMyCharName(), ON_ID_LENGTH);
					IP = lpOnCharPlayerInfo->siDBAccount;					

					_pWorld->pIOnlineChar->Init(lpOnCharPlayerInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharPlayerInfo->uiKind, lpOnCharPlayerInfo->siX, lpOnCharPlayerInfo->siY, IP, 0, lpOnCharPlayerInfo->siFightingPower, _pWorld,(UI16*)lpOnCharPlayerInfo->uiEquipWeapon);
					_pWorld->pIOnlineChar->SetAttackMode(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bAttackMode);
					_pWorld->pIOnlineChar->SetEquipItem(lpOnCharPlayerInfo->uiAccount, (UI16*)lpOnCharPlayerInfo->uiEquipWeapon);
					_pWorld->pIOnlineChar->SetSiegeWarfare_Hp(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->siFieldHp);
					_pWorld->pIOnlineChar->SetCaptain(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->bCaptain);
					_pWorld->pIOnlineChar->SetSiegeWarfare_CurrentSupplyGoodsAmount(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->siSupplyCharge);

					// robypark 2004/12/17 17:29
					// �����������̽��� �ִ� ĳ�����ΰ��� ����
					_pWorld->pIOnlineChar->SetInVillageDefenceInterface(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bVillageInLimit);
					_pWorld->pMyData->SetInVillageDefenceInterface(lpOnCharPlayerInfo->bVillageInLimit);

					_pWorld->pIOnlineChar->SetRank(_pWorld->pMyData->GetMyCharUniqueID(), _pWorld->pMyData->nRank);
					_pWorld->pIOnlineChar->SetScore(_pWorld->pMyData->GetMyCharUniqueID(), _pWorld->pMyData->nScore);

					// ��� ó��
					if(_pWorld->pMyData->GetGuildName())
					{
						_pWorld->pIOnlineChar->SetGuildData(lpOnCharPlayerInfo->uiAccount, _pWorld->pMyData->GetGuildName(), _pWorld->pMyData->GetGuildClass(), _pWorld->pMyData->GetGuildFlag());
					}
					_pWorld->pOnlineMap->SetCurrentFocusCharID(_pWorld->pMyData->GetMyCharUniqueID());
					_pWorld->pOnlineMap->PlayFitMusic();
					_pWorld->pOnlineClientChatting->MessageSend_MoveSell(TempPortal->siPortalMoveMapIndex, lpOnCharPlayerInfo->siX, lpOnCharPlayerInfo->siY);

				}
				else
				{
					IP = lpOnCharPlayerInfo->siDBAccount;

					_pWorld->pIOnlineChar->Init(lpOnCharPlayerInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharPlayerInfo->uiKind, lpOnCharPlayerInfo->siX, lpOnCharPlayerInfo->siY, IP, 0, lpOnCharPlayerInfo->siFightingPower, _pWorld,(UI16*)lpOnCharPlayerInfo->uiEquipWeapon);
					_pWorld->pIOnlineChar->SetAttackMode(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bAttackMode);
					_pWorld->pIOnlineChar->SetEquipItem(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->uiEquipWeapon);							
					_pWorld->pIOnlineChar->SetSiegeWarfare_Hp(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->siFieldHp);
					_pWorld->pIOnlineChar->SetCaptain(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->bCaptain);
					_pWorld->pIOnlineChar->SetSiegeWarfare_CurrentSupplyGoodsAmount(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->siSupplyCharge);
					
					// robypark 2004/12/17 17:29
					// �����������̽��� �ִ� ĳ�����ΰ��� ����
					_pWorld->pIOnlineChar->SetInVillageDefenceInterface(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bVillageInLimit);

					// ������ ������ �� ���õ� ������ ���� ��� ���
					LPSSiegeUnitTable lpsSiegeUnitTable = _pWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTable(lpOnCharPlayerInfo->uiKind);

					// ������ ������ ���� ���
					if (NULL != lpsSiegeUnitTable)
					{
						// ������ ���� ����� ĳ������ �� ������ ���ٸ�
						if(_pWorld->pIOnlineChar->GetDataType(lpOnCharPlayerInfo->uiAccount) == 1)
						{
							if (_pWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
							{
								// �������� ��û�Ѵ�.
								_pWorld->pOnlineClient->SendPlayerDetailInfo(lpOnCharPlayerInfo->uiAccount);
								_pWorld->pIOnlineChar->SetDataType(lpOnCharPlayerInfo->uiAccount, 2);	// ���� �ٿ�ε� ��� ��...
							}
						}
					}
				}				

				_pWorld->pIOnlineChar->SetGuildData(lpOnCharPlayerInfo->uiAccount, _pWorld->pIOnlineChar->GetGuildName(lpOnCharPlayerInfo->uiAccount), lpOnCharPlayerInfo->siGuildInGrade, _pWorld->pIOnlineChar->GetGuildFlag(lpOnCharPlayerInfo->uiAccount));
				_pWorld->pIOnlineChar->SetMercenary(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->siVMercenaryKind);

				// ���� ����
				if(lpOnCharPlayerInfo->siPrivateMarketItemNum >= 0) 
				{
					OnBaseBoothItem				*lpOnBaseBoothItem, *pTempOnBaseBoothItem;
					SellItemInfo				TempSellItemInfo;

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
				_pWorld->pIOnlineChar->Init(lpOnCharMonsterInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharMonsterInfo->uiKind, lpOnCharMonsterInfo->siX, lpOnCharMonsterInfo->siY, IP, 0.0f, lpOnCharMonsterInfo->siFightingPower, _pWorld,NULL);
				
				lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharMonsterInfo));
			}
			else if(ISSHIP(lpOnCharBaseInfo->uiAccount))
			{
				lpOnCharShipInfo = (OnCharShipInfo*)lpOnCharBaseInfo;

				// �ٸ� ĳ������ ���� ������ �����Ѵ�.						
				_pWorld->pIOnlineChar->Init(lpOnCharShipInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharShipInfo->uiKind, lpOnCharShipInfo->siX, lpOnCharShipInfo->siY, IP, 0.0f, 0, _pWorld,NULL);

				lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharShipInfo));
			}
		}
		
		// ��� Item �ʱ�ȭ
		pReceivedFieldItemInfo	=	(OnlineReceiveFieldItemInfo*)((CHAR*)lpOnCharBaseInfo);		
		for(TempCount = 0; TempCount < lpOnResMoveMapOkMsg->uiAroundItemNum; TempCount++)
		{
			// �ʵ忡 �������� �����Ѵ�.
			_pWorld->pOnlineMap->SetID(pReceivedFieldItemInfo[TempCount].siX, pReceivedFieldItemInfo[TempCount].siY, 1, 1, 0, 0, pReceivedFieldItemInfo[TempCount].uiItemID, pReceivedFieldItemInfo[TempCount].uiQuantity);
		}
		
		// ���ο� �� ����
		_pWorld->pMyData->SetMoney(lpOnResMoveMapOkMsg->mnMoney);
		
		// Game �� �����ϰ� ��
		_pWorld->pMyData->SetGotoPortal(FALSE, 0);
		_pWorld->pMyData->SetWaitGotoPortalResponse(FALSE);		
		_pWorld->pOnlineCharUI->SetAction(TRUE);

		// Warp Effect ����
		if(TempPortal->i64PortalMoney > 0)
		{
			SEffectData		TempEffect;
			SI16			TempX, TempY;

			_pWorld->pIOnlineChar->GetPos(_pWorld->pMyData->GetMyCharUniqueID(), &TempX, &TempY);
			TempEffect.m_Type                                 = EFFECTTYPE_WARPEFFECT;
			TempEffect.m_EffectData_WarpEffect.m_Frame        = 0;
			TempEffect.m_EffectData_WarpEffect.m_XPos         = TempX;
			TempEffect.m_EffectData_WarpEffect.m_YPos         = TempY;
			_pWorld->pOnlineFieldEffect->AddEffect(TempEffect);
		}

		if(_pWorld->pOnlineMiniMap && TempPortal->siPortalMoveMapIndex == 0)
			_pWorld->pOnlineMiniMap->MakeTargetList();
	}
	else if(lpOnResMoveMapMsg->dwResponse == ON_RET_NO)
	{
		// ��Ż�� �� ���� ����.
		_pWorld->pMyData->SetGotoPortal(FALSE, 0);
		_pWorld->pMyData->SetWaitGotoPortalResponse(FALSE);		
		_pWorld->pOnlineCharUI->SetAction(TRUE);
	}

	return 0;
}

// �÷��̾� ������ �޴´�.
UI16 OnlineMsgProc::OnResPlayerInfo(BYTE *pMsg)
{
	SI32						i;
	DWORD						IP;
	CHAR						TempBuffer[128];
	
	OnResPlayerInfoMsg*		pRecvPlayerMsg		= (OnResPlayerInfoMsg*)pMsg;
	OnCharBaseInfo* 		lpOnCharBaseInfo	= (OnCharBaseInfo*)((CHAR*)pRecvPlayerMsg + sizeof(OnResPlayerInfoMsg));

	OnCharPlayerInfo*		lpOnCharPlayerInfo;
	OnCharMonsterInfo*		lpOnCharMonsterInfo;
	OnCharShipInfo*			lpOnCharShipInfo;
	
	for(i = 0; i < pRecvPlayerMsg->uiCharNum; i++)
	{
		// �÷��̾� ������ �޾Ҵٰ� �����Ѵ�.
		_pWorld->RPI.Del(lpOnCharBaseInfo->uiAccount);
		
		ZeroMemory(TempBuffer, 128);
		IP = 0;
		if(ISPLAYER(lpOnCharBaseInfo->uiAccount))
		{
			lpOnCharPlayerInfo = (OnCharPlayerInfo*)lpOnCharBaseInfo;

			if(_pWorld->pIOnlineChar->IsExistUniqueID(lpOnCharBaseInfo->uiAccount)  == TRUE)
			{
				_pWorld->pIOnlineChar->SetPlayerInfo(lpOnCharBaseInfo, 0, NULL);
				_pWorld->pIOnlineChar->SetEquipItem(lpOnCharPlayerInfo->uiAccount, (UI16*)lpOnCharPlayerInfo->uiEquipWeapon);
			}
			else
			{
				if(_pWorld->pMyData->GetMyCharUniqueID() == lpOnCharPlayerInfo->uiAccount)
				{
					CopyMemory(TempBuffer, _pWorld->pMyData->GetMyCharName(), ON_ID_LENGTH);
					IP = lpOnCharPlayerInfo->siDBAccount;

					_pWorld->pIOnlineChar->Init(lpOnCharPlayerInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharPlayerInfo->uiKind, lpOnCharPlayerInfo->siX, lpOnCharPlayerInfo->siY, IP, 0, lpOnCharPlayerInfo->siFightingPower, _pWorld,(UI16*)lpOnCharPlayerInfo->uiEquipWeapon);
					_pWorld->pIOnlineChar->SetAttackMode(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bAttackMode);
					_pWorld->pIOnlineChar->SetEquipItem(lpOnCharPlayerInfo->uiAccount, (UI16*)lpOnCharPlayerInfo->uiEquipWeapon);
					_pWorld->pIOnlineChar->SetSiegeWarfare_Hp(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->siFieldHp);
					_pWorld->pIOnlineChar->SetCaptain(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->bCaptain);
					_pWorld->pIOnlineChar->SetSiegeWarfare_CurrentSupplyGoodsAmount(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->siSupplyCharge);

					// robypark 2004/12/17 17:29
					// �����������̽��� �ִ� ĳ�����ΰ��� ����
					_pWorld->pIOnlineChar->SetInVillageDefenceInterface(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bVillageInLimit);
					_pWorld->pMyData->SetInVillageDefenceInterface(lpOnCharPlayerInfo->bVillageInLimit);

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
					_pWorld->pIOnlineChar->SetAttackMode(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bAttackMode);
					_pWorld->pIOnlineChar->SetEquipItem(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->uiEquipWeapon);
					_pWorld->pIOnlineChar->SetSiegeWarfare_Hp(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->siFieldHp);
					_pWorld->pIOnlineChar->SetCaptain(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->bCaptain);
					_pWorld->pIOnlineChar->SetSiegeWarfare_CurrentSupplyGoodsAmount(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->siSupplyCharge);

					// robypark 2004/12/17 17:29
					// �����������̽��� �ִ� ĳ�����ΰ��� ����
					_pWorld->pIOnlineChar->SetInVillageDefenceInterface(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bVillageInLimit);

					// ������ ������ �� ���õ� ������ ���� ��� ���
					LPSSiegeUnitTable lpsSiegeUnitTable = _pWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTable(lpOnCharPlayerInfo->uiKind);

					// ������ ������ ���� ���
					if (NULL != lpsSiegeUnitTable)
					{
						// ������ ���� ����� ĳ������ �� ������ ���ٸ�
						if(_pWorld->pIOnlineChar->GetDataType(lpOnCharPlayerInfo->uiAccount) == 1)
						{
							if (_pWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
							{
								// �������� ��û�Ѵ�.
								_pWorld->pOnlineClient->SendPlayerDetailInfo(lpOnCharPlayerInfo->uiAccount);
								_pWorld->pIOnlineChar->SetDataType(lpOnCharPlayerInfo->uiAccount, 2);	// ���� �ٿ�ε� ��� ��...
							}
						}
					}
				}
			}

			_pWorld->pIOnlineChar->SetGuildData(lpOnCharPlayerInfo->uiAccount, _pWorld->pIOnlineChar->GetGuildName(lpOnCharPlayerInfo->uiAccount), lpOnCharPlayerInfo->siGuildInGrade, _pWorld->pIOnlineChar->GetGuildFlag(lpOnCharPlayerInfo->uiAccount));
			_pWorld->pIOnlineChar->SetMercenary(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->siVMercenaryKind);

			// ���� ����
			if(lpOnCharPlayerInfo->siPrivateMarketItemNum >= 0) 
			{
				_pWorld->pIOnlineChar->SetBoothOpen(lpOnCharPlayerInfo->uiAccount, TRUE);

				OnBaseBoothItem				*lpOnBaseBoothItem, *pTempOnBaseBoothItem;
				SellItemInfo				TempSellItemInfo;

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
			if(_pWorld->pIOnlineChar->IsExistUniqueID(lpOnCharBaseInfo->uiAccount)  == TRUE)
			{
				_pWorld->pIOnlineChar->SetPlayerInfo(lpOnCharBaseInfo, 1, NULL);
			}
			else
			{
				_pWorld->pIOnlineChar->Init(lpOnCharMonsterInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharMonsterInfo->uiKind, lpOnCharMonsterInfo->siX, lpOnCharMonsterInfo->siY, IP, 0.0f, lpOnCharMonsterInfo->siFightingPower, _pWorld,NULL);
			}
			
			lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharMonsterInfo));
		}
		else if(ISSHIP(lpOnCharBaseInfo->uiAccount))
		{
			lpOnCharShipInfo = (OnCharShipInfo*)lpOnCharBaseInfo;

			// �ٸ� ĳ������ ���� ������ �����Ѵ�.						
			if(_pWorld->pIOnlineChar->IsExistUniqueID(lpOnCharBaseInfo->uiAccount)  == TRUE)
			{
				_pWorld->pIOnlineChar->SetPlayerInfo(lpOnCharBaseInfo, 2, NULL);
			}
			else
			{
				_pWorld->pIOnlineChar->Init(lpOnCharShipInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharShipInfo->uiKind, lpOnCharShipInfo->siX, lpOnCharShipInfo->siY, IP, 0.0f, 0, _pWorld,NULL);
			}

			lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharShipInfo));
		}
	}		

	return 0;
}

UI16 OnlineMsgProc::OnResBattle(BYTE *pMsg)
{
	REPORT_INITBATTLE *pBattleMsg = (REPORT_INITBATTLE*)pMsg;

	if(pBattleMsg == NULL)	return 0;
	
	if(pBattleMsg->subcommand == ON_RET_BATTLE_BEGIN)
	{
		REPORT_INITBATTLE		*pREPORT_INITBATTLE;
		
//		_pWorld->m_LanguageMode = _pWorld->pOnlinePannel->pOnlineChatEditControl->GetLanguage();
		
		// �ʱ�ȭ�� �Ѵ�.
		ZeroMemory(&GOnlineImjin2I.BattleInfo, sizeof(GOnlineImjin2I.BattleInfo));
		pREPORT_INITBATTLE			=	(REPORT_INITBATTLE*)pBattleMsg;	

		GOnlineImjin2I.BattleInfo.siMyCharUniqueID = _pWorld->pMyData->GetMyCharUniqueID();
		GOnlineImjin2I.BattleInfo.uiRandomSeed     = pREPORT_INITBATTLE->randomSeed;
		GOnlineImjin2I.BattleInfo.UserMoney        = pREPORT_INITBATTLE->userMoney; 

		GOnlineImjin2I.BattleInfo.PlayerNumber     = pREPORT_INITBATTLE->numPlayer;
		GOnlineImjin2I.BattleInfo.IsSingleMode     = pREPORT_INITBATTLE->isSingleMode;


		int a =  sizeof(OnImjinBattlePlayerInfo);

		CopyMemory(GOnlineImjin2I.BattleInfo.OIBPI, pREPORT_INITBATTLE->OIBPI, sizeof(OnImjinBattlePlayerInfo) * pREPORT_INITBATTLE->numPlayer);
		/*
		pBattleStartMsg			=	(OnResBattleStartMsg*)pBattleMsg;	
		pBPI					=	(OnBattlePlayerInfo*)((CHAR*)pBattleStartMsg + sizeof(OnResBattleStartMsg));
		
		// ���� ���� ID�� �����Ѵ�.
		GOnlineImjin2I.BattleInfo.siMyCharUniqueID	=	pMyData->GetMyCharUniqueID();
		
		// ���� ������ IP�� �����Ѵ�.
		pIP											=	(CHAR*)&pBattleStartMsg->dwBattleServerIP;
		
		wsprintf(GOnlineImjin2I.BattleInfo.szBattleServerIP, "%u.%u.%u.%u", UI08(pIP[0]), UI08(pIP[1]), UI08(pIP[2]), UI08(pIP[3]));
		
		// ���� ������ ä���� �����Ѵ�.
		GOnlineImjin2I.BattleInfo.siBattleServerChannel		=	pBattleStartMsg->uiBattleServerChannelNum;
		GOnlineImjin2I.BattleInfo.uiRandomSeed              =   pBattleStartMsg->uiRandomSeed;
		*/
		
		_pWorld->m_BattleAfterMiniMapFlag = _pWorld->pOnlineMiniMap->IsActive();

		_pWorld->pOnlineClientChatting->MessageSend_NotIsMap();

		// �ŷ� ���̶�� �ݾ� �ش�.
		_pWorld->pOnlineTrade->m_pMsgBox->Initialize();


		// ������ ���۵Ǿ���.
		return ON_MAINPOLL_INITGOTOIMJINGAME;

		/*uiNextMenu	=	ON_MAINPOLL_ENTERBATTLEEFFECT;	
		m_siUpdateMethod = 1 + (rand() % 2);
		m_siUpdateCounter = 0;*/
		
	}
	else if(pBattleMsg->subcommand == ON_RET_BATTLE_NO)
	{
		// ������ ��ҵǾ���.
		_pWorld->pMyData->SetWaitBattleResponse(FALSE);
		_pWorld->pOnlineCharUI->SetAction(TRUE);
	}
	else if(pBattleMsg->subcommand == ON_RET_BATTLE_WAIT)
	{
		// ������ ����϶�� �޽����� �Դ�.
		
		// �� �ڸ��� �����Ѵ�.
		_pWorld->pIOnlineChar->AddOrder(_pWorld->pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);			
		
		// ������� ����� ���� ���ϴ� ���·� ���д�.
		_pWorld->pOnlineCharUI->SetAction(FALSE);
	}
	else
	{
		clGrp.Error("ON_RESPONSE_BATTLE", "Unknow Data : [%d]", pBattleMsg->subcommand);
	}

	return 0;
}

UI16 OnlineMsgProc::OnResBattlePlay(BYTE *pMsg)
{
	OnResBattlePlayer* lpOnResBattlePlayer = (OnResBattlePlayer*)pMsg;

	if(lpOnResBattlePlayer)
	{
		// �� Character�� ã�Ƽ� Flag�� ����
		_pWorld->pIOnlineChar->SetBattleFlag(lpOnResBattlePlayer->uiUniqueID, lpOnResBattlePlayer->fFlag);

		// �ڽ��̰ų� �÷��̾ �ƴ� ���带 ���
		if((ISPLAYER(lpOnResBattlePlayer->uiUniqueID) == FALSE) || (lpOnResBattlePlayer->uiUniqueID == _pWorld->pMyData->GetMyCharUniqueID()))
		{
			PlayTryAttackEffect(_pWorld->pIOnlineChar->GetKind(lpOnResBattlePlayer->uiUniqueID));
		}

		if(lpOnResBattlePlayer->uiUniqueID == _pWorld->pMyData->GetMyCharUniqueID())
		{
			if(lpOnResBattlePlayer->fFlag)
			{
				if(_pWorld->pMyData->IsWaitBattleResponse() == FALSE)
				{
					// ĳ���Ϳ��� ���� ����� �����ش�.
					_pWorld->pIOnlineChar->AddOrder(_pWorld->pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
	
					// �� �̻��� ����� �Է��� ���� �ʴ´�.
					_pWorld->pOnlineCharUI->SetAction(FALSE);
				}
			}
		}
	}

	return 0;
}

// �Ǻ��� �ٲ� �Դ´�.
UI16 OnlineMsgProc::OnChangeChlothes(BYTE *pMsg)
{
	OnChangeClothesMsg*	lpOnChangeClothesMsg = (OnChangeClothesMsg*)pMsg;	
	_pWorld->pIOnlineChar->SetEquipItem(lpOnChangeClothesMsg->uiCharacID, lpOnChangeClothesMsg->uiItemID);
	return 0;
}

// ���� �ȳ� ���� �д´�.
UI16 OnlineMsgProc::OnResReadDescrit(BYTE *pMsg)
{
	OnResVillageDescript* pOnResVillageDescript = (OnResVillageDescript*)pMsg;
	char*				  lpBuffer;
	char				  TempBuffer[1024];

	lpBuffer = (char*)((char*)pOnResVillageDescript + sizeof(OnResVillageDescript));
	ZeroMemory(TempBuffer, 1024);
	CopyMemory(TempBuffer, lpBuffer, pOnResVillageDescript->uiLength);

	_pWorld->pOnlineVillageDescriptList->SetDescriptData(pOnResVillageDescript->uiVillageCode, (char*)TempBuffer);

	return 0;
}

// ������ ������ ��� �´�.
UI16 OnlineMsgProc::OnResVillageNation(BYTE *pMsg)
{
	OnResVillageGetNation*		lpOnResVillageGetNation = (OnResVillageGetNation*)pMsg;

	if(lpOnResVillageGetNation->dwResponse != ON_RET_OK)	return 0;

	switch(lpOnResVillageGetNation->cNationCode)
	{
		case NATION_CODE_KOREA:
			lpOnResVillageGetNation->cNationCode = ON_COUNTRY_KOREA;
			break;

		case NATION_CODE_CHINA:
			lpOnResVillageGetNation->cNationCode = ON_COUNTRY_CHINA;
			break;

		case NATION_CODE_JAPAN:
			lpOnResVillageGetNation->cNationCode = ON_COUNTRY_JAPAN;
			break;

		case NATION_CODE_TAIWAN:
			lpOnResVillageGetNation->cNationCode = ON_COUNTRY_TAIWAN;
			break;
	}
	_pWorld->pOnlineVillageDescriptList->SetVillageDataData(lpOnResVillageGetNation->uiVillageCode, lpOnResVillageGetNation->cNationCode
		, (char*)lpOnResVillageGetNation->GuildName
		, lpOnResVillageGetNation->siGuildFlag
		, lpOnResVillageGetNation->Durability
		, lpOnResVillageGetNation->MaxDurability
		, lpOnResVillageGetNation->dwGuildRelation
		, lpOnResVillageGetNation->siDefenceSoldier	// ���� ���� �δ��
		, lpOnResVillageGetNation->siDefenceSoldier/* lpOnResVillageGetNation->siMaxDefenceSoldier*/	// �ִ� ���� �δ��
		, lpOnResVillageGetNation->siAttackType	// ���� ���� Ÿ��
		);

	if(_pWorld->pOnlineMap->m_MapInfo.uiVillageCode)
	{
		_pWorld->pOnlineClient->SendRequestVillageInfo(_pWorld->pOnlineMap->m_MapInfo.uiVillageCode);
	}

	return 0;
}

// �̸�Ƽ��.
UI16 OnlineMsgProc::OnResImoticon(BYTE *pMsg)
{
	OnResImoticonMsg *lpOnResImoticonMsg = (OnResImoticonMsg*)pMsg;
	_pWorld->pIOnlineChar->SetImoticon(lpOnResImoticonMsg->UserID, lpOnResImoticonMsg->uiImoticonID);
	return 0;
}


// �ŷ� ���
UI16 OnlineMsgProc::OnResAcceptTrade(BYTE *pMsg)
{
	OnResAcceptTradeMsg		*pOnResAcceptTradeMsg;
	pOnResAcceptTradeMsg	=	(OnResAcceptTradeMsg*)pMsg;
	
	if(pOnResAcceptTradeMsg->dwResponse == ON_RET_OK)
	{
		if(!_pWorld->pOnlineTrade->IsActive())
		{
			_pWorld->pOnlineTrade->SetActive(TRUE);
		}

		OnResAcceptTradeOkMsg *pOnResAcceptTradeOkMsg = (OnResAcceptTradeOkMsg*)pOnResAcceptTradeMsg;

		_pWorld->pOnlineTrade->SetFriendInfo(pOnResAcceptTradeOkMsg->szOppGuildID, pOnResAcceptTradeOkMsg->siOppGuildClass, pOnResAcceptTradeOkMsg->siOppLevel);

/*					// ��� ���� �����Ǹ� �ʾ� �ش�.					
		CHAR szIP[32];
		CHAR *pTemp=NULL;
		
		pTemp	=	(CHAR*)(&pOnResAcceptTradeMsg->dwIP);					
		sprintf( szIP, "%u.%u.%u.%u", UI08(pTemp[0]), UI08(pTemp[1]), UI08(pTemp[2]), UI08(pTemp[3]) );
		sprintf( szIP, "192.168.0.226");

		FILE *fp = fopen("tempip.txt","rt");
		if (fp) 
		{
			fgets(szIP,256,fp);
			fclose(fp);
		}
		
		pOnlineTrade->SetIP( szIP );*/
	}
	else
	{
		_pWorld->pOnlineTrade->SetActive(FALSE);
	}

	return 0;
}
               
UI16 OnlineMsgProc::OnResAcceptParty(BYTE *pMsg)
{
	CHAR		   pName[ON_ID_LENGTH + 1];
	SI16		   siNum;
	OnPartyInfo*   pOnPartyInfo;
	ZeroMemory(pName, sizeof(pName));

	OnResAcceptPartyMsg			*pOnResAcceptPartyMsg;
	pOnResAcceptPartyMsg	=	(OnResAcceptPartyMsg*)pMsg;

	if(pOnResAcceptPartyMsg)
	{
		sprintf(pName,pOnResAcceptPartyMsg->szID);
		if(pOnResAcceptPartyMsg->dwResponse == ON_RET_OK)
		{
			//000�� ��Ƽ�� �����Ͽ����ϴ�.
			pOnPartyInfo = (OnPartyInfo*)((BYTE*)pOnResAcceptPartyMsg + sizeof(OnResAcceptPartyMsg));
			siNum = pOnResAcceptPartyMsg->siNum;

			//pName
			_pWorld->pOnlineParty->m_pMsgBox->SetMsg( ON_MSGBOX_TYPE_OK,
			"", _pWorld->pOnlineText->Get(2264),pName);

			_pWorld->pOnlineParty->SetPartyMsg(3);
		
			_pWorld->pOnlineParty->SetPartyInfo(pOnPartyInfo,siNum);
		}
		else
		{
			//000�� ��Ƽ�� �����Ͽ����ϴ�.

			//pName

			_pWorld->pOnlineParty->SetPartyMsg(3);
			_pWorld->pOnlineParty->m_pMsgBox->SetMsg( ON_MSGBOX_TYPE_OK,
			"", _pWorld->pOnlineText->Get(2265),pName);
		}
	}
	return 0;
}

UI16 OnlineMsgProc::OnResLeaveParty(BYTE *pMsg)
{
	CHAR				    pName[ON_ID_LENGTH + 1];
	CHAR				    pNameLeader[ON_ID_LENGTH + 1];
	SI16					siNum; 
	OnResLeavePartyMsg*		pOnResLeavePartyMsg;
	OnPartyInfo*		    pOnPartyInfo;
	ZeroMemory(pName, sizeof(pName));
	ZeroMemory(pNameLeader,sizeof(pNameLeader));
	

	pOnResLeavePartyMsg =   (OnResLeavePartyMsg*)pMsg;

	if(pOnResLeavePartyMsg)
	{
		if(pOnResLeavePartyMsg->dwResponse == ON_RET_OK)
		{
			pOnPartyInfo = (OnPartyInfo*)((BYTE*)pOnResLeavePartyMsg + sizeof(OnResLeavePartyMsg));
			
			siNum = pOnResLeavePartyMsg->siMembersNum;
			sprintf(pName,pOnResLeavePartyMsg->szLeavedPlayerID);
			

			for(int i=0 ; i < siNum; i++)
			{
				 if(pOnPartyInfo[i].bIsPartyLeader == TRUE)
					sprintf(pNameLeader,pOnPartyInfo[i].Name);
			}
			

			if(strcmp(_pWorld->pOnlineParty->m_szPartyLeader, pNameLeader) == 0)
			{
				_pWorld->pOnlineParty->m_pMsgBox->SetMsg( ON_MSGBOX_TYPE_OK,
				"", _pWorld->pOnlineText->Get(2266),pName);
				
			}
			else if(siNum == 0)
			{
				_pWorld->pOnlineParty->m_pMsgBox->SetMsg( ON_MSGBOX_TYPE_OK,
				"", _pWorld->pOnlineText->Get(2266),pName);
			}
			else 
			{
				_pWorld->pOnlineParty->m_pMsgBox->SetMsg( ON_MSGBOX_TYPE_OK,
				"", _pWorld->pOnlineText->Get(2268),pName,pNameLeader);
			}
			

			_pWorld->pOnlineParty->SetPartyMsg(3);
		
			_pWorld->pOnlineParty->SetPartyInfo(pOnPartyInfo,siNum);

		}
		else if(pOnResLeavePartyMsg->dwResponse == ON_RET_NO)
		{
			_pWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK,
			"", _pWorld->pOnlineText->Get(2269));
			
		}
	}

	return 0;
}

UI16 OnlineMsgProc::OnResPartyDeleted(BYTE *pMsg)
{
	OnMsg	*pOnMsg;
	pOnMsg = (OnMsg*)pMsg;

	if(pOnMsg)
	{
		_pWorld->pOnlineParty->m_pMsgBox->SetMsg( ON_MSGBOX_TYPE_OK,
			"", _pWorld->pOnlineText->Get(2270));

		_pWorld->pOnlineParty->SetPartyInfo(NULL,0);

		_pWorld->pOnlineParty->SetPartyMsg(6); //PARTY_DISS

		if(_pWorld->pOnlineParty->m_siNoField != -1)
			_pWorld->pOnlineFieldArea->SetFieldArea( _pWorld->pOnlineParty->m_siNoField );
		_pWorld->pOnlineParty->m_siNoField = -1;
	}
	return 0;
}

UI16 OnlineMsgProc::OnResCancelRequest(BYTE *pMsg)
{
	OnResponseMsg	*pOnResponseMsg;
	pOnResponseMsg	= (OnResponseMsg*)pMsg;

	if(pOnResponseMsg)
	{
		if(pOnResponseMsg->dwResponse == ON_RET_OK)
		{
			_pWorld->pOnlineParty->m_pMsgBox->SetMsg( ON_MSGBOX_TYPE_OK,
				"",_pWorld->pOnlineText->Get(2271));
			_pWorld->pOnlineParty->SetPartyMsg(3); //PARTY_ACTION

		}
		else if(pOnResponseMsg->dwResponse == ON_RET_NO)
		{
			_pWorld->pOnlineParty->m_pMsgBox->SetMsg( ON_MSGBOX_TYPE_OK,
				"",_pWorld->pOnlineText->Get(2272));
			_pWorld->pOnlineParty->SetPartyMsg(3); //PARTY_ACTION
			//�ٽ� ������ ��ٸ��� �޼����� ���;� �ϴ���?
		}
	}
	return 0;
}


UI16 OnlineMsgProc::OnResQuitParty(BYTE *pMsg)
{
	CHAR		   pName[ON_ID_LENGTH + 1];
	SI16		   siNum;
	OnPartyInfo*   pOnPartyInfo;
	ZeroMemory(pName, sizeof(pName));

	OnResAcceptPartyMsg			*pOnResAcceptPartyMsg;
	pOnResAcceptPartyMsg	=	(OnResAcceptPartyMsg*)pMsg;

	if(pOnResAcceptPartyMsg)
	{
		sprintf(pName,pOnResAcceptPartyMsg->szID);
		if(pOnResAcceptPartyMsg->dwResponse == ON_RET_OK)
		{
			//000�� ��Ƽ�� �����Ͽ����ϴ�.

			pOnPartyInfo = (OnPartyInfo*)((BYTE*)pOnResAcceptPartyMsg + sizeof(OnResAcceptPartyMsg));
			siNum = pOnResAcceptPartyMsg->siNum;

			//pName
			_pWorld->pOnlineParty->m_pMsgBox->SetMsg( ON_MSGBOX_TYPE_OK,
			"", _pWorld->pOnlineText->Get(2273),pName);

			_pWorld->pOnlineParty->SetPartyMsg(3); //PARTY_ACTION
		
			_pWorld->pOnlineParty->SetPartyInfo(pOnPartyInfo,siNum);
		}
		else
		{
			//pName
			_pWorld->pOnlineParty->m_pMsgBox->SetMsg( ON_MSGBOX_TYPE_OK,
			"", _pWorld->pOnlineText->Get(2274),pName);

			_pWorld->pOnlineParty->SetPartyMsg(6); //PARTY_DISS

			if(_pWorld->pOnlineParty->m_siNoField != -1)
				_pWorld->pOnlineFieldArea->SetFieldArea( _pWorld->pOnlineParty->m_siNoField );
			_pWorld->pOnlineParty->m_siNoField = -1;
		}
	}
	return 0;
}

// �ŷ� ��û
UI16 OnlineMsgProc::OnResTrade(BYTE *pMsg)
{
	OnResAskTradeMsg *pOnReqAskTradeMsg = (OnResAskTradeMsg*)pMsg;
	
	if(pOnReqAskTradeMsg)
	{
		if(pOnReqAskTradeMsg->dwResponse == ON_RET_NO)
		{
			_pWorld->pOnlineTrade->SetActive(FALSE);
		}
	}

	return 0;
}

// ��Ƽ�� ��û���� �޼����� �޾�.'��Ƽ ��û���Դϴ�'��� �޼����� �Ѹ���.
UI16 OnlineMsgProc::OnResParty(BYTE *pMsg)
{
	OnResponseMsg *pOnResponseMsg = (OnResponseMsg*)pMsg;
	CHAR		   pName[ON_ID_LENGTH + 1];
	ZeroMemory(pName, sizeof(pName));

	if(pOnResponseMsg)
	{
		if(pOnResponseMsg->dwResponse == ON_RET_OK)
		{
			//����
			//��Ƽ ��û���Դϴ�. �޼����� �Ѹ���.
			_pWorld->pOnlineParty->m_pMsgBox->SetMsg( ON_MSGBOX_TYPE_CANCEL, 
			"", _pWorld->pOnlineText->Get(2275) );

		}
		else if(pOnResponseMsg->dwResponse == ON_RET_NO)
		{
			
			//����.
			//��Ƽ���� ���� ���.
			/*
			if(pOnResPartyMsg->dwErrorCode == ON_RET_PARTY_FULL)
				_pWorld->pOnlineParty->m_pMsgBox->SetMsg( ON_MSGBOX_TYPE_CANCEL, 
				"", "�� ��Ƽ�� 7�� �̻� �����Ҽ� �����ϴ�.", pName );
			//�ٸ� ��Ƽ�� ���ԵǾ� �ִ� ���.
			else if(pOnResPartyMsg->dwErrorCode == ON_RET_PARTY_DIFPARTY)
				_pWorld->pOnlineParty->m_pMsgBox->SetMsg( ON_MSGBOX_TYPE_CANCEL, 
				"", "%s �� �ٸ� ��Ƽ�� ���ԵǾ� �ִ� �����Դϴ�.", pName );
			//��Ƽ���� �ƴѰ��.
			else if(pOnResPartyMsg->dwErrorCode ==ON_RET_NO_PARTY_CAP)
				_pWorld->pOnlineParty->m_pMsgBox->SetMsg( ON_MSGBOX_TYPE_CANCEL, 
				"", "��Ƽ�常�� ��Ƽ�� ��û�Ҽ� �ֽ��ϴ�", pName );
			//�ٸ� ������ ���.
			else 
			*/
			_pWorld->pOnlineParty->m_pMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, 
			"", _pWorld->pOnlineText->Get(2276), pName );
		}
	}
	return 0;
}

UI16 OnlineMsgProc::OnResAksParty(BYTE *pMsg)
{
	OnReceiveAskPartyMsg	*pOnReceiveAskPartyMsg;

	CHAR					pName[ON_ID_LENGTH + 1];
	ZeroMemory(pName, sizeof(pName));

	pOnReceiveAskPartyMsg	=	(OnReceiveAskPartyMsg*)pMsg;	

	// �ŷ�����尡 �ƴҰ�� ��Ҹ޼��� ����
	if(!_pWorld->pOnlineTrade->m_bPermitTrade)
	{
		_pWorld->pOnlineClient->SendRequestParty(ON_RET_NO);
		_pWorld->bCharMove = TRUE;
		return 0;
	}

	sprintf(pName,pOnReceiveAskPartyMsg->szID);

	if(pName[0] != 0)
	{
		//000�� ��Ƽ�� ��û�Ͽ����ϴ�.��Ƽ�� ���� �Ͻðڽ��ϱ�?
		_pWorld->pOnlineParty->m_pMsgBox->SetMsg( ON_MSGBOX_TYPE_OKCANCEL,   "", _pWorld->pOnlineText->Get(2277), pName );
		_pWorld->pOnlineParty->SetAction(TRUE,2);//2 == PARTY_MSG
	}
	else
	{
		//ON_ID_LENGTH
		//_pWorld->pOnlineClient->SendRequestTrade(FALSE);
	}
	return 0;
}

UI16 OnlineMsgProc::OnResPartyCancel(BYTE *pMsg)
{
	_pWorld->pOnlineParty->m_pMsgBox->Initialize();
	_pWorld->pOnlineParty->SetAction(FALSE,5);  //PARTY_NO_MSG
	return 0;
}

// ���濡�� �ŷ� ��û�Ѱ��� �˷� �ش�.
UI16 OnlineMsgProc::OnResAksTrade(BYTE *pMsg)
{
	OnReceiveAskTradeMsg	*pOnReceiveAskTradeMsg;
	CHAR					pName[ON_ID_LENGTH + 1];
	ZeroMemory(pName, sizeof(pName));
	
	pOnReceiveAskTradeMsg	=	(OnReceiveAskTradeMsg*)pMsg;
	
	// �ŷ�����尡 �ƴҰ�� ��Ҹ޼��� ����
	if(!_pWorld->pOnlineTrade->m_bPermitTrade)
	{
		_pWorld->pOnlineClient->SendRequestTrade(FALSE);
		return 0;
	}

	// ������̸� �ŷ���û�� �ô´�.
	if(_pWorld->pOnlineFieldAuction->IsEntered())
	{
		_pWorld->pOnlineClient->SendRequestTrade(FALSE);					
		return 0;
	}

	strcpy(pName, pOnReceiveAskTradeMsg->szID);
	// ����ڰ��� ���� �ƴҶ��� �ŷ� ����
	if(pName[0] != 0)
	{
		if(_pWorld->pOnlineInventory->GetLCInventory()->GetDragSlot() != -1)
			_pWorld->pOnlineInventory->GetLCInventory()->StopDrag();

		_pWorld->pOnlineTrade->SetCharID( pName );					
		_pWorld->pOnlineTrade->m_pMsgBox->SetMsg( ON_MSGBOX_TYPE_OKCANCEL, _pWorld->pOnlineText->Get(ON_TEXT_TRADEINFO_TRADE), _pWorld->pOnlineText->Get(ON_TEXT_TRADETEXT_REQUEST), pName );
	}
	else
	{
		_pWorld->pOnlineClient->SendRequestTrade(FALSE);
	}

	return 0;
}

// �ŷ� ���
UI16 OnlineMsgProc::OnResCancelTrade(BYTE *pMsg)
{	
	_pWorld->pOnlineTrade->m_pMsgBox->Initialize();
	_pWorld->pOnlineTrade->m_bRecvTrade = FALSE;

	// ���� ������ ������ ��Ʈ���� ���� �ִٸ� �ݴ´�. (��ҹ�ư Ŭ���� ������ ó��)
	if (_pWorld->pOnlineNumPad->pNumberControl->IsShow())
		_pWorld->pOnlineNumPad->pNumberControl->CloseNumberControl();

	_pWorld->pOnlineTrade->SetActive( FALSE );
	_pWorld->pOnlineInventory->FillItemListBox();

	return 0;
}

// �ŷ� ��ư ����
UI16 OnlineMsgProc::OnResFinishTrade(BYTE *pMsg)
{
	OnResFinishChooseTradeItemMsg *pOnResFinishChooseTradeItemMsg =	(OnResFinishChooseTradeItemMsg*)pMsg;

	if(!pOnResFinishChooseTradeItemMsg->bIsMe)
		_pWorld->pOnlineTrade->SetOkButton( pOnResFinishChooseTradeItemMsg->bFinishChooseItem );

	return 0;
}

// �ŷ� �ϴ°��� ����
UI16 OnlineMsgProc::OnResDecideTrade(BYTE *pMsg)
{
	OnResDecideTradeMsg	*pOnResDecideTradeMsg = (OnResDecideTradeMsg*)pMsg;

	if(pOnResDecideTradeMsg->dwResponse == ON_RET_OK)						// ���� ��ư�� ���� ���� �Ƶ�.
	{					
		_pWorld->pOnlineTrade->SetPushAccept(pOnResDecideTradeMsg->bIsMe);
	}
	else																	// �ŷ� ����
	{					
		_pWorld->pOnlineTrade->Success(FALSE);
	}

	return 0;
}

// �ŷ� ����
UI16 OnlineMsgProc::OnCompleteTrade(BYTE *pMsg)
{
	OnCompleteTradeMsg	*pOnCompleteTradeMsg = (OnCompleteTradeMsg*)pMsg;

	// ������ ������, �뺴 ����
	SI16 siTempPos=0;

	for(SI16 i = 0 ; i < 8; i++ )
	{
		if(_pWorld->pOnlineTrade->GetList()->GetSlotInfo(i))
		{
			if(_pWorld->pOnlineTrade->GetList()->GetSlotInfo(i)->siType == LIST_ITEM)
			{
/*							if(_pWorld->pOnlineTrade->m_pTradeItem[i].uiItemCode != 0)
				{
					_pWorld->pMyData->GetFollowersInterface()->DelItem(_pWorld->pOnlineTrade->m_pTradeItem[i].uiFollower, _pWorld->pOnlineTrade->m_pTradeItem[i].uiItemCode, _pWorld->pOnlineTrade->m_pTradeItem[i].siQuantity, &siTempPos);
					pOnlineFollowerInfo->SetCharacter(_pWorld->pOnlineTrade->m_pTradeItem[i].uiFollower);
					_pWorld->pOnlineInventory->SetMercenarySlot(_pWorld->pOnlineTrade->m_pTradeItem[i].uiFollower);
					_pWorld->pOnlineInventory->FillItemListBox();
				}*/
			}
			else if(_pWorld->pOnlineTrade->GetList()->GetSlotInfo(i)->siType == LIST_FOLLOWER)
			{
				if(_pWorld->pOnlineInventory->GetMercenarySlot() == _pWorld->pOnlineTrade->GetList()->GetSlotInfo(i)->Follower.uiSlot)
				{
					_pWorld->m_pMercenary->Set_SelectedSlot( 0 );		//.actdoll (021224)
					_pWorld->pOnlineInventory->SetMercenarySlot(0);
					_pWorld->pOnlineInventory->FillItemListBox();
				}
				
				_pWorld->pMyData->DeleteFollower(_pWorld->pOnlineTrade->GetList()->GetSlotInfo(i)->Follower.uiSlot);
			}
			else if(_pWorld->pOnlineTrade->GetList()->GetSlotInfo(i)->siType == LIST_PROPERTY)
			{
			}
		}
	}

	OnFollowerInfo	 *pFollowerInfo		= NULL;
	OnlineReceivedMyItemInfo *pItemInfo = NULL;
	
	pFollowerInfo = (OnFollowerInfo*)(pOnCompleteTradeMsg+1);
	pItemInfo	  =	(OnlineReceivedMyItemInfo*)((CHAR*)pFollowerInfo + sizeof(OnFollowerInfo));
	
	for(i = 0; i < pOnCompleteTradeMsg->uiTradeFollowerNum; i++)
	{
		// ���� ������ ����
		_pWorld->pMyData->InitFollower(pFollowerInfo->uiSlot, pFollowerInfo);						
		
		// �κ��丮 ������ ����
		for(SI16 y=0; y<pFollowerInfo->uiItemNum; y++)
		{
			_pWorld->pMyData->AddItem(pFollowerInfo->uiSlot, &pItemInfo[y]);
		}

		_pWorld->m_pMercenary->Set_SelectedSlot( pFollowerInfo->uiSlot );		//.actdoll (021224)
		_pWorld->pOnlineInventory->SetMercenarySlot(pFollowerInfo->uiSlot);
		_pWorld->pOnlineInventory->FillItemListBox();

		pFollowerInfo = (OnFollowerInfo*)((CHAR*)pItemInfo + (sizeof(OnlineReceivedMyItemInfo)*pFollowerInfo->uiItemNum));
		pItemInfo     = (OnlineReceivedMyItemInfo*)((CHAR*)pFollowerInfo + sizeof(OnFollowerInfo));
	}

	// �ŷ� ������
	if(pOnCompleteTradeMsg->uiTradeItemNum > 0)
	{
		UI16 *TemplpBuffer1, *TemplpBuffer2;
		UI16 *uiItemID	 = new UI16[pOnCompleteTradeMsg->uiTradeItemNum];
		UI16 *uiInvenPos = new UI16[pOnCompleteTradeMsg->uiTradeItemNum];
		ZeroMemory( uiItemID, sizeof( UI16 ) * (pOnCompleteTradeMsg->uiTradeItemNum) );
		ZeroMemory( uiInvenPos, sizeof( UI16 ) * (pOnCompleteTradeMsg->uiTradeItemNum) );
		for(i = 0; i < pOnCompleteTradeMsg->uiTradeItemNum; i++)
		{
			if(pOnCompleteTradeMsg->uiTradeFollowerNum > 0)
			{
				TemplpBuffer1 = (UI16*)((CHAR*)pFollowerInfo + sizeof(UI16) * (i*2));
				TemplpBuffer2 = (UI16*)((CHAR*)pFollowerInfo + sizeof(UI16) * (i*2+1));
			}
			else
			{
				TemplpBuffer1 = (UI16*)((CHAR*)pOnCompleteTradeMsg + sizeof(OnCompleteTradeMsg) + sizeof(UI16) * (i*2));
				TemplpBuffer2 = (UI16*)((CHAR*)pOnCompleteTradeMsg + sizeof(OnCompleteTradeMsg) + sizeof(UI16) * (i*2+1));
			}
			
			uiItemID[i]	  = *TemplpBuffer1;
			uiInvenPos[i] = *TemplpBuffer2;
		}
		
		// �޴� ������ ����
		for( i = 0; i < 8 ; i++ )
		{
			if(_pWorld->pOnlineTrade->GetYouList()->GetSlotInfo(i))
			{
				if(_pWorld->pOnlineTrade->GetYouList()->GetSlotInfo(i)->uiItemID != 0 || _pWorld->pOnlineTrade->GetYouList()->GetSlotInfo(i)->siType == LIST_ITEM)
				{
					for(SI16 j = 0; j < pOnCompleteTradeMsg->uiTradeItemNum; j++)
					{
						if(_pWorld->pOnlineTrade->GetYouList()->GetSlotInfo(i)->uiItemID == uiItemID[j])
						{
							MyItemData AddItemData;
							AddItemData.siPosInInventory	= uiInvenPos[j];
							AddItemData.uiID				= uiItemID[j];
							AddItemData.uiQuantity			= _pWorld->pOnlineTrade->GetYouList()->GetSlotInfo(i)->ItemCount;
							
							_pWorld->pMyData->AddItem(0, &AddItemData);
							_pWorld->pOnlineInventory->FillItemListBox();
							break;
						}
					}
				}
			}
		}

		delete [] uiItemID;
		delete [] uiInvenPos;
	}

	_pWorld->pOnlineTrade->Success(TRUE);

	_pWorld->m_pMercenary->Set_SelectedSlot( 0 );		//.actdoll (021224)
	_pWorld->pOnlineInventory->SetMercenarySlot(0);
	_pWorld->pOnlineInventory->FillItemListBox();

	return 0;
}

// �ŷ� ��ǰ ����
UI16 OnlineMsgProc::OnChangeTrade(BYTE *pMsg)
{
	OnChangeTradeGoods	*pOnChangeTradeGoods = (OnChangeTradeGoods*)pMsg;

	switch(pOnChangeTradeGoods->Kind)
	{
	case TRADE_GOODS_ITEM:
		_pWorld->pOnlineTrade->ReceiveMoney(0, LOWORD(pOnChangeTradeGoods->lParam2), HIWORD(pOnChangeTradeGoods->lParam2), 0, pOnChangeTradeGoods->uiSlot);
		break;

	case TRADE_GOODS_FOLLOWER:
		{
			OnFollowerInfo *pOnFollowerInfo = (OnFollowerInfo*)(pOnChangeTradeGoods + 1);
			MyItemData *pItemInfo =	(MyItemData*)((CHAR*)pOnFollowerInfo + sizeof(OnFollowerInfo));
			_pWorld->pOnlineTrade->ReceiveMoney(0, 0, 0, (UI08)pOnChangeTradeGoods->lParam1, pOnChangeTradeGoods->uiSlot, pItemInfo, pOnFollowerInfo);
		}
		break;

	case TRADE_GOODS_REALESTATE:
		{
			OnIndustrialComplexTradeInfo *pProperty = (OnIndustrialComplexTradeInfo*)(pOnChangeTradeGoods + 1);
			_pWorld->pOnlineTrade->ReceiveMoney(0, 0, 0, 0, pOnChangeTradeGoods->uiSlot, NULL, NULL, pProperty);
		}
		break;

	case TRADE_GOODS_MONEY:
		_pWorld->pOnlineTrade->ReceiveMoney((MONEY)pOnChangeTradeGoods->lParam1, 0, 0, 0, 0);
		break;
	}

	return 0;
}

// ���� ����
UI16 OnlineMsgProc::OnResAttackVillage(BYTE *pMsg)
{
	OnResAttackVillageMsg		*pOnResAttackVillageMsg = NULL;

	VillageHeader*				lpVillageHeader;
	SVillageData*				lpSVillageData;
	SEffectData					EffectData;
	SEffectData					TempSEffectData;				
	SI16						TempXPos;
	SI16						TempYPos;

	pOnResAttackVillageMsg	=	(OnResAttackVillageMsg*)pMsg;

	lpVillageHeader = _pWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(pOnResAttackVillageMsg->uiVillageCode);

	if(_pWorld->pIOnlineChar->IsExistUniqueID(pOnResAttackVillageMsg->uiCharID))
	{
		// ���࿡ ������ ĳ���� Kind�� ������ Ŭ���̾�Ʈ���� �������� ĳ����Kind�� �ٸ� ��쿡 ó��
		if (pOnResAttackVillageMsg->uiKind != _pWorld->pIOnlineChar->GetKind(pOnResAttackVillageMsg->uiCharID))
		{
			DWORD		IP;
			CHAR		TempBuffer[128];

			// ĳ�� ���� ���
			UI16 uiUniqueID, uiKind, uiEquipWeapon[WEAR_ITEM_NUM];
			SI16 siX, siY;
			SI32 siFightPower, siGuildInGrade, siGuildFlag, siMercenaryKind;
			CHAR TempBuffer1[128];
			ZeroMemory(TempBuffer1, sizeof(TempBuffer1));

			SI32 siCurrentHP;
			SI32 siCurrentSupply;
			BOOL bCaptain;

			uiUniqueID		= pOnResAttackVillageMsg->uiCharID;
			uiKind			= pOnResAttackVillageMsg->uiKind;
			IP				= _pWorld->pIOnlineChar->GetID(uiUniqueID);
			siFightPower	= _pWorld->pIOnlineChar->GetFightPower(uiUniqueID);
			siGuildInGrade	= _pWorld->pIOnlineChar->GetGuildInGrade(uiUniqueID);
			siGuildFlag		= _pWorld->pIOnlineChar->GetGuildFlag(uiUniqueID);
			siMercenaryKind = _pWorld->pIOnlineChar->GetMercenary(uiUniqueID);
			siCurrentHP		= _pWorld->pIOnlineChar->GetSiegeWarfare_Hp(uiUniqueID);
			siCurrentSupply	= _pWorld->pIOnlineChar->GetSiegeWarfare_CurrentSupplyGoodsAmount(uiUniqueID);
			bCaptain		= _pWorld->pIOnlineChar->GetCaptain(uiUniqueID);

			if (_pWorld->pIOnlineChar->GetCharName(uiUniqueID))
				CopyMemory(TempBuffer, _pWorld->pIOnlineChar->GetCharName(uiUniqueID), ON_ID_LENGTH);
			else
				CopyMemory(TempBuffer, _pWorld->pMyData->GetMyCharName(), ON_ID_LENGTH);

			if(_pWorld->pIOnlineChar->GetGuildName(uiUniqueID))
				CopyMemory(TempBuffer1, _pWorld->pIOnlineChar->GetGuildName(uiUniqueID), ON_ID_LENGTH);

			_pWorld->pIOnlineChar->GetPos(uiUniqueID, &siX, &siY);

			// �����ϴ� ĳ���Ͱ� ����� �ڽ��� �ƴ� ���
			if(_pWorld->pMyData->GetMyCharUniqueID() != uiUniqueID)
			{
				_pWorld->pIOnlineChar->GetEquip(uiUniqueID, uiEquipWeapon);
			}

			_pWorld->pIOnlineChar->Free(uiUniqueID);
			_pWorld->pIOnlineChar->Init(uiUniqueID, (CHAR*)TempBuffer, uiKind, siX, siY, IP, 0, siFightPower, _pWorld,(UI16*)uiEquipWeapon);
			_pWorld->pIOnlineChar->SetEquipItem(uiUniqueID, (UI16*)uiEquipWeapon);

			_pWorld->pIOnlineChar->SetSiegeWarfare_Hp(uiUniqueID, siCurrentHP);
			_pWorld->pIOnlineChar->SetSiegeWarfare_CurrentSupplyGoodsAmount(uiUniqueID, siCurrentSupply);
			_pWorld->pIOnlineChar->SetCaptain(uiUniqueID, bCaptain);

			_pWorld->pIOnlineChar->SetGuildData(uiUniqueID, (CHAR*)TempBuffer1, siGuildInGrade, siGuildFlag);
			_pWorld->pIOnlineChar->SetMercenary(uiUniqueID, siMercenaryKind);

			_pWorld->pOnlineInventory->SetMercenarySlot(0);

			_pWorld->pOnlineCharUI->SetAction(TRUE);

			if(_pWorld->pMyData->GetMyCharUniqueID() != uiUniqueID)
			{
				// ������ ĳ������ �� ������ ���ٸ�
				if(_pWorld->pIOnlineChar->GetDataType(uiUniqueID) == 1)
				{
					if (_pWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
					{
						// �������� ��û�Ѵ�.
						_pWorld->pOnlineClient->SendPlayerDetailInfo(uiUniqueID);
						_pWorld->pIOnlineChar->SetDataType(uiUniqueID, 2);	// ���� �ٿ�ε� ��� ��...
					}
				}
			}
		}
	}

	// ���Ÿ� ������ ��� ĳ���� ���� ���� ��� �ʿ����
	if (FALSE == pOnResAttackVillageMsg->bIsLong)
	{
		// ĳ���� ���� ����
		// ���� �ڱ��̸� ����
		if(pOnResAttackVillageMsg->uiCharID != _pWorld->pMyData->GetMyCharUniqueID())
		{
			_pWorld->pIOnlineChar->AddOrder(pOnResAttackVillageMsg->uiCharID, ON_ORDER_SHORTDISTANCE_ATTACKVILLAGEACTION
				, lpVillageHeader->siX, lpVillageHeader->siY);
		}
	}

	// ���� ��ġ �˻�
	TempXPos = lpVillageHeader->siX;
	TempYPos = lpVillageHeader->siY;
	if(_pWorld->pIOnlineChar->IsExistUniqueID(pOnResAttackVillageMsg->uiCharID))
	{
		SI16 siHeroX, siHeroY;

		if (FALSE == pOnResAttackVillageMsg->bIsLong)
		{
			_pWorld->pIOnlineChar->GetPos(pOnResAttackVillageMsg->uiCharID, &TempXPos, &TempYPos);

			if(lpVillageHeader->siX > TempXPos) TempXPos += 1;
			if(lpVillageHeader->siX < TempXPos) TempXPos -= 1;
			if(lpVillageHeader->siY > TempYPos) TempYPos += 1;
			if(lpVillageHeader->siY < TempYPos) TempYPos -= 1;
		}
		else
		{
			_pWorld->pIOnlineChar->GetPos(pOnResAttackVillageMsg->uiCharID, &siHeroX, &siHeroY);

			if(lpVillageHeader->siX > siHeroX) TempXPos += 1;
			if(lpVillageHeader->siX < siHeroX) TempXPos -= 1;
			if(lpVillageHeader->siY > siHeroY) TempYPos += 1;
			if(lpVillageHeader->siY < siHeroY) TempYPos -= 1;
		}
	}

	// ���� ������ ȿ�� ó��
	EffectData.m_Type = EFFECTTYPE_FIELD_CHANGENUMBER;
	EffectData.m_EffectData_FieldChangeNumber.m_Frame     = 0;
	EffectData.m_EffectData_FieldChangeNumber.m_MapXPos   = TempXPos;
	EffectData.m_EffectData_FieldChangeNumber.m_MapYPos   = TempYPos;
	EffectData.m_EffectData_FieldChangeNumber.m_bVillageDefence	= true;
	EffectData.m_EffectData_FieldChangeNumber.m_ChangeNumber    = -pOnResAttackVillageMsg->siVillageDefenceDamage;

	// �Ʊ� ����� ���� ��� �������� ����̴�
	if (_pWorld->IsMemberOfSameGuild( pOnResAttackVillageMsg->uiCharID ))
	{
		EffectData.m_EffectData_FieldChangeNumber.m_Color	= EFFECTDATA_FIELD_CHANGENUMBER_COLOR_GREEN;

		_pWorld->pOnlineFieldEffect->AddEffect(EffectData);
	}
	// ���� ��ܿ��� ���
	else if (_pWorld->pIOnlineChar->GetIsHostilityGuild( pOnResAttackVillageMsg->uiCharID ))
	{
		EffectData.m_EffectData_FieldChangeNumber.m_Color	= EFFECTDATA_FIELD_CHANGENUMBER_COLOR_RED;

		_pWorld->pOnlineFieldEffect->AddEffect(EffectData);
	}
	// else	// �� �� ��ܿ��� ���

	// ���Ÿ� ������ ��� ĳ���� ���� ����Ʈ ���� �ʿ� ����
	if (FALSE == pOnResAttackVillageMsg->bIsLong)
	{
		// robypark 2004/11/3 14:18
		// ������ ���� ������ ���� ����Ʈ ����
		// ActionMsg�� ������ ������ �ʿ䰡 ����. �̹� ó���� �Ǿ����Ƿ�.
		_pWorld->SiegeWarfare_CreateAttackEffectVillage(pOnResAttackVillageMsg->uiCharID, TempXPos, TempYPos, 0);
	}

	// ������ ������ ���� ���� ���� �� ���� �� ����
	lpSVillageData = _pWorld->pOnlineVillageDescriptList->GetVillageDataData(lpVillageHeader->uiVillageCode);				
	if(lpSVillageData)
	{
		if(lpSVillageData->m_fWait == FALSE)
		{
			if(pOnResAttackVillageMsg->vdDefence <= 0)
			{
				// ������ ��尡 ������ ���
				_pWorld->pOnlineVillageDescriptList->DelVillageDataData(lpVillageHeader->uiVillageCode);

				if(_pWorld->pOnlineMap->m_MapInfo.uiVillageCode)
				{
					_pWorld->pOnlineClient->SendRequestVillageInfo(_pWorld->pOnlineMap->m_MapInfo.uiVillageCode);
				}
			}
			else
			{
				lpSVillageData->m_vdChangedDefence    = pOnResAttackVillageMsg->vdDefence;	// ���� ����

				// ���� ���� �ִ밪���� ū ���
				if(lpSVillageData->m_vdChangedMaxDefence < lpSVillageData->m_vdChangedDefence)
				{
					lpSVillageData->m_vdChangedMaxDefence = lpSVillageData->m_vdChangedDefence;
				}
			}
		}
	}
	
	// robypark 2004/14:32
	// ġ���� ����, �߰��� ���, �Ϲ� ����, ����� ��� ȿ�� ���
	switch (pOnResAttackVillageMsg->siAttackType)
	{
		// ġ���� ����. ������ ĳ���Ϳ� ȿ�� �߰�
		case ON_RET_ATTACK_POWER:
			{				
				// ������ ĳ���� ǥ�� ����Ʈ�� ġ���� ���� ��뿡�� ����Ѵ�.
				_pWorld->pIOnlineChar->SetNewSelectedFlag(pOnResAttackVillageMsg->uiCharID, TRUE);
				_pWorld->pIOnlineChar->SetSiegeWarfare_DeathBlow(pOnResAttackVillageMsg->uiCharID);
//				_pWorld->pIOnlineChar->SetSpeech(pOnResAttackVillageMsg->uiCharID, "Deathblow!!!");
				PushEffect(EFFECT_CRITICAL, 0 , 0);	// ġ���� ���� ȿ���� ���
			}
			break;
		// �߰��� ���. �ǰ��� ĳ���Ϳ� ȿ�� �߰�
		case ON_RET_DEFENCE_POWER:
			{				
				_pWorld->pIOnlineChar->SetSiegeWarfare_Evasion(pOnResAttackVillageMsg->uiCharID);
				PushEffect(EFFECT_DEFENCE, 0 , 0);	// �߰��� ��� ȿ���� ���
			}
			break;
		// �Ϲ� ����, ���
		case ON_RET_NONE_POWER:
		default:
				//_pWorld->pIOnlineChar->SetSpeech(pOnResAttackVillageMsg->uiCharID, "Hit!!!");
			break;
	}

	// ���� ���߽� (���� ����)
	if(lpSVillageData)
	{
		if(lpSVillageData->m_fWait == FALSE)
		{
			// ���� ������ 0�� �� ��� ���� ����Ʈ ���.
			if (lpSVillageData->m_vdChangedDefence == 0)
			{
				TempXPos = lpVillageHeader->siX;
				TempYPos = lpVillageHeader->siY;

				TempSEffectData.m_fCreate = TRUE;
				TempSEffectData.m_Type    = EFFECTTYPE_VILLAGEEXPLOSION;
				TempSEffectData.m_EffectData_VillageExplosion.m_Frame = 0;
				TempSEffectData.m_EffectData_VillageExplosion.m_XPos  = TempXPos;
				TempSEffectData.m_EffectData_VillageExplosion.m_YPos  = TempYPos;

				_pWorld->pOnlineFieldEffect->AddEffect(TempSEffectData);

				// ������ ���ɵǾ��� ȿ�� �Ѹ���
				TempXPos = lpVillageHeader->siX;
				TempYPos = lpVillageHeader->siY;

				TempSEffectData.m_fCreate = TRUE;
				TempSEffectData.m_Type    = EFFECTTYPE_OCCUPIED_VILLAGE;
				TempSEffectData.m_EffectData_OccupiedVillage.m_Frame = 0;

				TempSEffectData.m_EffectData_OccupiedVillage.m_XPos  = TempXPos + 1;
				TempSEffectData.m_EffectData_OccupiedVillage.m_YPos  = TempYPos;

				// robypark 2004/12/27 18:19
				TempSEffectData.m_EffectData_OccupiedVillage.m_scMaxRepeatTimes = ON_FIELDEFFECT_ANIMATION_OCCUPIEDVILLAGE_REPEATTIMES;	// Animation ��� �ݺ� Ƚ�� ����

				_pWorld->pOnlineFieldEffect->AddEffect(TempSEffectData);

				PushEffect(EFFECT_GETCASTLE, 0, 0);
			}
		}
	}

	return 0;
}

// �̴ϸʿ��� ĳ���� ������ ��� �´�.	
UI16 OnlineMsgProc::OnResCharInfoMinimap(BYTE *pMsg)
{
	OnResponseMsg *pResponseMsg = (OnResponseMsg*)pMsg;
	
	if(_pWorld->pOnlineMiniMap && _pWorld->pOnlineMiniMap->IsActive())
	{				
		switch(pResponseMsg->dwResponse)
		{
		case ON_RET_ON_FIELD:
			{
				OnResPIInMiniMapOFMsg		*pPIInMiniMapOFMsg;
				pPIInMiniMapOFMsg	=	(OnResPIInMiniMapOFMsg*)pResponseMsg;
				_pWorld->pOnlineMiniMap->MakeCharTarget(ON_RET_ON_FIELD, pPIInMiniMapOFMsg->uiKind,
					pPIInMiniMapOFMsg->uiX, pPIInMiniMapOFMsg->uiY, pPIInMiniMapOFMsg->uiMapID, 0);
			}
			break;
		case ON_RET_IN_BATTLE:
			{
				OnResPIInMiniMapOFMsg		*pPIInMiniMapOFMsg;
				pPIInMiniMapOFMsg	=	(OnResPIInMiniMapOFMsg*)pResponseMsg;
				_pWorld->pOnlineMiniMap->MakeCharTarget(ON_RET_IN_BATTLE, pPIInMiniMapOFMsg->uiKind,
					pPIInMiniMapOFMsg->uiX, pPIInMiniMapOFMsg->uiY, pPIInMiniMapOFMsg->uiMapID, 0);
			}
			break;
		case ON_RET_NOT_CONNECTED:
			{
				_pWorld->pOnlineMiniMap->MakeCharTarget(ON_RET_NOT_CONNECTED, 0, 0, 0, 0, 0);
			}
			break;
		case ON_RET_IN_VILLAGE:
			{
				OnResPIInMiniMapIVMsg		*pPIInMiniMapOFMsg;
				pPIInMiniMapOFMsg	=	(OnResPIInMiniMapIVMsg*)pResponseMsg;
				_pWorld->pOnlineMiniMap->MakeCharTarget(ON_RET_IN_VILLAGE, pPIInMiniMapOFMsg->uiKind,
					0, 0, 0, pPIInMiniMapOFMsg->uiVillageCode);
			}
			break;
		}
	}
	return 0;
}

// ��� �޼���
UI16 OnlineMsgProc::OnGuildMessage(BYTE *pMsg)
{
	OnGuildMsg*	lpOnGuildMsg = (OnGuildMsg*)pMsg;
	SI32*			lpSI32;
	
	switch(lpOnGuildMsg->dwResponse)
	{
	case ON_RET_GLD_DELETEGLD:
		if(_pWorld->pOnlineVillage)
		{
			// ���� ������ ���� ���, �� ������ ���� ���� ���õ� ��� ���� Refresh
			if(!_pWorld->pMyData->GetGuildName())		break;

			if(strncmp((char*)_pWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID, _pWorld->pMyData->GetGuildName(), ON_GUILDNAME_LENGTH) == 0)
			{
				ZeroMemory(_pWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID, ON_GUILDNAME_LENGTH);

				if(_pWorld->pOnlineMap->m_MapInfo.uiVillageCode)
				{
					_pWorld->pOnlineClient->SendRequestVillageInfo(_pWorld->pOnlineMap->m_MapInfo.uiVillageCode);
				}
			}
		}
		
		// My Data ����
		_pWorld->pMyData->SetGuild(NULL, 0, 0);
		
		// ĳ���� Data ����
		_pWorld->pIOnlineChar->SetGuildData(_pWorld->pMyData->GetMyCharUniqueID(), NULL, 0, 0);
		
		_pWorld->pOnlineMsgBoxPay->SetMsg( ON_MSGBOX_TYPE_OK, _pWorld->pOnlineText->Get(ON_TEXT_GUILDTEXT), _pWorld->pOnlineText->Get(ON_TEXT_GUILDTEXT_RELEASEGUILD));
		break;
		
	case ON_RET_GLD_CHANGECLASS:
		lpSI32 = (SI32*)((BYTE*)lpOnGuildMsg + sizeof(OnGuildMsg));
		
		// My Data ����
		_pWorld->pMyData->SetGuild(_pWorld->pMyData->GetGuildName(), *lpSI32, _pWorld->pMyData->GetGuildFlag());
		
		// ĳ���� Data ����
		_pWorld->pIOnlineChar->SetGuildData(_pWorld->pMyData->GetMyCharUniqueID(), _pWorld->pMyData->GetGuildName(), *lpSI32, _pWorld->pMyData->GetGuildFlag());
		
		_pWorld->pOnlineMsgBoxPay->SetMsg( ON_MSGBOX_TYPE_OK, _pWorld->pOnlineText->Get(ON_TEXT_GUILDTEXT), _pWorld->pOnlineText->Get(ON_TEXT_GUILDTEXT_CHANGEGRADE), _pWorld->pOnlineText->Get(_pWorld->m_cltIGuildClass.GetClassName(*lpSI32)));
		break;
		
	case ON_RET_GLD_DISMISSAL:
		// My Data ����
		_pWorld->pMyData->SetGuild(NULL, 0, 0);
		
		// ĳ���� Data ����
		_pWorld->pIOnlineChar->SetGuildData(_pWorld->pMyData->GetMyCharUniqueID(), NULL, 0, 0);
		
		_pWorld->pOnlineMsgBoxPay->SetMsg( ON_MSGBOX_TYPE_OK, _pWorld->pOnlineText->Get(ON_TEXT_GUILDTEXT), _pWorld->pOnlineText->Get(ON_TEXT_GUILDTEXT_DISMISSAL));
		break;
	}

	return 0;
}

// ����Ʈ �޼���
UI16 OnlineMsgProc::OnResQuestSelectNpc(BYTE *pMsg)
{
	OnResQuestSelectNPC*	lpOnResQuestSelectNPC = (OnResQuestSelectNPC*)pMsg;
	SI32					TempCount;
	SI32					siTempItemCount;

	_pWorld->pMyData->SetSelectNPC(FALSE, 0);
	_pWorld->pMyData->SetWaitSelectNPCResponse(FALSE);
	_pWorld->pOnlineCharUI->SetAction(TRUE);
	switch(lpOnResQuestSelectNPC->dwResponse)
	{
		case ON_RET_QUEST_SELECTNPC_ERROR:
		case ON_RET_QUEST_SELECTNPC_STARTBEFOREQUEST:
		case ON_RET_QUEST_SELECTNPC_PLAYINGQUEST:
		case ON_RET_QUEST_SELECTNPC_ENDAFTERQUEST:
		case ON_RET_QUEST_SELECTNPC_NOTSAMENTION:
			_pWorld->pOnlineQuestPlay->SetAction(TRUE, (SI16)lpOnResQuestSelectNPC->m_QuestCode, (SI16)lpOnResQuestSelectNPC->dwResponse);
			break;

		case ON_RET_QUEST_SELECTNPC_STARTQUEST:
			_pWorld->pOnlineQuestPlay->SetAction(TRUE, (SI16)lpOnResQuestSelectNPC->m_QuestCode, (SI16)lpOnResQuestSelectNPC->dwResponse);
			break;

		case ON_RET_QUEST_SELECTNPC_ENDQUEST:
			{
				OnResQuestSelectNPC_EndQuest*		lpOnResQuestSelectNPC_EndQuest = (OnResQuestSelectNPC_EndQuest*)lpOnResQuestSelectNPC;
				SI32								TempYPos;
				char								TempPayBuffer[256];
				CItemHeader*						lpCItemHeader;
				MercenaryItemData*					lpMercenaryItemData;

				PushEffect(EFFECT_GETITEM,     0, 0);
				PushEffect(EFFECT_QUEST_CLEAR, 0, 0);

				_pWorld->pOnlineQuestPlay->SetAction(TRUE, (SI16)lpOnResQuestSelectNPC->m_QuestCode, (SI16)lpOnResQuestSelectNPC->dwResponse);
				_pWorld->m_pQuestInterface->RecvMsg_QuestFinished(lpOnResQuestSelectNPC->m_QuestCode);
				// actdoll (2004/04/06 10:59) : = �̺�Ʈ = ��������Ʈ�� ����� ��� �÷��׸� �� �ش�.
//				if( lpOnResQuestSelectNPC->m_QuestCode != 22100 )	_pWorld->m_pQuestInterface->RecvMsg_AddCompletedQuest(lpOnResQuestSelectNPC->m_QuestCode);
//				else												_pWorld->m_bEventDokdoQuestPlay	= false;

				// ����ޱ�
				TempYPos = 405;
				for(TempCount = 0; TempCount < ON_QUSETDATA_MAXQUESTPAY; TempCount++)
				{
					switch(lpOnResQuestSelectNPC_EndQuest->m_QuestPay[TempCount].m_QuestPayType)
					{
						case QUESTPAYTYPE_MONEY:
							_pWorld->pMyData->SetMoney(_pWorld->pMyData->GetMoney() + lpOnResQuestSelectNPC_EndQuest->m_QuestPay[TempCount].m_QuestPayData_Money.m_AddMoney);

							sprintf((char*)TempPayBuffer, _pWorld->pOnlineText->GetQuestText(ON_QUESTTEXT_PAYMONEY), lpOnResQuestSelectNPC_EndQuest->m_QuestPay[TempCount].m_QuestPayData_Money.m_AddMoney);
							pGame->pItemInfoWindow->AddETCInfoWindow(150, 494, TempYPos, ON_GAMESPR_ITEM_MONEY, (char*)TempPayBuffer);
							TempYPos -= 95;
							break;

						case QUESTPAYTYPE_CREDIT:
							_pWorld->pMyData->SetTradeCredit(_pWorld->pMyData->GetTradeCredit() + lpOnResQuestSelectNPC_EndQuest->m_QuestPay[TempCount].m_QuestPayData_Credit.m_AddCredit, 0);

							sprintf((char*)TempPayBuffer, _pWorld->pOnlineText->GetQuestText(ON_QUESTTEXT_PAYCREDIT), lpOnResQuestSelectNPC_EndQuest->m_QuestPay[TempCount].m_QuestPayData_Credit.m_AddCredit);
							pGame->pItemInfoWindow->AddETCInfoWindow(150, 494, TempYPos, ON_GAMESPR_ITEM_CREDIT, (char*)TempPayBuffer);
							TempYPos -= 95;
							break;

						case QUESTPAYTYPE_EXP:
							OnlineFollower*		lpOnlineFollower;
							
							lpOnlineFollower = _pWorld->pMyData->GetFollowersInterface()->GetFollower(0);
							lpOnlineFollower->ParametaBox.IP.IncreaseExp(lpOnResQuestSelectNPC_EndQuest->m_QuestPay[TempCount].m_QuestPayData_EXP.m_AddHeroEXP);

							sprintf((char*)TempPayBuffer, _pWorld->pOnlineText->GetQuestText(ON_QUESTTEXT_PAYEXP), lpOnResQuestSelectNPC_EndQuest->m_QuestPay[TempCount].m_QuestPayData_EXP.m_AddHeroEXP);
							pGame->pItemInfoWindow->AddETCInfoWindow(150, 494, TempYPos, ON_GAMESPR_ITEM_EXP, (char*)TempPayBuffer);
							TempYPos -= 95;
							break;

						case QUESTPAYTYPE_ITEM:
							// actdoll (2004/03/31 15:06) : ���� ���������� �߻��ϴ�, ���װ� �ƴ� �۵� ���� ����.
							//	AddItem���� OnlineFollowŬ������ AddItem�� �Ҵ�� �ڽſ��� �Ȱ��� �������� ���� ��� ī��Ʈ�� ���� ������ �ִ� ������ �ٲ�� ������ �ִ�.
							//	����ϴ� ������ ���� ���Ҹ� �ٲ� �� �����Ƿ� ����� ����. ���߿� ������ ����
							siTempItemCount		= lpOnResQuestSelectNPC_EndQuest->m_QuestPay[TempCount].m_QuestPayData_Item.m_AddItemData.uiQuantity;
							_pWorld->pMyData->AddItem(lpOnResQuestSelectNPC_EndQuest->m_QuestPay[TempCount].m_QuestPayData_Item.m_FollowerSlot, &lpOnResQuestSelectNPC_EndQuest->m_QuestPay[TempCount].m_QuestPayData_Item.m_AddItemData);
							_pWorld->pOnlineInventory->FillItemListBox();
							lpCItemHeader = _pWorld->pItemsInfo->GetItem(lpOnResQuestSelectNPC_EndQuest->m_QuestPay[TempCount].m_QuestPayData_Item.m_AddItemData.uiID);

							sprintf((char*)TempPayBuffer, _pWorld->pOnlineText->GetQuestText(ON_QUESTTEXT_PAYITEM), _pWorld->pOnlineText->Get(lpCItemHeader->m_siItemName), siTempItemCount );
							pGame->pItemInfoWindow->AddItemInfoWindow(150, 494, TempYPos, lpOnResQuestSelectNPC_EndQuest->m_QuestPay[TempCount].m_QuestPayData_Item.m_AddItemData.uiID, (char*)TempPayBuffer);
							TempYPos -= 95;
							break;

						// robypark 2004/8/30 20:00
						// ���ǽ� �̺�Ʈ ��ǰ ��÷. ���� �� �������� �ƴ� ���Ǽ��� ������ ����(���� �߼۵Ǵ� ��ǰ) ó��
						case QUESTPAYTYPE_EVENT_ITEM:
							// robypark 2004/9/17 15:5
							// ��÷ �޽����� ����Ʈ�ļ����� ���� ���� �̿��Ͽ� ��µǵ��� ����
							SQuestHeader* lpSQuestHeader = _pWorld->pQuestParser->GetQuest((SI16)lpOnResQuestSelectNPC->m_QuestCode);
							
							if (NULL == lpSQuestHeader)
							{
								_pWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", \
																"%s", \
																lpOnResQuestSelectNPC_EndQuest->m_QuestPay[TempCount].m_QuestPayData_Event.szEventName);
							}
							else
							{
								_pWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", \
																_pWorld->pOnlineText->Get(lpSQuestHeader->siJackpotTextID), \
																lpOnResQuestSelectNPC_EndQuest->m_QuestPay[TempCount].m_QuestPayData_Event.szEventName);
							}
							break;
					}
				}

				// �� �����ϱ�
				if(lpOnResQuestSelectNPC_EndQuest->m_DeleteMoney)
				{
					_pWorld->pMyData->SetMoney(_pWorld->pMyData->GetMoney() - lpOnResQuestSelectNPC_EndQuest->m_DeleteMoney);
				}

				// Item �����ϱ�
				if(lpOnResQuestSelectNPC_EndQuest->m_DeleteItemCount)
				{
					lpMercenaryItemData = (MercenaryItemData*)((BYTE*)lpOnResQuestSelectNPC_EndQuest + sizeof(OnResQuestSelectNPC_EndQuest));
					for(TempCount = 0; TempCount < lpOnResQuestSelectNPC_EndQuest->m_DeleteItemCount; TempCount++)
					{
						switch(lpMercenaryItemData[TempCount].siFlag)
						{
							case 0:			// �κ��丮 ������
								_pWorld->pMyData->DelItem(lpMercenaryItemData[TempCount].siSlot, &lpMercenaryItemData[TempCount].siInventoryItemInfo);
								break;

							case 1:			// ���� ������
								// ����� ����
								break;
						}
					}

					_pWorld->pOnlineInventory->FillItemListBox();
				}
			}
			break;

		default:						// Error
			clGrp.Error("Quest", "Select NPC Error Code : %d", lpOnResQuestSelectNPC->dwResponse);
			break;
	}

	return 0;
}

// ����Ʈ �޼��� ���� ����	
UI16 OnlineMsgProc::OnResQuestAccept(BYTE *pMsg)
{

	OnResQuestAcceptQuest*	lpOnResQuestAcceptQuest = (OnResQuestAcceptQuest*)pMsg;
	
	// actdoll (2004/04/06 10:14) : = �̺�Ʈ = ��������Ʈ�� ������ ��� �±ر⸦ ����ش�.
//	if( lpOnResQuestAcceptQuest->m_QuestCode == 22100 )
//	{
//		_pWorld->m_bEventDokdoQuestPlay	= true;
//
//		SetRect( &_pWorld->m_rtIconDokdoQuest, 700, 465, 700 + _pWorld->m_Spr_Icon_DokdoQuest.Header.Xsize, 465 + _pWorld->m_Spr_Icon_DokdoQuest.Header.Ysize);
//		_pWorld->m_dwDelay				= timeGetTime();
//		_pWorld->m_bReceivdeItemShow	= TRUE;
//	}

	if(lpOnResQuestAcceptQuest->dwResponse == ON_RET_OK)
	{
		_pWorld->m_pQuestInterface->RecvMsg_QuestStarted(lpOnResQuestAcceptQuest->m_QuestCode);
		PushEffect(EFFECT_QUEST_ASIGN, 0, 0);
	}

	return 0;
}

// ����Ʈ ���	
UI16 OnlineMsgProc::OnResQuestCamcel(BYTE *pMsg)	
{
	OnResQuestCancelQuest*	lpOnResQuestCancelQuest = (OnResQuestCancelQuest*)pMsg;

	// actdoll (2004/04/06 10:14) : = �̺�Ʈ = ��������Ʈ�� ����� ��� �±ر⸦ �����Ѵ�
//	if( lpOnResQuestCancelQuest->m_QuestCode == 22100 )
//	{
//		_pWorld->m_bEventDokdoQuestPlay	= false;
//	}

	if(lpOnResQuestCancelQuest->dwResponse == ON_RET_OK)
	{
		_pWorld->m_pQuestInterface->RecvMsg_QuestFinished(lpOnResQuestCancelQuest->m_QuestCode);
	}

	return 0;
}

// �������� ����Ʈ
UI16 OnlineMsgProc::OnResQuestGetPlay(BYTE *pMsg)
{
	OnResQuestGetPlayQuest*	lpOnResQuestGetPlayQuest = (OnResQuestGetPlayQuest*)pMsg;
	BYTE*					lpBuffer1;
	BYTE*					lpBuffer2;

	lpBuffer1 = (BYTE*)((BYTE*)lpOnResQuestGetPlayQuest + sizeof(OnResQuestGetPlayQuest));
	lpBuffer2 = lpBuffer1 + (sizeof(SPlayingQuest) * lpOnResQuestGetPlayQuest->m_PlayingQuestCodeCount);
	_pWorld->m_pQuestInterface->RecvMsg_SetQuestData(lpOnResQuestGetPlayQuest->m_PlayingQuestCodeCount, lpOnResQuestGetPlayQuest->m_CompletedQuestListCount, (SPlayingQuest*)lpBuffer1, (SI16*)lpBuffer2);

	// actdoll (2004/04/06 10:14) : = �̺�Ʈ = ���� ����Ʈ�� �˻��ؼ� �߰ߵ� ��� ���ֵ��� �Ѵ�.
//	if( !lpBuffer1 )	return 0;
//	SPlayingQuest*	pPlayingQuest	= (SPlayingQuest*)lpBuffer1;
//
//	for( SI32 i=0; i<lpOnResQuestGetPlayQuest->m_PlayingQuestCodeCount; i++)
//	{
//		if( pPlayingQuest->m_QuestCode == 22100 )
//		{
//			_pWorld->m_bEventDokdoQuestPlay	= true;
//			
//			SetRect( &_pWorld->m_rtIconDokdoQuest, 700, 465, 700 + _pWorld->m_Spr_Icon_DokdoQuest.Header.Xsize, 465 + _pWorld->m_Spr_Icon_DokdoQuest.Header.Ysize);
//			_pWorld->m_dwDelay				= timeGetTime();
//			_pWorld->m_bReceivdeItemShow	= TRUE;
//			break;
//		}
//		pPlayingQuest++;
//	}

	return 0;
}

// �������� ����Ʈ ����Ÿ
UI16 OnlineMsgProc::OnResQuestGetPlayData(BYTE *pMsg)
{
	OnResQuestGetPlayQuestData*	lpOnResQuestGetPlayQuestData = (OnResQuestGetPlayQuestData*)pMsg;

	_pWorld->m_pQuestInterface->RecvMsg_SetPlayingQuestData(&lpOnResQuestGetPlayQuestData->m_PlayingQuestData);

	return 0;
}

// ����Ʈ �ʱ�ȭ
UI16 OnlineMsgProc::OnResQuestClear(BYTE *pMsg)
{
	_pWorld->m_pQuestInterface->RecvMsg_ClearAllQuest();
	return 0;
}


// ���ǿ��� �������� ����
UI16 OnlineMsgProc::OnChangeBoothItem(BYTE *pMsg)
{
	// ������ ���������� �����ߴ��� �����ؼ� ��ǳ�� �ٲٱ�
	SI16	i;
	OnChangeBoothItemMsg *pOnChangeBoothItemMsg = (OnChangeBoothItemMsg *)pMsg;
	
	if(pOnChangeBoothItemMsg != NULL)
	{
		// ���� �κ��丮���� ����� ������ ����(���� �ŷ����� �����̶��)
		if(_pWorld->pOnlineBooth->GetAccount() == pOnChangeBoothItemMsg->uiAccount)
		{
			// ������ ����
			if(pOnChangeBoothItemMsg->BaseBoothItem.uiProductID == 0 && _pWorld->pOnlineBooth->GetStatusOut() != ON_BENCH_OUT_BUY_FOLLOWER_OK)
				_pWorld->pOnlineBooth->GetList()->DeleteSlot(pOnChangeBoothItemMsg->BaseBoothItem.uiBoothSlot, 0);
			else
			{
				_pWorld->pOnlineClient->SendGetBoothItemInfo((UI16)pOnChangeBoothItemMsg->uiAccount);
/*
				// ������ �߰�
				ItemInfo TempItemInfo;
				TempItemInfo.uiItemID		= pOnChangeBoothItemMsg->BaseBoothItem.uiItemID;
				TempItemInfo.ItemCount		= 0;
				TempItemInfo.uiBuyPrice		= 0;
				_pWorld->pOnlineBooth->GetList()->AddItem(TempItemInfo, pOnChangeBoothItemMsg->BaseBoothItem.uiBoothSlot);
				ZeroMemory(_pWorld->pOnlineBooth->m_szBuyItemDesc[pOnChangeBoothItemMsg->BaseBoothItem.uiBoothSlot], sizeof(_pWorld->pOnlineBooth->m_szBuyItemDesc[pOnChangeBoothItemMsg->BaseBoothItem.uiBoothSlot]));
				sprintf(_pWorld->pOnlineBooth->m_szBuyItemDesc[pOnChangeBoothItemMsg->BaseBoothItem.uiBoothSlot], "");
*/
			}
		}
		
		SI16 TempSelect, siHeroX, siHeroY;
		_pWorld->pIOnlineChar->GetPos(pOnChangeBoothItemMsg->uiAccount, &siHeroX, &siHeroY);
		MapStoreHeader *pStoreHeader = _pWorld->pOnlineMap->GetMapStoreParser( siHeroX, siHeroY, &TempSelect );
		if(pStoreHeader)
		{
			// �����̶�� ���ǿ� ������ �̹��� �÷�����
			SMapBoothData* pTempSMapBoothData = _pWorld->pOnlineMap->m_MapBoothInfo.GetBoothData(pStoreHeader->siMapStoreCode);
			if(pTempSMapBoothData && pOnChangeBoothItemMsg->BaseBoothItem.siType == LIST_ITEM)
			{
				pTempSMapBoothData->uiBoothID	= pStoreHeader->siMapStoreCode;
				pTempSMapBoothData->siTileX		= pStoreHeader->siMapStoreMapXPosition;
				pTempSMapBoothData->siTileY		= pStoreHeader->siMapStoreMapYPosition;
				pTempSMapBoothData->siMapIndex	= pStoreHeader->siMapStoreMapIndex;
				pTempSMapBoothData->uiItemID[pOnChangeBoothItemMsg->BaseBoothItem.uiBoothSlot]	= pOnChangeBoothItemMsg->BaseBoothItem.uiProductID;
				pTempSMapBoothData->sifData		= TRUE;
			}
		}
		//else
		{
			// ��ǳ�� ����
			ZeroMemory(_pWorld->pOnlineBooth->m_szBoothName, sizeof(_pWorld->pOnlineBooth->m_szBoothName));
			CHAR buf[256];
			ZeroMemory(buf, sizeof(buf));
			SI08 siFirst = 0;
			
			for(i = 0; i < ON_MAX_BOOTH_ITEM; i++)
			{
				SellItemInfo	*pTempSellItemInfo, TempSellItemInfo;
				CItemHeader		*pItem;
				
				if(i == pOnChangeBoothItemMsg->BaseBoothItem.uiBoothSlot)	// ���� ����� ������
				{
					// pIOnlineChar�� ���������� �����Ѵ�.
					ZeroMemory(&TempSellItemInfo, sizeof(SellItemInfo));
					if(pOnChangeBoothItemMsg->BaseBoothItem.siType == LIST_ITEM)
						TempSellItemInfo.Item.uiItemID = pOnChangeBoothItemMsg->BaseBoothItem.uiProductID;
					else if(pOnChangeBoothItemMsg->BaseBoothItem.siType == LIST_FOLLOWER)
						TempSellItemInfo.Follower.uiKind = pOnChangeBoothItemMsg->BaseBoothItem.uiProductID;
					TempSellItemInfo.siType = pOnChangeBoothItemMsg->BaseBoothItem.siType;
					_pWorld->pIOnlineChar->SetBoothItem((UI16)pOnChangeBoothItemMsg->uiAccount, (UI08)i, TempSellItemInfo);
					
					if(pOnChangeBoothItemMsg->BaseBoothItem.siType == LIST_ITEM)
					{
						pItem = _pWorld->pItemsInfo->GetItem(pOnChangeBoothItemMsg->BaseBoothItem.uiProductID);
						if(!pItem)		continue;
						
						if(!siFirst)	sprintf(buf, "%s", _pWorld->pOnlineText->Get(pItem->m_siItemName));
						else			sprintf(buf, "	%s", _pWorld->pOnlineText->Get(pItem->m_siItemName));
						strcat(_pWorld->pOnlineBooth->m_szBoothName, buf);
						siFirst++;
					}
					else if(pOnChangeBoothItemMsg->BaseBoothItem.siType == LIST_FOLLOWER)
					{
						UI32 TextID = _pWorld->pIOnlineCharKI->GetName(_pWorld->pIOnlineCharKI->GetIndex(pOnChangeBoothItemMsg->BaseBoothItem.uiProductID));
						if(TextID <= 0)	continue;

						if(!siFirst)	sprintf(buf, "%s", _pWorld->pOnlineText->Get(TextID));
						else			sprintf(buf, "	%s", _pWorld->pOnlineText->Get(TextID));
						strcat(_pWorld->pOnlineBooth->m_szBoothName, buf);
						siFirst++;
					}
				}
				else		// ������ �÷��� �����۵�
				{
					// pIOnlineChar�� ���������� ���´�.
					pTempSellItemInfo = _pWorld->pIOnlineChar->GetBoothItem((UI16)pOnChangeBoothItemMsg->uiAccount, (UI08)i);
					if(!pTempSellItemInfo)		continue;
					
					if(pTempSellItemInfo->siType == LIST_ITEM)	
					{
						pItem = _pWorld->pItemsInfo->GetItem(pTempSellItemInfo->Item.uiItemID);
						if(!pItem)		continue;

						if(!siFirst)	sprintf(buf, "%s", _pWorld->pOnlineText->Get(pItem->m_siItemName));
						else			sprintf(buf, "	%s", _pWorld->pOnlineText->Get(pItem->m_siItemName));
						strcat(_pWorld->pOnlineBooth->m_szBoothName, buf);
						siFirst++;
					}
					else if(pTempSellItemInfo->siType == LIST_FOLLOWER)
					{
						UI32 TextID = _pWorld->pIOnlineCharKI->GetName(_pWorld->pIOnlineCharKI->GetIndex(pTempSellItemInfo->Follower.uiKind));
						if(TextID <= 0)	continue;

						if(!siFirst)	sprintf(buf, "%s", _pWorld->pOnlineText->Get(TextID));
						else			sprintf(buf, "	%s", _pWorld->pOnlineText->Get(TextID));
						strcat(_pWorld->pOnlineBooth->m_szBoothName, buf);
						siFirst++;
					}
				}
			}
			_pWorld->pIOnlineChar->SetSpeech(pOnChangeBoothItemMsg->uiAccount, _pWorld->pOnlineBooth->m_szBoothName, TRUE);
		}
	}

	return 0;
}

// ���ǿ��� ������ ������ ��´�.
UI16 OnlineMsgProc::OnResChangeBoothItem(BYTE *pMsg)
{
	// �����κ��� ���� ������ �޴´�.
	OnResPMItemInfoMsg *pOnResPMItemInfoMsg = (OnResPMItemInfoMsg*)pMsg;
	if(pOnResPMItemInfoMsg == NULL)	return 0;
	
	if(pOnResPMItemInfoMsg->dwResponse == ON_RET_OK)
	{
		// �������� ������ �������� ������ ��´�. (ItemID, Quantity, Price, Desc) 5��
		_pWorld->pOnlineBooth->GetList()->Clear();
		
		OnBoothItemInfo *pOnBoothItem = new OnBoothItemInfo[pOnResPMItemInfoMsg->uiItemNum];
		ZeroMemory( pOnBoothItem, sizeof( OnBoothItemInfo ) * (pOnResPMItemInfoMsg->uiItemNum) );

		BYTE	*lpStart, *lpItemInfoStart;
		lpStart	 = (BYTE*)((BYTE*)pOnResPMItemInfoMsg + sizeof(OnResPMItemInfoMsg));
		lpItemInfoStart = (BYTE*)((BYTE*)lpStart + sizeof(OnBoothItemInfo) * pOnResPMItemInfoMsg->uiItemNum);
		CopyMemory(pOnBoothItem, lpStart, sizeof(OnBoothItemInfo) * pOnResPMItemInfoMsg->uiItemNum);

		for(SI32 i = 0; i < pOnResPMItemInfoMsg->uiItemNum; i++)
		{
			ItemInfo TempItemInfo;
			TempItemInfo.siType			= pOnBoothItem[i].siType;
			TempItemInfo.uiItemID		= pOnBoothItem[i].Item.uiItemID;
			TempItemInfo.ItemCount		= pOnBoothItem[i].Item.uiItemCount;
			TempItemInfo.uiBuyPrice		= pOnBoothItem[i].siPrice;
			CopyMemory(&TempItemInfo.Follower, &pOnBoothItem[i].Follower, sizeof(OnFollowerInfo));
			_pWorld->pOnlineBooth->GetList()->AddItem(TempItemInfo, pOnBoothItem[i].uiBoothSlot);
			
			// m_pBoothList�� ������ ���� ������ �����Ƿ� ������ ���� �����صд�.
			ZeroMemory(_pWorld->pOnlineBooth->m_szBuyItemDesc[pOnBoothItem[i].uiBoothSlot], sizeof(_pWorld->pOnlineBooth->m_szBuyItemDesc[pOnBoothItem[i].uiBoothSlot]));
			strncpy(_pWorld->pOnlineBooth->m_szBuyItemDesc[pOnBoothItem[i].uiBoothSlot], pOnBoothItem[i].Note, ON_MAX_BOOTH_NOTE);

			// �뺴�� �κ������� �����Ѵ�.
			ZeroMemory(&_pWorld->pOnlineBooth->m_FollowerItemData, sizeof(MyItemData)*ON_MAX_FOLLOWER_NUM*ON_MYDATA_MAX_ITEM_NUM);
			if(pOnBoothItem[i].siType == LIST_FOLLOWER)
			{
				MyItemData *pItemInfo = (MyItemData *)lpItemInfoStart;
				for(SI16 j=0; j < pOnBoothItem[i].Follower.uiItemNum; j++)
					memcpy(&_pWorld->pOnlineBooth->m_FollowerItemData[pOnBoothItem[i].Follower.uiSlot][j], &pItemInfo[j], sizeof(MyItemData));
				lpItemInfoStart = (BYTE*)((BYTE*)lpItemInfoStart + sizeof(MyItemData) * pOnBoothItem[i].Follower.uiItemNum);
			}
		}
		delete [] pOnBoothItem;
		// ���ʵ� ����
		if(_pWorld->pOnlineBooth->m_siNoField == -1)
			_pWorld->pOnlineBooth->m_siNoField	= _pWorld->pOnlineFieldArea->SetNoFieldArea(0,0,799,599);
		
		_pWorld->pOnlineBooth->SetStatusOut(ON_BENCH_OUT_INVENTORY_POLL);
	}
	else
	{
		_pWorld->bCharMove = TRUE;
		_pWorld->pOnlineBooth->SetStatusOut(ON_BENCH_OUT_INIT);
	}

	return 0;
}

// ĳ������ �� ����	
UI16 OnlineMsgProc::OnResCharDetailInfo(BYTE *pMsg)
{
	OnResCharDetailInfoMsg*		lpOnResCharDetailInfoMsg = (OnResCharDetailInfoMsg*)pMsg;
	OnCharPlayerDetailInfo*		lpOnCharPlayerDetailInfo;

	if(lpOnResCharDetailInfoMsg == NULL) return 0;

	if(ISPLAYER(lpOnResCharDetailInfoMsg->uiAccount))
	{
		lpOnCharPlayerDetailInfo = (OnCharPlayerDetailInfo*)((BYTE*)lpOnResCharDetailInfoMsg + sizeof(OnResCharDetailInfoMsg));

		_pWorld->pIOnlineChar->SetPlayerInfo(lpOnResCharDetailInfoMsg->uiAccount,  lpOnCharPlayerDetailInfo);
	}				

	return 0;
}

// �ʿ� �ִ� �������� ����
UI16 OnlineMsgProc::OnChangeFieldItem(BYTE *pMsg)
{
	OnChangeFieldItemMsg *pFieldMsg = (OnChangeFieldItemMsg *)pMsg;

	if ( pFieldMsg->uiItemID != 0 && pFieldMsg->uiItemQuantity != 0) 
		_pWorld->pOnlineMap->SetID(pFieldMsg->siX, pFieldMsg->siY, 1, 1, 0, 0, pFieldMsg->uiItemID, pFieldMsg->uiItemQuantity);
	else
		_pWorld->pOnlineMap->DelID(pFieldMsg->siX, pFieldMsg->siY, 1, 1,ON_MAP_DELID_ITEM);

	return 0;
}

//
UI16 OnlineMsgProc::OnResProbeArea(BYTE *pMsg)
{
	BURIED::sResponseProbeArea* lpsResponseProbeArea = (BURIED::sResponseProbeArea*)pMsg;

	if(lpsResponseProbeArea->foundItem)
	{
		_pWorld->m_pMercenary->Set_SelectedSlot( 0 );		//.actdoll (021224)
		_pWorld->pOnlineInventory->SetMercenarySlot(0);
		_pWorld->pOnlineInventory->FillItemListBox();
		SI16 siSlot = _pWorld->pOnlineInventory->GetLCInventory()->GetEmptySlot(lpsResponseProbeArea->foundItem);
		if(siSlot != -1)
		{
			// �̵�����
			SI16 siHeroX, siHeroY;
			_pWorld->pIOnlineChar->GetPos(_pWorld->pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);
			_pWorld->pOnlineCharUI->SetMove(siHeroX, siHeroY);

			// ���� �κ��� �ʾ� �ش�.
			ItemInfo TempItemInfo;
			_pWorld->pOnlineInventory->GetLCInventory()->CopyData(siSlot, TempItemInfo);
			TempItemInfo.ItemCount		+= 1;					
			_pWorld->pOnlineInventory->GetLCInventory()->AddItem(TempItemInfo, siSlot);

			// ���� ����Ÿ�� �ʾ� �ش�.
			MyItemData MyItemData;	
			MyItemData.siPosInInventory	=	siSlot;
			MyItemData.uiID				=	(UI16)lpsResponseProbeArea->foundItem;
			MyItemData.uiQuantity		=	1;				
			_pWorld->pMyData->AddItem(0, &MyItemData);
			_pWorld->pOnlineInventory->FillItemListBox();

			// ������ ȹ�� ǥ�����ֱ�
			CHAR	buffer[512];
			CItemHeader *lpCItemHeader = _pWorld->pItemsInfo->GetItem(lpsResponseProbeArea->foundItem);
			sprintf(buffer, _pWorld->pOnlineText->Get(ON_TEXT_AQUIRE_BURIED_ITEM), _pWorld->pOnlineText->Get(lpCItemHeader->m_siItemName));
			pGame->pItemInfoWindow->AddItemInfoWindow(60, 250, 10, lpsResponseProbeArea->foundItem, buffer);

			// ȿ���� ���
			PushEffect(EFFECT_GETITEM,     0, 0);
			PushEffect(EFFECT_QUEST_CLEAR, 0, 0);
		}
	}

	return 0;
}

// ����ο��� ��� �˸�	
UI16 OnlineMsgProc::NotifyResult(BYTE *pMsg)
{
	AUCTION::sNotifyResult *lpOnNotifyMsg = (AUCTION::sNotifyResult *)pMsg;

	if (lpOnNotifyMsg)
	{
		char str[256];
		if (lpOnNotifyMsg->biddingPrice)
		{
			sprintf(str, _pWorld->pOnlineText->Get(ON_TEXT_AUCTION_SUCCESS),lpOnNotifyMsg->biddingPrice);
			_pWorld->pMyData->ModifySaving(lpOnNotifyMsg->biddingPrice);
			
		}
		else
		{
			sprintf(str, _pWorld->pOnlineText->Get(ON_TEXT_AUCTION_FAIL),lpOnNotifyMsg->biddingPrice);
			
		}
		
		
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK,"",str);

	}

	return 0;
}


// ��� ����	
UI16 OnlineMsgProc::NotifyAdvertise(BYTE *pMsg)
{
	AUCTION::sNotifyAdvertise *lpNotifyAdvertise = (AUCTION::sNotifyAdvertise *)pMsg;

	if (lpNotifyAdvertise) {
		char szStr[256];
		ZeroMemory(szStr,256);

		switch (lpNotifyAdvertise->articleKind)
		{
			
			// �������� ������̴�.
		case AUCTION::ARTICLE_ITEM:
			{
				
				AUCTION::sArticleItemInfo *pItemInfo = (AUCTION::sArticleItemInfo *)(lpNotifyAdvertise->articleData);
				CItemHeader	*pItem;
				pItem	= _pWorld->pItemsInfo->GetItem( pItemInfo->itemID );																									
				
				if (pItem) {									
					// ǰ�� : %s  ���� : %ld
					sprintf(szStr, _pWorld->pOnlineText->Get(ON_TEXT_AUCTION_PROGRESS), _pWorld->pOnlineText->Get( pItem->m_siItemName ),pItemInfo->quantity);
					
				}
				
				
			}
			break;
		case AUCTION::ARTICLE_FOLLOWER:
			{
				
				sprintf(szStr, _pWorld->pOnlineText->Get(ON_TEXT_AUCTION_FOLLOWER_PROGRESS));
				
			}
			break;
		case AUCTION::ARTICLE_PRODUCTION:
			{
				sprintf(szStr, _pWorld->pOnlineText->Get(ON_TEXT_AUCTION_FOLLOWER_PROGRESS));
				
			}
			break;
		}
		
		_pWorld->pOnlineNoticeList->AddNoticeData((char*)szStr, RGB(125, 167, 217));
	}

	return 0;
}

// 
UI16 OnlineMsgProc::OnResPersonal(BYTE *pMsg)
{
	OnResPersonalInfo*	lpOnResPersonalInfo = (OnResPersonalInfo*)pMsg;

	if(lpOnResPersonalInfo->dwResponse  == ON_RET_OK)
	{
		_pWorld->pOnlineMessenger->SetGuildInfo(lpOnResPersonalInfo->szID, lpOnResPersonalInfo->szGuildName, lpOnResPersonalInfo->siClassInGuildID);
	}

	return 0;
}


// ���� �ٴϴ� �뺴 ��ü
UI16 OnlineMsgProc::OnVmercenaryChange(BYTE *pMsg)
{
	OnResponse_VMercenary_Change*	lpOnResponse_VMercenary_Change = (OnResponse_VMercenary_Change*)pMsg;

	_pWorld->pIOnlineChar->SetMercenary(lpOnResponse_VMercenary_Change->uiUniqueID, lpOnResponse_VMercenary_Change->siKind);
	if(_pWorld->pMyData->GetMyCharUniqueID() == lpOnResponse_VMercenary_Change->uiUniqueID)
	{
		if(lpOnResponse_VMercenary_Change->siKind != 0)
		{
			PlayShowPetEffect(lpOnResponse_VMercenary_Change->siKind);
		}
	}

	return 0;
}

// ���� �ٴϴ� �뺴 ��ü ����	
UI16 OnlineMsgProc::OnVmercenaryChangeMe(BYTE *pMsg)
{
	OnResponse_VMercenary_ChangeMain*	lpOnResponse_VMercenary_ChangeMain = (OnResponse_VMercenary_ChangeMain*)pMsg;

	_pWorld->pMyData->SetMercenary(lpOnResponse_VMercenary_ChangeMain->siSlot);

	return 0;
}

// robypark 2005/1/5 17:12
// ������ ������ ���¿��� �Ϲ� �������� ���ư��ڴٰ��� ��û�� ���� ����
// ON_RESPONSE_NORMAL_CHAR
UI16 OnlineMsgProc::OnResponseNormalCharacter(BYTE *pMsg)
{
	OnResNormalChar* lpOnResNormalChar = (OnResNormalChar *)pMsg;

	DWORD		IP;
	CHAR		TempBuffer[128];
	
	// ĳ�� ���� ���
	UI16 uiUniqueID, uiKind, uiEquipWeapon[WEAR_ITEM_NUM];
	SI16 siX, siY;
	SI32 siFightPower, siGuildInGrade, siGuildFlag, siMercenaryKind;
	CHAR TempBuffer1[128];
	ZeroMemory(TempBuffer1, sizeof(TempBuffer1));

	uiUniqueID		= lpOnResNormalChar->uiUniqueID;
	uiKind			= lpOnResNormalChar->uiCharCode;
	IP				= _pWorld->pIOnlineChar->GetID(uiUniqueID);
	siFightPower	= _pWorld->pIOnlineChar->GetFightPower(uiUniqueID);
	siGuildInGrade	= _pWorld->pIOnlineChar->GetGuildInGrade(uiUniqueID);
	siGuildFlag		= _pWorld->pIOnlineChar->GetGuildFlag(uiUniqueID);
	siMercenaryKind = _pWorld->pIOnlineChar->GetMercenary(uiUniqueID);

	CopyMemory(TempBuffer, _pWorld->pMyData->GetMyCharName(), ON_ID_LENGTH);
	if(_pWorld->pIOnlineChar->GetGuildName(uiUniqueID))
		CopyMemory(TempBuffer1, _pWorld->pIOnlineChar->GetGuildName(uiUniqueID), ON_ID_LENGTH);

	_pWorld->pIOnlineChar->GetPos(uiUniqueID, &siX, &siY);

	if(_pWorld->pMyData->GetMyCharUniqueID() == uiUniqueID)
	{
		_pWorld->pMyData->GetFollowersInterface()->GetWearItems(0, uiEquipWeapon);
		_pWorld->pMyData->SetMyCharKind(uiKind);

		// ������ ������ �ƴϴ�.
		_pWorld->SetIsShowInformationAboutSiegeWarfareUnit(0);

		// robypark 2005/1/28 14:59
		// ������ ���� �������̽� �ݱ�
		_pWorld->pOnlinePannel->SiegeWarfareUnitUI_DestroyWindow();

		// ���� �Ұ��� �ʱ�ȭ
		_pWorld->pMyData->SetTransformSiegeWarfareUnit(false);
	}
	else
	{
		_pWorld->pIOnlineChar->GetEquip(uiUniqueID, uiEquipWeapon);
	}
	
	_pWorld->pIOnlineChar->Free(uiUniqueID);
	_pWorld->pIOnlineChar->Init(uiUniqueID, (CHAR*)TempBuffer, uiKind, siX, siY, IP, 0, siFightPower, _pWorld,(UI16*)uiEquipWeapon);
	_pWorld->pIOnlineChar->SetEquipItem(uiUniqueID, (UI16*)uiEquipWeapon);
	

	_pWorld->pIOnlineChar->SetGuildData(uiUniqueID, (CHAR*)TempBuffer1, siGuildInGrade, siGuildFlag);
	_pWorld->pIOnlineChar->SetMercenary(uiUniqueID, siMercenaryKind);

	_pWorld->pIOnlineChar->SetChangeEffect(uiUniqueID);

	_pWorld->pOnlineInventory->SetMercenarySlot(0);

	_pWorld->pOnlineCharUI->SetAction(TRUE);

	return 0;
}

// �ֺ��� ������� �ڵ尡 ����	
UI16 OnlineMsgProc::OnChangeCharCode(BYTE *pMsg)
{
	OnChangeCharacterCodeMsg* lpOnChangeCharacterCodeMsg = (OnChangeCharacterCodeMsg*)pMsg;

	DWORD		IP;
	CHAR		TempBuffer[128];
	
	// ĳ�� ���� ���
	UI16 uiUniqueID, uiKind, uiEquipWeapon[WEAR_ITEM_NUM];
	SI16 siX, siY;
	SI32 siFightPower, siGuildInGrade, siGuildFlag, siMercenaryKind;
	CHAR TempBuffer1[128];
	ZeroMemory(TempBuffer1, sizeof(TempBuffer1));

	uiUniqueID		= lpOnChangeCharacterCodeMsg->uiCharacID;
	uiKind			= lpOnChangeCharacterCodeMsg->uiNewCharCode;
	IP				= _pWorld->pIOnlineChar->GetID(uiUniqueID);
	siFightPower	= _pWorld->pIOnlineChar->GetFightPower(uiUniqueID);
	siGuildInGrade	= _pWorld->pIOnlineChar->GetGuildInGrade(uiUniqueID);
	siGuildFlag		= _pWorld->pIOnlineChar->GetGuildFlag(uiUniqueID);
	siMercenaryKind = _pWorld->pIOnlineChar->GetMercenary(uiUniqueID);

	// ������ ������ ���¿��� �׾� �Ϲ� ������ �Ǵ� ���
	if (TRUE == (_pWorld->pIOnlineChar->IsSiegeWarfareUnit(uiUniqueID)))	// ������ �����̾���.
	{
		PushEffect(EFFECT_UNITDEAD, 0 , 0);	// ȿ���� ���

		// ���� �Ұ��� ����
		_pWorld->pMyData->SetTransformSiegeWarfareUnit(false);
	}

	CopyMemory(TempBuffer, _pWorld->pMyData->GetMyCharName(), ON_ID_LENGTH);
	if(_pWorld->pIOnlineChar->GetGuildName(uiUniqueID))
		CopyMemory(TempBuffer1, _pWorld->pIOnlineChar->GetGuildName(uiUniqueID), ON_ID_LENGTH);

	_pWorld->pIOnlineChar->GetPos(uiUniqueID, &siX, &siY);

	if(_pWorld->pMyData->GetMyCharUniqueID() == lpOnChangeCharacterCodeMsg->uiCharacID)
	{
		_pWorld->pMyData->GetFollowersInterface()->GetWearItems(0, uiEquipWeapon);
		_pWorld->pMyData->SetMyCharKind(lpOnChangeCharacterCodeMsg->uiNewCharCode);
	}
	else
	{
		_pWorld->pIOnlineChar->GetEquip(uiUniqueID, uiEquipWeapon);
	}

	
	_pWorld->pIOnlineChar->Free(uiUniqueID);
	_pWorld->pIOnlineChar->Init(uiUniqueID, (CHAR*)TempBuffer, uiKind, siX, siY, IP, 0, siFightPower, _pWorld,(UI16*)uiEquipWeapon);
	_pWorld->pIOnlineChar->SetEquipItem(uiUniqueID, (UI16*)uiEquipWeapon);
	

	_pWorld->pIOnlineChar->SetGuildData(uiUniqueID, (CHAR*)TempBuffer1, siGuildInGrade, siGuildFlag);
	_pWorld->pIOnlineChar->SetMercenary(uiUniqueID, siMercenaryKind);

	_pWorld->pIOnlineChar->SetChangeEffect(uiUniqueID);

	_pWorld->pOnlineInventory->SetMercenarySlot(0);

	_pWorld->pOnlineCharUI->SetAction(TRUE);

	return 0;
}

UI16 OnlineMsgProc::OnImperiumUse(BYTE *pMsg)
{
	OnImperiumUseMsg*	pOnImperiumUseMsg			= (OnImperiumUseMsg*)pMsg;

	DWORD		IP;
	CHAR		TempBuffer[128];
	SI32		siSiegeWarfareTransformType;		// ���� ���� ���� ����Ʈ

	// ĳ�� ���� ���
	UI16 uiUniqueID, uiKind, uiEquipWeapon[WEAR_ITEM_NUM];
	SI16 siX, siY;
	SI32 siFightPower, siGuildInGrade, siGuildFlag, siMercenaryKind;
	CHAR TempBuffer1[128];
	ZeroMemory(TempBuffer1, sizeof(TempBuffer1));

	uiUniqueID		= pOnImperiumUseMsg->uiUniqueID;
	uiKind			= pOnImperiumUseMsg->uiKind;
	IP				= _pWorld->pIOnlineChar->GetID(uiUniqueID);
	siFightPower	= _pWorld->pIOnlineChar->GetFightPower(uiUniqueID);
	siGuildInGrade	= _pWorld->pIOnlineChar->GetGuildInGrade(uiUniqueID);
	siGuildFlag		= _pWorld->pIOnlineChar->GetGuildFlag(uiUniqueID);
	siMercenaryKind = _pWorld->pIOnlineChar->GetMercenary(uiUniqueID);

	CopyMemory(TempBuffer, _pWorld->pMyData->GetMyCharName(), ON_ID_LENGTH);
	if(_pWorld->pIOnlineChar->GetGuildName(uiUniqueID))
		CopyMemory(TempBuffer1, _pWorld->pIOnlineChar->GetGuildName(uiUniqueID), ON_ID_LENGTH);

	_pWorld->pIOnlineChar->GetPos(uiUniqueID, &siX, &siY);

	// �����ϴ� ĳ���Ͱ� ����� �ڽ��� ���
	if(_pWorld->pMyData->GetMyCharUniqueID() == pOnImperiumUseMsg->uiUniqueID)
	{
		_pWorld->pMyData->GetFollowersInterface()->GetWearItems(0, uiEquipWeapon);

		_pWorld->pMyData->SetMyCharKind(pOnImperiumUseMsg->uiKind);

		// robypark 2004/11/19 17:1
		// �������� ���� ���� �� ���ΰ� ĳ���� DB ID ����
		_pWorld->pMyData->SetPrevCharOnlineKind(pOnImperiumUseMsg->uiPrevKind);

		// �������ִ� â�� �ݱ�
		// ���â
		if (_pWorld->pOnlineTradeBook->IsAction() )		
			_pWorld->pOnlineTradeBook->SetAction( FALSE );

		// �̴ϸ� â
		if (_pWorld->pOnlineMiniMap->IsActive() )		
			_pWorld->pOnlineMiniMap->SetActive( FALSE );
		// �뺴â
		if (_pWorld->m_pMercenary->IsAction() )	
			_pWorld->m_pMercenary->OnButtonPush();
		// ���� â
		if (_pWorld->pOnlineTip->IsAction())				
			_pWorld->pOnlineTip->SetAction(FALSE);
	}
	else
	{
		_pWorld->pIOnlineChar->GetEquip(uiUniqueID, uiEquipWeapon);
	}

	// ������ �������� ó�� ���ϴ��� Ȥ�� ���� �������� �˻��Ͽ� ����Ʈ�� ����Ѵ�.
	if (FALSE == (_pWorld->pIOnlineChar->IsSiegeWarfareUnit(uiUniqueID)))
	{
		siSiegeWarfareTransformType = EFFECT_CONSTANT_SIEGEWARFARE_TRANSFORM;	// ó�� ����
		PushEffect(EFFECT_UNITCHANGE, 0 , 0);	// ȿ���� ���

		// ������ ������ �� �� �� ���� ���ǻ��� �޽��� �ڽ� ���
		if(_pWorld->pMyData->GetMyCharUniqueID() == pOnImperiumUseMsg->uiUniqueID)
		{
			/*
			_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, 
											"",
											_pWorld->pOnlineText->Get(ON_TEXT_FIRST_TRANSFORM_TO_SIEGEWARFARE_UNIT));
			*/
			HDC hdc;
			// ������ ������ �� �� �� ���� ���ǻ��� �޽��� �ڽ� ���
			// HDC�� �̿��Ͽ� ���� �� ������ ����
			if(GETSCREENSURFACE(_pWorld)->GetDC(&hdc) == DD_OK)
			{
				SetMessage4((char*)_pWorld->pOnlineText->Get(ON_TEXT_FIRST_TRANSFORM_TO_SIEGEWARFARE_UNIT), hdc);

				GETSCREENSURFACE(_pWorld)->ReleaseDC(hdc);
			}
			_pWorld->pOnlinePannel->AddChat((char*)_pWorld->pOnlineText->Get(ON_TEXT_FIRST_TRANSFORM_TO_SIEGEWARFARE_UNIT), ON_INN_COLOR_WHITE, CHATTINGMODE_GUILD);
		}
	}
	else
	{
		siSiegeWarfareTransformType = EFFECT_CONSTANT_SIEGEWARFARE_RETRANSFORM;	// ������ ���� ����
		PushEffect(EFFECT_SUPPLEMENT, 0 , 0);	// ȿ���� ���
	}
	
	_pWorld->pIOnlineChar->Free(uiUniqueID);
	_pWorld->pIOnlineChar->Init(uiUniqueID, (CHAR*)TempBuffer, uiKind, siX, siY, IP, 0, siFightPower, _pWorld,(UI16*)uiEquipWeapon);
	_pWorld->pIOnlineChar->SetEquipItem(uiUniqueID, (UI16*)uiEquipWeapon);

	_pWorld->pIOnlineChar->SetGuildData(uiUniqueID, (CHAR*)TempBuffer1, siGuildInGrade, siGuildFlag);
	_pWorld->pIOnlineChar->SetMercenary(uiUniqueID, siMercenaryKind);

	// ������ �������� ó�� ���ϴ��� Ȥ�� ���� �������� �˻��Ͽ� ����Ʈ�� ����Ѵ�.
	_pWorld->pIOnlineChar->SetSiegeWarfare_TransformEffect(uiUniqueID, siSiegeWarfareTransformType);
	
	_pWorld->pOnlineInventory->SetMercenarySlot(0);

	_pWorld->pOnlineCharUI->SetAction(TRUE);

	_pWorld->pIOnlineChar->SetSiegeWarfare_Hp(uiUniqueID, pOnImperiumUseMsg->siHp);
	_pWorld->pIOnlineChar->SetSiegeWarfare_MaximumHP(uiUniqueID, pOnImperiumUseMsg->siHp);
	_pWorld->pIOnlineChar->SetSiegeWarfare_SupplyGoodsAmount(uiUniqueID, pOnImperiumUseMsg->siSupply);
	_pWorld->pIOnlineChar->SetSiegeWarfare_CurrentSupplyGoodsAmount(uiUniqueID, pOnImperiumUseMsg->siSupply);

	// ���� ////////////////////////////////
	// robypark 2004/11/9 21:10
	// ������ ������ ���� ����ǰ�� �ʱ�ȭ
	if(_pWorld->pMyData->GetMyCharUniqueID() == pOnImperiumUseMsg->uiUniqueID)
	{
		_pWorld->SetIsShowInformationAboutSiegeWarfareUnit(ON_GAMEOPTION_SIEGEWARFAREUNIT);	// ������ �����̴�.

		_pWorld->pOnlinePannel->SiegeWarfareUnitUI_DestroyWindow();

		// robypark 2005/1/28 14:59
		// ������ ���� �������̽� ����
		_pWorld->pOnlinePannel->SiegeWarfareUnitUI_CreateWindow(SW_NORMAL);
		_pWorld->pOnlinePannel->ShowWindow_SiegeWarfareUnitUI(SW_NORMAL);

		// ������ ������ ���� ���� �ð� ����(�ٷ� �� ������ �����ϵ��� 0���� �������ش�.)
		_pWorld->pIOnlineChar->SetSiegeWarfareUnit_LastAttackDelay(_pWorld->pMyData->GetMyCharUniqueID(), 0);
	}
	else
	{
		// ������ ĳ������ �� ������ ���ٸ�
		if(_pWorld->pIOnlineChar->GetDataType(pOnImperiumUseMsg->uiUniqueID) == 1)
		{
			if (_pWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
			{
				// �������� ��û�Ѵ�.
				_pWorld->pOnlineClient->SendPlayerDetailInfo(pOnImperiumUseMsg->uiUniqueID);
				_pWorld->pIOnlineChar->SetDataType(pOnImperiumUseMsg->uiUniqueID, 2);	// ���� �ٿ�ε� ��� ��...
			}
		}
	}
	// �� ////////////////////////////////

	_pWorld->pIOnlineChar->SetCaptain(uiUniqueID,pOnImperiumUseMsg->bCaptain);
	
	return 0;
}

// robypark 2005/1/28 18:44
// ������ �������� ���� ��û ����
// ON_RESPONSE_CHANGE_WAR
UI16 OnlineMsgProc::OnResChangeWar(BYTE *pMsg)
{
	OnResChangeUnit		*pOnResChangeUnit = (OnResChangeUnit *)pMsg;

	DWORD		IP;
	CHAR		TempBuffer[128];

	// ���� ����
	if (ON_CHANGE_OK == pOnResChangeUnit->dwResPonse)
	{
		// ĳ�� ���� ���
		UI16 uiUniqueID, uiKind, uiPrevKind, uiEquipWeapon[WEAR_ITEM_NUM];
		SI16 siX, siY;
		SI32 siFightPower, siGuildInGrade, siGuildFlag, siMercenaryKind;
		CHAR TempBuffer1[128];
		ZeroMemory(TempBuffer1, sizeof(TempBuffer1));

		uiUniqueID		= pOnResChangeUnit->uiUniqueID;
		uiKind			= pOnResChangeUnit->siKind;
		uiPrevKind		= _pWorld->pIOnlineChar->GetKind(uiUniqueID);
		IP				= _pWorld->pIOnlineChar->GetID(uiUniqueID);
		siFightPower	= _pWorld->pIOnlineChar->GetFightPower(uiUniqueID);
		siGuildInGrade	= _pWorld->pIOnlineChar->GetGuildInGrade(uiUniqueID);
		siGuildFlag		= _pWorld->pIOnlineChar->GetGuildFlag(uiUniqueID);
		siMercenaryKind = _pWorld->pIOnlineChar->GetMercenary(uiUniqueID);

		CopyMemory(TempBuffer, _pWorld->pMyData->GetMyCharName(), ON_ID_LENGTH);
		if(_pWorld->pIOnlineChar->GetGuildName(uiUniqueID))
			CopyMemory(TempBuffer1, _pWorld->pIOnlineChar->GetGuildName(uiUniqueID), ON_ID_LENGTH);

		_pWorld->pIOnlineChar->GetPos(uiUniqueID, &siX, &siY);

		// �����ϴ� ĳ���Ͱ� ����� �ڽ��� ���
		if(_pWorld->pMyData->GetMyCharUniqueID() == uiUniqueID)
		{
			_pWorld->pMyData->GetFollowersInterface()->GetWearItems(0, uiEquipWeapon);

			_pWorld->pMyData->SetMyCharKind(uiKind);

			// robypark 2004/11/19 17:1
			// �������� ���� ���� �� ���ΰ� ĳ���� DB ID ����
			_pWorld->pMyData->SetPrevCharOnlineKind(uiPrevKind);

			// �������ִ� â�� �ݱ�
			// ���â
			if (_pWorld->pOnlineTradeBook->IsAction() )		
				_pWorld->pOnlineTradeBook->SetAction( FALSE );

			// �̴ϸ� â
			if (_pWorld->pOnlineMiniMap->IsActive() )		
				_pWorld->pOnlineMiniMap->SetActive( FALSE );
			// �뺴â
			if (_pWorld->m_pMercenary->IsAction() )	
				_pWorld->m_pMercenary->OnButtonPush();
			// ���� â
			if (_pWorld->pOnlineTip->IsAction())				
				_pWorld->pOnlineTip->SetAction(FALSE);

			_pWorld->SetIsShowInformationAboutSiegeWarfareUnit(ON_GAMEOPTION_SIEGEWARFAREUNIT);	// ������ �����̴�.

			_pWorld->pOnlinePannel->SiegeWarfareUnitUI_DestroyWindow();

			// robypark 2005/1/28 14:59
			// ������ ���� �������̽� ����
			_pWorld->pOnlinePannel->SiegeWarfareUnitUI_CreateWindow(SW_NORMAL);
			_pWorld->pOnlinePannel->ShowWindow_SiegeWarfareUnitUI(SW_NORMAL);

			// ������ ������ ���� ���� �ð� ����(�ٷ� �� ������ �����ϵ��� 0���� �������ش�.)
			_pWorld->pIOnlineChar->SetSiegeWarfareUnit_LastAttackDelay(_pWorld->pMyData->GetMyCharUniqueID(), 0);

			// ���� ����(�޵�) ���� �ڵ� ȸ�� �ð� ����
			_pWorld->GetRecoveryManaInfo()->m_dwLeftTime = _pWorld->GetSOptionSiegeWarfare()->siMedicRechargeManaCoolTime;

			PushEffect(EFFECT_UNITCHANGE, 0 , 0);	// ȿ���� ���

			HDC hdc;
			// ������ ������ �� �� �� ���� ���ǻ��� �޽��� ���
			// HDC�� �̿��Ͽ� ���� �� ������ ����
			if(GETSCREENSURFACE(_pWorld)->GetDC(&hdc) == DD_OK)
			{
				SetMessage4((char*)_pWorld->pOnlineText->Get(ON_TEXT_FIRST_TRANSFORM_TO_SIEGEWARFARE_UNIT), hdc);

				GETSCREENSURFACE(_pWorld)->ReleaseDC(hdc);
			}
			_pWorld->pOnlinePannel->AddChat((char*)_pWorld->pOnlineText->Get(ON_TEXT_FIRST_TRANSFORM_TO_SIEGEWARFARE_UNIT), ON_INN_COLOR_WHITE, CHATTINGMODE_GUILD);

			// ����� ������ ����
			_pWorld->pMyData->SetMoney(_pWorld->pMyData->GetMoney() - pOnResChangeUnit->siMoney);

			// ������ ��ȭ ȿ�� ó��
			SEffectData					EffectData;
			ZeroMemory(&EffectData, sizeof(SEffectData));

			EffectData.m_Type = EFFECTTYPE_CHANGENUMBER;
			EffectData.m_EffectData_ChangeNum.m_Frame			= 0;
			EffectData.m_EffectData_ChangeNum.m_XPos			= 93;
			EffectData.m_EffectData_ChangeNum.m_YPos			= 579;
			EffectData.m_EffectData_ChangeNum.m_ChangeNumber    = -pOnResChangeUnit->siMoney;
			_pWorld->pOnlineFieldEffect->AddEffect(EffectData);
		}
		else
		{
			_pWorld->pIOnlineChar->GetEquip(uiUniqueID, uiEquipWeapon);
		}
		
		_pWorld->pIOnlineChar->Free(uiUniqueID);
		_pWorld->pIOnlineChar->Init(uiUniqueID, (CHAR*)TempBuffer, uiKind, siX, siY, IP, 0, siFightPower, _pWorld,(UI16*)uiEquipWeapon);
		_pWorld->pIOnlineChar->SetEquipItem(uiUniqueID, (UI16*)uiEquipWeapon);

		_pWorld->pIOnlineChar->SetGuildData(uiUniqueID, (CHAR*)TempBuffer1, siGuildInGrade, siGuildFlag);
		_pWorld->pIOnlineChar->SetMercenary(uiUniqueID, siMercenaryKind);

		// ������ �������� ���� ����Ʈ�� ����Ѵ�.
		_pWorld->pIOnlineChar->SetSiegeWarfare_TransformEffect(uiUniqueID, EFFECT_CONSTANT_SIEGEWARFARE_TRANSFORM);
		
		_pWorld->pOnlineInventory->SetMercenarySlot(0);

		_pWorld->pOnlineCharUI->SetAction(TRUE);

		_pWorld->pIOnlineChar->SetSiegeWarfare_Hp(uiUniqueID, pOnResChangeUnit->siHP);
		_pWorld->pIOnlineChar->SetSiegeWarfare_MaximumHP(uiUniqueID, pOnResChangeUnit->siHP);
		_pWorld->pIOnlineChar->SetSiegeWarfare_SupplyGoodsAmount(uiUniqueID, pOnResChangeUnit->siMP);
		_pWorld->pIOnlineChar->SetSiegeWarfare_CurrentSupplyGoodsAmount(uiUniqueID, pOnResChangeUnit->siMP);

		// ���� ////////////////////////////////
		// robypark 2004/11/9 21:10
		// ������ ������ ���� ����ǰ�� �ʱ�ȭ
		if(_pWorld->pMyData->GetMyCharUniqueID() == uiUniqueID)
		{
		}
		else
		{
			// ������ ĳ������ �� ������ ���ٸ�
			if(_pWorld->pIOnlineChar->GetDataType(uiUniqueID) == 1)
			{
				if (_pWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
				{
					// �������� ��û�Ѵ�.
					_pWorld->pOnlineClient->SendPlayerDetailInfo(uiUniqueID);
					_pWorld->pIOnlineChar->SetDataType(uiUniqueID, 2);	// ���� �ٿ�ε� ��� ��...
				}
			}
		}
		// �� ////////////////////////////////

		_pWorld->pIOnlineChar->SetCaptain(uiUniqueID, pOnResChangeUnit->bQuantity);
	}
	else if (ON_CHANGE_NOT_GUILD == pOnResChangeUnit->dwResPonse)          //��ܿ� ���ԵǾ����� �ʴ�.
	{
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, 
										"", 
										_pWorld->pOnlineText->Get(8100123));
	}
	else if (ON_CHANGE_ENOUGH_LEVEL == pOnResChangeUnit->dwResPonse)          //������ �����ϴ�. 
	{
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, 
										"", 
										_pWorld->pOnlineText->Get(8100122));
	}
	else if (ON_CHANGE_ENOUGH_MONEY == pOnResChangeUnit->dwResPonse)          //���� �����ϴ�.
	{
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, 
										"", 
										_pWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_NOTMONEY));
	}
	else if (ON_CHANGE_ENOUGH_KIND == pOnResChangeUnit->dwResPonse)          //�ش� �Ӽ��� ������ ����.
	{
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, 
										"", 
										_pWorld->pOnlineText->Get(8100121));
	}
	else if (ON_CHANGE_FAILED == pOnResChangeUnit->dwResPonse)          //�Ϲ����� ����
	{
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, 
										"", 
										_pWorld->pOnlineText->Get(8100123));
	}

	// ���� �޽��� ó�� �Ϸ�
	_pWorld->pOnlinePannel->GetSiegeWarfareTransformUIWnd()->SetSendedMsg(FALSE);

	return 0;
}

// ��� �ε��꺸�� ����(����)	
UI16 OnlineMsgProc::AuditOwn(BYTE *pMsg)
{
/*
	OnResAuditOwnListMsg* pOnResAuditOwnListMsg = (OnResAuditOwnListMsg*)pMsg;

	if(_pWorld->pOnlineTradeBook->IsAction())		return 0;

	if( pOnResAuditOwnListMsg->dwResponse == ON_RET_AUDIT_OK )
	{	
		OnResAuditOwnListOkMsg		*pOnResAuditOwnListOkMsg;
		OnAuditOwnInfo				*pOnAuditOwnInfo;
		
		pOnResAuditOwnListOkMsg	=	(OnResAuditOwnListOkMsg*)pOnResAuditOwnListMsg;				
		pOnAuditOwnInfo			=	(OnAuditOwnInfo*)(pOnResAuditOwnListOkMsg + 1 );

		OnlineVillageParser *lpOnlineVillageParser = _pWorld->pOnlineVillageManager->GetVillageParser();
		VillageHeader *lpVillageHeader = lpOnlineVillageParser->GetVillageHeaderCode(pOnAuditOwnInfo->uiVillageCode);
		UI08 uiTempPlantID = 0;
		for(int i = 0; i < ON_MAX_VIL_STRUCTNUM; i++)
		{
			switch( lpVillageHeader->siStructureKinds[i])
			{
			case ON_VIL_ST_KIND_FARM:
				uiTempPlantID = ON_VIL_ST_KIND_FARM;
				break;
			case ON_VIL_ST_KIND_PASTURE:
				uiTempPlantID = ON_VIL_ST_KIND_PASTURE;
				break;
			case ON_VIL_ST_KIND_FACTORY:
				uiTempPlantID = ON_VIL_ST_KIND_FACTORY;
				break;
			case ON_VIL_ST_KIND_ARMOR:
				uiTempPlantID = ON_VIL_ST_KIND_ARMOR;
				break;
			case ON_VIL_ST_KIND_WEAPON:
				uiTempPlantID = ON_VIL_ST_KIND_WEAPON;
				break;
			}
		}

		// �������� ����
		_pWorld->pMyData->uiPropertyVillageCode	= pOnAuditOwnInfo->uiVillageCode;
		_pWorld->pMyData->uiPlantID				= uiTempPlantID;
	}
*/
	_pWorld->pOnlineTradeBook->AuditOwn(pMsg);
	return 0;
}

UI16 OnlineMsgProc::GuildInfo(BYTE *pMsg)
{
	_pWorld->pOnlineTradeBook->GuildInfo(pMsg);
	return 0;
}

// ���� ������ ���� ���ϰ� �ִ�
UI16 OnlineMsgProc::MyVillageAttacked(BYTE *pMsg)
{
	OnMyVillageAttackedMsg* pOnMyVillageAttackedMsg = (OnMyVillageAttackedMsg*)pMsg;

	CHAR	TempBuffer[1024];
	VillageHeader* lpVillageHeader = _pWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(pOnMyVillageAttackedMsg->uiVillageCode);
	sprintf((char*)TempBuffer, _pWorld->pOnlineText->Get(ON_TEXT_PACKET_VILLAGE_ATTACK), _pWorld->pOnlineText->Get(lpVillageHeader->siVillageName));
//	_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", TempBuffer);

	HDC hdc;
	// HDC�� �̿��Ͽ� ���� �� ������ ����
	if(GETSCREENSURFACE(_pWorld)->GetDC(&hdc) == DD_OK)
	{
		SetMessage4((char*)TempBuffer, hdc);

		GETSCREENSURFACE(_pWorld)->ReleaseDC(hdc);
	}
	_pWorld->pOnlinePannel->AddChat((char*)TempBuffer, ON_INN_COLOR_WHITE, CHATTINGMODE_GUILD);

	return 0;
}

// ģ�� ����Ʈ
UI16 OnlineMsgProc::FriendList(BYTE *pMsg)
{
	OnResAuditFriendList *pOnResAuditFriendList = (OnResAuditFriendList*)pMsg;

	OnAuditFriendInfo *pOnAuditFriendInfo = (OnAuditFriendInfo*)(pOnResAuditFriendList+1);

	for(SI32 i = 0; i < pOnResAuditFriendList->siFriendCount; i++)
	{
		_pWorld->pMyData->m_GoodFriendList.AddFriend(_pWorld->pMyData->GetMyCharName(), pOnAuditFriendInfo->szID, pOnAuditFriendInfo->bLogin);
		pOnAuditFriendInfo++;
	}

	return 0;
}

// ģ�� ����Ʈ�� �ִ� ģ���� �α� �ƿ�
UI16 OnlineMsgProc::FriendLogOut(BYTE *pMsg)
{
	char szTemp[512];
	char szID[ON_ID_LENGTH+1];
	ZeroMemory( szID, sizeof(szID) );
	OnFriendLoggedInOut *pOnFriendLoggedInOut = (OnFriendLoggedInOut*)pMsg;
	
	strncpy( szID, pOnFriendLoggedInOut->szID, sizeof(pOnFriendLoggedInOut->szID) );
	_pWorld->pMyData->m_GoodFriendList.SetLogin(szID, pOnFriendLoggedInOut->bState);


	// �α���(�ƿ�) ���� ��� �޽��� ���
	string msg = pOnFriendLoggedInOut->bState == TRUE ? _pWorld->pOnlineText->Get(4000292) : _pWorld->pOnlineText->Get(4000293);
	sprintf( szTemp, "%s %s", szID, msg.c_str() );
	SetMessage( szTemp );
	return 0;
}

// ���� ����	
UI16 OnlineMsgProc::VillageInfo(BYTE *pMsg)
{
	OnResVillageInfoMsg*		lpOnResVillageInfoMsg = (OnResVillageInfoMsg*)pMsg;
	OnRecvVillageInfo*			lpOnRecvVillageInfo;

	if(lpOnResVillageInfoMsg->dwResponse == ON_RET_OK)
	{
		lpOnRecvVillageInfo = (OnRecvVillageInfo*)((BYTE*)lpOnResVillageInfoMsg + sizeof(OnResVillageInfoMsg));

		if(_pWorld->pOnlineVillageInfoData->m_uiVillageCode != (SI16)lpOnResVillageInfoMsg->uiVillageCode)
		{
			_pWorld->pOnlineVillageInfoData->m_siFrame = 0;
		}
		_pWorld->pOnlineVillageInfoData->m_siVillageSeriesWait = 0;
		_pWorld->pOnlineVillageInfoData->m_uiVillageCode     = (SI16)lpOnResVillageInfoMsg->uiVillageCode;
		CopyMemory(&_pWorld->pOnlineVillageInfoData->m_VillageInfo, lpOnRecvVillageInfo, sizeof(OnRecvVillageInfo));

		_pWorld->pOnlineVillageInfoData->m_lpVillageHeader = _pWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(_pWorld->pOnlineVillageInfoData->m_uiVillageCode);

		// ���� ���� �ȿ� �ִٸ�
		if (_pWorld->pOnlineVillage)
			_pWorld->pOnlineVillage->m_OnRecvVillageInfo.uiPopulation = lpOnRecvVillageInfo->uiPopulation;
	}

	return 0;
}

// �����	
UI16 OnlineMsgProc::BankAccountList(BYTE *pMsg)
{
	OnResBankBankBookListCountMsg*	lpOnResBankBankBookListCountMsg = (OnResBankBankBookListCountMsg*)pMsg;

	if(lpOnResBankBankBookListCountMsg->dwResponse == ON_RET_OK)
	{
		_pWorld->pOnlineTradeBook->Quest_SetBankBookCount(lpOnResBankBankBookListCountMsg->BankBookCount);
	}

	return 0;
}

// ���(�뺴,������) ��û�� ���� �Ѵ�	
UI16 OnlineMsgProc::SellArticle(BYTE *pMsg)
{
	_pWorld->pOnlineReception->WaitingReLay(pMsg);
	return 0;
}

// ������ ���� ��û	
UI16 OnlineMsgProc::Bidding(BYTE *pMsg)
{
	_pWorld->pOnlineFieldAuction->Bidding(pMsg);
	return 0;
}

UI16 OnlineMsgProc::ReportReMainTime(BYTE *pMsg)
{
	_pWorld->pOnlineFieldAuction->ReportReMainTime(pMsg);
	return 0;
}

// ���� ����		
UI16 OnlineMsgProc::ReportBidding(BYTE *pMsg)
{
	_pWorld->pOnlineFieldAuction->ReportBidding(pMsg);
	return 0;
}

// ����
UI16 OnlineMsgProc::ReportSuccessful(BYTE *pMsg)
{
	_pWorld->pOnlineFieldAuction->ReportSuccessful(pMsg);
	return 0;
}

// �ƹ��͵� �Ȼ�	
UI16 OnlineMsgProc::ReportFail(BYTE *pMsg)
{
	_pWorld->pOnlineFieldAuction->ReportSuccessful(pMsg);
	return 0;
}

// ��Ű� ���� ����
UI16 OnlineMsgProc::ReportNew(BYTE *pMsg)
{
	_pWorld->pOnlineFieldAuction->ReportNew(pMsg);
	return 0;
}

UI16 OnlineMsgProc::Enter(BYTE *pMsg)
{
	_pWorld->pOnlineFieldAuction->Enter(pMsg);
	return 0;
}

// ���� ����
UI16 OnlineMsgProc::OnResOpen(BYTE *pMsg)
{
	_pWorld->pOnlineBooth->OnResOpen(pMsg);
	return 0;
}

// ���ǿ��� ������ ����
UI16 OnlineMsgProc::OnResBuyItem(BYTE *pMsg)
{
	_pWorld->pOnlineBooth->OnResBuyItem(pMsg);
	return 0;
}

// ���� �ݾ�
UI16 OnlineMsgProc::OnResClose(BYTE *pMsg)
{
	_pWorld->pOnlineBooth->OnResClose(pMsg);
	return 0;
}

// ���� �뺴 ����
UI16 OnlineMsgProc::OnResBuyFollower(BYTE *pMsg)
{
	_pWorld->pOnlineBooth->OnResBuyFollower(pMsg);
	return 0;
}

// ���� ������ ����
UI16 OnlineMsgProc::OnResChangeItem(BYTE *pMsg)
{
	_pWorld->pOnlineBooth->OnResChangeItem(pMsg);
	return 0;
}

// �뺴 �̸� ����	
UI16 OnlineMsgProc::OnResSetName(BYTE *pMsg)
{
	OnResSetNameMsg	*pOnResSetNameMsg = (OnResSetNameMsg*)pMsg;

	if( pOnResSetNameMsg->dwResponse == ON_RET_AUDIT_OK )	_pWorld->pOnlineTradeBook->m_pOnlineCharStatus->RecvMsg_Rename(TRUE);
	else													_pWorld->pOnlineTradeBook->m_pOnlineCharStatus->RecvMsg_Rename(FALSE);
	return 0;
}

UI16 OnlineMsgProc::WearItem(BYTE *pMsg)
{
	_pWorld->pOnlineInventory->WearItem(pMsg);
	return 0;
}


UI16 OnlineMsgProc::StripItem(BYTE *pMsg)
{
	_pWorld->pOnlineInventory->StripItem(pMsg);
	return 0;
}

UI16 OnlineMsgProc::MoveItem(BYTE *pMsg)
{
	_pWorld->pOnlineInventory->MoveItem(pMsg);
	return 0;
}

UI16 OnlineMsgProc::ThrowItem(BYTE *pMsg)
{
	_pWorld->pOnlineInventory->ThrowItem(pMsg);
	return 0;
}

UI16 OnlineMsgProc::PickupItem(BYTE *pMsg)
{
	_pWorld->pOnlineInventory->PickupItem(pMsg);
	return 0;
}

UI16 OnlineMsgProc::SpendItem(BYTE *pMsg)
{
	_pWorld->pOnlineInventory->SpendItem(pMsg);
	return 0;
}

UI16 OnlineMsgProc::AddFollower(BYTE *pMsg)
{
	_pWorld->pOnlineInventory->AddFollower(pMsg);
	return 0;
}

UI16 OnlineMsgProc::RefineItem(BYTE *pMsg)
{
	_pWorld->pOnlineSmith->RefineItem(pMsg);
	return 0;
}

UI16 OnlineMsgProc::ChangeTradeGoods(BYTE *pMsg)
{	
	_pWorld->pOnlineTrade->ChangeTradeGoods(pMsg);
	return 0;
}

UI16 OnlineMsgProc::AuditTotal(BYTE *pMsg)
{
	_pWorld->pOnlineTradeBook->AuditTotal(pMsg);
	return 0;
}

UI16 OnlineMsgProc::FriendAddOne(BYTE *pMsg)
{
	_pWorld->pOnlineTradeBook->FriendAddOne(pMsg);
	return 0;
}

UI16 OnlineMsgProc::FriendRemove(BYTE *pMsg)
{
	_pWorld->pOnlineTradeBook->FriendRemove(pMsg);
	return 0;
}

UI16 OnlineMsgProc::FriendRemoveAll(BYTE *pMsg)
{
	_pWorld->pOnlineTradeBook->FriendRemoveAll(pMsg);
	return 0;
}

UI16 OnlineMsgProc::VillageList(BYTE *pMsg)
{
	_pWorld->pOnlineTradeBook->VillageList(pMsg);
	return 0;
}

/*UI16 OnlineMsgProc::StatiStics(BYTE *pMsg)
{
	_pWorld->pOnlineTradeBook->StatiStics(pMsg);
	return 0;
}*/

// actdoll (2004/03/30 11:21) : = �̺�Ʈ = ���� ����Ʈ �Ϸ��� ����
//UI16 OnlineMsgProc::OnDokdoQuestCompletedUser( BYTE *pMsg )
//{
//	OnDokdoQuestCompletedUserInfo	*pData	= (OnDokdoQuestCompletedUserInfo*)pMsg;
//	_pWorld->SetDokdoQuestCompleteUserCount( pData->uiQuestCompletedUserCount );
//
//	// �ѱ��� �����϶��� ǥ��ȴ�.
//	if( pGame->LanguageCode == ELANGAGECODE_KOREAN )	
//	{
//		if(_pWorld->m_LanguageCode == 0) //ELANGAGECODE_KOREA
//		{
//			char	pszText[128] = {0,};
//			sprintf( pszText, "[����] %s ���� %d ��°�� ������ �±ر⸦ �Ⱦҽ��ϴ�!", pData->szID, pData->uiQuestCompletedUserCount );
//			_pWorld->pOnlineNoticeList->AddNoticeData( pszText, RGB( 255, 255, 0 ) );
//		}
//	}
//	
//	return 0;
//}

//-----------------------------------------------------------------------------
// Name: OnReLoadWearItem()
// Code: actdoll (2004-07-19)
// Desc: ON_RELOAD_WEAR_ITEM ��ȣ ����. Server -> Client ��
//		���� �ð����� 24�ð����� ĳ���͵��� �����ִ� �ð��� ������ ������ �����϶�� ���̴�.
//		�� ����ü �Ĺ̿� stWearItem�� siMercenaryCount��ŭ �ٰ� �ȴ�.
//	pMsg	- �� �޽����� ������ ����( OnReLoadWearItem + ... )
//	return	- ����� �Դٸ� 0
//-----------------------------------------------------------------------------
UI16 OnlineMsgProc::OnBroadCast_ReloadWearItem( BYTE *pMsg )
{
	register	SI32	i, j;
	OnReLoadWearItem	*pORW;
//	stWearItem			*pstWI;
	OnTimerItemInfo		*pstOTII;
	IOnlineFollower		*pFG;
	OnlineFollower		*pFollower;
	UI16				uiOldItemID;
	SI16				siLimitDate;
	BOOL				bIsTimerItem, bDeleted = false;

	pORW	= (OnReLoadWearItem*)pMsg;									// �޽��� ��ü �ް�
//	pstWI	= (stWearItem*)( pMsg + sizeof(OnReLoadWearItem) );
	pstOTII	= (OnTimerItemInfo*)( pMsg + sizeof(OnReLoadWearItem) );	// �� �޽��� �Ĺ̿� ������ �ð��� ������ ���� ù ������
	pFG		= _pWorld->pMyData->GetFollowersInterface();				// ���� �� �뺴�� �׷� ������ �޴´�.

	// �� �޽����� �������� ��¥�� �ϴ� �����϶� �Ҹ���.
	for( i=0; i<ON_MAX_FOLLOWER_NUM; i++ )
	{
		pFollower	= pFG->GetFollower( i );
		if( !pFollower )	continue;
		
		// �����κ��� �������� Ȯ��
		for( j=0; j<WEAR_ITEM_NUM; j++ )
		{
			uiOldItemID	= pFollower->GetWearItem( j, bIsTimerItem, siLimitDate );	// �ش� ��ġ�� �������� �ް�
			if( uiOldItemID > 0 && bIsTimerItem )									// �ð��� �������� �´ٸ�
			{
				pFollower->SetWearItem( j, uiOldItemID, siLimitDate - 1 );			// �Ϸ縦 ���� �ٽ� ����
			}
		}
	}

	// �߰� �����Ͱ� �ִٸ� �������� �����϶� ���
	for( i=0; i<pORW->siItemCount; i++ )
	{
		// �뺴 ���� �ϳ��� �ް�
		pFollower = pFG->GetFollower( pstOTII->siMercenarySlot );
		if( !pFollower )	
		{
			pstOTII++;
			continue;
		}

		// �޾ƿ� ������ ���� Ȯ�λ��
		if( !_pWorld->pItemsInfo->GetItem( pstOTII->siItemID ) )			continue;
		if( !_pWorld->pItemsInfo->IsTimeLimitItem( pstOTII->siItemID ) )	continue;
		uiOldItemID	= pFollower->GetWearItem( pstOTII->siLoc, bIsTimerItem, siLimitDate );		// ���� ������ ���縦 �ް�
		if( pstOTII->siItemID != uiOldItemID )								continue;

		// ������� ������ Ȯ���ߴٸ� ������ �ֵ��� �Ѵ�.
		if( pstOTII->bDeleteTimerItem )
		{
			pFollower->SetWearItem( pstOTII->siLoc, 0, 0 );	// �ش� ������ �԰��ִ� ������ ������ Ŭ�����Ų��.

			// ���� ��ĳ���Ϳ� ���� �޽������
			if( pstOTII->siMercenarySlot == 0 )	
			{
				UI16	Equip[WEAR_ITEM_NUM];
				UI16	uiMyUniqueID	= _pWorld->pMyData->GetMyCharUniqueID();
				
				// �ϴ� ���� ��� ������ �ް�
				_pWorld->pIOnlineChar->GetEquip( uiMyUniqueID, Equip );
				
				// �޽����� ���Ե� ��ġ�� ���� ���¸� Ŭ����
				Equip[pstOTII->siLoc] = 0;

				// �� ������ �ٽ� ����ִ´�.
				_pWorld->pIOnlineChar->SetEquipItem( uiMyUniqueID, Equip );

				// �Ź� �������� ��� �� �ʵ� �̵� �ӵ��� �������� �����ش�.
				if( pstOTII->siLoc ==  ON_WEAR_SHOES_ITEM )
				{
					SI32	uiCharIndex		= _pWorld->pIOnlineChar->GetID( uiMyUniqueID );
					SI32	siCharKindIndex = _pWorld->pIOnlineCharKI->GetIndex( _pWorld->pMyData->GetMyCharKind() );
					_pWorld->pIOnlineChar->GetOnlineChar()[uiCharIndex].SetMoveSpeed( _pWorld->pIOnlineCharKI->GetMoveSpeed(siCharKindIndex) );
				}
			}

			bDeleted	= true;
		}

		// ����������~
		pstOTII++;
	}

	// ������ �������� ���� ��� �޽��� �ڽ��� ����� ��.
	if( bDeleted )	_pWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, _pWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_TIMERITEM), _pWorld->pOnlineText->Get(ON_TEXT_NOTICE_ITEM_TIMERITEM_IS_DELETED) );

	// �κ��丮�� Ȱ��ȭ �Ǿ��ִ� ��� ������ ������ �ѹ� ���Ž�Ų��.
	if( _pWorld->pOnlineInventory->IsActive() )	_pWorld->pOnlineInventory->FillItemListBox();

	return 0;
}

//���->������ ������.(���Ÿ�)
UI16 OnlineMsgProc::LongVillageAttack_Ready(BYTE* pMsg)
{
	VillageHeader*				lpVillageHeader;
//	SEffectData					EffectData;
	SI16						TempXPos;
	SI16						TempYPos;
	SI16						siAttackerPosX, siAttackerPosY;
//	SI16						siVillageXsize;
//	SI16						siVillageYsize;

	OnResLongAttackVillageRdyMsg*	pOnResLongAttackVillageRdyMsg;

	pOnResLongAttackVillageRdyMsg	=	(OnResLongAttackVillageRdyMsg*)pMsg;
	
	lpVillageHeader = _pWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(pOnResLongAttackVillageRdyMsg->uiVillageCode);
/*
	if(pOnResLongAttackVillageRdyMsg->uiAttackCharID != _pWorld->pMyData->GetMyCharUniqueID())
	{
		if(_pWorld->pIOnlineChar->IsWarp(pOnResLongAttackVillageRdyMsg->uiAttackCharID,
			pOnResLongAttackVillageRdyMsg->siAttackCharX, pOnResLongAttackVillageRdyMsg->siAttackCharY,1) == TRUE
			|| _pWorld->pIOnlineChar->IsMoving(pOnResLongAttackVillageRdyMsg->uiAttackCharID) == TRUE)	
		{
			_pWorld->pIOnlineChar->ClearOrder(pOnResLongAttackVillageRdyMsg->uiAttackCharID);

			_pWorld->pIOnlineChar->Warp(pOnResLongAttackVillageRdyMsg->uiAttackCharID, 
			pOnResLongAttackVillageRdyMsg->siAttackCharX, pOnResLongAttackVillageRdyMsg->siAttackCharY);
		}
	}
*/
	// ���� ��ġ �˻�
	TempXPos = lpVillageHeader->siX;
	TempYPos = lpVillageHeader->siY;


	if(_pWorld->pIOnlineChar->IsExistUniqueID(pOnResLongAttackVillageRdyMsg->uiAttackCharID))
	{
		// ���࿡ ������ ĳ���� Kind�� ������ Ŭ���̾�Ʈ���� �������� ĳ����Kind�� �ٸ� ��쿡 ó��
		if (pOnResLongAttackVillageRdyMsg->uiKind != _pWorld->pIOnlineChar->GetKind(pOnResLongAttackVillageRdyMsg->uiAttackCharID))
		{
			DWORD		IP;
			CHAR		TempBuffer[128];

			// ĳ�� ���� ���
			UI16 uiUniqueID, uiKind, uiEquipWeapon[WEAR_ITEM_NUM];
			SI16 siX, siY;
			SI32 siFightPower, siGuildInGrade, siGuildFlag, siMercenaryKind;
			CHAR TempBuffer1[128];
			
			SI32 siCurrentHP;
			SI32 siCurrentSupply;
			BOOL bCaptain;

			ZeroMemory(TempBuffer1, sizeof(TempBuffer1));

			uiUniqueID		= pOnResLongAttackVillageRdyMsg->uiAttackCharID;
			uiKind			= pOnResLongAttackVillageRdyMsg->uiKind;
			IP				= _pWorld->pIOnlineChar->GetID(uiUniqueID);
			siFightPower	= _pWorld->pIOnlineChar->GetFightPower(uiUniqueID);
			siGuildInGrade	= _pWorld->pIOnlineChar->GetGuildInGrade(uiUniqueID);
			siGuildFlag		= _pWorld->pIOnlineChar->GetGuildFlag(uiUniqueID);
			siMercenaryKind = _pWorld->pIOnlineChar->GetMercenary(uiUniqueID);
			siCurrentHP		= _pWorld->pIOnlineChar->GetSiegeWarfare_Hp(uiUniqueID);
			siCurrentSupply	= _pWorld->pIOnlineChar->GetSiegeWarfare_CurrentSupplyGoodsAmount(uiUniqueID);
			bCaptain		= _pWorld->pIOnlineChar->GetCaptain(uiUniqueID);

			if (_pWorld->pIOnlineChar->GetCharName(uiUniqueID))
				CopyMemory(TempBuffer, _pWorld->pIOnlineChar->GetCharName(uiUniqueID), ON_ID_LENGTH);
			else
				CopyMemory(TempBuffer, _pWorld->pMyData->GetMyCharName(), ON_ID_LENGTH);

			if(_pWorld->pIOnlineChar->GetGuildName(uiUniqueID))
				CopyMemory(TempBuffer1, _pWorld->pIOnlineChar->GetGuildName(uiUniqueID), ON_ID_LENGTH);

			_pWorld->pIOnlineChar->GetPos(uiUniqueID, &siX, &siY);

			// �����ϴ� ĳ���Ͱ� ����� �ڽ��� �ƴ� ���
			if(_pWorld->pMyData->GetMyCharUniqueID() != uiUniqueID)
			{
				_pWorld->pIOnlineChar->GetEquip(uiUniqueID, uiEquipWeapon);
			}

			_pWorld->pIOnlineChar->Free(uiUniqueID);
			_pWorld->pIOnlineChar->Init(uiUniqueID, (CHAR*)TempBuffer, uiKind, siX, siY, IP, 0, siFightPower, _pWorld,(UI16*)uiEquipWeapon);
			_pWorld->pIOnlineChar->SetEquipItem(uiUniqueID, (UI16*)uiEquipWeapon);

			_pWorld->pIOnlineChar->SetGuildData(uiUniqueID, (CHAR*)TempBuffer1, siGuildInGrade, siGuildFlag);
			_pWorld->pIOnlineChar->SetMercenary(uiUniqueID, siMercenaryKind);

			_pWorld->pOnlineInventory->SetMercenarySlot(0);

			_pWorld->pOnlineCharUI->SetAction(TRUE);

			_pWorld->pIOnlineChar->SetSiegeWarfare_Hp(uiUniqueID, siCurrentHP);
			_pWorld->pIOnlineChar->SetSiegeWarfare_CurrentSupplyGoodsAmount(uiUniqueID, siCurrentSupply);
			_pWorld->pIOnlineChar->SetCaptain(uiUniqueID, bCaptain);

			if(_pWorld->pMyData->GetMyCharUniqueID() != uiUniqueID)
			{
				// ������ ĳ������ �� ������ ���ٸ�
				if(_pWorld->pIOnlineChar->GetDataType(uiUniqueID) == 1)
				{
					if (_pWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
					{
						// �������� ��û�Ѵ�.
						_pWorld->pOnlineClient->SendPlayerDetailInfo(uiUniqueID);
						_pWorld->pIOnlineChar->SetDataType(uiUniqueID, 2);	// ���� �ٿ�ε� ��� ��...
					}
				}
			}
		}

		// ������ ��ġ ���
		_pWorld->pIOnlineChar->GetPos(pOnResLongAttackVillageRdyMsg->uiAttackCharID, &siAttackerPosX, &siAttackerPosY);

		if(lpVillageHeader->siX > siAttackerPosX)
			TempXPos -= 1;

		if(lpVillageHeader->siX < siAttackerPosX)
			TempXPos += 1;

		if(lpVillageHeader->siY > siAttackerPosY)
			TempYPos -= 1;

		if(lpVillageHeader->siY < siAttackerPosY)
			TempYPos += 1;
	
		//������ �����ٴ� ORDER�� ������.
		_pWorld->pIOnlineChar->AddOrder(pOnResLongAttackVillageRdyMsg->uiAttackCharID, 
										ON_ORDER_LONGDISTANCE_ATTACKVILLAGE_ACTION
										, TempXPos, TempYPos);

		// ���Ÿ� ����
		//�̺κ��� ��� ó���Ұ������� ����.
		//���⸦ �� ������ �Ǵ��Ͽ� �¾����� ó���Ѵ�.������ ����ϵ� �ѵ�.�ϴ�.�����.
		if(pOnResLongAttackVillageRdyMsg->uiAttackCharID == _pWorld->pMyData->GetMyCharUniqueID())
		{
			// robypark 2004/11/3 14:5
			// ������ ���� ������ ���� ����Ʈ ����
			_pWorld->SiegeWarfare_CreateAttackEffectVillage(pOnResLongAttackVillageRdyMsg->uiAttackCharID, TempXPos, TempYPos, pOnResLongAttackVillageRdyMsg->uiVillageCode);
		}
		else
		{
			// robypark 2004/11/3 14:5
			// ������ ���� ������ ���� ����Ʈ ����
			_pWorld->SiegeWarfare_CreateAttackEffectVillage(pOnResLongAttackVillageRdyMsg->uiAttackCharID, TempXPos, TempYPos, 0);
		}
		/*
			// robypark 2004/11/4 13:56	
			// ����ȿ��(���ݷ� ��), �������� ���� ȿ�� ó�� �߰�
			SI16 scGeneralEffect = _pWorld->SiegeWarfare_IsInAreaGeneralEffect();
			if (-1 != scGeneralEffect)
				_pWorld->pOnlineClient->SendLongDistanceAttackVillageAction(pOnResLongAttackVillageRdyMsg->uiVillageCode, scGeneralEffect);
		*/
	}
	
	return 0;
}

UI16 OnlineMsgProc::LongVillageAttack_Action(BYTE* pMsg)
{
	OnResLongAttackVillageActionMsg*		pOnResLongAttackVillageActionMsg = NULL;
	SVillageData*							lpSVillageData = NULL;
	VillageHeader*							lpVillageHeader = NULL;
//	SEffectData								EffectData;
	SEffectData								TempSEffectData;				
	SI16									TempXPos;
	SI16									TempYPos;
	VDEF									vdTempVillageDefence = 0;

	pOnResLongAttackVillageActionMsg	=	(OnResLongAttackVillageActionMsg*)pMsg;

	lpVillageHeader = _pWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(pOnResLongAttackVillageActionMsg->uiVillageCode);

	// ������ ������ ���� ���� ���� �� ���� �� ����
	lpSVillageData = _pWorld->pOnlineVillageDescriptList->GetVillageDataData(lpVillageHeader->uiVillageCode);				
	if(lpSVillageData)
	{
		vdTempVillageDefence = lpSVillageData->m_vdChangedDefence;		// ���� ���� ���� ����. ���� ����Ʈ�� �ѹ���

		if(lpSVillageData->m_fWait == FALSE)
		{
			if(pOnResLongAttackVillageActionMsg->siDefenceSoldier <= 0)
			{
				// ������ ��尡 ������ ���
				_pWorld->pOnlineVillageDescriptList->DelVillageDataData(lpVillageHeader->uiVillageCode);

				if(_pWorld->pOnlineMap->m_MapInfo.uiVillageCode)
				{
					_pWorld->pOnlineClient->SendRequestVillageInfo(_pWorld->pOnlineMap->m_MapInfo.uiVillageCode);
				}
			}
			else
			{
				lpSVillageData->m_vdChangedDefence    = pOnResLongAttackVillageActionMsg->vdDefence;	// ���� ����

				lpSVillageData->m_siCurrentGarrison = pOnResLongAttackVillageActionMsg->siDefenceSoldier;	// ���� ����

				// ���� ���� �ִ밪���� ū ���
				if(lpSVillageData->m_vdChangedMaxDefence < lpSVillageData->m_vdChangedDefence)
				{
					lpSVillageData->m_vdChangedMaxDefence = lpSVillageData->m_vdChangedDefence;
				}

				// ���� ���� �ִ밪���� ū ���
				if(lpSVillageData->m_siMaximumGarrison < lpSVillageData->m_siCurrentGarrison)
				{
					lpSVillageData->m_siMaximumGarrison = lpSVillageData->m_siCurrentGarrison;
				}
			}
		}
	}

	// ���� ���߽� (���� ����)
	if(lpSVillageData)
	{
		// ���� ������ 0�� �� ��� ���� ����Ʈ ���.
		if (lpSVillageData->m_vdChangedDefence == 0)
		{
			TempXPos = lpVillageHeader->siX;
			TempYPos = lpVillageHeader->siY;

			TempSEffectData.m_fCreate = TRUE;
			TempSEffectData.m_Type    = EFFECTTYPE_VILLAGEEXPLOSION;
			TempSEffectData.m_EffectData_VillageExplosion.m_Frame = 0;
			TempSEffectData.m_EffectData_VillageExplosion.m_XPos  = TempXPos;
			TempSEffectData.m_EffectData_VillageExplosion.m_YPos  = TempYPos;

			_pWorld->pOnlineFieldEffect->AddEffect(TempSEffectData);

			// ������ ���ɵǾ��� ȿ�� �Ѹ���
			TempXPos = lpVillageHeader->siX;
			TempYPos = lpVillageHeader->siY;

			TempSEffectData.m_fCreate = TRUE;
			TempSEffectData.m_Type    = EFFECTTYPE_OCCUPIED_VILLAGE;
			TempSEffectData.m_EffectData_OccupiedVillage.m_Frame = 0;

			TempSEffectData.m_EffectData_OccupiedVillage.m_XPos  = TempXPos + 1;
			TempSEffectData.m_EffectData_OccupiedVillage.m_YPos  = TempYPos;

			// robypark 2004/12/27 18:19
			TempSEffectData.m_EffectData_OccupiedVillage.m_scMaxRepeatTimes = ON_FIELDEFFECT_ANIMATION_OCCUPIEDVILLAGE_REPEATTIMES;	// Animation ��� �ݺ� Ƚ�� ����

			_pWorld->pOnlineFieldEffect->AddEffect(TempSEffectData);

			PushEffect(EFFECT_GETCASTLE, 0, 0);
		}
	}

	// robypark 2004/14:32
	// ġ���� ����, �߰��� ���, �Ϲ� ����, ����� ��� ȿ�� ���
	switch (pOnResLongAttackVillageActionMsg->siAttackType)
	{
		// ġ���� ����. ������ ĳ���Ϳ� ȿ�� �߰�
		case ON_RET_ATTACK_POWER:
			{
				// ������ ĳ���� ǥ�� ����Ʈ�� ġ���� ���� ��뿡�� ����Ѵ�.
				_pWorld->pIOnlineChar->SetNewSelectedFlag(pOnResLongAttackVillageActionMsg->uiAttackCharID, TRUE);
				_pWorld->pIOnlineChar->SetSiegeWarfare_DeathBlow(pOnResLongAttackVillageActionMsg->uiAttackCharID);
				PushEffect(EFFECT_CRITICAL, 0 , 0);	// ġ���� ���� ȿ���� ���
			}
			break;
		// �߰��� ���. �ǰ��� ĳ���Ϳ� ȿ�� �߰�
		case ON_RET_DEFENCE_POWER:
			{
				//_pWorld->pIOnlineChar->SetSiegeWarfare_Evasion(pOnResLongAttackVillageActionMsg->uiAttackCharID);
				//PushEffect(EFFECT_DEFENCE, 0 , 0);	// �߰��� ��� ȿ���� ���
			}
			break;
		// �Ϲ� ����, ���
		case ON_RET_NONE_POWER:
		default:
			break;
	}

	/*
	OnResLongAttackVillageActionMsg*		pOnResLongAttackVillageActionMsg;
	SVillageData*							lpSVillageData;
	VillageHeader*							lpVillageHeader;

	pOnResLongAttackVillageActionMsg	=	(OnResLongAttackVillageActionMsg*)pMsg;
	

	lpVillageHeader = _pWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(pOnResLongAttackVillageActionMsg->uiVillageCode);

	pOnResLongAttackVillageActionMsg->uiAttackCharID;
	//�������� ���ݿ� ���ؼ� ������ ����.
	pOnResLongAttackVillageActionMsg->siDamage;	

	pOnResLongAttackVillageActionMsg->vdChangedDefence;

	lpSVillageData = _pWorld->pOnlineVillageDescriptList->GetVillageDataData(lpVillageHeader->uiVillageCode);				

	if(lpSVillageData)
	{
		if(lpSVillageData->m_fWait == FALSE)
		{
			if(pOnResLongAttackVillageActionMsg->vdChangedDefence <= 0)
			{
				// ������ ��尡 ������ ���
				_pWorld->pOnlineVillageDescriptList->DelVillageDataData(lpVillageHeader->uiVillageCode);

				if(_pWorld->pOnlineMap->m_MapInfo.uiVillageCode)
				{
					_pWorld->pOnlineClient->SendRequestVillageInfo(_pWorld->pOnlineMap->m_MapInfo.uiVillageCode);
				}
			}
			else
			{
				lpSVillageData->m_vdChangedDefence    = pOnResLongAttackVillageActionMsg->vdChangedDefence;
				if(lpSVillageData->m_vdChangedMaxDefence < lpSVillageData->m_vdChangedDefence)
				{
					lpSVillageData->m_vdChangedMaxDefence = lpSVillageData->m_vdChangedDefence;
				}
			}
		}
	}

	// ���� ���߽�
	if(lpSVillageData)
	{
		if(lpSVillageData->m_vdChangedDefence == 0)
		{
			TempXPos = lpVillageHeader->siX;
			TempYPos = lpVillageHeader->siY;

			TempSEffectData.m_fCreate = TRUE;
			TempSEffectData.m_Type    = EFFECTTYPE_VILLAGEEXPLOSION;
			TempSEffectData.m_EffectData_VillageExplosion.m_Frame = 0;
			TempSEffectData.m_EffectData_VillageExplosion.m_XPos  = TempXPos;
			TempSEffectData.m_EffectData_VillageExplosion.m_YPos  = TempYPos;

			_pWorld->pOnlineFieldEffect->AddEffect(TempSEffectData);

			PushEffect(EFFECT_GETCASTLE, 0, 0);
		}
	}
	*/



	return 0;
}

//����->����� ������.
// ���� �⸧ �ݰ�
UI16 OnlineMsgProc::VillagePlayerOilAttack(BYTE* pMsg)
{
	OnResVillagePlayerOilAttack*	pOnResVillagePlayerOilAttack;
	VillageHeader*					lpVillageHeader;
	SVillageData*					lpSVillageData;

	pOnResVillagePlayerOilAttack		=	(OnResVillagePlayerOilAttack*)pMsg;

	if (ON_RET_OIL_OK == pOnResVillagePlayerOilAttack->dwResponse)
	{
		// ���� ������ ��� ���
		lpVillageHeader = _pWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(pOnResVillagePlayerOilAttack->uiVillageCode);

		if (lpVillageHeader)
		{
			// ���� ����(����) ������ ���
			lpSVillageData = _pWorld->pOnlineVillageDescriptList->GetVillageDataData(lpVillageHeader->uiVillageCode);				
		
			if (lpSVillageData)
			{
				// ���� ���� �����͸� �ٿ�ε� ���� �ƴϸ�
				if(lpSVillageData->m_fWait == FALSE)
				{
					lpSVillageData->m_ssCounterAttackType = ON_VILLAGE_ATTACK_OIL;	// ������ �⸧ �����Ѵٰ� ����
				}
			}
			else
			{
				// ������ Server�� Data ��û
				_pWorld->pOnlineClient->SendRequestVillageData(lpVillageHeader->uiVillageCode);
				_pWorld->pOnlineVillageDescriptList->AddVillageDataData(lpVillageHeader->uiVillageCode);
			}

			// �����ϱ⸦ �̿��� ����ڰ� �ڽ��̶��
			if (_pWorld->pMyData->GetMyCharUniqueID() == pOnResVillagePlayerOilAttack->uiUniqueID)
			{
				// ����� ������ ����
				_pWorld->pMyData->SetMoney(_pWorld->pMyData->GetMoney() - pOnResVillagePlayerOilAttack->mnMoney);

				// ������ ��ȭ ȿ�� ó��
				SEffectData					EffectData;
				ZeroMemory(&EffectData, sizeof(SEffectData));

				EffectData.m_Type = EFFECTTYPE_CHANGENUMBER;
				EffectData.m_EffectData_ChangeNum.m_Frame			= 0;
				EffectData.m_EffectData_ChangeNum.m_XPos			= 93;
				EffectData.m_EffectData_ChangeNum.m_YPos			= 579;
				EffectData.m_EffectData_ChangeNum.m_ChangeNumber    = -pOnResVillagePlayerOilAttack->mnMoney;
				_pWorld->pOnlineFieldEffect->AddEffect(EffectData);

				// ���ο� ���� �ݰ� ���� �ð� ����
				_pWorld->GetVillageDefenceInterfaceInfo()->SetDelayTimeUsingVillageAttackType(pOnResVillagePlayerOilAttack->siLimitTime);
			}
		}
	}
	else if (ON_RET_OIL_NOT_MONEY == pOnResVillagePlayerOilAttack->dwResponse)
	{
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", _pWorld->pOnlineText->Get(ON_TEXT_ERRORNOTMONEY));
	}
	else if (ON_RET_OIL_NO == pOnResVillagePlayerOilAttack->dwResponse)
	{
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", _pWorld->pOnlineText->Get(ON_TEXT_FAIL_TO_COUNTERATTACK_OIL));
	}

	return 0;
}

// ���� �� �ݰ�
UI16 OnlineMsgProc::VillagePlayerStoneAttack(BYTE* pMsg)
{
	OnResVillagePlayerStoneAttack*	pOnResVillagePlayerStoneAttack = NULL;

	VillageHeader*					lpVillageHeader;
	SVillageData*					lpSVillageData;

	pOnResVillagePlayerStoneAttack		=	(OnResVillagePlayerStoneAttack*)pMsg;

	if (ON_RET_STONE_OK == pOnResVillagePlayerStoneAttack->dwResponse)
	{
		// ���� ������ ��� ���
		lpVillageHeader = _pWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(pOnResVillagePlayerStoneAttack->uiVillageCode);

		if (lpVillageHeader)
		{
			// ���� ����(����) ������ ���
			lpSVillageData = _pWorld->pOnlineVillageDescriptList->GetVillageDataData(lpVillageHeader->uiVillageCode);				
			
			if (lpSVillageData)
			{
				// ���� ���� �����͸� �ٿ�ε� ���� �ƴϸ�
				if(lpSVillageData->m_fWait == FALSE)
				{
					lpSVillageData->m_ssCounterAttackType = ON_VILLAGE_ATTACK_STONE;	// ������ �� �����Ѵٰ� ����
				}
			}
			else
			{
				// ������ Server�� Data ��û
				_pWorld->pOnlineClient->SendRequestVillageData(lpVillageHeader->uiVillageCode);
				_pWorld->pOnlineVillageDescriptList->AddVillageDataData(lpVillageHeader->uiVillageCode);
			}

			// �����ϱ⸦ �̿��� ����ڰ� �ڽ��̶��
			if (_pWorld->pMyData->GetMyCharUniqueID() == pOnResVillagePlayerStoneAttack->uiUniqueID)
			{
				// ����� ������ ����
				_pWorld->pMyData->SetMoney(_pWorld->pMyData->GetMoney() - pOnResVillagePlayerStoneAttack->mnMoney);

				// ������ ��ȭ ȿ�� ó��
				SEffectData					EffectData;
				ZeroMemory(&EffectData, sizeof(SEffectData));

				EffectData.m_Type = EFFECTTYPE_CHANGENUMBER;
				EffectData.m_EffectData_ChangeNum.m_Frame			= 0;
				EffectData.m_EffectData_ChangeNum.m_XPos			= 93;
				EffectData.m_EffectData_ChangeNum.m_YPos			= 579;
				EffectData.m_EffectData_ChangeNum.m_ChangeNumber    = -pOnResVillagePlayerStoneAttack->mnMoney;
				_pWorld->pOnlineFieldEffect->AddEffect(EffectData);

				// ���ο� ���� �ݰ� ���� �ð� ����
				_pWorld->GetVillageDefenceInterfaceInfo()->SetDelayTimeUsingVillageAttackType(pOnResVillagePlayerStoneAttack->siLimitTime);
			}
		}
	}
	else if (ON_RET_STONE_NOT_MONEY == pOnResVillagePlayerStoneAttack->dwResponse)
	{
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", _pWorld->pOnlineText->Get(ON_TEXT_ERRORNOTMONEY));
	}
	else if (ON_RET_STONE_NO == pOnResVillagePlayerStoneAttack->dwResponse)
	{
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", _pWorld->pOnlineText->Get(ON_TEXT_FAIL_TO_COUNTERATTACK_STONE));
	}

	return 0;
}	

// ������ ���� �ݰ� - ���� ���� ���
// ON_RESPONSE_VILLAGE_DEFENCE
UI16 OnlineMsgProc::OnResponseVillageDefence(BYTE *pMsg)
{
	OnResPlayerVillageDefenceUp			*pOnResPlayerVillageDefenceUp = (OnResPlayerVillageDefenceUp *)pMsg;
	VillageHeader*						lpVillageHeader;
	SVillageData*						lpSVillageData;

	// ���� ���� ��� ����
	if (ON_RET_DEFENSE_OK == pOnResPlayerVillageDefenceUp->dwResponse)
	{
		// ���� ������ ��� ���
		lpVillageHeader = _pWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(pOnResPlayerVillageDefenceUp->uiVillageCode);

		if (lpVillageHeader)
		{
			// ���� ����(����) ������ ���
			lpSVillageData = _pWorld->pOnlineVillageDescriptList->GetVillageDataData(lpVillageHeader->uiVillageCode);
		
			if (lpSVillageData)
			{
				// ���� ���� �����͸� �ٿ�ε� ���� �ƴϸ�
				if(lpSVillageData->m_fWait == FALSE)
					lpSVillageData->m_vdChangedDefence += pOnResPlayerVillageDefenceUp->vdDefenceUp;	// �ö� ������ �����ش�.
			}
			else
			{
				// ������ Server�� Data ��û
				_pWorld->pOnlineClient->SendRequestVillageData(lpVillageHeader->uiVillageCode);
				_pWorld->pOnlineVillageDescriptList->AddVillageDataData(lpVillageHeader->uiVillageCode);
			}
		}

		// �����ϱ⸦ �̿��� ����ڰ� �ڽ��̶��
		if (_pWorld->pMyData->GetMyCharUniqueID() == pOnResPlayerVillageDefenceUp->uiUniqueID)
		{
			// ����� ������ ����
			_pWorld->pMyData->SetMoney(_pWorld->pMyData->GetMoney() - pOnResPlayerVillageDefenceUp->mnMoney);

			// ������ ��ȭ ȿ�� ó��
			SEffectData					EffectData;
			ZeroMemory(&EffectData, sizeof(SEffectData));

			EffectData.m_Type = EFFECTTYPE_CHANGENUMBER;
			EffectData.m_EffectData_ChangeNum.m_Frame			= 0;
			EffectData.m_EffectData_ChangeNum.m_XPos			= 93;
			EffectData.m_EffectData_ChangeNum.m_YPos			= 579;
			EffectData.m_EffectData_ChangeNum.m_ChangeNumber    = -pOnResPlayerVillageDefenceUp->mnMoney;
			_pWorld->pOnlineFieldEffect->AddEffect(EffectData);

			// ���ο� ���� �ݰ� ���� �ð� ����
			_pWorld->GetVillageDefenceInterfaceInfo()->SetDelayTimeUsingVillageAttackType(pOnResPlayerVillageDefenceUp->siLimitTime);

			CSiegeWarfareUnitUI *pSiegeWarfareUnitUI = _pWorld->pOnlinePannel->GetSiegeWarfareUnitUIHandle();

			if (pSiegeWarfareUnitUI)
			{
				pSiegeWarfareUnitUI->SetCurrentDelayTimeUsingVillageAttackType(
						pOnResPlayerVillageDefenceUp->siLimitTime);

				pSiegeWarfareUnitUI->SetMaxDelayTimeUsingVillageAttackType(
						pOnResPlayerVillageDefenceUp->siLimitTime);
			}
		}

		// ���� ��� ����Ʈ �߰�
		SI16 TempXPos = lpVillageHeader->siX;	// ���� ��ġ
		SI16 TempYPos = lpVillageHeader->siY;

		SEffectData					TempSEffectData;
		ZeroMemory(&TempSEffectData, sizeof(SEffectData));

		TempSEffectData.m_fCreate = TRUE;

		TempSEffectData.m_Type    = EFFECTTYPE_VILLAGE_DEFENCE_UP;
		TempSEffectData.m_EffectData_VillageDefenceUp.m_Frame = 0;

		// ���� ũ�⿡ ���� ���� ��� ����Ʈ ��ġ ����
		switch (_pWorld->pOnlineResource->GetVillageSize((char *)lpVillageHeader->szImageFile))
		{
			case 0:	// ū ����
				TempXPos -= 3;
				TempYPos -= 3;
				break;
			case 1: // �߰� ����
				TempXPos -= 2;
				TempYPos -= 2;
				break;
			case 2: // ���� ����
				TempXPos -= 1;
				TempYPos -= 1;
				break;
			default:
				break;
		}

		TempSEffectData.m_EffectData_VillageDefenceUp.m_XPos  = TempXPos;
		TempSEffectData.m_EffectData_VillageDefenceUp.m_YPos  = TempYPos;
			
		_pWorld->pOnlineFieldEffect->AddEffect(TempSEffectData);

		PushEffect(EFFECT_WALLREPAIR, 0 , 0);	// ���� ���� ����ϴ� ȿ���� ���
	}
	// ���� ������ ���
	else if (ON_RET_DEFENSE_NOT_MONEY == pOnResPlayerVillageDefenceUp->dwResponse)
	{
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", _pWorld->pOnlineText->Get(ON_TEXT_ERRORNOTMONEY));
	}
	else	// ON_RET_DEFENSE_NO
	{
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", _pWorld->pOnlineText->Get(ON_TEXT_FAIL_TO_INCREASE_VILLAGE_DEFENCE));
	}

	return 0;
}

// ������ ���� �ݰ��� ������.
// ON_CHANGED_VILLAGE_ATTACK_TYPE
UI16	OnlineMsgProc::OnBroadcastChangedVillageAttackType(BYTE *pMsg )
{
	OnChangedVillageAttackType		*pOnChangedVillageAttackType;
	VillageHeader*					lpVillageHeader;
	SVillageData*					lpSVillageData;

	pOnChangedVillageAttackType		=	(OnChangedVillageAttackType *)pMsg;

	// ���� ������ ��� ���
	lpVillageHeader = _pWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(pOnChangedVillageAttackType->uiVillageCode);

	if (lpVillageHeader)
	{
		// ���� ����(����) ������ ���
		lpSVillageData = _pWorld->pOnlineVillageDescriptList->GetVillageDataData(lpVillageHeader->uiVillageCode);				
		
		if (lpSVillageData)
		{
			// ���� ���� �����͸� �ٿ�ε� ���� �ƴϸ�
			if(lpSVillageData->m_fWait == FALSE)
			{
				lpSVillageData->m_ssCounterAttackType = pOnChangedVillageAttackType->siAttackType;	// ������ �ݰ� Ÿ�� ����
			}
		}
		else
		{
			// ������ Server�� Data ��û
			_pWorld->pOnlineClient->SendRequestVillageData(lpVillageHeader->uiVillageCode);
			_pWorld->pOnlineVillageDescriptList->AddVillageDataData(lpVillageHeader->uiVillageCode);
		}
	}

	return 0;
}

UI16 OnlineMsgProc::FieldAttack_Ready(BYTE* pMsg)
{
	SI16 siAttackCharX,siAttackCharY;
	OnResFieldAttackRdyMsg*		pOnResFieldAttackRdyMsg;

	pOnResFieldAttackRdyMsg		=	(OnResFieldAttackRdyMsg*)pMsg;

	if(pOnResFieldAttackRdyMsg->dwResponse == ON_RET_OK)
	{
		_pWorld->pIOnlineChar->GetPos(pOnResFieldAttackRdyMsg->uiEnemyCharID, &siAttackCharX, &siAttackCharY);
/*
		if(pOnResFieldAttackRdyMsg->uiAttackCharID != _pWorld->pMyData->GetMyCharUniqueID())
		{
			if(_pWorld->pIOnlineChar->IsWarp(pOnResFieldAttackRdyMsg->uiAttackCharID,
				pOnResFieldAttackRdyMsg->siAttackCharX, pOnResFieldAttackRdyMsg->siAttackCharY,1) == TRUE
				|| _pWorld->pIOnlineChar->IsMoving(pOnResFieldAttackRdyMsg->uiAttackCharID) == TRUE)
			{
				//_pWorld->pIOnlineChar->bMovingSwitch = FALSE;
				_pWorld->pIOnlineChar->ClearOrder(pOnResFieldAttackRdyMsg->uiAttackCharID);

				_pWorld->pIOnlineChar->Warp(pOnResFieldAttackRdyMsg->uiAttackCharID, 
				pOnResFieldAttackRdyMsg->siAttackCharX, pOnResFieldAttackRdyMsg->siAttackCharY);
			}
		}
*/
		if(_pWorld->pIOnlineChar->IsExistUniqueID(pOnResFieldAttackRdyMsg->uiAttackCharID))
		{
			// ���࿡ ������ ĳ���� Kind�� ������ Ŭ���̾�Ʈ���� �������� ĳ����Kind�� �ٸ� ��쿡 ó��
			if (pOnResFieldAttackRdyMsg->uiKind != _pWorld->pIOnlineChar->GetKind(pOnResFieldAttackRdyMsg->uiAttackCharID))
			{
				DWORD		IP;
				CHAR		TempBuffer[128];

				// ĳ�� ���� ���
				UI16 uiUniqueID, uiKind, uiEquipWeapon[WEAR_ITEM_NUM];
				SI16 siX, siY;
				SI32 siFightPower, siGuildInGrade, siGuildFlag, siMercenaryKind;
				CHAR TempBuffer1[128];
				SI32 siCurrentHP;
				SI32 siCurrentSupply;
				BOOL bCaptain;

				ZeroMemory(TempBuffer1, sizeof(TempBuffer1));

				uiUniqueID		= pOnResFieldAttackRdyMsg->uiAttackCharID;
				uiKind			= pOnResFieldAttackRdyMsg->uiKind;
				IP				= _pWorld->pIOnlineChar->GetID(uiUniqueID);
				siFightPower	= _pWorld->pIOnlineChar->GetFightPower(uiUniqueID);
				siGuildInGrade	= _pWorld->pIOnlineChar->GetGuildInGrade(uiUniqueID);
				siGuildFlag		= _pWorld->pIOnlineChar->GetGuildFlag(uiUniqueID);
				siMercenaryKind = _pWorld->pIOnlineChar->GetMercenary(uiUniqueID);
				siCurrentHP		= _pWorld->pIOnlineChar->GetSiegeWarfare_Hp(uiUniqueID);
				siCurrentSupply	= _pWorld->pIOnlineChar->GetSiegeWarfare_CurrentSupplyGoodsAmount(uiUniqueID);
				bCaptain		= _pWorld->pIOnlineChar->GetCaptain(uiUniqueID);

				if (_pWorld->pIOnlineChar->GetCharName(uiUniqueID))
					CopyMemory(TempBuffer, _pWorld->pIOnlineChar->GetCharName(uiUniqueID), ON_ID_LENGTH);
				else
					CopyMemory(TempBuffer, _pWorld->pMyData->GetMyCharName(), ON_ID_LENGTH);

				if(_pWorld->pIOnlineChar->GetGuildName(uiUniqueID))
					CopyMemory(TempBuffer1, _pWorld->pIOnlineChar->GetGuildName(uiUniqueID), ON_ID_LENGTH);

				_pWorld->pIOnlineChar->GetPos(uiUniqueID, &siX, &siY);

				// �����ϴ� ĳ���Ͱ� ����� �ڽ��� �ƴ� ���
				if(_pWorld->pMyData->GetMyCharUniqueID() != uiUniqueID)
				{
					_pWorld->pIOnlineChar->GetEquip(uiUniqueID, uiEquipWeapon);
				}

				_pWorld->pIOnlineChar->Free(uiUniqueID);
				_pWorld->pIOnlineChar->Init(uiUniqueID, (CHAR*)TempBuffer, uiKind, siX, siY, IP, 0, siFightPower, _pWorld,(UI16*)uiEquipWeapon);
				_pWorld->pIOnlineChar->SetEquipItem(uiUniqueID, (UI16*)uiEquipWeapon);

				_pWorld->pIOnlineChar->SetGuildData(uiUniqueID, (CHAR*)TempBuffer1, siGuildInGrade, siGuildFlag);
				_pWorld->pIOnlineChar->SetMercenary(uiUniqueID, siMercenaryKind);

				_pWorld->pOnlineInventory->SetMercenarySlot(0);

				_pWorld->pOnlineCharUI->SetAction(TRUE);

				_pWorld->pIOnlineChar->SetSiegeWarfare_Hp(uiUniqueID, siCurrentHP);
				_pWorld->pIOnlineChar->SetSiegeWarfare_CurrentSupplyGoodsAmount(uiUniqueID, siCurrentSupply);
				_pWorld->pIOnlineChar->SetCaptain(uiUniqueID, bCaptain);

				if(_pWorld->pMyData->GetMyCharUniqueID() != uiUniqueID)
				{
					// ������ ĳ������ �� ������ ���ٸ�
					if(_pWorld->pIOnlineChar->GetDataType(uiUniqueID) == 1)
					{
						if (_pWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
						{
							// �������� ��û�Ѵ�.
							_pWorld->pOnlineClient->SendPlayerDetailInfo(uiUniqueID);
							_pWorld->pIOnlineChar->SetDataType(uiUniqueID, 2);	// ���� �ٿ�ε� ��� ��...
						}
					}
				}
			}

			_pWorld->pIOnlineChar->AddOrder(pOnResFieldAttackRdyMsg->uiAttackCharID, ON_ORDER_FIELDATTACK_ACTION, siAttackCharX,siAttackCharY);

			//�̺κ��� ��� ó���Ұ������� ����.
			//���⸦ �� ������ �Ǵ��Ͽ� �¾����� ó���Ѵ�.������ ����ϵ� �ѵ�.�ϴ�.�����.
			if(pOnResFieldAttackRdyMsg->uiAttackCharID == _pWorld->pMyData->GetMyCharUniqueID())
			{
				// robypark 2004/11/2 15:38
				// ������ ���� ������ ���� ����Ʈ ����
				_pWorld->SiegeWarfare_CreateAttackEffectCharacter(pOnResFieldAttackRdyMsg->uiAttackCharID, pOnResFieldAttackRdyMsg->uiEnemyCharID, true);
			}
			else
			{
				// robypark 2004/11/2 15:38
				// ������ ���� ������ ���� ����Ʈ ����
				_pWorld->SiegeWarfare_CreateAttackEffectCharacter(pOnResFieldAttackRdyMsg->uiAttackCharID, pOnResFieldAttackRdyMsg->uiEnemyCharID);
			}		
			/*
				// robypark 2004/11/3 19:49
				// ����ȿ��(���ݷ� ��), �������� ���� ȿ�� ó�� �߰�
				SI16 scGeneralEffect = _pWorld->SiegeWarfare_IsInAreaGeneralEffect();
				if (-1 != scGeneralEffect)
					_pWorld->pOnlineClient->SendFieldAttackAction(pOnResFieldAttackRdyMsg->uiEnemyCharID, scGeneralEffect);
			*/
		}
	}
	else if(pOnResFieldAttackRdyMsg->dwResponse == ON_RET_NO)
	{
		// robypark 2005/1/12 19:30
		// ������ ���� ��� ���� �ʱ�ȭ
		_pWorld->CancelSiegeWarfare_AttackMode();

		_pWorld->pIOnlineChar->AddOrder(pOnResFieldAttackRdyMsg->uiAttackCharID, ON_ORDER_WAIT,0,0);
		_pWorld->pOnlineCharUI->SetAction(TRUE);
	}
	
	return 0;
}

UI16 OnlineMsgProc::FieldAttack_Action(BYTE* pMsg)
{
	OnResFieldAttackActionMsg*		pOnResFieldAttackActionMsg;

	pOnResFieldAttackActionMsg		=	(OnResFieldAttackActionMsg*)pMsg;

	//�¾������� ó��.���� ����ڿ��Ը� �̸޼����� ��Ÿ����.
	
	//_pWorld->pMyData->GetMyCharUniqueID() �ڽ��� ���̵�.

	pOnResFieldAttackActionMsg->uiShooterID;
	pOnResFieldAttackActionMsg->siDamage;
	pOnResFieldAttackActionMsg->bAlive;	// �����ڰ� ����ֳ�?

	//���� ������ �Ϲ� ĳ���ͷ� �ٲ�� ������ �����ϰ� 
	//������ ����鿡�� ���� �׾��ٴ°� �˸���.
	if(pOnResFieldAttackActionMsg->dwResponse == ON_RET_OK)
	{
		//�¾Ƽ� ���� ���ΰ��� �����.
		if(pOnResFieldAttackActionMsg->uiEnemyID == _pWorld->pMyData->GetMyCharUniqueID())
		{
			//������ ������.
			//_pWorld->pIOnlineChar->GetKind(_pWorld->pMyData->GetMyCharUniqueID()) �̰� �ƴ϶� PrevKind�� �������Ѵ�.

			if(_pWorld->pIOnlineChar->IsFieldAlive(_pWorld->pMyData->GetMyCharUniqueID()) == TRUE)
			{
				//_pWorld->pMyData->GetMyCharUniqueID() - pOnResFieldAttackActionMsg->siDamage > 0
				if((_pWorld->pIOnlineChar->GetSiegeWarfare_Hp(pOnResFieldAttackActionMsg->uiEnemyID) - pOnResFieldAttackActionMsg->siDamage) > 0)
				{
					_pWorld->pIOnlineChar->SetSiegeWarfare_Damage(pOnResFieldAttackActionMsg->uiEnemyID,pOnResFieldAttackActionMsg->siDamage);
				}
				else
				{
					_pWorld->pOnlineClient->SendFieldAttackDead(pOnResFieldAttackActionMsg->uiEnemyID);
					_pWorld->pOnlineCharUI->SetAction(TRUE);

					// ���� �Ұ��� ����
					_pWorld->pMyData->SetTransformSiegeWarfareUnit(false);

					// ������ ������ �ƴϴٷ� ����
					_pWorld->SetIsShowInformationAboutSiegeWarfareUnit(0);

					// robypark 2005/1/28 14:59
					// ������ ���� �������̽� �ݱ�
					_pWorld->pOnlinePannel->SiegeWarfareUnitUI_DestroyWindow();
				}
			}
		}
		else
		{
			if(_pWorld->pIOnlineChar->IsFieldAlive(pOnResFieldAttackActionMsg->uiEnemyID) == TRUE)
			{
				_pWorld->pIOnlineChar->SetSiegeWarfare_Damage(pOnResFieldAttackActionMsg->uiEnemyID, pOnResFieldAttackActionMsg->siDamage);
			}
		}

		// ĳ���� ������ ȿ�� ó��
		SI16 siHeroX, siHeroY;
		_pWorld->pIOnlineChar->GetPos(pOnResFieldAttackActionMsg->uiEnemyID, &siHeroX, &siHeroY);

		SEffectData					EffectData;
		EffectData.m_Type = EFFECTTYPE_FIELD_CHANGENUMBER;
		EffectData.m_EffectData_FieldChangeNumber.m_Frame     = 0;
		EffectData.m_EffectData_FieldChangeNumber.m_MapXPos   = siHeroX;
		EffectData.m_EffectData_FieldChangeNumber.m_MapYPos   = siHeroY;
		EffectData.m_EffectData_FieldChangeNumber.m_bVillageDefence	= false;
		EffectData.m_EffectData_FieldChangeNumber.m_ChangeNumber    = -pOnResFieldAttackActionMsg->siDamage;

		// �Ʊ� ����� ���� ��� �������� ����̴�
		if (_pWorld->IsMemberOfSameGuild( pOnResFieldAttackActionMsg->uiShooterID ))
		{
			EffectData.m_EffectData_FieldChangeNumber.m_Color	= EFFECTDATA_FIELD_CHANGENUMBER_COLOR_GREEN;

			_pWorld->pOnlineFieldEffect->AddEffect(EffectData);
		}
		// ���� ��ܿ��� ���
		else if (_pWorld->pIOnlineChar->GetIsHostilityGuild( pOnResFieldAttackActionMsg->uiShooterID ))
		{
			EffectData.m_EffectData_FieldChangeNumber.m_Color	= EFFECTDATA_FIELD_CHANGENUMBER_COLOR_RED;

			_pWorld->pOnlineFieldEffect->AddEffect(EffectData);
		}
		// else	// �� �� ��ܿ��� ���

		// robypark 2004/14:32
		// ġ���� ����, �߰��� ���, �Ϲ� ����, ����� ��� ȿ�� ���
		switch (pOnResFieldAttackActionMsg->siAttackType)
		{
			// ġ���� ����. ������ ĳ���Ϳ� ȿ�� �߰�
			case ON_RET_ATTACK_POWER:
				{
					// ������ ĳ���� ǥ�� ����Ʈ�� ġ���� ���� ��뿡�� ����Ѵ�.
					_pWorld->pIOnlineChar->SetNewSelectedFlag(pOnResFieldAttackActionMsg->uiShooterID, TRUE);
					_pWorld->pIOnlineChar->SetSiegeWarfare_DeathBlow(pOnResFieldAttackActionMsg->uiShooterID);
					PushEffect(EFFECT_CRITICAL, 0 , 0);	// ġ���� ���� ȿ���� ���
				}
				break;
			// �߰��� ���. �ǰ��� ĳ���Ϳ� ȿ�� �߰�
			case ON_RET_DEFENCE_POWER:
				{
					_pWorld->pIOnlineChar->SetSiegeWarfare_Evasion(pOnResFieldAttackActionMsg->uiEnemyID);
					PushEffect(EFFECT_DEFENCE, 0 , 0);	// �߰��� ��� ȿ���� ���
				}
				break;
			// �Ϲ� ����, ���
			case ON_RET_NONE_POWER:
			default:
				break;
		}
	}
//
//	else if(pOnResFieldAttackActionMsg->dwResponse == ON_RET_NO)
//	{
//		_pWorld->pIOnlineChar->AddOrder(pOnResFieldAttackRdyMsg->uiAttackCharID, ON_ORDER_WAIT,0,0);
//		_pWorld->pOnlineCharUI->SetAction(TRUE);
//	}
	return 0;
}	

UI16 OnlineMsgProc::FieldAttack_Dead(BYTE* pMsg)
{
	OnResFieldAttackDeadMsg*		pOnResFieldAttackDeadMsg;

	pOnResFieldAttackDeadMsg		=	(OnResFieldAttackDeadMsg*)pMsg;

	// �״� ���� ������ ������� ���
	if(pOnResFieldAttackDeadMsg->uiDeadCharID == _pWorld->pMyData->GetMyCharUniqueID())
	{
		// ������ ������ �ƴϴٷ� ����
		_pWorld->SetIsShowInformationAboutSiegeWarfareUnit(0);
	}

	//pOnResFieldAttackDeadMsg->uiDeadCharID �� �׾��ٴ°� �ѷ��ش�.
	//ó������� ���߿� ó���Ѵ�.
	//pOnResFieldAttackDeadMsg->uiShoorterID �� ���������� ���� ���ΰɷ� �ѷ��൵ �ǰ�..�ƴϸ� ���߿� ����.
	return 0;
}

// ��û���� ����̳� ����� ���� ������ ��û�� ���� �޽��� ó��
UI16 OnlineMsgProc::OnIncreaseGuildUnit(BYTE* pMsg)
{
//	OnResIncreaseGuildUnit*			pOnResIncreaseGuildUnit;

//	pOnResIncreaseGuildUnit			=	(OnResIncreaseGuildUnit*)pMsg;
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if (NULL == pStructure)
	{
		return 0;
	}

	// ���� �ǹ��� �� ���, �� ��û�� ��쿡�� ��û ���¸��� �������̽����� �޽��� ó��
	return pStructure->OnIncreaseGuildUnit(pMsg);
}

// ��ܿ� �ִ� ������ ���� �δ븦 ������ ���������� ��ȯ�� ���
UI16 OnlineMsgProc::OnDecreaseGuildUnit(BYTE* pMsg)
{
//	OnResDecreaseGuildUnit*			pOnResDecreaseGuildUnit;

//	pOnResDecreaseGuildUnit			=	(OnResDecreaseGuildUnit*)pMsg;

	return _pWorld->pOnlineTradeBook->OnDecreaseGuildUnit(pMsg);

	return 0;
}

// robypark 2004/11/16 15:36
// ������ ��� ���� ���� ����Ʈ ��û ����
// ON_RESPONSE_LIST_GUILD_UNIT
UI16 OnlineMsgProc::OnResponseSiegeWarfareUnitList(BYTE *pMsg)
{
	// OnResGetGuildUnitList	*pOnResGetGuildUnitList = (OnResGetGuildUnitList *)pMsg;
	// ���� �ǹ��� �� ���, �� ��û�� ��쿡�� ��û ���¸��� �������̽����� �޽��� ó��
	OnlineGovernMgr *pStructure = NULL;

	if (NULL != _pWorld->pOnlineVillage)
		pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	// ��û
	if (NULL != pStructure)
	{
		return pStructure->OnResponseSiegeWarfareUnitList(pMsg);
	}

	// ������� ����
	if (_pWorld->pOnlineTradeBook->IsActivityBook(ON_BOOK_SIEGEWARFARE))
	{
		// ��� �������̽����� �޽��� ó��
		_pWorld->pOnlineTradeBook->OnResponseSiegeWarfareUnitList(pMsg);
	}

	// ������ ���� ����
	_pWorld->pOnlinePannel->OnResponseSiegeWarfareUnitList(pMsg);

	return 0;
}

//-----------------------------------------------------------------------------
// Name: OnBroadCast_CurrentServerTime()
// Code: robypark (2004/9/14)
// Desc: ON_RESOPNSE_REAL_TIME ��ȣ ����. Server -> Client ��
//		���� ���� �ð��� �˷��ش�. ��, ��, �Ͽ� ���� �̺�Ʈ ����Ʈ�� Ȱ��ȭ
//		��Ȱ��ȭ���ش�.
//	pMsg	- �� �޽����� ������ ����( ���� ��, ��, �� ���� )
//	return	- ����� �Դٸ� 0
//-----------------------------------------------------------------------------
UI16 OnlineMsgProc::OnBroadCast_CurrentServerTime( BYTE *pMsg )
{
	// ���� �ð� ����ü�� ĳ����
	OnResRealTime *lpOnResRealTime = (OnResRealTime *)pMsg;

	// ������ ���
	char szHQNation[128];
	sprintf(szHQNation, "%s", (char*)pGame->m_pszHQNation);

	// ����Ʈ ���� ��ȭ���� �ݱ�
	if (TRUE == _pWorld->pOnlineQuestPlay->IsAction())
		_pWorld->pOnlineQuestPlay->SetAction(FALSE);

	// ����Ʈ ��� ��ȭ���� �ݱ�
	if (TRUE == _pWorld->m_pQuestInterface->IsAction())
		_pWorld->m_pQuestInterface->OnButtonPush();		

	// ����Ʈ �ļ� �ʱ�ȭ
	_pWorld->pQuestParser->Free();

	// �̺�Ʈ ����Ʈ�� ��ȿ������ ���� ��ó��(�ε�)
	_pWorld->pQuestParser->ReadText("Online\\Quest\\quest.txt", szHQNation, lpOnResRealTime->uiYear, lpOnResRealTime->uiMon, lpOnResRealTime->uiDay);
	
	// �̺�Ʈ �Ⱓ, ������ ���� �̺�Ʈ ����Ʈ�� �ִ� NPC Ȱ��ȭ, ��Ȱ��ȭ�� ���� ó��
	_pWorld->DisableEventNPC();

	_pWorld->m_pQuestInterface->SetQuestList();

	return 0;
}

// robypark 2005/1/29 14:25
// ���� �߿� ���޼����� �ڽ��� ����ȸ�� ��û ����
// ON_RESPONSE_CHARGE_MP
UI16	OnlineMsgProc::OnResponseChargeMP(BYTE *pMsg)
{
	OnResChargeMP	*pOnResChargeMP	= (OnResChargeMP *)pMsg;

	// ����
	if (ON_RET_OK == pOnResChargeMP->dwResponse)
	{
		SI32 siPrevMP = _pWorld->pIOnlineChar->GetSiegeWarfare_CurrentSupplyGoodsAmount(_pWorld->pMyData->GetMyCharUniqueID());

		// ���� ȸ���� �� ����
		_pWorld->pIOnlineChar->SetSiegeWarfare_CurrentSupplyGoodsAmount(_pWorld->pMyData->GetMyCharUniqueID(), pOnResChargeMP->siNowMP);

		/*
		// ��ȭ�� ������ �� �� ���
		SI16 siHeroX, siHeroY;
		_pWorld->pIOnlineChar->GetPos(_pWorld->pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);

		SEffectData					EffectData;
		EffectData.m_Type = EFFECTTYPE_FIELD_CHANGENUMBER;
		EffectData.m_EffectData_FieldChangeNumber.m_Frame     = 0;
		EffectData.m_EffectData_FieldChangeNumber.m_MapXPos   = siHeroX;
		EffectData.m_EffectData_FieldChangeNumber.m_MapYPos   = siHeroY;
		EffectData.m_EffectData_FieldChangeNumber.m_ChangeNumber    = pOnResChargeMP->siNowMP - siPrevMP;
		EffectData.m_EffectData_FieldChangeNumber.m_Color	= EFFECTDATA_FIELD_CHANGENUMBER_COLOR_GREEN;
		_pWorld->pOnlineFieldEffect->AddEffect(EffectData);
		*/
	}

	_pWorld->GetRecoveryManaInfo()->m_bSendedRequest = FALSE;

	return 0;
}

// robypark 2004/11/9 17:25
// ������ ���� ������ ���� ����ǰ �ޱ�
// ON_RESPONSE_GET_SUPPLY_AT_GUILD_UNIT
UI16 OnlineMsgProc::OnResponseSupplyGoodsFromSupplyWagon( BYTE *pMsg )
{
	OnResGetSupplyAtGuildUnit *pOnResGetSupplyAtGuildUnit = (OnResGetSupplyAtGuildUnit *)pMsg;

	// ġ�� ���� ĳ���Ͱ� ����� �ڽ��� ���
//	if (pOnResGetSupplyAtGuildUnit->uiUniqueID == _pWorld->pMyData->GetMyCharUniqueID())
	{
		// ���ο� ü�°� ����
		_pWorld->pIOnlineChar->SetSiegeWarfare_Hp(pOnResGetSupplyAtGuildUnit->uiTagetID, pOnResGetSupplyAtGuildUnit->siHP);

		PushEffect(EFFECT_HEAL, 0 , 0);	// ȿ���� ���
	}

	// ġ�� ȿ�� ���
	if ((_pWorld->IsMemberOfSameGuild( pOnResGetSupplyAtGuildUnit->uiTagetID ))		// �Ʊ� ����̰ų�
		|| (_pWorld->pIOnlineChar->GetIsHostilityGuild( pOnResGetSupplyAtGuildUnit->uiTagetID )))	// ���� ��ܿ��� ���
	{
		// ġ�� �޴� ����Ʈ �߰�
		_pWorld->pIOnlineChar->SetSiegeWarfare_EffectSupplyGoods(pOnResGetSupplyAtGuildUnit->uiTagetID, EFFECT_CONSTANT_SUPPLYGOODS_FILLED);

		// ġ�� ���� �� �� ���
		SI16 siHeroX, siHeroY;
		_pWorld->pIOnlineChar->GetPos(pOnResGetSupplyAtGuildUnit->uiTagetID, &siHeroX, &siHeroY);

		SEffectData					EffectData;
		EffectData.m_Type = EFFECTTYPE_FIELD_CHANGENUMBER;
		EffectData.m_EffectData_FieldChangeNumber.m_Frame     = 0;
		EffectData.m_EffectData_FieldChangeNumber.m_MapXPos   = siHeroX;
		EffectData.m_EffectData_FieldChangeNumber.m_MapYPos   = siHeroY;
		EffectData.m_EffectData_FieldChangeNumber.m_ChangeNumber    = pOnResGetSupplyAtGuildUnit->siHeAmount;
		EffectData.m_EffectData_FieldChangeNumber.m_Color	= EFFECTDATA_FIELD_CHANGENUMBER_COLOR_WHITE;
		_pWorld->pOnlineFieldEffect->AddEffect(EffectData);

		SI32 siPrevMP = _pWorld->pIOnlineChar->GetSiegeWarfare_CurrentSupplyGoodsAmount(pOnResGetSupplyAtGuildUnit->uiUniqueID);
	}

	// ���� ������ ����ǰ ������ ����
	_pWorld->pIOnlineChar->SetSiegeWarfare_CurrentSupplyGoodsAmount(pOnResGetSupplyAtGuildUnit->uiUniqueID, pOnResGetSupplyAtGuildUnit->siNowMP);

	/*
	// ���� ������ ����� �ڽ��� ���
	if (pOnResGetSupplyAtGuildUnit->uiUniqueID == _pWorld->pMyData->GetMyCharUniqueID())
	{
		// ��ȭ�� ������ �� �� ���
		SI16 siHeroX, siHeroY;
		_pWorld->pIOnlineChar->GetPos(pOnResGetSupplyAtGuildUnit->uiUniqueID, &siHeroX, &siHeroY);

		SEffectData					EffectData;
		EffectData.m_Type = EFFECTTYPE_FIELD_CHANGENUMBER;
		EffectData.m_EffectData_FieldChangeNumber.m_Frame     = 0;
		EffectData.m_EffectData_FieldChangeNumber.m_MapXPos   = siHeroX;
		EffectData.m_EffectData_FieldChangeNumber.m_MapYPos   = siHeroY;
		EffectData.m_EffectData_FieldChangeNumber.m_ChangeNumber    = pOnResGetSupplyAtGuildUnit->siNowMP - siPrevMP;
		EffectData.m_EffectData_FieldChangeNumber.m_Color	= EFFECTDATA_FIELD_CHANGENUMBER_COLOR_BLUE;
		_pWorld->pOnlineFieldEffect->AddEffect(EffectData);
	}
	*/
/*
	if (0 >= pOnResGetSupplyAtGuildUnit->siSupplyCharge)
	{
		// ���� ������ ���� ����ǰ �ٴڳ� ȿ�� �߰�(ran out)
	}
*/
	return 0;
}

// robypark 2004/11/9 18:46
// ������ ���� ������ �������� ����ǰ �ޱ�
// ON_RESPONSE_DECREASE_GUILD_SUPPLY
UI16 OnlineMsgProc::OnResponseSupplyGoodsFromVillage( BYTE *pMsg )
{
	OnResDecreaseGuildSupply	*pOnResDecreaseGuildSupply = (OnResDecreaseGuildSupply *)pMsg;

	if (ON_RET_OK == pOnResDecreaseGuildSupply->dwResponse)
	{
		// ���� ������ ����ǰ�� �޾Ҵٴ� ����Ʈ �߰�
		_pWorld->pIOnlineChar->SetSiegeWarfare_EffectSupplyGoods(_pWorld->pMyData->GetMyCharUniqueID(), EFFECT_CONSTANT_SUPPLYGOODS_FILLED);

		PushEffect(EFFECT_SUPPLY, 0 , 0);	// ȿ���� ���

		// ���� ������ ����ǰ ���� ����
		_pWorld->pIOnlineChar->SetSiegeWarfare_CurrentSupplyGoodsAmount(_pWorld->pMyData->GetMyCharUniqueID(), _pWorld->pIOnlineChar->GetSiegeWarfare_CurrentSupplyGoodsAmount(_pWorld->pMyData->GetMyCharUniqueID()) + pOnResDecreaseGuildSupply->siSupplyQuantity);

		_pWorld->pOnlineCharUI->SetAction(TRUE);
	}
	else
	{
		_pWorld->pIOnlineChar->AddOrder(_pWorld->pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT,0,0);
		_pWorld->pOnlineCharUI->SetAction(TRUE);
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", _pWorld->pOnlineText->Get(ON_TEXT_FAIL_TO_RECEIVE_SUPPLYGOODS));
	}

	return 0;
}

// robypark 2004/11/26 12:47
// ȭģ���� ���� ����
// ON_RESPONSE_GUILD_FRIENDLY_MESSAGE_ACCEPT
UI16 OnlineMsgProc::OnResponseGuildFriendlyMessage_Accept(BYTE *pMsg)
{
	_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", _pWorld->pOnlineText->Get(ON_TEXT_ACCEPT_OFFERED_PEACE_MESSAGE));

	// ��û���� ���� �ִ� ���
	OnlineGovernMgr *pStructure = NULL;

	if (NULL != _pWorld->pOnlineVillage)
		pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	// ��ܸ���Ʈ �����ϱ�

	return 0;
}

// robypark 2004/11/26 12:47
// ȭģ���� ���� ����
// ON_RESPONSE_GUILD_FRIENDLY_MESSAGE_CANCEL
UI16 OnlineMsgProc::OnResponseGuildFriendlyMessage_Cancel(BYTE *pMsg)
{
	_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", _pWorld->pOnlineText->Get(ON_TEXT_DECLINE_OFFERED_PEACE_MESSAGE));

	// ��ܸ���Ʈ �����ϱ�

	return 0;
}

// �����ϱ� �������̽��� ���� ��û�� ���� ���� ó��
// ON_RESPONSE_WAR_VILLAGE_DEFENCE
UI16	OnlineMsgProc::OnResponseEnterVillageDefenceInterface(BYTE *pMsg)
{
	_pWorld->OnResponseEnterVillageDefenceInterface(pMsg);

	return 0;
}

/*
// �����ϱ� �������̽����� ������ ���� ��û�� ���� ���� ó��
// ON_RESPONSE_WAR_VILLAGE_IN
UI16	OnlineMsgProc::OnResponseEnterVillageFromVillageDefenceInterface(BYTE *pMsg)
{
	_pWorld->OnResponseEnterVillageFromVillageDefenceInterface(pMsg);

	return 0;
}
*/

// �����ϱ� �������̽��� ���´�. �ʵ��
// ON_RESPONSE_VILLAGE_DEFFNCE_STOP
UI16	OnlineMsgProc::OnResponseLeaveVillageDefenceInterface(BYTE *pMsg)
{
	_pWorld->OnResponseLeaveVillageDefenceInterface(pMsg);

	return 0;
}

// �����ϱ� �������̽����� ���� �Ӽ� ���� �ʱ�ȭ ���� ���
// ON_RESPONSE_VULLAGE_ATTACK_TYPE_CHECK
UI16	OnlineMsgProc::OnResponseVullageAttackTypeCheck(BYTE *pMsg)
{
	_pWorld->OnResponseVullageAttackTypeCheck(pMsg);

	return 0;
}
