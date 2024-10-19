//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------

/* 
	합본을 만들자(임.버 + 천.버 = 버튼)
	표시 목록
	1. 임진록 : 임진록에만 있는 부분
	2. 천  년 : 천년의 신화에만 있는 부분
*/

#include <GSL.h>

#include <main.h>
#include <Mouse.h>

#include <colortable.h>
#include <etc.h>
#include "..\Gersang\music.h"
#include "..\Gersang\ytitlefile.h"
#include "..\Gersang\yprg.h"
#include "..\Gersang\text.h"
#include <button.h>

extern	unsigned char	DarkTable[][256];		// 어두운 단계 
extern _InputDevice		IpD;
// 텍스트 
extern	_Text			Text;
extern	char			SavePath[];

#define		BUTTON_BACK_TEXT_COLOR				(RGB(1, 1, 1))
#define		BUTTON_FRONT_SELECTED_TEXT_COLOR	(RGB(206, 113, 14)) // 선택했을시에 나타나는 붉은색

#define		BUTTON_FRONT_UNSELECTED_TEXT_COLOR	(RGB(161, 161, 161))

#define		BUTTON_FRONT_UNSELECTED_TEXT_COLOR_PLUS	(RGB(0, 0, 0))	// 임진록2pLUS에서만 사용

#define		BUTTON_FRONT_ONMOUSE_TEXT_COLOR			 (RGB(219, 213, 200)) // 마우스가 위에 있을때
#define		BUTTON_FRONT_ONMOUSE_TEXT_COLOR_PLUS	 (RGB(100, 100, 100)) // 마우스가 위에 있을때

#define		BUTTON_UNACTION_TEXT_COLOR			(RGB(100, 100, 100))

#define		BOX_BACK_TEXT_COLOR					BUTTON_BACK_TEXT_COLOR
#define		BOX_FRONT_TEXT_COLOR				BUTTON_FRONT_UNSELECTED_TEXT_COLOR

#define		BUTTON_SELECT_BOX_COLOR				(COLOR_YELLOW)
#define		BUTTON_UNSELECT_BOX_COLOR			(255)
#define		BUTTON_DEFAULT_SELECTCOLOR			(248)

#define		BUTTON_DARK_STEP					5

BOOL	_SelectButton::m_PrevMousePressedTime;
BOOL	_SelectButton::m_IsMouseLeftButtonPressed;

//버튼들의 집합.
_Button		g_Button[GLOBAL_BUTTON_NUM];

//버튼들의 스프라이트.
XSPR		g_Buttons_Spr[TOTAL_BUTTONS_KINDOF_NUM];
XSPR		GOverButtonSpr;

//------------------------------------------------------------------------------------------------
/// _Button::_Button()
/// 목적 : 버튼을 생성한다.
//------------------------------------------------------------------------------------------------
_Button::_Button()
{
	m_x					= 0;
	m_y					= 0;
	m_Height			= 0;
	m_Width				= 0;
	m_bSoundPush		= FALSE;	
	m_bAction			= FALSE;	
	m_bProcess			= FALSE;
	MuiFlag				= 0;
	MbPrevFrameInButton	= FALSE;

	m_bVisibleWindow	= TRUE;

	ZeroMemory(m_MenuName, sizeof(m_MenuName));	
}

//------------------------------------------------------------------------------------------------
/// _Button::_Button(SHORT x, SHORT y, SHORT width, SHORT height, CHAR* lpMenuName, BOOL bAction)
/// 목적 : 주어진 인자 값으로 버튼을 생성한다.
//------------------------------------------------------------------------------------------------
_Button::_Button(SHORT x, SHORT y, SHORT width, SHORT height, CHAR* lpMenuName, BOOL bAction)
{
	Create(x, y, width, height, lpMenuName, bAction);
}

//------------------------------------------------------------------------------------------------
/// _Button::_Button(SHORT x, SHORT y, SHORT width, SHORT height, CHAR* lpMenuName, WORD flag, BOOL bAction)
/// 목적 : 주어진 인자 값으로 버튼을 생성한다.
//------------------------------------------------------------------------------------------------
_Button::_Button(SHORT x, SHORT y, SHORT width, SHORT height, CHAR* lpMenuName, WORD flag, BOOL bAction)
{
	Create(x, y, width, height, lpMenuName, flag, bAction);
}

//------------------------------------------------------------------------------------------------
// robypark 2004/11/15 14:6
// 이 버튼 윈도가 보이는지를 얻어온다.
//------------------------------------------------------------------------------------------------
BOOL	_Button::IsVisibleWindow(void)
{
	return m_bVisibleWindow;
}

//------------------------------------------------------------------------------------------------
// robypark 2004/11/15 14:6
// 이 버튼 윈도가 보이는지를 설정한다.
//------------------------------------------------------------------------------------------------
void	_Button::ShowWindow(BOOL bVisible)
{
	m_bVisibleWindow = bVisible;
}

//------------------------------------------------------------------------------------------------
/// void _Button::Create(INT x, INT y, INT Height, INT Width, CHAR* MenuName, BOOL bAction)
/// 목적 : 버튼을 생성한다.
//------------------------------------------------------------------------------------------------
void _Button::Create(INT x, INT y, INT Width, INT Height, CHAR* MenuName, BOOL bAction)
{	
	Create(x, y, Width, Height, MenuName, BUTTON_PUT_LEFT, bAction);	
}

//------------------------------------------------------------------------------------------------
/// void _Button::Create(INT x, INT y, INT Height, INT Width, CHAR* MenuName, WORD flag, BOOL bAction)
/// 목적 : 버튼을 생성한다.
//------------------------------------------------------------------------------------------------
void _Button::Create(INT x, INT y, INT Width, INT Height, CHAR* MenuName, WORD flag, BOOL bAction)
{				

	if(flag & BUTTON_PUT_LEFT)
	{
		m_x			=	x;
		m_y			=	y;
		m_Width		=	Width;
		m_Height	=	Height;
	}
	else if(flag & BUTTON_PUT_CENTER)
	{
		m_x			=	x - (Width / 2);
		m_y			=	y - (Height / 2);
		m_Width		=	Width;
		m_Height	=	Height;
	}	
	else
	{
		clGrp.Error("FKJE8567", "BUTTON000");
	}

	m_bAction			=	bAction;
	m_bProcess			=	TRUE;
	MuiFlag				=	flag;
	MbPrevFrameInButton	=	FALSE;

	m_bVisibleWindow	= TRUE;

	SetMenuName(MenuName);
}

VOID	_Button::SetX(SHORT NewX)
{
	m_x	=	NewX;
}

VOID	_Button::SetY(SHORT NewY)
{
	m_y	=	NewY;
}


VOID	_Button::SetMenuName(CHAR* lpNewMenuName)	
{
	if(lpNewMenuName)	strcpy(m_MenuName, lpNewMenuName);
	else				strcpy(m_MenuName, "");		
}

void	_Button::SetAction(BOOL bAction)
{
	m_bAction=bAction;
	
	if(bAction == TRUE)
	{
		SetProcess(TRUE);
	}
	else
	{
		SetProcess(FALSE);
	}
}

void	_Button::SetProcess(BOOL bProcess)
{
	m_bProcess=bProcess;
}

//------------------------------------------------------------------------------------------------
/// VOID _Button::Put(LPDIRECTDRAWSURFACE	lpSurface, DWORD PutFlag)
/// 목적 : 버튼의 모양을 찍는다.
//------------------------------------------------------------------------------------------------
VOID _Button::Put(LPDIRECTDRAWSURFACE7	lpSurface, DWORD PutFlag)
{	
	Put(m_x, m_y, lpSurface, PutFlag);
}

//------------------------------------------------------------------------------------------------
/// VOID _Button::Put(INT x, INT y, LPDIRECTDRAWSURFACE	lpSurface, DWORD PutFlag)
/// 목적 : 버튼의 모양을 찍는다. 찍을 좌표를 넘겨준다.
//------------------------------------------------------------------------------------------------
VOID _Button::Put(INT x, INT y, LPDIRECTDRAWSURFACE7	lpSurface, DWORD PutFlag)
{	
	INT			x1, y1;
	COLORREF	TextColor;		
	BOOL		IsMoveWindow;
	BOOL		IsDrawBorder;
	BOOL		IsEnableDarkText;

	// robypark 2004/11/15 14:8
	// 이 버튼 윈도가 보이지 않는 경우 리턴
	if (FALSE == m_bVisibleWindow)
		return;

	IsMoveWindow		=	(PutFlag & BUTTON_PUT_NOMOVE) ? FALSE : TRUE;	
	IsDrawBorder		=	(PutFlag & BUTTON_PUT_NOBORDER) ? FALSE : TRUE;
	IsEnableDarkText	=	(PutFlag & BUTTON_PUT_NODARK) ? FALSE : TRUE;

	if(m_bAction)
	{
		if(m_bProcess)
		{
			if(IsInMouse(x, y)==TRUE) 	
			{
				TextColor = BUTTON_FRONT_ONMOUSE_TEXT_COLOR;	

				if(IpD.LeftPressSwitch && IsMoveWindow)
				{
					x1=x+2;
					y1=y+2;
				}
				else
				{
					x1=x;
					y1=y;	
				}
			}
			else
			{
				x1=x;
				y1=y;		

				TextColor = BUTTON_FRONT_UNSELECTED_TEXT_COLOR;	
			}
		}
		else
		{
			x1=x;
			y1=y;		

			TextColor	= BUTTON_FRONT_UNSELECTED_TEXT_COLOR;
		}		
	}
	else				
	{		
		if(IsEnableDarkText == TRUE)
			TextColor = BUTTON_UNACTION_TEXT_COLOR;		
		else
			TextColor = BUTTON_FRONT_UNSELECTED_TEXT_COLOR;		

		x1=x;
		y1=y;		
	}

	//테두리(박스)를 그린다.
	if(IsDrawBorder == TRUE)
	{
		if(clGrp.LockSurface(lpSurface)==TRUE)
		{
			clGrp.BoxC(x1, y1, x1+m_Width, y1+m_Height, BUTTON_UNSELECT_BOX_COLOR);	
			clGrp.UnlockSurface(lpSurface);
		}	
	}
	
	PutText(x1, y1, lpSurface, m_MenuName, TextColor);	
}

//------------------------------------------------------------------------------------------------
/// VOID _Button::Put(HDC hdc, BYTE Color, DWORD PutFlag)
/// 목적 : 버튼의 모양을 찍는다.
//------------------------------------------------------------------------------------------------
VOID _Button::Put(LPDIRECTDRAWSURFACE7	lpSurface, BYTE Color, DWORD PutFlag)
{	
	Put(m_x, m_y, lpSurface, Color, PutFlag);
}

//------------------------------------------------------------------------------------------------
/// VOID _Button::Put(INT x, INT y, LPDIRECTDRAWSURFACE	lpSurface, BYTE Color, DWORD PutFlag)
/// 목적 : 버튼의 모양을 찍는다. 찍을 좌표를 넘겨준다.
//------------------------------------------------------------------------------------------------
VOID _Button::Put(INT x, INT y, LPDIRECTDRAWSURFACE7	lpSurface, BYTE Color, DWORD PutFlag)
{	
	INT			x1, y1;	
	COLORREF	TextColor;
	BOOL		IsMoveWindow;
	BOOL		IsDrawBorder;
	BOOL		IsEnableDarkText;
	
	// robypark 2004/11/15 14:8
	// 이 버튼 윈도가 보이지 않는 경우 리턴
	if (FALSE == m_bVisibleWindow)
		return;

	IsMoveWindow		=	(PutFlag & BUTTON_PUT_NOMOVE) ? FALSE : TRUE;			
	IsDrawBorder		=	(PutFlag & BUTTON_PUT_NOBORDER) ? FALSE : TRUE;
	IsEnableDarkText	=	(PutFlag & BUTTON_PUT_NODARK) ? FALSE : TRUE;

	if(m_bAction==TRUE && m_bProcess)
	{
		if(IsInMouse(x, y)==TRUE)	
		{
			TextColor = BUTTON_FRONT_ONMOUSE_TEXT_COLOR;		

			if(IpD.LeftPressSwitch && IsMoveWindow)
			{
				x1=x+2;
				y1=y+2;	
			}
			else
			{
				x1=x;
				y1=y;	
			}
		}
		else
		{		
			TextColor = BUTTON_FRONT_UNSELECTED_TEXT_COLOR;		

			x1=x;
			y1=y;		
		}		
	}
	else
	{
		if(IsEnableDarkText == TRUE)
			TextColor = BUTTON_UNACTION_TEXT_COLOR;		
		else
			TextColor = BUTTON_FRONT_UNSELECTED_TEXT_COLOR;		

		x1=x;
		y1=y;		
	}
	

	//박스를 그린다. 
	//잠시 hdc를 푼다.
	//그리고 락을 건다.		
	if(clGrp.LockSurface(lpSurface)==TRUE)
	{		
		clGrp.FillBoxC(x1+1, y1+1, x1+m_Width-1, y1+m_Height-1, (UI08)Color);	

		if(IsDrawBorder == TRUE)
			clGrp.BoxC(x1, y1, x1+m_Width, y1+m_Height, BUTTON_UNSELECT_BOX_COLOR);	
		clGrp.UnlockSurface(lpSurface);
	}	
	
	PutText(x1, y1, lpSurface, m_MenuName, TextColor);
}

//------------------------------------------------------------------------------------------------
/// VOID _Button::Put(SHORT x, SHORT y, XSPR* lpXspr, SHORT FontNumber)
/// 목적 : x, y의 위치에 텍스트를 출력한다.
//------------------------------------------------------------------------------------------------
VOID _Button::Put(SHORT x, SHORT y, XSPR* lpXspr, SHORT FontNumber)
{		
	if(m_bAction==TRUE)	PutSpriteButton(x, y, lpXspr, FontNumber, FALSE, FALSE);	
	else				PutSpriteButton(x, y, lpXspr, FontNumber, TRUE, FALSE);		
}

//------------------------------------------------------------------------------------------------
/// VOID _Button::Put(XSPR* lpXspr, SHORT FontNumber)
/// 목적 : x, y의 위치에 텍스트를 출력한다.
//------------------------------------------------------------------------------------------------
VOID _Button::Put(XSPR* lpXspr, SHORT FontNumber)
{	
	SHORT	sx, sy;	

	// robypark 2004/11/15 14:8
	// 이 버튼 윈도가 보이지 않는 경우 리턴
	if (FALSE == m_bVisibleWindow)
		return;
	
	//왼쪽 버튼이 눌러져 있고.
	if(IpD.LeftPressSwitch  == TRUE)
	{	
		sx = m_x + 2;
		sy = m_y + 2;
	}
	else
	{			
		sx = m_x;
		sy = m_y;
	}
	
	clGrp.FillBox((m_x + 3), (m_y + 3), (m_x + 3) + m_Width - 1, (m_y + 3) + m_Height - 1, 254, 254, 254);

	if(m_bAction == TRUE)
	{
		if(m_bProcess == TRUE)
		{
			if(IsInMouse() == TRUE)			PutSpriteButton(sx, sy, lpXspr, FontNumber, FALSE, FALSE);									
			else							PutSpriteButton(m_x, m_y, lpXspr, FontNumber, FALSE, FALSE);						
		}
		else
		{
			PutSpriteButton(m_x, m_y, lpXspr, FontNumber, FALSE, FALSE);			
		}
	}
	else
	{
		PutSpriteButton(m_x, m_y, lpXspr, FontNumber, TRUE, FALSE);			
	}	
}

//------------------------------------------------------------------------------------------------
/// void _Button::Put(SHORT x, SHORT y, XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag)
/// 목적 : 기본적인 위치에 스프라이트를 출력한다. 
//------------------------------------------------------------------------------------------------
VOID	_Button::Put1(XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag)
{
	Put1(m_x, m_y, lpXspr, DefaultFontNum, SelectedFontNum, OnMouseFontNum, PutFlag);
}

//------------------------------------------------------------------------------------------------
/// void _Button::Put(SHORT x, SHORT y, XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag)
/// 목적 : 기본적인 위치에 스프라이트를 출력한다. 
//------------------------------------------------------------------------------------------------
VOID	_Button::Put(XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag)
{
	Put(m_x, m_y, lpXspr, DefaultFontNum, SelectedFontNum, OnMouseFontNum, PutFlag);
}

//------------------------------------------------------------------------------------------------
/// void _Button::Put(SHORT x, SHORT y, XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag)
/// 목적 : x, y의 위치에 스프라이트를 출력한다. 
//------------------------------------------------------------------------------------------------
void _Button::Put1(SHORT x, SHORT y, XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag)
{
	INT			x1, y1;		
	BOOL		IsMoveWindow;	
	SHORT		FontNumber;
	
	// robypark 2004/11/15 14:8
	// 이 버튼 윈도가 보이지 않는 경우 리턴
	if (FALSE == m_bVisibleWindow)
		return;

	IsMoveWindow = (PutFlag & BUTTON_PUT_NOMOVE) ? FALSE : TRUE;				
	
	x1 = x;
	y1 = y;
	
	if(m_bAction == TRUE)
	{
		if(m_bProcess == TRUE)
		{
			if(MbPrevFrameInButton == TRUE && (m_bProcess == TRUE))
			{			
				if(IpD.LeftPressSwitch == TRUE)			
				{
					if(IsMoveWindow == TRUE)
					{
						x1 = x+2;
						y1 = y+2;
					}			

					FontNumber = SelectedFontNum;						
				}
				else							
					FontNumber = OnMouseFontNum;			
			}
			else					
				FontNumber = DefaultFontNum;		
		}
		else
			FontNumber	= DefaultFontNum;

		PutSpriteButton(x1, y1, lpXspr, FontNumber, FALSE, FALSE);

	}
	else			
	{
		FontNumber = DefaultFontNum;

		if(BUTTON_PUT_NODARK & PutFlag)		PutSpriteButton(x1, y1, lpXspr, FontNumber, FALSE, FALSE);					
		else								PutSpriteButton(x1, y1, lpXspr, FontNumber, TRUE, FALSE);
	}	
}

//------------------------------------------------------------------------------------------------
/// void _Button::Put(SHORT x, SHORT y, XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag)
/// 목적 : x, y의 위치에 스프라이트를 출력한다. 
//------------------------------------------------------------------------------------------------
void _Button::Put(SHORT x, SHORT y, XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag)
{
	INT			x1, y1;		
	BOOL		IsMoveWindow;	
	SHORT		FontNumber;
	
	// robypark 2004/11/15 14:8
	// 이 버튼 윈도가 보이지 않는 경우 리턴
	if (FALSE == m_bVisibleWindow)
		return;

	IsMoveWindow = (PutFlag & BUTTON_PUT_NOMOVE) ? FALSE : TRUE;				
	
	x1 = x;
	y1 = y;
	
	if(m_bAction == TRUE)
	{
		if(m_bProcess == TRUE)
		{
			if(IsInMouse(x, y) == TRUE && (m_bProcess == TRUE))
			{			
				if(IpD.LeftPressSwitch == TRUE)			
				{
					if(IsMoveWindow == TRUE)
					{
						x1 = x+2;
						y1 = y+2;
					}			

					FontNumber = SelectedFontNum;						
				}
				else							
					FontNumber = OnMouseFontNum;			
			}
			else					
				FontNumber = DefaultFontNum;		
		}
		else
			FontNumber	= DefaultFontNum;

		PutSpriteButton(x1, y1, lpXspr, FontNumber, FALSE, FALSE);

	}
	else			
	{
		FontNumber = DefaultFontNum;

		if(BUTTON_PUT_NODARK & PutFlag)		PutSpriteButton(x1, y1, lpXspr, FontNumber, FALSE, FALSE);					
		else								PutSpriteButton(x1, y1, lpXspr, FontNumber, TRUE, FALSE);
	}	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	버튼을 출력한다 (반투명 해서 출력하는 기능이 포함되었다.)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	_Button::Put(XSPR* lpXspr, SI16 siDefaultFontNum, SI16 siSelectedFontNum, SI16 siOnMouseFontNum, BOOL bDefaultFontTrans, BOOL bSelectedFontTrans, BOOL bOnMouseTrans, DWORD PutFlag)
{
	Put(m_x, m_y, lpXspr, siDefaultFontNum, siSelectedFontNum, siOnMouseFontNum, bDefaultFontTrans, bSelectedFontTrans, bOnMouseTrans, PutFlag);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	버튼을 출력한다 (반투명 해서 출력하는 기능이 포함되었다.)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	_Button::Put(SI16 siX, SI16 siY, XSPR* lpXspr, SI16 siDefaultFontNum, SI16 siSelectedFontNum, SI16 siOnMouseFontNum, BOOL bDefaultFontTrans, BOOL bSelectedFontTrans, BOOL bOnMouseTrans, DWORD PutFlag)
{
	INT			x1, y1;		
	BOOL		IsMoveWindow;	
	BOOL		bTrans = FALSE;
	SHORT		FontNumber;
	
	// robypark 2004/11/15 14:8
	// 이 버튼 윈도가 보이지 않는 경우 리턴
	if (FALSE == m_bVisibleWindow)
		return;

	IsMoveWindow = (PutFlag & BUTTON_PUT_NOMOVE) ? FALSE : TRUE;				
	
	x1 = siX;
	y1 = siY;
	
	if(m_bAction == TRUE)
	{
		if(m_bProcess == TRUE)
		{
			if(IsInMouse(siX, siY) == TRUE && (m_bProcess == TRUE))
			{			
				if(IpD.LeftPressSwitch == TRUE)			
				{
					if(IsMoveWindow == TRUE)
					{
						x1 = siX + 2;
						y1 = siY +2;
					}			

					FontNumber	=	siSelectedFontNum;						
					bTrans		=	bSelectedFontTrans;
				}
				else							
				{
					FontNumber	=	siOnMouseFontNum;			
					bTrans		=	bOnMouseTrans;
				}
			}
			else					
			{
				FontNumber	=	siDefaultFontNum;	
				bTrans		=	bDefaultFontTrans;
			}			
		}
		else
		{
			FontNumber	=	siDefaultFontNum;
			bTrans		=	bDefaultFontTrans;
		}

		PutSpriteButton(x1, y1, lpXspr, FontNumber, FALSE, bTrans);
	}
	else			
	{
		FontNumber	=	siDefaultFontNum;
		bTrans		=	bDefaultFontTrans;

		if(BUTTON_PUT_NODARK & PutFlag)		PutSpriteButton(x1, y1, lpXspr, FontNumber, FALSE, bTrans);					
		else								PutSpriteButton(x1, y1, lpXspr, FontNumber, TRUE, bTrans);
	}	
}											

//------------------------------------------------------------------------------------------------
//	설명	:
//------------------------------------------------------------------------------------------------
// 임진록
VOID	_Button::Put2(XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, SHORT NoActionFontNum, DWORD PutFlag)
{
	INT			x1, y1;		
	BOOL		IsMoveWindow;	
	SHORT		FontNumber;
	
	// robypark 2004/11/15 14:8
	// 이 버튼 윈도가 보이지 않는 경우 리턴
	if (FALSE == m_bVisibleWindow)
		return;

	IsMoveWindow = (PutFlag & BUTTON_PUT_NOMOVE) ? FALSE : TRUE;				
	
	x1 = m_x;
	y1 = m_y;
	
	if(m_bAction == TRUE)
	{
		if(m_bProcess == TRUE)
		{
			if(IsInMouse(m_x, m_y) == TRUE && (m_bProcess == TRUE))
			{			
				if(IpD.LeftPressSwitch == TRUE)			
				{
					if(IsMoveWindow == TRUE)
					{
						x1 = m_x+2;
						y1 = m_y+2;
					}			

					FontNumber = SelectedFontNum;						
				}
				else							
					FontNumber = OnMouseFontNum;			
			}
			else					
				FontNumber = DefaultFontNum;		
		}
		else
			FontNumber	= DefaultFontNum;


		PutSpriteButton(x1, y1, lpXspr, FontNumber, FALSE, FALSE);		
	}
	else			
	{
		if(BUTTON_PUT_NODARK & PutFlag)
		{
			FontNumber = DefaultFontNum;
			PutSpriteButton(x1, y1, lpXspr, FontNumber, FALSE, FALSE);					
		}
		else
		{
			FontNumber = NoActionFontNum;
			PutSpriteButton(x1, y1, lpXspr, FontNumber, FALSE, FALSE);			
		}
	}	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	스프라이트를 이용하여 버튼을 찍는다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	_Button::PutSpriteButton(SI16 siX, SI16 siY, XSPR *pSpr, SI16 siFont, BOOL bDark, BOOL bTrans)
{
	// robypark 2004/11/15 14:8
	// 이 버튼 윈도가 보이지 않는 경우 리턴
	if (FALSE == m_bVisibleWindow)
		return;

	switch(siFont)
	{
	case -1:			// 버튼을 찍지 않는다.
		break;
	default:			// 버튼을 찍는다.
		if(bTrans == TRUE)
		{
			if(bDark == TRUE)
			{
				// 어둡게 찍는다. (아직 지원되지 않는다.)
				clGrp.PutSpriteLightCT( siX, siY, pSpr->Header.Xsize, pSpr->Header.Ysize, 
						&pSpr->Image[pSpr->Header.Start[siFont]]);				
			}
			else
			{
				// 보통으로 찍는다.
				clGrp.PutSpriteLightCT( siX, siY, pSpr->Header.Xsize, pSpr->Header.Ysize, 
						&pSpr->Image[pSpr->Header.Start[siFont]]);				
			}
		}
		else
		{
			if(bDark == TRUE)
			{
				clGrp.PutSpriteT(siX, siY, pSpr->Header.Xsize, pSpr->Header.Ysize, 
						&pSpr->Image[pSpr->Header.Start[siFont]], DarkTable[min(MAX_DARK_STEP,BUTTON_DARK_STEP)]);				
			}
			else
			{
				clGrp.PutSpriteT(siX, siY, pSpr->Header.Xsize, pSpr->Header.Ysize, 
						&pSpr->Image[pSpr->Header.Start[siFont]]);				
			}
		}		
	}
}

//------------------------------------------------------------------------------------------------
//	설명	:	버튼 출력함수.
//				hdc를 이용한다 그러므로 글자가 버튼을 넘어서도 잘리지 않는다.
//				박스도 칠 수 없다. 단 속도는 빠르다.
//	hdc		:	출력할 HDC.
//	PutFlag	:	출력 속성.
//------------------------------------------------------------------------------------------------
VOID	_Button::Put1(HDC hdc, DWORD PutFlag)
{
	Put1(hdc, m_x, m_y, PutFlag);
}

//------------------------------------------------------------------------------------------------
//	설명	:	버튼 출력함수.
//				hdc를 이용한다 그러므로 글자가 버튼을 넘어서도 잘리지 않는다.
//				박스도 칠 수 없다. 단 속도는 빠르다.
//	hdc		:	출력할 HDC.
//	PutFlag	:	출력 속성.
//------------------------------------------------------------------------------------------------
VOID	_Button::Put(HDC hdc, DWORD PutFlag,  COLORREF OffTextColor, COLORREF OnTextColor)
{
	Put(hdc, m_x, m_y, OffTextColor, OnTextColor, PutFlag);
}

//------------------------------------------------------------------------------------------------
//	설명	:	버튼 출력함수.
//				hdc를 이용한다 그러므로 글자가 버튼을 넘어서도 잘리지 않는다.
//				박스도 칠 수 없다. 단 속도는 빠르다.
//	hdc		:	출력할 HDC.
//	x, y	:	출력 위치.
//	PutFlag	:	출력 속성.
//------------------------------------------------------------------------------------------------
VOID	_Button::Put(HDC hdc, SHORT x, SHORT y, COLORREF OffTextColor, COLORREF OnTextColor, DWORD PutFlag)
{
	INT			x1, y1;
	COLORREF	TempTextColor;		
	BOOL		IsMoveWindow;	
	BOOL		IsEnableDarkText;

	// robypark 2004/11/15 14:8
	// 이 버튼 윈도가 보이지 않는 경우 리턴
	if (FALSE == m_bVisibleWindow)
		return;

	IsMoveWindow		=	(PutFlag & BUTTON_PUT_NOMOVE) ? FALSE : TRUE;		
	IsEnableDarkText	=	(PutFlag & BUTTON_PUT_NODARK) ? FALSE : TRUE;

	if(m_bAction)
	{
		if(m_bProcess)
		{
			if(IsInMouse(x, y)==TRUE) 	
			{
				TempTextColor = OnTextColor;	

				if(IpD.LeftPressSwitch && IsMoveWindow)
				{
					x1 = x + 2;
					y1 = y + 2;
				}
				else
				{
					x1 = x;
					y1 = y;	
				}
			}
			else
			{
				x1 = x;
				y1 = y;		

				TempTextColor = OffTextColor;	
			}
		}
		else
		{
			x1 = x;
			y1 = y;		

			TempTextColor	= BUTTON_FRONT_UNSELECTED_TEXT_COLOR;
		}		
	}
	else				
	{		
		if(IsEnableDarkText == TRUE)
			TempTextColor = BUTTON_UNACTION_TEXT_COLOR;		
		else
			TempTextColor = BUTTON_FRONT_UNSELECTED_TEXT_COLOR;		

		x1=x;
		y1=y;		
	}

	PutText(x1, y1, hdc, m_MenuName, TempTextColor);		
}

//------------------------------------------------------------------------------------------------
//	설명	:	버튼 출력함수.
//				hdc를 이용한다 그러므로 글자가 버튼을 넘어서도 잘리지 않는다.
//				박스도 칠 수 없다. 단 속도는 빠르다.
//	hdc		:	출력할 HDC.
//	x, y	:	출력 위치.
//	PutFlag	:	출력 속성.
//------------------------------------------------------------------------------------------------
VOID	_Button::Put1(HDC hdc, SHORT x, SHORT y, DWORD PutFlag)
{
	INT			x1, y1;
	COLORREF	TextColor;		
	BOOL		IsMoveWindow;	
	BOOL		IsEnableDarkText;

	// robypark 2004/11/15 14:8
	// 이 버튼 윈도가 보이지 않는 경우 리턴
	if (FALSE == m_bVisibleWindow)
		return;

	IsMoveWindow		=	(PutFlag & BUTTON_PUT_NOMOVE) ? FALSE : TRUE;		
	IsEnableDarkText	=	(PutFlag & BUTTON_PUT_NODARK) ? FALSE : TRUE;

	if(m_bAction)
	{
		if(m_bProcess)
		{
			if(MbPrevFrameInButton==TRUE) 	
			{
				TextColor = BUTTON_FRONT_ONMOUSE_TEXT_COLOR;	

				if(IpD.LeftPressSwitch && IsMoveWindow)
				{
					x1 = x + 2;
					y1 = y + 2;
				}
				else
				{
					x1 = x;
					y1 = y;	
				}
			}
			else
			{
				x1 = x;
				y1 = y;		

				TextColor = BUTTON_FRONT_UNSELECTED_TEXT_COLOR;	
			}
		}
		else
		{
			x1 = x;
			y1 = y;		

			TextColor	= BUTTON_FRONT_UNSELECTED_TEXT_COLOR;
		}		
	}
	else				
	{		
		if(IsEnableDarkText == TRUE)
			TextColor = BUTTON_UNACTION_TEXT_COLOR;		
		else
			TextColor = BUTTON_FRONT_UNSELECTED_TEXT_COLOR;		

		x1=x;
		y1=y;		
	}

	PutText(x1, y1, hdc, m_MenuName, TextColor);		
}

//------------------------------------------------------------------------------------------------
//	설명	: 
//------------------------------------------------------------------------------------------------
// 임진록
VOID	_Button::Put(HDC hdc, SI16 siX, SI16 siY, SI16 siWidth, SI16 siHeight, DWORD PutFlag)
{
	INT			x1, y1;
	COLORREF	TextColor;		
	BOOL		IsMoveWindow;	
	BOOL		IsEnableDarkText;

	// robypark 2004/11/15 14:8
	// 이 버튼 윈도가 보이지 않는 경우 리턴
	if (FALSE == m_bVisibleWindow)
		return;

	IsMoveWindow		=	(PutFlag & BUTTON_PUT_NOMOVE) ? FALSE : TRUE;		
	IsEnableDarkText	=	(PutFlag & BUTTON_PUT_NODARK) ? FALSE : TRUE;

	if(m_bAction)
	{
		if(m_bProcess)
		{
			if(IsInMouse()==TRUE) 	
			{
				TextColor = BUTTON_FRONT_ONMOUSE_TEXT_COLOR;	

				if(IpD.LeftPressSwitch && IsMoveWindow)
				{
					x1 = siX + 2;
					y1 = siY + 2;
				}
				else
				{
					x1 = siX;
					y1 = siY;	
				}
			}
			else
			{
				x1 = siX;
				y1 = siY;		

				TextColor = BUTTON_FRONT_UNSELECTED_TEXT_COLOR;	
			}
		}
		else
		{
			x1 = siX;
			y1 = siY;		

			TextColor	= BUTTON_FRONT_UNSELECTED_TEXT_COLOR;
		}		
	}
	else				
	{		
		if(IsEnableDarkText == TRUE)
			TextColor = BUTTON_UNACTION_TEXT_COLOR;		
		else
			TextColor = BUTTON_FRONT_UNSELECTED_TEXT_COLOR;		

		x1=siX;
		y1=siY;		
	}
	
	PutText(x1, y1, siWidth, siHeight, hdc, m_MenuName, TextColor);
}

//------------------------------------------------------------------------------------------------
/// VOID	_Button::PutText(INT x, INT y, LPDIRECTDRAWSURFACE	lpSurface, CHAR *lpText, COLORREF TextColor, WORD TextArrange)
/// 목적 : x, y의 위치에 텍스트를 출력한다.
//------------------------------------------------------------------------------------------------
VOID	_Button::PutText(INT x, INT y, LPDIRECTDRAWSURFACE7	lpSurface, CHAR *lpText, COLORREF TextColor, WORD TextArrange)
{
	PutText(x, y, m_Width, m_Height, lpSurface, lpText, TextColor, TextArrange);
}

//------------------------------------------------------------------------------------------------
/// VOID	_Button::PutText(INT x, INT y, SHORT Width, SHORT Height, LPDIRECTDRAWSURFACE lpSurface, CHAR *lpText, COLORREF TextColor, WORD TextArrange)
/// 목적 : x, y의 위치에 텍스트를 출력한다.
//------------------------------------------------------------------------------------------------
VOID	_Button::PutText(INT x, INT y, SHORT Width, SHORT Height, LPDIRECTDRAWSURFACE7 lpSurface, CHAR *lpText, COLORREF TextColor, WORD TextArrange)
{
	SHORT	nStringHeight;
	SHORT	nStringWidth;
	RECT	rect;
	SHORT	nx, ny;

	if(lpText == NULL)
		return;	
	
	PutTextToTextBufferOneLine(lpText, &nStringWidth, &nStringHeight, TextColor);

	switch(TextArrange)
	{
	case BUTTON_PUT_LEFT:		
		nx	=	x;
		ny	=	max(y, y + ((Height / 2) - (nStringHeight / 2)));
		break;
	case BUTTON_PUT_CENTER:
	case BUTTON_PUT_DEFAULT:
		nx	=   max(x, x + ((Width / 2) - (nStringWidth / 2)));
		ny	=	max(y, y + ((Height / 2) - (nStringHeight / 2)));		
		break;
	}		

	rect.left	=	0;
	rect.top	=	0;
	rect.right	=	rect.left + min(Width, nStringWidth);
	rect.bottom	=	rect.top  + min(Height, nStringHeight);			

	lpSurface->BltFast(nx, ny, lpBriefingTextBuffer, &rect, DDBLTFAST_SRCCOLORKEY|DDBLTFAST_WAIT);		
}

//------------------------------------------------------------------------------------------------
//	설명			:	버튼의 텍스트를 출력한다.
//						hdc를 사용하므로 버튼을 넘더라도 잘리지는 않는다.
//	x, y			:	버튼을 출력할 좌표.
//	hdc				:	버튼을 출력할 hdc.
//	TextColor		:	텍스트의 색.
//	TextArrange		:	텍스트 정렬 방식.
//------------------------------------------------------------------------------------------------
VOID	_Button::PutText(INT x, INT y, HDC hdc, CHAR *lpText, COLORREF TextColor, WORD TextArrange)
{
	PutText(x, y, m_Width, m_Height, hdc, lpText, TextColor, TextArrange);
}


//------------------------------------------------------------------------------------------------
//	설명			:	버튼의 텍스트를 출력한다.
//						hdc를 사용하므로 버튼을 넘더라도 잘리지는 않는다.
//	x, y			:	버튼을 출력할 좌표.
//	hdc				:	버튼을 출력할 hdc.
//	TextColor		:	텍스트의 색.
//	TextArrange		:	텍스트 정렬 방식.
//------------------------------------------------------------------------------------------------
VOID	_Button::PutTextPlus(INT x, INT y, HDC hdc, CHAR *lpText, COLORREF TextColor, WORD TextArrange)
{
	PutTextPlus(x, y, m_Width, m_Height, hdc, lpText, TextColor, TextArrange);
}
//------------------------------------------------------------------------------------------------
//	설명			:	버튼의 텍스트를 출력한다.
//						hdc를 사용하므로 버튼을 넘더라도 잘리지는 않는다.
//	x, y			:	버튼을 출력할 좌표.
//	Width, Height	:	버튼의 크기.
//	hdc				:	버튼을 출력할 hdc.
//	TextColor		:	텍스트의 색.
//	TextArrange		:	텍스트 정렬 방식.
//------------------------------------------------------------------------------------------------
VOID	_Button::PutText(INT x, INT y, INT Width, INT Height, HDC hdc, CHAR *lpText, COLORREF TextColor, WORD TextArrange)
{	
	SHORT		nx, ny;
	SIZE		size;
	COLORREF	OldTextColor;	

	if(!lpText)	
		return;	
		
	GetTextExtentPoint32(hdc, lpText, strlen(lpText), &size);

	// 그림자의높이까지 처리해준다.
	size.cy	+=	1;

	if(x != -1 || y != -1)
	{
		switch(TextArrange)
		{
		case BUTTON_PUT_LEFT:		
			nx	=	x;
			ny	=	max(y, y + ((Height / 2) - (size.cy / 2)));
			break;
		case BUTTON_PUT_CENTER:
		case BUTTON_PUT_DEFAULT:
			nx	=   max(x, x + ((Width / 2) - (size.cx / 2)));
			ny	=	max(y, y + ((Height / 2) - (size.cy / 2)));		
			break;
		}		
	}
	else
	{
		// m_TextPutX과 m_TextPutY가 정의 되어 있다면
		// 그 위치에 텍스트를 출력하고
		// 그렇지 않다면 가운데 정렬되게 텍스트를 출력한다.						
		nx			=	max(m_x, m_x + ((Width / 2) - (size.cx / 2)));
		ny			=	max(m_y, m_y + ((Height / 2) - (size.cy / 2)));
	}

	OldTextColor	=	SetTextColor(hdc, RGB(1, 1, 1));	
	TextOut(hdc, nx - 1, ny - 1, lpText, strlen(lpText));

	SetTextColor(hdc, TextColor);
	TextOut(hdc, nx, ny, lpText, strlen(lpText));	

	SetTextColor(hdc, OldTextColor);
}


//------------------------------------------------------------------------------------------------
//	설명			:	버튼의 텍스트를 출력한다.
//						hdc를 사용하므로 버튼을 넘더라도 잘리지는 않는다.
//	x, y			:	버튼을 출력할 좌표.
//	Width, Height	:	버튼의 크기.
//	hdc				:	버튼을 출력할 hdc.
//	TextColor		:	텍스트의 색.
//	TextArrange		:	텍스트 정렬 방식.
//------------------------------------------------------------------------------------------------
VOID	_Button::PutTextPlus(INT x, INT y, INT Width, INT Height, HDC hdc, CHAR *lpText, COLORREF TextColor, WORD TextArrange)
{	
	SHORT		nx, ny;
	SIZE		size;

	if(!lpText)	
		return;	
		
	GetTextExtentPoint32(hdc, lpText, strlen(lpText), &size);

	// 그림자의높이까지 처리해준다.
	size.cy	+=	1;

	if(x != -1 || y != -1)
	{
		switch(TextArrange)
		{
		case BUTTON_PUT_LEFT:		
			nx	=	x + 20;
			ny	=	max(y, y + ((Height / 2) - (size.cy / 2)));
			break;
		case BUTTON_PUT_CENTER:
		case BUTTON_PUT_DEFAULT:
			nx	=	x + 20;
			ny	=	max(y, y + ((Height / 2) - (size.cy / 2)));
			break;
		}		
	}
	else
	{
		// m_TextPutX과 m_TextPutY가 정의 되어 있다면
		// 그 위치에 텍스트를 출력하고
		// 그렇지 않다면 가운데 정렬되게 텍스트를 출력한다.						
		nx	=	x + 20;
		ny	=	max(y, y + ((Height / 2) - (size.cy / 2)));
	}

	SetTextColor(hdc, TextColor);
	TextOut(hdc, nx, ny, lpText, strlen(lpText));	

}

//------------------------------------------------------------------------------------------------
/// BOOL _Button::Process(BOOL bPush)
/// 목적 : 버튼의 기본적인 일들을 처리한다. 현재의 마우스 상태를 보여준다.
/// 리턴값 : 버튼이 눌리어졌나.
//------------------------------------------------------------------------------------------------
BOOL _Button::Process(BOOL bPush)
{	
	return Process(m_x, m_y, bPush);
}

//------------------------------------------------------------------------------------------------
/// BOOL _Button::Process(INT x, INT y, BOOL bPush)
/// 목적 : 버튼의 기본적인 일들을 처리한다. 현재의 마우스 상태와 시작 위치를 보여준다.
/// 리턴값 : 버튼이 눌리어졌나.
//------------------------------------------------------------------------------------------------
BOOL _Button::Process(INT x, INT y, BOOL bPush)
{
	// robypark 2004/11/15 14:8
	// 이 버튼 윈도가 보이지 않는 경우 리턴 FALSE
	if (FALSE == m_bVisibleWindow)
		return FALSE;

	if(m_bAction == TRUE && m_bProcess == TRUE )
	{
		if(IsInMouse(x, y)==TRUE)
		{	
			if((MbPrevFrameInButton == FALSE) && (MuiFlag & BUTTON_PLAYSOUND_ONMOUSE))
			{
				PushEffect(EFFECT_SELECTCOUNTRY, 0, 0);
			}

			if(IpD.LeftPressSwitch == FALSE)
			{
				//전에 버튼이 눌러져 있었는데 버튼을 뗀 상태라면
				//버튼을 선택했다는 것이다.
				if(bPush==TRUE)
				{					
					bPush=FALSE;
					m_bSoundPush=FALSE;
					return TRUE;
				}
			}
			else
			{
				if(m_bSoundPush==FALSE)
				{
					PushEffect(EFFECT_TITLEBUTTON, 0, 0); 
					m_bSoundPush=TRUE;
				}			
			}

			MbPrevFrameInButton	=	TRUE;
		}
		else
		{
			m_bSoundPush		=	FALSE;
			MbPrevFrameInButton	=	FALSE;
		}
	}
	
	return FALSE;
}
/*
//------------------------------------------------------------------------------------------------
/// BOOL	_Button::IsInMouse()
/// 목적 :	마우스가 영역 안에 있는지검사한다.
//------------------------------------------------------------------------------------------------
BOOL	_Button::IsInMouse()
{
	if(((m_x < Mouse_X) && (m_x+m_Width) > Mouse_X) && ((m_y < Mouse_Y) && (m_y+m_Height) > Mouse_Y))
		return TRUE;
	else
		return FALSE;
}

//------------------------------------------------------------------------------------------------
/// BOOL	_Button::IsInMouse(INT x, INT y)
/// 목적 :	마우스가 영역 안에 있는지검사한다.
//------------------------------------------------------------------------------------------------
BOOL	_Button::IsInMouse(INT x, INT y)
{
	if(((x < Mouse_X) && (x+m_Width) > Mouse_X) && ((y < Mouse_Y) && (y+m_Height) > Mouse_Y))
		return TRUE;
	else
		return FALSE;
}
*/

//------------------------------------------------------------------------------------------------
/// BOOL	_Button::IsInMouse()
/// 목적 :	마우스가 영역 안에 있는지검사한다.
//------------------------------------------------------------------------------------------------
BOOL	_Button::IsInMouse()
{
	return IsInMouse(m_x, m_y);
}

//------------------------------------------------------------------------------------------------
/// BOOL	_Button::IsInMouse(INT x, INT y)
/// 목적 :	마우스가 영역 안에 있는지검사한다.
//------------------------------------------------------------------------------------------------
BOOL	_Button::IsInMouse(INT x, INT y)
{
	if (FALSE == m_bVisibleWindow)
		return FALSE;

	if(((x <= IpD.Mouse_X) && (x + m_Width - 1) >= IpD.Mouse_X) && ((y <= IpD.Mouse_Y) && (y + m_Height - 1) >= IpD.Mouse_Y))
		return TRUE;
	else
		return FALSE;
}

//------------------------------------------------------------------------------------------------
/// RECT	_Button::GetSize()
/// 목적 :	마우스가 영역 안에 있는지검사한다.
//------------------------------------------------------------------------------------------------
RECT	_Button::GetSize()
{
	RECT	rect;

	rect.left	= m_x;
	rect.top	= m_y;
	rect.right  = m_x + m_Width;
	rect.bottom = m_y + m_Height;

	return rect;
}

//천년의 신화에만 있는 함수(2개)
//------------------------------------------------------------------------------------------------
//	설명	:	현재 버튼을 누르고 있는 중인지 알아온다.
//	bPush	:	현재 마우스 버튼의 상태.
//	리턴	:	만약 누르고 있으면 TRUE. 그렇지 않으면 FALSE.
//------------------------------------------------------------------------------------------------
//천년
BOOL	_Button::IsPressing(BOOL bPush)
{
	return IsPressing(m_x, m_y, bPush);
}

//------------------------------------------------------------------------------------------------
//	설명	:	현재 버튼을 누르고 있는 중인지 알아온다.
//	x, y	:	버튼의 좌표.
//	bPush	:	현재 마우스 버튼의 상태.
//	리턴	:	만약 누르고 있으면 TRUE. 그렇지 않으면 FALSE.
//------------------------------------------------------------------------------------------------
//천년
BOOL	_Button::IsPressing(INT x, INT y, BOOL bPush)
{
	if(IsInMouse(x, y) && bPush && m_bAction && m_bProcess)	
		return TRUE;	
	else
		return FALSE;
}
// 여기까지 기본 버튼

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 여기서부터 선택 버튼
//------------------------------------------------------------------------------------------------
/// _SelectButton::_SelectButton()
//------------------------------------------------------------------------------------------------
_SelectButton::_SelectButton() : _Button()
{
	m_IsMouseLeftButtonPressed		= FALSE; // 임진록
	m_PrevMousePressedTime			= 0;
}

//------------------------------------------------------------------------------------------------
/// 함수 : _SelectButton::_SelectButton(SHORT TextPutX, SHORT TextPutY, SHORT SelectBarX, 
///										SHORT SelectBarY, SHORT SelectBarWidth, SHORT SelectBarHeight, 
///										CHAR* MenuName, BOOL bAction)
//------------------------------------------------------------------------------------------------
_SelectButton::_SelectButton(SHORT TextPutX, SHORT TextPutY, SHORT SelectBarX, SHORT SelectBarY, SHORT SelectBarWidth, SHORT SelectBarHeight, CHAR* MenuName, BOOL bAction)
{
	m_IsMouseLeftButtonPressed		= FALSE; // 임진록
	m_PrevMousePressedTime			= 0;

	Create(TextPutX, TextPutY, SelectBarX, SelectBarY, SelectBarWidth, SelectBarHeight, MenuName, bAction);
}

//------------------------------------------------------------------------------------------------
/// 함수 : _SelectButton::_SelectButton(SHORT TextPutX, SHORT TextPutY, SHORT SelectBarX, SHORT SelectBarY, 
///						 SHORT SelectBarWidth, SHORT SelectBarHeight, CHAR* MenuName, 
///						 WORD flag, BOOL bAction)
///
//------------------------------------------------------------------------------------------------
_SelectButton::_SelectButton(SHORT TextPutX, SHORT TextPutY, SHORT SelectBarX, SHORT SelectBarY, SHORT SelectBarWidth, SHORT SelectBarHeight, CHAR* MenuName, WORD flag, BOOL bAction)
{
	m_IsMouseLeftButtonPressed		= FALSE; // 임진록
	m_PrevMousePressedTime			= 0;

	Create(TextPutX, TextPutY, SelectBarX, SelectBarY, SelectBarWidth, SelectBarHeight, MenuName, flag, bAction);
}
	

//------------------------------------------------------------------------------------------------
/// _SelectButton::_SelectButton()
//------------------------------------------------------------------------------------------------
_SelectButton::~_SelectButton()
{
}

/* 천년의 신화(3개 함수) */
//------------------------------------------------------------------------------------------------
//	설명			:	선택 버튼을 생성한다.
//	x, y			:	버튼의 위치.
//	Width, Height	:	버튼의 크기.
//	MenuName		:	버튼의 이름.
//	bAction			:	버튼의 활동 여부(기본값 = TRUE).
//------------------------------------------------------------------------------------------------
// 천년
VOID	_SelectButton::Create(INT x, INT y, INT Width, INT Height, CHAR* MenuName, BOOL bAction)
{	
	_SelectButton::Create(x, y, Width, Height, x, y, MenuName, bAction);
	m_bSelected	=	FALSE;	
}

//------------------------------------------------------------------------------------------------
/// 함수 : VOID _SelectButton::Create(SHORT TextPutX, SHORT TextPutY, SHORT SelectBarX, SHORT SelectBarY, SHORT SelectBarWidth, SHORT SelectBarHeight, CHAR* MenuName, BOOL bAction)
/// 목적 : 버튼을 생성한다.
//------------------------------------------------------------------------------------------------
// 천년
VOID	_SelectButton::Create(INT x, INT y, INT Width, INT Height, INT TextPutX, INT TextPutY, CHAR* MenuName, BOOL bAction)
{
	m_TextPutX	=	TextPutX;
	m_TextPutY	=	TextPutY;	

	_Button::Create(x, y, Width, Height, MenuName, bAction);
	m_bSelected = FALSE;	
}


//------------------------------------------------------------------------------------------------
/// 함수 : VOID _SelectButton::Create(SHORT TextPutX, SHORT TextPutY, SHORT SelectBarX, SHORT SelectBarY, SHORT SelectBarWidth, SHORT SelectBarHeight, CHAR* MenuName, WORD flag, BOOL bAction)
/// 목적 : 버튼을 생성한다.
//------------------------------------------------------------------------------------------------
// 천년
VOID _SelectButton::Create(INT x, INT y, INT Width, INT Height, INT TextPutX, INT TextPutY, CHAR* MenuName, WORD flag, BOOL bAction)
{
	m_TextPutX	=	TextPutX;
	m_TextPutY	=	TextPutY;

	_Button::Create(x, y, Width, Height, MenuName, flag, bAction);
	m_bSelected = FALSE;
}


//------------------------------------------------------------------------------------------------
/// void _SelectButton::Put(HDC hdc, DWORD PutFlag, DWORD PutTextFlag)
/// 목적 : 선택 버튼의 테두리를 화면에 그린다.
//------------------------------------------------------------------------------------------------
void _SelectButton::Put(LPDIRECTDRAWSURFACE7	lpSurface, DWORD PutFlag, DWORD PutTextFlag)
{		
	Put(m_TextPutX, m_TextPutY, m_x, m_y, lpSurface, PutFlag, PutTextFlag);
}

/* 임진록(3개 함수) */
//------------------------------------------------------------------------------------------------
/// VOID _SelectButton::Put(SHORT TextPutX, SHORT TextPutY, SHORT SelectBarX, SHORT SelectBarY, 
///							LPDIRECTDRAWSURFACE	lpSurface, DWORD PutFlag, DWORD PutTextFlag)
/// 목적 : 버튼을 화면에 그린다.
//------------------------------------------------------------------------------------------------
// 임진록
VOID _SelectButton::Put(SHORT TextPutX, SHORT TextPutY, SHORT SelectBarX, SHORT SelectBarY, LPDIRECTDRAWSURFACE7	lpSurface, DWORD PutFlag, DWORD PutTextFlag)
{
	COLORREF	TextColor;	
	BOOL		IsDrawBorder;
	BOOL		IsEnableDarkText;
	BOOL		IsFillColor;
		
	IsDrawBorder		=	(PutFlag & BUTTON_PUT_NOBORDER) ? FALSE : TRUE;
	IsEnableDarkText	=	(PutFlag & BUTTON_PUT_NODARK) ? FALSE : TRUE;				
	IsFillColor			=	(PutFlag & BUTTON_PUT_FILLCOLOR)? TRUE : FALSE;

	if(m_bAction == TRUE ||  IsEnableDarkText == FALSE)
	{
		if(IsEnableDarkText == FALSE)
		{
			if(m_bSelected == TRUE)
				TextColor = BUTTON_FRONT_SELECTED_TEXT_COLOR;			
			else
				TextColor = BUTTON_FRONT_UNSELECTED_TEXT_COLOR;
		}
		else
		{
			if(m_bProcess == TRUE)
			{
				if(m_bSelected == TRUE)
					TextColor = BUTTON_FRONT_SELECTED_TEXT_COLOR;
				else if(IsInMouse(SelectBarX, SelectBarY)==TRUE)
					TextColor = BUTTON_FRONT_ONMOUSE_TEXT_COLOR;
				else
					TextColor = BUTTON_FRONT_UNSELECTED_TEXT_COLOR;
			}
			else
			{
				if(m_bSelected == TRUE)
					TextColor = BUTTON_FRONT_SELECTED_TEXT_COLOR;
				else
					TextColor = BUTTON_FRONT_UNSELECTED_TEXT_COLOR;
			}
		}
	}
	else
	{
		TextColor = BUTTON_UNACTION_TEXT_COLOR;		
	}
	

	//박스를 그린다. 
	//잠시 hdc를 푼다.
	//그리고 락을 건다.		
	if(clGrp.LockSurface(lpSurface)==TRUE)
	{	
		if(IsDrawBorder == TRUE)
		{
			if(m_bSelected==TRUE)
				clGrp.BoxC(SelectBarX, SelectBarY, SelectBarX+m_Width, SelectBarY+m_Height, BUTTON_SELECT_BOX_COLOR);	
			else
				clGrp.BoxC(SelectBarX, SelectBarY, SelectBarX+m_Width, SelectBarY+m_Height, BUTTON_UNSELECT_BOX_COLOR);	
		}

		if(IsFillColor == TRUE)
		{
			if(m_bSelected==TRUE)
				clGrp.FillBox/*Light*/(SelectBarX, SelectBarY, SelectBarX+m_Width, SelectBarY+m_Height, BUTTON_DEFAULT_SELECTCOLOR);				
		}

		clGrp.UnlockSurface(lpSurface);
	}				
	 
	PutText(TextPutX, TextPutY,
			m_Width - (abs(TextPutX - SelectBarX)),			
			m_Height,
			lpSurface, 
			m_MenuName, 
			TextColor,
			(SHORT)PutTextFlag);
}

//------------------------------------------------------------------------------------------------
//	설명	:	
//------------------------------------------------------------------------------------------------
// 임진록
VOID	_SelectButton::Put(HDC hdc, DWORD PutFlag)
{
	COLORREF	TextColor;		
	BOOL		IsEnableDarkText;
		
	IsEnableDarkText	=	(PutFlag & BUTTON_PUT_NODARK) ? FALSE : TRUE;					

	if(m_bAction == TRUE ||  IsEnableDarkText == FALSE)
	{
		if(IsEnableDarkText == FALSE)
		{
			if(m_bSelected == TRUE)
				TextColor = BUTTON_FRONT_SELECTED_TEXT_COLOR;			
			else
				TextColor = BUTTON_FRONT_UNSELECTED_TEXT_COLOR;
		}
		else
		{
			if(m_bProcess == TRUE)
			{
				if(m_bSelected == TRUE)
					TextColor = BUTTON_FRONT_SELECTED_TEXT_COLOR;
				else if(IsInMouse()==TRUE)
					TextColor = BUTTON_FRONT_ONMOUSE_TEXT_COLOR;
				else
					TextColor = BUTTON_FRONT_UNSELECTED_TEXT_COLOR;
			}
			else
			{
				if(m_bSelected == TRUE)
					TextColor = BUTTON_FRONT_SELECTED_TEXT_COLOR;
				else
					TextColor = BUTTON_FRONT_UNSELECTED_TEXT_COLOR;
			}
		}
	}
	else
	{
		TextColor = BUTTON_UNACTION_TEXT_COLOR;		
	}
	

	PutText(m_TextPutX, m_TextPutY, hdc, m_MenuName, TextColor);
}

//------------------------------------------------------------------------------------------------
//	설명	:	
//------------------------------------------------------------------------------------------------
// 임진록
VOID	_SelectButton::PutPlus(HDC hdc, DWORD PutFlag)
{
	COLORREF	TextColor;		
	BOOL		IsEnableDarkText;
		
	IsEnableDarkText	=	(PutFlag & BUTTON_PUT_NODARK) ? FALSE : TRUE;					

	if(m_bAction == TRUE ||  IsEnableDarkText == FALSE)
	{
		if(IsEnableDarkText == FALSE)
		{
			if(m_bSelected == TRUE)
				TextColor = BUTTON_FRONT_SELECTED_TEXT_COLOR;			// 선택시에 보여지는 붉은색
			else
				TextColor = BUTTON_FRONT_UNSELECTED_TEXT_COLOR_PLUS;	// 평상시에 보여지는 색
		}
		else
		{
			if(m_bProcess == TRUE)
			{
				if(m_bSelected == TRUE)
					TextColor = BUTTON_FRONT_SELECTED_TEXT_COLOR;	// 선택되었을때
				else if(IsInMouse()==TRUE)
					TextColor = BUTTON_FRONT_ONMOUSE_TEXT_COLOR_PLUS;	// 마우스가 위에 있을때(현재는 흰색)
				else
					TextColor = BUTTON_FRONT_UNSELECTED_TEXT_COLOR_PLUS;	// 평상시에 보여지는 색 
			}
			else
			{
				if(m_bSelected == TRUE)
					TextColor = BUTTON_FRONT_SELECTED_TEXT_COLOR;
				else
					TextColor = BUTTON_FRONT_UNSELECTED_TEXT_COLOR_PLUS;
			}
		}
	}
	else
	{
		TextColor = BUTTON_UNACTION_TEXT_COLOR;		
	}
	

	PutTextPlus(m_TextPutX, m_TextPutY, hdc, m_MenuName, TextColor);
}

/* 천년의 신화(4개 함수) */
//------------------------------------------------------------------------------------------------
//	설명					:	선택 버튼을 그린다.
//	x, y					:	버튼의 좌표.
//	TextPutX, TextPutY		:	선택 버튼 내부의 텍스트를 찍을 위치.
//								(서피시를 기준으로 상대좌표가아닌 절대좌표이다.)
//	lpSurface				:	버튼을 그릴 서피스.
//	pXspr, FontNum			:	선택한 그림을 그리기 위한 스프라이트.
//	PutFlag					:	일반 버튼 출력 옵션.
//	PutTextFlag				:	버튼 내 텍스트 출력 옵션.
//------------------------------------------------------------------------------------------------
// 천년
VOID _SelectButton::Put(INT x, INT y, INT TextPutX, INT TextPutY,
						LPDIRECTDRAWSURFACE7 lpSurface,
						XSPR *pXspr, SHORT FontNum, DWORD PutFlag, DWORD PutTextFlag)
{
	COLORREF	TextColor;	
	BOOL		IsDrawBorder;
	BOOL		IsEnableDarkText;	
	BOOL		IsDrawSelectImage;
		
	IsDrawBorder		=	(PutFlag & BUTTON_PUT_NOBORDER) ? FALSE : TRUE;
	IsEnableDarkText	=	(PutFlag & BUTTON_PUT_NODARK) ? FALSE : TRUE;					
	IsDrawSelectImage	=	pXspr ? TRUE : FALSE;

	if(m_bAction == TRUE ||  IsEnableDarkText == FALSE)
	{
		if(IsEnableDarkText == FALSE)
		{
			if(m_bSelected == TRUE)
				TextColor = BUTTON_FRONT_SELECTED_TEXT_COLOR;			
			else
				TextColor = BUTTON_FRONT_UNSELECTED_TEXT_COLOR;
		}
		else
		{
			if(m_bProcess == TRUE)
			{
				if(m_bSelected == TRUE)
					TextColor = BUTTON_FRONT_SELECTED_TEXT_COLOR;
				else if(IsInMouse(x, y)==TRUE)
					TextColor = BUTTON_FRONT_ONMOUSE_TEXT_COLOR;
				else
					TextColor = BUTTON_FRONT_UNSELECTED_TEXT_COLOR;
			}
			else
			{
				if(m_bSelected == TRUE)
					TextColor = BUTTON_FRONT_SELECTED_TEXT_COLOR;
				else
					TextColor = BUTTON_FRONT_UNSELECTED_TEXT_COLOR;
			}
		}
	}
	else
	{
		TextColor = BUTTON_UNACTION_TEXT_COLOR;		
	}
	

	//박스를 그린다. 
	//잠시 hdc를 푼다.
	//그리고 락을 건다.		
	if(clGrp.LockSurface(lpSurface)==TRUE)
	{	
		if(IsDrawBorder)
		{
			if(m_bSelected)
				clGrp.BoxC(x, y, x + m_Width - 1, y + m_Height - 1, BUTTON_SELECT_BOX_COLOR);	
			else
				clGrp.BoxC(x, y, x + m_Width - 1, y + m_Height - 1, BUTTON_UNSELECT_BOX_COLOR);	
		}

		/*
		if(m_bSelected)
			clGrp.FillBoxLight(x, y, x + m_Width - 1, y + m_Height - 1, BUTTON_DEFAULT_SELECTCOLOR);
		*/

		
		if(IsDrawSelectImage)
		{
			if(m_bSelected)
				clGrp.PutSpriteT(x, y, 
								pXspr->Header.Xsize, pXspr->Header.Ysize,
								&pXspr->Image[pXspr->Header.Start[FontNum]]);
		}
		

		clGrp.UnlockSurface(lpSurface);
	}				
	
	if(TextPutX != -1 || TextPutY != -1)
	{
		PutText(TextPutX, TextPutY,
				m_Width - (abs(TextPutX - x)),			
				m_Height,
				lpSurface, 
				m_MenuName, 
				TextColor,
				(SHORT)PutTextFlag);
	}
	else
	{
		PutText(-1, -1,
				m_Width, m_Height,
				lpSurface, m_MenuName, TextColor,
				(SHORT)PutTextFlag);
	}
}

//------------------------------------------------------------------------------------------------
//	설명	:	버튼 출력 함수.
//------------------------------------------------------------------------------------------------
// 천년
VOID	_SelectButton::Put(SHORT x, SHORT y, XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag)
{
	_Button::Put(x, y, lpXspr, DefaultFontNum, SelectedFontNum, OnMouseFontNum, PutFlag);
}
						
//------------------------------------------------------------------------------------------------
//	설명	:	버튼 출력 함수.
//------------------------------------------------------------------------------------------------
// 천년
VOID	_SelectButton::Put(XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag)
{
	_Button::Put(lpXspr, DefaultFontNum, SelectedFontNum, OnMouseFontNum, PutFlag);
}

//------------------------------------------------------------------------------------------------
//	설명				:	버튼 출력 함수.
//	x, y				:	x, y 좌표.
//	TextPutX, TextPutY	:	텍스트 출력 좌표.
//	hdc					:	DC.
//	PutTextFlag			:	찍을때의 옵션.
//------------------------------------------------------------------------------------------------
// 천년
VOID	_SelectButton::Put(INT x, INT y, INT TextPutX, INT TextPutY, HDC hdc, DWORD PutTextFlag)
{	
	COLORREF	TextColor;		
	
	if(m_bAction == TRUE)
	{		
		if(m_bProcess == TRUE)
		{
			if(m_bSelected == TRUE)
				TextColor = BUTTON_FRONT_SELECTED_TEXT_COLOR;
			else if(IsInMouse(x, y)==TRUE)
				TextColor = BUTTON_FRONT_ONMOUSE_TEXT_COLOR;
			else
				TextColor = BUTTON_FRONT_UNSELECTED_TEXT_COLOR;
		}
		else
		{
			if(m_bSelected == TRUE)
				TextColor = BUTTON_FRONT_SELECTED_TEXT_COLOR;
			else
				TextColor = BUTTON_FRONT_UNSELECTED_TEXT_COLOR;
		}	
	}
	else
	{
		TextColor = BUTTON_UNACTION_TEXT_COLOR;		
	}
	
	
	if(TextPutX != -1 || TextPutY != -1)
	{		
		PutText(TextPutX, TextPutY,
				m_Width - (abs(TextPutX - x)),			
				m_Height,
				hdc, 
				m_MenuName, 
				TextColor,
				(SHORT)PutTextFlag);
	}
	else
	{
		PutText(-1, -1,
				m_Width, m_Height,
				hdc, m_MenuName, TextColor,
				(SHORT)PutTextFlag);
	}
}

/////////////////led///////////////////////////////////////////////////////////////////////////////
/// void _SelectButton::SetSelected(BOOL bSelected)
/// 목적 : 현 버튼을 선택한다.
//------------------------------------------------------------------------------------------------
void _SelectButton::SetSelected(BOOL bSelected)
{
	m_bSelected=bSelected;
}

//------------------------------------------------------------------------------------------------
/// BOOL _SelectButton::IsSelected()
/// 목적 : 현 버튼을 선택한다.
//------------------------------------------------------------------------------------------------
BOOL _SelectButton::IsSelected()
{
	return m_bSelected;	
}

/*천년의 함수(2개함수)*/
//------------------------------------------------------------------------------------------------
//	설명			:	SelectButton이 처리 함수. m_PrevMousePressedTime
//	bPush			:	현재 마우스의 눌림 상태.
//	pbIsDoubleClick	:	더블 클릭을 하였는지.
//------------------------------------------------------------------------------------------------

BOOL	_SelectButton::Process(BOOL bPush, BOOL *pbIsDoubleClick)
{
	BOOL	bPress;
	SI32	siCurrentTime;

	bPress			=	_Button::Process(bPush);

	siCurrentTime	=	timeGetTime();

	if(pbIsDoubleClick && bPress)
	{			
		if(siCurrentTime - m_PrevMousePressedTime < DOUBLE_CLICK_DELAY)
			*pbIsDoubleClick	=	TRUE;		
		else		
			*pbIsDoubleClick	=	FALSE;		
	}


	if(bPress)
		m_PrevMousePressedTime	=	siCurrentTime;
		
	return bPress;
}

//------------------------------------------------------------------------------------------------
//	설명			:	SelectButton이 처리 함수.
//	x, y			:	좌표.
//	bPush			:	현재 마우스의 눌림 상태.
//	pbIsDoubleClick	:	더블 클릭을 하였는지.
//------------------------------------------------------------------------------------------------
BOOL	_SelectButton::Process(INT x, INT y, BOOL bPush, BOOL *pbIsDoubleClick)
{
	BOOL	bPress;
	SI32	siCurrentTime;

	bPress	=	_Button::Process(x, y, bPush);	

	siCurrentTime	=	timeGetTime();

	if(pbIsDoubleClick && bPress)
	{
		if(siCurrentTime - m_PrevMousePressedTime < DOUBLE_CLICK_DELAY)	
			*pbIsDoubleClick	=	TRUE;
		else
			*pbIsDoubleClick	=	FALSE;
	}


	if(bPress)
		m_PrevMousePressedTime	=	siCurrentTime;

	return bPress;
}


/* 임진록(1개 함수) */
//------------------------------------------------------------------------------------------------
/// BOOL _SelectButton::IsDoubleClick()
/// 목적 : 더블 클릭을 했는지 검사한다.
//------------------------------------------------------------------------------------------------
// 임진록
BOOL _SelectButton::IsDoubleClick()
{
	DWORD		dwCurrentTime = timeGetTime();

	if((m_bAction == TRUE) && (m_bProcess == TRUE))
	{
		if(m_IsMouseLeftButtonPressed)				//전에 마우스를 눌렀다.
		{
			if(IpD.LeftPressSwitch==FALSE)				//마우스가 떼어졌다.	즉 한번 클릭했다.
			{
				m_IsMouseLeftButtonPressed = FALSE;							

				if((dwCurrentTime - m_PrevMousePressedTime) < DOUBLE_CLICK_DELAY)
				{							
					m_PrevMousePressedTime = dwCurrentTime;
					return TRUE;		
				}					

				m_PrevMousePressedTime = dwCurrentTime;
			}
		}
		else
		{
			if(IpD.LeftPressSwitch == TRUE)
			{
				m_IsMouseLeftButtonPressed  = TRUE;			
			}
		}
	}

	return FALSE;
}

//------------------------------------------------------------------------------------------------
/// 함수 : VOID _SelectButton::SetTextPutX(SHORT TextPutX)
/// 목적 : 버튼을 생성한다.
//------------------------------------------------------------------------------------------------
VOID _SelectButton::SetTextPutX(SHORT TextPutX)
{
	m_TextPutX	=	TextPutX;
}

//------------------------------------------------------------------------------------------------
/// 함수 : VOID _SelectButton::SetTextPutY(SHORT TextPutY)
/// 목적 : 버튼을 생성한다.
//------------------------------------------------------------------------------------------------
VOID _SelectButton::SetTextPutY(SHORT TextPutY)
{
	m_TextPutY	=	TextPutY;
}

// 여기까지 선택 버튼

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* 임진록에만 있음(3개 함수) */
//------------------------------------------------------------------------------------------------
/// _ServiceProviderButton::_ServiceProviderButton()
/// 목적 : 서비스 프로바이더 버튼의 생성자.
//------------------------------------------------------------------------------------------------
// 임진록
_ServiceProviderButton::_ServiceProviderButton() : _SelectButton()
{	
	m_bSelected=FALSE;
}

//------------------------------------------------------------------------------------------------
/// _ServiceProviderButton::_ServiceProviderButton(SHORT x, SHORT y, SHORT width, SHORT height, CHAR* lpMenuName)
/// 목적 : 서비스 프로바이더 버튼의 생성자. 주어진 위치와 크기로 버튼을 생성한다.
//------------------------------------------------------------------------------------------------
// 임진록
_ServiceProviderButton::_ServiceProviderButton(SHORT x, SHORT y, SHORT width, SHORT height, CHAR* lpMenuName)  
{
	_Button::_Button(x, y, width, height, lpMenuName);	
	m_bSelected=FALSE;
}

//------------------------------------------------------------------------------------------------
/// _ServiceProviderButton::~_ServiceProviderButton()
/// 목적 : 서비스 프로바이더 버튼의 소멸자. 할당 받은 메모리를 해제 한다.
//------------------------------------------------------------------------------------------------
// 임진록
_ServiceProviderButton::~_ServiceProviderButton()
{	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* 임진록에만 있는 부분*/
//------------------------------------------------------------------------------------------------
/// _EnumSessionButton::__EnumSessionButton()
/// 목적 : 서비스 프로바이더 버튼의 생성자.
//------------------------------------------------------------------------------------------------
// 임진록
_EnumSessionButton::_EnumSessionButton() : _SelectButton()
{
}

//------------------------------------------------------------------------------------------------
/// _EnumSessionButton::_EnumSessionButton(SHORT x, SHORT y, SHORT width, SHORT height, CHAR* lpMenuName)
/// 목적 : 서비스 프로바이더 버튼의 생성자. 주어진 위치와 크기로 버튼을 생성한다.
//------------------------------------------------------------------------------------------------
// 임진록
_EnumSessionButton::_EnumSessionButton(SHORT x, SHORT y, SHORT width, SHORT height, CHAR* lpMenuName)  
{
	_Button::_Button(x, y, width, height,lpMenuName);	
}

//------------------------------------------------------------------------------------------------
/// _EnumSessionButton::~_EnumSessionButton()
/// 목적 : 서비스 프로바이더 버튼의 소멸자. 할당 받은 메모리를 해제 한다.
//------------------------------------------------------------------------------------------------
// 임진록
_EnumSessionButton::~_EnumSessionButton()
{	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* 임진록에만 있음 */
//------------------------------------------------------------------------------------------------
/// 함수 : _SelectMapFileButton::_SelectMapFileButton()
/// 설명 : 
//------------------------------------------------------------------------------------------------
// 임진록
_SelectMapFileButton::_SelectMapFileButton() : _SelectButton()
{
}

//------------------------------------------------------------------------------------------------
/// 함수 : _SelectMapFileButton::_SelectMapFileButton(SHORT x, SHORT y, SHORT width, SHORT height, CHAR* lpMenuName)
/// 설명 : 
//------------------------------------------------------------------------------------------------
// 임진록
_SelectMapFileButton::_SelectMapFileButton(SHORT x, SHORT y, SHORT width, SHORT height, CHAR* lpMenuName)
{
	_Button::_Button(x, y, width, height,lpMenuName);	
}

//------------------------------------------------------------------------------------------------
/// 함수 : _SelectMapFileButton::~_SelectMapFileButton()
/// 설명 : 
//------------------------------------------------------------------------------------------------
// 임진록
_SelectMapFileButton::~_SelectMapFileButton()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------
/// 함수 : _Box::_Box()
/// 목적 : 박스의 생성자.
//------------------------------------------------------------------------------------------------
_Box::_Box()
{
	m_x = 0;
	m_y = 0;
	m_Width = 0;
	m_Height = 0;

	ZeroMemory(m_Name, sizeof(m_Name));
}

//------------------------------------------------------------------------------------------------
/// 함수 : _Box::_Box(SHORT x, SHORT y, SHORT Width, SHORT Height, CHAR *lpName, WORD PutFlag)
/// 목적 : 박스의 생성자.
//------------------------------------------------------------------------------------------------
_Box::_Box(SHORT x, SHORT y, SHORT Width, SHORT Height, CHAR* lpName, WORD PutFlag)
{
	Create(x, y, Width, Height, lpName, PutFlag);
}

//------------------------------------------------------------------------------------------------
/// 함수 : _Box::~_Box()
/// 목적 : 박스의 소멸자
//------------------------------------------------------------------------------------------------
_Box::~_Box()
{
}

//------------------------------------------------------------------------------------------------
/// 함수 : void	_Box::Create(SHORT x, SHORT y, SHORT Width, SHORT Height, CHAR *lpName, WORD PutFlag)
/// 목적 : 박스를 생성한다.
//------------------------------------------------------------------------------------------------
void	_Box::Create(SHORT x, SHORT y, SHORT Width, SHORT Height, CHAR *lpName, WORD PutFlag)
{
	switch(PutFlag)
	{
	case BOX_PUT_LEFT:
		m_x=x;
		m_y=y;
		m_Width=Width;
		m_Height=Height;
		break;	
	case BOX_PUT_CENTER:
		m_x=x - (Width / 2);
		m_y=y - (Height / 2);
		m_Width=Width;
		m_Height=Height;
		break;
	}	

	if(lpName)
		strcpy(m_Name, lpName);
}

//------------------------------------------------------------------------------------------------
/// 함수 : void _Box::Put(LPDIRECTDRAWSURFACE	lpSurface, DWORD PutFlag, WORD PutTextFlag)
/// 목적 : 박스를 출력한다. 
/// 인자 : PutFlag - 글자를 어떻게 출력할 것인지 ..
//------------------------------------------------------------------------------------------------
void _Box::Put(LPDIRECTDRAWSURFACE7	lpSurface, DWORD PutFlag, WORD PutTextFlag)
{	
	BOOL	IsDrawBorder;
	
	IsDrawBorder = (PutFlag & BOX_PUT_NOBORDER) ? FALSE : TRUE;

	if(IsDrawBorder == TRUE)
	{		
		if(clGrp.LockSurface(lpSurface)==TRUE)
		{			
			clGrp.BoxC(m_x, m_y, m_x+m_Width, m_y+m_Height, BUTTON_UNSELECT_BOX_COLOR);	
			clGrp.UnlockSurface(lpSurface);
		}			
	}	

	PutText(m_x, m_y, lpSurface, m_Name, PutTextFlag);
}

//------------------------------------------------------------------------------------------------
/// 함수 : void _Box::Put(HDC	hdc, DWORD PutFlag, WORD PutTextFlag)
/// 목적 : 박스를 출력한다. 
/// 인자 : PutFlag - 글자를 어떻게 출력할 것인지 ..
//------------------------------------------------------------------------------------------------
void _Box::Put(HDC	hdc, DWORD PutFlag, WORD PutTextFlag)
{	
	PutText(m_x, m_y, hdc, m_Name, PutTextFlag);
}

//------------------------------------------------------------------------------------------------
/// 함수 : VOID _Box::PutText(INT x, INT y, LPDIRECTDRAWSURFACE	lpSurface, CHAR *lpText, WORD TextArrange)
/// 목적 : 박스의 소멸자
//------------------------------------------------------------------------------------------------
VOID	_Box::PutText(INT x, INT y, LPDIRECTDRAWSURFACE7	lpSurface, CHAR *lpText, WORD TextArrange)
{	
	SHORT	nStringHeight;
	SHORT	nStringWidth;
	RECT	rect;
	SHORT	nx, ny;

	if(lpText==NULL)
		return;
	
	PutTextToTextBufferOneLine(lpText, &nStringWidth, &nStringHeight, BOX_FRONT_TEXT_COLOR);

	switch(TextArrange)
	{
	case BOX_PUT_LEFT:				
		nx	=	x;
		ny	=	max(y, y + ((m_Height / 2) - (nStringHeight / 2)));
		break;
	case BOX_PUT_CENTER:
	case BOX_PUT_DEFAULT:
		nx	=   max(x, x + ((m_Width / 2) - (nStringWidth / 2)));
		ny	=	max(y, y + ((m_Height / 2) - (nStringHeight / 2)));		
		break;
	case BOX_PUT_ORIGINAL:
		nx	=	x;
		ny	=	y;
		break;
	}

	rect.left	=	0;
	rect.top	=	0;
	rect.right	=	rect.left + min(m_Width, nStringWidth);
	rect.bottom	=	rect.top  + min(m_Height, nStringHeight);	
	lpSurface->BltFast(nx, ny, lpBriefingTextBuffer, &rect, DDBLTFAST_SRCCOLORKEY|DDBLTFAST_WAIT);		
}

//------------------------------------------------------------------------------------------------
/// 함수 : VOID _Box::PutText(INT x, INT y, HDC	hdc, CHAR *lpText, WORD TextArrange)
/// 목적 : 
//------------------------------------------------------------------------------------------------
VOID	_Box::PutText(INT x, INT y, HDC	hdc, CHAR *lpText, WORD TextArrange)
{		
	SHORT	nx, ny;
	SIZE	Size;

	if(lpText==NULL)
		return;	
	
	GetTextExtentPoint32(hdc, lpText, strlen(lpText), &Size);

	switch(TextArrange)
	{
	case BOX_PUT_LEFT:				
		nx	=	x;
		ny	=	max(y, y + ((m_Height / 2) - (Size.cy / 2)));
		break;
	case BOX_PUT_CENTER:
	case BOX_PUT_DEFAULT:
		nx	=   max(x, x + ((m_Width / 2) - (Size.cx / 2)));
		ny	=	max(y, y + ((m_Height / 2) - (Size.cy / 2)));		
		break;
	case BOX_PUT_ORIGINAL:
		nx	=	x;
		ny	=	y;
		break;
	}

	TextOut(hdc, nx, ny, lpText, strlen(lpText));
}

//------------------------------------------------------------------------------------------------
/// 함수 : SHORT _Box::GetX()
/// 목적 : 
//------------------------------------------------------------------------------------------------
SHORT _Box::GetX()
{
	return m_x;
}


//------------------------------------------------------------------------------------------------
/// 함수 : SHORT _Box::GetY()
/// 목적 : 
//------------------------------------------------------------------------------------------------
SHORT _Box::GetY()
{
	return m_y;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------
/// 함수 : _ImageButton::_ImageButton()
/// 설명 : 
//------------------------------------------------------------------------------------------------
_ImageButton::_ImageButton() : _Button()
{
	m_lpSampleBuffer	=	NULL;
	m_SelectColor		=	NULL;
	m_SampleXsize		=	0;
}

//------------------------------------------------------------------------------------------------
/// 함수 : _ImageButton::~_ImageButton()
/// 설명 : 
//------------------------------------------------------------------------------------------------
_ImageButton::~_ImageButton()
{	
}

///------------------------------------------------------------------------------------------------------
// 설명				: 이미지 버튼을 생성한다.
// x, y				: 이미지 버튼을 놓을 위치
// width, height	: 이미지 버튼의 크기
// lpSampleBuffer	: 이미지 버튼을 선택하기 위하여 단색으로 그려 놓은 버퍼
// Color			: lpSampleBuffer에서 버튼을 선택하기 위하여 단색의 팔레트 인덱스.
// nSampleXsize		: Sample 버퍼의 가로 사이즈.
// uiFlag           : 각종 옵션.
///------------------------------------------------------------------------------------------------------
VOID _ImageButton::Create(SHORT x, SHORT y, SHORT Width, SHORT Height, BYTE* lpSampleBuffer, BYTE Color, SHORT nSampleXsize, WORD uiFlag)
{
	m_lpSampleBuffer	=	lpSampleBuffer;
	m_SelectColor		=	Color;
	m_SampleXsize		=	nSampleXsize;
	
	_Button::Create(x, y, Width, Height, "", (WORD)uiFlag);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	이미지 버튼을 생성한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	_ImageButton::Create(SHORT x, SHORT y, SHORT Width, SHORT Height, CHAR* MenuName, BYTE* lpSampleBuffer, BYTE Color, WORD uiFlag)
{
	m_lpSampleBuffer	=	lpSampleBuffer;
	m_SelectColor		=	Color;
	m_SampleXsize		=	Width;

	_Button::Create(x, y, Width, Height, MenuName, (WORD)uiFlag);
}

//------------------------------------------------------------------------------------------------
/// 함수 : BOOL _ImageButton::IsInMouse()
/// 설명 : 
//------------------------------------------------------------------------------------------------
BOOL _ImageButton::IsInMouse()
{	
	if(_Button::IsInMouse() == TRUE)
	{
		if(m_SelectColor == m_lpSampleBuffer[(IpD.Mouse_Y - m_y) * m_SampleXsize + (IpD.Mouse_X - m_x)])	return TRUE;		
		else																							return FALSE;		
	}

	return FALSE;
}

//------------------------------------------------------------------------------------------------
/// 함수 : BOOL _ImageButton::Process(BOOL bPush)
/// 설명 : 
//------------------------------------------------------------------------------------------------
BOOL _ImageButton::Process(BOOL bPush)
{
	if(m_bAction==TRUE && m_bProcess == TRUE)
	{
		if(IsInMouse()==TRUE)
		{	
			if(MbPrevFrameInButton	== FALSE && (MuiFlag & BUTTON_PLAYSOUND_ONMOUSE))		
				PushEffect(EFFECT_SELECTCOUNTRY, 0, 0);

			if(IpD.LeftPressSwitch == FALSE)
			{
				//전에 버튼이 눌러져 있었는데 버튼을 뗀 상태라면
				//버튼을 선택했다는 것이다.
				if(bPush==TRUE)
				{				
					bPush=FALSE;
					m_bSoundPush=FALSE;
					return TRUE;
				}
			}
			else
			{
				if(m_bSoundPush==FALSE)
				{
					PushEffect(EFFECT_TITLEBUTTON, 0, 0);
					m_bSoundPush=TRUE;
				}			
			}

			MbPrevFrameInButton		=	TRUE;
		}
		else
		{
			m_bSoundPush			=	FALSE;
			MbPrevFrameInButton		=	FALSE;
		}
	}

	return FALSE;
}

//------------------------------------------------------------------------------------------------
/// VOID	_ImageButton::Put(XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag)
/// 목적 : 기본적인 위치에 스프라이트를 출력한다. 
//------------------------------------------------------------------------------------------------
VOID	_ImageButton::Put(XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag)
{
	Put(m_x, m_y, lpXspr, DefaultFontNum, SelectedFontNum, OnMouseFontNum, PutFlag);
}

//------------------------------------------------------------------------------------------------
/// void _ImageButton::Put(SHORT x, SHORT y, XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag)
/// 목적 : x, y의 위치에 스프라이트를 출력한다. 
//------------------------------------------------------------------------------------------------
VOID	_ImageButton::Put(SHORT x, SHORT y, XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag)
{
	INT			x1, y1;		
	BOOL		IsMoveWindow;	
	SHORT		FontNumber;
	
	IsMoveWindow = (PutFlag & BUTTON_PUT_NOMOVE) ? FALSE : TRUE;				
	
	x1 = x;
	y1 = y;
	
	if(m_bAction == TRUE)
	{
		if(m_bProcess == TRUE)
		{
			if(IsInMouse() == TRUE)
			{			
				if(IpD.LeftPressSwitch == TRUE)			
				{
					if(IsMoveWindow == TRUE)
					{
						x1 = x+2;
						y1 = y+2;
					}			

					FontNumber = SelectedFontNum;						
				}
				else							
					FontNumber = OnMouseFontNum;			
			}
			else					
				FontNumber = DefaultFontNum;		
		}
		else
			FontNumber	= DefaultFontNum;

		PutSpriteButton(x1, y1, lpXspr, FontNumber, FALSE, FALSE);	
	}
	else			
	{
		FontNumber = OnMouseFontNum;

		if(BUTTON_PUT_NODARK & PutFlag)		PutSpriteButton(x1, y1, lpXspr, FontNumber, FALSE, FALSE);						
		else								PutSpriteButton(x1, y1, lpXspr, FontNumber, TRUE, FALSE);									
	}	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	_ImageButton::Put(HDC hdc, DWORD PutFlag)
{
	INT			x1, y1;
	COLORREF	TextColor;		
	BOOL		IsMoveWindow;	
	BOOL		IsEnableDarkText;

	IsMoveWindow		=	(PutFlag & BUTTON_PUT_NOMOVE) ? FALSE : TRUE;		
	IsEnableDarkText	=	(PutFlag & BUTTON_PUT_NODARK) ? FALSE : TRUE;

	if(m_bAction)
	{
		if(m_bProcess)
		{
			if(IsInMouse()==TRUE) 	
			{
				TextColor = BUTTON_FRONT_ONMOUSE_TEXT_COLOR;	

				if(IpD.LeftPressSwitch && IsMoveWindow)
				{
					x1 = m_x + 2;
					y1 = m_y + 2;
				}
				else
				{
					x1 = m_x;
					y1 = m_y;	
				}
			}
			else
			{
				x1 = m_x;
				y1 = m_y;		

				TextColor = BUTTON_FRONT_UNSELECTED_TEXT_COLOR;	
			}
		}
		else
		{
			x1 = m_x;
			y1 = m_y;		

			TextColor	= BUTTON_FRONT_UNSELECTED_TEXT_COLOR;
		}		
	}
	else				
	{		
		if(IsEnableDarkText == TRUE)
			TextColor = BUTTON_UNACTION_TEXT_COLOR;		
		else
			TextColor = BUTTON_FRONT_UNSELECTED_TEXT_COLOR;		

		x1=m_x;
		y1=m_y;		
	}

	PutText(x1, y1, hdc, m_MenuName, TextColor);		
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------
// Name: VOID	OnceInitButton()
// Desc: 버튼의 위치 및 크기를 정한다.
//------------------------------------------------------------------------------------------------
VOID	OnceInitButton()
{
	INT			i;
	SHORT		x, y;
	CHAR		szButtonName[128];
	
	//              	8
	for(i=0; i < GLOBAL_BUTTON_NUM; i++)
	{
		switch(i)
		{
		case BUTTON_GLOBAL_DECIDE:
			x = BUTTON_DECIDE_X;		y = BUTTON_DECIDE_Y;
			strcpy(szButtonName, Text.ButtonDecide.Get());
			break;
		case BUTTON_GLOBAL_BACK:
			x = BUTTON_BACK_X;		    y = BUTTON_BACK_Y;
			strcpy(szButtonName, Text.ButtonReturn.Get());
			break;

		case BUTTON_GLOBAL_EXIT:
			x = BUTTON_EXIT_X; 			y = BUTTON_EXIT_Y;
			strcpy(szButtonName, Text.ButtonExit.Get());
			break;

		case BUTTON_GLOBAL_SEARCH:
			x = BUTTON_SEARCH_X;		y = BUTTON_SEARCH_Y;
			strcpy(szButtonName, Text.ButtonSearch.Get());
			break;
		case BUTTON_GLOBAL_CREATE:
			x = BUTTON_CREATE_X;		y = BUTTON_CREATE_Y;
			strcpy(szButtonName, Text.ButtonMakeRoom.Get());
			break;
		case BUTTON_GLOBAL_JOIN:
			x = BUTTON_JOIN_X;			y = BUTTON_JOIN_Y;
			strcpy(szButtonName, Text.ButtonJoin.Get());
			break;
		case BUTTON_GLOBAL_REPLAY:
			x = BUTTON_REPLAY_X;		y =	BUTTON_REPLAY_Y;
			strcpy(szButtonName, Text.ButtonReview.Get());
			break;
		case BUTTON_GLOBAL_GOPLAY:
			x = BUTTON_GOPLAY_X;		y = BUTTON_GOPLAY_Y;
			strcpy(szButtonName, Text.ButtonGame.Get());
			break;
		default:
			clGrp.Error("FKJE8567 : Y0983LL23421", "Overflow Button...");
		}
		
		g_Button[i].Create(x, y, BUTTON_WIDTH, BUTTON_HEIGHT, szButtonName, BUTTON_PUT_LEFT); 
	}
}


//------------------------------------------------------------------------------------------------
///	함수 : VOID	SetSetActionAllButton(BOOL bAction)
/// 목적 : 모든 버튼의 액티브 시킨다.
//------------------------------------------------------------------------------------------------
VOID	SetActionAllButton(BOOL bAction)
{
	INT		i;

	for(i=0; i < GLOBAL_BUTTON_NUM; i++)	
		g_Button[i].SetAction(bAction);	
}


//------------------------------------------------------------------------------------------------
///	함수 : BOOL	LoadAllButtons()
/// 목적 : 모든 버튼을 로드 한다.
//------------------------------------------------------------------------------------------------
BOOL	LoadAllButtons()
{
	CHAR	szFileNameBuffer[1024];
	CHAR	szButtonsFileName[TOTAL_BUTTONS_KINDOF_NUM][256]=
			{
				BUTTONS_FILENAME_TYPE_01_01,
				BUTTONS_FILENAME_TYPE_02_01,
				BUTTONS_FILENAME_TYPE_03_01,
				BUTTONS_FILENAME_TYPE_04_01,
				BUTTONS_FILENAME_TYPE_05_01,
				BUTTONS_FILENAME_TYPE_06_01,
				BUTTONS_FILENAME_TYPE_07_01,	
				BUTTONS_FILENAME_TYPE_08_01,
				BUTTONS_FILENAME_TYPE_09_01,
				BUTTONS_FILENAME_TYPE_10_01	
			};
	SHORT	i;

	for(i=0; i < TOTAL_BUTTONS_KINDOF_NUM; i++)
	{
		if(g_Buttons_Spr[i].Image) clGrp.Error("FKJE8567", "BUTTON001");
		
		if(szButtonsFileName[i][0] == NULL)
			continue;

		GetFileNamePath(szButtonsFileName[i], SavePath, szFileNameBuffer);
		if(clGrp.LoadXspr(szFileNameBuffer, g_Buttons_Spr[i]) == FALSE) clGrp.Error("FKJE8567", "YBUTTON002 [%s]", szFileNameBuffer);
	}


	return TRUE;
}

//------------------------------------------------------------------------------------------------
///	함수 : VOID	FreeAllButtons()
/// 목적 : 모든 버튼을 해제 한다.
//------------------------------------------------------------------------------------------------
VOID	FreeAllButtons()
{
	SHORT i;

	for(i=0; i < TOTAL_BUTTONS_KINDOF_NUM; i++)	
		if(g_Buttons_Spr[i].Image) clGrp.FreeXspr(g_Buttons_Spr[i]);	

	if(GOverButtonSpr.Image) clGrp.FreeXspr(GOverButtonSpr);
}




