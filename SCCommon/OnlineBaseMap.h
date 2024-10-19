/************************************************************************************************
	파일명 : OnlineBaseMap.h
	담당자 : 이윤석
************************************************************************************************/
#ifndef	_ONLINE_BASEMAP_H
#define _ONLINE_BASEMAP_H

#define	ONLINE_MAP_VERSION		0.11

#include "OnlineFieldParser.h"
#include "OnlinePortal-Parser.h"
#include "OnlineMapIndex-Parser.h"
#include "OnlineMap-Bf.h"

#define	GETFONTINDEX(x)				(x & 0x7fff)
#define	ISCOLLFONTINDEX(x)			((x & 0x8000) ? TRUE : FALSE)

#define	MAX_ONLINEMAP_XSIZE			3200
#define	MAX_ONLINEMAP_YSIZE			3200

// 각 SearchMapInfo 함수에서 검색할 것들의 대한 상수.
#define	ON_MAP_SI_CHAR				1		// 캐릭터 검색
#define	ON_MAP_SI_ITEM				2		// 아이템 검색.

// 온라인 타일 속성
#define	ON_TILE_ATB_COLL		1			// 충돌 속성

// 온라인 타일의 반대 속성(해당 비트를 OFF 시키기 위해 필요)	RATB -> ReverseATB
#define	ON_TILE_RATB_COLL		0xfffffffe

// DelID를 사용하여 맵에서 캐릭터 및 오브젝트를 지울때의 옵션
#define	ON_MAP_DELID_CHAR					1
#define	ON_MAP_DELID_OBJECT					2
#define	ON_MAP_DELID_ITEM					4
#define	ON_MAP_DELID_ALL					(ON_MAP_DELID_CHAR | ON_MAP_DELID_OBJECT | ON_MAP_DELID_ITEM)

// Tile Kind, 타일 종류이다.
#define	ON_TILE_KIND_GROUND					1		// 일반 땅
#define	ON_TILE_KIND_SEA					2		// 바다

// 맵의 헤더.
struct	LoadSaveMapHeader
{
	R64		rVersion;			// 버전
	SI16	siXsize;			// X 사이즈
	SI16	siYsize;			// Y 사이즈
};

// 맵의 각각의 타일 정보.
//typedef	TileInfoBf	LoadSaveMapTI;

typedef struct
{
	UI32	uiFile		: 12;
	UI32	uiFont		: 8;
	UI32	uiAtb		: 12;	
} LoadSaveTile;

typedef struct
{
	UI32	uiRoadFile	: 12;
	UI32	uiRoadFont	: 8;
	UI32	uiRoadAtb	: 12;	
} LoadSaveRoad;

struct	LoadSaveMapTI
{
	LoadSaveTile		stLoadSaveTile;
	LoadSaveRoad		stLoadSaveRoad;
	
	UI16				uiObjectInfo;
};


// 실질적으로 사용하는 맵을 저장,로드할때 쓰는 구조체
struct	LoadSaveMap
{
	LoadSaveMapHeader	Header;
	LoadSaveMapTI		*pTI;	
};


// 온라인에서 사용하는 기본 지도(서버, 및 클라이언트)
class	OnlineBaseMap
{
public:
	SI32					m_siXsize, m_siYsize;		// 지도의 사이즈.

public:
	OnlineBaseMap();									// 생성자.
	virtual	~OnlineBaseMap();							// 소멸자.	
	
	virtual	BOOL	IsInMapArea(SI32 siX, SI32 siY);									// 해당 Map X,Y가 Map의 크기를 벗어나지 않는지 검사한다.
	virtual	BOOL	IsColl(SI32 siAtb);													// 충돌 타일인지 알아온다.
	virtual	BOOL	IsColl(SI32 siAtb, SI32 siTileKind, SI32 siCharMoveableTileKind);	// 충돌 타일인지 알아온다.(특정 캐릭터 입장에서)
};


#endif
