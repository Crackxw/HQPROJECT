#ifndef	_ONLINE_VILLAGE_H
#define _ONLINE_VILLAGE_H

// ���ϸ� : OnlineVillage.h
// ����� : ������

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
	CHAR						szName[128];											// ���� �̸�
	SI16						siNationCode;											// ������ ���� �ڵ�

public:
	BOOL						fDead;													// �׾ ������ ���Դ��� Flag
	SI32						m_siMsgResult;
	UI16						BackgroundMusic;										// �������

private:
	BOOL						m_fStructLeaveVillageLeave;

	int							NowScrollXI;
	int							NowScrollYI;
	float						NowScrollX;
	float						NowScrollY;
	LPDIRECTDRAWSURFACE7		BackImageSpr;											// ��� ȭ��.
	BOOL						bIsNight;												// ���� ��/���� ����. TRUE �̸� ��

	CHAR						szBackImageSprFileName[256];							// ���ȭ�� File Name
	SI32						BackImageSprWidth;
	SI32						BackImageSprHeight;
	CHAR						szStructName[ON_MAX_VIL_STRUCTNUM][32];					// ���� ���� �ǹ� �̸�.
	SI32						siStructCode[ON_MAX_VIL_STRUCTNUM];						// ������ Ư�� �ڵ�	
	SI32						siStructNum;
	
	BOOL						bMousePush;

	_ImageButton				BStruct[ON_MAX_VIL_STRUCTNUM];							// �ǹ��� ��ư.
	XSPR						StructSpr[ON_MAX_VIL_STRUCTNUM];						// �ǹ��� ��������Ʈ.
	XSPR						StructSprAni[ON_MAX_VIL_STRUCTNUM][3];					// �ǹ��� Animation ��������Ʈ.
	XSPR						StructSprBack[ON_MAX_VIL_STRUCTNUM];					// �ǹ��� Back ��������Ʈ.
	SI16						StructSprBackMoveXPosition[ON_MAX_VIL_STRUCTNUM];
	SI16						StructSprBackMoveYPosition[ON_MAX_VIL_STRUCTNUM];
	UI08						**ppStruct;

	_Button						BExit;													// ������ ��ư.
	XSPR						*pExitSpr;												// ������ ��ư�� ��������Ʈ.

	UI08						m_VillageSignAnimationFlag;								// ���� ���� Animation Flag(0 : ����, 1 : ���ö�, 2 : ����, 3 : ���� �Ⱥ��϶�)
	UI08						m_VillageSignFlag;										// ���� �ٸ� ������ �̵����϶� ���� Flag(0xff : ���� ������ �̵�, �׿ܴ� �̵��ҷ��� �ǹ� Index)
	DWORD						m_VillageSignTickCount;									// ���� ���� Animation ���� TickCount
	XSPR						m_WallPaperImage, m_TopImage, m_TopBoardImage;
	XSPR						m_ExitImage, m_ExitBackImage;

	UI08						*pExit;

	SI32						siSubStructButtonNum;									// ���� �ǹ��� ��ư ����.
	SI32						*psiSubStructCode;										// ���� �ǹ��� �ڵ��.
	//VillageStructBorderInfo		SubBorderInfo;										// ���� ��ư���� ����.	
	
	cltOnlineWorld				*pMyOnlineWorld;										// Online World
	VillageHeader				*pVillageHeader;

	SI16						siMakeStatus;											// ������ ����	
	DWORD						dwDrawTime;
	SI16						siSkip;

public:
	VillageStructManager		*pVillageStructManager;									// ���� �ȿ� �ִ� �ǹ����� �����ϴ� ��ü.
	OnlineVillageStructMake		*pMakeList;												// ����Ʈ�� �����ش�.(����)

	OnRecvVillageInfo			m_OnRecvVillageInfo;									// ���� ����	
	BOOL						fFlip;
	BOOL						LeaveMiniMapFlag;
	BOOL						LeaveMessengerFlag;
	SI16						siStatus;												// ���� ������ � �κ��� ó�� ���ΰ�.

private:
	VOID	CreateVillageStructManager(SI32 siID);										// ���� �ǹ����� �����ϴ� �����ڸ� �����Ѵ�.
	VOID	CreateSubStructButton();													// ���� �ǹ��� ��ư�� �����Ѵ�.
	VOID	FreeSubStructButton();														// ���� �ǹ��� ��ư�� �����Ѵ�.


public:
	OnlineVillage();																	// ������.
	~OnlineVillage();																	// �Ҹ���.

	VOID	Init(UI16 uiVillageCode, OnRecvVillageInfo* lpOnRecvVillageInfo, cltOnlineWorld *pOnlineWorld);		// �ʱ�ȭ.
	VOID	Free();																		// ����.
	SI32	Poll(UI16 uiClientRet);														// ���� �Լ�.	
//	SI32	GetID();																	// ������ ID.
	char*	GetMaxInvestmentID(void);													// �ְ� ������ ID ���

	SI16	GetStatus(void);																			// ���� ������¸� ������
	VOID	Scroll(float x, float y);																	// Scroll
	VOID	MoveInStruct(SI32 siStructKind, BOOL fStructLeaveVillageLeave, BOOL fNoneSelect = FALSE);		// Move In Struct
	BOOL	IsVaildStruct(SI32 siIndex);																// ��ȿ�� �ǹ����� �˾ƿ´�.

	SI16	InVillagePoll(UI16 uiClientRet);															// ���� �ȿ� �������� ó��.
	void	ViewVillagePoll(BOOL fStructure);															// ���� �� ���
	SI16	InStructurePoll(BOOL fVer = TRUE);															// �ǹ� �ȿ� �������� ó��.
	

	BOOL	MakeStrcutBackSprite(char* lpName, char* lpSaveName, SI16 Index, SI16 StructType);			// �ǹ��� ��� Sprite �̸� ����
	BOOL	MakeStructAnimationSprite(char* lpName, char* lpSaveName, int AniType);						// �ǹ��� Animation Sprite �̸� ����
	OnlineVillageStructMake	*GetMakeList();

	VillageHeader	*GetVillageHeader();

	BOOL	MakeVillageInStructor(char* lpChar);
	char*	GetVillageName(void);																		// ���� �̸��� ������
	SI16	GetNation()	{ return siNationCode; };


	// Field Struct
	BOOL	InitMapStruct(UI16 VillageCode, SI16 siStructCode, cltOnlineWorld* lpWorld, OnlineVillageInfoData* lpOnlineVillageInfoData);
	void	FreeMapStruct(void);

	BOOL	CheckFieldRender(void);
	BOOL	PollField(void);


	VOID	DrawVillageInfo(LPDIRECTDRAWSURFACE7 pSurface, BOOL bStruct, BOOL bFarm=FALSE);	// �����̸�, �α�, ���� ����� ������...

	VOID	LoadBackImageSprite( BOOL local_bIsNight );
};
 

#endif
