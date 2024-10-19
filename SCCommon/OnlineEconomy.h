#ifndef __ONLINE_ECONOMY_H
#define __ONLINE_ECONOMY_H

#include <GSCDefine.h>


struct	EconomyHeader
{
	SI16				siCode;
	SI32				siNameCode;																	// ��ǰ��( Text )
	SI16				siType;

	BOOL				bMultiAccount;
	MONEY				siMinMoney;
	MONEY				uiMaxMoney;
	MONEY				siUnitMoney;
	float				fFixedInterest;
	float				fFlexInterest;
	SI16				siFixedPeriod;
	SI16				siUnitPeriod;
	SI16				siMinPeriod;
	SI16				siMaxPeriod;
	SI16				siDescription;
};
 

class OnlineEconomy
{
private:

	EconomyHeader		*pEconomyHeader;
	SI32				siTotalNum;
	SI32				siAccount;																	// �� ���� ����
	SI32				siInstallment;																// �� ���� ����
	SI32				siLoan;																		// �� ���� ����

public:
	OnlineEconomy();
	~OnlineEconomy();


	VOID				Init( CHAR *pFileName );
	VOID				Free();
	EconomyHeader		*GetHeader( SI32 siIndex );
	EconomyHeader		*GetHeaderCode( SI16 siCode );
	SI32				GetTotalNum();
	SI32				GetAccount();																// �� ���� �շ��� �����Ѵ�.
	SI32				GetInstallment();															// �� ���� ���ݸ� �����Ѵ�.
	SI32				GetLoan();																	// �� ���� ���⸦ �����Ѵ�.

private:
	VOID				Sort();																		// ������ �������� �����Ѵ�.
};



#endif