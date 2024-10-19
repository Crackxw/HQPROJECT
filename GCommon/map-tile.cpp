#ifndef _ROCKET_NEW_TILE_MAP
//--------------------------------------------------------------------
//  ��� : ���°�
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
// Desc: Ÿ�� �迭�� �ʱ�ȭ�Ѵ�.
//--------------------------------------------------------------------------
void _Map::InitTile()
{
	int i;

    // ��� Ÿ���� �ִ� ���� �� ���� �ݺ��Ѵ�.
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

	// �ֺ��� Ÿ�� ������ �ľ��Ѵ�. 
	if( pTileAtbMap[x+YOff[y]] == TILE_GREEN )
	{
		if( pLevelFile )	pLevelFile[x+YOff[y]] = FileTileHill1 + hilltype;
		if( pLevelFont )	pLevelFont[x+YOff[y]] = GreenType[hilltype][14][GD.Random()%MAX_GRASS_VARY];
		if( pLevelType )	pLevelType[x+YOff[y]] = hilltype;
	}
	// �ֺ��� Ÿ�� ������ �ľ��Ѵ�. 
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


		// 4������ ����Ÿ���� ���¸� ���Ѵ� 
		if(neartype == TILE_GREEN)
		{
		    number=0;
			// ���� 
			if(y+1<MH.MapYsize)
			{
				if( pTileAtbMap[x + YOff[y+1]] != TILE_DEFAULT )
				{
				    number|=MAPTILE_SOUTH;
				}
			}
			// ���� 
			if(x-1>=0)
			{
				if( pTileAtbMap[(x-1) + YOff[y]] != TILE_DEFAULT )
				{
					number|=MAPTILE_WEST;
				}
			}
			// ���� 
			if(y-1>=0)
			{
				if( pTileAtbMap[x + YOff[y-1]] != TILE_DEFAULT )
				{
				    number|=MAPTILE_NORTH;
				}
			}
			// ���� 
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
		// 4������ ����Ÿ���� ���¸� ���Ѵ� 
		else if(neartype==TILE_GRASS)
		{
		    number=0;
			// ���� 
			if(y+1<MH.MapYsize)
			{
				if( pTileAtbMap[x+ YOff[y+1]] != TILE_DEFAULT )
				{
				    number|=MAPTILE_SOUTH;
				}
			}
			// ���� 
			if(x-1>=0)
			{
				if( pTileAtbMap[(x-1) + YOff[y]] != TILE_DEFAULT )
				{
					number|=MAPTILE_WEST;
				}
			}
			// ���� 
			if(y-1>=0)
			{
				if( pTileAtbMap[x + YOff[y-1]] != TILE_DEFAULT )
				{
				    number|=MAPTILE_NORTH;
				}
			}
			// ���� 
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
		// 4������ ����Ÿ���� ���¸� ���Ѵ� 
		else if(neartype==TILE_CROPAREA)
		{
		    number=0;
			// ���� 
			if(y+1<MH.MapYsize)
			{
				if( pTileAtbMap[x + YOff[y+1]] != TILE_DEFAULT )
				{
				    number|=MAPTILE_SOUTH;
				}
			}
			// ���� 
			if(x-1>=0)
			{
				if( pTileAtbMap[(x-1) + YOff[y]] != TILE_DEFAULT )
				{
					number|=MAPTILE_WEST;
				}
			}
			// ���� 
			if(y-1>=0)
			{
				if( pTileAtbMap[x + YOff[y-1]] != TILE_DEFAULT )
				{
				    number|=MAPTILE_NORTH;
				}
			}
			// ���� 
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

	// ��ȿ�� ���� �������� Ȯ���Ѵ�. 
	if(IsInMapArea(sx, sy) == FALSE) return ;


	// ��ġ�� Ÿ���� ������ �����Ѵ�. 
	if( pTileAtbMap )	pTileAtbMap[sx + YOff[sy]] = type;


	// Ÿ���� ������ �κ��� �浹 �Ӽ��� �����Ѵ�. 
	if(type == TILE_DEFAULT 
	|| type == TILE_GREEN
	|| type == TILE_GRASS
	|| type == TILE_CROPAREA)
	{
		SetCollMap(sx, sy,     BLOCK_NONE);
	}


	// �ڽŰ� �ֺ��� Ÿ���� �����ϰ� �����Ѵ�. 
	for(i = -1;i <= 1;i++)
		for(j = -1;j <= 1;j++)
		{
			// ��ȿ�� ���� �������� �ִٸ�, 
			if(IsInMapArea(sx + j, sy + i) == TRUE)
			{
				if( pLevelType )	SetProperFileNFont(sx + j, sy + i, pLevelType[(sx+j) + YOff[sy + i]]);
			}
		}

	for(repeat = 0;repeat < 9;repeat++)
	{
		// �ڽ� 
		if(repeat == 0){x = sx;     y = sy;}
		// ���� 
		if(repeat == 1){x = sx;     y = sy + 1;}
		// ���� 
		if(repeat == 2){x = sx - 1; y = sy;}
		// ���� 
		if(repeat == 3){x = sx;     y = sy - 1;}
		// ���� 
		if(repeat == 4){x = sx + 1; y = sy;}
		// ������ 
		if(repeat == 5){x = sx - 1; y = sy + 1;}
		// �ϼ��� 
		if(repeat == 6){x = sx - 1; y = sy - 1;}
		// �ϵ��� 
		if(repeat == 7){x = sx + 1; y = sy - 1;}
		// ������ 
		if(repeat == 8){x = sx + 1; y = sy + 1;}

		// ���� �� Ÿ���� �⺻���̰� �ֺ��� ���� Ÿ���� ������ �⺻������ �ٲ۴�. 
		if( type == TILE_DEFAULT )
		{
			int greenswitch		= FALSE;
			int grassswitch		= FALSE;
			int cropareaswitch	= FALSE;

			// ������ ����Ÿ���� ������ ��� �����ϴ��� ���� �ľ��Ѵ�. 
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
				
				// ������ ���� Ÿ���� ���� ���� ���Ѵ�. 
				if(greenswitch    == TRUE) number++;
				if(grassswitch    == TRUE) number++;
				if(cropareaswitch == TRUE) number++;
				
				// �ֺ��� ����Ÿ���� 2���̻� �����Ѵٰ� �ϸ�, 
				// �⺻ Ÿ�Ͽ� 2���� �̻��� ���� Ÿ���� ���� ���� �����Ƿ�
				// �ֺ��� ���� Ÿ���� �⺻ Ÿ�Ϸ� ����� �ش�. 
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
				// ���� Ÿ���� 1�� ���Ϸ� �����ϸ� ������ �׸��� ã�Ƽ� �־��ָ� �ȴ�. 
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
		// ���� �� Ÿ���� ����Ÿ���̸�, 
		else
		{
       		for(i = -1;i <= 1;i++)
				for(j = -1;j <= 1;j++)
				{
					if(j == 0 && i == 0 )continue;
					
					if(IsInMapArea(x + j, y + i) == TRUE)
					{
						// �α��� Ÿ���� �⺻ Ÿ�ϵ� �ƴϰ� �ڽŰ� ������  �ʴٸ�, 
						// �⺻ Ÿ�Ϸ� ����Ÿ���� ���ش�. 
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
//  ��� : ���°�
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
// Desc: Ÿ�� �迭�� �ʱ�ȭ�Ѵ�.
//--------------------------------------------------------------------------
void _Map::InitTile()
{
	int i;

    // ��� Ÿ���� �ִ� ���� �� ���� �ݺ��Ѵ�.
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

	// �ֺ��� Ÿ�� ������ �ľ��Ѵ�. 
	if( pTileAtbMap[x+YOff[y]] == TILE_GREEN )
	{
		if( pLevelFile )	pLevelFile[x+YOff[y]] = FileTileHill1 + hilltype;
		if( pLevelFont )	pLevelFont[x+YOff[y]] = GreenType[hilltype][14][GD.Random()%MAX_GRASS_VARY];
		if( pLevelType )	pLevelType[x+YOff[y]] = hilltype;
	}
	// �ֺ��� Ÿ�� ������ �ľ��Ѵ�. 
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


		// 4������ ����Ÿ���� ���¸� ���Ѵ� 
		if(neartype == TILE_GREEN)
		{
		    number=0;
			// ���� 
			if(y+1<MH.MapYsize)
			{
				if( pTileAtbMap[x + YOff[y+1]] != TILE_DEFAULT )
				{
				    number|=MAPTILE_SOUTH;
				}
			}
			// ���� 
			if(x-1>=0)
			{
				if( pTileAtbMap[(x-1) + YOff[y]] != TILE_DEFAULT )
				{
					number|=MAPTILE_WEST;
				}
			}
			// ���� 
			if(y-1>=0)
			{
				if( pTileAtbMap[x + YOff[y-1]] != TILE_DEFAULT )
				{
				    number|=MAPTILE_NORTH;
				}
			}
			// ���� 
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
		// 4������ ����Ÿ���� ���¸� ���Ѵ� 
		else if(neartype==TILE_GRASS)
		{
		    number=0;
			// ���� 
			if(y+1<MH.MapYsize)
			{
				if( pTileAtbMap[x+ YOff[y+1]] != TILE_DEFAULT )
				{
				    number|=MAPTILE_SOUTH;
				}
			}
			// ���� 
			if(x-1>=0)
			{
				if( pTileAtbMap[(x-1) + YOff[y]] != TILE_DEFAULT )
				{
					number|=MAPTILE_WEST;
				}
			}
			// ���� 
			if(y-1>=0)
			{
				if( pTileAtbMap[x + YOff[y-1]] != TILE_DEFAULT )
				{
				    number|=MAPTILE_NORTH;
				}
			}
			// ���� 
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
		// 4������ ����Ÿ���� ���¸� ���Ѵ� 
		else if(neartype==TILE_CROPAREA)
		{
		    number=0;
			// ���� 
			if(y+1<MH.MapYsize)
			{
				if( pTileAtbMap[x + YOff[y+1]] != TILE_DEFAULT )
				{
				    number|=MAPTILE_SOUTH;
				}
			}
			// ���� 
			if(x-1>=0)
			{
				if( pTileAtbMap[(x-1) + YOff[y]] != TILE_DEFAULT )
				{
					number|=MAPTILE_WEST;
				}
			}
			// ���� 
			if(y-1>=0)
			{
				if( pTileAtbMap[x + YOff[y-1]] != TILE_DEFAULT )
				{
				    number|=MAPTILE_NORTH;
				}
			}
			// ���� 
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
	#define DEFINE_ORIGINAL_TILE 72//12*6  ���� ��� Ÿ���� �����ϴ� ��ġ..(�������� ����Ÿ�ϰ� �����ϴ� Ÿ��)
		//���� Ÿ���� �Ӽ� ����..
	//1�̸� �ڱ��ڽ�, 0�̸� Normal Ÿ�ϰ� ����Ǵºκ�..
	//27: Ÿ�ϼ¿���  ��ü ������ 27��
	//4 : �Ѱ��� Ÿ���� 4���� �ɰ�����.
	char LinkAtb[27][4] = { 
		{1,0,0,0}, {0,1,0,0}, {0,0,0,1}, {0,0,1,0}, //����Ǵºκ��� 1��¥���� �͵�
		{1,1,0,0}, {1,1,0,0}, {0,1,0,1}, {0,1,0,1}, {0,0,1,1}, {0,0,1,1}, {1,0,1,0}, {1,0,1,0},//2��¥���ΰ͵�..
		{1,1,0,1}, {0,1,1,1}, {1,0,1,1}, {1,1,1,0}, //3��¥���ΰ͵�
		{0,1,1,0}, {1,0,0,1},//������ 2��¥��
		{1,1,1,1}, {1,1,1,1}, {1,1,1,1}, {1,1,1,1}, {1,1,1,1}, {1,1,1,1}, {1,1,1,1}, {1,1,1,1}, {1,1,1,1}};//4��¥��

	char Type[4];
	char TileSetting[6][6];


	

	if( FileTileNo == FileTileHill0 )
		memset(TileSetting, 1, sizeof( TileSetting));
	else
		memset(TileSetting, 0, sizeof( TileSetting));
	
	//��³�� �� �������� 1�� ����..����Ʈ�϶��� 0���� ����..
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

	//�� �ֺ������ �ֺ� ȯ�濡 ���缭 ã�´�.
	//X��..Y=0�γ��..
	for( i = 0; i < 6; i++)
	{
		if(IsInMapArea(sx+i-2, sy-2-1) == FALSE) continue;

		int font = pLevelFont[(sx+i-2)+YOff[sy-2-1]];
		if( pLevelFile[(sx+i-2)+YOff[sy-2-1]] != FileTileHill0 )
		{
			//��Ʈ�� ���� ä���� ���̶��..			
			if( LinkAtb[font/4][font%4] == 1)
				TileSetting[0][i] += (FileTileNo == FileTileHill0 ) ? 0 : 1;
			else 
				TileSetting[0][i] += (FileTileNo == FileTileHill0 ) ? -1 : 0;
		}
		else if( FileTileNo == FileTileHill0 )
		{
			//����Ʈ�϶�..������ ó���ȵȳ��� ó���Ѵ�..							
			TileSetting[0][i] += -1;
		}
		
	}

	//X��..Y=5�γ��..
	for( i = 0; i < 6; i++)
	{
		if(IsInMapArea(sx+i-2, sy+3+1) == FALSE) continue;
		
		int font = pLevelFont[(sx+i-2)+YOff[sy+3+1]];

		if( pLevelFile[(sx+i-2)+YOff[sy+3+1]] != FileTileHill0 )
		{
			//��Ʈ�� ���� ä���� ���̶��..
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
	
	//Y��..X=0�γ��..
	for( i = 0; i < 6; i++)
	{
		if(IsInMapArea(sx-2-1, sy-2+i) == FALSE) continue;
		int font = pLevelFont[(sx-2-1)+YOff[sy-2+i]];
		if( pLevelFile[(sx-2-1)+YOff[sy-2+i]] != FileTileHill0 )
		{
			//��Ʈ�� ���� ä���� ���̶��..			
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

	//Y��..X=5�γ��..
	for( i = 0; i < 6; i++)
	{
		if(IsInMapArea(sx+3+1, sy-2+i) == FALSE) continue;
		int font = pLevelFont[(sx+3+1)+YOff[sy-2+i]];
		if( pLevelFile[(sx+3+1)+YOff[sy-2+i]] != FileTileHill0 )
		{
			//��Ʈ�� ���� ä���� ���̶��..			
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

	//4 �����̴� ������ 2�̾���Ѵ�..
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

	//������ Ÿ���� ã�Ƽ� �����Ѵ�.
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

			//����� ���� Ÿ���� ã�´�.
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
				//2���� �پ��ִ� Ÿ���� �ִ°Ŷ��.. 2�־������Ƿ�..
				k += rand()%2;
			}
			if( k >= 18 ) 
			{
				//���� Ÿ���̸� �ٸ� ����� ���� ������� �����Ѵ�.
				k+=rand()%9;
			}
			//������ Ÿ���� ã�Ҵٸ�..
			if( FindIndex > -1 )
			{		
				
				//�ش� �ϴ� Ÿ���� ��´�.
				if( FileTileNo == FileTileHill0)
				{
					//����Ʈ Ÿ���� ���������ߴٸ�..

					//�������� ��ġ�� Ÿ�� ������ ����Ʈ�� �ٸ��ٸ�..
					if( FileTileNo != pLevelFile[(sx+(j-1)*2)+YOff[sy+(i-1)*2]] )
					{
						//�������� ��ġ�� Ÿ�� ������ ��´�..						
						PutTile_OnlySet_Daesang(sx+(j-1)*2,  sy+(i-1)*2, type, pLevelFile[(sx+(j-1)*2)+YOff[sy+(i-1)*2]], k*4);
					}
					else
					{
						//����Ʈ Ÿ�ϰ� ���ٸ�.. �׳� ����Ʈ �������� ��´�.
						PutTile_OnlySet_Daesang(sx+(j-1)*2,  sy+(i-1)*2, type, FileTileHill0, DEFINE_ORIGINAL_TILE+((rand()%9)*4));
					}
				}
				else
				{
					//����Ʈ�� �ƴϸ� �ֺ� ȯ�濡 ���� �׳���´�.
					PutTile_OnlySet_Daesang(sx+(j-1)*2,  sy+(i-1)*2, type, FileTileNo, k*4);
				}				
			}
			else
			{
				//�����Ѱ� ��ã�Ҵٸ�..
				PutTile_OnlySet_Daesang(sx+(j-1)*2,  sy+(i-1)*2, type, FileTileHill0, DEFINE_ORIGINAL_TILE+((rand()%9)*4));
			}
		}
	}	
}


void _Map::PutTile_OnlySet_Daesang( SHORT sx, SHORT sy, int type, int FileTileNo, int StartFont)
{

	//���Ÿ���� 4�� ������ ������.
	int randTileFont = rand()%6;//Ÿ���� ������ 6��..	

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

			//Ÿ�� ���� ����..
			if( pTileAtbMap ) pTileAtbMap[(sx+j) + YOff[sy+i]] = type;

			// Ÿ���� ������ �κ��� �浹 �Ӽ��� �����Ѵ�. 
			if(type == TILE_DEFAULT 
				|| type == TILE_GREEN
				|| type == TILE_GRASS
				|| type == TILE_CROPAREA)
			{
				SetCollMap(sx+j, sy+i,     BLOCK_NONE);
			}

			if( pLevelFile )	pLevelFile[(sx+j)+YOff[sy+i]] = FileTileNo;			
			if( pLevelFont )	pLevelFont[(sx+j)+YOff[sy+i]] = fontcount + StartFont;
			if( pLevelType )	pLevelType[(sx+j)+YOff[sy+i]] = 0; //����Ÿ�� : 15������ Ÿ�� ����

			fontcount++;
		}
	}
}

void _Map::PutTile_Daesang(SHORT sx, SHORT sy, int type)
{
	// ��ȿ�� ���� �������� Ȯ���Ѵ�.	
	//Ȧ���� ¦���� �����.
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

	//���� �Ҽ����� Ÿ���� ã�Ƽ� NormalTile�� �ʱ�ȭ��Ų��.
	for( int i = -1; i<2; i++)
	{
		for( int j = -1; j<2; j++)
		{
			if( i == 0 && j == 0) continue;
			
			if(IsInMapArea(sx+j*2, sy+i*2) == FALSE) continue;

			//���� �������� �Ͱ� ���ٸ� �Ѿ��..
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

	//�ֺ��� ���� Ÿ���� ������ 2�� �̻��̶�� ��� ������ �ʱ�ȭ�Ѵ�.
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



	//�ֺ��� ����Ÿ���� �����Ѵ�.
	SetProperTileFont_Daesang(sx, sy, type, FileTileNo);


}


void _Map::PutTile(SHORT sx, SHORT sy, int type)
{
	int x, y;
	int i, j;
	int repeat;

	// ��ȿ�� ���� �������� Ȯ���Ѵ�. 
	if(IsInMapArea(sx, sy) == FALSE) return ;


	// ��ġ�� Ÿ���� ������ �����Ѵ�. 
	if( pTileAtbMap )	pTileAtbMap[sx + YOff[sy]] = type;


	// Ÿ���� ������ �κ��� �浹 �Ӽ��� �����Ѵ�. 
	if(type == TILE_DEFAULT 
	|| type == TILE_GREEN
	|| type == TILE_GRASS
	|| type == TILE_CROPAREA)
	{
		SetCollMap(sx, sy,     BLOCK_NONE);
	}


	// �ڽŰ� �ֺ��� Ÿ���� �����ϰ� �����Ѵ�. 
	for(i = -1;i <= 1;i++)
		for(j = -1;j <= 1;j++)
		{
			// ��ȿ�� ���� �������� �ִٸ�, 
			if(IsInMapArea(sx + j, sy + i) == TRUE)
			{
				if( pLevelType )	SetProperFileNFont(sx + j, sy + i, pLevelType[(sx+j) + YOff[sy + i]]);
			}
		}

	for(repeat = 0;repeat < 9;repeat++)
	{
		// �ڽ� 
		if(repeat == 0){x = sx;     y = sy;}
		// ���� 
		if(repeat == 1){x = sx;     y = sy + 1;}
		// ���� 
		if(repeat == 2){x = sx - 1; y = sy;}
		// ���� 
		if(repeat == 3){x = sx;     y = sy - 1;}
		// ���� 
		if(repeat == 4){x = sx + 1; y = sy;}
		// ������ 
		if(repeat == 5){x = sx - 1; y = sy + 1;}
		// �ϼ��� 
		if(repeat == 6){x = sx - 1; y = sy - 1;}
		// �ϵ��� 
		if(repeat == 7){x = sx + 1; y = sy - 1;}
		// ������ 
		if(repeat == 8){x = sx + 1; y = sy + 1;}

		// ���� �� Ÿ���� �⺻���̰� �ֺ��� ���� Ÿ���� ������ �⺻������ �ٲ۴�. 
		if( type == TILE_DEFAULT )
		{
			int greenswitch		= FALSE;
			int grassswitch		= FALSE;
			int cropareaswitch	= FALSE;

			// ������ ����Ÿ���� ������ ��� �����ϴ��� ���� �ľ��Ѵ�. 
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
				
				// ������ ���� Ÿ���� ���� ���� ���Ѵ�. 
				if(greenswitch    == TRUE) number++;
				if(grassswitch    == TRUE) number++;
				if(cropareaswitch == TRUE) number++;
				
				// �ֺ��� ����Ÿ���� 2���̻� �����Ѵٰ� �ϸ�, 
				// �⺻ Ÿ�Ͽ� 2���� �̻��� ���� Ÿ���� ���� ���� �����Ƿ�
				// �ֺ��� ���� Ÿ���� �⺻ Ÿ�Ϸ� ����� �ش�. 
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
				// ���� Ÿ���� 1�� ���Ϸ� �����ϸ� ������ �׸��� ã�Ƽ� �־��ָ� �ȴ�. 
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
		// ���� �� Ÿ���� ����Ÿ���̸�, 
		else
		{
       		for(i = -1;i <= 1;i++)
				for(j = -1;j <= 1;j++)
				{
					if(j == 0 && i == 0 )continue;
					
					if(IsInMapArea(x + j, y + i) == TRUE)
					{
						// �α��� Ÿ���� �⺻ Ÿ�ϵ� �ƴϰ� �ڽŰ� ������  �ʴٸ�, 
						// �⺻ Ÿ�Ϸ� ����Ÿ���� ���ش�. 
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