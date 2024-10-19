/************************************************************************************************
	파일명 : IOnlineObject.h
	담당자 : 이윤석
************************************************************************************************/
#ifndef	_ONLINE_IOBJECT_H
#define _ONLINE_IOBJECT_H

//struct  VillageHeader;
//struct  PortalHeader;
#include "OnlineObject.h"


#define	MAX_ONLINE_OBJECT		512							// 최대 오브젝트 수


class	IOnlineObject
{ 
private:
	OnlineObject				*pOnlineObject;
	LPDIRECTDRAWSURFACE7		pTempSurface;		// 위치 검사를 위해 사용하는 Surface

private:
	// 비어 있는 ID를 구한다.
	SI16	GetEmptyID();
public:
	IOnlineObject();
	~IOnlineObject();
	
	BOOL					Init();															// 초기화.															
	SI16					Init(SI32 siKind, SI16 siX, SI16 siY, cltOnlineWorld *pWorld);	// 오브젝트을 초기화 한다.	
	SI16					Init(VillageHeader *pVillageHeader, cltOnlineWorld *pWorld);	// 오브젝트을 초기화 한다.(마을인 경우에만)	
	SI16					Init(PortalHeader *pPortalHeader, cltOnlineWorld *pWorld);		// 오브젝트을 초기화 한다.(포탈인 경우에만)	
	LPDIRECTDRAWSURFACE7	GetTempSurface(void);											// 임시 Surface 얻기

	VOID	Free();																			// 모든 오브젝트를 없앤다.
	VOID	Free(SI16 siX, SI16 siY);														// 오브젝트 하나를 없앤다
	VOID	Free(SI16 siID);																// 오브젝트 하나를 없앤다
	VOID	FreeAll();																		// 오브젝트만 모두 삭제
	VOID	Action();																		// 오브젝트의 Action.
	VOID	Draw(SI16 siID, SI16 SelectID);													// 오브젝트을 화면에 그린다.	
	VOID	DrawTileType(SI16 siID, SI16 SelectID); 										// 오브젝트을 화면에 Tile 방식으로 그린다.	
	VOID	GetName(SI16 siID, CHAR pszName[32]);											// 오브젝트의 이름을 얻어온다.	
	CHAR	*GetMark(SI16 siID);															// 오브젝트의 설명을 얻어온다.
	BOOL	IsAtb(SI16 siID, SI32 siAtb) { return pOnlineObject[siID].IsAtb(siAtb); }		// 오브젝트에게 속성이 있는지 얻어온다.	
	SI32	GetVillageKind(SI16 siID);														// 마을이라면 종류를 얻어온다.	
	VOID	GetPos(SI16 siID, SI16 *psiX, SI16 *psiY);										// 오브젝트이 있는 위치를 얻어온다.	
	VOID	GetSize(SI16 siID, SI16 *psiXsize, SI16 *psiYsize);								// 오브젝트이 차지하는 영역을 얻어온다.	
	VOID	GetVillageFileName(SI16 siID, CHAR *pszVillageFileName);						// 마을일 경우 마을과 연결되는 화일을 얻어온다.
	SI16	GetObjectID(SI16 siX, SI16 siY);												// 해당 위치에 심겨져 있는 오브젝트의 ID(배열중의 인덱스)를 얻어온다.	
	VOID	WriteFileForBug(CHAR *pszTitle);												// 버그를 잡기 위한 정보를 화일에 쓴다.
	SI16	GetKind(SI16 siID);																// 종류를 얻어온다.
	SI32	GetKindIndex(SI16 siID)	{ return pOnlineObject[siID].GetKindIndex();}			// 종류를 얻어온다.
	BOOL	IsPortal(SI16 siID);															// 오브젝트가 포탈인지 검사한다.
	SI16	GetPortalID(SI16 siID);															// Portal ID를 돌려준다.
	char*	GetPortalName(SI16 siID);														// Portal Name를 돌려준다.
	UI16	GetVillageCode(SI16 siID);														// Village Code를 돌려준다.
	BOOL	CheckImage(SI16 siID, SI16 x, SI16 y, LPDIRECTDRAWSURFACE7 pSurface);			// x, y 위치에 Image가 있는지 검사한다.
	void	CheckZone(SI16 sx, SI16 sy, SI16 ex, SI16 ey);									// 범위내에 있지 않은 Object들을 삭제한다.
	BOOL	CheckTileDraw(SI16 siID)	{ return pOnlineObject[siID].CheckTileDraw(); }		// Tile Draw 인지 검사

	UI16	GetVillageObject(UI16 uiVillageCode);											// Village Object를 찾는다.
};	

#endif
