//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <main.h>
#include <clGame.h>

//#include <ympegover.h>
#include <etc.h>
#include "..\Gersang\gmenu.h"
#include "..\Gersang\opening.h"

extern _clGame* pGame;

/*
BOOL InitOpening()
{
//	CHAR	szFileNameBuffer[1024];
	
	FreeMenuBackgroundMusic();					
	
	// 동영상을 연주하기 전에 현재 프로시져에 포커스를 맞춘다. 
	SetFocus(pGame->Hwnd);

	if(MpegPlayer.Init() == TRUE)
	{
		GetFileNamePath(OPENING_FILENAME, SavePath, szFileNameBuffer);
		
		if(MpegPlayer.Load(szFileNameBuffer) == TRUE)
		{
#ifdef _IMJIN2PLUS
			MpegPlayer.Play(FALSE, 592, 384, clGrp.GetScreenXsize(), clGrp.GetScreenYsize());
#else
			MpegPlayer.Play(FALSE, 640, 360, clGrp.GetScreenXsize(), clGrp.GetScreenYsize());
#endif
			
			
		}
		
	}

	
	return TRUE;
}
*/

/*
SHORT Opening()
{
	SHORT	menuindex = 0;
	
	if(MpegPlayer.IsPlay() == TRUE)
	{
		MpegPlayer.Action();
	}
	else
	{
		FreeOpening();
		menuindex = GS_INITINITMENU;
	}

	return menuindex;
}
*/
/*
VOID FreeOpening()
{
	MpegPlayer.Free();
}
*/