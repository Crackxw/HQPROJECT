
#ifndef	__ONLINECREDIT_H_
#define	__ONLINECREDIT_H_

#include "GSCDefine.h"
struct	CreditIndex
{
	SI32	Serial;
	SI32	MinCredit;
	float	AddCredit;
	SI32	NeedMoney;

	CreditIndex()
	{
		Init();
	}

	VOID	Init()
	{
		Serial		=	0;
		MinCredit	=	0;
		AddCredit	=	0;
		NeedMoney	=	0;
	}

	UI32	GetAddCredit(MONEY Money)
	{
		return UI32(Money / NeedMoney);
	}

	UI32	GetMaxCredit(SI32 Credit)
	{
		return UI32(Credit*AddCredit);
	}

};

class	OnlineCredit
{
public:
	SI32			Num;
	CreditIndex		*pCreditIndex;

public:
	OnlineCredit();
	~OnlineCredit();

	VOID	Init(CHAR *FileName);
};

#endif