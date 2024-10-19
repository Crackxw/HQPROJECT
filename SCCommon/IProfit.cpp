// ¼öÁ¤ ÀÏÀÚ : 2002-02-19 ¿ÀÀü 10:20:43 - ¾çÁ¤¸ð

#include <GSL.h>
#include "IProfit.h"

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	»ý¼ºÀÚ.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
cltIProfit::cltIProfit()
{

}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¼Ò¸êÀÚ.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
cltIProfit::~cltIProfit()
{
	Free();
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	ÃÊ±âÈ­.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	cltIProfit::Init(CHAR *pszFileName)
{
	if(m_clProfitParser.Init(pszFileName) == FALSE)	return FALSE;

	return TRUE;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¸Þ¸ð¸® ÇØÁ¦.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	cltIProfit::Free()
{
	m_clProfitParser.Free();
}


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	¾ÆÀÌÅÛÀÇ °¡°ÝÀ» ¾ò¾î¿Â´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-02-19 ¿ÀÈÄ 1:39:57 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	cltIProfit::GetItemPrice(MONEY BasePrice, UI16 uiQuantity, strItemPriceInMarket *pstItemItemPriceInMarket)
{
	MONEY		MinPrice, MaxPrice, Price;		// ¾ÆÀÌÅÛÀÇ ÃÖ¼Ò ÃÖ´ë°ª.	
	SI32		i;
	strProfit	*pstProfit;


	for(i = 0; i < m_clProfitParser.m_siProfitNum; i++)
	{
		// ÀÌ ¾ÆÀÌÅÛ ±âÁØ°¡°Ý¿¡ ÇØ´çµÇ´Â Profit °´Ã¼¸¦ ±¸ÇÑ´Ù.
		if(BasePrice <= m_clProfitParser.m_pstProfit[i].uiBasePrice)
			break;			
	}

	pstProfit	=	&m_clProfitParser.m_pstProfit[min(i, m_clProfitParser.m_siProfitNum - 1)];

	// ÀÌ ¾ÆÀÌÅÛÀÇ ÃÖÀú, ÃÖ´ë °ªÀ» ±¸ÇÑ´Ù.
	MinPrice				=	BasePrice - ((BasePrice * pstProfit->siRangePercent) / 100);
	MaxPrice				=	BasePrice + ((BasePrice * pstProfit->siRangePercent) / 100);

	// ÀÌ ¾ÆÀÌÅÛÀÇ °¡°ÝÀ» ±¸ÇÑ´Ù.
	Price							=	(20000 * BasePrice) / max(uiQuantity, 1);

	pstItemItemPriceInMarket->Buy			= min(max(Price, MinPrice), MaxPrice);																			// ÀÏ¹Ý ÇÃ·¹ÀÌ¾î°¡ ¾ÆÀÌÅÛÀ» »ç´Â °¡°Ý.
	//pstItemItemPriceInMarket->GuildBuy		= pstItemItemPriceInMarket->Buy - ((pstProfit->siProfitPercent / 4) * pstItemItemPriceInMarket->Buy) / 100;		// »ó´Ü¿ø ÇÃ·¹ÀÌ¾î°¡ ¾ÆÀÌÅÛÀ» »ç´Â °¡°Ý.
	pstItemItemPriceInMarket->EnemyGuildBuy	= pstItemItemPriceInMarket->Buy + ((pstProfit->siProfitPercent / 4) * pstItemItemPriceInMarket->Buy) / 100;		// Àû´ë ÀûÀÎ »ó´Ü¿ø ÇÃ·¹ÀÌ¾î°¡ ¾ÆÀÌÅÛÀ» »ç´Â °¡°Ý.

	pstItemItemPriceInMarket->Sell			= min(pstItemItemPriceInMarket->Buy - ((pstItemItemPriceInMarket->Buy * pstProfit->siProfitPercent) / 100), pstItemItemPriceInMarket->Buy - 1);			// ÀÏ¹Ý ÇÃ·¹ÀÌ¾î°¡ ¾ÆÀÌÅÛÀ» ÆÄ´Â °¡°Ý.
	if(pstItemItemPriceInMarket->Sell > pstItemItemPriceInMarket->Buy)	pstItemItemPriceInMarket->Sell	=	0;

	//pstItemItemPriceInMarket->GuildSell		= pstItemItemPriceInMarket->Sell + ((pstProfit->siProfitPercent / 4) * pstItemItemPriceInMarket->Buy) / 100;	// »ó´Ü¿ø ÇÃ·¹ÀÌ¾î°¡ ¾ÆÀÌÅÛÀ» ÆÄ´Â °¡°Ý.
	pstItemItemPriceInMarket->EnemyGuildSell= pstItemItemPriceInMarket->Sell - ((pstProfit->siProfitPercent / 4) * pstItemItemPriceInMarket->Buy) / 100;	// Àû´ë ÀûÀÎ »ó´Ü¿ø ÇÃ·¹ÀÌ¾î°¡ ¾ÆÀÌÅÛÀ» ÆÄ´Â °¡°Ý.
}

