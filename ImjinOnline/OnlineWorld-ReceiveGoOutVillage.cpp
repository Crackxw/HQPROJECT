#include <GSL.h>
#include <clGame.h>
#include <OnlineWorld.h>
#include <DebugTool.h>
#include "ChattingServer\\HQChattingServerClientSocket.h"
#include "OnlineGameMsg.h"
#include "OnlineClient.h"
#include "OnlineVillage.h"
#include "OnlineMiniMap.h"
#include "OnlineTrade.h"
#include "OnlineBooth.h"

// 공성 유닛 관리자
#include	"SiegeUnitTableMgr.h"

extern _clGame* pGame;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	마을을 나가기 위한 초기화
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::InitReceiveGoOutVillage()
{
	// 마을을 나간다는 메시지를 보낸다.
	pOnlineClient->SendGoOutVillage();	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	마을을 나가기 위한 작업
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI16	cltOnlineWorld::ReceiveGoOutVillage()
{	// 수신 버퍼에 있는 데이타를 처리한다.
	UI16 uiProc = pOnlineClient->DataProc();
	if( uiProc )	return ReceiveMessageProc( uiProc );
	return 0;
}

UI16 cltOnlineWorld::LeaveVillage(BYTE *pMsg)
{
	UI16									uiNextMenu  = 0;
	SI32									i;	
	OnCharBaseInfo*							lpOnCharBaseInfo;
	OnCharMonsterInfo*						lpOnCharMonsterInfo;
	OnCharPlayerInfo*						lpOnCharPlayerInfo;
	OnCharShipInfo*							lpOnCharShipInfo; 
	OnlineReceiveFieldItemInfo				*pReceivedFieldItemInfo;
	BYTE									TempBuffer[128];
	DWORD									IP;
	SI32									TempCount1;
	OnBaseBoothItem							*lpOnBaseBoothItem, *pTempOnBaseBoothItem;
	SellItemInfo							TempSellItemInfo;


	OnResGoOutVillageMsg *pResGoOutVillageMsg = (OnResGoOutVillageMsg*)pMsg;
	
	if(pResGoOutVillageMsg)
	{
		// 서버로 부터 나가도 좋다는 허락을 받았다.
		if(pResGoOutVillageMsg->dwResponse == ON_RET_LEAVE_VILLAGE_OK)
		{
			// 수성인터페이스 관련 리소스 해제
			FreeVillageDefenceInterface();

			OnResGoOutVillageOkMsg	*pGoOutVillageOkMsg;
			
			pGoOutVillageOkMsg		=	(OnResGoOutVillageOkMsg*)pResGoOutVillageMsg;
			
			// 주인공 캐릭터의 위치를 기준으로 맵을 설정한다.				
			lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)pGoOutVillageOkMsg + sizeof(OnResGoOutVillageOkMsg));
			for(i = 0; i < pGoOutVillageOkMsg->uiPlayerNum; i++)
			{	
				// 한양에서 나온다면 하트비트 한번 보내기
				if(pOnlineVillage->GetVillageHeader()->uiVillageCode == 101 && i == (SI16)(pGoOutVillageOkMsg->uiPlayerNum / 2))
					SendHeartBeat();

				if(ISPLAYER(lpOnCharBaseInfo->uiAccount))
				{
					lpOnCharPlayerInfo = (OnCharPlayerInfo*)lpOnCharBaseInfo;

					if(pMyData->GetMyCharUniqueID() == lpOnCharPlayerInfo->uiAccount)
					{					
						// 주인공 캐릭터의 위치를 기준으로 맵을 설정한다.				
						pOnlineMap->SetXY(lpOnCharPlayerInfo->siX, lpOnCharPlayerInfo->siY);
						pOnlineMap->SetCurrentFocusCharID(pMyData->GetMyCharUniqueID());

						pOnlineClientChatting->MessageSend_MoveSell(pOnlineMap->m_MapIndex, lpOnCharPlayerInfo->siX, lpOnCharPlayerInfo->siY);
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
			
			// 캐릭터의 대한 정보를 설정한다.	
			lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)pGoOutVillageOkMsg + sizeof(OnResGoOutVillageOkMsg));
			for(i = 0; i < pGoOutVillageOkMsg->uiPlayerNum; i++)
			{	
				if(pOnlineVillage->GetVillageHeader()->uiVillageCode == 101 && i == (SI16)(pGoOutVillageOkMsg->uiPlayerNum / 2))
					SendHeartBeat();	

				if(ISPLAYER(lpOnCharBaseInfo->uiAccount))
				{
					lpOnCharPlayerInfo = (OnCharPlayerInfo*)lpOnCharBaseInfo;

					ZeroMemory(TempBuffer, 128);
					if(pMyData->GetMyCharUniqueID() == lpOnCharPlayerInfo->uiAccount)
					{
						CopyMemory(TempBuffer, pMyData->GetMyCharName(), ON_ID_LENGTH);
						IP = lpOnCharPlayerInfo->siDBAccount;

						pIOnlineChar->Init(lpOnCharPlayerInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharPlayerInfo->uiKind, lpOnCharPlayerInfo->siX, lpOnCharPlayerInfo->siY, IP, 0, lpOnCharPlayerInfo->siFightingPower, this,(UI16*)lpOnCharPlayerInfo->uiEquipWeapon);
						pIOnlineChar->SetEquipItem(lpOnCharPlayerInfo->uiAccount, (UI16*)lpOnCharPlayerInfo->uiEquipWeapon);

						pIOnlineChar->SetSiegeWarfare_Hp(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->siFieldHp);
						pIOnlineChar->SetCaptain(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->bCaptain);
						pIOnlineChar->SetSiegeWarfare_CurrentSupplyGoodsAmount(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->siSupplyCharge);

						// robypark 2004/12/17 17:29
						// 수성인터페이스에 있는 캐릭터인가를 설정
						pIOnlineChar->SetInVillageDefenceInterface(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bVillageInLimit);
						pMyData->SetInVillageDefenceInterface(lpOnCharPlayerInfo->bVillageInLimit);

						// 길드 처리
						if(pMyData->GetGuildName())
						{
							pIOnlineChar->SetGuildData(lpOnCharPlayerInfo->uiAccount, pMyData->GetGuildName(), pMyData->GetGuildClass(), pMyData->GetGuildFlag());
						}
					}
					else
					{
						IP = lpOnCharPlayerInfo->siDBAccount;

						pIOnlineChar->Init(lpOnCharPlayerInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharPlayerInfo->uiKind, lpOnCharPlayerInfo->siX, lpOnCharPlayerInfo->siY, IP, 0, lpOnCharPlayerInfo->siFightingPower, this,(UI16*)lpOnCharPlayerInfo->uiEquipWeapon);
						pIOnlineChar->SetEquipItem(lpOnCharPlayerInfo->uiAccount, (UI16*)lpOnCharPlayerInfo->uiEquipWeapon);

						pIOnlineChar->SetSiegeWarfare_Hp(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->siFieldHp);
						pIOnlineChar->SetCaptain(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->bCaptain);
						pIOnlineChar->SetSiegeWarfare_CurrentSupplyGoodsAmount(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->siSupplyCharge);

						// robypark 2004/12/17 17:29
						// 수성인터페이스에 있는 캐릭터인가를 설정
						pIOnlineChar->SetInVillageDefenceInterface(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bVillageInLimit);

						// 공성 유닛 정보 헤더 얻기
						LPSSiegeUnitTable lpsSiegeUnitTable = GetSiegeUnitTableMgr()->GetSiegeUnitTable(lpOnCharPlayerInfo->uiKind);

						// 공성전 유닛이 맞을 경우
						if (NULL != lpsSiegeUnitTable)
						{
							// 공성전 유닛 사용자 캐릭터의 상세 정보가 없다면
							if(pIOnlineChar->GetDataType(lpOnCharPlayerInfo->uiAccount) == 1)
							{
								if (GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
								{
									// 상세정보를 요청한다.
									pOnlineClient->SendPlayerDetailInfo(lpOnCharPlayerInfo->uiAccount);
									pIOnlineChar->SetDataType(lpOnCharPlayerInfo->uiAccount, 2);	// 정보 다운로드 대기 중...
								}
							}
						}
					}
					pIOnlineChar->SetGuildData(lpOnCharPlayerInfo->uiAccount, pIOnlineChar->GetGuildName(lpOnCharPlayerInfo->uiAccount), lpOnCharPlayerInfo->siGuildInGrade, pIOnlineChar->GetGuildFlag(lpOnCharPlayerInfo->uiAccount));
					pIOnlineChar->SetMercenary(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->siVMercenaryKind);

					// 개인 상점
					if(lpOnCharPlayerInfo->siPrivateMarketItemNum >= 0) 
					{
						pIOnlineChar->SetBoothOpen(lpOnCharPlayerInfo->uiAccount, TRUE);
						lpOnBaseBoothItem = (OnBaseBoothItem*)((CHAR*)lpOnCharPlayerInfo + sizeof(OnCharPlayerInfo));
						pTempOnBaseBoothItem = lpOnBaseBoothItem;
						for(TempCount1 = 0; TempCount1 < lpOnCharPlayerInfo->siPrivateMarketItemNum; TempCount1++)
						{
							ZeroMemory(&TempSellItemInfo, sizeof(SellItemInfo));
							if(pTempOnBaseBoothItem->siType == LIST_ITEM)
								TempSellItemInfo.Item.uiItemID = pTempOnBaseBoothItem->uiProductID;
							else if(pTempOnBaseBoothItem->siType == LIST_FOLLOWER)
								TempSellItemInfo.Follower.uiKind = pTempOnBaseBoothItem->uiProductID;
							TempSellItemInfo.siType = pTempOnBaseBoothItem->siType;
							pIOnlineChar->SetBoothItem(lpOnCharPlayerInfo->uiAccount, pTempOnBaseBoothItem->uiBoothSlot, TempSellItemInfo);
							pTempOnBaseBoothItem++;
						}

						pOnlineBooth->SetBoothSpeech(lpOnCharPlayerInfo->uiAccount, lpOnBaseBoothItem, lpOnCharPlayerInfo->siPrivateMarketItemNum);
					}
					else
					{
						pIOnlineChar->SetBoothOpen(lpOnCharPlayerInfo->uiAccount, FALSE);
					}

					if(lpOnCharPlayerInfo->siPrivateMarketItemNum >= 0) lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharPlayerInfo) + (lpOnCharPlayerInfo->siPrivateMarketItemNum * sizeof(OnBaseBoothItem)));
					else                                                lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharPlayerInfo));
				}
				else if(ISMONSTER(lpOnCharBaseInfo->uiAccount))
				{
					lpOnCharMonsterInfo = (OnCharMonsterInfo*)lpOnCharBaseInfo;

					// 다른 캐릭터의 대한 정보를 설정한다.						
					ZeroMemory(TempBuffer, 128);
					IP = 0;
					pIOnlineChar->Init(lpOnCharMonsterInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharMonsterInfo->uiKind, lpOnCharMonsterInfo->siX, lpOnCharMonsterInfo->siY, IP, 0.0f, lpOnCharMonsterInfo->siFightingPower, this,NULL);
					
					lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharMonsterInfo));
				}
				else if(ISSHIP(lpOnCharBaseInfo->uiAccount))
				{
					lpOnCharShipInfo = (OnCharShipInfo*)lpOnCharBaseInfo;

					// 다른 캐릭터의 대한 정보를 설정한다.						
					ZeroMemory(TempBuffer, 128);
					IP = 0;
					pIOnlineChar->Init(lpOnCharShipInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharShipInfo->uiKind, lpOnCharShipInfo->siX, lpOnCharShipInfo->siY, IP, 0.0f, 0, this,NULL);

					lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharShipInfo));
				}
			}

			pReceivedFieldItemInfo	=	(OnlineReceiveFieldItemInfo*)((CHAR*)lpOnCharBaseInfo);		
			for(i = 0; i < pGoOutVillageOkMsg->uiFieldItemNum; i++)
			{
				// 필드에 아이템을 설정한다.
				pOnlineMap->SetID(pReceivedFieldItemInfo[i].siX, pReceivedFieldItemInfo[i].siY, 1, 1, 0, 0,
					pReceivedFieldItemInfo[i].uiItemID, pReceivedFieldItemInfo[i].uiQuantity);
			}
			
			// 미니맵을 사용하였으면 처리
			if(pOnlineVillage->LeaveMiniMapFlag)
			{
				pOnlineMiniMap->SetActive(TRUE);
			}
			::SetFocus(pGame->Hwnd);

			GotoField();
			uiNextMenu	=	ON_MAINPOLL_GAME;
		}	
	}
	return uiNextMenu;
}