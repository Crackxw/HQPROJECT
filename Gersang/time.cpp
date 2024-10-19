#include <GSL.h>

#include <main.h>

#include "mytime.h"
#include "screenpoint.h"
#include "music.h"
#include "directx.h"
#include <OnlineWorld.h>

#include <map.h>
#include <myfile.h>

extern	_Map 				Map;
extern	_ScreenPoint 		SP;									// 화면에 사용될 여러 정보 


void _Time::Init()
{
	Day=0;
	OldFont=-1;
	DaySwitch=TRUE;
	Hour=0;
	Minute=0;
	PalVary=OldPalVary=0;
	TimeStopSwitch=FALSE;
}

void _Time::SetTimeStop()
{
    TimeStopSwitch=TRUE;
	Hour=1;
	Minute=0;
}

void _Time::PassTime()
{

	int minute_delay = MAX_MINUTE;

	if(TimeStopSwitch==FALSE)Minute++;


	// 겨울 지형은 낮이 짧고 밤이 길다. 
	if(Map.MH.MapType == MAP_TYPE_SNOW)
	{
		// 낮이면, 
		if(DaySwitch == TRUE)
		{
			minute_delay = MAX_MINUTE - 100;
		}
		else
		{
			minute_delay = MAX_MINUTE + 100;
		}
	}
	else
	{
		if(DaySwitch == TRUE)
		{
			minute_delay = MAX_MINUTE ;
		}
		else
		{
			minute_delay = MAX_MINUTE ;
		}
		
	}

	if(Minute >= minute_delay)
	{
		Minute=0;

		Hour++;
		if(Hour>=MAX_HOUR)
		{
			Hour=0;
			Day++;
		}
	}

	if(Hour>=0 && Hour<(MAX_HOUR/2))
	{
	    DaySwitch=TRUE;
	}
	else
	{
		DaySwitch=FALSE;
	}

}

void _Time::DrawTime(LPDIRECTDRAWSURFACE7 psurface)
{

/*	SI32 sifile;


	sifile = FILE_CLOCK_PLUS;

	if(clGrp.LockSurface(psurface)==TRUE)
	{
		Draw_PutSpriteT(SP.ClockStartX, SP.ClockStartY, sifile, Hour);
		clGrp.UnlockSurface(psurface);
	}
	*/
}