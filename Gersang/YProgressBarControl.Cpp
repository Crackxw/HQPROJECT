#include <GSL.h>
#include <main.h>
#include <Mouse.h>
#include <clGame.h>

#include <colortable.h>
#include <etc.h>

#include "YProgressBarControl.h"

extern	char 				SavePath[];
extern	unsigned char		DarkTable[][256];					// 어두운 단계 
extern _InputDevice			IpD;
extern _clGame *pGame;


////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : _YProgressBar::_YProgressBar()
/// 설명 : 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
_YProgressBar::_YProgressBar()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : _YProgressBar::~_YProgressBar()
/// 설명 : 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
_YProgressBar::~_YProgressBar()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YProgressBar::Init(SHORT x, SHORT y, SHORT Width, SHORT Height, BOOL bAction)
/// 설명 : 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YProgressBar::Init(SHORT x, SHORT y, SHORT Width, SHORT Height, BOOL bAction)
{	
	clGrp.LoadXspr("Online\\gamespr\\SCR_HANDLE_1.Spr", m_XSpr);

	m_x		   = x;
	m_y		   = y;
	m_Width	   = Width;
	m_Height   = Height;
	m_Controlx = Width;
	m_Controly = Height;

	m_ScrollSX = m_x + (m_XSpr.Header.Xsize / 2);
	m_ScrollEX = m_x + Width - (m_XSpr.Header.Xsize / 2);

	m_CurrentProgress = 0;

	MbAction	=	bAction;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YProgressBar::Free()
/// 설명 : 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YProgressBar::Free()
{
	if(m_XSpr.Image)
	{
		clGrp.FreeXspr(m_XSpr);
		ZeroMemory(&m_XSpr, sizeof(m_XSpr));
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YProgressBar::Draw()
/// 설명 : 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YProgressBar::Draw()
{
	SHORT	x;	

	x = m_ScrollSX + ((m_ScrollEX - m_ScrollSX) * m_CurrentProgress) / 100;
	if(MbAction == TRUE)
	{
		clGrp.PutSpriteT(x - (m_XSpr.Header.Xsize / 2), m_y + (m_Height / 2) - (m_XSpr.Header.Ysize / 2)
			, m_XSpr.Header.Xsize
			, m_XSpr.Header.Ysize
			, m_XSpr.Image);
	}
	else
	{
		clGrp.PutSpriteT(x - (m_XSpr.Header.Xsize / 2), m_y + (m_Height / 2) - (m_XSpr.Header.Ysize / 2)
			, m_XSpr.Header.Xsize
			, m_XSpr.Header.Ysize
			, m_XSpr.Image
			, DarkTable[min(MAX_DARK_STEP, 6)]);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YProgressBar::RefreshData()
/// 설명 : 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YProgressBar::RefreshData()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YProgressBar::Action(BOOL bPush)
/// 설명 : 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YProgressBar::Action(BOOL bPush)
{
	SHORT	NewPos;

	if(MbAction)
	{
		if(bPush == TRUE && IsInMouse() == TRUE)			
		{
			SI16	x = IpD.Mouse_X;
			SI16	y = IpD.Mouse_Y;

			if(x <  m_ScrollSX) x = m_ScrollSX;
			if(x >= m_ScrollEX) x = m_ScrollEX - 1;

			NewPos = ((x - m_ScrollSX) * 100) / (m_ScrollEX - m_ScrollSX);
			SetCurrentPos(NewPos);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YProgressBar::SetCurrentPos(SHORT Index)
/// 설명 : 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YProgressBar::SetCurrentPos(SHORT Index)
{
	m_CurrentProgress = Index;

	if(m_CurrentProgress < 0)   m_CurrentProgress = 0;
	if(m_CurrentProgress > 100) m_CurrentProgress = 100;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : SHORT _YProgressBar::GetCurrentPos()
/// 설명 : 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT _YProgressBar::GetCurrentPos()
{
	return m_CurrentProgress;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : _YSoundVolumeProgressBar::_YSoundVolumeProgressBar()
/// 목적 : 
////////////////////////////////////////////////////////////////////////////////////////////////////////
_YSoundVolumeProgressBar::_YSoundVolumeProgressBar() : _YProgressBar()
{	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : _YSoundVolumeProgressBar::~_YSoundVolumeProgressBar()
/// 목적 : 
////////////////////////////////////////////////////////////////////////////////////////////////////////
_YSoundVolumeProgressBar::~_YSoundVolumeProgressBar()
{		
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YSoundVolumeProgressBar::Init(BOOL bAction)
/// 목적 : 
////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YSoundVolumeProgressBar::Init(BOOL bAction, SI16 x, SI16 y, SI16 Width, SI16 Height)
{	
	_YProgressBar::Init(x, y, Width, Height, bAction);

	// dwVolume에는 사운드 볼륨이 들어간다. (0 ~ 100)
//	SetCurrentPos(pGame->Config.m_siEffectSoundVolume);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YSoundVolumeProgressBar::RefreshData()
/// 목적 : 
////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YSoundVolumeProgressBar::RefreshData()
{
	pGame->Config.SetEffectSoundVolume(_YProgressBar::GetCurrentPos());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : _YGammaProgressBar::_YGammaProgressBar()
/// 목적 : 
////////////////////////////////////////////////////////////////////////////////////////////////////////
_YGammaProgressBar::_YGammaProgressBar() : _YProgressBar()
{	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : _YGammaProgressBar::~_YGammaProgressBar()
/// 목적 : 
////////////////////////////////////////////////////////////////////////////////////////////////////////
_YGammaProgressBar::~_YGammaProgressBar()
{		
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YGammaProgressBar::Init()
/// 목적 : 
////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YGammaProgressBar::Init(SI16 x, SI16 y, SI16 Width, SI16 Height)
{	
	_YProgressBar::Init(x, y, Width, Height);
	
	m_OldGamma = pGame->Config.m_siGamma;

	SetCurrentPos(m_OldGamma * 5);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YGammaProgressBar::RefreshData()
/// 목적 : 
////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YGammaProgressBar::RefreshData()
{		
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YGammaProgressBar::RestoreData()
/// 목적 : 
////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YGammaProgressBar::RestoreData()
{		
	pGame->Config.GammaFunction(m_OldGamma); 		
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID _YGammaProgressBar::Action(BOOL bPush)
/// 목적 : 
////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID _YGammaProgressBar::Action(BOOL bPush)
{
	SHORT	nSaveProgress;

	nSaveProgress	=	(_YProgressBar::GetCurrentPos() / 5);
	_YProgressBar::Action(bPush);

	if(nSaveProgress != m_OldGamma)
	{
		pGame->Config.GammaFunction((_YProgressBar::GetCurrentPos() / 5)); 		

		m_OldGamma = nSaveProgress;
	}
}
