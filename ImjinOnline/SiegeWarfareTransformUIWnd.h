//////////////////////////////////////////////////////////////////////
//
// 공성전 유닛으로 변신할 때 변신하고자 하는 병과를 보여주는 인터페이스
//
// SiegeWarfareTransformUIWnd.h: interface for the CSiegeWarfareTransformUIWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIEGEWARFARETRANSFORMUIWND_H__A1E4CC32_2696_43C6_917F_0B16907FBCF4__INCLUDED_)
#define AFX_SIEGEWARFARETRANSFORMUIWND_H__A1E4CC32_2696_43C6_917F_0B16907FBCF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include	<GSL.h>
#include	"GSCDefine.h"
#include	<DDraw.h>
#include	"button.h"
#include	<graphics16.h>

class cltOnlineWorld;

struct SAnimationButton
{
	RECT		m_rtDraw;					// 버튼이 그려질 위치
	XSPR		m_sprAnimation;				// 그려질 image
	SI08		m_scCurrentAnimationIdx;	// 현재 그려지는 애니메이션 인덱스
	SI16		m_ssCurrentFont;			// 현재 그려지는 폰트 번호
	SI08		m_scCurrentDelay;			// 애니메이션 딜레이
	SI08		m_scGrayed;					// 비활성화 여부
};

class CSiegeWarfareTransformUIWnd  
{
	public:
		CSiegeWarfareTransformUIWnd();
		virtual ~CSiegeWarfareTransformUIWnd();

	public:
		// 초기화
		void	Init(cltOnlineWorld *pOnlineWorld);

		// 윈도 보이기 설정
		BOOL	ShowWindow(BOOL bVisibleWindow);

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

		// 공성전 상단 공성 유닛 리스트 요청 응답
		// ON_RESPONSE_LIST_GUILD_UNIT
		UI16	OnResponseSiegeWarfareUnitList(BYTE *pMsg);

	protected:
		// 버튼 갱신
		void	UpdateButton(void);

		// 리소스 읽기
		void	LoadResource(void);

		// 리소스 해제
		void	FreeResource(void);

		// 버튼 애니메이션 갱신
		void	UpdateAnimation(void);

	public:
		BOOL	IsSendedMsg(void);			// 변신 메시지를 서버에 전송했는가?

		void	SetSendedMsg(BOOL bSendedMsg);	// 변신 메시지를 서버에 전송했는가를 설정

	protected:
		cltOnlineWorld						*m_pMyOnlineWorld;
		BOOL								m_bVisibleWindow;
		BOOL								m_bSendedMsg;			// 변신 메시지를 서버에 전송했는가?

		SI32								m_siX;
		SI32								m_siY;

		// 공성 유닛 리스트
		SI32								m_siGuildCount[ON_MAX_GUILD_UNIT];			// 상단 내에 존재하는 공성 유닛 수. 수비병 제외
		BOOL								m_bDownloadedGuildCount;					// 상단 내 공성 유닛 리스트를 받아왔는가?

		// No Field Area Idx
		SI32								m_siNoFieldAreaIdx;
/*
		// 변신하려는 병과 캐릭터 이미지 surface로 Stretch 출력을 위함
		LPDIRECTDRAWSURFACE7				m_pSurfaceSprArmy;

		// 생성한 서페이스에서 원본 이미지에서 한 폰트가 차지하는 크기
		SI32								m_siPortraitWidth;
		SI32								m_siPortraitHeight;

		// 서페이스상 병과 영역(크기 및 위치)
		RECT								m_rtSurfaceArmyKind[ON_MAX_GUILD_UNIT];
*/
		// 병과 설명 텍스트 박스
		RECT								m_rtArmyDesc;

		// 버튼들
		_Button								m_btnConfirm;					// 변신(확인)
		_Button								m_btnCancel;					// 취소

		// 이미지
		XSPR								m_sprBackground;				// 배경 이미지
		XSPR								m_sprOkCancel;					// 변신(확인), 취소 버튼 이미지
		XSPR								m_sprSelect;					// 선택한 병과 위에 덧그림

		// 선택한 병과 인덱스
		SI32								m_siSelectArmyKindIdx;
		SI32								m_siPrevSelectArmyKindIdx;

		// 커서 병과 인덱스
		SI32								m_siHoverArmyKindIdx;

		// 그려질 병과 영역(크기 및 위치)
//		RECT								m_rtArmyKind[ON_MAX_GUILD_UNIT];

		// 윈도 타이틀 문자열
		char								m_szTitle[256];

		BOOL								m_bMouseDown;

		SAnimationButton					m_sAnimationButton[ON_MAX_GUILD_UNIT];
};

#endif // !defined(AFX_SIEGEWARFARETRANSFORMUIWND_H__A1E4CC32_2696_43C6_917F_0B16907FBCF4__INCLUDED_)
