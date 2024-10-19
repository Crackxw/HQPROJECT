//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>
#include <graphics16.h>
#include <main.h>

#include "..\Gersang\maindata.h"

#include <myfile.h>
#include <map.h>

extern	_MainData		GD;
extern	XSPR			TileSpr[];			// 지도 폰트용 타일 
extern	SHORT			Dir8Q[];
extern	unsigned char	NewBlkType[][MAX_NEWBLK_VARY*2];


// sx, sy:4배수 좌표 
int _Map::GetNewBlkNear1(SHORT sx, SHORT sy)
{

	int number=0;
	SHORT x, y;

	// 남쪽 
	x=sx; y=sy+1;
	if(y<MH.MapYsize/2)
	{
		if(GetNewBlkMode(x*2, y*2))number|=4;
		if(GetNewBlkMode(x*2+1, y*2))number|=8;
	}


	// 서쪽 
	x=sx-1;
	y=sy;
	if(x>=0)
	{
		if(GetNewBlkMode(x*2+1, y*2))number|=1;
		if(GetNewBlkMode(x*2+1, y*2+1))number|=4;
	}


	// 북쪽 
	 x=sx;
	 y=sy-1;
	 if(y>=0)
	 {
		if(GetNewBlkMode(x*2, y*2+1))number|=1;
		if(GetNewBlkMode(x*2+1, y*2+1))number|=2;

	 }

	 // 동쪽 
	 x=sx+1;
	 y=sy;
	 if(x<MH.MapXsize/2)
	 {
		if(GetNewBlkMode(x*2, y*2))number|=2;
		if(GetNewBlkMode(x*2, y*2+1))number|=8;

	 }

	 // 남서쪽 
	 x=sx-1;
	 y=sy+1;
	 if(x>=0 && y<MH.MapYsize/2)
	 {
		if(GetNewBlkMode(x*2+1, y*2))number|=4;
	 }

	 // 북서쪽 
	 x=sx-1;
	 y=sy-1;
	 if(x>=0 && y>=0)
	 {
		if(GetNewBlkMode(x*2+1, y*2+1))number|=1;

	 }

	 // 북동쪽 
	 x=sx+1;
	 y=sy-1;
	 if(x<MH.MapXsize/2 && y>=0)
	 {
		if(GetNewBlkMode(x*2, y*2+1))number|=2;
	 }

	 // 남동쪽 
	 x=sx+1;
	 y=sy+1;
	 if(x<MH.MapXsize/2 && y<MH.MapYsize/2)
	 {
		if(GetNewBlkMode(x*2, y*2))number|=8;
	 }

	return number;

}

// sx, sy:4배수 좌표 
int _Map::GetNewBlkNear2(SHORT sx, SHORT sy)
{

	int number=0;
	SHORT x, y;

	// 남쪽 
	x=sx; y=sy+1;
	if(y<MH.MapYsize/2)
	{
		if(GetNewBlkMode(x*2, y*2)==0)number|=4;
		if(GetNewBlkMode(x*2+1, y*2)==0)number|=8;
	}


	// 서쪽 
	x=sx-1;
	y=sy;
	if(x>=0)
	{
		if(GetNewBlkMode(x*2+1, y*2)==0)number|=1;
		if(GetNewBlkMode(x*2+1, y*2+1)==0)number|=4;
	}


	// 북쪽 
	 x=sx;
	 y=sy-1;
	 if(y>=0)
	 {
		if(GetNewBlkMode(x*2, y*2+1)==0)number|=1;
		if(GetNewBlkMode(x*2+1, y*2+1)==0)number|=2;

	 }

	 // 동쪽 
	 x=sx+1;
	 y=sy;
	 if(x<MH.MapXsize/2)
	 {
		if(GetNewBlkMode(x*2, y*2)==0)number|=2;
		if(GetNewBlkMode(x*2, y*2+1)==0)number|=8;

	 }

	 // 남서쪽 
	 x=sx-1;
	 y=sy+1;
	 if(x>=0 && y<MH.MapYsize/2)
	 {
		if(GetNewBlkMode(x*2+1, y*2)==0)number|=4;
	 }

	 // 북서쪽 
	 x=sx-1;
	 y=sy-1;
	 if(x>=0 && y>=0)
	 {
		if(GetNewBlkMode(x*2+1, y*2+1)==0)number|=1;

	 }

	 // 북동쪽 
	 x=sx+1;
	 y=sy-1;
	 if(x<MH.MapXsize/2 && y>=0)
	 {
		if(GetNewBlkMode(x*2, y*2+1)==0)number|=2;
	 }

	 // 남동쪽 
	 x=sx+1;
	 y=sy+1;
	 if(x<MH.MapXsize/2 && y<MH.MapYsize/2)
	 {
		if(GetNewBlkMode(x*2, y*2)==0)number|=8;
	 }

			   

	return number;

}


// sx, sy: 2배수 좌표 
void _Map::PutNewBlk(SHORT sx, SHORT sy, SHORT mode)
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
		   SetNewBlkMode(1, sx, sy);
	   }
	   else
	   {
		   SetNewBlkMode(0, sx, sy);
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
		       number=GetNewBlkNear1(qx+j, qy+i);

			   if(GetNewBlkMode((qx+j)*2, (qy+i)*2)==1)
			   {
				   number|=1;
			   }
			   if(GetNewBlkMode((qx+j)*2+1, (qy+i)*2)==1)
			   {
				   number|=2;
			   }
			   if(GetNewBlkMode((qx+j)*2, (qy+i)*2+1)==1)
			   {
				   number|=4;
			   }
			   if(GetNewBlkMode((qx+j)*2+1, (qy+i)*2+1)==1)
			   {
				   number|=8;
			   }
			}
			else
			{
			   number=GetNewBlkNear2(qx+j, qy+i);

			   if(GetNewBlkMode((qx+j)*2, (qy+i)*2)==0)
			   {
				      number|=1;
			   }
			   if(GetNewBlkMode((qx+j)*2+1, (qy+i)*2)==0)
			   {
				      number|=2;
			   }
			   if(GetNewBlkMode((qx+j)*2, (qy+i)*2+1)==0)
			   {
				      number|=4;
			   }
			   if(GetNewBlkMode((qx+j)*2+1, (qy+i)*2+1)==0)
			   {
				     number|=8;
			   }

			   //Error("", "x:%d y:%d number:%d", qx+j, qy+i, number);			   

	           number=15-number;

			}

			// 주변에 전혀 없다. 
			if(Dir8Q[number]==-2)
			{
				SetNewBlkFile(0, (qx+j), (qy+i));
	            SetNewBlkFont(0, (qx+j), (qy+i));

				SetNewBlkMode(0, (qx+j)*2,  (qy+i)*2);
				SetNewBlkMode(0, (qx+j)*2+1, (qy+i)*2);
				SetNewBlkMode(0, (qx+j)*2,   (qy+i)*2+1);
				SetNewBlkMode(0, (qx+j)*2+1, (qy+i)*2+1);

			}
			// 적절한 세트가 없다. 
			else if(Dir8Q[number]==-1)
			{
				if(mode==TRUE)
				{
					SetNewBlkMode(1, (qx+j)*2,  (qy+i)*2);
					SetNewBlkMode(1, (qx+j)*2+1, (qy+i)*2);
					SetNewBlkMode(1, (qx+j)*2,  (qy+i)*2+1);
					SetNewBlkMode(1, (qx+j)*2+1,  (qy+i)*2+1);
				}
				else
				{
					SetNewBlkMode(0, (qx+j)*2,  (qy+i)*2);
					SetNewBlkMode(0, (qx+j)*2+1, (qy+i)*2);
					SetNewBlkMode(0, (qx+j)*2,  (qy+i)*2+1);
					SetNewBlkMode(0, (qx+j)*2+1, (qy+i)*2+1);
				}
				
				PutNewBlk((qx+j)*2, (qy+i)*2, mode);

			}
			else
			{
				unsigned char _randfile=NewBlkType[ Dir8Q[number] ][(GD.Random()%MAX_NEWBLK_VARY)*2];
	            unsigned char _randfont=NewBlkType[ Dir8Q[number] ][(GD.Random()%MAX_NEWBLK_VARY)*2+1]; 

				SetNewBlkFile(_randfile, (qx+j),(qy+i));
	            SetNewBlkFont(_randfont, (qx+j),(qy+i));

				if(mode==TRUE)
				{
				   if(number&1)
					   SetNewBlkMode(1, (qx+j)*2, (qy+i)*2);
				   else 
					   SetNewBlkMode(0, (qx+j)*2,  (qy+i)*2);
				   
				   if(number&2)
				 	   SetNewBlkMode(1, (qx+j)*2+1, (qy+i)*2);
				   else 
					   SetNewBlkMode(0, (qx+j)*2+1,	(qy+i)*2);

				   if(number&4)
					   SetNewBlkMode(1, (qx+j)*2,(qy+i)*2+1);
				   else 
					   SetNewBlkMode(0, (qx+j)*2,(qy+i)*2+1);


				   if(number&8)
					   SetNewBlkMode(1, (qx+j)*2+1, (qy+i)*2+1);
				   else
					   SetNewBlkMode(0, (qx+j)*2+1, (qy+i)*2+1);
				}
				else
				{
				   if((number&1)==0)
					   SetNewBlkMode(0, (qx+j)*2,(qy+i)*2);
				   else 
					   SetNewBlkMode(1, (qx+j)*2,(qy+i)*2);

				   if((number&2)==0)
				 	   SetNewBlkMode(0, (qx+j)*2+1, (qy+i)*2);
				   else
					   SetNewBlkMode(1, (qx+j)*2+1, (qy+i)*2);

				   if((number&4)==0)
					   SetNewBlkMode(0, (qx+j)*2, (qy+i)*2+1);
				   else
					   SetNewBlkMode(1, (qx+j)*2, (qy+i)*2+1);
				   
				   if((number&8)==0)
					   SetNewBlkMode(0, (qx+j)*2+1, (qy+i)*2+1);
				   else
					   SetNewBlkMode(1, (qx+j)*2+1, (qy+i)*2+1);

				}


			}
		}

}

// x, y 4배수 좌표 
SHORT _Map::GetNewBlkGateMode(SHORT sx, SHORT sy)
{
	// 유효한 영역인지 확인한다. 
	if(sx>=0 && sx<MH.MapXsize/2 && sy>=0 && sy<MH.MapYsize/2){}
	else return 0;

	SHORT number=0;
	if(GetNewBlkMode(sx*2,sy*2) == 1)
	{
	   number|=1;
	}
	if(GetNewBlkMode(sx*2+1,sy*2)==1)
	{
	   number|=2;
	}
	if(GetNewBlkMode(sx*2,sy*2+1)==1)
	{
	   number|=4;
	}
	if(GetNewBlkMode(sx*2+1,sy*2+1)==1)
	{
 	   number|=8;
    }

	return number;

}

// x, y 4배수 좌표 
void _Map::PutNewBlkGate(SHORT sx, SHORT sy)
{
	// 유효한 영역인지 확인한다. 
	if(sx>=0 && sx<MH.MapXsize/2 && sy>=0 && sy<MH.MapYsize/2){}
	else return ;


	SHORT number=0;
	if(GetNewBlkMode(sx*2,sy*2) == 1)
	{
	   number|=1;
	}
	if(GetNewBlkMode(sx*2+1,sy*2)==1)
	{
	   number|=2;
	}
	if(GetNewBlkMode(sx*2,sy*2+1)==1)
	{
	   number|=4;
	}
	if(GetNewBlkMode(sx*2+1,sy*2+1)==1)
	{
 	   number|=8;
    }

	// 남쪽 문 
	if(number==3)
	{
		SetNewBlkFile(FileTileNewBlkGate, sx,sy);
	    SetNewBlkFont(0, sx,sy);
	}
	// 동쪽 문 
	if(number==5)
	{
		SetNewBlkFile(FileTileNewBlkGate, sx,sy);
	    SetNewBlkFont(4, sx,sy);
	}
	// 서쪽 문 
	if(number==10)
	{
		SetNewBlkFile(FileTileNewBlkGate, sx,sy);
	    SetNewBlkFont(8, sx,sy);
	}
	// 북쪽 문 
	if(number==12)
	{
		SetNewBlkFile(FileTileNewBlkGate, sx,sy);
	    SetNewBlkFont(12, sx,sy);
	}

}

// 주변의 상황을 고려하여 충돌영역을 설정한다. 
void _Map::PutCollNewBlk(SHORT x, SHORT y)
{

	SHORT dir=0;
	SHORT file=GetNewBlkFile(x/2, y/2);
	SHORT font=GetNewBlkFont(x/2, y/2);
	SHORT _font;

   	_font=font+(x%2)+(y%2)*2;

	if(TileSpr[file].Header.Coll[_font])
	{
	   SetCollMap(x,   y,   BLOCK_NEWBLK);
    }
	// 장애물에 의한 높이를 설정한다. 
	if(TileSpr[file].Header.Coll[_font]==0)
	{
		if(file == FileTileNewBlkGate)
		{
			SetNewBlkLevel(1, x,y);
		}
		else
			SetNewBlkLevel(2, x,y);
	}
}

