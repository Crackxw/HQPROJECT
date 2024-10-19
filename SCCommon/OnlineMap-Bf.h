#ifndef	_ONLINE_MAP_OBF_H
#define _ONLINE_MAP_OBF_H

#define	ON_MAP_OI_CODE_KIND				1
#define	ON_MAP_OI_CODE_RELATIVEPOS		2
#define	ON_MAP_OI_CODE_VILLAGE			3

#define	ON_MAP_TI_ATB_COLL				1			// 충돌 타일.
#define	ON_MAP_TI_ATB_SEA				2			// 물 타일.

struct	ObjectInfoBaseBf
{
	UI16	uiCode			: 3;
	UI16	siParameter		: 13;
};

struct	ObjectInfoRelativePosBf
{
	UI16	uiCode		: 3;
	SI16	siReverse	: 1;
	SI16	siX			: 6;
	SI16	siY			: 6;
};

union	ObjectInfoBfs
{
	ObjectInfoBaseBf			BaseBf;
	ObjectInfoRelativePosBf		RelativePosBf;
};

struct	TileInfoBf
{
	UI32	uiFile		: 12;
	UI32	uiFont		: 8;
	UI32	uiAtb		: 12;	

	UI32	uiRoadFile	: 12;
	UI32	uiRoadFont	: 8;
	UI32	uiRoadAtb	: 12;	
	
	UI16	uiObjectInfo;
};

#endif
