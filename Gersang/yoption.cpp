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

// �¶��� �ؽ�Ʈ(OnlineText.dat)�� �̿��ϱ� ����. �� �̻� GameText.dat�� �̿����� �ʴ´�.
#include "OnlineWorld.h"
#include <OnlineText.h>
#include <OnlineMegaText.h>

static	BOOL				g_bPush;
static	SI16				siX_Pos, siY_Pos;

extern	char 				SavePath[];
extern	_Text 				Text;								// �ؽ�Ʈ 
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

	// ȿ���� ���� ���� �� ����
	if (g_pSoundManager)
	{
		m_YEffectSoundVolume.Init(g_pSoundManager->bSoundDriver, siX_Pos + 95, siY_Pos + 27, 160, 12);
		m_YEffectSoundVolume.SetCurrentPos(pGame->Config.m_siEffectSoundVolume);
	}

	// ��� ���� ���� ���� �� ����
	if (g_pSoundManager)
	{
		m_YBackgroundMusicVolume.Init(g_pSoundManager->bSoundDriver, siX_Pos + 95, siY_Pos + 52, 160, 12);
		m_YBackgroundMusicVolume.SetCurrentPos(pGame->Config.m_siBackgroundMusicVolume);
	}

	// ȭ�� ��� ���� ��
	m_YGamma.Init(siX_Pos + 95, siY_Pos + 77, 160, 12);

	// CD ����. ��� ����.
//	m_YCDOnOff.Init(FALSE, siX_Pos + 112, siY_Pos + 110);

	// ���콺 ��ư ����
	m_MouseButtonSwitch.Init(siX_Pos + 112, siY_Pos + 107);	
	
	// Ȯ�� ��ư ����
	m_BOk.Create(siX_Pos + 110, siY_Pos + 147, 
					m_BSpr.Header.Xsize, m_BSpr.Header.Ysize, 
					pGame->pOnlineWorld->pOnlineText->Get(8000126), BUTTON_PUT_LEFT);
}

SHORT _YOption::Action()
{
	SHORT	Index = 0;	

	m_YEffectSoundVolume.Action(g_bPush);		// ȿ���� ���� ���� �� ó��
	m_YBackgroundMusicVolume.Action(g_bPush);	// ��� ���� ���� ���� �� ó��
	m_YGamma.Action(g_bPush);					// ȭ�� ��� ���� �� ó��
//	m_YCDOnOff.Action(g_bPush);	
	m_MouseButtonSwitch.Action(g_bPush);		// ���콺 ��ư ���� ��ư ó��

	// Ȯ�� ��ư Ŭ�� ó��
	if(m_BOk.Process(g_bPush) == TRUE)							
	{
//		m_YSoundVolume.RefreshData();
//		m_YCDOnOff.RefreshData();
		pGame->Config.SetEffectSoundVolume(m_YEffectSoundVolume.GetCurrentPos());			// ����Ǵ� ȿ���� ���� ����
		pGame->Config.SetBackgroundMusicVolume(m_YBackgroundMusicVolume.GetCurrentPos());	// ����Ǵ� ������� ���� ����

		m_MouseButtonSwitch.RefreshData();	// ���콺 ��ư ���� �� ����
		m_YGamma.RefreshData();				// ȭ�� ��� ����

		pGame->Config.Save();				// config.ln���� ����
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

	// ȿ���� ���� ���� �� �׸���
	clGrp.BoxC(siX_Pos + 97, siY_Pos + 32, 
				siX_Pos + 97 + 160 - 4, siY_Pos + 32 + 4,
				253);

	m_YEffectSoundVolume.Draw();

	// ��� ���� ���� ���� �� �׸���
	clGrp.BoxC(siX_Pos + 97, siY_Pos + 56,
				siX_Pos + 97 + 160 - 4, siY_Pos + 56 + 4,
				253);

	m_YBackgroundMusicVolume.Draw();

	// ȭ�� ��� ���� �� �׸���
	clGrp.BoxC(siX_Pos + 97, siY_Pos + 81,
				siX_Pos + 97 + 160 - 4, siY_Pos + 81 + 4,
				253);

	m_YGamma.Draw();

	// ���콺 ��ư ���� ��ư �׸���
	m_MouseButtonSwitch.Draw();

	//�����ϱ�� ����ϱ� ��ư�� �׸���.
	m_BOk.Put(&m_BSpr, 0, 0, 1, BUTTON_PUT_NOMOVE);

	clGrp.SetClipArea(clipx1, clipy1, clipx2, clipy2);
}

VOID	_YOption::Put(HDC hdc)
{
	_Box		BSoundVolume, BMusic, BGamma, BMI;
	COLORREF					OldTextColor;		
	OnlineText					*pOnlineText;			// �¶��ο��� ����ϴ� ��� �ؽ�Ʈ�� ��� ���� ��ü.
	OnlineMegaText				*pOnlineMegaText;		// Text ����,	
	pOnlineText = pGame->pOnlineWorld->pOnlineText;
	pOnlineMegaText = pGame->pOnlineWorld->pOnlineMegaText;

	OldTextColor	=	SetTextColor(hdc, RGB(245, 245, 245));

	// 1000601	ȿ���� ����
	pOnlineMegaText->DrawTextCenter(hdc, siX_Pos + 15, siY_Pos + 25, 72, 16,
									pOnlineText->Get(ON_TEXT_GAME_OPTION_VOLUME_EFFECT), RGB(245, 245, 245));

	// 1000602	������� ����	; CD ���� On, Off ��ſ� �߰���.
	pOnlineMegaText->DrawTextCenter(hdc, siX_Pos + 15, siY_Pos + 50, 72, 16,
									pOnlineText->Get(ON_TEXT_GAME_OPTION_VOLUME_BACKGROUNDMUSIC), RGB(245, 245, 245));

	// 1000603	ȭ����
	pOnlineMegaText->DrawTextCenter(hdc, siX_Pos + 15, siY_Pos + 75, 72, 16,
									pOnlineText->Get(ON_TEXT_GAME_OPTION_GAMMA), RGB(245, 245, 245));

	// 1000604	���콺���
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
