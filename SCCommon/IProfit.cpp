// ���� ���� : 2002-02-19 ���� 10:20:43 - ������

#include <GSL.h>
#include "IProfit.h"

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
cltIProfit::cltIProfit()
{

}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Ҹ���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
cltIProfit::~cltIProfit()
{
	Free();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ʱ�ȭ.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltIProfit::Init(CHAR *pszFileName)
{
	if(m_clProfitParser.Init(pszFileName) == FALSE)	return FALSE;

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�޸� ����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltIProfit::Free()
{
	m_clProfitParser.Free();
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ������ ���´�.
//	���� ����	:	2002-02-19 ���� 1:39:57 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltIProfit::GetItemPrice(MONEY BasePrice, UI16 uiQuantity, strItemPriceInMarket *pstItemItemPriceInMarket)
{
	MONEY		MinPrice, MaxPrice, Price;		// �������� �ּ� �ִ밪.	
	SI32		i;
	strProfit	*pstProfit;


	for(i = 0; i < m_clProfitParser.m_siProfitNum; i++)
	{
		// �� ������ ���ذ��ݿ� �ش�Ǵ� Profit ��ü�� ���Ѵ�.
		if(BasePrice <= m_clProfitParser.m_pstProfit[i].uiBasePrice)
			break;			
	}

	pstProfit	=	&m_clProfitParser.m_pstProfit[min(i, m_clProfitParser.m_siProfitNum - 1)];

	// �� �������� ����, �ִ� ���� ���Ѵ�.
	MinPrice				=	BasePrice - ((BasePrice * pstProfit->siRangePercent) / 100);
	MaxPrice				=	BasePrice + ((BasePrice * pstProfit->siRangePercent) / 100);

	// �� �������� ������ ���Ѵ�.
	Price							=	(20000 * BasePrice) / max(uiQuantity, 1);

	pstItemItemPriceInMarket->Buy			= min(max(Price, MinPrice), MaxPrice);																			// �Ϲ� �÷��̾ �������� ��� ����.
	//pstItemItemPriceInMarket->GuildBuy		= pstItemItemPriceInMarket->Buy - ((pstProfit->siProfitPercent / 4) * pstItemItemPriceInMarket->Buy) / 100;		// ��ܿ� �÷��̾ �������� ��� ����.
	pstItemItemPriceInMarket->EnemyGuildBuy	= pstItemItemPriceInMarket->Buy + ((pstProfit->siProfitPercent / 4) * pstItemItemPriceInMarket->Buy) / 100;		// ���� ���� ��ܿ� �÷��̾ �������� ��� ����.

	pstItemItemPriceInMarket->Sell			= min(pstItemItemPriceInMarket->Buy - ((pstItemItemPriceInMarket->Buy * pstProfit->siProfitPercent) / 100), pstItemItemPriceInMarket->Buy - 1);			// �Ϲ� �÷��̾ �������� �Ĵ� ����.
	if(pstItemItemPriceInMarket->Sell > pstItemItemPriceInMarket->Buy)	pstItemItemPriceInMarket->Sell	=	0;

	//pstItemItemPriceInMarket->GuildSell		= pstItemItemPriceInMarket->Sell + ((pstProfit->siProfitPercent / 4) * pstItemItemPriceInMarket->Buy) / 100;	// ��ܿ� �÷��̾ �������� �Ĵ� ����.
	pstItemItemPriceInMarket->EnemyGuildSell= pstItemItemPriceInMarket->Sell - ((pstProfit->siProfitPercent / 4) * pstItemItemPriceInMarket->Buy) / 100;	// ���� ���� ��ܿ� �÷��̾ �������� �Ĵ� ����.
}

