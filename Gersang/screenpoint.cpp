#include <GSL.h>

#include <main.h>
#include "screenpoint.h"

#include <myfile.h>
#include "..\Gersang\DirectX.h"
#include "bindjxFile.h"

extern	_ScreenPoint 		SP;									// ȭ�鿡 ���� ���� ���� 


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
	// ������ �о �׸� ������ ��ġ ������ ����.			2001.10.19 hojae v0.1
	////////////////////////////////////////////////////////////////////////////////
	char	*FileName;				
	short	Check;
	
	
	FileName="screenPt_plus.dat";	
	buttonFile = FILE_BUTTON_PLUS;
	
	if((Check=Compare(FileName,"")) == -1)	// �켱 ������ �ִ��� �������� üũ�Ͽ�
	{
		return;								// ������ �ٷ� ��~~~
	}
	
	// �޴� ��ư�� ���η� �� ������ ��ġ�� �� �ִ°�?
	MaxMenuXCount=Compare(FileName,"MaxMenuXCount");
	// �޴� ��ư�� ���η� �� ������ ��ġ�� �� �ִ°�?
	MaxMenuYCount=Compare(FileName,"MaxMenuYCount");
	
	MenuButtonXsize=GetFileXsize(buttonFile);
	MenuButtonYsize=GetFileYsize(buttonFile);
	MenuMarginX=Compare(FileName,"MenuMarginX");
	MenuMarginY=Compare(FileName,"MenuMarginY");
	
	// ������ �ϴ� �޴� ��ư�� �׷����� ���� ��ġ 
	
	
	MenuStartX= 615;//Compare(FileName,"MenuStartX")+2;
	MenuStartY= 523;//Compare(FileName,"MenuStartY");
	
	
	// ���� ��Ʃ ��ư�� �׷����� ���� ��ġ 
	ShortCutMenuStartX = Compare(FileName,"ShortCutMenuStartX");
	ShortCutMenuStartY = Compare(FileName,"ShortCutMenuStartY");
	ShortCutMenuMarginX = Compare(FileName,"ShortCutMenuMarginX");
	ShortCutMenuMarginY = Compare(FileName,"ShortCutMenuMarginY");
	
	
	// �޴� ��ư�� ���η� �� ������ ��ġ�� �� �ִ°�?
	MaxShortCutMenuXCount=Compare(FileName,"MaxShortCutMenuXCount");
	// �޴� ��ư�� ���η� �� ������ ��ġ�� �� �ִ°�?
	MaxShortCutMenuYCount=Compare(FileName,"MaxShortCutMenuYCount");
	
	ShortCutMenuButtonXsize = GetFileXsize(buttonFile);
	ShortCutMenuButtonYsize = GetFileYsize(buttonFile);
	
	GeneralMenuStartX=Compare(FileName,"GeneralMenuStartX");
	GeneralMenuStartY=Compare(FileName,"GeneralMenuStartY");
	
	
	GeneralMenuMarginX=Compare(FileName,"GeneralMenuMarginX");
	
	
	// �ʻ�ȭ �׸�
	PortraitStartX				= 689;	
	PortraitStartY				= 16;

	TrainUnitPortraintX				= Compare(FileName,"TrainUnitPortraintX");
	TrainUnitPortraintY				= Compare(FileName,"TrainUnitPortraintY");
	
	
	// ĳ���� ���� 
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

	// �ð� �ǳ�
	ClockStartX						= Compare(FileName,"ClockStartX");
	ClockStartY						= Compare(FileName,"ClockStartY");
	
	InventoryMenuStartX				= Compare(FileName,"InventoryMenuStartX");
	InventoryMenuStartY				= Compare(FileName,"InventoryMenuStartY");
	InventoryMenuXsize				= Compare(FileName,"InventoryMenuXsize");
	InventoryMenuMarginX			= Compare(FileName,"InventoryMenuMarginX");

	// ������忡�� ĳ���� ����â
	OnlineCharInfoStartX			= 616;//Compare(FileName,"OnlineCharInfoStartX");
	OnlineCharInfoStartY			= 6;//Compare(FileName,"OnlineCharInfoStartY");
		
	HealthGageStartX				= 717;//Compare(FileName, "HealthGageStartX");
	HealthGageStartY				= Compare(FileName, "HealthGageStartY");
}
