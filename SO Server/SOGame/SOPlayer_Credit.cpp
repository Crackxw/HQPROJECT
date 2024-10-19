#include "SOMain.h"
#include "SOWorld.h"
#include "SOPlayer.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	무역 신용도를 올린다.
//	수정 일자	:	2002-11-13 오후 5:34:57 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI32	SOPlayer::IncreaseTradeCredit( UI32 uiIncreaseCredit, BOOL bSendToDB )
{
	UI32	uiPrevTradeCredit;

	uiPrevTradeCredit	=	uiTradeCredit;

	if( ( UINT_MAX - uiTradeCredit ) >=  uiIncreaseCredit )
		uiTradeCredit		+=	uiIncreaseCredit;
	else
		uiTradeCredit		=	0xffffffff;

	// DB에 설정한다.
	if( bSendToDB == TRUE )
	{
		DBIKPlayerCredit				PlayerCredit;
		PlayerCredit.DBAccount		=	stAccountInfo.siDBAccount;
		PlayerCredit.uiSlot			=	stAccountInfo.uiSlot;
		PlayerCredit.Credit			=	uiTradeCredit;
		PlayerCredit.DayCredit		=	siDayCredit;

		//pMyWorld->clDatabase->SetDBInfo(SO_DB_INFOKIND_SETTRADECREDIT, &PlayerCredit, NULL);
		pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_SETTRADECREDIT, &PlayerCredit, sizeof(DBIKPlayerCredit));
	}

	return	UI32( uiTradeCredit - uiPrevTradeCredit );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	무역 신용도를 감소시킨다.
//	수정 일자	:	2002-11-13 오후 5:35:00 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::DecreaseTradeCredit( UI32 uiDecreaseCredit, BOOL bSendToDB )
{
	if(uiTradeCredit > uiDecreaseCredit)
		uiTradeCredit		-=	uiDecreaseCredit;
	else
		uiTradeCredit		=	0;

	// DB에 설정한다.
	if( bSendToDB == TRUE )
	{
		DBIKPlayerCredit				PlayerCredit;
		PlayerCredit.DBAccount		=	stAccountInfo.siDBAccount;
		PlayerCredit.uiSlot			=	stAccountInfo.uiSlot;
		PlayerCredit.Credit			=	uiTradeCredit;
		PlayerCredit.DayCredit		=	siDayCredit;

		//pMyWorld->clDatabase->SetDBInfo(SO_DB_INFOKIND_SETTRADECREDIT, &PlayerCredit, NULL);
		pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_SETTRADECREDIT, &PlayerCredit, sizeof(DBIKPlayerCredit));
	}
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	무역 신용도를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI32	SOPlayer::GetTradeCredit()
{
	return uiTradeCredit;
}



