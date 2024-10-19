//	���� ����	: 2002-02-19 ���� 1:37:15 - ������

#ifndef	SO_IPROFIT_HEADER
#define SO_IPROFIT_HEADER

#include "GSCDefine.h"
#include "ProfitParser.h"

struct	strItemPriceInMarket
{
	MONEY	Buy;			// �Ϲ� ����� �춧�� ����.
	MONEY	Sell;			// �Ϲ� �����  �ȶ��� ����.
//	MONEY	GuildBuy;		// ������ �춧�� ����.
//	MONEY	GuildSell;		// ������ �ȶ��� ����.
	MONEY	EnemyGuildBuy;	// �� ������ �춧�� ����.
	MONEY	EnemyGuildSell;	// �� ������ �ȶ��� ����.
};

class	cltIProfit
{
private:
	cltProfitParser		m_clProfitParser;

public:
	cltIProfit();		// ������.
	~cltIProfit();		// �Ҹ���.

	BOOL	Init(CHAR *pszFileName);																		// �ʱ�ȭ.
	VOID	Free();																							// �޸� ����.
	VOID	GetItemPrice(MONEY BasePrice, UI16 uiQuantity, strItemPriceInMarket *pstItemItemPriceInMarket); // �������� ������ ���´�.
};

#endif