#include "SOMain.h"
#include "SOWorld.h"
#include "SOPlayer.h"

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �ſ뵵�� �ø���.
//	���� ����	:	2002-11-13 ���� 5:34:57 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
UI32	SOPlayer::IncreaseTradeCredit( UI32 uiIncreaseCredit, BOOL bSendToDB )
{
	UI32	uiPrevTradeCredit;

	uiPrevTradeCredit	=	uiTradeCredit;

	if( ( UINT_MAX - uiTradeCredit ) >=  uiIncreaseCredit )
		uiTradeCredit		+=	uiIncreaseCredit;
	else
		uiTradeCredit		=	0xffffffff;

	// DB�� �����Ѵ�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �ſ뵵�� ���ҽ�Ų��.
//	���� ����	:	2002-11-13 ���� 5:35:00 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::DecreaseTradeCredit( UI32 uiDecreaseCredit, BOOL bSendToDB )
{
	if(uiTradeCredit > uiDecreaseCredit)
		uiTradeCredit		-=	uiDecreaseCredit;
	else
		uiTradeCredit		=	0;

	// DB�� �����Ѵ�.
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



//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� �ſ뵵�� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
UI32	SOPlayer::GetTradeCredit()
{
	return uiTradeCredit;
}



