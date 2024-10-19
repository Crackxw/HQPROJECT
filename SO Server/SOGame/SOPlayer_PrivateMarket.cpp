#include "SOMain.h"
#include "SOWorld.h"
#include "SOPlayer.h"
#include "PrivateMarket.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	喻薄鼻擊 翱棻.
//	熱薑 橾濠	:	2002-07-02 螃�� 3:54:15 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvPMOpen()
{
	OnReqPMOpenMsg		*pPMOpenMsg;
	OnBoothItemForSell	*pBoothItem;
	SI32				i;
	BOOL				bSuccess	=	FALSE;
	strPMItem			*pstPMItem;

	pPMOpenMsg		=	(OnReqPMOpenMsg*)cMsg;
	pBoothItem		=	(OnBoothItemForSell*)((CHAR*)pPMOpenMsg + sizeof(OnReqPMOpenMsg));

	// ⑷營 瞪癱 渠晦醞檜剪釭 瞪癱 醞縑 氈戲賊 寰脹棻.
	if(GetBattle() != NULL)	
	{
		goto	END;
	}
	
	// ⑷營 偃檣 鼻薄擊 翱 鼻鷓檣雖 匐餌и棻.
	if(clPrivateMarket.IsOpen() == TRUE)
	{
		goto	END;
	}

	for(i = 0; i < pPMOpenMsg->uiItemNum; i++)
	{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 檜 鼻 團 熱薑 ( 2003. 3. 14 )
// 辨煽 剪楚 睡碟 蹺陛 熱薑 衛濛 睡碟
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		if ( pBoothItem[i].uiTradeFollowerID  && pBoothItem[i].uiItemID  ) 
		{
			writeInfoToFile("Private.txt", "[%d] pBoothItem[%d].uiTradeFollowerID  && pBoothItem[%d].uiTradeFollowerID -> both have value 0\n", i, i, i);
			goto END;
		}

		if ( pBoothItem[i].uiTradeFollowerID )			//辨煽 剪楚檣 唳辦
		{
			// 嶸�褲� 辨煽檣雖蒂 羹觼
			if( IFollower.IsValidFollower(pBoothItem[i].uiTradeFollowerID) == FALSE )
			{
				goto	END;
			}

			if( pBoothItem[i].uiTradeFollowerID == 0 )		//輿檣奢擊 だ朝 氈擊 熱 橈朝 唳辦歜. 斜傖 蕨諼瞳檣 鼻�窕� 釭顫陳 匙縑 渠綠п憮..
			{
				goto	END;
			}

			//辨煽檜 濰熱檣雖蒂 羹觼
			if ( this->IsGeneral(pBoothItem[i].uiTradeFollowerID) )
			{
				goto	END;
			}


			if( pBoothItem[i].Price == 0 )
			{
				goto	END;
			}
			
			/*
			//辨煽檜 跨蝶攪檣雖蒂 羹觼
			if ( this->IsMonster(pBoothItem[i].uiTradeFollowerID) )
			{
				goto	END;
			}
			*/

			
			
			if(clPrivateMarket.AddItem(&pBoothItem[i]) == FALSE)
			{
				goto	END;
			}
			OnlineFollower*	 lpOnlineFollower = IFollower.GetFollower(pBoothItem[i].uiTradeFollowerID);

			if(lpOnlineFollower != NULL)
			{
				for(SI16 j = 0; j < FIELD_BATTLE_ATTACK_UNIT_NUM ; j++)
				{
					MyItemData*		pMyItemData;
					SI16 siItemID = (SI16)pMyWorld->m_FieldBattleParser.GetItemID(j);
					
					if((pMyItemData = lpOnlineFollower->GetItem(siItemID)) != NULL)
					{
						goto END;
					}
				}
			}
		}
		else											//嬴檜蠱 剪楚檣 唳辦
		{
			// 奩萄衛 嶸�褲� 辨煽檜罹撿 и棻.
			if(IFollower.IsValidFollower(pBoothItem[i].uiFollowerID) == FALSE)
			{
				goto	END;
			}

			// 褒薯 辨煽縑啪 偽擎 嬴檜蠱檜 氈朝雖 匐餌и棻.		
			if(IFollower.GetItemByID(pBoothItem[i].uiFollowerID, pBoothItem[i].uiItemID) == NULL)
			{
				goto	END;		// 嬴檜蠱擊 橢橫螞棻.
			}

			// 嬴檜蠱擊 蹺陛и棻.		
			//if(clPrivateMarket.AddItem(pBoothItem[i].uiBoothSlot, pBoothItem[i].uiFollowerID, pBoothItem[i].uiItemID, pBoothItem[i].Price, pBoothItem[i].Desc) == FALSE)
			if(clPrivateMarket.AddItem(&pBoothItem[i]) == FALSE)
			{
				goto	END;
			}
			OnlineFollower*	 lpOnlineFollower = IFollower.GetFollower(pBoothItem[i].uiTradeFollowerID);

			if(lpOnlineFollower != NULL)
			{
				for(SI16 j = 0; j < FIELD_BATTLE_ATTACK_UNIT_NUM ; j++)
				{
					MyItemData*		pMyItemData;
					SI16 siItemID = (SI16)pMyWorld->m_FieldBattleParser.GetItemID(j);
					
					if((pMyItemData = lpOnlineFollower->GetItem(siItemID)) != NULL)
					{
						goto END;
					}
				}
			}

		}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 檜 鼻 團 熱薑 ( 2003. 3. 14 )
// 辨煽 剪楚 睡碟 蹺陛 部 睡碟
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}	


	// 鼻薄擊 翱棻.
	clPrivateMarket.Open();
	
	// 顫 嶸盪縑啪 だじ 褐羶醞檜剪釭 
	// 棻艇 嶸盪縑 だじ 褐羶擊 嫡懊棻賊 檜蒂 鏃模и棻.
	CloseRequestingJoinParty();

	bSuccess					=	TRUE;

END:
	if(bSuccess == TRUE)
	{	
		// 辦摹 釭諦 頂 輿嬪縑啪 謝っ擊 翮塭朝 詭衛雖蒂 爾頂遽棻.
		SendFieldMsg(ON_RESPONSE_PM_OPEN, SO_SFM_MEANDNEARCHAR, LPVOID(ON_RET_OK));	

		// 輿嬪曖 Ы溯檜橫縑啪朝 嬴檜蠱擊 蹺陛ж塭堅紫 憲溥遽棻.
		for(i = 0; i < ON_MAX_BOOTH_ITEM; i++)		
		{	
			if((pstPMItem = clPrivateMarket.GetItem(i)) != NULL)
			{	
				switch ( pstPMItem->siType )
				{
				case LIST_ITEM:
					SendFieldMsg(ON_PM_CHANGE_BOOTH_ITEM, SO_SFM_ONLYNEARCHAR, LPVOID(uiAccount), LPVOID(i), LPVOID(pstPMItem->uiItemID), LPVOID(pstPMItem->siType));
					break;
				case LIST_FOLLOWER:
					SendFieldMsg(ON_PM_CHANGE_BOOTH_ITEM, SO_SFM_ONLYNEARCHAR, LPVOID(uiAccount), LPVOID(i), LPVOID(IFollower.GetFollowerParameterKind(pstPMItem->uiTradeFollowerSlotID)), LPVOID(pstPMItem->siType) );
					break;
				}
				
				
			}
		}
	}
	else
	{
		// 褒ぬц擊衛 濛機醞縑 菟橫除 噙溯晦高菟擊 雖辦晦 嬪ж罹 謝っ擊 棻衛 蟾晦�� и棻.
		clPrivateMarket.Init();

		// 褒ぬц棻堅 詭衛雖蒂 爾頂遽棻.
		SendFieldMsg(ON_RESPONSE_PM_OPEN, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	喻薄鼻擊 殘朝棻.
//	熱薑 橾濠	:	2002-06-25 螃�� 4:30:14 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvPMClose()
{
	OnReqPMCloseMsg		*pPMCloseMsg;
	BOOL				bSuccess	=	FALSE;

	pPMCloseMsg		=	(OnReqPMCloseMsg*)cMsg;

	// ⑷營 偃檣 鼻薄擊 翱 鼻鷓橾陽縑虜 嫡擊 熱 氈棻.
	if(clPrivateMarket.IsOpen() == FALSE)	
	{
		goto	END;		
	}
	
	// 鼻薄擊 殘朝棻.
	
	clPrivateMarket.Close();

	bSuccess	=	TRUE;
END:
	if(bSuccess == TRUE)
	{
		// 鼻薄擊 殘擊 熱 氈棻.
		SendFieldMsg(ON_RESPONSE_PM_CLOSE, SO_SFM_MEANDNEARCHAR, LPVOID(ON_RET_OK));
	}
	else
	{
		SendFieldMsg(ON_RESPONSE_PM_CLOSE, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	喻薄鼻縑啪 辨煽 塽 嬴檜蠱擊 骯棻.
//	熱薑 橾濠	:	2002-06-25 螃�� 4:59:40 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvPMBuyItem()
{
	
	OnReqPMBuyItemMsg		*pPMBuyItemMsg;
	SOPlayer				*pPlayer;	
	BOOL					bSuccess = FALSE;		
	strPMItem				*pstPMItem, stTempPMItem;		

	MONEY					mnTempTotalItemPrice;
	MONEY					TotalItemPrice;
	MONEY					toMoney;
	MONEY					fromMoney;
	UI16					uiRemainQuantity;					
	SI16                    siFromMercenarySlot = -1;

		
	UI08					uiFollowerNum = 0;

	MyItemData              *pMyItem;

	CHAR					cFollowerBuffer1[10240];
	CHAR					szTrade1DBArg[1024];
	CHAR					szTrade2DBArg[1024];

	CHAR					szAccountDBArg[1024];
	CHAR					szCompletedDBArg[1024];	

	DWORD					dwFollowerDataSize;
	OnFollowerInfo			*pFollowerInfo;
	SI32					siNation;
	SI16					siEmptyFollowerSlot;

	pPMBuyItemMsg		=	(OnReqPMBuyItemMsg*)cMsg;

	ZeroMemory( szCompletedDBArg, sizeof( szCompletedDBArg ) );	
	ZeroMemory( szAccountDBArg, sizeof( szAccountDBArg ) );	
	ZeroMemory( szTrade1DBArg, sizeof( szTrade1DBArg ) );
	ZeroMemory( szTrade2DBArg, sizeof( szTrade2DBArg ) );

	pFollowerInfo = ( OnFollowerInfo * ) cFollowerBuffer1;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 檜 鼻 團 熱薑 ( 2003. 3. 14 )
// 辨煽 剪楚 睡碟 蹺陛 衛濛 睡碟
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// ⑷營 嬴檜蠱擊 餌溥堅 ж朝 議葛攪陛 嶸�褲捘� 憲嬴螞棻.
	if( ( pPlayer = pMyWorld->GetPlayer( pPMBuyItemMsg->uiAccount ) ) == NULL)
	{
		goto	END;
	}

	// slot曖 廓�ㄧ� 晦遽戲煎 餌溥堅 ж朝 嬴檜蠱檜釭 辨煽曖 薑爾蒂 橢橫螞棻.
	if( ( pstPMItem = pPlayer->clPrivateMarket.GetItem( pPMBuyItemMsg->uiSlotID ) ) == NULL )
	{
		goto	END;
	}

	if(clPrivateMarket.IsOpen() == TRUE)
	{
		goto	END;
	}

	stTempPMItem	=	*pstPMItem;

	// п渡 ヶ跡(辨煽 塽 嬴檜蠱)擊 掘殮ж艘擊 衛 絲擊 嫡朝 Ы溯檜橫陛 模嶸й 熱 氈朝 譆渠 絲擊 剩朝雖 匐餌и棻.
	// 虜擒縑 模嶸й 熱 譆渠 歜旎擊 剩朝棻賊 ヶ跡擊 つ雖 跤и棻.
	if( pPlayer->clPrivateMarket.GetTotalPrice( pPMBuyItemMsg->uiSlotID, pPMBuyItemMsg->uiQuantity, &mnTempTotalItemPrice ) == FALSE )
	{
		goto	END;
	}

	if( ( pPlayer->GetMaxMoneyToGet() < mnTempTotalItemPrice ) ||
		( EnoughMoney( mnTempTotalItemPrice ) == FALSE ) )
	{
		goto	END;
	}

	switch( pstPMItem->siType )
	{
	case LIST_FOLLOWER:
		{
			//Trade й溥朝 辨煽檜 嶸�褲捘�..
			if( ! pPlayer->IFollower.IsValidFollower( pstPMItem->uiTradeFollowerSlotID ) )
			{
				goto	END;
			}

			// 頂陛 譆渠 陛韓 熱 氈朝 辨煽爾棻 蟾婁ж雖 彊朝雖 憲嬴螞棻.
			if(uiFollowerNum >= GetMaxFollowerNum() )
			{
				goto	END;
			}

			//輿檣奢擊 掘殮ж朝 氈擊 熱 橈朝 縑楝檜雖虜, 蕨諼 籀葬煎 п 輿歷擠.
			if ( pstPMItem->uiTradeFollowerSlotID == 0)
			{
				goto	END;
			}

			//濰熱 剪楚 й 熱 橈擠
			if( pPlayer->IsGeneral( pstPMItem->uiTradeFollowerSlotID ) )
			{
				goto	END;
			}


			// 跨蝶攪朝 3 葆葬 檜鼻 陛韓 熱陛 橈棻.
			// ⑷營 陛雖堅 氈朝 跨蝶攪陛 2葆葬檜堅, 跨蝶攪蒂 掘殮ж溥堅 ж艘擊衛
			// 褒ぬж紫煙 и棻.
			if( pPlayer->IsMonster( pstPMItem->uiTradeFollowerSlotID ) )
			{
				if( GetMonsterNum() >= 2 )			goto	END;
			}

			/*
			if( ! CanAddFollower( pstPMItem->uiTradeFollowerSlotID ) )
			{
				goto	END;
			}
			*/

			// 辨煽曖 措陛蒂 橢橫螞棻.
			siNation	=	pPlayer->IFollower.GetFollowerParameterNation(pstPMItem->uiTradeFollowerSlotID);

			// 辨煽曖 薑爾蒂 橢橫螃朝等 撩奢п撿и棻.
			if( (dwFollowerDataSize = pPlayer->GetFollowerInfo(pstPMItem->uiTradeFollowerSlotID, pFollowerInfo)) == 0 )
			{
				goto	END;
			}

			// 鼻渠寞 辨煽擎 餉薯. 
			if(pPlayer->IFollower.DeleteFollower(pstPMItem->uiTradeFollowerSlotID) == FALSE)	
			{
				goto	END;
			}

			// 辨煽擊 嫡朝 Ы溯檜橫(釭 濠褐 = this)陛 辨煽擊 橫蛤縑 蹺陛衛麵撿 ж朝雖 憲嬴螞棻.
			if((siEmptyFollowerSlot = IFollower.FindEmptyFollowerSlot()) == -1)
			{
				goto	END;
			}

			// 釭縑啪 鼻渠寞曖 辨煽擊 蹺陛衛麵遽棻.
			if(IFollower.InitFollower(pFollowerInfo, siNation, &pMyWorld->clItem) == TRUE)	
			{
				//*puiOppFollowerID				=	siEmptyFollowerSlot;

				// pPlayer: 謝っ擊 翮堅 氈朝 Ы溯檜橫
				if( pstPMItem->uiTradeFollowerSlotID == pPlayer->siVMercenarySlot )
				{
					pPlayer->RecvVMercenaryChange(0);
				}

				//	dwFollowerDataSize;
			}
			else
			{
//				clServer->WriteInfo( "..\\FollowerTradeError.txt", 	"InitFollower failed" );
				writeInfoToFile( "FollowerTradeError.txt", 	"InitFollower failed" );
				goto	END;
			}

			// 撩奢瞳戲煎 辨煽擊 嫡懊棻.

			//pFollowerInfo	=	(OnFollowerInfo*)((CHAR*)pFollowerInfo + dwFollowerDataSize);

			toMoney = DecreaseMoneyReturnMoney(pstPMItem->Price);
			// db蘭葬蒂 嬪п 曄熱煎 夥羞棻.
			toMoney = -toMoney;
			fromMoney = pPlayer->IncreaseMoney(pstPMItem->Price,FALSE);


			//siDBGoodKind	=	2;
			//siDBGoodParam1	=	pstPMItem->uiTradeFollowerSlotID;
			//siDBGoodParam2	=	siEmptyFollowerSlot;
			

			break;
		}
	case LIST_ITEM:
		{
			// 嬴檜蠱擊 餌憮 檣漸饜葬縑 厥橫撿 ж朝等
			// ⑷營 檣漸饜葬縑 嬴檜蠱擊 蹺陛 衛鑒 熱 氈朝雖 憲嬴螞棻.
			if(IFollower.CanAddItem(pPMBuyItemMsg->uiFollowerID, stTempPMItem.uiItemID, pPMBuyItemMsg->uiQuantity, pPMBuyItemMsg->uiInvenPos) == FALSE)
			{
				goto	END;
			}
				

			// 偃檣 鼻薄縑憮 嬴檜蠱擊 掘殮и棻.
			if( pPlayer->BuyPMItem( pPMBuyItemMsg->uiSlotID, pPMBuyItemMsg->uiQuantity, siMoney, 
									&uiRemainQuantity, &TotalItemPrice,&siFromMercenarySlot ) == FALSE )
			{
				goto	END;			
			}

			// 嬴檜蠱擊 渦п遽棻.
			if((pMyItem = AddItem(pPMBuyItemMsg->uiFollowerID, stTempPMItem.uiItemID, pPMBuyItemMsg->uiQuantity, pPMBuyItemMsg->uiInvenPos,FALSE)) == NULL)
			{
//				pMyWorld->clServer->WriteInfo( "BuyItemFromPMFailed.txt", "[ Buyer : %s, Seller : %s, ItemID : %u, ItemQuantity : %u, Price : %I64d ]", szID, pPlayer->szID, 
				writeInfoToFile( "BuyItemFromPMFailed.txt", "[ Buyer : %s, Seller : %s, ItemID : %u, ItemQuantity : %u, Price : %I64d ]", szID, pPlayer->szID, 
													stTempPMItem.uiItemID, pPMBuyItemMsg->uiQuantity, TotalItemPrice );
				goto	END;		
			}
			// 掘殮ж溥朝 Ы溯檜橫曖 絲擊 貍堅 っ衙и Ы溯檜橫曖 絲擊 隸陛 衛鑑棻.
			toMoney = DecreaseMoneyReturnMoney(TotalItemPrice);
			// db蘭葬蒂 嬪п 曄熱煎 夥羞棻.
			toMoney = -toMoney;
			fromMoney = pPlayer->IncreaseMoney(TotalItemPrice,FALSE);

			break;
		}
	}	
	bSuccess = TRUE;

END:
	if ( bSuccess )
	{
		switch ( pstPMItem->siType )
		{
		case LIST_ITEM:
			{
				
				DBIKBoothTrade dbinfo;

				dbinfo.siFromDBAccount = pPlayer->stAccountInfo.siDBAccount;
				dbinfo.uiFromDBSlot = pPlayer->stAccountInfo.uiSlot;
				dbinfo.uiFromMercenarySlot = siFromMercenarySlot;
				
				dbinfo.moFromMoney = fromMoney;

				dbinfo.siToDBAccount = this->stAccountInfo.siDBAccount;
				dbinfo.uiToDBSlot = this->stAccountInfo.uiSlot;
				dbinfo.uiToMercenarySlot = pPMBuyItemMsg->uiFollowerID;
				dbinfo.uiToItemSlot = pMyItem->siPosInInventory;
				dbinfo.moToMoney = toMoney;

				dbinfo.uiItemID = stTempPMItem.uiItemID;
				dbinfo.uiQuantity = pPMBuyItemMsg->uiQuantity;

				pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_BOOTHTRADE, LPVOID( &dbinfo ), sizeof( dbinfo ) );
				
				
				SendFieldMsg(ON_RESPONSE_PM_BUYITEM, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(pPMBuyItemMsg->uiSlotID), LPVOID(uiRemainQuantity));
				pPlayer->SendFieldMsg(ON_RESPONSE_PM_BUYITEM, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(pPMBuyItemMsg->uiSlotID), LPVOID(uiRemainQuantity));
				
				if(uiRemainQuantity == 0)
				{
					// �公� 嬴檜蠱擊 棻 餌憮 嬴檜蠱檜 橈橫螺棻賊 輿嬪 Ы溯檜橫縑啪紫 憲溥遽棻.
					pPlayer->SendFieldMsg(ON_PM_CHANGE_BOOTH_ITEM, SO_SFM_ONLYNEARCHAR, LPVOID(pPMBuyItemMsg->uiAccount), LPVOID(pPMBuyItemMsg->uiSlotID), LPVOID(0), LPVOID(pstPMItem->siType));
				}

				break;
			}

		case LIST_FOLLOWER:
			{
				DBIKBoothFollowerTrade dbinfo;

				dbinfo.siFromDBAccount = pPlayer->stAccountInfo.siDBAccount;
				dbinfo.uiFromDBSlot = pPlayer->stAccountInfo.uiSlot;
				dbinfo.siFromMercenarySlot = pstPMItem->uiTradeFollowerSlotID;
				
				dbinfo.moFromMoney = fromMoney;

				dbinfo.siToDBAccount = this->stAccountInfo.siDBAccount;
				dbinfo.uiToDBSlot = this->stAccountInfo.uiSlot;
				dbinfo.siToMercenarySlot = siEmptyFollowerSlot;

				dbinfo.moToMoney = toMoney;				
				
				pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_BOOTHFOLLOWERTRADE, LPVOID( &dbinfo ), sizeof( dbinfo ) );
								
				SendFieldMsg( ON_RESPONSE_PM_BUYFOLLOWER, SO_SFM_ONLYME, LPVOID( dwFollowerDataSize ), LPVOID( cFollowerBuffer1 ), LPVOID(1) );
				pPlayer->SendFieldMsg(ON_RESPONSE_PM_BUYITEM, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(pPMBuyItemMsg->uiSlotID), LPVOID(0));		//辨煽擎 つ堅 釭賊 陴擎 偎熱陛 0檜 腎嘎煎..

				//辨煽擊 掘殮п憮 謝っ檜釭 偃檣 鼻薄縑 辨煽檜 橈橫螺棻賊 輿嬪 Ы溯檜橫縑啪紫 憲溥遽棻.
				pPlayer->SendFieldMsg(ON_PM_CHANGE_BOOTH_ITEM, SO_SFM_ONLYNEARCHAR, LPVOID(pPMBuyItemMsg->uiAccount), LPVOID(pPMBuyItemMsg->uiSlotID), LPVOID(0), LPVOID(pstPMItem->siType));

				pPlayer->clPrivateMarket.DelItem(pPMBuyItemMsg->uiSlotID);
				break;
			}
		}
	}
	else
	{
		if ( pPMBuyItemMsg ->uiItemID )
		{
			SendFieldMsg(ON_RESPONSE_PM_BUYITEM, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		}
		else
		{
			SendFieldMsg(ON_RESPONSE_PM_BUYFOLLOWER, SO_SFM_ONLYME, LPVOID(10));
		}
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 檜 鼻 團 熱薑 ( 2003. 3. 14 )
// 辨煽 剪楚 睡碟 蹺陛 部 睡碟
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	


	
	//stTempPMItem	=	*pstPMItem;


	/*	
	// ⑷營 嬴檜蠱擊 餌溥堅 ж朝 議葛攪陛 嶸�褲捘� 憲嬴螞棻.
	if( ( pPlayer = pMyWorld->GetPlayer( pPMBuyItemMsg->uiAccount ) ) == NULL)
	{
		goto	END;
	}
	
	// 餌溥堅 ж朝 嬴檜蠱曖 薑爾蒂 橢橫螞棻.
	if( ( pstPMItem = pPlayer->clPrivateMarket.GetItem( pPMBuyItemMsg->uiSlotID ) ) == NULL )
	{
		goto	END;
	}

	stTempPMItem	=	*pstPMItem;

	// 嬴檜蠱擊 餌憮 檣漸饜葬縑 厥橫撿 ж朝等
	// ⑷營 檣漸饜葬縑 嬴檜蠱擊 蹺陛 衛鑒 熱 氈朝雖 憲嬴螞棻.
	if(IFollower.CanAddItem(pPMBuyItemMsg->uiFollowerID, stTempPMItem.uiItemID, pPMBuyItemMsg->uiQuantity, pPMBuyItemMsg->uiInvenPos) == FALSE)		goto	END;
	
	// 偃檣 鼻薄縑憮 嬴檜蠱擊 掘殮и棻.
	if(pPlayer->BuyPMItem(pPMBuyItemMsg->uiSlotID, pPMBuyItemMsg->uiQuantity, siMoney, &uiRemainQuantity, &TotalItemPrice,&siFromMercenarySlot) == FALSE)			goto	END;			

	// 嬴檜蠱擊 渦п遽棻.
	if((pMyItem = AddItem(pPMBuyItemMsg->uiFollowerID, stTempPMItem.uiItemID, pPMBuyItemMsg->uiQuantity, pPMBuyItemMsg->uiInvenPos,FALSE)) == NULL)
	{
		pMyWorld->clServer->WriteInfo( "BuyItemFromPMFailed.txt", "[ Buyer : %s, Seller : %s, ItemID : %u, ItemQuantity : %u, Price : %I64d ]", szID, pPlayer->szID, 
											stTempPMItem.uiItemID, pPMBuyItemMsg->uiQuantity, TotalItemPrice );
		goto	END;		
	}

	// 掘殮ж溥朝 Ы溯檜橫曖 絲擊 貍堅 っ衙и Ы溯檜橫曖 絲擊 隸陛 衛鑑棻.
	toMoney = DecreaseMoneyReturnMoney(TotalItemPrice);
	// db蘭葬蒂 嬪п 曄熱煎 夥羞棻.
	toMoney = -toMoney;
	fromMoney = pPlayer->IncreaseMoney(TotalItemPrice,FALSE);

	bSuccess	=	TRUE;
	
	
END:
	if(bSuccess == TRUE)
	{

		DBIKBoothTrade dbinfo;

		dbinfo.siFromDBAccount = pPlayer->stAccountInfo.siDBAccount;
		dbinfo.uiFromDBSlot = pPlayer->stAccountInfo.uiSlot;
		dbinfo.uiFromMercenarySlot = siFromMercenarySlot;
		
		dbinfo.moFromMoney = fromMoney;

		dbinfo.siToDBAccount = this->stAccountInfo.siDBAccount;
		dbinfo.uiToDBSlot = this->stAccountInfo.uiSlot;
		dbinfo.uiToMercenarySlot = pPMBuyItemMsg->uiFollowerID;
		dbinfo.uiToItemSlot = pMyItem->siPosInInventory;
		dbinfo.moToMoney = toMoney;


		dbinfo.uiItemID = pstPMItem->uiItemID;
		dbinfo.uiQuantity = pPMBuyItemMsg->uiQuantity;

		pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_BOOTHTRADE, LPVOID( &dbinfo ), sizeof( dbinfo ) );
		
		SendFieldMsg(ON_RESPONSE_PM_BUYITEM, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(pPMBuyItemMsg->uiSlotID), LPVOID(uiRemainQuantity));
		pPlayer->SendFieldMsg(ON_RESPONSE_PM_BUYITEM, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(pPMBuyItemMsg->uiSlotID), LPVOID(uiRemainQuantity));									
		
		if(uiRemainQuantity == 0)
		{
			// �公� 嬴檜蠱擊 棻 餌憮 嬴檜蠱檜 橈橫螺棻賊 輿嬪 Ы溯檜橫縑啪紫 憲溥遽棻.
			pPlayer->SendFieldMsg(ON_PM_CHANGE_BOOTH_ITEM, SO_SFM_ONLYNEARCHAR, LPVOID(pPMBuyItemMsg->uiAccount), LPVOID(pPMBuyItemMsg->uiSlotID), LPVOID(0));		
		}		
	}
	else
	{
		SendFieldMsg(ON_RESPONSE_PM_BUYITEM, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}
*/


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	喻薄鼻檜 陛雖堅 氈朝 嬴檜蠱曖 薑爾蒂 蹂掘и棻.
//	熱薑 橾濠	:	2002-06-25 螃�� 4:30:43 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvPMReqItemInfo()
{
	OnReqPMItemInfoMsg		*pPMItemInfo;
	BOOL					bSuccess = FALSE;
	SOPlayer				*pPlayer;	
	
	pPMItemInfo	=	(OnReqPMItemInfoMsg*)cMsg;

	if( IsTrading() )		goto END;

	// ⑷營 嬴檜蠱擊 餌溥堅 ж朝 議葛攪陛 偃檣 鼻薄擊 翮歷朝雖 憲嬴螞棻.
	if((pPlayer = pMyWorld->GetPlayer(pPMItemInfo->uiAccount)) == NULL)
	{
		goto	END;
	}

	if(pPlayer->clPrivateMarket.IsOpen() == FALSE)
	{
		goto	END;
	}

	bSuccess	=	TRUE;

END:
	if(bSuccess == TRUE)
	{
		SendFieldMsg(ON_RESPONSE_PM_ITEMINFO, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(pPlayer));
	}
	else
	{
		SendFieldMsg(ON_RESPONSE_PM_ITEMINFO, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	喻薄鼻檜 陛雖堅 氈朝 嬴檜蠱擊 滲唳и棻.
//	熱薑 橾濠	:	2002-07-02 螃�� 7:02:03 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvPMChangeItem()
{
	OnReqChangeBoothItemMsg		*pChangeBoothItem;
	OnBoothItemForSell			*pBoothItem;
	BOOL						bSuccess = FALSE;
	SI32						i;	
	MyItemData					*pMyItem;	
	strPMItem					*pstPMItem;

	pChangeBoothItem		=	(OnReqChangeBoothItemMsg*)cMsg;
	pBoothItem				=	(OnBoothItemForSell*)((CHAR*)pChangeBoothItem + sizeof(OnReqChangeBoothItemMsg));

	// ⑷營 偃檣 鼻薄擊 翱 鼻鷓檣雖 匐餌и棻.
	if(clPrivateMarket.IsOpen() == FALSE)
	{
		goto	END;
	}
	
	SI08 type;

	for(i = 0; i < 	pChangeBoothItem->uiItemNum; i++)
	{
		if ( pBoothItem[i].uiTradeFollowerID )
		{
			type = LIST_FOLLOWER;
			// 奩萄衛 嶸�褲� 辨煽檜罹撿 и棻.
			if( IFollower.GetFollower( pBoothItem[i].uiTradeFollowerID ) == NULL )
			{
				goto	END;
			}

			//濰熱 羹觼 塽 跨蝶攪 羹觼 輿 議葛攪 羹觼朝 賒(嬪縑憮 п 鍍戲嘎煎..)
			if( pBoothItem[i].uiTradeFollowerID == 0 )		//輿檣奢擊 だ朝 氈擊 熱 橈朝 唳辦歜. 斜傖 蕨諼瞳檣 鼻�窕� 釭顫陳 匙縑 渠綠п憮..
			{
				goto	END;
			}

			//辨煽檜 濰熱檣雖蒂 羹觼
			if ( this->IsGeneral(pBoothItem[i].uiTradeFollowerID) )
			{
				goto	END;
			}

			if( pBoothItem[i].Price == 0 )
			{
				goto	END;
			}

			if(pBoothItem[i].uiTradeFollowerID == 0)
			{
				// п渡 謝っ曖 嬴檜蠱擊 餉薯и棻.
				clPrivateMarket.DelItem(pBoothItem[i].uiBoothSlot);
				
				// 輿嬪曖 Ы溯檜橫菟縑啪 憲溥遽棻.
				SendFieldMsg(ON_PM_CHANGE_BOOTH_ITEM, SO_SFM_ONLYNEARCHAR, LPVOID(uiAccount), LPVOID(pBoothItem[i].uiBoothSlot), LPVOID(0), LPVOID(type));
			}
			else
			{
				if(clPrivateMarket.ChangeItem(pBoothItem[i].uiBoothSlot, 0, 0, pBoothItem[i].Price, pBoothItem[i].Desc, pBoothItem[i].uiTradeFollowerID) == FALSE)
				{
					goto	END;
				}
				if( (pstPMItem = clPrivateMarket.GetItem(pBoothItem[i].uiBoothSlot)) != NULL )
				{
					if((pstPMItem = clPrivateMarket.GetItem(pBoothItem[i].uiBoothSlot)) != NULL)
					{
						SendFieldMsg(ON_PM_CHANGE_BOOTH_ITEM, SO_SFM_ONLYNEARCHAR, LPVOID(uiAccount), LPVOID(pBoothItem[i].uiBoothSlot), LPVOID(IFollower.GetFollowerParameterKind(pstPMItem->uiTradeFollowerSlotID)), LPVOID(type));
					}
				}
			}
		}
		else if ( pBoothItem[i].uiItemID )
		{
			type = LIST_ITEM;
			// 奩萄衛 嶸�褲� 辨煽檜罹撿 и棻.
			if( IFollower.GetFollower(pBoothItem[i].uiFollowerID) == NULL )
			{
				goto	END;
			}
			
			if(pBoothItem[i].uiItemID == 0)
			{
				// п渡 謝っ曖 嬴檜蠱擊 餉薯и棻.
				clPrivateMarket.DelItem(pBoothItem[i].uiBoothSlot);
				
				// 輿嬪曖 Ы溯檜橫菟縑啪 憲溥遽棻.
				SendFieldMsg(ON_PM_CHANGE_BOOTH_ITEM, SO_SFM_ONLYNEARCHAR, LPVOID(uiAccount), LPVOID(pBoothItem[i].uiBoothSlot), LPVOID(0), LPVOID(type));
			}
			else
			{
				// п渡 謝っ曖 嬴檜蠱擊 滲唳и棻.
				// 褒薯 辨煽縑啪 偽擎 嬴檜蠱檜 氈朝雖 匐餌и棻.			
				if(IFollower.GetItemByID(pBoothItem[i].uiFollowerID, pBoothItem[i].uiItemID) == NULL)	goto	END;		// 嬴檜蠱擊 橢橫螞棻.
				if(clPrivateMarket.ChangeItem(pBoothItem[i].uiBoothSlot, pBoothItem[i].uiFollowerID, pBoothItem[i].uiItemID, pBoothItem[i].Price, pBoothItem[i].Desc, 0 ) == FALSE)
					goto	END;
				
				// 輿嬪曖 Ы溯檜橫菟縑啪 憲溥遽棻.
				if((pstPMItem = clPrivateMarket.GetItem(pBoothItem[i].uiBoothSlot)) != NULL)
				{
					if((pMyItem = IFollower.GetItemByID(pstPMItem->uiFollowerID, pstPMItem->uiItemID)) != NULL)
					{
						SendFieldMsg(ON_PM_CHANGE_BOOTH_ITEM, SO_SFM_ONLYNEARCHAR, LPVOID(uiAccount), LPVOID(pBoothItem[i].uiBoothSlot), LPVOID(pMyItem->uiID), LPVOID(type));
					}
				}		
			}
		}
	}

	bSuccess	=	TRUE;
END:
	if(bSuccess == TRUE)
	{
		SendFieldMsg(ON_RESPONSE_PM_CHANGEITEM, SO_SFM_ONLYME, LPVOID(ON_RET_OK));				
	}
	else
	{
		SendFieldMsg(ON_RESPONSE_PM_CHANGEITEM, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	偃檣 鼻薄縑憮 嬴檜蠱擊 髦 熱 氈朝陛?	
//	熱薑 橾濠	:	2002-07-05 螃�� 9:19:06 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::CanBuyPMItem(UI08 uiBoothSlot, UI16 uiItemQuantity, MONEY Money)
{
	return	clPrivateMarket.CanBuyItem(uiBoothSlot, uiItemQuantity, Money);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	偃檣 鼻薄縑憮 嬴檜蠱擊 骯棻.
//	熱薑 橾濠	:	2002-07-05 螃�� 9:32:12 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::BuyPMItem(UI08 uiBoothSlot, UI16 uiItemQuantity, MONEY Money, UI16 *puiRemainQuantity, MONEY *pTotalItemPrice,SI16 *pMercenarySlot)
{	
	if(CanBuyPMItem(uiBoothSlot, uiItemQuantity, Money) == FALSE)
	{
		return FALSE;
	}

	return	clPrivateMarket.BuyItem(uiBoothSlot, uiItemQuantity, puiRemainQuantity, pTotalItemPrice,pMercenarySlot);
}