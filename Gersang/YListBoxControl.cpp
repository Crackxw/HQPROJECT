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
extern	cltMapInterface		clifMap;							// 맵관련 인터페이스 
extern _InputDevice				IpD;

//리스트 박스의 정적 변수들을 초기화 한다..

// actdoll (2004/06/29 16:30) : 누수발견! 쓰면 안되는 방법이지만 어쩔 수 없다.
XSPR	_YListBox::m_UpDownSpr		= {0,};
XSPR	_YListBox::m_BarSpr			= {0,};
SHORT	_YListBox::m_nReferenceCount = 0;


///////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : _YListBox::_YListBox() 
/// 목적 : 리스트 박스의 생성자.
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
/// 함수 : _YListBox::~_YListBox()
/// 목적 : 리스트 박스의 소멸자.
///////////////////////////////////////////////////////////////////////////////////////////////////
_YListBox::~_YListBox()
{
	if(m_UpDownSpr.Image)	clGrp.FreeXspr(m_UpDownSpr);			
	if(m_BarSpr.Image)		clGrp.FreeXspr(m_BarSpr);						
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	리스트 박스를 생성한다.
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

	//리스트 박스에서 한번에 보여지는 셀의 개수.
	m_VisibleSellNum			=	m_PutTextHeight / m_SellHeight;	

	for(i=0; i < MAX_LIST_BOX_SELL; i++)	
	{		
		m_lplpData[i]		= NULL;
		ZeroMemory(m_lplpSellName[i], sizeof(m_lplpSellName[i]));
	}

	//위로 향하는 버튼의 위치를 구한다.
	m_rcUpButton.left			= (x + width) - m_UpDownSpr.Header.Xsize;
	m_rcUpButton.top			= y;
	m_rcUpButton.right			= (x + width);
	m_rcUpButton.bottom			= y + m_UpDownSpr.Header.Ysize;

	//아래로 향하는 버튼의 위치를 구한다.
	m_rcDownButton.left			= (x + width) - m_UpDownSpr.Header.Xsize;
	m_rcDownButton.top			= (y + height) - m_UpDownSpr.Header.Ysize; 
	m_rcDownButton.right		= (x + width);
	m_rcDownButton.bottom		= (y + height);

	//리스트바의 위치를 구한다.
	m_rcBarInfo.left			= m_rcUpButton.left;
	m_rcBarInfo.top				= m_rcUpButton.bottom;
	m_rcBarInfo.right			= m_rcUpButton.right;
	m_rcBarInfo.bottom			= m_rcDownButton.top;				
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	리스트 박스를 처리하지 않는다.(시각적으로도 나타난다)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	_YListBox::SetAction(BOOL bAction)
{
	m_bAction	=	bAction;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	리스트 박스를 처리하지 않는다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	_YListBox::SetProcess(BOOL bProcess)
{
	m_bProcess	=	bProcess;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : void _YListBox::RefreshData()
/// 목적 : 데이타를 갱신한다.
///////////////////////////////////////////////////////////////////////////////////////////////////
void    _YListBox::RefreshData()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : void _YListBox::Draw()
/// 목적 : 화면에 그린다.
///////////////////////////////////////////////////////////////////////////////////////////////////
void _YListBox::Draw()
{	
	SHORT		i;		
	HDC			hdc;		

	//리스트 박스 안에 있는 텍스트를 출력한다.
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
/// 함수 : void _YListBox::Action(BOOL bPush)
/// 목적 : 항상 해야 할일들을 처리해 준다.
///////////////////////////////////////////////////////////////////////////////////////////////////
void _YListBox::Action(BOOL bPush)
{	
	// m_bAction이나 m_bProcess 둘중의 하나라도 FALSE이라면 작동 불능 상태이다,
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
/// 함수 : SRHOT _YListBox::AddSell(LPVOID lpData, char* SellName)
/// 목적 : 항상 해야 할일들을 처리해 준다.
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
/// 함수 : void _YListBox::DelSell(SI16 siIndex)
/// 목적 : 셀을 리스트에서 삭제한다.
///////////////////////////////////////////////////////////////////////////////////////////////////
void _YListBox::DelSell(SI16 siIndex)
{	
	UI32		i;

	if(IsValidIndex(siIndex)==TRUE)
	{	
		// siIndex 부터 한 칸씩 앞으로 당긴다.		
		for(i = siIndex; i < (UI32)(m_SellNum - 1); i++)
		{
			m_lplpData[i] = m_lplpData[i + 1];
			strcpy(m_lplpSellName[i], m_lplpSellName[i + 1]);				
		}	
	
		// 마지막 셀을 NULL로 채운다.
		m_lplpData[m_SellNum - 1] = NULL;
		ZeroMemory(m_lplpSellName[m_SellNum - 1], sizeof(m_lplpSellName[m_SellNum]));	

		// 셀을 하나 삭제한다.
		m_SellNum--;	
	}	
}	

///////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : LPVOID _YListBox::GetData(SHORT SellIndex)
/// 목적 : 셀의 번호로 데이타를 얻어 온다.
///////////////////////////////////////////////////////////////////////////////////////////////////
LPVOID _YListBox::GetData(SHORT SellIndex)
{
	if(IsValidIndex(SellIndex)==TRUE)	
		return m_lplpData[SellIndex];	
	else
		return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : BOOL _YListBox::IsValidIndex(SHORT Index)
/// 목적 : 유효한 인덱스인지를 본다.
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL _YListBox::IsValidIndex(SHORT Index)
{
	//먼저 유효한 범위인지를 체크한다.
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
/// 함수 : SHORT	_YListBox::GetWidth()
/// 목적 : 컨트롤의 넓이를 구한다.
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT	_YListBox::GetWidth()
{
	return m_Controlx;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : SHORT	_YListBox::GetHeight()
/// 목적 : 컨트롤의 넓이를 구한다.
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT	_YListBox::GetHeight()
{
	return m_Controly;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : SHORT _YListBox::GetSellWidth()
/// 목적 : 셀 하나의 폭을 구한다.
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT _YListBox::GetSellWidth()
{
	return m_SellWidth;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : SHORT _YListBox::GetSellHeight()
/// 목적 : 셀 하나의 폭을 구한다.
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT _YListBox::GetSellHeight()
{
	return m_SellHeight;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : SHORT _YListBox::GetSellNum()
/// 목적 : 총 셀의 개수를 구한다.
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT _YListBox::GetSellNum()
{
	return m_SellNum;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : void _YListBox::SetPosition(SHORT NewPosition)
/// 목적 : 새로운 위치를 지정한다.
////////////////////////////////////////////////////////////////////////////////////////////////////
void _YListBox::SetPosition(SHORT NewPosition)
{
	if(IsValidIndex(NewPosition) == TRUE)
		m_SelectedSell = NewPosition;		
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : SHORT  _YListBox::GetCurrentPosition()
/// 목적 : 현재의 위치를 얻어온다.
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT  _YListBox::GetCurrentPosition()
{
	if(IsValidIndex(m_SelectedSell))
		return m_SelectedSell;
	else
		return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : SHORT  _YListBox::NextSell()
/// 목적 : 다음 셀로 이동한다.
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT  _YListBox::NextSell()
{
	if((m_SellNum - m_VisibleSellNum) > m_StartSellNumber)
				m_StartSellNumber++;					
	
	return m_SelectedSell;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : SHORT  _YListBox::PrevSell()
/// 목적 : 이전 셀로 이동한다.
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT  _YListBox::PrevSell()
{
	if(m_StartSellNumber > 0)
		m_StartSellNumber--;

	return m_SelectedSell;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : SHORT  _YListBox::GetVisibleSellNum()
/// 목적 : 한 번에 리스트 목록에 보여지는 셀의 개수를 구한다.
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT  _YListBox::GetVisibleSellNum()
{
	return m_VisibleSellNum;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : SHORT  _YListBox::GetStartSellNumber()
/// 목적 : 리스트 박스 처음에서 보여주어야 하는 셀의 번호.
////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT   _YListBox::GetStartSellNumber()
{
	return m_StartSellNumber;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : BOOL	_YListBox::IsTopSell()
/// 목적 : 리스트 박스에 맨위에(0번 인덱스가 아니라) 있는지..
/////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	_YListBox::IsTopSell()
{
	if(m_StartSellNumber == m_SelectedSell)	
		return TRUE;	
	else
		return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : BOOL	_YListBox::IsBottomSell()
/// 목적 : 리스트 박스에 맨아래(마지막번 인덱스가 아니라) 있는지..
/////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	_YListBox::IsBottomSell()
{
	if((m_StartSellNumber + m_VisibleSellNum - 1) == m_SelectedSell)
		return TRUE;
	else
		return FALSE;	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : BOOL	_YListBox::IsPressedUpButton()
/// 목적 : 위로 향하는 버튼이 눌리어졌는지.. 체크한다.
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
/// 함수 : BOOL	_YListBox::IsPressedDownButton()
/// 목적 : 위로 향하는 버튼이 눌리어졌는지.. 체크한다.
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
/// 함수 : RECT	_YListBox::GetBarInfo()
/// 목적 : 바의 크기 및 위치를 구한다.
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

		//우선 높이를 저장한다.
		Rect.bottom				=	ViewHeight * BarHeight / TotalHeight;		

		if(Rect.bottom < CONTROL_SCROLL_MIN_HEIGHT)
			Rect.bottom = CONTROL_SCROLL_MIN_HEIGHT;
		
		//바의 위치를 결정한다.
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
/// 함수 : VOID	_YListBox::DrawScrollBar()
/// 목적 : 스크롤 바를 그린다.
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
			//위 버튼을 그린다.
			clGrp.PutSpriteT(m_rcUpButton.left, m_rcUpButton.top, 
						m_UpDownSpr.Header.Xsize,
						m_UpDownSpr.Header.Ysize,
						&m_UpDownSpr.Image[m_UpDownSpr.Header.Start[m_siUpPressedFont]]);
		}
		else
		{
			//위 버튼을 그린다.
			clGrp.PutSpriteT(m_rcUpButton.left, m_rcUpButton.top, 
						m_UpDownSpr.Header.Xsize,
						m_UpDownSpr.Header.Ysize,
						&m_UpDownSpr.Image[m_UpDownSpr.Header.Start[m_siUpDefaultFont]]);
		}		

		
		//바를 그린다.			
		rect = GetBarInfo();	
		
		//위 부분 및 아래 부분을 제거한 크기를 구한다.
		Height		=	(rect.bottom - rect.top) - (m_BarSpr.Header.Ysize * 2);

		//윗부분.
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

		//아래부분.
		clGrp.PutSpriteT(rect.left, rect.bottom - m_BarSpr.Header.Ysize,
					m_BarSpr.Header.Xsize, m_BarSpr.Header.Ysize,
					&m_BarSpr.Image[m_BarSpr.Header.Start[m_siScrollBarStartFont + 2]]);								
	
		//아래 버튼을 그린다.
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
/// 함수 : BOOL	_YListBox::IsPressedPageUpButton()
/// 목적 : 페이지 업 버튼이 눌리어졌는지 본다.
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
/// 함수 : BOOL	_YListBox::IsPressedPageDownButton()
/// 목적 : 페이지 다운 버튼이 눌리어졌는지 본다.
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
/// 함수 : RECT	_YListBox::GetRectPageUpButton()
/// 목적 : 페이지 업의 사각형을 구한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
RECT	_YListBox::GetRectPageUpButton()
{
	RECT	rect;
	RECT	rectListBar;

	//리스트바의 막대를 구한다.
	rectListBar = GetBarInfo();

	rect.left		= m_rcBarInfo.left;
	rect.top		= m_rcBarInfo.top;
	rect.right		= m_rcBarInfo.right;
	rect.bottom		= rectListBar.top;	

	return rect;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : RECT	_YListBox::GetRectPageDownButton()
/// 목적 : 페이지 업의 사각형을 구한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////
RECT	_YListBox::GetRectPageDownButton()
{
	RECT	rect;
	RECT	rectListBar;

	//리스트바의 막대를 구한다.
	rectListBar = GetBarInfo();

	rect.left		= m_rcBarInfo.left;
	rect.top		= rectListBar.bottom;
	rect.right		= m_rcBarInfo.right;
	rect.bottom		= m_rcBarInfo.bottom;	

	return rect;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : SHORT _YListBox::NextPage()
/// 목적 : 페이지 업의 사각형을 구한다.
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
/// 함수 : SHORT _YListBox::PrevPage()
/// 목적 : 페이지 업의 사각형을 구한다.
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
/// 함수 : VOID _YListBox::GetStartSellOnMovingListBar()
/// 목적 : 페이지 업의 사각형을 구한다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	리스트 박스에서 모든 아이템을 없앤다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	_YListBox::DelAllSell()
{
	m_SellNum					=	0;
	m_SelectedSell				=	-1;
	m_StartSellNumber			=	0;

}

////////////////////////서비스 프로바이더의 리스트 박스 클래스//////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : _YSelectMapFileListBox::_YSelectMapFileListBox()
/// 설명 : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
_YSelectMapFileListBox::_YSelectMapFileListBox() : _YListBox()
{
	m_lpBSelectMapFiles =	NULL;
	m_FileSearchHandle	=	NULL;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : _YSelectMapFileListBox::~_YSelectMapFileListBox()
/// 설명 : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
_YSelectMapFileListBox::~_YSelectMapFileListBox()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : BOOL _YSelectMapFileListBox::Create()
/// 설명 :  멥에서 사용할 것들...
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
	//모든 지도 이름과 그 지도에서 플레이 가능한 사용자 수를 얻는다.		
	StartSearchMapFile();	
	while(IsSearchMapFile(szFileName,szFullFileName) == TRUE)
	{
		// 조반이면, 
		if(Map.GetMapInfo(szFullFileName, &m_MapHeader))
		{			
	#ifdef	_DEMO
			if(IsMapForDemo(szFileName) == TRUE)
			{					
				nMapPlayableUserNum[nMapCount]	=	m_MapHeader.PlayerNumber;
				CutFileExpansion(szFileName);
				strcpy(szCuttedMapFileName[nMapCount], szFileName);	
				
				// 마지막으로 맵을 하나 증가 시킨다.
				nMapCount++;
			}
	#else
			
			nMapPlayableUserNum[nMapCount]	=	SHORT(m_MapHeader.PlayerNumber);			
			CutFileExpansion(szFileName);
			strcpy(szCuttedMapFileName[nMapCount], szFileName);	

			// 마지막으로 맵을 하나 증가 시킨다.
			nMapCount++;	
	#endif
		}
	}
	EndSearchMapFile();


	//정렬을 한다.
	IsChange	=	TRUE;

	while(IsChange == TRUE)
	{
		IsChange = FALSE;

		for(i=0; i < (nMapCount - 1); i++)
		{
			if(nMapPlayableUserNum[i] > nMapPlayableUserNum[i+1])
			{
				//사용자 수를 바꾼다.
				nTempPlayerNum				=	nMapPlayableUserNum[i];
				nMapPlayableUserNum[i]		=	nMapPlayableUserNum[i+1];
				nMapPlayableUserNum[i+1]	=	nTempPlayerNum;

				//파일 이름도 역시 바꾼다.			
				strcpy(szFileName, szCuttedMapFileName[i]);
				strcpy(szCuttedMapFileName[i], szCuttedMapFileName[i+1]);
				strcpy(szCuttedMapFileName[i+1], szFileName);

				IsChange	=	TRUE;
			}
		}
	}

	//지도 화일 이름을 집어 넣는다.
	for(i=0; i < nMapCount; i++)
	{
		MapFileInfo.m_MaxPlayerNum = nMapPlayableUserNum[i];
		AddSell(szCuttedMapFileName[i], &MapFileInfo);
	}
		
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID	_YSelectMapFileListBox::Draw()
/// 설명 : 
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
/// 함수 : VOID	_YSelectMapFileListBox::SetPosition(SHORT NewPosition)
/// 설명 : 
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
/// 함수 : SHORT	_YSelectMapFileListBox::NextSell()
/// 설명 : 
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
/// 함수 : SHORT	_YSelectMapFileListBox::PrevSell()
/// 설명 : 
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
/// 함수 : VOID	_YSelectMapFileListBox::SetAction(BOOL bAction, SHORT PlayerNum)
/// 설명 : 
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

		// 현재 보이는 지도 중에 로긴한 사용자 수보다 
		// 지도에서 플레이할 수 있는 사용자 수가 더 적으면
		// 그 지도는 절대 활성화 시키지 않는다.

		if((lpMapFileInfo->m_MaxPlayerNum < PlayerNum) && PlayerNum)
			m_lpBSelectMapFiles[i].SetAction(FALSE);
		else
			m_lpBSelectMapFiles[i].SetAction(bAction);
	}		
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// VOID	_YSelectMapFileListBox::SetProcess(BOOL bProcess)
/// 설명 : 
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
/// 함수 : BOOL	_YSelectMapFileListBox::AddSell(CHAR* MenuName, _MapFileInfo* lpMapFileInfo)
/// 설명 : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	_YSelectMapFileListBox::AddSell(CHAR* MenuName, _MapFileInfo* lpMapFileInfo)
{
	SHORT						FileNum;	
	_SelectMapFileButton		*lpBTempSelectMapFileButton;
	BOOL						IsSuccess;		

	FileNum = _YListBox::GetSellNum();
	
	lpBTempSelectMapFileButton	=	new	_SelectMapFileButton[FileNum + 1];

	//새로운 메모리를 할당 받기 위해 지금 있는 값을 다른 배열에 잠시 저장해 둔다.	
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
		//버튼이 하나의 경우 활성화 시킨다.
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
/// 함수 : _MapFileInfo*	_YSelectMapFileListBox::GetMapFileInfo(SHORT nIndex)
/// 설명 : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
_MapFileInfo*	_YSelectMapFileListBox::GetMapFileInfo(SHORT nIndex)
{	
	return &m_lpBSelectMapFiles[nIndex].m_MapFileInfo;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID	_YSelectMapFileListBox::Action(BOOL bPush)
/// 설명 : 
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
/// 함수 : VOID	_YSelectMapFileListBox::AllDestroyFile()
/// 설명 : 
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
/// 함수 : CHAR*	_YSelectMapFileListBox::GetFileName()
/// 설명 : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
CHAR*	_YSelectMapFileListBox::GetFileName()
{	
	return GetFileName(_YListBox::GetCurrentPosition());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : CHAR*	_YSelectMapFileListBox::GetFileName(SHORT Index)
/// 설명 : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
CHAR*	_YSelectMapFileListBox::GetFileName(SHORT Index)
{	
	if(IsValidIndex(Index) == TRUE)	return m_lpBSelectMapFiles[Index].GetMenuName();
	else							return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID	_YSelectMapFileListBox::StartSerachMapFile()
/// 설명 : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YSelectMapFileListBox::StartSearchMapFile()
{	
	m_FileSearchHandle = INVALID_HANDLE_VALUE;
	ZeroMemory(&m_ffd, sizeof(m_ffd));
}
 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : BOOL	_YSelectMapFileListBox::IsSearchMapFile(CHAR *GetFileName)
/// 설명 : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL _YSelectMapFileListBox::IsSearchMapFile(CHAR *GetFileName, CHAR *GetFullFileName)
{	
	CHAR				szSearchFileWildCard[1024];		

	//화일을 찾아야 하는 핸들이 아직 정해지지 않았다.
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
/// 함수 : VOID	_YSelectMapFileListBox::EndSerachMapFile()
/// 설명 : 
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
/// 함수 : VOID	_YSelectMapFileListBox::CutFileExpansion(CHAR* szFileName)
/// 설명 : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	_YSelectMapFileListBox::CutFileExpansion(CHAR* szFileName)
{
	SHORT	nstrlen;

	nstrlen = strlen(szFileName);

	szFileName[nstrlen - 4] = NULL;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : BOOL _YSelectMapFileListBox::IsUseCurrentSelectedMap()
/// 설명 : 현재 선택된 지도가 활성화 되어 있는지 리턴한다.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL _YSelectMapFileListBox::IsUseCurrentSelectedMap()
{	
	if(IsValidIndex(_YListBox::GetCurrentPosition()) == TRUE)
		return m_lpBSelectMapFiles[_YListBox::GetCurrentPosition()].IsAction();
	else
		return FALSE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : BOOL _YSelectMapFileListBox::IsHaveMap(CHAR* szFileName)
/// 설명 : 현재 선택된 지도가 활성화 되어 있는지 리턴한다.
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


