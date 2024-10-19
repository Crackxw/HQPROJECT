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
#include	<OnlineSMS.h>		// actdoll (2004/06/15 10:46) : SMS 용 데이터 추가
#include	<OnlinePannel.h>

// 공성 유닛 관리자
#include	"SiegeUnitTableMgr.h"

extern _clGame* pGame;

#define _USE_ON_RESPONE_WAR_TIME

enum
{   
	ON_GAMESERVER_LOGIN_INIT				= 0, 
	ON_GAMESERVER_LOGIN_POLL
};


//----------------------------------------------------------------------------------------------------------------
//	설명	:	서버로부터 나의 데이타를 받을때 호출하는 함수.(초기화)
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::InitGameServerLogIn()
{
	// 처음으로 이 프로시저를 들어왔다면 메모리를 할당 받는다.	
	pOnlineLogInMgr->m_GameServerLogIn.siStatus= 0;

	// 나의 부하들에 대한 모든 데이타를 없앤다.
	pMyData->InitFollowers();
	
	pOnlineLogInMgr->m_GameServerLogIn.siLogInTime			=	timeGetTime();
	pOnlineLogInMgr->m_GameServerLogIn.bMessageBoxFloating	=	TRUE;
}


//----------------------------------------------------------------------------------------------------------------
//	설명	:	서버로부터 나의 데이타를 전송 받는다.
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
			// 연결에 실패하여 현재 어떤 메시지 박스가 떠있는 상태이다.	
			if(m_siMsgBoxStatus == ON_MSGBOX_TYPE_OK && pOnlineLogInMgr->bStatusBack)
			{
				return ON_RETURN_EXIT;
			}

			SI16 siTempCount = max(10 - ((timeGetTime() - pOnlineLogInMgr->m_GameServerLogIn.siLogInTime) / 1000), 0);

			if(siTempCount < 0)		// 10초간 대기
			{
				if(!pOnlineLoginMsgBox->IsActive())
				{
					pOnlineLoginMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pOnlineText->Get(ON_TEXT_DATA_GAMESERVERCONNECTERROR));
					pOnlineLogInMgr->bStatusBack = TRUE;
				}
				break;
			}
		
			OnGameServerLogInMsg *pLogInMsg = (OnGameServerLogInMsg*)pOnlineClient->GetMsgBuffer(ON_RESPONSE_LOGIN);

			// 서버로 부터 나의 데이타를 받았다.	
			if(!pLogInMsg)		break;
			
			if(pLogInMsg->dwResponse == ON_RET_OK)
			{				
				pOnlineClient->m_BlackPig.SetAutoKey(TRUE);

				bReturnKeyDown = FALSE;
				m_GameTickCount = GetTickCount();
				m_GameTime = timeGetTime();
				SendHeartBeat(TRUE);

				SI16 siCount = 1;
				
				// 각 포인터들을 초기화 시켜준다.
				lpOnGameServerLogInMsgData = (OnGameServerLogInMsgData*)(pLogInMsg + 1);

				// 장수 고용수를 받는다.
				pMyData->siMaxGeneral = lpOnGameServerLogInMsgData->siMaxGeneral;

				m_GMFlag   = lpOnGameServerLogInMsgData->USGrade;
				m_InfoFlag = TRUE;
				
				// Time을 설정한다.
				pTimer->SetNowTime(lpOnGameServerLogInMsgData->Time);
				fFirstGetTime = FALSE;	// 다음에 서버에서 오는 시간을 처음으로 간주한다.(지금 시간은 최대 2분이라는 오차가 생기므로)
				m_RealTime    = lpOnGameServerLogInMsgData->RealTime;
				
				// 나의 돈을 설정한다.
				pMyData->SetMoney(lpOnGameServerLogInMsgData->siMoney);		
				
				// 나의 저금액을 설정한다. (지금은 일단 0원)
				pMyData->SetSaving(lpOnGameServerLogInMsgData->moSaving);

				// 신용도를 설정한다.
				pMyData->SetBatleCredit(0, 0);
				pMyData->SetTradeCredit(lpOnGameServerLogInMsgData->uiTradeCredit, 0);
				
				// 나의 배고픔을 설정 한다.			
				pMyData->SetHealth(lpOnGameServerLogInMsgData->siHealth);			
				
				// 나의 Name을 설정한다.
				ZeroMemory(TempBuffer, 128);
				CopyMemory(TempBuffer, lpOnGameServerLogInMsgData->szID, ON_ID_LENGTH);
				pMyData->SetMyCharName((char*)TempBuffer);
				pMyData->SetMyCharUniqueID(lpOnGameServerLogInMsgData->uiMyCharUniqueID);
				
				// 따라다니는 용병
				pMyData->SetMercenary(lpOnGameServerLogInMsgData->siVMercenaySlot);
				
				// 길드를 설정한다.
				if(lpOnGameServerLogInMsgData->szGuildID[0])
					pMyData->SetGuild((char*)lpOnGameServerLogInMsgData->szGuildID, lpOnGameServerLogInMsgData->siGuildClass, lpOnGameServerLogInMsgData->siFlag);
				else
					pMyData->SetGuild(NULL, 0, 0);

				// robypark 2004/11/19 17:1
				// 공성전을 위해 변신 전 주인공 캐릭터 DB ID 저장
				pMyData->SetPrevCharOnlineKind(lpOnGameServerLogInMsgData->m_uiPrevKind);

				// robypark 2004/12/7 12:48
				// 공성전 유닛인 상태에서 죽은 뒤에 다시 공성 유닛으로 변신하기 위한 지연시간 설정
				pMyData->SetTransformSiegeWarfareUnit((lpOnGameServerLogInMsgData->m_dwDeadSec <= 0) ? true : false);
				
				// 우선 맵을 주인공의 위치로 옮긴다.
				lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnGameServerLogInMsgData + sizeof(OnGameServerLogInMsgData));

#ifdef _USE_ON_RESPONE_WAR_TIME
				// robypark 2005/2/16 3:0
				// 만약 ON_RESPONE_WAR_TIME 메시지를 사용한다면, 다음의 공성 설정해서는 안된다.
				// 공성 시간이되면, 서버에서 공성이 있다고 알려준다.
				if (IsInTimeSiegeWarfare())	// 공성시간이라면
				{
					// 오늘 공성전이 있는지?
					pMyData->SetSiegeWarfareDay(lpOnGameServerLogInMsgData->m_bGuildWarDay);
				}
#else
				// 그러나 ON_RESPONSE_NOT_WAR 메시를 사용할 경우 공성이 있다고 설정해주며,
				// 공성시간 전에 화친제의를 통하여 더 이상 공성이 없어진 경우에 메시지 처리를
				// 통하여 공성여부를 판단한다.
				// 오늘 공성전이 있는지?
				pMyData->SetSiegeWarfareDay(lpOnGameServerLogInMsgData->m_bGuildWarDay);
#endif

				// 주인공 모드(피난민 처리를 위함)
				m_scSiegeTransformMode = SIEGE_TRANSFORM_MODE_HERO;

				// robypark 2005/1/17 11:0
				// 피난민으로 변신 관련 처리
				if (IsInTimeSiegeWarfare())	// 공성시간이라면
				{
					if (lpOnGameServerLogInMsgData->m_bGuildWarDay)	// 공성하는 날의 사용자이며,
					{
						// 피난민 모드
						m_scSiegeTransformMode = SIEGE_TRANSFORM_MODE_REFUGEE;
					}

					// robypark 2005/1/29 13:32
					// 초보자 수련장으로 워프 될 수 있다는 경고 메시지 설정
					m_sWarningMsgInSiegeWarfare.m_dwLeftTime = 0;

				}
				else
				{
					// robypark 2005/1/29 13:32
					// 초보자 수련장으로 워프 될 수 있다는 경고 메시지 설정
					m_sWarningMsgInSiegeWarfare.m_dwLeftTime = WARNINGMSGINSIEGEWARFARE_DELAY;
				}

				// robypark 2005/1/29 14:6
				// 보급수레(메딕)일 경우 10초마다 마나가 10%씩 회복된다.
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
							// 주인공 캐릭터의 위치를 기준으로 맵을 설정한다.			
							pOnlineMap->ChangeMap(lpOnGameServerLogInMsgData->uiMapID, lpOnCharPlayerInfo->siX, lpOnCharPlayerInfo->siY);
							// 맵이 따라 다녀야 하는 맵의 ID 역시 설정해준다.
							pOnlineMap->SetCurrentFocusCharID(lpOnGameServerLogInMsgData->uiMyCharUniqueID);
							pOnlineMap->PlayFitMusic();
							
							// Chatting Server에 LogIn을 한다.
							pOnlineClientChatting->MessageSend_LogIn(pMyData->GetMyCharName(), lpOnCharPlayerInfo->siDBAccount, m_GMFlag);
							
							// 주인공의 현재위치를 아이템 탐사에 쓰이는 변수에 저장
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
				
				// 각 부하들에 대한 정보를 설정한다.
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
				
				// 필드의 아이템을 설정한다. 
				for(i = 0; i < 	lpOnGameServerLogInMsgData->uiFieldItemNum; i++)
				{
					pOnlineMap->SetID(pRecvFieldItemInfo[i].siX, pRecvFieldItemInfo[i].siY, 1, 1, 0, 0, pRecvFieldItemInfo[i].uiItemID, pRecvFieldItemInfo[i].uiQuantity);
				}
				
				// 주위의 플레이어를 설정한다.
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

							// 나의 캐릭터의 대한 정보를 설정한다.		
							pMyData->SetMyCharDBAccount(IP);
							pMyData->SetMyCharKind(lpOnCharPlayerInfo->uiKind);
							
							CopyMemory(TempBuffer, pMyData->GetMyCharName(), ON_ID_LENGTH);

							pIOnlineChar->Init(lpOnCharPlayerInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharPlayerInfo->uiKind, lpOnCharPlayerInfo->siX, lpOnCharPlayerInfo->siY, IP, 0, lpOnCharPlayerInfo->siFightingPower, this,(UI16*)lpOnCharPlayerInfo->uiEquipWeapon);
							pIOnlineChar->SetAttackMode(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bAttackMode);
							pIOnlineChar->SetEquipItem(lpOnCharPlayerInfo->uiAccount, (UI16*)lpOnCharPlayerInfo->uiEquipWeapon);
							pIOnlineChar->SetSiegeWarfare_Hp(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->siFieldHp);
							pIOnlineChar->SetCaptain(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->bCaptain);
							pIOnlineChar->SetSiegeWarfare_CurrentSupplyGoodsAmount(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->siSupplyCharge);

							// 시작 ////////////////////////////////
							// robypark 2004/11/9 21:16
							// 공성 유닛 정보 헤더 얻기
							LPSSiegeUnitTable lpsSiegeUnitTable = GetSiegeUnitTableMgr()->GetSiegeUnitTable(lpOnCharPlayerInfo->uiKind);

							// 공성전 유닛이 맞을 경우
							if (NULL != lpsSiegeUnitTable)
							{
								SetIsShowInformationAboutSiegeWarfareUnit(ON_GAMEOPTION_SIEGEWARFAREUNIT);	// 공성전 유닛이다.

								// 변신 버튼 비활성화
								pOnlinePannel->SetShowTransformSiegeUnitBtn(FALSE);

								// 공성전 유닛용 인터페이스 활성화
								pOnlinePannel->SiegeWarfareUnitUI_CreateWindow(SW_NORMAL);

								pOnlinePannel->ShowWindow_SiegeWarfareUnitUI(SW_NORMAL);
							}
							else
							{
								SetIsShowInformationAboutSiegeWarfareUnit(0);	// 공성전 유닛이 아니다.

								// 피난민 모드
								if (SIEGE_TRANSFORM_MODE_REFUGEE == m_scSiegeTransformMode)
								{
									// 변신 가능한가?
									if (pMyData->CanTransformSiegeWarfareUnit())
									{
										// 변신 버튼 활성화
										pOnlinePannel->SetShowTransformSiegeUnitBtn(TRUE);
									}
								}
							}

							// 끝 ////////////////////////////////

// by kuzimo

							// 주인공 캐릭터의 위치를 기준으로 맵을 설정한다.			
//							pOnlineMap->ChangeMap(lpOnGameServerLogInMsgData->uiMapID, lpOnCharPlayerInfo->siX, lpOnCharPlayerInfo->siY);
							// 맵이 따라 다녀야 하는 맵의 ID 역시 설정해준다.
							pOnlineMap->SetCurrentFocusCharID(lpOnGameServerLogInMsgData->uiMyCharUniqueID);
							pOnlineMap->PlayFitMusic();
							
							// Chatting Server에 LogIn을 한다.
							pOnlineClientChatting->MessageSend_LogIn(pMyData->GetMyCharName(), lpOnCharPlayerInfo->siDBAccount, m_GMFlag);
							
							// 주인공의 현재위치를 아이템 탐사에 쓰이는 변수에 저장
							m_CharPosition_X = lpOnCharPlayerInfo->siX;
							m_CharPosition_Y = lpOnCharPlayerInfo->siY;

//---------------------
							
							pMyData->nRank = lpOnGameServerLogInMsgData->siEventRanking;
							pMyData->nScore = lpOnGameServerLogInMsgData->siEventScore;


							pIOnlineChar->SetRank(pMyData->GetMyCharUniqueID(), lpOnGameServerLogInMsgData->siEventRanking);
							pIOnlineChar->SetScore(pMyData->GetMyCharUniqueID(), lpOnGameServerLogInMsgData->siEventScore);					
							
							
							// 내 캐릭터의 인벤토리를 설정한다.
							pOnlineInventory->Init(pMyData->GetMyCharUniqueID(), this);
							pOnlineTradeBook->m_pOnlineCharStatus->Init(pMyData->GetMyCharUniqueID(), this, pMyData->GetFollowersInterface());
							
							// 길드 처리
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
						
						// 개인 상점 정보
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
						IP = 0;
						pIOnlineChar->Init(lpOnCharMonsterInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharMonsterInfo->uiKind, lpOnCharMonsterInfo->siX, lpOnCharMonsterInfo->siY, IP, 0.0f, lpOnCharMonsterInfo->siFightingPower, this,NULL);
						
						lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharMonsterInfo));
					}
					else if(ISSHIP(lpOnCharBaseInfo->uiAccount))
					{
						lpOnCharShipInfo = (OnCharShipInfo*)lpOnCharBaseInfo;
						
						// 다른 캐릭터의 대한 정보를 설정한다.						
						IP = 0;
						pIOnlineChar->Init(lpOnCharShipInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharShipInfo->uiKind, lpOnCharShipInfo->siX, lpOnCharShipInfo->siY, IP, 0.0f, 0, this,NULL);
						
						lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharShipInfo));
					}
				}
				
				// 마을 투자 수익으로 얼마를 벌었으며 마을이 팔려나갔다
				if(lpOnGameServerLogInMsgData->YearProfit && lpOnGameServerLogInMsgData->SoldFacilityPrice)
				{
					sprintf((char*)TempBuffer1, pOnlineText->Get(ON_TEXT_PAY_INVESTMENT), lpOnGameServerLogInMsgData->YearProfit);
					pOnlineMsgBoxPay->SetMsg(ON_MSGBOX_TYPE_OK, "", (char*)TempBuffer1);
				}
				// 마을 투자 수익으로 얼마를 벌었다.
				else if(lpOnGameServerLogInMsgData->YearProfit)
				{
					sprintf((char*)TempBuffer1, pOnlineText->Get(ON_TEXT_PAY_INVESTMENT), lpOnGameServerLogInMsgData->YearProfit);
					pOnlineMsgBoxPay->SetMsg(ON_MSGBOX_TYPE_OK, "", (char*)TempBuffer1);
				}
				// 가동이 없어서 팔려나갔다.
				else if(lpOnGameServerLogInMsgData->SoldFacilityPrice)
				{
					sprintf((char*)TempBuffer1, pOnlineText->Get(ON_TEXT_PAY_PRODUCTFACILITY), lpOnGameServerLogInMsgData->SoldFacilityPrice);
					pOnlineMsgBoxPay->SetMsg(ON_MSGBOX_TYPE_OK, "", (char*)TempBuffer1);
				}

				// actdoll (2004/07/19 10:43) : = 시간제 아이템 =
				//	기간이 만료되어 삭제되었다면 알려준다.
				if( lpOnGameServerLogInMsgData->bCompareWearItem )
				{
					// 메시지 나중에 추가할 것
					pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_ITEMHELP_TIMERITEM), pOnlineText->Get(ON_TEXT_NOTICE_ITEM_TIMERITEM_IS_DELETED) );
				}

				
				// 부동산 정보 요청(임시)
				pOnlineClient->SendAuditOwnListMsg();
				
				// actdoll (2004/03/30 11:04) : = 이벤트 = 독도퀘스트 추가용
				//		여기서 현재 퀘스트를 끝낸 사용자 숫자를 받아오도록 한다. 추후 이벤트 끝난 후에 삭제할 것.
//				m_uiDokdoQuestCompleteUserCount	= lpOnGameServerLogInMsgData->uiDokdoQuestCompletedUserCount;

//				m_pMacroProcess->SetSendAble(TRUE);

				// robypark 2004/9/9 15:28
				// 현재 이벤트 기간인가?
				m_bIsInEventDay = lpOnGameServerLogInMsgData->bEvent;

				// robypark 2004/9/9 20:56
				// 이벤트 기간, 국가에 따른 이벤트 퀘스트를 주는 NPC 비활성화에 대한 처리
				DisableEventNPC();

				// robypark 2004/12/16 15:50
				// 오늘 공성전이 있는지를 표시해준다.
				if (TRUE == lpOnGameServerLogInMsgData->m_bGuildWarDay)
				{
					pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", pOnlineText->Get(ON_TEXT_TODAY_IS_SIEGEWARFARE_DAY), m_SOptionSiegeWarfare.siSIEGEWARFARETIME_BEGIN, m_SOptionSiegeWarfare.siSIEGEWARFARETIME_END);
				}

				return ON_RETURN_OK;
			}
			else
			{
				// 게임서버 로그인 실패
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
//	설명	:	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::FreeGameServerLogIn()
{
}

// robypark 2004/9/9 15:30
// 현재 이벤트 기간인지를 얻어온다.
BOOL	cltOnlineWorld::IsInEventDay(void)
{
	return m_bIsInEventDay;
}

// robypark 2004/9/9 20:56
// 이벤트 기간, 국가에 따른 이벤트 퀘스트를 주는 NPC 비활성화
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
		// 이벤트 기간이 아닐 경우
		if (FALSE == m_bIsInEventDay)
		{
			// 이벤트로 설정된 퀘스트 NPC를 비활성화
			if (NORMAL_QUEST != lpSQuestHeader->m_siQuestEvent)
			{
				// 혼잣말 중지 설정
				lpNPCIndexHeader = pOnlineMap->m_NPCParser.GetQuestNPCAllMap(lpSQuestHeader->siNPCCode);

				if (NULL != lpNPCIndexHeader)
				{
					lpNPCIndexHeader->bTalkOneself = FALSE;

					// 퀘스트 NPC 클릭 막기
					lpNPCIndexHeader = pOnlineMap->m_NPCParser.GetNPC(lpNPCIndexHeader->siCode);

					if (NULL != lpNPCIndexHeader)
					{
						lpNPCIndexHeader->bTalkOneself = FALSE;
					}
				}
			}
		}
		// 이벤트 기간이며
		else
		{
*/			// 해당 국가(거상 클라이언트 국가)에 해당되지 않는 퀘스트 NPC 비활성화
			if (QUEST_EVENT_CANCEL == lpSQuestHeader->m_siQuestEvent)
			{
				// 혼잣말 중지 설정
				lpNPCIndexHeader = pOnlineMap->m_NPCParser.GetQuestNPCAllMap(lpSQuestHeader->siNPCCode);

				if (NULL != lpNPCIndexHeader)
				{
					lpNPCIndexHeader->bTalkOneself = FALSE;

					// 퀘스트 NPC 클릭 막기
					lpNPCIndexHeader = pOnlineMap->m_NPCParser.GetNPC(lpNPCIndexHeader->siCode);

					if (NULL != lpNPCIndexHeader)
					{
						lpNPCIndexHeader->bTalkOneself = FALSE;
					}
				}
			}
			// robypark 2004/9/16 16:33
			// 퀘스트 NPC 활성화
			else if (QUEST_EVENT_PLAY == lpSQuestHeader->m_siQuestEvent)
			{
				// 혼잣말 가능하도록 설정
				lpNPCIndexHeader = pOnlineMap->m_NPCParser.GetQuestNPCAllMap(lpSQuestHeader->siNPCCode);

				if (NULL != lpNPCIndexHeader)
				{
					lpNPCIndexHeader->bTalkOneself = TRUE;

					// 퀘스트 NPC 클릭 가능하도록 수정
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
