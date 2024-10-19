#ifndef __ONLINEMAPINDEX_PARSER_H
#define __ONLINEMAPINDEX_PARSER_H

#include "OnlineVillage-Parser.h"

#include <iostream>
#include <map>
#include <vector>

using namespace std;


#define MAPINDEXPARSERMAPTYPE_FIELD			1					// 일반 맵
#define MAPINDEXPARSERMAPTYPE_DUNGEON		2					// 동굴
#define MAPINDEXPARSERMAPTYPE_FOREST		3					// 숲
#define MAPINDEXPARSERMAPTYPE_WATER			4					// 바다
#define MAPINDEXPARSERMAPTYPE_VILLAGE		5					// 마을

#define MAPINDEXPARSERWEATHER_RAIN			0x00000001			// 비
#define MAPINDEXPARSERWEATHER_SNOW			0x00000002			// 눈
#define MAPINDEXPARSERWEATHER_TIME			0x00000004			// 밤낮 효과
#define MAPINDEXPARSERWEATHER_CLOUD			0x00000008			// 구름


struct	MapIndexHeader
{
	SI16	siMapIndex;											// Map Index
	CHAR	szMapFileName[256];									// Map File Name
	CHAR	szMiniMapFileName[256];								// Mini Map File Name

	UI32	siWeather;											// 날씨(Bit 단위로 사용된다.)
	SI16	siMapType;											// Map Type
	BOOL	fFog;												// Fog Flag

	UI16	uiVillageCode;										// 마을 코드
	SI32	fWarFlag;											// 전투 Flag
	SI32	siNameCode;											// Name Code

	CHAR	szBGMFileName[256];									// BGM File Name

	BOOL	fMixingVillageMap;									// 조합형 마을 Map Flag
	SI16	MixingVillageMapNationCode;							// 조합형 마을 Map 국가 Code

	SI32	siWarFieldMap;										// 전투 필드맵
	SI16	siPortalCode;
};


class OnlineMapIndexParser
{
public:
	MapIndexHeader			*pMapIndexHeader;					// 맵의 모든 정보를 가진 구조체

	typedef map< SI16, MapIndexHeader*, less< SI16 > > Map_Index;
	Map_Index				_MapIndex;

	vector<MapIndexHeader>	_Map;
	vector<MapIndexHeader>	_Village;	


	SI16					siMapIndexNum;						// 총 맵
	
	MapIndexHeader			*pVillageMapIndexHeader;			// Village 맵의 모든 정보를 가진 구조체	
	
public:
	OnlineMapIndexParser()		{};
	~OnlineMapIndexParser()		{ Free(); }

	BOOL					ReadText(char *pFileName, OnlineVillageParser* lpOnlineVillageParser);			// 맵 Index 로드
	VOID					Free();																			// 지운다

	SI32					GetMapDataCount(void);															// Get Map Data Count
	SI32					GetVillageMapDataCount(void);													// Get Village Map Data Count
	MapIndexHeader*			GetMapData(SI16 MapIndex);														// Get Map Data
	MapIndexHeader*			GetVillageMapData(SI16 MapIndex);												// Get Village Map Data
	MapIndexHeader*			GetMapIndexData(SI16 MapIndex);													// Get Map Index Data

private:
	void					Add( vector<MapIndexHeader> *v );
};


#endif
