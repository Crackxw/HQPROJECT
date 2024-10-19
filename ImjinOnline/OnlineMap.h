/************************************************************************************************
	파일명 : OnlineMap.h
	담당자 : 이윤석
************************************************************************************************/
#ifndef	_ONLINE_MAP_H
#define _ONLINE_MAP_H

#include "OnlineBaseMap.h"
#include "OnlineFieldParser.h"
#include "OnlineMapStore-Parser.h"
#include "OnlineNPCIndex-Parser.h"
#include "OnlineMapBoothInfo.h"
#include <DDraw.h>

// 전체 지도를 다 가지고 있지는 않고 자기를 중심으로 100x100 지도만 가지고 있는다.
#define	MAX_ONLINE_CHAR_MAP_XSIZE			61
#define	MAX_ONLINE_CHAR_MAP_YSIZE			61
#define	MAX_ONLINE_CHAR_MAP_HALF_XSIZE		30
#define MAX_ONLINE_CHAR_MAP_HALF_YSIZE		30

// 타일의 이미지 크기
#define	ONLINE_TILE_XSIZE					64
#define	ONLINE_TILE_XSIZE_HALF				32
#define	ONLINE_TILE_YSIZE					32
#define	ONLINE_TILE_YSIZE_HALF				16

#define ON_MAP_VIEW_XSIZE_HALF				30
#define	ON_MAP_VIEW_YSIZE_HALF				30

// 길의 종류 
#define	ON_MAP_ROAD_ATB_ROAD				1			// 보통 길


// 온라인 타일 속성
//#define	ON_TILE_ATB_COLL				1								// 충돌 속성 (이미 OnlineBaseMap.h에서 선언했다.)
#define	ON_TILE_ATB_CHAR_RESERVE			(2 | ON_TILE_ATB_COLL)			// 어느 캐릭터가 이미 자리를 예약했다.

// 한 타일의 정보.
struct	strTI
{
	SI16	siFile;
	SI16	siFont;				
	UI32	uiTileAtb;
	SI32	siAtb;		
	SI32	siBackupAtb;		
	SI32	siKind;
	SI32	siSpeed;															

	SI16	siCharID;						// 캐릭터의 ID		

	UI16	uiObjectID;														
	UI32	uiItemID;															
	UI16	uiItemQuantity;													

	UI16	uiRoadFile;
	UI16	uiRoadFont;
	UI32	uiRoadAtb;
	UI08	uiRoadKind;		

	UI08	bUnknownObjectID;												
	SI16	siSourceObjectX;
	SI16	siSourceObjectY;												

	SI16	siFieldGuideTable;												
};

struct Animation
{
	BOOL	bAction;
	BOOL	bRepeat;
	DWORD	dwCurFrame;
	UI08	uiMaxFrame;
	SI32	siX, siY;
	XSPR	AniSpr;
	SI08	siCurCount;
	SI08	siMaxCount;
	bool	bFlag;
};

class	OnlineMap : public OnlineBaseMap 
{
private:			
	cltOnlineWorld			*pMyOnlineWorld;

	CHAR					szFileName[1024];
	UI32					uiMultiplyCharMapYOff[MAX_ONLINE_CHAR_MAP_YSIZE];

	FILE					*fpMap;
	fpos_t					fposMap;
	strTI					*m_pstMap;														// 맵의 각각의 타일 정보.(배열임),
	SI16*					siVMercenary;													// 따라다니는 용병들, 한자리에 8명의 용병까지 출력가능하다.	
	XSPR*					m_BlackTile;
	Animation				m_AniData;
	BOOL					m_IsBoothMap;

public:
	// Tile Draw Cache
	LPDIRECTDRAWSURFACE7	m_TileDrawCacheSurface;

public:
	// 타일 좌표.
	MapIndexHeader			m_MapInfo;
	SI16					m_MapIndex;
	SI16					m_siX, m_siY;
	SI16					m_siRX, m_siRY;
	UI16					m_uiCurrentFocusCharID;
	BOOL					m_fHeroSelect;

	// 사각형을 찍는 범위
	RECT					m_rcScreenRect;

	OnlineMapStoreParser	m_MapStoreParser;
	OnlineNPCIndexParser	m_NPCParser;
	OnlineMapBoothInfo		m_MapBoothInfo;
	OnlineFieldParser		FieldParser;
	OnlineMapIndexParser	MapIndexParser;
	OnlinePortalParser		PortalParser;

public:	
	OnlineMap();			// 생성자.
	~OnlineMap();			// 소멸자.

	BOOL	Create(cltOnlineWorld *pOnlineWorld);																// Create
	BOOL	Init(cltOnlineWorld *pOnlineWorld);																	// 초기화.	
	BOOL	Init(SI16 MapIndex, SI16 siXsize, SI16 siYsize, LoadSaveMapTI *pTI, cltOnlineWorld *pOnlineWorld);	// 초기화.
	VOID	Draw(LPDIRECTDRAWSURFACE7 pSurface);													// 전 지도를 전부 그린다(캐릭터, 오브젝트, 타일).
	VOID	DrawTile(LPDIRECTDRAWSURFACE7 pSurface);												// 타일만 그린다.

	VOID	DrawTileCache();																		// Tile Cache에 타일만 그린다.
	VOID	Free();																					// 해제.		
	VOID	SetCurrentFocusCharID(UI16 uiNewFocusCharID);											// 새로이 맵의 포커스를 가진 캐릭터를 설정한다
	UI16	GetCurrentFocusCharID();																// 새로이 맵의 포커스를 가진 캐릭터를 얻어온다.
	VOID	Action();																				// 포커스를 맞춘 캐릭터를 가운데에 오도록 맵을 이동한다.	
	VOID	PlayFitMusic(VOID);
	

	BOOL	TransformMouseCoordToMapXY(SI32	siMouseX,SI32 siMouseY, SI32 &siMapX, SI32 &siMapY);						// 화면의 마우스 좌표를 Map의 X, Y 좌표로 얻어온다.	
	BOOL	TransformScreenDotXYToRealDotXY(SI32 siScreenDotX, SI32 siScreenDotY, SI32 &siRealDotX, SI32 &siRealDotY);	// Screen Dot 좌표를 실제 절대적인 위치로 전환을 해서 Dot값으로 얻어온다.	
	BOOL	TransformRealDotXYToMapXY(SI32	siRealDotX, SI32 siRealDotY, SI32 &siMapX, SI32 &siMapY);					// 절대 좌표를 Map의 좌표로 바꾸어준다.	
	BOOL	TransformRealDotXYToScreenDotXY(SI32 siRealDotX, SI32 siRealDotY, SI32 &siScreenDotX, SI32 &siScreenDotY);	// 실제 절대적인 위치로 Screen Dot 좌표로 전환을 한다.	
	BOOL 	TransformMapXYToRealDotCenterXY(SI32 siMapX, SI32 siMapY, SI32 &siRealDotCX, SI32 &siRealDotCY);			// Map[y][x]의 중심위치를 절대적인 위치로 전환을 해서 Dot값으로 얻어온다.	
	BOOL	TransformMapXYToScreenDotCenterXY(SI32 siMapX, SI32 siMapY, SI32 &siScreenDotCX, SI32 &siScreenDotCY);	
	BOOL	TransformMapXYToRealXY(SI32 siMapX, SI32 siMapY, SI32 &siRealX, SI32 &siRealY);								// 맵의 좌표를 실제 맵상의 도트 좌표로 변환한다.
	BOOL	TransformRealXYToMapXY(SI32 siRealX, SI32 siRealY, SI32 &siMapX, SI32 &siMapY);								// 실제 맵상의 도트 좌표를 맵의 좌표로 변환한다.
	BOOL	GetMapRealSize(SI32 &siRealX, SI32 &siRealY);																// 맵의 실제 크기를 계산한다.
	
	BOOL	IsInScreenMapArea(SI32 siX, SI32 siY);							// 해당 Map X,Y가 화면에 보여주고 있는 Map의 크기를 벗어나지 않는지 검사한다.
	BOOL	IsInScreenRect(SI32 siX, SI32 siY);								// 해당 스크린상의 X, Y가 맵을 출력하는 사각형 영역을 벗어나지 않는지 검사한다.
	BOOL	IsColl(SI32 siX, SI32 siY);										// 충돌 좌표인지 얻어온다.
	BOOL	IsColl(SI32 siX, SI32 siY, SI32 siCharMoveableTileAtb);			// 충돌 좌표인지 얻어온다.
	SI16	CollCheckType(SI32 siX, SI32 siY);								// 충돌 Type을 얻어온다.(0 : 이동불가, 1 : 캐릭터가 위에 있음, 2 : 이동 가능)
	BOOL	IsCharReserved(SI32 siX, SI32 siY);								// 이미 다른 캐릭터가 자리를 예약해 두었는지 본다.
	VOID	Move(SI32 siDirect, SI32 siUnit);								// 이동 시킨다.		
	BOOL	Save(CHAR *pszFileName);										// 맵을 저장한다.
	BOOL	Load(SI16 MapIndex, cltOnlineWorld *pOnlineWorld);			// 맵을 로드한다.
	
	BOOL	Load(cltOnlineWorld *pOnlineWorld);								// PCX를 사용하여 맵을 설정한다.--> 2/27
	VOID	ReadPaletteMatchTile();											//PCX를 사용하여 맵을 설정하기 위하여 팔레트에 대응하는 타일 화일과 폰트를 읽어온다. --> 2/28

	VOID	SetTile(SI16 siTileX, SI16 siTileY, SI16 siFileIndex, SI16 siFontIndex, UI32 siAtb, UI16 uiRoadFileIndex, UI16 uiRoadFontIndex);			// 타일을 설정한다.		
	VOID	SetID(SI16 siTileX, SI16 siTileY, SI16 siXTileNum, SI16 siYTileNum, SI16 siCharID, UI16 uiObjectID, UI32 uiItemID, UI16 uiItemQuantity);	// 특정 위치에 캐릭터 및 오브젝트을 심는다.
	VOID	DelID(SI16 siTileX, SI16 siTileY, SI16 siXTileNum, SI16 siYTileNum, SI32 siDelInfo);														// 특정 위치에서 캐릭터 및 오브젝트을 뺀다.	
	VOID	EditItem(SI16 siTileX, SI16 siTileY, UI16 Qut);																								// 특정 위치에서 아이템 data를 수정한다.
	VOID	SetTileAtb(SI16 siTileX, SI16 siTileY, SI16 siXTileNum, SI16 siYTileNum, SI32 siAtb, BOOL bSet);											// 특정 타일에 특정 속성을 정의하거나 뺀다.

	VOID	GetCurrentPos(SI32 *psiX, SI32 *psiY);							// 인터패이스를 없애구 추가하는 함수
	RECT	GetScreenRect();												// 인터패이스를 없애구 추가하는 함수
	VOID	GetSize(SI32 *psiXsize, SI32 *psiYsize);						// 인터패이스를 없애구 추가하는 함수	

	BOOL	ChangeMap(SI16 MapIndex, SI16 siX, SI16 siY);					// 새로운 맵을 로딩하고 그 지역으로 이동
	VOID	SetXY(SI16 siX, SI16 siY);										// 현재 맵의 X Y 위치를 설정한다.
	VOID	SetX(SI16 siX);													// 현재 맵의 X 위치를 설정한다.
	VOID	SetY(SI16 siY);													// 현재 맵의 Y 위치를 설정한다.
	SI16	GetRX() { return m_siRX; }										// 현재 맵의 X 변위를 얻어온다.	
	SI16	GetRY() { return m_siRY; }										// 현재 맵의 Y 변위를 얻어온다.

	SI32	GetObjectID		(SI16 siTileX, SI16 siTileY);		// 맵에 설정되어 있는 오브젝트의 ID를 얻어온다.
	SI32	GetCharID		(SI16 siTileX, SI16 siTileY);		// 맵에 설정되어 있는 캐릭터의 ID를 얻어온다.
	UI32	GetItemID		(SI16 siTileX, SI16 siTileY);		// 맵에 설정되어 있는 아이템의 ID를 얻어온다.
	UI16	GetItemQuantity	(SI16 siTileX, SI16 siTileY);		// 맵에 설정되어 있는 아이템의 수를 얻어온다.

	VOID	PreProcessorBeforeLoadUnloadedTile(SI32 siDirection = 0);						// m_siX와 m_siY를 수정하고 LoadUnloadedTile을 호출하려고 할때 m_siX와 m_siY를 수정하기 전에 호출해 주어야한다
	BOOL	LoadUnloadedTile(SI32 siDirection = 0);											// 로드되어있지 않은 타일을 로드한다.		

	VOID	MakeMapFile(SI32 siDirection);	

	VOID	CreateDrawTable();

	SI32	siTileNo;
	SI32	siDrawOrder[((ON_MAP_VIEW_XSIZE_HALF)*2+1)*((ON_MAP_VIEW_YSIZE_HALF)*2+1)*2]; //순서 테이블

	BOOL	GetRoadSpeed(SI16 siTileX, SI16 siTileY, SI32 *psiAddMoveSpeed);			// 길인 경우 추가 이동 속도를 구한다.
	SI08	GetWaterFlag(SI16 siTileX, SI16 siTileY);									// Tile의 물 속성을 구해온다.
	SI08	GetWaterFlag(strTI* lpTileData);											// Tile의 물 속성을 구해온다.
	SI32	GetPKMapFlag()	{ return m_MapInfo.fWarFlag; };

	BOOL	GetPointTo(LPPOINT pptMouse, SI32 *psiCharID, SI32 *psiItemPos, SI32 *psiObjectID);		// 마우스가 가리키는 캐릭터의 ID를 얻어온다.	
	
	POINT	SearchMap(SI32 siCX, SI32 siCY, SI32 siSearchInfo, SI32 siParam1  = 0);
	inline	VOID	AnalysisObjectInfo(SI16 siTileX, SI16 siTileY, UI16 uiObjectInfo);				// 오브젝트의 정보를 분석하여 처리해준다.	

	VOID	PrepareHeader(LoadSaveMapHeader *pHeader);									// 헤더에 데이타를 넣는다.
	MapStoreHeader*	GetMapStoreParser(SI16 siX, SI16 siY, SI16* lpMPointFlag);			// Map Store 정보를 얻는다.

	inline	VOID	SetObjectID(SI16 siTileX, SI16 siTileY, UI16 uiObjectID);			// 특정 위치에 1x1 오브젝트를 심는다.
	inline	VOID	SetObjectID2(SI16 siTileX, SI16 siTileY, UI16 uiObjectID);			// 특정 위치에 오브젝트를 심는다.
	
	inline	BOOL	TransformMapXYToCharMapXY(SI32 siMapX, SI32 siMapY, SI32 &siCharMapX, SI32 &siCharMapY);			// 타일 좌표를 캐릭터만의 맵좌표로 변환한다.
	inline	BOOL	TransformMapXYToCharMapXYNonClipping(SI32 siMapX, SI32 siMapY, SI32 &siCharMapX, SI32 &siCharMapY);	// 타일 좌표를 캐릭터만의 맵좌표로 변환한다.

	SI32	GetWarFieldMap(void);														// Get War Field Map
	BOOL	CheckPKFieldMap(void);														// 현재 FieldMap이 PK가 가능한지 검사
	BOOL	CheckPKFieldMap(SI16 siMapIndex);											// 현재 FieldMap이 PK가 가능한지 검사

	void	VMercenaryBufferClear(void);
	void	VMercenaryAdd(SI16 x, SI16 y, SI16 siCharID);
	void	VMercenaryDelete(SI16 x, SI16 y, SI16 siCharID);
	SI16	VMercenaryGetCount(SI16 x, SI16 y);
	void	SetAnimation(SI32 siX, SI32 siY, UI08 MaxFrame, CHAR *szPath, SI08 count = 1, BOOL repeat = FALSE,bool bThunder = TRUE);
	VOID	AnimationDraw(LPDIRECTDRAWSURFACE7 pSurface);
	VOID	BoothMapDraw(SI16 siTileX, SI16 siTileY, POINT &ptMouse, LPDIRECTDRAWSURFACE7 pSurface);

	BOOL	IsInCharMapArea(SI32 siX, SI32 siY)								// 해당 Map X,Y가 캐릭터 Map의 크기를 벗어나지 않는지 검사한다.
	{
		if((siX < max(0, m_siX - MAX_ONLINE_CHAR_MAP_HALF_XSIZE)) || (siY < max(0, m_siY - MAX_ONLINE_CHAR_MAP_HALF_YSIZE)) ||
			(siX > min(m_siXsize - 1, m_siX + MAX_ONLINE_CHAR_MAP_HALF_XSIZE)) || (siY > min(m_siYsize - 1, m_siY + MAX_ONLINE_CHAR_MAP_HALF_YSIZE)))
			return FALSE;

		return TRUE;
	}
};

 
#endif
