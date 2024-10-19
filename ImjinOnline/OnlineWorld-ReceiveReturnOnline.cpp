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
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS �� ������ �߰�
#include "OnlinePannel.h"
#include "maindata.h"

// ���� ���� ������
#include	"SiegeUnitTableMgr.h"

#ifdef _OGG_PLAY
#include "BgmMgr_PreHeader.h"
#endif

extern SoundManager *g_pSoundManager;
extern	_MainData			GD;


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������2 Game���� ������ �¶������� ���� ���ؼ� �����ߴ� �޸𸮸� �ٽ� �Ҵ� �޴´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltOnlineWorld::InitReturnOnline()
{	
	//CHAR	szSpriteFileName[1024];
	//CHAR	szSmallFileName[1024];
	//SI32	siFile;
	//XSPR	*pTempSpr;
	//SI32	i;

	//DWriteStringToFile("InitReturnOnline() ����");
/*
	// ���ҽ��� �Ҵ� �޴´�.	
	pOnlineResource		=	new	OnlineResource;	
	pOnlineResource->Init();		

	for( i=1 ; i<(SI32)pItemsInfo->GetCount()+1 ; i++)											// ���ҽ��� ��ȯ
	{
		pItemsInfo->GetSpriteFileName( i, szSpriteFileName, szSmallFileName );
				
		if( ( stricmp( szSpriteFileName, "0" ) != 0 ) && ( stricmp( szSmallFileName, "0" ) != 0 ) )
		{
			if( pOnlineResource->GetItemFileIndex( szSpriteFileName ) == -1 )					// ū �׸��� �ε��Ѵ�.( �׸��� �ε尡 �ž� ���� �ʴٸ� �ε���)			
				pOnlineResource->AddItemSpr( szSpriteFileName );									

			if( pOnlineResource->GetItemFileIndex( szSmallFileName ) == -1 )					// ���� �׸��� �ε��Ѵ�.			
				pOnlineResource->AddItemSpr( szSmallFileName );			
		}		

		ZeroMemory( szSpriteFileName, 1024 );
		ZeroMemory( szSmallFileName, 1024 );
	}

	
	for( i=1; i<(SI32)pItemsInfo->GetCount() + 1; i++)											// �ٽ� �ѹ� ������ ����ü�� ���ҽ� �ε����� �־� �ش�.
	{		
		pItemsInfo->GetSpriteFileName( i, szSpriteFileName, szSmallFileName );					// ������ ������ �޴´�.

		if( ( stricmp( szSpriteFileName, "0" ) != 0 ) && ( stricmp( szSmallFileName, "0" ) != 0 ) )
		{
			// ū �׸�
			siFile		=	pOnlineResource->GetObjectFileIndex( szSpriteFileName );			// ���ҽ����� ���� �̸��־� �ε����� �޴´�.
			pTempSpr	=	pOnlineResource->GetxsprObject( siFile );

	
			if( siFile != -1 )		pItemsInfo->SetFile( siFile, szSpriteFileName, pTempSpr->Header.Xsize, pTempSpr->Header.Ysize, FALSE );
			else					clGrp.Error("", "�ƾ��� ���� ã�� ���� [%s]", szSpriteFileName );

			// ���� �׸�
			siFile		=	pOnlineResource->GetObjectFileIndex( szSmallFileName );				// ���ҽ����� ���� �̸��־� �ε����� �޴´�.
			pTempSpr	=	pOnlineResource->GetxsprObject( siFile );
			
	
			if( siFile != -1 )		pItemsInfo->SetFile( siFile, szSmallFileName, pTempSpr->Header.Xsize, pTempSpr->Header.Ysize, TRUE );
			else					clGrp.Error("", "�ƾ��� ���� ã�� ���� [%s]", szSmallFileName );
		}

		ZeroMemory( szSpriteFileName, 1024 );
		ZeroMemory( szSmallFileName, 1024 );
	}

	// ���̷�Ʈ ��ο� ��ü�� �����Ѵ�. �ݵ�� ���ҽ��� �ε��� �� ȣ���� �־�� �Ѵ�.
	pOnlineDirectDraw	=	new	OnlineDirectDraw;
	pOnlineDirectDraw->Init(this);

	// ���� �޸𸮸� �Ҵ� �޴´�.
	pOnlineMap	=	new OnlineMap;		
	pOnlineMap->Load("Online\\Map\\map.map", this);
*/
	// ����Ÿ�� �Ҵ� �޴´�.
	pOnlineReturnOnline	=	new OnlineReturnOnline;

	// �޽��� ���ۿ� �ִ� �޽����� �ʱ�ȭ�Ѵ�.
	InitMessage();


	// ĳ���� �������̽��� �����Ѵ�.
	if(!pIOnlineChar)
	{
		pIOnlineChar = new IOnlineChar;	
		pIOnlineChar->Init(this);
	}
	pIOnlineChar->AllFree();

	// ������Ʈ �������̽��� �����Ѵ�.
	pIOnlineObject	=	new	IOnlineObject;
	pIOnlineObject->Init();			

	// ����� �Է��� ���� �� �ֵ��� �Ѵ�.
	// �׸��� Battle Response�� �� �̻� ��ٸ��� �ʴ´�.
	pMyData->SetWaitBattleResponse(FALSE);
	pOnlineCharUI->SetAction(TRUE);	

	// ������ ���� �¶������� ���ư��ٰ� ������ �ش�.	
	pOnlineClient->SendReturnOnline(GD.WinSwitch);	

	pOnlineReturnOnline->dwRequestGameDataTime	=	timeGetTime();

	m_siUpdateMethod = 0;
	m_siUpdateCounter = 0;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������2���� �¶������� �ٽ� ���ư���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
UI16	cltOnlineWorld::ReturnOnline()
{
	UI16 uiProc = pOnlineClient->DataProc();
	if( uiProc )	return ReceiveMessageProc( uiProc );
	return 0;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������2���� �¶������� �ٽ� ���ư���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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
				//DWriteStringToFile("ON_CMDPROC_RETURNONLINE �޽��� ����");
				pReturnOnlineFieldMsg		=	(OnResReturnOnlineFieldMsg*)pReturnOnlineMsg;
				
				// Base Data
				pMyData->SetMoney(pReturnOnlineFieldMsg->siMoney);
				pMyData->SetHealth(pReturnOnlineFieldMsg->siHealth);
				pIOnlineChar->SetRank(pMyData->GetMyCharUniqueID(), pReturnOnlineFieldMsg->siEventRanking);			
				pIOnlineChar->SetScore(pMyData->GetMyCharUniqueID(), pReturnOnlineFieldMsg->siEventScore);
				
				
				// Map��ġ�� �����Ѵ�.
				lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)pReturnOnlineFieldMsg + sizeof(OnResReturnOnlineFieldMsg));
				for(i = 0; i < pReturnOnlineFieldMsg->uiPlayerNum; i++)
				{
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
				
				// ������ �÷��̾ �����Ѵ�.
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
							
							// ���� ////////////////////////////////
							// robypark 2004/11/9 21:16
							// ���� ���� ���� ��� ���
							LPSSiegeUnitTable lpsSiegeUnitTable = GetSiegeUnitTableMgr()->GetSiegeUnitTable(lpOnCharPlayerInfo->uiKind);

							// ������ ������ ���� ���
							if (NULL != lpsSiegeUnitTable)
							{
								SetIsShowInformationAboutSiegeWarfareUnit(ON_GAMEOPTION_SIEGEWARFAREUNIT);	// ������ �����̴�.
							}
							// �� ////////////////////////////////

							pMyData->nRank = pReturnOnlineFieldMsg->siEventRanking;
							pMyData->nScore = pReturnOnlineFieldMsg->siEventScore;
							pIOnlineChar->SetRank(pMyData->GetMyCharUniqueID(), pReturnOnlineFieldMsg->siEventRanking);			
							pIOnlineChar->SetScore(pMyData->GetMyCharUniqueID(), pReturnOnlineFieldMsg->siEventScore);

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
				
				pRecvFieldItemInfo		=	(OnlineReceiveFieldItemInfo*)((CHAR*)lpOnCharBaseInfo);
				pRecvFollowerInfo       =   (OnFollowerInfo*)((CHAR*)pRecvFieldItemInfo + sizeof(OnlineReceiveFieldItemInfo) * pReturnOnlineFieldMsg->uiFieldItemNum);
				uiFollowerCount			=	pReturnOnlineFieldMsg->uiFollowerNum;
				
				// �ʵ��� �������� �����Ѵ�. 
				for(i = 0; i < 	pReturnOnlineFieldMsg->uiFieldItemNum; i++)
				{
					pOnlineMap->SetID(pRecvFieldItemInfo[i].siX, pRecvFieldItemInfo[i].siY, 1, 1, 0, 0, pRecvFieldItemInfo[i].uiItemID, pRecvFieldItemInfo[i].uiQuantity);
				}
				
				uiNextMenu	=	ON_MAINPOLL_GAME;
#ifdef _OGG_PLAY
				GetBgmMgr()->PlayNextBgm(TRUE);
#else
				// ������ ���ֽ�Ų��.
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
				
				// ���� ������ �߰��Ѵ�.
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
					, pReturnOnlineVillageMsg->VillageInfo.siDefenceSoldier	// ���� ���� �δ��
					, pReturnOnlineVillageMsg->VillageInfo.siDefenceSoldier	/*pReturnOnlineVillageMsg->VillageInfo.siMaxDefenceSoldier*/	// �ִ� ���� �δ��
					, pReturnOnlineVillageMsg->VillageInfo.siAttackType	// ���� ���� Ÿ��
					);

				// ���� ��ü�� �����Ѵ�.
				pOnlineVillage	=	new OnlineVillage;	
				
				// ������ �ʱ�ȭ �Ѵ�.
				pVillageHeader	=	pOnlineVillageManager->GetVillageByCode(pReturnOnlineVillageMsg->uiVillageCode);
				pOnlineMap->ChangeMap(pVillageHeader->siMapIndex, pVillageHeader->siX, pVillageHeader->siY);
				pOnlineMap->PlayFitMusic();
				if(pOnlineMap->m_MapInfo.uiVillageCode)
				{
					pOnlineClient->SendRequestVillageInfo(pOnlineMap->m_MapInfo.uiVillageCode);
				}
				
				pOnlineVillage->Init(pReturnOnlineVillageMsg->uiVillageCode, &pReturnOnlineVillageMsg->VillageInfo, this);	
				
				// �ǿ��� �鿩������.
				pOnlineVillage->MoveInStruct(ON_VIL_ST_KIND_HOSPITAL, FALSE, TRUE);
				pOnlineVillage->fDead = TRUE;
				
				// ���� �޴��� �����Ѵ�.
				uiNextMenu	=	ON_MAINPOLL_GAME;		
				
				// ���� ���� ���°� ���� �ȿ� �ִٰ� �����Ѵ�.
				siGamePollStatus	=	ON_GAMEPOLL_VILLAGE;	
				
				pRecvFollowerInfo       =   (OnFollowerInfo*)((CHAR*)pReturnOnlineVillageMsg + sizeof(OnResReturnOnlineVillageMsg));
			}
			break;
		default:
			clGrp.Error("ReceiveReturnOnline", "Unknow Data : [%d]", pReturnOnlineMsg->dwResponse);
			break;
		}
	}

	// ���� � �޽����� ���� �ʾҴ�.			
	// �ð��� ����Ǹ� �α��� ȭ������ �ǵ�����.
	if(timeGetTime() > DWORD((pOnlineReturnOnline->dwRequestGameDataTime + (60 * 1000))))
	{
		uiNextMenu		=	ON_MAINPOLL_FORCED_LOGOUT;
//		SetLogOut(GS_INITQUIT, 0);
	}
	
	// �ð��� ȭ�鿡 ����ش�.
	sprintf(szTextBuffer, pOnlineText->Get(ON_TEXT_DATA_WAITTINGSERVER), max(60 - ((timeGetTime() - pOnlineReturnOnline->dwRequestGameDataTime) / 1000), 0));		
	
	// ȭ�鿡 ����ش�.
	// �׸���.
	if(GETSCREENSURFACE(this)->IsLost() != DD_OK)
	{
		GETSCREENSURFACE(this)->Restore();
	}
	clGrp.ClearSurface(GETSCREENSURFACE(this));
	
	// ���ڸ� ��´�.
	if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
	{
		// ���� ��� ����.
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
		
	// ���ϵ��� ������ �� �����Ѵ�.
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
	// actdoll (2004/06/16 20:00) : ä�� ������ ������� �����Ѵ�.
	pOnlinePannel->Initialize();

	m_LastNowUserConnectMessageTickCount = 0;

	if(uiNextMenu != 0)			
	{
		FreeReturnOnline();	
	}

	return uiNextMenu;
}