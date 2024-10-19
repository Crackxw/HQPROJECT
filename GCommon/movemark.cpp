//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>

#include <main.h>


#include "..\Gersang\maindata.h"


#include <myfile.h>
#include "..\Gersang\DirectX.h"

#include <map.h>
#include <mapinterface.h>

extern	_Map			Map;
extern	_MainData		GD;
extern	cltMapInterface clifMap;			// 맵     관련 인터페이스

void _MoveMark::Init()
{
	X=Y=0;
	MoveMarkSwitch=FALSE;
	Step=0;
	MaxStep = 8;
}

void _MoveMark::SetMoveMark(SHORT x, SHORT y)
{
    MoveMarkSwitch = TRUE;
	Step = 0;
	X=x; Y=y;

	Map.GetRealDotCenterXY(X, Y, RealX, RealY);
}

// real 좌표로 설정한다. 
void _MoveMark::SetMoveMarkReal(SHORT x, SHORT y)
{
    MoveMarkSwitch=TRUE;
	Step=0;
	RealX=x; RealY=y;

}


void _MoveMark::Action()
{
    if(MoveMarkSwitch==FALSE)return ;

	if(GD.Frame%1==0)
	{
	   if(Step<(MaxStep-1))Step++;
	   else 
	   {
		   MoveMarkSwitch=FALSE;
	   }
	}
}

// lock하지 않고 그린다. 
void _MoveMark::DrawMoveMark()
{
	if(MoveMarkSwitch==FALSE)return ;

	int file=FILE_MOVEMARK;
	int font=Step;
	
	// hojae_repair
	SHORT xsize=GetFileXsize(file);
	SHORT ysize=GetFileYsize(file);
	SHORT x=0;
	SHORT y=0;

	// 절대좌표를 화면좌표로 옮긴다. 
	clifMap.TransFromRealDotToScreenDot(RealX, RealY, x, y);

	x-=(xsize/2);
	y-=(ysize/2);

	Draw_PutSpriteLightImjinCT(x, y, xsize, ysize, file, font);	// hojae_append
}



void _WarningMark::Init()
{
	X=Y=0;
	WarningMarkSwitch=FALSE;
	Step=0;
	MaxStep=14;
}

void _WarningMark::SetWarningMark(SHORT x, SHORT y)
{
    WarningMarkSwitch=TRUE;
	Step=0;
	X=x; Y=y;
}

void _WarningMark::Action()
{
    if(WarningMarkSwitch==FALSE)return ;
	
	   if(Step<(MaxStep-1))Step++;
	   else 
	   {
		   WarningMarkSwitch=FALSE;
	   }
	   
}

// lock하지 않고 그린다. 
void _WarningMark::DrawWarningMark()
{
/*	if(WarningMarkSwitch==FALSE)return ;

	int file=FILE_MAPEDITMENU;
	int font=Step;
	int x=0;
	int y=0;


    SMap.GetSXYByBXY(X, Y, x, y);

	x=SMap.StartX+x-16;
	y=SMap.StartY+y-16;

    SetRedrawMap(TRUE, x, y, x+GameSp[file].Header.Xsize-1, y+GameSp[file].Header.Ysize-1);
	
	int cx1, cx2, cy1, cy2;

	GetClipArea(cx1, cy1, cx2, cy2);
	
	SetClipArea(0, 0, ScreenXsize-1, ScreenYsize-1);

	PutSpriteCT(x, y, GameSp[file].Header.Xsize, GameSp[file].Header.Ysize,
                             &GameSp[file].Image[GameSp[file].Header.Start[font]]);

	GetClipArea(cx1, cy1, cx2, cy2);
	*/
}
