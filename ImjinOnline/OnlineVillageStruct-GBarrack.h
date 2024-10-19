/************************************************************************************************
	파일명 : OnlineVillageStruct-GBarrack.h
	담당자 : 이윤석
************************************************************************************************/
#ifndef	_ONLINE_VILLAGEBASESTRUCTUREGBARRACK_HEADER
#define _ONLINE_VILLAGEBASESTRUCTUREGBARRACK_HEADER


#include <DDraw.h>
#include "Online.h"
#include "OnlineVillageBaseBarrack.h"


#define GBARRACK_NOWWORK_BUY_START							1
#define GBARRACK_NOWWORK_BUY_STARTPOOL						2
#define GBARRACK_NOWWORK_BUY_POOL							3
#define GBARRACK_NOWWORK_BUYCHECK_POOL						4
#define GBARRACK_NOWWORK_BUYCHECK_POOL_RETURNWAIT			5
#define GBARRACK_NOWWORK_BUY_END							6
#define GBARRACK_NOWWORK_BUY_ENDPOOL						7	
#define GBARRACK_NOWWORK_CHANGEJOB_GETLIST					8
#define GBARRACK_NOWWORK_CHANGEJOB_POOL						9
#define GBARRACK_NOWWORK_CHANGEJOBCHECK_POOL				10
#define GBARRACK_NOWWORK_CHANGEJOBCHECK_POOL_RETURNWAIT		11
#define GBARRACK_NOWWORK_CHANGEJOB_END						12
#define GBARRACK_NOWWORK_CHANGEJOB_ENDPOOL					13	
#define GBARRACK_NOWWORK_DISMISSAL_GETLIST					14
#define GBARRACK_NOWWORK_DISMISSAL_POOL						15
#define GBARRACK_NOWWORK_DISMISSALCHECK_POOL				16
#define GBARRACK_NOWWORK_DISMISSALCHECK_POOL_RETURNWAIT		17
#define GBARRACK_NOWWORK_DISMISSAL_END						18
#define GBARRACK_NOWWORK_DISMISSAL_ENDPOOL					19
//2차 전직 추가 작업.
#define GBARRACK_NOWWORK_CHANGEGENERAL_GETLIST					20
#define GBARRACK_NOWWORK_CHANGEGENERAL_POOL						21
#define GBARRACK_NOWWORK_CHANGEGENERAL_CHECK_POOL				22
#define GBARRACK_NOWWORK_CHANGEGENERAL_CHECK_POOL_RETURNWAIT	23
#define GBARRACK_NOWWORK_CHANGEGENERAL_END						24
#define GBARRACK_NOWWORK_CHANGEGENERAL_ENDPOOL					25
#define GBARRACK_NOWWORK_CHANGEGENERAL_SELECT					26			

#define MAX_BUYGSOLDER_LIST								32


// 훈련소 Class
class	OnlineVillageStructureGBarrack : public OnlineVillageBaseBarrack
{
private:
	// Base
	SI16						m_Say;

	// Image
	XSPR						m_Image_MainInterface_Back1;						// 후련소 바닥 판넬
	XSPR						m_Image_CheckInterface;
//	XSPR*						m_Image_CharacterSlotInterface;

	XSPR						m_Image_Button_All_MM;
	XSPR						m_Image_Button_All_EM;
	XSPR						m_Image_Button_All_BM;
	XSPR						m_Image_Check_Button;
	XSPR						m_Image_Line_Check_Button;

	XSPR						m_Image_Portrait;

	// Scroll
	SI16						m_NowScroll;

	SI16						m_siX,m_siY;
	SI16						m_siCheckX,m_siCheckY;

	// Button
	_Button						m_Button_Main_Buy;
	_Button						m_Button_Main_ChangeJob;
	_Button						m_Button_Main_Dismissal;
	_Button						m_Button_Main_ChangeGeneral;

	_Button						m_Button_Main_General;
	_Button						m_Button_Main_GeneralEx;

	_Button						m_Button_Buy;
	_Button						m_Button_BuyOK;
	_Button						m_Button_BuyCancel;
	_Button						m_Button_ChangeJob;
	_Button						m_Button_ChangeJobOK;
	_Button						m_Button_ChangeJobCancel;
	_Button						m_Button_Dismissal;
	_Button						m_Button_DismissalOK;
	_Button						m_Button_DismissalCancel;

	_Button						m_Button_GeneralExBuy;
	_Button						m_Button_GeneralExChange;
	_Button						m_Button_GeneralExDismissal;

public:
	// Solder Data
	OnInnBuySolderInfo			m_BuySolderList[MAX_BUYGSOLDER_LIST];
	SI16						m_BuyMaxSelectSolder;
	SI16						m_BuyNowSelectSolder;
	OnDismissalSolderInfo		m_DismissalSolderList[MAX_BUYGSOLDER_LIST];
	SI16						m_DismissalMaxSelectSolder;
	SI16						m_DismissalNowSelectSolder;
	OnChangeJobSolderInfo		m_ChangeJobSolderList[MAX_BUYGSOLDER_LIST];
	SI16						m_ChangeJobMaxSelectSolder;
	SI16						m_ChangeJobNowSelectSolder;
	SI32						m_Discount;
	SI32						m_ChangeJobSelectType;

	SI16						m_NowWork;
	DWORD						m_StartTickCount;
	BOOL						m_bGeneralExMode;

public:
	OnlineVillageStructureGBarrack();												// 생성자.
	virtual	~OnlineVillageStructureGBarrack();										// 소멸자.
		
	virtual	VOID	Init(UI16 VillageCode, cltOnlineWorld *pOnlineWorld, VillageStructManager	*pVillageStructManager);// 초기화
	virtual	VOID	Free();															// 해제.
	virtual BOOL	Action();

	virtual BOOL	Poll(LPDIRECTDRAWSURFACE7 pSurface);									// 액션.	
	virtual BOOL	Draw_BuySelect(LPDIRECTDRAWSURFACE7 pSurface);			// Draw Buy
	virtual BOOL	Draw_BuyCheck(LPDIRECTDRAWSURFACE7 pSurface);			// Draw Buy Check
	virtual BOOL	Draw_ChangeJobSelect(LPDIRECTDRAWSURFACE7 pSurface);	// Draw ChangeJob
	virtual BOOL	Draw_ChangeJobCheck(LPDIRECTDRAWSURFACE7 pSurface);	// Draw ChangeJob Check
	virtual BOOL	Draw_DismissalSelect(LPDIRECTDRAWSURFACE7 pSurface);	// Draw Dismissal
	virtual BOOL	Draw_DismissalCheck(LPDIRECTDRAWSURFACE7 pSurface);	// Draw Dismissal Check

	virtual BOOL	Draw_ChangeGeneralExWait(LPDIRECTDRAWSURFACE7 pSurface);

	virtual SI32	CheckLevel(SI32 Kind, SI32 Lv, SI32 Credit);										// Check Level(Return Value : 0 : 성공, 1 : 신용도 부족, 2 : 같은 장수를 데리고 있음, 3 : 장수수 초과
	virtual BOOL	CheckChangeJob(SI32 Index);															// Check Change Job
	virtual SI32	CheckChangeJobG(SI32 Index, SI32 Slot);												// Check Change Job(Return Value : 0 : 성공, 1 : 레벨부족, 2 : 같은 장수를 데리고 있음, 3 : 장수수 초과
	virtual SI32	GetFollowerGBarrackText(UI16 Kind);										// 각 용병별로 Text 지정

	VOID	SetGeneralExMode(BOOL bMode) { m_bGeneralExMode = bMode;};
	BOOL	GetGeneralExMode()			 { return m_bGeneralExMode;};
};


#endif
