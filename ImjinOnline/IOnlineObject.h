/************************************************************************************************
	���ϸ� : IOnlineObject.h
	����� : ������
************************************************************************************************/
#ifndef	_ONLINE_IOBJECT_H
#define _ONLINE_IOBJECT_H

//struct  VillageHeader;
//struct  PortalHeader;
#include "OnlineObject.h"


#define	MAX_ONLINE_OBJECT		512							// �ִ� ������Ʈ ��


class	IOnlineObject
{ 
private:
	OnlineObject				*pOnlineObject;
	LPDIRECTDRAWSURFACE7		pTempSurface;		// ��ġ �˻縦 ���� ����ϴ� Surface

private:
	// ��� �ִ� ID�� ���Ѵ�.
	SI16	GetEmptyID();
public:
	IOnlineObject();
	~IOnlineObject();
	
	BOOL					Init();															// �ʱ�ȭ.															
	SI16					Init(SI32 siKind, SI16 siX, SI16 siY, cltOnlineWorld *pWorld);	// ������Ʈ�� �ʱ�ȭ �Ѵ�.	
	SI16					Init(VillageHeader *pVillageHeader, cltOnlineWorld *pWorld);	// ������Ʈ�� �ʱ�ȭ �Ѵ�.(������ ��쿡��)	
	SI16					Init(PortalHeader *pPortalHeader, cltOnlineWorld *pWorld);		// ������Ʈ�� �ʱ�ȭ �Ѵ�.(��Ż�� ��쿡��)	
	LPDIRECTDRAWSURFACE7	GetTempSurface(void);											// �ӽ� Surface ���

	VOID	Free();																			// ��� ������Ʈ�� ���ش�.
	VOID	Free(SI16 siX, SI16 siY);														// ������Ʈ �ϳ��� ���ش�
	VOID	Free(SI16 siID);																// ������Ʈ �ϳ��� ���ش�
	VOID	FreeAll();																		// ������Ʈ�� ��� ����
	VOID	Action();																		// ������Ʈ�� Action.
	VOID	Draw(SI16 siID, SI16 SelectID);													// ������Ʈ�� ȭ�鿡 �׸���.	
	VOID	DrawTileType(SI16 siID, SI16 SelectID); 										// ������Ʈ�� ȭ�鿡 Tile ������� �׸���.	
	VOID	GetName(SI16 siID, CHAR pszName[32]);											// ������Ʈ�� �̸��� ���´�.	
	CHAR	*GetMark(SI16 siID);															// ������Ʈ�� ������ ���´�.
	BOOL	IsAtb(SI16 siID, SI32 siAtb) { return pOnlineObject[siID].IsAtb(siAtb); }		// ������Ʈ���� �Ӽ��� �ִ��� ���´�.	
	SI32	GetVillageKind(SI16 siID);														// �����̶�� ������ ���´�.	
	VOID	GetPos(SI16 siID, SI16 *psiX, SI16 *psiY);										// ������Ʈ�� �ִ� ��ġ�� ���´�.	
	VOID	GetSize(SI16 siID, SI16 *psiXsize, SI16 *psiYsize);								// ������Ʈ�� �����ϴ� ������ ���´�.	
	VOID	GetVillageFileName(SI16 siID, CHAR *pszVillageFileName);						// ������ ��� ������ ����Ǵ� ȭ���� ���´�.
	SI16	GetObjectID(SI16 siX, SI16 siY);												// �ش� ��ġ�� �ɰ��� �ִ� ������Ʈ�� ID(�迭���� �ε���)�� ���´�.	
	VOID	WriteFileForBug(CHAR *pszTitle);												// ���׸� ��� ���� ������ ȭ�Ͽ� ����.
	SI16	GetKind(SI16 siID);																// ������ ���´�.
	SI32	GetKindIndex(SI16 siID)	{ return pOnlineObject[siID].GetKindIndex();}			// ������ ���´�.
	BOOL	IsPortal(SI16 siID);															// ������Ʈ�� ��Ż���� �˻��Ѵ�.
	SI16	GetPortalID(SI16 siID);															// Portal ID�� �����ش�.
	char*	GetPortalName(SI16 siID);														// Portal Name�� �����ش�.
	UI16	GetVillageCode(SI16 siID);														// Village Code�� �����ش�.
	BOOL	CheckImage(SI16 siID, SI16 x, SI16 y, LPDIRECTDRAWSURFACE7 pSurface);			// x, y ��ġ�� Image�� �ִ��� �˻��Ѵ�.
	void	CheckZone(SI16 sx, SI16 sy, SI16 ex, SI16 ey);									// �������� ���� ���� Object���� �����Ѵ�.
	BOOL	CheckTileDraw(SI16 siID)	{ return pOnlineObject[siID].CheckTileDraw(); }		// Tile Draw ���� �˻�

	UI16	GetVillageObject(UI16 uiVillageCode);											// Village Object�� ã�´�.
};	

#endif
