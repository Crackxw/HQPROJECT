#ifndef _ROCKET_NEW_TILE_MAP

//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>
#include <main.h>

#include "..\Gersang\maindata.h"
#include "..\Gersang\mapedit.h"

#include <map.h>

extern	_MapEdit		MapEdit;
extern	_MainData		GD;
extern	_Map 			Map;

extern	SHORT			Dir8Q[];
extern	unsigned char	SeaType[][MAX_SEA_VARY*2];


// sx, sy:4배수 좌표 
int _Map::GetSeaNear1(SHORT sx, SHORT sy)
{
	int number=0;
	SHORT x, y;

	// 남쪽 
	x=sx; y=sy+1;
	if(y<MH.MapYsize/2)
	{
		if(GetSeaMode(x*2,y*2))number|=4;
		if(GetSeaMode(x*2+1,y*2))number|=8;
	}


	// 서쪽 
	x=sx-1;
	y=sy;
	if(x>=0)
	{
		if(GetSeaMode(x*2+1,y*2))number|=1;
		if(GetSeaMode(x*2+1,y*2+1))number|=4;
	}


	// 북쪽 
	 x=sx;
	 y=sy-1;
	 if(y>=0)
	 {
		if(GetSeaMode(x*2,y*2+1))number|=1;
		if(GetSeaMode(x*2+1,y*2+1))number|=2;

	 }

	 // 동쪽 
	 x=sx+1;
	 y=sy;
	 if(x<MH.MapXsize/2)
	 {
		if(GetSeaMode(x*2,y*2))number|=2;
		if(GetSeaMode(x*2,y*2+1))number|=8;

	 }

	 // 남서쪽 
	 x=sx-1;
	 y=sy+1;
	 if(x>=0 && y<MH.MapYsize/2)
	 {
		if(GetSeaMode(x*2+1,y*2))number|=4;
	 }

	 // 북서쪽 
	 x=sx-1;
	 y=sy-1;
	 if(x>=0 && y>=0)
	 {
		if(GetSeaMode(x*2+1,y*2+1))number|=1;

	 }

	 // 북동쪽 
	 x=sx+1;
	 y=sy-1;
	 if(x<MH.MapXsize/2 && y>=0)
	 {
		if(GetSeaMode(x*2,y*2+1))number|=2;
	 }

	 // 남동쪽 
	 x=sx+1;
	 y=sy+1;
	 if(x<MH.MapXsize/2 && y<MH.MapYsize/2)
	 {
		if(GetSeaMode(x*2,y*2))number|=8;
	 }

	return number;

}

// sx, sy:4배수 좌표 
int _Map::GetSeaNear2(SHORT sx, SHORT sy)
{
	
	int number=0;
	SHORT x, y;
	
	// 남쪽 
	x=sx; y=sy+1;
	if(y<MH.MapYsize/2)
	{
		if(GetSeaMode(x*2,y*2)==0)number|=4;
		if(GetSeaMode(x*2+1,y*2)==0)number|=8;
	}
	
	
	// 서쪽 
	x=sx-1;
	y=sy;
	if(x>=0)
	{
		if(GetSeaMode(x*2+1,y*2)==0)number|=1;
		if(GetSeaMode(x*2+1,y*2+1)==0)number|=4;
	}
	
	
	// 북쪽 
	x=sx;
	y=sy-1;
	if(y>=0)
	{
		if(GetSeaMode(x*2,y*2+1)==0)number|=1;
		if(GetSeaMode(x*2+1,y*2+1)==0)number|=2;
		
	}
	
	// 동쪽 
	x=sx+1;
	y=sy;
	if(x<MH.MapXsize/2)
	{
		if(GetSeaMode(x*2,y*2)==0)number|=2;
		if(GetSeaMode(x*2,y*2+1)==0)number|=8;
		
	}
	
	// 남서쪽 
	x=sx-1;
	y=sy+1;
	if(x>=0 && y<MH.MapYsize/2)
	{
		if(GetSeaMode(x*2+1,y*2)==0)number|=4;
	}
	
	// 북서쪽 
	x=sx-1;
	y=sy-1;
	if(x>=0 && y>=0)
	{
		if(GetSeaMode(x*2+1,y*2+1)==0)number|=1;
		
	}
	
	// 북동쪽 
	x=sx+1;
	y=sy-1;
	if(x<MH.MapXsize/2 && y>=0)
	{
		if(GetSeaMode(x*2,y*2+1)==0)number|=2;
	}
	
	// 남동쪽 
	x=sx+1;
	y=sy+1;
	if(x<MH.MapXsize/2 && y<MH.MapYsize/2)
	{
		if(GetSeaMode(x*2,y*2)==0)number|=8;
	}
	
	
	return number;
	
}



// 주변의 상황을 고려하여 충돌영역을 설정한다. 
// x, y :2배수 좌표 
void _Map::PutCollSea(SHORT x, SHORT y)
{

	SHORT dir=0;

	if(GetSeaMode(x, y)==0)return ;

	if(GetSeaMode(x, y))
	{
		SetCollMap(x,   y,   BLOCK_DEEPWATER);
	}


}


// sx, sy: 2배수 좌표 
// pushmode : 주변의 이종 타일을 없앨것인가?
void _Map::PutSea(SHORT sx, SHORT sy, SHORT mode, SHORT pushmode)
{
	int number;
	int i, j;
	SHORT qx, qy;

	// 유효한 영역인지 확인한다. 
	if(sx>=0 && sx<MH.MapXsize && sy>=0 && sy<MH.MapYsize){}
	else return ;
	
	if(mode==TRUE)
	{
		// 꽉차있는 모습을 저장한다. 
		SetSeaMode(1, sx,sy);
		
		// 언덕을 0으로 만든다. 
		if(pushmode==TRUE)
		{
			if( pTopLevel )	
			{
				if( pTopLevel[sx + YOff[sy]] )
				{
					MapEdit.MapEdit_PutLevel(sx,   sy, 0, FALSE);
				}
			}
		}
		
	}
	else
	{
		SetSeaMode(0, sx,sy);
	}
	
	
	qx=sx/2;
	qy=sy/2;

	for(i=-1;i<=1;i++)
		for(j=-1;j<=1;j++)
		{
			// 범위를 벗어난 것도 제외된다. 
			if(qx+j>=0 && qx+j<MH.MapXsize/2 && qy+i>=0 && qy+i<MH.MapYsize/2){}
			else continue;

			if(mode==TRUE)
			{
		       number=GetSeaNear1(qx+j, qy+i);

			   if(GetSeaMode((qx+j)*2,(qy+i)*2)==1)
			   {
				   number|=1;
			   }
			   if(GetSeaMode((qx+j)*2+1,(qy+i)*2)==1)
			   {
				   number|=2;
			   }
			   if(GetSeaMode((qx+j)*2,(qy+i)*2+1)==1)
			   {
				   number|=4;
			   }
			   if(GetSeaMode((qx+j)*2+1,(qy+i)*2+1)==1)
			   {
				   number|=8;
			   }
			}
			else
			{
			   number=GetSeaNear2(qx+j, qy+i);

			   if(GetSeaMode((qx+j)*2,(qy+i)*2)==0)
			   {
				      number|=1;
			   }
			   if(GetSeaMode((qx+j)*2+1,(qy+i)*2)==0)
			   {
				      number|=2;
			   }
			   if(GetSeaMode((qx+j)*2,(qy+i)*2+1)==0)
			   {
				      number|=4;
			   }
			   if(GetSeaMode((qx+j)*2+1, (qy+i)*2+1)==0)
			   {
				     number|=8;
			   }

			   //Error("", "x:%d y:%d number:%d", qx+j, qy+i, number);			   

	           number=15-number;

			}

			// 주변에 전혀 없다. 
			if(Dir8Q[number]==-2)
			{
				SetSeaFile(0, (qx+j), (qy+i));
	            SetSeaFont(0, (qx+j), (qy+i));

				SetSeaMode(0, (qx+j)*2, (qy+i)*2);
				SetSeaMode(0, (qx+j)*2+1, (qy+i)*2);
				SetSeaMode(0, (qx+j)*2, (qy+i)*2+1);
				SetSeaMode(0, (qx+j)*2+1, (qy+i)*2+1);

			}
			// 적절한 세트가 없다. 
			else if(Dir8Q[number]==-1)
			{
				if(mode==TRUE)
				{
					SetSeaMode(1, (qx+j)*2, (qy+i)*2);
					SetSeaMode(1, (qx+j)*2+1, (qy+i)*2);
					SetSeaMode(1, (qx+j)*2, (qy+i)*2+1);
					SetSeaMode(1, (qx+j)*2+1, (qy+i)*2+1);
				}
				else
				{
					SetSeaMode(0, (qx+j)*2, (qy+i)*2);
					SetSeaMode(0, (qx+j)*2+1, (qy+i)*2);
					SetSeaMode(0, (qx+j)*2, (qy+i)*2+1);
					SetSeaMode(0, (qx+j)*2+1, (qy+i)*2+1);
				}
				
				PutSea((qx+j)*2, (qy+i)*2, mode, pushmode);

			}
			else
			{
				unsigned char _randfile=SeaType[ Dir8Q[number] ][(GD.Random()%MAX_SEA_VARY)*2];
	            unsigned char _randfont=SeaType[ Dir8Q[number] ][(GD.Random()%MAX_SEA_VARY)*2+1]; 

				SetSeaFile(_randfile, (qx+j), (qy+i));
	            SetSeaFont(_randfont, (qx+j), (qy+i));

				if(mode==TRUE)
				{
					// 주변의 언덕을 지운다. 
					if(pushmode==TRUE)
					{
					   MapEdit.MapEdit_PutLevel((qx+j)*2,     (qy+i)*2,   0, FALSE);
					   MapEdit.MapEdit_PutLevel((qx+j)*2+1,   (qy+i)*2,   0, FALSE);
					   MapEdit.MapEdit_PutLevel((qx+j)*2,     (qy+i)*2+1, 0, FALSE);
					   MapEdit.MapEdit_PutLevel((qx+j)*2+1,   (qy+i)*2+1, 0, FALSE);
					}

				    if(number&1)
					{
					   SetSeaMode(1, (qx+j)*2, (qy+i)*2);
					}
				    else 
					   SetSeaMode(0, (qx+j)*2, (qy+i)*2);
				   
				    if(number&2)
					{
				 	   SetSeaMode(1, (qx+j)*2+1, (qy+i)*2);
					}
				    else 
					   SetSeaMode(0, (qx+j)*2+1, (qy+i)*2);

				    if(number&4)
					{
					   SetSeaMode(1, (qx+j)*2, (qy+i)*2+1);
					}
				    else 
					   SetSeaMode(0, (qx+j)*2, (qy+i)*2+1);

				    if(number&8)
					{
					   SetSeaMode(1, (qx+j)*2+1, (qy+i)*2+1);
					}
				    else
					   SetSeaMode(0, (qx+j)*2+1, (qy+i)*2+1);
				}
				else
				{
				   if((number&1)==0)
					   SetSeaMode(0, (qx+j)*2, (qy+i)*2);
				   else 
					   SetSeaMode(1, (qx+j)*2, (qy+i)*2);

				   if((number&2)==0)
				 	   SetSeaMode(0, (qx+j)*2+1, (qy+i)*2);
				   else
					   SetSeaMode(1, (qx+j)*2+1, (qy+i)*2);

				   if((number&4)==0)
					   SetSeaMode(0, (qx+j)*2, (qy+i)*2+1);
				   else
					   SetSeaMode(1, (qx+j)*2, (qy+i)*2+1);
				   
				   if((number&8)==0)
					   SetSeaMode(0, (qx+j)*2+1, (qy+i)*2+1);
				   else
					   SetSeaMode(1, (qx+j)*2+1, (qy+i)*2+1);
				}
			}
		}

}

// 물은 만들지 않고 얕은 영역만 만든다. 
void _Map::PutShallow(SHORT x, SHORT y, SHORT mode)
{
	// 유효한 영역인지 확인한다. 
	if(x>=0 && x<MH.MapXsize && y>=0 && y<MH.MapYsize){}
	else return ;
	
	if( mode==TRUE )
	{
		if( pShallowMap )	pShallowMap[x+YOff[y]] = 1;
		if( pShallowFont )	pShallowFont[x+YOff[y]] = GD.Random()%4;
	}
	else
	{
		SetSeaMode(0, x, y);
		if( pShallowMap )	pShallowMap[x+YOff[y]] = 0;
	}
}


// 물과 얕은 영역을 동시에 만든다. 
// sx, sy: 2배수 좌표 
void _Map::PutShallowSea(SHORT sx, SHORT sy, SHORT mode)
{
	int number;
	int i, j;
	SHORT qx, qy;

	// 유효한 영역인지 확인한다. 
	if(sx>=0 && sx<MH.MapXsize && sy>=0 && sy<MH.MapYsize){}
	else return ;
	
	if(mode==TRUE)
	{
		// 꽉차있는 모습을 저장한다. 
		SetSeaMode(1, sx, sy);
		if( pShallowMap )	pShallowMap[sx + YOff[sy]] = 1;
		if( pShallowFont )	pShallowFont[sx + YOff[sy]] = GD.Random()%4;
	}
	else
	{
		SetSeaMode(0, sx, sy); 
		if( pShallowMap )	pShallowMap[sx + YOff[sy]] = 0;
	}
	
	qx=sx/2;
	qy=sy/2;

	for(i=-1;i<=1;i++)
		for(j=-1;j<=1;j++)
		{
			// 범위를 벗어난것도 제외된다. 
			if(qx+j>=0 && qx+j<MH.MapXsize/2 && qy+i>=0 && qy+i<MH.MapYsize/2){}
			else continue;

			if(mode==TRUE)
			{
		       number=GetSeaNear1(qx+j, qy+i);

			   if(GetSeaMode((qx+j)*2, (qy+i)*2)==1)
			   {
				   number|=1;
			   }
			   if(GetSeaMode((qx+j)*2+1, (qy+i)*2)==1)
			   {
				   number|=2;
			   }
			   if(GetSeaMode((qx+j)*2, (qy+i)*2+1)==1)
			   {
				   number|=4;
			   }
			   if(GetSeaMode((qx+j)*2+1, (qy+i)*2+1)==1)
			   {
				   number|=8;
			   }
			}
			else
			{
			   number=GetSeaNear2(qx+j, qy+i);

			   if(GetSeaMode((qx+j)*2, (qy+i)*2)==0)
			   {
				      number|=1;
			   }
			   if(GetSeaMode((qx+j)*2+1, (qy+i)*2)==0)
			   {
				      number|=2;
			   }
			   if(GetSeaMode((qx+j)*2, (qy+i)*2+1)==0)
			   {
				      number|=4;
			   }
			   if(GetSeaMode((qx+j)*2+1, (qy+i)*2+1)==0)
			   {
				     number|=8;
			   }

			   //Error("", "x:%d y:%d number:%d", qx+j, qy+i, number);			   

	           number=15-number;

			}

			// 주변에 전혀 없다. 
			if(Dir8Q[number]==-2)
			{
				SetSeaFile(0, (qx+j), (qy+i));
	            SetSeaFont(0, (qx+j), (qy+i));

				SetSeaMode(0, (qx+j)*2, (qy+i)*2);
				SetSeaMode(0, (qx+j)*2+1, (qy+i)*2);
				SetSeaMode(0, (qx+j)*2, (qy+i)*2+1);
				SetSeaMode(0, (qx+j)*2+1, (qy+i)*2+1);

			}
			// 적절한 세트가 없다. 
			else if(Dir8Q[number]==-1)
			{
				if(mode==TRUE)
				{
					SetSeaMode(1, (qx+j)*2, (qy+i)*2);
					SetSeaMode(1, (qx+j)*2+1, (qy+i)*2);
					SetSeaMode(1, (qx+j)*2, (qy+i)*2+1);
					SetSeaMode(1, (qx+j)*2+1, (qy+i)*2+1);
				}
				else
				{
					SetSeaMode(0, (qx+j)*2, (qy+i)*2);
					SetSeaMode(0, (qx+j)*2+1, (qy+i)*2);
					SetSeaMode(0, (qx+j)*2, (qy+i)*2+1);
					SetSeaMode(0, (qx+j)*2+1, (qy+i)*2+1);
				}
				
				PutShallowSea((qx+j)*2, (qy+i)*2, mode);

			}
			else
			{
				unsigned char _randfile=SeaType[ Dir8Q[number] ][(GD.Random()%MAX_SEA_VARY)*2];
	            unsigned char _randfont=SeaType[ Dir8Q[number] ][(GD.Random()%MAX_SEA_VARY)*2+1]; 

				SetSeaFile(_randfile, (qx+j), (qy+i));
	            SetSeaFont(_randfont, (qx+j), (qy+i));

				if(mode==TRUE)
				{
				   if(number&1)
					   SetSeaMode(1, (qx+j)*2, (qy+i)*2);
				   else 
					   SetSeaMode(0, (qx+j)*2, (qy+i)*2);
				   
				   if(number&2)
				 	   SetSeaMode(1, (qx+j)*2+1, (qy+i)*2);
				   else 
					   SetSeaMode(0, (qx+j)*2+1, (qy+i)*2);

				   if(number&4)
					   SetSeaMode(1, (qx+j)*2, (qy+i)*2+1);
				   else 
					   SetSeaMode(0, (qx+j)*2, (qy+i)*2+1);

				   if(number&8)
					   SetSeaMode(1, (qx+j)*2+1, (qy+i)*2+1);
				   else
					   SetSeaMode(0, (qx+j)*2+1, (qy+i)*2+1);
				}
				else
				{
				   if((number&1)==0)
					   SetSeaMode(0, (qx+j)*2, (qy+i)*2);
				   else 
					   SetSeaMode(1, (qx+j)*2, (qy+i)*2);

				   if((number&2)==0)
				 	   SetSeaMode(0, (qx+j)*2+1, (qy+i)*2);
				   else
					   SetSeaMode(1, (qx+j)*2+1, (qy+i)*2);

				   if((number&4)==0)
					   SetSeaMode(0, (qx+j)*2, (qy+i)*2+1);
				   else
					   SetSeaMode(1, (qx+j)*2, (qy+i)*2+1);
				   
				   if((number&8)==0)
					   SetSeaMode(0, (qx+j)*2+1, (qy+i)*2+1);
				   else
					   SetSeaMode(1, (qx+j)*2+1, (qy+i)*2+1);

				}
			}
		}

}






#else
//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>
#include <main.h>

#include "..\Gersang\maindata.h"
#include "..\Gersang\mapedit.h"

#include <map.h>

extern	_MapEdit		MapEdit;
extern	_MainData		GD;
extern	_Map 			Map;

extern	SHORT			Dir8Q[];
extern	unsigned char	SeaType[][MAX_SEA_VARY*2];
extern int FileTileSea0;;


// sx, sy:4배수 좌표 
int _Map::GetSeaNear1(SHORT sx, SHORT sy)
{
	int number=0;
	SHORT x, y;

	// 남쪽 
	x=sx; y=sy+1;
	if(y<MH.MapYsize/2)
	{
		if(GetSeaMode(x*2,y*2))number|=4;
		if(GetSeaMode(x*2+1,y*2))number|=8;
	}


	// 서쪽 
	x=sx-1;
	y=sy;
	if(x>=0)
	{
		if(GetSeaMode(x*2+1,y*2))number|=1;
		if(GetSeaMode(x*2+1,y*2+1))number|=4;
	}


	// 북쪽 
	 x=sx;
	 y=sy-1;
	 if(y>=0)
	 {
		if(GetSeaMode(x*2,y*2+1))number|=1;
		if(GetSeaMode(x*2+1,y*2+1))number|=2;

	 }

	 // 동쪽 
	 x=sx+1;
	 y=sy;
	 if(x<MH.MapXsize/2)
	 {
		if(GetSeaMode(x*2,y*2))number|=2;
		if(GetSeaMode(x*2,y*2+1))number|=8;

	 }

	 // 남서쪽 
	 x=sx-1;
	 y=sy+1;
	 if(x>=0 && y<MH.MapYsize/2)
	 {
		if(GetSeaMode(x*2+1,y*2))number|=4;
	 }

	 // 북서쪽 
	 x=sx-1;
	 y=sy-1;
	 if(x>=0 && y>=0)
	 {
		if(GetSeaMode(x*2+1,y*2+1))number|=1;

	 }

	 // 북동쪽 
	 x=sx+1;
	 y=sy-1;
	 if(x<MH.MapXsize/2 && y>=0)
	 {
		if(GetSeaMode(x*2,y*2+1))number|=2;
	 }

	 // 남동쪽 
	 x=sx+1;
	 y=sy+1;
	 if(x<MH.MapXsize/2 && y<MH.MapYsize/2)
	 {
		if(GetSeaMode(x*2,y*2))number|=8;
	 }

	return number;

}

// sx, sy:4배수 좌표 
int _Map::GetSeaNear2(SHORT sx, SHORT sy)
{
	
	int number=0;
	SHORT x, y;
	
	// 남쪽 
	x=sx; y=sy+1;
	if(y<MH.MapYsize/2)
	{
		if(GetSeaMode(x*2,y*2)==0)number|=4;
		if(GetSeaMode(x*2+1,y*2)==0)number|=8;
	}
	
	
	// 서쪽 
	x=sx-1;
	y=sy;
	if(x>=0)
	{
		if(GetSeaMode(x*2+1,y*2)==0)number|=1;
		if(GetSeaMode(x*2+1,y*2+1)==0)number|=4;
	}
	
	
	// 북쪽 
	x=sx;
	y=sy-1;
	if(y>=0)
	{
		if(GetSeaMode(x*2,y*2+1)==0)number|=1;
		if(GetSeaMode(x*2+1,y*2+1)==0)number|=2;
		
	}
	
	// 동쪽 
	x=sx+1;
	y=sy;
	if(x<MH.MapXsize/2)
	{
		if(GetSeaMode(x*2,y*2)==0)number|=2;
		if(GetSeaMode(x*2,y*2+1)==0)number|=8;
		
	}
	
	// 남서쪽 
	x=sx-1;
	y=sy+1;
	if(x>=0 && y<MH.MapYsize/2)
	{
		if(GetSeaMode(x*2+1,y*2)==0)number|=4;
	}
	
	// 북서쪽 
	x=sx-1;
	y=sy-1;
	if(x>=0 && y>=0)
	{
		if(GetSeaMode(x*2+1,y*2+1)==0)number|=1;
		
	}
	
	// 북동쪽 
	x=sx+1;
	y=sy-1;
	if(x<MH.MapXsize/2 && y>=0)
	{
		if(GetSeaMode(x*2,y*2+1)==0)number|=2;
	}
	
	// 남동쪽 
	x=sx+1;
	y=sy+1;
	if(x<MH.MapXsize/2 && y<MH.MapYsize/2)
	{
		if(GetSeaMode(x*2,y*2)==0)number|=8;
	}
	
	
	return number;
	
}



// 주변의 상황을 고려하여 충돌영역을 설정한다. 
// x, y :2배수 좌표 
void _Map::PutCollSea(SHORT x, SHORT y)
{

	SHORT dir=0;

	if(GetSeaMode(x, y)==0)return ;

	if(GetSeaMode(x, y))
	{
		SetCollMap(x,   y,   BLOCK_DEEPWATER);
	}


}

void _Map::PutSea_Daesang(SHORT sx, SHORT sy, SHORT mode)
{
	#define DEFINE_ORIGINAL_TILE 72//12*6  실제 가운데 타일이 시작하는 위치..(그전에는 이종타일과 연결하는 타일)
	// 유효한 지도 영역인지 확인한다.	
	//홀수면 짝수로 만든다.
	if( sx%2 ) sx--;
	if( sy%2 ) sy--;
	
	int FileTileNo = -1;	
	
	PutSea_OnlySet_Daesang( sx, sy, mode, DEFINE_ORIGINAL_TILE+(rand()%6)*4);	
	
	int greenno		= 0;
	int grassno		= 0;
	int cropareano	= 0;
	bool bIsCleanToNormal	= false;
	
	//연결 할수없는 타일을 찾아서 NormalTile로 초기화시킨다.
	for( int i = -1; i<2; i++)
	{
		for( int j = -1; j<2; j++)
		{
			if( i == 0 && j == 0) continue;
			
			if(IsInMapArea(sx+j*2, sy+i*2) == FALSE) continue;
			
			//현재 찍으려는 것과 같다면 넘어간다..
			if( GetSeaMode(sx+j*2, sy+i*2) == 1 ) continue;
			
			//Grass
			if( GetSeaMode( sx+j*2, sy+i*2) == 0 )
			{
				grassno++;
				if( greenno > 0 || cropareano > 0 )
				{
					bIsCleanToNormal = true;					
				}
			}
		}
	}
	
	//주변의 이종 타일의 종류가 2개 이상이라면 노멀 맵으로 초기화한다.
	if( bIsCleanToNormal == true )
	{		
		for( i = -1; i<2; i++)
		{
			for( int j = -1; j<2; j++)
			{				
				//PutSea_OnlySet_Daesang( sx+j*2, sy+i*2 ,mode);
			}
		}
	}
	
	//주변의 이종타일을 설정한다.
	SetProperSeaFont_Daesang(sx, sy, mode);
}


void _Map::PutSea_OnlySet_Daesang( SHORT sx, SHORT sy, SHORT mode, SHORT startfont)
{		
	//대상타일은 4개 단위로 찍힌다.
	//int randTileFont = (rand()%6)*4;//타일의 종류는 6개..	

	int fontcount = 0;
	for( int i =0 ; i < 2; i++)
	{
		for( int j = 0; j < 2; j++)
		{
			if(IsInMapArea(sx+j, sy+i) == FALSE) 
			{
				fontcount++;
				continue;
			}

			if( mode == TRUE )
			{
				SetSeaMode(1, sx+j, sy+i);
				SetSeaFile(FileTileSea0, sx+j, sy+i);
				SetSeaFont( startfont + fontcount, sx+j, sy+i);
			}
			else
			{
				SetSeaMode(0, sx+j, sy+i);	
				SetSeaFile(0, sx+j, sy+i);
				SetSeaFont( 0, sx+j, sy+i);				
			}

			

			fontcount++;
		}
	}
}


void _Map::SetProperSeaFont_Daesang(SHORT sx, SHORT sy , SHORT mode )
{
	#define DEFINE_ORIGINAL_TILE 72//12*6  실제 가운데 타일이 시작하는 위치..(그전에는 이종타일과 연결하는 타일)
	//연결 타일의 속성 설정..
	//1이면 자기자신, 0이면 Normal 타일과 연결되는부분..
	//27: 타일셋에서  전체 개수가 27개
	//4 : 한개의 타일은 4개로 쪼개진다.
	char LinkAtb[27][4] = { 
		{1,0,0,0}, {0,1,0,0}, {0,0,0,1}, {0,0,1,0}, //연결되는부분이 1개짜리인 것들
		{1,1,0,0}, {1,1,0,0}, {0,1,0,1}, {0,1,0,1}, {0,0,1,1}, {0,0,1,1}, {1,0,1,0}, {1,0,1,0},//2개짜리인것들..
		{1,1,0,1}, {0,1,1,1}, {1,0,1,1}, {1,1,1,0}, //3개짜리인것들
		{0,1,1,0}, {1,0,0,1},//떨어진 2개짜리
		{1,1,1,1}, {1,1,1,1}, {1,1,1,1}, {1,1,1,1}, {1,1,1,1}, {1,1,1,1}, {1,1,1,1}, {1,1,1,1}, {1,1,1,1}};//4개짜리
		
		char Type[4];
		char TileSetting[6][6];
		
		memset(TileSetting, 0, sizeof( TileSetting));
		
		//찍는놈과 그 주위놈은 1로 세팅..디폴트일때는 0으로 세팅..
		for(  int i = 1; i < 5; i ++)
		{
			for( int j = 1; j < 5; j++ )
			{
				TileSetting[i][j] = 1;			
			}
		}
		
		//그 주변놈들은 주변 환경에 맞춰서 찾는다.
		//X축..Y=0인놈들..
		for( i = 0; i < 6; i++)
		{
			if(IsInMapArea(sx+i-2, sy-2-1) == FALSE) continue;
			
			int font = pSeaFont[(sx+i-2)+YOff[sy-2-1]];
			if( GetSeaMode(sx+i-2, sy-2-1) == 1)
			{
				//폰트를 봐서 채워진 놈이라면..			
				if( LinkAtb[font/4][font%4] == 1)
					TileSetting[0][i] += 1;
				else 
					TileSetting[0][i] += 0;
			}		
		}
		
		//X축..Y=5인놈들..
		for( i = 0; i < 6; i++)
		{
			if(IsInMapArea(sx+i-2, sy+3+1) == FALSE) continue;
			
			int font = pSeaFont[(sx+i-2)+YOff[sy+3+1]];
			
			if( GetSeaMode( (sx+i-2), sy+3+1) == 1 )
			{
				//폰트를 봐서 채워진 놈이라면..
				if( LinkAtb[font/4][font%4] == 1)
					TileSetting[5][i] += 1;
				else 
					TileSetting[5][i] += 0;
			}		
		}
		
		//Y축..X=0인놈들..
		for( i = 0; i < 6; i++)
		{
			if(IsInMapArea(sx-2-1, sy-2+i) == FALSE) continue;
			int font = pSeaFont[(sx-2-1)+YOff[sy-2+i]];
			if( GetSeaMode(sx-2-1, sy-2+i) == 1)
			{
				//폰트를 봐서 채워진 놈이라면..			
				if( LinkAtb[font/4][font%4] == 1)
					TileSetting[i][0] += 1;
				else 
					TileSetting[i][0] += 0;
			}
		}
		
		//Y축..X=5인놈들..
		for( i = 0; i < 6; i++)
		{
			if(IsInMapArea(sx+3+1, sy-2+i) == FALSE) continue;
			int font = pSeaFont[(sx+3+1)+YOff[sy-2+i]];
			if( GetSeaMode(sx+3+1, sy-2+i) == 1)	
			{
				//폰트를 봐서 채워진 놈이라면..			
				if( LinkAtb[font/4][font%4] == 1)
					TileSetting[i][5] += 1;
				else 
					TileSetting[i][5] += 0;
			}
		}
		
		
		
		if( TileSetting[0][0] == 2)	TileSetting[0][0] = 1;
		else TileSetting[0][0] =  0;
		
		if( TileSetting[0][5] == 2)	TileSetting[0][5] =  1;
		else TileSetting[0][5] =0;
		
		if( TileSetting[5][0] == 2)	TileSetting[5][0] = 1;
		else TileSetting[5][0] = 0;
		
		if( TileSetting[5][5] == 2)	TileSetting[5][5] = 1;
		else TileSetting[5][5] = 0;	
		
		//적합한 타일을 찾아서 설정한다.
		char tmpData[4];
		int FindIndex = -1;
		for( i = 0 ; i < 3; i++)
		{
			for( int j = 0; j < 3; j++)
			{
				tmpData[0] = TileSetting[i*2][j*2];
				tmpData[1] = TileSetting[i*2][j*2+1];
				tmpData[2] = TileSetting[i*2+1][j*2];
				tmpData[3] = TileSetting[i*2+1][j*2+1];
				
				int FindIndex = -1;
				
				//모양이 같은 타일을 찾는다.
				for( int k = 0; k < 27; k++)
				{
					if( tmpData[0] == LinkAtb[k][0] && 
						tmpData[1] == LinkAtb[k][1] && 
						tmpData[2] == LinkAtb[k][2] && 
						tmpData[3] == LinkAtb[k][3] )
					{
						FindIndex = k;		
						break;
					}
				}
				
				if( k >= 4 && k <= 10)
				{
					//2개씩 붙어있는 타일이 있는거라면.. 2쌍씩있으므로..
					k += rand()%2;
				}
				if( k >= 18 ) 
				{
					//꽉찬 타일이면 다른 모양의 랜덤 모양으로 설정한다.
					k+=rand()%9;
				}
				//적합한 타일을 찾았다면..
				if( FindIndex > -1 )
				{		
					PutSea_OnlySet_Daesang(sx+(j-1)*2,  sy+(i-1)*2, mode, k*4);
				}
				else
				{
					//적당한걸 못찾았다면..
					PutSea_OnlySet_Daesang(sx+(j-1)*2,  sy+(i-1)*2, mode, DEFINE_ORIGINAL_TILE +(rand()%6)*4);					
				}
			}	
		}
	
}



// sx, sy: 2배수 좌표 
// pushmode : 주변의 이종 타일을 없앨것인가?
void _Map::PutSea(SHORT sx, SHORT sy, SHORT mode, SHORT pushmode)
{
	int number;
	int i, j;
	SHORT qx, qy;

	// 유효한 영역인지 확인한다. 
	if(sx>=0 && sx<MH.MapXsize && sy>=0 && sy<MH.MapYsize){}
	else return ;
	
	if(mode==TRUE)
	{
		// 꽉차있는 모습을 저장한다. 
		SetSeaMode(1, sx,sy);
		
		// 언덕을 0으로 만든다. 
		if(pushmode==TRUE)
		{
			if( pTopLevel )	
			{
				if( pTopLevel[sx + YOff[sy]] )
				{
					MapEdit.MapEdit_PutLevel(sx,   sy, 0, FALSE);
				}
			}
		}
		
	}
	else
	{
		SetSeaMode(0, sx,sy);
	}
	
	
	qx=sx/2;
	qy=sy/2;

	for(i=-1;i<=1;i++)
		for(j=-1;j<=1;j++)
		{
			// 범위를 벗어난 것도 제외된다. 
			if(qx+j>=0 && qx+j<MH.MapXsize/2 && qy+i>=0 && qy+i<MH.MapYsize/2){}
			else continue;

			if(mode==TRUE)
			{
		       number=GetSeaNear1(qx+j, qy+i);

			   if(GetSeaMode((qx+j)*2,(qy+i)*2)==1)
			   {
				   number|=1;
			   }
			   if(GetSeaMode((qx+j)*2+1,(qy+i)*2)==1)
			   {
				   number|=2;
			   }
			   if(GetSeaMode((qx+j)*2,(qy+i)*2+1)==1)
			   {
				   number|=4;
			   }
			   if(GetSeaMode((qx+j)*2+1,(qy+i)*2+1)==1)
			   {
				   number|=8;
			   }
			}
			else
			{
			   number=GetSeaNear2(qx+j, qy+i);

			   if(GetSeaMode((qx+j)*2,(qy+i)*2)==0)
			   {
				      number|=1;
			   }
			   if(GetSeaMode((qx+j)*2+1,(qy+i)*2)==0)
			   {
				      number|=2;
			   }
			   if(GetSeaMode((qx+j)*2,(qy+i)*2+1)==0)
			   {
				      number|=4;
			   }
			   if(GetSeaMode((qx+j)*2+1, (qy+i)*2+1)==0)
			   {
				     number|=8;
			   }

			   //Error("", "x:%d y:%d number:%d", qx+j, qy+i, number);			   

	           number=15-number;

			}

			// 주변에 전혀 없다. 
			if(Dir8Q[number]==-2)
			{
				SetSeaFile(0, (qx+j), (qy+i));
	            SetSeaFont(0, (qx+j), (qy+i));

				SetSeaMode(0, (qx+j)*2, (qy+i)*2);
				SetSeaMode(0, (qx+j)*2+1, (qy+i)*2);
				SetSeaMode(0, (qx+j)*2, (qy+i)*2+1);
				SetSeaMode(0, (qx+j)*2+1, (qy+i)*2+1);

			}
			// 적절한 세트가 없다. 
			else if(Dir8Q[number]==-1)
			{
				if(mode==TRUE)
				{
					SetSeaMode(1, (qx+j)*2, (qy+i)*2);
					SetSeaMode(1, (qx+j)*2+1, (qy+i)*2);
					SetSeaMode(1, (qx+j)*2, (qy+i)*2+1);
					SetSeaMode(1, (qx+j)*2+1, (qy+i)*2+1);
				}
				else
				{
					SetSeaMode(0, (qx+j)*2, (qy+i)*2);
					SetSeaMode(0, (qx+j)*2+1, (qy+i)*2);
					SetSeaMode(0, (qx+j)*2, (qy+i)*2+1);
					SetSeaMode(0, (qx+j)*2+1, (qy+i)*2+1);
				}
				
				PutSea((qx+j)*2, (qy+i)*2, mode, pushmode);

			}
			else
			{
				unsigned char _randfile=SeaType[ Dir8Q[number] ][(GD.Random()%MAX_SEA_VARY)*2];
	            unsigned char _randfont=SeaType[ Dir8Q[number] ][(GD.Random()%MAX_SEA_VARY)*2+1]; 

				SetSeaFile(_randfile, (qx+j), (qy+i));
	            SetSeaFont(_randfont, (qx+j), (qy+i));

				if(mode==TRUE)
				{
					// 주변의 언덕을 지운다. 
					if(pushmode==TRUE)
					{
					   MapEdit.MapEdit_PutLevel((qx+j)*2,     (qy+i)*2,   0, FALSE);
					   MapEdit.MapEdit_PutLevel((qx+j)*2+1,   (qy+i)*2,   0, FALSE);
					   MapEdit.MapEdit_PutLevel((qx+j)*2,     (qy+i)*2+1, 0, FALSE);
					   MapEdit.MapEdit_PutLevel((qx+j)*2+1,   (qy+i)*2+1, 0, FALSE);
					}

				    if(number&1)
					{
					   SetSeaMode(1, (qx+j)*2, (qy+i)*2);
					}
				    else 
					   SetSeaMode(0, (qx+j)*2, (qy+i)*2);
				   
				    if(number&2)
					{
				 	   SetSeaMode(1, (qx+j)*2+1, (qy+i)*2);
					}
				    else 
					   SetSeaMode(0, (qx+j)*2+1, (qy+i)*2);

				    if(number&4)
					{
					   SetSeaMode(1, (qx+j)*2, (qy+i)*2+1);
					}
				    else 
					   SetSeaMode(0, (qx+j)*2, (qy+i)*2+1);

				    if(number&8)
					{
					   SetSeaMode(1, (qx+j)*2+1, (qy+i)*2+1);
					}
				    else
					   SetSeaMode(0, (qx+j)*2+1, (qy+i)*2+1);
				}
				else
				{
				   if((number&1)==0)
					   SetSeaMode(0, (qx+j)*2, (qy+i)*2);
				   else 
					   SetSeaMode(1, (qx+j)*2, (qy+i)*2);

				   if((number&2)==0)
				 	   SetSeaMode(0, (qx+j)*2+1, (qy+i)*2);
				   else
					   SetSeaMode(1, (qx+j)*2+1, (qy+i)*2);

				   if((number&4)==0)
					   SetSeaMode(0, (qx+j)*2, (qy+i)*2+1);
				   else
					   SetSeaMode(1, (qx+j)*2, (qy+i)*2+1);
				   
				   if((number&8)==0)
					   SetSeaMode(0, (qx+j)*2+1, (qy+i)*2+1);
				   else
					   SetSeaMode(1, (qx+j)*2+1, (qy+i)*2+1);
				}
			}
		}

}

// 물은 만들지 않고 얕은 영역만 만든다. 
void _Map::PutShallow(SHORT x, SHORT y, SHORT mode)
{
	// 유효한 영역인지 확인한다. 
	if(x>=0 && x<MH.MapXsize && y>=0 && y<MH.MapYsize){}
	else return ;
	
	if( mode==TRUE )
	{
		if( pShallowMap )	pShallowMap[x+YOff[y]] = 1;
		if( pShallowFont )	pShallowFont[x+YOff[y]] = GD.Random()%4;
	}
	else
	{
		SetSeaMode(0, x, y);
		if( pShallowMap )	pShallowMap[x+YOff[y]] = 0;
	}
}


// 물과 얕은 영역을 동시에 만든다. 
// sx, sy: 2배수 좌표 
void _Map::PutShallowSea(SHORT sx, SHORT sy, SHORT mode)
{
	int number;
	int i, j;
	SHORT qx, qy;

	// 유효한 영역인지 확인한다. 
	if(sx>=0 && sx<MH.MapXsize && sy>=0 && sy<MH.MapYsize){}
	else return ;
	
	if(mode==TRUE)
	{
		// 꽉차있는 모습을 저장한다. 
		SetSeaMode(1, sx, sy);
		if( pShallowMap )	pShallowMap[sx + YOff[sy]] = 1;
		if( pShallowFont )	pShallowFont[sx + YOff[sy]] = GD.Random()%4;
	}
	else
	{
		SetSeaMode(0, sx, sy); 
		if( pShallowMap )	pShallowMap[sx + YOff[sy]] = 0;
	}
	
	qx=sx/2;
	qy=sy/2;

	for(i=-1;i<=1;i++)
		for(j=-1;j<=1;j++)
		{
			// 범위를 벗어난것도 제외된다. 
			if(qx+j>=0 && qx+j<MH.MapXsize/2 && qy+i>=0 && qy+i<MH.MapYsize/2){}
			else continue;

			if(mode==TRUE)
			{
		       number=GetSeaNear1(qx+j, qy+i);

			   if(GetSeaMode((qx+j)*2, (qy+i)*2)==1)
			   {
				   number|=1;
			   }
			   if(GetSeaMode((qx+j)*2+1, (qy+i)*2)==1)
			   {
				   number|=2;
			   }
			   if(GetSeaMode((qx+j)*2, (qy+i)*2+1)==1)
			   {
				   number|=4;
			   }
			   if(GetSeaMode((qx+j)*2+1, (qy+i)*2+1)==1)
			   {
				   number|=8;
			   }
			}
			else
			{
			   number=GetSeaNear2(qx+j, qy+i);

			   if(GetSeaMode((qx+j)*2, (qy+i)*2)==0)
			   {
				      number|=1;
			   }
			   if(GetSeaMode((qx+j)*2+1, (qy+i)*2)==0)
			   {
				      number|=2;
			   }
			   if(GetSeaMode((qx+j)*2, (qy+i)*2+1)==0)
			   {
				      number|=4;
			   }
			   if(GetSeaMode((qx+j)*2+1, (qy+i)*2+1)==0)
			   {
				     number|=8;
			   }

			   //Error("", "x:%d y:%d number:%d", qx+j, qy+i, number);			   

	           number=15-number;

			}

			// 주변에 전혀 없다. 
			if(Dir8Q[number]==-2)
			{
				SetSeaFile(0, (qx+j), (qy+i));
	            SetSeaFont(0, (qx+j), (qy+i));

				SetSeaMode(0, (qx+j)*2, (qy+i)*2);
				SetSeaMode(0, (qx+j)*2+1, (qy+i)*2);
				SetSeaMode(0, (qx+j)*2, (qy+i)*2+1);
				SetSeaMode(0, (qx+j)*2+1, (qy+i)*2+1);

			}
			// 적절한 세트가 없다. 
			else if(Dir8Q[number]==-1)
			{
				if(mode==TRUE)
				{
					SetSeaMode(1, (qx+j)*2, (qy+i)*2);
					SetSeaMode(1, (qx+j)*2+1, (qy+i)*2);
					SetSeaMode(1, (qx+j)*2, (qy+i)*2+1);
					SetSeaMode(1, (qx+j)*2+1, (qy+i)*2+1);
				}
				else
				{
					SetSeaMode(0, (qx+j)*2, (qy+i)*2);
					SetSeaMode(0, (qx+j)*2+1, (qy+i)*2);
					SetSeaMode(0, (qx+j)*2, (qy+i)*2+1);
					SetSeaMode(0, (qx+j)*2+1, (qy+i)*2+1);
				}
				
				PutShallowSea((qx+j)*2, (qy+i)*2, mode);

			}
			else
			{
				unsigned char _randfile=SeaType[ Dir8Q[number] ][(GD.Random()%MAX_SEA_VARY)*2];
	            unsigned char _randfont=SeaType[ Dir8Q[number] ][(GD.Random()%MAX_SEA_VARY)*2+1]; 

				SetSeaFile(_randfile, (qx+j), (qy+i));
	            SetSeaFont(_randfont, (qx+j), (qy+i));

				if(mode==TRUE)
				{
				   if(number&1)
					   SetSeaMode(1, (qx+j)*2, (qy+i)*2);
				   else 
					   SetSeaMode(0, (qx+j)*2, (qy+i)*2);
				   
				   if(number&2)
				 	   SetSeaMode(1, (qx+j)*2+1, (qy+i)*2);
				   else 
					   SetSeaMode(0, (qx+j)*2+1, (qy+i)*2);

				   if(number&4)
					   SetSeaMode(1, (qx+j)*2, (qy+i)*2+1);
				   else 
					   SetSeaMode(0, (qx+j)*2, (qy+i)*2+1);

				   if(number&8)
					   SetSeaMode(1, (qx+j)*2+1, (qy+i)*2+1);
				   else
					   SetSeaMode(0, (qx+j)*2+1, (qy+i)*2+1);
				}
				else
				{
				   if((number&1)==0)
					   SetSeaMode(0, (qx+j)*2, (qy+i)*2);
				   else 
					   SetSeaMode(1, (qx+j)*2, (qy+i)*2);

				   if((number&2)==0)
				 	   SetSeaMode(0, (qx+j)*2+1, (qy+i)*2);
				   else
					   SetSeaMode(1, (qx+j)*2+1, (qy+i)*2);

				   if((number&4)==0)
					   SetSeaMode(0, (qx+j)*2, (qy+i)*2+1);
				   else
					   SetSeaMode(1, (qx+j)*2, (qy+i)*2+1);
				   
				   if((number&8)==0)
					   SetSeaMode(0, (qx+j)*2+1, (qy+i)*2+1);
				   else
					   SetSeaMode(1, (qx+j)*2+1, (qy+i)*2+1);

				}
			}
		}

}


#endif