#include <main.h>

#ifdef _IMJIN2
	#include <etc.h>
#endif

//#include "ympegover.h"
#include "ysamsunglogo.h"

extern	char 				SavePath[];


////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 함수 : VOID	InitSamsungLogo()
/// 설명 : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
VOID	InitSamsungLogo()
{
	
	CHAR	szFileNameBuffer[1024];

	if( MpegPlayer.Init() == TRUE )
	{
		// cd 에서 읽는다.
		GetFileNamePath(SAMSUNGLOGO_FILENAME, SavePath, szFileNameBuffer);
		if( MpegPlayer.Load(szFileNameBuffer) == TRUE )	
		{
			MpegPlayer.Play(FALSE, 0, 0, clGrp.GetScreenXsize(), clGrp.GetScreenYsize());		
		}
		else
		{
			if( MpegPlayer.Init() == TRUE )
			{
				GetFileNamePath(SAMSUNGLOGO_FILENAME, SavePath, szFileNameBuffer);
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
/// 함수 : SHORT	SamsungLogo()
/// 설명 : 계속 작업이 필요하다면 0을 리턴, 더이상의 작업이 필요 없으면 0이 아닌값을 리턴.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
SHORT 	SamsungLogo()
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
			//FreeYSamsungLogo()를 호출해 준다.
			if(( IsPlay = MpegPlayer.IsPlay()) == FALSE)	
				FreeYSamsungLogo();
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
VOID	FreeYSamsungLogo()
{
	MpegPlayer.Free();
}
*/