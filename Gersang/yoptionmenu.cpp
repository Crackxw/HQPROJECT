#include <GSL.h>
#include <main.h>
#include <clGame.h>

#include <Game2.h>

#include "yoptionmenu.h"
#include "yoption.h"
#include "gamemenu.h"


extern	_clGame					*pGame;
extern	LPDIRECTDRAWSURFACE7	lpScreenBuffer;
static	_YOption				YOption;			

SI16							OnlineWorldFieldArea = -1;


VOID InitOptionMenu()
{	
	YOption.Init();	
}

SHORT OptionMenu()
{
	SHORT	menuindex;
	int		ClipLeft, ClipTop, ClipRight, ClipBottom;
	HFONT	OldFont;
	HDC		hdc;

	menuindex = YOption.Action();

	if(clGrp.LockSurface(lpScreenBuffer) == TRUE)
	{
		//클리핑 영역을 바꾼다.
		clGrp.GetClipArea(ClipLeft, ClipTop, ClipRight, ClipBottom);
		clGrp.SetClipFullScreen();

		YOption.Put();

		clGrp.UnlockSurface(lpScreenBuffer);
		clGrp.SetClipArea(ClipLeft, ClipTop, ClipRight, ClipBottom);	
	}		
	
	if(lpScreenBuffer->GetDC(&hdc) == DD_OK)
	{
		SetBkMode(hdc, TRANSPARENT);		
		OldFont = (HFONT)SelectObject(hdc, pGame->pBattle->BriefingFont);

		YOption.Put(hdc);

		SelectObject(hdc, OldFont);
		lpScreenBuffer->ReleaseDC(hdc);
	}

	if(menuindex == -1)					return Y_GAME_MENU_INITMAIN;	
	else if(IsExitMenu() == TRUE)		return GS_GAME;							
	else								return 0;	
}

VOID FreeOptionMenu()
{
	YOption.Free();
}

SHORT OptionActionMenuFromOnline()
{
	SHORT	menuindex;

	menuindex = YOption.Action();

	if(menuindex == -1) return 0;

	return 1;
}

SHORT OptionDrawMenuFromOnline(LPDIRECTDRAWSURFACE7 pSurface)
{
	int		ClipLeft, ClipTop, ClipRight, ClipBottom;
	HFONT	OldFont;
	HDC		hdc;

	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		//클리핑 영역을 바꾼다.
		clGrp.GetClipArea(ClipLeft, ClipTop, ClipRight, ClipBottom);
		clGrp.SetClipFullScreen();

		YOption.Put();

		clGrp.UnlockSurface(pSurface);
		clGrp.SetClipArea(ClipLeft, ClipTop, ClipRight, ClipBottom);	
	}		
	
	if(pSurface->GetDC(&hdc) == DD_OK)
	{
		SetBkMode(hdc, TRANSPARENT);		
		OldFont = (HFONT)SelectObject(hdc, pGame->pBattle->BriefingFont);

		YOption.Put(hdc);

		SelectObject(hdc, OldFont);
		pSurface->ReleaseDC(hdc);
	}

	return 0;
}
