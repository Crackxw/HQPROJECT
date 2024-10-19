#include <GSL.h>

#include	"Directives.h"
#include	"OnlineMapStore-Parser.h"
#include	"bindJxFile.h"

     
OnlineMapStoreParser::OnlineMapStoreParser()
{
}

OnlineMapStoreParser::~OnlineMapStoreParser()
{
	Free();
}

BOOL OnlineMapStoreParser::ReadText(char *pFileName)
{
	FILE	*fp		= NULL;
	char	*pToken = NULL;
	char	szBuffer[1024];	

	fp = BindJXFile::GetInstance()->GetFile( pFileName );
	if(fp == NULL) return FALSE;

	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);

//	if(pMapStoreHeader) delete[] pMapStoreHeader;
//	pMapStoreHeader = new MapStoreHeader[siMapStoreNum];

	MapStoreHeader	Ms;

	ZeroMemory(&Ms, sizeof(MapStoreHeader));

	while(fgets(szBuffer, 1024, fp))
	{
		// Code
		pToken = strtok(szBuffer, "\t\n\r");
		if(pToken == NULL)	break;

		if(pToken) Ms.siMapStoreCode = atoi(pToken);
		pToken = strtok(NULL, "\t\n\r");              
		 
		// Map Index
		pToken = strtok(NULL, "\t\n\r"); 
		if(pToken) Ms.siMapStoreMapIndex = atoi(pToken);

		// Map X Position        
		pToken = strtok(NULL, "\t\n\r");
		if(pToken) Ms.siMapStoreMapXPosition = atoi(pToken);
		
		// Map Y Position
		pToken = strtok(NULL, "\t\n\r");
		if(pToken) Ms.siMapStoreMapYPosition = atoi(pToken);

		// Map X Start Position
		pToken = strtok(NULL, "\t\n\r");
		if(pToken) Ms.siMapStoreMapXStartPosition = atoi(pToken);
		
		// Map Y Start Position
		pToken = strtok(NULL, "\t\n\r");
		if(pToken) Ms.siMapStoreMapYStartPosition = atoi(pToken);

		// Map X End Position
		pToken = strtok(NULL, "\t\n\r");
		if(pToken) Ms.siMapStoreMapXEndPosition = atoi(pToken);
		
		// Map Y End Position
		pToken = strtok(NULL, "\t\n\r");
		if(pToken) Ms.siMapStoreMapYEndPosition = atoi(pToken);
		
		// 범위
		pToken = strtok(NULL, "\t\n\r");
		if(pToken) Ms.siRange = atoi(pToken);

		switch( Ms.siMapStoreMapIndex )
		{
		case 2:
			_Msh01.insert( MSH::value_type( MAKELONG( Ms.siMapStoreMapXPosition, Ms.siMapStoreMapYPosition ), Ms ) );
			break;

		case 39:
			_Msh02.insert( MSH::value_type( MAKELONG( Ms.siMapStoreMapXPosition, Ms.siMapStoreMapYPosition ), Ms ) );
			break;

		case 40:
			_Msh03.insert( MSH::value_type( MAKELONG( Ms.siMapStoreMapXPosition, Ms.siMapStoreMapYPosition ), Ms ) );
			break;
		}	
		ZeroMemory(szBuffer, 1024);		
	}

	fclose(fp);

	_Index.push_back( 2 );
	_Index.push_back( 39 );
	_Index.push_back( 40 );
	
	return TRUE;
}

VOID OnlineMapStoreParser::Free()
{
/*
	if(pMapStoreHeader)
	{
		delete[] pMapStoreHeader;
		pMapStoreHeader = NULL;
	}
	pMapStoreHeader = NULL;
	siMapStoreNum   = 0;

	if(pVillageMapStoreHeader)
	{
		delete[] pVillageMapStoreHeader;
		pVillageMapStoreHeader = NULL;
	}
	pVillageMapStoreHeader = NULL;
	siVillageMapStoreNum   = 0;

	if(pSortMapStoreHeader)
	{
		delete[] pSortMapStoreHeader;
		pSortMapStoreHeader = NULL;
	}
	pSortMapStoreHeader = NULL;
	siSortMapStoreNum   = 0;
*/
	_Msh01.clear();
	_Msh02.clear();
	_Msh03.clear();
}

BOOL OnlineMapStoreParser::SetMapIndex(SI16 MapIndex)
{
	return TRUE;
/*
	SI16		TempCount;
	SI16		TempCount1;

	if(pSortMapStoreHeader)
	{
		delete[] pSortMapStoreHeader;
		pSortMapStoreHeader = NULL;
	}

	pSortMapStoreHeader = NULL;
	siSortMapStoreNum   = 0;

	// 우선 갯수를 검사한다.
	for(TempCount = 0; TempCount < siMapStoreNum; TempCount++)
	{
		if(pMapStoreHeader[TempCount].siMapStoreMapIndex == MapIndex)
		{
			siSortMapStoreNum++;
		}
	}

	if(siSortMapStoreNum)
	{
		pSortMapStoreHeader = new MapStoreHeader[siSortMapStoreNum];
		for(TempCount = 0, TempCount1 = 0; TempCount < siMapStoreNum; TempCount++)
		{
			if(pMapStoreHeader[TempCount].siMapStoreMapIndex == MapIndex)
			{
				CopyMemory(&pSortMapStoreHeader[TempCount1], &pMapStoreHeader[TempCount], sizeof(MapStoreHeader));
				TempCount1++;
			}
		}
	}

	return TRUE;
*/
}

/*
MapStoreHeader* OnlineMapStoreParser::GetMapStore(SI16 XPos, SI16 YPos, SI16 MapIndex)
{
	DWORD Key = MAKELONG( XPos, YPos );

	MSH::iterator find = _Msh01.lower_bound( Key );

	if( find != _Msh01.end() && !(_Msh01.key_comp()( Key, find->first )) )
	{
		return &find->second;
	}

	return NULL;
}
*/
/*
BOOL OnlineMapStoreParser::AddVillageMapStoreList(SI16 VillageMapStoreListCount, MapStoreAddHeader* lpVillageMapStoreList, SI16 siVillageSeries)
{
	if(pVillageMapStoreHeader)
	{
		delete[] pVillageMapStoreHeader;
		pVillageMapStoreHeader = NULL;
	}
	siVillageMapStoreNum   = VillageMapStoreListCount;
	pVillageMapStoreHeader = new MapStoreHeader[siVillageMapStoreNum];
	ZeroMemory(pVillageMapStoreHeader, sizeof(MapStoreHeader) * siVillageMapStoreNum);

	for(SI16 i = 0; i < siVillageMapStoreNum; i++)
	{
		pVillageMapStoreHeader[i].siMapStoreCode         = (siVillageSeries << 16) + i;
		pVillageMapStoreHeader[i].siMapStoreMapXPosition = lpVillageMapStoreList[i].siXPos;
		pVillageMapStoreHeader[i].siMapStoreMapYPosition = lpVillageMapStoreList[i].siYPos;
		pVillageMapStoreHeader[i].siMapStoreMapIndex     = lpVillageMapStoreList[i].siMapIndex;

		switch(lpVillageMapStoreList[i].uiDegree % 4)
		{
			case 0:
			case 2:
				pVillageMapStoreHeader[i].siMapStoreMapXStartPosition = lpVillageMapStoreList[i].siXPos - 1;
				pVillageMapStoreHeader[i].siMapStoreMapYStartPosition = lpVillageMapStoreList[i].siYPos - 1;
				pVillageMapStoreHeader[i].siMapStoreMapXEndPosition   = lpVillageMapStoreList[i].siXPos + 1;
				pVillageMapStoreHeader[i].siMapStoreMapYEndPosition   = lpVillageMapStoreList[i].siYPos;
				break;

			case 1:
			case 3:
				pVillageMapStoreHeader[i].siMapStoreMapXStartPosition = lpVillageMapStoreList[i].siXPos;
				pVillageMapStoreHeader[i].siMapStoreMapYStartPosition = lpVillageMapStoreList[i].siYPos - 1;
				pVillageMapStoreHeader[i].siMapStoreMapXEndPosition   = lpVillageMapStoreList[i].siXPos + 1;
				pVillageMapStoreHeader[i].siMapStoreMapYEndPosition   = lpVillageMapStoreList[i].siYPos + 1;
				break;
		}

		pVillageMapStoreHeader[i].siRange = 0;
	}

	return TRUE;
}
*/