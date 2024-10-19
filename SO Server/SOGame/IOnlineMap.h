#ifndef	ISOMAP_H
#define ISOMAP_H

#include "SOMap.h"

class	ISOMap
{
private:
	static		SOWorld		*pMyWorld;
	SOMap		*pMap;
	SI32		siTotalMapNum;
	DWORD		dwSentCharMoveMsgTime;										// ĳ������ �̵� �޽����� ���´� �ð�.
protected:
	inline	BOOL	IsInitialize();											// �ʱ�ȭ�� �Ǿ��°�?
public:
	ISOMap();
	~ISOMap();

	BOOL	Init();															// �ʱ�ȭ.
	VOID	Free();															// ����.


	// �ʿ� ID�� �����Ѵ�.	
//	BOOL	SetID(UI16 uiMapID, SI32 siX, SI32 siY, BaseChar *pBaseChar, UI16 uiItemID, UI16 uiItemQuantity, UI16 uiCharMoveableTileInfo);
//	VOID	DelID(UI16 uiMapID, SI32 siX, SI32 siY, SI32 siDelInfo);					// �ʿ��� ID�� �����Ѵ�.
	BOOL	SetChar(BaseChar *pBaseChar, UI32 uiCharMoveableTileInfo);												// ĳ���͸� �����Ѵ�.
	BOOL	MoveChar(BaseChar *pBaseChar, SI32 siX, SI32 siY,BOOL bWarUnit, UI32 uiCharMoveableTileInfo);			// ĳ���͸� �̵� ��Ų��.	
	BOOL	DelChar(BaseChar *pBaseChar);																			// ĳ���͸� �����Ѵ�.
	BOOL	SetItem( UI16 uiMapID, SI32 siX, SI32 siY, UI16 uiItemID, UI16 uiItemQuantity, BOOL bSendToDB = TRUE );	// �������� �ʿ� �����Ѵ�.
	BOOL	DelItem( SOPlayer *pPlayer, UI16 uiMapID, SI32 siX, SI32 siY, BOOL bSendToDB = TRUE );					// �������� �ʿ��� �����Ѵ�.
	VOID	GetID(UI16 uiMapID, SI32 siSX, SI32 siSY, SI32 siEX, SI32 siEY, UI16 *pCharUniIDs, 
					OnlineReceiveFieldItemInfo *pFieldItemInfo, 
					UI16 *psiCharNum, UI16 *psiItemNum, SI32 siGetCharInfo, UI16 uiExceptionCharID = 0);
	UI32	GetCharUniID(UI16 uiMapID, SI32 siX, SI32 siY, SI32 siGetCharInfo);							// �ʿ��� ĳ������ ID�� ���´�.

	// ���� ������ �ִ� ĳ���͵��� ID�� ���´�.
	SI32	GetCharUniID(UI16 uiMapID, SI32 siSX, SI32 siSY, SI32 siEX, SI32 siEY, UI16 *pCharUniIDs, UI16 uiMaxGetCharIDNum, SI32 siGetCharInfo);
	// ���� ������ �ִ� �������� ID�� ���´�.
	SI32	GetItemID(UI16 uiMapID, SI32 siSX, SI32 siSY, SI32 siEX, SI32 siEY, UI16 *pItemIDs, UI16 *pItemQuantity);
	BOOL	GetItemID(UI16 uiMapID, SI32 siX, SI32 siY, UI16 *puiItemID, UI16 *pItemQuantity);			// �� Ÿ�Ͽ� �����Ǿ� �ִ� �������� ������ ���´�.
	BOOL	SetMap(UI16 uiMapID, LoadSaveMapTI* pMap);								// �ʿ� �Ӽ��� �������ش�.
	POINT	SearchMap(UI16 uiMapID, SI32 siCX, SI32 siCY, SI32 siSearchInfo, SI32 siParam1, SI32 siParam2 = 0 );		// �ʿ��� Ư�� ��ǥ�� ã�´�.
	VOID	SetItemFromDB();														// DB�κ��� �ʵ忡 ������ �ִ� �������� ������ ���ͼ� �ʿ� �����Ѵ�.
	BOOL	IsValidMapID(UI16 uiMapID);												// ��ȿ�� �� ���̵� ���� ����.
	SIZE	GetMapSize(UI16 uiMapID);												// ���� ����� ���Ѵ�.	
	UI16	GetVillageCode(UI16 uiMapID);											// ���õ� ������ Index Code�� ���´�.
	BOOL	IsColl(UI16 uiMapID, SI32 siX, SI32 siY);								// �浹 Ÿ������ �˾ƿ´�.
	BOOL	IsColl(UI16 uiMapID, SI32 siX, SI32 siY, SI32 siCharMoveableTileAtb);	// �浹 Ÿ������ �˾ƿ´�.(Ư�� ĳ���� ���忡��)	
	BOOL	IsDungeon(UI16 uiMapID);												// �����ΰ�?
	BOOL	CanBeRainy( UI16 uiMapID );												// �� �� �� �ִ� �����ΰ�?
	BOOL	CanBeSnowy( UI16 uiMapID );												// ���� �� �� �ִ� �����ΰ�?
	
	SI32	GetNearSectorPlayerList(UI16 uiMapID, SI32 siX, SI32 siY, UI16 *pCharUniIDs, SI32 siGetCharInfo);		// X, Y�� �������� ���� ������ �÷��̾� ID�� ���´�.
	SI32	GetNearSectorItemList(UI16 uiMapID, SI32 siX, SI32 siY, OnlineReceiveFieldItemInfo *pFieldItemInfo);	// X, Y�� �������� ���� ������ ������ ������ ���´�.

	SI32	GetSectorItemList( UI16 uiMapID, strSector **pSector, OnlineReceiveFieldItemInfo *pFieldItemInfo );
	SI32	GetSectorPlayerList( UI16 uiMapID, strSector **pSector, UI16 *pCharUniIDs, SI32 siGetCharInfo );

	VOID	Poll();																	// Polling

	BOOL	IsWarFieldMap(UI16 uiMapID);											// ���������� �˾ƿ´�.
	BOOL	GetTileInfo(UI16 uiMapID, SI32 siX, SI32 siY, CHAR *pszBuffer);			// Ÿ���� ������ ���´�.

	static	VOID	InitStaticVariable(SOWorld *pWorld);							// ���� ������ �ʱ�ȭ�Ѵ�.
};

#endif
