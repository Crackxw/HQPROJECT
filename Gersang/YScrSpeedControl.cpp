#include <GSL.h>
#include <graphics16.h>
#include <main.h>

#include <etc.h>

#include "YSelectControl.h"
#include "YScrSpeedControl.h"
#include "text.h"

extern	_Text 				Text;								// 텍스트 
extern	char 				SavePath[];

XSPR	_YScrollSpeed::m_Xspr;

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : _YScrollSpeed::_YScrollSpeed()
/// 설명 :
//////////////////////////////////////////////////////////////////////////////////////////////////////
_YScrollSpeed::_YScrollSpeed()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : _YScrollSpeed::~_YScrollSpeed()
/// 설명 :
//////////////////////////////////////////////////////////////////////////////////////////////////////
_YScrollSpeed::~_YScrollSpeed()
{
	Free();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YScrollSpeed::Init()
/// 설명 :
//////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YScrollSpeed::Init()
{
	CHAR						szFileNameBuffer[1024];	
	strBaseSelectButtonInfo		stBaseSelectButtonInfo[3];

	stBaseSelectButtonInfo[0].rcButton.left		=	Y_SCROLLSPEED_SLOW_X;
	stBaseSelectButtonInfo[0].rcButton.top		=	Y_SCROLLSPEED_SLOW_Y;
	
	stBaseSelectButtonInfo[1].rcButton.left		=	Y_SCROLLSPEED_MEDIUM_X;
	stBaseSelectButtonInfo[1].rcButton.top		=	Y_SCROLLSPEED_MEDIUM_Y;

	stBaseSelectButtonInfo[2].rcButton.left		=	Y_SCROLLSPEED_FAST_X;
	stBaseSelectButtonInfo[2].rcButton.top		=	Y_SCROLLSPEED_FAST_Y;

	stBaseSelectButtonInfo[0].rcButton.right	=	stBaseSelectButtonInfo[0].rcButton.left + Y_SCROLLSPEED_WIDTH;
	stBaseSelectButtonInfo[0].rcButton.bottom	=	stBaseSelectButtonInfo[0].rcButton.top +  Y_SCROLLSPEED_HEIGHT;
	strcpy(stBaseSelectButtonInfo[0].szName, Text.ScrollSlow.Get());		

	stBaseSelectButtonInfo[1].rcButton.right	=	stBaseSelectButtonInfo[1].rcButton.left + Y_SCROLLSPEED_WIDTH;
	stBaseSelectButtonInfo[1].rcButton.bottom	=	stBaseSelectButtonInfo[1].rcButton.top +  Y_SCROLLSPEED_HEIGHT;
	strcpy(stBaseSelectButtonInfo[1].szName, Text.ScrollNormal.Get());		

	
	stBaseSelectButtonInfo[2].rcButton.right	=	stBaseSelectButtonInfo[2].rcButton.left + Y_SCROLLSPEED_WIDTH;
	stBaseSelectButtonInfo[2].rcButton.bottom	=	stBaseSelectButtonInfo[2].rcButton.top +  Y_SCROLLSPEED_HEIGHT;
	strcpy(stBaseSelectButtonInfo[2].szName, Text.ScrollFast.Get());		
	
	//버튼을 생성한다.
	m_BaseSelectButton.Create(3, stBaseSelectButtonInfo);

	GetFileNamePath(Y_OPTION_SCROLLSPEED_SPR_FILENAME, SavePath, szFileNameBuffer);
	if(m_Xspr.Image) clGrp.Error("FKJE8567", "YYSCRSPEEDCONTROL0000001");
	if(clGrp.LoadXspr(szFileNameBuffer, m_Xspr) == FALSE) clGrp.Error("FKJE8567", "YYSCRSPEEDCONTROL0000002 [%s]", szFileNameBuffer);

	SetSlow();	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YScrollSpeed::Init()
/// 설명 :
//////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YScrollSpeed::Free()
{
	if(m_Xspr.Image) clGrp.FreeXspr(m_Xspr);
	m_BaseSelectButton.Free();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YScrollSpeed::RefreshData()
/// 설명 :
//////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YScrollSpeed::RefreshData()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YScrollSpeed::Action(BOOL bPush)
/// 설명 :
//////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YScrollSpeed::Action(BOOL bPush)
{
	SHORT	nRes;

	nRes = m_BaseSelectButton.Process(bPush);

	if(nRes != -1)		
		m_nSpeed = nRes;	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID	_YScrollSpeed::Draw()
/// 설명 :
//////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	_YScrollSpeed::Draw()
{
	SHORT	FontArray[3][3];
	
	if(IsSlow() == TRUE)		
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

	if(IsMedium() == TRUE)
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

	if(IsFast() == TRUE)
	{
		FontArray[2][0]= 0;
		FontArray[2][1]= 0;
		FontArray[2][2]= 0;		
	}
	else	
	{
		FontArray[2][0]= 0;
		FontArray[2][1]= 0;
		FontArray[2][2]= 1;				
	}

	m_BaseSelectButton.Draw(&m_Xspr, FontArray);
}

//------------------------------------------------------------------------------------------------
//	설명	:
//------------------------------------------------------------------------------------------------
VOID	_YScrollSpeed::Draw(HDC hdc)
{
	m_BaseSelectButton.Draw(hdc, 0);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : BOOL	_YScrollSpeed::IsSlow()
/// 설명 :
//////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	_YScrollSpeed::IsSlow()
{
	if(m_nSpeed == SLOW)
		return TRUE;
	else
		return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : BOOL	_YScrollSpeed::IsMedium()
/// 설명 :
//////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	_YScrollSpeed::IsMedium()
{
	if(m_nSpeed == MEDIUM)
		return TRUE;
	else
		return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : BOOL	_YScrollSpeed::IsFast()
/// 설명 :
//////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	_YScrollSpeed::IsFast()
{
	if(m_nSpeed == FAST)
		return TRUE;
	else
		return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID	_YScrollSpeed::SetSlow()
/// 설명 :
//////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	_YScrollSpeed::SetSlow()
{
	m_nSpeed = SLOW;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID	_YScrollSpeed::SetMedium()
/// 설명 :
//////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	_YScrollSpeed::SetMedium()
{
	m_nSpeed = MEDIUM;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID	_YScrollSpeed::SetFast()
/// 설명 :
//////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	_YScrollSpeed::SetFast()
{
	m_nSpeed = FAST;
}

