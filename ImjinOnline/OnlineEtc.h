/************************************************************************************************
	���ϸ� : OnlineETC.h
	����� : ������
************************************************************************************************/
#ifndef _ONLINE_ETC_H
#define _ONLINE_ETC_H


#include "GSCDefine.h"


// �¶��ο��� ����ϴ� ��ư�� �⺻���� ��Ʈ
struct	OnlineButtonFont
{
	SI16	siDefault;			// �⺻ ��ư
	SI16	siOn;				// ��ư ���� ���콺�� ���ٴ������
	SI16	siPress;			// ��ư�� ��������.

	BOOL	bDefaultTrans;
	BOOL	bOnTrans;
	BOOL	bPressTrans;
};


DWORD		GetNowCpuTime(void);
float		TimeScroll(DWORD TimeScroll);

BOOL		MakeMoneyText(char* lpSrcMoney, char* lpDesMoney);
BOOL		MakeMoneyText(MONEY iMoney, char* lpDesMoney);


// robypark 2004/11/16 14:29, SI16 siSkipPageNumber = 10 : ���� �������� ���� �� ������ ��ȣ�� ����, �������� ������ ���� ��
BOOL		CheckRenderPage(SI16 RenderPage, SI16 NowPage, SI16& RenderStartPage, SI16& RenderEndPage, SI16& RenderPrior, SI16& RenderNext, SI16 siSkipPageNumber = 10);


#endif