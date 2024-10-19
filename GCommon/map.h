
#ifndef _ROCKET_NEW_TILE_MAP
#ifndef _MAP_H
#define _MAP_H     1

#pragma warning ( disable : 4786 )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4018 )

#include <Player.h>


#define CLIENT_SCREEN_XSIZE  832  //(64*13)
#define CLIENT_SCREEN_YSIZE  576  //(32*18)

#define MAX_DRAWMAP_SCREEN_XSIZE 13
#define MAX_DRAWMAP_SCREEN_YSIZE 18


#define MAX_MAP_CHAR_NUMBER 800

#define MAP_TILE_XSIZE      64
#define MAP_TILE_XSIZE_HALF 32
#define MAP_TILE_YSIZE      32
#define MAP_TILE_YSIZE_HALF 16
#define HEIGHT_PER_LEVEL 16
#define MAP_BY_REAL_DIV     10


// 타일용 방위 
#define MAPTILE_SOUTH  1
#define MAPTILE_WEST   2
#define MAPTILE_NORTH  4
#define MAPTILE_EAST   8
#define MAPTILE_SW    16
#define MAPTILE_NW    32
#define MAPTILE_NE    64
#define MAPTILE_SE   128

// 언덕용 방위 
#define MAPTILEHILL_SOUTH  1
#define MAPTILEHILL_SW     2
#define MAPTILEHILL_WEST   4
#define MAPTILEHILL_NW     8
#define MAPTILEHILL_NORTH 16
#define MAPTILEHILL_NE    32
#define MAPTILEHILL_EAST  64
#define MAPTILEHILL_SE   128

#define TILE_DEFAULT       0              // 기본형 타일
#define TILE_GREEN         1    
#define TILE_CROPAREA      2               // 경작지 
#define TILE_GRASS         3              // 풀밭 
#define TILE_CASTLEAREA    4
#define TILE_BLOCK         7               // 장애물 언덕 
#define TILE_CASTLE        8


#define MAX_LEVEL_STEP     5             // 최대 층 높이 (0~4)

#define MAX_TILE_TYPE_NUMBER  15         // 언덕 타일의 최대 종류 


// 변종의 종류 
#define MAX_TILE_VARY     7
#define MAX_SEA_VARY      8
#define MAX_GRASS_VARY    6
#define MAX_GREEN_VARY    11
#define MAX_NEWBLK_VARY   2
#define MAX_CASTLE_VARY   3
#define MAX_CROPAREA_VARY 7

#define MAX_SCREEN_HALF_SIZE 16

#define MAX_SHIP_PATROL_POINT_NUMBER    50         // 배 순찰 지점 
#define MAX_SHIP_LANDING_POINT_NUMBER   50         // 배 상륙 지점 


// 타일의 각종 정보를 저장하는 곳 
class _Tile 
{
public:
   int Type;      // 어떤 언덕모양인가?
   SHORT X, Y;    // 좌상단의 변위 (맨 좌측 상단에서부터의 변위)

};

class _Castle
{
public:
	SHORT File, Font;
};

class _MapHeader{
public:
	DWORD Version;                          // 지도의 버전 
	DWORD Unique;                           // 고유 정보 
	char Text[128];                         // 문자열 정보 
	DWORD PlayerNumber ;                    // 게임에 참여하는 유저 수 .
	SHORT MapXsize, MapYsize;               // 지도의 가로 세로 사이즈 
	SHORT MapType;                          // 지도 타일의 종류 
	SI32  CheckSum;							// 지도가 해킹되어 있는지 확인한다. 
	DWORD Reserved[6];                   
};




class cltMapPlayerInfo{
public:
	POINT StartPoint;
	_PlayerInfo PI;
	DWORD Money;
	DWORD Tree;


	/////////////////////////////////////////////////////////////////
	//KIND_ON_MAX
	/////////////////////////////////////////////////////////////////
	// 게임에 등장할 수 있는 최대 캐릭터 수 
	SHORT MaxUnitNumber[200];
	// 캐릭터가 게임에 등장하게 만드는가?
	SHORT IsCharInGame[200];
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////

	void Init(SI32 pnumber)
	{
		ZeroMemory(this, sizeof(cltMapPlayerInfo));
		PI.Init(pnumber);

		StartPoint.x = -1;
		StartPoint.y = -1;

		for(int i=0;i<200;i++)
		{
			IsCharInGame[i] = TRUE;
		}
	}

};

class cltMapUnitInfo{
public:
	SI32 Kind;
	SI32 X, Y;
	SI32 Pnumber;
};

class _Map {
public:

	// 지도의 기본 정보. 
	_MapHeader				MH;
	// 게임에  등장하는 플레이어의 정보. 
	cltMapPlayerInfo		clMapPlayerInfo[MAX_MULTI_PLAYER];
	// 심어진 캐릭터의 정보 
	cltMapUnitInfo			clMapUnitInfo[MAX_MAP_CHAR_NUMBER];

		
	// 배정찰 지점	
	POINT					ShipPatrolPoint[MAX_SHIP_PATROL_POINT_NUMBER];

	// 배 상륙 지점		
	POINT					ShipLandingPoint[MAX_SHIP_LANDING_POINT_NUMBER];
	
	POINT					ExpansionBasePoint[MAX_MULTI_PLAYER];   // 확장 기지지을 수 있는 위치 
	SHORT					ExpansionBaseNumber;                    // 확장 기지의 개수 
	
	int						VirtualMapX, VirtualMapY;

	SI32					YOff[MAX_MAP_YSIZE];
	SI32					YOff2[MAX_MAP_YSIZE];

    unsigned char			Reserved[1024+640];                 // 예약되어 있는 변수들 


public:

	char					DrawForceSwitch;   // 강제로 지도를 그리게 한다. 
	char					MapDrawSwitch;                    // 지도를 그리느냐 마느냐


private:
	short					*pIDMap;
	short					*pSkyMap;

	// 충돌 속성을 저장한다. 
	short					*pCollMap;  

	short					*pBarrierMap;

	// 층  
	// 상위 4비트 level
    // 하위 4비트 levelmode
	char					*pLevelInfo;     
	unsigned char			*pLevelFile;
	unsigned char			*pLevelFont;
	unsigned char			*pLevelType;
	unsigned char			*pLevelMode[MAX_LEVEL_STEP];
	unsigned char			*pTopLevel;

	short					*pTileAtbMap;

	// 얕은 물용 지도 
	unsigned char			*pShallowMap;
	// 얕은 물용 지도 
	unsigned char			*pShallowFont;

	// 일반적인 물용 지도 
	unsigned char 			*pSeaMode;
	unsigned char 			*pSeaFont;
	unsigned char 			*pSeaFile;

	// 일반적인 장애물 지도 
	unsigned char 			*pNewBlkMode;
	unsigned char 			*pNewBlkFont;
	unsigned char 			*pNewBlkFile;
	unsigned char 			*pNewBlkLevel;


	// 성에 대한 정보를 저장한다.  
	unsigned char 			*pCastleMode;  
	unsigned char 			*pCastleFont;
	unsigned char 			*pCastleFile;

	// 곡물 데이터를 저장한다.
	unsigned char 			*pCropTypeMap;        // 곡물의 종류
	unsigned char 			*pCropVaryMap;        // 곡물의 변종
	unsigned char 			*pCropMountMap;       // 남은 곡물량 
	unsigned char 			*pCropStepMap;        // 곡물의 단계



public:

	// 보물 지도 
	// 0x00ff -> 식량 
	// 0xff00 -> 목재 
	SHORT * pTreasureMap;

private:
	// 지뢰지도 
	char * pMineMap;
//	SI32 * pMineCharMap;
public:

	// 불 지도 . 
	UI08*	pFireMap;
	_NewID*	pFireCharMap; 
	SI16*	pFirePlayerMap; 
	BOOL	m_bFire;


	void InitMap(SHORT xsize, SHORT ysize, BOOL bvillagemapswitch);
	void FreeMap();
	void AllocMap(SHORT xsize, SHORT ysize, BOOL bvillagemapswitch);
	void MakeVillageMap(SI32 xsize, SI32 ysize);

	SI32 WriteIDMap(FILE *fp);
	SI32 ReadIDMap(FILE *fp);
	SI32 WriteSkyMap(FILE *fp);
	SI32 ReadSkyMap(FILE *fp);
	SI32 WriteVillageMap(FILE *fp);
	SI32 ReadVillageMap(FILE *fp);

	SI32 WriteCollMap(FILE *fp);
	SI32 ReadCollMap(FILE *fp);

	SI32 WriteLevelInfo(FILE *fp);
	SI32 ReadLevelInfo(FILE *fp);
	SI32 WriteLevelFile(FILE *fp);
	SI32 ReadLevelFile(FILE *fp);
	SI32 WriteLevelFont(FILE *fp);
	SI32 ReadLevelFont(FILE *fp);
	SI32 WriteLevelType(FILE *fp);
	SI32 ReadLevelType(FILE *fp);
	SI32 WriteLevelMode(FILE *fp);
	SI32 ReadLevelMode(FILE *fp);
	SI32 WriteTopLevel(FILE *fp);
	SI32 ReadTopLevel(FILE *fp);

	SI32 WriteTileAtbMap(FILE *fp);
	SI32 ReadTileAtbMap(FILE *fp);

	SI32 WriteShallowMap(FILE *fp);
	SI32 ReadShallowMap(FILE *fp);
	SI32 WriteShallowFont(FILE *fp);
	SI32 ReadShallowFont(FILE *fp);

	SI32 WriteSeaMode(FILE* fp);
	SI32 ReadSeaMode(FILE *fp);
	SI32 WriteSeaFont(FILE* fp);
	SI32 ReadSeaFont(FILE *fp);
	SI32 WriteSeaFile(FILE* fp);
	SI32 ReadSeaFile(FILE *fp);

	SI32 WriteNewBlkMode(FILE* fp);
	SI32 ReadNewBlkMode(FILE *fp);
	SI32 WriteNewBlkFont(FILE* fp);
	SI32 ReadNewBlkFont(FILE *fp);
	SI32 WriteNewBlkFile(FILE* fp);
	SI32 ReadNewBlkFile(FILE *fp);
	SI32 WriteNewBlkLevel(FILE* fp);
	SI32 ReadNewBlkLevel(FILE *fp);

	SI32 WriteCastleMode(FILE* fp);
	SI32 ReadCastleMode(FILE *fp);
	SI32 WriteCastleFont(FILE* fp);
	SI32 ReadCastleFont(FILE *fp);
	SI32 WriteCastleFile(FILE* fp);
	SI32 ReadCastleFile(FILE *fp);

	SI32 WriteCropTypeMap(FILE* fp);
	SI32 ReadCropTypeMap(FILE *fp);
	SI32 WriteCropVaryMap(FILE* fp);
	SI32 ReadCropVaryMap(FILE *fp);
	SI32 WriteCropMountMap(FILE* fp);
	SI32 ReadCropMountMap(FILE *fp);
	SI32 WriteCropStepMap(FILE* fp);
	SI32 ReadCropStepMap(FILE *fp);

	SI32 WriteTreasureMap(FILE* fp);
	SI32 ReadTreasureMap(FILE*fp);

	SI32 WriteMineMap(FILE* fp);
	SI32 ReadMineMap(FILE* fp);

	SI32 WriteFireMap(FILE* fp);
	SI32 ReadFireMap(FILE*fp);

	// MapByRealX[], MapByRealY[] 변수를 업데이트한다. 
    void UpdateMapByRealXY();

	void MakeLastMap();

	
	void SetLevelSlope(SHORT x, SHORT y, char levelmode)
	{
		if( pLevelInfo == NULL )	return;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return;
		pLevelInfo[x+YOff[y]] &= 0xf0;
		pLevelInfo[x+YOff[y]] |= levelmode;
	}
	
	SHORT GetLevelInfo(SHORT x, SHORT y)
	{ 
		if( pLevelInfo == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return (pLevelInfo[x+YOff[y]] & 0x0f);
	} 

    unsigned char GetLevelFile(SI32 x, SI32 y)
	{
		if( pLevelFile == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pLevelFile[x+YOff[y]];
	}
    unsigned char GetLevelFont(SI32 x, SI32 y)
	{
		if( pLevelFont == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pLevelFont[x+YOff[y]];
	}
    unsigned char GetTopLevel(SI32 x, SI32 y)
	{
		if( pTopLevel == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pTopLevel[x+YOff[y]];
	}

	SHORT GetLevelMode(SI32 level, SHORT x, SHORT y)
	{ 
		if( pLevelMode[level] == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pLevelMode[level][x+YOff[y]];
	} 

	void SetLevelMode(SI32 level, SHORT x, SHORT y, unsigned char mode)
	{ 
		if( pLevelMode[level] == NULL )	return ;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return ;
		pLevelMode[level][x+YOff[y]] = mode;
	} 


///////////////////////////////////////////////////////////////
//   타일 관련 함수 
//////////////////////////////////////////////////////////////
    void	SetProperFileNFont(SHORT x, SHORT y, int hilltype);
	void	PutTile(SHORT sx, SHORT sy, int type);
	// 타일에 대한 각종 정보를 초기화한다. 
    void	InitTile();
	short	GetTileAtb(SI32 x, SI32 y)
	{
		if( pTileAtbMap == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pTileAtbMap[x+YOff[y]];
	}

    void SetMapDrawSwitch(int mode);



	
	void	SetIDMap(SHORT id, SI32 x, SI32 y)
	{
		if( pIDMap == NULL )	return;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return;
		pIDMap[x+YOff[y]] = id;
	}
    SHORT	GetIDMap(SI32 x, SI32 y)
	{
		if( pIDMap == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pIDMap[x+YOff[y]];
	}
	void	ClearIDMap(int x, int y)
	{
		if( pIDMap == NULL )	return;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return;
		
        pIDMap[x+YOff[y]] = 0;
	}
	
	
	void	SetSkyMap(SHORT id, SHORT x, SHORT y)
	{
		if( pSkyMap == NULL )	return;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return;
		pSkyMap[x+YOff[y]] = id;
		
	}
    SHORT	GetSkyMap(SHORT x, SHORT y)
	{
		if( pSkyMap == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pSkyMap[x+YOff[y]];
	}
	void	ClearSkyMap(SHORT x, SHORT y)
	{
		if( pSkyMap == NULL )	return;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return;
        pSkyMap[x+YOff[y]] = 0;
	}
	
	// 어느지점의 충돌 여부를 알려준다. 
    BOOL	IsColl(SHORT x, SHORT y, SHORT block)
	{
		if( pCollMap == NULL ) return TRUE;
		if( x<0 || x>=MH.MapXsize || y<0  || y>=MH.MapYsize )	return TRUE;
		
		if( pCollMap[x+YOff[y]] & block )	return TRUE;
		
		return FALSE;
	}
	
	// 충돌 속성을 설정한다. 
    BOOL	SetCollMap(SHORT x, SHORT y, SHORT block)
	{
		if( pCollMap == NULL )	return TRUE;
		if( x<0 || x>=MH.MapXsize || y<0  || y>=MH.MapYsize )	return TRUE;

		pCollMap[x+YOff[y]] |= block;
		return TRUE;
	}
	
	SHORT	GetCollMap(SI32 x, SI32 y)
	{
		if( pCollMap == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0  || y>=MH.MapYsize )	return 0;

		return pCollMap[x+YOff[y]];
	}
	
    // 특정 비트를 지운다. 
    void	DeleteCollMap(SHORT x, SHORT y, SHORT block)
	{
		if( pCollMap == NULL )	return;

		pCollMap[x+YOff[y]] &= (~block);
	}

	// Ice Barrier의 충돌 속성을 설정한다. 
    BOOL	SetBarrierCollMap(SHORT x, SHORT y)
	{
		if( pBarrierMap == NULL )	return TRUE;
		if( x<0 || x>=MH.MapXsize || y<0  || y>=MH.MapYsize )	return TRUE;

		pBarrierMap[x+YOff[y]] += 1;
		return TRUE;
	}

	// Ice Barrier의 충돌지정 횟수를 얻는다.
	short	GetBarrierCollMap(SHORT x, SHORT y)
	{
		if( pBarrierMap == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0  || y>=MH.MapYsize )	return 0;

		return pBarrierMap[x+YOff[y]];
	}

	void	DeleteBarrierCollMap(SHORT x, SHORT y)
	{
		if( pBarrierMap == NULL )	return;

		if(pBarrierMap[x+YOff[y]] > 0)
			pBarrierMap[x+YOff[y]] -= 1;
	}
	
	BOOL	DeleteCharCollMap(SHORT x, SHORT y);
	// 비행체의 움직임으로 인한 삭제 
    BOOL	DeleteAirCharCollMap(SHORT x, SHORT y);
	

	BOOL	IsValidShallowMap()
	{
		if( pShallowMap == NULL )	return FALSE;
		return TRUE;
	}
    unsigned char	GetShallowMap(SI32 x, SI32 y)
	{
		if( pShallowMap == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pShallowMap[x+YOff[y]];
	}
    unsigned char	GetShallowFont(SI32 x, SI32 y)
	{
		if( pShallowFont == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pShallowFont[x+YOff[y]];
	}

    
    unsigned char GetSeaMode(SI32 x, SI32 y)
	{
		if( pSeaMode == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pSeaMode[x+YOff[y]];
	}
    void SetSeaMode(UI08 seamode, SI32 x, SI32 y)
	{
		if( pSeaMode == NULL )	return ;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return ;
		pSeaMode[x+YOff[y]] = seamode;
	}

    unsigned char GetSeaFont(SI32 x, SI32 y)
	{
		if( pSeaFont == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize/2 || y<0 || y>=MH.MapYsize/2 )	return 0;
		return pSeaFont[x+YOff2[y]];
	}
    void SetSeaFont(UI08 seafont, SI32 x, SI32 y)
	{
		if( pSeaFont == NULL )	return ;
		if( x<0 || x>=MH.MapXsize/2 || y<0 || y>=MH.MapYsize/2 )	return ;
		pSeaFont[x+YOff2[y]] = seafont;
	}

    unsigned char GetSeaFile(SI32 x, SI32 y)
	{
		if( pSeaFile == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize/2 || y<0 || y>=MH.MapYsize/2 )	return 0;
		return pSeaFile[x+YOff2[y]];
	}

    void SetSeaFile(UI08 seafile, SI32 x, SI32 y)
	{
		if( pSeaFile == NULL )	return ;
		if( x<0 || x>=MH.MapXsize/2 || y<0 || y>=MH.MapYsize/2 )	return ;
		pSeaFile[x+YOff2[y]] = seafile;
	}

    unsigned char GetNewBlkMode(SI32 x, SI32 y)
	{
		if( pNewBlkMode == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pNewBlkMode[x+YOff[y]];
	}
    void SetNewBlkMode(unsigned char newblkmode, SI32 x, SI32 y)
	{
		if( pNewBlkMode == NULL )	return;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return;
		pNewBlkMode[x+YOff[y]] = newblkmode;
	}
    unsigned char GetNewBlkFont(SI32 x, SI32 y)
	{
		if( pNewBlkFont == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize/2 || y<0 || y>=MH.MapYsize/2 )	return 0;
		return pNewBlkFont[x+YOff2[y]];
	}
    void SetNewBlkFont(unsigned char newblkfont, SI32 x, SI32 y)
	{
		if( pNewBlkFont == NULL )	return;
		if( x<0 || x>=MH.MapXsize/2 || y<0 || y>=MH.MapYsize/2 )	return;
		pNewBlkFont[x+YOff2[y]] = newblkfont;
	}
    unsigned char GetNewBlkFile(SI32 x, SI32 y)
	{
		if( pNewBlkFile == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize/2 || y<0 || y>=MH.MapYsize/2 )	return 0;
		return pNewBlkFile[x+YOff2[y]];
	}
    void SetNewBlkFile(unsigned char newblkfile, SI32 x, SI32 y)
	{
		if( pNewBlkFile == NULL )	return;
		if( x<0 || x>=MH.MapXsize/2 || y<0 || y>=MH.MapYsize/2 )	return;
		pNewBlkFile[x+YOff2[y]] = newblkfile;
	}
    unsigned char GetNewBlkLevel(SI32 x, SI32 y)
	{
		if( pNewBlkLevel == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pNewBlkLevel[x+YOff[y]];
	}
    void SetNewBlkLevel(UI08 blklevel, SI32 x, SI32 y)
	{
		if( pNewBlkLevel == NULL )	return ;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return ;
		pNewBlkLevel[x+YOff[y]] = blklevel;
	}

    unsigned char GetCastleMode(SI32 x, SI32 y)
	{
		if( pCastleMode == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pCastleMode[x+YOff[y]];
	}


    void SetCastleMode(UI08 castlemode, SI32 x, SI32 y)
	{
		if( pCastleMode == NULL )	return ;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return ;
		pCastleMode[x+YOff[y]] = castlemode;
	}

    unsigned char GetCastleFont(SI32 x, SI32 y)
	{
		if( pCastleFont == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize/2 || y<0 || y>=MH.MapYsize/2 )	return 0;
		return pCastleFont[x+YOff2[y]];
	}

    void SetCastleFont(UI08 castlefont, SI32 x, SI32 y)
	{
		if( pCastleFont == NULL )	return ;
		if( x<0 || x>=MH.MapXsize/2 || y<0 || y>=MH.MapYsize/2 )	return ;
		pCastleFont[x+YOff2[y]] = castlefont;
	}

    unsigned char GetCastleFile(SI32 x, SI32 y)
	{
		if( pCastleFile == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize/2 || y<0 || y>=MH.MapYsize/2 )	return 0;
		return pCastleFile[x+YOff2[y]];
	}

    void SetCastleFile(UI08 castlefile, SI32 x, SI32 y)
	{
		if( pCastleFile == NULL )	return ;
		if( x<0 || x>=MH.MapXsize/2 || y<0 || y>=MH.MapYsize/2 )	return ;
		pCastleFile[x+YOff2[y]] = castlefile;
	}

    unsigned char GetCropTypeMap(SI32 x, SI32 y)
	{
		if( pCropTypeMap == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pCropTypeMap[x+YOff[y]];
	}
    void SetCropTypeMap(UI08 croptype, SI32 x, SI32 y)
	{
		if( pCropTypeMap == NULL )	return ;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return ;
		pCropTypeMap[x+YOff[y]] = croptype;
	}

    unsigned char GetCropVaryMap(SI32 x, SI32 y)
	{
		if( pCropVaryMap == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pCropVaryMap[x+YOff[y]];
	}
    void SetCropVaryMap(UI08 cropvary, SI32 x, SI32 y)
	{
		if( pCropVaryMap == NULL )	return ;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return ;
		pCropVaryMap[x+YOff[y]] = cropvary;
	}

    unsigned char GetCropMountMap(SI32 x, SI32 y)
	{
		if( pCropMountMap == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pCropMountMap[x+YOff[y]];
	}
	void SetCropMountMap(unsigned char cropmount, SI32 x, SI32 y)
	{
		if( pCropMountMap == NULL )	return;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return;
		pCropMountMap[x+YOff[y]] = cropmount;
	}
    unsigned char GetCropStepMap(SI32 x, SI32 y)
	{
		if( pCropStepMap == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pCropStepMap[x+YOff[y]];
	}
	void SetCropStepMap(unsigned char cropstep, SI32 x, SI32 y)
	{
		if( pCropStepMap == NULL )	return;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return;
		pCropStepMap[x+YOff[y]] = cropstep;
	}
    void IncreaseCropStepMap(SI32 x, SI32 y)
	{
		if( pCropStepMap == NULL )	return;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return;
		pCropStepMap[x+YOff[y]]++;
	}
	
	
	SHORT	GetXsize(){ return MH.MapXsize; }
	SHORT	GetYsize(){ return MH.MapYsize; }
	
	
	SHORT	GetVirtualMapX();
	SHORT	GetVirtualMapY();
	
	// x, y가 지도 좌표 안에 있는가?
	// x, y 기본좌표 
	BOOL	IsInMapArea(SHORT x, SHORT y)
	{
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return FALSE;
		return TRUE;
	}
	// x, y좌표가 화면안에 있는지 확인한다.
    BOOL	IsInScreen(SHORT x, SHORT y);
	
	
	// 지도상의 X, Y좌표의 중심의 좌표를 돌려준다. (절대 좌표)
	BOOL	GetRealDotCenterXY(SHORT x, SHORT y, int & centerx, int & centery);
	
    // 지도상의 X, Y좌표의 중심의 좌표를 돌려준다. (화면좌표)
    BOOL	GetCenterXYByMapCenterXY(SHORT x, SHORT y, SHORT & centerx, SHORT & centery);
	
	// 성곽등을 고려한 지도상의 X, Y좌표의 중심의 좌표를 돌려준다. (절대좌표)
	// 기준 좌표 
	// 지도상의 X, Y좌표의 중심의 좌표를 돌려준다. (화면 좌표)
    BOOL	GetCenterXY(SHORT X, SHORT Y, SHORT & centerx, SHORT & centery);
    BOOL	GetRefXY(SHORT X, SHORT Y, SHORT & centerx, SHORT & centery);
	
	
    // 절대 xy 좌표를 근거로 해서 지도 좌표를 구한다. 
    BOOL	DecideMapXYByXY(SHORT x, SHORT y, SHORT& map_x, SHORT& map_y, SHORT & map_center_x, SHORT & map_center_y);
    // 절대 좌표를 근거로 해서 기본 좌표를 구한다. 
    BOOL	DecideMapXYByXYLow(SHORT x, SHORT y, SHORT& map_x, SHORT& map_y, SHORT & map_center_x, SHORT & map_center_y);


    // 절대 좌표를 화면 좌표로 변환한다.
    void	TransFromRealDotToScreenDot(SHORT realx, SHORT realy, SHORT& screenx, SHORT &screeny);
    // 화면 좌표를 절대 좌표로 변환한다.
    void	TransFromScreenToRealXY(SHORT screenx, SHORT screeny, SHORT& realx, SHORT &realy);


    int		GetTileArrayNumber(SHORT x, SHORT y, int atb2);
    // 접경타일을 구한다. 
    int		GetNeighbourTileType(SHORT sx, SHORT sy);
    

	SHORT	GetHillType(SHORT x, SHORT y)
	{ 
		if( pLevelType == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pLevelType[x+YOff[y]];
	}



	// 특정 좌표의 색을 구한다. 
    unsigned char	GetColor(SHORT x, SHORT y);
	// x, y위치의 타일의 화일번호와 폰트 번호를 구한다. 
    BOOL	GetMapFileNFont(SHORT x, SHORT y, SHORT& file, SHORT &font);


	//////////////////////////////////////////////////////////////
	// 곡물 관련 함수 
	/////////////////////////////////////////////////////////////

	BOOL	IsCrop(SHORT x, SHORT y);
    void	PutCrop(SHORT x, SHORT y, SHORT type);
	void	PutCrop(SHORT x, SHORT y, SHORT type, SHORT vary, SHORT mount);

	void	PutTree(SHORT x, SHORT y, SHORT type);

	// 곡물을 줄인다. 
    SHORT	DecreaseCrop(SHORT x, SHORT y, SHORT amount);


	// 주변에 나무 심기가 적당한가?
    BOOL	IsGoodForTree(SHORT x, SHORT y);



	BOOL	IncreaseVirtualMapX(int mount);
	BOOL	DecreaseVirtualMapX(int mount);
	BOOL	IncreaseVirtualMapY(int mount);
	BOOL	DecreaseVirtualMapY(int mount);


	// mousex, mousey가 클라이언트 영역에 있는가?
    BOOL	IsPtInMap(SHORT mouse_x, SHORT mouse_y);

	// 지상이 비어 있는가?
    // 지상 캐릭터가 있는곳, 물, 일반 장애물
    BOOL	IsEmpty(SHORT x, SHORT y)
	{
	    if( x<0  || x>=MH.MapXsize )	return FALSE;
	    if( y<=0 || y>=MH.MapYsize )	return FALSE;
		if( pIDMap == NULL )	return FALSE;
		if( pCollMap == NULL )	return FALSE;
	    if( pIDMap[x+YOff[y]] )	return FALSE;
	    if( pCollMap[x+YOff[y]] & (BLOCK_DEEPWATER|BLOCK_MOVE|BLOCK_NORMAL|BLOCK_RUIN|BLOCK_CASTLE|BLOCK_NEWBLK|BLOCK_TREE|BLOCK_ICEWALL) )
			return FALSE;
	    return TRUE;
	}

	// 지상이 비어 있는가?
    // 지상 캐릭터가 있는곳, 물, 일반 장애물
    BOOL	IsEmptyWater(SHORT x, SHORT y)
	{
	    if( x<0  || x>=MH.MapXsize )	return FALSE;
	    if( y<=0 || y>=MH.MapYsize )	return FALSE;
		if( pIDMap == NULL )	return FALSE;
		if( pCollMap == NULL )	return FALSE;
  	    if( pIDMap[x+YOff[y]] )	return FALSE;
	    if( (pCollMap[x+YOff[y]] & BLOCK_DEEPWATER) == 0 )	return FALSE;
	    return TRUE;
	}


    // 지상이 비어 있는가?
    // 지상 캐릭터가 있는곳, 물, 일반 장애물
    BOOL	IsEmpty(SHORT x, SHORT y, SHORT xsize, SHORT ysize, SHORT idx, SHORT idy, SHORT idxsize, SHORT idysize);

	// 비어 있는가?
    BOOL	IsEmptyWater(SHORT x, SHORT y, SHORT xsize, SHORT ysize, SHORT idx, SHORT idy, SHORT idxsize, SHORT idysize);


	// x,y에 건물을 지을 수 있는 곳인가?
    // x,y 기본 좌표 
    BOOL	CanBuild(SHORT x,SHORT y, SHORT kind);

	// x,y에 건물을 지을 수 있는 곳인가?
    // x,y 기본 좌표 
    BOOL	CanBuild(SHORT x,SHORT y, _NewID builderid, SHORT kind);

	// x, y :기본 좌표 
    BOOL	CanBuild(SHORT x, SHORT y, SHORT xsize, SHORT ysize, SHORT kind);

    // kind종류의 건물을 지을 수 있는가?
    // x , y :기본 좌표 
    BOOL	CanBuildByKind(SHORT x, SHORT y, SHORT kind);
	// kind종류의 건물을 지을 수 있는가?
    // x, y :기본 좌표 
    BOOL	CanBuildByKind(SHORT x, SHORT y, SHORT kind, _NewID builderid);

	// 조선소를 지을 수 있는지 확인한다. 
    BOOL	CanBuildShipyard(SHORT x,SHORT y, SHORT xsize, SHORT ysize, _NewID builderid);

//////////////////////////////////////////////////////////////////
///  언덕 제작 관련 
/////////////////////////////////////////////////////////////////
	// sx, sy주변의 층 배열을 확인한다. 
    // sx, sy:2배수 좌표 
    int		GetLevelNear1(SHORT sx, SHORT sy, SHORT level);
	int		GetLevelNear2(SHORT sx, SHORT sy, SHORT level);

    void	PutLevel(SHORT x, SHORT y, SHORT level, SHORT mode);
	// 높이를 지운다. 
    void	DeleteLevel(SHORT x, SHORT y, SHORT level);


    //  level 설정 및 구하기 관련 함수 
	void	SetLevel(SHORT x, SHORT y, SHORT level);

	SHORT	GetLevel(SHORT x, SHORT y);
	// 성곽등을 고려한 최종 높이 
    SHORT	GetLastLevel(SHORT x, SHORT y);

    // 레벨에 따라 적절한 타일을 조합한다. 
    BOOL	PutProperTile(SHORT sx, SHORT sy, int level);

	// 높이를 구한다.경사지는 -1을 리턴하고 나머지 지역은 높이를 리턴한다. 
	SHORT	GetHillLayer(SHORT x, SHORT y);


////////////////////////////////////////////////////////
//  물관련 
////////////////////////////////////////////////////////
	// sx, sy주변의 물 배열을 확인한다. 
    // sx, sy:4배수 좌표 
    int		GetSeaNear1(SHORT sx, SHORT sy);
	int		GetSeaNear2(SHORT sx, SHORT sy);

	// 물은 만들지 않고 얕은 영역만 만든다. 
    void	PutShallow(SHORT x, SHORT y, SHORT mode);
    void	PutShallowSea(SHORT sx, SHORT sy, SHORT mode);
 	// 일반적인 물을 심는다. 
    // sx, sy: 4배수 좌표 
    void	PutSea(SHORT sx, SHORT sy, SHORT mode, SHORT pushmode);

	// 주변의 상황을 고려하여 충돌영역을 설정한다. 
    // x, y :4배수 좌표 
    void	PutCollSea(SHORT x, SHORT y);


/////////////////////////////////////////////////////
// 장애물용 언덕 관련 
///////////////////////////////////////////////////
    int GetNewBlkNear1(SHORT sx, SHORT sy);
	int GetNewBlkNear2(SHORT sx, SHORT sy);
	void PutNewBlk(SHORT sx, SHORT sy, SHORT mode);

	// 주변의 상황을 고려하여 충돌영역을 설정한다. 
    // x, y :2배수 좌표 
    void PutCollNewBlk(SHORT x, SHORT y);
    // x, y 4배수 좌표 
    void PutNewBlkGate(SHORT sx, SHORT sy);
    // x, y 4배수 좌표 
    SHORT GetNewBlkGateMode(SHORT sx, SHORT sy);


//////////////////////////////////////////////////////
// 성곽 관련 함수
/////////////////////////////////////////////////////
    int GetCastleNear1(SHORT sx, SHORT sy);
	int GetCastleNear2(SHORT sx, SHORT sy);
	void PutCastle(SHORT sx, SHORT sy, SHORT mode);
	void PutCastleGate(SHORT sx, SHORT sy, SHORT dir, SHORT mode);

	// x, y주변의 성이 위차한 방위를 돌려준다. 


    // 주변의 상황을 고려하여 충돌영역을 설정한다. 
    // x, y :4배수 좌표 
    void PutCollCastle(SHORT x, SHORT y);

//////////////////////////////////////////////////////////////////////////////////
/// 그리기 함수 
/////////////////////////////////////////////////////////////////////////////////
	// 지도를 그린다. 
    BOOL DrawMap();
    BOOL DrawObj();
	BOOL DrawMapEntire();

	BOOL IsInCrop(SHORT mapx, SHORT mapy, SHORT mousex, SHORT mousey, SHORT &length);
	void DecideCropFileNFont(SHORT mapx, SHORT mapy, SHORT &file, SHORT &font);
	void DrawCrop(SHORT x, SHORT y, SHORT mapx, SHORT mapy, unsigned char * table);

	// 지뢰를 그린다. 
    void DrawMine(SHORT x, SHORT y, SHORT mapx, SHORT mapy, unsigned char * table);

	// 불의 수명을 가져온다.
	SI32 GetFireLifeTime(SI32 x, SI32 y);
	// 땅에 붙은 불을 그린다.
	void DrawFire(SHORT x, SHORT y, SHORT mapx, SHORT mapy, unsigned char* table);

	// 불을 심는다. 
	void PutFire(SI32 siX, SI32 siY, SI32 siFireType, SI32 siLifeTime,_NewID siChar,SI16 siPlayer);

	BOOL DecreaseFireLifeTime(SI32 siX, SI32 siY, SI32 siDecreaseFactor);

	void DrawTreasure(SHORT x, SHORT y, SHORT mapx, SHORT mapy, unsigned char * table, SHORT type);


	BOOL DrawDragBox(SHORT x1, SHORT y1, SHORT x2, SHORT y2);

	// 지도 요소를 그리는 함수들 
    // 언덕을 그린다. 
    // x, y:2배수좌표 
    BOOL DrawHill(SHORT centerx, SHORT centery, SHORT x, SHORT y,  SHORT mode);
	BOOL DrawHillBlack(SHORT centerx, SHORT centery, SHORT x, SHORT y);

    // x, y:2배수좌표 
    BOOL DrawFog(SHORT centerx, SHORT centery, SHORT x, SHORT y, SHORT dirmode);

	// x, y:2배수좌표 
    BOOL DrawMousePos(SHORT centerx, SHORT centery, SHORT x, SHORT y, unsigned char * table, SHORT mode);


	// 얕은 물을 그린다. 
    // x, y:2배수좌표 
    BOOL DrawShallow(SHORT centerx, SHORT centery, SHORT x, SHORT y, unsigned char * table);

	// 물을 그린다. 
    // x, y:2배수좌표 
    BOOL DrawSea(SHORT centerx, SHORT centery, SHORT x, SHORT y );	
	// x, y:2배수좌표 
    BOOL DrawNewBlk(SHORT centerx, SHORT centery, SHORT x, SHORT y);

	// 성을그린다.
    // x, y:4배수좌표 
    BOOL DrawCastle(SHORT centerx, SHORT centery, SHORT x, SHORT y);


	void DrawBuilding(SHORT kind, SHORT buildx, SHORT buildy, SHORT canbuildswitch, SHORT dir);
	// 플레이어 시작 위치를 그려준다. 
    void DrawPlayerStartPoint(SHORT x, SHORT y, SHORT pnumber);

	void DrawShipPatrol();
	void DrawShipLanding();
	// 배 순찰지점을 그려준다.
    void DrawShipPatrol(SHORT x, SHORT y);
	// 배 상륙지점을 그려준다.
    void DrawShipLanding(SHORT x, SHORT y);

	void DrawTile(SI32 file, SI32 font, SI32 x, SI32 y, SI32 startx, SI32 starty);


////////////////////////////////////////////////////////////////////
/// 지도를 저장하거나 불러온다. 
///////////////////////////////////////////////////////////////////
    // 지도를 저장한다. 
    BOOL Save(char* filename);
    // 지도를 불러온다. 
    BOOL Load(char* filename);

	// 화일이름을 갖고 지도의 정보를 얻어온다. 
	BOOL GetMapInfo(char* filename, _MapHeader* header);

	// x, y기본 좌표 
	BOOL IsLand(SHORT x, SHORT y)
	{
		if( pCollMap == NULL )	return TRUE;
		if( pCollMap[x+YOff[y]] & BLOCK_DEEPWATER )		return FALSE;   
		return TRUE;
	}
	
	// x, y기본 좌표 
	BOOL IsSea(SHORT x, SHORT y)
	{
		if( pCollMap == NULL )	return FALSE;
		if( pCollMap[x+YOff[y]] & BLOCK_DEEPWATER )	return TRUE;
		return FALSE;
	}

/*	// x, y기본 좌표 
    SHORT GetSector(SHORT x, SHORT y)
	{
		if(pSectorMap== NULL)return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return (SHORT) pSectorMap[x+YOff[y]];
	}
	
	// x, y기본 좌표 
    void SetSector(SHORT x, SHORT y, unsigned char sector)
	{
		if(pSectorMap== NULL)return ;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return;
		pSectorMap[x+YOff[y]]=sector;
	}
*/
	
	void SetVirtualPos(SI16 siX, SI16 siY)
	{
		VirtualMapX  = siX;
		VirtualMapY  = siY;
	}


	// 지도에 각종 지형지물을 넣는다. 
    void PutRuin(SHORT x, SHORT y, SHORT type, SHORT mount);

	void PushExpansionBasePoint(SHORT x, SHORT y);

	// 작은지도가 참조할 색을 저장한다. 
	void MakeSmallMapBase();
    void UpdateSmallMapBase(SHORT x, SHORT y);


	BOOL GetRealTileByRealDot(SHORT realx, SHORT realy, SHORT* x, SHORT* y);

	void PutTreasure(SI32 six, SI32 siy, SHORT type, SI32 amount);
	void DeleteTreasure(SI32 x, SI32 y);

	// 각 플레이어의 시작 위치를 랜덤하게 바꾼다. 
	void RandomStartPos();

	SI32 GetMineMap(SI32 x, SI32 y)		 {return pMineMap[x + YOff[y]];}
//	SI32 GetMineCharMap(SI32 x, SI32 y) { return pMineCharMap[x + YOff[y]];}
	void SetMineMap(SI32 x, SI32 y, char data)
	{
		pMineMap[x + YOff[y]] = data;
//		pMineCharMap[x + YOff[y]] = ID;
	}

	SI32 GetTreasureMap(SI32 x, SI32 y){return pTreasureMap[x + YOff[y]];}
	void SetTreasureMap(SI32 x, SI32 y, SHORT data){pTreasureMap[x + YOff[y]] = data;}

	SI32 GetFireMap(SI32 x, SI32 y){return pFireMap[x + YOff[y]];}
	void SetFireMap(SI32 x, SI32 y, UI08 data){pFireMap[x + YOff[y]] = data;}

	_NewID GetFireCharMap(SI32 x, SI32 y){return pFireCharMap[x + YOff[y]];}
	SI16 GetFirePlayerMap(SI32 x, SI32 y){return pFirePlayerMap[x + YOff[y]];}
	void SetFireCharMap(SI32 x, SI32 y, _NewID siChar,SI16 siPlayer,BOOL bFire)
	{
		pFireCharMap[x + YOff[y]].ID  = siChar.ID;
		pFireCharMap[x + YOff[y]].Unique  = siChar.Unique;
		pFirePlayerMap[x + YOff[y]] = siPlayer;
		m_bFire	= bFire;
	}




	// 곡물을 랜덤하게 찍는다. 
	void SetCropRandom(SHORT centerx, SHORT centery, SHORT range, int randompercent, SHORT type);
	void SetSeaRandom(SHORT centerx, SHORT centery, SHORT range, int randompercent);
	void SetNewBlkRandom(SHORT centerx, SHORT centery, SHORT range, int randompercent);
	void SetShallowWaterRandom(SHORT centerx, SHORT centery, SHORT range, int randompercent);
	void SetGreenRandom(SHORT centerx, SHORT centery, SHORT range, int randompercent);
	void SetGrassRandom(SHORT centerx, SHORT centery, SHORT range, int randompercent);


};




// 지도의 플레이어 수를 알려준다. 
SI32 GetPlayerNumberOfMap(char* filename);

// filename의 지도가 데모용 지도인지 파악한다. 
BOOL IsMapForDemo(char* filename_ext);


#endif



#else

#ifndef _MAP_H
#define _MAP_H     1

#pragma warning ( disable : 4786 )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4018 )

#include <Player.h>


#define CLIENT_SCREEN_XSIZE  832  //(64*13)
#define CLIENT_SCREEN_YSIZE  576  //(32*18)

#define MAX_DRAWMAP_SCREEN_XSIZE 13
#define MAX_DRAWMAP_SCREEN_YSIZE 18


#define MAX_MAP_CHAR_NUMBER 800

#define MAP_TILE_XSIZE      64
#define MAP_TILE_XSIZE_HALF 32
#define MAP_TILE_YSIZE      32
#define MAP_TILE_YSIZE_HALF 16
#define HEIGHT_PER_LEVEL 16
#define MAP_BY_REAL_DIV     10


// 타일용 방위 
#define MAPTILE_SOUTH  1
#define MAPTILE_WEST   2
#define MAPTILE_NORTH  4
#define MAPTILE_EAST   8
#define MAPTILE_SW    16
#define MAPTILE_NW    32
#define MAPTILE_NE    64
#define MAPTILE_SE   128

// 언덕용 방위 
#define MAPTILEHILL_SOUTH  1
#define MAPTILEHILL_SW     2
#define MAPTILEHILL_WEST   4
#define MAPTILEHILL_NW     8
#define MAPTILEHILL_NORTH 16
#define MAPTILEHILL_NE    32
#define MAPTILEHILL_EAST  64
#define MAPTILEHILL_SE   128

#define TILE_DEFAULT       0              // 기본형 타일
#define TILE_GREEN         1    
#define TILE_CROPAREA      2               // 경작지 
#define TILE_GRASS         3              // 풀밭 
#define TILE_CASTLEAREA    4
#define TILE_BLOCK         7               // 장애물 언덕 
#define TILE_CASTLE        8


#define MAX_LEVEL_STEP     5             // 최대 층 높이 (0~4)

#define MAX_TILE_TYPE_NUMBER  15         // 언덕 타일의 최대 종류 


// 변종의 종류 
#define MAX_TILE_VARY     7
#define MAX_SEA_VARY      8
#define MAX_GRASS_VARY    6
#define MAX_GREEN_VARY    11
#define MAX_NEWBLK_VARY   2
#define MAX_CASTLE_VARY   3
#define MAX_CROPAREA_VARY 7

#define MAX_SCREEN_HALF_SIZE 16

#define MAX_SHIP_PATROL_POINT_NUMBER    50         // 배 순찰 지점 
#define MAX_SHIP_LANDING_POINT_NUMBER   50         // 배 상륙 지점 


// 타일의 각종 정보를 저장하는 곳 
class _Tile 
{
public:
   int Type;      // 어떤 언덕모양인가?
   SHORT X, Y;    // 좌상단의 변위 (맨 좌측 상단에서부터의 변위)

};

class _Castle
{
public:
	SHORT File, Font;
};

class _MapHeader{
public:
	DWORD Version;                          // 지도의 버전 
	DWORD Unique;                           // 고유 정보 
	char Text[128];                         // 문자열 정보 
	DWORD PlayerNumber ;                    // 게임에 참여하는 유저 수 .
	SHORT MapXsize, MapYsize;               // 지도의 가로 세로 사이즈 
	SHORT MapType;                          // 지도 타일의 종류 
	SI32  CheckSum;							// 지도가 해킹되어 있는지 확인한다. 
	DWORD Reserved[6];                   
};




class cltMapPlayerInfo{
public:
	POINT StartPoint;
	_PlayerInfo PI;
	DWORD Money;
	DWORD Tree;


	/////////////////////////////////////////////////////////////////
	//KIND_ON_MAX
	/////////////////////////////////////////////////////////////////
	// 게임에 등장할 수 있는 최대 캐릭터 수 
	SHORT MaxUnitNumber[200];
	// 캐릭터가 게임에 등장하게 만드는가?
	SHORT IsCharInGame[200];
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////

	void Init(SI32 pnumber)
	{
		ZeroMemory(this, sizeof(cltMapPlayerInfo));
		PI.Init(pnumber);

		StartPoint.x = -1;
		StartPoint.y = -1;

		for(int i=0;i<200;i++)
		{
			IsCharInGame[i] = TRUE;
		}
	}

};

class cltMapUnitInfo{
public:
	SI32 Kind;
	SI32 X, Y;
	SI32 Pnumber;
};

class _Map {
public:

	// 지도의 기본 정보. 
	_MapHeader				MH;
	// 게임에  등장하는 플레이어의 정보. 
	cltMapPlayerInfo		clMapPlayerInfo[MAX_MULTI_PLAYER];
	// 심어진 캐릭터의 정보 
	cltMapUnitInfo			clMapUnitInfo[MAX_MAP_CHAR_NUMBER];

		
	// 배정찰 지점	
	POINT					ShipPatrolPoint[MAX_SHIP_PATROL_POINT_NUMBER];

	// 배 상륙 지점		
	POINT					ShipLandingPoint[MAX_SHIP_LANDING_POINT_NUMBER];
	
	POINT					ExpansionBasePoint[MAX_MULTI_PLAYER];   // 확장 기지지을 수 있는 위치 
	SHORT					ExpansionBaseNumber;                    // 확장 기지의 개수 
	
	int						VirtualMapX, VirtualMapY;

	SI32					YOff[MAX_MAP_YSIZE];
	SI32					YOff2[MAX_MAP_YSIZE];

    unsigned char			Reserved[1024+640];                 // 예약되어 있는 변수들 


public:

	char					DrawForceSwitch;   // 강제로 지도를 그리게 한다. 
	char					MapDrawSwitch;                    // 지도를 그리느냐 마느냐


private:
	short					*pIDMap;
	short					*pSkyMap;

	// 충돌 속성을 저장한다. 
	short					*pCollMap;  

	short					*pBarrierMap;

	// 층  
	// 상위 4비트 level
    // 하위 4비트 levelmode
	char					*pLevelInfo;     
	unsigned char			*pLevelFile;
	unsigned char			*pLevelFont;
	unsigned char			*pLevelType;
	unsigned char			*pLevelMode[MAX_LEVEL_STEP];
	unsigned char			*pTopLevel;

	short					*pTileAtbMap;

	// 얕은 물용 지도 
	unsigned char			*pShallowMap;
	// 얕은 물용 지도 
	unsigned char			*pShallowFont;

	// 일반적인 물용 지도 
	unsigned char 			*pSeaMode;
	unsigned char 			*pSeaFont;
	unsigned char 			*pSeaFile;

	// 일반적인 장애물 지도 
	unsigned char 			*pNewBlkMode;
	unsigned char 			*pNewBlkFont;
	unsigned char 			*pNewBlkFile;
	unsigned char 			*pNewBlkLevel;


	// 성에 대한 정보를 저장한다.  
	unsigned char 			*pCastleMode;  
	unsigned char 			*pCastleFont;
	unsigned char 			*pCastleFile;

	// 곡물 데이터를 저장한다.
	unsigned char 			*pCropTypeMap;        // 곡물의 종류
	unsigned char 			*pCropVaryMap;        // 곡물의 변종
	unsigned char 			*pCropMountMap;       // 남은 곡물량 
	unsigned char 			*pCropStepMap;        // 곡물의 단계
	unsigned char			*pCropTreeMap;		  // 트리일때 설정되는 트리 타입..




public:

	// 보물 지도 
	// 0x00ff -> 식량 
	// 0xff00 -> 목재 
	SHORT * pTreasureMap;

private:
	// 지뢰지도 
	char * pMineMap;
//	SI32 * pMineCharMap;
public:

	// 불 지도 . 
	UI08*	pFireMap;
	_NewID*	pFireCharMap; 
	SI16*	pFirePlayerMap; 
	BOOL	m_bFire;


	void InitMap(SHORT xsize, SHORT ysize, BOOL bvillagemapswitch);
	void FreeMap();
	void AllocMap(SHORT xsize, SHORT ysize, BOOL bvillagemapswitch);
	void MakeVillageMap(SI32 xsize, SI32 ysize);	

	SI32 WriteIDMap(FILE *fp);
	SI32 ReadIDMap(FILE *fp);
	SI32 WriteSkyMap(FILE *fp);
	SI32 ReadSkyMap(FILE *fp);
	SI32 WriteVillageMap(FILE *fp);
	SI32 ReadVillageMap(FILE *fp);

	SI32 WriteCollMap(FILE *fp);
	SI32 ReadCollMap(FILE *fp);

	SI32 WriteLevelInfo(FILE *fp);
	SI32 ReadLevelInfo(FILE *fp);
	SI32 WriteLevelFile(FILE *fp);
	SI32 ReadLevelFile(FILE *fp);
	SI32 WriteLevelFont(FILE *fp);
	SI32 ReadLevelFont(FILE *fp);
	SI32 WriteLevelType(FILE *fp);
	SI32 ReadLevelType(FILE *fp);
	SI32 WriteLevelMode(FILE *fp);
	SI32 ReadLevelMode(FILE *fp);
	SI32 WriteTopLevel(FILE *fp);
	SI32 ReadTopLevel(FILE *fp);

	SI32 WriteTileAtbMap(FILE *fp);
	SI32 ReadTileAtbMap(FILE *fp);

	SI32 WriteShallowMap(FILE *fp);
	SI32 ReadShallowMap(FILE *fp);
	SI32 WriteShallowFont(FILE *fp);
	SI32 ReadShallowFont(FILE *fp);

	SI32 WriteSeaMode(FILE* fp);
	SI32 ReadSeaMode(FILE *fp);
	SI32 WriteSeaFont(FILE* fp);
	SI32 ReadSeaFont(FILE *fp);
	SI32 WriteSeaFile(FILE* fp);
	SI32 ReadSeaFile(FILE *fp);

	SI32 WriteNewBlkMode(FILE* fp);
	SI32 ReadNewBlkMode(FILE *fp);
	SI32 WriteNewBlkFont(FILE* fp);
	SI32 ReadNewBlkFont(FILE *fp);
	SI32 WriteNewBlkFile(FILE* fp);
	SI32 ReadNewBlkFile(FILE *fp);
	SI32 WriteNewBlkLevel(FILE* fp);
	SI32 ReadNewBlkLevel(FILE *fp);

	SI32 WriteCastleMode(FILE* fp);
	SI32 ReadCastleMode(FILE *fp);
	SI32 WriteCastleFont(FILE* fp);
	SI32 ReadCastleFont(FILE *fp);
	SI32 WriteCastleFile(FILE* fp);
	SI32 ReadCastleFile(FILE *fp);

	SI32 WriteCropTypeMap(FILE* fp);
	SI32 ReadCropTypeMap(FILE *fp);
	SI32 WriteCropVaryMap(FILE* fp);
	SI32 ReadCropVaryMap(FILE *fp);
	SI32 WriteCropMountMap(FILE* fp);
	SI32 ReadCropMountMap(FILE *fp);
	SI32 WriteCropStepMap(FILE* fp);
	SI32 ReadCropStepMap(FILE *fp);
	SI32 WriteCropTreeMap(FILE *fp);
	SI32 ReadCropTreeMap(FILE *fp);

	SI32 WriteTreasureMap(FILE* fp);
	SI32 ReadTreasureMap(FILE*fp);

	SI32 WriteMineMap(FILE* fp);
	SI32 ReadMineMap(FILE* fp);

	SI32 WriteFireMap(FILE* fp);
	SI32 ReadFireMap(FILE*fp);

	// MapByRealX[], MapByRealY[] 변수를 업데이트한다. 
    void UpdateMapByRealXY();

	void MakeLastMap();

	
	void SetLevelSlope(SHORT x, SHORT y, char levelmode)
	{
		if( pLevelInfo == NULL )	return;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return;
		pLevelInfo[x+YOff[y]] &= 0xf0;
		pLevelInfo[x+YOff[y]] |= levelmode;
	}
	
	SHORT GetLevelInfo(SHORT x, SHORT y)
	{ 
		if( pLevelInfo == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return (pLevelInfo[x+YOff[y]] & 0x0f);
	} 

    unsigned char GetLevelFile(SI32 x, SI32 y)
	{
		if( pLevelFile == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pLevelFile[x+YOff[y]];
	}
    unsigned char GetLevelFont(SI32 x, SI32 y)
	{
		if( pLevelFont == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pLevelFont[x+YOff[y]];
	}
    unsigned char GetTopLevel(SI32 x, SI32 y)
	{
		if( pTopLevel == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pTopLevel[x+YOff[y]];
	}

	SHORT GetLevelMode(SI32 level, SHORT x, SHORT y)
	{ 
		if( pLevelMode[level] == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pLevelMode[level][x+YOff[y]];
	} 

	void SetLevelMode(SI32 level, SHORT x, SHORT y, unsigned char mode)
	{ 
		if( pLevelMode[level] == NULL )	return ;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return ;
		pLevelMode[level][x+YOff[y]] = mode;
	} 

//대상용 타일 관련 함수///////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
	void	PutDaesangBattleTile( SHORT sx, SHORT sy, int type);



///////////////////////////////////////////////////////////////
//   타일 관련 함수 
//////////////////////////////////////////////////////////////
    void	SetProperFileNFont(SHORT x, SHORT y, int hilltype);
	void	PutTile(SHORT sx, SHORT sy, int type);
	void	PutTile_Daesang(SHORT sx, SHORT sy, int type);
	void	PutTile_OnlySet_Daesang( SHORT sx, SHORT sy, int type, int FileTileNo , int StartFont);
	void	SetProperTileFont_Daesang(SHORT sx, SHORT sy, int type, int FileTileNo);


	// 타일에 대한 각종 정보를 초기화한다. 
    void	InitTile();
	short	GetTileAtb(SI32 x, SI32 y)
	{
		if( pTileAtbMap == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pTileAtbMap[x+YOff[y]];
	}

    void SetMapDrawSwitch(int mode);



	
	void	SetIDMap(SHORT id, SI32 x, SI32 y)
	{
		if( pIDMap == NULL )	return;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return;
		pIDMap[x+YOff[y]] = id;
	}
    SHORT	GetIDMap(SI32 x, SI32 y)
	{
		if( pIDMap == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pIDMap[x+YOff[y]];
	}
	void	ClearIDMap(int x, int y)
	{
		if( pIDMap == NULL )	return;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return;
		
        pIDMap[x+YOff[y]] = 0;
	}
	
	
	void	SetSkyMap(SHORT id, SHORT x, SHORT y)
	{
		if( pSkyMap == NULL )	return;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return;
		pSkyMap[x+YOff[y]] = id;
		
	}
    SHORT	GetSkyMap(SHORT x, SHORT y)
	{
		if( pSkyMap == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pSkyMap[x+YOff[y]];
	}
	void	ClearSkyMap(SHORT x, SHORT y)
	{
		if( pSkyMap == NULL )	return;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return;
        pSkyMap[x+YOff[y]] = 0;
	}
	
	// 어느지점의 충돌 여부를 알려준다. 
    BOOL	IsColl(SHORT x, SHORT y, SHORT block)
	{
		if( pCollMap == NULL ) return TRUE;
		if( x<0 || x>=MH.MapXsize || y<0  || y>=MH.MapYsize )	return TRUE;
		
		if( pCollMap[x+YOff[y]] & block )	return TRUE;
		
		return FALSE;
	}
	
	// 충돌 속성을 설정한다. 
    BOOL	SetCollMap(SHORT x, SHORT y, SHORT block)
	{
		if( pCollMap == NULL )	return TRUE;
		if( x<0 || x>=MH.MapXsize || y<0  || y>=MH.MapYsize )	return TRUE;

		pCollMap[x+YOff[y]] |= block;
		return TRUE;
	}
	
	SHORT	GetCollMap(SI32 x, SI32 y)
	{
		if( pCollMap == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0  || y>=MH.MapYsize )	return 0;

		return pCollMap[x+YOff[y]];
	}
	
    // 특정 비트를 지운다. 
    void	DeleteCollMap(SHORT x, SHORT y, SHORT block)
	{
		if( pCollMap == NULL )	return;

		pCollMap[x+YOff[y]] &= (~block);
	}

	// Ice Barrier의 충돌 속성을 설정한다. 
    BOOL	SetBarrierCollMap(SHORT x, SHORT y)
	{
		if( pBarrierMap == NULL )	return TRUE;
		if( x<0 || x>=MH.MapXsize || y<0  || y>=MH.MapYsize )	return TRUE;

		pBarrierMap[x+YOff[y]] += 1;
		return TRUE;
	}

	// Ice Barrier의 충돌지정 횟수를 얻는다.
	short	GetBarrierCollMap(SHORT x, SHORT y)
	{
		if( pBarrierMap == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0  || y>=MH.MapYsize )	return 0;

		return pBarrierMap[x+YOff[y]];
	}

	void	DeleteBarrierCollMap(SHORT x, SHORT y)
	{
		if( pBarrierMap == NULL )	return;

		if(pBarrierMap[x+YOff[y]] > 0)
			pBarrierMap[x+YOff[y]] -= 1;
	}
	
	BOOL	DeleteCharCollMap(SHORT x, SHORT y);
	// 비행체의 움직임으로 인한 삭제 
    BOOL	DeleteAirCharCollMap(SHORT x, SHORT y);
	

	BOOL	IsValidShallowMap()
	{
		if( pShallowMap == NULL )	return FALSE;
		return TRUE;
	}
    unsigned char	GetShallowMap(SI32 x, SI32 y)
	{
		if( pShallowMap == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pShallowMap[x+YOff[y]];
	}
    unsigned char	GetShallowFont(SI32 x, SI32 y)
	{
		if( pShallowFont == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pShallowFont[x+YOff[y]];
	}

    
    unsigned char GetSeaMode(SI32 x, SI32 y)
	{
		if( pSeaMode == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pSeaMode[x+YOff[y]];
	}
    void SetSeaMode(UI08 seamode, SI32 x, SI32 y)
	{
		if( pSeaMode == NULL )	return ;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return ;
		pSeaMode[x+YOff[y]] = seamode;
	}

    unsigned char GetSeaFont(SI32 x, SI32 y)
	{
		if( pSeaFont == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pSeaFont[x+YOff[y]];
		//if( x<0 || x>=MH.MapXsize/2 || y<0 || y>=MH.MapYsize/2 )	return 0;
		//return pSeaFont[x+YOff2[y]];

	}
    void SetSeaFont(UI08 seafont, SI32 x, SI32 y)
	{
		if( pSeaFont == NULL )	return ;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return ;
		pSeaFont[x+YOff[y]] = seafont;
		//if( x<0 || x>=MH.MapXsize/2 || y<0 || y>=MH.MapYsize/2 )	return ;
		//pSeaFont[x+YOff2[y]] = seafont;
	}

    unsigned char GetSeaFile(SI32 x, SI32 y)
	{
		if( pSeaFile == NULL )	return 0;
		//if( x<0 || x>=MH.MapXsize/2 || y<0 || y>=MH.MapYsize/2 )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pSeaFile[x+YOff[y]];
		//return pSeaFile[x+YOff2[y]];
	}

    void SetSeaFile(UI08 seafile, SI32 x, SI32 y)
	{
		if( pSeaFile == NULL )	return ;
		//if( x<0 || x>=MH.MapXsize/2 || y<0 || y>=MH.MapYsize/2 )	return ;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return ;
		pSeaFile[x+YOff[y]] = seafile;
		//pSeaFile[x+YOff2[y]] = seafile;
	}

    unsigned char GetNewBlkMode(SI32 x, SI32 y)
	{
		if( pNewBlkMode == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pNewBlkMode[x+YOff[y]];
	}
    void SetNewBlkMode(unsigned char newblkmode, SI32 x, SI32 y)
	{
		if( pNewBlkMode == NULL )	return;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return;
		pNewBlkMode[x+YOff[y]] = newblkmode;
	}
    unsigned char GetNewBlkFont(SI32 x, SI32 y)
	{
		if( pNewBlkFont == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize/2 || y<0 || y>=MH.MapYsize/2 )	return 0;
		return pNewBlkFont[x+YOff2[y]];
	}
    void SetNewBlkFont(unsigned char newblkfont, SI32 x, SI32 y)
	{
		if( pNewBlkFont == NULL )	return;
		if( x<0 || x>=MH.MapXsize/2 || y<0 || y>=MH.MapYsize/2 )	return;
		pNewBlkFont[x+YOff2[y]] = newblkfont;
	}
    unsigned char GetNewBlkFile(SI32 x, SI32 y)
	{
		if( pNewBlkFile == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize/2 || y<0 || y>=MH.MapYsize/2 )	return 0;
		return pNewBlkFile[x+YOff2[y]];
	}
    void SetNewBlkFile(unsigned char newblkfile, SI32 x, SI32 y)
	{
		if( pNewBlkFile == NULL )	return;
		if( x<0 || x>=MH.MapXsize/2 || y<0 || y>=MH.MapYsize/2 )	return;
		pNewBlkFile[x+YOff2[y]] = newblkfile;
	}
    unsigned char GetNewBlkLevel(SI32 x, SI32 y)
	{
		if( pNewBlkLevel == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pNewBlkLevel[x+YOff[y]];
	}
    void SetNewBlkLevel(UI08 blklevel, SI32 x, SI32 y)
	{
		if( pNewBlkLevel == NULL )	return ;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return ;
		pNewBlkLevel[x+YOff[y]] = blklevel;
	}

    unsigned char GetCastleMode(SI32 x, SI32 y)
	{
		if( pCastleMode == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pCastleMode[x+YOff[y]];
	}


    void SetCastleMode(UI08 castlemode, SI32 x, SI32 y)
	{
		if( pCastleMode == NULL )	return ;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return ;
		pCastleMode[x+YOff[y]] = castlemode;
	}

    unsigned char GetCastleFont(SI32 x, SI32 y)
	{
		if( pCastleFont == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize/2 || y<0 || y>=MH.MapYsize/2 )	return 0;
		return pCastleFont[x+YOff2[y]];
	}

    void SetCastleFont(UI08 castlefont, SI32 x, SI32 y)
	{
		if( pCastleFont == NULL )	return ;
		if( x<0 || x>=MH.MapXsize/2 || y<0 || y>=MH.MapYsize/2 )	return ;
		pCastleFont[x+YOff2[y]] = castlefont;
	}

    unsigned char GetCastleFile(SI32 x, SI32 y)
	{
		if( pCastleFile == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize/2 || y<0 || y>=MH.MapYsize/2 )	return 0;
		return pCastleFile[x+YOff2[y]];
	}

    void SetCastleFile(UI08 castlefile, SI32 x, SI32 y)
	{
		if( pCastleFile == NULL )	return ;
		if( x<0 || x>=MH.MapXsize/2 || y<0 || y>=MH.MapYsize/2 )	return ;
		pCastleFile[x+YOff2[y]] = castlefile;
	}

    unsigned char GetCropTypeMap(SI32 x, SI32 y)
	{
		if( pCropTypeMap == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pCropTypeMap[x+YOff[y]];
	}
    void SetCropTypeMap(UI08 croptype, SI32 x, SI32 y)
	{
		if( pCropTypeMap == NULL )	return ;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return ;
		pCropTypeMap[x+YOff[y]] = croptype;
	}

	unsigned char GetCropTreeMap(SI32 x, SI32 y)
	{
		if( pCropTreeMap == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pCropTreeMap[x+YOff[y]];
	}
	
    void SetCropTreeMap(UI08 treetype, SI32 x, SI32 y)
	{
		if( pCropTreeMap == NULL )	return ;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return ;
		pCropTreeMap[x+YOff[y]] = treetype;
	}
	


    unsigned char GetCropVaryMap(SI32 x, SI32 y)
	{
		if( pCropVaryMap == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pCropVaryMap[x+YOff[y]];
	}
    void SetCropVaryMap(UI08 cropvary, SI32 x, SI32 y)
	{
		if( pCropVaryMap == NULL )	return ;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return ;
		pCropVaryMap[x+YOff[y]] = cropvary;
	}

    unsigned char GetCropMountMap(SI32 x, SI32 y)
	{
		if( pCropMountMap == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pCropMountMap[x+YOff[y]];
	}
	void SetCropMountMap(unsigned char cropmount, SI32 x, SI32 y)
	{
		if( pCropMountMap == NULL )	return;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return;
		pCropMountMap[x+YOff[y]] = cropmount;
	}
    unsigned char GetCropStepMap(SI32 x, SI32 y)
	{
		if( pCropStepMap == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pCropStepMap[x+YOff[y]];
	}
	void SetCropStepMap(unsigned char cropstep, SI32 x, SI32 y)
	{
		if( pCropStepMap == NULL )	return;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return;
		pCropStepMap[x+YOff[y]] = cropstep;
	}
    void IncreaseCropStepMap(SI32 x, SI32 y)
	{
		if( pCropStepMap == NULL )	return;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return;
		pCropStepMap[x+YOff[y]]++;
	}
	
	
	SHORT	GetXsize(){ return MH.MapXsize; }
	SHORT	GetYsize(){ return MH.MapYsize; }
	
	
	SHORT	GetVirtualMapX();
	SHORT	GetVirtualMapY();
	
	// x, y가 지도 좌표 안에 있는가?
	// x, y 기본좌표 
	BOOL	IsInMapArea(SHORT x, SHORT y)
	{
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return FALSE;
		return TRUE;
	}
	// x, y좌표가 화면안에 있는지 확인한다.
    BOOL	IsInScreen(SHORT x, SHORT y);
	
	
	// 지도상의 X, Y좌표의 중심의 좌표를 돌려준다. (절대 좌표)
	BOOL	GetRealDotCenterXY(SHORT x, SHORT y, int & centerx, int & centery);
	
    // 지도상의 X, Y좌표의 중심의 좌표를 돌려준다. (화면좌표)
    BOOL	GetCenterXYByMapCenterXY(SHORT x, SHORT y, SHORT & centerx, SHORT & centery);
	
	// 성곽등을 고려한 지도상의 X, Y좌표의 중심의 좌표를 돌려준다. (절대좌표)
	// 기준 좌표 
	// 지도상의 X, Y좌표의 중심의 좌표를 돌려준다. (화면 좌표)
    BOOL	GetCenterXY(SHORT X, SHORT Y, SHORT & centerx, SHORT & centery);
    BOOL	GetRefXY(SHORT X, SHORT Y, SHORT & centerx, SHORT & centery);
	
	
    // 절대 xy 좌표를 근거로 해서 지도 좌표를 구한다. 
    BOOL	DecideMapXYByXY(SHORT x, SHORT y, SHORT& map_x, SHORT& map_y, SHORT & map_center_x, SHORT & map_center_y);
    // 절대 좌표를 근거로 해서 기본 좌표를 구한다. 
    BOOL	DecideMapXYByXYLow(SHORT x, SHORT y, SHORT& map_x, SHORT& map_y, SHORT & map_center_x, SHORT & map_center_y);


    // 절대 좌표를 화면 좌표로 변환한다.
    void	TransFromRealDotToScreenDot(SHORT realx, SHORT realy, SHORT& screenx, SHORT &screeny);
    // 화면 좌표를 절대 좌표로 변환한다.
    void	TransFromScreenToRealXY(SHORT screenx, SHORT screeny, SHORT& realx, SHORT &realy);


    int		GetTileArrayNumber(SHORT x, SHORT y, int atb2);
    // 접경타일을 구한다. 
    int		GetNeighbourTileType(SHORT sx, SHORT sy);
    

	SHORT	GetHillType(SHORT x, SHORT y)
	{ 
		if( pLevelType == NULL )	return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return pLevelType[x+YOff[y]];
	}



	// 특정 좌표의 색을 구한다. 
    unsigned char	GetColor(SHORT x, SHORT y);
	// x, y위치의 타일의 화일번호와 폰트 번호를 구한다. 
    BOOL	GetMapFileNFont(SHORT x, SHORT y, SHORT& file, SHORT &font);


	//////////////////////////////////////////////////////////////
	// 곡물 관련 함수 
	/////////////////////////////////////////////////////////////
	//TreeType은 Tree일때만 설정되고 아닐때는 -1
	BOOL	IsCrop(SHORT x, SHORT y);
    void	PutCrop(SHORT x, SHORT y, SHORT type, SHORT TreeType);
	void	PutCrop(SHORT x, SHORT y, SHORT type, SHORT vary, SHORT mount, SHORT TreeType);

	void	PutTree(SHORT x, SHORT y, SHORT type, SHORT TreeType);

	// 곡물을 줄인다. 
    SHORT	DecreaseCrop(SHORT x, SHORT y, SHORT amount);


	// 주변에 나무 심기가 적당한가?
    BOOL	IsGoodForTree(SHORT x, SHORT y);



	BOOL	IncreaseVirtualMapX(int mount);
	BOOL	DecreaseVirtualMapX(int mount);
	BOOL	IncreaseVirtualMapY(int mount);
	BOOL	DecreaseVirtualMapY(int mount);


	// mousex, mousey가 클라이언트 영역에 있는가?
    BOOL	IsPtInMap(SHORT mouse_x, SHORT mouse_y);

	// 지상이 비어 있는가?
    // 지상 캐릭터가 있는곳, 물, 일반 장애물
    BOOL	IsEmpty(SHORT x, SHORT y)
	{
	    if( x<0  || x>=MH.MapXsize )	return FALSE;
	    if( y<=0 || y>=MH.MapYsize )	return FALSE;
		if( pIDMap == NULL )	return FALSE;
		if( pCollMap == NULL )	return FALSE;
	    if( pIDMap[x+YOff[y]] )	return FALSE;
	    if( pCollMap[x+YOff[y]] & (BLOCK_DEEPWATER|BLOCK_MOVE|BLOCK_NORMAL|BLOCK_RUIN|BLOCK_CASTLE|BLOCK_NEWBLK|BLOCK_TREE|BLOCK_ICEWALL) )
			return FALSE;
	    return TRUE;
	}

	// 지상이 비어 있는가?
    // 지상 캐릭터가 있는곳, 물, 일반 장애물
    BOOL	IsEmptyWater(SHORT x, SHORT y)
	{
	    if( x<0  || x>=MH.MapXsize )	return FALSE;
	    if( y<=0 || y>=MH.MapYsize )	return FALSE;
		if( pIDMap == NULL )	return FALSE;
		if( pCollMap == NULL )	return FALSE;
  	    if( pIDMap[x+YOff[y]] )	return FALSE;
	    if( (pCollMap[x+YOff[y]] & BLOCK_DEEPWATER) == 0 )	return FALSE;
	    return TRUE;
	}


    // 지상이 비어 있는가?
    // 지상 캐릭터가 있는곳, 물, 일반 장애물
    BOOL	IsEmpty(SHORT x, SHORT y, SHORT xsize, SHORT ysize, SHORT idx, SHORT idy, SHORT idxsize, SHORT idysize);

	// 비어 있는가?
    BOOL	IsEmptyWater(SHORT x, SHORT y, SHORT xsize, SHORT ysize, SHORT idx, SHORT idy, SHORT idxsize, SHORT idysize);


	// x,y에 건물을 지을 수 있는 곳인가?
    // x,y 기본 좌표 
    BOOL	CanBuild(SHORT x,SHORT y, SHORT kind);

	// x,y에 건물을 지을 수 있는 곳인가?
    // x,y 기본 좌표 
    BOOL	CanBuild(SHORT x,SHORT y, _NewID builderid, SHORT kind);

	// x, y :기본 좌표 
    BOOL	CanBuild(SHORT x, SHORT y, SHORT xsize, SHORT ysize, SHORT kind);

    // kind종류의 건물을 지을 수 있는가?
    // x , y :기본 좌표 
    BOOL	CanBuildByKind(SHORT x, SHORT y, SHORT kind);
	// kind종류의 건물을 지을 수 있는가?
    // x, y :기본 좌표 
    BOOL	CanBuildByKind(SHORT x, SHORT y, SHORT kind, _NewID builderid);

	// 조선소를 지을 수 있는지 확인한다. 
    BOOL	CanBuildShipyard(SHORT x,SHORT y, SHORT xsize, SHORT ysize, _NewID builderid);

//////////////////////////////////////////////////////////////////
///  언덕 제작 관련 
/////////////////////////////////////////////////////////////////
	// sx, sy주변의 층 배열을 확인한다. 
    // sx, sy:2배수 좌표 
    int		GetLevelNear1(SHORT sx, SHORT sy, SHORT level);
	int		GetLevelNear2(SHORT sx, SHORT sy, SHORT level);

    void	PutLevel(SHORT x, SHORT y, SHORT level, SHORT mode);
	// 높이를 지운다. 
    void	DeleteLevel(SHORT x, SHORT y, SHORT level);


    //  level 설정 및 구하기 관련 함수 
	void	SetLevel(SHORT x, SHORT y, SHORT level);

	SHORT	GetLevel(SHORT x, SHORT y);
	// 성곽등을 고려한 최종 높이 
    SHORT	GetLastLevel(SHORT x, SHORT y);

    // 레벨에 따라 적절한 타일을 조합한다. 
    BOOL	PutProperTile(SHORT sx, SHORT sy, int level);

	// 높이를 구한다.경사지는 -1을 리턴하고 나머지 지역은 높이를 리턴한다. 
	SHORT	GetHillLayer(SHORT x, SHORT y);


////////////////////////////////////////////////////////
//  물관련 
////////////////////////////////////////////////////////
	// sx, sy주변의 물 배열을 확인한다. 
    // sx, sy:4배수 좌표 
    int		GetSeaNear1(SHORT sx, SHORT sy);
	int		GetSeaNear2(SHORT sx, SHORT sy);

	// 물은 만들지 않고 얕은 영역만 만든다. 
    void	PutShallow(SHORT x, SHORT y, SHORT mode);
    void	PutShallowSea(SHORT sx, SHORT sy, SHORT mode);
 	// 일반적인 물을 심는다. 
    // sx, sy: 4배수 좌표 
    void	PutSea(SHORT sx, SHORT sy, SHORT mode, SHORT pushmode);	
	void	PutSea_Daesang(SHORT sx, SHORT sy, SHORT mode );//대상용 타일찍기..
	void	PutSea_OnlySet_Daesang( SHORT sx, SHORT sy, SHORT mode , SHORT startfont);
	void	SetProperSeaFont_Daesang(SHORT sx, SHORT sy , SHORT mode );


	// 주변의 상황을 고려하여 충돌영역을 설정한다. 
    // x, y :4배수 좌표 
    void	PutCollSea(SHORT x, SHORT y);


/////////////////////////////////////////////////////
// 장애물용 언덕 관련 
///////////////////////////////////////////////////
    int GetNewBlkNear1(SHORT sx, SHORT sy);
	int GetNewBlkNear2(SHORT sx, SHORT sy);
	void PutNewBlk(SHORT sx, SHORT sy, SHORT mode);

	// 주변의 상황을 고려하여 충돌영역을 설정한다. 
    // x, y :2배수 좌표 
    void PutCollNewBlk(SHORT x, SHORT y);
    // x, y 4배수 좌표 
    void PutNewBlkGate(SHORT sx, SHORT sy);
    // x, y 4배수 좌표 
    SHORT GetNewBlkGateMode(SHORT sx, SHORT sy);


//////////////////////////////////////////////////////
// 성곽 관련 함수
/////////////////////////////////////////////////////
    int GetCastleNear1(SHORT sx, SHORT sy);
	int GetCastleNear2(SHORT sx, SHORT sy);
	void PutCastle(SHORT sx, SHORT sy, SHORT mode);
	void PutCastleGate(SHORT sx, SHORT sy, SHORT dir, SHORT mode);

	// x, y주변의 성이 위차한 방위를 돌려준다. 


    // 주변의 상황을 고려하여 충돌영역을 설정한다. 
    // x, y :4배수 좌표 
    void PutCollCastle(SHORT x, SHORT y);

//////////////////////////////////////////////////////////////////////////////////
/// 그리기 함수 
/////////////////////////////////////////////////////////////////////////////////
	// 지도를 그린다. 
    BOOL DrawMap();
    BOOL DrawObj();
	BOOL DrawMapEntire();

	BOOL IsInCrop(SHORT mapx, SHORT mapy, SHORT mousex, SHORT mousey, SHORT &length);
	void DecideCropFileNFont(SHORT mapx, SHORT mapy, SHORT &file, SHORT &font);
	void DrawCrop(SHORT x, SHORT y, SHORT mapx, SHORT mapy, unsigned char * table);

	// 지뢰를 그린다. 
    void DrawMine(SHORT x, SHORT y, SHORT mapx, SHORT mapy, unsigned char * table);

	// 불의 수명을 가져온다.
	SI32 GetFireLifeTime(SI32 x, SI32 y);
	// 땅에 붙은 불을 그린다.
	void DrawFire(SHORT x, SHORT y, SHORT mapx, SHORT mapy, unsigned char* table);

	// 불을 심는다. 
	void PutFire(SI32 siX, SI32 siY, SI32 siFireType, SI32 siLifeTime,_NewID siChar,SI16 siPlayer);

	BOOL DecreaseFireLifeTime(SI32 siX, SI32 siY, SI32 siDecreaseFactor);

	void DrawTreasure(SHORT x, SHORT y, SHORT mapx, SHORT mapy, unsigned char * table, SHORT type);


	BOOL DrawDragBox(SHORT x1, SHORT y1, SHORT x2, SHORT y2);

	// 지도 요소를 그리는 함수들 
    // 언덕을 그린다. 
    // x, y:2배수좌표 
    BOOL DrawHill(SHORT centerx, SHORT centery, SHORT x, SHORT y,  SHORT mode);
	BOOL DrawHillBlack(SHORT centerx, SHORT centery, SHORT x, SHORT y);

    // x, y:2배수좌표 
    BOOL DrawFog(SHORT centerx, SHORT centery, SHORT x, SHORT y, SHORT dirmode);

	// x, y:2배수좌표 
    BOOL DrawMousePos(SHORT centerx, SHORT centery, SHORT x, SHORT y, unsigned char * table, SHORT mode);


	// 얕은 물을 그린다. 
    // x, y:2배수좌표 
    BOOL DrawShallow(SHORT centerx, SHORT centery, SHORT x, SHORT y, unsigned char * table);

	// 물을 그린다. 
    // x, y:2배수좌표 
    BOOL DrawSea(SHORT centerx, SHORT centery, SHORT x, SHORT y );	
	// x, y:2배수좌표 
    BOOL DrawNewBlk(SHORT centerx, SHORT centery, SHORT x, SHORT y);

	// 성을그린다.
    // x, y:4배수좌표 
    BOOL DrawCastle(SHORT centerx, SHORT centery, SHORT x, SHORT y);


	void DrawBuilding(SHORT kind, SHORT buildx, SHORT buildy, SHORT canbuildswitch, SHORT dir);
	// 플레이어 시작 위치를 그려준다. 
    void DrawPlayerStartPoint(SHORT x, SHORT y, SHORT pnumber);

	void DrawShipPatrol();
	void DrawShipLanding();
	// 배 순찰지점을 그려준다.
    void DrawShipPatrol(SHORT x, SHORT y);
	// 배 상륙지점을 그려준다.
    void DrawShipLanding(SHORT x, SHORT y);

	void DrawTile(SI32 file, SI32 font, SI32 x, SI32 y, SI32 startx, SI32 starty);


////////////////////////////////////////////////////////////////////
/// 지도를 저장하거나 불러온다. 
///////////////////////////////////////////////////////////////////
    // 지도를 저장한다. 
    BOOL Save(char* filename);
    // 지도를 불러온다. 
    BOOL Load(char* filename);

	// 화일이름을 갖고 지도의 정보를 얻어온다. 
	BOOL GetMapInfo(char* filename, _MapHeader* header);

	// x, y기본 좌표 
	BOOL IsLand(SHORT x, SHORT y)
	{
		if( pCollMap == NULL )	return TRUE;
		if( pCollMap[x+YOff[y]] & BLOCK_DEEPWATER )		return FALSE;   
		return TRUE;
	}
	
	// x, y기본 좌표 
	BOOL IsSea(SHORT x, SHORT y)
	{
		if( pCollMap == NULL )	return FALSE;
		if( pCollMap[x+YOff[y]] & BLOCK_DEEPWATER )	return TRUE;
		return FALSE;
	}

/*	// x, y기본 좌표 
    SHORT GetSector(SHORT x, SHORT y)
	{
		if(pSectorMap== NULL)return 0;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
		return (SHORT) pSectorMap[x+YOff[y]];
	}
	
	// x, y기본 좌표 
    void SetSector(SHORT x, SHORT y, unsigned char sector)
	{
		if(pSectorMap== NULL)return ;
		if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return;
		pSectorMap[x+YOff[y]]=sector;
	}
*/
	
	void SetVirtualPos(SI16 siX, SI16 siY)
	{
		VirtualMapX  = siX;
		VirtualMapY  = siY;
	}


	// 지도에 각종 지형지물을 넣는다. 
    void PutRuin(SHORT x, SHORT y, SHORT type, SHORT mount);

	void PushExpansionBasePoint(SHORT x, SHORT y);

	// 작은지도가 참조할 색을 저장한다. 
	void MakeSmallMapBase();
    void UpdateSmallMapBase(SHORT x, SHORT y);


	BOOL GetRealTileByRealDot(SHORT realx, SHORT realy, SHORT* x, SHORT* y);

	void PutTreasure(SI32 six, SI32 siy, SHORT type, SI32 amount);
	void DeleteTreasure(SI32 x, SI32 y);

	// 각 플레이어의 시작 위치를 랜덤하게 바꾼다. 
	void RandomStartPos();

	SI32 GetMineMap(SI32 x, SI32 y)		 {return pMineMap[x + YOff[y]];}
//	SI32 GetMineCharMap(SI32 x, SI32 y) { return pMineCharMap[x + YOff[y]];}
	void SetMineMap(SI32 x, SI32 y, char data)
	{
		pMineMap[x + YOff[y]] = data;
//		pMineCharMap[x + YOff[y]] = ID;
	}

	SI32 GetTreasureMap(SI32 x, SI32 y){return pTreasureMap[x + YOff[y]];}
	void SetTreasureMap(SI32 x, SI32 y, SHORT data){pTreasureMap[x + YOff[y]] = data;}

	SI32 GetFireMap(SI32 x, SI32 y){return pFireMap[x + YOff[y]];}
	void SetFireMap(SI32 x, SI32 y, UI08 data){pFireMap[x + YOff[y]] = data;}

	_NewID GetFireCharMap(SI32 x, SI32 y){return pFireCharMap[x + YOff[y]];}
	SI16 GetFirePlayerMap(SI32 x, SI32 y){return pFirePlayerMap[x + YOff[y]];}
	void SetFireCharMap(SI32 x, SI32 y, _NewID siChar,SI16 siPlayer,BOOL bFire)
	{
		pFireCharMap[x + YOff[y]].ID  = siChar.ID;
		pFireCharMap[x + YOff[y]].Unique  = siChar.Unique;
		pFirePlayerMap[x + YOff[y]] = siPlayer;
		m_bFire	= bFire;
	}




	// 곡물을 랜덤하게 찍는다. 
		//TreeType 은 Tree일때만 설정한다.
	void SetCropRandom(SHORT centerx, SHORT centery, SHORT range, int randompercent, SHORT type, SHORT TreeType);	

	void SetSeaRandom(SHORT centerx, SHORT centery, SHORT range, int randompercent);
	void SetNewBlkRandom(SHORT centerx, SHORT centery, SHORT range, int randompercent);
	void SetShallowWaterRandom(SHORT centerx, SHORT centery, SHORT range, int randompercent);
	void SetGreenRandom(SHORT centerx, SHORT centery, SHORT range, int randompercent);
	void SetGrassRandom(SHORT centerx, SHORT centery, SHORT range, int randompercent);


};




// 지도의 플레이어 수를 알려준다. 
SI32 GetPlayerNumberOfMap(char* filename);

// filename의 지도가 데모용 지도인지 파악한다. 
BOOL IsMapForDemo(char* filename_ext);


#endif

#endif