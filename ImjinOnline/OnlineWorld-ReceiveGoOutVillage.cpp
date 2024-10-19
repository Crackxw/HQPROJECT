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

// ���� ���� ������
#include	"SiegeUnitTableMgr.h"

extern _clGame* pGame;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	������ ������ ���� �ʱ�ȭ
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::InitReceiveGoOutVillage()
{
	// ������ �����ٴ� �޽����� ������.
	pOnlineClient->SendGoOutVillage();	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	������ ������ ���� �۾�
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI16	cltOnlineWorld::ReceiveGoOutVillage()
{	// ���� ���ۿ� �ִ� ����Ÿ�� ó���Ѵ�.
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
		// ������ ���� ������ ���ٴ� ����� �޾Ҵ�.
		if(pResGoOutVillageMsg->dwResponse == ON_RET_LEAVE_VILLAGE_OK)
		{
			// �����������̽� ���� ���ҽ� ����
			FreeVillageDefenceInterface();

			OnResGoOutVillageOkMsg	*pGoOutVillageOkMsg;
			
			pGoOutVillageOkMsg		=	(OnResGoOutVillageOkMsg*)pResGoOutVillageMsg;
			
			// ���ΰ� ĳ������ ��ġ�� �������� ���� �����Ѵ�.				
			lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)pGoOutVillageOkMsg + sizeof(OnResGoOutVillageOkMsg));
			for(i = 0; i < pGoOutVillageOkMsg->uiPlayerNum; i++)
			{	
				// �Ѿ翡�� ���´ٸ� ��Ʈ��Ʈ �ѹ� ������
				if(pOnlineVillage->GetVillageHeader()->uiVillageCode == 101 && i == (SI16)(pGoOutVillageOkMsg->uiPlayerNum / 2))
					SendHeartBeat();

				if(ISPLAYER(lpOnCharBaseInfo->uiAccount))
				{
					lpOnCharPlayerInfo = (OnCharPlayerInfo*)lpOnCharBaseInfo;

					if(pMyData->GetMyCharUniqueID() == lpOnCharPlayerInfo->uiAccount)
					{					
						// ���ΰ� ĳ������ ��ġ�� �������� ���� �����Ѵ�.				
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
			
			// ĳ������ ���� ������ �����Ѵ�.	
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
						// �����������̽��� �ִ� ĳ�����ΰ��� ����
						pIOnlineChar->SetInVillageDefenceInterface(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bVillageInLimit);
						pMyData->SetInVillageDefenceInterface(lpOnCharPlayerInfo->bVillageInLimit);

						// ��� ó��
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
						// �����������̽��� �ִ� ĳ�����ΰ��� ����
						pIOnlineChar->SetInVillageDefenceInterface(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bVillageInLimit);

						// ���� ���� ���� ��� ���
						LPSSiegeUnitTable lpsSiegeUnitTable = GetSiegeUnitTableMgr()->GetSiegeUnitTable(lpOnCharPlayerInfo->uiKind);

						// ������ ������ ���� ���
						if (NULL != lpsSiegeUnitTable)
						{
							// ������ ���� ����� ĳ������ �� ������ ���ٸ�
							if(pIOnlineChar->GetDataType(lpOnCharPlayerInfo->uiAccount) == 1)
							{
								if (GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
								{
									// �������� ��û�Ѵ�.
									pOnlineClient->SendPlayerDetailInfo(lpOnCharPlayerInfo->uiAccount);
									pIOnlineChar->SetDataType(lpOnCharPlayerInfo->uiAccount, 2);	// ���� �ٿ�ε� ��� ��...
								}
							}
						}
					}
					pIOnlineChar->SetGuildData(lpOnCharPlayerInfo->uiAccount, pIOnlineChar->GetGuildName(lpOnCharPlayerInfo->uiAccount), lpOnCharPlayerInfo->siGuildInGrade, pIOnlineChar->GetGuildFlag(lpOnCharPlayerInfo->uiAccount));
					pIOnlineChar->SetMercenary(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->siVMercenaryKind);

					// ���� ����
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

					// �ٸ� ĳ������ ���� ������ �����Ѵ�.						
					ZeroMemory(TempBuffer, 128);
					IP = 0;
					pIOnlineChar->Init(lpOnCharMonsterInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharMonsterInfo->uiKind, lpOnCharMonsterInfo->siX, lpOnCharMonsterInfo->siY, IP, 0.0f, lpOnCharMonsterInfo->siFightingPower, this,NULL);
					
					lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharMonsterInfo));
				}
				else if(ISSHIP(lpOnCharBaseInfo->uiAccount))
				{
					lpOnCharShipInfo = (OnCharShipInfo*)lpOnCharBaseInfo;

					// �ٸ� ĳ������ ���� ������ �����Ѵ�.						
					ZeroMemory(TempBuffer, 128);
					IP = 0;
					pIOnlineChar->Init(lpOnCharShipInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharShipInfo->uiKind, lpOnCharShipInfo->siX, lpOnCharShipInfo->siY, IP, 0.0f, 0, this,NULL);

					lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharShipInfo));
				}
			}

			pReceivedFieldItemInfo	=	(OnlineReceiveFieldItemInfo*)((CHAR*)lpOnCharBaseInfo);		
			for(i = 0; i < pGoOutVillageOkMsg->uiFieldItemNum; i++)
			{
				// �ʵ忡 �������� �����Ѵ�.
				pOnlineMap->SetID(pReceivedFieldItemInfo[i].siX, pReceivedFieldItemInfo[i].siY, 1, 1, 0, 0,
					pReceivedFieldItemInfo[i].uiItemID, pReceivedFieldItemInfo[i].uiQuantity);
			}
			
			// �̴ϸ��� ����Ͽ����� ó��
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