#ifndef	SOMAP_H
#define SOMAP_H

#include "SOCommonHeader.h"
#include "OnlineBaseMap.h"
#include "OnlineCommonStruct.h"
#include "OnlineMapIndex-Parser.h"
#include "Stack.h"
#include "SOBaseChar.h"
#include "..\\XCommon\\JXList.h"
#include "SOPlayer.h"


// �¶��� Ÿ�� �Ӽ�
//#define	ON_TILE_ATB_COLL		1			// �浹 �Ӽ�		<-- �� �κ��� �̹� OnlineBaseMap.h���� �����ߴ�.
#define	ON_TILE_ATB_VILLAGE			2

#define	ON_MAP_SI_CHAR				1
#define	ON_MAP_SI_ITEM				2

#define ON_MAP_CHARID_PLAYER		1
#define	ON_MAP_CHARID_MONSTER		2
#define	ON_MAP_CHARID_SHIP			4
#define	ON_MAP_CHARID_PLAYERINSHIP	8
#define	ON_MAP_CHARID_ALLPLAYER		(ON_MAP_CHARID_PLAYER | ON_MAP_CHARID_PLAYERINSHIP)
#define	ON_MAP_CHARID_FIELD			(ON_MAP_CHARID_PLAYER | ON_MAP_CHARID_SHIP | ON_MAP_CHARID_MONSTER)
#define	ON_MAP_CHARID_ALL			(ON_MAP_CHARID_PLAYER | ON_MAP_CHARID_MONSTER | ON_MAP_CHARID_SHIP | ON_MAP_CHARID_PLAYERINSHIP)

#define	ON_TILENUM_PER_SECTOR		14

#define	ON_MAX_ITEMNUM_ON_TILE		30000		// �� Ÿ�Ͽ� ���� �� �ִ� �ִ� �������� ��.

struct	strTI
{
	UI16	uiItemID;					// �������� ID
	UI16	uiItemQuantity;				// �������� ����.	
	UI32	uiTileKind;					// Ÿ���� ����.	
	UI16	uiCharUniID;				// ĳ������ UniqueID

	UI16	uiX, uiY;					// �� Ÿ���� X, Y ��ǥ

	UI08	uiTileAtb;					// Ÿ���� �Ӽ�.
};

class cSectorNode;
class cSectorList;

struct	strSector 
{
	UI16			uiX;
	UI16			uiY;
	SStack<UI16>	PlayerCharList;				// �÷��̾��� ����Ʈ.
	SStack<UI16>	MonsterCharList;			// ������ ����Ʈ.
	SStack<UI16>	ShipCharList;				// ���� ����Ʈ.
	SStack<strTI*>	ItemList;					// ������ ����Ʈ
	strSector		*pNearSector[9];			// ������ ���� ������. 8�����̴�. 
												// 9��°���� �ڱ� �ڽ��� ���� �����Ͱ� ��..	
	cSectorNode		*pSectorNode;				// �ڽ��� ������ �ִ� ����Ʈ�� ������
	int				temp;						// �ӽ÷� ����� ����

};


class cSectorNode : public JXObject
{
public:
	cSectorNode() { m_pSector = NULL; };
	virtual ~cSectorNode() {};

public:
	strSector		*m_pSector;
};


class cSectorList : public JXList
{
public:
	cSectorList() {};
	virtual ~cSectorList() {};

public:


};


class	SOMap : public OnlineBaseMap
{
private:		
	static		SOWorld*	pMyWorld;					// World

	strTI		*pstMap;
	UI32		*puiMultiplyMapYOff;

	SI32		siHorizonSectorNum;			// ���� ���� ��
	SI32		siVerticalSectorNum;		// ���� ���� ��
	strSector	*m_pstSector;				// ����

	BOOL		bUpdateDBSwitch;	// DB�� ������Ʈ �� ���ΰ�?
	
	UI16		uiMapID;			// Map�� ID
	SIZE		szMapSize;			// Map�� Size	
	SI16		siMapType;			// Map�� Ÿ��.
	UI16		uiVillageCode;		// Map�� ���õ� ����.
	BOOL		bIsFieldMap;		// ���� �ʵ��
	SI32		siWeather;			// ���� ����
	
	cSectorNode	*m_pSectorNode;
	cSectorList	m_sectorlist;

	// ���� ������ ���..
	UI16						uiAddDelCharsID[ ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE ];
	OnlineReceiveFieldItemInfo	FieldItemInfo[ ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE ];
	UI16						uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];
	UI16						uiMovedChars[1000];		// �ѹ��� �ִ� 1000�� �̵��� �� �ִ�.


private:
	VOID	SetCharID(SI32 siX, SI32 siY, BaseChar *pBaseChar);												// �ܼ��� ĳ������ ID�� �ʿ� ������ ���ش�.
	VOID	DelCharID(SI32 siX, SI32 siY);																	// �ܼ��� ĳ������ ID�� �ʿ��� ������ �ش�.
	VOID	SetItemID( SI32 siX, SI32 siY, UI16 uiItemID, UI16 uiItemQuantity, BOOL bSendToDB = TRUE );		// �ܼ��� �ʿ� �������� ������ ���ش�.
	VOID	DelItemID( SOPlayer *pPlayer, SI32 siX, SI32 siY, BOOL bSendToDB = TRUE );						// �ܼ��� �ʿ� �������� ���Ÿ� ���ش�.

	BOOL	IsPlayerInNearSector(SI32 siArrayIndex);													// �ֺ� ���Ϳ� �÷��̾ �ִ� ���Ͱ� �ִ��� �˾ƿ´�.
	BOOL	GetPosByPointer(strTI *pstTile, SI32 *psiX, SI32 *psiY);									// Ÿ���� �����͸� ����Ͽ� �� Ÿ���� X, Y ��ǥ�� ���´�.

	BOOL	IsNearSector( SI32 siSector1, SI32 siSector2 );													// siSector2�� siSector1�� ���� �������� �˾ƿ´�.

public:
	SOMap();						// ������.
	~SOMap();						// �Ҹ���.

	BOOL	Init(MapIndexHeader *pMapIndexHeader);														// �ʱ�ȭ.
	VOID	Free();																						// ����.

	// �ʿ� ID�� �����Ѵ�.	
	BOOL	SetChar(BaseChar *pBaseChar, UI32 uiCharMoveableTileInfo);									// ĳ���͸� �����Ѵ�.
	BOOL	MoveChar(BaseChar *pBaseChar, SI32 siX, SI32 siY, BOOL bWarUnit,UI32 uiCharMoveableTileInfo);				// ĳ���͸� �̵� ��Ų��.	
	BOOL	DelChar(BaseChar *pBaseChar);																// ĳ���͸� �����Ѵ�.

	BOOL	SetItem( SI32 siX, SI32 siY, UI16 uiItemID, UI16 uiItemQuantity, BOOL bSendToDB = TRUE );	// �������� �ʿ� �����Ѵ�.
	BOOL	DelItem( SOPlayer *pPlayer, SI32 siX, SI32 siY, BOOL bSendToDB = TRUE );					// �������� �ʿ��� �����Ѵ�.
	//BOOL	SetID(SI32 siX, SI32 siY, BaseChar *pBaseChar, UI16 uiItemID, UI16 uiItemQuantity, UI32 uiCharMoveableTileInfo);	
	//VOID	DelID(SI32 siX, SI32 siY, SI32 siDelInfo);					// �ʿ��� ID�� �����Ѵ�.	

	VOID	GetID(SI32 siSX, SI32 siSY, SI32 siEX, SI32 siEY, UI16 *pCharUniIDs, 
					OnlineReceiveFieldItemInfo *pFieldItemInfo, 
					UI16 *psiCharNum, UI16 *psiItemNum, SI32 siGetCharInfo, UI16 uiExceptionCharID = 0);
	UI32	GetCharUniID(SI32 siX, SI32 siY, SI32 siGetCharInfo);							// �ʿ��� ĳ������ ID�� ���´�.

	// ���� ������ �ִ� ĳ���͵��� ID�� ���´�.
	SI32	GetCharUniID(SI32 siSX, SI32 siSY, SI32 siEX, SI32 siEY, UI16 *pCharUniIDs, UI16 uiMaxGetCharIDNum, SI32 siGetCharInfo);
	// ���� ������ �ִ� �������� ID�� ���´�.
	SI32	GetItemID(SI32 siSX, SI32 siSY, SI32 siEX, SI32 siEY, UI16 *pItemIDs, UI16 *pItemQuantity);
	BOOL	GetItemID(SI32 siX, SI32 siY, UI16 *pItemIDs, UI16 *pItemQuantity);			// �� Ÿ�Ͽ� �����Ǿ� �ִ� �������� ������ ���´�.
	BOOL	SetMap(LoadSaveMapTI* pMap);												// �ʿ� �Ӽ��� �������ش�.
	POINT	SearchMap(SI32 siCX, SI32 siCY, SI32 siSearchInfo, SI32 siParam1, SI32 siParam2 );			// �ʿ��� Ư�� ��ǥ�� ã�´�.
	VOID	SetItemFromDB();											// DB�κ��� �ʵ忡 ������ �ִ� �������� ������ ���ͼ� �ʿ� �����Ѵ�.
	SIZE	GetMapSize();												// ���� ����� ���Ѵ�.
	UI16	GetVillageCode();											// ���õ� ���� �ڵ带 ���Ѵ�.
	BOOL	IsColl(SI32 siX, SI32 siY);									// �浹 Ÿ������ �˾ƿ´�.
	BOOL	IsColl(SI32 siX, SI32 siY, SI32 siCharMoveableTileAtb);		// �浹 Ÿ������ �˾ƿ´�.(Ư�� ĳ���� ���忡��)	
	BOOL	IsDungeon();												// �����ΰ�?
	BOOL	CanBeRainy();												// �� ���� �� �ִ� �����ΰ�?
	BOOL	CanBeSnowy();												// ���� ���� �� �ִ� �����ΰ�?
	BOOL	InitSector(SI32 siArrayIndex);								// ���͸� �ʱ�ȭ �Ѵ�.
	VOID	FreeSector(SI32 siArrayIndex);								// ���͸� �����Ѵ�.
	SI32	GetSectorIndex(SI32 siX, SI32 siY);							// ������ Index�� ���Ѵ�.
	BOOL	AddCharToSector(SI32 siX, SI32 siY, BaseChar *pBaseChar);	// ���Ϳ� �÷��̾ �߰� ��Ų��.
	BOOL	AddCharToSector(SI32 siArrayIndex, BaseChar *pBaseChar);	// ���Ϳ� �÷��̾ �߰� ��Ų��.
	VOID	DelCharToSector(SI32 siX, SI32 siY, BaseChar *pBaseChar);	// ���Ϳ��� �÷��̾ �����Ѵ�.
	VOID	DelCharToSector(SI32 siArrayIndex, BaseChar *pBaseChar);	// ���Ϳ��� �÷��̾ �����Ѵ�.
	VOID	AddItemToSector(SI32 siX, SI32 siY, strTI *pstTile);		// ���Ϳ��� �������� �ִ� Ÿ���� �߰��Ѵ�.
	VOID	DelItemToSector(SI32 siX, SI32 siY, strTI *pstTile);		// ���Ϳ��� �������� �ִ� Ÿ���� �����Ѵ�.
	SI32	GetNearSectorPlayerList(SI32 siX, SI32 siY, UI16 *pCharUniIDs, SI32 siGetCharInfo);		// X, Y�� �������� ���� ������ �÷��̾� ID�� ���´�.
	SI32	GetNearSectorItemList(SI32 siX, SI32 siY, OnlineReceiveFieldItemInfo *pFieldItemInfo);	// X, Y�� �������� ���� ������ ������ ������ ���´�.		

	SI32	GetSectorItemList( strSector **pSector, OnlineReceiveFieldItemInfo *pFieldItemInfo );
	SI32	GetSectorPlayerList( strSector **pSector, UI16 *pCharUniIDs, SI32 siGetCharInfo );

	VOID	Poll(BOOL bSendMoveMsg);									// Polling

	inline	VOID	CorrectionPos(SI32 *psiSX, SI32 *psiSY, SI32 *psiEX, SI32 *psiEY);	// ��ǥ�� �������ش�.	

	void    GetNewSectorList( SI32 siOldIndex, SI32 siNewIndex, strSector **pAddSector, strSector **pDelSector );
	void    PrintSectorList( strSector **pSector );

	BOOL	IsValidSectorIndex( SI32 siArrayIndex ){ return ( siArrayIndex >= 0 && siArrayIndex < siHorizonSectorNum * siVerticalSectorNum ); };

	BOOL	IsWarFieldMap();														// ���������� �˾ƿ´�.
	BOOL	GetTileInfo(SI32 siX, SI32 siY, CHAR *pszBuffer);						// Ÿ���� ������ ���´�.

	UI16	GetMapID();																// ���� ID�� ���´�.

	static	VOID	InitStaticVariable(SOWorld *pWorld);							// ���� ������ �ʱ�ȭ�Ѵ�.
};

#endif