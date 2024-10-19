/************************************************************************************************
	���ϸ� : OnlineETC.cpp
	����� : ������

	Game���� ���Ŵ� ������ Util ����
************************************************************************************************/
#include <GSL.h>
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
		rdtsc			// Ŭ��ī��Ʈ�� edx:eax�������Ϳ� ����
		mov	dwhi, edx	// ���� dword �޸𸮿� ���
		mov	dwlo, eax	// ���� dword �޸𸮿� ���	
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

// robypark 2004/11/16 14:29, SI16 siSkipPageNumber = 10 : ���� �������� ���� �� ������ ��ȣ�� ����, �������� ������ ���� ��
BOOL CheckRenderPage(SI16 RenderPage, SI16 NowPage, SI16& RenderStartPage, SI16& RenderEndPage, SI16& RenderPrior, SI16& RenderNext, SI16 siSkipPageNumber/* = 10*/)
{
	// �켱 Page �� ���
	if(RenderPage <= 0)
	{
		// �������� �������
		RenderStartPage = 1;
		RenderEndPage   = 1;

		RenderPrior = FALSE;
		RenderNext  = FALSE;
	}
	else
	{
		if(RenderPage <= siSkipPageNumber)
		{
			// siSkipPageNumber ��
			// �������� 10�� �����ϰ��
			RenderStartPage = 1;
			RenderEndPage   = RenderPage;

			RenderPrior = FALSE;
			RenderNext  = FALSE;
		}
		else
		{
			// siSkipPageNumber ��
			// �������� 10�� �̻��ϰ��
			RenderStartPage = (((NowPage - 1) / siSkipPageNumber) * siSkipPageNumber) + 1;
			RenderEndPage   = (((NowPage - 1) / siSkipPageNumber) * siSkipPageNumber) + siSkipPageNumber;

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
