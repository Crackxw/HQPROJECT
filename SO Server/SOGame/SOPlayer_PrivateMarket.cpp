#include "SOMain.h"
#include "SOWorld.h"
#include "SOPlayer.h"
#include "PrivateMarket.h"

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ����.
//	���� ����	:	2002-07-02 ���� 3:54:15 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvPMOpen()
{
	OnReqPMOpenMsg		*pPMOpenMsg;
	OnBoothItemForSell	*pBoothItem;
	SI32				i;
	BOOL				bSuccess	=	FALSE;
	strPMItem			*pstPMItem;

	pPMOpenMsg		=	(OnReqPMOpenMsg*)cMsg;
	pBoothItem		=	(OnBoothItemForSell*)((CHAR*)pPMOpenMsg + sizeof(OnReqPMOpenMsg));

	// ���� ���� ������̰ų� ���� �߿� ������ �ȵȴ�.
	if(GetBattle() != NULL)	
	{
		goto	END;
	}
	
	// ���� ���� ������ �� �������� �˻��Ѵ�.
	if(clPrivateMarket.IsOpen() == TRUE)
	{
		goto	END;
	}

	for(i = 0; i < pPMOpenMsg->uiItemNum; i++)
	{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �� �� �� ���� ( 2003. 3. 14 )
// �뺴 �ŷ� �κ� �߰� ���� ���� �κ�
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		if ( pBoothItem[i].uiTradeFollowerID  && pBoothItem[i].uiItemID  ) 
		{
			writeInfoToFile("Private.txt", "[%d] pBoothItem[%d].uiTradeFollowerID  && pBoothItem[%d].uiTradeFollowerID -> both have value 0\n", i, i, i);
			goto END;
		}

		if ( pBoothItem[i].uiTradeFollowerID )			//�뺴 �ŷ��� ���
		{
			// ��ȿ�� �뺴������ üũ
			if( IFollower.IsValidFollower(pBoothItem[i].uiTradeFollowerID) == FALSE )
			{
				goto	END;
			}

			if( pBoothItem[i].uiTradeFollowerID == 0 )		//���ΰ��� �Ĵ� ���� �� ���� �����. �׳� �������� ��Ȳ�� ��Ÿ�� �Ϳ� ����ؼ�..
			{
				goto	END;
			}

			//�뺴�� ��������� üũ
			if ( this->IsGeneral(pBoothItem[i].uiTradeFollowerID) )
			{
				goto	END;
			}


			if( pBoothItem[i].Price == 0 )
			{
				goto	END;
			}
			
			/*
			//�뺴�� ���������� üũ
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
		else											//������ �ŷ��� ���
		{
			// �ݵ�� ��ȿ�� �뺴�̿��� �Ѵ�.
			if(IFollower.IsValidFollower(pBoothItem[i].uiFollowerID) == FALSE)
			{
				goto	END;
			}

			// ���� �뺴���� ���� �������� �ִ��� �˻��Ѵ�.		
			if(IFollower.GetItemByID(pBoothItem[i].uiFollowerID, pBoothItem[i].uiItemID) == NULL)
			{
				goto	END;		// �������� ���´�.
			}

			// �������� �߰��Ѵ�.		
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
// �� �� �� ���� ( 2003. 3. 14 )
// �뺴 �ŷ� �κ� �߰� �� �κ�
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}	


	// ������ ����.
	clPrivateMarket.Open();
	
	// Ÿ �������� ��Ƽ ��û���̰ų� 
	// �ٸ� ������ ��Ƽ ��û�� �޾Ҵٸ� �̸� ����Ѵ�.
	CloseRequestingJoinParty();

	bSuccess					=	TRUE;

END:
	if(bSuccess == TRUE)
	{	
		// �켱 ���� �� �������� ������ ����� �޽����� �����ش�.
		SendFieldMsg(ON_RESPONSE_PM_OPEN, SO_SFM_MEANDNEARCHAR, LPVOID(ON_RET_OK));	

		// ������ �÷��̾�Դ� �������� �߰��϶�� �˷��ش�.
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
		// ���������� �۾��߿� �� �����Ⱚ���� ����� ���Ͽ� ������ �ٽ� �ʱ�ȭ �Ѵ�.
		clPrivateMarket.Init();

		// �����ߴٰ� �޽����� �����ش�.
		SendFieldMsg(ON_RESPONSE_PM_OPEN, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �ݴ´�.
//	���� ����	:	2002-06-25 ���� 4:30:14 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvPMClose()
{
	OnReqPMCloseMsg		*pPMCloseMsg;
	BOOL				bSuccess	=	FALSE;

	pPMCloseMsg		=	(OnReqPMCloseMsg*)cMsg;

	// ���� ���� ������ �� �����϶����� ���� �� �ִ�.
	if(clPrivateMarket.IsOpen() == FALSE)	
	{
		goto	END;		
	}
	
	// ������ �ݴ´�.
	
	clPrivateMarket.Close();

	bSuccess	=	TRUE;
END:
	if(bSuccess == TRUE)
	{
		// ������ ���� �� �ִ�.
		SendFieldMsg(ON_RESPONSE_PM_CLOSE, SO_SFM_MEANDNEARCHAR, LPVOID(ON_RET_OK));
	}
	else
	{
		SendFieldMsg(ON_RESPONSE_PM_CLOSE, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�����󿡰� �뺴 �� �������� ���.
//	���� ����	:	2002-06-25 ���� 4:59:40 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
// �� �� �� ���� ( 2003. 3. 14 )
// �뺴 �ŷ� �κ� �߰� ���� �κ�
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// ���� �������� ����� �ϴ� ĳ���Ͱ� ��ȿ���� �˾ƿ´�.
	if( ( pPlayer = pMyWorld->GetPlayer( pPMBuyItemMsg->uiAccount ) ) == NULL)
	{
		goto	END;
	}

	// slot�� ��ȣ�� �������� ����� �ϴ� �������̳� �뺴�� ������ ���´�.
	if( ( pstPMItem = pPlayer->clPrivateMarket.GetItem( pPMBuyItemMsg->uiSlotID ) ) == NULL )
	{
		goto	END;
	}

	if(clPrivateMarket.IsOpen() == TRUE)
	{
		goto	END;
	}

	stTempPMItem	=	*pstPMItem;

	// �ش� ǰ��(�뺴 �� ������)�� �����Ͽ��� �� ���� �޴� �÷��̾ ������ �� �ִ� �ִ� ���� �Ѵ��� �˻��Ѵ�.
	// ���࿡ ������ �� �ִ� �ӱ��� �Ѵ´ٸ� ǰ���� ���� ���Ѵ�.
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
			//Trade �ҷ��� �뺴�� ��ȿ����..
			if( ! pPlayer->IFollower.IsValidFollower( pstPMItem->uiTradeFollowerSlotID ) )
			{
				goto	END;
			}

			// ���� �ִ� ���� �� �ִ� �뺴���� �ʰ����� �ʴ��� �˾ƿ´�.
			if(uiFollowerNum >= GetMaxFollowerNum() )
			{
				goto	END;
			}

			//���ΰ��� �����ϴ� ���� �� ���� ����������, ���� ó���� �� �־���.
			if ( pstPMItem->uiTradeFollowerSlotID == 0)
			{
				goto	END;
			}

			//��� �ŷ� �� �� ����
			if( pPlayer->IsGeneral( pstPMItem->uiTradeFollowerSlotID ) )
			{
				goto	END;
			}


			// ���ʹ� 3 ���� �̻� ���� ���� ����.
			// ���� ������ �ִ� ���Ͱ� 2�����̰�, ���͸� �����Ϸ��� �Ͽ�����
			// �����ϵ��� �Ѵ�.
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

			// �뺴�� ������ ���´�.
			siNation	=	pPlayer->IFollower.GetFollowerParameterNation(pstPMItem->uiTradeFollowerSlotID);

			// �뺴�� ������ �����µ� �����ؾ��Ѵ�.
			if( (dwFollowerDataSize = pPlayer->GetFollowerInfo(pstPMItem->uiTradeFollowerSlotID, pFollowerInfo)) == 0 )
			{
				goto	END;
			}

			// ���� �뺴�� ����. 
			if(pPlayer->IFollower.DeleteFollower(pstPMItem->uiTradeFollowerSlotID) == FALSE)	
			{
				goto	END;
			}

			// �뺴�� �޴� �÷��̾�(�� �ڽ� = this)�� �뺴�� ��� �߰����Ѿ� �ϴ��� �˾ƿ´�.
			if((siEmptyFollowerSlot = IFollower.FindEmptyFollowerSlot()) == -1)
			{
				goto	END;
			}

			// ������ ������ �뺴�� �߰������ش�.
			if(IFollower.InitFollower(pFollowerInfo, siNation, &pMyWorld->clItem) == TRUE)	
			{
				//*puiOppFollowerID				=	siEmptyFollowerSlot;

				// pPlayer: ������ ���� �ִ� �÷��̾�
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

			// ���������� �뺴�� �޾Ҵ�.

			//pFollowerInfo	=	(OnFollowerInfo*)((CHAR*)pFollowerInfo + dwFollowerDataSize);

			toMoney = DecreaseMoneyReturnMoney(pstPMItem->Price);
			// db������ ���� ����� �ٲ۴�.
			toMoney = -toMoney;
			fromMoney = pPlayer->IncreaseMoney(pstPMItem->Price,FALSE);


			//siDBGoodKind	=	2;
			//siDBGoodParam1	=	pstPMItem->uiTradeFollowerSlotID;
			//siDBGoodParam2	=	siEmptyFollowerSlot;
			

			break;
		}
	case LIST_ITEM:
		{
			// �������� �缭 �κ��丮�� �־�� �ϴµ�
			// ���� �κ��丮�� �������� �߰� ��ų �� �ִ��� �˾ƿ´�.
			if(IFollower.CanAddItem(pPMBuyItemMsg->uiFollowerID, stTempPMItem.uiItemID, pPMBuyItemMsg->uiQuantity, pPMBuyItemMsg->uiInvenPos) == FALSE)
			{
				goto	END;
			}
				

			// ���� �������� �������� �����Ѵ�.
			if( pPlayer->BuyPMItem( pPMBuyItemMsg->uiSlotID, pPMBuyItemMsg->uiQuantity, siMoney, 
									&uiRemainQuantity, &TotalItemPrice,&siFromMercenarySlot ) == FALSE )
			{
				goto	END;			
			}

			// �������� �����ش�.
			if((pMyItem = AddItem(pPMBuyItemMsg->uiFollowerID, stTempPMItem.uiItemID, pPMBuyItemMsg->uiQuantity, pPMBuyItemMsg->uiInvenPos,FALSE)) == NULL)
			{
//				pMyWorld->clServer->WriteInfo( "BuyItemFromPMFailed.txt", "[ Buyer : %s, Seller : %s, ItemID : %u, ItemQuantity : %u, Price : %I64d ]", szID, pPlayer->szID, 
				writeInfoToFile( "BuyItemFromPMFailed.txt", "[ Buyer : %s, Seller : %s, ItemID : %u, ItemQuantity : %u, Price : %I64d ]", szID, pPlayer->szID, 
													stTempPMItem.uiItemID, pPMBuyItemMsg->uiQuantity, TotalItemPrice );
				goto	END;		
			}
			// �����Ϸ��� �÷��̾��� ���� ���� �Ǹ��� �÷��̾��� ���� ���� ��Ų��.
			toMoney = DecreaseMoneyReturnMoney(TotalItemPrice);
			// db������ ���� ����� �ٲ۴�.
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
					// Ȥ�� �������� �� �缭 �������� �������ٸ� ���� �÷��̾�Ե� �˷��ش�.
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
				pPlayer->SendFieldMsg(ON_RESPONSE_PM_BUYITEM, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(pPMBuyItemMsg->uiSlotID), LPVOID(0));		//�뺴�� �Ȱ� ���� ���� ������ 0�� �ǹǷ�..

				//�뺴�� �����ؼ� �����̳� ���� ������ �뺴�� �������ٸ� ���� �÷��̾�Ե� �˷��ش�.
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
// �� �� �� ���� ( 2003. 3. 14 )
// �뺴 �ŷ� �κ� �߰� �� �κ�
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	


	
	//stTempPMItem	=	*pstPMItem;


	/*	
	// ���� �������� ����� �ϴ� ĳ���Ͱ� ��ȿ���� �˾ƿ´�.
	if( ( pPlayer = pMyWorld->GetPlayer( pPMBuyItemMsg->uiAccount ) ) == NULL)
	{
		goto	END;
	}
	
	// ����� �ϴ� �������� ������ ���´�.
	if( ( pstPMItem = pPlayer->clPrivateMarket.GetItem( pPMBuyItemMsg->uiSlotID ) ) == NULL )
	{
		goto	END;
	}

	stTempPMItem	=	*pstPMItem;

	// �������� �缭 �κ��丮�� �־�� �ϴµ�
	// ���� �κ��丮�� �������� �߰� ��ų �� �ִ��� �˾ƿ´�.
	if(IFollower.CanAddItem(pPMBuyItemMsg->uiFollowerID, stTempPMItem.uiItemID, pPMBuyItemMsg->uiQuantity, pPMBuyItemMsg->uiInvenPos) == FALSE)		goto	END;
	
	// ���� �������� �������� �����Ѵ�.
	if(pPlayer->BuyPMItem(pPMBuyItemMsg->uiSlotID, pPMBuyItemMsg->uiQuantity, siMoney, &uiRemainQuantity, &TotalItemPrice,&siFromMercenarySlot) == FALSE)			goto	END;			

	// �������� �����ش�.
	if((pMyItem = AddItem(pPMBuyItemMsg->uiFollowerID, stTempPMItem.uiItemID, pPMBuyItemMsg->uiQuantity, pPMBuyItemMsg->uiInvenPos,FALSE)) == NULL)
	{
		pMyWorld->clServer->WriteInfo( "BuyItemFromPMFailed.txt", "[ Buyer : %s, Seller : %s, ItemID : %u, ItemQuantity : %u, Price : %I64d ]", szID, pPlayer->szID, 
											stTempPMItem.uiItemID, pPMBuyItemMsg->uiQuantity, TotalItemPrice );
		goto	END;		
	}

	// �����Ϸ��� �÷��̾��� ���� ���� �Ǹ��� �÷��̾��� ���� ���� ��Ų��.
	toMoney = DecreaseMoneyReturnMoney(TotalItemPrice);
	// db������ ���� ����� �ٲ۴�.
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
			// Ȥ�� �������� �� �缭 �������� �������ٸ� ���� �÷��̾�Ե� �˷��ش�.
			pPlayer->SendFieldMsg(ON_PM_CHANGE_BOOTH_ITEM, SO_SFM_ONLYNEARCHAR, LPVOID(pPMBuyItemMsg->uiAccount), LPVOID(pPMBuyItemMsg->uiSlotID), LPVOID(0));		
		}		
	}
	else
	{
		SendFieldMsg(ON_RESPONSE_PM_BUYITEM, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}
*/


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ������ �ִ� �������� ������ �䱸�Ѵ�.
//	���� ����	:	2002-06-25 ���� 4:30:43 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvPMReqItemInfo()
{
	OnReqPMItemInfoMsg		*pPMItemInfo;
	BOOL					bSuccess = FALSE;
	SOPlayer				*pPlayer;	
	
	pPMItemInfo	=	(OnReqPMItemInfoMsg*)cMsg;

	if( IsTrading() )		goto END;

	// ���� �������� ����� �ϴ� ĳ���Ͱ� ���� ������ �������� �˾ƿ´�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ������ �ִ� �������� �����Ѵ�.
//	���� ����	:	2002-07-02 ���� 7:02:03 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

	// ���� ���� ������ �� �������� �˻��Ѵ�.
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
			// �ݵ�� ��ȿ�� �뺴�̿��� �Ѵ�.
			if( IFollower.GetFollower( pBoothItem[i].uiTradeFollowerID ) == NULL )
			{
				goto	END;
			}

			//��� üũ �� ���� üũ �� ĳ���� üũ�� ��(������ �� �����Ƿ�..)
			if( pBoothItem[i].uiTradeFollowerID == 0 )		//���ΰ��� �Ĵ� ���� �� ���� �����. �׳� �������� ��Ȳ�� ��Ÿ�� �Ϳ� ����ؼ�..
			{
				goto	END;
			}

			//�뺴�� ��������� üũ
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
				// �ش� ������ �������� �����Ѵ�.
				clPrivateMarket.DelItem(pBoothItem[i].uiBoothSlot);
				
				// ������ �÷��̾�鿡�� �˷��ش�.
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
			// �ݵ�� ��ȿ�� �뺴�̿��� �Ѵ�.
			if( IFollower.GetFollower(pBoothItem[i].uiFollowerID) == NULL )
			{
				goto	END;
			}
			
			if(pBoothItem[i].uiItemID == 0)
			{
				// �ش� ������ �������� �����Ѵ�.
				clPrivateMarket.DelItem(pBoothItem[i].uiBoothSlot);
				
				// ������ �÷��̾�鿡�� �˷��ش�.
				SendFieldMsg(ON_PM_CHANGE_BOOTH_ITEM, SO_SFM_ONLYNEARCHAR, LPVOID(uiAccount), LPVOID(pBoothItem[i].uiBoothSlot), LPVOID(0), LPVOID(type));
			}
			else
			{
				// �ش� ������ �������� �����Ѵ�.
				// ���� �뺴���� ���� �������� �ִ��� �˻��Ѵ�.			
				if(IFollower.GetItemByID(pBoothItem[i].uiFollowerID, pBoothItem[i].uiItemID) == NULL)	goto	END;		// �������� ���´�.
				if(clPrivateMarket.ChangeItem(pBoothItem[i].uiBoothSlot, pBoothItem[i].uiFollowerID, pBoothItem[i].uiItemID, pBoothItem[i].Price, pBoothItem[i].Desc, 0 ) == FALSE)
					goto	END;
				
				// ������ �÷��̾�鿡�� �˷��ش�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �������� �������� �� �� �ִ°�?	
//	���� ����	:	2002-07-05 ���� 9:19:06 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::CanBuyPMItem(UI08 uiBoothSlot, UI16 uiItemQuantity, MONEY Money)
{
	return	clPrivateMarket.CanBuyItem(uiBoothSlot, uiItemQuantity, Money);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �������� �������� ���.
//	���� ����	:	2002-07-05 ���� 9:32:12 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::BuyPMItem(UI08 uiBoothSlot, UI16 uiItemQuantity, MONEY Money, UI16 *puiRemainQuantity, MONEY *pTotalItemPrice,SI16 *pMercenarySlot)
{	
	if(CanBuyPMItem(uiBoothSlot, uiItemQuantity, Money) == FALSE)
	{
		return FALSE;
	}

	return	clPrivateMarket.BuyItem(uiBoothSlot, uiItemQuantity, puiRemainQuantity, pTotalItemPrice,pMercenarySlot);
}