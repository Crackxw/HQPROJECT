#include <GSL.h>

#include <mouse.h>
#include <clGame.h>
#include "OnlineWorld.h"
#include "OnlineFont.h"
#include "OnlineVillage.h"
#include "OnlineMegaText.h"
#include "Common.h"
#include "OnlineNumberControl.h"

#include "OnlineGovernMgr.h"

#include "..\EnumData\Enum_KI_Character.h"		// KindInfo - Character
#include "OnlineNumberControl.h"

#include "OnlineHelp.h"

// 공성 유닛 관리자
#include	"SiegeUnitTableMgr.h"

extern	_clGame* pGame;
extern _InputDevice				IpD;


VOID OnlineGovernMgr::InitVillageInfo()
{
	m_BVillageInfo.SetAction(FALSE);

	clGrp.LoadXspr("Online\\GameSpr\\Village\\town_office_info.spr", m_VillageData.m_MainImage);
	clGrp.LoadXspr("Online\\GameSpr\\Village\\enter_button.spr", m_VillageData.m_DescOkImage);
	clGrp.LoadXspr("Online\\GameSpr\\Village\\town_button_medium.spr", m_VillageData.m_ButImage);

	// 인구
	m_VillageData.m_uiPopulation = pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.uiPopulation;

	// 내부시설
	ZeroMemory(m_VillageData.m_szStruct, sizeof(m_VillageData.m_szStruct));
	pMyOnlineWorld->pOnlineVillage->MakeVillageInStructor((char*)m_VillageData.m_szStruct);

	// 마을안내문
	// actdoll (2004/05/04 16:33) : [OECEX]
//	m_VillageData.m_pEditBox = new OnlineEditControl;
//	m_VillageData.m_pEditBox->CreateEditBox(1000, 1000, 320, 108, pGame->Hwnd, pGame->hInst, FALSE, FALSE, VILLAGE_DESCRIPT_NOTE, FALSE, TRUE);
//	m_VillageData.m_pEditBox->SetFont(pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));
//	m_VillageData.m_pEditBox->SetKeyLock('\'', TRUE);	m_VillageData.m_pEditBox->SetKeyLock('\"', TRUE);	m_VillageData.m_pEditBox->SetKeyLock('\\', TRUE);
	OnlineEditControlEx	*pOecEx;
	RECT	rcRect = { m_siX + 107, m_siY + 229, m_siX + 107 + 320, m_siY + 229 + 131 };
	m_VillageData.m_hOecNotice	= Handler_OnlineEditControlEx::CreateControl( DF_FLAG_OEC_TYPE_MULTILINE, &rcRect, VILLAGE_DESCRIPT_NOTE );
	pOecEx	=	Handler_OnlineEditControlEx::GetControl( m_VillageData.m_hOecNotice );
	pOecEx->SetFont( pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT) );
	pOecEx->SetColor( RGB(250, 250, 250) );
	pOecEx->SetExceptionChar( '\'' );
	pOecEx->SetExceptionChar( '\"' );
	pOecEx->SetExceptionChar( '\\' );

	m_VillageData.m_BDescOk.Create(m_siX+440, m_siY+347, m_VillageData.m_DescOkImage.Header.Xsize, m_VillageData.m_DescOkImage.Header.Ysize, "", BUTTON_PUT_LEFT, FALSE);
	m_VillageData.m_BDefence.Create(m_siX+16, m_siY+158, m_VillageData.m_ButImage.Header.Xsize, m_VillageData.m_ButImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_DEFUP), BUTTON_PUT_LEFT, FALSE);

	// robypark 2004/11/15 13:58
	// 공성전을 위한 버튼 추가
	// 병력 모집 버튼
	m_VillageData.m_btnRecruitArmy.Create(m_siX+16, m_siY+158, m_VillageData.m_ButImage.Header.Xsize, m_VillageData.m_ButImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_RECRUIT_ARMY), BUTTON_PUT_LEFT, FALSE);

	/*
	// 수성하기 버튼
	m_VillageData.m_btnToDefenceMenu.Create(m_siX+110, m_siY+164, m_VillageData.m_ButImage.Header.Xsize, m_VillageData.m_ButImage.Header.Ysize, "수성하기", BUTTON_PUT_LEFT, FALSE);
	m_VillageData.m_btnToDefenceMenu.ShowWindow(FALSE);
	// 병력 모집 버튼
//	m_VillageData.m_btnRecruitArmy.Create(m_siX+110, m_siY+164, m_VillageData.m_ButImage.Header.Xsize, m_VillageData.m_ButImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_RECRUIT_ARMY), BUTTON_PUT_LEFT, FALSE);
	m_VillageData.m_btnRecruitArmy.Create(m_siX+16, m_siY+158, m_VillageData.m_ButImage.Header.Xsize, m_VillageData.m_ButImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_RECRUIT_ARMY), BUTTON_PUT_LEFT, FALSE);
	// 보급품 생산 버튼
	m_VillageData.m_btnProduceSupplyGoods.Create(m_siX+227, m_siY+164, m_VillageData.m_ButImage.Header.Xsize, m_VillageData.m_ButImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_PRODUCE_SUPPLY_GOODS), BUTTON_PUT_LEFT, FALSE);
	// 점령선포
	m_VillageData.m_btnDeclareOccupation.Create(m_siX+344, m_siY+164, m_VillageData.m_ButImage.Header.Xsize, m_VillageData.m_ButImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_DECLARE_OCCUPATION), BUTTON_PUT_LEFT, FALSE);
	*/

	// 마을 주인이거나 운영자만이 안내문을 쓸 수 있다.
	if((strcmp(pMyOnlineWorld->pOnlineVillage->GetMaxInvestmentID(), pMyOnlineWorld->pMyData->GetMyCharName()) == 0)
		|| pMyOnlineWorld->m_GMFlag == ON_USERGRADE_ADMIN5)
	{
		m_VillageData.m_BDescOk.SetAction(TRUE);
	}

	// 마을 주인일 경우 마을 방어력 투자 가능
	if(strcmp(pMyOnlineWorld->pOnlineVillage->GetMaxInvestmentID(), pMyOnlineWorld->pMyData->GetMyCharName()) == 0)
	{
		m_VillageData.m_BDefence.SetAction(TRUE);
	}
	
	// 같은 길드의 대행수라면 방어력 투자가능
	if(pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID[0])
	{
		if(pMyOnlineWorld->pMyData->GetGuildClass() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DHS))
		{
			if(strcmp((char*)pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID, pMyOnlineWorld->pMyData->GetGuildName()) == 0)
				m_VillageData.m_BDefence.SetAction(TRUE);
		}
	}

	m_VillageData.m_BDefence.SetAction(FALSE);
	m_VillageData.m_BDefence.ShowWindow(FALSE);

	// 관청 정보 탭에서 사용되는 버튼 수성하기, 병력모집, 보급품 생산, 점령선포 버튼 상태 설정(활성, 비활성)
	UpdateVillageInfoButton();
}

// 관청 정보 탭에서 사용되는 버튼 수성하기, 병력모집, 보급품 생산, 점령선포 버튼 상태 설정(활성, 비활성)
void	OnlineGovernMgr::UpdateVillageInfoButton(void)
{
	// 관청에 들어온 사용자의 상단 내 직급 얻기
	SI32 siMyGuildClass = pMyOnlineWorld->pMyData->GetGuildClass();

	// 마을에 상단이 존재하며
	if(pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID[0])
	{
		// 사용자가 상단 직급이 대방, 마을 최고 투자자이며
		if ((siMyGuildClass == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
			|| (strcmp(pMyOnlineWorld->pOnlineVillage->GetMaxInvestmentID(), pMyOnlineWorld->pMyData->GetMyCharName()) == 0))
		{
			// 또한 같은 상단의 마을일 경우
			if (((char*)pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID)
				&& (pMyOnlineWorld->pMyData->GetGuildName())
				&& (strcmp((char*)pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID, pMyOnlineWorld->pMyData->GetGuildName()) == 0))
			{
				// 병력 모집 버튼 활성화
				m_VillageData.m_btnRecruitArmy.SetAction(TRUE);

				/*
				// 보급품 생산 버튼 활성화
				m_VillageData.m_btnProduceSupplyGoods.SetAction(TRUE);
				*/
			}
		}

	}
	else	// 상단이 없는 마을이며
	{
		// 사용자 상단 직급이 대방일 경우
		if (siMyGuildClass == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
		{
			// 마을 점령 선포 버튼 활성화
			m_VillageData.m_btnDeclareOccupation.SetAction(TRUE);
		}
	}

	// 추후에 점령 선포가 추가될 예정이다. 2004/11/18 현재
	// 마을 점령 선포 버튼 활성화
	m_VillageData.m_btnDeclareOccupation.SetAction(FALSE);
}

VOID OnlineGovernMgr::FreeVillageInfo()
{
	clGrp.FreeXspr(m_VillageData.m_MainImage);
	clGrp.FreeXspr(m_VillageData.m_DescOkImage);
	clGrp.FreeXspr(m_VillageData.m_ButImage);

	// actdoll (2004/05/04 16:33) : [OECEX]
//	if(m_VillageData.m_pEditBox)
//	{
//		m_VillageData.m_pEditBox->Free();
//
//		delete m_VillageData.m_pEditBox;
//		m_VillageData.m_pEditBox = NULL;
//	}
	Handler_OnlineEditControlEx::KillControl( m_VillageData.m_hOecNotice );

	m_VillageData.m_uiStatus = ON_SUB_VILLAGEINFO_INIT;
	m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_INIT;

	pMyOnlineWorld->SetShortKey(FALSE);

	m_BVillageInfo.SetAction(TRUE);
}

VOID OnlineGovernMgr::InitVillageInfo_RecruitArmy()		// 병력모집 인터페이스 초기화
{
	ZeroMemory(m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount, sizeof(SI32) * ON_MAX_GUILD_UNIT);

	m_VillageData.m_siSupplyGoodsAmount = 0;

	///////////////////////////////////////
	// 이미지 로드
	// 모집 버튼
	clGrp.LoadXspr("Online\\GameSpr\\Village\\town_button_large.spr", m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit);
	// 배경 이미지
	clGrp.LoadXspr("Online\\GameSpr\\Village\\TOWN_OFFICE_RECRUIT.Spr", m_VillageData.m_sRECRUIT_Army_InGovern.m_MainImage);

	///////////////////////////////////////
	// 병력모집에서 사용하는 버튼 생성
	// 검병 모집
	m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Swordman.Create(m_siX+316, m_siY+139 + (28 * 0),
					m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Xsize, m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Ysize,
					pMyOnlineWorld->pOnlineText->Get(ON_TEXT_RECRUIT_ARMY_SWORDMAN), BUTTON_PUT_LEFT, TRUE);
	// 창병 모집
	m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Spearman.Create(m_siX+316, m_siY+139 + (28 * 1),
					m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Xsize, m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Ysize,
					pMyOnlineWorld->pOnlineText->Get(ON_TEXT_RECRUIT_ARMY_SPEARMAN), BUTTON_PUT_LEFT, TRUE);
	// 궁병 모집
	m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Archerman.Create(m_siX+316, m_siY+139 + (28 * 2),
					m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Xsize, m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Ysize,
					pMyOnlineWorld->pOnlineText->Get(ON_TEXT_RECRUIT_ARMY_ARCHER), BUTTON_PUT_LEFT, TRUE);
	// 기마병 모집
	m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Horseman.Create(m_siX+316, m_siY+139 + (28 * 3),
					m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Xsize, m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Ysize,
					pMyOnlineWorld->pOnlineText->Get(ON_TEXT_RECRUIT_ARMY_HORSEMAN), BUTTON_PUT_LEFT, TRUE);
	// 충차 모집
	m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Siege_tower.Create(m_siX+316, m_siY+139 + (28 * 4),
					m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Xsize, m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Ysize,
					pMyOnlineWorld->pOnlineText->Get(ON_TEXT_RECRUIT_ARMY_SIEGETOWER), BUTTON_PUT_LEFT, TRUE);
	// 발석거 모집
	m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Catapult.Create(m_siX+316, m_siY+139 + (28 * 5),
					m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Xsize, m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Ysize,
					pMyOnlineWorld->pOnlineText->Get(ON_TEXT_RECRUIT_ARMY_CATAPULT), BUTTON_PUT_LEFT, TRUE);
	// 보급수레 모집
	m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Supply_wagon.Create(m_siX+316, m_siY+139 + (28 * 6),
					m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Xsize, m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Ysize,
					pMyOnlineWorld->pOnlineText->Get(ON_TEXT_RECRUIT_ARMY_SUPPLYGOODS_WAGON), BUTTON_PUT_LEFT, TRUE);
	/*
	// 수비병 모집
	m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Garrison.Create(m_siX+316, m_siY+139 + (28 * 7),
					m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Xsize, m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Ysize,
					pMyOnlineWorld->pOnlineText->Get(ON_TEXT_RECRUIT_ARMY_GARRISON), BUTTON_PUT_LEFT, TRUE);
	*/

	// 상위메뉴로 돌아가기 버튼
	m_VillageData.m_btnBack.Create(m_siX+227, m_siY+364,
					m_VillageData.m_ButImage.Header.Xsize, m_VillageData.m_ButImage.Header.Ysize,
					pMyOnlineWorld->pOnlineText->Get(1000114), BUTTON_PUT_LEFT, TRUE);
}

// robypark 2004/12/27 15:42
// 관청 정보 탭 - 병력 모집에서 모집 버튼 활성화 여부 설정하기
void	OnlineGovernMgr::UpdateVillageInfo_BtnRecruitArmy(void)
{
	// 공성전 준비를 위하여 대방, 대행수, 행수가 상단 소속 마을에서 병력모집 가능 시간이며
	if(FALSE == pMyOnlineWorld->IsInTimeDisabledRecruitArmyInGovernment())
	{
		// 검병 모집
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Swordman.SetAction(TRUE);
		// 창병 모집
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Spearman.SetAction(TRUE);
		// 궁병 모집
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Archerman.SetAction(TRUE);
		// 기마병 모집
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Horseman.SetAction(TRUE);
		// 충차 모집
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Siege_tower.SetAction(TRUE);
		// 발석거 모집
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Catapult.SetAction(TRUE);
		// 보급수레 모집
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Supply_wagon.SetAction(TRUE);
		/*
		// 수비병 모집
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Garrison.SetAction(TRUE);
		*/
	}
	else
	{
		// 검병 모집
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Swordman.SetAction(FALSE);
		// 창병 모집
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Spearman.SetAction(FALSE);
		// 궁병 모집
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Archerman.SetAction(FALSE);
		// 기마병 모집
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Horseman.SetAction(FALSE);
		// 충차 모집
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Siege_tower.SetAction(FALSE);
		// 발석거 모집
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Catapult.SetAction(FALSE);
		// 보급수레 모집
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Supply_wagon.SetAction(FALSE);
		/*
		// 수비병 모집
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Garrison.SetAction(FALSE);
		*/
	}
}

VOID OnlineGovernMgr::FreeVillageInfo_RecruitArmy()		// 병력모집 인터페이스 할당 메모리 초기화
{
	m_VillageData.m_siSupplyGoodsAmount = 0;

	///////////////////////////////////////
	// 이미지 해제
	// 모집 버튼
	clGrp.FreeXspr(m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit);
	// 배경 이미지
	clGrp.FreeXspr(m_VillageData.m_sRECRUIT_Army_InGovern.m_MainImage);
}

VOID OnlineGovernMgr::InitVillageInfo_ProduceSupplyGoods()	// 보급품 생산 인터페이스 초기화
{
	///////////////////////////////////////
	// 이미지 로드
	// 배경 이미지
	clGrp.LoadXspr("Online\\GameSpr\\Village\\TOWN_OFFICE_SUPPLY.Spr", m_VillageData.m_sProduceSupplyGoods.m_MainImage);
	// 생산 버튼
	clGrp.LoadXspr("Online\\GameSpr\\Book\\BOOK_BUTTON_MINI.Spr", m_VillageData.m_sProduceSupplyGoods.m_sprMiniButton);

	// 보급품 생산 버튼
	m_VillageData.m_sProduceSupplyGoods.m_btnProduceSupplyGoods.Create(m_siX+413, m_siY+123,
					m_VillageData.m_sProduceSupplyGoods.m_sprMiniButton.Header.Xsize, m_VillageData.m_sProduceSupplyGoods.m_sprMiniButton.Header.Ysize,
					pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_PRODUCTION), BUTTON_PUT_LEFT, FALSE);

	// 상위메뉴로 돌아가기 버튼
	m_VillageData.m_btnBack.Create(m_siX+227, m_siY+364,
					m_VillageData.m_ButImage.Header.Xsize, m_VillageData.m_ButImage.Header.Ysize,
					pMyOnlineWorld->pOnlineText->Get(1000114), BUTTON_PUT_LEFT, TRUE);

	// 보급품 하나당 생산 비용 설정
//	m_VillageData.m_sProduceSupplyGoods.m_siCostSupplyGoods = pMyOnlineWorld->GetSOptionSiegeWarfare()->siCostSupplyGoods;
}

VOID OnlineGovernMgr::FreeVillageInfo_ProduceSupplyGoods()	// 보급품 생산 인터페이스 할당 메모리 초기화
{
	///////////////////////////////////////
	// 이미지 해제
	// 배경 이미지
	clGrp.FreeXspr(m_VillageData.m_sProduceSupplyGoods.m_MainImage);
	// 생산 버튼
	clGrp.FreeXspr(m_VillageData.m_sProduceSupplyGoods.m_sprMiniButton);
}

UI08 OnlineGovernMgr::PollVillageInfo(LPDIRECTDRAWSURFACE7 pSurface)
{
	OnlineEditControlEx	*pOecEx	= Handler_OnlineEditControlEx::GetControl( m_VillageData.m_hOecNotice );
	switch(m_VillageData.m_uiStatus)
	{
	case ON_SUB_VILLAGEINFO_INIT:
		{
			InitVillageInfo();
			pMyOnlineWorld->pOnlineClient->SendRequestGovernment_VillageInfo();
			m_VillageData.m_uiStatus = ON_SUB_VILLAGEINFO_WAIT;
			m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_INIT;
		}
		break;
	case ON_SUB_VILLAGEINFO_POLL:
		{
			// 메인메뉴 전환
			if(m_BInvest.Process(m_bMouseDown))
			{
				m_uiReturnStatus = ON_GOVERN_MAIN_STATUS_INVESTMENT;
				m_VillageData.m_uiStatus = ON_SUB_VILLAGEINFO_EXIT;
			}
			else if(m_BGuild.Process(m_bMouseDown))
			{
				m_uiReturnStatus = ON_GOVERN_MAIN_STATUS_GUILD;
				m_VillageData.m_uiStatus = ON_SUB_VILLAGEINFO_EXIT;
			}
			else if(m_BExit.Process(m_bMouseDown))
			{
				pMyOnlineWorld->pOnlineClient->SendRequestGovernment_Main_Leave();
				m_uiReturnStatus = ON_GOVERN_MAIN_STATUS_EXIT_WAIT;
				m_VillageData.m_uiStatus = ON_SUB_VILLAGEINFO_EXIT;
			}

			// actdoll (2004/05/04 16:33) : [OECEX]
//			if(m_bMouseDown)
//			{
//				if((PtInRect(&m_VillageData.m_rcDescRect, ptMouse) == TRUE))
//				{
//					m_VillageData.m_pEditBox->EnableWindow(TRUE);
//					m_VillageData.m_pEditBox->SetFocus();
//				}
//				else
//				{
//					m_VillageData.m_pEditBox->EnableWindow(FALSE);
//					::SetFocus(pGame->Hwnd);
//				}
//			}
//			if( m_VillageData.m_pEditBox->IsFocus() )
//			{
//				pMyOnlineWorld->SetShortKey(TRUE);
//			}
//			else
//			{
//				pMyOnlineWorld->SetShortKey(FALSE);
//			}
			// 안내문을 클릭하였을 경우
			if( m_bMouseDown )
			{
				// 마을 주인이거나 운영자만이 안내문을 쓸 수 있다.
				if( ( strcmp( pMyOnlineWorld->pOnlineVillage->GetMaxInvestmentID(), pMyOnlineWorld->pMyData->GetMyCharName() ) == 0 ) || 
					pMyOnlineWorld->m_GMFlag == ON_USERGRADE_ADMIN5 )
				{
					if( pOecEx->IsPointInRect( ptMouse ) )	Handler_OnlineEditControlEx::SetFocus( m_VillageData.m_hOecNotice );
					else									Handler_OnlineEditControlEx::ClearCurrentFocus();
				}
			}

			// 안내문에 포커스가 가 있을 경우
			if( Handler_OnlineEditControlEx::IsFocus( m_VillageData.m_hOecNotice ) )	pMyOnlineWorld->SetShortKey( true );
			else																		pMyOnlineWorld->SetShortKey( false );

			if(m_VillageData.m_BDescOk.Process(m_bMouseDown))
			{
				CHAR szDescript[512];
				ZeroMemory(szDescript, sizeof(szDescript));

				// actdoll (2004/05/04 16:33) : [OECEX]
//				m_VillageData.m_pEditBox->GetString( szDescript );
				strcpy( szDescript, pOecEx->GetString() );
				if(CheckLeadByte(szDescript))
				{
					pMyOnlineWorld->pOnlineClient->SendRequestVillageDescript_Write(szDescript);
					// actdoll (2004/05/04 16:33) : [OECEX]
//					m_VillageData.m_pEditBox->EnableWindow(FALSE);
//					::SetFocus(pGame->Hwnd);
					Handler_OnlineEditControlEx::ClearCurrentFocus();

					m_VillageData.m_uiStatus = ON_SUB_VILLAGEINFO_WAIT;
				}
				else
					pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(8000479));
			}
			else if(m_VillageData.m_BDefence.Process(m_bMouseDown))
			{
				m_VillageData.m_BDefence.SetAction(FALSE);
				pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetMaxVillageDefenceInvestment();
				m_VillageData.m_uiStatus = ON_SUB_VILLAGEINFO_WAIT;
			}
			/*
			// 공성전 기능 버튼 처리 robypark 2004/11/15 14:31
			else if (m_VillageData.m_btnToDefenceMenu.Process(m_bMouseDown))	// 수성하기 버튼
			{
				m_VillageData.m_uiStatus = ON_SUB_VILLAGEINFO_TO_DEFENCE_MENU;
			}
			*/
			else if (m_VillageData.m_btnRecruitArmy.Process(m_bMouseDown))		// 병력 모집 버튼
			{
				m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_INIT;
				m_VillageData.m_uiStatus = ON_SUB_VILLAGEINFO_RECRUIT_ARMY;
			}
			/*
			else if (m_VillageData.m_btnProduceSupplyGoods.Process(m_bMouseDown))	// 보급품 생산 버튼
			{
				m_VillageData.m_uiStatus = ON_SUB_VILLAGEINFO_PRODUCE_SUPPLYGOODS;
			}
			else if (m_VillageData.m_btnDeclareOccupation.Process(m_bMouseDown))	// 점령선포 버튼
			{
				m_VillageData.m_uiStatus = ON_SUB_VILLAGEINFO_DECLARE_OCCUPATION;
			}
			*/

			// 마을 방어력 정보 갱신
			SVillageData *lpSVillageData = pMyOnlineWorld->pOnlineVillageDescriptList->GetVillageDataData(pMyOnlineWorld->pOnlineVillage->GetVillageHeader()->uiVillageCode);				
			if(lpSVillageData)
			{
				if(lpSVillageData->m_fWait == FALSE)
				{
					m_VillageData.m_CurDefPower = lpSVillageData->m_vdChangedDefence;
					m_VillageData.m_MaxDefPower = lpSVillageData->m_vdChangedMaxDefence;
				}
			}
		}
		break;
	case ON_SUB_VILLAGEINFO_SETDEFENCE:
		{
			if(pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() && pMyOnlineWorld->pOnlineNumPad->siCount != 0)
			{
				m_VillageData.m_uiStatus = ON_SUB_VILLAGEINFO_POLL;

				MONEY MaxMoney = min((m_VillageData.m_MaxDefPower-m_VillageData.m_CurDefPower)*m_VillageData.m_MoneyPerDef, m_VillageData.m_MaxMoney);
				if(pMyOnlineWorld->pOnlineNumPad->siCount > MaxMoney)
				{
					m_VillageData.m_BDefence.SetAction(TRUE);
					pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(8000488));
				}
				else
				{
					m_VillageData.m_uiStatus = ON_SUB_VILLAGEINFO_WAIT;
					pMyOnlineWorld->pOnlineClient->SendRequestGovernment_SetVillageDefenceInvestment(pMyOnlineWorld->pOnlineNumPad->siCount);
				}
				pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
			}
			else if(pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() && pMyOnlineWorld->pOnlineNumPad->siCount == 0)
			{
				m_VillageData.m_BDefence.SetAction(TRUE);
				pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
				m_VillageData.m_uiStatus = ON_SUB_VILLAGEINFO_POLL;
			}
		}
		break;
	case ON_SUB_VILLAGEINFO_WAIT:
		{
			// 마을안내문, 방어력향상 응답대기(GovernVillageInfo, GovernDescript, GovernVillageDefenceInfo, GovernVillageDefenceSet)
			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
		}
		break;
	case ON_SUB_VILLAGEINFO_EXIT:
		{
			DrawVillageInfo(pSurface);
			FreeVillageInfo();
			return m_uiReturnStatus;
		}
		break;

	case ON_SUB_VILLAGEINFO_TO_DEFENCE_MENU:		// 수성하기
		{
			m_VillageData.m_uiStatus = ActionToDefenceMenu();
		}
		break;

	case ON_SUB_VILLAGEINFO_RECRUIT_ARMY:			// 병력 모집
		{
			m_VillageData.m_uiStatus = ActionRecruitArmy();
		}
		break;

	case ON_SUB_VILLAGEINFO_PRODUCE_SUPPLYGOODS:	// 보급품 생산
		{
			m_VillageData.m_uiStatus = ActionProduceSupplyGoods();
		}
		break;

	case ON_SUB_VILLAGEINFO_DECLARE_OCCUPATION:		// 점령 선포
		{
			m_VillageData.m_uiStatus = ActionToDefenceMenu();
		}
		break;
	}

	DrawVillageInfo(pSurface);

	return ON_GOVERN_MAIN_STATUS_VILLAGEINFO;
}

VOID OnlineGovernMgr::DrawVillageInfo(LPDIRECTDRAWSURFACE7 pSurface)
{
	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		switch (m_VillageData.m_uiStatus)
		{
			case ON_SUB_VILLAGEINFO_TO_DEFENCE_MENU:		// 수성하기
				break;
			case ON_SUB_VILLAGEINFO_RECRUIT_ARMY:			// 병력 모집
				if (NULL != m_VillageData.m_sRECRUIT_Army_InGovern.m_MainImage.Image)
				{
					// 관청 정보 메인 이미지 그리기
					clGrp.PutSpriteT(m_siX, m_siY,
						m_VillageData.m_sRECRUIT_Army_InGovern.m_MainImage.Header.Xsize, 
						m_VillageData.m_sRECRUIT_Army_InGovern.m_MainImage.Header.Ysize, 
						m_VillageData.m_sRECRUIT_Army_InGovern.m_MainImage.Image);
				}

				// 버튼 이미지 그리기
				if (NULL != m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Image)
				{
					// 검병 모집
					m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Swordman.Put(&m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit, 0, 2, 1, BUTTON_PUT_NOMOVE);
					// 창병 모집
					m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Spearman.Put(&m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit, 0, 2, 1, BUTTON_PUT_NOMOVE);
					// 궁병 모집
					m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Archerman.Put(&m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit, 0, 2, 1, BUTTON_PUT_NOMOVE);
					// 기마병 모집
					m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Horseman.Put(&m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit, 0, 2, 1, BUTTON_PUT_NOMOVE);
					// 충차 모집
					m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Siege_tower.Put(&m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit, 0, 2, 1, BUTTON_PUT_NOMOVE);
					// 발석거 모집
					m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Catapult.Put(&m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit, 0, 2, 1, BUTTON_PUT_NOMOVE);
					// 보급수레 모집
					m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Supply_wagon.Put(&m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit, 0, 2, 1, BUTTON_PUT_NOMOVE);
					/*
					// 수비병(주둔군) 모집
					m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Garrison.Put(&m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit, 0, 2, 1, BUTTON_PUT_NOMOVE);
					*/
				}
				
				// 돌아가기 버튼
				m_VillageData.m_btnBack.Put(&m_VillageData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);

				break;

			case ON_SUB_VILLAGEINFO_PRODUCE_SUPPLYGOODS:	// 보급품 생산
				// 관청 정보 메인 이미지 그리기
				clGrp.PutSpriteT(m_siX, m_siY,
					m_VillageData.m_sProduceSupplyGoods.m_MainImage.Header.Xsize, 
					m_VillageData.m_sProduceSupplyGoods.m_MainImage.Header.Ysize,
					m_VillageData.m_sProduceSupplyGoods.m_MainImage.Image);

				// 버튼 이미지 그리기
				// 생산 버튼
				m_VillageData.m_sProduceSupplyGoods.m_btnProduceSupplyGoods.Put(&m_VillageData.m_sProduceSupplyGoods.m_sprMiniButton, 0, 2, 1, BUTTON_PUT_NOMOVE);
				// 돌아가기 버튼
				m_VillageData.m_btnBack.Put(&m_VillageData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
				break;

			case ON_SUB_VILLAGEINFO_DECLARE_OCCUPATION:		// 점령 선포
				break;
			default:
				// 관청 정보 메인 이미지 그리기
				clGrp.PutSpriteT(m_siX, m_siY, 
					m_VillageData.m_MainImage.Header.Xsize,
					m_VillageData.m_MainImage.Header.Ysize,
					m_VillageData.m_MainImage.Image);

				// robypark 2004/11/15 13:58
				// 공성전을 위한 버튼 이미지 그리기
				// 병력 모집 버튼
				m_VillageData.m_btnRecruitArmy.Put(&m_VillageData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
				/*
				// 수성하기 버튼
				m_VillageData.m_btnToDefenceMenu.Put(&m_VillageData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
				// 병력 모집 버튼
				m_VillageData.m_btnRecruitArmy.Put(&m_VillageData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
				// 보급품 생산 버튼
				m_VillageData.m_btnProduceSupplyGoods.Put(&m_VillageData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
				// 점령선포
				m_VillageData.m_btnDeclareOccupation.Put(&m_VillageData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
				*/

				// 관청 정보 버튼 그리기
				m_VillageData.m_BDescOk.Put(&m_VillageData.m_DescOkImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
				m_VillageData.m_BDefence.Put(&m_VillageData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
				break;
		}

		clGrp.UnlockSurface( pSurface );
	}

	HDC	hdc;
	CHAR szTemp[128], szTemp2[256];
	OnlineEditControlEx	*pOecEx	= Handler_OnlineEditControlEx::GetControl( m_VillageData.m_hOecNotice );
	if(pSurface->GetDC(&hdc) == DD_OK)
	{
		SetBkMode( hdc, TRANSPARENT );
		SelectObject( hdc, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));

		switch (m_VillageData.m_uiStatus)
		{
			case ON_SUB_VILLAGEINFO_TO_DEFENCE_MENU:		// 수성하기
				break;
			case ON_SUB_VILLAGEINFO_RECRUIT_ARMY:			// 병력 모집
				// 정보 텍스트 그리기
				// 캐릭터 소지금(자본금) 출력
				MakeMoneyText(pMyOnlineWorld->pMyData->GetMoney(), szTemp);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 16, m_siY + 365, 52, 15, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CAPITAL_FUND), RGB(216, 201, 183));
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 74, m_siY + 364, 94, 17, szTemp, RGB(250, 250, 250));

				// 모집 비용 출력
				// 필드 전투 가능한 공성전 유닛 모집, 마을 주둔군 모집일 때 보여지는 계산기 인터페이스 작동 중에만
//				if ((ON_SUB_VILLAGEINFO_SUB_RECRUIT_FIELD_ARMY == m_VillageData.m_uiSubStatus) 
//					|| (ON_SUB_VILLAGEINFO_SUB_RECRUIT_GARRISON_ARMY == m_VillageData.m_uiSubStatus))
				if (pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsShow())
				{
					// 병과 설명 텍스트 인덱스 얻기
					LPSSiegeUnitTable lpsSiegeUnitTable = pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyKind);

					int siArmyCnt = (int)pMyOnlineWorld->pOnlineNumPad->pNumberControl->GetNumber();

					char szArmyCnt[32];
					MakeMoneyText(siArmyCnt, szArmyCnt);
					char szArmyCost[32];
					MakeMoneyText(siArmyCnt * m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitCost,
									szArmyCost);
					char szArmyPop[32];
					MakeMoneyText(siArmyCnt * m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation, 
									szArmyPop);
					// xxx nn 부대 모집비용: aaa 명, bbb 냥
					sprintf(szTemp2, "%s %s %s %s %s %s, %s %s", 
						pMyOnlineWorld->pOnlineText->Get(lpsSiegeUnitTable->m_siUnitNameTextIdx), 
						szArmyCnt,
						pMyOnlineWorld->pOnlineText->Get(ON_TEXT_UNIT_ARMY),
						pMyOnlineWorld->pOnlineText->Get(ON_TEXT_RECRUIT_ARMY_INFO_TEXT), 
						szArmyCost,
						pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WON),
						szArmyPop,
						pMyOnlineWorld->pOnlineText->Get(8000540));
						/*
						m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitSupplyGoods * siArmyCnt,
						pMyOnlineWorld->pOnlineText->Get(ON_TEXT_UNIT));
						*/
						//m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.);
					pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 132, m_siY + 73, 371, 17, szTemp2, RGB(250, 250, 250));
				}

				/*
				// 상단 보유 보급품 수
				MakeMoneyText(m_VillageData.m_siSupplyGoodsAmount, szTemp);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 365, m_siY + 365, 52, 15, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_COUNT_SUPPLYGOODS), RGB(216, 201, 183));
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 423, m_siY + 364, 94, 17, szTemp, RGB(250, 250, 250));
				*/

				// 모집 가능한 인구수 출력
				MakeMoneyText(m_VillageData.m_sRECRUIT_Army_InGovern.m_siRecruitablePopulation, szTemp);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 133, m_siY + 112 + (28 * 0), 82, 15, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_RECRUITABLED_POPULATION), RGB(216, 201, 183));
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 221, m_siY + 111 + (28 * 0), 82, 15, szTemp, RGB(250, 250, 250));

				// 검병
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 133, m_siY + 112 + (28 * 1), 82, 15, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_COUNT_SWORDMAN_ARMY), RGB(216, 201, 183));
				sprintf(szTemp, "%d", m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SWORDMAN]);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 221, m_siY + 111 + (28 * 1), 84, 17, szTemp, RGB(250, 250, 250));
				// 창병
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 133, m_siY + 112 + (28 * 2), 82, 15, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_COUNT_SPEARMAN_ARMY), RGB(216, 201, 183));
				sprintf(szTemp, "%d", m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SPEARMAN]);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 221, m_siY + 111 + (28 * 2), 84, 17, szTemp, RGB(250, 250, 250));
				// 궁병
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 133, m_siY + 112 + (28 * 3), 82, 15, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_COUNT_ARCHER_ARMY), RGB(216, 201, 183));
				sprintf(szTemp, "%d", m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_ARCHERMAN]);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 221, m_siY + 111 + (28 * 3), 84, 17, szTemp, RGB(250, 250, 250));
				// 기마병
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 133, m_siY + 112 + (28 * 4), 82, 15, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_COUNT_HORSEMAN_ARMY), RGB(216, 201, 183));
				sprintf(szTemp, "%d", m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_HORSEMAN]);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 221, m_siY + 111 + (28 * 4), 84, 17, szTemp, RGB(250, 250, 250));
				// 충차
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 133, m_siY + 112 + (28 * 5), 82, 15, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_COUNT_SIEGETOWER), RGB(216, 201, 183));
				sprintf(szTemp, "%d", m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SIEGETOWER]);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 221, m_siY + 111 + (28 * 5), 84, 17, szTemp, RGB(250, 250, 250));
				// 발석거
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 133, m_siY + 112 + (28 * 6), 82, 15, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_COUNT_CATAPULT), RGB(216, 201, 183));
				sprintf(szTemp, "%d", m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_CATAPULT]);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 221, m_siY + 111 + (28 * 6), 84, 17, szTemp, RGB(250, 250, 250));
				// 보급수레
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 133, m_siY + 112 + (28 * 7), 82, 15, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_COUNT_SUPPLYGOODS_WAGON), RGB(216, 201, 183));
				sprintf(szTemp, "%d", m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SUPPLYWAGON]);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 221, m_siY + 111 + (28 * 7), 84, 17, szTemp, RGB(250, 250, 250));
				/*
				// 주둔군(수비병)
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 133, m_siY + 112 + (28 * 8), 82, 15, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_COUNT_GARRISON), RGB(216, 201, 183));
				sprintf(szTemp, "%d", m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_DEFENCESOLIDER]);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 221, m_siY + 111 + (28 * 8), 84, 17, szTemp, RGB(250, 250, 250));
				*/

				// 버튼 텍스트 그리기
				// 검병 모집
				m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Swordman.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				// 창병 모집
				m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Spearman.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				// 궁병 모집
				m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Archerman.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				// 기마병 모집
				m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Horseman.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				// 충차 모집
				m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Siege_tower.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				// 발석거 모집
				m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Catapult.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				// 보급수레 모집
				m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Supply_wagon.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				/*
				// 수비병(주둔군) 모집
				m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Garrison.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				*/

				// 돌아가기 버튼
				m_VillageData.m_btnBack.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				break;

			case ON_SUB_VILLAGEINFO_PRODUCE_SUPPLYGOODS:	// 보급품 생산
				// 정보 텍스트 그리기
				// 캐릭터 소지금(자본금) 출력
				MakeMoneyText(pMyOnlineWorld->pMyData->GetMoney(), szTemp);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 16, m_siY + 365, 52, 15, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CAPITAL_FUND), RGB(216, 201, 183));
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 74, m_siY + 364, 94, 17, szTemp, RGB(250, 250, 250));

				// 보급품 생산 비용 출력
				// 보급품 생산 계산기 인터페이스 작동 중에만
//				if (ON_SUB_VILLAGEINFO_SUB_PRODUCE_SUPPLYGOODS == m_VillageData.m_uiSubStatus) 
				if (pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsShow())
				{
					// 생산에 필요한 자금 출력
					pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 132, m_siY + 76 + (22 * 2), 120, 17, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CAPITAL_FUND_TO_PROCUDE_SUPPLYGOODS), RGB(250, 250, 250));

					MakeMoneyText(m_VillageData.m_sProduceSupplyGoods.m_siCostSupplyGoods * pMyOnlineWorld->pOnlineNumPad->pNumberControl->GetNumber(), szTemp);
					sprintf(szTemp2, "%s %s", szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WON));
					pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 132 + 120, m_siY + 76 + (22 * 2), 151, 17, szTemp2, RGB(250, 250, 250));
				}
				else
				{
					// 생산에 필요한 자금 출력
					pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 132, m_siY + 76 + (22 * 2), 120, 17, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CAPITAL_FUND_TO_PROCUDE_SUPPLYGOODS), RGB(250, 250, 250));

					MakeMoneyText(m_VillageData.m_sProduceSupplyGoods.m_siCostSupplyGoods * pMyOnlineWorld->pOnlineNumPad->pNumberControl->GetNumber(), szTemp);
					sprintf(szTemp2, "%s %s", "0", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WON));
					pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 132 + 120, m_siY + 76 + (22 * 2), 151, 17, szTemp2, RGB(250, 250, 250));
				}

				// 상단 보유 보급품 수
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 132, m_siY + 76 + (22 * 0), 120, 17, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CURRENT_SUPPLYGOODS_COUNT), RGB(250, 250, 250));

				MakeMoneyText(m_VillageData.m_siSupplyGoodsAmount, szTemp);
				sprintf(szTemp2, "%s %s", szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_UNIT));
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 132 + 120, m_siY + 76 + (22 * 0), 151, 17, szTemp2, RGB(250, 250, 250));

				// 보급품 1개당 생산 비용 출력
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 132, m_siY + 76 + (22 * 1), 120, 17, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_PRICE_OF_A_SUPPLYGOODS), RGB(250, 250, 250));

				MakeMoneyText(m_VillageData.m_sProduceSupplyGoods.m_siCostSupplyGoods, szTemp);
				sprintf(szTemp2, "%s %s", szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WON));
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 132 + 120, m_siY + 76 + (22 * 1), 151, 17, szTemp2, RGB(250, 250, 250));

				// 보급품 생산 버튼 텍스트 출력
				m_VillageData.m_sProduceSupplyGoods.m_btnProduceSupplyGoods.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

				// 돌아가기 버튼
				m_VillageData.m_btnBack.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				break;

			case ON_SUB_VILLAGEINFO_DECLARE_OCCUPATION:		// 점령 선포
				break;
			// 관청 정보
			default:
				m_VillageData.m_BDescOk.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				m_VillageData.m_BDefence.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

				// robypark 2004/11/15 13:58
				// 공성전을 위한 버튼 텍스트 그리기
				// 병력 모집 버튼
				m_VillageData.m_btnRecruitArmy.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

				/*
				// 수성하기 버튼
				m_VillageData.m_btnToDefenceMenu.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				// 병력 모집 버튼
				m_VillageData.m_btnRecruitArmy.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				// 보급품 생산 버튼
				m_VillageData.m_btnProduceSupplyGoods.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				// 점령선포
				m_VillageData.m_btnDeclareOccupation.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				*/

				// 인구
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 112, m_siY + 68, 82, 13, pMyOnlineWorld->pOnlineText->Get(8000489), RGB(200, 200, 200));
				sprintf(szTemp, "%d", m_VillageData.m_uiPopulation);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 199, m_siY + 66, 163, 17, szTemp, RGB(200, 200, 200));

				// 연간 출입자 수
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 112, m_siY + 99, 82, 13, pMyOnlineWorld->pOnlineText->Get(8000490), RGB(200, 200, 200));
				sprintf(szTemp, "%d", m_VillageData.m_siInOutPlayer);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 199, m_siY + 99, 163, 13, szTemp, RGB(200, 200, 200));

				// 방어력
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 112, m_siY + 131, 82, 13, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_AP), RGB(200, 200, 200));
				sprintf(szTemp, "%I64d / %I64d", m_VillageData.m_CurDefPower, m_VillageData.m_MaxDefPower);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 199, m_siY + 131, 163, 13, szTemp, RGB(200, 200, 200));

				// 내부시설
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 384, m_siY + 68, 82, 13, pMyOnlineWorld->pOnlineText->Get(8000491), RGB(200, 200, 200));
		//		pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hdc, m_siX + 378, m_siY + 90, 125, m_VillageData.m_szStruct, RGB(200, 200, 200));
				pMyOnlineWorld->pOnlineMegaText->DrawTextInBox( hdc, m_siX + 380, m_siY + 90, 130, 60, NULL, m_VillageData.m_szStruct, RGB(200, 200, 200), DT_CENTER );

				// 마을안내문
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 108, m_siY + 207, 72, 12, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_VILIMPOMATION), RGB(200, 200, 200));
				// actdoll (2004/05/04 16:33) : [OECEX]
		//		if(m_VillageData.m_pEditBox)
		//		{
		//			if(m_VillageData.m_pEditBox->IsFocus())
		//				m_VillageData.m_pEditBox->DrawTextAsBox(hdc, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT), pMyOnlineWorld->pOnlineFonts->GetFontUnderline(ON_FONT_CHAT), 
		//					RGB(250, 250, 250), m_siX+107, m_siY+229, 320, NULL, TRUE);
		//			else
		//				m_VillageData.m_pEditBox->DrawTextAsBox(hdc, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT), pMyOnlineWorld->pOnlineFonts->GetFontUnderline(ON_FONT_CHAT), 
		//					RGB(250, 250, 250), m_siX+107, m_siY+229, 320, NULL, FALSE);
		//		}
				pOecEx->Draw( hdc );

				break;
		}

		pSurface->ReleaseDC(hdc);
	}
}

UI08	OnlineGovernMgr::ActionToDefenceMenu(void)			// 수성하기
{
	/*
	switch (m_VillageData.m_uiSubStatus)
	{
		break;
	}
	*/

	return ON_SUB_VILLAGEINFO_POLL;
}

UI08	OnlineGovernMgr::ActionRecruitArmy(void)			// 병력모집
{
	switch (m_VillageData.m_uiSubStatus)
	{
		case ON_SUB_VILLAGEINFO_SUB_INIT:
			{
				// 현재 상단이 생산한 공성 유닛 부대 수 및 소집가능한 인구수를 얻어온다.
				m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedUnitCount = FALSE;
				pMyOnlineWorld->pOnlineClient->SendGetGuildUnitList(0);
				
				// 현재 상단이 가지고 있는 보급품 수를 얻어온다.
				m_VillageData.m_siSupplyGoodsAmount = 0;
				/*
				pMyOnlineWorld->pOnlineClient->SendGetGuildSupplyList(0);
				
				// 현재 마을 수비병 부대 수를 얻어온다.
				m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedDefenceSoldier = FALSE;
				pMyOnlineWorld->pOnlineClient->SendVillageGarrisonCount(pVillageHeader->uiVillageCode);
				*/

				m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_POLL;
			
				// 버튼 비활성화
				// 수성하기 버튼
				m_VillageData.m_btnToDefenceMenu.SetAction(FALSE);
				// 병력 모집 버튼
				m_VillageData.m_btnRecruitArmy.SetAction(FALSE);
				// 보급품 생산 버튼
				m_VillageData.m_btnProduceSupplyGoods.SetAction(FALSE);
				// 점령선포
				m_VillageData.m_btnDeclareOccupation.SetAction(FALSE);

				// 병력모집 인터페이스 리소스 초기화
				InitVillageInfo_RecruitArmy();

				// 나가기 버튼 비활성화
				m_BExit.SetAction(FALSE);
			}
			break;

		case ON_SUB_VILLAGEINFO_SUB_POLL:
			{
				// 공성전 준비를 위하여 대방, 대행수, 행수가 상단 소속 마을에서 병력모집 가능 시간에 따른 버튼 활성화, 비활성화
				UpdateVillageInfo_BtnRecruitArmy();

				// 공성 검병 모집 버튼
				if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Swordman.Process(m_bMouseDown))
				{
					if (m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedUnitCount)
					{
						// 공성 유닛 정보 헤더 얻기
						LPSSiegeUnitTable lpsSiegeUnitTable = pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(ARRAY_INDEX_SIEGEWARFAREUNIT_SWORDMAN);

						if (lpsSiegeUnitTable)
						{
							// 모집하려는 공성 유닛의 아이템 정보 텍스트 파일의 인덱스
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyKind = lpsSiegeUnitTable->m_siID;

							// 한 사람당 부대 고용 비용
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitCost = lpsSiegeUnitTable->m_siPrice;
							// 한 부대가 되기 위한 인구수
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation = lpsSiegeUnitTable->m_siPopulation;
							// 고용비용은 m_siArmyUnitCost * m_siArmyUnitPopulation 이다.
							// 소모 보급품 수
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitSupplyGoods = 0;

							// 최대 고용할 수 있는 부대수 얻기
							SI32 siMaxArmyUintCount = m_VillageData.m_sRECRUIT_Army_InGovern.m_siRecruitablePopulation / m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation;

							// 모집 가능한 인구수가 되는지 검사
							if (siMaxArmyUintCount <= 0)
								break;

							// 계산기 인터페이스 생성
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(siMaxArmyUintCount);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_ARMY);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(m_siX + 417, m_siY + 144);

							m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_RECRUIT_FIELD_ARMY;
						}
					}
				}
				// 공성 창병 모집 버튼
				else if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Spearman.Process(m_bMouseDown))
				{
					if (m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedUnitCount)
					{
						// 공성 유닛 정보 헤더 얻기
						LPSSiegeUnitTable lpsSiegeUnitTable = pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(ARRAY_INDEX_SIEGEWARFAREUNIT_SPEARMAN);

						if (lpsSiegeUnitTable)
						{
							// 모집하려는 공성 유닛의 아이템 정보 텍스트 파일의 인덱스
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyKind = lpsSiegeUnitTable->m_siID;

							// 한 사람당 부대 고용 비용
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitCost = lpsSiegeUnitTable->m_siPrice;
							// 한 부대가 되기 위한 인구수
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation = lpsSiegeUnitTable->m_siPopulation;
							// 고용비용은 m_siArmyUnitCost * m_siArmyUnitPopulation 이다.
							// 소모 보급품 수
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitSupplyGoods = 0;

							// 최대 고용할 수 있는 부대수 얻기
							SI32 siMaxArmyUintCount = m_VillageData.m_sRECRUIT_Army_InGovern.m_siRecruitablePopulation / m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation;

							// 모집 가능한 인구수가 되는지 검사
							if (siMaxArmyUintCount <= 0)
								break;

							// 계산기 인터페이스 생성
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(siMaxArmyUintCount);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_ARMY);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(m_siX + 417, m_siY + 144);

							m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_RECRUIT_FIELD_ARMY;
						}
					}
				}
				// 공성 궁병 모집 버튼
				else if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Archerman.Process(m_bMouseDown))
				{
					if (m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedUnitCount)
					{
						// 공성 유닛 정보 헤더 얻기
						LPSSiegeUnitTable lpsSiegeUnitTable = pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(ARRAY_INDEX_SIEGEWARFAREUNIT_ARCHERMAN);

						if (lpsSiegeUnitTable)
						{
							// 모집하려는 공성 유닛의 아이템 정보 텍스트 파일의 인덱스
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyKind = lpsSiegeUnitTable->m_siID;

							// 한 사람당 부대 고용 비용
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitCost = lpsSiegeUnitTable->m_siPrice;
							// 한 부대가 되기 위한 인구수
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation = lpsSiegeUnitTable->m_siPopulation;
							// 고용비용은 m_siArmyUnitCost * m_siArmyUnitPopulation 이다.
							// 소모 보급품 수
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitSupplyGoods = 0;

							// 최대 고용할 수 있는 부대수 얻기
							SI32 siMaxArmyUintCount = m_VillageData.m_sRECRUIT_Army_InGovern.m_siRecruitablePopulation / m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation;

							// 모집 가능한 인구수가 되는지 검사
							if (siMaxArmyUintCount <= 0)
								break;

							// 계산기 인터페이스 생성
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(siMaxArmyUintCount);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_ARMY);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(m_siX + 417, m_siY + 144);

							m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_RECRUIT_FIELD_ARMY;
						}
					}
				}
				// 공성 기마병 모집 버튼
				else if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Horseman.Process(m_bMouseDown))
				{
					if (m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedUnitCount)
					{
						// 공성 유닛 정보 헤더 얻기
						LPSSiegeUnitTable lpsSiegeUnitTable = pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(ARRAY_INDEX_SIEGEWARFAREUNIT_HORSEMAN);

						if (lpsSiegeUnitTable)
						{
							// 모집하려는 공성 유닛의 아이템 정보 텍스트 파일의 인덱스
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyKind = lpsSiegeUnitTable->m_siID;

							// 한 사람당 부대 고용 비용
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitCost = lpsSiegeUnitTable->m_siPrice;
							// 한 부대가 되기 위한 인구수
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation = lpsSiegeUnitTable->m_siPopulation;
							// 고용비용은 m_siArmyUnitCost * m_siArmyUnitPopulation 이다.
							// 소모 보급품 수
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitSupplyGoods = 0;

							// 최대 고용할 수 있는 부대수 얻기
							SI32 siMaxArmyUintCount = m_VillageData.m_sRECRUIT_Army_InGovern.m_siRecruitablePopulation / m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation;

							// 모집 가능한 인구수가 되는지 검사
							if (siMaxArmyUintCount <= 0)
								break;

							// 계산기 인터페이스 생성
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(siMaxArmyUintCount);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_ARMY);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(m_siX + 417, m_siY + 144);

							m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_RECRUIT_FIELD_ARMY;
						}
					}
				}
				// 공성 충차 모집 버튼
				else if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Siege_tower.Process(m_bMouseDown))
				{
					if (m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedUnitCount)
					{
						// 공성 유닛 정보 헤더 얻기
						LPSSiegeUnitTable lpsSiegeUnitTable = pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(ARRAY_INDEX_SIEGEWARFAREUNIT_SIEGETOWER);

						if (lpsSiegeUnitTable)
						{
							// 모집하려는 공성 유닛의 아이템 정보 텍스트 파일의 인덱스
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyKind = lpsSiegeUnitTable->m_siID;

							// 한 사람당 부대 고용 비용
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitCost = lpsSiegeUnitTable->m_siPrice;
							// 한 부대가 되기 위한 인구수
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation = lpsSiegeUnitTable->m_siPopulation;
							// 고용비용은 m_siArmyUnitCost * m_siArmyUnitPopulation 이다.
							// 소모 보급품 수
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitSupplyGoods = 0;

							// 최대 고용할 수 있는 부대수 얻기
							SI32 siMaxArmyUintCount = m_VillageData.m_sRECRUIT_Army_InGovern.m_siRecruitablePopulation / m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation;

							// 모집 가능한 인구수가 되는지 검사
							if (siMaxArmyUintCount <= 0)
								break;

							// 계산기 인터페이스 생성
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(siMaxArmyUintCount);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_ARMY);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(m_siX + 417, m_siY + 144);

							m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_RECRUIT_FIELD_ARMY;
						}
					}
				}
				// 공성 발석거 모집 버튼
				else if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Catapult.Process(m_bMouseDown))
				{
					if (m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedUnitCount)
					{
						// 공성 유닛 정보 헤더 얻기
						LPSSiegeUnitTable lpsSiegeUnitTable = pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(ARRAY_INDEX_SIEGEWARFAREUNIT_CATAPULT);

						if (lpsSiegeUnitTable)
						{
							// 모집하려는 공성 유닛의 아이템 정보 텍스트 파일의 인덱스
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyKind = lpsSiegeUnitTable->m_siID;

							// 한 사람당 부대 고용 비용
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitCost = lpsSiegeUnitTable->m_siPrice;
							// 한 부대가 되기 위한 인구수
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation = lpsSiegeUnitTable->m_siPopulation;
							// 고용비용은 m_siArmyUnitCost * m_siArmyUnitPopulation 이다.
							// 소모 보급품 수
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitSupplyGoods = 0;

							// 최대 고용할 수 있는 부대수 얻기
							SI32 siMaxArmyUintCount = m_VillageData.m_sRECRUIT_Army_InGovern.m_siRecruitablePopulation / m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation;

							// 모집 가능한 인구수가 되는지 검사
							if (siMaxArmyUintCount <= 0)
								break;

							// 계산기 인터페이스 생성
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(siMaxArmyUintCount);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_ARMY);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(m_siX + 417, m_siY + 144);

							m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_RECRUIT_FIELD_ARMY;
						}
					}
				}
				// 공성 보급 수레 모집 버튼
				else if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Supply_wagon.Process(m_bMouseDown))
				{
					if (m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedUnitCount)
					{
						// 공성 유닛 정보 헤더 얻기
						LPSSiegeUnitTable lpsSiegeUnitTable = pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(ARRAY_INDEX_SIEGEWARFAREUNIT_SUPPLYWAGON);

						if (lpsSiegeUnitTable)
						{
							// 모집하려는 공성 유닛의 아이템 정보 텍스트 파일의 인덱스
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyKind = lpsSiegeUnitTable->m_siID;

							// 한 사람당 부대 고용 비용
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitCost = lpsSiegeUnitTable->m_siPrice;
							// 한 부대가 되기 위한 인구수
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation = lpsSiegeUnitTable->m_siPopulation;
							// 고용비용은 m_siArmyUnitCost * m_siArmyUnitPopulation 이다.
							// 소모 보급품 수
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitSupplyGoods = 0;

							// 최대 고용할 수 있는 부대수 얻기
							SI32 siMaxArmyUintCount = m_VillageData.m_sRECRUIT_Army_InGovern.m_siRecruitablePopulation / m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation;

							// 모집 가능한 인구수가 되는지 검사
							if (siMaxArmyUintCount <= 0)
								break;

							// 계산기 인터페이스 생성
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(siMaxArmyUintCount);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_ARMY);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(m_siX + 417, m_siY + 144);

							m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_RECRUIT_FIELD_ARMY;
						}
					}
				}
				/*
				// 공성 수비병(주둔군) 모집 버튼
				else if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Garrison.Process(m_bMouseDown))
				{
					if (m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedDefenceSoldier)
					{
						// 공성전 소집권 아이템 정보 헤더 얻기
						CItemHeader *pItemHeader = pMyOnlineWorld->pItemsInfo->GetItemHeaderFromImperium(TransKindImjinToOnline(KIND_ON_SIEGEWARFARE_GARRISON_ARMY, FALSE));

						if (pItemHeader)
						{
							// 모집하려는 공성 유닛의 아이템 정보 텍스트 파일의 인덱스
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyKind = pItemHeader->m_pImperium->m_siID;

							// 한 사람당 부대 고용 비용
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitCost = pItemHeader->m_pImperium->m_siPrice;
							// 한 부대가 되기 위한 인구수
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation = pItemHeader->m_pImperium->m_siPopulation;
							// 고용비용은 m_siArmyUnitCost * m_siArmyUnitPopulation 이다.
							// 소모 보급품 수
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitSupplyGoods = pItemHeader->m_pImperium->m_siSupply;

							// 최대 고용할 수 있는 부대수 얻기
							SI32 siMaxArmyUintCount = m_VillageData.m_sRECRUIT_Army_InGovern.m_siRecruitablePopulation / m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation;

							// 모집 가능한 인구수가 되는지 검사
							if (siMaxArmyUintCount <= 0)
								break;

							// 계산기 인터페이스 생성
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(siMaxArmyUintCount);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_ARMY);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(m_siX + 417, m_siY + 144);

							m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_RECRUIT_GARRISON_ARMY;
						}
					}
				}
				*/
				// 상위 메뉴로 돌아가기 버튼
				else if (m_VillageData.m_btnBack.Process(m_bMouseDown))
				{
					m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_EXIT;
				}

				char szTitle[64], szContent[1024];
				ZeroMemory(szTitle, sizeof(szTitle));
				ZeroMemory(szContent, sizeof(szContent));

				/****************************************************************************************/
				// robypark 2005/1/6 11:57
				// 모집하려는 병과 버튼 풍선 도움말 처리하기
				// 공성 검병 모집 버튼
				if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Swordman.IsInMouse())
				{
					strcpy(szTitle, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SWORDMAN));
					strcpy(szContent, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TOOLTIP_RECRUITARMY_SWORDMAN));
				}
				// 공성 창병 모집 버튼
				else if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Spearman.IsInMouse())
				{
					strcpy(szTitle, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SPEARMAN));
					strcpy(szContent, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TOOLTIP_RECRUITARMY_SPEARMAN));
				}
				// 공성 궁병 모집 버튼
				else if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Archerman.IsInMouse())
				{
					strcpy(szTitle, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ARCHER));
					strcpy(szContent, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TOOLTIP_RECRUITARMY_ARCHER));
				}
				// 공성 기마병 모집 버튼
				else if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Horseman.IsInMouse())
				{
					strcpy(szTitle, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HORSEMAN));
					strcpy(szContent, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TOOLTIP_RECRUITARMY_HORSEMAN));
				}
				// 공성 충차 모집 버튼
				else if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Siege_tower.IsInMouse())
				{
					strcpy(szTitle, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SIEGETOWER));
					strcpy(szContent, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TOOLTIP_RECRUITARMY_SIEGETOWER));
				}
				// 공성 발석거 모집 버튼
				else if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Catapult.IsInMouse())
				{
					strcpy(szTitle, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CATAPULT));
					strcpy(szContent, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TOOLTIP_RECRUITARMY_CATAPULT));
				}
				// 공성 보급 수레 모집 버튼
				else if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Supply_wagon.IsInMouse())
				{
					strcpy(szTitle, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SUPPLYGOODS_WAGON));
					strcpy(szContent, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TOOLTIP_RECRUITARMY_SUPPLYGOODSWAGON));
				}
				/*
				// 공성 수비병(주둔군) 모집 버튼
				else if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Garrison.IsInMouse())
				{
					strcpy(szTitle, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GARRISON));
					strcpy(szContent, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TOOLTIP_RECRUITARMY_GARRISON));
				}
				*/

				// 출력해야할 풍선 도움말이 있다면
				if ('\0' != szTitle[0])
				{
					pMyOnlineWorld->pOnlineHelp->SetTextHasTitle( szTitle, szContent, IpD.Mouse_X, IpD.Mouse_Y, ON_HELP_LEFT | ON_HELP_VCENTER, TRUE, RGB(255, 255, 255), RGB(255, 255, 255) );
				}
				/****************************************************************************************/
			}
			break;
		case ON_SUB_VILLAGEINFO_SUB_WAIT:
			{
				// 응답을 기다린다.
				pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
			}
			break;
		case ON_SUB_VILLAGEINFO_SUB_EXIT:
			{
				m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_INIT;

				// 관청 정보 탭에서 사용되는 버튼 수성하기, 병력모집, 보급품 생산, 점령선포 버튼 상태 설정(활성, 비활성)
				UpdateVillageInfoButton();

				// 나가기 버튼 활성화
				m_BExit.SetAction(TRUE);

				// 병력모집 인터페이스 리소스 해제
				FreeVillageInfo_RecruitArmy();

				return ON_SUB_VILLAGEINFO_POLL;
			}
			break;

		case ON_SUB_VILLAGEINFO_SUB_RECRUIT_FIELD_ARMY:		// 필드 전투 가능한 공성전 유닛 모집
			{
				// 계산기에서 확인 버튼이 눌려져, 유효한 값이 있는 경우
				if(pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() && pMyOnlineWorld->pOnlineNumPad->siCount > 0)
				{
					// 공성전 준비를 위하여 대방, 대행수, 행수가 상단 소속 마을에서 병력모집 가능 시간이며
					if (FALSE == pMyOnlineWorld->IsInTimeDisabledRecruitArmyInGovernment())
					{
						m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_WAIT;

						// 공성 유닛 정보 헤더 얻기
						LPSSiegeUnitTable lpsSiegeUnitTable = pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyKind);

						if (lpsSiegeUnitTable)
						{
							// 공성 병력 소집 요청 메시지 전달
							pMyOnlineWorld->pOnlineClient->SendIncreaseGuildUnit(lpsSiegeUnitTable->m_uiKind, pMyOnlineWorld->pOnlineNumPad->siCount, pVillageHeader->uiVillageCode);
						}
						else
						{
							m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_POLL;
						}
					}
					else
					{
						m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_POLL;

						/*
						// 병력 모집 가능 시간이 아니라는 메시지 박스  출력
						pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, 
														pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT),
														pMyOnlineWorld->pOnlineText->Get(ON_TEXT_DISABLED_TIME_RECRUITARMY_IN_GOV), 
														pMyOnlineWorld->GetSOptionSiegeWarfare()->siSiegeWarfareTime_RecruitArmy_Begin,
														pMyOnlineWorld->GetSOptionSiegeWarfare()->siSiegeWarfareTime_RecruitArmy_End);
						*/
					}

					//m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedUnitCount = FALSE;

					pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
				}
				else if(pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() && pMyOnlineWorld->pOnlineNumPad->siCount == 0)
				{
					pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
					m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_POLL;
				}
			}
			break;
		case ON_SUB_VILLAGEINFO_SUB_RECRUIT_GARRISON_ARMY:	// 마을 주둔군 모집
			{
				// 계산기에서 확인 버튼이 눌려져, 유효한 값이 있는 경우
				if(pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() && pMyOnlineWorld->pOnlineNumPad->siCount > 0)
				{
					// 공성전 준비를 위하여 대방, 대행수, 행수가 상단 소속 마을에서 병력모집 가능 시간이며
					if (FALSE == pMyOnlineWorld->IsInTimeDisabledRecruitArmyInGovernment())
					{
						m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_WAIT;

						// 공성 마을 주둔군 증가 
						pMyOnlineWorld->pOnlineClient->SendIncreaseVillageDefenceSoldier(pMyOnlineWorld->pOnlineNumPad->siCount, pVillageHeader->uiVillageCode);
					}
					else
					{
						m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_POLL;

						// 병력 모집 가능 시간이 아니라는 메시지 박스  출력
						pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, 
														pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT),
														pMyOnlineWorld->pOnlineText->Get(ON_TEXT_DISABLED_TIME_RECRUITARMY_IN_GOV), 
														pMyOnlineWorld->GetSOptionSiegeWarfare()->siSiegeWarfareTime_RecruitArmy_Begin,
														pMyOnlineWorld->GetSOptionSiegeWarfare()->siSiegeWarfareTime_RecruitArmy_End);
					}

					pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
				}
				else if(pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() && pMyOnlineWorld->pOnlineNumPad->siCount == 0)
				{
					pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
					m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_POLL;
				}
			}
			break;
	}

	return ON_SUB_VILLAGEINFO_RECRUIT_ARMY;
}

// 병력모집 인터페이스 그리기
VOID	OnlineGovernMgr::DrawRecruitArmy(LPDIRECTDRAWSURFACE7 pSurface)
{
}

UI08	OnlineGovernMgr::ActionProduceSupplyGoods(void)		// 보급품 생산
{
	switch (m_VillageData.m_uiSubStatus)
	{
		case ON_SUB_VILLAGEINFO_SUB_INIT:
			{
				// 현재 상단이 가지고 있는 보급품 수를 얻어온다.
				m_VillageData.m_siSupplyGoodsAmount = 0;
				pMyOnlineWorld->pOnlineClient->SendGetGuildSupplyList(0);
				// 사용자의 소지금 얻기

				m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_POLL;

				// 보급품 생산 인터페이스 초기화
				InitVillageInfo_ProduceSupplyGoods();
			
				// 버튼 비활성화
				// 수성하기 버튼
				m_VillageData.m_btnToDefenceMenu.SetAction(FALSE);
				// 병력 모집 버튼
				m_VillageData.m_btnRecruitArmy.SetAction(FALSE);
				// 보급품 생산 버튼
				m_VillageData.m_btnProduceSupplyGoods.SetAction(FALSE);
				// 점령선포
				m_VillageData.m_btnDeclareOccupation.SetAction(FALSE);

				// 나가기 버튼 비활성화
				m_BExit.SetAction(FALSE);
			}
			break;

		case ON_SUB_VILLAGEINFO_SUB_POLL:
			{
				// 공성전 준비를 위하여 대방, 대행수, 행수가 상단 소속 마을에서 병력모집 가능 시간이며
				if(FALSE == pMyOnlineWorld->IsInTimeDisabledRecruitArmyInGovernment())
					m_VillageData.m_sProduceSupplyGoods.m_btnProduceSupplyGoods.SetAction(TRUE);
				else
					m_VillageData.m_sProduceSupplyGoods.m_btnProduceSupplyGoods.SetAction(FALSE);

				// 보급품 생산 버튼
				if (m_VillageData.m_sProduceSupplyGoods.m_btnProduceSupplyGoods.Process(m_bMouseDown))
				{
					// 공성전 준비를 위하여 대방, 대행수, 행수가 상단 소속 마을에서 보급품 생산 가능 시간이며
					if (FALSE == pMyOnlineWorld->IsInTimeDisabledRecruitArmyInGovernment())
					{
						// 소지금이 있다면
						if (pMyOnlineWorld->pMyData->GetMoney() > 0)
						{
							// 최대 생산할 수 있는 보급품 개수 얻기
							SI32 siMaxSupplyGoodsCount = pMyOnlineWorld->pMyData->GetMoney() / m_VillageData.m_sProduceSupplyGoods.m_siCostSupplyGoods;

							// 돈이 되는지 검사
							if (siMaxSupplyGoodsCount <= 0)
								break;

							// 계산기 인터페이스 생성
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(siMaxSupplyGoodsCount);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_THING);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(m_VillageData.m_sProduceSupplyGoods.m_btnProduceSupplyGoods.GetX() + 30, m_VillageData.m_sProduceSupplyGoods.m_btnProduceSupplyGoods.GetY() + 15);

							m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_PRODUCE_SUPPLYGOODS;
						}
					}
				}
				// 상위 메뉴로 돌아가기 버튼
				else if (m_VillageData.m_btnBack.Process(m_bMouseDown))
				{
					m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_EXIT;
				}
			}
			break;
		case ON_SUB_VILLAGEINFO_SUB_WAIT:
			{
				// 응답을 기다린다.
				pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
			}
			break;
		case ON_SUB_VILLAGEINFO_SUB_EXIT:
			{
				m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_INIT;

				// 보급품 생산 인터페이스 해제
				FreeVillageInfo_ProduceSupplyGoods();

				// 관청 정보 탭에서 사용되는 버튼 수성하기, 병력모집, 보급품 생산, 점령선포 버튼 상태 설정(활성, 비활성)
				UpdateVillageInfoButton();

				// 나가기 버튼 활성화
				m_BExit.SetAction(TRUE);

				return ON_SUB_VILLAGEINFO_POLL;
			}
			break;
		// 마을 보급품 생산
		case ON_SUB_VILLAGEINFO_SUB_PRODUCE_SUPPLYGOODS:
			{
				// 계산기에서 확인 버튼이 눌려져, 유효한 값이 있는 경우
				if(pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() && pMyOnlineWorld->pOnlineNumPad->siCount > 0)
				{
					// 공성전 준비를 위하여 대방, 대행수, 행수가 상단 소속 마을에서 보급품 생산 가능 시간이며
					if (FALSE == pMyOnlineWorld->IsInTimeDisabledRecruitArmyInGovernment())
					{
						m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_WAIT;

						// 상단 보급품 생산 요청 메시지
						pMyOnlineWorld->pOnlineClient->SendIncreaseGuildSupply(pMyOnlineWorld->pOnlineNumPad->siCount, 0);
					}
					else
					{
						m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_POLL;

						// 보급품 생산 가능 시간이 아니라는 메시지 박스  출력
						pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, 
														pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT),
														pMyOnlineWorld->pOnlineText->Get(ON_TEXT_DISABLED_TIME_PRODUCE_SUPPLYGOODS_GOV), 
														pMyOnlineWorld->GetSOptionSiegeWarfare()->siSiegeWarfareTime_RecruitArmy_Begin,
														pMyOnlineWorld->GetSOptionSiegeWarfare()->siSiegeWarfareTime_RecruitArmy_End);
					}

					pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
				}
				else if(pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() && pMyOnlineWorld->pOnlineNumPad->siCount == 0)
				{
					pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
					m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_POLL;
				}
			}
			break;
	}

	return ON_SUB_VILLAGEINFO_PRODUCE_SUPPLYGOODS;
}

UI08	OnlineGovernMgr::ActionDeclareOccupation(void)		// 점령선포
{
	return ON_SUB_VILLAGEINFO_POLL;
}

// robypark 2004/11/16 15:36
// 공성전 상단 공성 유닛 리스트 요청 응답
// ON_RESPONSE_LIST_GUILD_UNIT
UI16 OnlineGovernMgr::OnResponseSiegeWarfareUnitList(BYTE *pMsg)
{
	OnResGetGuildUnitList	*pOnResGetGuildUnitList = (OnResGetGuildUnitList *)pMsg;

//	ZeroMemory(m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount, sizeof(SI32) * ON_MAX_GUILD_UNIT);

	// 공성 검병 부대수 설정
	m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SWORDMAN] = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SWORDMAN];
	// 공성 창병 부대수 설정
	m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SPEARMAN] = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SPEARMAN];
	// 공성 궁병 부대수 설정
	m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_ARCHERMAN] = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_ARCHERMAN];
	// 공성 기마병 부대수 설정
	m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_HORSEMAN] = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_HORSEMAN];
	// 공성 충차 부대수 설정
	m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SIEGETOWER] = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SIEGETOWER];
	// 공성 발석거 부대수 설정
	m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_CATAPULT] = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_CATAPULT];
	// 공성 보급수레 부대수 설정
	m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SUPPLYWAGON] = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SUPPLYWAGON];

	// 고용 가능한 마을 인구수 설정
	m_VillageData.m_sRECRUIT_Army_InGovern.m_siRecruitablePopulation = pOnResGetGuildUnitList->uiPopulation;

	m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedUnitCount = TRUE;

	return 0;
}

// 관청에서 대방이나 행수가 병력 모집을 요청한 응답 메시지 처리
UI16 OnlineGovernMgr::OnIncreaseGuildUnit(BYTE* pMsg)
{
	OnResIncreaseGuildUnit*			pOnResIncreaseGuildUnit;

	pOnResIncreaseGuildUnit			=	(OnResIncreaseGuildUnit*)pMsg;

	m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_POLL;

	if (ON_RET_OK == pOnResIncreaseGuildUnit->dwResponse)
	{
		// 현재 상단이 생산한 공성 유닛 부대 수 및 소집가능한 인구수를 얻어온다.
		m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedUnitCount = FALSE;
		pMyOnlineWorld->pOnlineClient->SendGetGuildUnitList(0);
		/*
		// 현재 상단이 가지고 있는 보급품 수를 얻어온다.
		m_VillageData.m_siSupplyGoodsAmount = 0;
		pMyOnlineWorld->pOnlineClient->SendGetGuildSupplyList(0);
		*/
	
		// 변경되는 마을 정보 새로 요청. 마을 인구수 관련
		if(pMyOnlineWorld->pOnlineVillage->GetVillageHeader()->uiVillageCode)
			pMyOnlineWorld->pOnlineClient->SendRequestVillageInfo(pMyOnlineWorld->pOnlineVillage->GetVillageHeader()->uiVillageCode);

		// 소지금 변경
		pMyOnlineWorld->pMyData->SetMoney(pMyOnlineWorld->pMyData->GetMoney() - pOnResIncreaseGuildUnit->mnMoney);

		// 소지금 및 보급품 수 변화 효과 처리
		SEffectData					EffectData;
		ZeroMemory(&EffectData, sizeof(SEffectData));

		EffectData.m_Type = EFFECTTYPE_CHANGENUMBER;
		EffectData.m_EffectData_ChangeNum.m_Frame			= 0;
		EffectData.m_EffectData_ChangeNum.m_XPos			= m_siX + 104;
		EffectData.m_EffectData_ChangeNum.m_YPos			= m_siY + 354;
		EffectData.m_EffectData_ChangeNum.m_ChangeNumber    = -pOnResIncreaseGuildUnit->mnMoney;
		pMyOnlineWorld->pOnlineFieldEffect->AddEffect(EffectData);
		
		/*
		// 보급품 수 변화 effect
		ZeroMemory(&EffectData, sizeof(SEffectData));

		EffectData.m_Type = EFFECTTYPE_CHANGENUMBER;
		EffectData.m_EffectData_ChangeNum.m_Frame			= 0;
		EffectData.m_EffectData_ChangeNum.m_XPos			= m_siX + 474;
		EffectData.m_EffectData_ChangeNum.m_YPos			= m_siY + 354;
		EffectData.m_EffectData_ChangeNum.m_ChangeNumber    = -pOnResIncreaseGuildUnit->siSupply;
		pMyOnlineWorld->pOnlineFieldEffect->AddEffect(EffectData);
		*/
	}
	else if (ON_RET_NOT_ENOUGH_MONEY == pOnResIncreaseGuildUnit->dwResponse)			// 돈이 부족하다.
	{
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ERRORNOTMONEY));
	}
	else if (ON_RET_NOT_ENOUGH_SUPPLY == pOnResIncreaseGuildUnit->dwResponse)			// 보급이 부족하다.
	{
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_LACK_SUPPLYGOODS_ERROR));
	}
	else if (ON_RET_NOT_ENOUGH_POPULATION == pOnResIncreaseGuildUnit->dwResponse)			// 인구수가 부족하다.
	{
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_LACK_RECRUITABLED_POPULATION_ERROR));
	}
	else if (ON_RET_NOT_LIMIT_POPULATION == pOnResIncreaseGuildUnit->dwResponse)
	{
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_OVERFLOW_ALLOWED_POPULATION_ERROR));
	}
	else 
	{
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_BOARD_DELFAIL), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_UNKNOWMESSAGE), pOnResIncreaseGuildUnit->dwResponse);
		// (ON_RET_NOT == pOnResIncreaseGuildUnit->dwResponse)			// 그밖의 에러.
	}
	
	return 0;
}

// 공성전 마을 수비병 증가 요청 메시지 응답
UI16 OnlineGovernMgr::OnResponseIncreaseVillageDefenceSoldier(BYTE *pMsg)
{
	OnResVillageDefenceSoldier	*pOnResVillageDefenceSoldier = (OnResVillageDefenceSoldier *)pMsg;

	m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_POLL;

	if (ON_RET_OK == pOnResVillageDefenceSoldier->dwResponse)
	{
		// 현재 상단이 생산한 공성 유닛 부대 수 및 소집가능한 인구수를 얻어온다.
		m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedUnitCount = FALSE;
		pMyOnlineWorld->pOnlineClient->SendGetGuildUnitList(0);
		// 현재 마을 수비병 부대 수를 얻어온다.
		m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedDefenceSoldier = FALSE;
		pMyOnlineWorld->pOnlineClient->SendVillageGarrisonCount(pVillageHeader->uiVillageCode);
		// 현재 상단이 가지고 있는 보급품 수를 얻어온다.
		m_VillageData.m_siSupplyGoodsAmount = 0;
		pMyOnlineWorld->pOnlineClient->SendGetGuildSupplyList(0);

		// 변경되는 마을 정보 새로 요청. 마을 인구수 관련
//		if(pMyOnlineWorld->pOnlineMap->m_MapInfo.uiVillageCode)
//			pMyOnlineWorld->pOnlineClient->SendRequestVillageInfo(pMyOnlineWorld->pOnlineMap->m_MapInfo.uiVillageCode);
		pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.uiPopulation -= pOnResVillageDefenceSoldier->siDefenceSoldier/* 부대수가 아니라 인구수이다.*/;

		// 소지금 변경
		pMyOnlineWorld->pMyData->SetMoney(pMyOnlineWorld->pMyData->GetMoney() - pOnResVillageDefenceSoldier->mnMoney);
	
		// 소지금 및 보급품 수 변화 효과 처리
		SEffectData					EffectData;
		ZeroMemory(&EffectData, sizeof(SEffectData));

		EffectData.m_Type = EFFECTTYPE_CHANGENUMBER;
		EffectData.m_EffectData_ChangeNum.m_Frame			= 0;
		EffectData.m_EffectData_ChangeNum.m_XPos			= m_siX + 104;
		EffectData.m_EffectData_ChangeNum.m_YPos			= m_siY + 354;
		EffectData.m_EffectData_ChangeNum.m_ChangeNumber    = -pOnResVillageDefenceSoldier->mnMoney;
		pMyOnlineWorld->pOnlineFieldEffect->AddEffect(EffectData);
		
		// 보급품 수 변화 effect
		ZeroMemory(&EffectData, sizeof(SEffectData));

		EffectData.m_Type = EFFECTTYPE_CHANGENUMBER;
		EffectData.m_EffectData_ChangeNum.m_Frame			= 0;
		EffectData.m_EffectData_ChangeNum.m_XPos			= m_siX + 474;
		EffectData.m_EffectData_ChangeNum.m_YPos			= m_siY + 354;
		EffectData.m_EffectData_ChangeNum.m_ChangeNumber    = -pOnResVillageDefenceSoldier->siSupply;
		pMyOnlineWorld->pOnlineFieldEffect->AddEffect(EffectData);
	}
	else if (ON_RET_NOT_ENOUGH_MONEY == pOnResVillageDefenceSoldier->dwResponse)			// 돈이 부족하다.
	{
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ERRORNOTMONEY));
	}
	else if (ON_RET_NOT_ENOUGH_SUPPLY == pOnResVillageDefenceSoldier->dwResponse)			// 보급이 부족하다.
	{
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_LACK_SUPPLYGOODS_ERROR));
	}
	else if (ON_RET_NOT_ENOUGH_POPULATION == pOnResVillageDefenceSoldier->dwResponse)			// 인구수가 부족하다.
	{
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_LACK_RECRUITABLED_POPULATION_ERROR));
	}
	else if (ON_RET_NOT_LIMIT_POPULATION == pOnResVillageDefenceSoldier->dwResponse)
	{
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_OVERFLOW_ALLOWED_POPULATION_ERROR));
	}
	else 
	{
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_BOARD_DELFAIL), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_UNKNOWMESSAGE), pOnResVillageDefenceSoldier->dwResponse);
		// (ON_RET_NOT == pOnResIncreaseGuildUnit->dwResponse)			// 그밖의 에러.
	}
	
	return 0;
}

// 공성전 마을 수비병 수 얻기
// ON_RESPONSE_GET_VILLAGE_DEFENCE_SOLDIER
UI16 OnlineGovernMgr::OnResponseGetVillageDefenceSoldier(BYTE *pMsg)
{
	OnResGetVillageDefenceSoldier	*pOnResGetVillageDefenceSoldier = (OnResGetVillageDefenceSoldier *)pMsg;

	m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_DEFENCESOLIDER] = pOnResGetVillageDefenceSoldier->siDefenceSoldier;

	m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedDefenceSoldier = TRUE;

	return 0;
}

// 공성전 상단 보급품 개수 얻기
// ON_RESPONSE_LIST_GUILD_SUPPLY
UI16 OnlineGovernMgr::OnResponseListGuildSupply(BYTE *pMsg)
{
	OnResGetGuildSupplyList *pOnResGetGuildSupplyList = (OnResGetGuildSupplyList *)pMsg;

	m_VillageData.m_siSupplyGoodsAmount = pOnResGetGuildSupplyList->siGuildSupply;

	return 0;
}

// 공성전 상단 보급품 생산 요청 응답
// ON_RESPONSE_INCREASE_GUILD_SUPPLY
UI16 OnlineGovernMgr::OnResponseIncreaseGuildSupply(BYTE *pMsg)
{
	OnResIncreaseGuildSupply *pOnResIncreaseGuildSupply = (OnResIncreaseGuildSupply *)pMsg;

	m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_POLL;

	if (ON_RET_OK == pOnResIncreaseGuildSupply->dwResponse)
	{
		// 소지금 변경
		pMyOnlineWorld->pMyData->SetMoney(pMyOnlineWorld->pMyData->GetMoney() - pOnResIncreaseGuildSupply->uiMoney);

		// 현재 상단이 가지고 있는 변경된 보급품 수를 얻어온다.
		m_VillageData.m_siSupplyGoodsAmount = pOnResIncreaseGuildSupply->siSupplyQuantity;
		pMyOnlineWorld->pOnlineClient->SendGetGuildSupplyList(0);

		// 소지금 변화 효과 처리
		SEffectData					EffectData;
		ZeroMemory(&EffectData, sizeof(SEffectData));

		EffectData.m_Type = EFFECTTYPE_CHANGENUMBER;
		EffectData.m_EffectData_ChangeNum.m_Frame			= 0;
		EffectData.m_EffectData_ChangeNum.m_XPos			= m_siX + 104;
		EffectData.m_EffectData_ChangeNum.m_YPos			= m_siY + 354;
		EffectData.m_EffectData_ChangeNum.m_ChangeNumber    = -pOnResIncreaseGuildSupply->uiMoney;
		pMyOnlineWorld->pOnlineFieldEffect->AddEffect(EffectData);
	}
	else if (ON_RET_NOT_ENOUGH_MONEY == pOnResIncreaseGuildSupply->dwResponse)
	{
		// 소지금 부족
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ERRORNOTMONEY));
	}
	else
	{
		// 생산 실패
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FAIL_TO_PRODUCE_SUPPLYGOODS));
	}

	return 0;
}

