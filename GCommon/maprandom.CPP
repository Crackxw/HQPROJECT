#ifndef _ROCKET_NEW_TILE_MAP

//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>
#include <main.h>
#include "..\Gersang\maindata.h"
#include <map.h>

extern	_MainData	GD;


// 물을 랜덤하게 찍는다. 
// centerx, centery: 4배수 좌표 
// randompercent: 넓게 퍼지는 정도 
void _Map::SetSeaRandom(SHORT centerx, SHORT centery, SHORT range, int randompercent)
{

	int i, j;
	SHORT temp;

	temp=(range*randompercent)/100;
	if(temp==0)temp=1;

	// 물을찍을 지점을 선택한다. 

	int divider;
	for(i=-range;i<=range;i++)
		for(j=-range;j<=range;j++)
		{
			divider=(abs(j)+abs(i))/temp;
			if(divider==0)divider=1;

		    switch(GD.Random()%divider)
			{ 
			    case 0:
			       PutSea(centerx+j, centery+i, TRUE, TRUE);
			       break;
			}
		}

}

// centerx, centery: 4배수 좌표 
// randompercent: 넓게 퍼지는 정도 
void _Map::SetNewBlkRandom(SHORT centerx, SHORT centery, SHORT range, int randompercent)
{

	int i, j;
	SHORT temp;

	temp=(range*randompercent)/100;
	if(temp==0)temp=1;

	int divider;
	for(i=-range;i<=range;i++)
		for(j=-range;j<=range;j++)
		{
			divider=(abs(j)+abs(i))/temp;
			if(divider==0)divider=1;

		    switch(GD.Random()%divider)
			{ 
			    case 0:
			       PutNewBlk(centerx+j, centery+i, TRUE);
			       break;
			}
		}

}


// 얕은물을 랜덤하게 찍는다. 
// randompercent: 넓게 퍼지는 정도 
// centerx, centery:4배수 좌표 
void _Map::SetShallowWaterRandom(SHORT centerx, SHORT centery, SHORT range, int randompercent)
{

	int i, j;
	SHORT temp;

	temp=(range*randompercent)/100;
	if(temp==0)temp=1;

	// 물을찍을 지점을 선택한다. 

	int divider;
	for(i=-range;i<=range;i++)
		for(j=-range;j<=range;j++)
		{
			divider=(abs(j)+abs(i))/temp;
			if(divider==0)divider=1;

		    switch(GD.Random()%divider)
			{ 
			    case 0:
			       PutShallowSea(centerx+j, centery+i, TRUE);
  				      break;
			}
		}
}


// 이종타일을 랜덤하게 찍는다. 
void _Map::SetGreenRandom(SHORT centerx, SHORT centery, SHORT range, int randompercent)
{

	int i, j;//, k, l;
	SHORT temp;
	temp=(range*randompercent)/100;
	if(temp==0)temp=1;

	// 물을찍을 지점을 선택한다. 

	int divider;
	for(i=-range;i<=range;i++)
		for(j=-range;j<=range;j++)
		{
			divider=(abs(j)+abs(i))/temp;
			if(divider==0)divider=1;

			switch(GD.Random()%divider)
			{
			    case 0:
					PutTile(centerx+j, centery+i, TILE_GREEN);
				   break;
			}
		}
}

// 이종타일을 랜덤하게 찍는다. 
void _Map::SetGrassRandom(SHORT centerx, SHORT centery, SHORT range, int randompercent)
{

	int i, j;//, k, l;
	SHORT temp;
	temp=(range*randompercent)/100;
	if(temp==0)temp=1;

	// 물을찍을 지점을 선택한다. 

	int divider;
	for(i=-range;i<=range;i++)
		for(j=-range;j<=range;j++)
		{
			divider=(abs(j)+abs(i))/temp;
			if(divider==0)divider=1;

			switch(GD.Random()%divider)
			{
			    case 0:
					PutTile(centerx+j, centery+i, TILE_GRASS);
				   break;
			}
		}
}


// 곡물을 랜덤하게 찍는다. 
void _Map::SetCropRandom(SHORT centerx, SHORT centery, SHORT range, int randompercent, SHORT type)
{

	int i, j;//, k, l;
	SHORT temp;
	
	if(range<0)range=0;
	if(range>10)range=10;

	temp=(range*randompercent)/100;
	if(temp==0)temp=1;


	// 을 지점을 선택한다. 

	int divider;
	for(i=-range;i<=range;i++)
		for(j=-range;j<=range;j++)
		{
			divider=(abs(j)+abs(i))/temp;
			if(divider==0)divider=1;

			switch(GD.Random()%divider)
			{
			    case 0:
					if(type==CROP_TYPE_TREE)
					{
			           PutTree(centerx+j, centery+i, type);
					}
					else if(type==CROP_TYPE_BAMBOO)
					{
			           PutTree(centerx+j, centery+i, type);
					}
					else
					{
					   PutTile(centerx+j, centery+i, TILE_CROPAREA);
				       PutCrop(centerx+j, centery+i, type);
					}
				   break;
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
#include <map.h>

extern	_MainData	GD;


// 물을 랜덤하게 찍는다. 
// centerx, centery: 4배수 좌표 
// randompercent: 넓게 퍼지는 정도 
void _Map::SetSeaRandom(SHORT centerx, SHORT centery, SHORT range, int randompercent)
{

	int i, j;
	SHORT temp;

	temp=(range*randompercent)/100;
	if(temp==0)temp=1;

	// 물을찍을 지점을 선택한다. 

	int divider;
	for(i=-range;i<=range;i++)
		for(j=-range;j<=range;j++)
		{
			divider=(abs(j)+abs(i))/temp;
			if(divider==0)divider=1;

		    switch(GD.Random()%divider)
			{ 
			    case 0:
			        //PutSea(centerx+j, centery+i, TRUE, TRUE);
					PutSea_Daesang(centerx+j, centery+i, TRUE );
			       break;
			}
		}

}

// centerx, centery: 4배수 좌표 
// randompercent: 넓게 퍼지는 정도 
void _Map::SetNewBlkRandom(SHORT centerx, SHORT centery, SHORT range, int randompercent)
{

	int i, j;
	SHORT temp;

	temp=(range*randompercent)/100;
	if(temp==0)temp=1;

	int divider;
	for(i=-range;i<=range;i++)
		for(j=-range;j<=range;j++)
		{
			divider=(abs(j)+abs(i))/temp;
			if(divider==0)divider=1;

		    switch(GD.Random()%divider)
			{ 
			    case 0:
			       PutNewBlk(centerx+j, centery+i, TRUE);
			       break;
			}
		}

}


// 얕은물을 랜덤하게 찍는다. 
// randompercent: 넓게 퍼지는 정도 
// centerx, centery:4배수 좌표 
void _Map::SetShallowWaterRandom(SHORT centerx, SHORT centery, SHORT range, int randompercent)
{

	int i, j;
	SHORT temp;

	temp=(range*randompercent)/100;
	if(temp==0)temp=1;

	// 물을찍을 지점을 선택한다. 

	int divider;
	for(i=-range;i<=range;i++)
		for(j=-range;j<=range;j++)
		{
			divider=(abs(j)+abs(i))/temp;
			if(divider==0)divider=1;

		    switch(GD.Random()%divider)
			{ 
			    case 0:
			       PutShallowSea(centerx+j, centery+i, TRUE);
  				      break;
			}
		}
}


// 이종타일을 랜덤하게 찍는다. 
void _Map::SetGreenRandom(SHORT centerx, SHORT centery, SHORT range, int randompercent)
{

	int i, j;//, k, l;
	SHORT temp;
	temp=(range*randompercent)/100;
	if(temp==0)temp=1;

	// 물을찍을 지점을 선택한다. 

	int divider;
	for(i=-range;i<=range;i++)
		for(j=-range;j<=range;j++)
		{
			divider=(abs(j)+abs(i))/temp;
			if(divider==0)divider=1;

			switch(GD.Random()%divider)
			{
			    case 0:
					PutTile(centerx+j, centery+i, TILE_GREEN);
				   break;
			}
		}
}

// 이종타일을 랜덤하게 찍는다. 
void _Map::SetGrassRandom(SHORT centerx, SHORT centery, SHORT range, int randompercent)
{

	int i, j;//, k, l;
	SHORT temp;
	temp=(range*randompercent)/100;
	if(temp==0)temp=1;

	// 물을찍을 지점을 선택한다. 

	int divider;
	for(i=-range;i<=range;i++)
		for(j=-range;j<=range;j++)
		{
			divider=(abs(j)+abs(i))/temp;
			if(divider==0)divider=1;

			switch(GD.Random()%divider)
			{
			    case 0:
					PutTile(centerx+j, centery+i, TILE_GRASS);
				   break;
			}
		}
}


// 곡물을 랜덤하게 찍는다. 
//void _Map::SetCropRandom(SHORT centerx, SHORT centery, SHORT range, int randompercent, SHORT type)
void _Map::SetCropRandom(SHORT centerx, SHORT centery, SHORT range, int randompercent, SHORT type, SHORT TreeType)
{

	int i, j;//, k, l;
	SHORT temp;
	
	if(range<0)range=0;
	if(range>10)range=10;

	temp=(range*randompercent)/100;
	if(temp==0)temp=1;


	// 을 지점을 선택한다. 

	int divider;
	for(i=-range;i<=range;i++)
		for(j=-range;j<=range;j++)
		{
			divider=(abs(j)+abs(i))/temp;
			if(divider==0)divider=1;

			switch(GD.Random()%divider)
			{
			    case 0:
					if(type==CROP_TYPE_TREE)
					{
			           //PutTree(centerx+j, centery+i, type);
						PutTree(centerx+j, centery+i, type, TreeType);
					}
					else if(type==CROP_TYPE_BAMBOO)
					{
			           //PutTree(centerx+j, centery+i, type);
						PutTree(centerx+j, centery+i, type, -1);
					}
					else
					{
					   PutTile(centerx+j, centery+i, TILE_CROPAREA);
				       //PutCrop(centerx+j, centery+i, type);
					    PutCrop(centerx+j, centery+i, type, -1);
					}
				   break;
			}
		}
}


#endif