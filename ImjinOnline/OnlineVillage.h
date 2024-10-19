#ifndef	_ONLINE_VILLAGE_H
#define _ONLINE_VILLAGE_H

// 파일명 : OnlineVillage.h
// 담당자 : 정진욱

#include <main.h>
#include <Online.h>
#include "OnlineVillageStructManager.h"
#include "OnlineItem.h"
#include "OnlineEtc.h"
#include "OnlineVillageStruct-Market.h"
#include "OnlineVillageStruct-Bank.h"
#include "OnlineVillageStruct-Barrack.h"
#include "OnlineVillageStruct-Hospital.h"
#include "OnlineVillageStruct-Inn.h"
#include "OnlineVillageStruct-Wharf.h"
#include "OnlineVillageStruct-Government.h"
#include "OnlineVillageStruct-GBarrack.h"
#include "OnlineVillageStruct-MBarrack.h"
#include "OnlineVillageStruct-Farm.h"
#include "OnlineGovernMgr.h"


#define	ON_VILLAGE_STATUS_IN_VILLAGE			1
#define	ON_VILLAGE_STATUS_IN_STRUCTURE			2
#define	ON_VILLAGE_STATUS_IN_EXIT				3


class	OnlineVillageStructMake;
class	OnlineVillage
{
private:
//	SI32						siID;

	SI32						siKind;
	CHAR						szName[128];											// 마을 이름
	SI16						siNationCode;											// 마을의 국가 코드

public:
	BOOL						fDead;													// 죽어서 마을에 들어왔는지 Flag
	SI32						m_siMsgResult;
	UI16						BackgroundMusic;										// 배경음악

private:
	BOOL						m_fStructLeaveVillageLeave;

	int							NowScrollXI;
	int							NowScrollYI;
	float						NowScrollX;
	float						NowScrollY;
	LPDIRECTDRAWSURFACE7		BackImageSpr;											// 배경 화면.
	BOOL						bIsNight;												// 현재 밤/낮의 상태. TRUE 이면 밤

	CHAR						szBackImageSprFileName[256];							// 배경화면 File Name
	SI32						BackImageSprWidth;
	SI32						BackImageSprHeight;
	CHAR						szStructName[ON_MAX_VIL_STRUCTNUM][32];					// 마을 내의 건물 이름.
	SI32						siStructCode[ON_MAX_VIL_STRUCTNUM];						// 마을의 특수 코드	
	SI32						siStructNum;
	
	BOOL						bMousePush;

	_ImageButton				BStruct[ON_MAX_VIL_STRUCTNUM];							// 건물의 버튼.
	XSPR						StructSpr[ON_MAX_VIL_STRUCTNUM];						// 건물의 스프라이트.
	XSPR						StructSprAni[ON_MAX_VIL_STRUCTNUM][3];					// 건물의 Animation 스프라이트.
	XSPR						StructSprBack[ON_MAX_VIL_STRUCTNUM];					// 건물의 Back 스프라이트.
	SI16						StructSprBackMoveXPosition[ON_MAX_VIL_STRUCTNUM];
	SI16						StructSprBackMoveYPosition[ON_MAX_VIL_STRUCTNUM];
	UI08						**ppStruct;

	_Button						BExit;													// 나가기 버튼.
	XSPR						*pExitSpr;												// 나가기 버튼의 스프라이트.

	UI08						m_VillageSignAnimationFlag;								// 현재 간판 Animation Flag(0 : 보통, 1 : 나올때, 2 : 들어갈때, 3 : 들어갈서 안보일때)
	UI08						m_VillageSignFlag;										// 현재 다른 곳으로 이동중일때 간판 Flag(0xff : 마을 밖으로 이동, 그외는 이동할려는 건물 Index)
	DWORD						m_VillageSignTickCount;									// 현재 간판 Animation 시작 TickCount
	XSPR						m_WallPaperImage, m_TopImage, m_TopBoardImage;
	XSPR						m_ExitImage, m_ExitBackImage;

	UI08						*pExit;

	SI32						siSubStructButtonNum;									// 서브 건물의 버튼 개수.
	SI32						*psiSubStructCode;										// 서브 건물의 코드들.
	//VillageStructBorderInfo		SubBorderInfo;										// 서브 버튼들의 보더.	
	
	cltOnlineWorld				*pMyOnlineWorld;										// Online World
	VillageHeader				*pVillageHeader;

	SI16						siMakeStatus;											// 지누기 수정	
	DWORD						dwDrawTime;
	SI16						siSkip;

public:
	VillageStructManager		*pVillageStructManager;									// 마을 안에 있는 건물들을 관리하는 객체.
	OnlineVillageStructMake		*pMakeList;												// 리스트를 보여준다.(공장)

	OnRecvVillageInfo			m_OnRecvVillageInfo;									// 마을 정보	
	BOOL						fFlip;
	BOOL						LeaveMiniMapFlag;
	BOOL						LeaveMessengerFlag;
	SI16						siStatus;												// 현재 마을의 어떤 부분을 처리 중인가.

private:
	VOID	CreateVillageStructManager(SI32 siID);										// 하위 건물들을 관리하는 관리자를 생성한다.
	VOID	CreateSubStructButton();													// 하위 건물의 버튼을 생성한다.
	VOID	FreeSubStructButton();														// 하위 건물의 버튼을 해제한다.


public:
	OnlineVillage();																	// 생성자.
	~OnlineVillage();																	// 소멸자.

	VOID	Init(UI16 uiVillageCode, OnRecvVillageInfo* lpOnRecvVillageInfo, cltOnlineWorld *pOnlineWorld);		// 초기화.
	VOID	Free();																		// 해제.
	SI32	Poll(UI16 uiClientRet);														// 폴링 함수.	
//	SI32	GetID();																	// 마을의 ID.
	char*	GetMaxInvestmentID(void);													// 최고 투자자 ID 얻기

	SI16	GetStatus(void);																			// 현재 잡업상태를 돌려줌
	VOID	Scroll(float x, float y);																	// Scroll
	VOID	MoveInStruct(SI32 siStructKind, BOOL fStructLeaveVillageLeave, BOOL fNoneSelect = FALSE);		// Move In Struct
	BOOL	IsVaildStruct(SI32 siIndex);																// 유효한 건물인지 알아온다.

	SI16	InVillagePoll(UI16 uiClientRet);															// 마을 안에 있을때의 처리.
	void	ViewVillagePoll(BOOL fStructure);															// 마을 안 출력
	SI16	InStructurePoll(BOOL fVer = TRUE);															// 건물 안에 있을때의 처리.
	

	BOOL	MakeStrcutBackSprite(char* lpName, char* lpSaveName, SI16 Index, SI16 StructType);			// 건물의 배경 Sprite 이름 생성
	BOOL	MakeStructAnimationSprite(char* lpName, char* lpSaveName, int AniType);						// 건물의 Animation Sprite 이름 생성
	OnlineVillageStructMake	*GetMakeList();

	VillageHeader	*GetVillageHeader();

	BOOL	MakeVillageInStructor(char* lpChar);
	char*	GetVillageName(void);																		// 마을 이름을 돌려줌
	SI16	GetNation()	{ return siNationCode; };


	// Field Struct
	BOOL	InitMapStruct(UI16 VillageCode, SI16 siStructCode, cltOnlineWorld* lpWorld, OnlineVillageInfoData* lpOnlineVillageInfoData);
	void	FreeMapStruct(void);

	BOOL	CheckFieldRender(void);
	BOOL	PollField(void);


	VOID	DrawVillageInfo(LPDIRECTDRAWSURFACE7 pSurface, BOOL bStruct, BOOL bFarm=FALSE);	// 마을이름, 인구, 주인 등등의 정보들...

	VOID	LoadBackImageSprite( BOOL local_bIsNight );
};
 

#endif
