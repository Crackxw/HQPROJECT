#include "SOMain.h"
#include "SOWorld.h"
#include "SOVillage.h"

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���忡�� ������ ���.
//	���� ����	:	2002-11-13 ���� 4:50:44 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	SOVillage::BuyItemInMarket( SOPlayer *pPlayer, UI08 uiFollowerID, UI16 uiItemID, UI16 uiItemQuantity, SI16 siPosInInventory )
{
	SI32			siGuildRelation = ON_GUILD_RELATION_NEUTRAL;
	DWORD			dwResponse;
	MONEY			mnCapital;
	MONEY			mnItemPrice;
	SI32			siTempCredit = 0, siIncreasedCredit = 0;
	MONEY			mnIncreasedCapital;
	MyItemData		*pMyItemData;
	CHAR			szDBArg[ 1024 ];

	// �� ������ ������ �ִ��� �˻��Ѵ�.
	if( DoesMarketExist() == FALSE )					return	ON_RET_NO;

	// �÷��̾�� �� ������ ��ܰ��� ���踦 �����Ѵ�.
	// ��� ������� �˾ƿ´�.
	if( ( uiGuildID != 0 ) && ( pPlayer->uiGuildID != 0 ) && ( pPlayer->siClassInGuild != clWorld->clIGuildClass.GetJWJCode() ) )
	{
		if( uiGuildID == pPlayer->uiGuildID )
		{
			siGuildRelation		=	ON_GUILD_RELATION_MINE;		// �� ������ ��ܿ��̴�.
		}
		else if( clWorld->clIGuild.IsHostilityRelation( pPlayer->uiGuildID, uiGuildID ) == TRUE )	
		{
			siGuildRelation		=	ON_GUILD_RELATION_ENEMY;	// �������� ������ ����̴�.
		}
		else if( clWorld->clIGuild.IsEnemyRelationWar( pPlayer->uiGuildID, uiGuildID ) == TRUE )	
		{
			siGuildRelation		=	ON_GUILD_RELATION_WAR;	// ���� �Ѵ�.
		}
	}

	dwResponse	=	m_pMarket->BuyItem( pPlayer, uiFollowerID, uiItemID, uiItemQuantity, siPosInInventory, siGuildRelation, &mnItemPrice, &mnCapital );

	if( dwResponse != ON_RET_OK )						return	dwResponse;	

	// ������� ���� ��´�.
	pPlayer->DecreaseMoney( mnItemPrice, FALSE );

	// �÷��̾��� �ſ뵵�� �÷��ش�.
	if( ( siTempCredit = pPlayer->GetGiveCredit( mnItemPrice ) ) > 0 )			siIncreasedCredit = pPlayer->IncreaseTradeCredit( siTempCredit, FALSE );

	// �׸��� �÷��̾� �κ��丮�� �������� �߰����ش�.
	pPlayer->IFollower.AddItem( uiFollowerID, uiItemID, uiItemQuantity, siPosInInventory );
	pMyItemData			=	pPlayer->GetItem( uiFollowerID, uiItemID );

	// ������ ������ �÷��ش�.
	mnIncreasedCapital	=	IncreaseCapital( mnCapital, ON_PROFIT_TRADE, FALSE );

	// Ŭ���̾�Ʈ���� �����ߴٰ� �޽����� �����ش�.
	pPlayer->SendFieldMsg( ON_RESPONSE_BUY_ITEM, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( &pPlayer->siMoney ), LPVOID( pPlayer->uiTradeCredit ) );	

	// DB�� �����Ѵ�.
	ZeroMemory( szDBArg, sizeof( szDBArg ) );
	sprintf( szDBArg, "%d, %d, %d, %d, %u, %u, %u, %I64d, %I64d, %d", 	pPlayer->stAccountInfo.siDBAccount, pPlayer->stAccountInfo.uiSlot,
																		uiFollowerID, pMyItemData->siPosInInventory, uiItemID, uiItemQuantity, 
																		uiVillageCode, mnItemPrice, mnIncreasedCapital, siIncreasedCredit );
	clWorld->clDatabase->writeDBQuery( SO_DB_KIND_BUYITEMINMARKET, LPVOID( szDBArg ), strlen( szDBArg ) + 1 );

	return	ON_RET_OK;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���忡 ������ �Ǵ�.	
//	���� ����	:	2002-11-13 ���� 8:51:02 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	SOVillage::SellItemInMarket( SOPlayer *pPlayer, UI08 uiFollowerID, UI16 uiItemID, UI16 uiItemQuantity )
{
	SI32			siGuildRelation = ON_GUILD_RELATION_NEUTRAL;
	DWORD			dwResponse;
	MONEY			mnCapital;
	MONEY			mnItemPrice;
	SI32			siTempCredit = 0, siIncreasedCredit = 0;
	MONEY			mnIncreasedCapital;
	CHAR			szDBArg[ 1024 ];
	MONEY			mnIncreasedMoney;
	SI16			siItemPosInInventory;

	// �� ������ ������ �ִ��� �˻��Ѵ�.
	if( DoesMarketExist() == FALSE )					return	ON_RET_NO;

	// �÷��̾�� �� ������ ��ܰ��� ���踦 �����Ѵ�.
	// ��� ������� �˾ƿ´�.
	if( ( uiGuildID != 0 ) && ( pPlayer->uiGuildID != 0 ) && ( pPlayer->siClassInGuild != clWorld->clIGuildClass.GetJWJCode() ) )
	{
		if( uiGuildID == pPlayer->uiGuildID )
		{
			siGuildRelation		=	ON_GUILD_RELATION_MINE;		// �� ������ ��ܿ��̴�.
		}
		else if( clWorld->clIGuild.IsHostilityRelation( pPlayer->uiGuildID, uiGuildID ) == TRUE )	
		{
			siGuildRelation		=	ON_GUILD_RELATION_ENEMY;	// �������� ������ ����̴�.
		}
		else if( clWorld->clIGuild.IsEnemyRelationWar( pPlayer->uiGuildID, uiGuildID ) == TRUE )	
		{
			siGuildRelation		=	ON_GUILD_RELATION_WAR;	// �������� ������ ����̴�.
		}

	}

	dwResponse	=	m_pMarket->SellItem( pPlayer, uiFollowerID, uiItemID, uiItemQuantity, siGuildRelation, &mnItemPrice, &mnCapital );

	if( dwResponse != ON_RET_OK )						return	dwResponse;

	// ����ڿ��� ���� �ִ´�.
	mnIncreasedMoney	=	pPlayer->IncreaseMoney( mnItemPrice, FALSE );

	// �÷��̾��� �ſ뵵�� �÷��ش�.
	if( ( siTempCredit = pPlayer->GetGiveCredit( mnItemPrice ) ) > 0 )			siIncreasedCredit = pPlayer->IncreaseTradeCredit( siTempCredit, FALSE );	

	// �׸��� �÷��̾� �κ��丮���� �������� �����Ѵ�.
	pPlayer->IFollower.DelItem( uiFollowerID, uiItemID, uiItemQuantity, &siItemPosInInventory );		

	// ������ ������ �÷��ش�.
	mnIncreasedCapital	=	IncreaseCapital( mnCapital, ON_PROFIT_TRADE, FALSE );

	// Ŭ���̾�Ʈ���� �����ߴٰ� �޽����� �����ش�.
	pPlayer->SendFieldMsg( ON_RESPONSE_SELL_ITEM, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( &pPlayer->siMoney ), LPVOID( pPlayer->uiTradeCredit ) );	

	// DB�� �����Ѵ�.
	ZeroMemory( szDBArg, sizeof( szDBArg ) );
	sprintf( szDBArg, "%d, %d, %d, %d, %u, %u, %u, %I64d, %I64d, %d", 	pPlayer->stAccountInfo.siDBAccount, pPlayer->stAccountInfo.uiSlot,
																		uiFollowerID, siItemPosInInventory, uiItemID, uiItemQuantity, 
																		uiVillageCode, mnIncreasedMoney, mnIncreasedCapital, siIncreasedCredit );
	clWorld->clDatabase->writeDBQuery( SO_DB_KIND_SELLITEMINMARKET, LPVOID( szDBArg ), strlen( szDBArg ) + 1 );

	return	ON_RET_OK;
}

/*
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ ����.
//	���� ����	:	2002-09-30 ���� 8:28:28 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::EnterMarket( SOPlayer *pPlayer )
{
	if( m_pMarket->EnterPlayer( pPlayer ) == FALSE )	return	FALSE;
	
	pPlayer->SendFieldMsg( ON_RESPONSE_ENTER_MARKET, SO_SFM_ONLYME, LPVOID( ON_RET_MARKET_OK ) );
	
	return	TRUE;	


}
*/
/*
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���忡�� ������.
//	���� ����	:	2002-09-30 ���� 9:40:49 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::LeaveMarket( SOPlayer *pPlayer )
{	
	if( m_pMarket->LeavePlayer( pPlayer ) == FALSE )	return	FALSE;
		
	

	return	TRUE;

	


	return FALSE;

}	*/
/*

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ��� �ȶ��� ������ ���´�. �Ϲ� ���ݸ� ���´�
//	���� ����	:	2002-04-24 ���� 9:56:26 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::GetItemPrice(UI16 uiItemCode, MONEY *pmnBuy, MONEY *pmnSell)
{
	SI32	siIndex;

	siIndex = FindItem(uiItemCode);

	if(siIndex >= 0)
	{		
		*pmnBuy		=	stMarketItem[siIndex].stItemPrices.Buy;
		*pmnSell	=	stMarketItem[siIndex].stItemPrices.Sell;

		return	TRUE;
	}	
	
	return	FALSE;	
}
*/
/*
//����������������������������������������������������������������������������
// ������ ���� �������� ���� ������ ���Ѵ�.
//����������������������������������������������������������������������������
SI32 SOVillage::GetTotalValue(UI16 uiTempItemCode, UI16 uiTempQuantity)
{
	return 	GetItemPrice(uiTempItemCode) * (UI32)uiTempQuantity;	// ���������� �������� ������ ���Ѵ�.	
}
*/


/*
//----------------------------------------------------------------------------------------------------------------
//	����	:	���� �� �������� ����ϴ� �������ΰ�?
//----------------------------------------------------------------------------------------------------------------
BOOL	SOVillage::IsSaleItem(UI16 uiID)
{
	if(FindItem(uiID) != -1)
		return TRUE;
	else
		return FALSE;
}

*/