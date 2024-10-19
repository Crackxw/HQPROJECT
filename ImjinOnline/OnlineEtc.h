/************************************************************************************************
	파일명 : OnlineETC.h
	담당자 : 이윤석
************************************************************************************************/
#ifndef _ONLINE_ETC_H
#define _ONLINE_ETC_H


#include "GSCDefine.h"


// 온라인에서 사용하는 버튼에 기본적인 폰트
struct	OnlineButtonFont
{
	SI16	siDefault;			// 기본 버튼
	SI16	siOn;				// 버튼 위에 마우스를 갖다대었을때
	SI16	siPress;			// 버튼을 눌렀을때.

	BOOL	bDefaultTrans;
	BOOL	bOnTrans;
	BOOL	bPressTrans;
};


DWORD		GetNowCpuTime(void);
float		TimeScroll(DWORD TimeScroll);

BOOL		MakeMoneyText(char* lpSrcMoney, char* lpDesMoney);
BOOL		MakeMoneyText(MONEY iMoney, char* lpDesMoney);


// robypark 2004/11/16 14:29, SI16 siSkipPageNumber = 10 : 여러 페이지로 나눌 때 페이지 번호가 이전, 다음으로 나누는 범위 값
BOOL		CheckRenderPage(SI16 RenderPage, SI16 NowPage, SI16& RenderStartPage, SI16& RenderEndPage, SI16& RenderPrior, SI16& RenderNext, SI16 siSkipPageNumber = 10);


#endif