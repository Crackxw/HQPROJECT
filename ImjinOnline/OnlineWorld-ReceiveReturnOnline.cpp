#include <GSL.h>
#include <Main.h> 
#include <Mouse.h>
#include "OnlineWorld.h"
#include "ChattingServer\\HQChattingServerClientSocket.h"
#include "OnlineVillageManager.h"
#include "OnlineGameMsg.h"
#include <SoundManager.h>
#include "..\Gersang\message.h"
#include "OnlineTradeBook.h"
#include "OnlineMyData.h"
#include "OnlineClient.h"
#include "OnlineVillage.h"
#include "OnlineFieldChat.h"
#include "OnlineMiniMap.h"
#include "OnlineFollower.h"
#include "OnlineNoticeList.h"
#include	"OnlineListControl.h"
#include "OnlineInventory.h"
#include "OnlineCharUI.h"
#include "OnlineTrade.h"
#include "OnlineBooth.h"
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS 용 데이터 추가
#include "OnlinePannel.h"
#include "maindata.h"

// 공성 유닛 관리자
#include	"SiegeUnitTableMgr.h"

#ifdef _OGG_PLAY
#include "BgmMgr_PreHeader.h"
#endif

extern SoundManager *g_pSoundManager;
extern	_MainData			GD;


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	임진록2 Game에서 임진록 온라인으로 가기 위해서 해제했던 메모리를 다시 할당 받는다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltOnlineWorld::InitReturnOnline()
{	
	//CHAR	szSpriteFileName[1024];
	//CHAR	szSmallFileName[1024];
	//SI32	siFile;
	//XSPR	*pTempSpr;
	//SI32	i;

	//DWriteStringToFile("InitReturnOnline() 진입");
/*
	// 리소스를 할당 받는다.	
	pOnlineResource		=	new	OnlineResource;	
	pOnlineResource->Init();		

	for( i=1 ; i<(SI32)pItemsInfo->GetCount()+1 ; i++)											// 리소스로 변환
	{
		pItemsInfo->GetSpriteFileName( i, szSpriteFileName, szSmallFileName );
				
		if( ( stricmp( szSpriteFileName, "0" ) != 0 ) && ( stricmp( szSmallFileName, "0" ) != 0 ) )
		{
			if( pOnlineResource->GetItemFileIndex( szSpriteFileName ) == -1 )					// 큰 그림을 로드한다.( 그림이 로드가 돼어 있지 않다면 로드함)			
				pOnlineResource->AddItemSpr( szSpriteFileName );									

			if( pOnlineResource->GetItemFileIndex( szSmallFileName ) == -1 )					// 작은 그림을 로드한다.			
				pOnlineResource->AddItemSpr( szSmallFileName );			
		}		

		ZeroMemory( szSpriteFileName, 1024 );
		ZeroMemory( szSmallFileName, 1024 );
	}

	
	for( i=1; i<(SI32)pItemsInfo->GetCount() + 1; i++)											// 다쉬 한번 아이템 구조체에 리소스 인덱스를 넣어 준다.
	{		
		pItemsInfo->GetSpriteFileName( i, szSpriteFileName, szSmallFileName );					// 아이템 파일을 받는다.

		if( ( stricmp( szSpriteFileName, "0" ) != 0 ) && ( stricmp( szSmallFileName, "0" ) != 0 ) )
		{
			// 큰 그림
			siFile		=	pOnlineResource->GetObjectFileIndex( szSpriteFileName );			// 리소스에서 파일 이름넣어 인덱스를 받는다.
			pTempSpr	=	pOnlineResource->GetxsprObject( siFile );

	
			if( siFile != -1 )		pItemsInfo->SetFile( siFile, szSpriteFileName, pTempSpr->Header.Xsize, pTempSpr->Header.Ysize, FALSE );
			else					clGrp.Error("", "아아템 파일 찾기 실패 [%s]", szSpriteFileName );

			// 작은 그림
			siFile		=	pOnlineResource->GetObjectFileIndex( szSmallFileName );				// 리소스에서 파일 이름넣어 인덱스를 받는다.
			pTempSpr	=	pOnlineResource->GetxsprObject( siFile );
			
	
			if( siFile != -1 )		pItemsInfo->SetFile( siFile, szSmallFileName, pTempSpr->Header.Xsize, pTempSpr->Header.Ysize, TRUE );
			else					clGrp.Error("", "아아템 파일 찾기 실패 [%s]", szSmallFileName );
		}

		ZeroMemory( szSpriteFileName, 1024 );
		ZeroMemory( szSmallFileName, 1024 );
	}

	// 다이렉트 드로우 객체를 생성한다. 반드시 리소스를 로드한 후 호출해 주어야 한다.
	pOnlineDirectDraw	=	new	OnlineDirectDraw;
	pOnlineDirectDraw->Init(this);

	// 맵의 메모리를 할당 받는다.
	pOnlineMap	=	new OnlineMap;		
	pOnlineMap->Load("Online\\Map\\map.map", this);
*/
	// 데이타를 할당 받는다.
	pOnlineReturnOnline	=	new OnlineReturnOnline;

	// 메시지 버퍼에 있는 메시지를 초기화한다.
	InitMessage();


	// 캐릭터 인터페이스를 생성한다.
	if(!pIOnlineChar)
	{
		pIOnlineChar = new IOnlineChar;	
		pIOnlineChar->Init(this);
	}
	pIOnlineChar->AllFree();

	// 오브젝트 인터페이스를 생성한다.
	pIOnlineObject	=	new	IOnlineObject;
	pIOnlineObject->Init();			

	// 사용자 입력을 받을 수 있도록 한다.
	// 그리고 Battle Response를 더 이상 기다리지 않는다.
	pMyData->SetWaitBattleResponse(FALSE);
	pOnlineCharUI->SetAction(TRUE);	

	// 서버로 부터 온라인으로 돌아간다고 설정해 준다.	
	pOnlineClient->SendReturnOnline(GD.WinSwitch);	

	pOnlineReturnOnline->dwRequestGameDataTime	=	timeGetTime();

	m_siUpdateMethod = 0;
	m_siUpdateCounter = 0;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	임진록2에서 온라인으로 다시 돌아간다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI16	cltOnlineWorld::ReturnOnline()
{
	UI16 uiProc = pOnlineClient->DataProc();
	if( uiProc )	return ReceiveMessageProc( uiProc );
	return 0;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	임진록2에서 온라인으로 다시 돌아간다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltOnlineWorld::FreeReturnOnline()
{
	if(pOnlineReturnOnline)
	{
		delete pOnlineReturnOnline;
		pOnlineReturnOnline	=	NULL;
	}
}

UI16	cltOnlineWorld::ReturnOnline(BYTE *pMsg)
{
	UI16									uiNextMenu	=	0;
	OnResReturnOnlineFieldMsg				*pReturnOnlineFieldMsg;
	OnResReturnOnlineVillageMsg				*pReturnOnlineVillageMsg;
	OnFollowerInfo*							pRecvFollowerInfo;
	OnCharBaseInfo*							lpOnCharBaseInfo;
	OnCharMonsterInfo*						lpOnCharMonsterInfo;
	OnCharPlayerInfo*						lpOnCharPlayerInfo;
	OnCharShipInfo*							lpOnCharShipInfo;
	OnlineReceiveFieldItemInfo				*pRecvFieldItemInfo;
	SI32									i;
	OnlineReceivedMyItemInfo*				lpMyItemInfo;
	SI32									TempCount;
	SI32									TempCount1;
	BYTE									TempBuffer[128];
	DWORD									IP;
	SI32									uiFollowerCount;
	CHAR									szTextBuffer[1024];
	HDC										hdc;
	SIZE									TextSize;
	SellItemInfo							TempSellItemInfo;
	OnBaseBoothItem							*lpOnBaseBoothItem, *pTempOnBaseBoothItem;

	OnResReturnOnlineMsg *pReturnOnlineMsg = (OnResReturnOnlineMsg*)pMsg;

	if(pReturnOnlineMsg)
	{
		switch(pReturnOnlineMsg->dwResponse)
		{
		case ON_RET_RETURNONLINE_FIELD:	
			{
				//DWriteStringToFile("ON_CMDPROC_RETURNONLINE 메시지 받음");
				pReturnOnlineFieldMsg		=	(OnResReturnOnlineFieldMsg*)pReturnOnlineMsg;
				
				// Base Data
				pMyData->SetMoney(pReturnOnlineFieldMsg->siMoney);
				pMyData->SetHealth(pReturnOnlineFieldMsg->siHealth);
				pIOnlineChar->SetRank(pMyData->GetMyCharUniqueID(), pReturnOnlineFieldMsg->siEventRanking);			
				pIOnlineChar->SetScore(pMyData->GetMyCharUniqueID(), pReturnOnlineFieldMsg->siEventScore);
				
				
				// Map위치를 설정한다.
				lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)pReturnOnlineFieldMsg + sizeof(OnResReturnOnlineFieldMsg));
				for(i = 0; i < pReturnOnlineFieldMsg->uiPlayerNum; i++)
				{
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
				
				// 주위의 플레이어를 설정한다.
				lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)pReturnOnlineFieldMsg + sizeof(OnResReturnOnlineFieldMsg));
				for(i = 0; i < pReturnOnlineFieldMsg->uiPlayerNum; i++)
				{
					if(ISPLAYER(lpOnCharBaseInfo->uiAccount))
					{
						lpOnCharPlayerInfo = (OnCharPlayerInfo*)lpOnCharBaseInfo;

						ZeroMemory(TempBuffer, 128);
						if(pMyData->GetMyCharUniqueID() == lpOnCharPlayerInfo->uiAccount)
						{
							CopyMemory(TempBuffer, pMyData->GetMyCharName(), ON_ID_LENGTH);
							IP = lpOnCharPlayerInfo->siDBAccount;

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
							}
							// 끝 ////////////////////////////////

							pMyData->nRank = pReturnOnlineFieldMsg->siEventRanking;
							pMyData->nScore = pReturnOnlineFieldMsg->siEventScore;
							pIOnlineChar->SetRank(pMyData->GetMyCharUniqueID(), pReturnOnlineFieldMsg->siEventRanking);			
							pIOnlineChar->SetScore(pMyData->GetMyCharUniqueID(), pReturnOnlineFieldMsg->siEventScore);

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
				
				pRecvFieldItemInfo		=	(OnlineReceiveFieldItemInfo*)((CHAR*)lpOnCharBaseInfo);
				pRecvFollowerInfo       =   (OnFollowerInfo*)((CHAR*)pRecvFieldItemInfo + sizeof(OnlineReceiveFieldItemInfo) * pReturnOnlineFieldMsg->uiFieldItemNum);
				uiFollowerCount			=	pReturnOnlineFieldMsg->uiFollowerNum;
				
				// 필드의 아이템을 설정한다. 
				for(i = 0; i < 	pReturnOnlineFieldMsg->uiFieldItemNum; i++)
				{
					pOnlineMap->SetID(pRecvFieldItemInfo[i].siX, pRecvFieldItemInfo[i].siY, 1, 1, 0, 0, pRecvFieldItemInfo[i].uiItemID, pRecvFieldItemInfo[i].uiQuantity);
				}
				
				uiNextMenu	=	ON_MAINPOLL_GAME;
#ifdef _OGG_PLAY
				GetBgmMgr()->PlayNextBgm(TRUE);
#else
				// 음악을 연주시킨다.
				if(g_pSoundManager)
					g_pSoundManager->PlaySound( BackgroundMusic );
#endif
			}
			break;
		case ON_RET_RETURNONLINE_VILLAGE:
			{
				VillageHeader *pVillageHeader;

				pReturnOnlineVillageMsg = (OnResReturnOnlineVillageMsg*)pReturnOnlineMsg;
				
				uiFollowerCount = pReturnOnlineVillageMsg->uiFollowerNum;

				pOnlineClientChatting->MessageSend_NotIsMap();
				
				// Base Data
				pMyData->SetMoney(pReturnOnlineVillageMsg->siMoney);
				pMyData->SetHealth(pReturnOnlineVillageMsg->siHealth); 
				
				// 마을 정보를 추가한다.
				switch(pReturnOnlineVillageMsg->VillageInfo.cNationCode)
				{
				case NATION_CODE_KOREA:
					pReturnOnlineVillageMsg->VillageInfo.cNationCode = ON_COUNTRY_KOREA;
					break;
					
				case NATION_CODE_CHINA:
					pReturnOnlineVillageMsg->VillageInfo.cNationCode = ON_COUNTRY_CHINA;
					break;
					
				case NATION_CODE_JAPAN:
					pReturnOnlineVillageMsg->VillageInfo.cNationCode = ON_COUNTRY_JAPAN;
					break;
					
				case NATION_CODE_TAIWAN:
					pReturnOnlineVillageMsg->VillageInfo.cNationCode = ON_COUNTRY_TAIWAN;
					break;
				}		
				
				pOnlineVillageDescriptList->SetVillageDataData(pReturnOnlineVillageMsg->uiVillageCode, pReturnOnlineVillageMsg->VillageInfo.cNationCode
					, (char*)pReturnOnlineVillageMsg->VillageInfo.szGuildID
					, pReturnOnlineVillageMsg->VillageInfo.siGuildFlag
					, pReturnOnlineVillageMsg->VillageInfo.vdVillageDefence
					, pReturnOnlineVillageMsg->VillageInfo.vdVillageMaxDefence
					, pReturnOnlineVillageMsg->VillageInfo.dwGuildRelation
					, pReturnOnlineVillageMsg->VillageInfo.siDefenceSoldier	// 현재 수비병 부대수
					, pReturnOnlineVillageMsg->VillageInfo.siDefenceSoldier	/*pReturnOnlineVillageMsg->VillageInfo.siMaxDefenceSoldier*/	// 최대 수비병 부대수
					, pReturnOnlineVillageMsg->VillageInfo.siAttackType	// 마을 공격 타입
					);

				// 마을 객체를 생성한다.
				pOnlineVillage	=	new OnlineVillage;	
				
				// 마을을 초기화 한다.
				pVillageHeader	=	pOnlineVillageManager->GetVillageByCode(pReturnOnlineVillageMsg->uiVillageCode);
				pOnlineMap->ChangeMap(pVillageHeader->siMapIndex, pVillageHeader->siX, pVillageHeader->siY);
				pOnlineMap->PlayFitMusic();
				if(pOnlineMap->m_MapInfo.uiVillageCode)
				{
					pOnlineClient->SendRequestVillageInfo(pOnlineMap->m_MapInfo.uiVillageCode);
				}
				
				pOnlineVillage->Init(pReturnOnlineVillageMsg->uiVillageCode, &pReturnOnlineVillageMsg->VillageInfo, this);	
				
				// 의원에 들여보낸다.
				pOnlineVillage->MoveInStruct(ON_VIL_ST_KIND_HOSPITAL, FALSE, TRUE);
				pOnlineVillage->fDead = TRUE;
				
				// 다음 메뉴를 설정한다.
				uiNextMenu	=	ON_MAINPOLL_GAME;		
				
				// 현재 게임 상태가 마을 안에 있다고 설정한다.
				siGamePollStatus	=	ON_GAMEPOLL_VILLAGE;	
				
				pRecvFollowerInfo       =   (OnFollowerInfo*)((CHAR*)pReturnOnlineVillageMsg + sizeof(OnResReturnOnlineVillageMsg));
			}
			break;
		default:
			clGrp.Error("ReceiveReturnOnline", "Unknow Data : [%d]", pReturnOnlineMsg->dwResponse);
			break;
		}
	}

	// 아직 어떤 메시지도 오지 않았다.			
	// 시간이 종료되면 로그인 화면으로 되돌린다.
	if(timeGetTime() > DWORD((pOnlineReturnOnline->dwRequestGameDataTime + (60 * 1000))))
	{
		uiNextMenu		=	ON_MAINPOLL_FORCED_LOGOUT;
//		SetLogOut(GS_INITQUIT, 0);
	}
	
	// 시간을 화면에 찍어준다.
	sprintf(szTextBuffer, pOnlineText->Get(ON_TEXT_DATA_WAITTINGSERVER), max(60 - ((timeGetTime() - pOnlineReturnOnline->dwRequestGameDataTime) / 1000), 0));		
	
	// 화면에 찍어준다.
	// 그린다.
	if(GETSCREENSURFACE(this)->IsLost() != DD_OK)
	{
		GETSCREENSURFACE(this)->Restore();
	}
	clGrp.ClearSurface(GETSCREENSURFACE(this));
	
	// 글자를 찍는다.
	if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
	{
		// 투명 모드 설정.
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255, 255, 255));
		
		GetTextExtentPoint32(hdc, szTextBuffer, strlen(szTextBuffer), &TextSize);
		
		if(szTextBuffer[0] != NULL)
		{
			TextOut(hdc, (clGrp.GetScreenXsize() - TextSize.cx) / 2, (clGrp.GetScreenYsize() - TextSize.cy) / 2, 
				//clGrp.GetScreenXsize() / 2 - TextSize.cx / 2, clGrp.GetScreenYsize() / 2 - TextSize.cy / 2,
				szTextBuffer, strlen(szTextBuffer));
		}
		
		GETSCREENSURFACE(this)->ReleaseDC(hdc);		
	}

	UpdateScreen(GETSCREENSURFACE(this), TRUE);
		
	// 부하들이 정보를 재 설정한다.
	if(pReturnOnlineMsg)
	{
		pMyData->InitFollowers();	
		for(i = 0; i < uiFollowerCount; i++)		
		{
			pMyData->InitFollower(pRecvFollowerInfo->uiSlot, pRecvFollowerInfo);
/*
			for( TempCount = 0 ;TempCount<WEAR_ITEM_NUM; TempCount++)
			{
				pMyData->StripItem( pRecvFollowerInfo->uiSlot, (SI16)TempCount );
				//pMyData->AttachItem( pRecvFollowerInfo->uiSlot, pRecvFollowerInfo->uiWearItem[TempCount], (SI16)TempCount );
			}
*/
			lpMyItemInfo = (OnlineReceivedMyItemInfo*)((BYTE*)pRecvFollowerInfo + sizeof(OnFollowerInfo));
			for(TempCount = 0; TempCount < pRecvFollowerInfo->uiItemNum; TempCount++)
			{
				pMyData->AddItem(pRecvFollowerInfo->uiSlot, (MyItemData*)&lpMyItemInfo[TempCount]);
			}

			pRecvFollowerInfo = (OnFollowerInfo*)((BYTE*)pRecvFollowerInfo + sizeof(OnFollowerInfo) + (sizeof(OnlineReceivedMyItemInfo) * pRecvFollowerInfo->uiItemNum));
		}
	}
	
	if(m_BattleAfterMiniMapFlag)
	{
		if(pReturnOnlineMsg)
		{
			if(pReturnOnlineMsg->dwResponse == ON_RET_RETURNONLINE_VILLAGE)
			{
				pOnlineMiniMap->SetActive( FALSE );
			}
			else
			{
				pOnlineMiniMap->SetActive( TRUE );
			}
		}
	}
	else
	{
		pOnlineMiniMap->SetActive( FALSE );
	}

//	pOnlinePannel->pOnlineChatEditControl->SetLanguage(m_LanguageMode);
//	pOnlineNoticeList->SetPosition(233, 582, 288, 4);
	// actdoll (2004/06/16 20:00) : 채팅 정보를 원래대로 설정한다.
	pOnlinePannel->Initialize();

	m_LastNowUserConnectMessageTickCount = 0;

	if(uiNextMenu != 0)			
	{
		FreeReturnOnline();	
	}

	return uiNextMenu;
}