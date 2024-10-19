#ifndef	TRADE_HEADER
#define	TRADE_HEADER

#include "SOCommonHeader.h"
#include "GSCDefine.h"

enum	TradeState{ TRADE_STATE_NONE, TRADE_STATE_REQUESTING, TRADE_STATE_REQUESTED, TRADE_STATE_TRADING, TRADE_STATE_TRADEITEM_CHOSE, 
					TRADE_STATE_DECIDE_TRADE, TRADE_STATE_SENDING_TRADE_BOOK, TRADE_STATE_COMPLETE_SENDING_TRADE_BOOK };

struct	strTradeGoods
{
	TradeGoodsKind	Kind;
	LPARAM			lParam1;
	LPARAM			lParam2;
};

class	cltTrade
{
private:
	TradeState		m_State;	
	strTradeGoods	m_stTradeGoods[MAX_TRADE_WINDOW];
	
	UI16			m_uiTradePlayerAccount;				// 교역을 원하는 플레이어의 ID.	
	UI32			m_uiTradeMoney;						// 줄 돈.

private:
	BOOL			IsValidSlotID(UI08 uiSlotID);		// 유효한 슬롯 ID 인지 알아온다.
public:
	VOID			Init();													// 초기화.
	VOID			SetState(TradeState State, LPARAM lParam1 = 0);			// 교역 상태를 설정한다.
	TradeState		GetState();												// 교역 상태를 얻어온다.
	BOOL			IsTrading();											// 현재 교역중인지 알아온다.
	BOOL			DoesExistTradeGoods( TradeGoodsKind Kind );				// 현재 교역중에 특정 품목이 존재하는지 알아온다.
	UI32			GetTradeMoney();										// 교역할 돈을 얻어온다.
	strTradeGoods*	GetTradeGoods();										// 교역할 물품을 얻어온다.
	BOOL			IsTradingItem( UI08 uiFollowerID, UI16 uiItemID );		// 특정 용병의 아이템이 거래중인지 알아온다.
	
	
	BOOL			IsWaitingAcceptTrade(UI16 uiAccount = 0);			// 상대방이 교역을 승낙하는 것을 대기하는지 알아온다.
	BOOL			AcceptedTrade();									// 상대방이 교역을 승낙했다.
	BOOL			NotAcceptedTrade();									// 상대방이 고역을 거부했다.
	BOOL			CancelTrade();										// 교역을 취소한다.
	BOOL			IsAcceptedTrading(UI16 uiAccount = 0);				// 현재 다른 플레이어가 교역을 수락해서 교역을 하고 있는지 알아온다.	
	UI16			GetTradePlayerAccount();							// 현재 교역을 하려는 플레이어의 ID를 얻어온다.
	BOOL			ChooseTradeItem();									// 교역할 아이템을 모두 선택했다.
	BOOL			IsChoseTradeItem(UI16 uiAccount = 0);				// 교역할 아이템을 모두 선택했는지 알아온다.
	BOOL			CancelChooseTradeItem();							// 교역할 아이템을 선택했던 것을 취소한다.
	BOOL			DecideTrade();										// 선택했던 아이템들을 가지고 교역할 것을 결정한다.
	BOOL			IsDecideTrading(UI16 uiAccount = 0);				// 선택했던 아이템들을 가지고 교역할 것을 결정했는지 알아온다.
	BOOL			CancelDecideTrading();								// 교역하려고 했던 것을 취소한다.	
	BOOL			IsSendingTradeBook();								// 장부를 전송중인지 알아온다.
	BOOL			DoesCompleteSendingTradeBook();						// 장부 전송을 완료했는지 알아온다.

	BOOL			WillTradeFollower( UI08 uiFollowerID );				// 특정 용병을 거래중인지 알아온다.
	BOOL			WillTradeItemOfFollower( UI08 uiFollowerID );		// 특정 용병의 아이템을 거래중인지 알아온다.
	

	
	BOOL			CanAddTradeGoods(UI08 uiSlotID, TradeGoodsKind Kind, LPARAM lParam1, LPARAM lParam2);	// 교역 물품을 추가할 수 있는지 알아온다.
	BOOL			AddTradeGoods(UI08 uiSlotID, TradeGoodsKind Kind, LPARAM lParam1, LPARAM lParam2);		// 교역 물품을 추가한다.
};

#endif
