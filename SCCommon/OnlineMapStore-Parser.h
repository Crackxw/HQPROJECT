#ifndef __ONLINEMAPSTORE_PARSER_H
#define __ONLINEMAPSTORE_PARSER_H

#pragma warning ( disable : 4786 )

#include <iostream>
#include <map>
#include <vector>

using namespace std;



struct	MapStoreHeader
{
	SI16	siMapStoreCode;																	// Code
	SI16	siMapStoreMapXPosition;															// Map X Position
	SI16	siMapStoreMapYPosition;															// Map Y Position
	SI16	siMapStoreMapIndex;																// Map Index

	SI16	siMapStoreMapXStartPosition;													// ������ ������ġ
	SI16	siMapStoreMapYStartPosition;													// ������ ������ġ
	SI16	siMapStoreMapXEndPosition;														// ������ ����ġ
	SI16	siMapStoreMapYEndPosition;														// ������ ����ġ

	SI16	siRange;																		// ����
};

/*
struct	MapStoreAddHeader
{
	SI16	siMapIndex;
	SI16	siXPos;
	SI16	siYPos;

	UI16	uiDegree;
};
*/

class OnlineMapStoreParser
{
public:
	typedef map< DWORD, MapStoreHeader, less< DWORD > >	MSH;
	MSH						_Msh01, _Msh02, _Msh03;
	vector<SI16>			_Index;

//	MapStoreHeader			*pVillageMapStoreHeader;											// Village MapStore�� ��� ������ ���� ����ü
//	SI16					siVillageMapStoreNum;												// �� Village MapStore

//	MapStoreHeader			*pSortMapStoreHeader;												// ������ MapStore�� ��� ������ ���� ����ü
//	SI16					siSortMapStoreNum;													// �� MapStore
	
public:
	OnlineMapStoreParser();
	~OnlineMapStoreParser();

	BOOL					ReadText(char *pFileName);											// Portal �ε�
	VOID					Free();																// �����

	BOOL					SetMapIndex(SI16 MapIndex);											// Set MapIndex	
	SI16					GetMapSize()						{ return _Msh01.size(); }		// SIZE
	MapStoreHeader*			GetMapStore(SI16 siMapStoreCode)	{ return &_Msh01[siMapStoreCode]; }
	MapStoreHeader*			GetMapStore(SI16 XPos, SI16 YPos, SI16 MapIndex)					// Get MapStore
	{
		DWORD Key = MAKELONG( XPos, YPos );
		MSH::iterator find = _Msh01.lower_bound( Key );
		return ( find != _Msh01.end() && !(_Msh01.key_comp()( Key, find->first )) ) == 1 ? &find->second : NULL;
	}	

	BOOL					IsMapIndex(SI16 MapIndex)
	{
		for( vector<SI16>::iterator i=_Index.begin(); i!=_Index.end(); ++i )
			if( *i == MapIndex )	return TRUE;
			
		return FALSE;
	}



//	BOOL					AddVillageMapStoreList(SI16 VillageMapStoreListCount, MapStoreAddHeader* lpVillageMapStoreList, SI16 siVillageSeries);	// Add Village Map Store
};


#endif
