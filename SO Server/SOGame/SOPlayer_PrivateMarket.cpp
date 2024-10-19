#include "SOMain.h"
#include "SOWorld.h"
#include "SOPlayer.h"
#include "PrivateMarket.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	노점상을 연다.
//	수정 일자	:	2002-07-02 오후 3:54:15 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvPMOpen()
{
	OnReqPMOpenMsg		*pPMOpenMsg;
	OnBoothItemForSell	*pBoothItem;
	SI32				i;
	BOOL				bSuccess	=	FALSE;
	strPMItem			*pstPMItem;

	pPMOpenMsg		=	(OnReqPMOpenMsg*)cMsg;
	pBoothItem		=	(OnBoothItemForSell*)((CHAR*)pPMOpenMsg + sizeof(OnReqPMOpenMsg));

	// 현재 전투 대기중이거나 전투 중에 있으면 안된다.
	if(GetBattle() != NULL)	
	{
		goto	END;
	}
	
	// 현재 개인 상점을 연 상태인지 검사한다.
	if(clPrivateMarket.IsOpen() == TRUE)
	{
		goto	END;
	}

	for(i = 0; i < pPMOpenMsg->uiItemNum; i++)
	{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 이 상 민 수정 ( 2003. 3. 14 )
// 용병 거래 부분 추가 수정 시작 부분
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		if ( pBoothItem[i].uiTradeFollowerID  && pBoothItem[i].uiItemID  ) 
		{
			writeInfoToFile("Private.txt", "[%d] pBoothItem[%d].uiTradeFollowerID  && pBoothItem[%d].uiTradeFollowerID -> both have value 0\n", i, i, i);
			goto END;
		}

		if ( pBoothItem[i].uiTradeFollowerID )			//용병 거래인 경우
		{
			// 유효한 용병인지를 체크
			if( IFollower.IsValidFollower(pBoothItem[i].uiTradeFollowerID) == FALSE )
			{
				goto	END;
			}

			if( pBoothItem[i].uiTradeFollowerID == 0 )		//주인공을 파는 있을 수 없는 경우임. 그냥 예외적인 상황이 나타날 것에 대비해서..
			{
				goto	END;
			}

			//용병이 장수인지를 체크
			if ( this->IsGeneral(pBoothItem[i].uiTradeFollowerID) )
			{
				goto	END;
			}


			if( pBoothItem[i].Price == 0 )
			{
				goto	END;
			}
			
			/*
			//용병이 몬스터인지를 체크
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
		else											//아이템 거래인 경우
		{
			// 반드시 유효한 용병이여야 한다.
			if(IFollower.IsValidFollower(pBoothItem[i].uiFollowerID) == FALSE)
			{
				goto	END;
			}

			// 실제 용병에게 같은 아이템이 있는지 검사한다.		
			if(IFollower.GetItemByID(pBoothItem[i].uiFollowerID, pBoothItem[i].uiItemID) == NULL)
			{
				goto	END;		// 아이템을 얻어온다.
			}

			// 아이템을 추가한다.		
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
// 이 상 민 수정 ( 2003. 3. 14 )
// 용병 거래 부분 추가 끝 부분
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}	


	// 상점을 연다.
	clPrivateMarket.Open();
	
	// 타 유저에게 파티 신청중이거나 
	// 다른 유저에 파티 신청을 받았다면 이를 취소한다.
	CloseRequestingJoinParty();

	bSuccess					=	TRUE;

END:
	if(bSuccess == TRUE)
	{	
		// 우선 나와 내 주위에게 좌판을 열라는 메시지를 보내준다.
		SendFieldMsg(ON_RESPONSE_PM_OPEN, SO_SFM_MEANDNEARCHAR, LPVOID(ON_RET_OK));	

		// 주위의 플레이어에게는 아이템을 추가하라고도 알려준다.
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
		// 실패했을시 작업중에 들어간 쓰레기값들을 지우기 위하여 좌판을 다시 초기화 한다.
		clPrivateMarket.Init();

		// 실패했다고 메시지를 보내준다.
		SendFieldMsg(ON_RESPONSE_PM_OPEN, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	노점상을 닫는다.
//	수정 일자	:	2002-06-25 오후 4:30:14 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvPMClose()
{
	OnReqPMCloseMsg		*pPMCloseMsg;
	BOOL				bSuccess	=	FALSE;

	pPMCloseMsg		=	(OnReqPMCloseMsg*)cMsg;

	// 현재 개인 상점을 연 상태일때에만 받을 수 있다.
	if(clPrivateMarket.IsOpen() == FALSE)	
	{
		goto	END;		
	}
	
	// 상점을 닫는다.
	
	clPrivateMarket.Close();

	bSuccess	=	TRUE;
END:
	if(bSuccess == TRUE)
	{
		// 상점을 닫을 수 있다.
		SendFieldMsg(ON_RESPONSE_PM_CLOSE, SO_SFM_MEANDNEARCHAR, LPVOID(ON_RET_OK));
	}
	else
	{
		SendFieldMsg(ON_RESPONSE_PM_CLOSE, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	노점상에게 용병 및 아이템을 산다.
//	수정 일자	:	2002-06-25 오후 4:59:40 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
// 이 상 민 수정 ( 2003. 3. 14 )
// 용병 거래 부분 추가 시작 부분
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// 현재 아이템을 사려고 하는 캐릭터가 유효한지 알아온다.
	if( ( pPlayer = pMyWorld->GetPlayer( pPMBuyItemMsg->uiAccount ) ) == NULL)
	{
		goto	END;
	}

	// slot의 번호를 기준으로 사려고 하는 아이템이나 용병의 정보를 얻어온다.
	if( ( pstPMItem = pPlayer->clPrivateMarket.GetItem( pPMBuyItemMsg->uiSlotID ) ) == NULL )
	{
		goto	END;
	}

	if(clPrivateMarket.IsOpen() == TRUE)
	{
		goto	END;
	}

	stTempPMItem	=	*pstPMItem;

	// 해당 품목(용병 및 아이템)을 구입하였을 시 돈을 받는 플레이어가 소유할 수 있는 최대 돈을 넘는지 검사한다.
	// 만약에 소유할 수 최대 임금을 넘는다면 품목을 팔지 못한다.
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
			//Trade 할려는 용병이 유효한지..
			if( ! pPlayer->IFollower.IsValidFollower( pstPMItem->uiTradeFollowerSlotID ) )
			{
				goto	END;
			}

			// 내가 최대 가질 수 있는 용병보다 초과하지 않는지 알아온다.
			if(uiFollowerNum >= GetMaxFollowerNum() )
			{
				goto	END;
			}

			//주인공을 구입하는 있을 수 없는 에러이지만, 예외 처리로 해 주었음.
			if ( pstPMItem->uiTradeFollowerSlotID == 0)
			{
				goto	END;
			}

			//장수 거래 할 수 없음
			if( pPlayer->IsGeneral( pstPMItem->uiTradeFollowerSlotID ) )
			{
				goto	END;
			}


			// 몬스터는 3 마리 이상 가질 수가 없다.
			// 현재 가지고 있는 몬스터가 2마리이고, 몬스터를 구입하려고 하였을시
			// 실패하도록 한다.
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

			// 용병의 국가를 얻어온다.
			siNation	=	pPlayer->IFollower.GetFollowerParameterNation(pstPMItem->uiTradeFollowerSlotID);

			// 용병의 정보를 얻어오는데 성공해야한다.
			if( (dwFollowerDataSize = pPlayer->GetFollowerInfo(pstPMItem->uiTradeFollowerSlotID, pFollowerInfo)) == 0 )
			{
				goto	END;
			}

			// 상대방 용병은 삭제. 
			if(pPlayer->IFollower.DeleteFollower(pstPMItem->uiTradeFollowerSlotID) == FALSE)	
			{
				goto	END;
			}

			// 용병을 받는 플레이어(나 자신 = this)가 용병을 어디에 추가시켜야 하는지 알아온다.
			if((siEmptyFollowerSlot = IFollower.FindEmptyFollowerSlot()) == -1)
			{
				goto	END;
			}

			// 나에게 상대방의 용병을 추가시켜준다.
			if(IFollower.InitFollower(pFollowerInfo, siNation, &pMyWorld->clItem) == TRUE)	
			{
				//*puiOppFollowerID				=	siEmptyFollowerSlot;

				// pPlayer: 좌판을 열고 있는 플레이어
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

			// 성공적으로 용병을 받았다.

			//pFollowerInfo	=	(OnFollowerInfo*)((CHAR*)pFollowerInfo + dwFollowerDataSize);

			toMoney = DecreaseMoneyReturnMoney(pstPMItem->Price);
			// db쿼리를 위해 양수로 바꾼다.
			toMoney = -toMoney;
			fromMoney = pPlayer->IncreaseMoney(pstPMItem->Price,FALSE);


			//siDBGoodKind	=	2;
			//siDBGoodParam1	=	pstPMItem->uiTradeFollowerSlotID;
			//siDBGoodParam2	=	siEmptyFollowerSlot;
			

			break;
		}
	case LIST_ITEM:
		{
			// 아이템을 사서 인벤토리에 넣어야 하는데
			// 현재 인벤토리에 아이템을 추가 시킬 수 있는지 알아온다.
			if(IFollower.CanAddItem(pPMBuyItemMsg->uiFollowerID, stTempPMItem.uiItemID, pPMBuyItemMsg->uiQuantity, pPMBuyItemMsg->uiInvenPos) == FALSE)
			{
				goto	END;
			}
				

			// 개인 상점에서 아이템을 구입한다.
			if( pPlayer->BuyPMItem( pPMBuyItemMsg->uiSlotID, pPMBuyItemMsg->uiQuantity, siMoney, 
									&uiRemainQuantity, &TotalItemPrice,&siFromMercenarySlot ) == FALSE )
			{
				goto	END;			
			}

			// 아이템을 더해준다.
			if((pMyItem = AddItem(pPMBuyItemMsg->uiFollowerID, stTempPMItem.uiItemID, pPMBuyItemMsg->uiQuantity, pPMBuyItemMsg->uiInvenPos,FALSE)) == NULL)
			{
//				pMyWorld->clServer->WriteInfo( "BuyItemFromPMFailed.txt", "[ Buyer : %s, Seller : %s, ItemID : %u, ItemQuantity : %u, Price : %I64d ]", szID, pPlayer->szID, 
				writeInfoToFile( "BuyItemFromPMFailed.txt", "[ Buyer : %s, Seller : %s, ItemID : %u, ItemQuantity : %u, Price : %I64d ]", szID, pPlayer->szID, 
													stTempPMItem.uiItemID, pPMBuyItemMsg->uiQuantity, TotalItemPrice );
				goto	END;		
			}
			// 구입하려는 플레이어의 돈을 빼고 판매한 플레이어의 돈을 증가 시킨다.
			toMoney = DecreaseMoneyReturnMoney(TotalItemPrice);
			// db쿼리를 위해 양수로 바꾼다.
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
					// 혹시 아이템을 다 사서 아이템이 없어졌다면 주위 플레이어에게도 알려준다.
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
				pPlayer->SendFieldMsg(ON_RESPONSE_PM_BUYITEM, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(pPMBuyItemMsg->uiSlotID), LPVOID(0));		//용병은 팔고 나면 남은 갯수가 0이 되므로..

				//용병을 구입해서 좌판이나 개인 상점에 용병이 없어졌다면 주위 플레이어에게도 알려준다.
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
// 이 상 민 수정 ( 2003. 3. 14 )
// 용병 거래 부분 추가 끝 부분
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	


	
	//stTempPMItem	=	*pstPMItem;


	/*	
	// 현재 아이템을 사려고 하는 캐릭터가 유효한지 알아온다.
	if( ( pPlayer = pMyWorld->GetPlayer( pPMBuyItemMsg->uiAccount ) ) == NULL)
	{
		goto	END;
	}
	
	// 사려고 하는 아이템의 정보를 얻어온다.
	if( ( pstPMItem = pPlayer->clPrivateMarket.GetItem( pPMBuyItemMsg->uiSlotID ) ) == NULL )
	{
		goto	END;
	}

	stTempPMItem	=	*pstPMItem;

	// 아이템을 사서 인벤토리에 넣어야 하는데
	// 현재 인벤토리에 아이템을 추가 시킬 수 있는지 알아온다.
	if(IFollower.CanAddItem(pPMBuyItemMsg->uiFollowerID, stTempPMItem.uiItemID, pPMBuyItemMsg->uiQuantity, pPMBuyItemMsg->uiInvenPos) == FALSE)		goto	END;
	
	// 개인 상점에서 아이템을 구입한다.
	if(pPlayer->BuyPMItem(pPMBuyItemMsg->uiSlotID, pPMBuyItemMsg->uiQuantity, siMoney, &uiRemainQuantity, &TotalItemPrice,&siFromMercenarySlot) == FALSE)			goto	END;			

	// 아이템을 더해준다.
	if((pMyItem = AddItem(pPMBuyItemMsg->uiFollowerID, stTempPMItem.uiItemID, pPMBuyItemMsg->uiQuantity, pPMBuyItemMsg->uiInvenPos,FALSE)) == NULL)
	{
		pMyWorld->clServer->WriteInfo( "BuyItemFromPMFailed.txt", "[ Buyer : %s, Seller : %s, ItemID : %u, ItemQuantity : %u, Price : %I64d ]", szID, pPlayer->szID, 
											stTempPMItem.uiItemID, pPMBuyItemMsg->uiQuantity, TotalItemPrice );
		goto	END;		
	}

	// 구입하려는 플레이어의 돈을 빼고 판매한 플레이어의 돈을 증가 시킨다.
	toMoney = DecreaseMoneyReturnMoney(TotalItemPrice);
	// db쿼리를 위해 양수로 바꾼다.
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
			// 혹시 아이템을 다 사서 아이템이 없어졌다면 주위 플레이어에게도 알려준다.
			pPlayer->SendFieldMsg(ON_PM_CHANGE_BOOTH_ITEM, SO_SFM_ONLYNEARCHAR, LPVOID(pPMBuyItemMsg->uiAccount), LPVOID(pPMBuyItemMsg->uiSlotID), LPVOID(0));		
		}		
	}
	else
	{
		SendFieldMsg(ON_RESPONSE_PM_BUYITEM, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}
*/


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	노점상이 가지고 있는 아이템의 정보를 요구한다.
//	수정 일자	:	2002-06-25 오후 4:30:43 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvPMReqItemInfo()
{
	OnReqPMItemInfoMsg		*pPMItemInfo;
	BOOL					bSuccess = FALSE;
	SOPlayer				*pPlayer;	
	
	pPMItemInfo	=	(OnReqPMItemInfoMsg*)cMsg;

	if( IsTrading() )		goto END;

	// 현재 아이템을 사려고 하는 캐릭터가 개인 상점을 열었는지 알아온다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	노점상이 가지고 있는 아이템을 변경한다.
//	수정 일자	:	2002-07-02 오후 7:02:03 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

	// 현재 개인 상점을 연 상태인지 검사한다.
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
			// 반드시 유효한 용병이여야 한다.
			if( IFollower.GetFollower( pBoothItem[i].uiTradeFollowerID ) == NULL )
			{
				goto	END;
			}

			//장수 체크 및 몬스터 체크 주 캐릭터 체크는 뺌(위에서 해 줬으므로..)
			if( pBoothItem[i].uiTradeFollowerID == 0 )		//주인공을 파는 있을 수 없는 경우임. 그냥 예외적인 상황이 나타날 것에 대비해서..
			{
				goto	END;
			}

			//용병이 장수인지를 체크
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
				// 해당 좌판의 아이템을 삭제한다.
				clPrivateMarket.DelItem(pBoothItem[i].uiBoothSlot);
				
				// 주위의 플레이어들에게 알려준다.
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
			// 반드시 유효한 용병이여야 한다.
			if( IFollower.GetFollower(pBoothItem[i].uiFollowerID) == NULL )
			{
				goto	END;
			}
			
			if(pBoothItem[i].uiItemID == 0)
			{
				// 해당 좌판의 아이템을 삭제한다.
				clPrivateMarket.DelItem(pBoothItem[i].uiBoothSlot);
				
				// 주위의 플레이어들에게 알려준다.
				SendFieldMsg(ON_PM_CHANGE_BOOTH_ITEM, SO_SFM_ONLYNEARCHAR, LPVOID(uiAccount), LPVOID(pBoothItem[i].uiBoothSlot), LPVOID(0), LPVOID(type));
			}
			else
			{
				// 해당 좌판의 아이템을 변경한다.
				// 실제 용병에게 같은 아이템이 있는지 검사한다.			
				if(IFollower.GetItemByID(pBoothItem[i].uiFollowerID, pBoothItem[i].uiItemID) == NULL)	goto	END;		// 아이템을 얻어온다.
				if(clPrivateMarket.ChangeItem(pBoothItem[i].uiBoothSlot, pBoothItem[i].uiFollowerID, pBoothItem[i].uiItemID, pBoothItem[i].Price, pBoothItem[i].Desc, 0 ) == FALSE)
					goto	END;
				
				// 주위의 플레이어들에게 알려준다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	개인 상점에서 아이템을 살 수 있는가?	
//	수정 일자	:	2002-07-05 오후 9:19:06 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::CanBuyPMItem(UI08 uiBoothSlot, UI16 uiItemQuantity, MONEY Money)
{
	return	clPrivateMarket.CanBuyItem(uiBoothSlot, uiItemQuantity, Money);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	개인 상점에서 아이템을 산다.
//	수정 일자	:	2002-07-05 오후 9:32:12 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::BuyPMItem(UI08 uiBoothSlot, UI16 uiItemQuantity, MONEY Money, UI16 *puiRemainQuantity, MONEY *pTotalItemPrice,SI16 *pMercenarySlot)
{	
	if(CanBuyPMItem(uiBoothSlot, uiItemQuantity, Money) == FALSE)
	{
		return FALSE;
	}

	return	clPrivateMarket.BuyItem(uiBoothSlot, uiItemQuantity, puiRemainQuantity, pTotalItemPrice,pMercenarySlot);
}