#include <main.h>

#ifdef _IMJIN2
	#include <etc.h>
#endif

//#include "ympegover.h"
#include "ysamsunglogo.h"

extern	char 				SavePath[];


////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID	InitSamsungLogo()
/// ���� : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
VOID	InitSamsungLogo()
{
	
	CHAR	szFileNameBuffer[1024];

	if( MpegPlayer.Init() == TRUE )
	{
		// cd ���� �д´�.
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
/// �Լ� : SHORT	SamsungLogo()
/// ���� : ��� �۾��� �ʿ��ϴٸ� 0�� ����, ���̻��� �۾��� �ʿ� ������ 0�� �ƴѰ��� ����.
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
			//MpegPlayer.Action()�� ����ڰ� Ű���带 ������ 
			//�����ϴ°��� ����ߴٸ� 1�� ���� �Ѵ�.
			nReturn = MpegPlayer.Action();	

			//Mpeg.Action() �Լ����� ����Ǿ��ٸ�(��� ���ֵưų� ����ڰ� ���������)
			//FreeYSamsungLogo()�� ȣ���� �ش�.
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