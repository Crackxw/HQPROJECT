#include <main.h>

#ifdef _IMJIN2
	#include <etc.h>
#endif

//#include "ympegover.h"
#include "yhqteamlogo.h"

extern	char 				SavePath[];

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID	InitHQLogo()
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
VOID	InitHQLogo()
{
	CHAR	szFileNameBuffer[1024];

	if(MpegPlayer.Init() == TRUE)
	{
		// cd ���� �д´�.
		GetFileNamePath(HQLOGO_FILENAME, SavePath, szFileNameBuffer);
		if(MpegPlayer.Load(szFileNameBuffer) == TRUE)	
		{
			MpegPlayer.Play(FALSE, 0, 0, clGrp.GetScreenXsize(), clGrp.GetScreenYsize());					
		}
		// ������ ��� �ϵ忡�� �д´�.
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
/// �Լ� : SHORT	HQLogo()
/// ���ϰ� : 0 - ��� �������̴�.
///          1 - ���ְ� ����Ǿ���.
///          2 - ����ڰ� ����ߴ�.
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
			//MpegPlayer.Action()�� ����ڰ� Ű���带 ������ 
			//�����ϴ°��� ����ߴٸ� 1�� ���� �Ѵ�.
			nReturn = MpegPlayer.Action();	

			//Mpeg.Action() �Լ����� ����Ǿ��ٸ�(��� ���ֵưų� ����ڰ� ���������)
			//FreeHQLogo()�� ȣ���� �ش�.
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
