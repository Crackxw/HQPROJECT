/************************************************************************************************
	파일명 : OnlineVillageStruct-MBarrack.h
	담당자 : 이윤석
************************************************************************************************/
#ifndef	_ONLINE_VILLAGEBASESTRUCTUREMBARRACK_HEADER
#define _ONLINE_VILLAGEBASESTRUCTUREMBARRACK_HEADER


#include <DDraw.h>
#include "Online.h"
#include "OnlineVillageBaseBarrack.h"


#define MBARRACK_NOWWORK_BUY_START							1
#define MBARRACK_NOWWORK_BUY_STARTPOOL						2
#define MBARRACK_NOWWORK_BUY_POOL							3
#define MBARRACK_NOWWORK_BUYCHECK_POOL						4
#define MBARRACK_NOWWORK_BUYCHECK_POOL_RETURNWAIT			5
#define MBARRACK_NOWWORK_BUY_END							6
#define MBARRACK_NOWWORK_BUY_ENDPOOL						7	
#define MBARRACK_NOWWORK_DISMISSAL_GETLIST					8
#define MBARRACK_NOWWORK_DISMISSAL_POOL						9
#define MBARRACK_NOWWORK_DISMISSALCHECK_POOL				10
#define MBARRACK_NOWWORK_DISMISSALCHECK_POOL_RETURNWAIT		11
#define MBARRACK_NOWWORK_DISMISSAL_END						12
#define MBARRACK_NOWWORK_DISMISSAL_ENDPOOL					13	

#define MAX_BUYSOLDER_LIST									128


// 몬스터양성소 Class
class	OnlineVillageStructureMBarrack : public OnlineVillageBaseBarrack
{
private:
	// Base
	SI16						m_Say;
	char						m_MonsterBuyItemName[256];

	// Image
	XSPR						m_Image_MainInterface_Back;							// 훈련소 가장 바닥 판넬
	XSPR						m_Image_MainInterface_Back1;						// 후련소 바닥 판넬
	XSPR						m_Image_MainInterface_Back2;						// 후련소 바닥 판넬
	XSPR						m_Image_CheckInterface;
//	XSPR*						m_Image_CharacterSlotInterface;

	XSPR						m_Image_Button_All_MM;
	XSPR						m_Image_Button_All_EM;
	XSPR						m_Image_Button_All_BM;
	XSPR						m_Image_Check_Button;
	XSPR						m_Image_Line_Check_Button;

	XSPR						m_Image_Portrait;

	SI16						m_siX,m_siY;
	SI16						m_siCheckX,m_siCheckY;

	// Scroll
	SI16						m_NowScroll;

	// Button
	_Button						m_Button_Main_Buy;
	_Button						m_Button_Main_Dismissal;

	_Button						m_Button_Buy;
	_Button						m_Button_BuyOK;
	_Button						m_Button_BuyCancel;
	_Button						m_Button_Dismissal;
	_Button						m_Button_DismissalOK;
	_Button						m_Button_DismissalCancel;

public:
	SI16						m_NowWork;

	DWORD						m_StartTickCount;

	// Solder Data
	OnInnBuyMonsterInfo			m_SolderList[MAX_BUYSOLDER_LIST];
	SI16						m_MaxSelectSolder;
	SI16						m_NowSelectSolder;
	OnDismissalSolderInfo		m_DismissalSolderList[MAX_BUYSOLDER_LIST];
	SI16						m_DismissalMaxSelectSolder;
	SI16						m_DismissalNowSelectSolder;
	SI32						m_Discount;
	SI16						m_DeleteItemCount;

public:
	OnlineVillageStructureMBarrack();												// 생성자.
	virtual	~OnlineVillageStructureMBarrack();										// 소멸자.
		
	virtual	VOID	Init(UI16 VillageCode, cltOnlineWorld *pOnlineWorld, VillageStructManager	*pVillageStructManager);// 초기화
	virtual	VOID	Free();															// 해제.
	virtual	BOOL	Poll(LPDIRECTDRAWSURFACE7 pSurface);							// 액션.	
	virtual BOOL	Action();


	virtual BOOL	Draw_Select(LPDIRECTDRAWSURFACE7 pSurface);			// Draw Main
	virtual BOOL	Draw_Check(LPDIRECTDRAWSURFACE7 pSurface, DWORD TickCount);				// Draw Main Check
	virtual BOOL	Draw_DismissalSelect(LPDIRECTDRAWSURFACE7 pSurface);	// Draw Dismissal
	virtual BOOL	Draw_DismissalCheck(LPDIRECTDRAWSURFACE7 pSurface, DWORD TickCount);	// Draw Dismissal Check


	virtual BOOL	CheckLevel(SI32 Lv, SI32 Credit);													// Check Level
	virtual BOOL	CheckItem(SI32 UseItemCount);									// 사용할 아이템이 충분한지 검사
	virtual BOOL	CheckMonsterNum(void);											// 고용할수 있는 몬스터 수가 되는지 검사	

	virtual SI32	GetFollowerBarrackText(UI16 Kind);								// 각 용병별로 Text 지정
	virtual char*	GetMonsterBuyItemName(void);									// 몬스터 고용시 사용하는 아이템 이름
};


#endif
