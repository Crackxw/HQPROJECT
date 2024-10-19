#ifndef __ONLINE_ECONOMY_H
#define __ONLINE_ECONOMY_H

#include <GSCDefine.h>


struct	EconomyHeader
{
	SI16				siCode;
	SI32				siNameCode;																	// 상품명( Text )
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
	SI32				siAccount;																	// 총 예금 종류
	SI32				siInstallment;																// 총 적금 종류
	SI32				siLoan;																		// 총 대출 종류

public:
	OnlineEconomy();
	~OnlineEconomy();


	VOID				Init( CHAR *pFileName );
	VOID				Free();
	EconomyHeader		*GetHeader( SI32 siIndex );
	EconomyHeader		*GetHeaderCode( SI16 siCode );
	SI32				GetTotalNum();
	SI32				GetAccount();																// 총 예금 죵류를 리턴한다.
	SI32				GetInstallment();															// 총 예금 적금를 리턴한다.
	SI32				GetLoan();																	// 총 예금 대출를 리턴한다.

private:
	VOID				Sort();																		// 각각의 종류별루 구분한다.
};



#endif