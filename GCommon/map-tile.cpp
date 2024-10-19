#ifndef _ROCKET_NEW_TILE_MAP
//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>
#include <main.h>

#include "..\Gersang\maindata.h"


#include <myfile.h>
#include <map.h>

extern _MainData GD;

extern int TileSet[];
extern int BlockSet[];
extern SHORT Dir8Q[];
extern unsigned char NormalHillType[][MAX_TILE_VARY];
extern unsigned char GrassType[][15][MAX_GRASS_VARY];
extern unsigned char GreenType[][15][MAX_GREEN_VARY];
extern unsigned char CropAreaType[][15][MAX_CROPAREA_VARY];
extern unsigned char SeaType[][MAX_SEA_VARY*2];

_Tile Tile[MAX_TILE_TYPE_NUMBER];

//--------------------------------------------------------------------------
// Name: InitTile()
// Desc: 타일 배열을 초기화한다.
//--------------------------------------------------------------------------
void _Map::InitTile()
{
	int i;

    // 언덕 타일의 최대 종류 수 까지 반복한다.
	for(i = 0;i < MAX_TILE_TYPE_NUMBER;i++)
	{
	   Tile[i].X =   0;

	   if(i < 1)
	       Tile[i].Y = 0;
	   else
		   Tile[i].Y = 9;
	}
}

void _Map::SetProperFileNFont(SHORT x, SHORT y, int hilltype)
{
	int number;
	int neartype=0;
	SHORT i, j;
	
	if( pTileAtbMap == NULL ) return;

	// 주변의 타일 종류를 파악한다. 
	if( pTileAtbMap[x+YOff[y]] == TILE_GREEN )
	{
		if( pLevelFile )	pLevelFile[x+YOff[y]] = FileTileHill1 + hilltype;
		if( pLevelFont )	pLevelFont[x+YOff[y]] = GreenType[hilltype][14][GD.Random()%MAX_GRASS_VARY];
		if( pLevelType )	pLevelType[x+YOff[y]] = hilltype;
	}
	// 주변의 타일 종류를 파악한다. 
	else if( pTileAtbMap[x+YOff[y]] == TILE_GRASS )
	{
		if( pLevelFile )	pLevelFile[x+YOff[y]] = FileTileGrss1 + hilltype;
		if( pLevelFont )	pLevelFont[x+YOff[y]] = GrassType[hilltype][14][GD.Random()%MAX_GRASS_VARY];
		if( pLevelType )	pLevelType[x+YOff[y]] = hilltype;
	}
	else if( pTileAtbMap[x+YOff[y]] == TILE_CROPAREA )
	{
		if( pLevelFile )	pLevelFile[x+YOff[y]] = FileTileDiff1 + hilltype;
		if( pLevelFont )	pLevelFont[x+YOff[y]] = CropAreaType[hilltype][14][GD.Random()%MAX_CROPAREA_VARY];
		if( pLevelType )	pLevelType[x+YOff[y]] = hilltype;
	}
	else
	{
		for(i=-1;i<=1;i++)
			for(j=-1;j<=1;j++)
			{
				if(0<=x+j && x+j<MH.MapXsize && 0<=y+i && y+i<MH.MapYsize){}
				else continue;
				if(i==0 && j==0 )continue;

				if( pTileAtbMap[(x+j) + YOff[y+i]] == TILE_GREEN )
				{
					neartype=TILE_GREEN;
				}
				else if( pTileAtbMap[(x+j) + YOff[y+i]] == TILE_GRASS )
				{
					neartype=TILE_GRASS;
				}
				else if( pTileAtbMap[(x+j) + YOff[y+i]] == TILE_CROPAREA )
				{
					neartype=TILE_CROPAREA;
				}

			}


		// 4방향의 이종타일의 형태를 구한다 
		if(neartype == TILE_GREEN)
		{
		    number=0;
			// 남쪽 
			if(y+1<MH.MapYsize)
			{
				if( pTileAtbMap[x + YOff[y+1]] != TILE_DEFAULT )
				{
				    number|=MAPTILE_SOUTH;
				}
			}
			// 서쪽 
			if(x-1>=0)
			{
				if( pTileAtbMap[(x-1) + YOff[y]] != TILE_DEFAULT )
				{
					number|=MAPTILE_WEST;
				}
			}
			// 북쪽 
			if(y-1>=0)
			{
				if( pTileAtbMap[x + YOff[y-1]] != TILE_DEFAULT )
				{
				    number|=MAPTILE_NORTH;
				}
			}
			// 동쪽 
			if(x+1<MH.MapXsize)
			{
				if( pTileAtbMap[(x+1) + YOff[y]] != TILE_DEFAULT )
				{
					number|=MAPTILE_EAST;
				}
			}
			
			if(number==0)
			{
				if( pLevelFile )	pLevelFile[x+YOff[y]] = FileTileHill0;
				if( pLevelFont )	pLevelFont[x+YOff[y]] = NormalHillType[hilltype][GD.Random()%MAX_TILE_VARY];
				if( pLevelType )	pLevelType[x+YOff[y]] = hilltype;
			}
			else
			{
				if( pLevelFile )	pLevelFile[x+YOff[y]] = FileTileHill1 + hilltype;
				if( pLevelFont )	pLevelFont[x+YOff[y]] = GreenType[hilltype][ TileSet[number] ][GD.Random()%MAX_GREEN_VARY];
				if( pLevelType )	pLevelType[x+YOff[y]] = hilltype;
			}
		}
		// 4방향의 이종타일의 형태를 구한다 
		else if(neartype==TILE_GRASS)
		{
		    number=0;
			// 남쪽 
			if(y+1<MH.MapYsize)
			{
				if( pTileAtbMap[x+ YOff[y+1]] != TILE_DEFAULT )
				{
				    number|=MAPTILE_SOUTH;
				}
			}
			// 서쪽 
			if(x-1>=0)
			{
				if( pTileAtbMap[(x-1) + YOff[y]] != TILE_DEFAULT )
				{
					number|=MAPTILE_WEST;
				}
			}
			// 북쪽 
			if(y-1>=0)
			{
				if( pTileAtbMap[x + YOff[y-1]] != TILE_DEFAULT )
				{
				    number|=MAPTILE_NORTH;
				}
			}
			// 동쪽 
			if(x+1<MH.MapXsize)
			{
				if( pTileAtbMap[(x+1) + YOff[y]] != TILE_DEFAULT )
				{
					number|=MAPTILE_EAST;
				}
			}
			
			if(number==0)
			{
				if( pLevelFile )	pLevelFile[x+YOff[y]] = FileTileHill0;
				if( pLevelFont )	pLevelFont[x+YOff[y]] = NormalHillType[hilltype][GD.Random()%MAX_TILE_VARY];
				if( pLevelType )	pLevelType[x+YOff[y]] = hilltype;
			}
			else
			{
				if( pLevelFile )	pLevelFile[x+YOff[y]] = FileTileGrss1 + hilltype;
				if( pLevelFont )	pLevelFont[x+YOff[y]] = GrassType[hilltype][ TileSet[number] ][GD.Random()%MAX_GRASS_VARY];
				if( pLevelType )	pLevelType[x+YOff[y]] = hilltype;
			}
		}
		// 4방향의 이종타일의 형태를 구한다 
		else if(neartype==TILE_CROPAREA)
		{
		    number=0;
			// 남쪽 
			if(y+1<MH.MapYsize)
			{
				if( pTileAtbMap[x + YOff[y+1]] != TILE_DEFAULT )
				{
				    number|=MAPTILE_SOUTH;
				}
			}
			// 서쪽 
			if(x-1>=0)
			{
				if( pTileAtbMap[(x-1) + YOff[y]] != TILE_DEFAULT )
				{
					number|=MAPTILE_WEST;
				}
			}
			// 북쪽 
			if(y-1>=0)
			{
				if( pTileAtbMap[x + YOff[y-1]] != TILE_DEFAULT )
				{
				    number|=MAPTILE_NORTH;
				}
			}
			// 동쪽 
			if(x+1<MH.MapXsize)
			{
				if( pTileAtbMap[(x+1) + YOff[y]] != TILE_DEFAULT )
				{
					number|=MAPTILE_EAST;
				}
			}

			if(number==0)
			{
				if( pLevelFile )	pLevelFile[x+YOff[y]] = FileTileHill0;
				if( pLevelFont )	pLevelFont[x+YOff[y]] = NormalHillType[hilltype][GD.Random()%MAX_TILE_VARY];
				if( pLevelType )	pLevelType[x+YOff[y]] = hilltype;
			}
			else
			{
				if( pLevelFile )	pLevelFile[x+YOff[y]] = FileTileDiff1 + hilltype;
				if( pLevelFont )	pLevelFont[x+YOff[y]] = CropAreaType[hilltype][ TileSet[number] ][GD.Random()%MAX_CROPAREA_VARY];
				if( pLevelType )	pLevelType[x+YOff[y]] = hilltype;
			}
		}
		else
		{
			if( pLevelFile )	pLevelFile[x+YOff[y]] = FileTileHill0;
			if( pLevelFont )	pLevelFont[x+YOff[y]] = NormalHillType[hilltype][GD.Random()%MAX_TILE_VARY];
			if( pLevelType )	pLevelType[x+YOff[y]] = hilltype;
		}
	}
}



void _Map::PutTile(SHORT sx, SHORT sy, int type)
{
	int x, y;
	int i, j;
	int repeat;

	// 유효한 지도 영역인지 확인한다. 
	if(IsInMapArea(sx, sy) == FALSE) return ;


	// 위치에 타일의 종류를 설정한다. 
	if( pTileAtbMap )	pTileAtbMap[sx + YOff[sy]] = type;


	// 타일이 찍히는 부분의 충돌 속성을 설정한다. 
	if(type == TILE_DEFAULT 
	|| type == TILE_GREEN
	|| type == TILE_GRASS
	|| type == TILE_CROPAREA)
	{
		SetCollMap(sx, sy,     BLOCK_NONE);
	}


	// 자신과 주변의 타일을 적절하게 설정한다. 
	for(i = -1;i <= 1;i++)
		for(j = -1;j <= 1;j++)
		{
			// 유효한 지도 영역내에 있다면, 
			if(IsInMapArea(sx + j, sy + i) == TRUE)
			{
				if( pLevelType )	SetProperFileNFont(sx + j, sy + i, pLevelType[(sx+j) + YOff[sy + i]]);
			}
		}

	for(repeat = 0;repeat < 9;repeat++)
	{
		// 자신 
		if(repeat == 0){x = sx;     y = sy;}
		// 남쪽 
		if(repeat == 1){x = sx;     y = sy + 1;}
		// 서쪽 
		if(repeat == 2){x = sx - 1; y = sy;}
		// 북쪽 
		if(repeat == 3){x = sx;     y = sy - 1;}
		// 동쪽 
		if(repeat == 4){x = sx + 1; y = sy;}
		// 남서쪽 
		if(repeat == 5){x = sx - 1; y = sy + 1;}
		// 북서쪽 
		if(repeat == 6){x = sx - 1; y = sy - 1;}
		// 북동쪽 
		if(repeat == 7){x = sx + 1; y = sy - 1;}
		// 남동쪽 
		if(repeat == 8){x = sx + 1; y = sy + 1;}

		// 찍어야 할 타일이 기본형이고 주변에 이종 타일이 있으면 기본형으로 바꾼다. 
		if( type == TILE_DEFAULT )
		{
			int greenswitch		= FALSE;
			int grassswitch		= FALSE;
			int cropareaswitch	= FALSE;

			// 인접한 이종타일의 종류가 몇개나 존재하는지 수를 파악한다. 
			int number=0;

       		for(i = -1;i <= 1;i++)
				for(j = -1;j <= 1;j++)
				{
					if(IsInMapArea(x + j, y + i) == TRUE)
					{
						if( pTileAtbMap )
						{
							if(      pTileAtbMap[(x+j) + YOff[y+i]] == TILE_GREEN    )   greenswitch    = TRUE;
							else if( pTileAtbMap[(x+j) + YOff[y+i]] == TILE_GRASS    )   grassswitch    = TRUE;
							else if( pTileAtbMap[(x+j) + YOff[y+i]] == TILE_CROPAREA )   cropareaswitch = TRUE;
						}
					}
				}
				
				// 인접한 이종 타일의 종류 수를 구한다. 
				if(greenswitch    == TRUE) number++;
				if(grassswitch    == TRUE) number++;
				if(cropareaswitch == TRUE) number++;
				
				// 주변에 이종타일이 2개이상 존재한다고 하면, 
				// 기본 타일에 2종류 이상의 이종 타일이 붙을 수는 없으므로
				// 주변의 이종 타일을 기본 타일로 만들어 준다. 
				if(number > 1)
				{
					
					for(i = -1;i <= 1;i++)
						for(j = -1;j <= 1;j++)
						{
							if(j == 0 && i == 0 )continue;
							
							if(IsInMapArea(x + j, y + i) == TRUE)
							{
								if( pTileAtbMap )
								{
									if( pTileAtbMap[(x+j) + YOff[y+i]] != TILE_DEFAULT )
									{
										PutTile(x + j, y + i, TILE_DEFAULT);
									}
									else
									{
										if( pLevelType )	SetProperFileNFont(x + j, y + i, pLevelType[(x+j) + YOff[y+i]]);
									}
								}
							}
						}
				}
				// 이종 타일이 1개 이하로 존재하면 적절한 그림만 찾아서 넣어주면 된다. 
				else
				{
					for(i=-1;i<=1;i++)
						for(j=-1;j<=1;j++)
						{
							if(j==0 && i==0 )continue;
							
							if(IsInMapArea(x + j, y + i) == TRUE)
							{
								if( pLevelType )	SetProperFileNFont(x + j, y + i, pLevelType[(x+j) + YOff[y+i]]);
							}
						}
				}
		}
		// 찍어야 할 타일이 이종타일이면, 
		else
		{
       		for(i = -1;i <= 1;i++)
				for(j = -1;j <= 1;j++)
				{
					if(j == 0 && i == 0 )continue;
					
					if(IsInMapArea(x + j, y + i) == TRUE)
					{
						// 인근의 타일이 기본 타일도 아니고 자신과 같지도  않다면, 
						// 기본 타일로 이종타일을 없앤다. 
						if( pTileAtbMap )
						{
							if( pTileAtbMap[(x+j) + YOff[y+i]] != TILE_DEFAULT && pTileAtbMap[(x+j) + YOff[y+i]] != type )
							{
								PutTile(x + j, y + i, TILE_DEFAULT);
							}
						}
					}
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


#include <myfile.h>
#include <map.h>

extern _MainData GD;

extern int TileSet[];
extern int BlockSet[];
extern SHORT Dir8Q[];
extern unsigned char NormalHillType[][MAX_TILE_VARY];
extern unsigned char GrassType[][15][MAX_GRASS_VARY];
extern unsigned char GreenType[][15][MAX_GREEN_VARY];
extern unsigned char CropAreaType[][15][MAX_CROPAREA_VARY];
extern unsigned char SeaType[][MAX_SEA_VARY*2];

_Tile Tile[MAX_TILE_TYPE_NUMBER];

//--------------------------------------------------------------------------
// Name: InitTile()
// Desc: 타일 배열을 초기화한다.
//--------------------------------------------------------------------------
void _Map::InitTile()
{
	int i;

    // 언덕 타일의 최대 종류 수 까지 반복한다.
	for(i = 0;i < MAX_TILE_TYPE_NUMBER;i++)
	{
	   Tile[i].X =   0;

	   if(i < 1)
	       Tile[i].Y = 0;
	   else
		   Tile[i].Y = 9;
	}
}

void _Map::SetProperFileNFont(SHORT x, SHORT y, int hilltype)
{
	int number;
	int neartype=0;
	SHORT i, j;
	
	if( pTileAtbMap == NULL ) return;

	// 주변의 타일 종류를 파악한다. 
	if( pTileAtbMap[x+YOff[y]] == TILE_GREEN )
	{
		if( pLevelFile )	pLevelFile[x+YOff[y]] = FileTileHill1 + hilltype;
		if( pLevelFont )	pLevelFont[x+YOff[y]] = GreenType[hilltype][14][GD.Random()%MAX_GRASS_VARY];
		if( pLevelType )	pLevelType[x+YOff[y]] = hilltype;
	}
	// 주변의 타일 종류를 파악한다. 
	else if( pTileAtbMap[x+YOff[y]] == TILE_GRASS )
	{
		if( pLevelFile )	pLevelFile[x+YOff[y]] = FileTileGrss1 + hilltype;
		if( pLevelFont )	pLevelFont[x+YOff[y]] = GrassType[hilltype][14][GD.Random()%MAX_GRASS_VARY];
		if( pLevelType )	pLevelType[x+YOff[y]] = hilltype;
	}
	else if( pTileAtbMap[x+YOff[y]] == TILE_CROPAREA )
	{
		if( pLevelFile )	pLevelFile[x+YOff[y]] = FileTileDiff1 + hilltype;
		if( pLevelFont )	pLevelFont[x+YOff[y]] = CropAreaType[hilltype][14][GD.Random()%MAX_CROPAREA_VARY];
		if( pLevelType )	pLevelType[x+YOff[y]] = hilltype;
	}
	else
	{
		for(i=-1;i<=1;i++)
			for(j=-1;j<=1;j++)
			{
				if(0<=x+j && x+j<MH.MapXsize && 0<=y+i && y+i<MH.MapYsize){}
				else continue;
				if(i==0 && j==0 )continue;

				if( pTileAtbMap[(x+j) + YOff[y+i]] == TILE_GREEN )
				{
					neartype=TILE_GREEN;
				}
				else if( pTileAtbMap[(x+j) + YOff[y+i]] == TILE_GRASS )
				{
					neartype=TILE_GRASS;
				}
				else if( pTileAtbMap[(x+j) + YOff[y+i]] == TILE_CROPAREA )
				{
					neartype=TILE_CROPAREA;
				}

			}


		// 4방향의 이종타일의 형태를 구한다 
		if(neartype == TILE_GREEN)
		{
		    number=0;
			// 남쪽 
			if(y+1<MH.MapYsize)
			{
				if( pTileAtbMap[x + YOff[y+1]] != TILE_DEFAULT )
				{
				    number|=MAPTILE_SOUTH;
				}
			}
			// 서쪽 
			if(x-1>=0)
			{
				if( pTileAtbMap[(x-1) + YOff[y]] != TILE_DEFAULT )
				{
					number|=MAPTILE_WEST;
				}
			}
			// 북쪽 
			if(y-1>=0)
			{
				if( pTileAtbMap[x + YOff[y-1]] != TILE_DEFAULT )
				{
				    number|=MAPTILE_NORTH;
				}
			}
			// 동쪽 
			if(x+1<MH.MapXsize)
			{
				if( pTileAtbMap[(x+1) + YOff[y]] != TILE_DEFAULT )
				{
					number|=MAPTILE_EAST;
				}
			}
			
			if(number==0)
			{
				if( pLevelFile )	pLevelFile[x+YOff[y]] = FileTileHill0;
				if( pLevelFont )	pLevelFont[x+YOff[y]] = NormalHillType[hilltype][GD.Random()%MAX_TILE_VARY];
				if( pLevelType )	pLevelType[x+YOff[y]] = hilltype;
			}
			else
			{
				if( pLevelFile )	pLevelFile[x+YOff[y]] = FileTileHill1 + hilltype;
				if( pLevelFont )	pLevelFont[x+YOff[y]] = GreenType[hilltype][ TileSet[number] ][GD.Random()%MAX_GREEN_VARY];
				if( pLevelType )	pLevelType[x+YOff[y]] = hilltype;
			}
		}
		// 4방향의 이종타일의 형태를 구한다 
		else if(neartype==TILE_GRASS)
		{
		    number=0;
			// 남쪽 
			if(y+1<MH.MapYsize)
			{
				if( pTileAtbMap[x+ YOff[y+1]] != TILE_DEFAULT )
				{
				    number|=MAPTILE_SOUTH;
				}
			}
			// 서쪽 
			if(x-1>=0)
			{
				if( pTileAtbMap[(x-1) + YOff[y]] != TILE_DEFAULT )
				{
					number|=MAPTILE_WEST;
				}
			}
			// 북쪽 
			if(y-1>=0)
			{
				if( pTileAtbMap[x + YOff[y-1]] != TILE_DEFAULT )
				{
				    number|=MAPTILE_NORTH;
				}
			}
			// 동쪽 
			if(x+1<MH.MapXsize)
			{
				if( pTileAtbMap[(x+1) + YOff[y]] != TILE_DEFAULT )
				{
					number|=MAPTILE_EAST;
				}
			}
			
			if(number==0)
			{
				if( pLevelFile )	pLevelFile[x+YOff[y]] = FileTileHill0;
				if( pLevelFont )	pLevelFont[x+YOff[y]] = NormalHillType[hilltype][GD.Random()%MAX_TILE_VARY];
				if( pLevelType )	pLevelType[x+YOff[y]] = hilltype;
			}
			else
			{
				if( pLevelFile )	pLevelFile[x+YOff[y]] = FileTileGrss1 + hilltype;
				if( pLevelFont )	pLevelFont[x+YOff[y]] = GrassType[hilltype][ TileSet[number] ][GD.Random()%MAX_GRASS_VARY];
				if( pLevelType )	pLevelType[x+YOff[y]] = hilltype;
			}
		}
		// 4방향의 이종타일의 형태를 구한다 
		else if(neartype==TILE_CROPAREA)
		{
		    number=0;
			// 남쪽 
			if(y+1<MH.MapYsize)
			{
				if( pTileAtbMap[x + YOff[y+1]] != TILE_DEFAULT )
				{
				    number|=MAPTILE_SOUTH;
				}
			}
			// 서쪽 
			if(x-1>=0)
			{
				if( pTileAtbMap[(x-1) + YOff[y]] != TILE_DEFAULT )
				{
					number|=MAPTILE_WEST;
				}
			}
			// 북쪽 
			if(y-1>=0)
			{
				if( pTileAtbMap[x + YOff[y-1]] != TILE_DEFAULT )
				{
				    number|=MAPTILE_NORTH;
				}
			}
			// 동쪽 
			if(x+1<MH.MapXsize)
			{
				if( pTileAtbMap[(x+1) + YOff[y]] != TILE_DEFAULT )
				{
					number|=MAPTILE_EAST;
				}
			}

			if(number==0)
			{
				if( pLevelFile )	pLevelFile[x+YOff[y]] = FileTileHill0;
				if( pLevelFont )	pLevelFont[x+YOff[y]] = NormalHillType[hilltype][GD.Random()%MAX_TILE_VARY];
				if( pLevelType )	pLevelType[x+YOff[y]] = hilltype;
			}
			else
			{
				if( pLevelFile )	pLevelFile[x+YOff[y]] = FileTileDiff1 + hilltype;
				if( pLevelFont )	pLevelFont[x+YOff[y]] = CropAreaType[hilltype][ TileSet[number] ][GD.Random()%MAX_CROPAREA_VARY];
				if( pLevelType )	pLevelType[x+YOff[y]] = hilltype;
			}
		}
		else
		{
			if( pLevelFile )	pLevelFile[x+YOff[y]] = FileTileHill0;
			if( pLevelFont )	pLevelFont[x+YOff[y]] = NormalHillType[hilltype][GD.Random()%MAX_TILE_VARY];
			if( pLevelType )	pLevelType[x+YOff[y]] = hilltype;
		}
	}
}

void _Map::SetProperTileFont_Daesang(SHORT sx, SHORT sy, int type, int FileTileNo )
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


	

	if( FileTileNo == FileTileHill0 )
		memset(TileSetting, 1, sizeof( TileSetting));
	else
		memset(TileSetting, 0, sizeof( TileSetting));
	
	//찍는놈과 그 주위놈은 1로 세팅..디폴트일때는 0으로 세팅..
	for(  int i = 1; i < 5; i ++)
	{
		for( int j = 1; j < 5; j++ )
		{
			//Normal
			if( FileTileNo == FileTileHill0 )
				TileSetting[i][j] = 0;
			else TileSetting[i][j] = 1;			
		}
	}

	//그 주변놈들은 주변 환경에 맞춰서 찾는다.
	//X축..Y=0인놈들..
	for( i = 0; i < 6; i++)
	{
		if(IsInMapArea(sx+i-2, sy-2-1) == FALSE) continue;

		int font = pLevelFont[(sx+i-2)+YOff[sy-2-1]];
		if( pLevelFile[(sx+i-2)+YOff[sy-2-1]] != FileTileHill0 )
		{
			//폰트를 봐서 채워진 놈이라면..			
			if( LinkAtb[font/4][font%4] == 1)
				TileSetting[0][i] += (FileTileNo == FileTileHill0 ) ? 0 : 1;
			else 
				TileSetting[0][i] += (FileTileNo == FileTileHill0 ) ? -1 : 0;
		}
		else if( FileTileNo == FileTileHill0 )
		{
			//디폴트일때..위에서 처리안된놈을 처리한다..							
			TileSetting[0][i] += -1;
		}
		
	}

	//X축..Y=5인놈들..
	for( i = 0; i < 6; i++)
	{
		if(IsInMapArea(sx+i-2, sy+3+1) == FALSE) continue;
		
		int font = pLevelFont[(sx+i-2)+YOff[sy+3+1]];

		if( pLevelFile[(sx+i-2)+YOff[sy+3+1]] != FileTileHill0 )
		{
			//폰트를 봐서 채워진 놈이라면..
			if( LinkAtb[font/4][font%4] == 1)
				TileSetting[5][i] += (FileTileNo == FileTileHill0 ) ? 0 : 1;
			else 
				TileSetting[5][i] += (FileTileNo == FileTileHill0 ) ? -1 : 0;
		}
		else if( FileTileNo == FileTileHill0 )
		{
			TileSetting[5][i] += -1 ;
		}
	}
	
	//Y축..X=0인놈들..
	for( i = 0; i < 6; i++)
	{
		if(IsInMapArea(sx-2-1, sy-2+i) == FALSE) continue;
		int font = pLevelFont[(sx-2-1)+YOff[sy-2+i]];
		if( pLevelFile[(sx-2-1)+YOff[sy-2+i]] != FileTileHill0 )
		{
			//폰트를 봐서 채워진 놈이라면..			
			if( LinkAtb[font/4][font%4] == 1)
				TileSetting[i][0] += (FileTileNo == FileTileHill0 ) ? 0 : 1;
			else 
				TileSetting[i][0] += (FileTileNo == FileTileHill0 ) ? -1 : 0;
		}
		else if( FileTileNo == FileTileHill0 )
		{
			TileSetting[i][0] += -1;
		}
	}

	//Y축..X=5인놈들..
	for( i = 0; i < 6; i++)
	{
		if(IsInMapArea(sx+3+1, sy-2+i) == FALSE) continue;
		int font = pLevelFont[(sx+3+1)+YOff[sy-2+i]];
		if( pLevelFile[(sx+3+1)+YOff[sy-2+i]] != FileTileHill0 )
		{
			//폰트를 봐서 채워진 놈이라면..			
			if( LinkAtb[font/4][font%4] == 1)
				TileSetting[i][5] += (FileTileNo == FileTileHill0 ) ? 0 : 1;
			else 
				TileSetting[i][5] += (FileTileNo == FileTileHill0 ) ? -1 : 0;
		}
		else if( FileTileNo == FileTileHill0 )
		{
			TileSetting[i][5] += -1;
		}
	}

	//4 귀퉁이는 총합이 2이어야한다..
	if( FileTileNo == FileTileHill0)
	{
		if( TileSetting[0][0] <= 0)	TileSetting[0][0] = 0;
		else TileSetting[0][0] =  1;
		
		if( TileSetting[0][5] <= 0)	TileSetting[0][5] =  0;
		else TileSetting[0][5] =1;
		
		if( TileSetting[5][0] <= 0)	TileSetting[5][0] = 0;
		else TileSetting[5][0] = 1;
		
		if( TileSetting[5][5] <= 0)	TileSetting[5][5] = 0;
		else TileSetting[5][5] = 1;
	}
	else
	{
		if( TileSetting[0][0] == 2)	TileSetting[0][0] = 1;
		else TileSetting[0][0] =  0;
		
		if( TileSetting[0][5] == 2)	TileSetting[0][5] =  1;
		else TileSetting[0][5] =0;
		
		if( TileSetting[5][0] == 2)	TileSetting[5][0] = 1;
		else TileSetting[5][0] = 0;
		
		if( TileSetting[5][5] == 2)	TileSetting[5][5] = 1;
		else TileSetting[5][5] = 0;
	}

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
				
				//해당 하는 타일을 찍는다.
				if( FileTileNo == FileTileHill0)
				{
					//디폴트 타일을 찍으려고했다면..

					//찍히려는 위치의 타일 종류가 디폴트와 다르다면..
					if( FileTileNo != pLevelFile[(sx+(j-1)*2)+YOff[sy+(i-1)*2]] )
					{
						//찍히려는 위치의 타일 종류로 찍는다..						
						PutTile_OnlySet_Daesang(sx+(j-1)*2,  sy+(i-1)*2, type, pLevelFile[(sx+(j-1)*2)+YOff[sy+(i-1)*2]], k*4);
					}
					else
					{
						//디폴트 타일과 같다면.. 그냥 디폴트 랜덤으로 찍는다.
						PutTile_OnlySet_Daesang(sx+(j-1)*2,  sy+(i-1)*2, type, FileTileHill0, DEFINE_ORIGINAL_TILE+((rand()%9)*4));
					}
				}
				else
				{
					//디폴트가 아니면 주변 환경에 따라 그냥찍는다.
					PutTile_OnlySet_Daesang(sx+(j-1)*2,  sy+(i-1)*2, type, FileTileNo, k*4);
				}				
			}
			else
			{
				//적당한걸 못찾았다면..
				PutTile_OnlySet_Daesang(sx+(j-1)*2,  sy+(i-1)*2, type, FileTileHill0, DEFINE_ORIGINAL_TILE+((rand()%9)*4));
			}
		}
	}	
}


void _Map::PutTile_OnlySet_Daesang( SHORT sx, SHORT sy, int type, int FileTileNo, int StartFont)
{

	//대상타일은 4개 단위로 찍힌다.
	int randTileFont = rand()%6;//타일의 종류는 6개..	

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

			//타일 종류 설정..
			if( pTileAtbMap ) pTileAtbMap[(sx+j) + YOff[sy+i]] = type;

			// 타일이 찍히는 부분의 충돌 속성을 설정한다. 
			if(type == TILE_DEFAULT 
				|| type == TILE_GREEN
				|| type == TILE_GRASS
				|| type == TILE_CROPAREA)
			{
				SetCollMap(sx+j, sy+i,     BLOCK_NONE);
			}

			if( pLevelFile )	pLevelFile[(sx+j)+YOff[sy+i]] = FileTileNo;			
			if( pLevelFont )	pLevelFont[(sx+j)+YOff[sy+i]] = fontcount + StartFont;
			if( pLevelType )	pLevelType[(sx+j)+YOff[sy+i]] = 0; //레벨타입 : 15가지의 타일 형태

			fontcount++;
		}
	}
}

void _Map::PutTile_Daesang(SHORT sx, SHORT sy, int type)
{
	// 유효한 지도 영역인지 확인한다.	
	//홀수면 짝수로 만든다.
	if( sx%2 ) sx--;
	if( sy%2 ) sy--;

	int FileTileNo = -1;
	if( type == TILE_DEFAULT ) 
	{
		FileTileNo = FileTileHill0;
	}
	else if( type == TILE_GRASS) {
		FileTileNo = FileTileGrss1;		
	}
	else if( type == TILE_GREEN) FileTileNo = FileTileHill1;	
	else if( type == TILE_CROPAREA) FileTileNo = FileTileDiff1;

	PutTile_OnlySet_Daesang( sx, sy, type, FileTileNo, DEFINE_ORIGINAL_TILE);

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
			if( pLevelFile[(sx+j*2)+YOff[sy+i*2]] == FileTileNo) continue;

			//Grass
			if( pLevelFile[(sx+j*2)+YOff[sy+i*2]] == FileTileGrss1 ||
				pLevelFile[(sx+j*2)+YOff[sy+i*2]] == FileTileDiff1 ||
				pLevelFile[(sx+j*2)+YOff[sy+i*2]] == FileTileHill1 )
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
				PutTile_OnlySet_Daesang( sx+j*2, sy+i*2, type, FileTileHill0, DEFINE_ORIGINAL_TILE);
			}
		}
	}



	//주변의 이종타일을 설정한다.
	SetProperTileFont_Daesang(sx, sy, type, FileTileNo);


}


void _Map::PutTile(SHORT sx, SHORT sy, int type)
{
	int x, y;
	int i, j;
	int repeat;

	// 유효한 지도 영역인지 확인한다. 
	if(IsInMapArea(sx, sy) == FALSE) return ;


	// 위치에 타일의 종류를 설정한다. 
	if( pTileAtbMap )	pTileAtbMap[sx + YOff[sy]] = type;


	// 타일이 찍히는 부분의 충돌 속성을 설정한다. 
	if(type == TILE_DEFAULT 
	|| type == TILE_GREEN
	|| type == TILE_GRASS
	|| type == TILE_CROPAREA)
	{
		SetCollMap(sx, sy,     BLOCK_NONE);
	}


	// 자신과 주변의 타일을 적절하게 설정한다. 
	for(i = -1;i <= 1;i++)
		for(j = -1;j <= 1;j++)
		{
			// 유효한 지도 영역내에 있다면, 
			if(IsInMapArea(sx + j, sy + i) == TRUE)
			{
				if( pLevelType )	SetProperFileNFont(sx + j, sy + i, pLevelType[(sx+j) + YOff[sy + i]]);
			}
		}

	for(repeat = 0;repeat < 9;repeat++)
	{
		// 자신 
		if(repeat == 0){x = sx;     y = sy;}
		// 남쪽 
		if(repeat == 1){x = sx;     y = sy + 1;}
		// 서쪽 
		if(repeat == 2){x = sx - 1; y = sy;}
		// 북쪽 
		if(repeat == 3){x = sx;     y = sy - 1;}
		// 동쪽 
		if(repeat == 4){x = sx + 1; y = sy;}
		// 남서쪽 
		if(repeat == 5){x = sx - 1; y = sy + 1;}
		// 북서쪽 
		if(repeat == 6){x = sx - 1; y = sy - 1;}
		// 북동쪽 
		if(repeat == 7){x = sx + 1; y = sy - 1;}
		// 남동쪽 
		if(repeat == 8){x = sx + 1; y = sy + 1;}

		// 찍어야 할 타일이 기본형이고 주변에 이종 타일이 있으면 기본형으로 바꾼다. 
		if( type == TILE_DEFAULT )
		{
			int greenswitch		= FALSE;
			int grassswitch		= FALSE;
			int cropareaswitch	= FALSE;

			// 인접한 이종타일의 종류가 몇개나 존재하는지 수를 파악한다. 
			int number=0;

       		for(i = -1;i <= 1;i++)
				for(j = -1;j <= 1;j++)
				{
					if(IsInMapArea(x + j, y + i) == TRUE)
					{
						if( pTileAtbMap )
						{
							if(      pTileAtbMap[(x+j) + YOff[y+i]] == TILE_GREEN    )   greenswitch    = TRUE;
							else if( pTileAtbMap[(x+j) + YOff[y+i]] == TILE_GRASS    )   grassswitch    = TRUE;
							else if( pTileAtbMap[(x+j) + YOff[y+i]] == TILE_CROPAREA )   cropareaswitch = TRUE;
						}
					}
				}
				
				// 인접한 이종 타일의 종류 수를 구한다. 
				if(greenswitch    == TRUE) number++;
				if(grassswitch    == TRUE) number++;
				if(cropareaswitch == TRUE) number++;
				
				// 주변에 이종타일이 2개이상 존재한다고 하면, 
				// 기본 타일에 2종류 이상의 이종 타일이 붙을 수는 없으므로
				// 주변의 이종 타일을 기본 타일로 만들어 준다. 
				if(number > 1)
				{
					
					for(i = -1;i <= 1;i++)
						for(j = -1;j <= 1;j++)
						{
							if(j == 0 && i == 0 )continue;
							
							if(IsInMapArea(x + j, y + i) == TRUE)
							{
								if( pTileAtbMap )
								{
									if( pTileAtbMap[(x+j) + YOff[y+i]] != TILE_DEFAULT )
									{
										PutTile(x + j, y + i, TILE_DEFAULT);
									}
									else
									{
										if( pLevelType )	SetProperFileNFont(x + j, y + i, pLevelType[(x+j) + YOff[y+i]]);
									}
								}
							}
						}
				}
				// 이종 타일이 1개 이하로 존재하면 적절한 그림만 찾아서 넣어주면 된다. 
				else
				{
					for(i=-1;i<=1;i++)
						for(j=-1;j<=1;j++)
						{
							if(j==0 && i==0 )continue;
							
							if(IsInMapArea(x + j, y + i) == TRUE)
							{
								if( pLevelType )	SetProperFileNFont(x + j, y + i, pLevelType[(x+j) + YOff[y+i]]);
							}
						}
				}
		}
		// 찍어야 할 타일이 이종타일이면, 
		else
		{
       		for(i = -1;i <= 1;i++)
				for(j = -1;j <= 1;j++)
				{
					if(j == 0 && i == 0 )continue;
					
					if(IsInMapArea(x + j, y + i) == TRUE)
					{
						// 인근의 타일이 기본 타일도 아니고 자신과 같지도  않다면, 
						// 기본 타일로 이종타일을 없앤다. 
						if( pTileAtbMap )
						{
							if( pTileAtbMap[(x+j) + YOff[y+i]] != TILE_DEFAULT && pTileAtbMap[(x+j) + YOff[y+i]] != type )
							{
								PutTile(x + j, y + i, TILE_DEFAULT);
							}
						}
					}
				}
		}
	}
	
}


#endif