// 熱薑 橾濠 : 2002-02-19 螃瞪 10:20:43 - 曄薑賅

#include <GSL.h>
#include "IProfit.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
cltIProfit::cltIProfit()
{

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
cltIProfit::~cltIProfit()
{
	Free();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦��.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltIProfit::Init(CHAR *pszFileName)
{
	if(m_clProfitParser.Init(pszFileName) == FALSE)	return FALSE;

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	詭賅葬 п薯.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltIProfit::Free()
{
	m_clProfitParser.Free();
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱曖 陛問擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-02-19 螃�� 1:39:57 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltIProfit::GetItemPrice(MONEY BasePrice, UI16 uiQuantity, strItemPriceInMarket *pstItemItemPriceInMarket)
{
	MONEY		MinPrice, MaxPrice, Price;		// 嬴檜蠱曖 譆模 譆渠高.	
	SI32		i;
	strProfit	*pstProfit;


	for(i = 0; i < m_clProfitParser.m_siProfitNum; i++)
	{
		// 檜 嬴檜蠱 晦遽陛問縑 п渡腎朝 Profit 偌羹蒂 掘и棻.
		if(BasePrice <= m_clProfitParser.m_pstProfit[i].uiBasePrice)
			break;			
	}

	pstProfit	=	&m_clProfitParser.m_pstProfit[min(i, m_clProfitParser.m_siProfitNum - 1)];

	// 檜 嬴檜蠱曖 譆盪, 譆渠 高擊 掘и棻.
	MinPrice				=	BasePrice - ((BasePrice * pstProfit->siRangePercent) / 100);
	MaxPrice				=	BasePrice + ((BasePrice * pstProfit->siRangePercent) / 100);

	// 檜 嬴檜蠱曖 陛問擊 掘и棻.
	Price							=	(20000 * BasePrice) / max(uiQuantity, 1);

	pstItemItemPriceInMarket->Buy			= min(max(Price, MinPrice), MaxPrice);																			// 橾奩 Ы溯檜橫陛 嬴檜蠱擊 餌朝 陛問.
	//pstItemItemPriceInMarket->GuildBuy		= pstItemItemPriceInMarket->Buy - ((pstProfit->siProfitPercent / 4) * pstItemItemPriceInMarket->Buy) / 100;		// 鼻欽錳 Ы溯檜橫陛 嬴檜蠱擊 餌朝 陛問.
	pstItemItemPriceInMarket->EnemyGuildBuy	= pstItemItemPriceInMarket->Buy + ((pstProfit->siProfitPercent / 4) * pstItemItemPriceInMarket->Buy) / 100;		// 瞳渠 瞳檣 鼻欽錳 Ы溯檜橫陛 嬴檜蠱擊 餌朝 陛問.

	pstItemItemPriceInMarket->Sell			= min(pstItemItemPriceInMarket->Buy - ((pstItemItemPriceInMarket->Buy * pstProfit->siProfitPercent) / 100), pstItemItemPriceInMarket->Buy - 1);			// 橾奩 Ы溯檜橫陛 嬴檜蠱擊 だ朝 陛問.
	if(pstItemItemPriceInMarket->Sell > pstItemItemPriceInMarket->Buy)	pstItemItemPriceInMarket->Sell	=	0;

	//pstItemItemPriceInMarket->GuildSell		= pstItemItemPriceInMarket->Sell + ((pstProfit->siProfitPercent / 4) * pstItemItemPriceInMarket->Buy) / 100;	// 鼻欽錳 Ы溯檜橫陛 嬴檜蠱擊 だ朝 陛問.
	pstItemItemPriceInMarket->EnemyGuildSell= pstItemItemPriceInMarket->Sell - ((pstProfit->siProfitPercent / 4) * pstItemItemPriceInMarket->Buy) / 100;	// 瞳渠 瞳檣 鼻欽錳 Ы溯檜橫陛 嬴檜蠱擊 だ朝 陛問.
}

