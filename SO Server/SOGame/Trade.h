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
	
	UI16			m_uiTradePlayerAccount;				// ������ ���ϴ� �÷��̾��� ID.	
	UI32			m_uiTradeMoney;						// �� ��.

private:
	BOOL			IsValidSlotID(UI08 uiSlotID);		// ��ȿ�� ���� ID ���� �˾ƿ´�.
public:
	VOID			Init();													// �ʱ�ȭ.
	VOID			SetState(TradeState State, LPARAM lParam1 = 0);			// ���� ���¸� �����Ѵ�.
	TradeState		GetState();												// ���� ���¸� ���´�.
	BOOL			IsTrading();											// ���� ���������� �˾ƿ´�.
	BOOL			DoesExistTradeGoods( TradeGoodsKind Kind );				// ���� �����߿� Ư�� ǰ���� �����ϴ��� �˾ƿ´�.
	UI32			GetTradeMoney();										// ������ ���� ���´�.
	strTradeGoods*	GetTradeGoods();										// ������ ��ǰ�� ���´�.
	BOOL			IsTradingItem( UI08 uiFollowerID, UI16 uiItemID );		// Ư�� �뺴�� �������� �ŷ������� �˾ƿ´�.
	
	
	BOOL			IsWaitingAcceptTrade(UI16 uiAccount = 0);			// ������ ������ �³��ϴ� ���� ����ϴ��� �˾ƿ´�.
	BOOL			AcceptedTrade();									// ������ ������ �³��ߴ�.
	BOOL			NotAcceptedTrade();									// ������ ���� �ź��ߴ�.
	BOOL			CancelTrade();										// ������ ����Ѵ�.
	BOOL			IsAcceptedTrading(UI16 uiAccount = 0);				// ���� �ٸ� �÷��̾ ������ �����ؼ� ������ �ϰ� �ִ��� �˾ƿ´�.	
	UI16			GetTradePlayerAccount();							// ���� ������ �Ϸ��� �÷��̾��� ID�� ���´�.
	BOOL			ChooseTradeItem();									// ������ �������� ��� �����ߴ�.
	BOOL			IsChoseTradeItem(UI16 uiAccount = 0);				// ������ �������� ��� �����ߴ��� �˾ƿ´�.
	BOOL			CancelChooseTradeItem();							// ������ �������� �����ߴ� ���� ����Ѵ�.
	BOOL			DecideTrade();										// �����ߴ� �����۵��� ������ ������ ���� �����Ѵ�.
	BOOL			IsDecideTrading(UI16 uiAccount = 0);				// �����ߴ� �����۵��� ������ ������ ���� �����ߴ��� �˾ƿ´�.
	BOOL			CancelDecideTrading();								// �����Ϸ��� �ߴ� ���� ����Ѵ�.	
	BOOL			IsSendingTradeBook();								// ��θ� ���������� �˾ƿ´�.
	BOOL			DoesCompleteSendingTradeBook();						// ��� ������ �Ϸ��ߴ��� �˾ƿ´�.

	BOOL			WillTradeFollower( UI08 uiFollowerID );				// Ư�� �뺴�� �ŷ������� �˾ƿ´�.
	BOOL			WillTradeItemOfFollower( UI08 uiFollowerID );		// Ư�� �뺴�� �������� �ŷ������� �˾ƿ´�.
	

	
	BOOL			CanAddTradeGoods(UI08 uiSlotID, TradeGoodsKind Kind, LPARAM lParam1, LPARAM lParam2);	// ���� ��ǰ�� �߰��� �� �ִ��� �˾ƿ´�.
	BOOL			AddTradeGoods(UI08 uiSlotID, TradeGoodsKind Kind, LPARAM lParam1, LPARAM lParam2);		// ���� ��ǰ�� �߰��Ѵ�.
};

#endif
