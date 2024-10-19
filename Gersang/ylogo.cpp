#include <main.h>

#ifdef _IMJIN2
	#include <etc.h>
#endif

//#include <ympegover.h>
#include "ylogo.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID	InitLogo()
/// 설명 : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
VOID	InitLogo()
{
	CHAR	szFileNameBuffer[1024];
	if(MpegPlayer.Init() == TRUE)
	{
		GetFileNamePath(LOGO_FILENAME, SavePath, szFileNameBuffer);
		if(MpegPlayer.Load(szFileNameBuffer) == TRUE)	
		{
			MpegPlayer.Play(FALSE, 592, 384, clGrp.GetScreenXsize(), clGrp.GetScreenYsize());
		}
	}	
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : SHORT	Logo()
/// 설명 : 계속 작업이 필요하다면 0을 리턴, 더이상의 작업이 필요 없으면 0이 아닌값을 리턴.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
SHORT 	Logo()
{	
	BOOL	IsPlay;		
	SHORT	nReturn =	0;

	if(MpegPlayer.IsInitialize() == TRUE)
	{
		if(MpegPlayer.IsPlay() == TRUE)	
		{
			//MpegPlayer.Action()은 사용자가 키보드를 눌러서 
			//연주하는것을 취소했다면 1을 리턴 한다.
			nReturn = MpegPlayer.Action();	

			//Mpeg.Action() 함수에서 종료되었다면(모두 연주됐거나 사용자가 취소했을때)
			//FreeHQLogo()를 호출해 준다.
			if(( IsPlay = MpegPlayer.IsPlay()) == FALSE)	
				FreeYLogo();
		}
	}
	else
	{
		nReturn =	0;
		IsPlay	=	FALSE;
	}


	if(nReturn == 1) return 2;
	else if((nReturn == 0) && (IsPlay == FALSE)) return 1;
	else return 0;
}
*/
/*
VOID	FreeYLogo()
{
	MpegPlayer.Free();
}
*/