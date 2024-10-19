#include "SOMain.h"
#include "SODatabase.h"
#include "SOVillage_MarketStructure.h"

CItem			*SOVillage_Market::m_pItem;
cltIProfit		*SOVillage_Market::m_pIProfit;
SODatabase		*SOVillage_Market::m_pDataBase;

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ �ʱ�ȭ �Ѵ�.
//	���� ����	:	2002-10-01 ���� 2:01:42 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage_Market::InitStaticVariable( CItem *pItem, cltIProfit *pIProfit, SODatabase *pDataBase )
{
	m_pItem			=	pItem;
	m_pIProfit		=	pIProfit;
	m_pDataBase		=	pDataBase;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������.
//	���� ����	:	2002-09-30 ���� 9:14:38 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOVillage_Market::SOVillage_Market()
{
	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�Ҹ���.
//	���� ����	:	2002-09-30 ���� 9:14:52 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOVillage_Market::~SOVillage_Market()
{

}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ʱ�ȭ.
//	���� ����	:	2002-10-01 ���� 9:55:26 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_Market::Init( UI16 uiVillageCode, UI32 uiItemsID[ ON_VILLAGE_MAX_ITEM ], SI32 siItemsAtb[ ON_VILLAGE_MAX_ITEM ], UI32 uiItemsQuantity[ ON_VILLAGE_MAX_ITEM ] )
{
	SI32				i;
	SI32				siItemInfo[ ON_VILLAGE_MAX_ITEM * 2 ];
//	DBIKVillageItem		VillageItem;
//	UI16				uiItemID;
//	UI16				uiItemQuantity;

	// ������ �� �� �ִ� �÷��̾��� ���� SOVILLAGE_MAX_MAKRET_PLAYER_NUM�� �����Ѵ�.
	SOVillage_BaseStructure::Init( uiVillageCode, SOVILLAGE_MAX_MAKRET_PLAYER_NUM );

	// �ʱ�ȭ�� �Ѵ�.
	ZeroMemory( m_SellItem, sizeof( m_SellItem ) );
	ZeroMemory( siItemInfo, sizeof( siItemInfo ) );

	// �������� ����ϴ� �������� �ʱ�ȭ �Ѵ�.
	for( i = 0; i < ON_VILLAGE_MAX_ITEM; i++ )
	{	
		// ȭ�� ���������� ID�� �Ӽ�, ��������(������ �ʴ°�)�� �־��ָ� �ȴ�, ������ DB���� ���;� �Ѵ�.
		m_SellItem[ i ].uiItemID		=	UI16( uiItemsID[ i ] );
		m_SellItem[ i ].uiItemAtb		=	UI08( siItemsAtb[ i ] );				
	}	

	// DB���� �������� ������ ���� ���� �Ѵ� 	
	if( m_pDataBase->GetDBInfo( SO_DB_INFOKIND_VILLAGEMARKETSELLITEM, LPVOID( &uiVillageCode ), LPVOID( siItemInfo ) ) == FALSE ) return FALSE;	
	for( i = 0; i < ON_VILLAGE_MAX_ITEM; i++ )	
	{
		if(uiItemsID[i] != 0)
		{
			if( siItemInfo[ 0 ] == 0 )
			{
				DBIKVillageItem	TempVillageItemInfo;
				
				TempVillageItemInfo.ItemID			=	uiItemsID[i];
				TempVillageItemInfo.uiItemQuantity		=	uiItemsQuantity[i];
				TempVillageItemInfo.VillageCode	=	uiVillageCode;
				TempVillageItemInfo.Slot			=	i;

				m_pDataBase->writeDBQuery( SO_DB_INFOKIND_VILLAGE_ITEM, &TempVillageItemInfo, sizeof( DBIKVillageItem ) );
			}
		}
		if( siItemInfo[ i * 2 ] != 0 )
			SetItemQuantity( UI16( siItemInfo[ i * 2 ] ), UI16( siItemInfo[ i * 2 + 1 ] ));
	}

	/*
		if( siItemInfo[ i * 2 ] == 0 )	continue;
			
		if( ( SetItemQuantity( UI16( siItemInfo[ i * 2 ] ), UI16( siItemInfo[ i * 2 + 1 ] ) ) ) == FALSE )			
		{
			// DB���� ������ ������ �ش� �������� �������� �ʴ´ٸ� DB�� �����.
			VillageItem.VillageCode			=	uiVillageCode;
			VillageItem.Slot				=	i;
			VillageItem.uiItemQuantity		=	0;
			VillageItem.ItemID				=	0;

			m_pDataBase->writeDBQuery( SO_DB_INFOKIND_SETVILLAGEITEM, &VillageItem, sizeof( VillageItem ) );
		}
	}*/

	/*
	// �ؽ�Ʈ���� ������ DB�� ���°͵��� ���� �Ѵ� 	
	for(i = 0; i < ON_VILLAGE_MAX_ITEM; i++)	
	{				
		if( GetItemID( i, &uiItemID ) == FALSE )							continue;		
		if( GetItemQuantity( uiItemID, &uiItemQuantity ) == FALSE )		continue;
		if( uiItemQuantity != 0 )													continue;

		VillageItem.VillageCode		=	uiVillageCode;
		VillageItem.Slot			=	i;
		VillageItem.ItemID			=	uiItemID;
		VillageItem.uiItemQuantity	=	UI16( uiItemsQuantity[ i ] );						
		m_pDataBase->writeDBQuery( SO_DB_INFOKIND_SETVILLAGEITEM, &VillageItem, sizeof( VillageItem ) );

		if( SetItemQuantity( uiItemID, UI16( uiItemsQuantity[ i ] ) )	== FALSE )	return	FALSE;		
	}	*/


	// �������� ������ �����Ѵ�.
	UpdateAllItemsPrice();

	return	TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�޸𸮸� �����Ѵ�.
//	���� ����	:	2002-09-30 ���� 9:16:16 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage_Market::Free()
{
	SOVillage_BaseStructure::Free();	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ������ �����Ѵ�.
//	���� ����	:	2002-10-01 ���� 10:05:06 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_Market::SetItemQuantity( UI16 uiItemID, UI16 uiItemQuantity )
{
	MARKET_ITEM		*pMarketItem;		

	if( ( pMarketItem =	FindItem( uiItemID ) ) == NULL )	return	FALSE;

	pMarketItem->uiItemQuantity	=	uiItemQuantity;

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ������ ���´�.
//	���� ����	:	2002-10-01 ���� 10:17:37 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_Market::GetItemQuantity( UI16 uiItemID, UI16 *puiItemQuantity )
{
	MARKET_ITEM		*pMarketItem;		

	if( ( pMarketItem =	FindItem( uiItemID ) ) == NULL )	return	FALSE;

	*puiItemQuantity	=	pMarketItem->uiItemQuantity;

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ã�´�.
//	���� ����	:	2002-10-01 ���� 10:08:02 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
MARKET_ITEM*	SOVillage_Market::FindItem( UI16 uiItemID )
{
	SI32	i;

	if( uiItemID == 0 )				return	NULL;

	for( i = 0; i < ON_VILLAGE_MAX_ITEM; i++ )
	{
		if( m_SellItem[ i ].uiItemID == uiItemID )
			return	&m_SellItem[ i ];
	}

	return	NULL;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ID�� �˾ƿ´�.
//	���� ����	:	2002-10-01 ���� 10:24:53 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_Market::GetItemID( SI32 siIndex, UI16 *puiItemID )
{
	if( IsValidItemIndex( siIndex ) == FALSE )			return	FALSE;

	if( m_SellItem[ siIndex ].uiItemID != 0)			return	FALSE;

	*puiItemID	=	m_SellItem[ siIndex ].uiItemID;

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ȿ�� �ε������� �˻��Ѵ�.
//	���� ����	:	2002-10-01 ���� 10:26:18 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_Market::IsValidItemIndex( SI32 siIndex )
{
	if( siIndex >= 0 && siIndex < ON_VILLAGE_MAX_ITEM )	return	TRUE;
	else												return	FALSE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� �������� ������ �����Ѵ�.
//	���� ����	:	2002-10-01 ���� 10:35:29 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_Market::UpdateAllItemsPrice()
{
	SI32			i;

	for( i = 0; i < ON_VILLAGE_MAX_ITEM; i++ )
	{
		if( m_SellItem[ i ].uiItemID == 0 )								continue;
		
		if( UpdateItemPrice( m_SellItem[ i ].uiItemID ) == FALSE )		return	FALSE;
	}

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ������ �����Ѵ�.
//	���� ����	:	2002-10-01 ���� 11:19:42 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_Market::UpdateItemPrice( UI16 uiItemID, MONEY *pItemBasePrice )
{
	MARKET_ITEM		*pMarketItem;				
	MONEY			mnItemBasePrice;

	if( ( pMarketItem =	FindItem( uiItemID ) ) == NULL )							return	FALSE;
	
	// �������� ������ ���´�.
	GetItemBasePrice( uiItemID, &mnItemBasePrice );
	m_pIProfit->GetItemPrice( mnItemBasePrice, pMarketItem->uiItemQuantity, &pMarketItem->stItemPrices );

	if( pItemBasePrice != NULL )	*pItemBasePrice	=	mnItemBasePrice;

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ������ ������Ų��.
//	���� ����	:	2002-10-01 ���� 11:14:52 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_Market::IncreaseItemQuantity( UI16 uiItemID, UI16 uiItemQuantity, BOOL bSendToDB )
{
	MARKET_ITEM		*pMarketItem;		
	MONEY			mnItemBasePrice;

	if( ( pMarketItem =	FindItem( uiItemID ) ) == NULL )							return	FALSE;

	pMarketItem->uiItemQuantity	=	min( USHRT_MAX, pMarketItem->uiItemQuantity + uiItemQuantity );	

	// �������� ���� ����� ������ �����Ѵ�.
	if( UpdateItemPrice( pMarketItem->uiItemID, &mnItemBasePrice ) == FALSE )		return	FALSE;																		

	SendChangedItemInfogMsgToPlayer( pMarketItem->uiItemID, pMarketItem->uiItemQuantity, mnItemBasePrice );

	// DB�� �����Ѵ�.
	if( bSendToDB == TRUE )
	{
		UpdateItemInfoInDB( pMarketItem->uiItemID, pMarketItem->uiItemQuantity );
	}

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ������ ���ҽ�Ų��.
//	���� ����	:	2002-10-01 ���� 11:26:50 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_Market::DecreaseItemQuantity( UI16 uiItemID, UI16 uiItemQuantity, BOOL bSendToDB )
{
	MARKET_ITEM		*pMarketItem;		
	MONEY			mnItemBasePrice;

	if( ( pMarketItem =	FindItem( uiItemID ) ) == NULL )				return	FALSE;

	// ������ �ִ� �����ۺ��� ���ҽ�Ű���� �������� �� ������ �˻��Ѵ�.
	if( pMarketItem->uiItemQuantity < uiItemQuantity )					return	FALSE;
	
	// �������� ���ҽ�Ų��.
	pMarketItem->uiItemQuantity		-=	uiItemQuantity;	

	// �������� ���� ����� ������ �����Ѵ�.
	if( UpdateItemPrice( uiItemID, &mnItemBasePrice ) == FALSE )		return	FALSE;																		

	SendChangedItemInfogMsgToPlayer( pMarketItem->uiItemID, pMarketItem->uiItemQuantity, mnItemBasePrice );

	// DB�� �����Ѵ�.
	if( bSendToDB == TRUE )
	{
		UpdateItemInfoInDB( pMarketItem->uiItemID, pMarketItem->uiItemQuantity );
	}

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�Ǹ��ϴ� �������� ����Ʈ�� ���´�.
//	���� ����	:	2002-10-01 ���� 11:32:12 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI16	SOVillage_Market::GetSellItemList( OnlineReceivedItemInfo *pItemInfo )
{
	SI32	i;
	SI16	siItemsNum;
	MONEY	mnItemBasePrice;
	
	for( siItemsNum = i = 0; i < ON_VILLAGE_MAX_ITEM; i++)
	{
		if( m_SellItem[ i ].uiItemID == 0 )															continue;
		if( GetItemBasePrice( m_SellItem[ i ].uiItemID, &mnItemBasePrice ) == FALSE )				continue;
	
		pItemInfo[ siItemsNum ].uiID			=	m_SellItem[ i ].uiItemID;			// �������� ID.
		pItemInfo[ siItemsNum ].uiQuantity		=	m_SellItem[ i ].uiItemQuantity;		// �������� ����.			
		pItemInfo[ siItemsNum ].BasePrice		=	mnItemBasePrice;					// �������� ���� ����

		siItemsNum++;	
	}

	return siItemsNum;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ���� ������ ���´�.
//	���� ����	:	2002-10-01 ���� 11:37:59 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_Market::GetItemBasePrice( UI16 uiItemID, MONEY *pMoney )
{
	CItemHeader		*pItemHeader;	

	if( ( pItemHeader = m_pItem->GetItem( uiItemID ) ) == NULL)		return	FALSE;

	*pMoney			=		MONEY( pItemHeader->m_siDBPrice );

	return	TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �����Ҷ��� ������ ���´�.
//	���� ����	:	2002-10-01 ���� 1:22:59 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_Market::GetBuyItemPrice( UI16 uiItemID, SI32 siGuildRelation, MONEY *pItemPrice )
{
	MARKET_ITEM		*pMarketItem;		
	BOOL			bSuccess = TRUE;

	if( ( pMarketItem =	FindItem( uiItemID ) ) == NULL )	return	FALSE;

	switch( siGuildRelation )
	{

	case ON_GUILD_RELATION_ENEMY:
	case ON_GUILD_RELATION_WAR:
		*pItemPrice		=	pMarketItem->stItemPrices.EnemyGuildBuy;
		break;


	case ON_GUILD_RELATION_MINE:
	case ON_GUILD_RELATION_NEUTRAL:

		*pItemPrice		=	pMarketItem->stItemPrices.Buy;
		break;


	default:

		bSuccess	=	FALSE;
		break;
	}

	return	bSuccess;	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �Ǹ��Ҷ��� ������ ���´�.
//	���� ����	:	2002-10-01 ���� 1:26:34 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_Market::GetSellItemPrice( UI16 uiItemID, SI32 siGuildRelation, MONEY *pItemPrice )
{
	MARKET_ITEM		*pMarketItem;		
	BOOL			bSuccess = TRUE;

	if( ( pMarketItem =	FindItem( uiItemID ) ) == NULL )	return	FALSE;

	switch( siGuildRelation )
	{

	case ON_GUILD_RELATION_ENEMY:
	case ON_GUILD_RELATION_WAR:
		*pItemPrice		=	pMarketItem->stItemPrices.EnemyGuildSell;
		break;
	case ON_GUILD_RELATION_MINE:
	case ON_GUILD_RELATION_NEUTRAL:

		*pItemPrice		=	pMarketItem->stItemPrices.Sell;
		break;


	default:

		bSuccess	=	FALSE;
		break;
	}

	return	bSuccess;	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ���� �� �Ǹ��Ҷ��� ������ ���´�.
//	���� ����	:	2002-10-01 ���� 3:39:45 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_Market::GetBuySellItemPrice( UI16 uiItemID, MONEY *pBuyItemPrice, MONEY *pSellItemPrice )
{
	MARKET_ITEM		*pMarketItem;		
	BOOL			bSuccess = TRUE;

	if( ( pMarketItem =	FindItem( uiItemID ) ) == NULL )	return	FALSE;

	*pBuyItemPrice	=	pMarketItem->stItemPrices.Buy;
	*pSellItemPrice	=	pMarketItem->stItemPrices.Sell;
	
	return	bSuccess;	
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� ������ ������ ������ �ִ� �÷��̾�� ��� �����ش�.
//	���� ����	:	2002-10-01 ���� 1:44:08 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage_Market::SendChangedItemInfogMsgToPlayer( UI16 uiItemID, UI16 uiItemQuantity, MONEY mnItemBasePrice )
{
	// �������� ����Ǿ��ٴ� �޽����� ������.
	SendMsgToAllPlayer( ON_RESPONSE_CHANGED_ITEMINFO, LPVOID( uiItemID ), LPVOID( uiItemQuantity ), LPVOID( &mnItemBasePrice ) );
	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	DB�� �ִ� �������� ������ �����Ѵ�.
//	���� ����	:	2002-10-01 ���� 2:07:58 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage_Market::UpdateItemInfoInDB( UI16 uiItemID, UI16 uiItemQuantity )
{
	DBIKVillageItemInfo		VillageItemInfo;	

	// DB�� ����� �������� ������ �����Ѵ�.
	VillageItemInfo.uiVillageCode	=	m_uiVillageCode;
	VillageItemInfo.siID			=	uiItemID;
	VillageItemInfo.uiQuantity		=	uiItemQuantity;				

	m_pDataBase->writeDBQuery( SO_DB_INFOKIND_UPDATEVILLAGEITEM, &VillageItemInfo, sizeof( DBIKVillageItemInfo ) );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ���� �� �Һ��Ѵ�
//	���� ����	:	2002-10-01 ���� 2:23:56 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage_Market::ProcessProduceReduceItem( UI32 uiPopulation )
{
	SI32	i;
	UI32	uiProduceItem, uiReduceItem;
	FILE	*fp;
	CHAR	szBuffer[1024];
	CHAR	*pszToken;
	CHAR	cSeps[] = " ,\t\n";
	SI32	siProduceValueOfItem, siReduceValueOfItem;
	MONEY	mnItemBasePrice;
	
	if((fp = fopen("ProduceReduceItemNum.txt", "rt")) != NULL)
	{
		while(fgets(szBuffer, 1023, fp) != NULL)
		{
			if((pszToken = strtok(szBuffer, cSeps)) != NULL)
			{
				if(stricmp(pszToken, "PRODUCE:") == 0)
				{
					pszToken				=	strtok(NULL, cSeps);
					siProduceValueOfItem	=	atoi(pszToken);
				}
				else if(stricmp(pszToken, "REDUCE:") == 0)
				{
					pszToken				=	strtok(NULL, cSeps);
					siReduceValueOfItem		=	atoi(pszToken);
				}
			}				
		}

		fclose(fp);
	}
	else
	{	
		return;
	}

	// ���� �������� ������ ���ͼ�
	// Ư��ǰ�̸� �������� ������Ű��
	// �Ҹ�ǰ�̸� �������� ���ҽ�Ų��.
	for(i = 0; i < ON_VILLAGE_MAX_ITEM; i++)	
	{
		// ��ȿ�� ���������� üũ�Ѵ�.
		if( m_SellItem[ i ].uiItemID == 0 )			continue;

		switch( m_SellItem[ i ].uiItemAtb ) 
		{
		case ON_VILLAGE_PRODUCTION :			// Ư��ǰ�̴�.
			
			// �� ���� 60000�� �̸� �϶���.. �������� �߰� ���� �ش�.
			if( m_SellItem[ i ].uiItemQuantity < 60000 )
			{			
				GetItemBasePrice( m_SellItem[ i ].uiItemID, &mnItemBasePrice );

				uiProduceItem	=	max( ( ( uiPopulation * siProduceValueOfItem ) / 10) / mnItemBasePrice, 1 );

				// �������� ������Ų��.
				IncreaseItemQuantity( m_SellItem[ i ].uiItemID, uiProduceItem );
			}

			break;


		case ON_VILLAGE_CONSUMPTION :			// �Ҹ�ǰ�̴�.

			// �� ���� �������� 12000�� �̻��϶��� ���� ���ش�.
			if( m_SellItem[ i ].uiItemQuantity >= 12000)
			{
				GetItemBasePrice( m_SellItem[ i ].uiItemID, &mnItemBasePrice );

				uiReduceItem	=	max( ( ( uiPopulation * siReduceValueOfItem) / 10) / mnItemBasePrice, 1);

				// �������� �Ҹ��Ѵ�.
				DecreaseItemQuantity( m_SellItem[ i ].uiItemID, uiReduceItem);
			}

			break;

		}	
	}		
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �������� �����Ѵ�.
//	���� ����	:	2002-10-21 ���� 4:42:43 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	SOVillage_Market::BuyItem( SOPlayer *pPlayer, UI08 uiFollowerID, UI16 uiItemID, UI16 uiItemQuantity, SI16 siPosInInventory, SI32 siGuildRelation, MONEY *pmnItemPrice, MONEY *pmnCapital )
{
	MONEY				uiRealItemsPrice;		
	MARKET_ITEM			*pMarketItem;		

	// �������� ���´�.
	if( ( pMarketItem = FindItem( uiItemID ) ) == NULL )							return	ON_RET_NOSALE;
	
	// ����� �ϴ� ��ŭ�� ������ ���� �ִ��� �˻��Ѵ�.
	if( pMarketItem->uiItemQuantity < uiItemQuantity )								return	ON_RET_NOITEM;	
	
	// ���� ������ �� �������� ����� �� �� �ִ��� �Ǵ��Ѵ�.		
	GetBuyItemPrice( uiItemID, siGuildRelation, &uiRealItemsPrice );
	uiRealItemsPrice	*=	uiItemQuantity;	

	// �������� �� �� ���� ��ŭ�� ���� ������ �ִ��� �˻��Ѵ�.
	if( pPlayer->EnoughMoney( uiRealItemsPrice ) == FALSE )							return	ON_RET_NOMONEY;		
	
	// ������ �������� ���ҽ�Ų��.								
	DecreaseItemQuantity( uiItemID, uiItemQuantity, FALSE );

	*pmnItemPrice	=	uiRealItemsPrice;
	*pmnCapital		=	uiRealItemsPrice * ON_CAPITAL_RATE_TRADE;	

	return	ON_RET_OK;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �������� �Ǹ��Ѵ�.
//	���� ����	:	2002-11-13 ���� 8:52:56 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	SOVillage_Market::SellItem( SOPlayer *pPlayer, UI08 uiFollowerID, UI16 uiItemID, UI16 uiItemQuantity, SI32 siGuildRelation, MONEY *pmnItemPrice, MONEY *pmnCapital )
{
	MONEY				uiRealItemsPrice;	
	SI32				siTempCredit=0;	
	MARKET_ITEM			*pMarketItem;

	// �� �������� ����ϴ� ���������� �˻��Ѵ�.
	if( ( pMarketItem = FindItem( uiItemID ) ) != NULL )
	{
		GetSellItemPrice( uiItemID, siGuildRelation, &uiRealItemsPrice );
		uiRealItemsPrice	*=	uiItemQuantity;		
	}	
	else
	{		
		uiRealItemsPrice	=	( m_pItem->GetItem( uiItemID )->m_siDBPrice * uiItemQuantity ) / 3;
	}

	// ������ �������� ������Ų��.
	IncreaseItemQuantity( uiItemID, uiItemQuantity, FALSE );

	*pmnItemPrice	=	uiRealItemsPrice;
	*pmnCapital		=	uiRealItemsPrice * ON_CAPITAL_RATE_TRADE;

	return	ON_RET_OK;
}

