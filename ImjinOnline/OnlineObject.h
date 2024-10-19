/************************************************************************************************
	파일명 : OnlineObject.h
	담당자 : 이윤석
************************************************************************************************/
#ifndef _ONLINE_OBJECT_H
#define _ONLINE_OBJECT_H

#include "OnlineVillage-Parser.h"

// OnlineObject 에서 마을만이 사용하는 데이타.
struct	OnlineVillageDataOfObject
{
	UI16				uiVillageCode;
	UI08				uiVillageStateCode;
	VillageHeader*		pVillageHeader;
	CHAR				szVillageFileName[1024];
	CHAR				szVillageFileNameHalfE[1024];

	SI16				siGuildFlagXPos;
	SI16				siGuildFlagYPos;

	SI16				siVillageSize;
	SI16				siVillageExplosionFlag;
	DWORD				siVillageExplosionSoundWait;

	// robypark 2004/12/28 19:3
	// 타일 좌표 아님. 마을 중심 타일에서 떨어진 거리(변환된 실제 dot 거리)
	SI16				m_ssSurrenderFlagPosX;					// 마을 점령 깃발 위치
	SI16				m_ssSurrenderFlagPosY;					// 마을 점령 깃발 위치

	SI16				m_ssEnabledOccupyVillagePosX;			// 수비병으로 공격하여 마을 점령을 하라는 이미지 위치
	SI16				m_ssEnabledOccupyVillagePosY;			// 수비병으로 공격하여 마을 점령을 하라는 이미지 위치

	// 마을 중심(변환된 dot)에서 위로 떨어진 거리에 위치하는 값
	SI16				m_ssVillageGarrisionGaugePosX;			// 마을 주둔군(수비병) 표시 게이지 위치
	SI16				m_ssVillageGarrisionGaugePosY;			// 마을 주둔군(수비병) 표시 게이지 위치
	SI16				m_ssVillageDefenceGaugePosX;			// 마을 방어력 표시 게이지 위치
	SI16				m_ssVillageDefenceGaugePosY;			// 마을 방어력 표시 게이지 위치
};

// OnlineObject 에서 Portal만이 사용하는 데이타.
struct	OnlinePortalDataOfObject
{
	SI16	siPortalCode;
	UI08	uifTileDraw;
	CHAR	szPortalFileName[1024];
};


class	OnlineObject
{
private:
	CHAR	szName[32];
	CHAR	*m_pszMark;

	SI16	siID;
	SI16	siKind;
	SI32	siObjectKindIndex;
	SI32	siAtb;				// 속성.
	SI16	siTotalFrame;		// 총 몇 프래임으로 애니 매이션이 되는가?

	SI16	siFile;
	SI16	siFont;	
	SI16	siFile2;
	SI16	siFont2;	
	SI16	siSprCenterX;
	SI16	siSprCenterY;

	SI16	siX;
	SI16	siY;

	SI16	siXsize;
	SI16	siYsize;

	SI16	siVillageKind;		// 마을이라면 어떤 마을인가?

	
	OnlineVillageDataOfObject*	pOnlineVillageDataOfObject;
	OnlinePortalDataOfObject*	pOnlinePortalDataOfObject;

	cltOnlineWorld	*pMyOnlineWorld;		
	
public:
	OnlineObject();
	~OnlineObject();

	// 초기화.
	VOID	Init(SI16 siID, SI16 siKind, SI16 siX, SI16 siY, cltOnlineWorld *pOnlineWorld);	

	// 초기화.(마을인 경우)
	VOID	Init(SI16 siID, VillageHeader *pVillageHeader, cltOnlineWorld *pOnlineWorld);	

	// 초기화.(Portal인 경우)
	VOID	Init(SI16 siID, PortalHeader *pPortalHeader, cltOnlineWorld *pOnlineWorld);	

	// 해제한다.
	VOID	Free();

	// 건물의 이름을 얻어온다.
	VOID	GetName(CHAR pszName[32]);

	// 설명을 얻어온다.
	CHAR	*GetMark(VOID);

	// 마을이라면 종류를 얻어온다.
	SI16	GetVillageKind();

	// 초기화가 되었는가?
	BOOL	IsInit()			{ return pMyOnlineWorld != NULL ? TRUE : FALSE; }

	// 화면에 그린다.
	VOID	Draw(SI16 SelectID); 
	VOID	DrawTileType(SI16 SelectID); 

	// robypark 2005/1/7 20:7
	// 마을 방어력 게이지 출력
	VOID	DrawGaugeVillageDefence(SI32 siSrcDotDrawBaseX, SI32 siSrcDotDrawBaseY);

	// robypark 2005/1/7 20:7
	// 마을 수비병 게이지 출력
	VOID	DrawGaugeVillageGarrison(SI32 siSrcDotDrawBaseX, SI32 siSrcDotDrawBaseY);

	// robypark 2005/1/7 20:7
	// 마을 검병으로 점령하는 이펙트 출력
	VOID	DrawEffectEnabledOccupyVillage(SI32 siSrcDotDrawBaseX, SI32 siSrcDotDrawBaseY);

	// 액션.
	VOID	Action();

	// 속성이 있는지 얻어온다.
	BOOL	IsAtb(SI32 siAtb);	

	// 건물이 있는 위치를 얻어온다.
	VOID	GetPos(SI16 *psiX, SI16 *psiY);

	// 건물이 차지하는 영역을 얻어온다.
	VOID	GetSize(SI16 *psiXsize, SI16 *psiYsize);

	// 마을일 경우 마을과 연결되는 화일을 얻어온다.
	VOID	GetVillageFileName(CHAR *pszVillageFileName);

	// 버그를 잡기 위한 정보를 화일에 쓴다.
	VOID	WriteFileForBug(FILE *fp);

	// 오브젝트의 종류를 얻어온다
	SI16	GetKind();
	SI32	GetKindIndex()	{ return siObjectKindIndex; }

	// 오브젝트가 포탈인지 검사한다.
	BOOL	IsPortal();

	// Portal ID를 돌려준다.
	SI16	GetPortalID(void);

	// Portal Name를 돌려준다.
	char*	GetPortalName(void);

	// Village Code를 돌려준다.
	UI16	GetVillageCode(void);

	// x, y 위치에 Image가 있는지 검사한다.
	BOOL	CheckImage(SI16 x, SI16 y, LPDIRECTDRAWSURFACE7 pSurface);

	// Tile Draw 인지 검사
	BOOL	CheckTileDraw(void)
	{
		if(pOnlinePortalDataOfObject)
			if(pOnlinePortalDataOfObject->uifTileDraw)
				return TRUE;

		return FALSE;
	}
};

#endif
