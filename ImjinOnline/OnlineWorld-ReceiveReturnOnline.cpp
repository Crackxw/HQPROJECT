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
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS 辨 等檜攪 蹺陛
#include "OnlinePannel.h"
#include "maindata.h"

// 奢撩 嶸棉 婦葬濠
#include	"SiegeUnitTableMgr.h"

#ifdef _OGG_PLAY
#include "BgmMgr_PreHeader.h"
#endif

extern SoundManager *g_pSoundManager;
extern	_MainData			GD;


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	歜霞煙2 Game縑憮 歜霞煙 螞塭檣戲煎 陛晦 嬪п憮 п薯ц湍 詭賅葬蒂 棻衛 й渡 嫡朝棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltOnlineWorld::InitReturnOnline()
{	
	//CHAR	szSpriteFileName[1024];
	//CHAR	szSmallFileName[1024];
	//SI32	siFile;
	//XSPR	*pTempSpr;
	//SI32	i;

	//DWriteStringToFile("InitReturnOnline() 霞殮");
/*
	// 葬模蝶蒂 й渡 嫡朝棻.	
	pOnlineResource		=	new	OnlineResource;	
	pOnlineResource->Init();		

	for( i=1 ; i<(SI32)pItemsInfo->GetCount()+1 ; i++)											// 葬模蝶煎 滲��
	{
		pItemsInfo->GetSpriteFileName( i, szSpriteFileName, szSmallFileName );
				
		if( ( stricmp( szSpriteFileName, "0" ) != 0 ) && ( stricmp( szSmallFileName, "0" ) != 0 ) )
		{
			if( pOnlineResource->GetItemFileIndex( szSpriteFileName ) == -1 )					// 贖 斜葡擊 煎萄и棻.( 斜葡檜 煎萄陛 腋橫 氈雖 彊棻賊 煎萄л)			
				pOnlineResource->AddItemSpr( szSpriteFileName );									

			if( pOnlineResource->GetItemFileIndex( szSmallFileName ) == -1 )					// 濛擎 斜葡擊 煎萄и棻.			
				pOnlineResource->AddItemSpr( szSmallFileName );			
		}		

		ZeroMemory( szSpriteFileName, 1024 );
		ZeroMemory( szSmallFileName, 1024 );
	}

	
	for( i=1; i<(SI32)pItemsInfo->GetCount() + 1; i++)											// 棻蓮 и廓 嬴檜蠱 掘褻羹縑 葬模蝶 檣策蝶蒂 厥橫 遽棻.
	{		
		pItemsInfo->GetSpriteFileName( i, szSpriteFileName, szSmallFileName );					// 嬴檜蠱 だ橾擊 嫡朝棻.

		if( ( stricmp( szSpriteFileName, "0" ) != 0 ) && ( stricmp( szSmallFileName, "0" ) != 0 ) )
		{
			// 贖 斜葡
			siFile		=	pOnlineResource->GetObjectFileIndex( szSpriteFileName );			// 葬模蝶縑憮 だ橾 檜葷厥橫 檣策蝶蒂 嫡朝棻.
			pTempSpr	=	pOnlineResource->GetxsprObject( siFile );

	
			if( siFile != -1 )		pItemsInfo->SetFile( siFile, szSpriteFileName, pTempSpr->Header.Xsize, pTempSpr->Header.Ysize, FALSE );
			else					clGrp.Error("", "嬴嬴蠱 だ橾 瓊晦 褒ぬ [%s]", szSpriteFileName );

			// 濛擎 斜葡
			siFile		=	pOnlineResource->GetObjectFileIndex( szSmallFileName );				// 葬模蝶縑憮 だ橾 檜葷厥橫 檣策蝶蒂 嫡朝棻.
			pTempSpr	=	pOnlineResource->GetxsprObject( siFile );
			
	
			if( siFile != -1 )		pItemsInfo->SetFile( siFile, szSmallFileName, pTempSpr->Header.Xsize, pTempSpr->Header.Ysize, TRUE );
			else					clGrp.Error("", "嬴嬴蠱 だ橾 瓊晦 褒ぬ [%s]", szSmallFileName );
		}

		ZeroMemory( szSpriteFileName, 1024 );
		ZeroMemory( szSmallFileName, 1024 );
	}

	// 棻檜滓お 萄煎辦 偌羹蒂 儅撩и棻. 奩萄衛 葬模蝶蒂 煎萄и �� ��轎п 輿橫撿 и棻.
	pOnlineDirectDraw	=	new	OnlineDirectDraw;
	pOnlineDirectDraw->Init(this);

	// 裘曖 詭賅葬蒂 й渡 嫡朝棻.
	pOnlineMap	=	new OnlineMap;		
	pOnlineMap->Load("Online\\Map\\map.map", this);
*/
	// 等檜顫蒂 й渡 嫡朝棻.
	pOnlineReturnOnline	=	new OnlineReturnOnline;

	// 詭衛雖 幗ぷ縑 氈朝 詭衛雖蒂 蟾晦�倆挬�.
	InitMessage();


	// 議葛攪 檣攪む檜蝶蒂 儅撩и棻.
	if(!pIOnlineChar)
	{
		pIOnlineChar = new IOnlineChar;	
		pIOnlineChar->Init(this);
	}
	pIOnlineChar->AllFree();

	// 螃粽薛お 檣攪む檜蝶蒂 儅撩и棻.
	pIOnlineObject	=	new	IOnlineObject;
	pIOnlineObject->Init();			

	// 餌辨濠 殮溘擊 嫡擊 熱 氈紫煙 и棻.
	// 斜葬堅 Battle Response蒂 渦 檜鼻 晦棻葬雖 彊朝棻.
	pMyData->SetWaitBattleResponse(FALSE);
	pOnlineCharUI->SetAction(TRUE);	

	// 憮幗煎 睡攪 螞塭檣戲煎 給嬴除棻堅 撲薑п 遽棻.	
	pOnlineClient->SendReturnOnline(GD.WinSwitch);	

	pOnlineReturnOnline->dwRequestGameDataTime	=	timeGetTime();

	m_siUpdateMethod = 0;
	m_siUpdateCounter = 0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	歜霞煙2縑憮 螞塭檣戲煎 棻衛 給嬴除棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI16	cltOnlineWorld::ReturnOnline()
{
	UI16 uiProc = pOnlineClient->DataProc();
	if( uiProc )	return ReceiveMessageProc( uiProc );
	return 0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	歜霞煙2縑憮 螞塭檣戲煎 棻衛 給嬴除棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
				//DWriteStringToFile("ON_CMDPROC_RETURNONLINE 詭衛雖 嫡擠");
				pReturnOnlineFieldMsg		=	(OnResReturnOnlineFieldMsg*)pReturnOnlineMsg;
				
				// Base Data
				pMyData->SetMoney(pReturnOnlineFieldMsg->siMoney);
				pMyData->SetHealth(pReturnOnlineFieldMsg->siHealth);
				pIOnlineChar->SetRank(pMyData->GetMyCharUniqueID(), pReturnOnlineFieldMsg->siEventRanking);			
				pIOnlineChar->SetScore(pMyData->GetMyCharUniqueID(), pReturnOnlineFieldMsg->siEventScore);
				
				
				// Map嬪纂蒂 撲薑и棻.
				lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)pReturnOnlineFieldMsg + sizeof(OnResReturnOnlineFieldMsg));
				for(i = 0; i < pReturnOnlineFieldMsg->uiPlayerNum; i++)
				{
					if(ISPLAYER(lpOnCharBaseInfo->uiAccount))
					{
						lpOnCharPlayerInfo = (OnCharPlayerInfo*)lpOnCharBaseInfo;

						if(pMyData->GetMyCharUniqueID() == lpOnCharPlayerInfo->uiAccount)
						{					
							// 輿檣奢 議葛攪曖 嬪纂蒂 晦遽戲煎 裘擊 撲薑и棻.				
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
				
				// 輿嬪曖 Ы溯檜橫蒂 撲薑и棻.
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
							
							// 衛濛 ////////////////////////////////
							// robypark 2004/11/9 21:16
							// 奢撩 嶸棉 薑爾 ④渦 橢晦
							LPSSiegeUnitTable lpsSiegeUnitTable = GetSiegeUnitTableMgr()->GetSiegeUnitTable(lpOnCharPlayerInfo->uiKind);

							// 奢撩瞪 嶸棉檜 蜃擊 唳辦
							if (NULL != lpsSiegeUnitTable)
							{
								SetIsShowInformationAboutSiegeWarfareUnit(ON_GAMEOPTION_SIEGEWARFAREUNIT);	// 奢撩瞪 嶸棉檜棻.
							}
							// 部 ////////////////////////////////

							pMyData->nRank = pReturnOnlineFieldMsg->siEventRanking;
							pMyData->nScore = pReturnOnlineFieldMsg->siEventScore;
							pIOnlineChar->SetRank(pMyData->GetMyCharUniqueID(), pReturnOnlineFieldMsg->siEventRanking);			
							pIOnlineChar->SetScore(pMyData->GetMyCharUniqueID(), pReturnOnlineFieldMsg->siEventScore);

							// 望萄 籀葬
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
							// 熱撩檣攪む檜蝶縑 氈朝 議葛攪檣陛蒂 撲薑
							pIOnlineChar->SetInVillageDefenceInterface(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bVillageInLimit);

							// 奢撩 嶸棉 薑爾 ④渦 橢晦
							LPSSiegeUnitTable lpsSiegeUnitTable = GetSiegeUnitTableMgr()->GetSiegeUnitTable(lpOnCharPlayerInfo->uiKind);

							// 奢撩瞪 嶸棉檜 蜃擊 唳辦
							if (NULL != lpsSiegeUnitTable)
							{
								// 奢撩瞪 嶸棉 餌辨濠 議葛攪曖 鼻撮 薑爾陛 橈棻賊
								if(pIOnlineChar->GetDataType(lpOnCharPlayerInfo->uiAccount) == 1)
								{
									if (GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
									{
										// 鼻撮薑爾蒂 蹂羶и棻.
										pOnlineClient->SendPlayerDetailInfo(lpOnCharPlayerInfo->uiAccount);
										pIOnlineChar->SetDataType(lpOnCharPlayerInfo->uiAccount, 2);	// 薑爾 棻遴煎萄 渠晦 醞...
									}
								}
							}
						}
						pIOnlineChar->SetGuildData(lpOnCharPlayerInfo->uiAccount, pIOnlineChar->GetGuildName(lpOnCharPlayerInfo->uiAccount), lpOnCharPlayerInfo->siGuildInGrade, pIOnlineChar->GetGuildFlag(lpOnCharPlayerInfo->uiAccount));
						pIOnlineChar->SetMercenary(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->siVMercenaryKind);

						// 偃檣 鼻薄
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

						// 棻艇 議葛攪曖 渠и 薑爾蒂 撲薑и棻.						
						ZeroMemory(TempBuffer, 128);
						IP = 0;
						pIOnlineChar->Init(lpOnCharMonsterInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharMonsterInfo->uiKind, lpOnCharMonsterInfo->siX, lpOnCharMonsterInfo->siY, IP, 0.0f, lpOnCharMonsterInfo->siFightingPower, this,NULL);
						
						lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharMonsterInfo));
					}
					else if(ISSHIP(lpOnCharBaseInfo->uiAccount))
					{
						lpOnCharShipInfo = (OnCharShipInfo*)lpOnCharBaseInfo;

						// 棻艇 議葛攪曖 渠и 薑爾蒂 撲薑и棻.						
						ZeroMemory(TempBuffer, 128);
						IP = 0;
						pIOnlineChar->Init(lpOnCharShipInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharShipInfo->uiKind, lpOnCharShipInfo->siX, lpOnCharShipInfo->siY, IP, 0.0f, 0, this,NULL);

						lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharShipInfo));
					}
				}		
				
				pRecvFieldItemInfo		=	(OnlineReceiveFieldItemInfo*)((CHAR*)lpOnCharBaseInfo);
				pRecvFollowerInfo       =   (OnFollowerInfo*)((CHAR*)pRecvFieldItemInfo + sizeof(OnlineReceiveFieldItemInfo) * pReturnOnlineFieldMsg->uiFieldItemNum);
				uiFollowerCount			=	pReturnOnlineFieldMsg->uiFollowerNum;
				
				// в萄曖 嬴檜蠱擊 撲薑и棻. 
				for(i = 0; i < 	pReturnOnlineFieldMsg->uiFieldItemNum; i++)
				{
					pOnlineMap->SetID(pRecvFieldItemInfo[i].siX, pRecvFieldItemInfo[i].siY, 1, 1, 0, 0, pRecvFieldItemInfo[i].uiItemID, pRecvFieldItemInfo[i].uiQuantity);
				}
				
				uiNextMenu	=	ON_MAINPOLL_GAME;
#ifdef _OGG_PLAY
				GetBgmMgr()->PlayNextBgm(TRUE);
#else
				// 擠學擊 翱輿衛鑑棻.
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
				
				// 葆擊 薑爾蒂 蹺陛и棻.
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
					, pReturnOnlineVillageMsg->VillageInfo.siDefenceSoldier	// ⑷營 熱綠煽 睡渠熱
					, pReturnOnlineVillageMsg->VillageInfo.siDefenceSoldier	/*pReturnOnlineVillageMsg->VillageInfo.siMaxDefenceSoldier*/	// 譆渠 熱綠煽 睡渠熱
					, pReturnOnlineVillageMsg->VillageInfo.siAttackType	// 葆擊 奢問 顫殮
					);

				// 葆擊 偌羹蒂 儅撩и棻.
				pOnlineVillage	=	new OnlineVillage;	
				
				// 葆擊擊 蟾晦�� и棻.
				pVillageHeader	=	pOnlineVillageManager->GetVillageByCode(pReturnOnlineVillageMsg->uiVillageCode);
				pOnlineMap->ChangeMap(pVillageHeader->siMapIndex, pVillageHeader->siX, pVillageHeader->siY);
				pOnlineMap->PlayFitMusic();
				if(pOnlineMap->m_MapInfo.uiVillageCode)
				{
					pOnlineClient->SendRequestVillageInfo(pOnlineMap->m_MapInfo.uiVillageCode);
				}
				
				pOnlineVillage->Init(pReturnOnlineVillageMsg->uiVillageCode, &pReturnOnlineVillageMsg->VillageInfo, this);	
				
				// 曖錳縑 菟罹爾魚棻.
				pOnlineVillage->MoveInStruct(ON_VIL_ST_KIND_HOSPITAL, FALSE, TRUE);
				pOnlineVillage->fDead = TRUE;
				
				// 棻擠 詭景蒂 撲薑и棻.
				uiNextMenu	=	ON_MAINPOLL_GAME;		
				
				// ⑷營 啪歜 鼻鷓陛 葆擊 寰縑 氈棻堅 撲薑и棻.
				siGamePollStatus	=	ON_GAMEPOLL_VILLAGE;	
				
				pRecvFollowerInfo       =   (OnFollowerInfo*)((CHAR*)pReturnOnlineVillageMsg + sizeof(OnResReturnOnlineVillageMsg));
			}
			break;
		default:
			clGrp.Error("ReceiveReturnOnline", "Unknow Data : [%d]", pReturnOnlineMsg->dwResponse);
			break;
		}
	}

	// 嬴霜 橫雯 詭衛雖紫 螃雖 彊懊棻.			
	// 衛除檜 謙猿腎賊 煎斜檣 �飛橉虞� 腎給萼棻.
	if(timeGetTime() > DWORD((pOnlineReturnOnline->dwRequestGameDataTime + (60 * 1000))))
	{
		uiNextMenu		=	ON_MAINPOLL_FORCED_LOGOUT;
//		SetLogOut(GS_INITQUIT, 0);
	}
	
	// 衛除擊 �飛橦� 鎰橫遽棻.
	sprintf(szTextBuffer, pOnlineText->Get(ON_TEXT_DATA_WAITTINGSERVER), max(60 - ((timeGetTime() - pOnlineReturnOnline->dwRequestGameDataTime) / 1000), 0));		
	
	// �飛橦� 鎰橫遽棻.
	// 斜萼棻.
	if(GETSCREENSURFACE(this)->IsLost() != DD_OK)
	{
		GETSCREENSURFACE(this)->Restore();
	}
	clGrp.ClearSurface(GETSCREENSURFACE(this));
	
	// 旋濠蒂 鎰朝棻.
	if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
	{
		// 癱貲 賅萄 撲薑.
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
		
	// 睡ж菟檜 薑爾蒂 營 撲薑и棻.
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
	// actdoll (2004/06/16 20:00) : 瓣た 薑爾蒂 錳楚渠煎 撲薑и棻.
	pOnlinePannel->Initialize();

	m_LastNowUserConnectMessageTickCount = 0;

	if(uiNextMenu != 0)			
	{
		FreeReturnOnline();	
	}

	return uiNextMenu;
}