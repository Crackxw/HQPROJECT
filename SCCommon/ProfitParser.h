// ���� ���� : 2002-02-19 ���� 9:46:51 - ������

#ifndef	_SO_PROFIT_HEADER
#define _SO_PROFIT_HEADER

#include <directives.h>
#include <windows.h>

struct	strProfit
{
	UI32	uiBasePrice;		// ���� ����.
	SI32	siProfitPercent;	// �ǸŰ�,���԰�����(%)
	SI32	siRangePercent;		// ������(%)
};

class	cltProfitParser
{
public:
	strProfit	*m_pstProfit;	// Profit
	SI32		m_siProfitNum;	// Profit�� ����.
public:	
	cltProfitParser();			// ������.
	~cltProfitParser();			// �Ҹ���.

	BOOL	Init(CHAR *pszFileName);	// �ʱ�ȭ.
	VOID	Free();						// ����.
};

#endif
