#include <GSL.h>
#include <graphics16.h>

#include <main.h>
#include <Mouse.h>

#include <etc.h>
#include "music.h"

#include "YListBoxControl.h"
#include <map.h>

#include <MapInterface.h>

extern	char 				SavePath[];
extern	_Map 				Map;
extern	cltMapInterface		clifMap;							// �ʰ��� �������̽� 
extern _InputDevice				IpD;

//����Ʈ �ڽ��� ���� �������� �ʱ�ȭ �Ѵ�..

// actdoll (2004/06/29 16:30) : �����߰�! ���� �ȵǴ� ��������� ��¿ �� ����.
XSPR	_YListBox::m_UpDownSpr		= {0,};
XSPR	_YListBox::m_BarSpr			= {0,};
SHORT	_YListBox::m_nReferenceCount = 0;


///////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : _YListBox::_YListBox() 
/// ���� : ����Ʈ �ڽ��� ������.
///////////////////////////////////////////////////////////////////////////////////////////////////
_YListBox::_YListBox() : _YControl()
{	
	m_SellWidth					=	0;
	m_SellHeight				=	0;
	m_SellNum					=	0;	
	m_SelectedSell				=	-1;		
	m_bAction					=	FALSE;
	m_bProcess					=	FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : _YListBox::~_YListBox()
/// ���� : ����Ʈ �ڽ��� �Ҹ���.
///////////////////////////////////////////////////////////////////////////////////////////////////
_YListBox::~_YListBox()
{
	if(m_UpDownSpr.Image)	clGrp.FreeXspr(m_UpDownSpr);			
	if(m_BarSpr.Image)		clGrp.FreeXspr(m_BarSpr);						
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	����Ʈ �ڽ��� �����Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void	_YListBox::Create(SHORT x, SHORT y, SHORT width, SHORT height, SHORT textx, SHORT texty, SHORT textwidth, SHORT textheight,  SHORT SellWidth, SHORT SellHeight,
						  SI16 siUpDefaultFont, SI16 siUpPressedFont, SI16 siDownDefaultFont, SI16 siDownPressedFont, SI16 siScrollBarStartFont)
{
	int		i;
	CHAR	szFileNameBuffer[1024];

	if(m_UpDownSpr.Image == NULL)
	{
		GetFileNamePath("yfnt\\scroll-up-down.spr", SavePath, szFileNameBuffer);		
		if(clGrp.LoadXspr(szFileNameBuffer, m_UpDownSpr) == FALSE)
			clGrp.Error("FKJE8567", "YYLISTBOXCONTROL0000002 [%s]", szFileNameBuffer);			
	}

	if(m_BarSpr.Image == NULL)
	{
		GetFileNamePath("yfnt\\scrollbar.spr", SavePath, szFileNameBuffer);
		
		if(clGrp.LoadXspr(szFileNameBuffer, m_BarSpr) == FALSE)		
			clGrp.Error("FKJE8567", "YYLISTBOXCONTROL0000004 [%s]", szFileNameBuffer);	
	}	

	m_bAction					=	TRUE;
	m_bProcess					=	TRUE;

	m_x							=	x;
	m_y							=	y;
	m_Controlx					=	width;
	m_Controly					=	height;

	
	m_PutTextX					=	textx;
	m_PutTextY					=	texty;
	m_PutTextWidth				=	textwidth;
	m_PutTextHeight				=	textheight;

	m_SellWidth					=	SellWidth - m_BarSpr.Header.Xsize;
	m_SellHeight				=	SellHeight;
	m_SellNum					=	0;
	m_SelectedSell				=	-1;
	m_StartSellNumber			=	0;
	m_siScrollBarStartFont		=	siScrollBarStartFont;

	m_siUpDefaultFont			=	siUpDefaultFont;
	m_siUpPressedFont			=	siUpPressedFont;
	m_siDownDefaultFont			=	siDownDefaultFont;
	m_siDownPressedFont			=	siDownPressedFont;

	//����Ʈ �ڽ����� �ѹ��� �������� ���� ����.
	m_VisibleSellNum			=	m_PutTextHeight / m_SellHeight;	

	for(i=0; i < MAX_LIST_BOX_SELL; i++)	
	{		
		m_lplpData[i]		= NULL;
		ZeroMemory(m_lplpSellName[i], sizeof(m_lplpSellName[i]));
	}

	//���� ���ϴ� ��ư�� ��ġ�� ���Ѵ�.
	m_rcUpButton.left			= (x + width) - m_UpDownSpr.Header.Xsize;
	m_rcUpButton.top			= y;
	m_rcUpButton.right			= (x + width);
	m_rcUpButton.bottom			= y + m_UpDownSpr.Header.Ysize;

	//�Ʒ��� ���ϴ� ��ư�� ��ġ�� ���Ѵ�.
	m_rcDownButton.left			= (x + width) - m_UpDownSpr.Header.Xsize;
	m_rcDownButton.top			= (y + height) - m_UpDownSpr.Header.Ysize; 
	m_rcDownButton.right		= (x + width);
	m_rcDownButton.bottom		= (y + height);

	//����Ʈ���� ��ġ�� ���Ѵ�.
	m_rcBarInfo.left			= m_rcUpButton.left;
	m_rcBarInfo.top				= m_rcUpButton.bottom;
	m_rcBarInfo.right			= m_rcUpButton.right;
	m_rcBarInfo.bottom			= m_rcDownButton.top;				
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����Ʈ �ڽ��� ó������ �ʴ´�.(�ð������ε� ��Ÿ����)
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	_YListBox::SetAction(BOOL bAction)
{
	m_bAction	=	bAction;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����Ʈ �ڽ��� ó������ �ʴ´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	_YListBox::SetProcess(BOOL bProcess)
{
	m_bProcess	=	bProcess;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : void _YListBox::RefreshData()
/// ���� : ����Ÿ�� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////////////////////////
void    _YListBox::RefreshData()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : void _YListBox::Draw()
/// ���� : ȭ�鿡 �׸���.
///////////////////////////////////////////////////////////////////////////////////////////////////
void _YListBox::Draw()
{	
	SHORT		i;		
	HDC			hdc;		

	//����Ʈ �ڽ� �ȿ� �ִ� �ؽ�Ʈ�� ����Ѵ�.
	if(clGrp.lpDDSBack->GetDC(&hdc) == DD_OK)
	{
		for(i=0; ((i+m_StartSellNumber) < m_SellNum) && (i < m_VisibleSellNum); i++)					
		{				
			TextOut(hdc, m_PutTextX, m_PutTextY + (m_SellHeight * i), 
						m_lplpSellName[i + m_StartSellNumber], strlen(m_lplpSellName[i + m_StartSellNumber]));				
		}

		clGrp.lpDDSBack->ReleaseDC(hdc);			
	}

	DrawScrollBar();
}
	

///////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : void _YListBox::Action(BOOL bPush)
/// ���� : �׻� �ؾ� ���ϵ��� ó���� �ش�.
///////////////////////////////////////////////////////////////////////////////////////////////////
void _YListBox::Action(BOOL bPush)
{	
	// m_bAction�̳� m_bProcess ������ �ϳ��� FALSE�̶�� �۵� �Ҵ� �����̴�,
	if(m_bAction == FALSE || m_bProcess == FALSE)
		return;

	m_bUpPressed = FALSE;
	m_bDownPressed = FALSE;

	if(IsPressedUpButton() == TRUE)	
	{	
		m_bUpPressed = TRUE;
		PrevSell();	
	}
	else if(IsPressedDownButton() == TRUE)	
	{
		m_bDownPressed = TRUE;
		NextSell();	
	}

	if(IsPressedPageUpButton() == TRUE)	
	{			
		PrevPage();				
	}
	else if(IsPressedPageDownButton() == TRUE)	
		NextPage();		
}


///////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : SRHOT _YListBox::AddSell(LPVOID lpData, char* SellName)
/// ���� : �׻� �ؾ� ���ϵ��� ó���� �ش�.
///////////////////////////////////////////////////////////////////////////////////////////////////
SHORT  _YListBox::AddSell(LPVOID lpData, char* SellName)
{	
	if(MAX_LIST_BOX_SELL <= m_SellNum)
	{
		clGrp.Error("FKJE8567", "YYLISTBOXCONTROL0000005");		
		return -1;
	}
	else
	{
		m_lplpData[m_SellNum]		= lpData;		

		strcpy(m_lplpSellName[m_SellNum], SellName);
		m_SellNum++;
	}

	return m_SellNum;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : void _YListBox::DelSell(SI16 siIndex)
/// ���� : ���� ����Ʈ���� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////////////////////////
void _YListBox::DelSell(SI16 siIndex)
{	
	UI32		i;

	if(IsValidIndex(siIndex)==TRUE)
	{	
		// siIndex ���� �� ĭ�� ������ ����.		
		for(i = siIndex; i < (UI32)(m_SellNum - 1); i++)
		{
			m_lplpData[i] = m_lplpData[i + 1];
			strcpy(m_lplpSellName[i], m_lplpSellName[i + 1]);				
		}	
	
		// ������ ���� NULL�� ä���.
		m_lplpData[m_SellNum - 1] = NULL;
		ZeroMemory(m_lplpSellName[m_SellNum - 1], sizeof(m_lplpSellName[m_SellNum]));	

		// ���� �ϳ� �����Ѵ�.
		m_SellNum--;	
	}	
}	

///////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : LPVOID _YListBox::GetData(SHORT SellIndex)
/// ���� : ���� ��ȣ�� ����Ÿ�� ��� �´�.
///////////////////////////////////////////////////////////////////////////////////////////////////
LPVOID _YListBox::GetData(SHORT SellIndex)
{
	if(IsValidIndex(SellIndex)==TRUE)	
		return m_lplpData[SellIndex];	
	else
		return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : BOOL _YListBox::IsValidIndex(SHORT Index)
/// ���� : ��ȿ�� �ε��������� ����.
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL _YListBox::IsValidIndex(SHORT Index)
{
	//���� ��ȿ�� ���������� üũ�Ѵ�.
	if((Index >= 0) && (Index < MAX_LIST_BOX_SELL))
	{
		if(Index < m_SellNum)		
			return TRUE;
		else
			return FALSE;		
	}

	return FALSE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : SHORT	_YListBox::GetWidth()
/// ���� : ��Ʈ���� ���̸� ���Ѵ�.
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT	_YListBox::GetWidth()
{
	return m_Controlx;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : SHORT	_YListBox::GetHeight()
/// ���� : ��Ʈ���� ���̸� ���Ѵ�.
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT	_YListBox::GetHeight()
{
	return m_Controly;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : SHORT _YListBox::GetSellWidth()
/// ���� : �� �ϳ��� ���� ���Ѵ�.
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT _YListBox::GetSellWidth()
{
	return m_SellWidth;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : SHORT _YListBox::GetSellHeight()
/// ���� : �� �ϳ��� ���� ���Ѵ�.
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT _YListBox::GetSellHeight()
{
	return m_SellHeight;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : SHORT _YListBox::GetSellNum()
/// ���� : �� ���� ������ ���Ѵ�.
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT _YListBox::GetSellNum()
{
	return m_SellNum;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : void _YListBox::SetPosition(SHORT NewPosition)
/// ���� : ���ο� ��ġ�� �����Ѵ�.
////////////////////////////////////////////////////////////////////////////////////////////////////
void _YListBox::SetPosition(SHORT NewPosition)
{
	if(IsValidIndex(NewPosition) == TRUE)
		m_SelectedSell = NewPosition;		
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : SHORT  _YListBox::GetCurrentPosition()
/// ���� : ������ ��ġ�� ���´�.
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT  _YListBox::GetCurrentPosition()
{
	if(IsValidIndex(m_SelectedSell))
		return m_SelectedSell;
	else
		return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : SHORT  _YListBox::NextSell()
/// ���� : ���� ���� �̵��Ѵ�.
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT  _YListBox::NextSell()
{
	if((m_SellNum - m_VisibleSellNum) > m_StartSellNumber)
				m_StartSellNumber++;					
	
	return m_SelectedSell;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : SHORT  _YListBox::PrevSell()
/// ���� : ���� ���� �̵��Ѵ�.
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT  _YListBox::PrevSell()
{
	if(m_StartSellNumber > 0)
		m_StartSellNumber--;

	return m_SelectedSell;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : SHORT  _YListBox::GetVisibleSellNum()
/// ���� : �� ���� ����Ʈ ��Ͽ� �������� ���� ������ ���Ѵ�.
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT  _YListBox::GetVisibleSellNum()
{
	return m_VisibleSellNum;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : SHORT  _YListBox::GetStartSellNumber()
/// ���� : ����Ʈ �ڽ� ó������ �����־�� �ϴ� ���� ��ȣ.
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT   _YListBox::GetStartSellNumber()
{
	return m_StartSellNumber;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : BOOL	_YListBox::IsTopSell()
/// ���� : ����Ʈ �ڽ��� ������(0�� �ε����� �ƴ϶�) �ִ���..
/////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	_YListBox::IsTopSell()
{
	if(m_StartSellNumber == m_SelectedSell)	
		return TRUE;	
	else
		return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : BOOL	_YListBox::IsBottomSell()
/// ���� : ����Ʈ �ڽ��� �ǾƷ�(�������� �ε����� �ƴ϶�) �ִ���..
/////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	_YListBox::IsBottomSell()
{
	if((m_StartSellNumber + m_VisibleSellNum - 1) == m_SelectedSell)
		return TRUE;
	else
		return FALSE;	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : BOOL	_YListBox::IsPressedUpButton()
/// ���� : ���� ���ϴ� ��ư�� ������������.. üũ�Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	_YListBox::IsPressedUpButton()
{
	static		DWORD	dwPrevPressedTime;

	if(IpD.LeftPressSwitch == TRUE && (timeGetTime() - dwPrevPressedTime > LISTBOX_SERVICEPROVIDER_UPDOWN_PRESS_DELAY) )
	{
		if((m_rcUpButton.left < IpD.Mouse_X) && (m_rcUpButton.right > IpD.Mouse_X) &&	
			(m_rcUpButton.top < IpD.Mouse_Y) && (m_rcUpButton.bottom > IpD.Mouse_Y))
		{
			PushEffect(EFFECT_TITLEBUTTON, 0, 0);
			dwPrevPressedTime = timeGetTime();
			return TRUE;
		}
	}

	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : BOOL	_YListBox::IsPressedDownButton()
/// ���� : ���� ���ϴ� ��ư�� ������������.. üũ�Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	_YListBox::IsPressedDownButton()
{
	static		DWORD	dwPrevPressedTime;

	if(IpD.LeftPressSwitch == TRUE && (timeGetTime() - dwPrevPressedTime > LISTBOX_SERVICEPROVIDER_UPDOWN_PRESS_DELAY) )
	{
		if((m_rcDownButton.left < IpD.Mouse_X) && (m_rcDownButton.right > IpD.Mouse_X) &&	
			(m_rcDownButton.top < IpD.Mouse_Y) && (m_rcDownButton.bottom > IpD.Mouse_Y))
		{
			PushEffect(EFFECT_TITLEBUTTON, 0, 0);
			dwPrevPressedTime = timeGetTime();
			return TRUE;
		}
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : RECT	_YListBox::GetBarInfo()
/// ���� : ���� ũ�� �� ��ġ�� ���Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
RECT	_YListBox::GetBarInfo()
{
	SHORT					TotalHeight;
	SHORT					ViewHeight;
	SHORT					BarHeight;
	RECT					Rect;
			
	if(m_SellNum != 0)
	{
		TotalHeight				=	m_SellHeight * m_SellNum;
		ViewHeight				=	m_SellHeight * m_VisibleSellNum;
		BarHeight				=	SHORT(m_rcBarInfo.bottom - m_rcBarInfo.top);

		if(TotalHeight < ViewHeight)
			TotalHeight = ViewHeight;	

		//�켱 ���̸� �����Ѵ�.
		Rect.bottom				=	ViewHeight * BarHeight / TotalHeight;		

		if(Rect.bottom < CONTROL_SCROLL_MIN_HEIGHT)
			Rect.bottom = CONTROL_SCROLL_MIN_HEIGHT;
		
		//���� ��ġ�� �����Ѵ�.
		Rect.left				=	m_rcBarInfo.left;	
		Rect.right				=	m_rcBarInfo.right;

		
		Rect.top				=	min((m_rcBarInfo.top) + (((m_SellHeight * m_StartSellNumber) * BarHeight) / TotalHeight), 
										m_rcBarInfo.bottom - CONTROL_SCROLL_MIN_HEIGHT);
		Rect.bottom				+=	Rect.top;	
	}
	else
	{
		
		Rect.left				=	m_rcBarInfo.left;
		Rect.top				=	m_rcBarInfo.top;
		Rect.right				=	m_rcBarInfo.right;
		Rect.bottom				=	m_rcBarInfo.bottom;
	}
	

	return Rect;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID	_YListBox::DrawScrollBar()
/// ���� : ��ũ�� �ٸ� �׸���.
/////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	_YListBox::DrawScrollBar()
{
	RECT	rect;
	SHORT	Height;
	SHORT	i;
	
	if(clGrp.LockSurface(SURFACE_BACK)==TRUE)
	{
		if(m_bUpPressed)
		{
			//�� ��ư�� �׸���.
			clGrp.PutSpriteT(m_rcUpButton.left, m_rcUpButton.top, 
						m_UpDownSpr.Header.Xsize,
						m_UpDownSpr.Header.Ysize,
						&m_UpDownSpr.Image[m_UpDownSpr.Header.Start[m_siUpPressedFont]]);
		}
		else
		{
			//�� ��ư�� �׸���.
			clGrp.PutSpriteT(m_rcUpButton.left, m_rcUpButton.top, 
						m_UpDownSpr.Header.Xsize,
						m_UpDownSpr.Header.Ysize,
						&m_UpDownSpr.Image[m_UpDownSpr.Header.Start[m_siUpDefaultFont]]);
		}		

		
		//�ٸ� �׸���.			
		rect = GetBarInfo();	
		
		//�� �κ� �� �Ʒ� �κ��� ������ ũ�⸦ ���Ѵ�.
		Height		=	(rect.bottom - rect.top) - (m_BarSpr.Header.Ysize * 2);

		//���κ�.
		clGrp.PutSpriteT(rect.left, rect.top,
					m_BarSpr.Header.Xsize, m_BarSpr.Header.Ysize,
					&m_BarSpr.Image[m_BarSpr.Header.Start[m_siScrollBarStartFont + 0]]);	
		

		for(i=0; i < (Height / m_BarSpr.Header.Ysize) ; i++)
		{
			clGrp.PutSpriteT(rect.left, (rect.top + m_BarSpr.Header.Ysize) + (i * m_BarSpr.Header.Ysize),
					m_BarSpr.Header.Xsize, m_BarSpr.Header.Ysize,
					&m_BarSpr.Image[m_BarSpr.Header.Start[m_siScrollBarStartFont + 1]]);	
		}

		if((Height % m_BarSpr.Header.Ysize) != 0)
		{			
			clGrp.PutSpriteT(rect.left, 
							(rect.top + m_BarSpr.Header.Ysize) + ((Height / m_BarSpr.Header.Ysize) * m_BarSpr.Header.Ysize), 
							m_BarSpr.Header.Xsize, Height % m_BarSpr.Header.Ysize, &m_BarSpr.Image[m_BarSpr.Header.Start[m_siScrollBarStartFont + 1]]);
		}

		//�Ʒ��κ�.
		clGrp.PutSpriteT(rect.left, rect.bottom - m_BarSpr.Header.Ysize,
					m_BarSpr.Header.Xsize, m_BarSpr.Header.Ysize,
					&m_BarSpr.Image[m_BarSpr.Header.Start[m_siScrollBarStartFont + 2]]);								
	
		//�Ʒ� ��ư�� �׸���.
		if(m_bDownPressed)
		{
			clGrp.PutSpriteT(m_rcDownButton.left, m_rcDownButton.top, 
					m_UpDownSpr.Header.Xsize,
					m_UpDownSpr.Header.Ysize,
					&m_UpDownSpr.Image[m_UpDownSpr.Header.Start[m_siDownPressedFont]]);

		}
		else
		{
			clGrp.PutSpriteT(m_rcDownButton.left, m_rcDownButton.top, 
				m_UpDownSpr.Header.Xsize,
				m_UpDownSpr.Header.Ysize,
				&m_UpDownSpr.Image[m_UpDownSpr.Header.Start[m_siDownDefaultFont]]);
		}
		
		clGrp.UnlockSurface(SURFACE_BACK);
	}	
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : BOOL	_YListBox::IsPressedPageUpButton()
/// ���� : ������ �� ��ư�� ������������ ����.
/////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	_YListBox::IsPressedPageUpButton()
{
	RECT		rcPageUpButton;
	static		DWORD	dwPrevPressedTime;

	rcPageUpButton	= GetRectPageUpButton();

	if(IpD.LeftPressSwitch == TRUE && (timeGetTime() - dwPrevPressedTime > LISTBOX_SERVICEPROVIDER_UPDOWN_PRESS_DELAY) )
	{
		if(	(rcPageUpButton.left < IpD.Mouse_X) && (rcPageUpButton.right  > IpD.Mouse_X) &&	
			(rcPageUpButton.top	 < IpD.Mouse_Y) && (rcPageUpButton.bottom > IpD.Mouse_Y))
		{
			PushEffect(EFFECT_TITLEBUTTON, 0, 0);
			dwPrevPressedTime = timeGetTime();
			return TRUE;
		}
	}
	

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : BOOL	_YListBox::IsPressedPageDownButton()
/// ���� : ������ �ٿ� ��ư�� ������������ ����.
/////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	_YListBox::IsPressedPageDownButton()
{
	RECT		rcPageDownButton;
	static		DWORD	dwPrevPressedTime;

	rcPageDownButton = GetRectPageDownButton();

	if(IpD.LeftPressSwitch == TRUE && (timeGetTime() - dwPrevPressedTime > LISTBOX_SERVICEPROVIDER_UPDOWN_PRESS_DELAY) )
	{
		if((rcPageDownButton.left < IpD.Mouse_X) && (rcPageDownButton.right > IpD.Mouse_X) &&	
			(rcPageDownButton.top < IpD.Mouse_Y) && (rcPageDownButton.bottom > IpD.Mouse_Y))
		{
			PushEffect(EFFECT_TITLEBUTTON, 0, 0);
			dwPrevPressedTime = timeGetTime();
			return TRUE;
		}
	}

	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : RECT	_YListBox::GetRectPageUpButton()
/// ���� : ������ ���� �簢���� ���Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
RECT	_YListBox::GetRectPageUpButton()
{
	RECT	rect;
	RECT	rectListBar;

	//����Ʈ���� ���븦 ���Ѵ�.
	rectListBar = GetBarInfo();

	rect.left		= m_rcBarInfo.left;
	rect.top		= m_rcBarInfo.top;
	rect.right		= m_rcBarInfo.right;
	rect.bottom		= rectListBar.top;	

	return rect;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : RECT	_YListBox::GetRectPageDownButton()
/// ���� : ������ ���� �簢���� ���Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
RECT	_YListBox::GetRectPageDownButton()
{
	RECT	rect;
	RECT	rectListBar;

	//����Ʈ���� ���븦 ���Ѵ�.
	rectListBar = GetBarInfo();

	rect.left		= m_rcBarInfo.left;
	rect.top		= rectListBar.bottom;
	rect.right		= m_rcBarInfo.right;
	rect.bottom		= m_rcBarInfo.bottom;	

	return rect;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : SHORT _YListBox::NextPage()
/// ���� : ������ ���� �簢���� ���Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT	_YListBox::NextPage()
{
	if((m_StartSellNumber + m_VisibleSellNum) < (m_SellNum -  m_VisibleSellNum))
		m_StartSellNumber += m_VisibleSellNum;
	else	
		m_StartSellNumber = m_SellNum -  m_VisibleSellNum;	

	return m_StartSellNumber;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : SHORT _YListBox::PrevPage()
/// ���� : ������ ���� �簢���� ���Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT	_YListBox::PrevPage()
{
	if(m_StartSellNumber - m_VisibleSellNum > 0)
		m_StartSellNumber -= m_VisibleSellNum;
	else
		m_StartSellNumber = 0;

	return m_StartSellNumber;
}
	
/////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID _YListBox::GetStartSellOnMovingListBar()
/// ���� : ������ ���� �簢���� ���Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YListBox::GetStartSellOnMovingListBar()
{
	if(IpD.Mouse_X > m_rcBarInfo.left && IpD.Mouse_X < m_rcBarInfo.right &&
	   IpD.Mouse_Y > m_rcBarInfo.top  && IpD.Mouse_Y < m_rcBarInfo.bottom)
		m_StartSellNumber = (IpD.Mouse_Y - m_rcBarInfo.top) / ((m_rcBarInfo.bottom - m_rcBarInfo.top) / m_SellNum);

	if(m_StartSellNumber < 0)
		m_StartSellNumber = 0;
	
	if(m_StartSellNumber > (m_SellNum -  m_VisibleSellNum))
		m_StartSellNumber = m_SellNum -  m_VisibleSellNum;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����Ʈ �ڽ����� ��� �������� ���ش�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	_YListBox::DelAllSell()
{
	m_SellNum					=	0;
	m_SelectedSell				=	-1;
	m_StartSellNumber			=	0;

}

////////////////////////���� ���ι��̴��� ����Ʈ �ڽ� Ŭ����//////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : _YSelectMapFileListBox::_YSelectMapFileListBox()
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
_YSelectMapFileListBox::_YSelectMapFileListBox() : _YListBox()
{
	m_lpBSelectMapFiles =	NULL;
	m_FileSearchHandle	=	NULL;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : _YSelectMapFileListBox::~_YSelectMapFileListBox()
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
_YSelectMapFileListBox::~_YSelectMapFileListBox()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : BOOL _YSelectMapFileListBox::Create()
/// ���� :  �㿡�� ����� �͵�...
////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL _YSelectMapFileListBox::Create()
{
	CHAR			szFileName[MAX_PATH];	
	CHAR			szFullFileName[MAX_PATH];
	CHAR			szCuttedMapFileName[MAX_LISTBOX_SELECTMAPFILE_SELL][256];
	SHORT			nMapPlayableUserNum[MAX_LISTBOX_SELECTMAPFILE_SELL];
	SHORT			nMapCount;
	SHORT			i;
	SHORT			nTempPlayerNum;
	BOOL			IsChange;	
	_MapFileInfo	MapFileInfo;
	_MapHeader		m_MapHeader;

	_YListBox::Create(440, 105, 316, 202,
					  446, 112, 280, 185,					  
					  280, 20, 4, 4, 5, 5, 6);

	nMapCount=0;
	//��� ���� �̸��� �� �������� �÷��� ������ ����� ���� ��´�.		
	StartSearchMapFile();	
	while(IsSearchMapFile(szFileName,szFullFileName) == TRUE)
	{
		// �����̸�, 
		if(Map.GetMapInfo(szFullFileName, &m_MapHeader))
		{			
	#ifdef	_DEMO
			if(IsMapForDemo(szFileName) == TRUE)
			{					
				nMapPlayableUserNum[nMapCount]	=	m_MapHeader.PlayerNumber;
				CutFileExpansion(szFileName);
				strcpy(szCuttedMapFileName[nMapCount], szFileName);	
				
				// ���������� ���� �ϳ� ���� ��Ų��.
				nMapCount++;
			}
	#else
			
			nMapPlayableUserNum[nMapCount]	=	SHORT(m_MapHeader.PlayerNumber);			
			CutFileExpansion(szFileName);
			strcpy(szCuttedMapFileName[nMapCount], szFileName);	

			// ���������� ���� �ϳ� ���� ��Ų��.
			nMapCount++;	
	#endif
		}
	}
	EndSearchMapFile();


	//������ �Ѵ�.
	IsChange	=	TRUE;

	while(IsChange == TRUE)
	{
		IsChange = FALSE;

		for(i=0; i < (nMapCount - 1); i++)
		{
			if(nMapPlayableUserNum[i] > nMapPlayableUserNum[i+1])
			{
				//����� ���� �ٲ۴�.
				nTempPlayerNum				=	nMapPlayableUserNum[i];
				nMapPlayableUserNum[i]		=	nMapPlayableUserNum[i+1];
				nMapPlayableUserNum[i+1]	=	nTempPlayerNum;

				//���� �̸��� ���� �ٲ۴�.			
				strcpy(szFileName, szCuttedMapFileName[i]);
				strcpy(szCuttedMapFileName[i], szCuttedMapFileName[i+1]);
				strcpy(szCuttedMapFileName[i+1], szFileName);

				IsChange	=	TRUE;
			}
		}
	}

	//���� ȭ�� �̸��� ���� �ִ´�.
	for(i=0; i < nMapCount; i++)
	{
		MapFileInfo.m_MaxPlayerNum = nMapPlayableUserNum[i];
		AddSell(szCuttedMapFileName[i], &MapFileInfo);
	}
		
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID	_YSelectMapFileListBox::Draw()
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	_YSelectMapFileListBox::Draw()
{
	SHORT		i;			
	SHORT		FileNum;
	SHORT		VisibleSellNum, StartSellNumber;
	SHORT		x, y;

	FileNum				= _YListBox::GetSellNum();		
	VisibleSellNum		= _YListBox::GetVisibleSellNum();
	StartSellNumber		= _YListBox::GetStartSellNumber();		
	
	
	for(i=0; ((i+StartSellNumber) < FileNum) && (i < VisibleSellNum); i++)					
	{				
		x = m_PutTextX ;
		y = m_PutTextY + i * GetSellHeight() ;
		
		m_lpBSelectMapFiles[i + StartSellNumber].Put(
				x, y, x, y, clGrp.lpDDSBack, 
				BUTTON_PUT_NOBORDER | BUTTON_PUT_FILLCOLOR, 
				BUTTON_PUT_LEFT);			
	}	

	_YListBox::DrawScrollBar();	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID	_YSelectMapFileListBox::SetPosition(SHORT NewPosition)
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	_YSelectMapFileListBox::SetPosition(SHORT NewPosition)
{
	if(IsValidIndex(_YListBox::GetCurrentPosition())==TRUE)
		m_lpBSelectMapFiles[_YListBox::GetCurrentPosition()].SetSelected(FALSE);

	_YListBox::SetPosition(NewPosition);

	if(IsValidIndex(_YListBox::GetCurrentPosition())==TRUE)
		m_lpBSelectMapFiles[_YListBox::GetCurrentPosition()].SetSelected(TRUE);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : SHORT	_YSelectMapFileListBox::NextSell()
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT	_YSelectMapFileListBox::NextSell()
{
	SHORT	SelectFileIndex;

	SelectFileIndex = _YListBox::GetCurrentPosition();

	if(IsValidIndex(SelectFileIndex) == TRUE)	
		m_lpBSelectMapFiles[SelectFileIndex].SetSelected(FALSE);
	
	SelectFileIndex = _YListBox::NextSell();

	if(IsValidIndex(SelectFileIndex) == TRUE)	
		m_lpBSelectMapFiles[SelectFileIndex].SetSelected(TRUE);

	return SelectFileIndex;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : SHORT	_YSelectMapFileListBox::PrevSell()
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT	_YSelectMapFileListBox::PrevSell()
{
	SHORT	SelectFileIndex;

	SelectFileIndex = _YListBox::GetCurrentPosition();

	if(IsValidIndex(SelectFileIndex) == TRUE)	
		m_lpBSelectMapFiles[SelectFileIndex].SetSelected(FALSE);
	
	SelectFileIndex = _YListBox::PrevSell();

	if(IsValidIndex(SelectFileIndex) == TRUE)	
		m_lpBSelectMapFiles[SelectFileIndex].SetSelected(TRUE);

	return SelectFileIndex;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID	_YSelectMapFileListBox::SetAction(BOOL bAction, SHORT PlayerNum)
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	_YSelectMapFileListBox::SetAction(BOOL bAction, SHORT PlayerNum)
{
	SHORT			FileNum;
	SHORT			i;
	_MapFileInfo*	lpMapFileInfo;
	
	FileNum = _YListBox::GetSellNum();

	
	for(i=0; i < FileNum; i++)
	{
		lpMapFileInfo	=	GetMapFileInfo(i);		

		// ���� ���̴� ���� �߿� �α��� ����� ������ 
		// �������� �÷����� �� �ִ� ����� ���� �� ������
		// �� ������ ���� Ȱ��ȭ ��Ű�� �ʴ´�.

		if((lpMapFileInfo->m_MaxPlayerNum < PlayerNum) && PlayerNum)
			m_lpBSelectMapFiles[i].SetAction(FALSE);
		else
			m_lpBSelectMapFiles[i].SetAction(bAction);
	}		
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// VOID	_YSelectMapFileListBox::SetProcess(BOOL bProcess)
/// ���� : 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	_YSelectMapFileListBox::SetProcess(BOOL bProcess)
{
	SHORT		FileNum;
	SHORT		i;
	
	FileNum = _YListBox::GetSellNum();

	for(i=0; i < FileNum; i++)
	{
		m_lpBSelectMapFiles[i].SetProcess(bProcess);
	}	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : BOOL	_YSelectMapFileListBox::AddSell(CHAR* MenuName, _MapFileInfo* lpMapFileInfo)
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	_YSelectMapFileListBox::AddSell(CHAR* MenuName, _MapFileInfo* lpMapFileInfo)
{
	SHORT						FileNum;	
	_SelectMapFileButton		*lpBTempSelectMapFileButton;
	BOOL						IsSuccess;		

	FileNum = _YListBox::GetSellNum();
	
	lpBTempSelectMapFileButton	=	new	_SelectMapFileButton[FileNum + 1];

	//���ο� �޸𸮸� �Ҵ� �ޱ� ���� ���� �ִ� ���� �ٸ� �迭�� ��� ������ �д�.	
	if(FileNum)		
		memcpy(lpBTempSelectMapFileButton, m_lpBSelectMapFiles, sizeof(_SelectMapFileButton) * FileNum);
	
	if(m_lpBSelectMapFiles)
	{
		delete [] m_lpBSelectMapFiles;
		m_lpBSelectMapFiles = NULL;
	}

	m_lpBSelectMapFiles = new _SelectMapFileButton[FileNum + 1];

	memcpy(m_lpBSelectMapFiles, lpBTempSelectMapFileButton, sizeof(_SelectMapFileButton) * FileNum);	
	
	m_lpBSelectMapFiles[FileNum].Create(0, 0, 
										GetSellWidth(), 
										GetSellHeight(), 
										0, 0,
										MenuName, BUTTON_PUT_LEFT);

	m_lpBSelectMapFiles[FileNum].m_MapFileInfo = *lpMapFileInfo;	

	IsSuccess = _YListBox::AddSell(&m_lpBSelectMapFiles[FileNum], MenuName);

	if(lpBTempSelectMapFileButton)
	{
		delete [] lpBTempSelectMapFileButton;
		lpBTempSelectMapFileButton = NULL;
	}

	if(IsSuccess)
	{
		//��ư�� �ϳ��� ��� Ȱ��ȭ ��Ų��.
		if(GetSellNum() == 1)					
			SetPosition(0);		
		
		return TRUE;
	}
	else
	{
		return FALSE;
	}	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : _MapFileInfo*	_YSelectMapFileListBox::GetMapFileInfo(SHORT nIndex)
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
_MapFileInfo*	_YSelectMapFileListBox::GetMapFileInfo(SHORT nIndex)
{	
	return &m_lpBSelectMapFiles[nIndex].m_MapFileInfo;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID	_YSelectMapFileListBox::Action(BOOL bPush)
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	_YSelectMapFileListBox::Action(BOOL bPush)
{
	SHORT	i;
	SHORT	FileNum;	
	SHORT	VisibleSellNum;
	SHORT	StartSellNumber;
	SHORT	x, y;
	BOOL	TempIsDoubleClicked;		

	FileNum				=	_YListBox::GetSellNum();
	VisibleSellNum		=	_YListBox::GetVisibleSellNum();
	StartSellNumber		=	_YListBox::GetStartSellNumber();	
		
	for(i = 0; i < FileNum; i++)
	{
		if((i >= StartSellNumber) && (i < (StartSellNumber + VisibleSellNum)))
		{
			x = m_PutTextX;
			y = m_PutTextY + (i - StartSellNumber) * GetSellHeight();				

			if(m_lpBSelectMapFiles[i].Process(x, y, bPush, &TempIsDoubleClicked)==TRUE)
				SetPosition(i);																
		}
	}	

	_YListBox::Action(bPush);			
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID	_YSelectMapFileListBox::AllDestroyFile()
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	_YSelectMapFileListBox::AllDestroyFile()
{
	if(m_lpBSelectMapFiles)
	{
		delete [] m_lpBSelectMapFiles;
		m_lpBSelectMapFiles  = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : CHAR*	_YSelectMapFileListBox::GetFileName()
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
CHAR*	_YSelectMapFileListBox::GetFileName()
{	
	return GetFileName(_YListBox::GetCurrentPosition());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : CHAR*	_YSelectMapFileListBox::GetFileName(SHORT Index)
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
CHAR*	_YSelectMapFileListBox::GetFileName(SHORT Index)
{	
	if(IsValidIndex(Index) == TRUE)	return m_lpBSelectMapFiles[Index].GetMenuName();
	else							return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID	_YSelectMapFileListBox::StartSerachMapFile()
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YSelectMapFileListBox::StartSearchMapFile()
{	
	m_FileSearchHandle = INVALID_HANDLE_VALUE;
	ZeroMemory(&m_ffd, sizeof(m_ffd));
}
 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : BOOL	_YSelectMapFileListBox::IsSearchMapFile(CHAR *GetFileName)
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL _YSelectMapFileListBox::IsSearchMapFile(CHAR *GetFileName, CHAR *GetFullFileName)
{	
	CHAR				szSearchFileWildCard[1024];		

	//ȭ���� ã�ƾ� �ϴ� �ڵ��� ���� �������� �ʾҴ�.
	if(m_FileSearchHandle == INVALID_HANDLE_VALUE)	
	{
		wsprintf(szSearchFileWildCard, "%scusmap\\%s", SavePath, "*.map");		
		m_FileSearchHandle = FindFirstFile(szSearchFileWildCard, &m_ffd);	
		if(m_FileSearchHandle == INVALID_HANDLE_VALUE)
			return FALSE;

		strcpy(GetFileName, m_ffd.cFileName);
		wsprintf(GetFullFileName, "%scusmap\\%s", SavePath, m_ffd.cFileName);
		return TRUE;
	}
	else
	{
		if(FindNextFile(m_FileSearchHandle, &m_ffd) == TRUE)
		{
			strcpy(GetFileName, m_ffd.cFileName);
			wsprintf(GetFullFileName, "%scusmap\\%s", SavePath, m_ffd.cFileName);
			return TRUE;
		}
		else
		{
			return FALSE;
		}	
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID	_YSelectMapFileListBox::EndSerachMapFile()
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	_YSelectMapFileListBox::EndSearchMapFile()
{
	if(	m_FileSearchHandle != INVALID_HANDLE_VALUE)
	{
		FindClose(m_FileSearchHandle);
		m_FileSearchHandle = NULL;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID	_YSelectMapFileListBox::CutFileExpansion(CHAR* szFileName)
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	_YSelectMapFileListBox::CutFileExpansion(CHAR* szFileName)
{
	SHORT	nstrlen;

	nstrlen = strlen(szFileName);

	szFileName[nstrlen - 4] = NULL;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : BOOL _YSelectMapFileListBox::IsUseCurrentSelectedMap()
/// ���� : ���� ���õ� ������ Ȱ��ȭ �Ǿ� �ִ��� �����Ѵ�.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL _YSelectMapFileListBox::IsUseCurrentSelectedMap()
{	
	if(IsValidIndex(_YListBox::GetCurrentPosition()) == TRUE)
		return m_lpBSelectMapFiles[_YListBox::GetCurrentPosition()].IsAction();
	else
		return FALSE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : BOOL _YSelectMapFileListBox::IsHaveMap(CHAR* szFileName)
/// ���� : ���� ���õ� ������ Ȱ��ȭ �Ǿ� �ִ��� �����Ѵ�.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL _YSelectMapFileListBox::IsHaveMap(CHAR* szFileName)
{
	SHORT	i;
	SHORT	nStr;
	SHORT	FileNum;
	CHAR	szCutExtFileName[256];

	if(szFileName)
	{
		nStr	=	strlen(szFileName);

		strcpy(szCutExtFileName, szFileName);
		szCutExtFileName[nStr - 4] = NULL;

		FileNum	=	_YListBox::GetSellNum();
		for(i=0; i < FileNum; i++)					
			if(stricmp(szCutExtFileName, GetFileName(i)) == 0)	return TRUE;						
	}
	
	return FALSE;
}


