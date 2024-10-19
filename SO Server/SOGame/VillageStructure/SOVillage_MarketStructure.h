#ifndef	_VILLAGE_MARKET_HEADER
#define	_VILLAGE_MARKET_HEADER

#include "SOCommonHeader.h"
#include "SOVillage_BaseStructure.h"
#include "OnlineItem.h"
#include "IProfit.h"

class	SODatabase;

typedef struct
{
	UI16					uiItemID;												// 아이템코드
	UI16					uiItemQuantity;											// 수량
	UI08					uiItemAtb;												// 속성
	strItemPriceInMarket	stItemPrices;											// 아이템 가격.
} MARKET_ITEM;

class	SOVillage_Market : public SOVillage_BaseStructure
{
	
private:

	static	CItem					*m_pItem;
	static	cltIProfit				*m_pIProfit;
	static	SODatabase				*m_pDataBase;

	MARKET_ITEM						m_SellItem[ ON_VILLAGE_MAX_ITEM ];				// 시전에서 판매하는 아이템.

private:

	BOOL							IsValidItemIndex( SI32 siIndex );				// 유효한 아이템 인덱스인지 검사한다.

public:
	SOVillage_Market();				// 생성자.
	~SOVillage_Market();			// 소멸자.
	
	BOOL			Init( UI16 uiVillageCode, UI32 uiItemsID[ ON_VILLAGE_MAX_ITEM ], SI32 siItemsAtb[ ON_VILLAGE_MAX_ITEM ], 
							UI32 uiItemsQuantity[ ON_VILLAGE_MAX_ITEM ]	);														// 초기화.
	VOID			Free();																										// 메모리를 해제한다.	

	BOOL			SetItemQuantity( UI16 uiItemID, UI16 uiItemQuantity );					// 아이템의 수량을 설정한다.
	BOOL			GetItemQuantity( UI16 uiItemID, UI16 *puiItemQuantity );				// 아이템의 수량을 얻어온다.
	BOOL			GetItemID( SI32 siIndex, UI16 *puiItemID );								// 아이템의 ID를 알아온다.
	
	BOOL			UpdateAllItemsPrice();													// 각 아이템의 가격을 결정한다.

	BOOL			IncreaseItemQuantity( UI16 uiItemID, UI16 uiItemQuantity, BOOL bSendToDB = TRUE );	// 아이템의 수량을 증가시킨다.
	BOOL			DecreaseItemQuantity( UI16 uiItemID, UI16 uiItemQuantity, BOOL bSendToDB = TRUE );	// 아이템의 수량을 감소시킨다.

	MARKET_ITEM*	FindItem( UI16 uiItemID );												// 아이템을 찾는다.	

	BOOL			UpdateItemPrice( UI16 uiItemID, MONEY *pItemBasePrice = NULL );			// 아이템의 가격을 갱신한다.

	SI16			GetSellItemList( OnlineReceivedItemInfo *pItemInfo );					// 판매하는 아이템의 리스트를 얻어온다.
	BOOL			GetItemBasePrice( UI16 uiItemID, MONEY *pMoney );						// 아이템의 기준 가격을 얻어온다.	

	BOOL			GetBuyItemPrice( UI16 uiItemID, SI32 siGuildRelation, MONEY *pItemPrice );		// 아이템을 구입할때의 가격을 얻어온다.
	BOOL			GetSellItemPrice( UI16 uiItemID, SI32 siGuildRelation, MONEY *pItemPrice );		// 아이템을 판매할때의 가격을 얻어온다.
	BOOL			GetBuySellItemPrice( UI16 uiItemID, MONEY *pBuyItemPrice, MONEY *pSellItemPrice );	// 아이템을 구입 및 판매할때의 가격을 얻어온다.

	VOID			SendChangedItemInfogMsgToPlayer( UI16 uiItemID, UI16 uiItemQuantity, MONEY mnItemBasePrice );	// 변경된 아이템 정보를 시전에 있는 플레이어에게 모두 보내준다.
	
	VOID			UpdateItemInfoInDB( UI16 uiItemID, UI16 uiItemQuantity );				// DB에 있는 아이템의 정보를 갱신한다.

	VOID			ProcessProduceReduceItem( UI32 uiPopulation );							// 아이템을 생산 및 소비한다

	DWORD			BuyItem( SOPlayer *pPlayer, UI08 uiFollowerID, UI16 uiItemID, UI16 uiItemQuantity, SI16 siPosInInventory, SI32 siGuildRelation, 
							MONEY *pmnItemPrice, MONEY *pmnCapital );		// 시전에서 아이템을 구입한다.
	DWORD			SellItem( SOPlayer *pPlayer, UI08 uiFollowerID, UI16 uiItemID, UI16 uiItemQuantity, SI32 siGuildRelation, 
							MONEY *pmnItemPrice, MONEY *pmnCapital );		// 시전에서 아이템을 판매한다.

	static	VOID	InitStaticVariable( CItem *pItem, cltIProfit *pIProfit, SODatabase *pDataBase );	// 정적 변수를 초기화 한다.
};


#endif
