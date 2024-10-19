/************************************************************************************************
	���ϸ� : OnlineBaseMap.h
	����� : ������
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

// �� SearchMapInfo �Լ����� �˻��� �͵��� ���� ���.
#define	ON_MAP_SI_CHAR				1		// ĳ���� �˻�
#define	ON_MAP_SI_ITEM				2		// ������ �˻�.

// �¶��� Ÿ�� �Ӽ�
#define	ON_TILE_ATB_COLL		1			// �浹 �Ӽ�

// �¶��� Ÿ���� �ݴ� �Ӽ�(�ش� ��Ʈ�� OFF ��Ű�� ���� �ʿ�)	RATB -> ReverseATB
#define	ON_TILE_RATB_COLL		0xfffffffe

// DelID�� ����Ͽ� �ʿ��� ĳ���� �� ������Ʈ�� ���ﶧ�� �ɼ�
#define	ON_MAP_DELID_CHAR					1
#define	ON_MAP_DELID_OBJECT					2
#define	ON_MAP_DELID_ITEM					4
#define	ON_MAP_DELID_ALL					(ON_MAP_DELID_CHAR | ON_MAP_DELID_OBJECT | ON_MAP_DELID_ITEM)

// Tile Kind, Ÿ�� �����̴�.
#define	ON_TILE_KIND_GROUND					1		// �Ϲ� ��
#define	ON_TILE_KIND_SEA					2		// �ٴ�

// ���� ���.
struct	LoadSaveMapHeader
{
	R64		rVersion;			// ����
	SI16	siXsize;			// X ������
	SI16	siYsize;			// Y ������
};

// ���� ������ Ÿ�� ����.
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


// ���������� ����ϴ� ���� ����,�ε��Ҷ� ���� ����ü
struct	LoadSaveMap
{
	LoadSaveMapHeader	Header;
	LoadSaveMapTI		*pTI;	
};


// �¶��ο��� ����ϴ� �⺻ ����(����, �� Ŭ���̾�Ʈ)
class	OnlineBaseMap
{
public:
	SI32					m_siXsize, m_siYsize;		// ������ ������.

public:
	OnlineBaseMap();									// ������.
	virtual	~OnlineBaseMap();							// �Ҹ���.	
	
	virtual	BOOL	IsInMapArea(SI32 siX, SI32 siY);									// �ش� Map X,Y�� Map�� ũ�⸦ ����� �ʴ��� �˻��Ѵ�.
	virtual	BOOL	IsColl(SI32 siAtb);													// �浹 Ÿ������ �˾ƿ´�.
	virtual	BOOL	IsColl(SI32 siAtb, SI32 siTileKind, SI32 siCharMoveableTileKind);	// �浹 Ÿ������ �˾ƿ´�.(Ư�� ĳ���� ���忡��)
};


#endif
