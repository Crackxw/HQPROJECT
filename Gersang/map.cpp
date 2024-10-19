#include <GSL.h>
#include <main.h>

#ifdef _IMJIN2
	#include "maindata.h"
	#include <etc.h>
	#include "music.h"
#endif

#include <map.h>
#include <ysmap.h>
#include <smoke.h>
#include "charanimation.h"
#include <kindinfo.h>


extern	RECT				ScreenRect;							// ������ ǥ�õǴ� ���� 
extern	int 				MapCenterX[][MAX_MAP_YSIZE];		// ������ �� ��ǥ������ �߽ɰ� 
extern	int 				MapCenterY[][MAX_MAP_YSIZE];
extern	_KindInfo			KI[];								// ĳ���� ������ ������ 
extern	char 				SavePath[];
extern	_Map 				Map;
extern	_MainData			GD;									// ������ �ֵ����� 
extern	_YSMap 				SMap;								// ���� ������ ������ 

extern	int					TileSet[];
extern	int					BlockSet[];
extern	SHORT				Dir8Q[];
extern	unsigned char		NormalHillType[][MAX_TILE_VARY];
extern	unsigned char		GrassType[][15][MAX_GRASS_VARY];
extern	unsigned char		GreenType[][15][MAX_GREEN_VARY];
extern	unsigned char		CropAreaType[][15][MAX_CROPAREA_VARY];
extern	unsigned char		SeaType[][MAX_SEA_VARY*2];
extern	_Tile				Tile[];
extern	unsigned char		MapByRealX[][(MAX_MAP_YSIZE)*MAP_TILE_YSIZE/MAP_BY_REAL_DIV+200/MAP_BY_REAL_DIV];
extern	unsigned char		MapByRealY[][(MAX_MAP_YSIZE)*MAP_TILE_YSIZE/MAP_BY_REAL_DIV+200/MAP_BY_REAL_DIV];


SHORT	OldVirtualMapX, OldVirtualMapY;
char	DrawMapPart[MAX_DRAWMAP_SCREEN_XSIZE][MAX_DRAWMAP_SCREEN_YSIZE];

// MapByRealX[], MapByRealY[] ������ ������Ʈ�Ѵ�. 
void _Map::UpdateMapByRealXY()
{

	SHORT i, j;
	SHORT smallx, smally;
	SHORT centerx, centery;


	int xsize=MH.MapXsize * MAP_TILE_XSIZE;
	int ysize=MH.MapYsize * MAP_TILE_YSIZE;
		
	for(i=-200;i<ysize;i+=MAP_BY_REAL_DIV)
		for(j=0;j<xsize;j+=MAP_BY_REAL_DIV)
		{
			smallx=0;
			smally=0;
			DecideMapXYByXY((j-xsize/2), i, smallx, smally, centerx, centery);
				
		    // ���� ��ƾ 
		    if(smallx<0)smallx=0;
			if(smallx>=MH.MapXsize)smallx=MH.MapXsize-1;

			if(smally<0)smally=0;
			if(smally>=MH.MapYsize)smally=MH.MapYsize-1;

			MapByRealX[j/MAP_BY_REAL_DIV][i/MAP_BY_REAL_DIV+200/MAP_BY_REAL_DIV]=(unsigned char)smallx;
            MapByRealY[j/MAP_BY_REAL_DIV][i/MAP_BY_REAL_DIV+200/MAP_BY_REAL_DIV]=(unsigned char)smally;
		}





}





// �������� X, Y��ǥ�� �߽��� ��ǥ�� �����ش�. (������ǥ)
BOOL _Map::GetRealDotCenterXY(SHORT x, SHORT y, int & centerx, int & centery)
{
	if(IsInMapArea(x, y) == FALSE)return FALSE;


	// ���̸� ���Ѵ�. 
	int height=GetLevel(x, y) * HEIGHT_PER_LEVEL;

	// ��� Ÿ���� ������ ���Ѵ�. 
	int hilltype=GetHillType(x, y);
	
	// ��ũ�� ������ �߽� ��ǥ�� ���Ѵ�. 
	centerx = x * MAP_TILE_XSIZE_HALF
		     -y * MAP_TILE_XSIZE_HALF ;

	centery = y * MAP_TILE_YSIZE_HALF
		     +x * MAP_TILE_YSIZE_HALF ;


	if(GetLevelInfo(x, y)==HILL_PLANE)  // ��� 
	{
	    centery = centery-(GetLevel(x,y)-1)*HEIGHT_PER_LEVEL      + Tile[hilltype].Y;
	}
    else                    // ��� 
       centery = centery -(abs(GetLevel(x,y) ) )*HEIGHT_PER_LEVEL + Tile[hilltype].Y;


	return TRUE;

}

// �������� X, Y��ǥ�� ��ġ�� �����ش�. ����� �������� �ȴ�. (ȭ����ǥ)
// x, y :�⺻ ��ǥ 
BOOL _Map::GetRefXY(SHORT x, SHORT y, SHORT & centerx, SHORT & centery)
{


	if(IsInMapArea(x, y) == FALSE)return FALSE;

	// ���̸� ���Ѵ�. 
	int height=GetLevel(x,y) * HEIGHT_PER_LEVEL;

	// ��� Ÿ���� ������ ���Ѵ�. 
	int hilltype=GetHillType(x, y);
	
	// ��ũ�� ������ �߽� ��ǥ�� ���Ѵ�. 
	centerx=-VirtualMapX*MAP_TILE_XSIZE_HALF
	        +VirtualMapY*MAP_TILE_XSIZE_HALF
			+ScreenRect.left+(ScreenRect.right-ScreenRect.left+1)/2
			+x*MAP_TILE_XSIZE_HALF
			-y*MAP_TILE_XSIZE_HALF;

	centery=-VirtualMapY*MAP_TILE_YSIZE_HALF
	 	    -VirtualMapX*MAP_TILE_YSIZE_HALF
			+ScreenRect.top+(ScreenRect.bottom-ScreenRect.top+1)/2
			+y*MAP_TILE_YSIZE_HALF
			+x*MAP_TILE_YSIZE_HALF;

	return TRUE;
}

// �������� X, Y��ǥ�� �߽��� ��ǥ�� �����ش�. (ȭ����ǥ)
// x, y :�⺻ ��ǥ 
BOOL _Map::GetCenterXY(SHORT x, SHORT y, SHORT & centerx, SHORT & centery)
{

	if(IsInMapArea(x, y) == FALSE)return FALSE;

	// ���̸� ���Ѵ�. 
	int height=GetLevel(x,y)*HEIGHT_PER_LEVEL;

	// ��� Ÿ���� ������ ���Ѵ�. 
	int hilltype=GetHillType(x, y);
	

	// ��ũ�� ������ �߽� ��ǥ�� ���Ѵ�. 
	centerx=-VirtualMapX*MAP_TILE_XSIZE_HALF
	        +VirtualMapY*MAP_TILE_XSIZE_HALF
			+ScreenRect.left+(ScreenRect.right-ScreenRect.left+1)/2
			+x*MAP_TILE_XSIZE_HALF
			-y*MAP_TILE_XSIZE_HALF;

	centery=-VirtualMapY*MAP_TILE_YSIZE_HALF
	 	    -VirtualMapX*MAP_TILE_YSIZE_HALF
			+ScreenRect.top+(ScreenRect.bottom-ScreenRect.top+1)/2
			+y*MAP_TILE_YSIZE_HALF
			+x*MAP_TILE_YSIZE_HALF;

	if(GetLevelInfo(x, y)==HILL_PLANE)  // ��� 
	{
	    centery = centery-(GetLevel(x,y)-1)*HEIGHT_PER_LEVEL      + Tile[hilltype].Y;
	}
    else                    // ��� 
       centery = centery -(abs(GetLevel(x,y) ) )*HEIGHT_PER_LEVEL + Tile[hilltype].Y;

	return TRUE;
}

// �������� X, Y��ǥ�� �߽��� ��ǥ�� �����ش�. (ȭ����ǥ)
BOOL _Map::GetCenterXYByMapCenterXY(SHORT x, SHORT y, SHORT & centerx, SHORT & centery)
{

	if(IsInMapArea(x, y)==FALSE)return FALSE;

	centerx=MapCenterX[x][y] - (VirtualMapX * MAP_TILE_XSIZE_HALF)
			                 + (VirtualMapY * MAP_TILE_XSIZE_HALF)
			                 + ScreenRect.left + (ScreenRect.right - ScreenRect.left + 1) / 2;

	centery=MapCenterY[x][y] - VirtualMapY * MAP_TILE_YSIZE_HALF
				             - VirtualMapX * MAP_TILE_YSIZE_HALF
				             + ScreenRect.top + (ScreenRect.bottom - ScreenRect.top + 1) / 2;

	return TRUE;

}


// ���� ��ǥ�� ȭ�� ��ǥ�� ��ȯ�Ѵ�.
void _Map::TransFromRealDotToScreenDot(SHORT realx, SHORT realy, SHORT& screenx, SHORT &screeny)
{

	screenx=realx+ScreenRect.left+(ScreenRect.right-ScreenRect.left+1)/2+(-VirtualMapX*MAP_TILE_XSIZE_HALF+VirtualMapY*MAP_TILE_XSIZE_HALF);
	screeny=realy+ScreenRect.top+(ScreenRect.bottom-ScreenRect.top+1)/2 +(-VirtualMapY*MAP_TILE_YSIZE_HALF-VirtualMapX*MAP_TILE_YSIZE_HALF);
}

// ȭ�� ��ǥ�� ���� ��ǥ�� ��ȯ�Ѵ�.
void _Map::TransFromScreenToRealXY(SHORT screenx, SHORT screeny, SHORT& realx, SHORT &realy)
{
	realx=screenx-(ScreenRect.left+(ScreenRect.right-ScreenRect.left+1)/2)-(-VirtualMapX*MAP_TILE_XSIZE_HALF+VirtualMapY*MAP_TILE_XSIZE_HALF);
	realy=screeny-(ScreenRect.top+(ScreenRect.bottom-ScreenRect.top+1)/2)-(-VirtualMapY*MAP_TILE_YSIZE_HALF-VirtualMapX*MAP_TILE_YSIZE_HALF);
}




// ĳ������ ���������� ���� ���� 
// ���� 4��Ʈ�� �����Ѵ�. 
// ���� 12��Ʈ�� ���� ������. 
BOOL _Map::DeleteCharCollMap(SHORT x, SHORT y)
{
	if( pCollMap == NULL )	return TRUE;
	if( IsInMapArea(x, y) == FALSE )	return FALSE;

	pCollMap[x+YOff[y]] &= 0x0fff;
	return TRUE;
}

// ����ü�� ���������� ���� ���� 
BOOL _Map::DeleteAirCharCollMap(SHORT x, SHORT y)
{
	if( pCollMap == NULL )	return TRUE;
	if( IsInMapArea(x, y) == FALSE )	return FALSE;
	
	pCollMap[x+YOff[y]] &= 0xf0ff;
	return TRUE;
}



// ���� ��ǥ�� �ٰŷ� �ؼ� �⺻ ��ǥ�� ���Ѵ�. 
BOOL _Map::DecideMapXYByXY(SHORT x, SHORT y, SHORT& map_x, SHORT& map_y, SHORT & map_center_x, SHORT & map_center_y)
{

	int i, j;
	int centerx, centery;

	// ���콺�� ��ǥ�� ���� ����� �߽��� ��ǥ�� ���Ѵ�. 
	int length;
	int min_length=100;

	SHORT virtualmapx=0;
	SHORT virtualmapy=0;

	// ����� ������� �ʰ� �⺻ ��ǥ�� ��������� ���Ѵ�. 
	virtualmapx = (x/2+y)/MAP_TILE_XSIZE_HALF;
	virtualmapy = (-x/2+y)/MAP_TILE_XSIZE_HALF;


	// ����� �ֱ� ������ �α��� �� Ÿ���� üũ�Ͽ� �������� ��ǥ�� ���Ѵ�.
	for(i=virtualmapy-2;i<virtualmapy+5;i++)
		for(j=virtualmapx-4;j<virtualmapx+4;j++)
		{
			if(IsInMapArea(j, i)==TRUE)
			{
				centerx=MapCenterX[j][i];
				centery=MapCenterY[j][i];


	           length=max(abs(x-centerx), abs(y-centery));

	           if(length<min_length)
			   { 
		          min_length=length;
		          map_x=j;
		          map_y=i;
		          map_center_x=centerx;
		          map_center_y=centery;
			   }
			}

		}

		// ����� ���� ã�� ���ߴٸ�, 
		if(min_length==100)return FALSE;

		return TRUE;
}

// ���� ��ǥ�� �ٰŷ� �ؼ� �⺻ ��ǥ�� ���Ѵ�. 
BOOL _Map::DecideMapXYByXYLow(SHORT x, SHORT y, SHORT& map_x, SHORT& map_y, SHORT & map_center_x, SHORT & map_center_y)
{

	int i, j;

	// ���콺�� ��ǥ�� ���� ����� �߽��� ��ǥ�� ���Ѵ�. 
	int length;
	int min_length=1000000;

	SHORT virtualmapx=0;
	SHORT virtualmapy=0;

	virtualmapx=abs(x/2+y)/MAP_TILE_XSIZE_HALF;
	virtualmapy=abs(-x/2+y)/MAP_TILE_XSIZE_HALF;

	for(i=virtualmapy-1;i<virtualmapy+4;i++)
		for(j=virtualmapx-3;j<virtualmapx+3;j++)
		{

			if(IsInMapArea(j, i)==TRUE)
			{
				int centerx, centery;
				GetRealDotCenterXY(j, i, centerx, centery);

	           length=(x-centerx)*(x-centerx)
		             +(y-centery)*(y-centery);

	           if(length<min_length)
			   { 
		          min_length=length;
		          map_x=j;
		          map_y=i;
		          map_center_x=centerx;
		          map_center_y=centery;
			   }
			}
		}
       return TRUE;
}


// mousex, mousey�� Ŭ���̾�Ʈ ������ �ִ°�?
BOOL _Map::IsPtInMap(SHORT mouse_x, SHORT mouse_y)
{

	if(mouse_x>=ScreenRect.left && mouse_x<=ScreenRect.right
	&& mouse_y>=ScreenRect.top  && mouse_y<=ScreenRect.bottom)
	{
		return TRUE;
	}

	return FALSE;
}







// x, y��ġ�� Ÿ���� ȭ�Ϲ�ȣ�� ��Ʈ ��ȣ�� ���Ѵ�. 
BOOL _Map::GetMapFileNFont(SHORT x, SHORT y, SHORT& file, SHORT &font)
{
	
	if( GetSeaMode(x, y) )
	{
		file = GetSeaFile(x/2, y/2);
		font = GetSeaFont(x/2, y/2);
	}
	else if( GetNewBlkMode(x, y) )
	{
		file = GetNewBlkFile(x/2, y/2);
		font = GetNewBlkFont(x/2, y/2);
	}
	else if( GetCastleMode(x, y) )
	{
		file = GetCastleFile(x/2, y/2);
		font = GetCastleFont(x/2, y/2);
	}
	else
	{
		SHORT atb = GetTileAtb(x, y);
		
		switch(atb)
		{
		case TILE_CROPAREA:
		case TILE_DEFAULT:
		default:
			file = GetLevelFile(x, y); 
			font = GetLevelFont(x, y);
			break;
		}
	}
	
	return TRUE;
	
}

void _Map::PushExpansionBasePoint(SHORT x, SHORT y)
{
	if(ExpansionBaseNumber>=MAX_MULTI_PLAYER)return ;

	ExpansionBasePoint[ExpansionBaseNumber].x=x;
	ExpansionBasePoint[ExpansionBaseNumber].y=y;
	ExpansionBaseNumber++;
}


// ������ �÷��̾� ���� �˷��ش�. (������2���� �ش��ϴ� �ڵ��̴�. ����, õ�⿡�� ������ ����� ���ԵǾ� �־� ����� �б⸸ �ϸ� ������ 
// ��2 ���� ������ �Ʒ� �Լ��� ����ؾ߸� �Ѵ�.)
SI32 GetPlayerNumberOfMap(char* filename)
{
	SHORT i;
	FILE* fp;
	SHORT count=0;
	POINT point[MAX_MULTI_PLAYER];
	char buffer[FILE_NAME_SIZE];
	char buffer1[FILE_NAME_SIZE];
	_MapHeader mapheader;
	SI32 siSize;

	// ������ �ʱ�ȭ�Ѵ�. 
	strcpy(buffer1, "cusmap\\");
	strcat(buffer1, filename);
	GetFileNamePath(buffer1, SavePath, buffer);

	fp=fopen(buffer, "rb");
	if(fp==0)return 0;

	


	// ȭ���� ����� ���Ѵ�. (���� �������� �� �������� �����ϱ� ����. )
	siSize = filelength(fileno(fp));

	// �÷��̾� ���� ��ġ������ �̵��Ѵ�. 

	// �������̳� �Ź����̳Ŀ� ���� ����� �ٸ���. 

#ifdef _IMJIN2
	// ������ �ݰ��̸�, 
#ifdef _IMJIN2PLUS
	// �������̸�, 
	if(siSize == 1100300)
	{
		fseek(fp, 4, SEEK_SET);
		
		fread(point, sizeof(POINT)*MAX_MULTI_PLAYER,1, fp);
		fclose(fp);
		
		for(i=0;i<MAX_MULTI_PLAYER;i++)
		{
			if(point[i].x>=0 && point[i].y>=0)
			{
				count++;
			}
		}

		return count;
	}
	// �Ź����̸�, 
	else
	{
		fclose(fp);

		// ����� �о �� ����ȿ� �ִ� ������ ��´�. 
		if(Map.GetMapInfo(buffer, &mapheader) == TRUE)
		{
			return mapheader.PlayerNumber;
		}
	}

	// �������̸�, 
#else

	// �������̸�, 
	if(siSize == 1097100)
	{
		fseek(fp, 4, SEEK_SET);
		
		fread(point, sizeof(POINT)*MAX_MULTI_PLAYER,1, fp);
		fclose(fp);
		
		for(i=0;i<MAX_MULTI_PLAYER;i++)
		{
			if(point[i].x>=0 && point[i].y>=0)
			{
				count++;
			}
		}

		return count;
	}
	else
	{
		fclose(fp);

		// ����� �о �� ����ȿ� �ִ� ������ ��´�. 
		if(Map.GetMapInfo(buffer, &mapheader) == TRUE)
		{

			return mapheader.PlayerNumber;
		}
		else
		{
			clGrp.Error("32947jd", "Fsd02w73");
		}

	}

#endif
#endif

	return 0;
}

// ����� �������� Ȯ���Ѵ�.
BOOL IsMapForDemo(char* filename_ext)
{
	SHORT i;
	SHORT count=0;
	int start_pos_add=0;
	char buffer[FILE_NAME_SIZE];
	char buffer1[FILE_NAME_SIZE];
	
	SHORT return_value = TRUE;


#ifdef TYPE_YONSEI
	// ȭ�� �̸��� ����� ȭ���� �ƴϸ� FALSE
	if(strcmp(filename_ext, "(4��,100X100)�����Ҹ����.map") != 0)
	{
		return FALSE;
	}

	return TRUE;

#endif


	_Map* map= NULL;

	// ������ �޸𸮸� �Ҵ��Ѵ�. 
	map= new _Map;
	if(map == NULL) return FALSE;
	else 
	{
		// 0���� �ʱ�ȭ�Ѵ�.
		ZeroMemory(map, sizeof(_Map));
	}


	// ������ �ʱ�ȭ�Ѵ�. 
	strcpy(buffer1, "cusmap\\");
	strcat(buffer1, filename_ext);
	GetFileNamePath(buffer1, SavePath, buffer);

	// ������ �ҷ��´�. 
	if(map->Load(buffer) == FALSE) 
	{
		delete map;
		return FALSE;
	}

	// ������ġ�� ������ ���Ѵ�. 
	for(i=0;i<MAX_MULTI_PLAYER;i++)
	{
		if(map->clMapPlayerInfo[i].StartPoint.x>=0 
		&& map->clMapPlayerInfo[i].StartPoint.y>=0)
		{
			count++;

			start_pos_add += map->clMapPlayerInfo[i].StartPoint.x;
			start_pos_add += map->clMapPlayerInfo[i].StartPoint.y;
		}
	}

#ifdef _IMJIN2PLUS
	if(count != 4)											return_value=FALSE;
	if(map->MH.MapType != MAP_TYPE_GREEN)					return_value=FALSE;
	if(map->MH.MapXsize != 120 || map->MH.MapYsize !=120)   return_value=FALSE;
	if(start_pos_add != 467)								return_value=FALSE;

#else
	if(count != 4)											return_value=FALSE;
	if(map->MH.MapType != MAP_TYPE_NORMAL)					return_value=FALSE;
	if(map->MH.MapXsize != 100 || map->MH.MapYsize !=100)   return_value=FALSE;
	if(start_pos_add != 422)								return_value=FALSE;
#endif

	delete map;

	return return_value;
}

void _Map::UpdateSmallMapBase(SHORT x, SHORT y)
{
	SHORT file, font;

	switch( GetCropTypeMap(x, y) )
	{
		case CROP_TYPE_RICE:
		case CROP_TYPE_POTATO:
			GD.SmallMapBase[x][y]=18;
			break;
		case CROP_TYPE_TREE:
		case CROP_TYPE_BAMBOO:
			GD.SmallMapBase[x][y]=2;
			break;

		default:
			GetMapFileNFont(x, y, file, font);
	        GD.SmallMapBase[x][y]=SMap.TileColor[file][font];
		    break;
	}
}


// x, y��ǥ�� ȭ��ȿ� �ִ��� Ȯ���Ѵ�.
BOOL _Map::IsInScreen(SHORT x, SHORT y)
{

	SHORT centerx, centery;

	if(GetCenterXY(x, y, centerx, centery)==TRUE)
	{
		if(ScreenRect.left<= centerx && centerx <= ScreenRect.right
		&& ScreenRect.top<=  centery && centery <= ScreenRect.bottom)return TRUE;
	}

	return FALSE;

}



// ���� �ɴ´�. 
void _Map::PutFire(SI32 siX, SI32 siY, SI32 siFireType, SI32 siLifeTime,_NewID siChar,SI16 siPlayer)
{
	// ��ȿ���� ���� �������� ���� ���� �� ����. 
	if(IsInMapArea(siX, siY) == FALSE)return ;

	// ���� ���� �� �ִ� ������ Ȯ���Ѵ�. 
	// ���� ��. ���� ��. ��� ���� ���� �� ����.  
	if( pCollMap )
	{
		if( pCollMap[siX+YOff[siY]] & (BLOCK_SHALLOWWATER|BLOCK_DEEPWATER|BLOCK_NORMAL|BLOCK_CASTLE|BLOCK_NEWBLK) )
		{
			return;
		}
	}

	// �� �����̸� ���� ���� �ʴ´�. 
	if( MH.MapType == MAP_TYPE_SNOW )	return;

	if( GetCastleFile(siX/2, siY/2) )	return;


	SetFireMap(siX, siY,  ((siFireType<<5) | siLifeTime));
	SetFireCharMap(siX,siY,siChar,siPlayer,TRUE);

	PushEffect(EFFECT_FIRE, siX, siY);

}

// ���� ������ �ٿ� ������. 
BOOL _Map::DecreaseFireLifeTime(SI32 siX, SI32 siY, SI32 siDecreaseFactor)
{
	UI08 siLifeTime = (GetFireMap(siX, siY) & 0x1f);

	if(siLifeTime)
	{

		if(siDecreaseFactor >= siLifeTime)
		{
			_NewID zeroID;
			zeroID.ID = -1;
			zeroID.Unique = 0;
			SetFireMap(siX, siY,  0);
			SetFireCharMap(siX,siY,zeroID,-1,FALSE);
			
			

			SI32 smokeid = FindEmptySmoke();
			if(smokeid)
				SetSmoke(KIND_SMOKE_GROUNDFIRESMOKE, smokeid, MapCenterX[siX][siY], MapCenterY[siX][siY], AREA_LAND);

			return TRUE;
		}
		else
		{
			
			siLifeTime-= (UI08)siDecreaseFactor;

			SetFireMap(siX, siY,  (GetFireMap(siX, siY) & 0xe0));
			SetFireMap(siX, siY,  (GetFireMap(siX, siY) | siLifeTime) );

		}

	}
	return FALSE;

}



