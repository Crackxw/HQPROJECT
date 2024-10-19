#ifndef __ONLINEMAPINDEX_PARSER_H
#define __ONLINEMAPINDEX_PARSER_H

#include "OnlineVillage-Parser.h"

#include <iostream>
#include <map>
#include <vector>

using namespace std;


#define MAPINDEXPARSERMAPTYPE_FIELD			1					// �Ϲ� ��
#define MAPINDEXPARSERMAPTYPE_DUNGEON		2					// ����
#define MAPINDEXPARSERMAPTYPE_FOREST		3					// ��
#define MAPINDEXPARSERMAPTYPE_WATER			4					// �ٴ�
#define MAPINDEXPARSERMAPTYPE_VILLAGE		5					// ����

#define MAPINDEXPARSERWEATHER_RAIN			0x00000001			// ��
#define MAPINDEXPARSERWEATHER_SNOW			0x00000002			// ��
#define MAPINDEXPARSERWEATHER_TIME			0x00000004			// �㳷 ȿ��
#define MAPINDEXPARSERWEATHER_CLOUD			0x00000008			// ����


struct	MapIndexHeader
{
	SI16	siMapIndex;											// Map Index
	CHAR	szMapFileName[256];									// Map File Name
	CHAR	szMiniMapFileName[256];								// Mini Map File Name

	UI32	siWeather;											// ����(Bit ������ ���ȴ�.)
	SI16	siMapType;											// Map Type
	BOOL	fFog;												// Fog Flag

	UI16	uiVillageCode;										// ���� �ڵ�
	SI32	fWarFlag;											// ���� Flag
	SI32	siNameCode;											// Name Code

	CHAR	szBGMFileName[256];									// BGM File Name

	BOOL	fMixingVillageMap;									// ������ ���� Map Flag
	SI16	MixingVillageMapNationCode;							// ������ ���� Map ���� Code

	SI32	siWarFieldMap;										// ���� �ʵ��
	SI16	siPortalCode;
};


class OnlineMapIndexParser
{
public:
	MapIndexHeader			*pMapIndexHeader;					// ���� ��� ������ ���� ����ü

	typedef map< SI16, MapIndexHeader*, less< SI16 > > Map_Index;
	Map_Index				_MapIndex;

	vector<MapIndexHeader>	_Map;
	vector<MapIndexHeader>	_Village;	


	SI16					siMapIndexNum;						// �� ��
	
	MapIndexHeader			*pVillageMapIndexHeader;			// Village ���� ��� ������ ���� ����ü	
	
public:
	OnlineMapIndexParser()		{};
	~OnlineMapIndexParser()		{ Free(); }

	BOOL					ReadText(char *pFileName, OnlineVillageParser* lpOnlineVillageParser);			// �� Index �ε�
	VOID					Free();																			// �����

	SI32					GetMapDataCount(void);															// Get Map Data Count
	SI32					GetVillageMapDataCount(void);													// Get Village Map Data Count
	MapIndexHeader*			GetMapData(SI16 MapIndex);														// Get Map Data
	MapIndexHeader*			GetVillageMapData(SI16 MapIndex);												// Get Village Map Data
	MapIndexHeader*			GetMapIndexData(SI16 MapIndex);													// Get Map Index Data

private:
	void					Add( vector<MapIndexHeader> *v );
};


#endif
