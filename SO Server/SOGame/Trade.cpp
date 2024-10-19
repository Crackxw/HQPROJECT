#include "Trade.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蟾晦��.
//	熱薑 橾濠	:	2002-07-04 螃瞪 11:59:27 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltTrade::Init()
{
	SetState(TRADE_STATE_NONE);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	掖羲 鼻鷓蒂 撲薑и棻.
//	熱薑 橾濠	:	2002-07-04 螃瞪 11:59:43 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltTrade::SetState(TradeState State, LPARAM lParam1)
{	
	m_State	=	State;

	switch(m_State)
	{
	case TRADE_STATE_NONE:
		m_uiTradePlayerAccount		=	0;
		m_uiTradeMoney				=	0;
		ZeroMemory(m_stTradeGoods, sizeof(m_stTradeGoods));
		break;
	case TRADE_STATE_REQUESTING:
	case TRADE_STATE_REQUESTED:
		m_uiTradePlayerAccount		=	UI16(lParam1);		
		break;	
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	掖羲 鼻鷓蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-04 螃�� 12:00:09 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
TradeState	cltTrade::GetState()
{
	return m_State;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 掖羲醞檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-07-06 螃�� 2:04:02 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltTrade::IsTrading()
{
	if(m_State != TRADE_STATE_NONE)		return	TRUE;
	else								return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻渠寞檜 掖羲擊 蝓釩ж朝 匙擊 渠晦ж朝雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-07-06 螃�� 2:16:25 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltTrade::IsWaitingAcceptTrade(UI16 uiAccount)
{
	if(m_State == TRADE_STATE_REQUESTING || m_State == TRADE_STATE_REQUESTED)
	{		
		if(uiAccount == 0)				return	TRUE;		
		else
		{
			if(m_uiTradePlayerAccount == uiAccount)	return	TRUE;
			else									return	FALSE;
		}	

	}
	else																			return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻渠寞檜 掖羲擊 蝓釩ц棻.
//	熱薑 橾濠	:	2002-07-06 螃�� 2:17:39 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltTrade::AcceptedTrade()
{
	if(IsWaitingAcceptTrade() == FALSE)		return	FALSE;
	
	// 掖羲 鼻鷓塭堅 撲薑и棻.
	SetState(TRADE_STATE_TRADING);

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻渠寞檜 堅羲擊 剪睡ц棻.
//	熱薑 橾濠	:	2002-07-06 螃�� 2:21:44 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltTrade::NotAcceptedTrade()
{
	if(IsWaitingAcceptTrade() == FALSE)		return	FALSE;
	
	// 掖羲縑 婦溼脹 匙擊 棻衛 蟾晦�� и棻.
	Init();

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	掖羲擊 鏃模и棻.
//	熱薑 橾濠	:	2002-07-06 螃�� 2:39:44 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltTrade::CancelTrade()
{	
	// ⑷營 褒薯煎 掖羲醞檜罹撿 и棻.
	if(IsAcceptedTrading() == FALSE)		return	FALSE;

	// 掖羲縑 婦溼脹 匙擊 棻衛 蟾晦�� и棻.
	Init();
	
	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 棻艇 Ы溯檜橫陛 掖羲擊 熱塊п憮 掖羲擊 ж堅 氈朝雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-07-06 螃�� 2:50:14 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltTrade::IsAcceptedTrading(UI16 uiAccount)
{
	// 檜嘐 掖羲擊 霞ч醞檣雖 �挫恉挬�.
	if(m_State == TRADE_STATE_NONE || m_State == TRADE_STATE_REQUESTING || m_State == TRADE_STATE_REQUESTED)		return	FALSE;

	if(uiAccount == 0)						
		return	TRUE;
	else
	{
		if(m_uiTradePlayerAccount == uiAccount)			return	TRUE;
		else											return	FALSE;
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 掖羲擊 ж溥朝 Ы溯檜橫曖 ID蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-06 螃�� 3:12:37 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI16	cltTrade::GetTradePlayerAccount()
{
	if(IsTrading() == FALSE)				return	FALSE;

	return	m_uiTradePlayerAccount;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	掖羲й 嬴檜蠱擊 賅舒 摹鷗ц棻.
//	熱薑 橾濠	:	2002-07-06 螃�� 6:48:29 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltTrade::ChooseTradeItem()
{
	if(IsAcceptedTrading() == FALSE)		return	FALSE;

	SetState(TRADE_STATE_TRADEITEM_CHOSE);
	
	return	TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	掖羲й 嬴檜蠱擊 賅舒 摹鷗ц朝雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-07-06 螃�� 8:59:57 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltTrade::IsChoseTradeItem(UI16 uiAccount)
{	
	if(m_State == TRADE_STATE_TRADEITEM_CHOSE || m_State == TRADE_STATE_DECIDE_TRADE)
	{
		if(uiAccount == 0)	return	TRUE;
		else
		{
			if(uiAccount == m_uiTradePlayerAccount) return	TRUE;
			else									return	FALSE;
		}
	}
	else									
		return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	掖羲й 嬴檜蠱擊 摹鷗ц湍 匙擊 鏃模и棻.
//	熱薑 橾濠	:	2002-07-06 螃�� 10:41:45 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltTrade::CancelChooseTradeItem()
{
	if(m_State != TRADE_STATE_TRADEITEM_CHOSE)	return	FALSE;

	SetState(TRADE_STATE_TRADING);	

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	摹鷗ц湍 嬴檜蠱菟擊 陛雖堅 掖羲й 匙擊 唸薑и棻.
//	熱薑 橾濠	:	2002-07-07 螃瞪 12:53:02 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltTrade::DecideTrade()
{
	if(m_State != TRADE_STATE_TRADEITEM_CHOSE)	return	FALSE;

	SetState(TRADE_STATE_DECIDE_TRADE);	

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	摹鷗ц湍 嬴檜蠱菟擊 陛雖堅 掖羲й 匙擊 唸薑ц朝雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-07-07 螃瞪 1:01:59 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltTrade::IsDecideTrading(UI16 uiAccount)
{
	if(m_State == TRADE_STATE_DECIDE_TRADE)
	{
		if(uiAccount == 0)	return	TRUE;
		else
		{
			if(uiAccount == m_uiTradePlayerAccount) return	TRUE;
			else									return	FALSE;
		}
	}
	else									
		return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	掖羲ж溥堅 ц湍 匙擊 鏃模и棻.	
//	熱薑 橾濠	:	2002-07-07 螃瞪 2:08:13 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltTrade::CancelDecideTrading()
{
	if(m_State != TRADE_STATE_DECIDE_TRADE)		return	FALSE;

	SetState(TRADE_STATE_TRADEITEM_CHOSE);	

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	掖羲 僭ヶ擊 蹺陛й 熱 氈朝雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-07-07 螃瞪 5:26:55 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltTrade::CanAddTradeGoods(UI08 uiSlotID, TradeGoodsKind Kind, LPARAM lParam1, LPARAM lParam2)
{
	SI32	i;

	if(m_State != TRADE_STATE_TRADING)
	{
		return	FALSE;
	}

	if(Kind == TRADE_GOODS_MONEY)
	{
		return	TRUE;
	}

	if(IsValidSlotID(uiSlotID) == FALSE)
	{
		return	FALSE;
	}

	if(Kind == TRADE_GOODS_NONE)			
	{
		if(m_stTradeGoods[uiSlotID].Kind != TRADE_GOODS_NONE)
		{
			return	TRUE;
		}
		else
		{
			return	FALSE;
		}
	}
	else
	{
		// 瞪縑 螢溥 場擎 ヶ跡擊 棻衛 螢溥 場朝雖 匐餌и棻.
		for(i = 0; i < MAX_TRADE_WINDOW; i++)
		{
			if(m_stTradeGoods[i].Kind == Kind && m_stTradeGoods[i].lParam1 == lParam1 && m_stTradeGoods[i].lParam2 == lParam2) 
			{
				return	FALSE;
			}
		}	

		switch( Kind )
		{
		case TRADE_GOODS_ITEM :
			// 嬴檜蠱擊 還 議葛攪陛 ⑷營 掖羲ж溥堅 ж朝 議葛攪檣雖 憲嬴螞棻.
			if( WillTradeFollower( UI08( lParam1 ) ) == TRUE )
			{
				return	FALSE;
			}
			
			break;

		case TRADE_GOODS_FOLLOWER :

			// 辨煽曖 還 議葛攪曖 嬴檜蠱擊 掖羲ж溥堅 ж朝雖 憲嬴螞棻.
			if( WillTradeItemOfFollower( UI08( lParam1 ) ) == TRUE )
			{
				return	FALSE;
			}

			break;
		}
	

		if(m_stTradeGoods[uiSlotID].Kind != TRADE_GOODS_NONE)
		{
			return	FALSE;
		}
		else
		{
			return	TRUE;
		}
	}
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	掖羲 僭ヶ擊 蹺陛и棻.
//	熱薑 橾濠	:	2002-07-07 螃瞪 5:37:20 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltTrade::AddTradeGoods(UI08 uiSlotID, TradeGoodsKind Kind, LPARAM lParam1, LPARAM lParam2)
{
	if(CanAddTradeGoods(uiSlotID, Kind, lParam1, lParam2) == FALSE)	return	FALSE;

	if(Kind != TRADE_GOODS_MONEY)
	{
		m_stTradeGoods[uiSlotID].Kind		=	Kind;
		m_stTradeGoods[uiSlotID].lParam1	=	lParam1;
		m_stTradeGoods[uiSlotID].lParam2	=	lParam2;
	}	
	else
	{
		m_uiTradeMoney						=	UI32(lParam1);
	}	

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	掖羲й 絲擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-07 螃瞪 6:50:26 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI32	cltTrade::GetTradeMoney()
{
	if(m_State == TRADE_STATE_TRADING || m_State == TRADE_STATE_TRADEITEM_CHOSE || m_State == TRADE_STATE_DECIDE_TRADE)
	{
		return m_uiTradeMoney;
	}
	else
	{
		return 0;
	}
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	掖羲й 僭ヶ擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-07 螃瞪 7:21:00 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
strTradeGoods*	cltTrade::GetTradeGoods()
{
	if(m_State == TRADE_STATE_TRADING || m_State == TRADE_STATE_TRADEITEM_CHOSE || m_State == TRADE_STATE_DECIDE_TRADE)	return m_stTradeGoods;
	else																												return 0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嶸�褲� 蝸煜 ID 檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-07-08 螃�� 4:18:31 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltTrade::IsValidSlotID(UI08 uiSlotID)
{
	if(uiSlotID >= 0 && uiSlotID < MAX_TRADE_WINDOW)	
	{
		return	TRUE;
	}
	else
	{
		return	FALSE;
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	濰睡蒂 瞪歎醞檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-09-24 螃�� 7:03:49 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltTrade::IsSendingTradeBook()
{
	if( m_State != TRADE_STATE_SENDING_TRADE_BOOK )		return	FALSE;

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	濰睡 瞪歎擊 諫猿ц朝雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-09-25 螃瞪 11:16:52 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltTrade::DoesCompleteSendingTradeBook()
{
	if( m_State != TRADE_STATE_COMPLETE_SENDING_TRADE_BOOK )		return	FALSE;

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 掖羲醞縑 か薑 ヶ跡檜 襄營ж朝雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-09-24 螃�� 7:17:07 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltTrade::DoesExistTradeGoods( TradeGoodsKind Kind )
{
	SI32	i;

	for( i = 0; i < MAX_TRADE_WINDOW; i++ )
	{
		if( m_stTradeGoods[i].Kind == Kind ) 
			return	TRUE;
	}	
	
	return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	か薑 辨煽擊 剪楚醞檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-09-30 螃�� 3:58:51 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltTrade::WillTradeFollower( UI08 uiFollowerID )
{
	SI32	i;

	for( i = 0; i < MAX_TRADE_WINDOW; i++ )
	{
		if( m_stTradeGoods[i].Kind == TRADE_GOODS_FOLLOWER ) 
		{
			if( UI08( m_stTradeGoods[i].lParam1 ) == uiFollowerID )
			{
				return	TRUE;
			}
		}
	}	
	
	return	FALSE;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	か薑 辨煽曖 嬴檜蠱擊 剪楚醞檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-09-30 螃�� 4:00:54 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltTrade::WillTradeItemOfFollower( UI08 uiFollowerID )
{
	SI32	i;

	for( i = 0; i < MAX_TRADE_WINDOW; i++ )
	{
		if( m_stTradeGoods[i].Kind == TRADE_GOODS_ITEM ) 
		{
			if( UI08( m_stTradeGoods[i].lParam1 ) == uiFollowerID )
				return	TRUE;
		}
	}	
	
	return	FALSE;		
}


// 撲貲 : か薑 辨煽曖 嬴檜蠱檜 剪楚醞檣雖 憲嬴螞棻.
BOOL	cltTrade::IsTradingItem( UI08 uiFollowerID, UI16 uiItemID )
{
	SI32	i;

	for( i = 0; i < MAX_TRADE_WINDOW; i++ )
	{
		if( m_stTradeGoods[i].Kind == TRADE_GOODS_ITEM ) 
		{
			if( ( uiFollowerID == UI16( m_stTradeGoods[i].lParam1 ) ) && ( uiItemID == HIWORD( m_stTradeGoods[i].lParam2 ) ) )
			{
				return	TRUE;
			}
		}
	}	
	
	return	FALSE;			
}
