#include <GSL.h>

#include	<clGame.h>
#include "OnlineWorld.h"
#include	"ChattingServer\\HQChattingServerClientSocket.h"
#include "OnlineGameMsg.h"
#include "OnlineTradeBook.h"
#include "OnlineClient.h"
#include "OnlineMsgBox.h"
#include	"OnlineListControl.h"
#include "OnlineInventory.h"
#include "OnlineCharStatus.h"
#include "OnlineFont.h"
#include "OnlineMegaText.h"
#include "OnlineTrade.h"
#include "OnlineBooth.h"
#include "OnlineMFGS.h"
#include "OnlineVersion.h"
#include "..\\SCCommon\\MacroCheckProcess.h"
#include	"OnlineQuest-Parser.h"
#include	<OnlineSMS.h>		// actdoll (2004/06/15 10:46) : SMS �� ������ �߰�
#include	<OnlinePannel.h>

// ���� ���� ������
#include	"SiegeUnitTableMgr.h"

extern _clGame* pGame;

#define _USE_ON_RESPONE_WAR_TIME

enum
{   
	ON_GAMESERVER_LOGIN_INIT				= 0, 
	ON_GAMESERVER_LOGIN_POLL
};


//----------------------------------------------------------------------------------------------------------------
//	����	:	�����κ��� ���� ����Ÿ�� ������ ȣ���ϴ� �Լ�.(�ʱ�ȭ)
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::InitGameServerLogIn()
{
	// ó������ �� ���ν����� ���Դٸ� �޸𸮸� �Ҵ� �޴´�.	
	pOnlineLogInMgr->m_GameServerLogIn.siStatus= 0;

	// ���� ���ϵ鿡 ���� ��� ����Ÿ�� ���ش�.
	pMyData->InitFollowers();
	
	pOnlineLogInMgr->m_GameServerLogIn.siLogInTime			=	timeGetTime();
	pOnlineLogInMgr->m_GameServerLogIn.bMessageBoxFloating	=	TRUE;
}


//----------------------------------------------------------------------------------------------------------------
//	����	:	�����κ��� ���� ����Ÿ�� ���� �޴´�.
//----------------------------------------------------------------------------------------------------------------
SI08	cltOnlineWorld::GameServerLogIn()
{
	SI32							i, TempCount, TempCount1;
	OnGameServerLogInMsgData*		lpOnGameServerLogInMsgData;
	OnCharBaseInfo*					lpOnCharBaseInfo;
	OnCharMonsterInfo*				lpOnCharMonsterInfo;
	OnCharPlayerInfo*				lpOnCharPlayerInfo;
	OnCharShipInfo*					lpOnCharShipInfo;
	OnlineReceiveFieldItemInfo		*pRecvFieldItemInfo;
	OnFollowerInfo					*pRecvFollowerInfo;
	OnlineReceivedMyItemInfo*		lpMyItemInfo;
	BYTE							TempBuffer[128];
	BYTE							TempBuffer1[256];
	DWORD							IP;
	OnBaseBoothItem					*lpOnBaseBoothItem, *pTempOnBaseBoothItem;
	SellItemInfo					TempSellItemInfo;

	switch(pOnlineLogInMgr->m_GameServerLogIn.siStatus)
	{
	case ON_GAMESERVER_LOGIN_INIT:
		{
			pOnlineClient->SendGameServerLogIn();
			pOnlineLogInMgr->m_GameServerLogIn.siStatus = ON_GAMESERVER_LOGIN_POLL;
		}
		break;
	case ON_GAMESERVER_LOGIN_POLL:
		{
			// ���ῡ �����Ͽ� ���� � �޽��� �ڽ��� ���ִ� �����̴�.	
			if(m_siMsgBoxStatus == ON_MSGBOX_TYPE_OK && pOnlineLogInMgr->bStatusBack)
			{
				return ON_RETURN_EXIT;
			}

			SI16 siTempCount = max(10 - ((timeGetTime() - pOnlineLogInMgr->m_GameServerLogIn.siLogInTime) / 1000), 0);

			if(siTempCount < 0)		// 10�ʰ� ���
			{
				if(!pOnlineLoginMsgBox->IsActive())
				{
					pOnlineLoginMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pOnlineText->Get(ON_TEXT_DATA_GAMESERVERCONNECTERROR));
					pOnlineLogInMgr->bStatusBack = TRUE;
				}
				break;
			}
		
			OnGameServerLogInMsg *pLogInMsg = (OnGameServerLogInMsg*)pOnlineClient->GetMsgBuffer(ON_RESPONSE_LOGIN);

			// ������ ���� ���� ����Ÿ�� �޾Ҵ�.	
			if(!pLogInMsg)		break;
			
			if(pLogInMsg->dwResponse == ON_RET_OK)
			{				
				pOnlineClient->m_BlackPig.SetAutoKey(TRUE);

				bReturnKeyDown = FALSE;
				m_GameTickCount = GetTickCount();
				m_GameTime = timeGetTime();
				SendHeartBeat(TRUE);

				SI16 siCount = 1;
				
				// �� �����͵��� �ʱ�ȭ �����ش�.
				lpOnGameServerLogInMsgData = (OnGameServerLogInMsgData*)(pLogInMsg + 1);

				// ��� ������ �޴´�.
				pMyData->siMaxGeneral = lpOnGameServerLogInMsgData->siMaxGeneral;

				m_GMFlag   = lpOnGameServerLogInMsgData->USGrade;
				m_InfoFlag = TRUE;
				
				// Time�� �����Ѵ�.
				pTimer->SetNowTime(lpOnGameServerLogInMsgData->Time);
				fFirstGetTime = FALSE;	// ������ �������� ���� �ð��� ó������ �����Ѵ�.(���� �ð��� �ִ� 2���̶�� ������ ����Ƿ�)
				m_RealTime    = lpOnGameServerLogInMsgData->RealTime;
				
				// ���� ���� �����Ѵ�.
				pMyData->SetMoney(lpOnGameServerLogInMsgData->siMoney);		
				
				// ���� ���ݾ��� �����Ѵ�. (������ �ϴ� 0��)
				pMyData->SetSaving(lpOnGameServerLogInMsgData->moSaving);

				// �ſ뵵�� �����Ѵ�.
				pMyData->SetBatleCredit(0, 0);
				pMyData->SetTradeCredit(lpOnGameServerLogInMsgData->uiTradeCredit, 0);
				
				// ���� ������� ���� �Ѵ�.			
				pMyData->SetHealth(lpOnGameServerLogInMsgData->siHealth);			
				
				// ���� Name�� �����Ѵ�.
				ZeroMemory(TempBuffer, 128);
				CopyMemory(TempBuffer, lpOnGameServerLogInMsgData->szID, ON_ID_LENGTH);
				pMyData->SetMyCharName((char*)TempBuffer);
				pMyData->SetMyCharUniqueID(lpOnGameServerLogInMsgData->uiMyCharUniqueID);
				
				// ����ٴϴ� �뺴
				pMyData->SetMercenary(lpOnGameServerLogInMsgData->siVMercenaySlot);
				
				// ��带 �����Ѵ�.
				if(lpOnGameServerLogInMsgData->szGuildID[0])
					pMyData->SetGuild((char*)lpOnGameServerLogInMsgData->szGuildID, lpOnGameServerLogInMsgData->siGuildClass, lpOnGameServerLogInMsgData->siFlag);
				else
					pMyData->SetGuild(NULL, 0, 0);

				// robypark 2004/11/19 17:1
				// �������� ���� ���� �� ���ΰ� ĳ���� DB ID ����
				pMyData->SetPrevCharOnlineKind(lpOnGameServerLogInMsgData->m_uiPrevKind);

				// robypark 2004/12/7 12:48
				// ������ ������ ���¿��� ���� �ڿ� �ٽ� ���� �������� �����ϱ� ���� �����ð� ����
				pMyData->SetTransformSiegeWarfareUnit((lpOnGameServerLogInMsgData->m_dwDeadSec <= 0) ? true : false);
				
				// �켱 ���� ���ΰ��� ��ġ�� �ű��.
				lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnGameServerLogInMsgData + sizeof(OnGameServerLogInMsgData));

#ifdef _USE_ON_RESPONE_WAR_TIME
				// robypark 2005/2/16 3:0
				// ���� ON_RESPONE_WAR_TIME �޽����� ����Ѵٸ�, ������ ���� �����ؼ��� �ȵȴ�.
				// ���� �ð��̵Ǹ�, �������� ������ �ִٰ� �˷��ش�.
				if (IsInTimeSiegeWarfare())	// �����ð��̶��
				{
					// ���� �������� �ִ���?
					pMyData->SetSiegeWarfareDay(lpOnGameServerLogInMsgData->m_bGuildWarDay);
				}
#else
				// �׷��� ON_RESPONSE_NOT_WAR �޽ø� ����� ��� ������ �ִٰ� �������ָ�,
				// �����ð� ���� ȭģ���Ǹ� ���Ͽ� �� �̻� ������ ������ ��쿡 �޽��� ó����
				// ���Ͽ� �������θ� �Ǵ��Ѵ�.
				// ���� �������� �ִ���?
				pMyData->SetSiegeWarfareDay(lpOnGameServerLogInMsgData->m_bGuildWarDay);
#endif

				// ���ΰ� ���(�ǳ��� ó���� ����)
				m_scSiegeTransformMode = SIEGE_TRANSFORM_MODE_HERO;

				// robypark 2005/1/17 11:0
				// �ǳ������� ���� ���� ó��
				if (IsInTimeSiegeWarfare())	// �����ð��̶��
				{
					if (lpOnGameServerLogInMsgData->m_bGuildWarDay)	// �����ϴ� ���� ������̸�,
					{
						// �ǳ��� ���
						m_scSiegeTransformMode = SIEGE_TRANSFORM_MODE_REFUGEE;
					}

					// robypark 2005/1/29 13:32
					// �ʺ��� ���������� ���� �� �� �ִٴ� ��� �޽��� ����
					m_sWarningMsgInSiegeWarfare.m_dwLeftTime = 0;

				}
				else
				{
					// robypark 2005/1/29 13:32
					// �ʺ��� ���������� ���� �� �� �ִٴ� ��� �޽��� ����
					m_sWarningMsgInSiegeWarfare.m_dwLeftTime = WARNINGMSGINSIEGEWARFARE_DELAY;
				}

				// robypark 2005/1/29 14:6
				// ���޼���(�޵�)�� ��� 10�ʸ��� ������ 10%�� ȸ���ȴ�.
				m_sRecoveryMana.m_bSendedRequest = FALSE;
				m_sRecoveryMana.m_dwLeftTime	 = m_SOptionSiegeWarfare.siMedicRechargeManaCoolTime;

				for(i = 0; i < lpOnGameServerLogInMsgData->uiPlayerNum; i++)
				{
					if(ISPLAYER(lpOnCharBaseInfo->uiAccount))
					{
						lpOnCharPlayerInfo = (OnCharPlayerInfo*)lpOnCharBaseInfo;

						if(pMyData->GetMyCharUniqueID() == lpOnCharPlayerInfo->uiAccount)
						{
							pOnlineMap->ChangeMap(lpOnGameServerLogInMsgData->uiMapID, lpOnCharPlayerInfo->siX, lpOnCharPlayerInfo->siY);

						}

/*						
						if(pMyData->GetMyCharUniqueID() == lpOnCharPlayerInfo->uiAccount)
						{
							// ���ΰ� ĳ������ ��ġ�� �������� ���� �����Ѵ�.			
							pOnlineMap->ChangeMap(lpOnGameServerLogInMsgData->uiMapID, lpOnCharPlayerInfo->siX, lpOnCharPlayerInfo->siY);
							// ���� ���� �ٳ�� �ϴ� ���� ID ���� �������ش�.
							pOnlineMap->SetCurrentFocusCharID(lpOnGameServerLogInMsgData->uiMyCharUniqueID);
							pOnlineMap->PlayFitMusic();
							
							// Chatting Server�� LogIn�� �Ѵ�.
							pOnlineClientChatting->MessageSend_LogIn(pMyData->GetMyCharName(), lpOnCharPlayerInfo->siDBAccount, m_GMFlag);
							
							// ���ΰ��� ������ġ�� ������ Ž�翡 ���̴� ������ ����
							m_CharPosition_X = lpOnCharPlayerInfo->siX;
							m_CharPosition_Y = lpOnCharPlayerInfo->siY;
						}
*/						
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
				
				SendHeartBeat();
				
				pRecvFieldItemInfo	=	(OnlineReceiveFieldItemInfo*)((CHAR*)lpOnCharBaseInfo);
				pRecvFollowerInfo	=	(OnFollowerInfo*)((CHAR*)pRecvFieldItemInfo + sizeof(OnlineReceiveFieldItemInfo) * lpOnGameServerLogInMsgData->uiFieldItemNum);
				
				// �� ���ϵ鿡 ���� ������ �����Ѵ�.
				for(i = 0; i < lpOnGameServerLogInMsgData->uiFollowerNum; i++)
				{	
					pMyData->InitFollower(pRecvFollowerInfo->uiSlot, pRecvFollowerInfo);
					
					lpMyItemInfo = (OnlineReceivedMyItemInfo*)((BYTE*)pRecvFollowerInfo + sizeof(OnFollowerInfo));

					for(TempCount = 0; TempCount < pRecvFollowerInfo->uiItemNum; TempCount++)
					{
						pMyData->AddItem(pRecvFollowerInfo->uiSlot, (MyItemData*)&lpMyItemInfo[TempCount]);
					}				
					
					pRecvFollowerInfo = (OnFollowerInfo*)((BYTE*)pRecvFollowerInfo + sizeof(OnFollowerInfo) + (sizeof(OnlineReceivedMyItemInfo) * pRecvFollowerInfo->uiItemNum));
				}
				
				// �ʵ��� �������� �����Ѵ�. 
				for(i = 0; i < 	lpOnGameServerLogInMsgData->uiFieldItemNum; i++)
				{
					pOnlineMap->SetID(pRecvFieldItemInfo[i].siX, pRecvFieldItemInfo[i].siY, 1, 1, 0, 0, pRecvFieldItemInfo[i].uiItemID, pRecvFieldItemInfo[i].uiQuantity);
				}
				
				// ������ �÷��̾ �����Ѵ�.
				lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnGameServerLogInMsgData + sizeof(OnGameServerLogInMsgData));

				for(i = 0; i < lpOnGameServerLogInMsgData->uiPlayerNum; i++)
				{	
					if(i == (SI16)(lpOnGameServerLogInMsgData->uiPlayerNum / 2))
						SendHeartBeat();
					
					ZeroMemory(TempBuffer, 128);
					if(ISPLAYER(lpOnCharBaseInfo->uiAccount))
					{
						lpOnCharPlayerInfo = (OnCharPlayerInfo*)lpOnCharBaseInfo;
						
						IP = lpOnCharPlayerInfo->siDBAccount;
						if(pMyData->GetMyCharUniqueID() == lpOnCharPlayerInfo->uiAccount)
						{	

							// ���� ĳ������ ���� ������ �����Ѵ�.		
							pMyData->SetMyCharDBAccount(IP);
							pMyData->SetMyCharKind(lpOnCharPlayerInfo->uiKind);
							
							CopyMemory(TempBuffer, pMyData->GetMyCharName(), ON_ID_LENGTH);

							pIOnlineChar->Init(lpOnCharPlayerInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharPlayerInfo->uiKind, lpOnCharPlayerInfo->siX, lpOnCharPlayerInfo->siY, IP, 0, lpOnCharPlayerInfo->siFightingPower, this,(UI16*)lpOnCharPlayerInfo->uiEquipWeapon);
							pIOnlineChar->SetAttackMode(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bAttackMode);
							pIOnlineChar->SetEquipItem(lpOnCharPlayerInfo->uiAccount, (UI16*)lpOnCharPlayerInfo->uiEquipWeapon);
							pIOnlineChar->SetSiegeWarfare_Hp(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->siFieldHp);
							pIOnlineChar->SetCaptain(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->bCaptain);
							pIOnlineChar->SetSiegeWarfare_CurrentSupplyGoodsAmount(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->siSupplyCharge);

							// ���� ////////////////////////////////
							// robypark 2004/11/9 21:16
							// ���� ���� ���� ��� ���
							LPSSiegeUnitTable lpsSiegeUnitTable = GetSiegeUnitTableMgr()->GetSiegeUnitTable(lpOnCharPlayerInfo->uiKind);

							// ������ ������ ���� ���
							if (NULL != lpsSiegeUnitTable)
							{
								SetIsShowInformationAboutSiegeWarfareUnit(ON_GAMEOPTION_SIEGEWARFAREUNIT);	// ������ �����̴�.

								// ���� ��ư ��Ȱ��ȭ
								pOnlinePannel->SetShowTransformSiegeUnitBtn(FALSE);

								// ������ ���ֿ� �������̽� Ȱ��ȭ
								pOnlinePannel->SiegeWarfareUnitUI_CreateWindow(SW_NORMAL);

								pOnlinePannel->ShowWindow_SiegeWarfareUnitUI(SW_NORMAL);
							}
							else
							{
								SetIsShowInformationAboutSiegeWarfareUnit(0);	// ������ ������ �ƴϴ�.

								// �ǳ��� ���
								if (SIEGE_TRANSFORM_MODE_REFUGEE == m_scSiegeTransformMode)
								{
									// ���� �����Ѱ�?
									if (pMyData->CanTransformSiegeWarfareUnit())
									{
										// ���� ��ư Ȱ��ȭ
										pOnlinePannel->SetShowTransformSiegeUnitBtn(TRUE);
									}
								}
							}

							// �� ////////////////////////////////

// by kuzimo

							// ���ΰ� ĳ������ ��ġ�� �������� ���� �����Ѵ�.			
//							pOnlineMap->ChangeMap(lpOnGameServerLogInMsgData->uiMapID, lpOnCharPlayerInfo->siX, lpOnCharPlayerInfo->siY);
							// ���� ���� �ٳ�� �ϴ� ���� ID ���� �������ش�.
							pOnlineMap->SetCurrentFocusCharID(lpOnGameServerLogInMsgData->uiMyCharUniqueID);
							pOnlineMap->PlayFitMusic();
							
							// Chatting Server�� LogIn�� �Ѵ�.
							pOnlineClientChatting->MessageSend_LogIn(pMyData->GetMyCharName(), lpOnCharPlayerInfo->siDBAccount, m_GMFlag);
							
							// ���ΰ��� ������ġ�� ������ Ž�翡 ���̴� ������ ����
							m_CharPosition_X = lpOnCharPlayerInfo->siX;
							m_CharPosition_Y = lpOnCharPlayerInfo->siY;

//---------------------
							
							pMyData->nRank = lpOnGameServerLogInMsgData->siEventRanking;
							pMyData->nScore = lpOnGameServerLogInMsgData->siEventScore;


							pIOnlineChar->SetRank(pMyData->GetMyCharUniqueID(), lpOnGameServerLogInMsgData->siEventRanking);
							pIOnlineChar->SetScore(pMyData->GetMyCharUniqueID(), lpOnGameServerLogInMsgData->siEventScore);					
							
							
							// �� ĳ������ �κ��丮�� �����Ѵ�.
							pOnlineInventory->Init(pMyData->GetMyCharUniqueID(), this);
							pOnlineTradeBook->m_pOnlineCharStatus->Init(pMyData->GetMyCharUniqueID(), this, pMyData->GetFollowersInterface());
							
							// ��� ó��
							if(pMyData->GetGuildName())
							{
								pIOnlineChar->SetGuildData(lpOnCharPlayerInfo->uiAccount, pMyData->GetGuildName(), pMyData->GetGuildClass(), pMyData->GetGuildFlag());
							}
						}
						else
						{
							pIOnlineChar->Init(lpOnCharPlayerInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharPlayerInfo->uiKind, lpOnCharPlayerInfo->siX, lpOnCharPlayerInfo->siY, IP, 0, lpOnCharPlayerInfo->siFightingPower, this,(UI16*)lpOnCharPlayerInfo->uiEquipWeapon);
							pIOnlineChar->SetAttackMode(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bAttackMode);
							pIOnlineChar->SetEquipItem(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->uiEquipWeapon);
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
						
						// ���� ���� ����
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
						IP = 0;
						pIOnlineChar->Init(lpOnCharMonsterInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharMonsterInfo->uiKind, lpOnCharMonsterInfo->siX, lpOnCharMonsterInfo->siY, IP, 0.0f, lpOnCharMonsterInfo->siFightingPower, this,NULL);
						
						lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharMonsterInfo));
					}
					else if(ISSHIP(lpOnCharBaseInfo->uiAccount))
					{
						lpOnCharShipInfo = (OnCharShipInfo*)lpOnCharBaseInfo;
						
						// �ٸ� ĳ������ ���� ������ �����Ѵ�.						
						IP = 0;
						pIOnlineChar->Init(lpOnCharShipInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharShipInfo->uiKind, lpOnCharShipInfo->siX, lpOnCharShipInfo->siY, IP, 0.0f, 0, this,NULL);
						
						lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharShipInfo));
					}
				}
				
				// ���� ���� �������� �󸶸� �������� ������ �ȷ�������
				if(lpOnGameServerLogInMsgData->YearProfit && lpOnGameServerLogInMsgData->SoldFacilityPrice)
				{
					sprintf((char*)TempBuffer1, pOnlineText->Get(ON_TEXT_PAY_INVESTMENT), lpOnGameServerLogInMsgData->YearProfit);
					pOnlineMsgBoxPay->SetMsg(ON_MSGBOX_TYPE_OK, "", (char*)TempBuffer1);
				}
				// ���� ���� �������� �󸶸� ������.
				else if(lpOnGameServerLogInMsgData->YearProfit)
				{
					sprintf((char*)TempBuffer1, pOnlineText->Get(ON_TEXT_PAY_INVESTMENT), lpOnGameServerLogInMsgData->YearProfit);
					pOnlineMsgBoxPay->SetMsg(ON_MSGBOX_TYPE_OK, "", (char*)TempBuffer1);
				}
				// ������ ��� �ȷ�������.
				else if(lpOnGameServerLogInMsgData->SoldFacilityPrice)
				{
					sprintf((char*)TempBuffer1, pOnlineText->Get(ON_TEXT_PAY_PRODUCTFACILITY), lpOnGameServerLogInMsgData->SoldFacilityPrice);
					pOnlineMsgBoxPay->SetMsg(ON_MSGBOX_TYPE_OK, "", (char*)TempBuffer1);
				}

				// actdoll (2004/07/19 10:43) : = �ð��� ������ =
				//	�Ⱓ�� ����Ǿ� �����Ǿ��ٸ� �˷��ش�.
				if( lpOnGameServerLogInMsgData->bCompareWearItem )
				{
					// �޽��� ���߿� �߰��� ��
					pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_ITEMHELP_TIMERITEM), pOnlineText->Get(ON_TEXT_NOTICE_ITEM_TIMERITEM_IS_DELETED) );
				}

				
				// �ε��� ���� ��û(�ӽ�)
				pOnlineClient->SendAuditOwnListMsg();
				
				// actdoll (2004/03/30 11:04) : = �̺�Ʈ = ��������Ʈ �߰���
				//		���⼭ ���� ����Ʈ�� ���� ����� ���ڸ� �޾ƿ����� �Ѵ�. ���� �̺�Ʈ ���� �Ŀ� ������ ��.
//				m_uiDokdoQuestCompleteUserCount	= lpOnGameServerLogInMsgData->uiDokdoQuestCompletedUserCount;

//				m_pMacroProcess->SetSendAble(TRUE);

				// robypark 2004/9/9 15:28
				// ���� �̺�Ʈ �Ⱓ�ΰ�?
				m_bIsInEventDay = lpOnGameServerLogInMsgData->bEvent;

				// robypark 2004/9/9 20:56
				// �̺�Ʈ �Ⱓ, ������ ���� �̺�Ʈ ����Ʈ�� �ִ� NPC ��Ȱ��ȭ�� ���� ó��
				DisableEventNPC();

				// robypark 2004/12/16 15:50
				// ���� �������� �ִ����� ǥ�����ش�.
				if (TRUE == lpOnGameServerLogInMsgData->m_bGuildWarDay)
				{
					pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", pOnlineText->Get(ON_TEXT_TODAY_IS_SIEGEWARFARE_DAY), m_SOptionSiegeWarfare.siSIEGEWARFARETIME_BEGIN, m_SOptionSiegeWarfare.siSIEGEWARFARETIME_END);
				}

				return ON_RETURN_OK;
			}
			else
			{
				// ���Ӽ��� �α��� ����
				pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", pOnlineText->Get(ON_TEXT_DATA_GAMESERVERCONNECTERROR));
				pOnlineLogInMgr->bStatusBack = TRUE;
			}
		}
		break;
	}

	return ON_RETURN_NONE;
}

VOID	cltOnlineWorld::DrawGameServerLogIn()
{
	HDC	hdc;
	if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
	{
		SetBkMode(hdc, TRANSPARENT);		
		SelectObject(hdc, pOnlineFonts->GetFont(ON_FONT_LOGIN));

		char temp[256];
		sprintf(temp, "Loading");
		SI16 siSLoginTime = (timeGetTime() - pOnlineLogInMgr->m_GameServerLogIn.siLogInTime) / 1000;
		for(int i=0; i<min(siSLoginTime, 10); i++)
			strcat(temp, ".");
		
		pOnlineMegaText->DrawTextCenter( hdc, 548, 304, 164, 20, temp, RGB(10, 10, 10));
		pOnlineMegaText->DrawTextCenter( hdc, 547, 303, 164, 20, temp, RGB(250, 250, 250));
		
		GETSCREENSURFACE(this)->ReleaseDC(hdc); 
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::FreeGameServerLogIn()
{
}

// robypark 2004/9/9 15:30
// ���� �̺�Ʈ �Ⱓ������ ���´�.
BOOL	cltOnlineWorld::IsInEventDay(void)
{
	return m_bIsInEventDay;
}

// robypark 2004/9/9 20:56
// �̺�Ʈ �Ⱓ, ������ ���� �̺�Ʈ ����Ʈ�� �ִ� NPC ��Ȱ��ȭ
VOID	cltOnlineWorld::DisableEventNPC(void)
{
	NPCIndexHeader*		lpNPCIndexHeader = NULL;
	SQuestHeader*		lpSQuestHeader	 = NULL;

	SI32 siNumberOfQuest = pQuestParser->m_QuestDataCount;

	for (SI32 index = 0; index < siNumberOfQuest; index++)
	{
		lpSQuestHeader = &pQuestParser->m_lpQuestData[index];

		if (NULL == lpSQuestHeader)
			continue;
/*
		// �̺�Ʈ �Ⱓ�� �ƴ� ���
		if (FALSE == m_bIsInEventDay)
		{
			// �̺�Ʈ�� ������ ����Ʈ NPC�� ��Ȱ��ȭ
			if (NORMAL_QUEST != lpSQuestHeader->m_siQuestEvent)
			{
				// ȥ�㸻 ���� ����
				lpNPCIndexHeader = pOnlineMap->m_NPCParser.GetQuestNPCAllMap(lpSQuestHeader->siNPCCode);

				if (NULL != lpNPCIndexHeader)
				{
					lpNPCIndexHeader->bTalkOneself = FALSE;

					// ����Ʈ NPC Ŭ�� ����
					lpNPCIndexHeader = pOnlineMap->m_NPCParser.GetNPC(lpNPCIndexHeader->siCode);

					if (NULL != lpNPCIndexHeader)
					{
						lpNPCIndexHeader->bTalkOneself = FALSE;
					}
				}
			}
		}
		// �̺�Ʈ �Ⱓ�̸�
		else
		{
*/			// �ش� ����(�Ż� Ŭ���̾�Ʈ ����)�� �ش���� �ʴ� ����Ʈ NPC ��Ȱ��ȭ
			if (QUEST_EVENT_CANCEL == lpSQuestHeader->m_siQuestEvent)
			{
				// ȥ�㸻 ���� ����
				lpNPCIndexHeader = pOnlineMap->m_NPCParser.GetQuestNPCAllMap(lpSQuestHeader->siNPCCode);

				if (NULL != lpNPCIndexHeader)
				{
					lpNPCIndexHeader->bTalkOneself = FALSE;

					// ����Ʈ NPC Ŭ�� ����
					lpNPCIndexHeader = pOnlineMap->m_NPCParser.GetNPC(lpNPCIndexHeader->siCode);

					if (NULL != lpNPCIndexHeader)
					{
						lpNPCIndexHeader->bTalkOneself = FALSE;
					}
				}
			}
			// robypark 2004/9/16 16:33
			// ����Ʈ NPC Ȱ��ȭ
			else if (QUEST_EVENT_PLAY == lpSQuestHeader->m_siQuestEvent)
			{
				// ȥ�㸻 �����ϵ��� ����
				lpNPCIndexHeader = pOnlineMap->m_NPCParser.GetQuestNPCAllMap(lpSQuestHeader->siNPCCode);

				if (NULL != lpNPCIndexHeader)
				{
					lpNPCIndexHeader->bTalkOneself = TRUE;

					// ����Ʈ NPC Ŭ�� �����ϵ��� ����
					lpNPCIndexHeader = pOnlineMap->m_NPCParser.GetNPC(lpNPCIndexHeader->siCode);

					if (NULL != lpNPCIndexHeader)
					{
						lpNPCIndexHeader->bTalkOneself = TRUE;
					}
				}
			}
//		}
	}

}
