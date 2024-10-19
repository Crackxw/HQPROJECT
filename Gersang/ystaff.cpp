#include <main.h>
#include <etc.h>
#include "directx.h"
#include <clGame.h>

#include "ytitlefile.h"
#include "Ystaff.h"
#include "Gmenu.h"
#include "yprg.h"
#include "text.h"

	#define		Y_STAFF_WIDTH					250
	#define		Y_STAFF_SCROLL_DELAY_TIME		40


#define		Y_STAFF_TITLE_FONT_COLOR		(RGB(255, 255, 0))
#define		Y_STAFF_NAME_FONT_COLOR			(RGB(255, 255, 255))
#define		Y_STAFF_DEFAULT_COLOR			Y_STAFF_NAME_FONT_COLOR			

#define		Y_STAFF_STRING_GAP				10

//#define		Y_STAFF_TITLE_CHANGE_TIME		10000

#define		Y_STAFF_X				27
#define		Y_STAFF_Y				63
#define		Y_STAFF_HEIGHT			371

extern	_clGame		*pGame;
extern	char 					SavePath[];
extern	XSPR 					TitleSpr;							// 타이틀 배경 그림에 사용될 포맷 

extern	LPDIRECTDRAWSURFACE7	lpBriefingBuffer;
extern	LPDIRECTDRAWSURFACE7	lpBriefingTextBuffer;
extern	LPDIRECTDRAWSURFACE7	lpBriefingScrollBuffer;
extern	CHAR					szStaffString[][32];

static	SHORT					nStringWidth;
static	SHORT					nStringHeight;
static	SHORT					nStringHeightOffset;
static	SHORT					nStaffIndex;
static	SHORT					nScrollCountWhenEndScroll;
static	BOOL					IsLastIndex;
static	BOOL					IsEndScroll;
static	BOOL					IsPutBriefingTextBuffer;
static	BOOL					IsPutGap;
static	SHORT					nCurrentTitle;
static	DWORD					dwStartScrollTime;


////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID	InitStaff()
/// 설명 : 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	InitStaff()
{
	CHAR	szFileNameBuffer[1024];
	BYTE Palette[768];
#ifdef _IMJIN2PLUS
	// 화면 해상도를 설정한다. 
//	if(clGrp.ChangeVideoMode(MODE_GRAPHICS800_600) == FALSE) clGrp.Error("guni", "ystaff_plus-해상도 fail");
	
	clGrp.SetAllPalette();
	
	// 임의의 서피스를 다시 만든다. 
//	CreateSurface();
	
	GetFileNamePath( "pal\\titlestaff1_plus.pal", SavePath, szFileNameBuffer );
	clGrp.LoadOnlyPaletteFile(szFileNameBuffer, Palette);
	SetGamePalette(Palette);
	
#endif

	PlayStaffBackgroundMusic();
	
	nStringHeight				=	0;
	nStringHeightOffset			=	nStringHeight;
	nStringWidth				=	0;	
	nCurrentTitle				=	-1;
	
	IsEndScroll					=	FALSE;
	IsPutBriefingTextBuffer		=	FALSE;
	IsPutGap					=	TRUE;
	nStaffIndex					=	0;
	
	if(stricmp(szStaffString[nStaffIndex], "end") == 0)		
	{
		nStaffIndex					=	-1;
		IsEndScroll					=	TRUE;
		clGrp.Error("FKJE8567", "YSTAFF001");
	}

	ClearBriefingBuffer();
	ClearBriefingScrollBuffer();

	if(TitleSpr.Image) clGrp.Error("FKJE8567 : Y9333424V23452", "TitleSpr Already Allocated Memory.");


	// 배경화면을 불러온다.	
	GetFileNamePath(Y_TITLE_FILE_STAFF1, SavePath, szFileNameBuffer);

	if(TitleSpr.Image) clGrp.FreeXspr(TitleSpr);
	if(clGrp.LoadXspr(szFileNameBuffer, TitleSpr) == FALSE)		
	clGrp.Error("FKJE8567 : Y0982214132423", "Load Sprite Error : [%s]", szFileNameBuffer);					

	dwStartScrollTime	=	timeGetTime();		
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : SHORT	Staff()
/// 설명 : 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT	Staff()
{
	static	DWORD	dwPrevScrollTime;
	SHORT	menuindex = 0;
	RECT	rect;	
	HGDIOBJ		TextFont;
	COLORREF	TextColor;
	CHAR	szBuffer[32]="         ";	
	
	

	/*
	CHAR	szFileNameBuffer[1024];		
	CHAR	szTitleFileNames[Y_STAFF_MAX_TITLE_FILE_NUM][128]=
	{		
		Y_TITLE_FILE_STAFF1,
		Y_TITLE_FILE_STAFF2,
		Y_TITLE_FILE_STAFF3
		
	};
	*/
	/*
	if(nCurrentTitle < (Y_STAFF_MAX_TITLE_FILE_NUM - 1))
	{
		if(nCurrentTitle != SHORT((timeGetTime() -  dwStartScrollTime) / 10000))
		{	
			nCurrentTitle = SHORT((timeGetTime() -  dwStartScrollTime) / 10000);

			//배경화면을 불러온다.	
			GetFileNamePath(szTitleFileNames[nCurrentTitle], SavePath, szFileNameBuffer);

			if(TitleSpr.Image) FreeXspr(TitleSpr);
			if(LoadXspr(szFileNameBuffer, TitleSpr) == FALSE)		
			Error("FKJE8567 : Y0982214132423", "Load Sprite Error : [%s]", szFileNameBuffer);					
		}
	}
	*/
		

	if(clGrp.LockSurface(SURFACE_BACK) == TRUE)
	{
		clGrp.PutImage32(0, 0, TitleSpr.Header.Xsize, TitleSpr.Header.Ysize,	
					&TitleSpr.Image[TitleSpr.Header.Start[0]]);
		clGrp.UnlockSurface(SURFACE_BACK);
	}	
	
	
	if(IsEndScroll == FALSE && ((timeGetTime() - dwPrevScrollTime) > Y_STAFF_SCROLL_DELAY_TIME))
	{
		//전에 문자열 만큼 다 돌았다면 새로운 문자열을 구한다.			
		if(nStringHeightOffset == nStringHeight)
		{			
			if(nStaffIndex != -1)
			{	
				if(IsPutGap == TRUE)
				{
					if(strcmpi(szStaffString[nStaffIndex], TITLE_STRING) == 0)
					{
						nStaffIndex++;
						TextFont	=	pGame->pBattle->StaffFont;
						TextColor	=	Y_STAFF_TITLE_FONT_COLOR;
					}
					else if(strcmpi(szStaffString[nStaffIndex], NAME_STRING) == 0)
					{
						nStaffIndex++;
						TextFont	=	pGame->pBattle->SaveInfoFont;
						TextColor	=	Y_STAFF_NAME_FONT_COLOR;
					}
					else
					{
						TextFont	=	pGame->pBattle->SaveInfoFont;
						TextColor	=	Y_STAFF_DEFAULT_COLOR;
					}
					
					if(szStaffString[nStaffIndex][0] == NULL)					
						PutTextToTextBufferOneLine(szBuffer, &nStringWidth, &nStringHeight, TextColor, TextFont);
					
					else										
						PutTextToTextBufferOneLine(szStaffString[nStaffIndex], &nStringWidth, &nStringHeight, TextColor, TextFont);

					IsPutBriefingTextBuffer	=	TRUE;									

					nStaffIndex++;		
					nStringHeightOffset = 0;		

					//문자열이 "END"이면 종료이다.
					if(stricmp(szStaffString[nStaffIndex], "end") == 0)		
					{
						nScrollCountWhenEndScroll	=	Y_STAFF_HEIGHT + nStringHeight;
						nStaffIndex					=	-1;						
					}

					IsPutGap	=	FALSE;
				}
				else
				{
					IsPutGap	=	TRUE;
					nStringHeight		=	Y_STAFF_STRING_GAP;	
					nStringHeightOffset	=	0;
				}

			}		
			else
			{
				IsPutBriefingTextBuffer	=	FALSE;
			}
		}		

		//lpBriefingScrollBuffer를 lpBriefingBuffer에 저장한다.
		lpBriefingBuffer->BltFast(0, 0, lpBriefingScrollBuffer, NULL, DDBLTFAST_WAIT);

		ClearBriefingScrollBuffer();
		//lpBriefingBuffer의 일부를 lpBriefingScrollBuffer에 복사한다.
		rect.left	=	Y_STAFF_X;
		rect.top	=	Y_STAFF_Y + 1;
		rect.right	=	rect.left + Y_STAFF_WIDTH;
		rect.bottom	=	rect.top + Y_STAFF_HEIGHT - 1;
		lpBriefingScrollBuffer->BltFast(Y_STAFF_X, Y_STAFF_Y, lpBriefingBuffer, &rect, 
										DDBLTFAST_SRCCOLORKEY|DDBLTFAST_WAIT);

		if(IsPutBriefingTextBuffer == TRUE && IsPutGap == FALSE)
		{			
			//lpBriefingTextBuffer에서의 한 줄을 lpBriefingScrollBuffer에 붙힌다.
			rect.left	=	0;
			rect.top	=	nStringHeightOffset;
			rect.right	=	rect.left + nStringWidth;
			rect.bottom	=	rect.top  + 1;
		
			
			lpBriefingScrollBuffer->BltFast(Y_STAFF_X, Y_STAFF_Y + Y_STAFF_HEIGHT - 1, lpBriefingTextBuffer,
									&rect, DDBLTFAST_SRCCOLORKEY|DDBLTFAST_WAIT);

			nStringHeightOffset++;			
		}
		else
		{
			if(clGrp.LockSurface(lpBriefingScrollBuffer)  == TRUE)
			{
				rect.left	=	Y_STAFF_X;
				rect.top	=	Y_STAFF_Y + Y_STAFF_HEIGHT;
				rect.right	=	rect.left + Y_STAFF_WIDTH - 1;
				rect.bottom	=	rect.top + 1 - 1;
		
				clGrp.FillBox(rect.left, rect.top, rect.right, rect.bottom, (UI08)TRANSCOLOR);
				clGrp.UnlockSurface(lpBriefingScrollBuffer);
			}

			if(IsPutGap) nStringHeightOffset++;
		}	
							
		if(nStaffIndex	==	-1)
		{			
			nScrollCountWhenEndScroll--;
			if(nScrollCountWhenEndScroll == 0)		IsEndScroll		=	TRUE;			
		}

		dwPrevScrollTime	=	timeGetTime();
	}	
	
	rect.left	=	Y_STAFF_X;
	rect.top	=	Y_STAFF_Y;
	rect.right	=	rect.left	+ Y_STAFF_WIDTH;
	rect.bottom =	rect.top	+ Y_STAFF_HEIGHT;
	clGrp.BltFast(SURFACE_BACK, Y_STAFF_X, Y_STAFF_Y, lpBriefingScrollBuffer, &rect, 
						DDBLTFAST_SRCCOLORKEY|DDBLTFAST_WAIT);	

	if( GetAsyncKeyState(VK_RETURN) < 0 || GetAsyncKeyState(VK_ESCAPE) < 0 
		|| GetAsyncKeyState(VK_SPACE) < 0 )
	{
		menuindex	=	1;
	}

	if(menuindex != 0)
	{
		FreeStaff();		
	}

	UpdateScreen();

	return menuindex;
}

VOID	FreeStaff()
{
	if(TitleSpr.Image) clGrp.FreeXspr(TitleSpr);
	FreeMenuBackgroundMusic();
}
