#include <GSL.h>

#include <main.h>
#include <Mouse.h>


#include <colortable.h>
#include <etc.h>
#include <config.h>
#include <SoundManager.h>
#include "..\Gersang\directx.h"


#include <Button.h>
#include <OnlineWorld.h>
#include <debugtool.h>
#include "GMenu.h"
#include "ytitlefile.h"
#include "text.h"

#ifdef _OGG_PLAY
#include "BgmMgr_PreHeader.h"
#endif

#define		GMENU_MAINMUSIC_FILENAME		"music\\menumusic.YAV"
#define		GMENU_TEAMMUSIC_FILENAME		"music\\teamsong.YAV"

extern	XSPR 				TitleSpr;							// Ÿ��Ʋ ��� �׸��� ���� ���� 
extern	char 				SavePath[];
extern	_Text 				Text;								// �ؽ�Ʈ 
extern	_Config 			Config;								// ȯ�� ���� 
extern _InputDevice			IpD;

BOOL						g_bPush;	

static	_Button				BStart, BLoad, BOpening, BMulti, BChoosePlayer, BExit;
static	UI16				g_BackgroundMusicChannel;
static	BOOL				bChoosePlayer;

#ifdef _IMJIN2PLUS
	static	XSPR			TitelMainButton;
	#ifndef	_ENGLISH
		static	_Button		BHQNet;
		static	_Button		BOnline;
	#endif
#endif


extern SoundManager		   *g_pSoundManager;


//----------------------------------------------------------------------------------------------------
//  �Լ� :	
//  ���� : 
//----------------------------------------------------------------------------------------------------
BOOL InitInitMenu()
{	
	CHAR					szFileNameBuffer[1024];	


	g_bPush	=	FALSE;

	SetMouse(MOUSE_INITMENU);
  
	//���ȭ���� �ҷ��´�.
	if(TitleSpr.Image) 
	{
		clGrp.Error("2001", "kkk");	
	}

#ifdef _DEMO	
	#ifdef _IMJIN2PLUS
		#ifdef TYPE_YONSEI
			GetFileNamePath(Y_TITLE_FILE_MAIN_DEMO_YONSEI, SavePath, szFileNameBuffer);	
		#else
			GetFileNamePath(Y_TITLE_FILE_MAIN_DEMO, SavePath, szFileNameBuffer);	
		#endif
	#else
		GetFileNamePath(Y_TITLE_FILE_MAIN_DEMO, SavePath, szFileNameBuffer);	
	#endif
#else
		GetFileNamePath(Y_TITLE_FILE_MAIN, SavePath, szFileNameBuffer);
#endif

#ifndef _IMJIN2PLUS
	if( clGrp.LoadXspr(szFileNameBuffer, TitleSpr) == FALSE ) clGrp.Error("FKJE8567", "GMENU001 [%s]", szFileNameBuffer);			

	SetActionAllButton(TRUE);	
	

	BStart.Create(	459, 24,	
					g_Buttons_Spr[BUTTONS_MAINMENU_2_BUTTONS].Header.Xsize,	
					g_Buttons_Spr[BUTTONS_MAINMENU_2_BUTTONS].Header.Ysize,
					Text.Scenario.Get(), BUTTON_PUT_LEFT | BUTTON_PLAYSOUND_ONMOUSE, TRUE);

	BLoad.Create(	 459, 60,
					g_Buttons_Spr[BUTTONS_MAINMENU_2_BUTTONS].Header.Xsize,	
					g_Buttons_Spr[BUTTONS_MAINMENU_2_BUTTONS].Header.Ysize,	
					Text.Load.Get(), BUTTON_PUT_LEFT | BUTTON_PLAYSOUND_ONMOUSE, TRUE);
	
	BOpening.Create( 459, 96,	
					g_Buttons_Spr[BUTTONS_MAINMENU_2_BUTTONS].Header.Xsize,	
					g_Buttons_Spr[BUTTONS_MAINMENU_2_BUTTONS].Header.Ysize,	
					Text.Prologue.Get(), BUTTON_PUT_LEFT | BUTTON_PLAYSOUND_ONMOUSE, TRUE);

	BMulti.Create(	459, 132,	
					g_Buttons_Spr[BUTTONS_MAINMENU_2_BUTTONS].Header.Xsize,	
					g_Buttons_Spr[BUTTONS_MAINMENU_2_BUTTONS].Header.Ysize,	
					Text.CustomGame.Get(), BUTTON_PUT_LEFT | BUTTON_PLAYSOUND_ONMOUSE, TRUE);


	BChoosePlayer.Create( 459,	168,
					g_Buttons_Spr[BUTTONS_MAINMENU_2_BUTTONS].Header.Xsize,
					g_Buttons_Spr[BUTTONS_MAINMENU_2_BUTTONS].Header.Ysize,	
					Text.ChangeUser.Get(), BUTTON_PUT_LEFT | BUTTON_PLAYSOUND_ONMOUSE, TRUE);

	BExit.Create(	459,	204,	 
					g_Buttons_Spr[BUTTONS_MAINMENU_2_BUTTONS].Header.Xsize,	
					g_Buttons_Spr[BUTTONS_MAINMENU_2_BUTTONS].Header.Ysize,	
					Text.ButtonExit.Get(), BUTTON_PUT_LEFT | BUTTON_PLAYSOUND_ONMOUSE, TRUE);

#else		
	BYTE palette[768];
	//Ÿ��Ʋ�� �ε���
	if(clGrp.LoadXspr(szFileNameBuffer, TitleSpr)==FALSE) clGrp.Error("FKJE8567", "GMENU001 [%s]", szFileNameBuffer);

	//�ȷ�Ʈ�� �ε���
	GetFileNamePath( "pal\\title_plus.pal", SavePath, szFileNameBuffer );
	clGrp.LoadOnlyPaletteFile(szFileNameBuffer, palette);

	SetGamePalette(palette);

	InitDarkTable(palette);
	SetMouse(MOUSE_INITMENU);
   
	//��ư�� �׸��� �ҷ��´�.	
	GetFileNamePath("yfnt\\TitelMainButton_plus.spr", SavePath, szFileNameBuffer);
	if(clGrp.LoadXspr(szFileNameBuffer, TitelMainButton) == FALSE) clGrp.Error("guni", "exit");	


	SetActionAllButton(TRUE);	

	BStart.Create(	21, 488,	// �ó�����
					TitelMainButton.Header.Xsize, TitelMainButton.Header.Ysize,
					Text.Scenario.Get(), BUTTON_PUT_LEFT | BUTTON_PLAYSOUND_ONMOUSE, TRUE);

	BLoad.Create(	247, 488, // �ҷ�����
					TitelMainButton.Header.Xsize, TitelMainButton.Header.Ysize,
					Text.Load.Get(), BUTTON_PUT_LEFT | BUTTON_PLAYSOUND_ONMOUSE, TRUE);
	
	BOpening.Create( 21, 542,	// ����
					TitelMainButton.Header.Xsize, TitelMainButton.Header.Ysize,
					Text.Prologue.Get(), BUTTON_PUT_LEFT | BUTTON_PLAYSOUND_ONMOUSE, TRUE);

	BMulti.Create(	134, 542,	// ���� ����
					TitelMainButton.Header.Xsize, TitelMainButton.Header.Ysize,
					Text.CustomGame.Get(), BUTTON_PUT_LEFT | BUTTON_PLAYSOUND_ONMOUSE, TRUE);


	BChoosePlayer.Create( 134,	488, // ����ڼ���
					TitelMainButton.Header.Xsize, TitelMainButton.Header.Ysize,
					Text.ChangeUser.Get(), BUTTON_PUT_LEFT | BUTTON_PLAYSOUND_ONMOUSE, TRUE);

#ifdef	_ENGLISH
	BExit.Create(	247, 542,	 
					TitelMainButton.Header.Xsize, TitelMainButton.Header.Ysize,
					Text.ButtonExit.Get(), BUTTON_PUT_LEFT | BUTTON_PLAYSOUND_ONMOUSE, TRUE);	
#else
	BExit.Create(	360, 542,	 
					TitelMainButton.Header.Xsize, TitelMainButton.Header.Ysize,
					Text.ButtonExit.Get(), BUTTON_PUT_LEFT | BUTTON_PLAYSOUND_ONMOUSE, TRUE);	
#endif
	

#ifndef	_ENGLISH
	BHQNet.Create(	247, 542,	// ��
					TitelMainButton.Header.Xsize, TitelMainButton.Header.Ysize,
					"HQNET", BUTTON_PUT_LEFT | BUTTON_PLAYSOUND_ONMOUSE, TRUE);

	BOnline.Create(	360, 488,	 
					TitelMainButton.Header.Xsize, TitelMainButton.Header.Ysize,
					Text.ImjinOnline.Get(), BUTTON_PUT_LEFT | BUTTON_PLAYSOUND_ONMOUSE, TRUE);		
#endif

#endif // ������2PLUS�� ���

	// ����� �ó�����, �ҷ�����, ���� Ŭ���� �� ���� �Ѵ�.
#ifdef	_DEMO
	BStart.SetAction(FALSE);
	BLoad.SetAction(FALSE);
	BOpening.SetAction(FALSE);
	// ���� �������� hqnet ��ư Ŭ���� �� ���� �Ѵ�.
	#ifdef	TYPE_YONSEI
		#ifndef	_ENGLISH
			BHQNet.SetAction(FALSE);
		#endif
	#endif
#endif


	// Ŭ���� ȭ���� ��ü �������� �����Ѵ�.
	clGrp.SetClipFullScreen();

	return TRUE;
}

//----------------------------------------------------------------------------------------------------
//  �Լ� :	
//  ���� : 
//----------------------------------------------------------------------------------------------------
SHORT InitMenu()
{		
	SHORT				menuindex=0;						


	if(bChoosePlayer == TRUE)
	{
		BStart.SetProcess(FALSE);
		BLoad.SetProcess(FALSE);
		BMulti.SetProcess(FALSE);
		BOpening.SetProcess(FALSE);
		BChoosePlayer.SetProcess(FALSE);
		BExit.SetProcess(FALSE);
#ifdef _IMJIN2PLUS
	#ifndef	_ENGLISH
		BHQNet.SetProcess(FALSE);
		BOnline.SetProcess(FALSE);
	#endif
#endif
	}
	else
	{
		BStart.SetProcess(TRUE);
		BLoad.SetProcess(TRUE);
		BMulti.SetProcess(TRUE);
		BOpening.SetProcess(TRUE);
		BChoosePlayer.SetProcess(TRUE);
		BExit.SetProcess(TRUE);
#ifdef _IMJIN2PLUS
	#ifndef	_ENGLISH
		BHQNet.SetProcess(TRUE);
		BOnline.SetProcess(TRUE);
	#endif
#endif
	}

	//���� �̸��� ��� �޴��� ���� �ִ� ���̸� 
	//�̸��� ���� ������ �� ��ƾ�� ó���� �ش�.
	if(bChoosePlayer == TRUE)	
	{
	}
	else
	{
		if(BStart.Process(g_bPush) == TRUE)								menuindex=GS_INITCHOOSENATION;				
		if(BLoad.Process(g_bPush) == TRUE)								menuindex=GS_INITLOADGAMEINITMENU;				 
		if(BOpening.Process(g_bPush) == TRUE)							menuindex=GS_INITOPENING;				
		if(BMulti.Process(g_bPush) == TRUE)								menuindex=GS_INITSETMULTI;			
		if(BChoosePlayer.Process(g_bPush) == TRUE)						bChoosePlayer = TRUE;
		#ifndef	_ENGLISH
		if(BOnline.Process(g_bPush) == TRUE)							menuindex=GS_INITONLINEWORLD;
		#endif		
		if(BExit.Process(g_bPush) == TRUE)								menuindex=GS_INITQUIT;		
	}
	
	
	//���ǽ����ٰ� ��ư�� �׸��� �׸���.
	if(clGrp.LockSurface(SURFACE_BACK) == TRUE)
	{	
		clGrp.PutImage32(0, 0, TitleSpr.Header.Xsize, TitleSpr.Header.Ysize, 
					&TitleSpr.Image[TitleSpr.Header.Start[0]]);

#ifndef _IMJIN2PLUS
		BStart.Put(&g_Buttons_Spr[BUTTONS_MAINMENU_2_BUTTONS], 	
					-1,	0, 1,					
					BUTTON_PUT_NOMOVE);		

		BLoad.Put(&g_Buttons_Spr[BUTTONS_MAINMENU_2_BUTTONS], 	
					-1,	2, 3,					
					BUTTON_PUT_NOMOVE);		

		BOpening.Put(&g_Buttons_Spr[BUTTONS_MAINMENU_2_BUTTONS], 	
					-1,	4, 5,					
					BUTTON_PUT_NOMOVE);		

		BMulti.Put(&g_Buttons_Spr[BUTTONS_MAINMENU_2_BUTTONS], 	
					-1,	6, 7,				
					BUTTON_PUT_NOMOVE);		

		BChoosePlayer.Put(&g_Buttons_Spr[BUTTONS_MAINMENU_2_BUTTONS],
					-1,	8, 9,					
					BUTTON_PUT_NOMOVE);

		BExit.Put(&g_Buttons_Spr[BUTTONS_MAINMENU_2_BUTTONS], 	
					-1,	10, 11,					
					BUTTON_PUT_NOMOVE);	
#else
		BStart.Put       (&TitelMainButton, 0,	1,  2, BUTTON_PUT_NOMOVE);				
		BLoad.Put        (&TitelMainButton, 0,	1,  2, BUTTON_PUT_NOMOVE);		
		BOpening.Put     (&TitelMainButton, 0,	1,  2, BUTTON_PUT_NOMOVE);		
		BMulti.Put       (&TitelMainButton, 0,	1,  2, BUTTON_PUT_NOMOVE);		
		BChoosePlayer.Put(&TitelMainButton, 0,	1,  2, BUTTON_PUT_NOMOVE);		
		BExit.Put        (&TitelMainButton, 0,	1,  2, BUTTON_PUT_NOMOVE);		

	#ifndef	_ENGLISH
		BHQNet.Put       (&TitelMainButton, 0,	1,  2, BUTTON_PUT_NOMOVE);		
		BOnline.Put       (&TitelMainButton, 0,	1,  2, BUTTON_PUT_NOMOVE);				
	#endif

#endif

		clGrp.UnlockSurface(SURFACE_BACK);
	}


#ifndef	_IMJIN2PLUS
#ifdef _ENGLISH
		BStart.Put       (clGrp.lpDDSBack, BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);		
		BLoad.Put        (clGrp.lpDDSBack, BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);		
		BOpening.Put     (clGrp.lpDDSBack, BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);		
		BMulti.Put       (clGrp.lpDDSBack, BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);		
		BChoosePlayer.Put(clGrp.lpDDSBack, BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);		
		BExit.Put        (clGrp.lpDDSBack, BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);		
#endif
#endif
	

	
	// ���콺�� �׷��ְ� ȭ���� �����Ѵ�. 
	UpdateScreen();	

	if(IpD.LeftPressSwitch)	g_bPush=TRUE;	
	else				g_bPush=FALSE;	

	if(menuindex != 0)
	{
		FreeInitMenu();
	}

	return menuindex;
}

//----------------------------------------------------------------------------------------------------
//  �Լ� :	
//  ���� : 
//----------------------------------------------------------------------------------------------------
VOID	FreeInitMenu()
{
	if(TitleSpr.Image) clGrp.FreeXspr(TitleSpr);	
	if(TitelMainButton.Image) clGrp.FreeXspr(TitelMainButton);	
}



//----------------------------------------------------------------------------------------------------
//  �Լ� :	
//  ���� : 
//----------------------------------------------------------------------------------------------------
VOID PlayStaffBackgroundMusic()
{
	CHAR	szFileNameBuffer[1024];

	if(g_BackgroundMusicChannel==0)
	{		
		GetFileNamePath(GMENU_TEAMMUSIC_FILENAME, SavePath, szFileNameBuffer);

		if(g_pSoundManager)
			g_BackgroundMusicChannel = g_pSoundManager->AddPlayID(szFileNameBuffer, TRUE, TRUE);
	}

	if(g_BackgroundMusicChannel)
	{
		if(g_pSoundManager)
		{
			if( g_pSoundManager->IsPlaying(g_BackgroundMusicChannel) == FALSE )
			{
				g_pSoundManager->PlaySound(g_BackgroundMusicChannel);
			}
		}
	}
}


//----------------------------------------------------------------------------------------------------
//  �Լ� :	
//  ���� : 
//----------------------------------------------------------------------------------------------------
void StopMenuBackgroundMusic()
{
#ifdef _OGG_PLAY
	if( GetBgmMgr()->IsPlaying())
		GetBgmMgr()->Close();
#else
	if(g_BackgroundMusicChannel)
	{
		if(g_pSoundManager)
		{
			if( g_pSoundManager->IsPlaying(g_BackgroundMusicChannel) )
				g_pSoundManager->StopSound(g_BackgroundMusicChannel);
		}
	}
#endif
}


//----------------------------------------------------------------------------------------------------
//  �Լ� :	
//  ���� : 
//----------------------------------------------------------------------------------------------------
void FreeMenuBackgroundMusic()
{
#ifdef _OGG_PLAY
#else
	if( g_pSoundManager )
	{
		if(g_BackgroundMusicChannel)
		{
			StopMenuBackgroundMusic();
			g_pSoundManager->ReleaseSound(g_BackgroundMusicChannel);
			g_BackgroundMusicChannel = 0;
		}
	}
#endif
}

