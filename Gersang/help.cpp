#include <GSL.h>

#include <main.h>
#include <clGame.h>

#include "maindata.h"
#include "help.h"

#include <myfile.h>
#include "..\Gersang\DirectX.h"

// 판넬 위에 있는 도움말 텍스트 시작 좌표
#ifdef _IMJIN2PLUS
	#define		HELP_EXTEND_TEXT_X		316
	#define		HELP_EXTEND_TEXT_Y		450
#else
	#define		HELP_EXTEND_TEXT_X		200
	#define		HELP_EXTEND_TEXT_Y		350
#endif

#define		HELP_EXTEND_TEXT_WIDTH		300

#define		HELP_FONTNUM_CROP			0
#define		HELP_FONTNUM_TREE			1
#define		HELP_FONTNUM_MAINTENANCE	2						 
#define		HELP_FONTNUM_MAGICPOWER		3


extern	_clGame					*pGame;
extern	_MainData			GD;									// 게임의 주데이터 

// 캐릭터 그림용
extern LPDIRECTDRAWSURFACE7	lpScreenBuffer;
extern LPDIRECTDRAWSURFACE7	lpHelpBuffer;
extern LPDIRECTDRAWSURFACE7	lpScrollBuffer;

VOID _Help::Init()
{
	SHORT	i;

	ZeroMemory(this, sizeof(_Help));

	m_ssHelpSwitch=FALSE;

	m_ssHelpDrawX=0;
	m_ssHelpDrawY=0;

	for(i=0; i < 3; i++)
	{
		m_bUsedInfo[i] = FALSE;
	}	
}

VOID _Help::SetHelp(SHORT drawx, SHORT drawy, CHAR* text, CHAR* extendtext,
					SHORT money, SHORT tree, SHORT maintenance, SHORT magicpower, CHAR hotKey, 
					CHAR* lpInfo1, CHAR* lpInfo2, CHAR* lpInfo3)
{	

	m_ssHelpSwitch=TRUE;
	
	m_ssHelpDrawX=drawx;
	m_ssHelpDrawY=drawy;

	m_ssMoney		= money;
	m_ssTree		= tree;
	m_scHotKey		= hotKey;	
	m_ssMaintenance	= maintenance;
	m_ssMagicPower	= magicpower;

	if(text!=NULL)
		strcpy(m_szText,text);	

	if(lpInfo1!=NULL)
	{		
		strcpy(m_szInfo[0], lpInfo1);
		m_bUsedInfo[0] = TRUE;
	}
	else
	{
		m_bUsedInfo[0] = FALSE;
	}

	if(lpInfo2!=NULL)
	{
		strcpy(m_szInfo[1], lpInfo2);
		m_bUsedInfo[1] = TRUE;
	}
	else
	{
		m_bUsedInfo[1] = FALSE;
	}

	if(lpInfo3!=NULL)
	{
		strcpy(m_szInfo[2], lpInfo3);
		m_bUsedInfo[2] = TRUE;
	}
	else
	{
		m_bUsedInfo[2] = FALSE;
	}

	if (extendtext == NULL)
		strcpy(m_szExtendText, "");
	else
		strcpy(m_szExtendText, extendtext);
}


VOID _Help::Draw()
{

	HDC		hdc=NULL;
	SHORT	MaxWidth = 0;
	SHORT	MaxHeight = 0;
	SHORT	TempWidth=0, TempHeight=0;
	SIZE	Size;
	CHAR	buffer[1024];
	SHORT	i;
	RECT	rect, rect1;
	SHORT	NextHelpDrawY;
	SHORT	NextHelpDrawX;
 

	if(m_ssHelpSwitch==FALSE) 
		return ;

	m_ssHelpSwitch=FALSE;	
	
	//유효한 인자중 최고의 개수를 센다.
	if(lpHelpBuffer->GetDC(&hdc) == DD_OK)
	{
		SelectObject(hdc, pGame->pBattle->HelpFont);
		SetBkColor( hdc, RGB( 0, 0, 255 ) );
		SetTextColor( hdc, RGB( 80, 80, 80 ) );
   	    SetBkMode(hdc, TRANSPARENT);

		if(m_scHotKey != NULL)			
			wsprintf(buffer, "%s(%c)", m_szText, m_scHotKey);
		else		
			wsprintf(buffer, "%s", m_szText);		

		GetTextExtentPoint32(hdc, buffer, strlen(buffer), &Size);

		MaxWidth  = (SHORT)Size.cx;
		MaxHeight = (SHORT)Size.cy;
		
		if(m_ssMoney)
		{
			wsprintf(buffer, " %d ", m_ssMoney);
			GetTextExtentPoint32(hdc, buffer, strlen(buffer), &Size);
			
			if(Size.cy < GetFileYsize(FILE_HELPRESOURCE))			
				TempHeight = GetFileYsize(FILE_HELPRESOURCE);				
			else			
				TempHeight = (SHORT)Size.cy;					

			TempWidth += GetFileXsize(FILE_HELPRESOURCE) + (SHORT)Size.cx;
		}

		if(m_ssTree)
		{
			wsprintf(buffer, " %d ", m_ssTree);
			GetTextExtentPoint32(hdc, buffer, strlen(buffer), &Size);

			if(Size.cy < GetFileYsize(FILE_HELPRESOURCE))			
				TempHeight = GetFileYsize(FILE_HELPRESOURCE);				
			else			
				TempHeight = (SHORT)Size.cy;					

			TempWidth += GetFileXsize(FILE_HELPRESOURCE) + (SHORT)Size.cx;
		}

		if(m_ssMaintenance)
		{
			wsprintf(buffer, " %d ", m_ssMaintenance);
			GetTextExtentPoint32(hdc, buffer, strlen(buffer), &Size);

			if(Size.cy < GetFileYsize(FILE_HELPRESOURCE))			
				TempHeight = GetFileYsize(FILE_HELPRESOURCE);				
			else			
				TempHeight = (SHORT)Size.cy;					

			TempWidth += GetFileXsize(FILE_HELPRESOURCE) + (SHORT)Size.cx;
		}

		if(m_ssMagicPower)
		{
			wsprintf(buffer, " %d ", m_ssMagicPower);
			GetTextExtentPoint32(hdc, buffer, strlen(buffer), &Size);

			if(Size.cy < GetFileYsize(FILE_HELPRESOURCE))			
				TempHeight = GetFileYsize(FILE_HELPRESOURCE);				
			else			
				TempHeight = (SHORT)Size.cy;					

			TempWidth += GetFileXsize(FILE_HELPRESOURCE) + (SHORT)Size.cx;
		}

		if(MaxWidth < TempWidth)		
			MaxWidth = TempWidth;		

		MaxHeight += TempHeight;

		for(i=0; i < 3; i++)
		{
			if(m_bUsedInfo[i] == TRUE)
			{
				GetTextExtentPoint32(hdc, m_szInfo[i], strlen(m_szInfo[i]), &Size);
				MaxHeight += (SHORT)Size.cy;

				if(MaxWidth < Size.cx)			
					MaxWidth = (SHORT)Size.cx;					
			}
		}

		lpHelpBuffer->ReleaseDC(hdc);
	}

	//MaxWidth, MaxHeight 만큼의 스크롤 모양을 찍는다.
	rect1.left=0;
	rect1.top=0;
	rect1.right=MaxWidth;
	rect1.bottom=MaxHeight;

	//스크롤이 정확히 사각형이 아니므로 이쁘게 하기 위해 끝을 위한 공간을 남겨 둔다.
	rect1.right+=15;
	rect1.bottom+=15;
	
	lpHelpBuffer->Blt(&rect1, lpScrollBuffer, NULL, DDBLT_WAIT, NULL);


	//HelpDrawX, HelpDrawY는 화면상의 찍어야 할 박스의 아래 부분을 준 것이다. 
	//그러므로 X, Y자리를 수정해야 한다.
	m_ssHelpDrawY -= (SHORT)rect1.bottom;
	m_ssHelpDrawX -= (SHORT)rect1.right / 2;


	//처음 찍는 곳의 위치가 알맞는 장소가 아니면 새로 교정한다.
	if((m_ssHelpDrawX+rect1.right) > (clGrp.GetScreenXsize()-1))
		m_ssHelpDrawX = (clGrp.GetScreenXsize() - 1) - (SHORT)rect1.right;

	if(m_ssHelpDrawX < 0)
		m_ssHelpDrawX = 0;

	if((m_ssHelpDrawY+rect1.bottom) > (clGrp.GetScreenYsize() - 1))	
		m_ssHelpDrawY = (clGrp.GetScreenYsize() - 1) - (SHORT)rect1.bottom;

	if(m_ssHelpDrawY < 0)
		m_ssHelpDrawY = 0;

	NextHelpDrawY = (rect1.bottom/2) - (MaxHeight/2);	


	if(lpHelpBuffer->GetDC(&hdc)==DD_OK)
	{       
	    SelectObject(hdc, pGame->pBattle->HelpFont);
        SetBkColor( hdc, RGB( 0, 0, 255 ) );
        SetTextColor( hdc, RGB( 80, 80, 80 ) );
   	    SetBkMode(hdc, TRANSPARENT);

		NextHelpDrawX = (rect1.right/2) - (MaxWidth/2);						

		if(m_scHotKey != NULL)
		{	
			for(i=0; i < 4; i++)
			{
				ZeroMemory(buffer, sizeof(buffer));

				switch(i)
				{
				case 0:															
					wsprintf(buffer, "%s", m_szText);
					TextOut(hdc, NextHelpDrawX, NextHelpDrawY, buffer, lstrlen(buffer));
					break;
				case 1:
					wsprintf(buffer, "(");
					TextOut(hdc, NextHelpDrawX, NextHelpDrawY, buffer, lstrlen(buffer));
					break;
				case 2:
					SetTextColor( hdc, RGB( 225, 220, 180 ) );
					wsprintf(buffer, "%c", m_scHotKey);
					TextOut(hdc, NextHelpDrawX, NextHelpDrawY, buffer, lstrlen(buffer));
					SetTextColor( hdc, RGB( 80, 80, 80 ) );
					break;
				case 3:
					wsprintf(buffer, ")");
					TextOut(hdc, NextHelpDrawX, NextHelpDrawY, buffer, lstrlen(buffer));
					break;				
				}
				
				GetTextExtentPoint32(hdc, buffer, strlen(buffer), &Size);
				NextHelpDrawX += (SHORT)Size.cx;
			}					

			wsprintf(buffer, "%s(%c)", m_szText, m_scHotKey);
		}
		else
		{
			wsprintf(buffer, "%s", m_szText);
			TextOut(hdc, NextHelpDrawX, NextHelpDrawY, m_szText, lstrlen(m_szText));
		}

		GetTextExtentPoint32(hdc, buffer, strlen(buffer), &Size);
		NextHelpDrawY += (SHORT)Size.cy;	

		NextHelpDrawX = (rect1.right/2) - (MaxWidth/2);						

		if(m_ssMoney)
		{	
			//서피스에 곡물을 출력한다.			
			lpHelpBuffer->ReleaseDC(hdc);		//락을 걸기 위해 장치 디바이스를 잠시 해제한다.
				
			if(clGrp.LockSurface(lpHelpBuffer)==TRUE)
			{
				Draw_PutSpriteT(NextHelpDrawX, NextHelpDrawY, FILE_HELPRESOURCE, HELP_FONTNUM_CROP);
				clGrp.UnlockSurface(lpHelpBuffer);

				NextHelpDrawX += GetFileXsize(FILE_HELPRESOURCE);
			}
			
			lpHelpBuffer->GetDC(&hdc);			
			SelectObject(hdc, pGame->pBattle->HelpFont);
			SetBkColor( hdc, RGB( 0, 0, 255 ) );
			SetTextColor( hdc, RGB( 80, 80, 80 ) );
   			SetBkMode(hdc, TRANSPARENT);

			wsprintf(buffer, " %d ", m_ssMoney);
			TextOut(hdc, NextHelpDrawX, NextHelpDrawY, buffer, strlen(buffer));
			GetTextExtentPoint32(hdc, buffer, strlen(buffer), &Size);			
			NextHelpDrawX += (SHORT)Size.cx;
		}

		if(m_ssTree)
		{
			//서피스에 나무를 출력한다.
			lpHelpBuffer->ReleaseDC(hdc);		//락을 걸기 위해 장치 디바이스를 잠시 해제한다.
			
			if(clGrp.LockSurface(lpHelpBuffer)==TRUE)
			{
				Draw_PutSpriteT(NextHelpDrawX, NextHelpDrawY, FILE_HELPRESOURCE, HELP_FONTNUM_TREE);

				clGrp.UnlockSurface(lpHelpBuffer);

				NextHelpDrawX += GetFileXsize(FILE_HELPRESOURCE);
			}
			
			lpHelpBuffer->GetDC(&hdc);			
			SelectObject(hdc, pGame->pBattle->HelpFont);
			SetBkColor( hdc, RGB( 0, 0, 255 ) );
			SetTextColor( hdc, RGB( 80, 80, 80 ) );
   			SetBkMode(hdc, TRANSPARENT);

			wsprintf(buffer, " %d ", m_ssTree);
			TextOut(hdc, NextHelpDrawX, NextHelpDrawY, buffer, strlen(buffer));
			GetTextExtentPoint32(hdc, buffer, strlen(buffer), &Size);		
			NextHelpDrawX += (SHORT)Size.cx;
		}		

		if(m_ssMaintenance)
		{
			//서피스에 나무를 출력한다.
			lpHelpBuffer->ReleaseDC(hdc);		//락을 걸기 위해 장치 디바이스를 잠시 해제한다.
			
			if(clGrp.LockSurface(lpHelpBuffer)==TRUE)
			{
				Draw_PutSpriteT(NextHelpDrawX, NextHelpDrawY, FILE_HELPRESOURCE, HELP_FONTNUM_MAINTENANCE);
				clGrp.UnlockSurface(lpHelpBuffer);

				NextHelpDrawX += GetFileXsize(FILE_HELPRESOURCE);
			}
			
			lpHelpBuffer->GetDC(&hdc);			
			SelectObject(hdc, pGame->pBattle->HelpFont);
			SetBkColor( hdc, RGB( 0, 0, 255 ) );
			SetTextColor( hdc, RGB( 80, 80, 80 ) );
   			SetBkMode(hdc, TRANSPARENT);

			wsprintf(buffer, " %d ", m_ssMaintenance);
			TextOut(hdc, NextHelpDrawX, NextHelpDrawY, buffer, strlen(buffer));
			GetTextExtentPoint32(hdc, buffer, strlen(buffer), &Size);		
			NextHelpDrawX += (SHORT)Size.cx;
		}		

		if(m_ssMagicPower)
		{
			//서피스에 나무를 출력한다.
			lpHelpBuffer->ReleaseDC(hdc);		//락을 걸기 위해 장치 디바이스를 잠시 해제한다.
			
			if(clGrp.LockSurface(lpHelpBuffer)==TRUE)
			{
				Draw_PutSpriteT(NextHelpDrawX, NextHelpDrawY, FILE_HELPRESOURCE, HELP_FONTNUM_MAGICPOWER);
				clGrp.UnlockSurface(lpHelpBuffer);

				NextHelpDrawX += GetFileXsize(FILE_HELPRESOURCE);
			}
			
			lpHelpBuffer->GetDC(&hdc);			
			SelectObject(hdc, pGame->pBattle->HelpFont);
			SetBkColor( hdc, RGB( 0, 0, 255 ) );
			SetTextColor( hdc, RGB( 80, 80, 80 ) );
   			SetBkMode(hdc, TRANSPARENT);

			wsprintf(buffer, " %d ", m_ssMagicPower);
			TextOut(hdc, NextHelpDrawX, NextHelpDrawY, buffer, strlen(buffer));
			GetTextExtentPoint32(hdc, buffer, strlen(buffer), &Size);		
			NextHelpDrawX += (SHORT)Size.cx;
		}		

		NextHelpDrawY += TempHeight;
		NextHelpDrawX = (rect1.right/2) - (MaxWidth/2);								

		for(i=0; i < 3; i++)
		{
			if(m_bUsedInfo[i] == TRUE)
			{
				TextOut(hdc, NextHelpDrawX, NextHelpDrawY, m_szInfo[i], strlen(m_szInfo[i]));

				GetTextExtentPoint32(hdc, m_szInfo[i], strlen(m_szInfo[i]), &Size);
				NextHelpDrawY += (SHORT)Size.cy;
			}	
		}		

	    lpHelpBuffer->ReleaseDC(hdc);
	}


	//확장된 텍스트를 출력한다.
	PutExtendHelp(m_szExtendText);	

	rect.left		= m_ssHelpDrawX;
	rect.top		= m_ssHelpDrawY;
	rect.right		= m_ssHelpDrawX + rect1.right -1;
	rect.bottom		= m_ssHelpDrawY + rect1.bottom - 1;

	if(lpScreenBuffer->BltFast(rect.left, rect.top, lpHelpBuffer, &rect1, 
						DDBLTFAST_SRCCOLORKEY|DDBLTFAST_WAIT) != DD_OK)
	{
		//clGrp.Error("", "[%d] [%d] :: [%d] [%d] [%d] [%d]", rect.left, rect.top, 
		//													rect1.left, rect1.top, rect1.right, rect1.bottom);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID	_Help::PutExtendHelp(CHAR* lpExtendText)
/// 목적 : 확장 헬프를 출력한다.
////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	_Help::PutExtendHelp(CHAR* lpExtendText)
{
	SIZE	size;	
	HDC		hdc;
	static	DWORD	PrevPutFrame;	
	static	CHAR	szPrevPutText[1024];

	if(strlen(lpExtendText) != 0 && lpExtendText != NULL)
	{
		if(PrevPutFrame != GD.Frame || (!strcmp(szPrevPutText, lpExtendText) && PrevPutFrame == GD.Frame))
		{
			PrevPutFrame = GD.Frame;
			strcpy(szPrevPutText, lpExtendText);
			
			if(lpScreenBuffer->GetDC(&hdc) == DD_OK) 
			{
				SelectObject(hdc, pGame->pBattle->HelpFont);
				SetBkColor( hdc, RGB( 0, 0, 255 ) );

				SetTextColor( hdc, NORMAL_TEXT_COLOR );

   				SetBkMode(hdc, TRANSPARENT);

				//문자의 사이즈를 구한다.
				GetTextExtentPoint32(hdc, lpExtendText, strlen(lpExtendText), &size);					

				//세밀한 텍스트를 출력해 준다.
				TextOut(hdc, HELP_EXTEND_TEXT_X, HELP_EXTEND_TEXT_Y, lpExtendText, strlen(lpExtendText));		

				lpScreenBuffer->ReleaseDC(hdc);
			}
		}
	}
}
