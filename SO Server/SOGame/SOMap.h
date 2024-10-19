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


// 온라인 타일 속성
//#define	ON_TILE_ATB_COLL		1			// 충돌 속성		<-- 이 부분은 이미 OnlineBaseMap.h에서 선언했다.
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

#define	ON_MAX_ITEMNUM_ON_TILE		30000		// 한 타일에 있을 수 있는 최대 아이템의 수.

struct	strTI
{
	UI16	uiItemID;					// 아이템의 ID
	UI16	uiItemQuantity;				// 아이템의 개수.	
	UI32	uiTileKind;					// 타일의 종류.	
	UI16	uiCharUniID;				// 캐릭터의 UniqueID

	UI16	uiX, uiY;					// 이 타일의 X, Y 좌표

	UI08	uiTileAtb;					// 타일의 속성.
};

class cSectorNode;
class cSectorList;

struct	strSector 
{
	UI16			uiX;
	UI16			uiY;
	SStack<UI16>	PlayerCharList;				// 플레이어의 리스트.
	SStack<UI16>	MonsterCharList;			// 몬스터의 리스트.
	SStack<UI16>	ShipCharList;				// 배의 리스트.
	SStack<strTI*>	ItemList;					// 아이템 리스트
	strSector		*pNearSector[9];			// 주위의 섹터 포인터. 8방향이다. 
												// 9번째에는 자기 자신의 섹터 포인터가 들어감..	
	cSectorNode		*pSectorNode;				// 자신을 가지고 있는 리스트의 포인터
	int				temp;						// 임시로 사용할 변수

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

	SI32		siHorizonSectorNum;			// 가로 섹터 수
	SI32		siVerticalSectorNum;		// 세로 섹터 수
	strSector	*m_pstSector;				// 섹터

	BOOL		bUpdateDBSwitch;	// DB를 업데이트 할 것인가?
	
	UI16		uiMapID;			// Map의 ID
	SIZE		szMapSize;			// Map의 Size	
	SI16		siMapType;			// Map의 타입.
	UI16		uiVillageCode;		// Map에 관련된 마을.
	BOOL		bIsFieldMap;		// 전투 필드맵
	SI32		siWeather;			// 날씨 정보
	
	cSectorNode	*m_pSectorNode;
	cSectorList	m_sectorlist;

	// 지역 변수로 사용..
	UI16						uiAddDelCharsID[ ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE ];
	OnlineReceiveFieldItemInfo	FieldItemInfo[ ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE ];
	UI16						uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];
	UI16						uiMovedChars[1000];		// 한번에 최대 1000명씩 이동할 수 있다.


private:
	VOID	SetCharID(SI32 siX, SI32 siY, BaseChar *pBaseChar);												// 단순히 캐릭터의 ID를 맵에 설정만 해준다.
	VOID	DelCharID(SI32 siX, SI32 siY);																	// 단순히 캐릭터의 ID를 맵에서 삭제해 준다.
	VOID	SetItemID( SI32 siX, SI32 siY, UI16 uiItemID, UI16 uiItemQuantity, BOOL bSendToDB = TRUE );		// 단순히 맵에 아이템을 설정만 해준다.
	VOID	DelItemID( SOPlayer *pPlayer, SI32 siX, SI32 siY, BOOL bSendToDB = TRUE );						// 단순히 맵에 아이템을 제거만 해준다.

	BOOL	IsPlayerInNearSector(SI32 siArrayIndex);													// 주변 섹터에 플레이어가 있는 섹터가 있는지 알아온다.
	BOOL	GetPosByPointer(strTI *pstTile, SI32 *psiX, SI32 *psiY);									// 타일의 포인터를 사용하여 그 타일의 X, Y 좌표를 얻어온다.

	BOOL	IsNearSector( SI32 siSector1, SI32 siSector2 );													// siSector2가 siSector1의 주위 섹터인지 알아온다.

public:
	SOMap();						// 생성자.
	~SOMap();						// 소멸자.

	BOOL	Init(MapIndexHeader *pMapIndexHeader);														// 초기화.
	VOID	Free();																						// 해제.

	// 맵에 ID를 설정한다.	
	BOOL	SetChar(BaseChar *pBaseChar, UI32 uiCharMoveableTileInfo);									// 캐릭터를 설정한다.
	BOOL	MoveChar(BaseChar *pBaseChar, SI32 siX, SI32 siY, BOOL bWarUnit,UI32 uiCharMoveableTileInfo);				// 캐릭터를 이동 시킨다.	
	BOOL	DelChar(BaseChar *pBaseChar);																// 캐릭터를 삭제한다.

	BOOL	SetItem( SI32 siX, SI32 siY, UI16 uiItemID, UI16 uiItemQuantity, BOOL bSendToDB = TRUE );	// 아이템을 맵에 설정한다.
	BOOL	DelItem( SOPlayer *pPlayer, SI32 siX, SI32 siY, BOOL bSendToDB = TRUE );					// 아이템을 맵에서 제거한다.
	//BOOL	SetID(SI32 siX, SI32 siY, BaseChar *pBaseChar, UI16 uiItemID, UI16 uiItemQuantity, UI32 uiCharMoveableTileInfo);	
	//VOID	DelID(SI32 siX, SI32 siY, SI32 siDelInfo);					// 맵에서 ID를 삭제한다.	

	VOID	GetID(SI32 siSX, SI32 siSY, SI32 siEX, SI32 siEY, UI16 *pCharUniIDs, 
					OnlineReceiveFieldItemInfo *pFieldItemInfo, 
					UI16 *psiCharNum, UI16 *psiItemNum, SI32 siGetCharInfo, UI16 uiExceptionCharID = 0);
	UI32	GetCharUniID(SI32 siX, SI32 siY, SI32 siGetCharInfo);							// 맵에서 캐릭터의 ID를 얻어온다.

	// 일정 영역에 있는 캐릭터들의 ID를 얻어온다.
	SI32	GetCharUniID(SI32 siSX, SI32 siSY, SI32 siEX, SI32 siEY, UI16 *pCharUniIDs, UI16 uiMaxGetCharIDNum, SI32 siGetCharInfo);
	// 일정 영역에 있는 아이템의 ID를 얻어온다.
	SI32	GetItemID(SI32 siSX, SI32 siSY, SI32 siEX, SI32 siEY, UI16 *pItemIDs, UI16 *pItemQuantity);
	BOOL	GetItemID(SI32 siX, SI32 siY, UI16 *pItemIDs, UI16 *pItemQuantity);			// 한 타일에 설정되어 있는 아이템의 정보를 얻어온다.
	BOOL	SetMap(LoadSaveMapTI* pMap);												// 맵에 속성을 설정해준다.
	POINT	SearchMap(SI32 siCX, SI32 siCY, SI32 siSearchInfo, SI32 siParam1, SI32 siParam2 );			// 맵에서 특정 좌표를 찾는다.
	VOID	SetItemFromDB();											// DB로부터 필드에 떨어져 있는 아이템의 정보를 얻어와서 맵에 설정한다.
	SIZE	GetMapSize();												// 맵의 사이즈를 구한다.
	UI16	GetVillageCode();											// 관련된 마을 코드를 구한다.
	BOOL	IsColl(SI32 siX, SI32 siY);									// 충돌 타일인지 알아온다.
	BOOL	IsColl(SI32 siX, SI32 siY, SI32 siCharMoveableTileAtb);		// 충돌 타일인지 알아온다.(특정 캐릭터 입장에서)	
	BOOL	IsDungeon();												// 던전인가?
	BOOL	CanBeRainy();												// 비가 내릴 수 있는 지역인가?
	BOOL	CanBeSnowy();												// 눈이 내릴 수 있는 지역인가?
	BOOL	InitSector(SI32 siArrayIndex);								// 섹터를 초기화 한다.
	VOID	FreeSector(SI32 siArrayIndex);								// 섹터를 삭제한다.
	SI32	GetSectorIndex(SI32 siX, SI32 siY);							// 섹터의 Index를 구한다.
	BOOL	AddCharToSector(SI32 siX, SI32 siY, BaseChar *pBaseChar);	// 섹터에 플레이어를 추가 시킨다.
	BOOL	AddCharToSector(SI32 siArrayIndex, BaseChar *pBaseChar);	// 섹터에 플레이어를 추가 시킨다.
	VOID	DelCharToSector(SI32 siX, SI32 siY, BaseChar *pBaseChar);	// 섹터에서 플레이어를 삭제한다.
	VOID	DelCharToSector(SI32 siArrayIndex, BaseChar *pBaseChar);	// 섹터에서 플레이어를 삭제한다.
	VOID	AddItemToSector(SI32 siX, SI32 siY, strTI *pstTile);		// 섹터에서 아이템이 있는 타일을 추가한다.
	VOID	DelItemToSector(SI32 siX, SI32 siY, strTI *pstTile);		// 섹터에서 아이템이 있는 타일을 삭제한다.
	SI32	GetNearSectorPlayerList(SI32 siX, SI32 siY, UI16 *pCharUniIDs, SI32 siGetCharInfo);		// X, Y를 기준으로 주위 섹터의 플레이어 ID를 얻어온다.
	SI32	GetNearSectorItemList(SI32 siX, SI32 siY, OnlineReceiveFieldItemInfo *pFieldItemInfo);	// X, Y를 기준으로 주위 섹터의 아이템 정보를 얻어온다.		

	SI32	GetSectorItemList( strSector **pSector, OnlineReceiveFieldItemInfo *pFieldItemInfo );
	SI32	GetSectorPlayerList( strSector **pSector, UI16 *pCharUniIDs, SI32 siGetCharInfo );

	VOID	Poll(BOOL bSendMoveMsg);									// Polling

	inline	VOID	CorrectionPos(SI32 *psiSX, SI32 *psiSY, SI32 *psiEX, SI32 *psiEY);	// 좌표를 교정해준다.	

	void    GetNewSectorList( SI32 siOldIndex, SI32 siNewIndex, strSector **pAddSector, strSector **pDelSector );
	void    PrintSectorList( strSector **pSector );

	BOOL	IsValidSectorIndex( SI32 siArrayIndex ){ return ( siArrayIndex >= 0 && siArrayIndex < siHorizonSectorNum * siVerticalSectorNum ); };

	BOOL	IsWarFieldMap();														// 무도장인지 알아온다.
	BOOL	GetTileInfo(SI32 siX, SI32 siY, CHAR *pszBuffer);						// 타일의 정보를 얻어온다.

	UI16	GetMapID();																// 맵의 ID를 얻어온다.

	static	VOID	InitStaticVariable(SOWorld *pWorld);							// 정적 변수를 초기화한다.
};

#endif