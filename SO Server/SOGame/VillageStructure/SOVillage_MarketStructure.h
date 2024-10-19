#ifndef	_VILLAGE_MARKET_HEADER
#define	_VILLAGE_MARKET_HEADER

#include "SOCommonHeader.h"
#include "SOVillage_BaseStructure.h"
#include "OnlineItem.h"
#include "IProfit.h"

class	SODatabase;

typedef struct
{
	UI16					uiItemID;												// �������ڵ�
	UI16					uiItemQuantity;											// ����
	UI08					uiItemAtb;												// �Ӽ�
	strItemPriceInMarket	stItemPrices;											// ������ ����.
} MARKET_ITEM;

class	SOVillage_Market : public SOVillage_BaseStructure
{
	
private:

	static	CItem					*m_pItem;
	static	cltIProfit				*m_pIProfit;
	static	SODatabase				*m_pDataBase;

	MARKET_ITEM						m_SellItem[ ON_VILLAGE_MAX_ITEM ];				// �������� �Ǹ��ϴ� ������.

private:

	BOOL							IsValidItemIndex( SI32 siIndex );				// ��ȿ�� ������ �ε������� �˻��Ѵ�.

public:
	SOVillage_Market();				// ������.
	~SOVillage_Market();			// �Ҹ���.
	
	BOOL			Init( UI16 uiVillageCode, UI32 uiItemsID[ ON_VILLAGE_MAX_ITEM ], SI32 siItemsAtb[ ON_VILLAGE_MAX_ITEM ], 
							UI32 uiItemsQuantity[ ON_VILLAGE_MAX_ITEM ]	);														// �ʱ�ȭ.
	VOID			Free();																										// �޸𸮸� �����Ѵ�.	

	BOOL			SetItemQuantity( UI16 uiItemID, UI16 uiItemQuantity );					// �������� ������ �����Ѵ�.
	BOOL			GetItemQuantity( UI16 uiItemID, UI16 *puiItemQuantity );				// �������� ������ ���´�.
	BOOL			GetItemID( SI32 siIndex, UI16 *puiItemID );								// �������� ID�� �˾ƿ´�.
	
	BOOL			UpdateAllItemsPrice();													// �� �������� ������ �����Ѵ�.

	BOOL			IncreaseItemQuantity( UI16 uiItemID, UI16 uiItemQuantity, BOOL bSendToDB = TRUE );	// �������� ������ ������Ų��.
	BOOL			DecreaseItemQuantity( UI16 uiItemID, UI16 uiItemQuantity, BOOL bSendToDB = TRUE );	// �������� ������ ���ҽ�Ų��.

	MARKET_ITEM*	FindItem( UI16 uiItemID );												// �������� ã�´�.	

	BOOL			UpdateItemPrice( UI16 uiItemID, MONEY *pItemBasePrice = NULL );			// �������� ������ �����Ѵ�.

	SI16			GetSellItemList( OnlineReceivedItemInfo *pItemInfo );					// �Ǹ��ϴ� �������� ����Ʈ�� ���´�.
	BOOL			GetItemBasePrice( UI16 uiItemID, MONEY *pMoney );						// �������� ���� ������ ���´�.	

	BOOL			GetBuyItemPrice( UI16 uiItemID, SI32 siGuildRelation, MONEY *pItemPrice );		// �������� �����Ҷ��� ������ ���´�.
	BOOL			GetSellItemPrice( UI16 uiItemID, SI32 siGuildRelation, MONEY *pItemPrice );		// �������� �Ǹ��Ҷ��� ������ ���´�.
	BOOL			GetBuySellItemPrice( UI16 uiItemID, MONEY *pBuyItemPrice, MONEY *pSellItemPrice );	// �������� ���� �� �Ǹ��Ҷ��� ������ ���´�.

	VOID			SendChangedItemInfogMsgToPlayer( UI16 uiItemID, UI16 uiItemQuantity, MONEY mnItemBasePrice );	// ����� ������ ������ ������ �ִ� �÷��̾�� ��� �����ش�.
	
	VOID			UpdateItemInfoInDB( UI16 uiItemID, UI16 uiItemQuantity );				// DB�� �ִ� �������� ������ �����Ѵ�.

	VOID			ProcessProduceReduceItem( UI32 uiPopulation );							// �������� ���� �� �Һ��Ѵ�

	DWORD			BuyItem( SOPlayer *pPlayer, UI08 uiFollowerID, UI16 uiItemID, UI16 uiItemQuantity, SI16 siPosInInventory, SI32 siGuildRelation, 
							MONEY *pmnItemPrice, MONEY *pmnCapital );		// �������� �������� �����Ѵ�.
	DWORD			SellItem( SOPlayer *pPlayer, UI08 uiFollowerID, UI16 uiItemID, UI16 uiItemQuantity, SI32 siGuildRelation, 
							MONEY *pmnItemPrice, MONEY *pmnCapital );		// �������� �������� �Ǹ��Ѵ�.

	static	VOID	InitStaticVariable( CItem *pItem, cltIProfit *pIProfit, SODatabase *pDataBase );	// ���� ������ �ʱ�ȭ �Ѵ�.
};


#endif
