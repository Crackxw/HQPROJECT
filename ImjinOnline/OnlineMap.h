/************************************************************************************************
	���ϸ� : OnlineMap.h
	����� : ������
************************************************************************************************/
#ifndef	_ONLINE_MAP_H
#define _ONLINE_MAP_H

#include "OnlineBaseMap.h"
#include "OnlineFieldParser.h"
#include "OnlineMapStore-Parser.h"
#include "OnlineNPCIndex-Parser.h"
#include "OnlineMapBoothInfo.h"
#include <DDraw.h>

// ��ü ������ �� ������ ������ �ʰ� �ڱ⸦ �߽����� 100x100 ������ ������ �ִ´�.
#define	MAX_ONLINE_CHAR_MAP_XSIZE			61
#define	MAX_ONLINE_CHAR_MAP_YSIZE			61
#define	MAX_ONLINE_CHAR_MAP_HALF_XSIZE		30
#define MAX_ONLINE_CHAR_MAP_HALF_YSIZE		30

// Ÿ���� �̹��� ũ��
#define	ONLINE_TILE_XSIZE					64
#define	ONLINE_TILE_XSIZE_HALF				32
#define	ONLINE_TILE_YSIZE					32
#define	ONLINE_TILE_YSIZE_HALF				16

#define ON_MAP_VIEW_XSIZE_HALF				30
#define	ON_MAP_VIEW_YSIZE_HALF				30

// ���� ���� 
#define	ON_MAP_ROAD_ATB_ROAD				1			// ���� ��


// �¶��� Ÿ�� �Ӽ�
//#define	ON_TILE_ATB_COLL				1								// �浹 �Ӽ� (�̹� OnlineBaseMap.h���� �����ߴ�.)
#define	ON_TILE_ATB_CHAR_RESERVE			(2 | ON_TILE_ATB_COLL)			// ��� ĳ���Ͱ� �̹� �ڸ��� �����ߴ�.

// �� Ÿ���� ����.
struct	strTI
{
	SI16	siFile;
	SI16	siFont;				
	UI32	uiTileAtb;
	SI32	siAtb;		
	SI32	siBackupAtb;		
	SI32	siKind;
	SI32	siSpeed;															

	SI16	siCharID;						// ĳ������ ID		

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
	strTI					*m_pstMap;														// ���� ������ Ÿ�� ����.(�迭��),
	SI16*					siVMercenary;													// ����ٴϴ� �뺴��, ���ڸ��� 8���� �뺴���� ��°����ϴ�.	
	XSPR*					m_BlackTile;
	Animation				m_AniData;
	BOOL					m_IsBoothMap;

public:
	// Tile Draw Cache
	LPDIRECTDRAWSURFACE7	m_TileDrawCacheSurface;

public:
	// Ÿ�� ��ǥ.
	MapIndexHeader			m_MapInfo;
	SI16					m_MapIndex;
	SI16					m_siX, m_siY;
	SI16					m_siRX, m_siRY;
	UI16					m_uiCurrentFocusCharID;
	BOOL					m_fHeroSelect;

	// �簢���� ��� ����
	RECT					m_rcScreenRect;

	OnlineMapStoreParser	m_MapStoreParser;
	OnlineNPCIndexParser	m_NPCParser;
	OnlineMapBoothInfo		m_MapBoothInfo;
	OnlineFieldParser		FieldParser;
	OnlineMapIndexParser	MapIndexParser;
	OnlinePortalParser		PortalParser;

public:	
	OnlineMap();			// ������.
	~OnlineMap();			// �Ҹ���.

	BOOL	Create(cltOnlineWorld *pOnlineWorld);																// Create
	BOOL	Init(cltOnlineWorld *pOnlineWorld);																	// �ʱ�ȭ.	
	BOOL	Init(SI16 MapIndex, SI16 siXsize, SI16 siYsize, LoadSaveMapTI *pTI, cltOnlineWorld *pOnlineWorld);	// �ʱ�ȭ.
	VOID	Draw(LPDIRECTDRAWSURFACE7 pSurface);													// �� ������ ���� �׸���(ĳ����, ������Ʈ, Ÿ��).
	VOID	DrawTile(LPDIRECTDRAWSURFACE7 pSurface);												// Ÿ�ϸ� �׸���.

	VOID	DrawTileCache();																		// Tile Cache�� Ÿ�ϸ� �׸���.
	VOID	Free();																					// ����.		
	VOID	SetCurrentFocusCharID(UI16 uiNewFocusCharID);											// ������ ���� ��Ŀ���� ���� ĳ���͸� �����Ѵ�
	UI16	GetCurrentFocusCharID();																// ������ ���� ��Ŀ���� ���� ĳ���͸� ���´�.
	VOID	Action();																				// ��Ŀ���� ���� ĳ���͸� ����� ������ ���� �̵��Ѵ�.	
	VOID	PlayFitMusic(VOID);
	

	BOOL	TransformMouseCoordToMapXY(SI32	siMouseX,SI32 siMouseY, SI32 &siMapX, SI32 &siMapY);						// ȭ���� ���콺 ��ǥ�� Map�� X, Y ��ǥ�� ���´�.	
	BOOL	TransformScreenDotXYToRealDotXY(SI32 siScreenDotX, SI32 siScreenDotY, SI32 &siRealDotX, SI32 &siRealDotY);	// Screen Dot ��ǥ�� ���� �������� ��ġ�� ��ȯ�� �ؼ� Dot������ ���´�.	
	BOOL	TransformRealDotXYToMapXY(SI32	siRealDotX, SI32 siRealDotY, SI32 &siMapX, SI32 &siMapY);					// ���� ��ǥ�� Map�� ��ǥ�� �ٲپ��ش�.	
	BOOL	TransformRealDotXYToScreenDotXY(SI32 siRealDotX, SI32 siRealDotY, SI32 &siScreenDotX, SI32 &siScreenDotY);	// ���� �������� ��ġ�� Screen Dot ��ǥ�� ��ȯ�� �Ѵ�.	
	BOOL 	TransformMapXYToRealDotCenterXY(SI32 siMapX, SI32 siMapY, SI32 &siRealDotCX, SI32 &siRealDotCY);			// Map[y][x]�� �߽���ġ�� �������� ��ġ�� ��ȯ�� �ؼ� Dot������ ���´�.	
	BOOL	TransformMapXYToScreenDotCenterXY(SI32 siMapX, SI32 siMapY, SI32 &siScreenDotCX, SI32 &siScreenDotCY);	
	BOOL	TransformMapXYToRealXY(SI32 siMapX, SI32 siMapY, SI32 &siRealX, SI32 &siRealY);								// ���� ��ǥ�� ���� �ʻ��� ��Ʈ ��ǥ�� ��ȯ�Ѵ�.
	BOOL	TransformRealXYToMapXY(SI32 siRealX, SI32 siRealY, SI32 &siMapX, SI32 &siMapY);								// ���� �ʻ��� ��Ʈ ��ǥ�� ���� ��ǥ�� ��ȯ�Ѵ�.
	BOOL	GetMapRealSize(SI32 &siRealX, SI32 &siRealY);																// ���� ���� ũ�⸦ ����Ѵ�.
	
	BOOL	IsInScreenMapArea(SI32 siX, SI32 siY);							// �ش� Map X,Y�� ȭ�鿡 �����ְ� �ִ� Map�� ũ�⸦ ����� �ʴ��� �˻��Ѵ�.
	BOOL	IsInScreenRect(SI32 siX, SI32 siY);								// �ش� ��ũ������ X, Y�� ���� ����ϴ� �簢�� ������ ����� �ʴ��� �˻��Ѵ�.
	BOOL	IsColl(SI32 siX, SI32 siY);										// �浹 ��ǥ���� ���´�.
	BOOL	IsColl(SI32 siX, SI32 siY, SI32 siCharMoveableTileAtb);			// �浹 ��ǥ���� ���´�.
	SI16	CollCheckType(SI32 siX, SI32 siY);								// �浹 Type�� ���´�.(0 : �̵��Ұ�, 1 : ĳ���Ͱ� ���� ����, 2 : �̵� ����)
	BOOL	IsCharReserved(SI32 siX, SI32 siY);								// �̹� �ٸ� ĳ���Ͱ� �ڸ��� ������ �ξ����� ����.
	VOID	Move(SI32 siDirect, SI32 siUnit);								// �̵� ��Ų��.		
	BOOL	Save(CHAR *pszFileName);										// ���� �����Ѵ�.
	BOOL	Load(SI16 MapIndex, cltOnlineWorld *pOnlineWorld);			// ���� �ε��Ѵ�.
	
	BOOL	Load(cltOnlineWorld *pOnlineWorld);								// PCX�� ����Ͽ� ���� �����Ѵ�.--> 2/27
	VOID	ReadPaletteMatchTile();											//PCX�� ����Ͽ� ���� �����ϱ� ���Ͽ� �ȷ�Ʈ�� �����ϴ� Ÿ�� ȭ�ϰ� ��Ʈ�� �о�´�. --> 2/28

	VOID	SetTile(SI16 siTileX, SI16 siTileY, SI16 siFileIndex, SI16 siFontIndex, UI32 siAtb, UI16 uiRoadFileIndex, UI16 uiRoadFontIndex);			// Ÿ���� �����Ѵ�.		
	VOID	SetID(SI16 siTileX, SI16 siTileY, SI16 siXTileNum, SI16 siYTileNum, SI16 siCharID, UI16 uiObjectID, UI32 uiItemID, UI16 uiItemQuantity);	// Ư�� ��ġ�� ĳ���� �� ������Ʈ�� �ɴ´�.
	VOID	DelID(SI16 siTileX, SI16 siTileY, SI16 siXTileNum, SI16 siYTileNum, SI32 siDelInfo);														// Ư�� ��ġ���� ĳ���� �� ������Ʈ�� ����.	
	VOID	EditItem(SI16 siTileX, SI16 siTileY, UI16 Qut);																								// Ư�� ��ġ���� ������ data�� �����Ѵ�.
	VOID	SetTileAtb(SI16 siTileX, SI16 siTileY, SI16 siXTileNum, SI16 siYTileNum, SI32 siAtb, BOOL bSet);											// Ư�� Ÿ�Ͽ� Ư�� �Ӽ��� �����ϰų� ����.

	VOID	GetCurrentPos(SI32 *psiX, SI32 *psiY);							// �������̽��� ���ֱ� �߰��ϴ� �Լ�
	RECT	GetScreenRect();												// �������̽��� ���ֱ� �߰��ϴ� �Լ�
	VOID	GetSize(SI32 *psiXsize, SI32 *psiYsize);						// �������̽��� ���ֱ� �߰��ϴ� �Լ�	

	BOOL	ChangeMap(SI16 MapIndex, SI16 siX, SI16 siY);					// ���ο� ���� �ε��ϰ� �� �������� �̵�
	VOID	SetXY(SI16 siX, SI16 siY);										// ���� ���� X Y ��ġ�� �����Ѵ�.
	VOID	SetX(SI16 siX);													// ���� ���� X ��ġ�� �����Ѵ�.
	VOID	SetY(SI16 siY);													// ���� ���� Y ��ġ�� �����Ѵ�.
	SI16	GetRX() { return m_siRX; }										// ���� ���� X ������ ���´�.	
	SI16	GetRY() { return m_siRY; }										// ���� ���� Y ������ ���´�.

	SI32	GetObjectID		(SI16 siTileX, SI16 siTileY);		// �ʿ� �����Ǿ� �ִ� ������Ʈ�� ID�� ���´�.
	SI32	GetCharID		(SI16 siTileX, SI16 siTileY);		// �ʿ� �����Ǿ� �ִ� ĳ������ ID�� ���´�.
	UI32	GetItemID		(SI16 siTileX, SI16 siTileY);		// �ʿ� �����Ǿ� �ִ� �������� ID�� ���´�.
	UI16	GetItemQuantity	(SI16 siTileX, SI16 siTileY);		// �ʿ� �����Ǿ� �ִ� �������� ���� ���´�.

	VOID	PreProcessorBeforeLoadUnloadedTile(SI32 siDirection = 0);						// m_siX�� m_siY�� �����ϰ� LoadUnloadedTile�� ȣ���Ϸ��� �Ҷ� m_siX�� m_siY�� �����ϱ� ���� ȣ���� �־���Ѵ�
	BOOL	LoadUnloadedTile(SI32 siDirection = 0);											// �ε�Ǿ����� ���� Ÿ���� �ε��Ѵ�.		

	VOID	MakeMapFile(SI32 siDirection);	

	VOID	CreateDrawTable();

	SI32	siTileNo;
	SI32	siDrawOrder[((ON_MAP_VIEW_XSIZE_HALF)*2+1)*((ON_MAP_VIEW_YSIZE_HALF)*2+1)*2]; //���� ���̺�

	BOOL	GetRoadSpeed(SI16 siTileX, SI16 siTileY, SI32 *psiAddMoveSpeed);			// ���� ��� �߰� �̵� �ӵ��� ���Ѵ�.
	SI08	GetWaterFlag(SI16 siTileX, SI16 siTileY);									// Tile�� �� �Ӽ��� ���ؿ´�.
	SI08	GetWaterFlag(strTI* lpTileData);											// Tile�� �� �Ӽ��� ���ؿ´�.
	SI32	GetPKMapFlag()	{ return m_MapInfo.fWarFlag; };

	BOOL	GetPointTo(LPPOINT pptMouse, SI32 *psiCharID, SI32 *psiItemPos, SI32 *psiObjectID);		// ���콺�� ����Ű�� ĳ������ ID�� ���´�.	
	
	POINT	SearchMap(SI32 siCX, SI32 siCY, SI32 siSearchInfo, SI32 siParam1  = 0);
	inline	VOID	AnalysisObjectInfo(SI16 siTileX, SI16 siTileY, UI16 uiObjectInfo);				// ������Ʈ�� ������ �м��Ͽ� ó�����ش�.	

	VOID	PrepareHeader(LoadSaveMapHeader *pHeader);									// ����� ����Ÿ�� �ִ´�.
	MapStoreHeader*	GetMapStoreParser(SI16 siX, SI16 siY, SI16* lpMPointFlag);			// Map Store ������ ��´�.

	inline	VOID	SetObjectID(SI16 siTileX, SI16 siTileY, UI16 uiObjectID);			// Ư�� ��ġ�� 1x1 ������Ʈ�� �ɴ´�.
	inline	VOID	SetObjectID2(SI16 siTileX, SI16 siTileY, UI16 uiObjectID);			// Ư�� ��ġ�� ������Ʈ�� �ɴ´�.
	
	inline	BOOL	TransformMapXYToCharMapXY(SI32 siMapX, SI32 siMapY, SI32 &siCharMapX, SI32 &siCharMapY);			// Ÿ�� ��ǥ�� ĳ���͸��� ����ǥ�� ��ȯ�Ѵ�.
	inline	BOOL	TransformMapXYToCharMapXYNonClipping(SI32 siMapX, SI32 siMapY, SI32 &siCharMapX, SI32 &siCharMapY);	// Ÿ�� ��ǥ�� ĳ���͸��� ����ǥ�� ��ȯ�Ѵ�.

	SI32	GetWarFieldMap(void);														// Get War Field Map
	BOOL	CheckPKFieldMap(void);														// ���� FieldMap�� PK�� �������� �˻�
	BOOL	CheckPKFieldMap(SI16 siMapIndex);											// ���� FieldMap�� PK�� �������� �˻�

	void	VMercenaryBufferClear(void);
	void	VMercenaryAdd(SI16 x, SI16 y, SI16 siCharID);
	void	VMercenaryDelete(SI16 x, SI16 y, SI16 siCharID);
	SI16	VMercenaryGetCount(SI16 x, SI16 y);
	void	SetAnimation(SI32 siX, SI32 siY, UI08 MaxFrame, CHAR *szPath, SI08 count = 1, BOOL repeat = FALSE,bool bThunder = TRUE);
	VOID	AnimationDraw(LPDIRECTDRAWSURFACE7 pSurface);
	VOID	BoothMapDraw(SI16 siTileX, SI16 siTileY, POINT &ptMouse, LPDIRECTDRAWSURFACE7 pSurface);

	BOOL	IsInCharMapArea(SI32 siX, SI32 siY)								// �ش� Map X,Y�� ĳ���� Map�� ũ�⸦ ����� �ʴ��� �˻��Ѵ�.
	{
		if((siX < max(0, m_siX - MAX_ONLINE_CHAR_MAP_HALF_XSIZE)) || (siY < max(0, m_siY - MAX_ONLINE_CHAR_MAP_HALF_YSIZE)) ||
			(siX > min(m_siXsize - 1, m_siX + MAX_ONLINE_CHAR_MAP_HALF_XSIZE)) || (siY > min(m_siYsize - 1, m_siY + MAX_ONLINE_CHAR_MAP_HALF_YSIZE)))
			return FALSE;

		return TRUE;
	}
};

 
#endif
