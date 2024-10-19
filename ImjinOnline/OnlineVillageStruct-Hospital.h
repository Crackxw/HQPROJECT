/************************************************************************************************
	파일명 : OnlineVillageStruct-Hospital.h
	담당자 : 이윤석
************************************************************************************************/
#ifndef	_ONLINE_VILLAGEBASESTRUCTUREHOSPITAL_HEADER
#define _ONLINE_VILLAGEBASESTRUCTUREHOSPITAL_HEADER


#include <DDraw.h>
#include "Online.h"
#include "OnlineVillageBaseBarrack.h"


#define HOSPITAL_NOWWORK_MAIN_START						1
#define HOSPITAL_NOWWORK_MAIN_STARTPOOL					2
#define HOSPITAL_NOWWORK_MAIN_POOL						3
#define HOSPITAL_NOWWORK_CHECK_POOL						4
#define HOSPITAL_NOWWORK_CHECK_POOL_RETURNWAIT			5
#define HOSPITAL_NOWWORK_CHECK_POOL_RETURNWAIT1			6
#define HOSPITAL_NOWWORK_MAIN_END						7
#define HOSPITAL_NOWWORK_MAIN_ENDPOOL					8	

#define MAX_CURESOLDER_LIST								128

 
typedef struct
{
	UI08			m_Slot;							// Slot
	MONEY			m_Money;						// 치료 가격
	MONEY			m_RevivalMoney;					// 부활 가격

	UI16			m_Kind;							// 캐릭터 종류
	char			m_Name[ON_ID_LENGTH + 1];		// 캐릭터 Name
	SI32			m_Life;							// 현재 Life
	SI32			m_MaxLife;						// 최대 Life
	SI32			m_Mana;							// 현재 Mana
	SI32			m_MaxMana;						// 최대 Mana
} SCureSolderData;


class OnlineVillageStructureHospital : public OnlineVillageBaseBarrack
{
private:
	// Image
	XSPR						m_Image_Background;
	XSPR						m_Image_MainInterface_Back;
//	XSPR*						m_Image_CharacterSlotInterface;

	XSPR						m_Image_Button_All_EM;
	XSPR						m_Image_Button_All_BM;

	XSPR						m_Image_Portrait;

	// Scroll
	SI16						m_NowScroll;
	SI16						m_siX,m_siY;

	// Button
	_Button						m_Button_Cure;
	_Button						m_Button_CureAll;
	_Button						m_Button_CureOK;
	_Button						m_Button_CureCancel;


	VillageStructManager		*m_pMyVillage;

public:
	// Base
	SI16						m_NowWork;
	SI16						m_Say;

	// Solder Data
	SCureSolderData				m_CureSolderList[MAX_CURESOLDER_LIST];
	SI16						m_MaxSelectSolder;
	SI16						m_NowSelectSolder;
	SI32						m_Discount;

	DWORD						m_StartTickCount;


public:
	OnlineVillageStructureHospital();																							// 생성자.
	virtual	~OnlineVillageStructureHospital();																					// 소멸자.
		
	virtual	VOID				Init(UI16 VillageCode, cltOnlineWorld *pOnlineWorld, VillageStructManager	*pVillageStructManager);	// 초기화
	virtual	VOID				Free();																							// 해제.
	virtual	BOOL				Poll(LPDIRECTDRAWSURFACE7 pSurface);															// 액션.
	virtual BOOL				Action();

	virtual BOOL				Draw_Select(LPDIRECTDRAWSURFACE7 pSurface);									// Draw Main

	virtual VOID				FindFollowerData(void);																			// 현재 Follower의 상태를 검색
};


#endif
