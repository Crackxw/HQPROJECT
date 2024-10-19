// 수정 일자 : 2002-02-19 오전 9:46:51 - 양정모

#ifndef	_SO_PROFIT_HEADER
#define _SO_PROFIT_HEADER

#include <directives.h>
#include <windows.h>

struct	strProfit
{
	UI32	uiBasePrice;		// 기준 가격.
	SI32	siProfitPercent;	// 판매가,구입가차이(%)
	SI32	siRangePercent;		// 변동폭(%)
};

class	cltProfitParser
{
public:
	strProfit	*m_pstProfit;	// Profit
	SI32		m_siProfitNum;	// Profit의 개수.
public:	
	cltProfitParser();			// 생성자.
	~cltProfitParser();			// 소멸자.

	BOOL	Init(CHAR *pszFileName);	// 초기화.
	VOID	Free();						// 해제.
};

#endif
