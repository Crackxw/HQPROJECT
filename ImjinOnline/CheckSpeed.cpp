#include <windows.h>
#include <Directives.h>
#include <math.h>
#include <stdio.h>
#include "GSCDefine.h"


//========================================================
// OnlinePaletteManager::OnlinePaletteManager() Constructor
DWORD GetNowCpuTime(void)
{
	DWORD	dwhi;
	DWORD	dwlo;
	UI64	TempTime;
	
	__asm
	{
		rdtsc			// 클럭카운트를 edx:eax레지스터에 저장
		mov	dwhi, edx	// 상위 dword 메모리에 백업
		mov	dwlo, eax	// 하위 dword 메모리에 백업	
	}

	TempTime = ((UI64)dwhi << 32) | ((UI64)dwlo);
	TempTime /= 1000;
	return (DWORD)TempTime;
}

float TimeScroll(DWORD TimeScroll)
{
	float		TempFloat;
	DWORD		TempTick;

	TempFloat = 0.0f;
	if(TimeScroll < 200)
	{
		TempTick  = TimeScroll;
		TempFloat = ((float)TempTick / 200.0f);
	}
	else if(TimeScroll < 500)
	{
		TempTick  =  TimeScroll - 200;
		TempFloat =  (((float)TempTick / 300.0f) * 180.0f);
		TempFloat =  (float)sin(TempFloat * 0.017444f);
		TempFloat =  TempFloat * TempFloat;
		TempFloat /= 8.0f;
		TempFloat =  1.0f - (float)fabs(TempFloat);
	}
	else if(TimeScroll < 600)
	{
		TempTick  =  TimeScroll - 500;
		TempFloat =  (((float)TempTick / 100.0f) * 180.0f);
		TempFloat =  (float)sin(TempFloat * 0.017444f);
		TempFloat =  TempFloat * TempFloat;
		TempFloat /= 16.0f;
		TempFloat =  1.0f - (float)fabs(TempFloat);
	}

	return TempFloat;
}

BOOL MakeMoneyText(char* lpSrcMoney, char* lpDesMoney)
{
	int		TempCount;
	int		TempCount1;
	int		StrLength;

	if( lpSrcMoney == NULL )	return FALSE;

	StrLength  = strlen(lpSrcMoney);
	TempCount1 = (StrLength) % 3;
	if(TempCount1 == 0) TempCount1 = 3;
	for(TempCount = 0; TempCount < StrLength; TempCount++)
	{
		if(TempCount1 == 0)
		{
			TempCount1 = 3;
			*lpDesMoney = ',';
			lpDesMoney++;
			TempCount--;
		}
		else
		{
			*lpDesMoney = *lpSrcMoney;
			lpDesMoney++;
			lpSrcMoney++;
			TempCount1--;
		}
	}

	*lpDesMoney = NULL;
	return TRUE;
}

BOOL MakeMoneyText(MONEY iMoney, char* lpDesMoney)
{
	char		TempBuffer[256];

	sprintf((char*)TempBuffer, "%I64d", iMoney);
	return MakeMoneyText((char*)TempBuffer, lpDesMoney);
}

BOOL CheckRenderPage(SI16 RenderPage, SI16 NowPage, SI16& RenderStartPage, SI16& RenderEndPage, SI16& RenderPrior, SI16& RenderNext)
{
	// 우선 Page 수 계산
	if(RenderPage <= 0)
	{
		// 페이지가 없을경우
		RenderStartPage = 1;
		RenderEndPage   = 1;

		RenderPrior = FALSE;
		RenderNext  = FALSE;
	}
	else
	{
		if(RenderPage <= 10)
		{
			// 페이지가 10개 이하일경우
			RenderStartPage = 1;
			RenderEndPage   = RenderPage;

			RenderPrior = FALSE;
			RenderNext  = FALSE;
		}
		else
		{
			// 페이지가 10개 이상일경우
			RenderStartPage = (((NowPage - 1) / 10) * 10) + 1;
			RenderEndPage   = (((NowPage - 1) / 10) * 10) + 10;

			if(RenderStartPage > RenderPage)
			{
				RenderStartPage = RenderPage;
				RenderEndPage   = RenderPage;
			}
			if(RenderEndPage > RenderPage)
			{
				RenderEndPage   = RenderPage;
			}

			RenderPrior = TRUE;
			RenderNext  = TRUE;

			if(RenderStartPage == 1)          RenderPrior = FALSE;
			if(RenderEndPage   == RenderPage) RenderNext  = FALSE;
		}
	}

	return TRUE;
}
