#ifndef _ROCKET_NEW_TILE_MAP

//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>
#include <main.h>
#include <map.h>
#include "bindjxfile.h"

extern	SHORT 		MapEditMode;

// 화일이름을 갖고 지도의 정보를 얻어온다. 
BOOL _Map::GetMapInfo(char* filename, _MapHeader* header)
{
	FILE* fp;
	int numread;

	fp=fopen(filename, "rb");
	if(fp==NULL)
	{
		return FALSE;
	}

	numread = fread(header, sizeof(_MapHeader), 1, fp);

	fclose(fp);

	return TRUE;

}


// 지도를 저장한다. 
BOOL _Map::Save(char* filename)
{

	SI32 size = 0;
	FILE* fp;

	DWORD i ;

	// 버전도 기록한다. 
	MH.Version = 3;

/*	// 체크섬을 알아낸다. 
	for(i=0;i<sizeof(_Map);i++)
	{

	}
*/
	// 게임에 참여하는 플레이어 수도 기록한다.
	MH.PlayerNumber = 0;
	for(i=0;i<MAX_MULTI_PLAYER;i++)
	{
		if(clMapPlayerInfo[i].StartPoint.x>=0 && clMapPlayerInfo[i].StartPoint.y>=0)
		{
			MH.PlayerNumber++;
		}
	}

	fp=fopen(filename, "wb");

	if(fp==NULL)
	{
		return FALSE;
	}

	
	size += fwrite(&MH,					sizeof(_MapHeader),			1,								fp);
	size += fwrite(clMapPlayerInfo,		sizeof(cltMapPlayerInfo),	MAX_MULTI_PLAYER,				fp);
	size += fwrite(clMapUnitInfo,		sizeof(cltMapUnitInfo),		MAX_MAP_CHAR_NUMBER,			fp);
	size += fwrite(ShipPatrolPoint,		sizeof(POINT),				MAX_SHIP_PATROL_POINT_NUMBER,	fp);
	size += fwrite(ShipLandingPoint,	sizeof(POINT),				MAX_SHIP_LANDING_POINT_NUMBER,	fp);
	size += fwrite(ExpansionBasePoint,	sizeof(POINT),				MAX_MULTI_PLAYER,				fp);
	size += fwrite(&ExpansionBaseNumber,sizeof(SHORT),				1,								fp);
	size += fwrite(&VirtualMapX,		sizeof(int),				1,								fp);
	size += fwrite(&VirtualMapY,		sizeof(int),				1,								fp);
	size += fwrite(YOff,				sizeof(SI32),				180/*MAX_MAP_YSIZE*/,					fp);
	size += fwrite(YOff2,				sizeof(SI32),				180/*MAX_MAP_YSIZE*/,					fp);
	size += fwrite(Reserved,			sizeof(unsigned char),		1024,							fp);

	size += fwrite(&DrawForceSwitch,	sizeof(char), 1, fp);
	size += fwrite(&MapDrawSwitch,		sizeof(char), 1, fp);

	

	size += WriteIDMap(fp);
	size += WriteSkyMap(fp);
	size += WriteCollMap(fp);
	size += WriteLevelInfo(fp);
	size += WriteLevelFile(fp);
	size += WriteLevelFont(fp);
	size += WriteLevelType(fp);
	size += WriteLevelMode(fp);
	size += WriteTopLevel(fp);
	size += WriteTileAtbMap(fp);
	size += WriteShallowMap(fp);
	size += WriteShallowFont(fp);
	size += WriteSeaMode(fp);
	size += WriteSeaFont(fp);
	size += WriteSeaFile(fp);

	size += WriteNewBlkMode(fp);
	size += WriteNewBlkFont(fp);
	size += WriteNewBlkFile(fp);
	size += WriteNewBlkLevel(fp);
	size += WriteCastleMode(fp);
	size += WriteCastleFont(fp);
	size += WriteCastleFile(fp);

	size += WriteCropTypeMap(fp);
	size += WriteCropVaryMap(fp);
	size += WriteCropMountMap(fp);
	size += WriteCropStepMap(fp);

	size += WriteTreasureMap(fp);
	size += WriteMineMap(fp);
	size += WriteFireMap(fp);

	fclose(fp);

	

	return TRUE;
}                     



// 지도를 불러온다. 
BOOL _Map::Load(char* filename)
{
	SI32 size = 0;
	FILE* fp;

	SI32 i, j ;

	if(MapEditMode==TRUE)
		fp = fopen(filename, "rb");
	else
		fp = BindJXFile::GetInstance()->GetFile( filename );	

	if(fp==NULL)
	{
		return FALSE;
	}

	
	size += fread(&MH,					sizeof(_MapHeader),			1,								fp);
	size += fread(clMapPlayerInfo,		sizeof(cltMapPlayerInfo),	MAX_MULTI_PLAYER,				fp);
	size += fread(clMapUnitInfo,		sizeof(cltMapUnitInfo),		MAX_MAP_CHAR_NUMBER,			fp);
	size += fread(ShipPatrolPoint,		sizeof(POINT),				MAX_SHIP_PATROL_POINT_NUMBER,	fp);
	size += fread(ShipLandingPoint,		sizeof(POINT),				MAX_SHIP_LANDING_POINT_NUMBER,	fp);
	size += fread(ExpansionBasePoint,	sizeof(POINT),				MAX_MULTI_PLAYER,				fp);
	size += fread(&ExpansionBaseNumber,	sizeof(SHORT),				1,								fp);
	size += fread(&VirtualMapX,			sizeof(int),				1,								fp);
	size += fread(&VirtualMapY,			sizeof(int),				1,								fp);
	size += fread(YOff,					sizeof(SI32),				180/*MAX_MAP_YSIZE*/,			fp);
	size += fread(YOff2,				sizeof(SI32),				180/*MAX_MAP_YSIZE*/,			fp);
	size += fread(Reserved,				sizeof(unsigned char),		1024,							fp);

	size += fread(&DrawForceSwitch,		sizeof(char), 1, fp);
	size += fread(&MapDrawSwitch,		sizeof(char), 1, fp);


	size += ReadIDMap(fp);
	size += ReadSkyMap(fp);
	size += ReadCollMap(fp);
	size += ReadLevelInfo(fp);
	size += ReadLevelFile(fp);
	size += ReadLevelFont(fp);
	size += ReadLevelType(fp);
	size += ReadLevelMode(fp);
	size += ReadTopLevel(fp);
	size += ReadTileAtbMap(fp);
	size += ReadShallowMap(fp);
	size += ReadShallowFont(fp);
	size += ReadSeaMode(fp);
	size += ReadSeaFont(fp);
	size += ReadSeaFile(fp);
	

	size += ReadNewBlkMode(fp);
	size += ReadNewBlkFont(fp);
	size += ReadNewBlkFile(fp);
	size += ReadNewBlkLevel(fp);
	size += ReadCastleMode(fp);
	size += ReadCastleFont(fp);
	size += ReadCastleFile(fp);

	size += ReadCropTypeMap(fp);
	size += ReadCropVaryMap(fp);
	size += ReadCropMountMap(fp);
	size += ReadCropStepMap(fp);

	size += ReadTreasureMap(fp);
	size += ReadMineMap(fp);
	size += ReadFireMap(fp);

	fclose(fp);


	// 오프셋을 구한다. 
	for(i=0;i<MH.MapYsize;i++)
	{
		YOff[i] = i* MH.MapXsize;
	}
	for(i=0;i < (MH.MapYsize/2);i++)
	{
		YOff2[i]	= i * (MH.MapXsize/2);
	}


	
	for(i=0;i<MH.MapYsize;i++)
		for(j=0;j<MH.MapXsize;j++)
		{
			SetIDMap(0, j, i);
		}
		
	for(i=0;i<MH.MapYsize;i++)
		for(j=0;j<MH.MapXsize;j++)
		{
			pCollMap[j+YOff[i]] = 0;
		}

	if(!pBarrierMap) pBarrierMap = new short[MH.MapXsize * MH.MapYsize];
	if(pBarrierMap)	ZeroMemory(pBarrierMap, sizeof(short)*MH.MapXsize*MH.MapYsize);
	
	return TRUE;

}


SI32 _Map::WriteIDMap(FILE* fp)
{
	SI32 size;

	if( pIDMap == NULL )	return FALSE;
	size = fwrite(pIDMap, sizeof(short), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::ReadIDMap(FILE*fp)
{
	SI32 size;

	if( pIDMap )	return FALSE;

	pIDMap = new short[MH.MapXsize * MH.MapYsize];
	if( pIDMap == NULL )	return FALSE;

	size = fread(pIDMap, sizeof(short), MH.MapXsize * MH.MapYsize, fp);
	return size;
}


SI32 _Map::WriteSkyMap(FILE* fp)
{
	SI32 size;

	if(pSkyMap==NULL)	return FALSE;

	size = fwrite(pSkyMap,	sizeof(short),            MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::ReadSkyMap(FILE*fp)
{
	SI32 size;

	if(pSkyMap)	return FALSE;

	pSkyMap = new short[MH.MapXsize*MH.MapYsize];
	if(pSkyMap==NULL)	return FALSE;

	size = fread(pSkyMap,	sizeof(short),            MH.MapXsize * MH.MapYsize, fp);
	return size;
}


SI32 _Map::WriteCollMap(FILE* fp)
{
	SI32 size;

	if( pCollMap == NULL )	return FALSE;

	size = fwrite(pCollMap, sizeof(short),	MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::ReadCollMap(FILE*fp)
{
	SI32 size;

	if( pCollMap )	return FALSE;

	pCollMap = new short[MH.MapXsize * MH.MapYsize];
	if( pCollMap == NULL )	return FALSE;

	size = fread(pCollMap, sizeof(short), MH.MapXsize * MH.MapYsize, fp);
	return size;
}


SI32 _Map::WriteLevelInfo(FILE* fp)
{
	SI32 size;

	if( pLevelInfo == NULL )	return FALSE;

	size = fwrite(pLevelInfo, sizeof(char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::ReadLevelInfo(FILE*fp)
{
	SI32 size;

	if( pLevelInfo )	return FALSE;

	pLevelInfo = new char[MH.MapXsize * MH.MapYsize];
	if( pLevelInfo == NULL )	return FALSE;

	size = fread(pLevelInfo, sizeof(char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::WriteMineMap(FILE* fp)
{
	SI32 size;

	if( pMineMap == NULL )	return FALSE;

	size = fwrite(pMineMap, sizeof(char), MH.MapXsize * MH.MapYsize, fp);
	
	return size;
}

SI32 _Map::ReadMineMap(FILE* fp)
{
	SI32 size;

	if( pMineMap )	return FALSE;

	pMineMap = new char[MH.MapXsize * MH.MapYsize];
	if( pMineMap == NULL )	return FALSE;

//	if( pMineCharMap )	return FALSE;

//	pMineCharMap = new SI32[MH.MapXsize * MH.MapYsize];
//	if( pMineCharMap == NULL )	return FALSE;


	size = fread(pMineMap, sizeof(char), MH.MapXsize * MH.MapYsize, fp);
	
	return size;
}



SI32 _Map::WriteLevelFile(FILE* fp)
{
	SI32 size;

	if( pLevelFile == NULL )	return FALSE;

	size = fwrite(pLevelFile, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::ReadLevelFile(FILE*fp)
{
	SI32 size;

	if( pLevelFile )	return FALSE;

	pLevelFile = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pLevelFile == NULL )	return FALSE;

	size = fread(pLevelFile, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}



SI32 _Map::WriteLevelFont(FILE* fp)
{
	SI32 size;

	if( pLevelFont == NULL )	return FALSE;

	size = fwrite(pLevelFont, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::ReadLevelFont(FILE *fp)
{
	SI32 size;

	if( pLevelFont )	return FALSE;

	pLevelFont = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pLevelFont == NULL )	return FALSE;

	size = fread(pLevelFont, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}


SI32 _Map::WriteLevelType(FILE* fp)
{
	SI32 size;

	if( pLevelType == NULL )	return FALSE;

	size = fwrite(pLevelType, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::ReadLevelType(FILE *fp)
{
	SI32 size;

	if( pLevelType )	return FALSE;

	pLevelType = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pLevelType == NULL )	return FALSE;

	size = fread(pLevelType, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}


SI32 _Map::WriteLevelMode(FILE* fp)
{
	SI32 size = 0;

	SI32 i;

	if(pLevelMode[0] == NULL)return FALSE;

	for(i = 0;i < MAX_LEVEL_STEP;i++)
	{
		size += fwrite(pLevelMode[i],			sizeof(unsigned char),	MH.MapXsize * MH.MapYsize, fp);
	}

	return size;
}

SI32 _Map::ReadLevelMode(FILE *fp)
{
	SI32 size = 0;

	SI32 i;

	if(pLevelMode[0])return FALSE;


	for(i = 0;i < MAX_LEVEL_STEP;i++)
	{
		pLevelMode[i] = new unsigned char[MH.MapXsize * MH.MapYsize];
		size += fread(pLevelMode[i],				sizeof(unsigned char),			MH.MapXsize * MH.MapYsize, fp);
	}
	return size;
}


SI32 _Map::WriteTopLevel(FILE* fp)
{
	SI32 size;

	if( pTopLevel == NULL )	return FALSE;

	size = fwrite(pTopLevel, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::ReadTopLevel(FILE *fp)
{
	SI32 size;

	if( pTopLevel )	return FALSE;

	pTopLevel = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pTopLevel == NULL )	return FALSE;

	size = fread(pTopLevel, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}


SI32 _Map::WriteTileAtbMap(FILE* fp)
{
	SI32 size;

	if( pTileAtbMap == NULL )	return FALSE;

	size = fwrite(pTileAtbMap, sizeof(short), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::ReadTileAtbMap(FILE *fp)
{
	SI32 size;

	if( pTileAtbMap )	return FALSE;

	pTileAtbMap = new short[MH.MapXsize * MH.MapYsize];
	if( pTileAtbMap == NULL )	return FALSE;

	size = fread(pTileAtbMap, sizeof(short), MH.MapXsize * MH.MapYsize, fp);
	return size;
}



SI32 _Map::WriteShallowMap(FILE* fp)
{
	SI32 size;

	if( pShallowMap == NULL )	return FALSE;

	size = fwrite(pShallowMap, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::ReadShallowMap(FILE *fp)
{
	SI32 size;

	if( pShallowMap )	return FALSE;

	pShallowMap = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pShallowMap == NULL )	return FALSE;

	size = fread(pShallowMap, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}


SI32 _Map::WriteShallowFont(FILE* fp)
{
	SI32 size;

	if( pShallowFont == NULL )	return FALSE;

	size = fwrite(pShallowFont, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::ReadShallowFont(FILE *fp)
{
	SI32 size;

	if( pShallowFont )	return FALSE;

	pShallowFont = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pShallowFont == NULL )	return FALSE;

	size = fread(pShallowFont, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::WriteSeaMode(FILE* fp)
{
	SI32 size;

	if( pSeaMode == NULL )	return FALSE;
	size = fwrite(pSeaMode, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}
SI32 _Map::ReadSeaMode(FILE*fp)
{
	SI32 size;

	if( pSeaMode )	return FALSE;

	pSeaMode = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pSeaMode == NULL )	return FALSE;

	size = fread(pSeaMode, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::WriteSeaFont(FILE* fp)
{
	SI32 size;

	if( pSeaFont == NULL )	return FALSE;
	size = fwrite(pSeaFont, sizeof(unsigned char), MH.MapXsize/2 * MH.MapYsize/2, fp);
	return size;
}
SI32 _Map::ReadSeaFont(FILE*fp)
{
	SI32 size;

	if( pSeaFont )	return FALSE;

	pSeaFont = new unsigned char[MH.MapXsize/2 * MH.MapYsize/2];
	if( pSeaFont == NULL )	return FALSE;

	size = fread(pSeaFont, sizeof(unsigned char), MH.MapXsize/2 * MH.MapYsize/2, fp);
	return size;
}

SI32 _Map::WriteSeaFile(FILE* fp)
{
	SI32 size;

	if( pSeaFile == NULL )	return FALSE;
	size = fwrite(pSeaFile, sizeof(unsigned char), MH.MapXsize/2 * MH.MapYsize/2, fp);
	return size;
}
SI32 _Map::ReadSeaFile(FILE*fp)
{
	SI32 size;

	if( pSeaFile )	return FALSE;

	pSeaFile = new unsigned char[MH.MapXsize/2 * MH.MapYsize/2];
	if( pSeaFile == NULL )	return FALSE;

	size = fread(pSeaFile, sizeof(unsigned char), MH.MapXsize/2 * MH.MapYsize/2, fp);
	return size;
}

SI32 _Map::WriteNewBlkMode(FILE* fp)
{
	SI32 size;

	if( pNewBlkMode == NULL )	return FALSE;
	size = fwrite(pNewBlkMode, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}
SI32 _Map::ReadNewBlkMode(FILE*fp)
{
	SI32 size;

	if( pNewBlkMode )	return FALSE;

	pNewBlkMode = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pNewBlkMode == NULL )	return FALSE;

	size = fread(pNewBlkMode, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::WriteNewBlkFont(FILE* fp)
{
	SI32 size;

	if( pNewBlkFont == NULL )	return FALSE;
	size = fwrite(pNewBlkFont, sizeof(unsigned char), MH.MapXsize/2 * MH.MapYsize/2, fp);
	return size;
}
SI32 _Map::ReadNewBlkFont(FILE*fp)
{
	SI32 size;

	if( pNewBlkFont )	return FALSE;

	pNewBlkFont = new unsigned char[MH.MapXsize/2 * MH.MapYsize/2];
	if( pNewBlkFont == NULL )	return FALSE;

	size = fread(pNewBlkFont, sizeof(unsigned char), MH.MapXsize/2 * MH.MapYsize/2, fp);
	return size;
}

SI32 _Map::WriteNewBlkFile(FILE* fp)
{
	SI32 size;

	if( pNewBlkFile == NULL )	return FALSE;
	size = fwrite(pNewBlkFile, sizeof(unsigned char), MH.MapXsize/2 * MH.MapYsize/2, fp);
	return size;
}
SI32 _Map::ReadNewBlkFile(FILE*fp)
{
	SI32 size;

	if( pNewBlkFile )	return FALSE;

	pNewBlkFile = new unsigned char[MH.MapXsize/2 * MH.MapYsize/2];
	if( pNewBlkFile == NULL )	return FALSE;

	size = fread(pNewBlkFile, sizeof(unsigned char), MH.MapXsize/2 * MH.MapYsize/2, fp);
	return size;
}

SI32 _Map::WriteNewBlkLevel(FILE* fp)
{
	SI32 size;

	if( pNewBlkLevel == NULL )	return FALSE;
	size = fwrite(pNewBlkLevel, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}
SI32 _Map::ReadNewBlkLevel(FILE*fp)
{
	SI32 size;

	if( pNewBlkLevel )	return FALSE;

	pNewBlkLevel = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pNewBlkLevel == NULL )	return FALSE;

	size = fread(pNewBlkLevel, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::WriteCastleMode(FILE* fp)
{
	SI32 size;

	if( pCastleMode == NULL )	return FALSE;
	size = fwrite(pCastleMode, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}
SI32 _Map::ReadCastleMode(FILE*fp)
{
	SI32 size;

	if( pCastleMode )	return FALSE;

	pCastleMode = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pCastleMode == NULL )	return FALSE;

	size = fread(pCastleMode, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::WriteCastleFont(FILE* fp)
{
	SI32 size;

	if( pCastleFont == NULL )	return FALSE;
	size = fwrite(pCastleFont, sizeof(unsigned char), MH.MapXsize/2 * MH.MapYsize/2, fp);
	return size;
}
SI32 _Map::ReadCastleFont(FILE*fp)
{
	SI32 size;

	if( pCastleFont )	return FALSE;

	pCastleFont = new unsigned char[MH.MapXsize/2 * MH.MapYsize/2];
	if( pCastleFont == NULL )	return FALSE;

	size = fread(pCastleFont, sizeof(unsigned char), MH.MapXsize/2 * MH.MapYsize/2, fp);
	return size;
}

SI32 _Map::WriteCastleFile(FILE* fp)
{
	SI32 size;

	if( pCastleFile == NULL )	return FALSE;
	size = fwrite(pCastleFile, sizeof(unsigned char), MH.MapXsize/2 * MH.MapYsize/2, fp);
	return size;
}
SI32 _Map::ReadCastleFile(FILE*fp)
{
	SI32 size;

	if( pCastleFile )	return FALSE;

	pCastleFile = new unsigned char[MH.MapXsize/2 * MH.MapYsize/2];
	if( pCastleFile == NULL )	return FALSE;

	size = fread(pCastleFile, sizeof(unsigned char), MH.MapXsize/2 * MH.MapYsize/2, fp);
	return size;
}


SI32 _Map::WriteCropTypeMap(FILE* fp)
{
	SI32 size;

	if( pCropTypeMap == NULL )	return FALSE;
	size = fwrite(pCropTypeMap, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}
SI32 _Map::ReadCropTypeMap(FILE*fp)
{
	SI32 size;

	if( pCropTypeMap )	return FALSE;

	pCropTypeMap = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pCropTypeMap == NULL )	return FALSE;

	size = fread(pCropTypeMap, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::WriteCropVaryMap(FILE* fp)
{
	SI32 size;

	if( pCropVaryMap == NULL )	return FALSE;
	size = fwrite(pCropVaryMap, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}
SI32 _Map::ReadCropVaryMap(FILE*fp)
{
	SI32 size;

	if( pCropVaryMap )	return FALSE;

	pCropVaryMap = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pCropVaryMap == NULL )	return FALSE;

	size = fread(pCropVaryMap, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::WriteCropMountMap(FILE* fp)
{
	SI32 size;

	if( pCropMountMap == NULL )	return FALSE;
	size = fwrite(pCropMountMap, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}
SI32 _Map::ReadCropMountMap(FILE*fp)
{
	SI32 size;

	if( pCropMountMap )	return FALSE;

	pCropMountMap = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pCropMountMap == NULL )	return FALSE;

	size = fread(pCropMountMap, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::WriteCropStepMap(FILE* fp)
{
	SI32 size;

	if( pCropStepMap == NULL )	return FALSE;
	size = fwrite(pCropStepMap, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::ReadCropStepMap(FILE*fp)
{
	SI32 size;

	if( pCropStepMap )	return FALSE;

	pCropStepMap = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pCropStepMap == NULL )	return FALSE;

	size = fread(pCropStepMap, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::WriteTreasureMap(FILE* fp)
{
	SI32 size;

	if( pTreasureMap == NULL )	return FALSE;
	size = fwrite(pTreasureMap, sizeof(SHORT), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::ReadTreasureMap(FILE*fp)
{
	SI32 size;

	if( pTreasureMap )	return FALSE;

	pTreasureMap = new SHORT[MH.MapXsize * MH.MapYsize];
	if( pTreasureMap == NULL )	return FALSE;

	size = fread(pTreasureMap, sizeof(SHORT), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::WriteFireMap(FILE* fp)
{
	SI32 size;

	if( pFireMap == NULL )	return FALSE;
	size = fwrite(pFireMap, sizeof(UI08), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::ReadFireMap(FILE*fp)
{
	SI32 size;

	pFireCharMap = new _NewID[MH.MapXsize * MH.MapYsize];
	if(pFireCharMap) ZeroMemory(pFireCharMap, sizeof(_NewID)*MH.MapXsize * MH.MapYsize);
	pFirePlayerMap = new SI16[MH.MapXsize * MH.MapYsize];
	if(pFirePlayerMap) ZeroMemory(pFirePlayerMap, sizeof(SI16)*MH.MapXsize * MH.MapYsize);

	if( pFireMap )	return FALSE;

	pFireMap = new UI08[MH.MapXsize * MH.MapYsize];
	if( pFireMap == NULL )	return FALSE;

	size = fread(pFireMap, sizeof(UI08), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

#else

//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>
#include <main.h>
#include <map.h>
#include "bindjxfile.h"

extern	SHORT 		MapEditMode;

// 화일이름을 갖고 지도의 정보를 얻어온다. 
BOOL _Map::GetMapInfo(char* filename, _MapHeader* header)
{
	FILE* fp;
	int numread;

	fp=fopen(filename, "rb");
	if(fp==NULL)
	{
		return FALSE;
	}

	numread = fread(header, sizeof(_MapHeader), 1, fp);

	fclose(fp);

	return TRUE;

}


// 지도를 저장한다. 
BOOL _Map::Save(char* filename)
{

	SI32 size = 0;
	FILE* fp;

	DWORD i ;

	// 버전도 기록한다. 
	MH.Version = 3;

/*	// 체크섬을 알아낸다. 
	for(i=0;i<sizeof(_Map);i++)
	{

	}
*/
	// 게임에 참여하는 플레이어 수도 기록한다.
	MH.PlayerNumber = 0;
	for(i=0;i<MAX_MULTI_PLAYER;i++)
	{
		if(clMapPlayerInfo[i].StartPoint.x>=0 && clMapPlayerInfo[i].StartPoint.y>=0)
		{
			MH.PlayerNumber++;
		}
	}

	fp=fopen(filename, "wb");

	if(fp==NULL)
	{
		return FALSE;
	}

	
	size += fwrite(&MH,					sizeof(_MapHeader),			1,								fp);
	size += fwrite(clMapPlayerInfo,		sizeof(cltMapPlayerInfo),	MAX_MULTI_PLAYER,				fp);
	size += fwrite(clMapUnitInfo,		sizeof(cltMapUnitInfo),		MAX_MAP_CHAR_NUMBER,			fp);
	size += fwrite(ShipPatrolPoint,		sizeof(POINT),				MAX_SHIP_PATROL_POINT_NUMBER,	fp);
	size += fwrite(ShipLandingPoint,	sizeof(POINT),				MAX_SHIP_LANDING_POINT_NUMBER,	fp);
	size += fwrite(ExpansionBasePoint,	sizeof(POINT),				MAX_MULTI_PLAYER,				fp);
	size += fwrite(&ExpansionBaseNumber,sizeof(SHORT),				1,								fp);
	size += fwrite(&VirtualMapX,		sizeof(int),				1,								fp);
	size += fwrite(&VirtualMapY,		sizeof(int),				1,								fp);
	size += fwrite(YOff,				sizeof(SI32),				180/*MAX_MAP_YSIZE*/,					fp);
	size += fwrite(YOff2,				sizeof(SI32),				180/*MAX_MAP_YSIZE*/,					fp);
	size += fwrite(Reserved,			sizeof(unsigned char),		1024,							fp);

	size += fwrite(&DrawForceSwitch,	sizeof(char), 1, fp);
	size += fwrite(&MapDrawSwitch,		sizeof(char), 1, fp);

	

	size += WriteIDMap(fp);
	size += WriteSkyMap(fp);
	size += WriteCollMap(fp);
	size += WriteLevelInfo(fp);
	size += WriteLevelFile(fp);
	size += WriteLevelFont(fp);
	size += WriteLevelType(fp);
	size += WriteLevelMode(fp);
	size += WriteTopLevel(fp);
	size += WriteTileAtbMap(fp);
	size += WriteShallowMap(fp);
	size += WriteShallowFont(fp);
	size += WriteSeaMode(fp);
	size += WriteSeaFont(fp);
	size += WriteSeaFile(fp);

	size += WriteNewBlkMode(fp);
	size += WriteNewBlkFont(fp);
	size += WriteNewBlkFile(fp);
	size += WriteNewBlkLevel(fp);
	size += WriteCastleMode(fp);
	size += WriteCastleFont(fp);
	size += WriteCastleFile(fp);

	size += WriteCropTypeMap(fp);
	size += WriteCropVaryMap(fp);
	size += WriteCropMountMap(fp);
	size += WriteCropStepMap(fp);
	size += WriteCropTreeMap(fp);

	size += WriteTreasureMap(fp);
	size += WriteMineMap(fp);
	size += WriteFireMap(fp);

	fclose(fp);

	

	return TRUE;
}                     



// 지도를 불러온다. 
BOOL _Map::Load(char* filename)
{
	SI32 size = 0;
	FILE* fp;

	SI32 i, j ;

	if(MapEditMode==TRUE)
		fp = fopen(filename, "rb");
	else
		fp = BindJXFile::GetInstance()->GetFile( filename );	

	if(fp==NULL)
	{
		return FALSE;
	}

	
	size += fread(&MH,					sizeof(_MapHeader),			1,								fp);
	size += fread(clMapPlayerInfo,		sizeof(cltMapPlayerInfo),	MAX_MULTI_PLAYER,				fp);
	size += fread(clMapUnitInfo,		sizeof(cltMapUnitInfo),		MAX_MAP_CHAR_NUMBER,			fp);
	size += fread(ShipPatrolPoint,		sizeof(POINT),				MAX_SHIP_PATROL_POINT_NUMBER,	fp);
	size += fread(ShipLandingPoint,		sizeof(POINT),				MAX_SHIP_LANDING_POINT_NUMBER,	fp);
	size += fread(ExpansionBasePoint,	sizeof(POINT),				MAX_MULTI_PLAYER,				fp);
	size += fread(&ExpansionBaseNumber,	sizeof(SHORT),				1,								fp);
	size += fread(&VirtualMapX,			sizeof(int),				1,								fp);
	size += fread(&VirtualMapY,			sizeof(int),				1,								fp);
	size += fread(YOff,					sizeof(SI32),				180/*MAX_MAP_YSIZE*/,			fp);
	size += fread(YOff2,				sizeof(SI32),				180/*MAX_MAP_YSIZE*/,			fp);
	size += fread(Reserved,				sizeof(unsigned char),		1024,							fp);

	size += fread(&DrawForceSwitch,		sizeof(char), 1, fp);
	size += fread(&MapDrawSwitch,		sizeof(char), 1, fp);


	size += ReadIDMap(fp);
	size += ReadSkyMap(fp);
	size += ReadCollMap(fp);
	size += ReadLevelInfo(fp);
	size += ReadLevelFile(fp);
	size += ReadLevelFont(fp);
	size += ReadLevelType(fp);
	size += ReadLevelMode(fp);
	size += ReadTopLevel(fp);
	size += ReadTileAtbMap(fp);
	size += ReadShallowMap(fp);
	size += ReadShallowFont(fp);
	size += ReadSeaMode(fp);
	size += ReadSeaFont(fp);
	size += ReadSeaFile(fp);
	

	size += ReadNewBlkMode(fp);
	size += ReadNewBlkFont(fp);
	size += ReadNewBlkFile(fp);
	size += ReadNewBlkLevel(fp);
	size += ReadCastleMode(fp);
	size += ReadCastleFont(fp);
	size += ReadCastleFile(fp);

	size += ReadCropTypeMap(fp);
	size += ReadCropVaryMap(fp);
	size += ReadCropMountMap(fp);
	size += ReadCropStepMap(fp);
	size += ReadCropTreeMap(fp);

	size += ReadTreasureMap(fp);
	size += ReadMineMap(fp);
	size += ReadFireMap(fp);

	fclose(fp);


	// 오프셋을 구한다. 
	for(i=0;i<MH.MapYsize;i++)
	{
		YOff[i] = i* MH.MapXsize;
	}
	for(i=0;i < (MH.MapYsize/2);i++)
	{
		YOff2[i]	= i * (MH.MapXsize/2);
	}


	
	for(i=0;i<MH.MapYsize;i++)
		for(j=0;j<MH.MapXsize;j++)
		{
			SetIDMap(0, j, i);
		}
		
	for(i=0;i<MH.MapYsize;i++)
		for(j=0;j<MH.MapXsize;j++)
		{
			pCollMap[j+YOff[i]] = 0;
		}

	if(!pBarrierMap) pBarrierMap = new short[MH.MapXsize * MH.MapYsize];
	if(pBarrierMap)	ZeroMemory(pBarrierMap, sizeof(short)*MH.MapXsize*MH.MapYsize);
	
	return TRUE;

}


SI32 _Map::WriteIDMap(FILE* fp)
{
	SI32 size;

	if( pIDMap == NULL )	return FALSE;
	size = fwrite(pIDMap, sizeof(short), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::ReadIDMap(FILE*fp)
{
	SI32 size;

	if( pIDMap )	return FALSE;

	pIDMap = new short[MH.MapXsize * MH.MapYsize];
	if( pIDMap == NULL )	return FALSE;

	size = fread(pIDMap, sizeof(short), MH.MapXsize * MH.MapYsize, fp);
	return size;
}


SI32 _Map::WriteSkyMap(FILE* fp)
{
	SI32 size;

	if(pSkyMap==NULL)	return FALSE;

	size = fwrite(pSkyMap,	sizeof(short),            MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::ReadSkyMap(FILE*fp)
{
	SI32 size;

	if(pSkyMap)	return FALSE;

	pSkyMap = new short[MH.MapXsize*MH.MapYsize];
	if(pSkyMap==NULL)	return FALSE;

	size = fread(pSkyMap,	sizeof(short),            MH.MapXsize * MH.MapYsize, fp);
	return size;
}


SI32 _Map::WriteCollMap(FILE* fp)
{
	SI32 size;

	if( pCollMap == NULL )	return FALSE;

	size = fwrite(pCollMap, sizeof(short),	MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::ReadCollMap(FILE*fp)
{
	SI32 size;

	if( pCollMap )	return FALSE;

	pCollMap = new short[MH.MapXsize * MH.MapYsize];
	if( pCollMap == NULL )	return FALSE;

	size = fread(pCollMap, sizeof(short), MH.MapXsize * MH.MapYsize, fp);
	return size;
}


SI32 _Map::WriteLevelInfo(FILE* fp)
{
	SI32 size;

	if( pLevelInfo == NULL )	return FALSE;

	size = fwrite(pLevelInfo, sizeof(char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::ReadLevelInfo(FILE*fp)
{
	SI32 size;

	if( pLevelInfo )	return FALSE;

	pLevelInfo = new char[MH.MapXsize * MH.MapYsize];
	if( pLevelInfo == NULL )	return FALSE;

	size = fread(pLevelInfo, sizeof(char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::WriteMineMap(FILE* fp)
{
	SI32 size;

	if( pMineMap == NULL )	return FALSE;

	size = fwrite(pMineMap, sizeof(char), MH.MapXsize * MH.MapYsize, fp);
	
	return size;
}

SI32 _Map::ReadMineMap(FILE* fp)
{
	SI32 size;

	if( pMineMap )	return FALSE;

	pMineMap = new char[MH.MapXsize * MH.MapYsize];
	if( pMineMap == NULL )	return FALSE;

//	if( pMineCharMap )	return FALSE;

//	pMineCharMap = new SI32[MH.MapXsize * MH.MapYsize];
//	if( pMineCharMap == NULL )	return FALSE;


	size = fread(pMineMap, sizeof(char), MH.MapXsize * MH.MapYsize, fp);
	
	return size;
}



SI32 _Map::WriteLevelFile(FILE* fp)
{
	SI32 size;

	if( pLevelFile == NULL )	return FALSE;

	size = fwrite(pLevelFile, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::ReadLevelFile(FILE*fp)
{
	SI32 size;

	if( pLevelFile )	return FALSE;

	pLevelFile = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pLevelFile == NULL )	return FALSE;

	size = fread(pLevelFile, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}



SI32 _Map::WriteLevelFont(FILE* fp)
{
	SI32 size;

	if( pLevelFont == NULL )	return FALSE;

	size = fwrite(pLevelFont, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::ReadLevelFont(FILE *fp)
{
	SI32 size;

	if( pLevelFont )	return FALSE;

	pLevelFont = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pLevelFont == NULL )	return FALSE;

	size = fread(pLevelFont, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}


SI32 _Map::WriteLevelType(FILE* fp)
{
	SI32 size;

	if( pLevelType == NULL )	return FALSE;

	size = fwrite(pLevelType, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::ReadLevelType(FILE *fp)
{
	SI32 size;

	if( pLevelType )	return FALSE;

	pLevelType = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pLevelType == NULL )	return FALSE;

	size = fread(pLevelType, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}


SI32 _Map::WriteLevelMode(FILE* fp)
{
	SI32 size = 0;

	SI32 i;

	if(pLevelMode[0] == NULL)return FALSE;

	for(i = 0;i < MAX_LEVEL_STEP;i++)
	{
		size += fwrite(pLevelMode[i],			sizeof(unsigned char),	MH.MapXsize * MH.MapYsize, fp);
	}

	return size;
}

SI32 _Map::ReadLevelMode(FILE *fp)
{
	SI32 size = 0;

	SI32 i;

	if(pLevelMode[0])return FALSE;


	for(i = 0;i < MAX_LEVEL_STEP;i++)
	{
		pLevelMode[i] = new unsigned char[MH.MapXsize * MH.MapYsize];
		size += fread(pLevelMode[i],				sizeof(unsigned char),			MH.MapXsize * MH.MapYsize, fp);
	}
	return size;
}


SI32 _Map::WriteTopLevel(FILE* fp)
{
	SI32 size;

	if( pTopLevel == NULL )	return FALSE;

	size = fwrite(pTopLevel, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::ReadTopLevel(FILE *fp)
{
	SI32 size;

	if( pTopLevel )	return FALSE;

	pTopLevel = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pTopLevel == NULL )	return FALSE;

	size = fread(pTopLevel, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}


SI32 _Map::WriteTileAtbMap(FILE* fp)
{
	SI32 size;

	if( pTileAtbMap == NULL )	return FALSE;

	size = fwrite(pTileAtbMap, sizeof(short), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::ReadTileAtbMap(FILE *fp)
{
	SI32 size;

	if( pTileAtbMap )	return FALSE;

	pTileAtbMap = new short[MH.MapXsize * MH.MapYsize];
	if( pTileAtbMap == NULL )	return FALSE;

	size = fread(pTileAtbMap, sizeof(short), MH.MapXsize * MH.MapYsize, fp);
	return size;
}



SI32 _Map::WriteShallowMap(FILE* fp)
{
	SI32 size;

	if( pShallowMap == NULL )	return FALSE;

	size = fwrite(pShallowMap, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::ReadShallowMap(FILE *fp)
{
	SI32 size;

	if( pShallowMap )	return FALSE;

	pShallowMap = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pShallowMap == NULL )	return FALSE;

	size = fread(pShallowMap, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}


SI32 _Map::WriteShallowFont(FILE* fp)
{
	SI32 size;

	if( pShallowFont == NULL )	return FALSE;

	size = fwrite(pShallowFont, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::ReadShallowFont(FILE *fp)
{
	SI32 size;

	if( pShallowFont )	return FALSE;

	pShallowFont = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pShallowFont == NULL )	return FALSE;

	size = fread(pShallowFont, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::WriteSeaMode(FILE* fp)
{
	SI32 size;

	if( pSeaMode == NULL )	return FALSE;
	size = fwrite(pSeaMode, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}
SI32 _Map::ReadSeaMode(FILE*fp)
{
	SI32 size;

	if( pSeaMode )	return FALSE;

	pSeaMode = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pSeaMode == NULL )	return FALSE;

	size = fread(pSeaMode, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::WriteSeaFont(FILE* fp)
{
	SI32 size;

	if( pSeaFont == NULL )	return FALSE;
	//size = fwrite(pSeaFont, sizeof(unsigned char), MH.MapXsize/2 * MH.MapYsize/2, fp);
	size = fwrite(pSeaFont, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);

	return size;
}
SI32 _Map::ReadSeaFont(FILE*fp)
{
	SI32 size;

	if( pSeaFont )	return FALSE;

	//pSeaFont = new unsigned char[MH.MapXsize/2 * MH.MapYsize/2];
	pSeaFont = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pSeaFont == NULL )	return FALSE;

	//size = fread(pSeaFont, sizeof(unsigned char), MH.MapXsize/2 * MH.MapYsize/2, fp);
	size = fread(pSeaFont, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::WriteSeaFile(FILE* fp)
{
	SI32 size;

	if( pSeaFile == NULL )	return FALSE;
	//size = fwrite(pSeaFile, sizeof(unsigned char), MH.MapXsize/2 * MH.MapYsize/2, fp);
	size = fwrite(pSeaFile, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}
SI32 _Map::ReadSeaFile(FILE*fp)
{
	SI32 size;

	if( pSeaFile )	return FALSE;

	//pSeaFile = new unsigned char[MH.MapXsize/2 * MH.MapYsize/2];
	pSeaFile = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pSeaFile == NULL )	return FALSE;

	//size = fread(pSeaFile, sizeof(unsigned char), MH.MapXsize/2 * MH.MapYsize/2, fp);
	size = fread(pSeaFile, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::WriteNewBlkMode(FILE* fp)
{
	SI32 size;

	if( pNewBlkMode == NULL )	return FALSE;
	size = fwrite(pNewBlkMode, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}
SI32 _Map::ReadNewBlkMode(FILE*fp)
{
	SI32 size;

	if( pNewBlkMode )	return FALSE;

	pNewBlkMode = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pNewBlkMode == NULL )	return FALSE;

	size = fread(pNewBlkMode, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::WriteNewBlkFont(FILE* fp)
{
	SI32 size;

	if( pNewBlkFont == NULL )	return FALSE;
	size = fwrite(pNewBlkFont, sizeof(unsigned char), MH.MapXsize/2 * MH.MapYsize/2, fp);
	return size;
}
SI32 _Map::ReadNewBlkFont(FILE*fp)
{
	SI32 size;

	if( pNewBlkFont )	return FALSE;

	pNewBlkFont = new unsigned char[MH.MapXsize/2 * MH.MapYsize/2];
	if( pNewBlkFont == NULL )	return FALSE;

	size = fread(pNewBlkFont, sizeof(unsigned char), MH.MapXsize/2 * MH.MapYsize/2, fp);
	return size;
}

SI32 _Map::WriteNewBlkFile(FILE* fp)
{
	SI32 size;

	if( pNewBlkFile == NULL )	return FALSE;
	size = fwrite(pNewBlkFile, sizeof(unsigned char), MH.MapXsize/2 * MH.MapYsize/2, fp);
	return size;
}
SI32 _Map::ReadNewBlkFile(FILE*fp)
{
	SI32 size;

	if( pNewBlkFile )	return FALSE;

	pNewBlkFile = new unsigned char[MH.MapXsize/2 * MH.MapYsize/2];
	if( pNewBlkFile == NULL )	return FALSE;

	size = fread(pNewBlkFile, sizeof(unsigned char), MH.MapXsize/2 * MH.MapYsize/2, fp);
	return size;
}

SI32 _Map::WriteNewBlkLevel(FILE* fp)
{
	SI32 size;

	if( pNewBlkLevel == NULL )	return FALSE;
	size = fwrite(pNewBlkLevel, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}
SI32 _Map::ReadNewBlkLevel(FILE*fp)
{
	SI32 size;

	if( pNewBlkLevel )	return FALSE;

	pNewBlkLevel = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pNewBlkLevel == NULL )	return FALSE;

	size = fread(pNewBlkLevel, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::WriteCastleMode(FILE* fp)
{
	SI32 size;

	if( pCastleMode == NULL )	return FALSE;
	size = fwrite(pCastleMode, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}
SI32 _Map::ReadCastleMode(FILE*fp)
{
	SI32 size;

	if( pCastleMode )	return FALSE;

	pCastleMode = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pCastleMode == NULL )	return FALSE;

	size = fread(pCastleMode, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::WriteCastleFont(FILE* fp)
{
	SI32 size;

	if( pCastleFont == NULL )	return FALSE;
	size = fwrite(pCastleFont, sizeof(unsigned char), MH.MapXsize/2 * MH.MapYsize/2, fp);
	return size;
}
SI32 _Map::ReadCastleFont(FILE*fp)
{
	SI32 size;

	if( pCastleFont )	return FALSE;

	pCastleFont = new unsigned char[MH.MapXsize/2 * MH.MapYsize/2];
	if( pCastleFont == NULL )	return FALSE;

	size = fread(pCastleFont, sizeof(unsigned char), MH.MapXsize/2 * MH.MapYsize/2, fp);
	return size;
}

SI32 _Map::WriteCastleFile(FILE* fp)
{
	SI32 size;

	if( pCastleFile == NULL )	return FALSE;
	size = fwrite(pCastleFile, sizeof(unsigned char), MH.MapXsize/2 * MH.MapYsize/2, fp);
	return size;
}
SI32 _Map::ReadCastleFile(FILE*fp)
{
	SI32 size;

	if( pCastleFile )	return FALSE;

	pCastleFile = new unsigned char[MH.MapXsize/2 * MH.MapYsize/2];
	if( pCastleFile == NULL )	return FALSE;

	size = fread(pCastleFile, sizeof(unsigned char), MH.MapXsize/2 * MH.MapYsize/2, fp);
	return size;
}


SI32 _Map::WriteCropTypeMap(FILE* fp)
{
	SI32 size;

	if( pCropTypeMap == NULL )	return FALSE;
	size = fwrite(pCropTypeMap, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}
SI32 _Map::ReadCropTypeMap(FILE*fp)
{
	SI32 size;

	if( pCropTypeMap )	return FALSE;

	pCropTypeMap = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pCropTypeMap == NULL )	return FALSE;

	size = fread(pCropTypeMap, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::WriteCropTreeMap(FILE* fp)
{
	SI32 size;
	
	if( pCropTreeMap == NULL )	return FALSE;
	size = fwrite(pCropTreeMap, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}
SI32 _Map::ReadCropTreeMap(FILE*fp)
{
	SI32 size;
	
	if( pCropTreeMap )	return FALSE;
	
	pCropTreeMap = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pCropTreeMap == NULL )	return FALSE;
	
	size = fread(pCropTreeMap, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::WriteCropVaryMap(FILE* fp)
{
	SI32 size;

	if( pCropVaryMap == NULL )	return FALSE;
	size = fwrite(pCropVaryMap, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}
SI32 _Map::ReadCropVaryMap(FILE*fp)
{
	SI32 size;

	if( pCropVaryMap )	return FALSE;

	pCropVaryMap = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pCropVaryMap == NULL )	return FALSE;

	size = fread(pCropVaryMap, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::WriteCropMountMap(FILE* fp)
{
	SI32 size;

	if( pCropMountMap == NULL )	return FALSE;
	size = fwrite(pCropMountMap, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}
SI32 _Map::ReadCropMountMap(FILE*fp)
{
	SI32 size;

	if( pCropMountMap )	return FALSE;

	pCropMountMap = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pCropMountMap == NULL )	return FALSE;

	size = fread(pCropMountMap, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::WriteCropStepMap(FILE* fp)
{
	SI32 size;

	if( pCropStepMap == NULL )	return FALSE;
	size = fwrite(pCropStepMap, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::ReadCropStepMap(FILE*fp)
{
	SI32 size;

	if( pCropStepMap )	return FALSE;

	pCropStepMap = new unsigned char[MH.MapXsize * MH.MapYsize];
	if( pCropStepMap == NULL )	return FALSE;

	size = fread(pCropStepMap, sizeof(unsigned char), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::WriteTreasureMap(FILE* fp)
{
	SI32 size;

	if( pTreasureMap == NULL )	return FALSE;
	size = fwrite(pTreasureMap, sizeof(SHORT), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::ReadTreasureMap(FILE*fp)
{
	SI32 size;

	if( pTreasureMap )	return FALSE;

	pTreasureMap = new SHORT[MH.MapXsize * MH.MapYsize];
	if( pTreasureMap == NULL )	return FALSE;

	size = fread(pTreasureMap, sizeof(SHORT), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::WriteFireMap(FILE* fp)
{
	SI32 size;

	if( pFireMap == NULL )	return FALSE;
	size = fwrite(pFireMap, sizeof(UI08), MH.MapXsize * MH.MapYsize, fp);
	return size;
}

SI32 _Map::ReadFireMap(FILE*fp)
{
	SI32 size;

	pFireCharMap = new _NewID[MH.MapXsize * MH.MapYsize];
	if(pFireCharMap) ZeroMemory(pFireCharMap, sizeof(_NewID)*MH.MapXsize * MH.MapYsize);
	pFirePlayerMap = new SI16[MH.MapXsize * MH.MapYsize];
	if(pFirePlayerMap) ZeroMemory(pFirePlayerMap, sizeof(SI16)*MH.MapXsize * MH.MapYsize);

	if( pFireMap )	return FALSE;

	pFireMap = new UI08[MH.MapXsize * MH.MapYsize];
	if( pFireMap == NULL )	return FALSE;

	size = fread(pFireMap, sizeof(UI08), MH.MapXsize * MH.MapYsize, fp);
	return size;
}
#endif