#include <GSL.h>

#include <main.h>
#include <Mouse.h>
#include <etc.h>
#include <SoundManager.h>


#include <debugtool.h>
#include "text.h"

#include <Button.h>
#include <OnlineWorld.h>
#include <clgame.h>
#include <message.h>

#ifdef _OGG_PLAY
#include "BgmMgr_PreHeader.h"
#endif

#define		Y_WINLOSE_MAX_FRAME		12							// �¸� �й� �ִϸ��̼� ������ ��. 
#define		Y_DEALY_FOR_NEXTFRAME	50

#define		Y_SPR_FILE_NUM			1

#define		Y_DELAY_FOR_WINLOSE		1000						// WinLoseȭ���� ���� �ּ� �ð� ���� (Ŭ������)

extern	char 				SavePath[];
extern	_Text 				Text;								// �ؽ�Ʈ 
extern	int					bActive;							// ���� ��Ƽ������ �ƴ��� �˷��ִ� ����  
extern _InputDevice				IpD;

extern	SoundManager	   *g_pSoundManager;

extern int WindowModeSwitch;
extern _clGame *pGame;
extern LPDIRECTDRAWSURFACE7   lpScreenBuffer;


static	XSPR	WinLoseSpr[Y_SPR_FILE_NUM];
static	UI16	WinLoseSoundNum;
static	DWORD	TimeForFinished;
static	DWORD	TimeForNextFrame;
static	SHORT	CurrentFrame;
static	_Box	BoxWinLose;
static	SI32	siDrawX, siDrawY;

VOID	InitWinLose(BOOL IsWin);
BOOL	WinLose(BOOL IsWin);
VOID	FreeWinLose();

static	SHORT	WinLoseFrame[Y_WINLOSE_MAX_FRAME * 2] =			//ȭ�� �ѹ�, ��Ʈ �ѹ�
{
	0, 0,	0, 1,
	0, 2,	0, 3,
	0, 4,	0, 5,
	0, 6,	0, 7,
	0, 8,	0, 9,

	0, 10,	0, 11
};

SHORT InitWin()
{		
	InitWinLose(TRUE);	
	return TRUE;
}

SHORT Win()
{
	if(WinLose(TRUE) == TRUE)
	{
		return GS_ONLINEWORLD;
	}
	else
		return 0;
}

SHORT InitLose()
{	
	InitWinLose(FALSE);
	return TRUE;
}


SHORT Lose()
{
	if(WinLose(FALSE) == TRUE)
	{

		return GS_ONLINEWORLD;
	}
	else
		return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� :
/// ���� :
////////////////////////////////////////////////////////////////////////////////////////////////
VOID	InitWinLose(BOOL IsWin)
{
	CHAR	szFileNameBuffer[1024];
//	char	buffer[128];

	CurrentFrame		=	0;
	TimeForFinished		=	TimeForNextFrame	=	timeGetTime();			

	//ShowMessage(10,70,clGrp.lpDDSPrimary);

	if(IsWin == TRUE)	
	{	
		//wsprintf(buffer, "�������� �¸��Ͽ����ϴ�.");
		//SetMessage(buffer);

		GetFileNamePath("yfnt\\winlogo_Tai.spr", SavePath, szFileNameBuffer);		
		if(clGrp.LoadXspr(szFileNameBuffer, WinLoseSpr[0]) == FALSE)
			clGrp.Error("FKJE8567", "WINLOSE");		

#ifdef _OGG_PLAY
		GetBgmMgr()->Play(OGGBGM_FILENAME_WIN, FALSE);
#else
		GetFileNamePath("music\\win.YAV", SavePath, szFileNameBuffer);		

		if(g_pSoundManager)
		{
			WinLoseSoundNum = g_pSoundManager->AddPlayID(szFileNameBuffer);
		}
#endif
	}
	else
	{
		//wsprintf(buffer, "�������� �й��Ͽ����ϴ�.");
		//SetMessage(buffer);

		GetFileNamePath("yfnt\\loselogo_Tai.spr", SavePath, szFileNameBuffer);		
		if(clGrp.LoadXspr(szFileNameBuffer, WinLoseSpr[0]) == FALSE)
			clGrp.Error("FKJE8567", "WINLOSE");		

#ifdef _OGG_PLAY
		GetBgmMgr()->Play(OGGBGM_FILENAME_LOSE, FALSE);
#else
		GetFileNamePath("music\\lose.YAV", SavePath, szFileNameBuffer);
		
		if(g_pSoundManager)
			WinLoseSoundNum = g_pSoundManager->AddPlayID(szFileNameBuffer);
#endif
	}

#ifndef _OGG_PLAY
	if(g_pSoundManager)
		g_pSoundManager->PlaySound(WinLoseSoundNum);
#endif


	siDrawX = (clGrp.GetScreenXsize() / 2) - (WinLoseSpr[0].Header.Xsize/2);
	siDrawY = (clGrp.GetScreenYsize() / 2) - (WinLoseSpr[0].Header.Ysize/2);

	if(IsWin == TRUE)	
		BoxWinLose.Create(siDrawX, siDrawY, WinLoseSpr[0].Header.Xsize, WinLoseSpr[0].Header.Ysize, Text.WinGame.Get(), BOX_PUT_LEFT);
	else
		BoxWinLose.Create(siDrawX, siDrawY, WinLoseSpr[0].Header.Xsize, WinLoseSpr[0].Header.Ysize, Text.LoseGame.Get(), BOX_PUT_LEFT);

	if (!WindowModeSwitch) {
		if( clGrp.lpClipper )
			clGrp.lpDDSBack->Blt(NULL, clGrp.lpDDSPrimary, NULL, DDBLT_WAIT, NULL);
		else					
			clGrp.BltFast(SURFACE_BACK, 0, 0, clGrp.lpDDSPrimary, NULL, DDBLTFAST_WAIT);
	}
	else
	{
		pGame->pOnlineWorld->UpdateScreen(clGrp.lpDDSBack);

	}
}
	
////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� :
/// ���� :
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	WinLose(BOOL IsWin)
{
	SHORT		NextMenu = 0;	
	HDC			hdc = 0;

	_Box		BoxWin, BoxLose;
	
	if(!bActive)
		return FALSE;
	// �޽����� ������ ǥ���Ѵ�. 
//	ShowMessage(10, 70, clGrp.lpDDSBack);
	
	if(clGrp.LockSurface(SURFACE_BACK) == TRUE)
	{
		if(((timeGetTime() - TimeForNextFrame) > Y_DEALY_FOR_NEXTFRAME) && (CurrentFrame < (Y_WINLOSE_MAX_FRAME - 1)))		
		{
			CurrentFrame++;		
			TimeForNextFrame	=	timeGetTime();
		
		}		
		clGrp.PutSpriteT(siDrawX, siDrawY, WinLoseSpr[0].Header.Xsize, WinLoseSpr[0].Header.Ysize,
					&WinLoseSpr[WinLoseFrame[CurrentFrame * 2]].Image[WinLoseSpr[WinLoseFrame[CurrentFrame * 2]].Header.Start[WinLoseFrame[CurrentFrame * 2 + 1]]]);		
	
		clGrp.UnlockSurface(SURFACE_BACK);
	}

#ifdef _ENGLISH  // ���������̸�
	// "�̰���ϴ�, �����ϴ�"�� ��´�.(�ڽ��� ����)
	if(clGrp.lpDDSBack->GetDC(&hdc) == DD_OK)
	{
		SetBkMode(hdc, TRANSPARENT);
		SelectObject(hdc, WinLoseFont);
		SetTextColor(hdc, RGB(240, 240, 240));		

		BoxWinLose.Put(hdc);		
		clGrp.lpDDSBack->ReleaseDC(hdc);
	}
#endif

	if (WindowModeSwitch) {

		pGame->pOnlineWorld->UpdateScreen(clGrp.lpDDSBack);

	}
	else {
		
		if( clGrp.lpClipper )
			clGrp.lpDDSPrimary->Blt(NULL, clGrp.lpDDSBack, NULL, DDBLT_WAIT, NULL);
		else
			clGrp.BltFast(SURFACE_PRIMARY, 0, 0, clGrp.lpDDSBack, NULL, DDBLTFAST_WAIT);
	}
	//ShowMessage(10,70,clGrp.lpDDSBack);
	

	//ȭ���� ������ 2�ʰ� ������ �� �ķ� �ʱ� ȭ������ �� �� �ִ�.
	if(((timeGetTime() - TimeForFinished) > Y_DELAY_FOR_WINLOSE))
	{			
		if((IpD.LeftPressSwitch) || (GetAsyncKeyState(VK_ESCAPE) < 0) || (GetAsyncKeyState(VK_RETURN) < 0) ||
			(GetAsyncKeyState(VK_SPACE) < 0))
		{			
			FreeWinLose();
			return TRUE;			
		}

		// 8�ʰ� ����ϸ� �ڵ������� ���ư����� �����. 
		if((timeGetTime() - TimeForFinished) > 8000)
		{
			FreeWinLose();
			return TRUE;			
		}
	}		

	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� :
/// ���� :
////////////////////////////////////////////////////////////////////////////////////////////////
VOID FreeWinLose()
{	
	SHORT	i;
			
	for(i=0; i <  Y_SPR_FILE_NUM; i++)	
		if(WinLoseSpr[i].Image) clGrp.FreeXspr(WinLoseSpr[i]);				

	if(WinLoseSoundNum)
	{
		if(g_pSoundManager)
		{
			if( g_pSoundManager->IsPlaying(WinLoseSoundNum) == TRUE )		
				g_pSoundManager->StopSound(WinLoseSoundNum);			
			
			g_pSoundManager->ReleaseSound(WinLoseSoundNum);
		}
		WinLoseSoundNum = 0;
	}
}