#ifndef _ROCKET_NEW_TILE_MAP
////////////////////////////여기서 부터는 게임 메뉴에 대한 항목////////////////////////

#include <GSL.h>

#include <main.h>
#include <Mouse.h>
#include <clGame.h>


#include <Game2.h>
#include "maindata.h"
#include <etc.h>


#include "ytitlefile.h"
#include "gamemenu.h"
#include "yoptionmenu.h"
#include "YOkCancel.h"
#include "text.h"
#include "OnlineWorld.h"

extern	_clGame					*pGame;
extern	int						bActive;							// 현재 액티브인지 아닌지 알려주는 변수  
extern _InputDevice				IpD;
extern	char 					SavePath[];
extern	XSPR 					TitleSpr;							// 타이틀 배경 그림에 사용될 포맷 
extern	_Text 					Text;								// 텍스트 
extern	_MainData				GD;									// 게임의 주데이터 
extern	SI32					UserPlayerNumber;					// 사용자 관련 
extern _NewID					ZeroID;
extern	SHORT				GameStatus;							// 현재의 게임 상황을 알리는 변수 
extern  SHORT					MapEditMode;

extern	LPDIRECTDRAWSURFACE7	lpScreenBuffer;
extern	LPDIRECTDRAWSURFACE7	lpBriefingScrollBuffer;

extern  int                     WindowModeSwitch;

static	_Button					BInitMenu,							// 초기메뉴
								BOption,							// 환경설정
								BObjective,							// 게임 목적
								BSave,								// 저장하기
								BLoad,								// 불러오기
								BContinue,							// 계속하기
								BExit,								// 종료하기
								BRestart;							// 다시하기

static	BOOL					GbIsExitMenu;
static	BOOL					g_IsGotoPreviousMenu;
static	SHORT					g_CurrentMenu;
static	BOOL					GbPush;

#ifdef _IMJIN2PLUS
	static	SI16				siX_Pos = 0, siY_Pos = 0;
#endif

VOID	InitMainMenu();
SHORT	MainMenu(BOOL bPush);

//----------------------------------------------------------------------------------------------------
/// 함수 : 
/// 설명 : 
//----------------------------------------------------------------------------------------------------
VOID InitGameMenu()
{		
	GbIsExitMenu			=	FALSE;
	g_IsGotoPreviousMenu	=	FALSE;
	g_CurrentMenu			=	Y_GAME_MENU_INITMAIN;			
	GbPush					=	FALSE;
	
	// 게임 메뉴에 들어오기전의 화면을 lpBriefingBuffer안에 넣는다.
	RECT rect;
	rect.left = 0;
	rect.right = clGrp.GetScreenXsize();
	rect.top   = 0;
	rect.bottom = clGrp.GetScreenYsize();

	lpBriefingScrollBuffer->BltFast(0, 0, lpScreenBuffer, &rect, DDBLTFAST_WAIT);
}


//----------------------------------------------------------------------------------------------------
/// 함수 : 
/// 설명 : 
//----------------------------------------------------------------------------------------------------
VOID	InitGameLoadMenu()
{		
}

//----------------------------------------------------------------------------------------------------
/// 함수 : 
/// 설명 : 
//----------------------------------------------------------------------------------------------------
SHORT GameMenu()
{
	SHORT	nNextMenu;	

	if(!bActive)
		return FALSE;

	// 배경을 그린다.	
	lpScreenBuffer->BltFast(0, 0, lpBriefingScrollBuffer, NULL, DDBLTFAST_WAIT);
	
	switch(g_CurrentMenu)
	{
	case Y_GAME_MENU_INITMAIN:		
		InitMainMenu();
		g_CurrentMenu	=	Y_GAME_MENU_MAIN;		
		break;
	case Y_GAME_MENU_MAIN:
		nNextMenu		=	MainMenu(GbPush);
		if(nNextMenu != 0)
		{
			FreeMainMenu();
			g_CurrentMenu	=	nNextMenu;			
		}			
		break;
	case Y_GAME_MENU_INITSAVE:		
		break;
	case Y_GAME_MENU_SAVE:
		break;
	case Y_GAME_MENU_INITLOAD:				
		break;
	case Y_GAME_MENU_LOAD:		
		break;
	case Y_GAME_MENU_INITOPTION:
		InitOptionMenu();
		g_CurrentMenu	=	Y_GAME_MENU_OPTION;		
		break;
	case Y_GAME_MENU_OPTION:		
		nNextMenu		=	OptionMenu();

		if(nNextMenu != 0)
		{
			g_CurrentMenu	=	nNextMenu;
			FreeOptionMenu();
		}		
		break;		
	case Y_GAME_MENU_INITOBJECTIVE:
		g_CurrentMenu		=	Y_GAME_MENU_OBJECTIVE;			
		break;
	case Y_GAME_MENU_OBJECTIVE:
		break;
	}	

	// 마우스를 그려주고 화면을 갱신한다. 
	DrawMouse(lpScreenBuffer);
	
	// 프라이머리 서피스를 갱신해준다.	
	RECT rect;
	rect.left = 0;
	rect.right = clGrp.GetScreenXsize();
	rect.top   = 0;
	rect.bottom = clGrp.GetScreenYsize();

	if (WindowModeSwitch) {
		pGame->pOnlineWorld->UpdateScreen(lpScreenBuffer);
	}
	else {
		if( clGrp.lpClipper )
			clGrp.lpDDSPrimary->Blt(NULL, lpScreenBuffer, &rect, DDBLT_WAIT, NULL);
		else
			clGrp.BltFast(SURFACE_PRIMARY, 0, 0, lpScreenBuffer, &rect, DDBLTFAST_WAIT);
	}
	
	if(IpD.LeftPressSwitch)		GbPush=TRUE;	
	else					GbPush=FALSE;	

	
	switch(g_CurrentMenu)
	{
	case GS_GAME:			
		return GS_GAME;
	case GS_INITINITMENU:	
		if(GameStatus == GS_GAMEMENU)
		{
			GD.InitInitMenuCount = 20;
			MakeOrder(UserPlayerNumber, ORDER_QUIT, 0, ZeroID, TRUE);

			// 시간이 지나지 않았는데 퇴각하는 것이면 경험치를 깎는다.
			if(GD.PlayTime_CurrentTime < RETREAT_PERMIT_CLOCK)
			{
				SendBattleMessage_LoseForRetreat();
			}


			return GS_GAME;
		}
		else
		{
			return GS_INITINITMENU;
		}
		
	case GS_INITRESTART:	
		return GS_INITRESTART;		
	case GS_INITQUIT:	
		GD.InitQuitCount = 20;
		MakeOrder(UserPlayerNumber, ORDER_QUIT, 0, ZeroID, TRUE);
		return GS_GAME;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// VOID	SetGotoPreviousMenu()
/// 목적 : 이전 메뉴로 돌아가라고 명령한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	SetGotoPreviousMenu()
{
	 g_IsGotoPreviousMenu	=	TRUE;	 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BOOL	IsGotoPreviousMenu()
/// 목적 : 이전 메뉴로 돌아갈 것인지 검사한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	IsGotoPreviousMenu()
{	
	if(g_IsGotoPreviousMenu == TRUE)
	{
		g_IsGotoPreviousMenu  = FALSE;
		return TRUE;
	}
	else
		return FALSE;	
}

//---------------------------------------------------------------------------------------------
//	설명	:	메뉴를 나가도록 명령한다.	
//---------------------------------------------------------------------------------------------
VOID	SetExitMenu()
{	
	GbIsExitMenu	=	TRUE;
}

//---------------------------------------------------------------------------------------------
//	설명	:	메뉴를 나갈 건지의 여부를 알아온다.
//	리턴	:	TRUE 메뉴를 나간다. FALSE 메뉴를 나가지 않는다.
//---------------------------------------------------------------------------------------------
BOOL	IsExitMenu()
{	

	if(GbIsExitMenu)
	{		
		GbIsExitMenu	=	FALSE;
		return TRUE;
	}
	else
		return FALSE;		
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// VOID	InitMainMenu()
/// 함수 : 게임 메뉴의 메인 초기화. 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	InitMainMenu()
{	
	CHAR	szFileNameBuffer[1024];	
	//버튼을 로드 한다.(버튼의 배경)
	GetFileNamePath(Y_TITLE_FILE_GAMEMENUBORDER, SavePath, szFileNameBuffer);
	if(TitleSpr.Image) clGrp.Error("FKJE8567", "GAMEMENU000");
	if(clGrp.LoadXspr(szFileNameBuffer, TitleSpr)==FALSE) clGrp.Error("FKJE8567", "GAMEMENU001 [%s]", szFileNameBuffer);		

#ifdef _IMJIN2PLUS
	siX_Pos = clGrp.GetScreenXsize()/2 - (TitleSpr.Header.Xsize)/2; 
	siY_Pos = clGrp.GetScreenYsize()/2 - (TitleSpr.Header.Ysize)/2; 
#endif


	BContinue.Create( siX_Pos+30, siY_Pos+ 19-45, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Xsize, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Ysize, Text.Continue.Get(),		BUTTON_PUT_LEFT);		
	BObjective.Create(siX_Pos+30, siY_Pos+ 54-45, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Xsize, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Ysize, Text.Objectives.Get(),	BUTTON_PUT_LEFT);		
	BOption.Create(	  siX_Pos+30, siY_Pos+ 89-45, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Xsize, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Ysize, Text.Optoin.Get(),		BUTTON_PUT_LEFT);		
	BSave.Create(	  siX_Pos+30, siY_Pos+124-45, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Xsize, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Ysize, Text.Save.Get(),			BUTTON_PUT_LEFT);		
	BLoad.Create(	  siX_Pos+30, siY_Pos+159-45, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Xsize, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Ysize, Text.Load.Get(),			BUTTON_PUT_LEFT);		
	BRestart.Create(  siX_Pos+30, siY_Pos+194-45, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Xsize, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Ysize, Text.Replay.Get(),		BUTTON_PUT_LEFT);		
	BInitMenu.Create( siX_Pos+30, siY_Pos+229-45, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Xsize, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Ysize, Text.InitMenu.Get(),		BUTTON_PUT_LEFT);		
	BExit.Create(	  siX_Pos+30, siY_Pos+264-45, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Xsize, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Ysize, Text.ButtonExit.Get(),	BUTTON_PUT_LEFT);		

	BContinue.SetAction(TRUE);	
	BOption.SetAction(TRUE);

	BInitMenu.SetAction(TRUE);


	// 지도 편집기 모드에서는 종료 버튼을 활성화시킨다. 
	if(MapEditMode == TRUE)
	{
		BExit.SetAction(TRUE);	
	}
	else
	{
		BExit.SetAction(FALSE);	
	}

	BObjective.SetAction(FALSE);
	BSave.SetAction(FALSE);
	BLoad.SetAction(FALSE);
	BRestart.SetAction(FALSE);

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SHORT	MainMenu(BOOL bPush)
/// 함수 : 게임 메뉴의 메인 프로시저
////////////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT	MainMenu(BOOL bPush)
{
	SHORT	menuindex	=	0;		
	int  	ClipLeft, ClipTop, ClipRight, ClipBottom;
	static	SHORT	nReturnMenu;
	SHORT	nOkCancel;
	HDC		hdc;
	HFONT	OldFont;

	

	if(YOkCancel.IsAction() == TRUE)
	{
		// 메뉴를 보는 중에 게임이 종료되었으면 강제로 다이알로그를 없앤다.
		if(IsExitMenu() == TRUE)
		{			
			YOkCancel.Free();
			menuindex	=	GS_GAME;
		}
		else
		{
			if(YOkCancel.Action(bPush, &nOkCancel) == TRUE)
			{			
				YOkCancel.Free();

				if(nOkCancel == Y_OKCANCEL_OK)
					menuindex	=	nReturnMenu;
			}
		}

		if(YOkCancel.IsAction() == TRUE)
			YOkCancel.Draw(lpScreenBuffer);
	}
	else
	{
		if(IsGotoPreviousMenu() == TRUE)			menuindex	=	GS_GAME;
		if(IsExitMenu() == TRUE)					menuindex	=	GS_GAME;
		if(BContinue.Process(bPush) == TRUE)		
		{
			menuindex	=	GS_GAME;	
			GD.m_bBlackCheck = TRUE;
		}
		if(BObjective.Process(bPush) == TRUE)		menuindex	=	Y_GAME_MENU_INITOBJECTIVE;	
		if(BOption.Process(bPush) == TRUE)			menuindex	=	Y_GAME_MENU_INITOPTION;	

		if(BInitMenu.Process(bPush) == TRUE)
		{
			YOkCancel.Create(Text.ReturnMenuMessage.Get());
			nReturnMenu	=	GS_INITINITMENU;	
		}

		if(BLoad.Process(bPush) == TRUE)			menuindex	=	Y_GAME_MENU_INITLOAD;	
		if(BSave.Process(bPush) == TRUE)			menuindex	=	Y_GAME_MENU_INITSAVE;		

		if(BRestart.Process(bPush) == TRUE)
		{
			YOkCancel.Create(Text.RestartMessage.Get());			
			nReturnMenu	=	GS_INITRESTART;			
		}

		if(BExit.Process(bPush) == TRUE)
		{
			YOkCancel.Create(Text.QuitMessage.Get());			
			nReturnMenu	=	GS_INITQUIT;		
		}

		if(clGrp.LockSurface(lpScreenBuffer) == TRUE)
		{
			//클리핑 영역을 바꾼다.
			clGrp.GetClipArea(ClipLeft, ClipTop, ClipRight, ClipBottom);
			clGrp.SetClipArea(0, 0, clGrp.GetScreenXsize()-1, clGrp.GetScreenYsize()-1);		

#ifdef	_IMJIN2PLUS
			// 임진록 2에서 게임 도중의 메뉴에 대하여...
			clGrp.PutImage32(siX_Pos, siY_Pos-45, TitleSpr.Header.Xsize, TitleSpr.Header.Ysize, 
				&TitleSpr.Image[TitleSpr.Header.Start[0]]);			
#else
			clGrp.PutImage32(234, 56, TitleSpr.Header.Xsize, TitleSpr.Header.Ysize, 
						&TitleSpr.Image[TitleSpr.Header.Start[0]]);			
#endif

			//중국테스트.
			BContinue.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS],		30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);
			BObjective.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS], 	30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);			
			BOption.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS],		30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);
			BInitMenu.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS],		30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);			
			BSave.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS], 		30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);
			BLoad.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS], 		30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);		
			BRestart.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS], 		30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);
			BExit.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS], 		30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);

			clGrp.UnlockSurface(lpScreenBuffer);

			//클리핑 영역을 다시 복구한다.
			clGrp.SetClipArea(ClipLeft, ClipTop, ClipRight, ClipBottom);	
		}				

		if(lpScreenBuffer->GetDC(&hdc) == DD_OK)
		{
			SetBkMode(hdc, TRANSPARENT);					
			OldFont	=	(HFONT) SelectObject(hdc, pGame->pBattle->BriefingFont);
				
			BContinue.Put(hdc, BUTTON_PUT_NOMOVE);
			BObjective.Put(hdc, BUTTON_PUT_NOMOVE);
			BOption.Put(hdc, BUTTON_PUT_NOMOVE);
			BInitMenu.Put(hdc, BUTTON_PUT_NOMOVE);
			BSave.Put(hdc, BUTTON_PUT_NOMOVE);
			BLoad.Put(hdc, BUTTON_PUT_NOMOVE);
			BRestart.Put(hdc, BUTTON_PUT_NOMOVE);
			BExit.Put(hdc, BUTTON_PUT_NOMOVE);

			SelectObject(hdc, OldFont);
			lpScreenBuffer->ReleaseDC(hdc);
		}
	}

	return menuindex;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// VOID	FreeMainMenu()
/// 함수 : MainMenu에서 할당했던 메모리를 해제한다.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	FreeMainMenu()
{

	if(TitleSpr.Image)			clGrp.FreeXspr(TitleSpr);			
}



#else

////////////////////////////여기서 부터는 게임 메뉴에 대한 항목////////////////////////

#include <GSL.h>

#include <main.h>
#include <Mouse.h>
#include <clGame.h>


#include <Game2.h>
#include "maindata.h"
#include <etc.h>


#include "ytitlefile.h"
#include "gamemenu.h"
#include "yoptionmenu.h"
#include "YOkCancel.h"
#include "text.h"
#include "OnlineWorld.h"

extern	_clGame					*pGame;
extern	int						bActive;							// 현재 액티브인지 아닌지 알려주는 변수  
extern _InputDevice				IpD;
extern	char 					SavePath[];
extern	XSPR 					TitleSpr;							// 타이틀 배경 그림에 사용될 포맷 
extern	_Text 					Text;								// 텍스트 
extern	_MainData				GD;									// 게임의 주데이터 
extern	SI32					UserPlayerNumber;					// 사용자 관련 
extern _NewID					ZeroID;
extern	SHORT				GameStatus;							// 현재의 게임 상황을 알리는 변수 
extern  SHORT					MapEditMode;

extern	LPDIRECTDRAWSURFACE7	lpScreenBuffer;
extern	LPDIRECTDRAWSURFACE7	lpBriefingScrollBuffer;

extern  int                     WindowModeSwitch;

static	_Button					BInitMenu,							// 초기메뉴
								BOption,							// 환경설정
								BObjective,							// 게임 목적
								BSave,								// 저장하기
								BLoad,								// 불러오기
								BContinue,							// 계속하기
								BExit,								// 종료하기
								BRestart;							// 다시하기

static	BOOL					GbIsExitMenu;
static	BOOL					g_IsGotoPreviousMenu;
static	SHORT					g_CurrentMenu;
static	BOOL					GbPush;

#ifdef _IMJIN2PLUS
	static	SI16				siX_Pos = 0, siY_Pos = 0;
#endif

VOID	InitMainMenu();
SHORT	MainMenu(BOOL bPush);

//----------------------------------------------------------------------------------------------------
/// 함수 : 
/// 설명 : 
//----------------------------------------------------------------------------------------------------
VOID InitGameMenu()
{		
	GbIsExitMenu			=	FALSE;
	g_IsGotoPreviousMenu	=	FALSE;
	g_CurrentMenu			=	Y_GAME_MENU_INITMAIN;			
	GbPush					=	FALSE;
	
	// 게임 메뉴에 들어오기전의 화면을 lpBriefingBuffer안에 넣는다.
	RECT rect;
	rect.left = 0;
	rect.right = clGrp.GetScreenXsize();
	rect.top   = 0;
	rect.bottom = clGrp.GetScreenYsize();

	lpBriefingScrollBuffer->BltFast(0, 0, lpScreenBuffer, &rect, DDBLTFAST_WAIT);
}


//----------------------------------------------------------------------------------------------------
/// 함수 : 
/// 설명 : 
//----------------------------------------------------------------------------------------------------
VOID	InitGameLoadMenu()
{		
}

//----------------------------------------------------------------------------------------------------
/// 함수 : 
/// 설명 : 
//----------------------------------------------------------------------------------------------------
SHORT GameMenu()
{
	SHORT	nNextMenu;	

	if(!bActive)
		return FALSE;

	// 배경을 그린다.	
	lpScreenBuffer->BltFast(0, 0, lpBriefingScrollBuffer, NULL, DDBLTFAST_WAIT);
	
	switch(g_CurrentMenu)
	{
	case Y_GAME_MENU_INITMAIN:		
		InitMainMenu();
		g_CurrentMenu	=	Y_GAME_MENU_MAIN;		
		break;
	case Y_GAME_MENU_MAIN:
		nNextMenu		=	MainMenu(GbPush);
		if(nNextMenu != 0)
		{
			FreeMainMenu();
			g_CurrentMenu	=	nNextMenu;			
		}			
		break;
	case Y_GAME_MENU_INITSAVE:		
		break;
	case Y_GAME_MENU_SAVE:
		break;
	case Y_GAME_MENU_INITLOAD:				
		break;
	case Y_GAME_MENU_LOAD:		
		break;
	case Y_GAME_MENU_INITOPTION:
		InitOptionMenu();
		g_CurrentMenu	=	Y_GAME_MENU_OPTION;		
		break;
	case Y_GAME_MENU_OPTION:		
		nNextMenu		=	OptionMenu();

		if(nNextMenu != 0)
		{
			g_CurrentMenu	=	nNextMenu;
			FreeOptionMenu();
		}		
		break;		
	case Y_GAME_MENU_INITOBJECTIVE:
		g_CurrentMenu		=	Y_GAME_MENU_OBJECTIVE;			
		break;
	case Y_GAME_MENU_OBJECTIVE:
		break;
	}	

	// 마우스를 그려주고 화면을 갱신한다. 
	DrawMouse(lpScreenBuffer);
	
	// 프라이머리 서피스를 갱신해준다.	
	RECT rect;
	rect.left = 0;
	rect.right = clGrp.GetScreenXsize();
	rect.top   = 0;
	rect.bottom = clGrp.GetScreenYsize();

	if (WindowModeSwitch) {
		if( pGame->pOnlineWorld )
			pGame->pOnlineWorld->UpdateScreen(lpScreenBuffer);
		else
		{
			//여기는 맵 에디터 모드일때 들어온다
			GetClientRect( pGame->Hwnd, &clGrp.m_rcWindow );
			ClientToScreen( pGame->Hwnd, (POINT*)&clGrp.m_rcWindow );
			ClientToScreen( pGame->Hwnd, (POINT*)&clGrp.m_rcWindow+1 );

			HDC destHdc;
			HDC hdc;
			
			if (lpScreenBuffer->GetDC(&hdc) == DD_OK) {
				clGrp.lpDDSConvertSurface->GetDC(&destHdc);
				BitBlt(destHdc, 0, 0, clGrp.GetScreenXsize(), clGrp.GetScreenYsize(), hdc, 0, 0, SRCCOPY);
				//GdiFlush();
				lpScreenBuffer->ReleaseDC(hdc);
				clGrp.lpDDSConvertSurface->ReleaseDC(destHdc);
				
				clGrp.m_rcWindow.bottom = clGrp.m_rcWindow.top + 600;
				clGrp.m_rcWindow.right = clGrp.m_rcWindow.left + 800;
				
				clGrp.lpDDSPrimary->Blt( &clGrp.m_rcWindow, clGrp.lpDDSConvertSurface,
					NULL, DDBLT_WAIT , NULL );
			}
		}
	}
	else {
		if( clGrp.lpClipper )
			clGrp.lpDDSPrimary->Blt(NULL, lpScreenBuffer, &rect, DDBLT_WAIT, NULL);
		else
			clGrp.BltFast(SURFACE_PRIMARY, 0, 0, lpScreenBuffer, &rect, DDBLTFAST_WAIT);
	}
	
	if(IpD.LeftPressSwitch)		GbPush=TRUE;	
	else					GbPush=FALSE;	

	
	switch(g_CurrentMenu)
	{
	case GS_GAME:			
		return GS_GAME;
	case GS_INITINITMENU:	
		if(GameStatus == GS_GAMEMENU)
		{
			GD.InitInitMenuCount = 20;
			MakeOrder(UserPlayerNumber, ORDER_QUIT, 0, ZeroID, TRUE);

			// 시간이 지나지 않았는데 퇴각하는 것이면 경험치를 깎는다.
			if(GD.PlayTime_CurrentTime < RETREAT_PERMIT_CLOCK)
			{
				SendBattleMessage_LoseForRetreat();
			}


			return GS_GAME;
		}
		else
		{
			return GS_INITINITMENU;
		}
		
	case GS_INITRESTART:	
		return GS_INITRESTART;		
	case GS_INITQUIT:	
		GD.InitQuitCount = 20;
		MakeOrder(UserPlayerNumber, ORDER_QUIT, 0, ZeroID, TRUE);
		return GS_GAME;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// VOID	SetGotoPreviousMenu()
/// 목적 : 이전 메뉴로 돌아가라고 명령한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	SetGotoPreviousMenu()
{
	 g_IsGotoPreviousMenu	=	TRUE;	 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BOOL	IsGotoPreviousMenu()
/// 목적 : 이전 메뉴로 돌아갈 것인지 검사한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	IsGotoPreviousMenu()
{	
	if(g_IsGotoPreviousMenu == TRUE)
	{
		g_IsGotoPreviousMenu  = FALSE;
		return TRUE;
	}
	else
		return FALSE;	
}

//---------------------------------------------------------------------------------------------
//	설명	:	메뉴를 나가도록 명령한다.	
//---------------------------------------------------------------------------------------------
VOID	SetExitMenu()
{	
	GbIsExitMenu	=	TRUE;
}

//---------------------------------------------------------------------------------------------
//	설명	:	메뉴를 나갈 건지의 여부를 알아온다.
//	리턴	:	TRUE 메뉴를 나간다. FALSE 메뉴를 나가지 않는다.
//---------------------------------------------------------------------------------------------
BOOL	IsExitMenu()
{	

	if(GbIsExitMenu)
	{		
		GbIsExitMenu	=	FALSE;
		return TRUE;
	}
	else
		return FALSE;		
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// VOID	InitMainMenu()
/// 함수 : 게임 메뉴의 메인 초기화. 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	InitMainMenu()
{	
	CHAR	szFileNameBuffer[1024];	
	//버튼을 로드 한다.(버튼의 배경)
	GetFileNamePath(Y_TITLE_FILE_GAMEMENUBORDER, SavePath, szFileNameBuffer);
	if(TitleSpr.Image) clGrp.Error("FKJE8567", "GAMEMENU000");
	if(clGrp.LoadXspr(szFileNameBuffer, TitleSpr)==FALSE) clGrp.Error("FKJE8567", "GAMEMENU001 [%s]", szFileNameBuffer);		

#ifdef _IMJIN2PLUS
	siX_Pos = clGrp.GetScreenXsize()/2 - (TitleSpr.Header.Xsize)/2; 
	siY_Pos = clGrp.GetScreenYsize()/2 - (TitleSpr.Header.Ysize)/2; 
#endif


	BContinue.Create( siX_Pos+30, siY_Pos+ 19-45, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Xsize, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Ysize, Text.Continue.Get(),		BUTTON_PUT_LEFT);		
	BObjective.Create(siX_Pos+30, siY_Pos+ 54-45, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Xsize, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Ysize, Text.Objectives.Get(),	BUTTON_PUT_LEFT);		
	BOption.Create(	  siX_Pos+30, siY_Pos+ 89-45, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Xsize, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Ysize, Text.Optoin.Get(),		BUTTON_PUT_LEFT);		
	BSave.Create(	  siX_Pos+30, siY_Pos+124-45, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Xsize, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Ysize, Text.Save.Get(),			BUTTON_PUT_LEFT);		
	BLoad.Create(	  siX_Pos+30, siY_Pos+159-45, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Xsize, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Ysize, Text.Load.Get(),			BUTTON_PUT_LEFT);		
	BRestart.Create(  siX_Pos+30, siY_Pos+194-45, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Xsize, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Ysize, Text.Replay.Get(),		BUTTON_PUT_LEFT);		
	BInitMenu.Create( siX_Pos+30, siY_Pos+229-45, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Xsize, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Ysize, Text.InitMenu.Get(),		BUTTON_PUT_LEFT);		
	BExit.Create(	  siX_Pos+30, siY_Pos+264-45, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Xsize, g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS].Header.Ysize, Text.ButtonExit.Get(),	BUTTON_PUT_LEFT);		

	BContinue.SetAction(TRUE);	
	BOption.SetAction(TRUE);

	BInitMenu.SetAction(TRUE);


	// 지도 편집기 모드에서는 종료 버튼을 활성화시킨다. 
	if(MapEditMode == TRUE)
	{
		BExit.SetAction(TRUE);	
	}
	else
	{
		BExit.SetAction(FALSE);	
	}

	BObjective.SetAction(FALSE);
	BSave.SetAction(FALSE);
	BLoad.SetAction(FALSE);
	BRestart.SetAction(FALSE);

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SHORT	MainMenu(BOOL bPush)
/// 함수 : 게임 메뉴의 메인 프로시저
////////////////////////////////////////////////////////////////////////////////////////////////////////////
SHORT	MainMenu(BOOL bPush)
{
	SHORT	menuindex	=	0;		
	int  	ClipLeft, ClipTop, ClipRight, ClipBottom;
	static	SHORT	nReturnMenu;
	SHORT	nOkCancel;
	HDC		hdc;
	HFONT	OldFont;

	

	if(YOkCancel.IsAction() == TRUE)
	{
		// 메뉴를 보는 중에 게임이 종료되었으면 강제로 다이알로그를 없앤다.
		if(IsExitMenu() == TRUE)
		{			
			YOkCancel.Free();
			menuindex	=	GS_GAME;
		}
		else
		{
			if(YOkCancel.Action(bPush, &nOkCancel) == TRUE)
			{			
				YOkCancel.Free();

				if(nOkCancel == Y_OKCANCEL_OK)
					menuindex	=	nReturnMenu;
			}
		}

		if(YOkCancel.IsAction() == TRUE)
			YOkCancel.Draw(lpScreenBuffer);
	}
	else
	{
		if(IsGotoPreviousMenu() == TRUE)			menuindex	=	GS_GAME;
		if(IsExitMenu() == TRUE)					menuindex	=	GS_GAME;
		if(BContinue.Process(bPush) == TRUE)		
		{
			menuindex	=	GS_GAME;	
			GD.m_bBlackCheck = TRUE;
		}
		if(BObjective.Process(bPush) == TRUE)		menuindex	=	Y_GAME_MENU_INITOBJECTIVE;	
		if(BOption.Process(bPush) == TRUE)			menuindex	=	Y_GAME_MENU_INITOPTION;	

		if(BInitMenu.Process(bPush) == TRUE)
		{
			YOkCancel.Create(Text.ReturnMenuMessage.Get());
			nReturnMenu	=	GS_INITINITMENU;	
		}

		if(BLoad.Process(bPush) == TRUE)			menuindex	=	Y_GAME_MENU_INITLOAD;	
		if(BSave.Process(bPush) == TRUE)			menuindex	=	Y_GAME_MENU_INITSAVE;		

		if(BRestart.Process(bPush) == TRUE)
		{
			YOkCancel.Create(Text.RestartMessage.Get());			
			nReturnMenu	=	GS_INITRESTART;			
		}

		if(BExit.Process(bPush) == TRUE)
		{
			YOkCancel.Create(Text.QuitMessage.Get());			
			nReturnMenu	=	GS_INITQUIT;		
		}

		if(clGrp.LockSurface(lpScreenBuffer) == TRUE)
		{
			//클리핑 영역을 바꾼다.
			clGrp.GetClipArea(ClipLeft, ClipTop, ClipRight, ClipBottom);
			clGrp.SetClipArea(0, 0, clGrp.GetScreenXsize()-1, clGrp.GetScreenYsize()-1);		

#ifdef	_IMJIN2PLUS
			// 임진록 2에서 게임 도중의 메뉴에 대하여...
			clGrp.PutImage32(siX_Pos, siY_Pos-45, TitleSpr.Header.Xsize, TitleSpr.Header.Ysize, 
				&TitleSpr.Image[TitleSpr.Header.Start[0]]);			
#else
			clGrp.PutImage32(234, 56, TitleSpr.Header.Xsize, TitleSpr.Header.Ysize, 
						&TitleSpr.Image[TitleSpr.Header.Start[0]]);			
#endif

			//중국테스트.
			BContinue.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS],		30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);
			BObjective.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS], 	30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);			
			BOption.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS],		30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);
			BInitMenu.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS],		30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);			
			BSave.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS], 		30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);
			BLoad.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS], 		30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);		
			BRestart.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS], 		30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);
			BExit.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS], 		30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);

			clGrp.UnlockSurface(lpScreenBuffer);

			//클리핑 영역을 다시 복구한다.
			clGrp.SetClipArea(ClipLeft, ClipTop, ClipRight, ClipBottom);	
		}				

		if(lpScreenBuffer->GetDC(&hdc) == DD_OK)
		{
			SetBkMode(hdc, TRANSPARENT);					
			OldFont	=	(HFONT) SelectObject(hdc, pGame->pBattle->BriefingFont);
				
			BContinue.Put(hdc, BUTTON_PUT_NOMOVE);
			BObjective.Put(hdc, BUTTON_PUT_NOMOVE);
			BOption.Put(hdc, BUTTON_PUT_NOMOVE);
			BInitMenu.Put(hdc, BUTTON_PUT_NOMOVE);
			BSave.Put(hdc, BUTTON_PUT_NOMOVE);
			BLoad.Put(hdc, BUTTON_PUT_NOMOVE);
			BRestart.Put(hdc, BUTTON_PUT_NOMOVE);
			BExit.Put(hdc, BUTTON_PUT_NOMOVE);

			SelectObject(hdc, OldFont);
			lpScreenBuffer->ReleaseDC(hdc);
		}
	}

	return menuindex;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// VOID	FreeMainMenu()
/// 함수 : MainMenu에서 할당했던 메모리를 해제한다.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	FreeMainMenu()
{

	if(TitleSpr.Image)			clGrp.FreeXspr(TitleSpr);			
}

#endif