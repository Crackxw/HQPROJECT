//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------

#include <GSL.h>
#include <Main.h>

#include "..\Gersang\maindata.h"
#include <CharOrg.h>

#include <windows.h>

#include <directives.h>
#include <pathfind.h>
#include <findpath5.h>
#include <main.h>
#include <charinterface.h>
#include <Map.h>


extern	_Map		Map;
extern	_Char		CR[];
extern	_MainData	GD;

_FindPathGame::_FindPathGame(int maxxsize, int maxysize) : _FindPath(maxxsize, maxysize)
{
}


BOOL _FindPathGame::IsColl(int x, int y)
{

	SI32 i, j;
	SI32 mapx, mapy;
	SI32 xsize = Map.MH.MapXsize;
	SI32 ysize = Map.MH.MapYsize;

	 for(i=0;i<CR[MoveID].m_scIDYsize;i++)
		 for(j=0;j<CR[MoveID].m_scIDXsize;j++)
		 {
			 mapx = x-CR[MoveID].m_scIDXsize+j+1;
			 mapy = y-CR[MoveID].m_scIDYsize+i+1;

			 if(mapx < MinX || mapx > MaxX || mapy < MinY  || mapy > MaxY)return TRUE;
			 if(Map.GetCollMap(mapx, mapy)&CR[MoveID].Block)return TRUE;
		 }


	return FALSE;
}


/////////////////////////
// return 1 if MOVE
//        0 if WAIT
/////////////////////////
int _FindPathGame::FindPath(SHORT x, SHORT y, SHORT dx, SHORT dy, SHORT& mx, SHORT& my, SHORT range, SHORT id)
{

	int returnval;

	if(GD.MoveCharNumber > 6000)
	{
		return -1;
	}
	else
	{
		GD.MoveCharID = id;
	}

	// 이동중인 캐릭터의 아이디 값 설정 
	MoveID=id;

	SHORT length=max(abs(x-dx), abs(y-dy));
	
	MaxNodes = MAXNODES;

	MinX = max(0, x-30);
	MaxX = min(Map.MH.MapXsize-1, x+30);
	MinY = max(0, y-30);
	MaxY = min(Map.MH.MapYsize-1, y+30);;

	returnval = FindPathL(x, y, dx, dy, &mx, &my, &GD.MoveCharNumber, MaxNodes); 

	return returnval;
}



/////////////////////////
// return 1 if MOVE
//        0 if WAIT
/////////////////////////

SHORT FindPathStraight(SHORT realx, SHORT realy, SHORT realdx, SHORT realdy, SHORT direction, SHORT range, SHORT gomode, SHORT& realnx, SHORT &realny)
{

	SHORT x1=realx,  y1=realy;
	SHORT x2=realdx, y2=realdy;
	SHORT x, y;
	SHORT delta_x, delta_y, half, error=0;
	SHORT step=0;
	SHORT index=0;

	

	if(gomode==FALSE)
	{	
		switch(direction)
		{
		case SOUTH:
			x2=x1-range;
			y2=y1+range;			
			break;
		case SSW:
			x2=x1-(range*2);
			y2=y1+range;
			break;
		case SOUTH|WEST:
			x2=x1-range;
			y2=y1;
			break;		
		case WWS:
			x2=x1-(range*2);
			y2=y1-range;
			break;
		case WEST:
			x2=x1-range;
			y2=y1-range;
			break;
		case WWN:
			x2=x1-range;
			y2=y1-(range*2);
			break;			
		case NORTH|WEST:
			x2=x1;
			y2=y1-range;
			break;
		case NNW:
			x2=x1+range;
			y2=y1-(range*2);
			break;			
		case NORTH:
			y2=y1-range;
			x2=x1+range;
			break;
		case NNE:
			x2=x1+(range*2);
			y2=y1-range;
			break;			
		case NORTH|EAST:			
			x2=x1+range;
			y2=y1;
			break;		
		case EEN:
			x2=x1+(range*2);
			y2=y1+range;
			break;
		case EAST:
			x2=x1+range;
			y2=y1+range;			
			break;
		case EES:
			x2=x1+range;
			y2=y1+(range*2);
			break;
		case SOUTH|EAST:
			x2=x1;
			y2=y1+range;
			break;
		case SSE:
			x2=x1-range;
			y2=y1+(range*2);		
			break;
		}		
		
		
	}

	if(x1>x2)delta_x=x1-x2;
	else     delta_x=x2-x1;
	if(y1>y2)delta_y=y1-y2;
	else	 delta_y=y2-y1;

	realnx=x1;
	realny=y1;

    step++;
	if(step>=range)return 1;

	if(delta_x>delta_y)
	{
	   y=y1;
	   half=(delta_x/2);

	   if(x1<x2)
	   {
		   for(x=x1+1;x<=x2;x++)
		   {
			  error+=delta_y;
			  if(error>half)
			  {
				   if(y1<y2)y++;
				   else y--;
				   error-=delta_x;
			  }
			  realnx=x;
			  realny=y;

			  step++;
	          if(step>=range)return 1;
		  }
	   }
	   else
	   {

		   for(x=x1-1;x>=x2;x--)
		   {
			  error+=delta_y;
			  if(error>half)
			  {
				   if(y1<y2)y++;
				   else y--;
				   error-=delta_x;
			  }
			  realnx=x;
			  realny=y;

			  step++;
	          if(step>=range)return 1;
		  }
	   }
	}
	else
	{
	   x=x1;
	   half=(delta_y/2);

	   if(y1<y2)
	   {
		  for(y=y1+1;y<=y2;y++)
		  {
			  error+=delta_x;
			  if(error>half)
			  {
				 if(x1<x2)x++;
				 else x--;
				 error-=delta_y;
			  }
				realnx=x;
				realny=y;
				step++;
	            if(step>=range)return 1;
		  }
	   }
	   else
	   {
		  for(y=y1-1;y>=y2;y--)
		  {
			  error+=delta_x;
			  if(error>half)
			  {
				 if(x1<x2)x++;
				 else x--;
				 error-=delta_y;
			  }
				realnx=x;
				realny=y;
				step++;
	            if(step>=range)return 1;
		  }
	   }
	}

	return 1;
}

