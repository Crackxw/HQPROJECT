//--------------------------------------------------------------------
//  ��� : ���°�
//--------------------------------------------------------------------

/* 
	�պ��� ������(��.�� + õ.�� = ��ư)
	ǥ�� ���
	1. ������ : �����Ͽ��� �ִ� �κ�
	2. õ  �� : õ���� ��ȭ���� �ִ� �κ�
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

extern	unsigned char	DarkTable[][256];		// ��ο� �ܰ� 
extern _InputDevice		IpD;
// �ؽ�Ʈ 
extern	_Text			Text;
extern	char			SavePath[];

#define		BUTTON_BACK_TEXT_COLOR				(RGB(1, 1, 1))
#define		BUTTON_FRONT_SELECTED_TEXT_COLOR	(RGB(206, 113, 14)) // ���������ÿ� ��Ÿ���� ������

#define		BUTTON_FRONT_UNSELECTED_TEXT_COLOR	(RGB(161, 161, 161))

#define		BUTTON_FRONT_UNSELECTED_TEXT_COLOR_PLUS	(RGB(0, 0, 0))	// ������2pLUS������ ���

#define		BUTTON_FRONT_ONMOUSE_TEXT_COLOR			 (RGB(219, 213, 200)) // ���콺�� ���� ������
#define		BUTTON_FRONT_ONMOUSE_TEXT_COLOR_PLUS	 (RGB(100, 100, 100)) // ���콺�� ���� ������

#define		BUTTON_UNACTION_TEXT_COLOR			(RGB(100, 100, 100))

#define		BOX_BACK_TEXT_COLOR					BUTTON_BACK_TEXT_COLOR
#define		BOX_FRONT_TEXT_COLOR				BUTTON_FRONT_UNSELECTED_TEXT_COLOR

#define		BUTTON_SELECT_BOX_COLOR				(COLOR_YELLOW)
#define		BUTTON_UNSELECT_BOX_COLOR			(255)
#define		BUTTON_DEFAULT_SELECTCOLOR			(248)

#define		BUTTON_DARK_STEP					5

BOOL	_SelectButton::m_PrevMousePressedTime;
BOOL	_SelectButton::m_IsMouseLeftButtonPressed;

//��ư���� ����.
_Button		g_Button[GLOBAL_BUTTON_NUM];

//��ư���� ��������Ʈ.
XSPR		g_Buttons_Spr[TOTAL_BUTTONS_KINDOF_NUM];
XSPR		GOverButtonSpr;

//------------------------------------------------------------------------------------------------
/// _Button::_Button()
/// ���� : ��ư�� �����Ѵ�.
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
/// ���� : �־��� ���� ������ ��ư�� �����Ѵ�.
//------------------------------------------------------------------------------------------------
_Button::_Button(SHORT x, SHORT y, SHORT width, SHORT height, CHAR* lpMenuName, BOOL bAction)
{
	Create(x, y, width, height, lpMenuName, bAction);
}

//------------------------------------------------------------------------------------------------
/// _Button::_Button(SHORT x, SHORT y, SHORT width, SHORT height, CHAR* lpMenuName, WORD flag, BOOL bAction)
/// ���� : �־��� ���� ������ ��ư�� �����Ѵ�.
//------------------------------------------------------------------------------------------------
_Button::_Button(SHORT x, SHORT y, SHORT width, SHORT height, CHAR* lpMenuName, WORD flag, BOOL bAction)
{
	Create(x, y, width, height, lpMenuName, flag, bAction);
}

//------------------------------------------------------------------------------------------------
// robypark 2004/11/15 14:6
// �� ��ư ������ ���̴����� ���´�.
//------------------------------------------------------------------------------------------------
BOOL	_Button::IsVisibleWindow(void)
{
	return m_bVisibleWindow;
}

//------------------------------------------------------------------------------------------------
// robypark 2004/11/15 14:6
// �� ��ư ������ ���̴����� �����Ѵ�.
//------------------------------------------------------------------------------------------------
void	_Button::ShowWindow(BOOL bVisible)
{
	m_bVisibleWindow = bVisible;
}

//------------------------------------------------------------------------------------------------
/// void _Button::Create(INT x, INT y, INT Height, INT Width, CHAR* MenuName, BOOL bAction)
/// ���� : ��ư�� �����Ѵ�.
//------------------------------------------------------------------------------------------------
void _Button::Create(INT x, INT y, INT Width, INT Height, CHAR* MenuName, BOOL bAction)
{	
	Create(x, y, Width, Height, MenuName, BUTTON_PUT_LEFT, bAction);	
}

//------------------------------------------------------------------------------------------------
/// void _Button::Create(INT x, INT y, INT Height, INT Width, CHAR* MenuName, WORD flag, BOOL bAction)
/// ���� : ��ư�� �����Ѵ�.
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
/// ���� : ��ư�� ����� ��´�.
//------------------------------------------------------------------------------------------------
VOID _Button::Put(LPDIRECTDRAWSURFACE7	lpSurface, DWORD PutFlag)
{	
	Put(m_x, m_y, lpSurface, PutFlag);
}

//------------------------------------------------------------------------------------------------
/// VOID _Button::Put(INT x, INT y, LPDIRECTDRAWSURFACE	lpSurface, DWORD PutFlag)
/// ���� : ��ư�� ����� ��´�. ���� ��ǥ�� �Ѱ��ش�.
//------------------------------------------------------------------------------------------------
VOID _Button::Put(INT x, INT y, LPDIRECTDRAWSURFACE7	lpSurface, DWORD PutFlag)
{	
	INT			x1, y1;
	COLORREF	TextColor;		
	BOOL		IsMoveWindow;
	BOOL		IsDrawBorder;
	BOOL		IsEnableDarkText;

	// robypark 2004/11/15 14:8
	// �� ��ư ������ ������ �ʴ� ��� ����
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

	//�׵θ�(�ڽ�)�� �׸���.
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
/// ���� : ��ư�� ����� ��´�.
//------------------------------------------------------------------------------------------------
VOID _Button::Put(LPDIRECTDRAWSURFACE7	lpSurface, BYTE Color, DWORD PutFlag)
{	
	Put(m_x, m_y, lpSurface, Color, PutFlag);
}

//------------------------------------------------------------------------------------------------
/// VOID _Button::Put(INT x, INT y, LPDIRECTDRAWSURFACE	lpSurface, BYTE Color, DWORD PutFlag)
/// ���� : ��ư�� ����� ��´�. ���� ��ǥ�� �Ѱ��ش�.
//------------------------------------------------------------------------------------------------
VOID _Button::Put(INT x, INT y, LPDIRECTDRAWSURFACE7	lpSurface, BYTE Color, DWORD PutFlag)
{	
	INT			x1, y1;	
	COLORREF	TextColor;
	BOOL		IsMoveWindow;
	BOOL		IsDrawBorder;
	BOOL		IsEnableDarkText;
	
	// robypark 2004/11/15 14:8
	// �� ��ư ������ ������ �ʴ� ��� ����
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
	

	//�ڽ��� �׸���. 
	//��� hdc�� Ǭ��.
	//�׸��� ���� �Ǵ�.		
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
/// ���� : x, y�� ��ġ�� �ؽ�Ʈ�� ����Ѵ�.
//------------------------------------------------------------------------------------------------
VOID _Button::Put(SHORT x, SHORT y, XSPR* lpXspr, SHORT FontNumber)
{		
	if(m_bAction==TRUE)	PutSpriteButton(x, y, lpXspr, FontNumber, FALSE, FALSE);	
	else				PutSpriteButton(x, y, lpXspr, FontNumber, TRUE, FALSE);		
}

//------------------------------------------------------------------------------------------------
/// VOID _Button::Put(XSPR* lpXspr, SHORT FontNumber)
/// ���� : x, y�� ��ġ�� �ؽ�Ʈ�� ����Ѵ�.
//------------------------------------------------------------------------------------------------
VOID _Button::Put(XSPR* lpXspr, SHORT FontNumber)
{	
	SHORT	sx, sy;	

	// robypark 2004/11/15 14:8
	// �� ��ư ������ ������ �ʴ� ��� ����
	if (FALSE == m_bVisibleWindow)
		return;
	
	//���� ��ư�� ������ �ְ�.
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
/// ���� : �⺻���� ��ġ�� ��������Ʈ�� ����Ѵ�. 
//------------------------------------------------------------------------------------------------
VOID	_Button::Put1(XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag)
{
	Put1(m_x, m_y, lpXspr, DefaultFontNum, SelectedFontNum, OnMouseFontNum, PutFlag);
}

//------------------------------------------------------------------------------------------------
/// void _Button::Put(SHORT x, SHORT y, XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag)
/// ���� : �⺻���� ��ġ�� ��������Ʈ�� ����Ѵ�. 
//------------------------------------------------------------------------------------------------
VOID	_Button::Put(XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag)
{
	Put(m_x, m_y, lpXspr, DefaultFontNum, SelectedFontNum, OnMouseFontNum, PutFlag);
}

//------------------------------------------------------------------------------------------------
/// void _Button::Put(SHORT x, SHORT y, XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag)
/// ���� : x, y�� ��ġ�� ��������Ʈ�� ����Ѵ�. 
//------------------------------------------------------------------------------------------------
void _Button::Put1(SHORT x, SHORT y, XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag)
{
	INT			x1, y1;		
	BOOL		IsMoveWindow;	
	SHORT		FontNumber;
	
	// robypark 2004/11/15 14:8
	// �� ��ư ������ ������ �ʴ� ��� ����
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
/// ���� : x, y�� ��ġ�� ��������Ʈ�� ����Ѵ�. 
//------------------------------------------------------------------------------------------------
void _Button::Put(SHORT x, SHORT y, XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag)
{
	INT			x1, y1;		
	BOOL		IsMoveWindow;	
	SHORT		FontNumber;
	
	// robypark 2004/11/15 14:8
	// �� ��ư ������ ������ �ʴ� ��� ����
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
//	����	:	��ư�� ����Ѵ� (������ �ؼ� ����ϴ� ����� ���ԵǾ���.)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	_Button::Put(XSPR* lpXspr, SI16 siDefaultFontNum, SI16 siSelectedFontNum, SI16 siOnMouseFontNum, BOOL bDefaultFontTrans, BOOL bSelectedFontTrans, BOOL bOnMouseTrans, DWORD PutFlag)
{
	Put(m_x, m_y, lpXspr, siDefaultFontNum, siSelectedFontNum, siOnMouseFontNum, bDefaultFontTrans, bSelectedFontTrans, bOnMouseTrans, PutFlag);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	��ư�� ����Ѵ� (������ �ؼ� ����ϴ� ����� ���ԵǾ���.)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	_Button::Put(SI16 siX, SI16 siY, XSPR* lpXspr, SI16 siDefaultFontNum, SI16 siSelectedFontNum, SI16 siOnMouseFontNum, BOOL bDefaultFontTrans, BOOL bSelectedFontTrans, BOOL bOnMouseTrans, DWORD PutFlag)
{
	INT			x1, y1;		
	BOOL		IsMoveWindow;	
	BOOL		bTrans = FALSE;
	SHORT		FontNumber;
	
	// robypark 2004/11/15 14:8
	// �� ��ư ������ ������ �ʴ� ��� ����
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
//	����	:
//------------------------------------------------------------------------------------------------
// ������
VOID	_Button::Put2(XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, SHORT NoActionFontNum, DWORD PutFlag)
{
	INT			x1, y1;		
	BOOL		IsMoveWindow;	
	SHORT		FontNumber;
	
	// robypark 2004/11/15 14:8
	// �� ��ư ������ ������ �ʴ� ��� ����
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
//	����	:	��������Ʈ�� �̿��Ͽ� ��ư�� ��´�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	_Button::PutSpriteButton(SI16 siX, SI16 siY, XSPR *pSpr, SI16 siFont, BOOL bDark, BOOL bTrans)
{
	// robypark 2004/11/15 14:8
	// �� ��ư ������ ������ �ʴ� ��� ����
	if (FALSE == m_bVisibleWindow)
		return;

	switch(siFont)
	{
	case -1:			// ��ư�� ���� �ʴ´�.
		break;
	default:			// ��ư�� ��´�.
		if(bTrans == TRUE)
		{
			if(bDark == TRUE)
			{
				// ��Ӱ� ��´�. (���� �������� �ʴ´�.)
				clGrp.PutSpriteLightCT( siX, siY, pSpr->Header.Xsize, pSpr->Header.Ysize, 
						&pSpr->Image[pSpr->Header.Start[siFont]]);				
			}
			else
			{
				// �������� ��´�.
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
//	����	:	��ư ����Լ�.
//				hdc�� �̿��Ѵ� �׷��Ƿ� ���ڰ� ��ư�� �Ѿ�� �߸��� �ʴ´�.
//				�ڽ��� ĥ �� ����. �� �ӵ��� ������.
//	hdc		:	����� HDC.
//	PutFlag	:	��� �Ӽ�.
//------------------------------------------------------------------------------------------------
VOID	_Button::Put1(HDC hdc, DWORD PutFlag)
{
	Put1(hdc, m_x, m_y, PutFlag);
}

//------------------------------------------------------------------------------------------------
//	����	:	��ư ����Լ�.
//				hdc�� �̿��Ѵ� �׷��Ƿ� ���ڰ� ��ư�� �Ѿ�� �߸��� �ʴ´�.
//				�ڽ��� ĥ �� ����. �� �ӵ��� ������.
//	hdc		:	����� HDC.
//	PutFlag	:	��� �Ӽ�.
//------------------------------------------------------------------------------------------------
VOID	_Button::Put(HDC hdc, DWORD PutFlag,  COLORREF OffTextColor, COLORREF OnTextColor)
{
	Put(hdc, m_x, m_y, OffTextColor, OnTextColor, PutFlag);
}

//------------------------------------------------------------------------------------------------
//	����	:	��ư ����Լ�.
//				hdc�� �̿��Ѵ� �׷��Ƿ� ���ڰ� ��ư�� �Ѿ�� �߸��� �ʴ´�.
//				�ڽ��� ĥ �� ����. �� �ӵ��� ������.
//	hdc		:	����� HDC.
//	x, y	:	��� ��ġ.
//	PutFlag	:	��� �Ӽ�.
//------------------------------------------------------------------------------------------------
VOID	_Button::Put(HDC hdc, SHORT x, SHORT y, COLORREF OffTextColor, COLORREF OnTextColor, DWORD PutFlag)
{
	INT			x1, y1;
	COLORREF	TempTextColor;		
	BOOL		IsMoveWindow;	
	BOOL		IsEnableDarkText;

	// robypark 2004/11/15 14:8
	// �� ��ư ������ ������ �ʴ� ��� ����
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
//	����	:	��ư ����Լ�.
//				hdc�� �̿��Ѵ� �׷��Ƿ� ���ڰ� ��ư�� �Ѿ�� �߸��� �ʴ´�.
//				�ڽ��� ĥ �� ����. �� �ӵ��� ������.
//	hdc		:	����� HDC.
//	x, y	:	��� ��ġ.
//	PutFlag	:	��� �Ӽ�.
//------------------------------------------------------------------------------------------------
VOID	_Button::Put1(HDC hdc, SHORT x, SHORT y, DWORD PutFlag)
{
	INT			x1, y1;
	COLORREF	TextColor;		
	BOOL		IsMoveWindow;	
	BOOL		IsEnableDarkText;

	// robypark 2004/11/15 14:8
	// �� ��ư ������ ������ �ʴ� ��� ����
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
//	����	: 
//------------------------------------------------------------------------------------------------
// ������
VOID	_Button::Put(HDC hdc, SI16 siX, SI16 siY, SI16 siWidth, SI16 siHeight, DWORD PutFlag)
{
	INT			x1, y1;
	COLORREF	TextColor;		
	BOOL		IsMoveWindow;	
	BOOL		IsEnableDarkText;

	// robypark 2004/11/15 14:8
	// �� ��ư ������ ������ �ʴ� ��� ����
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
/// ���� : x, y�� ��ġ�� �ؽ�Ʈ�� ����Ѵ�.
//------------------------------------------------------------------------------------------------
VOID	_Button::PutText(INT x, INT y, LPDIRECTDRAWSURFACE7	lpSurface, CHAR *lpText, COLORREF TextColor, WORD TextArrange)
{
	PutText(x, y, m_Width, m_Height, lpSurface, lpText, TextColor, TextArrange);
}

//------------------------------------------------------------------------------------------------
/// VOID	_Button::PutText(INT x, INT y, SHORT Width, SHORT Height, LPDIRECTDRAWSURFACE lpSurface, CHAR *lpText, COLORREF TextColor, WORD TextArrange)
/// ���� : x, y�� ��ġ�� �ؽ�Ʈ�� ����Ѵ�.
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
//	����			:	��ư�� �ؽ�Ʈ�� ����Ѵ�.
//						hdc�� ����ϹǷ� ��ư�� �Ѵ��� �߸����� �ʴ´�.
//	x, y			:	��ư�� ����� ��ǥ.
//	hdc				:	��ư�� ����� hdc.
//	TextColor		:	�ؽ�Ʈ�� ��.
//	TextArrange		:	�ؽ�Ʈ ���� ���.
//------------------------------------------------------------------------------------------------
VOID	_Button::PutText(INT x, INT y, HDC hdc, CHAR *lpText, COLORREF TextColor, WORD TextArrange)
{
	PutText(x, y, m_Width, m_Height, hdc, lpText, TextColor, TextArrange);
}


//------------------------------------------------------------------------------------------------
//	����			:	��ư�� �ؽ�Ʈ�� ����Ѵ�.
//						hdc�� ����ϹǷ� ��ư�� �Ѵ��� �߸����� �ʴ´�.
//	x, y			:	��ư�� ����� ��ǥ.
//	hdc				:	��ư�� ����� hdc.
//	TextColor		:	�ؽ�Ʈ�� ��.
//	TextArrange		:	�ؽ�Ʈ ���� ���.
//------------------------------------------------------------------------------------------------
VOID	_Button::PutTextPlus(INT x, INT y, HDC hdc, CHAR *lpText, COLORREF TextColor, WORD TextArrange)
{
	PutTextPlus(x, y, m_Width, m_Height, hdc, lpText, TextColor, TextArrange);
}
//------------------------------------------------------------------------------------------------
//	����			:	��ư�� �ؽ�Ʈ�� ����Ѵ�.
//						hdc�� ����ϹǷ� ��ư�� �Ѵ��� �߸����� �ʴ´�.
//	x, y			:	��ư�� ����� ��ǥ.
//	Width, Height	:	��ư�� ũ��.
//	hdc				:	��ư�� ����� hdc.
//	TextColor		:	�ؽ�Ʈ�� ��.
//	TextArrange		:	�ؽ�Ʈ ���� ���.
//------------------------------------------------------------------------------------------------
VOID	_Button::PutText(INT x, INT y, INT Width, INT Height, HDC hdc, CHAR *lpText, COLORREF TextColor, WORD TextArrange)
{	
	SHORT		nx, ny;
	SIZE		size;
	COLORREF	OldTextColor;	

	if(!lpText)	
		return;	
		
	GetTextExtentPoint32(hdc, lpText, strlen(lpText), &size);

	// �׸����ǳ��̱��� ó�����ش�.
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
		// m_TextPutX�� m_TextPutY�� ���� �Ǿ� �ִٸ�
		// �� ��ġ�� �ؽ�Ʈ�� ����ϰ�
		// �׷��� �ʴٸ� ��� ���ĵǰ� �ؽ�Ʈ�� ����Ѵ�.						
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
//	����			:	��ư�� �ؽ�Ʈ�� ����Ѵ�.
//						hdc�� ����ϹǷ� ��ư�� �Ѵ��� �߸����� �ʴ´�.
//	x, y			:	��ư�� ����� ��ǥ.
//	Width, Height	:	��ư�� ũ��.
//	hdc				:	��ư�� ����� hdc.
//	TextColor		:	�ؽ�Ʈ�� ��.
//	TextArrange		:	�ؽ�Ʈ ���� ���.
//------------------------------------------------------------------------------------------------
VOID	_Button::PutTextPlus(INT x, INT y, INT Width, INT Height, HDC hdc, CHAR *lpText, COLORREF TextColor, WORD TextArrange)
{	
	SHORT		nx, ny;
	SIZE		size;

	if(!lpText)	
		return;	
		
	GetTextExtentPoint32(hdc, lpText, strlen(lpText), &size);

	// �׸����ǳ��̱��� ó�����ش�.
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
		// m_TextPutX�� m_TextPutY�� ���� �Ǿ� �ִٸ�
		// �� ��ġ�� �ؽ�Ʈ�� ����ϰ�
		// �׷��� �ʴٸ� ��� ���ĵǰ� �ؽ�Ʈ�� ����Ѵ�.						
		nx	=	x + 20;
		ny	=	max(y, y + ((Height / 2) - (size.cy / 2)));
	}

	SetTextColor(hdc, TextColor);
	TextOut(hdc, nx, ny, lpText, strlen(lpText));	

}

//------------------------------------------------------------------------------------------------
/// BOOL _Button::Process(BOOL bPush)
/// ���� : ��ư�� �⺻���� �ϵ��� ó���Ѵ�. ������ ���콺 ���¸� �����ش�.
/// ���ϰ� : ��ư�� ����������.
//------------------------------------------------------------------------------------------------
BOOL _Button::Process(BOOL bPush)
{	
	return Process(m_x, m_y, bPush);
}

//------------------------------------------------------------------------------------------------
/// BOOL _Button::Process(INT x, INT y, BOOL bPush)
/// ���� : ��ư�� �⺻���� �ϵ��� ó���Ѵ�. ������ ���콺 ���¿� ���� ��ġ�� �����ش�.
/// ���ϰ� : ��ư�� ����������.
//------------------------------------------------------------------------------------------------
BOOL _Button::Process(INT x, INT y, BOOL bPush)
{
	// robypark 2004/11/15 14:8
	// �� ��ư ������ ������ �ʴ� ��� ���� FALSE
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
				//���� ��ư�� ������ �־��µ� ��ư�� �� ���¶��
				//��ư�� �����ߴٴ� ���̴�.
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
/// ���� :	���콺�� ���� �ȿ� �ִ����˻��Ѵ�.
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
/// ���� :	���콺�� ���� �ȿ� �ִ����˻��Ѵ�.
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
/// ���� :	���콺�� ���� �ȿ� �ִ����˻��Ѵ�.
//------------------------------------------------------------------------------------------------
BOOL	_Button::IsInMouse()
{
	return IsInMouse(m_x, m_y);
}

//------------------------------------------------------------------------------------------------
/// BOOL	_Button::IsInMouse(INT x, INT y)
/// ���� :	���콺�� ���� �ȿ� �ִ����˻��Ѵ�.
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
/// ���� :	���콺�� ���� �ȿ� �ִ����˻��Ѵ�.
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

//õ���� ��ȭ���� �ִ� �Լ�(2��)
//------------------------------------------------------------------------------------------------
//	����	:	���� ��ư�� ������ �ִ� ������ �˾ƿ´�.
//	bPush	:	���� ���콺 ��ư�� ����.
//	����	:	���� ������ ������ TRUE. �׷��� ������ FALSE.
//------------------------------------------------------------------------------------------------
//õ��
BOOL	_Button::IsPressing(BOOL bPush)
{
	return IsPressing(m_x, m_y, bPush);
}

//------------------------------------------------------------------------------------------------
//	����	:	���� ��ư�� ������ �ִ� ������ �˾ƿ´�.
//	x, y	:	��ư�� ��ǥ.
//	bPush	:	���� ���콺 ��ư�� ����.
//	����	:	���� ������ ������ TRUE. �׷��� ������ FALSE.
//------------------------------------------------------------------------------------------------
//õ��
BOOL	_Button::IsPressing(INT x, INT y, BOOL bPush)
{
	if(IsInMouse(x, y) && bPush && m_bAction && m_bProcess)	
		return TRUE;	
	else
		return FALSE;
}
// ������� �⺻ ��ư

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ���⼭���� ���� ��ư
//------------------------------------------------------------------------------------------------
/// _SelectButton::_SelectButton()
//------------------------------------------------------------------------------------------------
_SelectButton::_SelectButton() : _Button()
{
	m_IsMouseLeftButtonPressed		= FALSE; // ������
	m_PrevMousePressedTime			= 0;
}

//------------------------------------------------------------------------------------------------
/// �Լ� : _SelectButton::_SelectButton(SHORT TextPutX, SHORT TextPutY, SHORT SelectBarX, 
///										SHORT SelectBarY, SHORT SelectBarWidth, SHORT SelectBarHeight, 
///										CHAR* MenuName, BOOL bAction)
//------------------------------------------------------------------------------------------------
_SelectButton::_SelectButton(SHORT TextPutX, SHORT TextPutY, SHORT SelectBarX, SHORT SelectBarY, SHORT SelectBarWidth, SHORT SelectBarHeight, CHAR* MenuName, BOOL bAction)
{
	m_IsMouseLeftButtonPressed		= FALSE; // ������
	m_PrevMousePressedTime			= 0;

	Create(TextPutX, TextPutY, SelectBarX, SelectBarY, SelectBarWidth, SelectBarHeight, MenuName, bAction);
}

//------------------------------------------------------------------------------------------------
/// �Լ� : _SelectButton::_SelectButton(SHORT TextPutX, SHORT TextPutY, SHORT SelectBarX, SHORT SelectBarY, 
///						 SHORT SelectBarWidth, SHORT SelectBarHeight, CHAR* MenuName, 
///						 WORD flag, BOOL bAction)
///
//------------------------------------------------------------------------------------------------
_SelectButton::_SelectButton(SHORT TextPutX, SHORT TextPutY, SHORT SelectBarX, SHORT SelectBarY, SHORT SelectBarWidth, SHORT SelectBarHeight, CHAR* MenuName, WORD flag, BOOL bAction)
{
	m_IsMouseLeftButtonPressed		= FALSE; // ������
	m_PrevMousePressedTime			= 0;

	Create(TextPutX, TextPutY, SelectBarX, SelectBarY, SelectBarWidth, SelectBarHeight, MenuName, flag, bAction);
}
	

//------------------------------------------------------------------------------------------------
/// _SelectButton::_SelectButton()
//------------------------------------------------------------------------------------------------
_SelectButton::~_SelectButton()
{
}

/* õ���� ��ȭ(3�� �Լ�) */
//------------------------------------------------------------------------------------------------
//	����			:	���� ��ư�� �����Ѵ�.
//	x, y			:	��ư�� ��ġ.
//	Width, Height	:	��ư�� ũ��.
//	MenuName		:	��ư�� �̸�.
//	bAction			:	��ư�� Ȱ�� ����(�⺻�� = TRUE).
//------------------------------------------------------------------------------------------------
// õ��
VOID	_SelectButton::Create(INT x, INT y, INT Width, INT Height, CHAR* MenuName, BOOL bAction)
{	
	_SelectButton::Create(x, y, Width, Height, x, y, MenuName, bAction);
	m_bSelected	=	FALSE;	
}

//------------------------------------------------------------------------------------------------
/// �Լ� : VOID _SelectButton::Create(SHORT TextPutX, SHORT TextPutY, SHORT SelectBarX, SHORT SelectBarY, SHORT SelectBarWidth, SHORT SelectBarHeight, CHAR* MenuName, BOOL bAction)
/// ���� : ��ư�� �����Ѵ�.
//------------------------------------------------------------------------------------------------
// õ��
VOID	_SelectButton::Create(INT x, INT y, INT Width, INT Height, INT TextPutX, INT TextPutY, CHAR* MenuName, BOOL bAction)
{
	m_TextPutX	=	TextPutX;
	m_TextPutY	=	TextPutY;	

	_Button::Create(x, y, Width, Height, MenuName, bAction);
	m_bSelected = FALSE;	
}


//------------------------------------------------------------------------------------------------
/// �Լ� : VOID _SelectButton::Create(SHORT TextPutX, SHORT TextPutY, SHORT SelectBarX, SHORT SelectBarY, SHORT SelectBarWidth, SHORT SelectBarHeight, CHAR* MenuName, WORD flag, BOOL bAction)
/// ���� : ��ư�� �����Ѵ�.
//------------------------------------------------------------------------------------------------
// õ��
VOID _SelectButton::Create(INT x, INT y, INT Width, INT Height, INT TextPutX, INT TextPutY, CHAR* MenuName, WORD flag, BOOL bAction)
{
	m_TextPutX	=	TextPutX;
	m_TextPutY	=	TextPutY;

	_Button::Create(x, y, Width, Height, MenuName, flag, bAction);
	m_bSelected = FALSE;
}


//------------------------------------------------------------------------------------------------
/// void _SelectButton::Put(HDC hdc, DWORD PutFlag, DWORD PutTextFlag)
/// ���� : ���� ��ư�� �׵θ��� ȭ�鿡 �׸���.
//------------------------------------------------------------------------------------------------
void _SelectButton::Put(LPDIRECTDRAWSURFACE7	lpSurface, DWORD PutFlag, DWORD PutTextFlag)
{		
	Put(m_TextPutX, m_TextPutY, m_x, m_y, lpSurface, PutFlag, PutTextFlag);
}

/* ������(3�� �Լ�) */
//------------------------------------------------------------------------------------------------
/// VOID _SelectButton::Put(SHORT TextPutX, SHORT TextPutY, SHORT SelectBarX, SHORT SelectBarY, 
///							LPDIRECTDRAWSURFACE	lpSurface, DWORD PutFlag, DWORD PutTextFlag)
/// ���� : ��ư�� ȭ�鿡 �׸���.
//------------------------------------------------------------------------------------------------
// ������
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
	

	//�ڽ��� �׸���. 
	//��� hdc�� Ǭ��.
	//�׸��� ���� �Ǵ�.		
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
//	����	:	
//------------------------------------------------------------------------------------------------
// ������
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
//	����	:	
//------------------------------------------------------------------------------------------------
// ������
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
				TextColor = BUTTON_FRONT_SELECTED_TEXT_COLOR;			// ���ýÿ� �������� ������
			else
				TextColor = BUTTON_FRONT_UNSELECTED_TEXT_COLOR_PLUS;	// ���ÿ� �������� ��
		}
		else
		{
			if(m_bProcess == TRUE)
			{
				if(m_bSelected == TRUE)
					TextColor = BUTTON_FRONT_SELECTED_TEXT_COLOR;	// ���õǾ�����
				else if(IsInMouse()==TRUE)
					TextColor = BUTTON_FRONT_ONMOUSE_TEXT_COLOR_PLUS;	// ���콺�� ���� ������(����� ���)
				else
					TextColor = BUTTON_FRONT_UNSELECTED_TEXT_COLOR_PLUS;	// ���ÿ� �������� �� 
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

/* õ���� ��ȭ(4�� �Լ�) */
//------------------------------------------------------------------------------------------------
//	����					:	���� ��ư�� �׸���.
//	x, y					:	��ư�� ��ǥ.
//	TextPutX, TextPutY		:	���� ��ư ������ �ؽ�Ʈ�� ���� ��ġ.
//								(���ǽø� �������� �����ǥ���ƴ� ������ǥ�̴�.)
//	lpSurface				:	��ư�� �׸� ���ǽ�.
//	pXspr, FontNum			:	������ �׸��� �׸��� ���� ��������Ʈ.
//	PutFlag					:	�Ϲ� ��ư ��� �ɼ�.
//	PutTextFlag				:	��ư �� �ؽ�Ʈ ��� �ɼ�.
//------------------------------------------------------------------------------------------------
// õ��
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
	

	//�ڽ��� �׸���. 
	//��� hdc�� Ǭ��.
	//�׸��� ���� �Ǵ�.		
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
//	����	:	��ư ��� �Լ�.
//------------------------------------------------------------------------------------------------
// õ��
VOID	_SelectButton::Put(SHORT x, SHORT y, XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag)
{
	_Button::Put(x, y, lpXspr, DefaultFontNum, SelectedFontNum, OnMouseFontNum, PutFlag);
}
						
//------------------------------------------------------------------------------------------------
//	����	:	��ư ��� �Լ�.
//------------------------------------------------------------------------------------------------
// õ��
VOID	_SelectButton::Put(XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag)
{
	_Button::Put(lpXspr, DefaultFontNum, SelectedFontNum, OnMouseFontNum, PutFlag);
}

//------------------------------------------------------------------------------------------------
//	����				:	��ư ��� �Լ�.
//	x, y				:	x, y ��ǥ.
//	TextPutX, TextPutY	:	�ؽ�Ʈ ��� ��ǥ.
//	hdc					:	DC.
//	PutTextFlag			:	�������� �ɼ�.
//------------------------------------------------------------------------------------------------
// õ��
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
/// ���� : �� ��ư�� �����Ѵ�.
//------------------------------------------------------------------------------------------------
void _SelectButton::SetSelected(BOOL bSelected)
{
	m_bSelected=bSelected;
}

//------------------------------------------------------------------------------------------------
/// BOOL _SelectButton::IsSelected()
/// ���� : �� ��ư�� �����Ѵ�.
//------------------------------------------------------------------------------------------------
BOOL _SelectButton::IsSelected()
{
	return m_bSelected;	
}

/*õ���� �Լ�(2���Լ�)*/
//------------------------------------------------------------------------------------------------
//	����			:	SelectButton�� ó�� �Լ�. m_PrevMousePressedTime
//	bPush			:	���� ���콺�� ���� ����.
//	pbIsDoubleClick	:	���� Ŭ���� �Ͽ�����.
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
//	����			:	SelectButton�� ó�� �Լ�.
//	x, y			:	��ǥ.
//	bPush			:	���� ���콺�� ���� ����.
//	pbIsDoubleClick	:	���� Ŭ���� �Ͽ�����.
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


/* ������(1�� �Լ�) */
//------------------------------------------------------------------------------------------------
/// BOOL _SelectButton::IsDoubleClick()
/// ���� : ���� Ŭ���� �ߴ��� �˻��Ѵ�.
//------------------------------------------------------------------------------------------------
// ������
BOOL _SelectButton::IsDoubleClick()
{
	DWORD		dwCurrentTime = timeGetTime();

	if((m_bAction == TRUE) && (m_bProcess == TRUE))
	{
		if(m_IsMouseLeftButtonPressed)				//���� ���콺�� ������.
		{
			if(IpD.LeftPressSwitch==FALSE)				//���콺�� ��������.	�� �ѹ� Ŭ���ߴ�.
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
/// �Լ� : VOID _SelectButton::SetTextPutX(SHORT TextPutX)
/// ���� : ��ư�� �����Ѵ�.
//------------------------------------------------------------------------------------------------
VOID _SelectButton::SetTextPutX(SHORT TextPutX)
{
	m_TextPutX	=	TextPutX;
}

//------------------------------------------------------------------------------------------------
/// �Լ� : VOID _SelectButton::SetTextPutY(SHORT TextPutY)
/// ���� : ��ư�� �����Ѵ�.
//------------------------------------------------------------------------------------------------
VOID _SelectButton::SetTextPutY(SHORT TextPutY)
{
	m_TextPutY	=	TextPutY;
}

// ������� ���� ��ư

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* �����Ͽ��� ����(3�� �Լ�) */
//------------------------------------------------------------------------------------------------
/// _ServiceProviderButton::_ServiceProviderButton()
/// ���� : ���� ���ι��̴� ��ư�� ������.
//------------------------------------------------------------------------------------------------
// ������
_ServiceProviderButton::_ServiceProviderButton() : _SelectButton()
{	
	m_bSelected=FALSE;
}

//------------------------------------------------------------------------------------------------
/// _ServiceProviderButton::_ServiceProviderButton(SHORT x, SHORT y, SHORT width, SHORT height, CHAR* lpMenuName)
/// ���� : ���� ���ι��̴� ��ư�� ������. �־��� ��ġ�� ũ��� ��ư�� �����Ѵ�.
//------------------------------------------------------------------------------------------------
// ������
_ServiceProviderButton::_ServiceProviderButton(SHORT x, SHORT y, SHORT width, SHORT height, CHAR* lpMenuName)  
{
	_Button::_Button(x, y, width, height, lpMenuName);	
	m_bSelected=FALSE;
}

//------------------------------------------------------------------------------------------------
/// _ServiceProviderButton::~_ServiceProviderButton()
/// ���� : ���� ���ι��̴� ��ư�� �Ҹ���. �Ҵ� ���� �޸𸮸� ���� �Ѵ�.
//------------------------------------------------------------------------------------------------
// ������
_ServiceProviderButton::~_ServiceProviderButton()
{	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* �����Ͽ��� �ִ� �κ�*/
//------------------------------------------------------------------------------------------------
/// _EnumSessionButton::__EnumSessionButton()
/// ���� : ���� ���ι��̴� ��ư�� ������.
//------------------------------------------------------------------------------------------------
// ������
_EnumSessionButton::_EnumSessionButton() : _SelectButton()
{
}

//------------------------------------------------------------------------------------------------
/// _EnumSessionButton::_EnumSessionButton(SHORT x, SHORT y, SHORT width, SHORT height, CHAR* lpMenuName)
/// ���� : ���� ���ι��̴� ��ư�� ������. �־��� ��ġ�� ũ��� ��ư�� �����Ѵ�.
//------------------------------------------------------------------------------------------------
// ������
_EnumSessionButton::_EnumSessionButton(SHORT x, SHORT y, SHORT width, SHORT height, CHAR* lpMenuName)  
{
	_Button::_Button(x, y, width, height,lpMenuName);	
}

//------------------------------------------------------------------------------------------------
/// _EnumSessionButton::~_EnumSessionButton()
/// ���� : ���� ���ι��̴� ��ư�� �Ҹ���. �Ҵ� ���� �޸𸮸� ���� �Ѵ�.
//------------------------------------------------------------------------------------------------
// ������
_EnumSessionButton::~_EnumSessionButton()
{	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* �����Ͽ��� ���� */
//------------------------------------------------------------------------------------------------
/// �Լ� : _SelectMapFileButton::_SelectMapFileButton()
/// ���� : 
//------------------------------------------------------------------------------------------------
// ������
_SelectMapFileButton::_SelectMapFileButton() : _SelectButton()
{
}

//------------------------------------------------------------------------------------------------
/// �Լ� : _SelectMapFileButton::_SelectMapFileButton(SHORT x, SHORT y, SHORT width, SHORT height, CHAR* lpMenuName)
/// ���� : 
//------------------------------------------------------------------------------------------------
// ������
_SelectMapFileButton::_SelectMapFileButton(SHORT x, SHORT y, SHORT width, SHORT height, CHAR* lpMenuName)
{
	_Button::_Button(x, y, width, height,lpMenuName);	
}

//------------------------------------------------------------------------------------------------
/// �Լ� : _SelectMapFileButton::~_SelectMapFileButton()
/// ���� : 
//------------------------------------------------------------------------------------------------
// ������
_SelectMapFileButton::~_SelectMapFileButton()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------
/// �Լ� : _Box::_Box()
/// ���� : �ڽ��� ������.
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
/// �Լ� : _Box::_Box(SHORT x, SHORT y, SHORT Width, SHORT Height, CHAR *lpName, WORD PutFlag)
/// ���� : �ڽ��� ������.
//------------------------------------------------------------------------------------------------
_Box::_Box(SHORT x, SHORT y, SHORT Width, SHORT Height, CHAR* lpName, WORD PutFlag)
{
	Create(x, y, Width, Height, lpName, PutFlag);
}

//------------------------------------------------------------------------------------------------
/// �Լ� : _Box::~_Box()
/// ���� : �ڽ��� �Ҹ���
//------------------------------------------------------------------------------------------------
_Box::~_Box()
{
}

//------------------------------------------------------------------------------------------------
/// �Լ� : void	_Box::Create(SHORT x, SHORT y, SHORT Width, SHORT Height, CHAR *lpName, WORD PutFlag)
/// ���� : �ڽ��� �����Ѵ�.
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
/// �Լ� : void _Box::Put(LPDIRECTDRAWSURFACE	lpSurface, DWORD PutFlag, WORD PutTextFlag)
/// ���� : �ڽ��� ����Ѵ�. 
/// ���� : PutFlag - ���ڸ� ��� ����� ������ ..
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
/// �Լ� : void _Box::Put(HDC	hdc, DWORD PutFlag, WORD PutTextFlag)
/// ���� : �ڽ��� ����Ѵ�. 
/// ���� : PutFlag - ���ڸ� ��� ����� ������ ..
//------------------------------------------------------------------------------------------------
void _Box::Put(HDC	hdc, DWORD PutFlag, WORD PutTextFlag)
{	
	PutText(m_x, m_y, hdc, m_Name, PutTextFlag);
}

//------------------------------------------------------------------------------------------------
/// �Լ� : VOID _Box::PutText(INT x, INT y, LPDIRECTDRAWSURFACE	lpSurface, CHAR *lpText, WORD TextArrange)
/// ���� : �ڽ��� �Ҹ���
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
/// �Լ� : VOID _Box::PutText(INT x, INT y, HDC	hdc, CHAR *lpText, WORD TextArrange)
/// ���� : 
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
/// �Լ� : SHORT _Box::GetX()
/// ���� : 
//------------------------------------------------------------------------------------------------
SHORT _Box::GetX()
{
	return m_x;
}


//------------------------------------------------------------------------------------------------
/// �Լ� : SHORT _Box::GetY()
/// ���� : 
//------------------------------------------------------------------------------------------------
SHORT _Box::GetY()
{
	return m_y;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------
/// �Լ� : _ImageButton::_ImageButton()
/// ���� : 
//------------------------------------------------------------------------------------------------
_ImageButton::_ImageButton() : _Button()
{
	m_lpSampleBuffer	=	NULL;
	m_SelectColor		=	NULL;
	m_SampleXsize		=	0;
}

//------------------------------------------------------------------------------------------------
/// �Լ� : _ImageButton::~_ImageButton()
/// ���� : 
//------------------------------------------------------------------------------------------------
_ImageButton::~_ImageButton()
{	
}

///------------------------------------------------------------------------------------------------------
// ����				: �̹��� ��ư�� �����Ѵ�.
// x, y				: �̹��� ��ư�� ���� ��ġ
// width, height	: �̹��� ��ư�� ũ��
// lpSampleBuffer	: �̹��� ��ư�� �����ϱ� ���Ͽ� �ܻ����� �׷� ���� ����
// Color			: lpSampleBuffer���� ��ư�� �����ϱ� ���Ͽ� �ܻ��� �ȷ�Ʈ �ε���.
// nSampleXsize		: Sample ������ ���� ������.
// uiFlag           : ���� �ɼ�.
///------------------------------------------------------------------------------------------------------
VOID _ImageButton::Create(SHORT x, SHORT y, SHORT Width, SHORT Height, BYTE* lpSampleBuffer, BYTE Color, SHORT nSampleXsize, WORD uiFlag)
{
	m_lpSampleBuffer	=	lpSampleBuffer;
	m_SelectColor		=	Color;
	m_SampleXsize		=	nSampleXsize;
	
	_Button::Create(x, y, Width, Height, "", (WORD)uiFlag);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�̹��� ��ư�� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	_ImageButton::Create(SHORT x, SHORT y, SHORT Width, SHORT Height, CHAR* MenuName, BYTE* lpSampleBuffer, BYTE Color, WORD uiFlag)
{
	m_lpSampleBuffer	=	lpSampleBuffer;
	m_SelectColor		=	Color;
	m_SampleXsize		=	Width;

	_Button::Create(x, y, Width, Height, MenuName, (WORD)uiFlag);
}

//------------------------------------------------------------------------------------------------
/// �Լ� : BOOL _ImageButton::IsInMouse()
/// ���� : 
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
/// �Լ� : BOOL _ImageButton::Process(BOOL bPush)
/// ���� : 
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
				//���� ��ư�� ������ �־��µ� ��ư�� �� ���¶��
				//��ư�� �����ߴٴ� ���̴�.
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
/// ���� : �⺻���� ��ġ�� ��������Ʈ�� ����Ѵ�. 
//------------------------------------------------------------------------------------------------
VOID	_ImageButton::Put(XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag)
{
	Put(m_x, m_y, lpXspr, DefaultFontNum, SelectedFontNum, OnMouseFontNum, PutFlag);
}

//------------------------------------------------------------------------------------------------
/// void _ImageButton::Put(SHORT x, SHORT y, XSPR* lpXspr, SHORT DefaultFontNum, SHORT SelectedFontNum, SHORT OnMouseFontNum, DWORD PutFlag)
/// ���� : x, y�� ��ġ�� ��������Ʈ�� ����Ѵ�. 
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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
// Desc: ��ư�� ��ġ �� ũ�⸦ ���Ѵ�.
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
///	�Լ� : VOID	SetSetActionAllButton(BOOL bAction)
/// ���� : ��� ��ư�� ��Ƽ�� ��Ų��.
//------------------------------------------------------------------------------------------------
VOID	SetActionAllButton(BOOL bAction)
{
	INT		i;

	for(i=0; i < GLOBAL_BUTTON_NUM; i++)	
		g_Button[i].SetAction(bAction);	
}


//------------------------------------------------------------------------------------------------
///	�Լ� : BOOL	LoadAllButtons()
/// ���� : ��� ��ư�� �ε� �Ѵ�.
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
///	�Լ� : VOID	FreeAllButtons()
/// ���� : ��� ��ư�� ���� �Ѵ�.
//------------------------------------------------------------------------------------------------
VOID	FreeAllButtons()
{
	SHORT i;

	for(i=0; i < TOTAL_BUTTONS_KINDOF_NUM; i++)	
		if(g_Buttons_Spr[i].Image) clGrp.FreeXspr(g_Buttons_Spr[i]);	

	if(GOverButtonSpr.Image) clGrp.FreeXspr(GOverButtonSpr);
}




