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


extern	RECT				ScreenRect;							// 지도가 표시되는 영역 
extern	int 				MapCenterX[][MAX_MAP_YSIZE];		// 지도의 각 좌표마다의 중심값 
extern	int 				MapCenterY[][MAX_MAP_YSIZE];
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 
extern	char 				SavePath[];
extern	_Map 				Map;
extern	_MainData			GD;									// 게임의 주데이터 
extern	_YSMap 				SMap;								// 작은 지도용 데이터 

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

// MapByRealX[], MapByRealY[] 변수를 업데이트한다. 
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
				
		    // 보정 루틴 
		    if(smallx<0)smallx=0;
			if(smallx>=MH.MapXsize)smallx=MH.MapXsize-1;

			if(smally<0)smally=0;
			if(smally>=MH.MapYsize)smally=MH.MapYsize-1;

			MapByRealX[j/MAP_BY_REAL_DIV][i/MAP_BY_REAL_DIV+200/MAP_BY_REAL_DIV]=(unsigned char)smallx;
            MapByRealY[j/MAP_BY_REAL_DIV][i/MAP_BY_REAL_DIV+200/MAP_BY_REAL_DIV]=(unsigned char)smally;
		}





}





// 지도상의 X, Y좌표의 중심의 좌표를 돌려준다. (절대좌표)
BOOL _Map::GetRealDotCenterXY(SHORT x, SHORT y, int & centerx, int & centery)
{
	if(IsInMapArea(x, y) == FALSE)return FALSE;


	// 높이를 구한다. 
	int height=GetLevel(x, y) * HEIGHT_PER_LEVEL;

	// 언덕 타일의 종류를 구한다. 
	int hilltype=GetHillType(x, y);
	
	// 스크린 쿼터의 중심 좌표를 구한다. 
	centerx = x * MAP_TILE_XSIZE_HALF
		     -y * MAP_TILE_XSIZE_HALF ;

	centery = y * MAP_TILE_YSIZE_HALF
		     +x * MAP_TILE_YSIZE_HALF ;


	if(GetLevelInfo(x, y)==HILL_PLANE)  // 평면 
	{
	    centery = centery-(GetLevel(x,y)-1)*HEIGHT_PER_LEVEL      + Tile[hilltype].Y;
	}
    else                    // 사면 
       centery = centery -(abs(GetLevel(x,y) ) )*HEIGHT_PER_LEVEL + Tile[hilltype].Y;


	return TRUE;

}

// 지도상의 X, Y좌표의 위치를 돌려준다. 출력의 기준점이 된다. (화면좌표)
// x, y :기본 좌표 
BOOL _Map::GetRefXY(SHORT x, SHORT y, SHORT & centerx, SHORT & centery)
{


	if(IsInMapArea(x, y) == FALSE)return FALSE;

	// 높이를 구한다. 
	int height=GetLevel(x,y) * HEIGHT_PER_LEVEL;

	// 언덕 타일의 종류를 구한다. 
	int hilltype=GetHillType(x, y);
	
	// 스크린 쿼터의 중심 좌표를 구한다. 
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

// 지도상의 X, Y좌표의 중심의 좌표를 돌려준다. (화면좌표)
// x, y :기본 좌표 
BOOL _Map::GetCenterXY(SHORT x, SHORT y, SHORT & centerx, SHORT & centery)
{

	if(IsInMapArea(x, y) == FALSE)return FALSE;

	// 높이를 구한다. 
	int height=GetLevel(x,y)*HEIGHT_PER_LEVEL;

	// 언덕 타일의 종류를 구한다. 
	int hilltype=GetHillType(x, y);
	

	// 스크린 쿼터의 중심 좌표를 구한다. 
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

	if(GetLevelInfo(x, y)==HILL_PLANE)  // 평면 
	{
	    centery = centery-(GetLevel(x,y)-1)*HEIGHT_PER_LEVEL      + Tile[hilltype].Y;
	}
    else                    // 사면 
       centery = centery -(abs(GetLevel(x,y) ) )*HEIGHT_PER_LEVEL + Tile[hilltype].Y;

	return TRUE;
}

// 지도상의 X, Y좌표의 중심의 좌표를 돌려준다. (화면좌표)
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


// 절대 좌표를 화면 좌표로 변환한다.
void _Map::TransFromRealDotToScreenDot(SHORT realx, SHORT realy, SHORT& screenx, SHORT &screeny)
{

	screenx=realx+ScreenRect.left+(ScreenRect.right-ScreenRect.left+1)/2+(-VirtualMapX*MAP_TILE_XSIZE_HALF+VirtualMapY*MAP_TILE_XSIZE_HALF);
	screeny=realy+ScreenRect.top+(ScreenRect.bottom-ScreenRect.top+1)/2 +(-VirtualMapY*MAP_TILE_YSIZE_HALF-VirtualMapX*MAP_TILE_YSIZE_HALF);
}

// 화면 좌표를 절대 좌표로 변환한다.
void _Map::TransFromScreenToRealXY(SHORT screenx, SHORT screeny, SHORT& realx, SHORT &realy)
{
	realx=screenx-(ScreenRect.left+(ScreenRect.right-ScreenRect.left+1)/2)-(-VirtualMapX*MAP_TILE_XSIZE_HALF+VirtualMapY*MAP_TILE_XSIZE_HALF);
	realy=screeny-(ScreenRect.top+(ScreenRect.bottom-ScreenRect.top+1)/2)-(-VirtualMapY*MAP_TILE_YSIZE_HALF-VirtualMapX*MAP_TILE_YSIZE_HALF);
}




// 캐릭터의 움직임으로 인한 삭제 
// 상위 4비트를 삭제한다. 
// 하위 12비트는 지도 정보다. 
BOOL _Map::DeleteCharCollMap(SHORT x, SHORT y)
{
	if( pCollMap == NULL )	return TRUE;
	if( IsInMapArea(x, y) == FALSE )	return FALSE;

	pCollMap[x+YOff[y]] &= 0x0fff;
	return TRUE;
}

// 비행체의 움직임으로 인한 삭제 
BOOL _Map::DeleteAirCharCollMap(SHORT x, SHORT y)
{
	if( pCollMap == NULL )	return TRUE;
	if( IsInMapArea(x, y) == FALSE )	return FALSE;
	
	pCollMap[x+YOff[y]] &= 0xf0ff;
	return TRUE;
}



// 절대 좌표를 근거로 해서 기본 좌표를 구한다. 
BOOL _Map::DecideMapXYByXY(SHORT x, SHORT y, SHORT& map_x, SHORT& map_y, SHORT & map_center_x, SHORT & map_center_y)
{

	int i, j;
	int centerx, centery;

	// 마우스의 좌표와 가장 가까운 중심의 좌표를 구한다. 
	int length;
	int min_length=100;

	SHORT virtualmapx=0;
	SHORT virtualmapy=0;

	// 언덕을 고려하지 않고 기본 좌표를 산술적으로 구한다. 
	virtualmapx = (x/2+y)/MAP_TILE_XSIZE_HALF;
	virtualmapy = (-x/2+y)/MAP_TILE_XSIZE_HALF;


	// 언덕이 있기 때문에 인근의 몇 타일을 체크하여 실제적인 좌표를 구한다.
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

		// 가까운 곳을 찾지 못했다면, 
		if(min_length==100)return FALSE;

		return TRUE;
}

// 절대 좌표를 근거로 해서 기본 좌표를 구한다. 
BOOL _Map::DecideMapXYByXYLow(SHORT x, SHORT y, SHORT& map_x, SHORT& map_y, SHORT & map_center_x, SHORT & map_center_y)
{

	int i, j;

	// 마우스의 좌표와 가장 가까운 중심의 좌표를 구한다. 
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


// mousex, mousey가 클라이언트 영역에 있는가?
BOOL _Map::IsPtInMap(SHORT mouse_x, SHORT mouse_y)
{

	if(mouse_x>=ScreenRect.left && mouse_x<=ScreenRect.right
	&& mouse_y>=ScreenRect.top  && mouse_y<=ScreenRect.bottom)
	{
		return TRUE;
	}

	return FALSE;
}







// x, y위치의 타일의 화일번호와 폰트 번호를 구한다. 
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


// 지도의 플레이어 수를 알려준다. (임진록2에만 해당하는 코드이다. 조반, 천년에는 지도에 헤더가 포함되어 있어 헤더를 읽기만 하면 되지만 
// 임2 예전 버전은 아래 함수를 사용해야만 한다.)
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

	// 지도를 초기화한다. 
	strcpy(buffer1, "cusmap\\");
	strcat(buffer1, filename);
	GetFileNamePath(buffer1, SavePath, buffer);

	fp=fopen(buffer, "rb");
	if(fp==0)return 0;

	


	// 화일의 사이즈를 구한다. (예전 버전인지 신 버전인지 구별하기 위함. )
	siSize = filelength(fileno(fp));

	// 플레이어 시작 위치정보로 이동한다. 

	// 구버전이냐 신버전이냐에 따라 방식이 다르다. 

#ifdef _IMJIN2
	// 조선의 반격이면, 
#ifdef _IMJIN2PLUS
	// 구버전이면, 
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
	// 신버전이면, 
	else
	{
		fclose(fp);

		// 헤더를 읽어서 그 헤더안에 있는 정보를 얻는다. 
		if(Map.GetMapInfo(buffer, &mapheader) == TRUE)
		{
			return mapheader.PlayerNumber;
		}
	}

	// 임진록이면, 
#else

	// 구버전이면, 
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

		// 헤더를 읽어서 그 헤더안에 있는 정보를 얻는다. 
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

// 데모용 지도인지 확인한다.
BOOL IsMapForDemo(char* filename_ext)
{
	SHORT i;
	SHORT count=0;
	int start_pos_add=0;
	char buffer[FILE_NAME_SIZE];
	char buffer1[FILE_NAME_SIZE];
	
	SHORT return_value = TRUE;


#ifdef TYPE_YONSEI
	// 화일 이름이 데모용 화일이 아니면 FALSE
	if(strcmp(filename_ext, "(4명,100X100)연세할리우드.map") != 0)
	{
		return FALSE;
	}

	return TRUE;

#endif


	_Map* map= NULL;

	// 지도용 메모리를 할당한다. 
	map= new _Map;
	if(map == NULL) return FALSE;
	else 
	{
		// 0으로 초기화한다.
		ZeroMemory(map, sizeof(_Map));
	}


	// 지도를 초기화한다. 
	strcpy(buffer1, "cusmap\\");
	strcat(buffer1, filename_ext);
	GetFileNamePath(buffer1, SavePath, buffer);

	// 지도를 불러온다. 
	if(map->Load(buffer) == FALSE) 
	{
		delete map;
		return FALSE;
	}

	// 시작위치의 갯수를 구한다. 
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


// x, y좌표가 화면안에 있는지 확인한다.
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



// 불을 심는다. 
void _Map::PutFire(SI32 siX, SI32 siY, SI32 siFireType, SI32 siLifeTime,_NewID siChar,SI16 siPlayer)
{
	// 유효하지 않은 지역에는 불을 붙일 수 없다. 
	if(IsInMapArea(siX, siY) == FALSE)return ;

	// 불이 붙을 수 있는 곳인지 확인한다. 
	// 얕은 물. 깊은 물. 등에는 불을 붙일 수 없다.  
	if( pCollMap )
	{
		if( pCollMap[siX+YOff[siY]] & (BLOCK_SHALLOWWATER|BLOCK_DEEPWATER|BLOCK_NORMAL|BLOCK_CASTLE|BLOCK_NEWBLK) )
		{
			return;
		}
	}

	// 눈 지형이면 불이 붙지 않는다. 
	if( MH.MapType == MAP_TYPE_SNOW )	return;

	if( GetCastleFile(siX/2, siY/2) )	return;


	SetFireMap(siX, siY,  ((siFireType<<5) | siLifeTime));
	SetFireCharMap(siX,siY,siChar,siPlayer,TRUE);

	PushEffect(EFFECT_FIRE, siX, siY);

}

// 불의 수명을 줄여 나간다. 
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



