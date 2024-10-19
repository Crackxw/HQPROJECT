#include <GSL.h>
#include <main.h>
#include <clGame.h>

#include <etc.h>

#include	"YMIControl.h"
#include "text.h"

extern	_Text 				Text;								// 텍스트 
extern	char 				SavePath[];
extern _clGame *pGame;


//////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : _YMouseInterface::_YMouseInterface()
/// 목적 : 
//////////////////////////////////////////////////////////////////////////////////////////////////////
_YMouseInterface::_YMouseInterface()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : _YMouseInterface::~_YMouseInterface()
/// 목적 : 
//////////////////////////////////////////////////////////////////////////////////////////////////////
_YMouseInterface::~_YMouseInterface()
{
	Free();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YMouseInterface::Init()
/// 목적 : 
//////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YMouseInterface::Init(SI16 x, SI16 y)
{
	strBaseSelectButtonInfo		stBaseSelectButtonInfo[2];
	
	clGrp.LoadXspr("Online\\GameSpr\\Option\\small_window_push_button.Spr", m_XSPR);
	
	stBaseSelectButtonInfo[0].rcButton.left		=	x;
	stBaseSelectButtonInfo[0].rcButton.top		=	y;
	
	stBaseSelectButtonInfo[1].rcButton.left		=	x + 61;
	stBaseSelectButtonInfo[1].rcButton.top		=	y;

	stBaseSelectButtonInfo[0].rcButton.right	=	stBaseSelectButtonInfo[0].rcButton.left + m_XSPR.Header.Xsize;
	stBaseSelectButtonInfo[0].rcButton.bottom	=	stBaseSelectButtonInfo[0].rcButton.top  + m_XSPR.Header.Ysize;
	strcpy(stBaseSelectButtonInfo[0].szName, pGame->pOnlineWorld->pOnlineText->Get(ON_TEXT_GAME_OPTION_MOUSE_1_BTN_METHOD));

	stBaseSelectButtonInfo[1].rcButton.right	=	stBaseSelectButtonInfo[1].rcButton.left + m_XSPR.Header.Xsize;
	stBaseSelectButtonInfo[1].rcButton.bottom	=	stBaseSelectButtonInfo[1].rcButton.top  + m_XSPR.Header.Ysize;
	strcpy(stBaseSelectButtonInfo[1].szName, pGame->pOnlineWorld->pOnlineText->Get(ON_TEXT_GAME_OPTION_MOUSE_2_BTN_METHOD));

	//버튼을 생성한다.
	m_BaseSelectButton.Create(2, stBaseSelectButtonInfo);	

	if(pGame->Config.m_uiInterfaceMode == INTERFACEMODE_1BUTTON)
	{
		SetOne();
	}
	else
	{
		if(pGame->Config.m_uiInterfaceMode == INTERFACEMODE_2BUTTON)
		{
			SetTwo();
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YMouseInterface::Free()
/// 목적 : 
//////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YMouseInterface::Free()
{
	m_BaseSelectButton.Free();

	if(m_XSPR.Image)
	{
		clGrp.FreeXspr(m_XSPR);
		ZeroMemory(&m_XSPR, sizeof(m_XSPR));
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : BOOL _YMouseInterface::IsOne()
/// 목적 : 
//////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL _YMouseInterface::IsOne()
{
	if(m_nOneTwo == ONE)
		return TRUE;
	else
		return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : BOOL _YMouseInterface::IsTwo()
/// 목적 : 
//////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL _YMouseInterface::IsTwo()
{
	if(m_nOneTwo == TWO)
		return TRUE;
	else
		return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YMouseInterface::SetOne()
/// 목적 : 
//////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YMouseInterface::SetOne()
{
	m_nOneTwo = ONE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YMouseInterface::SetTwo()
/// 목적 : 
//////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YMouseInterface::SetTwo()
{
	m_nOneTwo = TWO;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YMouseInterface::RefreshData()
/// 목적 : 
//////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YMouseInterface::RefreshData()
{
	if(IsOne() == TRUE)	
		pGame->Config.m_uiInterfaceMode = INTERFACEMODE_1BUTTON;	
	else if(IsTwo() == TRUE)	
		pGame->Config.m_uiInterfaceMode = INTERFACEMODE_2BUTTON;		
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YMouseInterface::Action(BOOL bPush)
/// 목적 : 
//////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YMouseInterface::Action(BOOL bPush)
{
	SHORT	nRes;

	nRes = m_BaseSelectButton.Process(bPush);

	if(nRes != -1)	
		m_nOneTwo = nRes;	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YMouseInterface::Draw()
/// 목적 : 
//////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YMouseInterface::Draw()
{
	SHORT	FontArray[2][3];
	
	if(IsOne() == TRUE)		
	{
		FontArray[0][0]= 0;
		FontArray[0][1]= 0;
		FontArray[0][2]= 0;		
	}
	else	
	{
		FontArray[0][0]= 0;
		FontArray[0][1]= 0;
		FontArray[0][2]= 1;				
	}

	if(IsTwo() == TRUE)
	{
		FontArray[1][0]= 0;
		FontArray[1][1]= 0;
		FontArray[1][2]= 0;
	}
	else	
	{
		FontArray[1][0]= 0;
		FontArray[1][1]= 0;
		FontArray[1][2]= 1;				
	}

	m_BaseSelectButton.Draw(&m_XSPR, FontArray, BUTTON_PUT_NOMOVE);
}

//------------------------------------------------------------------------------------------------
//	설명	:	
//------------------------------------------------------------------------------------------------
VOID	_YMouseInterface::Draw(HDC hdc)
{
	SI32	siSelectButton;
	
	if(IsOne())
		siSelectButton	=	0;
	else
		siSelectButton	=	1;

	m_BaseSelectButton.Draw(hdc, siSelectButton);
}
