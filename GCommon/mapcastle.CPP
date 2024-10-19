//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
//////////////////////////////////////////////////////
// 성곽 관련 함수
/////////////////////////////////////////////////////

#include <GSL.h>
#include <main.h>

#include "..\Gersang\maindata.h"


#include <myfile.h>
#include <map.h>

extern	XSPR			TileSpr[];				// 지도 폰트용 타일 
extern	_MainData		GD;

extern	_Tile			Tile[];
extern	_Castle			Castle[];

extern	SHORT			Dir8Q[];
extern	unsigned char	CastleType[][MAX_CASTLE_VARY*2];



// 주변의 상황을 고려하여 충돌영역을 설정한다. `
void _Map::PutCollCastle(SHORT x, SHORT y)
{

	SHORT dir=0;
	SHORT file=GetCastleFile(x/2, y/2);
	SHORT font=GetCastleFont(x/2, y/2);
	SHORT _font;

   	_font=font+(x%2)+(y%2)*2;

	if(TileSpr[file].Header.Coll[_font]==1)
	{
	   SetCollMap(x,   y,   BLOCK_NORMAL);
    }
	// 장애물에 의한 높이를 설정한다. 
	if(TileSpr[file].Header.Coll[_font]==0)
	{
		// 성문은 1층으로 설정한다. 
		if(file == FileTileCastle2 && _font>=12)
		{
			SetNewBlkLevel(1, x, y);
		}
		// 성위의 평지는 3층이다. 
		else
		{
			SetNewBlkLevel(3, x, y);
		}
	}



}



// sx, sy:4배수 좌표 
int _Map::GetCastleNear1(SHORT sx, SHORT sy)
{

	int number=0;
	SHORT x, y;

	// 남쪽 
	x=sx; y=sy+1;
	if(y<MH.MapYsize/2)
	{
		if(GetCastleMode(x*2, y*2))number|=4;
		if(GetCastleMode(x*2+1, y*2))number|=8;
	}


	// 서쪽 
	x=sx-1;
	y=sy;
	if(x>=0)
	{
		if(GetCastleMode(x*2+1, y*2))number|=1;
		if(GetCastleMode(x*2+1, y*2+1))number|=4;
	}


	// 북쪽 
	 x=sx;
	 y=sy-1;
	 if(y>=0)
	 {
		if(GetCastleMode(x*2, y*2+1))number|=1;
		if(GetCastleMode(x*2+1, y*2+1))number|=2;

	 }

	 // 동쪽 
	 x=sx+1;
	 y=sy;
	 if(x<MH.MapXsize/2)
	 {
		if(GetCastleMode(x*2, y*2))number|=2;
		if(GetCastleMode(x*2, y*2+1))number|=8;

	 }

	 // 남서쪽 
	 x=sx-1;
	 y=sy+1;
	 if(x>=0 && y<MH.MapYsize/2)
	 {
		if(GetCastleMode(x*2+1, y*2))number|=4;
	 }

	 // 북서쪽 
	 x=sx-1;
	 y=sy-1;
	 if(x>=0 && y>=0)
	 {
		if(GetCastleMode(x*2+1, y*2+1))number|=1;

	 }

	 // 북동쪽 
	 x=sx+1;
	 y=sy-1;
	 if(x<MH.MapXsize/2 && y>=0)
	 {
		if(GetCastleMode(x*2, y*2+1))number|=2;
	 }

	 // 남동쪽 
	 x=sx+1;
	 y=sy+1;
	 if(x<MH.MapXsize/2 && y<MH.MapYsize/2)
	 {
		if(GetCastleMode(x*2, y*2))number|=8;
	 }

	return number;

}

// sx, sy:4배수 좌표 
int _Map::GetCastleNear2(SHORT sx, SHORT sy)
{

	int number=0;
	SHORT x, y;

	// 남쪽 
	x=sx; y=sy+1;
	if(y<MH.MapYsize/2)
	{
		if(GetCastleMode(x*2, y*2)==0)number|=4;
		if(GetCastleMode(x*2+1, y*2)==0)number|=8;
	}


	// 서쪽 
	x=sx-1;
	y=sy;
	if(x>=0)
	{
		if(GetCastleMode(x*2+1, y*2)==0)number|=1;
		if(GetCastleMode(x*2+1, y*2+1)==0)number|=4;
	}


	// 북쪽 
	 x=sx;
	 y=sy-1;
	 if(y>=0)
	 {
		if(GetCastleMode(x*2, y*2+1)==0)number|=1;
		if(GetCastleMode(x*2+1, y*2+1)==0)number|=2;

	 }

	 // 동쪽 
	 x=sx+1;
	 y=sy;
	 if(x<MH.MapXsize/2)
	 {
		if(GetCastleMode(x*2, y*2)==0)number|=2;
		if(GetCastleMode(x*2, y*2+1)==0)number|=8;

	 }

	 // 남서쪽 
	 x=sx-1;
	 y=sy+1;
	 if(x>=0 && y<MH.MapYsize/2)
	 {
		if(GetCastleMode(x*2+1, y*2)==0)number|=4;
	 }

	 // 북서쪽 
	 x=sx-1;
	 y=sy-1;
	 if(x>=0 && y>=0)
	 {
		if(GetCastleMode(x*2+1, y*2+1)==0)number|=1;

	 }

	 // 북동쪽 
	 x=sx+1;
	 y=sy-1;
	 if(x<MH.MapXsize/2 && y>=0)
	 {
		if(GetCastleMode(x*2, y*2+1)==0)number|=2;
	 }

	 // 남동쪽 
	 x=sx+1;
	 y=sy+1;
	 if(x<MH.MapXsize/2 && y<MH.MapYsize/2)
	 {
		if(GetCastleMode(x*2, y*2)==0)number|=8;
	 }

	return number;

}


// sx, sy: 2배수 좌표 
void _Map::PutCastle(SHORT sx, SHORT sy, SHORT mode)
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
		   SetCastleMode(1, sx, sy);
	   }
	   else
	   {
		   SetCastleMode(0, sx, sy);
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
		       number=GetCastleNear1(qx+j, qy+i);

			   if(GetCastleMode((qx+j)*2, (qy+i)*2)==1)
			   {
				   number|=1;
			   }
			   if(GetCastleMode((qx+j)*2+1, (qy+i)*2)==1)
			   {
				   number|=2;
			   }
			   if(GetCastleMode((qx+j)*2, (qy+i)*2+1)==1)
			   {
				   number|=4;
			   }
			   if(GetCastleMode((qx+j)*2+1, (qy+i)*2+1)==1)
			   {
				   number|=8;
			   }
			}
			else
			{
			   number=GetCastleNear2(qx+j, qy+i);

			   if(GetCastleMode((qx+j)*2, (qy+i)*2)==0)
			   {
				      number|=1;
			   }
			   if(GetCastleMode((qx+j)*2+1, (qy+i)*2)==0)
			   {
				      number|=2;
			   }
			   if(GetCastleMode((qx+j)*2, (qy+i)*2+1)==0)
			   {
				      number|=4;
			   }
			   if(GetCastleMode((qx+j)*2+1, (qy+i)*2+1)==0)
			   {
				     number|=8;
			   }

			   //Error("", "x:%d y:%d number:%d", qx+j, qy+i, number);			   

	           number=15-number;

			}



			// 주변에 전혀 없다. 
			if(Dir8Q[number]==-2)
			{
				SetCastleFile(0, (qx+j), (qy+i));
	            SetCastleFont(0, (qx+j), (qy+i));

				SetCastleMode(0, (qx+j)*2, (qy+i)*2);
				SetCastleMode(0, (qx+j)*2+1, (qy+i)*2);
				SetCastleMode(0, (qx+j)*2, (qy+i)*2+1);
				SetCastleMode(0, (qx+j)*2+1, (qy+i)*2+1);

			}
			// 적절한 세트가 없다. 
			else if(Dir8Q[number]==-1)
			{
				if(mode==TRUE)
				{
					SetCastleMode(1, (qx+j)*2, (qy+i)*2);
					SetCastleMode(1, (qx+j)*2+1, (qy+i)*2);
					SetCastleMode(1, (qx+j)*2, (qy+i)*2+1);
					SetCastleMode(1, (qx+j)*2+1, (qy+i)*2+1);
				}
				else
				{
					SetCastleMode(0, (qx+j)*2, (qy+i)*2);
					SetCastleMode(0, (qx+j)*2+1, (qy+i)*2);
					SetCastleMode(0, (qx+j)*2, (qy+i)*2+1);
					SetCastleMode(0, (qx+j)*2+1, (qy+i)*2+1);
				}
				
				PutCastle((qx+j)*2, (qy+i)*2, mode);

			}
			else
			{
				unsigned char _randfile=CastleType[ Dir8Q[number] ][(GD.Random()%MAX_CASTLE_VARY)*2];
	            unsigned char _randfont=CastleType[ Dir8Q[number] ][(GD.Random()%MAX_CASTLE_VARY)*2+1]; 

				SetCastleFile(_randfile, (qx+j), (qy+i));
	            SetCastleFont(_randfont, (qx+j), (qy+i));

				if(mode==TRUE)
				{
				   if(number&1)
					   SetCastleMode(1, (qx+j)*2, (qy+i)*2);
				   else 
					   SetCastleMode(0, (qx+j)*2, (qy+i)*2);
				   
				   if(number&2)
				 	   SetCastleMode(1, (qx+j)*2+1, (qy+i)*2);
				   else 
					   SetCastleMode(0, (qx+j)*2+1, (qy+i)*2);

				   if(number&4)
					   SetCastleMode(1, (qx+j)*2, (qy+i)*2+1);
				   else 
					   SetCastleMode(0, (qx+j)*2, (qy+i)*2+1);


				   if(number&8)
					   SetCastleMode(1, (qx+j)*2+1, (qy+i)*2+1);
				   else
					   SetCastleMode(0, (qx+j)*2+1, (qy+i)*2+1);
				}
				else
				{
				   if((number&1)==0)
					   SetCastleMode(0, (qx+j)*2, (qy+i)*2);
				   else 
					   SetCastleMode(1, (qx+j)*2, (qy+i)*2);

				   if((number&2)==0)
				 	   SetCastleMode(0, (qx+j)*2+1, (qy+i)*2);
				   else
					   SetCastleMode(1, (qx+j)*2+1, (qy+i)*2);

				   if((number&4)==0)
					   SetCastleMode(0, (qx+j)*2, (qy+i)*2+1);
				   else
					   SetCastleMode(1, (qx+j)*2, (qy+i)*2+1);
				   
				   if((number&8)==0)
					   SetCastleMode(0, (qx+j)*2+1, (qy+i)*2+1);
				   else
					   SetCastleMode(1, (qx+j)*2+1, (qy+i)*2+1);

				}
			}
		}
	
}



void _Map::PutCastleGate(SHORT sx, SHORT sy, SHORT dir, SHORT mode)
{
	SHORT qx, qy;

	// 유효한 영역인지 확인한다. 
	if(sx>=0 && sx<MH.MapXsize && sy>=0 && sy<MH.MapYsize){}
	else return ;

	   if(mode==TRUE)
	   {
		   // 꽉차있는 모습을 저장한다. 
		   SetCastleMode(1, sx, sy);
	   }
	   else
	   {
		   SetCastleMode(0, sx, sy);
	   }


	   qx=sx/2;
	   qy=sy/2;

	   if(dir==0)
	   {
	      SetCastleFile(FileTileCastle2, qx, qy);
          SetCastleFont(12, qx, qy);

	      SetCastleMode(1, sx, sy);
	      SetCastleMode(1, sx+1, sy);
	      SetCastleMode(1, sx, sy+1);
	      SetCastleMode(1, sx+1, sy+1);


	      SetCastleFile(FileTileCastle2, qx+1, qy);
          SetCastleFont(16, qx+1, qy);

	      SetCastleMode(1, sx+2, sy);
	      SetCastleMode(1, sx+2+1, sy);
	      SetCastleMode(1, sx+2, sy+1);
	      SetCastleMode(1, sx+2+1, sy+1);

	      SetCastleFile(FileTileCastle2, qx, qy+1);
          SetCastleFont(20, qx, qy+1);

	      SetCastleMode(1, sx, sy+2);
	      SetCastleMode(1, sx+1, sy+2);
	      SetCastleMode(1, sx, sy+2+1);
	      SetCastleMode(1, sx+1, sy+2+1);

	      SetCastleFile(FileTileCastle2, qx+1, qy+1);
          SetCastleFont(24, qx+1, qy+1);

	      SetCastleMode(1, sx+2, sy+2);
	      SetCastleMode(1, sx+2+1, sy+2);
	      SetCastleMode(1, sx+2, sy+2+1);
	      SetCastleMode(1, sx+2+1, sy+2+1);
	   }
	   else
	   {
	      SetCastleFile(FileTileCastle2, qx, qy);
          SetCastleFont(28, qx, qy);

	      SetCastleMode(1, sx, sy);
	      SetCastleMode(1, sx+1, sy);
	      SetCastleMode(1, sx, sy+1);
	      SetCastleMode(1, sx+1, sy+1);

	      SetCastleFile(FileTileCastle2, qx+1, qy);
          SetCastleFont(32, qx+1, qy);

	      SetCastleMode(1, sx+2, sy);
	      SetCastleMode(1, sx+2+1, sy);
	      SetCastleMode(1, sx+2, sy+1);
	      SetCastleMode(1, sx+2+1, sy+1);

	      SetCastleFile(FileTileCastle2, qx, qy+1);
          SetCastleFont(36, qx, qy+1);

	      SetCastleMode(1, sx, sy+2);
	      SetCastleMode(1, sx+1, sy+2);
	      SetCastleMode(1, sx, sy+2+1);
	      SetCastleMode(1, sx+1, sy+2+1);

	      SetCastleFile(FileTileCastle2, qx+1, qy+1);
          SetCastleFont(40, qx+1, qy+1);

	      SetCastleMode(1, sx+2, sy+2);
	      SetCastleMode(1, sx+2+1, sy+2);
	      SetCastleMode(1, sx+2, sy+2+1);
	      SetCastleMode(1, sx+2+1, sy+2+1);


	   }	
}
