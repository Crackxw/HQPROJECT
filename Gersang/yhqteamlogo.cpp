#include <main.h>

#ifdef _IMJIN2
	#include <etc.h>
#endif

//#include "ympegover.h"
#include "yhqteamlogo.h"

extern	char 				SavePath[];

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID	InitHQLogo()
/// 설명 : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
VOID	InitHQLogo()
{
	CHAR	szFileNameBuffer[1024];

	if(MpegPlayer.Init() == TRUE)
	{
		// cd 에서 읽는다.
		GetFileNamePath(HQLOGO_FILENAME, SavePath, szFileNameBuffer);
		if(MpegPlayer.Load(szFileNameBuffer) == TRUE)	
		{
			MpegPlayer.Play(FALSE, 0, 0, clGrp.GetScreenXsize(), clGrp.GetScreenYsize());					
		}
		// 실패할 경우 하드에서 읽는다.
		else
		{
			if(MpegPlayer.Init() == TRUE)
			{
				GetFileNamePath(HQLOGO_FILENAME, SavePath, szFileNameBuffer);
				if(MpegPlayer.Load(szFileNameBuffer) == TRUE)	
				{
					MpegPlayer.Play(FALSE, 0, 0, clGrp.GetScreenXsize(), clGrp.GetScreenYsize());					
				}		
			}
		}
	}	
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : SHORT	HQLogo()
/// 리턴값 : 0 - 계속 진행중이다.
///          1 - 연주가 종료되었다.
///          2 - 사용자가 취소했다.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
SHORT 	HQLogo()
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
				FreeHQLogo();
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
VOID	FreeHQLogo()
{
	MpegPlayer.Free();
}
*/
