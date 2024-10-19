#ifndef	PRIVATE_MARKET_HEADER
#define PRIVATE_MARKET_HEADER


class	SOPlayer;

#include "SOCommonHeader.h"

struct	strPMItem			// Private Market Item Class
{		
	MONEY		Price;							// 아이템의 가격.
	CHAR		szNote[ON_MAX_BOOTH_NOTE+1];	// 아이템의 설명.
	UI16		uiFollowerID;					// 거래할 아이템을 가진 용병 ID.
	UI16		uiItemID;						// 거래할 아이템의 ID.	
	UI08		uiTradeFollowerSlotID;			// 거래할 용병의 ID Slot. ( 장수는 거래 안됨 )
	SI08		siType;							// 아이템 거래인지 용병 거래인지 구분
};

class	cltPrivateMarket
{
private:
	SOPlayer		*m_pOwnerPlayer;

	strPMItem		m_stPMItem[ON_MAX_BOOTH_ITEM];
	BOOL			m_bOpen;
private:
	BOOL		IsValidSlot(UI08 uiSlot);		// 유효한 슬롯인지 알아온다.	
public:
	VOID	Init(SOPlayer *pOwnerPlayer);					// 초기화.
	VOID	Init();											// 초기화.
	VOID	Open();											// 개인 상점을 연다.
	VOID	Close();										// 개인 상점을 닫는다.
	BOOL	IsOpen();										// 개인 상점이 열려 있는가?	
	
	
	BOOL	AddItem(OnBoothItemForSell	*pBoothItem);		// 개인 상점에 아이템을 추가한다.
	//BOOL	AddItem(UI08 uiSlotID, UI08 uiFollowerID, UI16 uiItemID, MONEY Price, CHAR *pszNote);		// 개인 상점에 아이템을 추가한다.
	BOOL	DelItem(UI08 uiSlotID);																		// 개인 상점에서 아이템을 삭제한다.
	BOOL	BuyItem(UI08 uiSlotID, UI16 uiQuantity, UI16 *puiRemainQuantity, MONEY *pTotalItemPrice,SI16 *psiMercenarySlot);	// 개인 상점에서 아이템을 산다.	
	BOOL	ChangeItem(UI08 uiSlotID, UI08 uiFollowerID, UI16 uiItemID, MONEY Price, CHAR *pszNote, UI08);	// 개인 상점에서 판매하는 아이템을 바꾼다.
	DWORD	GetBaseItems(OnBaseBoothItem *pBaseBoothItem, SI08 *psiItemNum);							// 개인 상점에서 현재 판매하는 아이템들을 얻어온다.	
	DWORD	GetItems(OnBoothItemInfo *pBoothItem, SI08 *psiItemNum, SOPlayer *pPlayer,MyItemData *pFollowerItems, UI16 &numFollowerItems);	// 개인 상점에서 현재 판매하는 아이템들을 얻어온다.	
	
	// 개인 상점에서 판매하는 아이템의 정보를 얻어온다.
	strPMItem*	GetItem(UI08 uiSlotID);				
	strPMItem*	GetItem(UI08 uiSlotID, UI16 uiItemID);


	BOOL	GetItemQuantity(UI08 uiSlotID, UI16 *puiQuantity);											// 개인 상점에서 판매하고 있는 아이템의 수량을 얻어온다.
	BOOL	CanBuyItem(UI08 uiSlotID, UI16 uiQuantity, MONEY Money);									// 개인 상점에서 아이템을 살 수 있는가?	

	BOOL	GetTotalPrice( UI08 uiSlotID, UI16 uiQuantity, MONEY *pPirce );								// 개인 상점에서 판매하고 있는 아이템의 가격을 알아온다.
};

#endif 