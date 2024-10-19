//	수정 일자	: 2002-02-19 오후 1:37:15 - 양정모

#ifndef	SO_IPROFIT_HEADER
#define SO_IPROFIT_HEADER

#include "GSCDefine.h"
#include "ProfitParser.h"

struct	strItemPriceInMarket
{
	MONEY	Buy;			// 일반 사람이 살때의 가격.
	MONEY	Sell;			// 일반 사람이  팔때의 가격.
//	MONEY	GuildBuy;		// 길드원이 살때의 가격.
//	MONEY	GuildSell;		// 길드원이 팔때의 가격.
	MONEY	EnemyGuildBuy;	// 적 길드원의 살때의 가격.
	MONEY	EnemyGuildSell;	// 적 길드원의 팔때의 가격.
};

class	cltIProfit
{
private:
	cltProfitParser		m_clProfitParser;

public:
	cltIProfit();		// 생성자.
	~cltIProfit();		// 소멸자.

	BOOL	Init(CHAR *pszFileName);																		// 초기화.
	VOID	Free();																							// 메모리 해제.
	VOID	GetItemPrice(MONEY BasePrice, UI16 uiQuantity, strItemPriceInMarket *pstItemItemPriceInMarket); // 아이템의 가격을 얻어온다.
};

#endif