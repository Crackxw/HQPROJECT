#include <GSL.h>

#include <main.h>
#include "screenpoint.h"

#include <myfile.h>
#include "..\Gersang\DirectX.h"
#include "bindjxFile.h"

extern	_ScreenPoint 		SP;									// 화면에 사용될 여러 정보 


short Compare(char *fileBuffer, char* compareString)	// Append to hojae 2001.10.19
{
	FILE	*fp;

	char	Buffer[1024];
	char	*pToken	= NULL;
	char	delimiter[]  = "\t\n\r";

	int		compareResult;
	int 	imsi_save;
	short	ret_value;

	if( (fp=BindJXFile::GetInstance()->GetFile( fileBuffer )) == NULL)	// File open
	{
		clGrp.Error("File not found","%s File not found.", fileBuffer);
		return -1;
	}

	ret_value = 0;
	while(fgets(Buffer, 1024, fp))	// Read File until end of File. Get one line
	{
		pToken = strtok(Buffer, delimiter);					// Nation
		// Compare to char
		//for(int i=0; i<37; i++)
		//{
			compareResult=strcmp(compareString,pToken);
			if(compareResult==0)
			{
				while(pToken !=NULL)
				{
					pToken=strtok(NULL,delimiter);
					imsi_save=atoi(pToken);
					ret_value=(short)imsi_save;
//					DWriteStringToFile("%s=%d",fileBuffer,ret_value);	// hojae_ddd
					break;
				}
			}
		//}
	}
	
	fclose(fp);
	return ret_value;
	
}

void _ScreenPoint::Init()
{
	SI32	buttonFile;
	
	////////////////////////////////////////////////////////////////////////////////
	// 파일을 읽어서 그림 파일의 위치 정보를 셋팅.			2001.10.19 hojae v0.1
	////////////////////////////////////////////////////////////////////////////////
	char	*FileName;				
	short	Check;
	
	
	FileName="screenPt_plus.dat";	
	buttonFile = FILE_BUTTON_PLUS;
	
	if((Check=Compare(FileName,"")) == -1)	// 우선 파일이 있는지 없는지를 체크하여
	{
		return;								// 없으면 바로 끝~~~
	}
	
	// 메뉴 버튼이 가로로 몇 개까지 위치할 수 있는가?
	MaxMenuXCount=Compare(FileName,"MaxMenuXCount");
	// 메뉴 버튼이 세로로 몇 개까지 위치할 수 있는가?
	MaxMenuYCount=Compare(FileName,"MaxMenuYCount");
	
	MenuButtonXsize=GetFileXsize(buttonFile);
	MenuButtonYsize=GetFileYsize(buttonFile);
	MenuMarginX=Compare(FileName,"MenuMarginX");
	MenuMarginY=Compare(FileName,"MenuMarginY");
	
	// 오른쪽 하단 메뉴 버튼이 그려지는 시작 위치 
	
	
	MenuStartX= 615;//Compare(FileName,"MenuStartX")+2;
	MenuStartY= 523;//Compare(FileName,"MenuStartY");
	
	
	// 단축 메튜 버튼이 그려지는 시작 위치 
	ShortCutMenuStartX = Compare(FileName,"ShortCutMenuStartX");
	ShortCutMenuStartY = Compare(FileName,"ShortCutMenuStartY");
	ShortCutMenuMarginX = Compare(FileName,"ShortCutMenuMarginX");
	ShortCutMenuMarginY = Compare(FileName,"ShortCutMenuMarginY");
	
	
	// 메뉴 버튼이 가로로 몇 개까지 위치할 수 있는가?
	MaxShortCutMenuXCount=Compare(FileName,"MaxShortCutMenuXCount");
	// 메뉴 버튼이 세로로 몇 개까지 위치할 수 있는가?
	MaxShortCutMenuYCount=Compare(FileName,"MaxShortCutMenuYCount");
	
	ShortCutMenuButtonXsize = GetFileXsize(buttonFile);
	ShortCutMenuButtonYsize = GetFileYsize(buttonFile);
	
	GeneralMenuStartX=Compare(FileName,"GeneralMenuStartX");
	GeneralMenuStartY=Compare(FileName,"GeneralMenuStartY");
	
	
	GeneralMenuMarginX=Compare(FileName,"GeneralMenuMarginX");
	
	
	// 초상화 그림
	PortraitStartX				= 689;	
	PortraitStartY				= 16;

	TrainUnitPortraintX				= Compare(FileName,"TrainUnitPortraintX");
	TrainUnitPortraintY				= Compare(FileName,"TrainUnitPortraintY");
	
	
	// 캐릭터 정보 
	HealthInfoStartX			= 722;
	HealthInfoStartY			= 135;

	
	SP.CharInfoYInterval		= Compare(FileName,"CharInfoYInterval") + 10;
	GameMenuButtonStartX		= 766;
	GameMenuButtonStartY		= 532;

	GameAllyTitleX				= 680;
	GameAllyTitleY				= 391;

	GameAllyWindowX				= 680;
	GameAllyWindowY				= 407;

		
	GameMenuButtonXsize				= Compare(FileName,"GameMenuButtonXsize");
	GameMenuButtonYsize				= Compare(FileName,"GameMenuButtonYsize");
	
	CrewStartX						= Compare(FileName,"CrewStartX");
	CrewStartY						= Compare(FileName,"CrewStartY");
	
	CrewXsize						= Compare(FileName,"CrewXsize");
	CrewYsize						= GetFileYsize(FILE_CREW);

	// 시계 판넬
	ClockStartX						= Compare(FileName,"ClockStartX");
	ClockStartY						= Compare(FileName,"ClockStartY");
	
	InventoryMenuStartX				= Compare(FileName,"InventoryMenuStartX");
	InventoryMenuStartY				= Compare(FileName,"InventoryMenuStartY");
	InventoryMenuXsize				= Compare(FileName,"InventoryMenuXsize");
	InventoryMenuMarginX			= Compare(FileName,"InventoryMenuMarginX");

	// 전투모드에서 캐릭터 정보창
	OnlineCharInfoStartX			= 616;//Compare(FileName,"OnlineCharInfoStartX");
	OnlineCharInfoStartY			= 6;//Compare(FileName,"OnlineCharInfoStartY");
		
	HealthGageStartX				= 717;//Compare(FileName, "HealthGageStartX");
	HealthGageStartY				= Compare(FileName, "HealthGageStartY");
}
