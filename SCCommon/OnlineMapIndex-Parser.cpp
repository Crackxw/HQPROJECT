// 담당자 : 이 상 원

#include <GSL.h>

#include	"Directives.h"
#include	"OnlineMapIndex-Parser.h"
#include	"GSCDefine.h"
#include	"BindJxFile.h"
#include	"utility.h"


BOOL OnlineMapIndexParser::ReadText(char *pFileName, OnlineVillageParser* lpOnlineVillageParser)
{
	FILE	*fp		= NULL;
	char	*pToken = NULL;
	char	szBuffer[1024];	
	SI16	i;
	SI16	TempCount;

	fp = BindJXFile::GetInstance()->GetFile( pFileName );
	if(fp  == NULL) return FALSE;

	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);

	
	MapIndexHeader	map;
	

	while( fgets(szBuffer, 1024, fp) )
	{
		ZeroMemory(&map, sizeof(MapIndexHeader));

		// Map Index
		pToken = strtok(szBuffer, "\n\t\r");
		map.siMapIndex = atoi(pToken);

		// Map File Name
		pToken = strtok(NULL, "\n\t\r");
		sprintf((char*)map.szMapFileName, "Online\\Map\\%s", pToken);

		// MiniMap File Name
		pToken = strtok(NULL, "\n\t\r");
		sprintf((char*)map.szMiniMapFileName, "Online\\Map\\%s", pToken);

		// Map Type
		pToken = strtok(NULL, "\n\t\r");
		map.siMapType = 0;
		if(strcmp(pToken, "FIELD") == 0)
		{
			map.siMapType = MAPINDEXPARSERMAPTYPE_FIELD;
		}
		else if(strcmp(pToken, "DUNGEON") == 0)
		{
			map.siMapType = MAPINDEXPARSERMAPTYPE_DUNGEON;
		}
		else if(strcmp(pToken, "FOREST") == 0)
		{
			map.siMapType = MAPINDEXPARSERMAPTYPE_FOREST;
		}
		else if(strcmp(pToken, "WATER") == 0)
		{
			map.siMapType = MAPINDEXPARSERMAPTYPE_WATER;
		}

		// Fog Flag
		pToken = strtok(NULL, "\n\t\r");
		map.fFog = FALSE;
		if(strcmp(pToken, "TRUE") == 0)
		{
			map.fFog = TRUE;
		}
		else if(strcmp(pToken, "FALSE") == 0)
		{
			map.fFog = FALSE;
		}

		// Weather
		pToken    = strtok(NULL, "\n\t\r");
		map.siWeather = 0;
		if(strcmp(pToken, "NONE") != 0)
		{
			TempCount = 0;
			while(pToken[TempCount])
			{
				switch(pToken[TempCount])
				{
					case 'R':			// 비
						map.siWeather |= MAPINDEXPARSERWEATHER_RAIN;
						break;

					case 'S':			// 눈
						map.siWeather |= MAPINDEXPARSERWEATHER_SNOW;
						break;

					case 'T':			// 밤낮
						map.siWeather |= MAPINDEXPARSERWEATHER_TIME;
						break;

					case 'C':			// 구름
						map.siWeather |= MAPINDEXPARSERWEATHER_CLOUD;
						break;
				}
				TempCount++;
			}
		}
		pToken    = strtok(NULL, "\n\t\r");

		// 마을 코드
		pToken    = strtok(NULL, "\n\t\r");
		map.uiVillageCode = atoi(pToken);

		// 전투 Flag
		pToken    = strtok(NULL, "\n\t\r");
		map.fWarFlag = atoi(pToken);

		// Name Code
		pToken    = strtok(NULL, "\n\t\r");
		map.siNameCode = atoi(pToken);

		// BGM File Name
		pToken    = strtok(NULL, "\n\t\r");
		strcpy(map.szBGMFileName, pToken);

		// Battle Map
		pToken    = strtok(NULL, "\n\t\r");
		map.siWarFieldMap = atoi(pToken);

		// Portal Code
		pToken    = strtok(NULL, "\n\t\r");
		map.siPortalCode = atoi(pToken);

		_Map.push_back( map );
	}

	fclose(fp);
	
	if(lpOnlineVillageParser)
	{
		VillageHeader*			pVillageHeader;		
		
		for(i = 0; i < lpOnlineVillageParser->GetTotalVillageNum(); i++)
		{
			ZeroMemory(&map, sizeof(MapIndexHeader));

			pVillageHeader = lpOnlineVillageParser->GetVillageHeaderIndex(i);

			map.siMapIndex = 10000 + pVillageHeader->uiVillageCode;
			map.siWeather  = MAPINDEXPARSERWEATHER_TIME;
			map.siMapType  = MAPINDEXPARSERMAPTYPE_VILLAGE;
			map.fFog       = FALSE;

			map.uiVillageCode = pVillageHeader->uiVillageCode;
			map.fWarFlag      = 1;
			map.siNameCode    = pVillageHeader->siVillageName;

			switch(pVillageHeader->uiNation)
			{
				case ON_COUNTRY_JAPAN:
					strcpy((char*)map.szBGMFileName, "japanvillage.YAV");
					break;

				case ON_COUNTRY_CHINA:
					strcpy((char*)map.szBGMFileName, "japanvillage.YAV");
					break;

				case ON_COUNTRY_TAIWAN:
					strcpy((char*)map.szBGMFileName, "taiwanvillage.YAV");
					break;

				case ON_COUNTRY_KOREA:
				default:
					strcpy((char*)map.szBGMFileName, "koreavillage.YAV");
					break;
			}

			map.fMixingVillageMap          = TRUE;
			map.MixingVillageMapNationCode = pVillageHeader->uiNation;
			map.siWarFieldMap              = 0;

			_Village.push_back( map );
		}
	}

	Add( &_Map );
	Add( &_Village );
	return TRUE;
}

VOID OnlineMapIndexParser::Free()
{
	_Map.clear();
	_Village.clear();
	_MapIndex.clear();
}

SI32 OnlineMapIndexParser::GetMapDataCount(void)
{
	return _Map.size();
}

SI32 OnlineMapIndexParser::GetVillageMapDataCount(void)
{
	return _Village.size();
}

MapIndexHeader*  OnlineMapIndexParser::GetMapData(SI16 MapIndex)
{
	return &_Map[MapIndex];
}

MapIndexHeader*  OnlineMapIndexParser::GetVillageMapData(SI16 MapIndex)
{
	return &_Village[MapIndex];
}

MapIndexHeader*  OnlineMapIndexParser::GetMapIndexData(SI16 MapIndex)
{
	Map_Index::iterator find = _MapIndex.lower_bound(MapIndex);

	if( find != _MapIndex.end() && !( _MapIndex.key_comp()( MapIndex, find->first )) )
		return find->second;

	for( Map_Index::iterator i=_MapIndex.begin(); i!=_MapIndex.end(); ++i )
	{
		if( i->first == MapIndex )
			return i->second;
	}
	return NULL;
}

void OnlineMapIndexParser::Add( vector<MapIndexHeader> *v )
{
	for( vector<MapIndexHeader>::iterator i=v->begin(); i!=v->end(); ++i )
		efficientAddOrUpdata( _MapIndex, i->siMapIndex, i );
}