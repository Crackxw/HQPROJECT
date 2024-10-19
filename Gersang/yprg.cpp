#include <GSL.h>
#include <main.h>
#include <mbstring.h>	// hojae_ddd
#include <clGame.h>


#include <colortable.h>
#include <etc.h>
#include <SoundManager.h>


#include "yprg.h"
#include <ddutil.h>

#include <mapinterface.h>
#include "text.h"
#include <DebugTool.h>

#define BUFFER				lpScreenBuffer

extern	_clGame					*pGame;
extern	cltMapInterface			clifMap;							// 맵관련 인터페이스 
extern	_Text 					Text;								// 텍스트 
extern	char 					SavePath[];

extern	LPDIRECTDRAWSURFACE7    lpBriefingBuffer;
extern	LPDIRECTDRAWSURFACE7	lpBriefingScrollBuffer;
extern	LPDIRECTDRAWSURFACE7	lpScreenBuffer;
extern	LPDIRECTDRAWSURFACE7	lpDDSPrimary;

#define	FINDRANDOM_STRAIGHT_X	(5.0)
#define	FINDRANDOM_STRAIGHT_Y	(5.0)
#define	FINDRANDOM_RATE			20 
 
#ifdef _IMJIN2PLUS // 임진록 2PLUS 일 경우
#define	BRIEFING_TEXT_X							240			// 게임 중 나오는 스크립트 좌표
#define	BRIEFING_TEXT_Y							110
#define	BRIEFING_TEXT_WIDTH						326
#define	BRIEFING_TEXT_HEIGHT					228
#else				// 임진록2 일 경우
#define	BRIEFING_TEXT_X							188			// 게임 중 나오는 스크립트 좌표
#define	BRIEFING_TEXT_Y							100
#define	BRIEFING_TEXT_WIDTH						278
#define	BRIEFING_TEXT_HEIGHT					180
#endif

#define	BRIEFING_TEXT_HEIGHT_PLUS_PERCENT		50
#define	BRIEFING_TEXT_SCROLL_BUFFER_WIDHT		(BRIEFING_TEXT_WIDTH)
#define	BRIEFING_TEXT_SCROLL_DELAY				110


#define	BRIEFING_TEXT_OBJECTIVE_X			BRIEFING_TEXT_X
#define	BRIEFING_TEXT_OBJECTIVE_Y			363
#define	BRIEFING_TEXT_OBJECTIVE_WIDTH		(BRIEFING_TEXT_WIDTH)
#define	BRIEFING_TEXT_OBJECTIVE_HEIGHT		131

#define	BRIEFING_TEXT_OBJECTIVE_1_X				BRIEFING_TEXT_OBJECTIVE_X
#define	BRIEFING_TEXT_OBJECTIVE_1_Y				BRIEFING_TEXT_OBJECTIVE_Y
#define	BRIEFING_TEXT_OBJECTIVE_1_WIDTH			BRIEFING_TEXT_OBJECTIVE_WIDTH
#define	BRIEFING_TEXT_OBJECTIVE_1_HEIGHT		(BRIEFING_TEXT_OBJECTIVE_HEIGHT / 2)

#define	BRIEFING_TEXT_OBJECTIVE_2_X				BRIEFING_TEXT_OBJECTIVE_X
#define	BRIEFING_TEXT_OBJECTIVE_2_Y				(BRIEFING_TEXT_OBJECTIVE_Y + BRIEFING_TEXT_OBJECTIVE_1_HEIGHT)
#define	BRIEFING_TEXT_OBJECTIVE_2_WIDTH			BRIEFING_TEXT_OBJECTIVE_WIDTH
#define	BRIEFING_TEXT_OBJECTIVE_2_HEIGHT		(BRIEFING_TEXT_OBJECTIVE_HEIGHT / 2)

#define	BRIEFING_TEXT_TITLE_X					BRIEFING_TEXT_X
#define	BRIEFING_TEXT_TITLE_Y					65
#define BRIEFING_TEXT_TITLE_WIDTH				BRIEFING_TEXT_WIDTH
#define	BRIEFING_TEXT_TITLE_HEIGHT				30


#define	BRIEFING_PORTRAIT_DELAY_FRAME			20
#define	BRIEFING_NAME_GAP						0			//초상화와 이름 사이의 공간 (픽셀) 최소 2픽셀 이여야지 안겹친다.

#define	BRIEFING_PORTRAIT_GAP_WIDTH				15
#define BRIEFING_PORTRAIT_GAP_HEIGHT			35

#define	BRIEFING_TEXT_SHADOW_GAP				1

#define	BRIEFING_OBJECTIVE_FONT					BriefingFont
#define	BRIEFING_PORTRAIT_PUT_MIN_TIME			2000


extern	unsigned char DarkTable[][256];

VOID	ClearBriefingTitle();
VOID	ClearBriefingObjective();



extern	SHORT			GameStatus;
extern	SoundManager	*g_pSoundManager;

BOOL FindRandomStraightLine(SHORT sx, SHORT sy, SHORT dx, SHORT dy, SHORT& num, SHORT* array, SHORT XUnit, SHORT YUnit, SHORT Gap)
{	
	double	xGap;
	double	yGap;
	int		i =0 ;
	BOOL	RanValue;

	num = 1;
	array[i * 2]=sx;
	array[i * 2 + 1]=sy;		
	i++;

	RanValue = rand() % 3;	


	if(abs(sx-dx) > abs(sy-dy))
	{
		xGap = abs(sx-dx) / XUnit;
		
		if(xGap <= (1.0))		
			xGap=1;			

		yGap = abs(sy-dy) / xGap;	

		for(; i < xGap; i++)
		{		
			if(sx < dx)
				array[(i * 2)]		= SHORT(sx + i * XUnit);			//x의 위치
			else if(sx > dx)
				array[(i * 2)]		= SHORT(sx - i * XUnit);			//x의 위치
			else 
				array[(i * 2)]		= sx;

			if(sy < dy)
				array[(i * 2)+1]	= SHORT(sy + i * yGap);							//y의 위치	
			else if(sy > dy)
				array[(i * 2)+1]	= SHORT(sy - i * yGap);							//y의 위치	
			else 
				array[(i * 2)+1]	= sy;	

			if(Gap)
				array[(i * 2)+1]		+= (rand() % (SHORT)Gap) - (Gap / 2);

			num++;
		}
	}
	else if(abs(sx-dx) < abs(sy-dy)) 
	{		
		yGap = abs(sy-dy) / YUnit;

		if(yGap <= (1.0))		
			yGap=1;		

		xGap = abs(sx-dx) / yGap;

		for(; i < yGap; i++)
		{		
			if(sx < dx)
				array[(i * 2)]		= SHORT(sx + i * xGap);			//x의 위치
			else if(sx > dx)
				array[(i * 2)]		= SHORT(sx - i * xGap);			//x의 위치
			else 
				array[(i * 2)]		= sx;

			if(sy < dy)
				array[(i * 2)+1]	= SHORT(sy + i * YUnit);			//y의 위치	
			else if(sy > dy)
				array[(i * 2)+1]	= SHORT(sy - i * YUnit);			//y의 위치	
			else 
				array[(i * 2)+1]	= sy;	

		
			if(Gap)
				array[(i * 2)]		+= (rand() % (SHORT)Gap) - (Gap / 2);				

			num++;
		}
	}	
	else
	{
		xGap = abs(sx-dx) / XUnit;
		yGap = abs(sy-dy) / YUnit;
		

		for(; i < xGap; i++)
		{		
			if(sx < dx)
				array[(i * 2)]		= SHORT(sx + i * XUnit);			//x의 위치
			else if(sx > dx)
				array[(i * 2)]		= SHORT(sx - i * XUnit);			//x의 위치
			else 
				array[(i * 2)]		= sx;

			if(sy < dy)
				array[(i * 2)+1]	= SHORT(sy + i * YUnit);			//y의 위치	
			else if(sy > dy)
				array[(i * 2)+1]	= SHORT(sy - i * YUnit);			//y의 위치	
			else 
				array[(i * 2)+1]	= sy;				

			if(Gap)
			{
				if(RanValue)
					array[(i * 2)]			+= (rand() % (SHORT)Gap) - (Gap / 2);
				else
					array[(i * 2)+ 1]		+= (rand() % (SHORT)Gap) - (Gap / 2);				
			}

			num++;
		}
	}
	

	array[(i * 2)]		= dx;
	array[(i * 2)+1]	= dy;
	num++;
	
	if(200 <= (i * 2))	clGrp.Error("FKJE8567", "YPRG000");

	return TRUE;
}


void DrawRandomLine(DWORD number, SHORT* array, BOOL IsSecond)
{
	DWORD i;
	SHORT x, y, nx, ny, screenx1, screeny1, screenx2, screeny2;
	SHORT Line[200];
	SHORT num;

	for(i=0;i<(number-1);i++)
	{
	   x=(SHORT)array[i*2];
	   y=(SHORT)array[i*2+1];

	   nx=(SHORT)array[(i+1)*2];
	   ny=(SHORT)array[(i+1)*2+1];
	  

	   if(IsSecond==TRUE)
	   {
		    clifMap.TransFromRealDotToScreenDot(x, y, screenx1, screeny1);
			clifMap.TransFromRealDotToScreenDot(nx,ny, screenx2, screeny2);

			clGrp.LineC(screenx1-1, screeny1-1, screenx2-1, screeny2-1, 255);
			clGrp.LineC(screenx1,   screeny1,   screenx2,   screeny2, 255);
	   }
	   else
	   {
			FindRandomStraightLine(x, y, nx, ny, num, Line, 2, 2, 10);

			if(num!=0)
			{
				DrawRandomLine(num, Line, TRUE);
			}
	   }  
	}	
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID	ClearBriefingTitle()
/// 목적 : 미션 목적을 쓸 장소를 깨끗히 청소해준다.
///////////////////////////////////////////////////////////////////////////////////////////////////
VOID	ClearBriefingTitle()
{
	if(clGrp.LockSurface(lpBriefingBuffer) == TRUE)
	{
		clGrp.FillBox(BRIEFING_TEXT_TITLE_X,  BRIEFING_TEXT_TITLE_Y,
				BRIEFING_TEXT_TITLE_X + BRIEFING_TEXT_TITLE_WIDTH - 1,
				BRIEFING_TEXT_TITLE_Y + BRIEFING_TEXT_TITLE_HEIGHT - 1,
				(UI08)TRANSCOLOR);
		
		clGrp.UnlockSurface(lpBriefingBuffer);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID	PutTextToTextBufferOneLine(LPSTR lpString, SHORT* lpWidth, SHORT* lpHeight, COLORREF TextColor)
/// 목적 : 텍스트를 한줄에 쓴다.
///////////////////////////////////////////////////////////////////////////////////////////////////
VOID	PutTextToTextBufferOneLine(LPSTR lpString, SHORT* lpWidth, SHORT* lpHeight, COLORREF TextColor)
{
	PutTextToTextBufferOneLine(lpString, lpWidth, lpHeight, TextColor, pGame->pBattle->BriefingFont);	
}	

///////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID	PutTextToTextBufferOneLine(LPSTR lpString, SHORT* lpWidth, SHORT* lpHeight, COLORREF TextColor)
/// 목적 : 텍스트를 한줄에 쓴다.
///////////////////////////////////////////////////////////////////////////////////////////////////
VOID	PutTextToTextBufferOneLine(LPSTR lpString, SHORT* lpWidth, SHORT* lpHeight, COLORREF TextColor, HGDIOBJ hFont)
{	
	SHORT	nStr;	
	SIZE	Size;
	HDC		hdc;		

	if(lpString)
	{
		//텍스트의 길이를 구한다
		nStr = strlen(lpString);			
		
		if(lpBriefingTextBuffer->GetDC(&hdc) == DD_OK)
		{
			SetBkMode(hdc, TRANSPARENT); 
			SelectObject(hdc, hFont);
			
			//그 단어의 사이즈를 구한다.
			GetTextExtentPoint32(hdc, lpString, nStr, &Size);			
			lpBriefingTextBuffer->ReleaseDC(hdc);
		}

		*lpWidth	= Size.cx + BRIEFING_TEXT_SHADOW_GAP;
		*lpHeight	= Size.cy + BRIEFING_TEXT_SHADOW_GAP;

		ClearTextBuffer(0, 0, *lpWidth, *lpHeight);

		if(lpBriefingTextBuffer->GetDC(&hdc) == DD_OK)
		{
			SetBkMode(hdc, TRANSPARENT); 
			SelectObject(hdc, hFont);		
			
			SetTextColor(hdc, RGB(1, 1, 1));
			TextOut(hdc, BRIEFING_TEXT_SHADOW_GAP,	BRIEFING_TEXT_SHADOW_GAP, lpString, nStr);	
			SetTextColor(hdc, TextColor);
			TextOut(hdc, 0,	 0, lpString, nStr);			

			lpBriefingTextBuffer->ReleaseDC(hdc);
		}
		
	}
	else
	{
		*lpWidth	=	0;
		*lpHeight	=	0;
	}
}	

///////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID	PutTextToTextBufferTwoLine(LPSTR lpString1, LPSTR lpString2, SHORT* lpWidth, SHORT* lpHeight, COLORREF TextColor)
/// 목적 : 두 텍스트를 두줄에 쓴다.
///////////////////////////////////////////////////////////////////////////////////////////////////
VOID	PutTextToTextBufferTwoLine(LPSTR lpString1, LPSTR lpString2, SHORT* lpWidth, SHORT* lpHeight, COLORREF TextColor)
{
	HDC		hdc;		
	SHORT	nStr;	
	SHORT	PutY;	
	SIZE	Size;

	ClearTextBuffer();
	
	*lpWidth	=	0;
	*lpHeight	=	0;
	PutY		=	0;

	if(lpString1 || lpString2)
	{
		if(lpBriefingTextBuffer->GetDC(&hdc) == DD_OK)
		{			
			SetBkMode(hdc, TRANSPARENT); 		
			SelectObject(hdc, pGame->pBattle->BriefingFont);

			//첫번째 텍스트를 출력한다.
			if(lpString1 != NULL)
			{
				//텍스트의 길이를 구한다
				nStr = strlen(lpString1);			

				//그 단어의 사이즈를 구한다.
				GetTextExtentPoint32(hdc, lpString1, nStr, &Size);						
				
				SetTextColor(hdc, RGB(1, 1, 1));
				TextOut(hdc, BRIEFING_TEXT_SHADOW_GAP,	BRIEFING_TEXT_SHADOW_GAP, lpString1, nStr);	
				SetTextColor(hdc, TextColor);
				TextOut(hdc, 0,	 0, lpString1, nStr);						

				*lpWidth	= max(Size.cx + BRIEFING_TEXT_SHADOW_GAP, *lpWidth);
				*lpHeight	= PutY = Size.cy + BRIEFING_TEXT_SHADOW_GAP;	
			}

			//두번째 텍스트를 출력한다.
			if(lpString2 != NULL)
			{			
				//텍스트의 길이를 구한다
				nStr = strlen(lpString2);			

				//그 단어의 사이즈를 구한다.
				GetTextExtentPoint32(hdc, lpString2, nStr, &Size);						
				
				SetTextColor(hdc, RGB(1, 1, 1));
				TextOut(hdc, BRIEFING_TEXT_SHADOW_GAP,	PutY + BRIEFING_TEXT_SHADOW_GAP, lpString2, nStr);	
				SetTextColor(hdc, TextColor);
				TextOut(hdc, 0,	 PutY, lpString2, nStr);			

				lpBriefingTextBuffer->ReleaseDC(hdc);

				*lpWidth	= max(Size.cx + BRIEFING_TEXT_SHADOW_GAP, *lpWidth);
				*lpHeight	+= Size.cy + BRIEFING_TEXT_SHADOW_GAP;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID	PutTextToTextBufferLines(CHAR lpStrings[][128], SHORT LineNum, LPRECT	pRect, COLORREF TextColor)
/// 목적 : LineNum의 텍스트를 찍는다.
///////////////////////////////////////////////////////////////////////////////////////////////////
VOID	PutTextToTextBufferLines(CHAR lpStrings[][128], SHORT LineNum, LPRECT	pRect, COLORREF TextColor)
{
	SHORT	i;
	SIZE	Size;
	SHORT	nStr;
	SHORT	PutYOffset;
	HDC		hdc;	

	ClearTextBuffer();	

	
	if(lpBriefingTextBuffer->GetDC(&hdc) == DD_OK)
	{
		SetBkMode(hdc, TRANSPARENT); 		
		SelectObject(hdc, pGame->pBattle->BriefingFont);

		PutYOffset	=	0;
		for(i=0; i < LineNum; i++)
		{	
			//텍스트의 길이를 구한다
			nStr = strlen(lpStrings[i]);			

			//그 단어의 사이즈를 구한다.
			GetTextExtentPoint32(hdc, lpStrings[i], nStr, &Size);								

			pRect[i].left	=	0;
			pRect[i].top	=	PutYOffset;
			pRect[i].right	=	pRect[i].left + Size.cx + BRIEFING_TEXT_SHADOW_GAP;
			pRect[i].bottom	=	pRect[i].top + Size.cy + BRIEFING_TEXT_SHADOW_GAP;
				
			SetTextColor(hdc, RGB(1, 1, 1));
			TextOut(hdc, BRIEFING_TEXT_SHADOW_GAP,	PutYOffset + BRIEFING_TEXT_SHADOW_GAP, lpStrings[i], nStr);	
			SetTextColor(hdc, TextColor);
			TextOut(hdc, 0,							PutYOffset, lpStrings[i], nStr);									

			PutYOffset	+=	Size.cy + BRIEFING_TEXT_SHADOW_GAP;	
		}	

		lpBriefingTextBuffer->ReleaseDC(hdc);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID	PutTextToTextBuffer(SHORT nLimitWidth, LPSTR lpString, SHORT* lpWidth, SHORT* lpHeight, COLORREF TextColor)
/// 목적 : 텍스트가 너무 길면 다음 칸으로 넘기면서 출력한다.
///////////////////////////////////////////////////////////////////////////////////////////////////
VOID	PutTextToTextBuffer(SHORT nLimitWidth, LPSTR lpString, SHORT* lpWidth, SHORT* lpHeight, COLORREF TextColor)
{
	SHORT	sx, sy;
	SHORT	ex, ey;
	SHORT	LineCount		=	0;	
	SHORT	Count;
	SI16	siPrevCount;
	BOOL	IsPutLine		=	FALSE;	
	SHORT	LineIndex;
	CHAR	szLineBuffer[1024];
	CHAR	szPrevLineBuffer[1024];
	SHORT	nTextHeight, siTotalTextHeight;
	SI32	nTextWidth;
	SHORT	nStr;
	HDC		hdc;
	CHAR	ch;
	SIZE	Size;

	ClearTextBuffer();
		
	sx	=	0;
	sy	=	0;
	ex	=	sx + min(nLimitWidth, MAX_BRIEFING_TEXT_XSIZE);
	ey	=	sy + MAX_BRIEFING_TEXT_YSIZE;

	//텍스트의 길이를 구한다
	nStr = strlen(lpString);

	// 문자열을 초기화 한다.
	ZeroMemory(szLineBuffer, sizeof(szLineBuffer));
	ZeroMemory(szPrevLineBuffer, sizeof(szPrevLineBuffer));
	siPrevCount	=	Count	=	0;

	nTextWidth			=	0;
	LineIndex			=	0;
	siTotalTextHeight	=	0;
	

	if(lpBriefingTextBuffer->GetDC(&hdc) == DD_OK)
	{
		// 배경 모드와 폰트를 설정한다.
		SetBkMode(hdc, TRANSPARENT); 
		SelectObject(hdc, pGame->pBattle->BriefingFont);		

		// 문자열의 높이를 얻어온다.
		nTextHeight	=	GetTextHeight(hdc, lpString);

		// 모든 텍스트를 읽을때까지를 루프를 돌린다.

		if(pGame->LanguageCode == ELANGAGECODE_CHINA 
			|| pGame->LanguageCode == ELANGAGECODE_JAPAN
			|| pGame->LanguageCode == ELANGAGECODE_TAIWAN
			|| pGame->LanguageCode == ELANGAGECODE_HONGKONG)
		{
			
			while(lpString[Count] != NULL)
			{
				// 이전에 문자열과 카운트를 백업 받는다.
				strcpy(szPrevLineBuffer, szLineBuffer);
				siPrevCount	=	Count;
				
				// 우선 한 문자를 읽어온다.
				ch = lpString[Count++];
				
				
				if(LineIndex == 0 && ch == ' ');
				else				
					szLineBuffer[LineIndex++]	=	ch;							
				
				if(BYTE(ch) > 127)
				{
					// 2바이트 코드이다.
					ch = lpString[Count++];
					szLineBuffer[LineIndex++]	=	ch;
				}
				
				// 현재 출력해야 할 문자열의 사이즈를 구한다.
				GetTextExtentPoint32(hdc, szLineBuffer, strlen(szLineBuffer), &Size);
				Size.cx	=	Size.cx + BRIEFING_TEXT_SHADOW_GAP;
				Size.cy	=	Size.cy + BRIEFING_TEXT_SHADOW_GAP;
				
				// 한줄을 넘었다.	
				if(Size.cx >= ex)
				{						
					// 현재 출력해야 할 문자열의 사이즈를 구한다.
					GetTextExtentPoint32(hdc, szPrevLineBuffer, strlen(szPrevLineBuffer), &Size);
					Size.cx	=	Size.cx + BRIEFING_TEXT_SHADOW_GAP;
					Size.cy	=	Size.cy + BRIEFING_TEXT_SHADOW_GAP;
					
					// 한줄에 찍을 수 있는 라인을 넘었다.
					// 이전에 라인을 찍고 카운트 역시 이전 쪽으로 돌린다.
					nTextWidth	=	max(nTextWidth, Size.cx);
					
					// 그림자를 찍는다.
					SetTextColor(hdc, RGB(1, 1, 1));
					TextOut(hdc, sx+BRIEFING_TEXT_SHADOW_GAP, sy+BRIEFING_TEXT_SHADOW_GAP, szPrevLineBuffer, strlen(szPrevLineBuffer));	
					
					// 텍슽를 찍는다.
					SetTextColor(hdc, TextColor);
					
					TextOut(hdc, sx, sy, szPrevLineBuffer, strlen(szPrevLineBuffer));						
					
					// 다시 시작하도록 초기화를 해준다.
					ZeroMemory(szLineBuffer, sizeof(szLineBuffer));
					Count		=	siPrevCount;
					LineIndex	=	0;
					
					// y의 값을 증가시킨다.
					sy					+=		nTextHeight + BRIEFING_TEXT_SHADOW_GAP;
					siTotalTextHeight	+=		nTextHeight + BRIEFING_TEXT_SHADOW_GAP;
					
					if(( sy + nTextHeight) > ey)
						TextOut(hdc, 0, 0, "String Too Long", strlen("String Too Long"));								
				}
			}
			
			// 마지막으로 버퍼에 있던 텍스트르 마져 출력해준다.
			if(LineIndex)
			{
				// 현재 출력해야 할 문자열의 사이즈를 구한다.
				GetTextExtentPoint32(hdc, szLineBuffer, strlen(szLineBuffer), &Size);
				Size.cx	=	Size.cx + BRIEFING_TEXT_SHADOW_GAP;
				Size.cy	=	Size.cy + BRIEFING_TEXT_SHADOW_GAP;
				
				// 그림자를 찍는다.
				SetTextColor(hdc, RGB(1, 1, 1));
				TextOut(hdc, sx+BRIEFING_TEXT_SHADOW_GAP, sy+BRIEFING_TEXT_SHADOW_GAP, szLineBuffer, strlen(szLineBuffer));	
				
				// 텍슽를 찍는다.
				SetTextColor(hdc, TextColor);
				TextOut(hdc, sx, sy, szLineBuffer, strlen(szLineBuffer));						
				
				// y의 값을 증가시킨다.
				nTextWidth			=		max(nTextWidth, Size.cx);
				sy					+=		nTextHeight + BRIEFING_TEXT_SHADOW_GAP;
				siTotalTextHeight	+=		nTextHeight + BRIEFING_TEXT_SHADOW_GAP;
			}
		}
		else
		{
			while((Count < nStr) || (IsPutLine == TRUE))
			{
				LineIndex = 0;
				
				if(IsPutLine == FALSE)
				{
					while(1)
					{
						ch = lpString[Count++];
						
						szLineBuffer[LineIndex++] = ch;
						
						if((ch == ' ') || (ch == NULL))					
							break;					
					}			
					
					szLineBuffer[LineIndex++]=NULL;				
					IsPutLine=TRUE;		//찍을 것이 있다.	
				}		
				
				GetTextExtentPoint32(hdc, szLineBuffer, strlen(szLineBuffer), &Size);
				
				//찍을 단어가 문장 끝 보다 작을 경우 그냥 찍는다.
				if((sx + Size.cx + BRIEFING_TEXT_SHADOW_GAP) < ex)
				{	
					SetTextColor(hdc, RGB(1, 1, 1));
					TextOut(hdc, sx+BRIEFING_TEXT_SHADOW_GAP, sy+BRIEFING_TEXT_SHADOW_GAP, szLineBuffer, strlen(szLineBuffer));	
					SetTextColor(hdc, TextColor);
					TextOut(hdc, sx, sy, szLineBuffer, strlen(szLineBuffer));																					
					
					sx			+=	(SHORT)Size.cx;
					nTextWidth	=	max(nTextWidth, sx);
					IsPutLine=FALSE;				
				}
				else
				{					
					sx					=		0;					
					sy					+=		nTextHeight	;										
					siTotalTextHeight	+=		nTextHeight + BRIEFING_TEXT_SHADOW_GAP;
					LineCount++;					//라인의 수를 증가 시킨다.
					
					if((sy+nTextHeight) > ey)
						break;								
				}			
			}							
		}
		
		lpBriefingTextBuffer->ReleaseDC(hdc);
	}	
	

	if(pGame->LanguageCode == ELANGAGECODE_CHINA 
		|| pGame->LanguageCode == ELANGAGECODE_JAPAN
		|| pGame->LanguageCode == ELANGAGECODE_TAIWAN
		|| pGame->LanguageCode == ELANGAGECODE_HONGKONG)
	{
		
		*lpWidth	= max(nTextWidth, sx);	
		*lpHeight	= siTotalTextHeight;	
	}
	else
	{
		siTotalTextHeight	+=	nTextHeight + BRIEFING_TEXT_SHADOW_GAP;
		
		*lpWidth	= nTextWidth;	
		*lpHeight	= siTotalTextHeight;	
	}
	
}	

///////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID  ClearBriefingBuffer()
/// 목적 : 텍스트의 높이를 구한다.
///////////////////////////////////////////////////////////////////////////////////////////////////
VOID	ClearBriefingBuffer()
{		
	if(clGrp.LockSurface(lpBriefingBuffer) == TRUE)
	{
		clGrp.FillScreen((unsigned char)TRANSCOLOR);					

		clGrp.UnlockSurface(lpBriefingBuffer);		
	}			
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : SHORT ClearBriefingTextBuffer()
/// 목적 : 텍스트의 높이를 구한다.
///////////////////////////////////////////////////////////////////////////////////////////////////
VOID	ClearBriefingTextBuffer()
{
	SHORT	sx, sy;
	SHORT	ex, ey;	

	if(lpBriefingBuffer)
	{		
		sx = BRIEFING_TEXT_X;
		sy = BRIEFING_TEXT_Y;
		ex = sx + BRIEFING_TEXT_WIDTH - 1;
		ey = sy + BRIEFING_TEXT_HEIGHT - 1;		
		
		if(clGrp.LockSurface(lpBriefingBuffer) == TRUE)
		{
			clGrp.FillBox(sx, sy, ex, ey, (UI08)TRANSCOLOR);					
			clGrp.UnlockSurface(lpBriefingBuffer);		
		}		
	}	
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID		ClearTextBuffer()
/// 목적 : 텍스트의 높이를 구한다.
///////////////////////////////////////////////////////////////////////////////////////////////////
VOID	ClearTextBuffer()
{
	ClearTextBuffer(0, 0, MAX_BRIEFING_TEXT_XSIZE, MAX_BRIEFING_TEXT_YSIZE);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID		ClearTextBuffer(SHORT x, SHORT y, SHORT width, SHORT height)
/// 목적 : 텍스트의 높이를 구한다.
///////////////////////////////////////////////////////////////////////////////////////////////////
VOID	ClearTextBuffer(SHORT x, SHORT y, SHORT width, SHORT height)
{
	if(lpBriefingTextBuffer)
	{			
		if(clGrp.LockSurface(lpBriefingTextBuffer) == TRUE)
		{
			clGrp.FillBox(x, y, width-1, height-1, (UI08)TRANSCOLOR);					
			clGrp.UnlockSurface(lpBriefingTextBuffer);		
		}		
	}	
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : SHORT GetTextHeight(HDC hdc, LPSTR lpString)
/// 목적 : 텍스트의 높이를 구한다.
///////////////////////////////////////////////////////////////////////////////////////////////////
SHORT GetTextHeight(HDC hdc, LPSTR lpString)
{
	SIZE	Size;

	GetTextExtentPoint32(hdc, lpString, strlen(lpString), &Size);		

	Size.cy += (Size.cy * BRIEFING_TEXT_HEIGHT_PLUS_PERCENT) / 100;

	return SHORT(Size.cy);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : SHORT _ClearBriefingScrollBuffer()
/// 목적 : 텍스트의 높이를 구한다.
///////////////////////////////////////////////////////////////////////////////////////////////////
VOID	ClearBriefingScrollBuffer()
{	
	if(clGrp.LockSurface(lpBriefingScrollBuffer) == TRUE)
	{
		clGrp.FillScreen((BYTE)TRANSCOLOR);		

		clGrp.UnlockSurface(lpBriefingScrollBuffer);		
	}	
}
