#include <GSL.h>

#include "OnlineGlobal.h"
#include "OnlineCharStatus.h"
#include "OnlineSmith.h"
#include "OnlineMsgProc.h"
#include "OnlineQuestInterface.h"
#include "OnlineTip.h"

// 공성 유닛 관리자
#include	"SiegeUnitTableMgr.h"

#include	"SiegeWarfareUnitUI.h"
#include	"SiegeWarfareTransformUIWnd.h"

extern _clGame* pGame;

// 이동중에 추가나 삭제할 캐릭터 정보.
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
				// 수성인터페이스에 있는 캐릭터인가를 설정
				_pWorld->pIOnlineChar->SetInVillageDefenceInterface(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bVillageInLimit);

				// 현재 수성하기 인터페이스에 있다고 설정
				_pWorld->pMyData->SetInVillageDefenceInterface(lpOnCharPlayerInfo->bVillageInLimit);

				// 길드 처리
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
				// 수성인터페이스에 있는 캐릭터인가를 설정
				_pWorld->pIOnlineChar->SetInVillageDefenceInterface(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bVillageInLimit);

				// 공성전 변신할 때 관련된 정보 헤더 얻기
				LPSSiegeUnitTable lpsSiegeUnitTable = _pWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTable(lpOnCharPlayerInfo->uiKind);

				// 공성전 유닛이 맞을 경우
				if (NULL != lpsSiegeUnitTable)
				{
					// 공성전 유닛 사용자 캐릭터의 상세 정보가 없다면
					if(_pWorld->pIOnlineChar->GetDataType(lpOnCharPlayerInfo->uiAccount) == 1)
					{
						if (_pWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
						{
							// 상세정보를 요청한다.
							_pWorld->pOnlineClient->SendPlayerDetailInfo(lpOnCharPlayerInfo->uiAccount);
							_pWorld->pIOnlineChar->SetDataType(lpOnCharPlayerInfo->uiAccount, 2);	// 정보 다운로드 대기 중...
						}
					}					
				}
			}

			_pWorld->pIOnlineChar->SetGuildData(lpOnCharPlayerInfo->uiAccount, _pWorld->pIOnlineChar->GetGuildName(lpOnCharPlayerInfo->uiAccount), lpOnCharPlayerInfo->siGuildInGrade, _pWorld->pIOnlineChar->GetGuildFlag(lpOnCharPlayerInfo->uiAccount));
			_pWorld->pIOnlineChar->SetMercenary(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->siVMercenaryKind);

			// 개인 상점
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

			// 다른 캐릭터의 대한 정보를 설정한다.						
			_pWorld->pIOnlineChar->Init(lpOnCharMonsterInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharMonsterInfo->uiKind, lpOnCharMonsterInfo->siX, lpOnCharMonsterInfo->siY, IP, 0.0f, lpOnCharMonsterInfo->siFightingPower, _pWorld,NULL);
			
			lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharMonsterInfo));
		}
		else if(ISSHIP(lpOnCharBaseInfo->uiAccount))
		{
			OnCharShipInfo*	lpOnCharShipInfo = (OnCharShipInfo*)lpOnCharBaseInfo;			

			// 다른 캐릭터의 대한 정보를 설정한다.						
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

// 캐릭터 제거
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
			// 좌판개설이라면
			_pWorld->pOnlineMap->m_MapBoothInfo.DelBoothData(pStoreHeader->siMapStoreCode);
			// 말풍선 없애기
			_pWorld->pIOnlineChar->SetSpeech(pDelCharMsg->uiCharID, 0);
		}
		_pWorld->pIOnlineChar->Free(pDelCharMsg->uiCharID);
	}	

	return 0;
}

// 캐릭터 추가
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
				// 수성인터페이스에 있는 캐릭터인가를 설정
				_pWorld->pIOnlineChar->SetInVillageDefenceInterface(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bVillageInLimit);
				_pWorld->pMyData->SetInVillageDefenceInterface(lpOnCharPlayerInfo->bVillageInLimit);

				// 길드 처리
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
				// 수성인터페이스에 있는 캐릭터인가를 설정
				_pWorld->pIOnlineChar->SetInVillageDefenceInterface(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bVillageInLimit);

				// 공성전 변신할 때 관련된 정보 헤더 얻기
				LPSSiegeUnitTable lpsSiegeUnitTable = _pWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTable(lpOnCharPlayerInfo->uiKind);

				// 공성전 유닛이 맞을 경우
				if (NULL != lpsSiegeUnitTable)
				{
					// 공성전 유닛 사용자 캐릭터의 상세 정보가 없다면
					if(_pWorld->pIOnlineChar->GetDataType(lpOnCharPlayerInfo->uiAccount) == 1)
					{
						if (_pWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
						{
							// 상세정보를 요청한다.
							_pWorld->pOnlineClient->SendPlayerDetailInfo(lpOnCharPlayerInfo->uiAccount);
							_pWorld->pIOnlineChar->SetDataType(lpOnCharPlayerInfo->uiAccount, 2);	// 정보 다운로드 대기 중...
							}
					}
				}
			}

			_pWorld->pIOnlineChar->SetGuildData(lpOnCharPlayerInfo->uiAccount, _pWorld->pIOnlineChar->GetGuildName(lpOnCharPlayerInfo->uiAccount), lpOnCharPlayerInfo->siGuildInGrade, _pWorld->pIOnlineChar->GetGuildFlag(lpOnCharPlayerInfo->uiAccount));
			_pWorld->pIOnlineChar->SetMercenary(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->siVMercenaryKind);

			// 개인 상점

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

			// 다른 캐릭터의 대한 정보를 설정한다.						
			_pWorld->pIOnlineChar->Init(lpOnCharMonsterInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharMonsterInfo->uiKind, lpOnCharMonsterInfo->siX, lpOnCharMonsterInfo->siY, IP, 0.0f, lpOnCharMonsterInfo->siFightingPower, _pWorld, NULL);
		}
		else if(ISSHIP(lpOnCharBaseInfo->uiAccount))
		{
			OnCharShipInfo *lpOnCharShipInfo = (OnCharShipInfo*)lpOnCharBaseInfo;

			// 다른 캐릭터의 대한 정보를 설정한다.						
			_pWorld->pIOnlineChar->Init(lpOnCharShipInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharShipInfo->uiKind, lpOnCharShipInfo->siX, lpOnCharShipInfo->siY, IP, 0.0f, 0, _pWorld, NULL);
		}
	}

	return 0;
}

// 다른 캐릭터의 위치를 받는다
UI16 OnlineMsgProc::OnResPosotion(BYTE *pMsg)
{
	OnCharMoveMsg*	pCharMoveMsg;
	UI16*			lpData;	

	pCharMoveMsg = (OnCharMoveMsg*)pMsg;	
	lpData       = (UI16*)((BYTE*)pCharMoveMsg + sizeof(OnCharMoveMsg));	

	
	// 우선 그 캐릭터가 있는지 보고 없으면 그 캐릭터의 정보를 달라고 요청한다.
	if(_pWorld->pIOnlineChar == NULL)		return 0;

	for(SI32 TempCount = 0; TempCount < pCharMoveMsg->uiMovedCharNum; TempCount++)
	{
		if(ISSHIP(lpData[0]) == TRUE)
		{
			// 배이다.
//					DWriteStringToFile("Ship X : [%d], Y : [%d]", pCharMoveMsg->uiX, pCharMoveMsg->uiY);
		}

		if(_pWorld->pIOnlineChar->IsExistUniqueID(lpData[0])  == TRUE)
		{	
			// 우선 위치가 너무 많은 차이가 나서 워프를 해야 하는 상황인지 본다.
			// 단 내 캐릭터가 아니여야 한다.
			if(_pWorld->pIOnlineChar->IsWarp(lpData[0], lpData[1], lpData[2],ON_CHAR_WAPR_DISTANCE) == TRUE)
			{
				if(lpData[0] != _pWorld->pMyData->GetMyCharUniqueID())
				{
					_pWorld->pIOnlineChar->Warp(lpData[0], lpData[1], lpData[2]);
				}
			} 
			else
			{
				// 사용자를 그 좌표로 이동 시킨다.			
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
			// 현재는 무시
			/*
			// 캐릭터의 정보를 달라고 요청한다.
			// 우선 주어진 정보를 캐릭터를 초기화 한다.
			pIOnlineChar->Init(pCharMoveMsg->uiCharID, "", pCharMoveMsg->uiKind, SI16(pCharMoveMsg->uiX), SI16(pCharMoveMsg->uiY), 0, (float)pCharMoveMsg->uiRank, pCharMoveMsg->siFightPower, this);
			
			// 만약 플레이어라면 플레이어의 정보를 요구한다.			
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

// 마을로 들어 간다.
UI16 OnlineMsgProc::OnResEnterVillage(BYTE *pMsg)
{
	OnResponseMsg *pResponseMsg = (OnResponseMsg*)pMsg;

	if(pResponseMsg == NULL)	return 0;
	
	if(pResponseMsg->dwResponse == ON_RET_OK)
	{
		UI16 uiVillageCode;
		OnResGotoVillageOkMsg *pGotoVillageOkMsg = (OnResGotoVillageOkMsg*)pResponseMsg;	
		
		// 캐릭터가 마을에 들어갔다. 마을에 들어가 캐리터가 자기 캐럭터이라면 자기가 가지고 있는 모든 캐릭터의 대한 정보를 날린다.
		// 그렇지 않고 다른 캐릭터이라면 그 캐릭터만 날린다.
		if(pGotoVillageOkMsg->uiCharID == _pWorld->pMyData->GetMyCharUniqueID())
		{
			// 내 캐릭터이다.
			// 인구 처리
			OnRecvVillageInfo*		lpOnRecvVillageInfo = (OnRecvVillageInfo*)((UI08*)(pGotoVillageOkMsg) + sizeof(OnResGotoVillageOkMsg));

			// 서버로 부터 마을에 들어갈 수 있또록 허가를 받았다.

			// 빌리지 객체를 생성한다.
			_pWorld->pOnlineVillage	=	new OnlineVillage;
			
			// 마을로 이동해야 한다.
			uiVillageCode	=	_pWorld->pMyData->GetGotoVillage();						
			
			_pWorld->pOnlineVillage->Init(uiVillageCode, lpOnRecvVillageInfo, _pWorld);
			_pWorld->SetGamePollStatus( ON_GAMEPOLL_VILLAGE );
			
			// 모든 캐릭터들을 없앤다.
			_pWorld->pIOnlineChar->AllFree();
			
			// 오브젝트를 모두 해제한다.
			_pWorld->pIOnlineObject->FreeAll();

			// 더 이상 서버로 부터 마을에 들어간다는 메시지를 기다리지 않도록 설정한다.
			_pWorld->pMyData->SetWaitGotoVillageResponse(FALSE);		
			
			// 마을에 들어간다는 변수를 초기화 한다.
			_pWorld->pMyData->SetGotoVillage(FALSE, 0);			

			_pWorld->pOnlineClientChatting->MessageSend_NotIsMap();

			if(_pWorld->m_VillageInStruct != -1)
			{
				_pWorld->pOnlineVillage->MoveInStruct(_pWorld->GetVillageInStruct(), TRUE, TRUE);
			}
		}
		else
		{
			// 다른 캐릭터이다.
			_pWorld->pIOnlineChar->Free(pGotoVillageOkMsg->uiCharID);
		}		
	}
	else if(pResponseMsg->dwResponse == ON_RET_NO)
	{
		// 마을에 들어갈 수가 없다.
		_pWorld->pMyData->SetWaitGotoVillageResponse(FALSE);		
		_pWorld->pMyData->SetGotoVillage(FALSE, 0);
		_pWorld->pOnlineCharUI->SetAction(TRUE);
	}
	else if(pResponseMsg->dwResponse == ON_RET_ENEMY_GUILD_VILLAGE)
	{
		// 마을에 들어갈 수가 없다.(적길드의 마을이라서)
		_pWorld->pMyData->SetWaitGotoVillageResponse(FALSE);		
		_pWorld->pMyData->SetGotoVillage(FALSE, 0);
		_pWorld->pOnlineCharUI->SetAction(TRUE);
		_pWorld->pIOnlineChar->SetSpeech(_pWorld->pMyData->GetMyCharUniqueID(), _pWorld->pOnlineText->Get(ON_TEXT_ENEMYVILLAGE));
		PushEffect(EFFECT_CANNOTENTERVILLAGE, 0, 0);
	}

	return 0;
}

// 포탈로 이동
UI16 OnlineMsgProc::OnResMoveMap(BYTE *pMsg)
{
	// 거래 중이라면 닫아 준다.
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
		
		// 모든 캐릭터를 초기화 한다.
		_pWorld->pIOnlineChar->AllFree();
		siCurMapIndex = _pWorld->pOnlineMap->m_MapIndex;

		// 맵 전환
		OnCharBaseInfo		*lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnResMoveMapOkMsg + sizeof(OnResMoveMapOkMsg));		
		OnCharMonsterInfo	*lpOnCharMonsterInfo;
		OnCharShipInfo		*lpOnCharShipInfo;
		OnCharPlayerInfo	*lpOnCharPlayerInfo;
		
		for(TempCount = 0; TempCount < lpOnResMoveMapOkMsg->uiAroundCharNum; TempCount++)
		{
			// 저자거리에서 한양으로 나올때 하트비트 보냄
			if(TempCount == (SI16)(lpOnResMoveMapOkMsg->uiAroundCharNum / 2) && siCurMapIndex == 2)
				_pWorld->SendHeartBeat();

			if(ISPLAYER(lpOnCharBaseInfo->uiAccount))
			{
				lpOnCharPlayerInfo = (OnCharPlayerInfo*)lpOnCharBaseInfo;

				if(_pWorld->pMyData->GetMyCharUniqueID() == lpOnCharPlayerInfo->uiAccount)
				{					
					// 주인공 캐릭터의 위치를 기준으로 맵을 설정한다.				
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
		
		// 캐릭터 추가
		lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnResMoveMapOkMsg + sizeof(OnResMoveMapOkMsg));

		CHAR					TempBuffer[128];

		for(TempCount = 0; TempCount < lpOnResMoveMapOkMsg->uiAroundCharNum; TempCount++)
		{
			// 저자거리에서 한양으로 나올때 하트비트 보냄
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
					// 수성인터페이스에 있는 캐릭터인가를 설정
					_pWorld->pIOnlineChar->SetInVillageDefenceInterface(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bVillageInLimit);
					_pWorld->pMyData->SetInVillageDefenceInterface(lpOnCharPlayerInfo->bVillageInLimit);

					_pWorld->pIOnlineChar->SetRank(_pWorld->pMyData->GetMyCharUniqueID(), _pWorld->pMyData->nRank);
					_pWorld->pIOnlineChar->SetScore(_pWorld->pMyData->GetMyCharUniqueID(), _pWorld->pMyData->nScore);

					// 길드 처리
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
					// 수성인터페이스에 있는 캐릭터인가를 설정
					_pWorld->pIOnlineChar->SetInVillageDefenceInterface(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bVillageInLimit);

					// 공성전 변신할 때 관련된 아이템 정보 헤더 얻기
					LPSSiegeUnitTable lpsSiegeUnitTable = _pWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTable(lpOnCharPlayerInfo->uiKind);

					// 공성전 유닛이 맞을 경우
					if (NULL != lpsSiegeUnitTable)
					{
						// 공성전 유닛 사용자 캐릭터의 상세 정보가 없다면
						if(_pWorld->pIOnlineChar->GetDataType(lpOnCharPlayerInfo->uiAccount) == 1)
						{
							if (_pWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
							{
								// 상세정보를 요청한다.
								_pWorld->pOnlineClient->SendPlayerDetailInfo(lpOnCharPlayerInfo->uiAccount);
								_pWorld->pIOnlineChar->SetDataType(lpOnCharPlayerInfo->uiAccount, 2);	// 정보 다운로드 대기 중...
							}
						}
					}
				}				

				_pWorld->pIOnlineChar->SetGuildData(lpOnCharPlayerInfo->uiAccount, _pWorld->pIOnlineChar->GetGuildName(lpOnCharPlayerInfo->uiAccount), lpOnCharPlayerInfo->siGuildInGrade, _pWorld->pIOnlineChar->GetGuildFlag(lpOnCharPlayerInfo->uiAccount));
				_pWorld->pIOnlineChar->SetMercenary(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->siVMercenaryKind);

				// 개인 상점
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

				// 다른 캐릭터의 대한 정보를 설정한다.						
				_pWorld->pIOnlineChar->Init(lpOnCharMonsterInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharMonsterInfo->uiKind, lpOnCharMonsterInfo->siX, lpOnCharMonsterInfo->siY, IP, 0.0f, lpOnCharMonsterInfo->siFightingPower, _pWorld,NULL);
				
				lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharMonsterInfo));
			}
			else if(ISSHIP(lpOnCharBaseInfo->uiAccount))
			{
				lpOnCharShipInfo = (OnCharShipInfo*)lpOnCharBaseInfo;

				// 다른 캐릭터의 대한 정보를 설정한다.						
				_pWorld->pIOnlineChar->Init(lpOnCharShipInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharShipInfo->uiKind, lpOnCharShipInfo->siX, lpOnCharShipInfo->siY, IP, 0.0f, 0, _pWorld,NULL);

				lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharShipInfo));
			}
		}
		
		// 모든 Item 초기화
		pReceivedFieldItemInfo	=	(OnlineReceiveFieldItemInfo*)((CHAR*)lpOnCharBaseInfo);		
		for(TempCount = 0; TempCount < lpOnResMoveMapOkMsg->uiAroundItemNum; TempCount++)
		{
			// 필드에 아이템을 설정한다.
			_pWorld->pOnlineMap->SetID(pReceivedFieldItemInfo[TempCount].siX, pReceivedFieldItemInfo[TempCount].siY, 1, 1, 0, 0, pReceivedFieldItemInfo[TempCount].uiItemID, pReceivedFieldItemInfo[TempCount].uiQuantity);
		}
		
		// 새로운 돈 지정
		_pWorld->pMyData->SetMoney(lpOnResMoveMapOkMsg->mnMoney);
		
		// Game 을 진행하게 함
		_pWorld->pMyData->SetGotoPortal(FALSE, 0);
		_pWorld->pMyData->SetWaitGotoPortalResponse(FALSE);		
		_pWorld->pOnlineCharUI->SetAction(TRUE);

		// Warp Effect 지정
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
		// 포탈에 들어갈 수가 없다.
		_pWorld->pMyData->SetGotoPortal(FALSE, 0);
		_pWorld->pMyData->SetWaitGotoPortalResponse(FALSE);		
		_pWorld->pOnlineCharUI->SetAction(TRUE);
	}

	return 0;
}

// 플레이어 정보를 받는다.
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
		// 플레이어 정보를 받았다고 설정한다.
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
					// 수성인터페이스에 있는 캐릭터인가를 설정
					_pWorld->pIOnlineChar->SetInVillageDefenceInterface(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bVillageInLimit);
					_pWorld->pMyData->SetInVillageDefenceInterface(lpOnCharPlayerInfo->bVillageInLimit);

					// 길드 처리
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
					// 수성인터페이스에 있는 캐릭터인가를 설정
					_pWorld->pIOnlineChar->SetInVillageDefenceInterface(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bVillageInLimit);

					// 공성전 변신할 때 관련된 아이템 정보 헤더 얻기
					LPSSiegeUnitTable lpsSiegeUnitTable = _pWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTable(lpOnCharPlayerInfo->uiKind);

					// 공성전 유닛이 맞을 경우
					if (NULL != lpsSiegeUnitTable)
					{
						// 공성전 유닛 사용자 캐릭터의 상세 정보가 없다면
						if(_pWorld->pIOnlineChar->GetDataType(lpOnCharPlayerInfo->uiAccount) == 1)
						{
							if (_pWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
							{
								// 상세정보를 요청한다.
								_pWorld->pOnlineClient->SendPlayerDetailInfo(lpOnCharPlayerInfo->uiAccount);
								_pWorld->pIOnlineChar->SetDataType(lpOnCharPlayerInfo->uiAccount, 2);	// 정보 다운로드 대기 중...
							}
						}
					}
				}
			}

			_pWorld->pIOnlineChar->SetGuildData(lpOnCharPlayerInfo->uiAccount, _pWorld->pIOnlineChar->GetGuildName(lpOnCharPlayerInfo->uiAccount), lpOnCharPlayerInfo->siGuildInGrade, _pWorld->pIOnlineChar->GetGuildFlag(lpOnCharPlayerInfo->uiAccount));
			_pWorld->pIOnlineChar->SetMercenary(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->siVMercenaryKind);

			// 개인 상점
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

			// 다른 캐릭터의 대한 정보를 설정한다.						
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

			// 다른 캐릭터의 대한 정보를 설정한다.						
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
		
		// 초기화를 한다.
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
		
		// 나의 고유 ID를 저장한다.
		GOnlineImjin2I.BattleInfo.siMyCharUniqueID	=	pMyData->GetMyCharUniqueID();
		
		// 전투 서버의 IP를 저장한다.
		pIP											=	(CHAR*)&pBattleStartMsg->dwBattleServerIP;
		
		wsprintf(GOnlineImjin2I.BattleInfo.szBattleServerIP, "%u.%u.%u.%u", UI08(pIP[0]), UI08(pIP[1]), UI08(pIP[2]), UI08(pIP[3]));
		
		// 전투 서버의 채널을 저장한다.
		GOnlineImjin2I.BattleInfo.siBattleServerChannel		=	pBattleStartMsg->uiBattleServerChannelNum;
		GOnlineImjin2I.BattleInfo.uiRandomSeed              =   pBattleStartMsg->uiRandomSeed;
		*/
		
		_pWorld->m_BattleAfterMiniMapFlag = _pWorld->pOnlineMiniMap->IsActive();

		_pWorld->pOnlineClientChatting->MessageSend_NotIsMap();

		// 거래 중이라면 닫아 준다.
		_pWorld->pOnlineTrade->m_pMsgBox->Initialize();


		// 전투가 시작되었다.
		return ON_MAINPOLL_INITGOTOIMJINGAME;

		/*uiNextMenu	=	ON_MAINPOLL_ENTERBATTLEEFFECT;	
		m_siUpdateMethod = 1 + (rand() % 2);
		m_siUpdateCounter = 0;*/
		
	}
	else if(pBattleMsg->subcommand == ON_RET_BATTLE_NO)
	{
		// 전투가 취소되었다.
		_pWorld->pMyData->SetWaitBattleResponse(FALSE);
		_pWorld->pOnlineCharUI->SetAction(TRUE);
	}
	else if(pBattleMsg->subcommand == ON_RET_BATTLE_WAIT)
	{
		// 전투를 대기하라는 메시지가 왔다.
		
		// 그 자리에 정지한다.
		_pWorld->pIOnlineChar->AddOrder(_pWorld->pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);			
		
		// 사용자의 명령을 받지 못하는 상태로 놔둔다.
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
		// 그 Character를 찾아서 Flag를 셋팅
		_pWorld->pIOnlineChar->SetBattleFlag(lpOnResBattlePlayer->uiUniqueID, lpOnResBattlePlayer->fFlag);

		// 자신이거나 플레이어가 아님 사운드를 출력
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
					// 캐릭터에게 정지 명령을 내려준다.
					_pWorld->pIOnlineChar->AddOrder(_pWorld->pMyData->GetMyCharUniqueID(), ON_ORDER_WAIT, 0, 0);
	
					// 더 이상의 사용자 입력을 받지 않는다.
					_pWorld->pOnlineCharUI->SetAction(FALSE);
				}
			}
		}
	}

	return 0;
}

// 의복을 바꺼 입는다.
UI16 OnlineMsgProc::OnChangeChlothes(BYTE *pMsg)
{
	OnChangeClothesMsg*	lpOnChangeClothesMsg = (OnChangeClothesMsg*)pMsg;	
	_pWorld->pIOnlineChar->SetEquipItem(lpOnChangeClothesMsg->uiCharacID, lpOnChangeClothesMsg->uiItemID);
	return 0;
}

// 마을 안내 문을 읽는다.
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

// 마을의 국적을 얻어 온다.
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
		, lpOnResVillageGetNation->siDefenceSoldier	// 현재 수비병 부대수
		, lpOnResVillageGetNation->siDefenceSoldier/* lpOnResVillageGetNation->siMaxDefenceSoldier*/	// 최대 수비병 부대수
		, lpOnResVillageGetNation->siAttackType	// 마을 공격 타입
		);

	if(_pWorld->pOnlineMap->m_MapInfo.uiVillageCode)
	{
		_pWorld->pOnlineClient->SendRequestVillageInfo(_pWorld->pOnlineMap->m_MapInfo.uiVillageCode);
	}

	return 0;
}

// 이모티콘.
UI16 OnlineMsgProc::OnResImoticon(BYTE *pMsg)
{
	OnResImoticonMsg *lpOnResImoticonMsg = (OnResImoticonMsg*)pMsg;
	_pWorld->pIOnlineChar->SetImoticon(lpOnResImoticonMsg->UserID, lpOnResImoticonMsg->uiImoticonID);
	return 0;
}


// 거래 허락
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

/*					// 상대 방의 아이피를 너어 준다.					
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
			//000가 파티에 참여하였습니다.
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
			//000가 파티를 거절하였습니다.

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
			//다시 응답을 기다리는 메세지가 나와야 하는지?
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
			//000가 파티에 참여하였습니다.

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

// 거래 요청
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

// 파티를 요청한후 메세지를 받아.'파티 신청중입니다'라는 메세지를 뿌린다.
UI16 OnlineMsgProc::OnResParty(BYTE *pMsg)
{
	OnResponseMsg *pOnResponseMsg = (OnResponseMsg*)pMsg;
	CHAR		   pName[ON_ID_LENGTH + 1];
	ZeroMemory(pName, sizeof(pName));

	if(pOnResponseMsg)
	{
		if(pOnResponseMsg->dwResponse == ON_RET_OK)
		{
			//성공
			//파티 신청중입니다. 메세지를 뿌린다.
			_pWorld->pOnlineParty->m_pMsgBox->SetMsg( ON_MSGBOX_TYPE_CANCEL, 
			"", _pWorld->pOnlineText->Get(2275) );

		}
		else if(pOnResponseMsg->dwResponse == ON_RET_NO)
		{
			
			//실패.
			//파티원이 꽉찬 경우.
			/*
			if(pOnResPartyMsg->dwErrorCode == ON_RET_PARTY_FULL)
				_pWorld->pOnlineParty->m_pMsgBox->SetMsg( ON_MSGBOX_TYPE_CANCEL, 
				"", "한 파티에 7명 이상 참여할수 없습니다.", pName );
			//다른 파티에 가입되어 있는 경우.
			else if(pOnResPartyMsg->dwErrorCode == ON_RET_PARTY_DIFPARTY)
				_pWorld->pOnlineParty->m_pMsgBox->SetMsg( ON_MSGBOX_TYPE_CANCEL, 
				"", "%s 는 다른 파티에 가입되어 있는 상태입니다.", pName );
			//파티장이 아닌경우.
			else if(pOnResPartyMsg->dwErrorCode ==ON_RET_NO_PARTY_CAP)
				_pWorld->pOnlineParty->m_pMsgBox->SetMsg( ON_MSGBOX_TYPE_CANCEL, 
				"", "파티장만이 파티를 신청할수 있습니다", pName );
			//다른 원인인 경우.
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

	// 거래허용모드가 아닐경우 취소메세지 보냄
	if(!_pWorld->pOnlineTrade->m_bPermitTrade)
	{
		_pWorld->pOnlineClient->SendRequestParty(ON_RET_NO);
		_pWorld->bCharMove = TRUE;
		return 0;
	}

	sprintf(pName,pOnReceiveAskPartyMsg->szID);

	if(pName[0] != 0)
	{
		//000가 파티를 신청하였습니다.파티에 참여 하시겠습니까?
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

// 상대방에게 거래 요청한것을 알려 준다.
UI16 OnlineMsgProc::OnResAksTrade(BYTE *pMsg)
{
	OnReceiveAskTradeMsg	*pOnReceiveAskTradeMsg;
	CHAR					pName[ON_ID_LENGTH + 1];
	ZeroMemory(pName, sizeof(pName));
	
	pOnReceiveAskTradeMsg	=	(OnReceiveAskTradeMsg*)pMsg;
	
	// 거래허용모드가 아닐경우 취소메세지 보냄
	if(!_pWorld->pOnlineTrade->m_bPermitTrade)
	{
		_pWorld->pOnlineClient->SendRequestTrade(FALSE);
		return 0;
	}

	// 경매중이면 거래요청을 씹는다.
	if(_pWorld->pOnlineFieldAuction->IsEntered())
	{
		_pWorld->pOnlineClient->SendRequestTrade(FALSE);					
		return 0;
	}

	strcpy(pName, pOnReceiveAskTradeMsg->szID);
	// 사용자가가 널이 아닐때만 거래 가능
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

// 거래 취소
UI16 OnlineMsgProc::OnResCancelTrade(BYTE *pMsg)
{	
	_pWorld->pOnlineTrade->m_pMsgBox->Initialize();
	_pWorld->pOnlineTrade->m_bRecvTrade = FALSE;

	// 현재 열려진 숫자판 컨트롤이 열려 있다면 닫는다. (취소버튼 클릭과 동일한 처리)
	if (_pWorld->pOnlineNumPad->pNumberControl->IsShow())
		_pWorld->pOnlineNumPad->pNumberControl->CloseNumberControl();

	_pWorld->pOnlineTrade->SetActive( FALSE );
	_pWorld->pOnlineInventory->FillItemListBox();

	return 0;
}

// 거래 버튼 누름
UI16 OnlineMsgProc::OnResFinishTrade(BYTE *pMsg)
{
	OnResFinishChooseTradeItemMsg *pOnResFinishChooseTradeItemMsg =	(OnResFinishChooseTradeItemMsg*)pMsg;

	if(!pOnResFinishChooseTradeItemMsg->bIsMe)
		_pWorld->pOnlineTrade->SetOkButton( pOnResFinishChooseTradeItemMsg->bFinishChooseItem );

	return 0;
}

// 거래 하는것을 수락
UI16 OnlineMsgProc::OnResDecideTrade(BYTE *pMsg)
{
	OnResDecideTradeMsg	*pOnResDecideTradeMsg = (OnResDecideTradeMsg*)pMsg;

	if(pOnResDecideTradeMsg->dwResponse == ON_RET_OK)						// 수락 버튼을 누를 수가 아따.
	{					
		_pWorld->pOnlineTrade->SetPushAccept(pOnResDecideTradeMsg->bIsMe);
	}
	else																	// 거래 실패
	{					
		_pWorld->pOnlineTrade->Success(FALSE);
	}

	return 0;
}

// 거래 성공
UI16 OnlineMsgProc::OnCompleteTrade(BYTE *pMsg)
{
	OnCompleteTradeMsg	*pOnCompleteTradeMsg = (OnCompleteTradeMsg*)pMsg;

	// 보내는 아이템, 용병 삭제
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
		// 장착 아이템 설정
		_pWorld->pMyData->InitFollower(pFollowerInfo->uiSlot, pFollowerInfo);						
		
		// 인벤토리 아이템 설정
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

	// 거래 아이템
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
		
		// 받는 아이템 삽입
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

// 거래 물품 변경
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

// 마을 공격
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
		// 만약에 공격한 캐릭터 Kind가 서버와 클라이언트에서 보여지는 캐릭터Kind가 다른 경우에 처리
		if (pOnResAttackVillageMsg->uiKind != _pWorld->pIOnlineChar->GetKind(pOnResAttackVillageMsg->uiCharID))
		{
			DWORD		IP;
			CHAR		TempBuffer[128];

			// 캐릭 정보 얻기
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

			// 변신하는 캐릭터가 사용자 자신이 아닌 경우
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
				// 변신한 캐릭터의 상세 정보가 없다면
				if(_pWorld->pIOnlineChar->GetDataType(uiUniqueID) == 1)
				{
					if (_pWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
					{
						// 상세정보를 요청한다.
						_pWorld->pOnlineClient->SendPlayerDetailInfo(uiUniqueID);
						_pWorld->pIOnlineChar->SetDataType(uiUniqueID, 2);	// 정보 다운로드 대기 중...
					}
				}
			}
		}
	}

	// 원거리 공격인 경우 캐릭터 공격 동작 명령 필요없슴
	if (FALSE == pOnResAttackVillageMsg->bIsLong)
	{
		// 캐릭터 공격 동작
		// 만약 자기이면 무시
		if(pOnResAttackVillageMsg->uiCharID != _pWorld->pMyData->GetMyCharUniqueID())
		{
			_pWorld->pIOnlineChar->AddOrder(pOnResAttackVillageMsg->uiCharID, ON_ORDER_SHORTDISTANCE_ATTACKVILLAGEACTION
				, lpVillageHeader->siX, lpVillageHeader->siY);
		}
	}

	// 공격 위치 검사
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

	// 마을 데미지 효과 처리
	EffectData.m_Type = EFFECTTYPE_FIELD_CHANGENUMBER;
	EffectData.m_EffectData_FieldChangeNumber.m_Frame     = 0;
	EffectData.m_EffectData_FieldChangeNumber.m_MapXPos   = TempXPos;
	EffectData.m_EffectData_FieldChangeNumber.m_MapYPos   = TempYPos;
	EffectData.m_EffectData_FieldChangeNumber.m_bVillageDefence	= true;
	EffectData.m_EffectData_FieldChangeNumber.m_ChangeNumber    = -pOnResAttackVillageMsg->siVillageDefenceDamage;

	// 아군 상단이 때린 경우 데미지는 녹색이다
	if (_pWorld->IsMemberOfSameGuild( pOnResAttackVillageMsg->uiCharID ))
	{
		EffectData.m_EffectData_FieldChangeNumber.m_Color	= EFFECTDATA_FIELD_CHANGENUMBER_COLOR_GREEN;

		_pWorld->pOnlineFieldEffect->AddEffect(EffectData);
	}
	// 적군 상단원일 경우
	else if (_pWorld->pIOnlineChar->GetIsHostilityGuild( pOnResAttackVillageMsg->uiCharID ))
	{
		EffectData.m_EffectData_FieldChangeNumber.m_Color	= EFFECTDATA_FIELD_CHANGENUMBER_COLOR_RED;

		_pWorld->pOnlineFieldEffect->AddEffect(EffectData);
	}
	// else	// 그 외 상단원인 경우

	// 원거리 공격인 경우 캐릭터 공격 이펙트 생성 필요 없슴
	if (FALSE == pOnResAttackVillageMsg->bIsLong)
	{
		// robypark 2004/11/3 14:18
		// 공성전 유닛 사이의 공격 이펙트 생성
		// ActionMsg를 서버에 전송할 필요가 없다. 이미 처리가 되었으므로.
		_pWorld->SiegeWarfare_CreateAttackEffectVillage(pOnResAttackVillageMsg->uiCharID, TempXPos, TempYPos, 0);
	}

	// 마을의 정보를 변경 마을 방어력 및 수비병 수 수정
	lpSVillageData = _pWorld->pOnlineVillageDescriptList->GetVillageDataData(lpVillageHeader->uiVillageCode);				
	if(lpSVillageData)
	{
		if(lpSVillageData->m_fWait == FALSE)
		{
			if(pOnResAttackVillageMsg->vdDefence <= 0)
			{
				// 마을의 길드가 없어진 경우
				_pWorld->pOnlineVillageDescriptList->DelVillageDataData(lpVillageHeader->uiVillageCode);

				if(_pWorld->pOnlineMap->m_MapInfo.uiVillageCode)
				{
					_pWorld->pOnlineClient->SendRequestVillageInfo(_pWorld->pOnlineMap->m_MapInfo.uiVillageCode);
				}
			}
			else
			{
				lpSVillageData->m_vdChangedDefence    = pOnResAttackVillageMsg->vdDefence;	// 마을 방어력

				// 마을 방어력 최대값보다 큰 경우
				if(lpSVillageData->m_vdChangedMaxDefence < lpSVillageData->m_vdChangedDefence)
				{
					lpSVillageData->m_vdChangedMaxDefence = lpSVillageData->m_vdChangedDefence;
				}
			}
		}
	}
	
	// robypark 2004/14:32
	// 치명적 공격, 견고한 방어, 일반 공격, 방어일 경우 효과 출력
	switch (pOnResAttackVillageMsg->siAttackType)
	{
		// 치명적 공격. 공격자 캐릭터에 효과 추가
		case ON_RET_ATTACK_POWER:
			{				
				// 선택한 캐릭터 표시 이펙트를 치명적 공격 사용에도 사용한다.
				_pWorld->pIOnlineChar->SetNewSelectedFlag(pOnResAttackVillageMsg->uiCharID, TRUE);
				_pWorld->pIOnlineChar->SetSiegeWarfare_DeathBlow(pOnResAttackVillageMsg->uiCharID);
//				_pWorld->pIOnlineChar->SetSpeech(pOnResAttackVillageMsg->uiCharID, "Deathblow!!!");
				PushEffect(EFFECT_CRITICAL, 0 , 0);	// 치명적 공격 효과음 출력
			}
			break;
		// 견고한 방어. 피격자 캐릭터에 효과 추가
		case ON_RET_DEFENCE_POWER:
			{				
				_pWorld->pIOnlineChar->SetSiegeWarfare_Evasion(pOnResAttackVillageMsg->uiCharID);
				PushEffect(EFFECT_DEFENCE, 0 , 0);	// 견고한 방어 효과음 출력
			}
			break;
		// 일반 공격, 방어
		case ON_RET_NONE_POWER:
		default:
				//_pWorld->pIOnlineChar->SetSpeech(pOnResAttackVillageMsg->uiCharID, "Hit!!!");
			break;
	}

	// 마을 폭발시 (마을 방어력)
	if(lpSVillageData)
	{
		if(lpSVillageData->m_fWait == FALSE)
		{
			// 현재 방어력이 0이 된 경우 폭발 이펙트 사용.
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

				// 마을이 점령되었을 효과 뿌리기
				TempXPos = lpVillageHeader->siX;
				TempYPos = lpVillageHeader->siY;

				TempSEffectData.m_fCreate = TRUE;
				TempSEffectData.m_Type    = EFFECTTYPE_OCCUPIED_VILLAGE;
				TempSEffectData.m_EffectData_OccupiedVillage.m_Frame = 0;

				TempSEffectData.m_EffectData_OccupiedVillage.m_XPos  = TempXPos + 1;
				TempSEffectData.m_EffectData_OccupiedVillage.m_YPos  = TempYPos;

				// robypark 2004/12/27 18:19
				TempSEffectData.m_EffectData_OccupiedVillage.m_scMaxRepeatTimes = ON_FIELDEFFECT_ANIMATION_OCCUPIEDVILLAGE_REPEATTIMES;	// Animation 재생 반복 횟수 설정

				_pWorld->pOnlineFieldEffect->AddEffect(TempSEffectData);

				PushEffect(EFFECT_GETCASTLE, 0, 0);
			}
		}
	}

	return 0;
}

// 미니맵에서 캐릭터 정보를 얻어 온다.	
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

// 길드 메세지
UI16 OnlineMsgProc::OnGuildMessage(BYTE *pMsg)
{
	OnGuildMsg*	lpOnGuildMsg = (OnGuildMsg*)pMsg;
	SI32*			lpSI32;
	
	switch(lpOnGuildMsg->dwResponse)
	{
	case ON_RET_GLD_DELETEGLD:
		if(_pWorld->pOnlineVillage)
		{
			// 현재 마을에 있을 경우, 그 마을이 현재 길드와 관련된 경우 마을 Refresh
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
		
		// My Data 수정
		_pWorld->pMyData->SetGuild(NULL, 0, 0);
		
		// 캐릭터 Data 수정
		_pWorld->pIOnlineChar->SetGuildData(_pWorld->pMyData->GetMyCharUniqueID(), NULL, 0, 0);
		
		_pWorld->pOnlineMsgBoxPay->SetMsg( ON_MSGBOX_TYPE_OK, _pWorld->pOnlineText->Get(ON_TEXT_GUILDTEXT), _pWorld->pOnlineText->Get(ON_TEXT_GUILDTEXT_RELEASEGUILD));
		break;
		
	case ON_RET_GLD_CHANGECLASS:
		lpSI32 = (SI32*)((BYTE*)lpOnGuildMsg + sizeof(OnGuildMsg));
		
		// My Data 수정
		_pWorld->pMyData->SetGuild(_pWorld->pMyData->GetGuildName(), *lpSI32, _pWorld->pMyData->GetGuildFlag());
		
		// 캐릭터 Data 수정
		_pWorld->pIOnlineChar->SetGuildData(_pWorld->pMyData->GetMyCharUniqueID(), _pWorld->pMyData->GetGuildName(), *lpSI32, _pWorld->pMyData->GetGuildFlag());
		
		_pWorld->pOnlineMsgBoxPay->SetMsg( ON_MSGBOX_TYPE_OK, _pWorld->pOnlineText->Get(ON_TEXT_GUILDTEXT), _pWorld->pOnlineText->Get(ON_TEXT_GUILDTEXT_CHANGEGRADE), _pWorld->pOnlineText->Get(_pWorld->m_cltIGuildClass.GetClassName(*lpSI32)));
		break;
		
	case ON_RET_GLD_DISMISSAL:
		// My Data 수정
		_pWorld->pMyData->SetGuild(NULL, 0, 0);
		
		// 캐릭터 Data 수정
		_pWorld->pIOnlineChar->SetGuildData(_pWorld->pMyData->GetMyCharUniqueID(), NULL, 0, 0);
		
		_pWorld->pOnlineMsgBoxPay->SetMsg( ON_MSGBOX_TYPE_OK, _pWorld->pOnlineText->Get(ON_TEXT_GUILDTEXT), _pWorld->pOnlineText->Get(ON_TEXT_GUILDTEXT_DISMISSAL));
		break;
	}

	return 0;
}

// 퀘스트 메세지
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
				// actdoll (2004/04/06 10:59) : = 이벤트 = 독도퀘스트가 종료된 경우 플래그를 꺼 준다.
//				if( lpOnResQuestSelectNPC->m_QuestCode != 22100 )	_pWorld->m_pQuestInterface->RecvMsg_AddCompletedQuest(lpOnResQuestSelectNPC->m_QuestCode);
//				else												_pWorld->m_bEventDokdoQuestPlay	= false;

				// 보상받기
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
							// actdoll (2004/03/31 15:06) : 아주 오래전부터 발생하던, 버그가 아닌 작동 오류 같다.
							//	AddItem에서 OnlineFollow클래스의 AddItem이 할당된 자신에게 똑같은 아이템이 있을 경우 카운트가 현재 가지고 있는 갯수로 바뀌는 문제가 있다.
							//	사용하는 구간이 많아 섣불리 바꿀 수 없으므로 편법을 쓴다. 나중에 연구해 볼것
							siTempItemCount		= lpOnResQuestSelectNPC_EndQuest->m_QuestPay[TempCount].m_QuestPayData_Item.m_AddItemData.uiQuantity;
							_pWorld->pMyData->AddItem(lpOnResQuestSelectNPC_EndQuest->m_QuestPay[TempCount].m_QuestPayData_Item.m_FollowerSlot, &lpOnResQuestSelectNPC_EndQuest->m_QuestPay[TempCount].m_QuestPayData_Item.m_AddItemData);
							_pWorld->pOnlineInventory->FillItemListBox();
							lpCItemHeader = _pWorld->pItemsInfo->GetItem(lpOnResQuestSelectNPC_EndQuest->m_QuestPay[TempCount].m_QuestPayData_Item.m_AddItemData.uiID);

							sprintf((char*)TempPayBuffer, _pWorld->pOnlineText->GetQuestText(ON_QUESTTEXT_PAYITEM), _pWorld->pOnlineText->Get(lpCItemHeader->m_siItemName), siTempItemCount );
							pGame->pItemInfoWindow->AddItemInfoWindow(150, 494, TempYPos, lpOnResQuestSelectNPC_EndQuest->m_QuestPay[TempCount].m_QuestPayData_Item.m_AddItemData.uiID, (char*)TempPayBuffer);
							TempYPos -= 95;
							break;

						// robypark 2004/8/30 20:00
						// 복권식 이벤트 상품 당첨. 게임 내 아이템이 아닌 현실세계 아이템 지급(추후 발송되는 상품) 처리
						case QUESTPAYTYPE_EVENT_ITEM:
							// robypark 2004/9/17 15:5
							// 당첨 메시지를 퀘스트파서에서 얻어온 값을 이용하여 출력되도록 수정
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

				// 돈 삭제하기
				if(lpOnResQuestSelectNPC_EndQuest->m_DeleteMoney)
				{
					_pWorld->pMyData->SetMoney(_pWorld->pMyData->GetMoney() - lpOnResQuestSelectNPC_EndQuest->m_DeleteMoney);
				}

				// Item 삭제하기
				if(lpOnResQuestSelectNPC_EndQuest->m_DeleteItemCount)
				{
					lpMercenaryItemData = (MercenaryItemData*)((BYTE*)lpOnResQuestSelectNPC_EndQuest + sizeof(OnResQuestSelectNPC_EndQuest));
					for(TempCount = 0; TempCount < lpOnResQuestSelectNPC_EndQuest->m_DeleteItemCount; TempCount++)
					{
						switch(lpMercenaryItemData[TempCount].siFlag)
						{
							case 0:			// 인벤토리 아이템
								_pWorld->pMyData->DelItem(lpMercenaryItemData[TempCount].siSlot, &lpMercenaryItemData[TempCount].siInventoryItemInfo);
								break;

							case 1:			// 장비된 아이템
								// 현재는 무시
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

// 퀘스트 메세지 수락 응답	
UI16 OnlineMsgProc::OnResQuestAccept(BYTE *pMsg)
{

	OnResQuestAcceptQuest*	lpOnResQuestAcceptQuest = (OnResQuestAcceptQuest*)pMsg;
	
	// actdoll (2004/04/06 10:14) : = 이벤트 = 독도퀘스트를 수락한 경우 태극기를 띄워준다.
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

// 퀘스트 취소	
UI16 OnlineMsgProc::OnResQuestCamcel(BYTE *pMsg)	
{
	OnResQuestCancelQuest*	lpOnResQuestCancelQuest = (OnResQuestCancelQuest*)pMsg;

	// actdoll (2004/04/06 10:14) : = 이벤트 = 독도퀘스트를 취소한 경우 태극기를 제거한다
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

// 진행중인 퀘스트
UI16 OnlineMsgProc::OnResQuestGetPlay(BYTE *pMsg)
{
	OnResQuestGetPlayQuest*	lpOnResQuestGetPlayQuest = (OnResQuestGetPlayQuest*)pMsg;
	BYTE*					lpBuffer1;
	BYTE*					lpBuffer2;

	lpBuffer1 = (BYTE*)((BYTE*)lpOnResQuestGetPlayQuest + sizeof(OnResQuestGetPlayQuest));
	lpBuffer2 = lpBuffer1 + (sizeof(SPlayingQuest) * lpOnResQuestGetPlayQuest->m_PlayingQuestCodeCount);
	_pWorld->m_pQuestInterface->RecvMsg_SetQuestData(lpOnResQuestGetPlayQuest->m_PlayingQuestCodeCount, lpOnResQuestGetPlayQuest->m_CompletedQuestListCount, (SPlayingQuest*)lpBuffer1, (SI16*)lpBuffer2);

	// actdoll (2004/04/06 10:14) : = 이벤트 = 독도 퀘스트를 검색해서 발견될 경우 켜주도록 한다.
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

// 진행중인 퀘스트 데이타
UI16 OnlineMsgProc::OnResQuestGetPlayData(BYTE *pMsg)
{
	OnResQuestGetPlayQuestData*	lpOnResQuestGetPlayQuestData = (OnResQuestGetPlayQuestData*)pMsg;

	_pWorld->m_pQuestInterface->RecvMsg_SetPlayingQuestData(&lpOnResQuestGetPlayQuestData->m_PlayingQuestData);

	return 0;
}

// 퀘스트 초기화
UI16 OnlineMsgProc::OnResQuestClear(BYTE *pMsg)
{
	_pWorld->m_pQuestInterface->RecvMsg_ClearAllQuest();
	return 0;
}


// 좌판에서 아이템이 변경
UI16 OnlineMsgProc::OnChangeBoothItem(BYTE *pMsg)
{
	// 누군가 좌판정보를 변경했는지 감지해서 말풍선 바꾸기
	SI16	i;
	OnChangeBoothItemMsg *pOnChangeBoothItemMsg = (OnChangeBoothItemMsg *)pMsg;
	
	if(pOnChangeBoothItemMsg != NULL)
	{
		// 좌판 인벤토리에서 변경된 아이템 갱신(현재 거래중인 상점이라면)
		if(_pWorld->pOnlineBooth->GetAccount() == pOnChangeBoothItemMsg->uiAccount)
		{
			// 아이템 삭제
			if(pOnChangeBoothItemMsg->BaseBoothItem.uiProductID == 0 && _pWorld->pOnlineBooth->GetStatusOut() != ON_BENCH_OUT_BUY_FOLLOWER_OK)
				_pWorld->pOnlineBooth->GetList()->DeleteSlot(pOnChangeBoothItemMsg->BaseBoothItem.uiBoothSlot, 0);
			else
			{
				_pWorld->pOnlineClient->SendGetBoothItemInfo((UI16)pOnChangeBoothItemMsg->uiAccount);
/*
				// 아이템 추가
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
			// 좌판이라면 좌판에 아이템 이미지 올려놓기
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
			// 말풍선 띄우기
			ZeroMemory(_pWorld->pOnlineBooth->m_szBoothName, sizeof(_pWorld->pOnlineBooth->m_szBoothName));
			CHAR buf[256];
			ZeroMemory(buf, sizeof(buf));
			SI08 siFirst = 0;
			
			for(i = 0; i < ON_MAX_BOOTH_ITEM; i++)
			{
				SellItemInfo	*pTempSellItemInfo, TempSellItemInfo;
				CItemHeader		*pItem;
				
				if(i == pOnChangeBoothItemMsg->BaseBoothItem.uiBoothSlot)	// 새로 변경된 아이템
				{
					// pIOnlineChar의 좌판정보를 갱신한다.
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
				else		// 기존에 올려진 아이템들
				{
					// pIOnlineChar의 좌판정보를 얻어온다.
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

// 좌판에서 아이템 정보를 얻는다.
UI16 OnlineMsgProc::OnResChangeBoothItem(BYTE *pMsg)
{
	// 서버로부터 좌판 정보를 받는다.
	OnResPMItemInfoMsg *pOnResPMItemInfoMsg = (OnResPMItemInfoMsg*)pMsg;
	if(pOnResPMItemInfoMsg == NULL)	return 0;
	
	if(pOnResPMItemInfoMsg->dwResponse == ON_RET_OK)
	{
		// 좌판위에 놓여진 아이템의 정보를 얻는다. (ItemID, Quantity, Price, Desc) 5개
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
			
			// m_pBoothList에 아이템 설명 변수가 없으므로 설명은 따로 저장해둔다.
			ZeroMemory(_pWorld->pOnlineBooth->m_szBuyItemDesc[pOnBoothItem[i].uiBoothSlot], sizeof(_pWorld->pOnlineBooth->m_szBuyItemDesc[pOnBoothItem[i].uiBoothSlot]));
			strncpy(_pWorld->pOnlineBooth->m_szBuyItemDesc[pOnBoothItem[i].uiBoothSlot], pOnBoothItem[i].Note, ON_MAX_BOOTH_NOTE);

			// 용병의 인벤정보를 세팅한다.
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
		// 노필드 설정
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

// 캐릭터의 상세 정보	
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

// 맵에 있는 아이템이 변경
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
			// 이동멈춤
			SI16 siHeroX, siHeroY;
			_pWorld->pIOnlineChar->GetPos(_pWorld->pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);
			_pWorld->pOnlineCharUI->SetMove(siHeroX, siHeroY);

			// 나의 인벤에 너어 준다.
			ItemInfo TempItemInfo;
			_pWorld->pOnlineInventory->GetLCInventory()->CopyData(siSlot, TempItemInfo);
			TempItemInfo.ItemCount		+= 1;					
			_pWorld->pOnlineInventory->GetLCInventory()->AddItem(TempItemInfo, siSlot);

			// 나의 데이타에 너어 준다.
			MyItemData MyItemData;	
			MyItemData.siPosInInventory	=	siSlot;
			MyItemData.uiID				=	(UI16)lpsResponseProbeArea->foundItem;
			MyItemData.uiQuantity		=	1;				
			_pWorld->pMyData->AddItem(0, &MyItemData);
			_pWorld->pOnlineInventory->FillItemListBox();

			// 아이템 획득 표시해주기
			CHAR	buffer[512];
			CItemHeader *lpCItemHeader = _pWorld->pItemsInfo->GetItem(lpsResponseProbeArea->foundItem);
			sprintf(buffer, _pWorld->pOnlineText->Get(ON_TEXT_AQUIRE_BURIED_ITEM), _pWorld->pOnlineText->Get(lpCItemHeader->m_siItemName));
			pGame->pItemInfoWindow->AddItemInfoWindow(60, 250, 10, lpsResponseProbeArea->foundItem, buffer);

			// 효과음 출력
			PushEffect(EFFECT_GETITEM,     0, 0);
			PushEffect(EFFECT_QUEST_CLEAR, 0, 0);
		}
	}

	return 0;
}

// 경매인에게 결과 알림	
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


// 경매 광고	
UI16 OnlineMsgProc::NotifyAdvertise(BYTE *pMsg)
{
	AUCTION::sNotifyAdvertise *lpNotifyAdvertise = (AUCTION::sNotifyAdvertise *)pMsg;

	if (lpNotifyAdvertise) {
		char szStr[256];
		ZeroMemory(szStr,256);

		switch (lpNotifyAdvertise->articleKind)
		{
			
			// 아이템을 경매중이다.
		case AUCTION::ARTICLE_ITEM:
			{
				
				AUCTION::sArticleItemInfo *pItemInfo = (AUCTION::sArticleItemInfo *)(lpNotifyAdvertise->articleData);
				CItemHeader	*pItem;
				pItem	= _pWorld->pItemsInfo->GetItem( pItemInfo->itemID );																									
				
				if (pItem) {									
					// 품목 : %s  갯수 : %ld
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


// 따라 다니는 용병 교체
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

// 따라 다니는 용병 교체 본인	
UI16 OnlineMsgProc::OnVmercenaryChangeMe(BYTE *pMsg)
{
	OnResponse_VMercenary_ChangeMain*	lpOnResponse_VMercenary_ChangeMain = (OnResponse_VMercenary_ChangeMain*)pMsg;

	_pWorld->pMyData->SetMercenary(lpOnResponse_VMercenary_ChangeMain->siSlot);

	return 0;
}

// robypark 2005/1/5 17:12
// 공성전 유닛인 상태에서 일반 유닛으로 돌아가겠다고한 요청에 대한 응답
// ON_RESPONSE_NORMAL_CHAR
UI16 OnlineMsgProc::OnResponseNormalCharacter(BYTE *pMsg)
{
	OnResNormalChar* lpOnResNormalChar = (OnResNormalChar *)pMsg;

	DWORD		IP;
	CHAR		TempBuffer[128];
	
	// 캐릭 정보 얻기
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

		// 공성전 유닛이 아니다.
		_pWorld->SetIsShowInformationAboutSiegeWarfareUnit(0);

		// robypark 2005/1/28 14:59
		// 공성전 유닛 인터페이스 닫기
		_pWorld->pOnlinePannel->SiegeWarfareUnitUI_DestroyWindow();

		// 변신 불가능 초기화
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

// 주변의 사람들의 코드가 변경	
UI16 OnlineMsgProc::OnChangeCharCode(BYTE *pMsg)
{
	OnChangeCharacterCodeMsg* lpOnChangeCharacterCodeMsg = (OnChangeCharacterCodeMsg*)pMsg;

	DWORD		IP;
	CHAR		TempBuffer[128];
	
	// 캐릭 정보 얻기
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

	// 공성전 유닛인 상태에서 죽어 일반 유닛이 되는 경우
	if (TRUE == (_pWorld->pIOnlineChar->IsSiegeWarfareUnit(uiUniqueID)))	// 공성전 유닛이었다.
	{
		PushEffect(EFFECT_UNITDEAD, 0 , 0);	// 효과음 출력

		// 변신 불가능 설정
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
	SI32		siSiegeWarfareTransformType;		// 공성 유닛 변신 이펙트

	// 캐릭 정보 얻기
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

	// 변신하는 캐릭터가 사용자 자신인 경우
	if(_pWorld->pMyData->GetMyCharUniqueID() == pOnImperiumUseMsg->uiUniqueID)
	{
		_pWorld->pMyData->GetFollowersInterface()->GetWearItems(0, uiEquipWeapon);

		_pWorld->pMyData->SetMyCharKind(pOnImperiumUseMsg->uiKind);

		// robypark 2004/11/19 17:1
		// 공성전을 위해 변신 전 주인공 캐릭터 DB ID 저장
		_pWorld->pMyData->SetPrevCharOnlineKind(pOnImperiumUseMsg->uiPrevKind);

		// 열려져있는 창들 닫기
		// 장부창
		if (_pWorld->pOnlineTradeBook->IsAction() )		
			_pWorld->pOnlineTradeBook->SetAction( FALSE );

		// 미니맵 창
		if (_pWorld->pOnlineMiniMap->IsActive() )		
			_pWorld->pOnlineMiniMap->SetActive( FALSE );
		// 용병창
		if (_pWorld->m_pMercenary->IsAction() )	
			_pWorld->m_pMercenary->OnButtonPush();
		// 도움말 창
		if (_pWorld->pOnlineTip->IsAction())				
			_pWorld->pOnlineTip->SetAction(FALSE);
	}
	else
	{
		_pWorld->pIOnlineChar->GetEquip(uiUniqueID, uiEquipWeapon);
	}

	// 공성전 유닛으로 처음 변하는지 혹은 재차 변신인지 검사하여 이펙트를 사용한다.
	if (FALSE == (_pWorld->pIOnlineChar->IsSiegeWarfareUnit(uiUniqueID)))
	{
		siSiegeWarfareTransformType = EFFECT_CONSTANT_SIEGEWARFARE_TRANSFORM;	// 처음 변신
		PushEffect(EFFECT_UNITCHANGE, 0 , 0);	// 효과음 출력

		// 공성전 유닛일 때 할 수 없는 주의사항 메시지 박스 출력
		if(_pWorld->pMyData->GetMyCharUniqueID() == pOnImperiumUseMsg->uiUniqueID)
		{
			/*
			_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, 
											"",
											_pWorld->pOnlineText->Get(ON_TEXT_FIRST_TRANSFORM_TO_SIEGEWARFARE_UNIT));
			*/
			HDC hdc;
			// 공성전 유닛일 때 할 수 없는 주의사항 메시지 박스 출력
			// HDC를 이용하여 찍어야 할 정보를 설정
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
		siSiegeWarfareTransformType = EFFECT_CONSTANT_SIEGEWARFARE_RETRANSFORM;	// 아이템 재사용 변신
		PushEffect(EFFECT_SUPPLEMENT, 0 , 0);	// 효과음 출력
	}
	
	_pWorld->pIOnlineChar->Free(uiUniqueID);
	_pWorld->pIOnlineChar->Init(uiUniqueID, (CHAR*)TempBuffer, uiKind, siX, siY, IP, 0, siFightPower, _pWorld,(UI16*)uiEquipWeapon);
	_pWorld->pIOnlineChar->SetEquipItem(uiUniqueID, (UI16*)uiEquipWeapon);

	_pWorld->pIOnlineChar->SetGuildData(uiUniqueID, (CHAR*)TempBuffer1, siGuildInGrade, siGuildFlag);
	_pWorld->pIOnlineChar->SetMercenary(uiUniqueID, siMercenaryKind);

	// 공성전 유닛으로 처음 변하는지 혹은 재차 변신인지 검사하여 이펙트를 사용한다.
	_pWorld->pIOnlineChar->SetSiegeWarfare_TransformEffect(uiUniqueID, siSiegeWarfareTransformType);
	
	_pWorld->pOnlineInventory->SetMercenarySlot(0);

	_pWorld->pOnlineCharUI->SetAction(TRUE);

	_pWorld->pIOnlineChar->SetSiegeWarfare_Hp(uiUniqueID, pOnImperiumUseMsg->siHp);
	_pWorld->pIOnlineChar->SetSiegeWarfare_MaximumHP(uiUniqueID, pOnImperiumUseMsg->siHp);
	_pWorld->pIOnlineChar->SetSiegeWarfare_SupplyGoodsAmount(uiUniqueID, pOnImperiumUseMsg->siSupply);
	_pWorld->pIOnlineChar->SetSiegeWarfare_CurrentSupplyGoodsAmount(uiUniqueID, pOnImperiumUseMsg->siSupply);

	// 시작 ////////////////////////////////
	// robypark 2004/11/9 21:10
	// 공성전 유닛의 현재 보급품량 초기화
	if(_pWorld->pMyData->GetMyCharUniqueID() == pOnImperiumUseMsg->uiUniqueID)
	{
		_pWorld->SetIsShowInformationAboutSiegeWarfareUnit(ON_GAMEOPTION_SIEGEWARFAREUNIT);	// 공성전 유닛이다.

		_pWorld->pOnlinePannel->SiegeWarfareUnitUI_DestroyWindow();

		// robypark 2005/1/28 14:59
		// 공성전 유닛 인터페이스 열기
		_pWorld->pOnlinePannel->SiegeWarfareUnitUI_CreateWindow(SW_NORMAL);
		_pWorld->pOnlinePannel->ShowWindow_SiegeWarfareUnitUI(SW_NORMAL);

		// 공성전 유닛의 공격 지연 시간 설정(바로 재 공격이 가능하도록 0으로 설정해준다.)
		_pWorld->pIOnlineChar->SetSiegeWarfareUnit_LastAttackDelay(_pWorld->pMyData->GetMyCharUniqueID(), 0);
	}
	else
	{
		// 변신한 캐릭터의 상세 정보가 없다면
		if(_pWorld->pIOnlineChar->GetDataType(pOnImperiumUseMsg->uiUniqueID) == 1)
		{
			if (_pWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
			{
				// 상세정보를 요청한다.
				_pWorld->pOnlineClient->SendPlayerDetailInfo(pOnImperiumUseMsg->uiUniqueID);
				_pWorld->pIOnlineChar->SetDataType(pOnImperiumUseMsg->uiUniqueID, 2);	// 정보 다운로드 대기 중...
			}
		}
	}
	// 끝 ////////////////////////////////

	_pWorld->pIOnlineChar->SetCaptain(uiUniqueID,pOnImperiumUseMsg->bCaptain);
	
	return 0;
}

// robypark 2005/1/28 18:44
// 공성전 유닛으로 변신 요청 응답
// ON_RESPONSE_CHANGE_WAR
UI16 OnlineMsgProc::OnResChangeWar(BYTE *pMsg)
{
	OnResChangeUnit		*pOnResChangeUnit = (OnResChangeUnit *)pMsg;

	DWORD		IP;
	CHAR		TempBuffer[128];

	// 변신 성공
	if (ON_CHANGE_OK == pOnResChangeUnit->dwResPonse)
	{
		// 캐릭 정보 얻기
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

		// 변신하는 캐릭터가 사용자 자신인 경우
		if(_pWorld->pMyData->GetMyCharUniqueID() == uiUniqueID)
		{
			_pWorld->pMyData->GetFollowersInterface()->GetWearItems(0, uiEquipWeapon);

			_pWorld->pMyData->SetMyCharKind(uiKind);

			// robypark 2004/11/19 17:1
			// 공성전을 위해 변신 전 주인공 캐릭터 DB ID 저장
			_pWorld->pMyData->SetPrevCharOnlineKind(uiPrevKind);

			// 열려져있는 창들 닫기
			// 장부창
			if (_pWorld->pOnlineTradeBook->IsAction() )		
				_pWorld->pOnlineTradeBook->SetAction( FALSE );

			// 미니맵 창
			if (_pWorld->pOnlineMiniMap->IsActive() )		
				_pWorld->pOnlineMiniMap->SetActive( FALSE );
			// 용병창
			if (_pWorld->m_pMercenary->IsAction() )	
				_pWorld->m_pMercenary->OnButtonPush();
			// 도움말 창
			if (_pWorld->pOnlineTip->IsAction())				
				_pWorld->pOnlineTip->SetAction(FALSE);

			_pWorld->SetIsShowInformationAboutSiegeWarfareUnit(ON_GAMEOPTION_SIEGEWARFAREUNIT);	// 공성전 유닛이다.

			_pWorld->pOnlinePannel->SiegeWarfareUnitUI_DestroyWindow();

			// robypark 2005/1/28 14:59
			// 공성전 유닛 인터페이스 열기
			_pWorld->pOnlinePannel->SiegeWarfareUnitUI_CreateWindow(SW_NORMAL);
			_pWorld->pOnlinePannel->ShowWindow_SiegeWarfareUnitUI(SW_NORMAL);

			// 공성전 유닛의 공격 지연 시간 설정(바로 재 공격이 가능하도록 0으로 설정해준다.)
			_pWorld->pIOnlineChar->SetSiegeWarfareUnit_LastAttackDelay(_pWorld->pMyData->GetMyCharUniqueID(), 0);

			// 보급 수레(메딕) 마나 자동 회복 시간 설정
			_pWorld->GetRecoveryManaInfo()->m_dwLeftTime = _pWorld->GetSOptionSiegeWarfare()->siMedicRechargeManaCoolTime;

			PushEffect(EFFECT_UNITCHANGE, 0 , 0);	// 효과음 출력

			HDC hdc;
			// 공성전 유닛일 때 할 수 없는 주의사항 메시지 출력
			// HDC를 이용하여 찍어야 할 정보를 설정
			if(GETSCREENSURFACE(_pWorld)->GetDC(&hdc) == DD_OK)
			{
				SetMessage4((char*)_pWorld->pOnlineText->Get(ON_TEXT_FIRST_TRANSFORM_TO_SIEGEWARFARE_UNIT), hdc);

				GETSCREENSURFACE(_pWorld)->ReleaseDC(hdc);
			}
			_pWorld->pOnlinePannel->AddChat((char*)_pWorld->pOnlineText->Get(ON_TEXT_FIRST_TRANSFORM_TO_SIEGEWARFARE_UNIT), ON_INN_COLOR_WHITE, CHATTINGMODE_GUILD);

			// 변경된 소지금 설정
			_pWorld->pMyData->SetMoney(_pWorld->pMyData->GetMoney() - pOnResChangeUnit->siMoney);

			// 소지금 변화 효과 처리
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

		// 공성전 유닛으로 변신 이펙트를 사용한다.
		_pWorld->pIOnlineChar->SetSiegeWarfare_TransformEffect(uiUniqueID, EFFECT_CONSTANT_SIEGEWARFARE_TRANSFORM);
		
		_pWorld->pOnlineInventory->SetMercenarySlot(0);

		_pWorld->pOnlineCharUI->SetAction(TRUE);

		_pWorld->pIOnlineChar->SetSiegeWarfare_Hp(uiUniqueID, pOnResChangeUnit->siHP);
		_pWorld->pIOnlineChar->SetSiegeWarfare_MaximumHP(uiUniqueID, pOnResChangeUnit->siHP);
		_pWorld->pIOnlineChar->SetSiegeWarfare_SupplyGoodsAmount(uiUniqueID, pOnResChangeUnit->siMP);
		_pWorld->pIOnlineChar->SetSiegeWarfare_CurrentSupplyGoodsAmount(uiUniqueID, pOnResChangeUnit->siMP);

		// 시작 ////////////////////////////////
		// robypark 2004/11/9 21:10
		// 공성전 유닛의 현재 보급품량 초기화
		if(_pWorld->pMyData->GetMyCharUniqueID() == uiUniqueID)
		{
		}
		else
		{
			// 변신한 캐릭터의 상세 정보가 없다면
			if(_pWorld->pIOnlineChar->GetDataType(uiUniqueID) == 1)
			{
				if (_pWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
				{
					// 상세정보를 요청한다.
					_pWorld->pOnlineClient->SendPlayerDetailInfo(uiUniqueID);
					_pWorld->pIOnlineChar->SetDataType(uiUniqueID, 2);	// 정보 다운로드 대기 중...
				}
			}
		}
		// 끝 ////////////////////////////////

		_pWorld->pIOnlineChar->SetCaptain(uiUniqueID, pOnResChangeUnit->bQuantity);
	}
	else if (ON_CHANGE_NOT_GUILD == pOnResChangeUnit->dwResPonse)          //상단에 가입되어있지 않다.
	{
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, 
										"", 
										_pWorld->pOnlineText->Get(8100123));
	}
	else if (ON_CHANGE_ENOUGH_LEVEL == pOnResChangeUnit->dwResPonse)          //레벨이 부족하다. 
	{
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, 
										"", 
										_pWorld->pOnlineText->Get(8100122));
	}
	else if (ON_CHANGE_ENOUGH_MONEY == pOnResChangeUnit->dwResPonse)          //돈이 부족하다.
	{
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, 
										"", 
										_pWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_NOTMONEY));
	}
	else if (ON_CHANGE_ENOUGH_KIND == pOnResChangeUnit->dwResPonse)          //해당 속성의 유닛이 없다.
	{
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, 
										"", 
										_pWorld->pOnlineText->Get(8100121));
	}
	else if (ON_CHANGE_FAILED == pOnResChangeUnit->dwResPonse)          //일반적인 실패
	{
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, 
										"", 
										_pWorld->pOnlineText->Get(8100123));
	}

	// 변신 메시지 처리 완료
	_pWorld->pOnlinePannel->GetSiegeWarfareTransformUIWnd()->SetSendedMsg(FALSE);

	return 0;
}

// 장부 부동산보유 정보(응답)	
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

		// 내정보에 설정
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

// 나의 마을이 공격 당하고 있다
UI16 OnlineMsgProc::MyVillageAttacked(BYTE *pMsg)
{
	OnMyVillageAttackedMsg* pOnMyVillageAttackedMsg = (OnMyVillageAttackedMsg*)pMsg;

	CHAR	TempBuffer[1024];
	VillageHeader* lpVillageHeader = _pWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(pOnMyVillageAttackedMsg->uiVillageCode);
	sprintf((char*)TempBuffer, _pWorld->pOnlineText->Get(ON_TEXT_PACKET_VILLAGE_ATTACK), _pWorld->pOnlineText->Get(lpVillageHeader->siVillageName));
//	_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", TempBuffer);

	HDC hdc;
	// HDC를 이용하여 찍어야 할 정보를 설정
	if(GETSCREENSURFACE(_pWorld)->GetDC(&hdc) == DD_OK)
	{
		SetMessage4((char*)TempBuffer, hdc);

		GETSCREENSURFACE(_pWorld)->ReleaseDC(hdc);
	}
	_pWorld->pOnlinePannel->AddChat((char*)TempBuffer, ON_INN_COLOR_WHITE, CHATTINGMODE_GUILD);

	return 0;
}

// 친구 리스트
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

// 친구 리스트에 있는 친구가 로그 아웃
UI16 OnlineMsgProc::FriendLogOut(BYTE *pMsg)
{
	char szTemp[512];
	char szID[ON_ID_LENGTH+1];
	ZeroMemory( szID, sizeof(szID) );
	OnFriendLoggedInOut *pOnFriendLoggedInOut = (OnFriendLoggedInOut*)pMsg;
	
	strncpy( szID, pOnFriendLoggedInOut->szID, sizeof(pOnFriendLoggedInOut->szID) );
	_pWorld->pMyData->m_GoodFriendList.SetLogin(szID, pOnFriendLoggedInOut->bState);


	// 로그인(아웃) 했을 경우 메시지 출력
	string msg = pOnFriendLoggedInOut->bState == TRUE ? _pWorld->pOnlineText->Get(4000292) : _pWorld->pOnlineText->Get(4000293);
	sprintf( szTemp, "%s %s", szID, msg.c_str() );
	SetMessage( szTemp );
	return 0;
}

// 마을 정보	
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

		// 현재 마을 안에 있다면
		if (_pWorld->pOnlineVillage)
			_pWorld->pOnlineVillage->m_OnRecvVillageInfo.uiPopulation = lpOnRecvVillageInfo->uiPopulation;
	}

	return 0;
}

// 통장수	
UI16 OnlineMsgProc::BankAccountList(BYTE *pMsg)
{
	OnResBankBankBookListCountMsg*	lpOnResBankBankBookListCountMsg = (OnResBankBankBookListCountMsg*)pMsg;

	if(lpOnResBankBankBookListCountMsg->dwResponse == ON_RET_OK)
	{
		_pWorld->pOnlineTradeBook->Quest_SetBankBookCount(lpOnResBankBankBookListCountMsg->BankBookCount);
	}

	return 0;
}

// 경매(용병,아이템) 요청에 응답 한다	
UI16 OnlineMsgProc::SellArticle(BYTE *pMsg)
{
	_pWorld->pOnlineReception->WaitingReLay(pMsg);
	return 0;
}

// 입찰에 대한 요청	
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

// 입찰 성공		
UI16 OnlineMsgProc::ReportBidding(BYTE *pMsg)
{
	_pWorld->pOnlineFieldAuction->ReportBidding(pMsg);
	return 0;
}

// 착찰
UI16 OnlineMsgProc::ReportSuccessful(BYTE *pMsg)
{
	_pWorld->pOnlineFieldAuction->ReportSuccessful(pMsg);
	return 0;
}

// 아무것두 안삼	
UI16 OnlineMsgProc::ReportFail(BYTE *pMsg)
{
	_pWorld->pOnlineFieldAuction->ReportSuccessful(pMsg);
	return 0;
}

// 경매가 새로 열림
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

// 좌판 오픈
UI16 OnlineMsgProc::OnResOpen(BYTE *pMsg)
{
	_pWorld->pOnlineBooth->OnResOpen(pMsg);
	return 0;
}

// 좌판에서 아이템 구입
UI16 OnlineMsgProc::OnResBuyItem(BYTE *pMsg)
{
	_pWorld->pOnlineBooth->OnResBuyItem(pMsg);
	return 0;
}

// 좌판 닫어
UI16 OnlineMsgProc::OnResClose(BYTE *pMsg)
{
	_pWorld->pOnlineBooth->OnResClose(pMsg);
	return 0;
}

// 좌판 용병 구입
UI16 OnlineMsgProc::OnResBuyFollower(BYTE *pMsg)
{
	_pWorld->pOnlineBooth->OnResBuyFollower(pMsg);
	return 0;
}

// 좌판 아이템 변경
UI16 OnlineMsgProc::OnResChangeItem(BYTE *pMsg)
{
	_pWorld->pOnlineBooth->OnResChangeItem(pMsg);
	return 0;
}

// 용병 이름 변경	
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

// actdoll (2004/03/30 11:21) : = 이벤트 = 독도 퀘스트 완료자 생김
//UI16 OnlineMsgProc::OnDokdoQuestCompletedUser( BYTE *pMsg )
//{
//	OnDokdoQuestCompletedUserInfo	*pData	= (OnDokdoQuestCompletedUserInfo*)pMsg;
//	_pWorld->SetDokdoQuestCompleteUserCount( pData->uiQuestCompletedUserCount );
//
//	// 한국어 버젼일때만 표기된다.
//	if( pGame->LanguageCode == ELANGAGECODE_KOREAN )	
//	{
//		if(_pWorld->m_LanguageCode == 0) //ELANGAGECODE_KOREA
//		{
//			char	pszText[128] = {0,};
//			sprintf( pszText, "[경축] %s 님이 %d 번째로 독도에 태극기를 꽂았습니다!", pData->szID, pData->uiQuestCompletedUserCount );
//			_pWorld->pOnlineNoticeList->AddNoticeData( pszText, RGB( 255, 255, 0 ) );
//		}
//	}
//	
//	return 0;
//}

//-----------------------------------------------------------------------------
// Name: OnReLoadWearItem()
// Code: actdoll (2004-07-19)
// Desc: ON_RELOAD_WEAR_ITEM 신호 접수. Server -> Client 용
//		게임 시간으로 24시간마다 캐릭터들이 차고있는 시간제 아이템 정보를 갱신하라는 뜻이다.
//		이 구조체 후미에 stWearItem이 siMercenaryCount만큼 붙게 된다.
//	pMsg	- 이 메시지와 딸려온 정보( OnReLoadWearItem + ... )
//	return	- 제대로 왔다면 0
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

	pORW	= (OnReLoadWearItem*)pMsg;									// 메시지 본체 받고
//	pstWI	= (stWearItem*)( pMsg + sizeof(OnReLoadWearItem) );
	pstOTII	= (OnTimerItemInfo*)( pMsg + sizeof(OnReLoadWearItem) );	// 본 메시지 후미에 딸려온 시간제 아이템 정보 첫 포인터
	pFG		= _pWorld->pMyData->GetFollowersInterface();				// 현재 내 용병들 그룹 정보를 받는다.

	// 이 메시지가 떨어지면 날짜를 일단 갱신하란 소리다.
	for( i=0; i<ON_MAX_FOLLOWER_NUM; i++ )
	{
		pFollower	= pFG->GetFollower( i );
		if( !pFollower )	continue;
		
		// 장착부별로 아이템을 확인
		for( j=0; j<WEAR_ITEM_NUM; j++ )
		{
			uiOldItemID	= pFollower->GetWearItem( j, bIsTimerItem, siLimitDate );	// 해당 위치의 아이템을 받고
			if( uiOldItemID > 0 && bIsTimerItem )									// 시간제 아이템이 맞다면
			{
				pFollower->SetWearItem( j, uiOldItemID, siLimitDate - 1 );			// 하루를 빼서 다시 장착
			}
		}
	}

	// 추가 데이터가 있다면 아이템을 삭제하란 얘기
	for( i=0; i<pORW->siItemCount; i++ )
	{
		// 용병 정보 하나씩 받고
		pFollower = pFG->GetFollower( pstOTII->siMercenarySlot );
		if( !pFollower )	
		{
			pstOTII++;
			continue;
		}

		// 받아온 아이템 정보 확인사살
		if( !_pWorld->pItemsInfo->GetItem( pstOTII->siItemID ) )			continue;
		if( !_pWorld->pItemsInfo->IsTimeLimitItem( pstOTII->siItemID ) )	continue;
		uiOldItemID	= pFollower->GetWearItem( pstOTII->siLoc, bIsTimerItem, siLimitDate );		// 기존 아이템 존재를 받고
		if( pstOTII->siItemID != uiOldItemID )								continue;

		// 여기까지 정보를 확인했다면 갱신해 주도록 한다.
		if( pstOTII->bDeleteTimerItem )
		{
			pFollower->SetWearItem( pstOTII->siLoc, 0, 0 );	// 해당 유닛의 입고있는 아이템 정보를 클리어시킨다.

			// 만약 본캐릭터에 대한 메시지라면
			if( pstOTII->siMercenarySlot == 0 )	
			{
				UI16	Equip[WEAR_ITEM_NUM];
				UI16	uiMyUniqueID	= _pWorld->pMyData->GetMyCharUniqueID();
				
				// 일단 현재 장비 정보를 받고
				_pWorld->pIOnlineChar->GetEquip( uiMyUniqueID, Equip );
				
				// 메시지에 포함된 위치의 장착 상태를 클리어
				Equip[pstOTII->siLoc] = 0;

				// 그 정보를 다시 집어넣는다.
				_pWorld->pIOnlineChar->SetEquipItem( uiMyUniqueID, Equip );

				// 신발 아이템의 경우 내 필드 이동 속도를 정상으로 돌려준다.
				if( pstOTII->siLoc ==  ON_WEAR_SHOES_ITEM )
				{
					SI32	uiCharIndex		= _pWorld->pIOnlineChar->GetID( uiMyUniqueID );
					SI32	siCharKindIndex = _pWorld->pIOnlineCharKI->GetIndex( _pWorld->pMyData->GetMyCharKind() );
					_pWorld->pIOnlineChar->GetOnlineChar()[uiCharIndex].SetMoveSpeed( _pWorld->pIOnlineCharKI->GetMoveSpeed(siCharKindIndex) );
				}
			}

			bDeleted	= true;
		}

		// 다음것으로~
		pstOTII++;
	}

	// 삭제된 아이템이 있을 경우 메시지 박스를 띄워줄 것.
	if( bDeleted )	_pWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, _pWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_TIMERITEM), _pWorld->pOnlineText->Get(ON_TEXT_NOTICE_ITEM_TIMERITEM_IS_DELETED) );

	// 인벤토리가 활성화 되어있는 경우 아이템 정보를 한번 갱신시킨다.
	if( _pWorld->pOnlineInventory->IsActive() )	_pWorld->pOnlineInventory->FillItemListBox();

	return 0;
}

//사람->마을을 때린다.(원거리)
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
	// 공격 위치 검사
	TempXPos = lpVillageHeader->siX;
	TempYPos = lpVillageHeader->siY;


	if(_pWorld->pIOnlineChar->IsExistUniqueID(pOnResLongAttackVillageRdyMsg->uiAttackCharID))
	{
		// 만약에 공격한 캐릭터 Kind가 서버와 클라이언트에서 보여지는 캐릭터Kind가 다른 경우에 처리
		if (pOnResLongAttackVillageRdyMsg->uiKind != _pWorld->pIOnlineChar->GetKind(pOnResLongAttackVillageRdyMsg->uiAttackCharID))
		{
			DWORD		IP;
			CHAR		TempBuffer[128];

			// 캐릭 정보 얻기
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

			// 변신하는 캐릭터가 사용자 자신이 아닌 경우
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
				// 변신한 캐릭터의 상세 정보가 없다면
				if(_pWorld->pIOnlineChar->GetDataType(uiUniqueID) == 1)
				{
					if (_pWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
					{
						// 상세정보를 요청한다.
						_pWorld->pOnlineClient->SendPlayerDetailInfo(uiUniqueID);
						_pWorld->pIOnlineChar->SetDataType(uiUniqueID, 2);	// 정보 다운로드 대기 중...
					}
				}
			}
		}

		// 공격자 위치 얻기
		_pWorld->pIOnlineChar->GetPos(pOnResLongAttackVillageRdyMsg->uiAttackCharID, &siAttackerPosX, &siAttackerPosY);

		if(lpVillageHeader->siX > siAttackerPosX)
			TempXPos -= 1;

		if(lpVillageHeader->siX < siAttackerPosX)
			TempXPos += 1;

		if(lpVillageHeader->siY > siAttackerPosY)
			TempYPos -= 1;

		if(lpVillageHeader->siY < siAttackerPosY)
			TempYPos += 1;
	
		//마을을 때린다는 ORDER를 내린다.
		_pWorld->pIOnlineChar->AddOrder(pOnResLongAttackVillageRdyMsg->uiAttackCharID, 
										ON_ORDER_LONGDISTANCE_ATTACKVILLAGE_ACTION
										, TempXPos, TempYPos);

		// 원거리 공격
		//이부분을 어떻게 처리할것인지에 대해.
		//무기를 쓴 유저가 판단하여 맞았을때 처리한다.괜찮은 방법일듯 한데.일단.여기로.
		if(pOnResLongAttackVillageRdyMsg->uiAttackCharID == _pWorld->pMyData->GetMyCharUniqueID())
		{
			// robypark 2004/11/3 14:5
			// 공성전 유닛 사이의 공격 이펙트 생성
			_pWorld->SiegeWarfare_CreateAttackEffectVillage(pOnResLongAttackVillageRdyMsg->uiAttackCharID, TempXPos, TempYPos, pOnResLongAttackVillageRdyMsg->uiVillageCode);
		}
		else
		{
			// robypark 2004/11/3 14:5
			// 공성전 유닛 사이의 공격 이펙트 생성
			_pWorld->SiegeWarfare_CreateAttackEffectVillage(pOnResLongAttackVillageRdyMsg->uiAttackCharID, TempXPos, TempYPos, 0);
		}
		/*
			// robypark 2004/11/4 13:56	
			// 대장효과(공격력 업), 대장인지 등의 효과 처리 추가
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

	// 마을의 정보를 변경 마을 방어력 및 수비병 수 수정
	lpSVillageData = _pWorld->pOnlineVillageDescriptList->GetVillageDataData(lpVillageHeader->uiVillageCode);				
	if(lpSVillageData)
	{
		vdTempVillageDefence = lpSVillageData->m_vdChangedDefence;		// 이전 마을 방어력 저장. 폭발 이펙트는 한번만

		if(lpSVillageData->m_fWait == FALSE)
		{
			if(pOnResLongAttackVillageActionMsg->siDefenceSoldier <= 0)
			{
				// 마을의 길드가 없어진 경우
				_pWorld->pOnlineVillageDescriptList->DelVillageDataData(lpVillageHeader->uiVillageCode);

				if(_pWorld->pOnlineMap->m_MapInfo.uiVillageCode)
				{
					_pWorld->pOnlineClient->SendRequestVillageInfo(_pWorld->pOnlineMap->m_MapInfo.uiVillageCode);
				}
			}
			else
			{
				lpSVillageData->m_vdChangedDefence    = pOnResLongAttackVillageActionMsg->vdDefence;	// 마을 방어력

				lpSVillageData->m_siCurrentGarrison = pOnResLongAttackVillageActionMsg->siDefenceSoldier;	// 마을 수비병

				// 마을 방어력 최대값보다 큰 경우
				if(lpSVillageData->m_vdChangedMaxDefence < lpSVillageData->m_vdChangedDefence)
				{
					lpSVillageData->m_vdChangedMaxDefence = lpSVillageData->m_vdChangedDefence;
				}

				// 마을 수비병 최대값보다 큰 경우
				if(lpSVillageData->m_siMaximumGarrison < lpSVillageData->m_siCurrentGarrison)
				{
					lpSVillageData->m_siMaximumGarrison = lpSVillageData->m_siCurrentGarrison;
				}
			}
		}
	}

	// 마을 폭발시 (마을 방어력)
	if(lpSVillageData)
	{
		// 현재 방어력이 0이 된 경우 폭발 이펙트 사용.
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

			// 마을이 점령되었을 효과 뿌리기
			TempXPos = lpVillageHeader->siX;
			TempYPos = lpVillageHeader->siY;

			TempSEffectData.m_fCreate = TRUE;
			TempSEffectData.m_Type    = EFFECTTYPE_OCCUPIED_VILLAGE;
			TempSEffectData.m_EffectData_OccupiedVillage.m_Frame = 0;

			TempSEffectData.m_EffectData_OccupiedVillage.m_XPos  = TempXPos + 1;
			TempSEffectData.m_EffectData_OccupiedVillage.m_YPos  = TempYPos;

			// robypark 2004/12/27 18:19
			TempSEffectData.m_EffectData_OccupiedVillage.m_scMaxRepeatTimes = ON_FIELDEFFECT_ANIMATION_OCCUPIEDVILLAGE_REPEATTIMES;	// Animation 재생 반복 횟수 설정

			_pWorld->pOnlineFieldEffect->AddEffect(TempSEffectData);

			PushEffect(EFFECT_GETCASTLE, 0, 0);
		}
	}

	// robypark 2004/14:32
	// 치명적 공격, 견고한 방어, 일반 공격, 방어일 경우 효과 출력
	switch (pOnResLongAttackVillageActionMsg->siAttackType)
	{
		// 치명적 공격. 공격자 캐릭터에 효과 추가
		case ON_RET_ATTACK_POWER:
			{
				// 선택한 캐릭터 표시 이펙트를 치명적 공격 사용에도 사용한다.
				_pWorld->pIOnlineChar->SetNewSelectedFlag(pOnResLongAttackVillageActionMsg->uiAttackCharID, TRUE);
				_pWorld->pIOnlineChar->SetSiegeWarfare_DeathBlow(pOnResLongAttackVillageActionMsg->uiAttackCharID);
				PushEffect(EFFECT_CRITICAL, 0 , 0);	// 치명적 공격 효과음 출력
			}
			break;
		// 견고한 방어. 피격자 캐릭터에 효과 추가
		case ON_RET_DEFENCE_POWER:
			{
				//_pWorld->pIOnlineChar->SetSiegeWarfare_Evasion(pOnResLongAttackVillageActionMsg->uiAttackCharID);
				//PushEffect(EFFECT_DEFENCE, 0 , 0);	// 견고한 방어 효과음 출력
			}
			break;
		// 일반 공격, 방어
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
	//때린놈의 공격에 의해서 떨어진 방어력.
	pOnResLongAttackVillageActionMsg->siDamage;	

	pOnResLongAttackVillageActionMsg->vdChangedDefence;

	lpSVillageData = _pWorld->pOnlineVillageDescriptList->GetVillageDataData(lpVillageHeader->uiVillageCode);				

	if(lpSVillageData)
	{
		if(lpSVillageData->m_fWait == FALSE)
		{
			if(pOnResLongAttackVillageActionMsg->vdChangedDefence <= 0)
			{
				// 마을의 길드가 없어진 경우
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

	// 마을 폭발시
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

//마을->사람을 때린다.
// 마을 기름 반격
UI16 OnlineMsgProc::VillagePlayerOilAttack(BYTE* pMsg)
{
	OnResVillagePlayerOilAttack*	pOnResVillagePlayerOilAttack;
	VillageHeader*					lpVillageHeader;
	SVillageData*					lpSVillageData;

	pOnResVillagePlayerOilAttack		=	(OnResVillagePlayerOilAttack*)pMsg;

	if (ON_RET_OIL_OK == pOnResVillagePlayerOilAttack->dwResponse)
	{
		// 마을 데이터 헤더 얻기
		lpVillageHeader = _pWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(pOnResVillagePlayerOilAttack->uiVillageCode);

		if (lpVillageHeader)
		{
			// 마을 정보(설명) 데이터 얻기
			lpSVillageData = _pWorld->pOnlineVillageDescriptList->GetVillageDataData(lpVillageHeader->uiVillageCode);				
		
			if (lpSVillageData)
			{
				// 마을 정보 데이터를 다운로드 중이 아니며
				if(lpSVillageData->m_fWait == FALSE)
				{
					lpSVillageData->m_ssCounterAttackType = ON_VILLAGE_ATTACK_OIL;	// 마을이 기름 공격한다고 설정
				}
			}
			else
			{
				// 없으면 Server에 Data 요청
				_pWorld->pOnlineClient->SendRequestVillageData(lpVillageHeader->uiVillageCode);
				_pWorld->pOnlineVillageDescriptList->AddVillageDataData(lpVillageHeader->uiVillageCode);
			}

			// 수성하기를 이용한 사용자가 자신이라면
			if (_pWorld->pMyData->GetMyCharUniqueID() == pOnResVillagePlayerOilAttack->uiUniqueID)
			{
				// 변경된 소지금 설정
				_pWorld->pMyData->SetMoney(_pWorld->pMyData->GetMoney() - pOnResVillagePlayerOilAttack->mnMoney);

				// 소지금 변화 효과 처리
				SEffectData					EffectData;
				ZeroMemory(&EffectData, sizeof(SEffectData));

				EffectData.m_Type = EFFECTTYPE_CHANGENUMBER;
				EffectData.m_EffectData_ChangeNum.m_Frame			= 0;
				EffectData.m_EffectData_ChangeNum.m_XPos			= 93;
				EffectData.m_EffectData_ChangeNum.m_YPos			= 579;
				EffectData.m_EffectData_ChangeNum.m_ChangeNumber    = -pOnResVillagePlayerOilAttack->mnMoney;
				_pWorld->pOnlineFieldEffect->AddEffect(EffectData);

				// 새로운 마을 반격 가능 시간 설정
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

// 마을 돌 반격
UI16 OnlineMsgProc::VillagePlayerStoneAttack(BYTE* pMsg)
{
	OnResVillagePlayerStoneAttack*	pOnResVillagePlayerStoneAttack = NULL;

	VillageHeader*					lpVillageHeader;
	SVillageData*					lpSVillageData;

	pOnResVillagePlayerStoneAttack		=	(OnResVillagePlayerStoneAttack*)pMsg;

	if (ON_RET_STONE_OK == pOnResVillagePlayerStoneAttack->dwResponse)
	{
		// 마을 데이터 헤더 얻기
		lpVillageHeader = _pWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(pOnResVillagePlayerStoneAttack->uiVillageCode);

		if (lpVillageHeader)
		{
			// 마을 정보(설명) 데이터 얻기
			lpSVillageData = _pWorld->pOnlineVillageDescriptList->GetVillageDataData(lpVillageHeader->uiVillageCode);				
			
			if (lpSVillageData)
			{
				// 마을 정보 데이터를 다운로드 중이 아니며
				if(lpSVillageData->m_fWait == FALSE)
				{
					lpSVillageData->m_ssCounterAttackType = ON_VILLAGE_ATTACK_STONE;	// 마을이 돌 공격한다고 설정
				}
			}
			else
			{
				// 없으면 Server에 Data 요청
				_pWorld->pOnlineClient->SendRequestVillageData(lpVillageHeader->uiVillageCode);
				_pWorld->pOnlineVillageDescriptList->AddVillageDataData(lpVillageHeader->uiVillageCode);
			}

			// 수성하기를 이용한 사용자가 자신이라면
			if (_pWorld->pMyData->GetMyCharUniqueID() == pOnResVillagePlayerStoneAttack->uiUniqueID)
			{
				// 변경된 소지금 설정
				_pWorld->pMyData->SetMoney(_pWorld->pMyData->GetMoney() - pOnResVillagePlayerStoneAttack->mnMoney);

				// 소지금 변화 효과 처리
				SEffectData					EffectData;
				ZeroMemory(&EffectData, sizeof(SEffectData));

				EffectData.m_Type = EFFECTTYPE_CHANGENUMBER;
				EffectData.m_EffectData_ChangeNum.m_Frame			= 0;
				EffectData.m_EffectData_ChangeNum.m_XPos			= 93;
				EffectData.m_EffectData_ChangeNum.m_YPos			= 579;
				EffectData.m_EffectData_ChangeNum.m_ChangeNumber    = -pOnResVillagePlayerStoneAttack->mnMoney;
				_pWorld->pOnlineFieldEffect->AddEffect(EffectData);

				// 새로운 마을 반격 가능 시간 설정
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

// 공성전 마을 반격 - 마을 방어력 향상
// ON_RESPONSE_VILLAGE_DEFENCE
UI16 OnlineMsgProc::OnResponseVillageDefence(BYTE *pMsg)
{
	OnResPlayerVillageDefenceUp			*pOnResPlayerVillageDefenceUp = (OnResPlayerVillageDefenceUp *)pMsg;
	VillageHeader*						lpVillageHeader;
	SVillageData*						lpSVillageData;

	// 마을 방어력 향상 성공
	if (ON_RET_DEFENSE_OK == pOnResPlayerVillageDefenceUp->dwResponse)
	{
		// 마을 데이터 헤더 얻기
		lpVillageHeader = _pWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(pOnResPlayerVillageDefenceUp->uiVillageCode);

		if (lpVillageHeader)
		{
			// 마을 정보(설명) 데이터 얻기
			lpSVillageData = _pWorld->pOnlineVillageDescriptList->GetVillageDataData(lpVillageHeader->uiVillageCode);
		
			if (lpSVillageData)
			{
				// 마을 정보 데이터를 다운로드 중이 아니며
				if(lpSVillageData->m_fWait == FALSE)
					lpSVillageData->m_vdChangedDefence += pOnResPlayerVillageDefenceUp->vdDefenceUp;	// 올라간 방어력을 더해준다.
			}
			else
			{
				// 없으면 Server에 Data 요청
				_pWorld->pOnlineClient->SendRequestVillageData(lpVillageHeader->uiVillageCode);
				_pWorld->pOnlineVillageDescriptList->AddVillageDataData(lpVillageHeader->uiVillageCode);
			}
		}

		// 수성하기를 이용한 사용자가 자신이라면
		if (_pWorld->pMyData->GetMyCharUniqueID() == pOnResPlayerVillageDefenceUp->uiUniqueID)
		{
			// 변경된 소지금 설정
			_pWorld->pMyData->SetMoney(_pWorld->pMyData->GetMoney() - pOnResPlayerVillageDefenceUp->mnMoney);

			// 소지금 변화 효과 처리
			SEffectData					EffectData;
			ZeroMemory(&EffectData, sizeof(SEffectData));

			EffectData.m_Type = EFFECTTYPE_CHANGENUMBER;
			EffectData.m_EffectData_ChangeNum.m_Frame			= 0;
			EffectData.m_EffectData_ChangeNum.m_XPos			= 93;
			EffectData.m_EffectData_ChangeNum.m_YPos			= 579;
			EffectData.m_EffectData_ChangeNum.m_ChangeNumber    = -pOnResPlayerVillageDefenceUp->mnMoney;
			_pWorld->pOnlineFieldEffect->AddEffect(EffectData);

			// 새로운 마을 반격 가능 시간 설정
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

		// 방어력 향상 이펙트 추가
		SI16 TempXPos = lpVillageHeader->siX;	// 마을 위치
		SI16 TempYPos = lpVillageHeader->siY;

		SEffectData					TempSEffectData;
		ZeroMemory(&TempSEffectData, sizeof(SEffectData));

		TempSEffectData.m_fCreate = TRUE;

		TempSEffectData.m_Type    = EFFECTTYPE_VILLAGE_DEFENCE_UP;
		TempSEffectData.m_EffectData_VillageDefenceUp.m_Frame = 0;

		// 마을 크기에 따라 방어력 향상 이펙트 위치 수정
		switch (_pWorld->pOnlineResource->GetVillageSize((char *)lpVillageHeader->szImageFile))
		{
			case 0:	// 큰 마을
				TempXPos -= 3;
				TempYPos -= 3;
				break;
			case 1: // 중간 마을
				TempXPos -= 2;
				TempYPos -= 2;
				break;
			case 2: // 작은 마을
				TempXPos -= 1;
				TempYPos -= 1;
				break;
			default:
				break;
		}

		TempSEffectData.m_EffectData_VillageDefenceUp.m_XPos  = TempXPos;
		TempSEffectData.m_EffectData_VillageDefenceUp.m_YPos  = TempYPos;
			
		_pWorld->pOnlineFieldEffect->AddEffect(TempSEffectData);

		PushEffect(EFFECT_WALLREPAIR, 0 , 0);	// 마을 방어력 향상하는 효과음 출력
	}
	// 돈이 부족한 경우
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

// 공성전 마을 반격이 끝났다.
// ON_CHANGED_VILLAGE_ATTACK_TYPE
UI16	OnlineMsgProc::OnBroadcastChangedVillageAttackType(BYTE *pMsg )
{
	OnChangedVillageAttackType		*pOnChangedVillageAttackType;
	VillageHeader*					lpVillageHeader;
	SVillageData*					lpSVillageData;

	pOnChangedVillageAttackType		=	(OnChangedVillageAttackType *)pMsg;

	// 마을 데이터 헤더 얻기
	lpVillageHeader = _pWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(pOnChangedVillageAttackType->uiVillageCode);

	if (lpVillageHeader)
	{
		// 마을 정보(설명) 데이터 얻기
		lpSVillageData = _pWorld->pOnlineVillageDescriptList->GetVillageDataData(lpVillageHeader->uiVillageCode);				
		
		if (lpSVillageData)
		{
			// 마을 정보 데이터를 다운로드 중이 아니며
			if(lpSVillageData->m_fWait == FALSE)
			{
				lpSVillageData->m_ssCounterAttackType = pOnChangedVillageAttackType->siAttackType;	// 마을의 반격 타입 변경
			}
		}
		else
		{
			// 없으면 Server에 Data 요청
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
			// 만약에 공격한 캐릭터 Kind가 서버와 클라이언트에서 보여지는 캐릭터Kind가 다른 경우에 처리
			if (pOnResFieldAttackRdyMsg->uiKind != _pWorld->pIOnlineChar->GetKind(pOnResFieldAttackRdyMsg->uiAttackCharID))
			{
				DWORD		IP;
				CHAR		TempBuffer[128];

				// 캐릭 정보 얻기
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

				// 변신하는 캐릭터가 사용자 자신이 아닌 경우
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
					// 변신한 캐릭터의 상세 정보가 없다면
					if(_pWorld->pIOnlineChar->GetDataType(uiUniqueID) == 1)
					{
						if (_pWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
						{
							// 상세정보를 요청한다.
							_pWorld->pOnlineClient->SendPlayerDetailInfo(uiUniqueID);
							_pWorld->pIOnlineChar->SetDataType(uiUniqueID, 2);	// 정보 다운로드 대기 중...
						}
					}
				}
			}

			_pWorld->pIOnlineChar->AddOrder(pOnResFieldAttackRdyMsg->uiAttackCharID, ON_ORDER_FIELDATTACK_ACTION, siAttackCharX,siAttackCharY);

			//이부분을 어떻게 처리할것인지에 대해.
			//무기를 쓴 유저가 판단하여 맞았을때 처리한다.괜찮은 방법일듯 한데.일단.여기로.
			if(pOnResFieldAttackRdyMsg->uiAttackCharID == _pWorld->pMyData->GetMyCharUniqueID())
			{
				// robypark 2004/11/2 15:38
				// 공성전 유닛 사이의 공격 이펙트 생성
				_pWorld->SiegeWarfare_CreateAttackEffectCharacter(pOnResFieldAttackRdyMsg->uiAttackCharID, pOnResFieldAttackRdyMsg->uiEnemyCharID, true);
			}
			else
			{
				// robypark 2004/11/2 15:38
				// 공성전 유닛 사이의 공격 이펙트 생성
				_pWorld->SiegeWarfare_CreateAttackEffectCharacter(pOnResFieldAttackRdyMsg->uiAttackCharID, pOnResFieldAttackRdyMsg->uiEnemyCharID);
			}		
			/*
				// robypark 2004/11/3 19:49
				// 대장효과(공격력 업), 대장인지 등의 효과 처리 추가
				SI16 scGeneralEffect = _pWorld->SiegeWarfare_IsInAreaGeneralEffect();
				if (-1 != scGeneralEffect)
					_pWorld->pOnlineClient->SendFieldAttackAction(pOnResFieldAttackRdyMsg->uiEnemyCharID, scGeneralEffect);
			*/
		}
	}
	else if(pOnResFieldAttackRdyMsg->dwResponse == ON_RET_NO)
	{
		// robypark 2005/1/12 19:30
		// 공성전 공격 모드 설정 초기화
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

	//맞았을때의 처리.맞은 당사자에게만 이메세지가 나타난다.
	
	//_pWorld->pMyData->GetMyCharUniqueID() 자신의 아이디.

	pOnResFieldAttackActionMsg->uiShooterID;
	pOnResFieldAttackActionMsg->siDamage;
	pOnResFieldAttackActionMsg->bAlive;	// 공격자가 살아있나?

	//만약 죽으면 일반 캐릭터로 바뀌는 정보를 세팅하고 
	//나머지 사람들에게 내가 죽었다는걸 알린다.
	if(pOnResFieldAttackActionMsg->dwResponse == ON_RET_OK)
	{
		//맞아서 보낸 주인공이 나라면.
		if(pOnResFieldAttackActionMsg->uiEnemyID == _pWorld->pMyData->GetMyCharUniqueID())
		{
			//서버로 보낸다.
			//_pWorld->pIOnlineChar->GetKind(_pWorld->pMyData->GetMyCharUniqueID()) 이게 아니라 PrevKind를 보내야한다.

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

					// 변신 불가능 설정
					_pWorld->pMyData->SetTransformSiegeWarfareUnit(false);

					// 공성전 유닛이 아니다로 설정
					_pWorld->SetIsShowInformationAboutSiegeWarfareUnit(0);

					// robypark 2005/1/28 14:59
					// 공성전 유닛 인터페이스 닫기
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

		// 캐릭터 데미지 효과 처리
		SI16 siHeroX, siHeroY;
		_pWorld->pIOnlineChar->GetPos(pOnResFieldAttackActionMsg->uiEnemyID, &siHeroX, &siHeroY);

		SEffectData					EffectData;
		EffectData.m_Type = EFFECTTYPE_FIELD_CHANGENUMBER;
		EffectData.m_EffectData_FieldChangeNumber.m_Frame     = 0;
		EffectData.m_EffectData_FieldChangeNumber.m_MapXPos   = siHeroX;
		EffectData.m_EffectData_FieldChangeNumber.m_MapYPos   = siHeroY;
		EffectData.m_EffectData_FieldChangeNumber.m_bVillageDefence	= false;
		EffectData.m_EffectData_FieldChangeNumber.m_ChangeNumber    = -pOnResFieldAttackActionMsg->siDamage;

		// 아군 상단이 때린 경우 데미지는 녹색이다
		if (_pWorld->IsMemberOfSameGuild( pOnResFieldAttackActionMsg->uiShooterID ))
		{
			EffectData.m_EffectData_FieldChangeNumber.m_Color	= EFFECTDATA_FIELD_CHANGENUMBER_COLOR_GREEN;

			_pWorld->pOnlineFieldEffect->AddEffect(EffectData);
		}
		// 적군 상단원일 경우
		else if (_pWorld->pIOnlineChar->GetIsHostilityGuild( pOnResFieldAttackActionMsg->uiShooterID ))
		{
			EffectData.m_EffectData_FieldChangeNumber.m_Color	= EFFECTDATA_FIELD_CHANGENUMBER_COLOR_RED;

			_pWorld->pOnlineFieldEffect->AddEffect(EffectData);
		}
		// else	// 그 외 상단원인 경우

		// robypark 2004/14:32
		// 치명적 공격, 견고한 방어, 일반 공격, 방어일 경우 효과 출력
		switch (pOnResFieldAttackActionMsg->siAttackType)
		{
			// 치명적 공격. 공격자 캐릭터에 효과 추가
			case ON_RET_ATTACK_POWER:
				{
					// 선택한 캐릭터 표시 이펙트를 치명적 공격 사용에도 사용한다.
					_pWorld->pIOnlineChar->SetNewSelectedFlag(pOnResFieldAttackActionMsg->uiShooterID, TRUE);
					_pWorld->pIOnlineChar->SetSiegeWarfare_DeathBlow(pOnResFieldAttackActionMsg->uiShooterID);
					PushEffect(EFFECT_CRITICAL, 0 , 0);	// 치명적 공격 효과음 출력
				}
				break;
			// 견고한 방어. 피격자 캐릭터에 효과 추가
			case ON_RET_DEFENCE_POWER:
				{
					_pWorld->pIOnlineChar->SetSiegeWarfare_Evasion(pOnResFieldAttackActionMsg->uiEnemyID);
					PushEffect(EFFECT_DEFENCE, 0 , 0);	// 견고한 방어 효과음 출력
				}
				break;
			// 일반 공격, 방어
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

	// 죽는 공성 유닛이 사용자일 경우
	if(pOnResFieldAttackDeadMsg->uiDeadCharID == _pWorld->pMyData->GetMyCharUniqueID())
	{
		// 공성전 유닛이 아니다로 설정
		_pWorld->SetIsShowInformationAboutSiegeWarfareUnit(0);
	}

	//pOnResFieldAttackDeadMsg->uiDeadCharID 가 죽었다는걸 뿌려준다.
	//처리방법은 나중에 처리한다.
	//pOnResFieldAttackDeadMsg->uiShoorterID 가 마지막으로 때려 준인걸로 뿌려줘도 되고..아니면 나중에 빼고.
	return 0;
}

// 관청에서 대방이나 행수가 병력 모집을 요청한 응답 메시지 처리
UI16 OnlineMsgProc::OnIncreaseGuildUnit(BYTE* pMsg)
{
//	OnResIncreaseGuildUnit*			pOnResIncreaseGuildUnit;

//	pOnResIncreaseGuildUnit			=	(OnResIncreaseGuildUnit*)pMsg;
	OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	if (NULL == pStructure)
	{
		return 0;
	}

	// 마을 건물안 일 경우, 즉 관청일 경우에는 관청 병력모집 인터페이스에서 메시지 처리
	return pStructure->OnIncreaseGuildUnit(pMsg);
}

// 상단에 있는 공성전 유닛 부대를 소집권 아이템으로 변환한 결과
UI16 OnlineMsgProc::OnDecreaseGuildUnit(BYTE* pMsg)
{
//	OnResDecreaseGuildUnit*			pOnResDecreaseGuildUnit;

//	pOnResDecreaseGuildUnit			=	(OnResDecreaseGuildUnit*)pMsg;

	return _pWorld->pOnlineTradeBook->OnDecreaseGuildUnit(pMsg);

	return 0;
}

// robypark 2004/11/16 15:36
// 공성전 상단 공성 유닛 리스트 요청 응답
// ON_RESPONSE_LIST_GUILD_UNIT
UI16 OnlineMsgProc::OnResponseSiegeWarfareUnitList(BYTE *pMsg)
{
	// OnResGetGuildUnitList	*pOnResGetGuildUnitList = (OnResGetGuildUnitList *)pMsg;
	// 마을 건물안 일 경우, 즉 관청일 경우에는 관청 병력모집 인터페이스에서 메시지 처리
	OnlineGovernMgr *pStructure = NULL;

	if (NULL != _pWorld->pOnlineVillage)
		pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

	// 관청
	if (NULL != pStructure)
	{
		return pStructure->OnResponseSiegeWarfareUnitList(pMsg);
	}

	// 보유재산 윈도
	if (_pWorld->pOnlineTradeBook->IsActivityBook(ON_BOOK_SIEGEWARFARE))
	{
		// 대방 인터페이스에서 메시지 처리
		_pWorld->pOnlineTradeBook->OnResponseSiegeWarfareUnitList(pMsg);
	}

	// 공성전 변신 윈도
	_pWorld->pOnlinePannel->OnResponseSiegeWarfareUnitList(pMsg);

	return 0;
}

//-----------------------------------------------------------------------------
// Name: OnBroadCast_CurrentServerTime()
// Code: robypark (2004/9/14)
// Desc: ON_RESOPNSE_REAL_TIME 신호 접수. Server -> Client 용
//		현재 서버 시간을 알려준다. 년, 월, 일에 따라 이벤트 퀘스트를 활성화
//		비활성화해준다.
//	pMsg	- 이 메시지와 딸려온 정보( 현재 년, 월, 일 정보 )
//	return	- 제대로 왔다면 0
//-----------------------------------------------------------------------------
UI16 OnlineMsgProc::OnBroadCast_CurrentServerTime( BYTE *pMsg )
{
	// 서버 시간 구조체로 캐스팅
	OnResRealTime *lpOnResRealTime = (OnResRealTime *)pMsg;

	// 국가명 얻기
	char szHQNation[128];
	sprintf(szHQNation, "%s", (char*)pGame->m_pszHQNation);

	// 퀘스트 진행 대화상자 닫기
	if (TRUE == _pWorld->pOnlineQuestPlay->IsAction())
		_pWorld->pOnlineQuestPlay->SetAction(FALSE);

	// 퀘스트 목록 대화상자 닫기
	if (TRUE == _pWorld->m_pQuestInterface->IsAction())
		_pWorld->m_pQuestInterface->OnButtonPush();		

	// 퀘스트 파서 초기화
	_pWorld->pQuestParser->Free();

	// 이벤트 퀘스트가 유효한지에 대한 재처리(로딩)
	_pWorld->pQuestParser->ReadText("Online\\Quest\\quest.txt", szHQNation, lpOnResRealTime->uiYear, lpOnResRealTime->uiMon, lpOnResRealTime->uiDay);
	
	// 이벤트 기간, 국가에 따른 이벤트 퀘스트를 주는 NPC 활성화, 비활성화에 대한 처리
	_pWorld->DisableEventNPC();

	_pWorld->m_pQuestInterface->SetQuestList();

	return 0;
}

// robypark 2005/1/29 14:25
// 공성 중에 보급수레가 자신의 마나회복 요청 응답
// ON_RESPONSE_CHARGE_MP
UI16	OnlineMsgProc::OnResponseChargeMP(BYTE *pMsg)
{
	OnResChargeMP	*pOnResChargeMP	= (OnResChargeMP *)pMsg;

	// 성공
	if (ON_RET_OK == pOnResChargeMP->dwResponse)
	{
		SI32 siPrevMP = _pWorld->pIOnlineChar->GetSiegeWarfare_CurrentSupplyGoodsAmount(_pWorld->pMyData->GetMyCharUniqueID());

		// 마나 회복된 값 설정
		_pWorld->pIOnlineChar->SetSiegeWarfare_CurrentSupplyGoodsAmount(_pWorld->pMyData->GetMyCharUniqueID(), pOnResChargeMP->siNowMP);

		/*
		// 변화된 마나량 량 값 출력
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
// 공성전 보급 수레로 부터 보급품 받기
// ON_RESPONSE_GET_SUPPLY_AT_GUILD_UNIT
UI16 OnlineMsgProc::OnResponseSupplyGoodsFromSupplyWagon( BYTE *pMsg )
{
	OnResGetSupplyAtGuildUnit *pOnResGetSupplyAtGuildUnit = (OnResGetSupplyAtGuildUnit *)pMsg;

	// 치료 받은 캐릭터가 사용자 자신인 경우
//	if (pOnResGetSupplyAtGuildUnit->uiUniqueID == _pWorld->pMyData->GetMyCharUniqueID())
	{
		// 새로운 체력값 설정
		_pWorld->pIOnlineChar->SetSiegeWarfare_Hp(pOnResGetSupplyAtGuildUnit->uiTagetID, pOnResGetSupplyAtGuildUnit->siHP);

		PushEffect(EFFECT_HEAL, 0 , 0);	// 효과음 출력
	}

	// 치료 효과 출력
	if ((_pWorld->IsMemberOfSameGuild( pOnResGetSupplyAtGuildUnit->uiTagetID ))		// 아군 상단이거나
		|| (_pWorld->pIOnlineChar->GetIsHostilityGuild( pOnResGetSupplyAtGuildUnit->uiTagetID )))	// 적군 상단원인 경우
	{
		// 치료 받는 이펙트 추가
		_pWorld->pIOnlineChar->SetSiegeWarfare_EffectSupplyGoods(pOnResGetSupplyAtGuildUnit->uiTagetID, EFFECT_CONSTANT_SUPPLYGOODS_FILLED);

		// 치료 받은 량 값 출력
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

	// 보급 수레의 보급품 보유량 수정
	_pWorld->pIOnlineChar->SetSiegeWarfare_CurrentSupplyGoodsAmount(pOnResGetSupplyAtGuildUnit->uiUniqueID, pOnResGetSupplyAtGuildUnit->siNowMP);

	/*
	// 보급 수레가 사용자 자신일 경우
	if (pOnResGetSupplyAtGuildUnit->uiUniqueID == _pWorld->pMyData->GetMyCharUniqueID())
	{
		// 변화된 마나량 량 값 출력
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
		// 보급 수레의 남은 보급품 바닥난 효과 추가(ran out)
	}
*/
	return 0;
}

// robypark 2004/11/9 18:46
// 공성전 보급 수레가 마을에서 보급품 받기
// ON_RESPONSE_DECREASE_GUILD_SUPPLY
UI16 OnlineMsgProc::OnResponseSupplyGoodsFromVillage( BYTE *pMsg )
{
	OnResDecreaseGuildSupply	*pOnResDecreaseGuildSupply = (OnResDecreaseGuildSupply *)pMsg;

	if (ON_RET_OK == pOnResDecreaseGuildSupply->dwResponse)
	{
		// 보급 수레가 보급품을 받았다는 이펙트 추가
		_pWorld->pIOnlineChar->SetSiegeWarfare_EffectSupplyGoods(_pWorld->pMyData->GetMyCharUniqueID(), EFFECT_CONSTANT_SUPPLYGOODS_FILLED);

		PushEffect(EFFECT_SUPPLY, 0 , 0);	// 효과음 출력

		// 보급 수레에 보급품 새로 설정
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
// 화친제의 수락 응답
// ON_RESPONSE_GUILD_FRIENDLY_MESSAGE_ACCEPT
UI16 OnlineMsgProc::OnResponseGuildFriendlyMessage_Accept(BYTE *pMsg)
{
	_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", _pWorld->pOnlineText->Get(ON_TEXT_ACCEPT_OFFERED_PEACE_MESSAGE));

	// 관청에서 보고 있는 경우
	OnlineGovernMgr *pStructure = NULL;

	if (NULL != _pWorld->pOnlineVillage)
		pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();
	// 상단리스트 갱신하기

	return 0;
}

// robypark 2004/11/26 12:47
// 화친제의 거절 응답
// ON_RESPONSE_GUILD_FRIENDLY_MESSAGE_CANCEL
UI16 OnlineMsgProc::OnResponseGuildFriendlyMessage_Cancel(BYTE *pMsg)
{
	_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", _pWorld->pOnlineText->Get(ON_TEXT_DECLINE_OFFERED_PEACE_MESSAGE));

	// 상단리스트 갱신하기

	return 0;
}

// 수성하기 인터페이스로 들어가기 요청에 대한 응답 처리
// ON_RESPONSE_WAR_VILLAGE_DEFENCE
UI16	OnlineMsgProc::OnResponseEnterVillageDefenceInterface(BYTE *pMsg)
{
	_pWorld->OnResponseEnterVillageDefenceInterface(pMsg);

	return 0;
}

/*
// 수성하기 인터페이스에서 마을로 들어가기 요청에 대한 응답 처리
// ON_RESPONSE_WAR_VILLAGE_IN
UI16	OnlineMsgProc::OnResponseEnterVillageFromVillageDefenceInterface(BYTE *pMsg)
{
	_pWorld->OnResponseEnterVillageFromVillageDefenceInterface(pMsg);

	return 0;
}
*/

// 수성하기 인터페이스에 나온다. 필드로
// ON_RESPONSE_VILLAGE_DEFFNCE_STOP
UI16	OnlineMsgProc::OnResponseLeaveVillageDefenceInterface(BYTE *pMsg)
{
	_pWorld->OnResponseLeaveVillageDefenceInterface(pMsg);

	return 0;
}

// 수성하기 인터페이스에서 공격 속성 등의 초기화 정보 얻기
// ON_RESPONSE_VULLAGE_ATTACK_TYPE_CHECK
UI16	OnlineMsgProc::OnResponseVullageAttackTypeCheck(BYTE *pMsg)
{
	_pWorld->OnResponseVullageAttackTypeCheck(pMsg);

	return 0;
}
