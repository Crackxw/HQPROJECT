/********************************************************************************************
	파일명	:	OnlineMiniMap.h
	작성자	:	정 진 욱
	담당자	:	이 준 석
	작성일	:	2001.07.20
	수정일	:	2002.01.05(Mini Map의 기능과 출력이 모두 변경되어 새로 작성함)
********************************************************************************************/

#ifndef	__ON_MINIMAP_H_
#define	__ON_MINIMAP_H_

#include "Online.h"
#include "OnlineVillageSave.h"

#define		MINIMAP_SIZE_MAX_WIDTH		606
#define		MINIMAP_SIZE_MAX_HEIGHT		412
#define		MINIMAP_SIZE_MIN_WIDTH		186
#define		MINIMAP_SIZE_MIN_HEIGHT		184
#define		TOT_STRUCTURE_NUM			19

#define		MINIMAP_LIST_MAX_LINE		19
#define		MINIMAP_SUB_LIST_MAX_LINE	12



enum
{   
	ON_MINIMAP_INIT = 0,
	ON_MINIMAP_VILLAGE,
	ON_MINIMAP_ITEM,
	ON_MINIMAP_STRUCT,
	ON_MINIMAP_DUNGEON,
	ON_MINIMAP_SHORTCUT,
	ON_MINIMAP_NPC,
	ON_MINIMAP_CHAR,
	ON_MINIMAP_WAIT,
	ON_MINIMAP_EXIT
};

enum
{   
	ON_MINIMAP_SUB_INIT,
	ON_MINIMAP_SUB_POLL,
	ON_MINIMAP_SUB_ITEMINFO
};


typedef struct
{
	char		m_TargetName[64];

	UI16		m_uiVillageCode;
	UI16		m_ItemCode;						// Item 일 경우만 사용
	UI16		m_StructCode;					// 건물일 경우만 사용
	UI16		m_CharKind;						// 찾는 캐릭터 타입

	SI32		m_PositionX;
	SI32		m_PositionY;
	UI16		m_Size;							// 마을 대(0), 중(1), 소(2)
	BOOL		m_IsStructure[TOT_STRUCTURE_NUM];// 해당 건물의 존재 여부( 알수없는건물, 훈련소, 시장, 전장, 병원, 주막, 부두, 
												 // 농장, 관청, 경매소, 무기상점, 음식상점, 약상점, 무기점, 갑주장, 목장, 공장, 장수양성소
	SI16		m_BattlePoint;
	BOOL		m_bQuestNPC;
	UI16		m_iDungeonCode;
} SMiniMapTargetData;

typedef struct
{
	char		m_TargetName[64];

	SI32		m_TargetX;
	SI32		m_TargetY;
	UI16		m_Size;							// 마을 대(0), 중(1), 소(2)
	BOOL		m_bWharf;						// 항구마을인지 여부
//	MONEY		m_BuyMoney;
//	MONEY		m_SellMoney;
} SMiniMapTargetDataV;

typedef struct
{
	SI32					m_siMaxCount;
	SMiniMapTargetData*		m_pList;
} SMiniMapTargetList;


class OnlineMiniMap
{
public:
	SI16					siNoFieldHandle;			// 마우스 선택방어 부분 Handle

private:	
	// 기본 정보
	SI32			m_siStatus, m_siSubStatus;			// 상태
	BOOL			m_bAction;
	SI32			m_siX, m_siY;
	SI32			m_siXsize, m_siYsize;

	BOOL			siFullMode;					// 현재 창 상태
	cltOnlineWorld	*pMyOnlineWorld;
	POINT			m_ptMouse;

	BOOL			bMouseDown;					// 마우스 버튼 상태
	BOOL			m_FanfLock;					// 마우스로 맵 스크롤 Lock
	SI32			m_FanXPosition;				// 마우스로 맵 스크롤 Position
	SI32			m_FanYPosition;				// 마우스로 맵 스크롤 Position
	int				m_LeftRight;				// 미니맵 위치가 왼쪽인가 오른쪽인가

	SI32			m_YetHeroXPos;
	SI32			m_YetHeroYPos;
	RECT			m_rtVillageHelp;

	// Image 정보(창을 띄우는데 필요한 Image)
	XSPR			VillageSpr;						// 0 : Small, 1 : Medium, 2 : Large Village
	XSPR			TargetSpr;
	XSPR			WharfVillageSpr, CharFaceSpr, DungeonSpr;	

	XSPR			m_ScrHUp, m_ScrHDown, m_ScrVUp, m_ScrVDown;
	XSPR			m_ScrHandle, m_ScrBack;

	XSPR			m_SmallBack, m_BigBack, MiniMapBorder;
	XSPR			m_BigMap, m_PlusToggle, m_MinusToggle;
	XSPR			m_ButMain, m_ButEnter, m_ButExit;
	XSPR			m_MagBase, m_Mag1, m_Mag2, m_Mag3;

	BYTE			*pImage;

	// 작은맵 버튼
	_Button			m_BBigmap, m_BSEnlarge, m_BSReduce;

	// 큰맵 버튼
	_Button			m_BVillage, m_BItem, m_BStruct, m_BChar;
	_Button			m_BDungeon, m_BShortCut, m_BNpc;			// 특수지형, 지름길, 임무수행원
	_Button			m_BCharCenter, m_BBEnlarge, m_BBReduce;
	_Button			m_BFind, m_BExit;
	_Button			m_BScrBut[4], m_BTargetScrBut[2];

	// Target 스크롤
	SI16			m_siScroll, m_siScrollPos, m_siSelect;
	RECT			m_rtScroll;

	
	// 아이템 정보관련 데이타
	SAVEFILE_DATAHEADER	*m_pVillageData;
	SI32			m_siSubX, m_siSubY;

	XSPR			m_SubBack, m_SubBackBorder, m_SubExit;
	XSPR			m_SubScrBack, m_SubScrHandle, m_SubScrUp, m_SubScrDown;

	SI16			m_siSubScroll, m_siSubScrollPos, m_siSubSelect;
	RECT			m_rtSubScroll;

	_Button			m_BSubExit, m_BSubScrUp, m_BSubScrDown;


	// Mini Map 정보 (Mini Map Image 에 관한 부분)
	LPDIRECTDRAWSURFACE7	MiniMap;					// 미니맵
	LPDIRECTDRAWSURFACE7	MiniMapBack;				// 미니맵 배경
	SI32					m_MiniMapWidth;
	SI32					m_MiniMapHeight;
	SI32					m_MapIndex;					// 현재 맵 Index

	// 현재 보여지는 Map 정보(현재 보여지는 MiniMap의 위치와 확대 비율)
	SI32					m_MPointX;					// 중앙 지점
	SI32					m_MPointY;					// 중앙 지점
	SI08					m_Percent;					// 확대 비율(1 ~ 9(1, 2, 3, 4, 5(기준), 6, 7, 8, 9))
	BOOL					m_MovePositionFlag;			// 중앙점 이동
	SI32					m_MovePositionX;			// 중앙점 이동
	SI32					m_MovePositionY;			// 중앙점 이동

	// 선택된 Village Target
	BOOL					m_SelectTargetVillageFlag;
	SI32					m_SelectTargetX;
	SI32					m_SelectTargetY;

	// 선택된 Item Target
	BOOL					m_SelectTargetItemFlag;
	SI32					m_SelectTargetItemCount;
	SMiniMapTargetDataV*	m_SelectTargetItemData;

	// 선택된 structure Target
	BOOL					m_SelectTargetstructFlag;
	SI32					m_SelectTargetstructCount;
	SMiniMapTargetData*		m_SelectTargetstructData;

	// 선택된 Character Target
	char					m_FindCharName[64];
	char					m_CharStatus[64];
//	SMiniMapTargetData*		m_SelectTargetCharData;

	// Target List(Target들 List)
	SI16					m_TargetType;
	SMiniMapTargetList		m_TargetList[5];

	SMiniMapTargetList      m_TargetTemp;

	// Target Find(Target 이름을 손으로 쳐서 찾을때)
	//OnlineEditControl*		m_pTargetFindEditBox;
	HOEC					m_hOecFindEdit;
	char					m_TargetFindEditBoxName[65];
	HWND					m_YetEditHWND;
	BOOL					m_YetEditHWNDEnable;
	SI16					m_YetEditLockShortKey;
	BOOL					m_bVillage;


public:
	OnlineMiniMap();
	~OnlineMiniMap();

	BOOL		IsActive();															// 지금 화성화 되어 있는가 알아 본다.
	VOID		Init(cltOnlineWorld *pOnlineWorld);									// 초기화
	VOID		Initialize();
	VOID		Free();

	BOOL		LoadMiniMap(CHAR* lpMiniMap, SI32 MapIndex);						// Load Mini Map
	VOID		Pool(BOOL bVillage = TRUE);															// 미니맵을 처리한다.
	VOID		Draw(LPDIRECTDRAWSURFACE7 pSurface);								// 미니맵을 출력한다.
	VOID		DrawMiniMap(LPDIRECTDRAWSURFACE7 pSurface, SI16 XPos, SI16 YPos, SI16 XSize, SI16 YSize);	// 미니맵만을 출력한다.
	VOID		SetActive(BOOL bActive);											// 활성화 시킬 것인가?
	BOOL		Warp(SI32 siX, SI32 siY);											// Warp

	BOOL		GetHeroPosition(SI32& xPos, SI32& yPos);							// 주인공 위치를 얻어옴

	BOOL		SetEditFocus(void);													// Edit Box로 Focus 이동
	BOOL		RemoveEditFocus(void);												// Edit Box Focus 해제

	BOOL		GetfEditBox(void);													// Edit Box를 사용하는지 Flag를 얻음
	BOOL		CheckScrollPercent(SI32 siXPos, SI32 siYPos, float& refXP, float& refYP);	// 맵 좌표로 미니맵의 스크롤비율을 구한다.
	BOOL		CheckScrollPercent(float fXP, float fYP, SI32& resiXPos, SI32& resiYPos);	// 스크롤 비율로 맵 좌표를 구한다.

	BOOL		CheckMiniMapScreen(POINT* lpPoint);									// 미니맵을 선택하고 있는지 검사
	

	BOOL		MakeTargetList(void);												// Make Target List
	BOOL		MakeCharTarget(UI16 status, UI16 charkind, UI16 PosX, UI16 PosY,
				UI16 MapID, UI16 VillageCode);	// Make Character Target
	BOOL		ChangePos(BOOL fFullMode);							// Change Pos
	BOOL		FindTargetItem(UI16 ItemCode);										// Find Target Item
	BOOL		FindTargetStructure(UI16 StructCode, BOOL bAllStruct = FALSE);		// Find Target Structure
	BOOL		FindDungeonStructure(UI16 DungeonCode);

	BOOL		MiniMap_AddView(void);												// 미니맵 확대
	BOOL		MiniMap_SubTractView(void);											// 미니맵 축소

	BOOL		ISFullMode();
	VOID		ChangeMode(BOOL FullMode);
	VOID		MovePosition();
	VOID		ActionMapScroll();
	VOID		ActionCommon();
	VOID		SelectTarget(SI16 siType);
	VOID		ActionTargetScroll(SI16 siType);
	VOID		ActionEditBox(SI16 siType);
	VOID		ButtonStatus(SI32 siPreStatus, SI32 siNextStatus);
	VOID		MinimapMove();
	VOID		ActionVillage();
	VOID		ActionSubItemScroll();
	VOID		DrawItemList(HDC hdc);

private:
	BOOL		IsInsideMinimapCircle( POINT* pPoint );
};
	

#endif