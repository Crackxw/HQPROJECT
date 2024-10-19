//////////////////////////////////////////////////////////////////////
//
// 공성전 유닛일 때 화만 좌측 상단에 보여지는 인터페이스
// 정보보기, 방어력 향상, 적대 상단 목록, 나가기 버튼 등
//
// SiegeWarfareUnitUI.cpp: implementation of the CSiegeWarfareUnitUI class.
//
//////////////////////////////////////////////////////////////////////

#include <GSL.h>
#include	<Mouse.h>

#include	"OnlineWorld.h"

#include	<OnlineHelp.h>
#include	<OnlineMegaText.h>
#include	<OnlineTradeBook.h>
#include	<OnlineTip.h>
#include	<OnlineFont.h>
#include	<OnlineSMS.h>		// actdoll (2004/06/15 10:46) : SMS 용 데이터 추가
#include	<OnlinePannel.h>
#include	<OnlineClient.h>
#include	<OnlineMyData.h>
#include	<OnlineVillage.h>
#include	<OnlineText.h>
#include	"OnlineMiniMap.h"
#include	<OnlineMsgBox.h>
#include	<OnlineMap.h>
#include	<OnlineListControl.h>
#include	<OnlineInventory.h>
#include	<OnlineFieldArea.h>
#include	"OnlineVillageManager.h"
#include	"OnlineSetting.h"
#include	<DebugTool.h>
#include	"OnlineQuestInterface.h"
#include	"OnlineMercenary.h"
#include	"OnlineMsgBox.h"
#include	"OnlineCharUI.h"
#include	<OnlineTrade.h>
#include	"OnlineQuestPlay.h"

#include	<kindinfo.h>

#include	"SiegeWarfareUnitUI.h"

// 공성 유닛 관리자
#include	"SiegeUnitTableMgr.h"

extern _InputDevice				IpD;

extern	_KindInfo					KI[];								// 캐릭터 종류별 데이터 

// robypark 2005/1/7 18:41
// 수치를 게이지 형태로 그려준다.
// 마을 방어력, 수비병 게이지 출력에서 사용
extern void DrawGaugeVillage(SI32 siX, SI32 siY, SI32 siRate, XSPR *psprImage);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSiegeWarfareUnitUI::CSiegeWarfareUnitUI()
{
	m_pMyOnlineWorld		= NULL;

	Init();
}

CSiegeWarfareUnitUI::~CSiegeWarfareUnitUI()
{
	SiegeWarfareUnitUI_DestroyWindow();

	m_pMyOnlineWorld		= NULL;
}

// 초기화
void	CSiegeWarfareUnitUI::Init(cltOnlineWorld *pOnlineWorld)
{
	Init();

	m_pMyOnlineWorld		= pOnlineWorld;
}

// 초기화
void	CSiegeWarfareUnitUI::Init()
{
	m_dwShowWindow			= SW_HIDE;
	m_bCreatedWindow		= FALSE;
	m_bDrawManaGauge		= FALSE;

	m_pConfirmToGiveUpBattleMsgBox	= NULL;

	ZeroMemory(m_szArmyKindName, sizeof(m_szArmyKindName));

	m_siNoFieldAreaHandle	= -1;
	m_siNoFieldAreaHandle_BtnMinimize	= -1;
	m_siCurrentDelayTimeUsingVillageAttackType	= 0;
	m_siMaxDelayTimeUsingVillageAttackType		= 0;
}

// 윈도 보이기 설정
void	CSiegeWarfareUnitUI::ShowWindow(DWORD dwShowWindow)
{
	// 보이기 상태 변화가 없는 경우
	if (m_dwShowWindow == dwShowWindow)
		return;

	// 초기화 되지 않은 경우
	if (!m_bCreatedWindow)
		return;

	m_dwShowWindow = dwShowWindow;

	// 활성화
	if (SW_NORMAL == m_dwShowWindow)
	{
		UpdateButton();

		if (-1 != m_siNoFieldAreaHandle)
		{
			// No Field Area 해제
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoFieldAreaHandle);
			m_siNoFieldAreaHandle = -1;
		}

		// No Field Area 설정
		m_siNoFieldAreaHandle = m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(0, 0, 115, 200);

		if (-1 != m_siNoFieldAreaHandle_BtnMinimize)
		{
			// No Field Area 해제
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoFieldAreaHandle_BtnMinimize);
			m_siNoFieldAreaHandle_BtnMinimize = -1;
		}

		// No Field Area 설정
		m_siNoFieldAreaHandle_BtnMinimize = m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(10, 115 + 96, 10 + m_sprBtnMinimize.Header.Xsize, 115 + 96 + 3 + m_sprBtnMinimize.Header.Ysize + 3);
	}
	// 최소화
	else if (SW_MINIMIZE == m_dwShowWindow)
	{
		UpdateButton();

		if (-1 != m_siNoFieldAreaHandle)
		{
			// No Field Area 해제
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoFieldAreaHandle);
			m_siNoFieldAreaHandle = -1;
		}

		// No Field Area 설정
		m_siNoFieldAreaHandle = m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(0, 0, 13 + m_sprBtnMinimize.Header.Xsize + 3, 3 + m_sprBtnMinimize.Header.Ysize + 3);

		if (-1 != m_siNoFieldAreaHandle_BtnMinimize)
		{
			// No Field Area 해제
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoFieldAreaHandle_BtnMinimize);
			m_siNoFieldAreaHandle_BtnMinimize = -1;
		}
	}
	else	// SW_HIDE
	{
		if (-1 != m_siNoFieldAreaHandle)
		{
			// No Field Area 해제
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoFieldAreaHandle);
			m_siNoFieldAreaHandle = -1;
		}

		if (-1 != m_siNoFieldAreaHandle_BtnMinimize)
		{
			// No Field Area 해제
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoFieldAreaHandle_BtnMinimize);
			m_siNoFieldAreaHandle_BtnMinimize = -1;
		}
	}
}

// 처리
BOOL	CSiegeWarfareUnitUI::Action(void)
{
	// 윈도 감쳐진 경우
	if (SW_HIDE == m_dwShowWindow)
		return FALSE;

	// 초기화 되지 않은 경우
	if (!m_bCreatedWindow)
		return FALSE;

	UpdateButton();

	// 공성 전투 포기 메시지 박스 처리
	if (m_pConfirmToGiveUpBattleMsgBox)
	{
		SI32 siResult = m_pConfirmToGiveUpBattleMsgBox->Process();

		if (ON_MSGBOX_RETURN_OK == siResult)
		{
			// 서버에 공성 전투 포기 요청
			m_pMyOnlineWorld->pOnlineClient->SendRequestNormalCharacter(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID());

			// 메시지 박스 삭제
			delete m_pConfirmToGiveUpBattleMsgBox;
			m_pConfirmToGiveUpBattleMsgBox = NULL;

			// 사용자 입력 활성화
			m_pMyOnlineWorld->pOnlineCharUI->SetAction(TRUE);
		}
		else if (ON_MSGBOX_RETURN_CANCEL == siResult)
		{
			// 메시지 박스 삭제
			delete m_pConfirmToGiveUpBattleMsgBox;
			m_pConfirmToGiveUpBattleMsgBox = NULL;

			// 사용자 입력 활성화
			m_pMyOnlineWorld->pOnlineCharUI->SetAction(TRUE);
		}
	}

	// 윈도 최소화 경우
	if (SW_MINIMIZE == m_dwShowWindow)
	{
		// 복원 버튼 처리
		if (m_btnRestore.Process(m_bMouseDown))
		{
			ShowWindow(SW_NORMAL);
		}

		if (m_btnRestore.IsInMouse())
		{
			// 풍선 도움말 처리하기
			char szTooltipText[1024];
			ZeroMemory(szTooltipText, sizeof(szTooltipText));

//			sprintf(szTooltipText, "%s", m_pMyOnlineWorld->pOnlineText->Get());
			sprintf(szTooltipText, m_pMyOnlineWorld->pOnlineText->Get(8100132));

			m_pMyOnlineWorld->pOnlineHelp->SetText( szTooltipText, IpD.Mouse_X, IpD.Mouse_Y, ON_HELP_RIGHT | ON_HELP_VCENTER, ON_HELP_WHITE, TRUE );
		}

		if( IpD.LeftPressSwitch )
			m_bMouseDown	=	TRUE;
		else						
			m_bMouseDown	=	FALSE;

		return TRUE;
	}

	// 인터페이스 버튼 처리
	// 공성전 유닛 상태 보기 버튼 처리
	SI08 scIsShowInformationAboutSiegeWarfareUnit = m_pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit();
	if (m_btnShowUnitInformation.Process(m_bMouseDown))
	{
		// 현재 공성 유닛 상태 정보 보이기 라면
		if (scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT)
		{
			// 현재 공성 유닛 상태 정보 안보이기로 설정
			scIsShowInformationAboutSiegeWarfareUnit -= ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT;
			m_pMyOnlineWorld->SetIsShowInformationAboutSiegeWarfareUnit(scIsShowInformationAboutSiegeWarfareUnit);
		}
		else
		{
			// 현재 공성 유닛 상태 정보 보이기로 설정
			scIsShowInformationAboutSiegeWarfareUnit += ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT;
			m_pMyOnlineWorld->SetIsShowInformationAboutSiegeWarfareUnit(scIsShowInformationAboutSiegeWarfareUnit);
		}
	}
	/*
	// 적대 상단  리스트 보기
	else if (m_btnShowEmemyGuildList.Process(m_bMouseDown))
	{
	}
	*/
	// 마을 방어력 향상
	else if (m_btnDefenceUp.Process(m_bMouseDown))
	{
		// 마을 데이터 존재, 공격 지연시간 모두 경과
		if ((m_uiVillageCode)
			&& (0 == m_siCurrentDelayTimeUsingVillageAttackType)
			&& (m_pMyOnlineWorld->pMyData->IsInVillageDefenceInterface()))
		{
			m_pMyOnlineWorld->pOnlineClient->SendVillageDefenceUp(m_uiVillageCode);
		}
	}
	// 수성인터페이스=>나가기 버튼
	else if (m_btnEnterVillage.Process(m_bMouseDown))
	{
		// 나가겠다고 요청
		if (m_uiVillageCode)
			m_pMyOnlineWorld->pOnlineClient->SendLeaveVillageDefenceInterface(m_uiVillageCode);
	}
	// 공성 정보창 최소화 버튼 처리
	else if (m_btnMinimize.Process(m_bMouseDown))
	{
		ShowWindow(SW_MINIMIZE);
	}
	// 공성 전투 포기
	else if (m_btnToNormalChar.Process(m_bMouseDown))
	{
		if ((FALSE == m_pMyOnlineWorld->pOnlineTrade->m_pMsgBox->IsActive())				// 거래 요청 중이 아니고
				&& (FALSE == m_pMyOnlineWorld->pOnlineTrade->IsActive())					// 거래 중이 아니고
				&& (FALSE == m_pMyOnlineWorld->pMyData->IsWaitBattleResponse())			// 전투 대기중일 때가 아니고
				&& (FALSE == m_pMyOnlineWorld->pMyData->IsGotoVillage()) 				// 마을에 들어가는 중이거나 아니고
				&& (FALSE == m_pMyOnlineWorld->pMyData->IsWaitGotoVillageResponse())		// 마을 들어가기 위한 대기 중이라면 아니고
				&& (FALSE == m_pMyOnlineWorld->pIOnlineChar->IsMoving(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID()))	// 이동 중이라면 아니고
				&& (FALSE == m_pMyOnlineWorld->pOnlineMsgBoxUsePortal->IsActive())		// 포탈을 이용하려는 경우가 아니고
				&& (FALSE == m_pMyOnlineWorld->pOnlineQuestPlay->IsAction())				// NPC에게 퀘스트를 받는 중이 아니고
				&& (NULL == m_pConfirmToGiveUpBattleMsgBox))					// 전투 포기 메시지 박스가 열리지 않은 경우
		{
			m_pConfirmToGiveUpBattleMsgBox = new OnlineMsgBox;
			m_pConfirmToGiveUpBattleMsgBox->Init(m_pMyOnlineWorld);

			m_pConfirmToGiveUpBattleMsgBox->SetMsg(ON_MSGBOX_TYPE_OKCANCEL, m_pMyOnlineWorld->pOnlineText->Get(8100128), m_pMyOnlineWorld->pOnlineText->Get(8100130));

			// 사용자 입력 비활성화
			m_pMyOnlineWorld->pOnlineCharUI->SetAction(FALSE);
		}
	}

	// 수성 인터페이스 버튼 풍선 도움말 처리하기
	char szTooltipText[1024];
	ZeroMemory(szTooltipText, sizeof(szTooltipText));

	// 공성전 유닛 상태 보기 버튼 처리
	if (m_btnShowUnitInformation.IsInMouse())
	{
		sprintf(szTooltipText, "%s", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TOOL_TIP_SHOW_SIEGEWARFAREUNIT_INFO));
	}
	/*
	// 적대 상단 리스트 보기
	else if (m_btnShowEmemyGuildList.IsInMouse())
	{
	}
	*/
	// 마을 방어력 향상
	else if (m_btnDefenceUp.IsInMouse())
	{
		char szCost[32];
		char szDefenceUpAmount[32];
		char szCoolTime[32];

		LPSOptionSiegeWarfare pSOptionSiegeWarfare = m_pMyOnlineWorld->GetSOptionSiegeWarfare();
		MakeMoneyText(pSOptionSiegeWarfare->siVillageCounterAttack_DefenceUp_Amount, szDefenceUpAmount);	// 방어력 향상되는 량
		MakeMoneyText(pSOptionSiegeWarfare->siVillageCounterAttack_DefenceUp_Cost, szCost);	// 비용
		MakeMoneyText(pSOptionSiegeWarfare->siVillageCounterAttack_DefenceUp_CoolTime, szCoolTime);	// cool time

		sprintf(szTooltipText, 
					m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TOOLTIP_VILLAGE_DEFENCE_UP),
						szDefenceUpAmount,
						szCost, 
						szCoolTime);
	}
	// 수성인터페이스=>마을 들어가기 버튼
	else if (m_btnEnterVillage.IsInMouse())
	{
		sprintf(szTooltipText, "%s", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TOOLTIP_VILLAGEDEFENCEINTERFACE_OUT));
	}
	// 공성 정보창 최소화 버튼
	else if (m_btnMinimize.IsInMouse())
	{
//		sprintf(szTooltipText, "%s", m_pMyOnlineWorld->pOnlineText->Get());
		sprintf(szTooltipText, m_pMyOnlineWorld->pOnlineText->Get(8100131));
	}
	else if (m_btnToNormalChar.IsInMouse())
	{
		sprintf(szTooltipText, m_pMyOnlineWorld->pOnlineText->Get(8100129));
	}

	// 풍선 도움말 표시할 게 있으면, 
	if ('\0' != szTooltipText[0])
		m_pMyOnlineWorld->pOnlineHelp->SetText( szTooltipText, IpD.Mouse_X, IpD.Mouse_Y, ON_HELP_RIGHT | ON_HELP_VCENTER, ON_HELP_WHITE, TRUE );

	if( IpD.LeftPressSwitch )
		m_bMouseDown	=	TRUE;
	else						
		m_bMouseDown	=	FALSE;

	return TRUE;
}

// 그리기
void	CSiegeWarfareUnitUI::Draw(LPDIRECTDRAWSURFACE7 pSurface)
{
	// 윈도가 보이지 않는 경우
	if (SW_HIDE == m_dwShowWindow)
		return;

	// 초기화 되지 않은 경우
	if (!m_bCreatedWindow)
		return;

	// 윈도 이미지들 그리기
	DrawImage(pSurface);

	// 윈도 텍스트들 그리기
	DrawText(pSurface);

	// 메시지 박스가 열린 경우 그리기
	if (m_pConfirmToGiveUpBattleMsgBox)
	{
		m_pConfirmToGiveUpBattleMsgBox->Draw(pSurface);
	}
}

// 그리기
void	CSiegeWarfareUnitUI::DrawImage(LPDIRECTDRAWSURFACE7 pSurface)
{
	// 이미지들 출력
	if( clGrp.LockSurface( pSurface ) == TRUE )
	{
		// 활성화된 윈도인 경우
		if (SW_NORMAL == m_dwShowWindow)
		{
			// 메인 이미지 출력
			clGrp.PutSpriteT(0, 0, m_sprStatusMain.Header.Xsize, m_sprStatusMain.Header.Ysize, m_sprStatusMain.Image);

			clGrp.PutSpriteT(0, 96, m_sprMainInterface.Header.Xsize, m_sprMainInterface.Header.Ysize, m_sprMainInterface.Image);

			// 공성전 유닛 초상화 그리기
			clGrp.PutSpriteT(8, 8, m_sprPortrait.Header.Xsize, m_sprPortrait.Header.Ysize, m_sprPortrait.Image);

			// 버튼 이미지 출력
			// 최소화 버튼
			m_btnMinimize.Put(&m_sprBtnMinimize, 0, 2, 1, BUTTON_PUT_NOMOVE);

			UI16 uiMyCharUniqueID = m_pMyOnlineWorld->pMyData->GetMyCharUniqueID();

			// 게이지 출력
			// 체력
			SI32 siRate = ((float)m_pMyOnlineWorld->pIOnlineChar->GetSiegeWarfare_Hp(uiMyCharUniqueID)
							/ (float)m_pMyOnlineWorld->pIOnlineChar->GetSiegeWarfare_MaximumHP(uiMyCharUniqueID)) * 100;

			DrawGaugeVillage(99, 10, siRate, &m_sprGaugeRed);

			// 마나
			if (m_bDrawManaGauge)
			{
				siRate = ((float)m_pMyOnlineWorld->pIOnlineChar->GetSiegeWarfare_CurrentSupplyGoodsAmount(uiMyCharUniqueID)
							/ (float)m_pMyOnlineWorld->pIOnlineChar->GetSiegeWarfare_SupplyGoodsAmount(uiMyCharUniqueID)) * 100;

				DrawGaugeVillage(99, 34, siRate, &m_sprGaugeBlue);
			}

			SI08 scIsShowInformationAboutSiegeWarfareUnit = m_pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit();

			// 공성전 유닛 정보 보기
			// 마우스 커서가 버튼위에 있다면,
			if (m_btnShowUnitInformation.IsInMouse())
			{
				m_btnShowUnitInformation.PutSpriteButton(
														m_btnShowUnitInformation.GetX(), 
														m_btnShowUnitInformation.GetY(), 
														&m_sprShowUnitInformation, 
														1, FALSE, FALSE);
			}
			// 현재 설정이 상태 보이기 라면
			else if (scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT)
			{
				m_btnShowUnitInformation.PutSpriteButton(
														m_btnShowUnitInformation.GetX(), 
														m_btnShowUnitInformation.GetY(), 
														&m_sprShowUnitInformation, 
														2, FALSE, FALSE);
			}
			else if (0 == (scIsShowInformationAboutSiegeWarfareUnit & ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT))
			{
				m_btnShowUnitInformation.PutSpriteButton(
														m_btnShowUnitInformation.GetX(), 
														m_btnShowUnitInformation.GetY(), 
														&m_sprShowUnitInformation, 
														0, FALSE, FALSE);
			}
			////////////////////////////////////////

			/*
			// 적대 상단 리스트 보기
			m_btnShowEmemyGuildList.Put(&m_sprFunctionBtnImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
			*/
			// 마을 방어력 향상
			m_btnDefenceUp.Put(&m_sprFunctionBtnImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
			// 나가기
			m_btnEnterVillage.Put(&m_sprFunctionBtnImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
			// 공성 전투 포기
			m_btnToNormalChar.Put(&m_sprFunctionBtnImage, 0, 2, 1, BUTTON_PUT_NOMOVE);

			// 다음 마을 반격 기능 사용을 위하여 남은 시간 표시
			if ((m_siMaxDelayTimeUsingVillageAttackType) && (m_pMyOnlineWorld->pMyData->IsInVillageDefenceInterface()))
			{
				// 경과된 시간 비율 계산
				float fRemainTimePct = (float)m_siCurrentDelayTimeUsingVillageAttackType / (float)m_siMaxDelayTimeUsingVillageAttackType;

				if (0 == m_siCurrentDelayTimeUsingVillageAttackType)
					clGrp.PutSpriteT(92, 48, m_sprTimer.Header.Xsize, m_sprTimer.Header.Ysize, &m_sprTimer.Image[m_sprTimer.Header.Start[7]]);
				else if (fRemainTimePct > 0.858f)
					clGrp.PutSpriteT(92, 48, m_sprTimer.Header.Xsize, m_sprTimer.Header.Ysize, &m_sprTimer.Image[m_sprTimer.Header.Start[6]]);
				else if (fRemainTimePct > 0.715f)
					clGrp.PutSpriteT(92, 48, m_sprTimer.Header.Xsize, m_sprTimer.Header.Ysize, &m_sprTimer.Image[m_sprTimer.Header.Start[5]]);
				else if (fRemainTimePct > 0.572f)
					clGrp.PutSpriteT(92, 48, m_sprTimer.Header.Xsize, m_sprTimer.Header.Ysize, &m_sprTimer.Image[m_sprTimer.Header.Start[4]]);
				else if (fRemainTimePct > 0.429f)
					clGrp.PutSpriteT(92, 48, m_sprTimer.Header.Xsize, m_sprTimer.Header.Ysize, &m_sprTimer.Image[m_sprTimer.Header.Start[3]]);
				else if (fRemainTimePct > 0.286f)
					clGrp.PutSpriteT(92, 48, m_sprTimer.Header.Xsize, m_sprTimer.Header.Ysize, &m_sprTimer.Image[m_sprTimer.Header.Start[2]]);
				else if (fRemainTimePct > 0.143f)
					clGrp.PutSpriteT(92, 48, m_sprTimer.Header.Xsize, m_sprTimer.Header.Ysize, &m_sprTimer.Image[m_sprTimer.Header.Start[1]]);
				else
					clGrp.PutSpriteT(92, 48, m_sprTimer.Header.Xsize, m_sprTimer.Header.Ysize, &m_sprTimer.Image[m_sprTimer.Header.Start[0]]);
			}
	//		else
	//			clGrp.PutSpriteT(92, 48, m_sprTimer.Header.Xsize, m_sprTimer.Header.Ysize, &m_sprTimer.Image[m_sprTimer.Header.Start[7]]);
		}
		else	// minimize
		{
			clGrp.PutSprite(0, 0,
								m_sprMainInterface.Header.Xsize,
								m_sprMainInterface.Header.Ysize,
								0,
								110,
								m_sprMainInterface.Header.Xsize,
								m_sprMainInterface.Header.Ysize - 110,
								m_sprMainInterface.Image);

			// 복원 버튼 이미지 그리기
			m_btnRestore.Put(&m_sprBtnMinimize, 0, 2, 1, BUTTON_PUT_NOMOVE);
		}

		clGrp.UnlockSurface( pSurface );
	}
}

// 그리기
void	CSiegeWarfareUnitUI::DrawText(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC			hDC;

	// 텍스트 출력
	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		SetBkMode( hDC, TRANSPARENT );
		SelectObject( hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );

		// 일반 활성화된 상태
		if (SW_NORMAL == m_dwShowWindow)
		{
			char szTemp1[64], szTemp2[64], szTemp3[128];

			UI16 uiMyCharUniqueID = m_pMyOnlineWorld->pMyData->GetMyCharUniqueID();

			// 공성전 유닛 상태 출력
			MakeMoneyText(m_pMyOnlineWorld->pIOnlineChar->GetSiegeWarfare_Hp(uiMyCharUniqueID), szTemp1);
			MakeMoneyText(m_pMyOnlineWorld->pIOnlineChar->GetSiegeWarfare_MaximumHP(uiMyCharUniqueID), szTemp2);
			sprintf(szTemp3, "%s / %s", szTemp1, szTemp2);

			// 체력
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, 99 + 1,	10 + 1,	122, 15, szTemp3, RGB(0, 0, 0));
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, 99,		10,		122, 15, szTemp3, RGB(250, 250, 250));

			// 마나
			if (m_bDrawManaGauge)
			{
				MakeMoneyText(m_pMyOnlineWorld->pIOnlineChar->GetSiegeWarfare_CurrentSupplyGoodsAmount(uiMyCharUniqueID), szTemp1);
				MakeMoneyText(m_pMyOnlineWorld->pIOnlineChar->GetSiegeWarfare_SupplyGoodsAmount(uiMyCharUniqueID), szTemp2);
				sprintf(szTemp3, "%s / %s", szTemp1, szTemp2);

				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, 99 + 1,	34 + 1, 122, 15, szTemp3, RGB(0, 0, 0));
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, 99,		34,		122, 15, szTemp3, RGB(250, 250, 250));
			}

			// 변신한 병과 이름
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, 102, 76, 86, 15, m_szArmyKindName, RGB(250, 250, 250));

			// 공성전 유닛 정보 보기
			m_btnShowUnitInformation.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			/*
			// 적대 상단 리스트 보기
			m_btnShowEmemyGuildList.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			*/
			// 마을 방어력 향상
			m_btnDefenceUp.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			// 마을 들어가기
			m_btnEnterVillage.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			// 공성 전투 포기
			m_btnToNormalChar.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		}
		else	// minimize
		{
			/*
			// 복원 버튼
			m_btnRestore.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			*/
		}

		pSurface->ReleaseDC( hDC );
	}
}

// 버튼 갱신
void	CSiegeWarfareUnitUI::UpdateButton(void)
{
	if (SW_NORMAL != m_dwShowWindow)
		return;

	// 마을 방어력 향상
	m_btnDefenceUp.SetAction(FALSE);

	// 수성 인터페이스 상 캐릭터라면
	if (m_pMyOnlineWorld->pMyData->IsInVillageDefenceInterface())
	{
		// 수성인터페이스에서 나가기
		m_btnEnterVillage.ShowWindow(TRUE);

		// 공성 전투 포기
		m_btnToNormalChar.ShowWindow(FALSE);
	}
	else
	{
		// 수성인터페이스에서 나가기
		m_btnEnterVillage.ShowWindow(FALSE);

		// 공성 전투 포기
		m_btnToNormalChar.ShowWindow(TRUE);
	}

	/*
	// 적대 상단 리스트 보기
	m_btnShowEmemyGuildList.SetAction(FALSE);
	*/

	LPSSiegeUnitTable lpsSiegeUnitTable = m_pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTable(m_pMyOnlineWorld->pMyData->GetMyCharKind());

	// 공성전 유닛 중 수성을 위한 마을 출입이 가능한 유닛인지 검사
	if ((lpsSiegeUnitTable)
		&& (lpsSiegeUnitTable->m_scTransformOption & ON_ITEM_SIEGEWARFARE_USABLE_VILLAGE_DEFENCE))
	{
		// 수성 인터페이스 상 캐릭터라면
		if (m_pMyOnlineWorld->pMyData->IsInVillageDefenceInterface())
		{
			if (0 == m_siCurrentDelayTimeUsingVillageAttackType)
			{
				// 마을 방어력 향상
				m_btnDefenceUp.SetAction(TRUE);
			}
		}
	}
}

// 리소스 읽기
void	CSiegeWarfareUnitUI::LoadResource(void)
{
	// 어떠한 공성 유닛인가?
	LPSSiegeUnitTable lpsSiegeUnitTable = m_pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTable(m_pMyOnlineWorld->pMyData->GetMyCharKind());

	// 마나를 가지고 있는 캐릭터인가?
	if (KI[TransKindOnlineToImjin(lpsSiegeUnitTable->m_uiKind, FALSE)].IsAttribute1(ATB_STORE_AND_SUPPLY_SUPPLYGOODS))
	{
		m_bDrawManaGauge = TRUE;
		clGrp.LoadXspr("Online\\GameSpr\\Panel\\SIEGE_PORT_BASE.Spr",	m_sprStatusMain);			// 공성 유닛 상태 정보 윈도 배경 이미지
	}
	else
	{
		m_bDrawManaGauge = FALSE;
		clGrp.LoadXspr("Online\\GameSpr\\Panel\\SIEGE_PORT_BASE_X.Spr",	m_sprStatusMain);			// 공성 유닛 상태 정보 윈도 배경 이미지
	}
	
	// 병과 이름 얻기
	ZeroMemory(m_szArmyKindName, sizeof(m_szArmyKindName));
	strcpy(m_szArmyKindName, m_pMyOnlineWorld->pOnlineText->Get(lpsSiegeUnitTable->m_siUnitNameTextIdx));

	// 이미지 로드 /////////////////////////////////////////
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\SIEGE_BTN_B.Spr",			m_sprShowUnitInformation);	// 모든 공성전 유닛 정보 보기
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\SIEGE_BTN_A.Spr",			m_sprFunctionBtnImage);		// 기능 버튼 이미지
//	clGrp.LoadXspr("Online\\GameSpr\\Panel\\SIEGE_BTN_C.Spr",			m_sprEnterVillage);			// 수성인터페이스=>마을 들어가기 버튼 이미지
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\SIEGE_DEFENSE_BASE.Spr",	m_sprMainInterface);		// 인터페이스 메인 이미지
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\SIEGE_BTN_TIME_GAGE.Spr",	m_sprTimer);				// 수성 기능 사용 남은 시간 표시 이미지

	clGrp.LoadXspr("Online\\GameSpr\\Panel\\SIEGE_PORT_BASE_GAUGE_0.Spr",	m_sprGaugeRed);			// 게이지 red
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\SIEGE_PORT_BASE_GAUGE_1.Spr",	m_sprGaugeBlue);		// 게이지 blue
	clGrp.LoadXspr("Online\\GameSpr\\Panel\\SIEGE_BTN_X.Spr",				m_sprBtnMinimize);		// 최소화 버튼 이미지
	clGrp.LoadXspr(lpsSiegeUnitTable->m_szPortraitFileName,					m_sprPortrait);			// 초상화 이미지

	// 버튼 생성 /////////////////////////////////////////
	// 최소화 버튼
	m_btnMinimize.Create(13, 115 + 96 + 3, 
					m_sprBtnMinimize.Header.Xsize,	m_sprBtnMinimize.Header.Ysize,
					"Minimize", BUTTON_PUT_LEFT, TRUE );
	// 복원 버튼
	m_btnRestore.Create(13, 5 + 3, 
					m_sprBtnMinimize.Header.Xsize,	m_sprBtnMinimize.Header.Ysize,
					"Restore", BUTTON_PUT_LEFT, TRUE );

	// 모든 공성전 유닛 보기
	m_btnShowUnitInformation.Create(12, 19 + 96, 
					m_sprShowUnitInformation.Header.Xsize,	m_sprShowUnitInformation.Header.Ysize,
					m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SHOW_SIEGEWARFAREUNIT_INFORMATION), BUTTON_PUT_LEFT, TRUE );
	/*
	// 적대 상단 리스트 보기 공격
	m_btnShowEmemyGuildList.Create(12, 49, 
					m_sprFunctionBtnImage.Header.Xsize,	m_sprFunctionBtnImage.Header.Ysize,
					m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_VILLAGE_COUNTERATTACK_STONE), BUTTON_PUT_LEFT, TRUE );
	*/

	// 마을 방어력 향상
	m_btnDefenceUp.Create(12, 49 + 96,
					m_sprFunctionBtnImage.Header.Xsize,	m_sprFunctionBtnImage.Header.Ysize,
					m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_DEFUP), BUTTON_PUT_LEFT, TRUE );

	// 수성인터페이스=>마을 들어가기
	m_btnEnterVillage.Create(12, 79 + 96,
					m_sprFunctionBtnImage.Header.Xsize,	m_sprFunctionBtnImage.Header.Ysize,	
					m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_EXITTEXT), BUTTON_PUT_LEFT, TRUE );

	// 공성유닛=>일반 유닛 변신 버튼
	m_btnToNormalChar.Create(12, 79 + 96,
					m_sprFunctionBtnImage.Header.Xsize,	m_sprFunctionBtnImage.Header.Ysize,	
					m_pMyOnlineWorld->pOnlineText->Get(8100128), BUTTON_PUT_LEFT, TRUE );

	// 인터페이스 사용 중에 일부 기능을 사용할 수 없다.
	m_pMyOnlineWorld->pOnlinePannel->SetFlagVillage(TRUE, ONLINEPANNEL_DISABLEFLAG_MINIMAP/*mini map*/ | ONLINEPANNEL_DISABLEFLAG_TIP/*Quest*/ | ONLINEPANNEL_DISABLEFLAG_OPTION/*system*/ | ONLINEPANNEL_DISABLEFLAG_IMOTICON/*imoticon*/ | ONLINEPANNEL_DISABLEFLAG_CHARINFO/*mercenary*/);

	// 열려 있는 창들 닫기 //////////////////////////////////////
	// 용병창
	if (m_pMyOnlineWorld->m_pMercenary->IsAction())
		m_pMyOnlineWorld->m_pMercenary->OnButtonPush();

	// 인벤토리가 열려 있으면 닫아준다.
	if(m_pMyOnlineWorld->pOnlineInventory->IsActive())
		m_pMyOnlineWorld->pOnlineInventory->SetActive(FALSE);
	
	// 장부가 열려 있으면 닫아준다.
	if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction())
		m_pMyOnlineWorld->pOnlineTradeBook->SetAction(FALSE);

	// 미니맵도 닫는다!!
	if(m_pMyOnlineWorld->pOnlineMiniMap->IsActive())
		m_pMyOnlineWorld->pOnlineMiniMap->SetActive(FALSE);

	// 시스템 메뉴(옵션) 닫기
	if(m_pMyOnlineWorld->pOnlineSetting->IsAction())
		m_pMyOnlineWorld->pOnlineSetting->SetAction(FALSE);
	///////////////////////////////////////////////////////////

	m_bMouseDown = FALSE;
}

// 리소스 해제
void	CSiegeWarfareUnitUI::FreeResource(void)
{
	// 이미지 해제 /////////////////////////////////////////
	clGrp.FreeXspr(m_sprShowUnitInformation);		// 모든 공성 유닛 정보보기 버튼 이미지
	clGrp.FreeXspr(m_sprEnterVillage);				// 마을 들어가기 버튼 이미지
	clGrp.FreeXspr(m_sprFunctionBtnImage);			// 수성 기능 버튼 이미지
	clGrp.FreeXspr(m_sprMainInterface);				// 수성인터페이스 메인 이미지
	clGrp.FreeXspr(m_sprTimer);						// 수성인터페이스 다은 수성 기능 사용 남은 시간 표시 이미지

	clGrp.FreeXspr(m_sprStatusMain);				// 공성 유닛 상태 정보 윈도 배경 이미지
	clGrp.FreeXspr(m_sprGaugeRed);					// 게이지 red
	clGrp.FreeXspr(m_sprGaugeBlue);					// 게이지 blue
	clGrp.FreeXspr(m_sprBtnMinimize);				// 최소화 버튼 이미지
	clGrp.FreeXspr(m_sprPortrait);					// 초상화 이미지
}

// 윈도 보이는가?
BOOL	CSiegeWarfareUnitUI::IsVisibleWindow(void)
{
	if (SW_NORMAL == m_dwShowWindow)
		return TRUE;

	return FALSE;
}

// 윈도가 최소화 되었는가?
BOOL	CSiegeWarfareUnitUI::IsMinimizeWindow(void)
{
	if (SW_MINIMIZE == m_dwShowWindow)
		return TRUE;

	return FALSE;
}

// 인터페이스 기능 사용할 때 주어지는 딜레이 설정
void	CSiegeWarfareUnitUI::SetCurrentDelayTimeUsingVillageAttackType(SI32 siDelay)
{
	m_siCurrentDelayTimeUsingVillageAttackType = siDelay;
}

void	CSiegeWarfareUnitUI::SetMaxDelayTimeUsingVillageAttackType(SI32 siDelay)
{
	m_siMaxDelayTimeUsingVillageAttackType = siDelay;
}

// 마을 코드
void	CSiegeWarfareUnitUI::SetVillageCode(UI16 uiVillageCode)
{
	m_uiVillageCode = uiVillageCode;
}

// 윈도 생성
BOOL	CSiegeWarfareUnitUI::SiegeWarfareUnitUI_CreateWindow(DWORD dwShowWindow)
{
	// 이미 생성한 경우
	if (m_bCreatedWindow)
	{
		// 이전 데이터 삭제 후에 다시 생성하도록 한다.
		SiegeWarfareUnitUI_DestroyWindow();

		Init();
	}

	// 리소스 로드
	LoadResource();

	// 생성 완료
	m_bCreatedWindow = TRUE;

	return TRUE;
}

// 윈도 파괴
BOOL	CSiegeWarfareUnitUI::SiegeWarfareUnitUI_DestroyWindow(void)
{
	// 생성하지 않은 경우
	if (FALSE == m_bCreatedWindow)
		return FALSE;

	if (m_pConfirmToGiveUpBattleMsgBox)
	{
		delete m_pConfirmToGiveUpBattleMsgBox;
		m_pConfirmToGiveUpBattleMsgBox = NULL;
	}

	// 리소스 해제
	FreeResource();

	// No Field Area 해제
	if (m_pMyOnlineWorld->pOnlineFieldArea)
	{
		if (-1 != m_siNoFieldAreaHandle)
		{
			// No Field Area 해제
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoFieldAreaHandle);
			m_siNoFieldAreaHandle = -1;
		}

		if (-1 != m_siNoFieldAreaHandle_BtnMinimize)
		{
			// No Field Area 해제
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoFieldAreaHandle_BtnMinimize);
			m_siNoFieldAreaHandle_BtnMinimize = -1;
		}
	}

	m_dwShowWindow = SW_HIDE;

	// 삭제 완료
	m_bCreatedWindow = FALSE;

	return TRUE;
}
