//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------

#include <GSL.h>

#include <Main.h>
#include <MyFile.h>
#include "charanimation.h"
#include <kindinfo.h>


void cltCharAnimation::Set(BOOL dirswitch, SI32 framenumber, char* file, SI32 font, SI32 instant, SI32 delay, SI32 atb)
{
	bDirSwitch		= dirswitch;
    siFrameNumber	= framenumber;
	siFile			= GetValueOfGameSprNumID(file);
	siFont			= font;
	siInstant		= instant;
	siDelay			= delay;
	siAtb			= atb;
}

SI32 cltCharAnimation::GetFrameNumber()
{
	if(siFrameNumber == 0)return 0;

	if(siAtb & ANI_ATB_SINGLEFRAME)
	{
		return 1;
	}

	return siFrameNumber;
}

BOOL cltCharAnimation::IsReady(SI32 step)
{
	if(step >= siInstant)
	{
		return TRUE;
	}
	return FALSE;
}


void cltCharAnimation::DecideFontAni(SI32 dir, SI32 step, int* return_file, int* return_font, BOOL * return_reverseswitch)
{
	SI32 file, font;
	BOOL reverseswitch;

	file =	siFile;

	// 애니메이션을 거꾸로 연주하는 옵션이 설정되어 있다면, 
	if(siAtb & ANI_ATB_REVERSE)
	{
		step = siFrameNumber - 1 - step;
	}

	if(bDirSwitch==TRUE)
	{
		if(siAtb & ANI_ATB_16DIR)
		{
			switch(dir)
			{
			case SOUTH:
				reverseswitch=FALSE;
				font=siFont+siFrameNumber * 2 + step;
				break;
			case SOUTH|WEST:
				reverseswitch=FALSE;
				font=siFont+siFrameNumber * 4 + step;
				break;
			case WEST:
				reverseswitch=FALSE;
				font=siFont+siFrameNumber * 6 + step;
				break;
			case NORTH|WEST:
				reverseswitch=FALSE;
				font=siFont+siFrameNumber * 8 + step;
				break;
			case NORTH:
				reverseswitch=TRUE;
				font=siFont+siFrameNumber * 6 + step;
				break;
			case NORTH|EAST:
				reverseswitch=TRUE;
				font=siFont+siFrameNumber * 4 + step;
				break;
			case EAST:
				reverseswitch=TRUE;
				font=siFont+siFrameNumber * 2 + step;
				break;
			case EAST|SOUTH:
				reverseswitch=FALSE;
				font=siFont+siFrameNumber * 0 + step;
				break;
			case SSW:
				reverseswitch=FALSE;
				font=siFont+siFrameNumber * 3 + step;
				break;
			case EEN:
				reverseswitch=TRUE;
				font=siFont+siFrameNumber * 3 + step;
				break;
			case WWS:
				reverseswitch=FALSE;
				font=siFont+siFrameNumber * 5 + step;
				break;
			case NNE:
				reverseswitch=TRUE;
				font=siFont+siFrameNumber * 5 + step;
				break;
			case WWN:
				reverseswitch=FALSE;
				font=siFont+siFrameNumber * 7 + step;
				break;
			case NNW:
				reverseswitch=TRUE;
				font=siFont+siFrameNumber * 7 + step;
				break;
			case SSE:
				reverseswitch=FALSE;
				font=siFont+siFrameNumber * 1 + step;
				break;
			case EES:
				reverseswitch=TRUE;
				font=siFont+siFrameNumber * 1 + step;
				break;
			default:
				font=step;
				break;
			}
		}
		else if(siAtb & ANI_ATB_2DIR)
		{
			switch(dir)
			{
			case SOUTH:
			case WEST:
			case SOUTH|WEST:
			case NORTH|WEST:
				reverseswitch=FALSE; 
				font = siFont + siFrameNumber * 0 + step;     
				break;
			case NORTH:
			case EAST:
			case EAST|SOUTH:
			case EAST|NORTH:
				reverseswitch=TRUE;  
				font=siFont+siFrameNumber * 0 + step;
				break;
			default:
				break;
			}

		}
		else
		{
			switch(dir)
			{
			case SOUTH:
				reverseswitch=FALSE;
				font=siFont + siFrameNumber*0+step;
				break;
			case SOUTH|WEST:
				reverseswitch=FALSE;
				font=siFont+siFrameNumber*1+step;
				break;
			case WEST:
				reverseswitch=FALSE;
				font=siFont+siFrameNumber*2+step;
				break;
			case NORTH|WEST:
				reverseswitch=FALSE;
				font=siFont+siFrameNumber*3+step;
				break;
			case NORTH:
				reverseswitch=TRUE;
				font=siFont+siFrameNumber*2+step;
				break;
			case NORTH|EAST:
				reverseswitch=TRUE;
				font=siFont+siFrameNumber*1+step;
				break;
			case EAST:
				reverseswitch=TRUE;
				font=siFont+siFrameNumber*0+step;
				break;
			case EAST|SOUTH:
				reverseswitch=FALSE;
				font=siFont+siFrameNumber*4+step;
				break;
			default:
				font=0;
				break;
			}
		}
	}
	else
	{
		reverseswitch=FALSE;
		font = siFont + step;
	}	

	*return_file = file;
	*return_font = font;
	*return_reverseswitch = reverseswitch;

	
}


