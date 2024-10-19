#ifndef _ROCKET_NEW_TILE_MAP
//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>
#include <main.h>

#ifdef _IMJIN2
	#include "..\Gersang\maindata.h"
#endif

#include "charanimation.h"
#include <kindinfo.h>
#include <myfile.h>

#include <map.h>
#include <volcano.h>


extern	_MainData		GD;
extern	int				MapCenterX[][MAX_MAP_YSIZE];		// 지도의 각 좌표마다의 중심값, 
extern	int				MapCenterY[][MAX_MAP_YSIZE];
extern	SHORT			MapEditMode;						// 지도 편집 모드인가?
extern	_KindInfo		KI[];								// 캐릭터 종류별 데이터 
extern	SHORT			BasicBlock;
extern	_Map			Map;

extern	int				TileSet[];
extern	int				BlockSet[];
extern	SHORT			Dir8Q[];
extern	unsigned char	NormalHillType[][MAX_TILE_VARY];
extern	unsigned char	GrassType[][15][MAX_GRASS_VARY];
extern	unsigned char	GreenType[][15][MAX_GREEN_VARY];
extern	unsigned char	CropAreaType[][15][MAX_CROPAREA_VARY];
extern	unsigned char	SeaType[][MAX_SEA_VARY*2];
extern	_Tile			Tile[];
extern	unsigned char	MapByRealX[][(MAX_MAP_YSIZE)*MAP_TILE_YSIZE/MAP_BY_REAL_DIV+200/MAP_BY_REAL_DIV];
extern	unsigned char	MapByRealY[][(MAX_MAP_YSIZE)*MAP_TILE_YSIZE/MAP_BY_REAL_DIV+200/MAP_BY_REAL_DIV];

void _Map::InitMap(SHORT xsize, SHORT ysize, BOOL bvillagemapswitch)
{
	int i, j;

	// 0으로 초기화한다. 
	ZeroMemory(this, sizeof(_Map));

	MH.Version = 2;
	MH.MapType=MAP_TYPE_NORMAL;

	// 기본적으로 모든 캐릭터가 게임에 등장한다. 
	for(i=0;i<MAX_MULTI_PLAYER;i++)
		for(j=0;j<KIND_ON_MAX;j++)
		{
		   clMapPlayerInfo[i].IsCharInGame[j]=TRUE;
		}

	// 플레이어 정보를 초기화한다. 
	for(i=0;i<MAX_MULTI_PLAYER;i++)
	{
		clMapPlayerInfo[i].PI.Init(i);

		clMapPlayerInfo[i].StartPoint.x=-1;
		clMapPlayerInfo[i].StartPoint.y=-1;
	}

	for(i=0;i<MAX_SHIP_PATROL_POINT_NUMBER;i++)
	{
		ShipPatrolPoint[i].x=-1;
		ShipPatrolPoint[i].y=-1;
	}

	for(i=0;i<MAX_SHIP_LANDING_POINT_NUMBER;i++)
	{
		ShipLandingPoint[i].x=-1;
		ShipLandingPoint[i].y=-1;
	}



	MapDrawSwitch=TRUE;
	DrawForceSwitch=TRUE;

	MH.MapXsize=xsize;
	MH.MapYsize=ysize;

	for(i=0;i<MH.MapYsize;i++)
	{
		YOff[i] = i* MH.MapXsize;
	}
	for(i=0;i < (MH.MapYsize/2);i++)
	{
		YOff2[i]	= i * (MH.MapXsize/2);
	}


	FreeMap();
	AllocMap(xsize, ysize, bvillagemapswitch);

	VirtualMapX = 0;
	VirtualMapY = 0;
	
	for(i=0;i<MH.MapYsize;i++)
		for(j=0;j<MH.MapXsize;j++)
		{
			if(pMineMap) SetMineMap(j, i, -1);
			
			if(pTileAtbMap) pTileAtbMap[j + YOff[i]] = TILE_DEFAULT;
			
			SetLevel(j, i, 0);
			SetLevelSlope(j, i, HILL_PLANE);
			
			if( pLevelFile )	pLevelFile[j + YOff[i]] = FileTileHill0;
			if( pLevelFont )	pLevelFont[j + YOff[i]] = NormalHillType[0][0];
			if( pLevelType )	pLevelType[j + YOff[i]] = 0;
		   
			SetLevelMode(0, j, i, 15);
		}
}



// 불러오거나 랜덤하게 생성된 지도로 최종적인 지도를 만든다. 
void _Map::MakeLastMap()
{
	int i, j;
	SHORT croptype;

	for(i=0;i<MH.MapYsize;i++)
		for(j=0;j<MH.MapXsize;j++)
		{
			GetRealDotCenterXY(j, i, MapCenterX[j][i], MapCenterY[j][i]);

			// 기존의 newblk정보를 삭제한다. 
			SetNewBlkLevel(0, j, i);
		}


	// 절대 좌표를 가지고 기본 좌표를 구한다. 
	UpdateMapByRealXY();

	// 모습은 물인데 실제는 물이 아닌곳이 많으므로 물전용 타일이 
	// 사용된 곳이면 물로 만든다. 
	for(i=0;i<MH.MapYsize;i++)
		for(j=0;j<MH.MapXsize;j++)
		{
			if(GetSeaFile(j/2, i/2) == FileTileSea0)
			{
				PutSea(j, i, TRUE, TRUE);
			}
		}


	if( pShallowMap )
	{
		// 얕은 물이 물 이외의 지역에 설정되어 있다면 물을 찍는다. 
		for(i=0;i<MH.MapYsize;i++)
			for(j=0;j<MH.MapXsize;j++)
			{
				if( pShallowMap[j + YOff[i]] && GetSeaMode(j, i)==FALSE )
				{
					PutSea(j, i, TRUE, TRUE);
				}
			}	
	}

		
	for(i=0;i<MH.MapYsize;i++)
		for(j=0;j<MH.MapXsize;j++)
		{
			// 성이 있은 경우 충돌체크하게 한다. 
			if(GetNewBlkFile(j/2, i/2))
			{
				PutCollNewBlk(j, i);
			}
			if(GetCastleFile(j/2, i/2))
			{
				// 주변의 상황을 고려하여 충돌체크를 한다. 
				PutCollCastle(j, i);
			}

			if( pShallowMap )
			{
				// 얕은 물인 경우 
				if( pShallowMap[j + YOff[i]] )
				{
					SetCollMap(j, i,	BLOCK_SHALLOWWATER);
				}
				// 바다가 있는 경우 
				else if( GetSeaMode(j, i) )
				{
					PutCollSea(j, i);
				}
				else 
				{
					SetCollMap(j, i,	BLOCK_LAND);
				}

			}
			// 기타 지형 지물을 찍는다. 
			croptype=GetCropTypeMap(j, i);
			
			// 곡물이면 아무 조치도 하지 않는다.
			if(croptype==CROP_TYPE_RICE || croptype==CROP_TYPE_POTATO || croptype==CROP_TYPE_TREE || croptype==CROP_TYPE_BAMBOO)
			{
			}
			else if(croptype)
			{
			   Map.PutRuin(j, i, croptype,  1);

#ifdef _IMJIN2
			   if(croptype == CROP_TYPE_VOLCANO )
			   {
				   SetVolcano(j, i);
			   }
#endif
			}
		}
		
		
	// 잘못찍힌 나무를 정리한다. 
	for(i=0;i<MH.MapYsize;i++)
		for(j=0;j<MH.MapXsize;j++)
		{
			if(GetCropTypeMap(j, i)==CROP_TYPE_TREE || GetCropTypeMap(j, i)==CROP_TYPE_BAMBOO)
			{
				// 지도 편집 모드에서는 임의대로 삭제하지 않는다.  
				if(MapEditMode==FALSE && IsGoodForTree(j, i)==FALSE)
				{
					PutTree(j, i, 0);
				}
				else 
				{
					if( pCollMap )
						pCollMap[j+YOff[i]] |= BLOCK_TREE;
				}
			}
		}

	// 최종 높이를 구한다. 
	for(i=0;i<MH.MapYsize;i++)
		for(j=0;j<MH.MapXsize;j++)
		{
			GD.LastLevel[j][i] = (UI08)GetLastLevel(j, i);
		}

}



BOOL _Map::IncreaseVirtualMapX(int mount)
{

	if((VirtualMapX+mount)<MH.MapXsize)
	{
		VirtualMapX+=mount;
		return TRUE;
	}
	return FALSE;
}

BOOL _Map::DecreaseVirtualMapX(int mount)
{
	if(VirtualMapX-mount>=0)
	{
		VirtualMapX-=mount;
		return TRUE;
	}

	return FALSE;
}

BOOL _Map::IncreaseVirtualMapY(int mount)
{

	if((VirtualMapY+mount)<MH.MapYsize)
	{
		VirtualMapY+=mount;
		return TRUE;
	}
	return FALSE;
}

BOOL _Map::DecreaseVirtualMapY(int mount)
{
	if(VirtualMapY-mount>=0)
	{
		VirtualMapY-=mount;
		return TRUE;
	}

	return FALSE;
}

SHORT _Map::GetVirtualMapX()
{
	return VirtualMapX;
}

SHORT _Map::GetVirtualMapY()
{
	return VirtualMapY;
}


// 지상이 비어 있는가?
// 지상 캐릭터가 있는곳, 물, 일반 장애물
BOOL _Map::IsEmpty(SHORT x, SHORT y, SHORT xsize, SHORT ysize, SHORT idx, SHORT idy, SHORT idxsize, SHORT idysize)
{
	SHORT i, j;
	SHORT tempx, tempy;

	// 지도 영역 안에 있는지 확인한다. 
	if(x<0 || x+xsize-1>=MH.MapXsize)return FALSE;
	if(y<0 || y+ysize-1>=MH.MapYsize)return FALSE;
	
	for(i=0;i<ysize;i++)
		for(j=0;j<xsize;j++)
		{
			tempx=x-xsize/2+j;
			tempy=y-ysize/2+i;

			if( (idx-idxsize/2)<=tempx && tempx<(idx-idxsize/2+idxsize) 
			&&  (idy-idysize/2)<=tempy && tempy<(idy-idysize/2+idysize) )return FALSE;


			if(IsEmpty(tempx, tempy)==FALSE)return FALSE;
		}

	return TRUE;
}

// 비어 있는가?
// 지상 캐릭터가 있는곳, 물, 일반 장애물
BOOL _Map::IsEmptyWater(SHORT x, SHORT y, SHORT xsize, SHORT ysize, SHORT idx, SHORT idy, SHORT idxsize, SHORT idysize)
{
	SHORT i, j;
	SHORT tempx, tempy;

	// 지도 영역 안에 있는지 확인한다. 
	if(x<0 || x+xsize-1>=MH.MapXsize)return FALSE;
	if(y<0 || y+ysize-1>=MH.MapYsize)return FALSE;

	for(i=0;i<ysize;i++)
		for(j=0;j<xsize;j++)
		{
			tempx=x-xsize/2+j;
			tempy=y-ysize/2+i;
			
			if( (idx-idxsize/2)<=tempx && tempx<(idx-idxsize/2+idxsize) 
			&&  (idy-idysize/2)<=tempy && tempy<(idy-idysize/2+idysize) )return FALSE;
			
			if(IsEmptyWater(tempx, tempy)==FALSE)return FALSE;
		}

	return TRUE;
}


// x,y에 건물을 지을 수 있는 곳인가?
// x,y 기본 좌표 
BOOL _Map::CanBuild(SHORT x,SHORT y, _NewID builderid, SHORT kind)
{
	int block;

	if(x<0  || x>=MH.MapXsize)return FALSE;
	if(y<0  || y>=MH.MapYsize)return FALSE;

	// 캐릭터가 있는곳
	SI32 id = GetIDMap(x, y); 
	if(id && id!=builderid.ID)
		return FALSE;
	
	// 곡물이 있는곳 
	// 목재는 
	if(GetCropTypeMap(x, y)==CROP_TYPE_TREE
	|| GetCropTypeMap(x, y)==CROP_TYPE_BAMBOO)
	{
		// 남은 양이 있으면 
		if(GetCropMountMap(x, y))return FALSE;
	}
#ifdef _IMJIN2
	// 바닥에 붙어 있는 광고면, 
	else if(GetCropTypeMap(x, y) == CROP_TYPE_AD12)
	{
	}
#endif
	// 기타 곡물은 무조건 불가. 
	else if(GetCropTypeMap(x, y))
	{
		return FALSE;
	}

	block=BLOCK_DEEPWATER|BLOCK_CASTLE|BLOCK_NORMAL|BLOCK_RUIN|BLOCK_NEWBLK|BLOCK_TREE|BLOCK_SHALLOWWATER|BLOCK_MOVE|BLOCK_WALL;

	if( pCollMap )
		if( pCollMap[x+YOff[y]] & block )	return FALSE;

	return TRUE;
}


void _Map::MakeSmallMapBase()
{
	SHORT i, j;

	for(i=0;i<MH.MapYsize;i++)
		for(j=0;j<MH.MapXsize;j++)
		{
			UpdateSmallMapBase(j, i);
		}
}

// 절대 좌표를 입력하면 기본 좌표가 나온다. 
// realx, realy :절대 좌표 
// x, y :기본 좌표 
BOOL _Map::GetRealTileByRealDot(SHORT realx, SHORT realy, SHORT* pX, SHORT* pY)
{
	// 전체 가로 사이즈를 구한다. 
	SHORT xsize=MH.MapXsize*MAP_TILE_XSIZE/MAP_BY_REAL_DIV;

	int posx=realx/MAP_BY_REAL_DIV+xsize/2;
	int posy=realy/MAP_BY_REAL_DIV+200/MAP_BY_REAL_DIV;

	if(posx<0)posx=0;
	if(posx>=(MAX_MAP_XSIZE)*MAP_TILE_XSIZE/MAP_BY_REAL_DIV)
		posx=(MAX_MAP_XSIZE)*MAP_TILE_XSIZE/MAP_BY_REAL_DIV-1;
	if(posy<0)posy=0;
	if(posy>=(MAX_MAP_YSIZE)*MAP_TILE_YSIZE/MAP_BY_REAL_DIV+200/MAP_BY_REAL_DIV)
		posy=posy>=(MAX_MAP_YSIZE)*MAP_TILE_YSIZE/MAP_BY_REAL_DIV+200/MAP_BY_REAL_DIV-1;

	*pX = MapByRealX[posx][posy];
    *pY = MapByRealY[posx][posy];

	if((*pX) < 0 || (*pY) < 0)return FALSE;

	return TRUE;
}


void _Map::AllocMap(SHORT xsize, SHORT ysize, BOOL bvillagemapswitch)
{
	SI32 i;

	pIDMap = new short[xsize * ysize];
	if( pIDMap )
		ZeroMemory(pIDMap, sizeof(short) * xsize * ysize);



	pSkyMap = new short[xsize * ysize];

	if(pSkyMap)
		ZeroMemory(pSkyMap, sizeof(short) * xsize * ysize);

	pCollMap = new short[xsize * ysize];
	if( pCollMap )	ZeroMemory(pCollMap, sizeof(short)*xsize*ysize);

	pBarrierMap = new short[xsize * ysize];
	if( pBarrierMap )	ZeroMemory(pBarrierMap, sizeof(short)*xsize*ysize);

	pLevelInfo = new char[xsize * ysize];
	if( pLevelInfo )	ZeroMemory(pLevelInfo, sizeof(char)*xsize*ysize);


	pLevelFile = new unsigned char[xsize * ysize];
	if( pLevelFile )	ZeroMemory(pLevelFile, sizeof(unsigned char)*xsize*ysize);


	pLevelFont = new unsigned char[xsize * ysize];
	if( pLevelFont )	ZeroMemory(pLevelFont, sizeof(unsigned char)*xsize*ysize);


	pLevelType = new unsigned char[xsize * ysize];
	if( pLevelType )	ZeroMemory(pLevelType, sizeof(unsigned char)*xsize*ysize);


	pTopLevel = new unsigned char[xsize * ysize];
	if( pTopLevel )		ZeroMemory(pTopLevel, sizeof(unsigned char)*xsize*ysize);


	for(i = 0;i < MAX_LEVEL_STEP;i++)
	{
		pLevelMode[i] = new unsigned char[xsize * ysize];
		if(pLevelMode[i])ZeroMemory(pLevelMode[i], sizeof(unsigned char) * xsize * ysize);
	}

	pTileAtbMap = new short[xsize * ysize];
	if( pTileAtbMap )	ZeroMemory(pTileAtbMap, sizeof(short)*xsize*ysize);


	pShallowMap = new unsigned char[xsize * ysize];
	if( pShallowMap)	ZeroMemory(pShallowMap, sizeof(unsigned char)*xsize*ysize);


	pShallowFont = new unsigned char[xsize * ysize];
	if( pShallowFont )	ZeroMemory(pShallowFont, sizeof(unsigned char)*xsize*ysize);


//	pSectorMap = new unsigned char[xsize * ysize];
//	if( pSectorMap)	ZeroMemory(pSectorMap, sizeof(unsigned char)*xsize*ysize);

	pSeaMode = new unsigned char[xsize * ysize];
	if( pSeaMode)	ZeroMemory(pSeaMode, sizeof(unsigned char)*xsize*ysize);
	pSeaFont = new unsigned char[(xsize/2) * (ysize/2)];
	if( pSeaFont)	ZeroMemory(pSeaFont, sizeof(unsigned char)*(xsize/2)*(ysize/2));
	pSeaFile = new unsigned char[(xsize/2) * (ysize/2)];
	if( pSeaFile)	ZeroMemory(pSeaFile, sizeof(unsigned char)*(xsize/2)*(ysize/2));


	pNewBlkMode = new unsigned char[xsize * ysize];
	if( pNewBlkMode)	ZeroMemory(pNewBlkMode, sizeof(unsigned char)*xsize*ysize);
	pNewBlkFont = new unsigned char[(xsize/2) * (ysize/2)];
	if( pNewBlkFont)	ZeroMemory(pNewBlkFont, sizeof(unsigned char)*(xsize/2)*(ysize/2));
	pNewBlkFile = new unsigned char[(xsize/2) * (ysize/2)];
	if( pNewBlkFile)	ZeroMemory(pNewBlkFile, sizeof(unsigned char)*(xsize/2)*(ysize/2));
	pNewBlkLevel = new unsigned char[xsize * ysize];
	if( pNewBlkLevel)	ZeroMemory(pNewBlkLevel, sizeof(unsigned char)*xsize*ysize);

	pCastleMode = new unsigned char[xsize * ysize];
	if( pCastleMode)	ZeroMemory(pCastleMode, sizeof(unsigned char)*xsize*ysize);
	pCastleFont = new unsigned char[(xsize/2) * (ysize/2)];
	if( pCastleFont)	ZeroMemory(pCastleFont, sizeof(unsigned char)*(xsize/2)*(ysize/2));
	pCastleFile = new unsigned char[(xsize/2) * (ysize/2)];
	if( pCastleFile)	ZeroMemory(pCastleFile, sizeof(unsigned char)*(xsize/2)*(ysize/2));

	pCropTypeMap = new unsigned char[xsize * ysize];
	if( pCropTypeMap)	ZeroMemory(pCropTypeMap, sizeof(unsigned char)*xsize*ysize);
	pCropVaryMap = new unsigned char[xsize * ysize];
	if( pCropVaryMap)	ZeroMemory(pCropVaryMap, sizeof(unsigned char)*xsize*ysize);
	pCropMountMap = new unsigned char[xsize * ysize];
	if( pCropMountMap)	ZeroMemory(pCropMountMap, sizeof(unsigned char)*xsize*ysize);
	pCropStepMap = new unsigned char[xsize * ysize];
	if( pCropStepMap)	ZeroMemory(pCropStepMap, sizeof(unsigned char)*xsize*ysize);

	pTreasureMap = new SHORT[xsize * ysize];
	if( pTreasureMap)	ZeroMemory(pTreasureMap, sizeof(SHORT)*xsize*ysize);

	pMineMap = new char[xsize * ysize];
	if( pMineMap)	ZeroMemory(pMineMap, sizeof(char)*xsize*ysize);

//	pMineCharMap = new SI32[xsize * ysize];
//	if(pMineCharMap) ZeroMemory(pMineCharMap,sizeof(SI32)*xsize*ysize);

	pFireMap = new UI08[xsize * ysize];
	if( pFireMap)	ZeroMemory(pFireMap, sizeof(UI08)*xsize*ysize);

	pFireCharMap = new _NewID[xsize * ysize];
	if(pFireCharMap) ZeroMemory(pFireCharMap, sizeof(_NewID)*xsize*ysize);

}

void _Map::FreeMap()
{
	SI32 i;
	if( pIDMap )
	{
		delete[] pIDMap;
		pIDMap = NULL;
	}

	if( pSkyMap )
	{
		delete[] pSkyMap;
		pSkyMap = NULL;
	}

	
	if( pBarrierMap )
	{
		delete[] pBarrierMap;
		pBarrierMap = NULL;
	}

	if( pCollMap )
	{
		delete[] pCollMap;
		pCollMap = NULL;
	}

	if( pLevelInfo )
	{
		delete[] pLevelInfo;
		pLevelInfo = NULL;
	}
	if( pLevelFile )
	{
		delete[] pLevelFile;
		pLevelFile = NULL;
	}
	if( pLevelFont )
	{
		delete[] pLevelFont;
		pLevelFont = NULL;
	}
	if( pLevelType )
	{
		delete[] pLevelType;
		pLevelType = NULL;
	}
	if( pTopLevel )
	{
		delete[] pTopLevel;
		pTopLevel = NULL;
	}

	for( i=0; i<MAX_LEVEL_STEP; i++ )
	{
		if( pLevelMode[i] )
		{
			delete[] pLevelMode[i];
			pLevelMode[i] = NULL;
		}
	}
	
	if( pTileAtbMap )
	{
		delete[] pTileAtbMap;
		pTileAtbMap = NULL;
	}


	if( pShallowMap )
	{
		delete[] pShallowMap;
		pShallowMap = NULL;
	}

	if( pShallowFont )
	{
		delete[] pShallowFont;
		pShallowFont = NULL;
	}

/*	if( pSectorMap )
	{
		delete[] pSectorMap;
		pSectorMap = NULL;
	}

*/
	if( pSeaMode )
	{
		delete[] pSeaMode;
		pSeaMode = NULL;
	}
	if( pSeaFont )
	{
		delete[] pSeaFont;
		pSeaFont = NULL;
	}
	if( pSeaFile )
	{
		delete[] pSeaFile;
		pSeaFile = NULL;
	}

	if( pNewBlkMode )
	{
		delete[] pNewBlkMode;
		pNewBlkMode = NULL;
	}
	if( pNewBlkFont )
	{
		delete[] pNewBlkFont;
		pNewBlkFont = NULL;
	}
	if( pNewBlkFile )
	{
		delete[] pNewBlkFile;
		pNewBlkFile = NULL;
	}
	if( pNewBlkLevel )
	{
		delete[] pNewBlkLevel;
		pNewBlkLevel = NULL;
	}

	
	if( pCastleMode )
	{
		delete[] pCastleMode;
		pCastleMode = NULL;
	}
	if( pCastleFont )
	{
		delete[] pCastleFont;
		pCastleFont = NULL;
	}
	if( pCastleFile )
	{
		delete[] pCastleFile;
		pCastleFile = NULL;
	}

	
	if( pCropTypeMap )
	{
		delete[] pCropTypeMap;
		pCropTypeMap = NULL;
	}
	if( pCropVaryMap )
	{
		delete[] pCropVaryMap;
		pCropVaryMap = NULL;
	}
	if( pCropMountMap )
	{
		delete[] pCropMountMap;
		pCropMountMap = NULL;
	}
	if( pCropStepMap )
	{
		delete[] pCropStepMap;
		pCropStepMap = NULL;
	}

	if(pTreasureMap )
	{
		delete[] pTreasureMap ;
		pTreasureMap = NULL;
	}

	if(pMineMap )
	{
		delete[] pMineMap ;
		pMineMap 	= NULL;
	}

//	if(pMineCharMap)
//	{
//		delete[] pMineCharMap;
//		pMineCharMap = NULL;
//	}


	if(pFireMap )
	{
		delete[] pFireMap ;
		pFireMap = NULL;
	}

	if(pFirePlayerMap)
	{
		delete[] pFirePlayerMap ;
		pFirePlayerMap = NULL;
	}

	if(pFireCharMap)
	{
		delete[] pFireCharMap ;
		pFireCharMap = NULL;
	}


}

void _Map::SetMapDrawSwitch(int mode)
{
	MapDrawSwitch=mode;
}



void _Map::PutTreasure(SI32 x, SI32 y, SHORT type, SI32 amount)
{
	Map.SetTreasureMap(x, y, ((type << 8) | amount) );
}

void _Map::DeleteTreasure(SI32 x, SI32 y)
{
	Map.SetTreasureMap(x, y, 0);
}



// 각 플레이어의 시작 위치를 랜덤하게 바꾼다. 
void _Map::RandomStartPos()
{
	SI32 i;

	POINT temp_point[MAX_MULTI_PLAYER];
	SHORT temp_point_index=0;

	for(i=0;i<Map.ExpansionBaseNumber;i++)
	{
		temp_point[i]=Map.ExpansionBasePoint[i];
	}
	temp_point_index=Map.ExpansionBaseNumber;

	SHORT index=0;
	SHORT outindex=0;
	while(temp_point_index)
	{
		
	   outindex=GD.Random()%temp_point_index;

       Map.clMapPlayerInfo[index].StartPoint=temp_point[outindex];
	   temp_point[outindex]=temp_point[temp_point_index-1];

	   index++;
	   temp_point_index--;
	}

}

#else
//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>
#include <main.h>

#ifdef _IMJIN2
	#include "..\Gersang\maindata.h"
#endif

#include "charanimation.h"
#include <kindinfo.h>
#include <myfile.h>

#include <map.h>
#include <volcano.h>

#include "BindJXFile.h"

extern _FileVary				FV;


extern	_MainData		GD;
extern	int				MapCenterX[][MAX_MAP_YSIZE];		// 지도의 각 좌표마다의 중심값, 
extern	int				MapCenterY[][MAX_MAP_YSIZE];
extern	SHORT			MapEditMode;						// 지도 편집 모드인가?
extern	_KindInfo		KI[];								// 캐릭터 종류별 데이터 
extern	SHORT			BasicBlock;
extern	_Map			Map;

extern	int				TileSet[];
extern	int				BlockSet[];
extern	SHORT			Dir8Q[];
extern	unsigned char	NormalHillType[][MAX_TILE_VARY];
extern	unsigned char	GrassType[][15][MAX_GRASS_VARY];
extern	unsigned char	GreenType[][15][MAX_GREEN_VARY];
extern	unsigned char	CropAreaType[][15][MAX_CROPAREA_VARY];
extern	unsigned char	SeaType[][MAX_SEA_VARY*2];
extern	_Tile			Tile[];
extern	unsigned char	MapByRealX[][(MAX_MAP_YSIZE)*MAP_TILE_YSIZE/MAP_BY_REAL_DIV+200/MAP_BY_REAL_DIV];
extern	unsigned char	MapByRealY[][(MAX_MAP_YSIZE)*MAP_TILE_YSIZE/MAP_BY_REAL_DIV+200/MAP_BY_REAL_DIV];

void _Map::InitMap(SHORT xsize, SHORT ysize, BOOL bvillagemapswitch)
{
	int i, j;

	// 0으로 초기화한다. 
	ZeroMemory(this, sizeof(_Map));

	MH.Version = 2;
	MH.MapType=MAP_TYPE_NORMAL;

	// 기본적으로 모든 캐릭터가 게임에 등장한다. 
	for(i=0;i<MAX_MULTI_PLAYER;i++)
		for(j=0;j<KIND_ON_MAX;j++)
		{
		   clMapPlayerInfo[i].IsCharInGame[j]=TRUE;
		}

	// 플레이어 정보를 초기화한다. 
	for(i=0;i<MAX_MULTI_PLAYER;i++)
	{
		clMapPlayerInfo[i].PI.Init(i);

		clMapPlayerInfo[i].StartPoint.x=-1;
		clMapPlayerInfo[i].StartPoint.y=-1;
	}

	for(i=0;i<MAX_SHIP_PATROL_POINT_NUMBER;i++)
	{
		ShipPatrolPoint[i].x=-1;
		ShipPatrolPoint[i].y=-1;
	}

	for(i=0;i<MAX_SHIP_LANDING_POINT_NUMBER;i++)
	{
		ShipLandingPoint[i].x=-1;
		ShipLandingPoint[i].y=-1;
	}



	MapDrawSwitch=TRUE;
	DrawForceSwitch=TRUE;

	MH.MapXsize=xsize;
	MH.MapYsize=ysize;

	for(i=0;i<MH.MapYsize;i++)
	{
		YOff[i] = i* MH.MapXsize;
	}
	for(i=0;i < (MH.MapYsize/2);i++)
	{
		YOff2[i]	= i * (MH.MapXsize/2);
	}


	FreeMap();
	AllocMap(xsize, ysize, bvillagemapswitch);

	VirtualMapX = 0;
	VirtualMapY = 0;
	
	for(i=0;i<MH.MapYsize;i++)
		for(j=0;j<MH.MapXsize;j++)
		{
			if(pMineMap) SetMineMap(j, i, -1);
			
			if(pTileAtbMap) pTileAtbMap[j + YOff[i]] = TILE_DEFAULT;
			
			SetLevel(j, i, 0);
			SetLevelSlope(j, i, HILL_PLANE);
			
			if( pLevelFile )	pLevelFile[j + YOff[i]] = FileTileHill0;
			if( pLevelFont )	pLevelFont[j + YOff[i]] = NormalHillType[0][0];
			if( pLevelType )	pLevelType[j + YOff[i]] = 0;
		   
			SetLevelMode(0, j, i, 15);
		}
		
	for( i =0 ; i < ysize; i+=2)
	{
		for( int j =0 ; j < xsize; j+=2)
		{
			int tmpTileType = rand()%6; //6가지 타일중 한가지 세트 선택.
			pLevelFont[xsize*i + j] = 0 + tmpTileType*4;
			pLevelFont[xsize*i + j +1] = 1+ tmpTileType*4;
			pLevelFont[xsize*(i+1) + j] = 2+ tmpTileType*4;
			pLevelFont[xsize*(i+1) + j+1] = 3+ tmpTileType*4;
		}
	}

}



// 불러오거나 랜덤하게 생성된 지도로 최종적인 지도를 만든다. 
void _Map::MakeLastMap()
{
	int i, j;
	SHORT croptype;

	for(i=0;i<MH.MapYsize;i++)
		for(j=0;j<MH.MapXsize;j++)
		{
			GetRealDotCenterXY(j, i, MapCenterX[j][i], MapCenterY[j][i]);

			// 기존의 newblk정보를 삭제한다. 
			SetNewBlkLevel(0, j, i);
		}


	// 절대 좌표를 가지고 기본 좌표를 구한다. 
	UpdateMapByRealXY();

	// 모습은 물인데 실제는 물이 아닌곳이 많으므로 물전용 타일이 
	// 사용된 곳이면 물로 만든다. 
	for(i=0;i<MH.MapYsize;i+=2)
		for(j=0;j<MH.MapXsize;j+=2)
		{
			if(GetSeaFile(j, i) == FileTileSea0)
			{
				//PutSea_Daesang(j, i, TRUE );
			}
		}
	/*
	for(i=0;i<MH.MapYsize;i++)
		for(j=0;j<MH.MapXsize;j++)
		{
			if(GetSeaFile(j/2, i/2) == FileTileSea0)
			{
				PutSea(j, i, TRUE, TRUE);
			}
		}
	*/


	if( pShallowMap )
	{
		// 얕은 물이 물 이외의 지역에 설정되어 있다면 물을 찍는다. 
		for(i=0;i<MH.MapYsize;i++)
			for(j=0;j<MH.MapXsize;j++)
			{
				if( pShallowMap[j + YOff[i]] && GetSeaMode(j, i)==FALSE )
				{
					//PutSea(j, i, TRUE, TRUE);
					PutSea_Daesang(j, i, TRUE);
				}
			}	
	}

		
	for(i=0;i<MH.MapYsize;i++)
		for(j=0;j<MH.MapXsize;j++)
		{
			// 성이 있은 경우 충돌체크하게 한다. 
			if(GetNewBlkFile(j/2, i/2))
			{
				PutCollNewBlk(j, i);
			}
			if(GetCastleFile(j/2, i/2))
			{
				// 주변의 상황을 고려하여 충돌체크를 한다. 
				PutCollCastle(j, i);
			}

			if( pShallowMap )
			{
				// 얕은 물인 경우 
				if( pShallowMap[j + YOff[i]] )
				{
					SetCollMap(j, i,	BLOCK_SHALLOWWATER);
				}
				// 바다가 있는 경우 
				else if( GetSeaMode(j, i) )
				{
					PutCollSea(j, i);
				}
				else 
				{
					SetCollMap(j, i,	BLOCK_LAND);
				}

			}
			// 기타 지형 지물을 찍는다. 
			croptype=GetCropTypeMap(j, i);
			
			// 곡물이면 아무 조치도 하지 않는다.
			if(croptype==CROP_TYPE_RICE || croptype==CROP_TYPE_POTATO || croptype==CROP_TYPE_TREE || croptype==CROP_TYPE_BAMBOO)
			{
			}
			else if(croptype)
			{
			   Map.PutRuin(j, i, croptype,  1);

#ifdef _IMJIN2
			   if(croptype == CROP_TYPE_VOLCANO )
			   {
				   SetVolcano(j, i);
			   }
#endif
			}
		}
		
		
	// 잘못찍힌 나무를 정리한다. 
	for(i=0;i<MH.MapYsize;i++)
		for(j=0;j<MH.MapXsize;j++)
		{
			if(GetCropTypeMap(j, i)==CROP_TYPE_TREE || GetCropTypeMap(j, i)==CROP_TYPE_BAMBOO)
			{
				// 지도 편집 모드에서는 임의대로 삭제하지 않는다.  
				if(MapEditMode==FALSE && IsGoodForTree(j, i)==FALSE)
				{
					//나무타입은 임시로 0
					PutTree(j, i, 0, 0);
				}
				else 
				{
					if( pCollMap )
						pCollMap[j+YOff[i]] |= BLOCK_TREE;

					if( GetCropTypeMap(j, i)==CROP_TYPE_TREE )
					{
						int BlockSize = FV. m_TreeImgFile[   pCropTreeMap[j+YOff[i]]    ].BlockSize;
						for( int yy = 0; yy < BlockSize; yy++)
						{
							for( int xx = 0; xx < BlockSize; xx++)
							{
								if( pCollMap )
								{
									if(IsInMapArea(xx+j, yy+i) == TRUE) 
										pCollMap[j+xx +YOff[i+yy]] |= BLOCK_TREE;
								}
							}
						}

					}
				}
			}
		}

	// 최종 높이를 구한다. 
	for(i=0;i<MH.MapYsize;i++)
		for(j=0;j<MH.MapXsize;j++)
		{
			GD.LastLevel[j][i] = (UI08)GetLastLevel(j, i);
		}

}



BOOL _Map::IncreaseVirtualMapX(int mount)
{

	if((VirtualMapX+mount)<MH.MapXsize)
	{
		VirtualMapX+=mount;
		return TRUE;
	}
	return FALSE;
}

BOOL _Map::DecreaseVirtualMapX(int mount)
{
	if(VirtualMapX-mount>=0)
	{
		VirtualMapX-=mount;
		return TRUE;
	}

	return FALSE;
}

BOOL _Map::IncreaseVirtualMapY(int mount)
{

	if((VirtualMapY+mount)<MH.MapYsize)
	{
		VirtualMapY+=mount;
		return TRUE;
	}
	return FALSE;
}

BOOL _Map::DecreaseVirtualMapY(int mount)
{
	if(VirtualMapY-mount>=0)
	{
		VirtualMapY-=mount;
		return TRUE;
	}

	return FALSE;
}

SHORT _Map::GetVirtualMapX()
{
	return VirtualMapX;
}

SHORT _Map::GetVirtualMapY()
{
	return VirtualMapY;
}


// 지상이 비어 있는가?
// 지상 캐릭터가 있는곳, 물, 일반 장애물
BOOL _Map::IsEmpty(SHORT x, SHORT y, SHORT xsize, SHORT ysize, SHORT idx, SHORT idy, SHORT idxsize, SHORT idysize)
{
	SHORT i, j;
	SHORT tempx, tempy;

	// 지도 영역 안에 있는지 확인한다. 
	if(x<0 || x+xsize-1>=MH.MapXsize)return FALSE;
	if(y<0 || y+ysize-1>=MH.MapYsize)return FALSE;
	
	for(i=0;i<ysize;i++)
		for(j=0;j<xsize;j++)
		{
			tempx=x-xsize/2+j;
			tempy=y-ysize/2+i;

			if( (idx-idxsize/2)<=tempx && tempx<(idx-idxsize/2+idxsize) 
			&&  (idy-idysize/2)<=tempy && tempy<(idy-idysize/2+idysize) )return FALSE;


			if(IsEmpty(tempx, tempy)==FALSE)return FALSE;
		}

	return TRUE;
}

// 비어 있는가?
// 지상 캐릭터가 있는곳, 물, 일반 장애물
BOOL _Map::IsEmptyWater(SHORT x, SHORT y, SHORT xsize, SHORT ysize, SHORT idx, SHORT idy, SHORT idxsize, SHORT idysize)
{
	SHORT i, j;
	SHORT tempx, tempy;

	// 지도 영역 안에 있는지 확인한다. 
	if(x<0 || x+xsize-1>=MH.MapXsize)return FALSE;
	if(y<0 || y+ysize-1>=MH.MapYsize)return FALSE;

	for(i=0;i<ysize;i++)
		for(j=0;j<xsize;j++)
		{
			tempx=x-xsize/2+j;
			tempy=y-ysize/2+i;
			
			if( (idx-idxsize/2)<=tempx && tempx<(idx-idxsize/2+idxsize) 
			&&  (idy-idysize/2)<=tempy && tempy<(idy-idysize/2+idysize) )return FALSE;
			
			if(IsEmptyWater(tempx, tempy)==FALSE)return FALSE;
		}

	return TRUE;
}


// x,y에 건물을 지을 수 있는 곳인가?
// x,y 기본 좌표 
BOOL _Map::CanBuild(SHORT x,SHORT y, _NewID builderid, SHORT kind)
{
	int block;

	if(x<0  || x>=MH.MapXsize)return FALSE;
	if(y<0  || y>=MH.MapYsize)return FALSE;

	// 캐릭터가 있는곳
	SI32 id = GetIDMap(x, y); 
	if(id && id!=builderid.ID)
		return FALSE;
	
	// 곡물이 있는곳 
	// 목재는 
	if(GetCropTypeMap(x, y)==CROP_TYPE_TREE
	|| GetCropTypeMap(x, y)==CROP_TYPE_BAMBOO)
	{
		// 남은 양이 있으면 
		if(GetCropMountMap(x, y))return FALSE;
	}
#ifdef _IMJIN2
	// 바닥에 붙어 있는 광고면, 
	else if(GetCropTypeMap(x, y) == CROP_TYPE_AD12)
	{
	}
#endif
	// 기타 곡물은 무조건 불가. 
	else if(GetCropTypeMap(x, y))
	{
		return FALSE;
	}

	block=BLOCK_DEEPWATER|BLOCK_CASTLE|BLOCK_NORMAL|BLOCK_RUIN|BLOCK_NEWBLK|BLOCK_TREE|BLOCK_SHALLOWWATER|BLOCK_MOVE|BLOCK_WALL;

	if( pCollMap )
		if( pCollMap[x+YOff[y]] & block )	return FALSE;

	return TRUE;
}


void _Map::MakeSmallMapBase()
{
	SHORT i, j;

	for(i=0;i<MH.MapYsize;i++)
		for(j=0;j<MH.MapXsize;j++)
		{
			UpdateSmallMapBase(j, i);
		}
}

// 절대 좌표를 입력하면 기본 좌표가 나온다. 
// realx, realy :절대 좌표 
// x, y :기본 좌표 
BOOL _Map::GetRealTileByRealDot(SHORT realx, SHORT realy, SHORT* pX, SHORT* pY)
{
	// 전체 가로 사이즈를 구한다. 
	SHORT xsize=MH.MapXsize*MAP_TILE_XSIZE/MAP_BY_REAL_DIV;

	int posx=realx/MAP_BY_REAL_DIV+xsize/2;
	int posy=realy/MAP_BY_REAL_DIV+200/MAP_BY_REAL_DIV;

	if(posx<0)posx=0;
	if(posx>=(MAX_MAP_XSIZE)*MAP_TILE_XSIZE/MAP_BY_REAL_DIV)
		posx=(MAX_MAP_XSIZE)*MAP_TILE_XSIZE/MAP_BY_REAL_DIV-1;
	if(posy<0)posy=0;
	if(posy>=(MAX_MAP_YSIZE)*MAP_TILE_YSIZE/MAP_BY_REAL_DIV+200/MAP_BY_REAL_DIV)
		posy=posy>=(MAX_MAP_YSIZE)*MAP_TILE_YSIZE/MAP_BY_REAL_DIV+200/MAP_BY_REAL_DIV-1;

	*pX = MapByRealX[posx][posy];
    *pY = MapByRealY[posx][posy];

	if((*pX) < 0 || (*pY) < 0)return FALSE;

	return TRUE;
}


void _Map::AllocMap(SHORT xsize, SHORT ysize, BOOL bvillagemapswitch)
{
	SI32 i;

	pIDMap = new short[xsize * ysize];
	if( pIDMap )
		ZeroMemory(pIDMap, sizeof(short) * xsize * ysize);



	pSkyMap = new short[xsize * ysize];

	if(pSkyMap)
		ZeroMemory(pSkyMap, sizeof(short) * xsize * ysize);

	pCollMap = new short[xsize * ysize];
	if( pCollMap )	ZeroMemory(pCollMap, sizeof(short)*xsize*ysize);

	pBarrierMap = new short[xsize * ysize];
	if( pBarrierMap )	ZeroMemory(pBarrierMap, sizeof(short)*xsize*ysize);

	pLevelInfo = new char[xsize * ysize];
	if( pLevelInfo )	ZeroMemory(pLevelInfo, sizeof(char)*xsize*ysize);


	pLevelFile = new unsigned char[xsize * ysize];
	if( pLevelFile )	ZeroMemory(pLevelFile, sizeof(unsigned char)*xsize*ysize);


	pLevelFont = new unsigned char[xsize * ysize];
	if( pLevelFont )	ZeroMemory(pLevelFont, sizeof(unsigned char)*xsize*ysize);


	pLevelType = new unsigned char[xsize * ysize];
	if( pLevelType )	ZeroMemory(pLevelType, sizeof(unsigned char)*xsize*ysize);


	pTopLevel = new unsigned char[xsize * ysize];
	if( pTopLevel )		ZeroMemory(pTopLevel, sizeof(unsigned char)*xsize*ysize);


	for(i = 0;i < MAX_LEVEL_STEP;i++)
	{
		pLevelMode[i] = new unsigned char[xsize * ysize];
		if(pLevelMode[i])ZeroMemory(pLevelMode[i], sizeof(unsigned char) * xsize * ysize);
	}

	pTileAtbMap = new short[xsize * ysize];
	if( pTileAtbMap )	ZeroMemory(pTileAtbMap, sizeof(short)*xsize*ysize);


	pShallowMap = new unsigned char[xsize * ysize];
	if( pShallowMap)	ZeroMemory(pShallowMap, sizeof(unsigned char)*xsize*ysize);


	pShallowFont = new unsigned char[xsize * ysize];
	if( pShallowFont )	ZeroMemory(pShallowFont, sizeof(unsigned char)*xsize*ysize);


//	pSectorMap = new unsigned char[xsize * ysize];
//	if( pSectorMap)	ZeroMemory(pSectorMap, sizeof(unsigned char)*xsize*ysize);

	pSeaMode = new unsigned char[xsize * ysize];
	if( pSeaMode)	ZeroMemory(pSeaMode, sizeof(unsigned char)*xsize*ysize);
	//pSeaFont = new unsigned char[(xsize/2) * (ysize/2)];
	//if( pSeaFont)	ZeroMemory(pSeaFont, sizeof(unsigned char)*(xsize/2)*(ysize/2));
	pSeaFont = new unsigned char[(xsize) * (ysize)];
	if( pSeaFont)	ZeroMemory(pSeaFont, sizeof(unsigned char)*(xsize)*(ysize));

	//pSeaFile = new unsigned char[(xsize/2) * (ysize/2)];
	//if( pSeaFile)	ZeroMemory(pSeaFile, sizeof(unsigned char)*(xsize/2)*(ysize/2));
	pSeaFile = new unsigned char[(xsize) * (ysize)];
	if( pSeaFile)	ZeroMemory(pSeaFile, sizeof(unsigned char)*(xsize)*(ysize));



	pNewBlkMode = new unsigned char[xsize * ysize];
	if( pNewBlkMode)	ZeroMemory(pNewBlkMode, sizeof(unsigned char)*xsize*ysize);
	pNewBlkFont = new unsigned char[(xsize/2) * (ysize/2)];
	if( pNewBlkFont)	ZeroMemory(pNewBlkFont, sizeof(unsigned char)*(xsize/2)*(ysize/2));
	pNewBlkFile = new unsigned char[(xsize/2) * (ysize/2)];
	if( pNewBlkFile)	ZeroMemory(pNewBlkFile, sizeof(unsigned char)*(xsize/2)*(ysize/2));
	pNewBlkLevel = new unsigned char[xsize * ysize];
	if( pNewBlkLevel)	ZeroMemory(pNewBlkLevel, sizeof(unsigned char)*xsize*ysize);

	pCastleMode = new unsigned char[xsize * ysize];
	if( pCastleMode)	ZeroMemory(pCastleMode, sizeof(unsigned char)*xsize*ysize);
	pCastleFont = new unsigned char[(xsize/2) * (ysize/2)];
	if( pCastleFont)	ZeroMemory(pCastleFont, sizeof(unsigned char)*(xsize/2)*(ysize/2));
	pCastleFile = new unsigned char[(xsize/2) * (ysize/2)];
	if( pCastleFile)	ZeroMemory(pCastleFile, sizeof(unsigned char)*(xsize/2)*(ysize/2));

	pCropTypeMap = new unsigned char[xsize * ysize];
	if( pCropTypeMap)	ZeroMemory(pCropTypeMap, sizeof(unsigned char)*xsize*ysize);
	pCropVaryMap = new unsigned char[xsize * ysize];
	if( pCropVaryMap)	ZeroMemory(pCropVaryMap, sizeof(unsigned char)*xsize*ysize);
	pCropMountMap = new unsigned char[xsize * ysize];
	if( pCropMountMap)	ZeroMemory(pCropMountMap, sizeof(unsigned char)*xsize*ysize);
	pCropStepMap = new unsigned char[xsize * ysize];
	if( pCropStepMap)	ZeroMemory(pCropStepMap, sizeof(unsigned char)*xsize*ysize);

	pCropTreeMap = new unsigned char[xsize * ysize];
	if( pCropTreeMap)	ZeroMemory(pCropTreeMap, sizeof(unsigned char)*xsize*ysize);


	pTreasureMap = new SHORT[xsize * ysize];
	if( pTreasureMap)	ZeroMemory(pTreasureMap, sizeof(SHORT)*xsize*ysize);

	pMineMap = new char[xsize * ysize];
	if( pMineMap)	ZeroMemory(pMineMap, sizeof(char)*xsize*ysize);

//	pMineCharMap = new SI32[xsize * ysize];
//	if(pMineCharMap) ZeroMemory(pMineCharMap,sizeof(SI32)*xsize*ysize);

	pFireMap = new UI08[xsize * ysize];
	if( pFireMap)	ZeroMemory(pFireMap, sizeof(UI08)*xsize*ysize);

	pFireCharMap = new _NewID[xsize * ysize];
	if(pFireCharMap) ZeroMemory(pFireCharMap, sizeof(_NewID)*xsize*ysize);

}

void _Map::FreeMap()
{
	SI32 i;
	if( pIDMap )
	{
		delete[] pIDMap;
		pIDMap = NULL;
	}

	if( pSkyMap )
	{
		delete[] pSkyMap;
		pSkyMap = NULL;
	}

	
	if( pBarrierMap )
	{
		delete[] pBarrierMap;
		pBarrierMap = NULL;
	}

	if( pCollMap )
	{
		delete[] pCollMap;
		pCollMap = NULL;
	}

	if( pLevelInfo )
	{
		delete[] pLevelInfo;
		pLevelInfo = NULL;
	}
	if( pLevelFile )
	{
		delete[] pLevelFile;
		pLevelFile = NULL;
	}
	if( pLevelFont )
	{
		delete[] pLevelFont;
		pLevelFont = NULL;
	}
	if( pLevelType )
	{
		delete[] pLevelType;
		pLevelType = NULL;
	}
	if( pTopLevel )
	{
		delete[] pTopLevel;
		pTopLevel = NULL;
	}

	for( i=0; i<MAX_LEVEL_STEP; i++ )
	{
		if( pLevelMode[i] )
		{
			delete[] pLevelMode[i];
			pLevelMode[i] = NULL;
		}
	}
	
	if( pTileAtbMap )
	{
		delete[] pTileAtbMap;
		pTileAtbMap = NULL;
	}


	if( pShallowMap )
	{
		delete[] pShallowMap;
		pShallowMap = NULL;
	}

	if( pShallowFont )
	{
		delete[] pShallowFont;
		pShallowFont = NULL;
	}

/*	if( pSectorMap )
	{
		delete[] pSectorMap;
		pSectorMap = NULL;
	}

*/
	if( pSeaMode )
	{
		delete[] pSeaMode;
		pSeaMode = NULL;
	}
	if( pSeaFont )
	{
		delete[] pSeaFont;
		pSeaFont = NULL;
	}
	if( pSeaFile )
	{
		delete[] pSeaFile;
		pSeaFile = NULL;
	}

	if( pNewBlkMode )
	{
		delete[] pNewBlkMode;
		pNewBlkMode = NULL;
	}
	if( pNewBlkFont )
	{
		delete[] pNewBlkFont;
		pNewBlkFont = NULL;
	}
	if( pNewBlkFile )
	{
		delete[] pNewBlkFile;
		pNewBlkFile = NULL;
	}
	if( pNewBlkLevel )
	{
		delete[] pNewBlkLevel;
		pNewBlkLevel = NULL;
	}

	
	if( pCastleMode )
	{
		delete[] pCastleMode;
		pCastleMode = NULL;
	}
	if( pCastleFont )
	{
		delete[] pCastleFont;
		pCastleFont = NULL;
	}
	if( pCastleFile )
	{
		delete[] pCastleFile;
		pCastleFile = NULL;
	}

	
	if( pCropTypeMap )
	{
		delete[] pCropTypeMap;
		pCropTypeMap = NULL;
	}

	if( pCropTreeMap )
	{
		delete[] pCropTreeMap;
		pCropTreeMap = NULL;
	}

	if( pCropVaryMap )
	{
		delete[] pCropVaryMap;
		pCropVaryMap = NULL;
	}
	if( pCropMountMap )
	{
		delete[] pCropMountMap;
		pCropMountMap = NULL;
	}
	if( pCropStepMap )
	{
		delete[] pCropStepMap;
		pCropStepMap = NULL;
	}

	if(pTreasureMap )
	{
		delete[] pTreasureMap ;
		pTreasureMap = NULL;
	}

	if(pMineMap )
	{
		delete[] pMineMap ;
		pMineMap 	= NULL;
	}

//	if(pMineCharMap)
//	{
//		delete[] pMineCharMap;
//		pMineCharMap = NULL;
//	}


	if(pFireMap )
	{
		delete[] pFireMap ;
		pFireMap = NULL;
	}

	if(pFirePlayerMap)
	{
		delete[] pFirePlayerMap ;
		pFirePlayerMap = NULL;
	}

	if(pFireCharMap)
	{
		delete[] pFireCharMap ;
		pFireCharMap = NULL;
	}


}

void _Map::SetMapDrawSwitch(int mode)
{
	MapDrawSwitch=mode;
}



void _Map::PutTreasure(SI32 x, SI32 y, SHORT type, SI32 amount)
{
	Map.SetTreasureMap(x, y, ((type << 8) | amount) );
}

void _Map::DeleteTreasure(SI32 x, SI32 y)
{
	Map.SetTreasureMap(x, y, 0);
}



// 각 플레이어의 시작 위치를 랜덤하게 바꾼다. 
void _Map::RandomStartPos()
{
	SI32 i;

	POINT temp_point[MAX_MULTI_PLAYER];
	SHORT temp_point_index=0;

	for(i=0;i<Map.ExpansionBaseNumber;i++)
	{
		temp_point[i]=Map.ExpansionBasePoint[i];
	}
	temp_point_index=Map.ExpansionBaseNumber;

	SHORT index=0;
	SHORT outindex=0;
	while(temp_point_index)
	{
		
	   outindex=GD.Random()%temp_point_index;

       Map.clMapPlayerInfo[index].StartPoint=temp_point[outindex];
	   temp_point[outindex]=temp_point[temp_point_index-1];

	   index++;
	   temp_point_index--;
	}

}

#endif