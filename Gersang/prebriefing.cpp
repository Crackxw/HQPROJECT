#include <main.h>
#include <Mouse.h>

#ifdef _IMJIN2
	#include <BNCa_Header.H>
	#include "maindata.h"
	#include <etc.h>
	#include "..\Gersang\Directx.h"
	#include "stage.h"
#endif

#include "Gmenu.h"
#include "prebriefing.h"
#include "yprg.h"
#include "text.h"
#include <script.h>

extern _Stage 					Stage;								// 스테이지 
extern	_SaveData				SD;
extern	char 					SavePath[];
extern	_Text 					Text;								// 텍스트 
extern	_Briefing 				Briefing;
extern  HFONT 					SaveInfoFont;
extern _InputDevice				IpD;

extern	LPDIRECTDRAWSURFACE7	lpBriefingBuffer;
extern	LPDIRECTDRAWSURFACE7	lpScreenBuffer;

#ifdef _IMJIN2PLUS
	XSPR						PreBridefingButtonXspr;
	_Button						ExitButton, ReplayButton, GameStartButton;
#endif

static	BOOL					GbPush;

_Y_Script						Script;


////////////////////////////////////////////////////////////////////////////////////////
/// VOID InitPreBriefing()
/// 목적 : PreBriefing을 초기화 한다.
////////////////////////////////////////////////////////////////////////////////////////
VOID InitPreBriefing()
{	
	CHAR	szFileNameBuffer[1024];

	GbPush	=	FALSE;

#ifdef _IMJIN2PLUS
	// 화면 해상도를 설정한다. 
	if(clGrp.ChangeVideoMode(MODE_GRAPHICS800_600) == FALSE) clGrp.Error("PreBridefing", "ChangeVideoMode(800*600)");
	clGrp.SetAllPalette();
	// 임의의 서피스를 다시 만든다. 
	CreateSurface();
#endif

	FreeMenuBackgroundMusic();
	
	//스크립트를 로드하고 연주한다.
	GetFileNamePath(Stage.ScriptFileName[SD.GlobalStageNumber], SavePath, szFileNameBuffer);	
	Script.LoadFile(szFileNameBuffer);	
	Script.Play();	

#ifndef _IMJIN2PLUS
	SetActionAllButton(TRUE);	
#else
	// 임진록2 PLUS : 버튼에 사용할 그림을 불러온다.
	GetFileNamePath("yfnt\\PreBridefingButton_plus.spr", SavePath, szFileNameBuffer);
	if(clGrp.LoadXspr(szFileNameBuffer, PreBridefingButtonXspr) == FALSE) clGrp.Error("guni", "PreBridefing Button Error");
	// 임진록2 PLUS : 버튼을 생성한다.
	ExitButton.Create     (150, 530, PreBridefingButtonXspr.Header.Xsize, PreBridefingButtonXspr.Header.Ysize, Text.ButtonReturn.Get(), BUTTON_PUT_LEFT, TRUE);
	ReplayButton.Create	  (350, 530, PreBridefingButtonXspr.Header.Xsize, PreBridefingButtonXspr.Header.Ysize, Text.ButtonReview.Get(), BUTTON_PUT_LEFT, TRUE);
	GameStartButton.Create(550, 530, PreBridefingButtonXspr.Header.Xsize, PreBridefingButtonXspr.Header.Ysize, Text.ButtonGame.Get(), BUTTON_PUT_LEFT, TRUE);
#endif
}

//----------------------------------------------------------------------------------------------------
/// 함수 : 
/// 설명 : 
//----------------------------------------------------------------------------------------------------
SHORT PreBriefing()
{	
	SHORT	menuindex = 0;		
	HDC		hdc;
	
	Script.Action();			
	Briefing.Action();	

	if(Script.IsCooperativeMode(Y_SCRIPT_COMMAND_PROCESSMODE) == TRUE)
	{
#ifndef _IMJIN2PLUS
		if(g_Button[BUTTON_GLOBAL_GOPLAY].Process(GbPush) == TRUE)		menuindex = GS_INITSTAGEGAME;					
		if(g_Button[BUTTON_GLOBAL_BACK].Process(GbPush) == TRUE)		menuindex = GS_INITCHOOSENATION;		
		if(g_Button[BUTTON_GLOBAL_REPLAY].Process(GbPush) == TRUE)		Script.Replay();	
#else
		// 임진록2 PLUS : 버튼의 상태를 확인한다.(ExitButton, ReplayButton, GameStartButton)	
		if(ExitButton.Process(GbPush)      == TRUE)	 menuindex = GS_INITCHOOSENATION; // 나가기
		if(ReplayButton.Process(GbPush)    == TRUE)	 Script.Replay();				  // 다시보기	
		if(GameStartButton.Process(GbPush) == TRUE)	 menuindex = GS_INITSTAGEGAME;	  // 시작하기
#endif
	}	

	if(Briefing.IsInBuffer()==TRUE) 	Briefing.Draw(clGrp.lpDDSBack);		

	if(Script.IsCooperativeMode(Y_SCRIPT_COMMAND_DRAWMODE) == TRUE)
	{
		if(clGrp.LockSurface(SURFACE_BACK)==TRUE)
		{		
#ifndef _IMJIN2PLUS
			g_Button[BUTTON_GLOBAL_GOPLAY].Put(&g_Buttons_Spr[BUTTONS_GLOBAL_BUTTONS], 
											0, 1, 2,
											BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER
											);		

			g_Button[BUTTON_GLOBAL_BACK].Put(&g_Buttons_Spr[BUTTONS_GLOBAL_BUTTONS], 
											0, 1, 2,
											BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER
											);		
			
			
			g_Button[BUTTON_GLOBAL_REPLAY].Put(&g_Buttons_Spr[BUTTONS_GLOBAL_BUTTONS], 
											0, 1, 2,
											BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER
											);					
#else
			// 임진록2 PLUS : 버튼을 찍어준다.(ExitButton, ReplayButton, GameStartButton)
			ExitButton.Put     (&PreBridefingButtonXspr, 0, 1, 2, BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);
			ReplayButton.Put   (&PreBridefingButtonXspr, 0, 1, 2, BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);
			GameStartButton.Put(&PreBridefingButtonXspr, 0, 1, 2, BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);
#endif
			clGrp.UnlockSurface(SURFACE_BACK);
		}		
	}
#ifndef _IMJIN2PLUS
	g_Button[BUTTON_GLOBAL_GOPLAY].Put(clGrp.lpDDSBack, BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);
	g_Button[BUTTON_GLOBAL_BACK].Put(clGrp.lpDDSBack, BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);
	g_Button[BUTTON_GLOBAL_REPLAY].Put(clGrp.lpDDSBack, BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);
#else
	if(clGrp.lpDDSBack->GetDC( &hdc ) == DD_OK)
	{
		SetBkMode(hdc, TRANSPARENT);
		SelectObject(hdc, SaveInfoFont);		
		// 임진록2 PLUS : 버튼을 찍어준다.(ExitButton, ReplayButton, GameStartButton)
		ExitButton.Put(hdc, BUTTON_PUT_NOMOVE);
		ReplayButton.Put(hdc, BUTTON_PUT_NOMOVE);
		GameStartButton.Put(hdc, BUTTON_PUT_NOMOVE);

		clGrp.lpDDSBack->ReleaseDC( hdc );
	}
#endif

	UpdateScreen();	

	if(IpD.LeftPressSwitch)			GbPush=TRUE;	
	else						GbPush=FALSE;	

	if(menuindex)
	{		
		FreePreBriefing();
	}

	return menuindex;
}

//----------------------------------------------------------------------------------------------------
/// 함수 : 
/// 설명 : 
//----------------------------------------------------------------------------------------------------
VOID	FreePreBriefing()
{
	if(Script.IsLoad() == TRUE)
	{
		if(Script.IsPlay() == TRUE)		Script.Stop();
		Script.Free();
	}				
}