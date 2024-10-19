#ifndef _ROCKET_NEW_TILE_MAP
////////////////////////////���⼭ ���ʹ� ���� �޴��� ���� �׸�////////////////////////

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
extern	int						bActive;							// ���� ��Ƽ������ �ƴ��� �˷��ִ� ����  
extern _InputDevice				IpD;
extern	char 					SavePath[];
extern	XSPR 					TitleSpr;							// Ÿ��Ʋ ��� �׸��� ���� ���� 
extern	_Text 					Text;								// �ؽ�Ʈ 
extern	_MainData				GD;									// ������ �ֵ����� 
extern	SI32					UserPlayerNumber;					// ����� ���� 
extern _NewID					ZeroID;
extern	SHORT				GameStatus;							// ������ ���� ��Ȳ�� �˸��� ���� 
extern  SHORT					MapEditMode;

extern	LPDIRECTDRAWSURFACE7	lpScreenBuffer;
extern	LPDIRECTDRAWSURFACE7	lpBriefingScrollBuffer;

extern  int                     WindowModeSwitch;

static	_Button					BInitMenu,							// �ʱ�޴�
								BOption,							// ȯ�漳��
								BObjective,							// ���� ����
								BSave,								// �����ϱ�
								BLoad,								// �ҷ�����
								BContinue,							// ����ϱ�
								BExit,								// �����ϱ�
								BRestart;							// �ٽ��ϱ�

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
/// �Լ� : 
/// ���� : 
//----------------------------------------------------------------------------------------------------
VOID InitGameMenu()
{		
	GbIsExitMenu			=	FALSE;
	g_IsGotoPreviousMenu	=	FALSE;
	g_CurrentMenu			=	Y_GAME_MENU_INITMAIN;			
	GbPush					=	FALSE;
	
	// ���� �޴��� ���������� ȭ���� lpBriefingBuffer�ȿ� �ִ´�.
	RECT rect;
	rect.left = 0;
	rect.right = clGrp.GetScreenXsize();
	rect.top   = 0;
	rect.bottom = clGrp.GetScreenYsize();

	lpBriefingScrollBuffer->BltFast(0, 0, lpScreenBuffer, &rect, DDBLTFAST_WAIT);
}


//----------------------------------------------------------------------------------------------------
/// �Լ� : 
/// ���� : 
//----------------------------------------------------------------------------------------------------
VOID	InitGameLoadMenu()
{		
}

//----------------------------------------------------------------------------------------------------
/// �Լ� : 
/// ���� : 
//----------------------------------------------------------------------------------------------------
SHORT GameMenu()
{
	SHORT	nNextMenu;	

	if(!bActive)
		return FALSE;

	// ����� �׸���.	
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

	// ���콺�� �׷��ְ� ȭ���� �����Ѵ�. 
	DrawMouse(lpScreenBuffer);
	
	// �����̸Ӹ� ���ǽ��� �������ش�.	
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

			// �ð��� ������ �ʾҴµ� ���ϴ� ���̸� ����ġ�� ��´�.
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
/// ���� : ���� �޴��� ���ư���� ����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	SetGotoPreviousMenu()
{
	 g_IsGotoPreviousMenu	=	TRUE;	 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BOOL	IsGotoPreviousMenu()
/// ���� : ���� �޴��� ���ư� ������ �˻��Ѵ�.
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
//	����	:	�޴��� �������� ����Ѵ�.	
//---------------------------------------------------------------------------------------------
VOID	SetExitMenu()
{	
	GbIsExitMenu	=	TRUE;
}

//---------------------------------------------------------------------------------------------
//	����	:	�޴��� ���� ������ ���θ� �˾ƿ´�.
//	����	:	TRUE �޴��� ������. FALSE �޴��� ������ �ʴ´�.
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
/// �Լ� : ���� �޴��� ���� �ʱ�ȭ. 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	InitMainMenu()
{	
	CHAR	szFileNameBuffer[1024];	
	//��ư�� �ε� �Ѵ�.(��ư�� ���)
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


	// ���� ������ ��忡���� ���� ��ư�� Ȱ��ȭ��Ų��. 
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
/// �Լ� : ���� �޴��� ���� ���ν���
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
		// �޴��� ���� �߿� ������ ����Ǿ����� ������ ���̾˷α׸� ���ش�.
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
			//Ŭ���� ������ �ٲ۴�.
			clGrp.GetClipArea(ClipLeft, ClipTop, ClipRight, ClipBottom);
			clGrp.SetClipArea(0, 0, clGrp.GetScreenXsize()-1, clGrp.GetScreenYsize()-1);		

#ifdef	_IMJIN2PLUS
			// ������ 2���� ���� ������ �޴��� ���Ͽ�...
			clGrp.PutImage32(siX_Pos, siY_Pos-45, TitleSpr.Header.Xsize, TitleSpr.Header.Ysize, 
				&TitleSpr.Image[TitleSpr.Header.Start[0]]);			
#else
			clGrp.PutImage32(234, 56, TitleSpr.Header.Xsize, TitleSpr.Header.Ysize, 
						&TitleSpr.Image[TitleSpr.Header.Start[0]]);			
#endif

			//�߱��׽�Ʈ.
			BContinue.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS],		30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);
			BObjective.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS], 	30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);			
			BOption.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS],		30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);
			BInitMenu.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS],		30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);			
			BSave.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS], 		30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);
			BLoad.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS], 		30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);		
			BRestart.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS], 		30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);
			BExit.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS], 		30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);

			clGrp.UnlockSurface(lpScreenBuffer);

			//Ŭ���� ������ �ٽ� �����Ѵ�.
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
/// �Լ� : MainMenu���� �Ҵ��ߴ� �޸𸮸� �����Ѵ�.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	FreeMainMenu()
{

	if(TitleSpr.Image)			clGrp.FreeXspr(TitleSpr);			
}



#else

////////////////////////////���⼭ ���ʹ� ���� �޴��� ���� �׸�////////////////////////

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
extern	int						bActive;							// ���� ��Ƽ������ �ƴ��� �˷��ִ� ����  
extern _InputDevice				IpD;
extern	char 					SavePath[];
extern	XSPR 					TitleSpr;							// Ÿ��Ʋ ��� �׸��� ���� ���� 
extern	_Text 					Text;								// �ؽ�Ʈ 
extern	_MainData				GD;									// ������ �ֵ����� 
extern	SI32					UserPlayerNumber;					// ����� ���� 
extern _NewID					ZeroID;
extern	SHORT				GameStatus;							// ������ ���� ��Ȳ�� �˸��� ���� 
extern  SHORT					MapEditMode;

extern	LPDIRECTDRAWSURFACE7	lpScreenBuffer;
extern	LPDIRECTDRAWSURFACE7	lpBriefingScrollBuffer;

extern  int                     WindowModeSwitch;

static	_Button					BInitMenu,							// �ʱ�޴�
								BOption,							// ȯ�漳��
								BObjective,							// ���� ����
								BSave,								// �����ϱ�
								BLoad,								// �ҷ�����
								BContinue,							// ����ϱ�
								BExit,								// �����ϱ�
								BRestart;							// �ٽ��ϱ�

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
/// �Լ� : 
/// ���� : 
//----------------------------------------------------------------------------------------------------
VOID InitGameMenu()
{		
	GbIsExitMenu			=	FALSE;
	g_IsGotoPreviousMenu	=	FALSE;
	g_CurrentMenu			=	Y_GAME_MENU_INITMAIN;			
	GbPush					=	FALSE;
	
	// ���� �޴��� ���������� ȭ���� lpBriefingBuffer�ȿ� �ִ´�.
	RECT rect;
	rect.left = 0;
	rect.right = clGrp.GetScreenXsize();
	rect.top   = 0;
	rect.bottom = clGrp.GetScreenYsize();

	lpBriefingScrollBuffer->BltFast(0, 0, lpScreenBuffer, &rect, DDBLTFAST_WAIT);
}


//----------------------------------------------------------------------------------------------------
/// �Լ� : 
/// ���� : 
//----------------------------------------------------------------------------------------------------
VOID	InitGameLoadMenu()
{		
}

//----------------------------------------------------------------------------------------------------
/// �Լ� : 
/// ���� : 
//----------------------------------------------------------------------------------------------------
SHORT GameMenu()
{
	SHORT	nNextMenu;	

	if(!bActive)
		return FALSE;

	// ����� �׸���.	
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

	// ���콺�� �׷��ְ� ȭ���� �����Ѵ�. 
	DrawMouse(lpScreenBuffer);
	
	// �����̸Ӹ� ���ǽ��� �������ش�.	
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
			//����� �� ������ ����϶� ���´�
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

			// �ð��� ������ �ʾҴµ� ���ϴ� ���̸� ����ġ�� ��´�.
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
/// ���� : ���� �޴��� ���ư���� ����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	SetGotoPreviousMenu()
{
	 g_IsGotoPreviousMenu	=	TRUE;	 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BOOL	IsGotoPreviousMenu()
/// ���� : ���� �޴��� ���ư� ������ �˻��Ѵ�.
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
//	����	:	�޴��� �������� ����Ѵ�.	
//---------------------------------------------------------------------------------------------
VOID	SetExitMenu()
{	
	GbIsExitMenu	=	TRUE;
}

//---------------------------------------------------------------------------------------------
//	����	:	�޴��� ���� ������ ���θ� �˾ƿ´�.
//	����	:	TRUE �޴��� ������. FALSE �޴��� ������ �ʴ´�.
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
/// �Լ� : ���� �޴��� ���� �ʱ�ȭ. 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	InitMainMenu()
{	
	CHAR	szFileNameBuffer[1024];	
	//��ư�� �ε� �Ѵ�.(��ư�� ���)
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


	// ���� ������ ��忡���� ���� ��ư�� Ȱ��ȭ��Ų��. 
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
/// �Լ� : ���� �޴��� ���� ���ν���
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
		// �޴��� ���� �߿� ������ ����Ǿ����� ������ ���̾˷α׸� ���ش�.
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
			//Ŭ���� ������ �ٲ۴�.
			clGrp.GetClipArea(ClipLeft, ClipTop, ClipRight, ClipBottom);
			clGrp.SetClipArea(0, 0, clGrp.GetScreenXsize()-1, clGrp.GetScreenYsize()-1);		

#ifdef	_IMJIN2PLUS
			// ������ 2���� ���� ������ �޴��� ���Ͽ�...
			clGrp.PutImage32(siX_Pos, siY_Pos-45, TitleSpr.Header.Xsize, TitleSpr.Header.Ysize, 
				&TitleSpr.Image[TitleSpr.Header.Start[0]]);			
#else
			clGrp.PutImage32(234, 56, TitleSpr.Header.Xsize, TitleSpr.Header.Ysize, 
						&TitleSpr.Image[TitleSpr.Header.Start[0]]);			
#endif

			//�߱��׽�Ʈ.
			BContinue.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS],		30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);
			BObjective.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS], 	30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);			
			BOption.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS],		30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);
			BInitMenu.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS],		30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);			
			BSave.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS], 		30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);
			BLoad.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS], 		30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);		
			BRestart.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS], 		30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);
			BExit.Put(&g_Buttons_Spr[BUTTONS_GAMEMENU_BUTTONS], 		30, 31, 32,		BUTTON_PUT_NOMOVE | BUTTON_PUT_NOBORDER);

			clGrp.UnlockSurface(lpScreenBuffer);

			//Ŭ���� ������ �ٽ� �����Ѵ�.
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
/// �Լ� : MainMenu���� �Ҵ��ߴ� �޸𸮸� �����Ѵ�.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID	FreeMainMenu()
{

	if(TitleSpr.Image)			clGrp.FreeXspr(TitleSpr);			
}

#endif