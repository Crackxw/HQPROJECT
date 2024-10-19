/********************************************************************************************
	���ϸ�	:	OnlineMiniMap.h
	�ۼ���	:	�� �� ��
	�����	:	�� �� ��
	�ۼ���	:	2001.07.20
	������	:	2002.01.05(Mini Map�� ��ɰ� ����� ��� ����Ǿ� ���� �ۼ���)
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
	UI16		m_ItemCode;						// Item �� ��츸 ���
	UI16		m_StructCode;					// �ǹ��� ��츸 ���
	UI16		m_CharKind;						// ã�� ĳ���� Ÿ��

	SI32		m_PositionX;
	SI32		m_PositionY;
	UI16		m_Size;							// ���� ��(0), ��(1), ��(2)
	BOOL		m_IsStructure[TOT_STRUCTURE_NUM];// �ش� �ǹ��� ���� ����( �˼����°ǹ�, �Ʒü�, ����, ����, ����, �ָ�, �ε�, 
												 // ����, ��û, ��ż�, �������, ���Ļ���, �����, ������, ������, ����, ����, ����缺��
	SI16		m_BattlePoint;
	BOOL		m_bQuestNPC;
	UI16		m_iDungeonCode;
} SMiniMapTargetData;

typedef struct
{
	char		m_TargetName[64];

	SI32		m_TargetX;
	SI32		m_TargetY;
	UI16		m_Size;							// ���� ��(0), ��(1), ��(2)
	BOOL		m_bWharf;						// �ױ��������� ����
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
	SI16					siNoFieldHandle;			// ���콺 ���ù�� �κ� Handle

private:	
	// �⺻ ����
	SI32			m_siStatus, m_siSubStatus;			// ����
	BOOL			m_bAction;
	SI32			m_siX, m_siY;
	SI32			m_siXsize, m_siYsize;

	BOOL			siFullMode;					// ���� â ����
	cltOnlineWorld	*pMyOnlineWorld;
	POINT			m_ptMouse;

	BOOL			bMouseDown;					// ���콺 ��ư ����
	BOOL			m_FanfLock;					// ���콺�� �� ��ũ�� Lock
	SI32			m_FanXPosition;				// ���콺�� �� ��ũ�� Position
	SI32			m_FanYPosition;				// ���콺�� �� ��ũ�� Position
	int				m_LeftRight;				// �̴ϸ� ��ġ�� �����ΰ� �������ΰ�

	SI32			m_YetHeroXPos;
	SI32			m_YetHeroYPos;
	RECT			m_rtVillageHelp;

	// Image ����(â�� ���µ� �ʿ��� Image)
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

	// ������ ��ư
	_Button			m_BBigmap, m_BSEnlarge, m_BSReduce;

	// ū�� ��ư
	_Button			m_BVillage, m_BItem, m_BStruct, m_BChar;
	_Button			m_BDungeon, m_BShortCut, m_BNpc;			// Ư������, ������, �ӹ������
	_Button			m_BCharCenter, m_BBEnlarge, m_BBReduce;
	_Button			m_BFind, m_BExit;
	_Button			m_BScrBut[4], m_BTargetScrBut[2];

	// Target ��ũ��
	SI16			m_siScroll, m_siScrollPos, m_siSelect;
	RECT			m_rtScroll;

	
	// ������ �������� ����Ÿ
	SAVEFILE_DATAHEADER	*m_pVillageData;
	SI32			m_siSubX, m_siSubY;

	XSPR			m_SubBack, m_SubBackBorder, m_SubExit;
	XSPR			m_SubScrBack, m_SubScrHandle, m_SubScrUp, m_SubScrDown;

	SI16			m_siSubScroll, m_siSubScrollPos, m_siSubSelect;
	RECT			m_rtSubScroll;

	_Button			m_BSubExit, m_BSubScrUp, m_BSubScrDown;


	// Mini Map ���� (Mini Map Image �� ���� �κ�)
	LPDIRECTDRAWSURFACE7	MiniMap;					// �̴ϸ�
	LPDIRECTDRAWSURFACE7	MiniMapBack;				// �̴ϸ� ���
	SI32					m_MiniMapWidth;
	SI32					m_MiniMapHeight;
	SI32					m_MapIndex;					// ���� �� Index

	// ���� �������� Map ����(���� �������� MiniMap�� ��ġ�� Ȯ�� ����)
	SI32					m_MPointX;					// �߾� ����
	SI32					m_MPointY;					// �߾� ����
	SI08					m_Percent;					// Ȯ�� ����(1 ~ 9(1, 2, 3, 4, 5(����), 6, 7, 8, 9))
	BOOL					m_MovePositionFlag;			// �߾��� �̵�
	SI32					m_MovePositionX;			// �߾��� �̵�
	SI32					m_MovePositionY;			// �߾��� �̵�

	// ���õ� Village Target
	BOOL					m_SelectTargetVillageFlag;
	SI32					m_SelectTargetX;
	SI32					m_SelectTargetY;

	// ���õ� Item Target
	BOOL					m_SelectTargetItemFlag;
	SI32					m_SelectTargetItemCount;
	SMiniMapTargetDataV*	m_SelectTargetItemData;

	// ���õ� structure Target
	BOOL					m_SelectTargetstructFlag;
	SI32					m_SelectTargetstructCount;
	SMiniMapTargetData*		m_SelectTargetstructData;

	// ���õ� Character Target
	char					m_FindCharName[64];
	char					m_CharStatus[64];
//	SMiniMapTargetData*		m_SelectTargetCharData;

	// Target List(Target�� List)
	SI16					m_TargetType;
	SMiniMapTargetList		m_TargetList[5];

	SMiniMapTargetList      m_TargetTemp;

	// Target Find(Target �̸��� ������ �ļ� ã����)
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

	BOOL		IsActive();															// ���� ȭ��ȭ �Ǿ� �ִ°� �˾� ����.
	VOID		Init(cltOnlineWorld *pOnlineWorld);									// �ʱ�ȭ
	VOID		Initialize();
	VOID		Free();

	BOOL		LoadMiniMap(CHAR* lpMiniMap, SI32 MapIndex);						// Load Mini Map
	VOID		Pool(BOOL bVillage = TRUE);															// �̴ϸ��� ó���Ѵ�.
	VOID		Draw(LPDIRECTDRAWSURFACE7 pSurface);								// �̴ϸ��� ����Ѵ�.
	VOID		DrawMiniMap(LPDIRECTDRAWSURFACE7 pSurface, SI16 XPos, SI16 YPos, SI16 XSize, SI16 YSize);	// �̴ϸʸ��� ����Ѵ�.
	VOID		SetActive(BOOL bActive);											// Ȱ��ȭ ��ų ���ΰ�?
	BOOL		Warp(SI32 siX, SI32 siY);											// Warp

	BOOL		GetHeroPosition(SI32& xPos, SI32& yPos);							// ���ΰ� ��ġ�� ����

	BOOL		SetEditFocus(void);													// Edit Box�� Focus �̵�
	BOOL		RemoveEditFocus(void);												// Edit Box Focus ����

	BOOL		GetfEditBox(void);													// Edit Box�� ����ϴ��� Flag�� ����
	BOOL		CheckScrollPercent(SI32 siXPos, SI32 siYPos, float& refXP, float& refYP);	// �� ��ǥ�� �̴ϸ��� ��ũ�Ѻ����� ���Ѵ�.
	BOOL		CheckScrollPercent(float fXP, float fYP, SI32& resiXPos, SI32& resiYPos);	// ��ũ�� ������ �� ��ǥ�� ���Ѵ�.

	BOOL		CheckMiniMapScreen(POINT* lpPoint);									// �̴ϸ��� �����ϰ� �ִ��� �˻�
	

	BOOL		MakeTargetList(void);												// Make Target List
	BOOL		MakeCharTarget(UI16 status, UI16 charkind, UI16 PosX, UI16 PosY,
				UI16 MapID, UI16 VillageCode);	// Make Character Target
	BOOL		ChangePos(BOOL fFullMode);							// Change Pos
	BOOL		FindTargetItem(UI16 ItemCode);										// Find Target Item
	BOOL		FindTargetStructure(UI16 StructCode, BOOL bAllStruct = FALSE);		// Find Target Structure
	BOOL		FindDungeonStructure(UI16 DungeonCode);

	BOOL		MiniMap_AddView(void);												// �̴ϸ� Ȯ��
	BOOL		MiniMap_SubTractView(void);											// �̴ϸ� ���

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