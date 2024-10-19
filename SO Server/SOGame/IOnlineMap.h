#ifndef	ISOMAP_H
#define ISOMAP_H

#include "SOMap.h"

class	ISOMap
{
private:
	static		SOWorld		*pMyWorld;
	SOMap		*pMap;
	SI32		siTotalMapNum;
	DWORD		dwSentCharMoveMsgTime;										// 캐릭터의 이동 메시지를 보냈던 시간.
protected:
	inline	BOOL	IsInitialize();											// 초기화가 되었는가?
public:
	ISOMap();
	~ISOMap();

	BOOL	Init();															// 초기화.
	VOID	Free();															// 해제.


	// 맵에 ID를 설정한다.	
//	BOOL	SetID(UI16 uiMapID, SI32 siX, SI32 siY, BaseChar *pBaseChar, UI16 uiItemID, UI16 uiItemQuantity, UI16 uiCharMoveableTileInfo);
//	VOID	DelID(UI16 uiMapID, SI32 siX, SI32 siY, SI32 siDelInfo);					// 맵에서 ID를 삭제한다.
	BOOL	SetChar(BaseChar *pBaseChar, UI32 uiCharMoveableTileInfo);												// 캐릭터를 설정한다.
	BOOL	MoveChar(BaseChar *pBaseChar, SI32 siX, SI32 siY,BOOL bWarUnit, UI32 uiCharMoveableTileInfo);			// 캐릭터를 이동 시킨다.	
	BOOL	DelChar(BaseChar *pBaseChar);																			// 캐릭터를 삭제한다.
	BOOL	SetItem( UI16 uiMapID, SI32 siX, SI32 siY, UI16 uiItemID, UI16 uiItemQuantity, BOOL bSendToDB = TRUE );	// 아이템을 맵에 설정한다.
	BOOL	DelItem( SOPlayer *pPlayer, UI16 uiMapID, SI32 siX, SI32 siY, BOOL bSendToDB = TRUE );					// 아이템을 맵에서 제거한다.
	VOID	GetID(UI16 uiMapID, SI32 siSX, SI32 siSY, SI32 siEX, SI32 siEY, UI16 *pCharUniIDs, 
					OnlineReceiveFieldItemInfo *pFieldItemInfo, 
					UI16 *psiCharNum, UI16 *psiItemNum, SI32 siGetCharInfo, UI16 uiExceptionCharID = 0);
	UI32	GetCharUniID(UI16 uiMapID, SI32 siX, SI32 siY, SI32 siGetCharInfo);							// 맵에서 캐릭터의 ID를 얻어온다.

	// 일정 영역에 있는 캐릭터들의 ID를 얻어온다.
	SI32	GetCharUniID(UI16 uiMapID, SI32 siSX, SI32 siSY, SI32 siEX, SI32 siEY, UI16 *pCharUniIDs, UI16 uiMaxGetCharIDNum, SI32 siGetCharInfo);
	// 일정 영역에 있는 아이템의 ID를 얻어온다.
	SI32	GetItemID(UI16 uiMapID, SI32 siSX, SI32 siSY, SI32 siEX, SI32 siEY, UI16 *pItemIDs, UI16 *pItemQuantity);
	BOOL	GetItemID(UI16 uiMapID, SI32 siX, SI32 siY, UI16 *puiItemID, UI16 *pItemQuantity);			// 한 타일에 설정되어 있는 아이템의 정보를 얻어온다.
	BOOL	SetMap(UI16 uiMapID, LoadSaveMapTI* pMap);								// 맵에 속성을 설정해준다.
	POINT	SearchMap(UI16 uiMapID, SI32 siCX, SI32 siCY, SI32 siSearchInfo, SI32 siParam1, SI32 siParam2 = 0 );		// 맵에서 특정 좌표를 찾는다.
	VOID	SetItemFromDB();														// DB로부터 필드에 떨어져 있는 아이템의 정보를 얻어와서 맵에 설정한다.
	BOOL	IsValidMapID(UI16 uiMapID);												// 유효한 맵 아이디 인지 본다.
	SIZE	GetMapSize(UI16 uiMapID);												// 맵의 사이즈를 구한다.	
	UI16	GetVillageCode(UI16 uiMapID);											// 관련된 마을의 Index Code를 얻어온다.
	BOOL	IsColl(UI16 uiMapID, SI32 siX, SI32 siY);								// 충돌 타일인지 알아온다.
	BOOL	IsColl(UI16 uiMapID, SI32 siX, SI32 siY, SI32 siCharMoveableTileAtb);	// 충돌 타일인지 알아온다.(특정 캐릭터 입장에서)	
	BOOL	IsDungeon(UI16 uiMapID);												// 던전인가?
	BOOL	CanBeRainy( UI16 uiMapID );												// 비가 올 수 있는 지역인가?
	BOOL	CanBeSnowy( UI16 uiMapID );												// 눈이 올 수 있는 지역인가?
	
	SI32	GetNearSectorPlayerList(UI16 uiMapID, SI32 siX, SI32 siY, UI16 *pCharUniIDs, SI32 siGetCharInfo);		// X, Y를 기준으로 주위 섹터의 플레이어 ID를 얻어온다.
	SI32	GetNearSectorItemList(UI16 uiMapID, SI32 siX, SI32 siY, OnlineReceiveFieldItemInfo *pFieldItemInfo);	// X, Y를 기준으로 주위 섹터의 아이템 정보를 얻어온다.

	SI32	GetSectorItemList( UI16 uiMapID, strSector **pSector, OnlineReceiveFieldItemInfo *pFieldItemInfo );
	SI32	GetSectorPlayerList( UI16 uiMapID, strSector **pSector, UI16 *pCharUniIDs, SI32 siGetCharInfo );

	VOID	Poll();																	// Polling

	BOOL	IsWarFieldMap(UI16 uiMapID);											// 무도장인지 알아온다.
	BOOL	GetTileInfo(UI16 uiMapID, SI32 siX, SI32 siY, CHAR *pszBuffer);			// 타일의 정보를 얻어온다.

	static	VOID	InitStaticVariable(SOWorld *pWorld);							// 정적 변수를 초기화한다.
};

#endif
