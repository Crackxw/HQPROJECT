#include <main.h>

#ifdef _IMJIN2
	#include <etc.h>
#endif

//#include <ympegover.h>
#include "ylogo.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� : VOID	InitLogo()
/// ���� : 
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
/// �Լ� : SHORT	Logo()
/// ���� : ��� �۾��� �ʿ��ϴٸ� 0�� ����, ���̻��� �۾��� �ʿ� ������ 0�� �ƴѰ��� ����.
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
			//MpegPlayer.Action()�� ����ڰ� Ű���带 ������ 
			//�����ϴ°��� ����ߴٸ� 1�� ���� �Ѵ�.
			nReturn = MpegPlayer.Action();	

			//Mpeg.Action() �Լ����� ����Ǿ��ٸ�(��� ���ֵưų� ����ڰ� ���������)
			//FreeHQLogo()�� ȣ���� �ش�.
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