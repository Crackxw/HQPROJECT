#include <GSL.h>
#include <main.h>
#include <Mouse.h>
#include <clGame.h>

#include <Game2.h>
#include <etc.h>
#include <SoundManager.h>

#include "ytitlefile.h"
#include "Yoption.h"
#include "gamemenu.h"
#include "text.h"

// 온라인 텍스트(OnlineText.dat)를 이용하기 위함. 더 이상 GameText.dat를 이용하지 않는다.
#include "OnlineWorld.h"
#include <OnlineText.h>
#include <OnlineMegaText.h>

static	BOOL				g_bPush;
static	SI16				siX_Pos, siY_Pos;

extern	char 				SavePath[];
extern	_Text 				Text;								// 텍스트 
extern	SoundManager       *g_pSoundManager;
extern _InputDevice			IpD;
extern _clGame *pGame;


_YOption::_YOption()
{	
}

_YOption::~_YOption()
{
	Free();
}

void _YOption::Init()
{	
//	clGrp.LoadXspr("Online\\GameSpr\\Option\\OPTION.Spr", m_Xspr);	
	clGrp.LoadXspr("Online\\GameSpr\\Village\\SMALL_WINDOW.Spr", m_Xspr);	
	clGrp.LoadXspr("Online\\GameSpr\\Village\\SMALL_WINDOW_LINE.Spr", m_Xspr_Bkgnd);
	clGrp.LoadXspr("Online\\GameSpr\\Option\\small_window_push_button.Spr", m_BSpr);
	
	siX_Pos = 262; 
	siY_Pos = 136;

	// 효과음 볼륨 조절 바 생성
	if (g_pSoundManager)
	{
		m_YEffectSoundVolume.Init(g_pSoundManager->bSoundDriver, siX_Pos + 95, siY_Pos + 27, 160, 12);
		m_YEffectSoundVolume.SetCurrentPos(pGame->Config.m_siEffectSoundVolume);
	}

	// 배경 음악 볼륨 조절 바 생성
	if (g_pSoundManager)
	{
		m_YBackgroundMusicVolume.Init(g_pSoundManager->bSoundDriver, siX_Pos + 95, siY_Pos + 52, 160, 12);
		m_YBackgroundMusicVolume.SetCurrentPos(pGame->Config.m_siBackgroundMusicVolume);
	}

	// 화면 밝기 조절 바
	m_YGamma.Init(siX_Pos + 95, siY_Pos + 77, 160, 12);

	// CD 연주. 사용 안함.
//	m_YCDOnOff.Init(FALSE, siX_Pos + 112, siY_Pos + 110);

	// 마우스 버튼 설정
	m_MouseButtonSwitch.Init(siX_Pos + 112, siY_Pos + 107);	
	
	// 확인 버튼 설정
	m_BOk.Create(siX_Pos + 110, siY_Pos + 147, 
					m_BSpr.Header.Xsize, m_BSpr.Header.Ysize, 
					pGame->pOnlineWorld->pOnlineText->Get(8000126), BUTTON_PUT_LEFT);
}

SHORT _YOption::Action()
{
	SHORT	Index = 0;	

	m_YEffectSoundVolume.Action(g_bPush);		// 효과음 볼륨 조절 바 처리
	m_YBackgroundMusicVolume.Action(g_bPush);	// 배경 음악 볼륨 조절 바 처리
	m_YGamma.Action(g_bPush);					// 화면 밝기 조절 바 처리
//	m_YCDOnOff.Action(g_bPush);	
	m_MouseButtonSwitch.Action(g_bPush);		// 마우스 버튼 설정 버튼 처리

	// 확인 버튼 클릭 처리
	if(m_BOk.Process(g_bPush) == TRUE)							
	{
//		m_YSoundVolume.RefreshData();
//		m_YCDOnOff.RefreshData();
		pGame->Config.SetEffectSoundVolume(m_YEffectSoundVolume.GetCurrentPos());			// 변경되는 효과음 볼륨 설정
		pGame->Config.SetBackgroundMusicVolume(m_YBackgroundMusicVolume.GetCurrentPos());	// 변경되는 배경음악 볼륨 설정

		m_MouseButtonSwitch.RefreshData();	// 마우스 버튼 설정 값 변경
		m_YGamma.RefreshData();				// 화변 밝기 적용

		pGame->Config.Save();				// config.ln으로 저장
		Index = -1;
	}

	if(IsGotoPreviousMenu() == TRUE)
	{
		m_YGamma.RestoreData();
		Index = -1;	
	}

	if(IpD.LeftPressSwitch)	g_bPush = TRUE;
	else	                g_bPush = FALSE;

	return Index;
}

void	_YOption::Put()
{	
	int	clipx1, clipy1, clipx2, clipy2;

	clGrp.GetClipArea(clipx1, clipy1, clipx2, clipy2);
	clGrp.SetClipFullScreen();	

	clGrp.PutSpriteJin(262, 136, m_Xspr.Header.Xsize, m_Xspr.Header.Ysize, m_Xspr.Image);
	clGrp.PutSpriteJin(262, 136, m_Xspr.Header.Xsize, m_Xspr.Header.Ysize, m_Xspr_Bkgnd.Image);

//	m_YCDOnOff.Draw();

	// 효과음 볼륨 조절 바 그리기
	clGrp.BoxC(siX_Pos + 97, siY_Pos + 32, 
				siX_Pos + 97 + 160 - 4, siY_Pos + 32 + 4,
				253);

	m_YEffectSoundVolume.Draw();

	// 배경 음악 볼륨 조절 바 그리기
	clGrp.BoxC(siX_Pos + 97, siY_Pos + 56,
				siX_Pos + 97 + 160 - 4, siY_Pos + 56 + 4,
				253);

	m_YBackgroundMusicVolume.Draw();

	// 화면 밝기 조절 바 그리기
	clGrp.BoxC(siX_Pos + 97, siY_Pos + 81,
				siX_Pos + 97 + 160 - 4, siY_Pos + 81 + 4,
				253);

	m_YGamma.Draw();

	// 마우스 버튼 설정 버튼 그리기
	m_MouseButtonSwitch.Draw();

	//결정하기및 취소하기 버튼을 그린다.
	m_BOk.Put(&m_BSpr, 0, 0, 1, BUTTON_PUT_NOMOVE);

	clGrp.SetClipArea(clipx1, clipy1, clipx2, clipy2);
}

VOID	_YOption::Put(HDC hdc)
{
	_Box		BSoundVolume, BMusic, BGamma, BMI;
	COLORREF					OldTextColor;		
	OnlineText					*pOnlineText;			// 온라인에서 사용하는 모든 텍스트를 모아 놓은 객체.
	OnlineMegaText				*pOnlineMegaText;		// Text 관리,	
	pOnlineText = pGame->pOnlineWorld->pOnlineText;
	pOnlineMegaText = pGame->pOnlineWorld->pOnlineMegaText;

	OldTextColor	=	SetTextColor(hdc, RGB(245, 245, 245));

	// 1000601	효과음 볼륨
	pOnlineMegaText->DrawTextCenter(hdc, siX_Pos + 15, siY_Pos + 25, 72, 16,
									pOnlineText->Get(ON_TEXT_GAME_OPTION_VOLUME_EFFECT), RGB(245, 245, 245));

	// 1000602	배경음악 볼륨	; CD 음악 On, Off 대신에 추가됨.
	pOnlineMegaText->DrawTextCenter(hdc, siX_Pos + 15, siY_Pos + 50, 72, 16,
									pOnlineText->Get(ON_TEXT_GAME_OPTION_VOLUME_BACKGROUNDMUSIC), RGB(245, 245, 245));

	// 1000603	화면밝기
	pOnlineMegaText->DrawTextCenter(hdc, siX_Pos + 15, siY_Pos + 75, 72, 16,
									pOnlineText->Get(ON_TEXT_GAME_OPTION_GAMMA), RGB(245, 245, 245));

	// 1000604	마우스방식
	pOnlineMegaText->DrawTextCenter(hdc, siX_Pos + 15, siY_Pos + 105, 72, 16,
									pOnlineText->Get(ON_TEXT_GAME_OPTION_MOUSE_BUTTON_METHOD), RGB(245, 245, 245));
	
	SetTextColor(hdc, OldTextColor);

	m_BOk.Put(hdc, BUTTON_PUT_NOMOVE);
	m_MouseButtonSwitch.Draw(hdc);	
//	m_YCDOnOff.Draw(hdc);
}

VOID _YOption::Free()
{
	m_YEffectSoundVolume.Free();
	m_YBackgroundMusicVolume.Free();
//	m_YCDOnOff.Free();
	m_MouseButtonSwitch.Free();
	m_YGamma.Free();

	clGrp.FreeXspr(m_Xspr);
	clGrp.FreeXspr(m_Xspr_Bkgnd);
	clGrp.FreeXspr(m_BSpr);
}
