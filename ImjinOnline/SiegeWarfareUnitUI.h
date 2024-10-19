//////////////////////////////////////////////////////////////////////
//
// 공성전 유닛일 때 화만 좌측 상단에 보여지는 인터페이스
// 정보보기, 방어력 향상, 적대 상단 목록, 나가기 버튼 등
//
// SiegeWarfareUnitUI.h: interface for the CSiegeWarfareUnitUI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIEGEWARFAREUNITUI_H__F380C79B_25D4_433F_8268_DEABF2F2EA9E__INCLUDED_)
#define AFX_SIEGEWARFAREUNITUI_H__F380C79B_25D4_433F_8268_DEABF2F2EA9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include	<GSL.h>
#include	"GSCDefine.h"
#include	<DDraw.h>
#include	"button.h"
#include	<graphics16.h>

class cltOnlineWorld;
class OnlineMsgBox;

class CSiegeWarfareUnitUI  
{
	public:
		CSiegeWarfareUnitUI();
		virtual ~CSiegeWarfareUnitUI();

	protected:
		// 초기화
		void	Init();

	public:
		// 초기화
		void	Init(cltOnlineWorld *pOnlineWorld);

		// 윈도 보이기 설정
		void	ShowWindow(DWORD dwShowWindow);

		// 처리
		BOOL	Action(void);

		// 그리기
		void	Draw(LPDIRECTDRAWSURFACE7 pSurface);

		// 그리기
		void	DrawImage(LPDIRECTDRAWSURFACE7 pSurface);

		// 그리기
		void	DrawText(LPDIRECTDRAWSURFACE7 pSurface);

		// 윈도 보이는가?
		BOOL	IsVisibleWindow(void);

		// 윈도가 최소화 되었는가?
		BOOL	IsMinimizeWindow(void);

	public:
		// 윈도 생성
		BOOL	SiegeWarfareUnitUI_CreateWindow(DWORD dwShowWindow);

		// 윈도 파괴
		BOOL	SiegeWarfareUnitUI_DestroyWindow(void);

	protected:
		// 버튼 갱신
		void	UpdateButton(void);

		// 리소스 읽기
		void	LoadResource(void);

		// 리소스 해제
		void	FreeResource(void);

	public:
		// 인터페이스 기능 사용할 때 주어지는 딜레이 설정
		void	SetCurrentDelayTimeUsingVillageAttackType(SI32 siDelay);
		void	SetMaxDelayTimeUsingVillageAttackType(SI32 siDelay);

		void	SetVillageCode(UI16 uiVillageCode);			// 마을 코드

	protected:
		cltOnlineWorld		*m_pMyOnlineWorld;
		DWORD				m_dwShowWindow;				// 윈도 보이기 옵션

		BOOL				m_bCreatedWindow;			// 윈도 생성되었는가?

		BOOL				m_bDrawManaGauge;			// 마나 게이지를 그려야 하나?

		// 이미지
		XSPR				m_sprMainInterface;			// 인터페이스 메인 배경 이미지
		XSPR				m_sprShowUnitInformation;	// 인터페이스 버튼 이미지 - 모든 공성 유닛 정보 보기
		XSPR				m_sprFunctionBtnImage;		// 인터페이스 버튼 이미지 - 기능 버튼
		XSPR				m_sprEnterVillage;			// 인터페이스 버튼 이미지 - 수성 인터페이스에 현재 마을로 들어가기
		XSPR				m_sprTimer;					// 인터페이스에서 수성 기능을 사용할 수 있는지를 표시하는 타이머 이미지

		XSPR				m_sprStatusMain;			// 공성 유닛 상태 정보 윈도 배경 이미지
		XSPR				m_sprGaugeRed;				// 게이지 red
		XSPR				m_sprGaugeBlue;				// 게이지 blue
		XSPR				m_sprBtnMinimize;			// 최소화 버튼 이미지
		XSPR				m_sprPortrait;				// 초상화 이미지

		// 버튼
		_Button				m_btnShowUnitInformation;	// 수성 인터페이스 버튼 - 모든 공성 유닛 정보 보기 토글 버튼
		_Button				m_btnShowEmemyGuildList;	// 수성 인터페이스 버튼 - 적대 마을 목록 보기
		_Button				m_btnDefenceUp;				// 수성 인터페이스 버튼 - 수성 기능 버튼(방어력 향상)
		_Button				m_btnEnterVillage;			// 수성 인터페이스 버튼 - 수성 인터페이스에 현재 마을로 들어가기

		_Button				m_btnToNormalChar;			// 공성 유닛에서 일반 유닛으로 변신 요청 버튼

		_Button				m_btnMinimize;				// 윈도 최소화 버튼
		_Button				m_btnRestore;				// 윈도 보이기 복원 버튼

		// 인터페이스 위치를 위한 No Field Area Handle
		SI32				m_siNoFieldAreaHandle;
		SI32				m_siNoFieldAreaHandle_BtnMinimize;

		// 인터페이스 기능 사용할 때 주어지는 딜레이
		SI32				m_siCurrentDelayTimeUsingVillageAttackType;
		SI32				m_siMaxDelayTimeUsingVillageAttackType;

		UI16				m_uiVillageCode;			// 마을 코드

		char				m_szArmyKindName[64];		// 변신한 공성 유닛 이름

		BOOL				m_bMouseDown;

		OnlineMsgBox		*m_pConfirmToGiveUpBattleMsgBox;	// 공성 전투 포기를 원하는지 묻기 대화 상자
};

#endif // !defined(AFX_SIEGEWARFAREUNITUI_H__F380C79B_25D4_433F_8268_DEABF2F2EA9E__INCLUDED_)
