#include <GSL.h>

#include <main.h>
#include "..\Gersang\earthquake.h"



SHORT EarthQuakeIndex[]={
	5,5,  0,0,  3,3,  0,0,  1,1,  0,0,  -1
};

void _EarthQuake::Init()
{
	IndexY=0;
}

void _EarthQuake::SetEarthQuake()
{
	if(EarthQuakeSwitch==TRUE)	return;

	EarthQuakeSwitch=TRUE;
	EarthQuakeStep=0;
}


void _EarthQuake::Action()
{

	if(EarthQuakeSwitch==FALSE)return;

	
	IndexY=EarthQuakeIndex[EarthQuakeStep];

	EarthQuakeStep++;

	if(EarthQuakeIndex[EarthQuakeStep]==-1)
	{

		EarthQuakeStep=0;
		EarthQuakeSwitch=FALSE;
		IndexY=0;
	}
}


SHORT _EarthQuake::GetOffsetY(void)
{
	return(IndexY);	
}
