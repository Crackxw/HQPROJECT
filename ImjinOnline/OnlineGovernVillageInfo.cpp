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

// ���� ���� ������
#include	"SiegeUnitTableMgr.h"

extern	_clGame* pGame;
extern _InputDevice				IpD;


VOID OnlineGovernMgr::InitVillageInfo()
{
	m_BVillageInfo.SetAction(FALSE);

	clGrp.LoadXspr("Online\\GameSpr\\Village\\town_office_info.spr", m_VillageData.m_MainImage);
	clGrp.LoadXspr("Online\\GameSpr\\Village\\enter_button.spr", m_VillageData.m_DescOkImage);
	clGrp.LoadXspr("Online\\GameSpr\\Village\\town_button_medium.spr", m_VillageData.m_ButImage);

	// �α�
	m_VillageData.m_uiPopulation = pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.uiPopulation;

	// ���νü�
	ZeroMemory(m_VillageData.m_szStruct, sizeof(m_VillageData.m_szStruct));
	pMyOnlineWorld->pOnlineVillage->MakeVillageInStructor((char*)m_VillageData.m_szStruct);

	// �����ȳ���
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
	// �������� ���� ��ư �߰�
	// ���� ���� ��ư
	m_VillageData.m_btnRecruitArmy.Create(m_siX+16, m_siY+158, m_VillageData.m_ButImage.Header.Xsize, m_VillageData.m_ButImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_RECRUIT_ARMY), BUTTON_PUT_LEFT, FALSE);

	/*
	// �����ϱ� ��ư
	m_VillageData.m_btnToDefenceMenu.Create(m_siX+110, m_siY+164, m_VillageData.m_ButImage.Header.Xsize, m_VillageData.m_ButImage.Header.Ysize, "�����ϱ�", BUTTON_PUT_LEFT, FALSE);
	m_VillageData.m_btnToDefenceMenu.ShowWindow(FALSE);
	// ���� ���� ��ư
//	m_VillageData.m_btnRecruitArmy.Create(m_siX+110, m_siY+164, m_VillageData.m_ButImage.Header.Xsize, m_VillageData.m_ButImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_RECRUIT_ARMY), BUTTON_PUT_LEFT, FALSE);
	m_VillageData.m_btnRecruitArmy.Create(m_siX+16, m_siY+158, m_VillageData.m_ButImage.Header.Xsize, m_VillageData.m_ButImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_RECRUIT_ARMY), BUTTON_PUT_LEFT, FALSE);
	// ����ǰ ���� ��ư
	m_VillageData.m_btnProduceSupplyGoods.Create(m_siX+227, m_siY+164, m_VillageData.m_ButImage.Header.Xsize, m_VillageData.m_ButImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_PRODUCE_SUPPLY_GOODS), BUTTON_PUT_LEFT, FALSE);
	// ���ɼ���
	m_VillageData.m_btnDeclareOccupation.Create(m_siX+344, m_siY+164, m_VillageData.m_ButImage.Header.Xsize, m_VillageData.m_ButImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_DECLARE_OCCUPATION), BUTTON_PUT_LEFT, FALSE);
	*/

	// ���� �����̰ų� ��ڸ��� �ȳ����� �� �� �ִ�.
	if((strcmp(pMyOnlineWorld->pOnlineVillage->GetMaxInvestmentID(), pMyOnlineWorld->pMyData->GetMyCharName()) == 0)
		|| pMyOnlineWorld->m_GMFlag == ON_USERGRADE_ADMIN5)
	{
		m_VillageData.m_BDescOk.SetAction(TRUE);
	}

	// ���� ������ ��� ���� ���� ���� ����
	if(strcmp(pMyOnlineWorld->pOnlineVillage->GetMaxInvestmentID(), pMyOnlineWorld->pMyData->GetMyCharName()) == 0)
	{
		m_VillageData.m_BDefence.SetAction(TRUE);
	}
	
	// ���� ����� �������� ���� ���ڰ���
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

	// ��û ���� �ǿ��� ���Ǵ� ��ư �����ϱ�, ���¸���, ����ǰ ����, ���ɼ��� ��ư ���� ����(Ȱ��, ��Ȱ��)
	UpdateVillageInfoButton();
}

// ��û ���� �ǿ��� ���Ǵ� ��ư �����ϱ�, ���¸���, ����ǰ ����, ���ɼ��� ��ư ���� ����(Ȱ��, ��Ȱ��)
void	OnlineGovernMgr::UpdateVillageInfoButton(void)
{
	// ��û�� ���� ������� ��� �� ���� ���
	SI32 siMyGuildClass = pMyOnlineWorld->pMyData->GetGuildClass();

	// ������ ����� �����ϸ�
	if(pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID[0])
	{
		// ����ڰ� ��� ������ ���, ���� �ְ� �������̸�
		if ((siMyGuildClass == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
			|| (strcmp(pMyOnlineWorld->pOnlineVillage->GetMaxInvestmentID(), pMyOnlineWorld->pMyData->GetMyCharName()) == 0))
		{
			// ���� ���� ����� ������ ���
			if (((char*)pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID)
				&& (pMyOnlineWorld->pMyData->GetGuildName())
				&& (strcmp((char*)pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID, pMyOnlineWorld->pMyData->GetGuildName()) == 0))
			{
				// ���� ���� ��ư Ȱ��ȭ
				m_VillageData.m_btnRecruitArmy.SetAction(TRUE);

				/*
				// ����ǰ ���� ��ư Ȱ��ȭ
				m_VillageData.m_btnProduceSupplyGoods.SetAction(TRUE);
				*/
			}
		}

	}
	else	// ����� ���� �����̸�
	{
		// ����� ��� ������ ����� ���
		if (siMyGuildClass == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
		{
			// ���� ���� ���� ��ư Ȱ��ȭ
			m_VillageData.m_btnDeclareOccupation.SetAction(TRUE);
		}
	}

	// ���Ŀ� ���� ������ �߰��� �����̴�. 2004/11/18 ����
	// ���� ���� ���� ��ư Ȱ��ȭ
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

VOID OnlineGovernMgr::InitVillageInfo_RecruitArmy()		// ���¸��� �������̽� �ʱ�ȭ
{
	ZeroMemory(m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount, sizeof(SI32) * ON_MAX_GUILD_UNIT);

	m_VillageData.m_siSupplyGoodsAmount = 0;

	///////////////////////////////////////
	// �̹��� �ε�
	// ���� ��ư
	clGrp.LoadXspr("Online\\GameSpr\\Village\\town_button_large.spr", m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit);
	// ��� �̹���
	clGrp.LoadXspr("Online\\GameSpr\\Village\\TOWN_OFFICE_RECRUIT.Spr", m_VillageData.m_sRECRUIT_Army_InGovern.m_MainImage);

	///////////////////////////////////////
	// ���¸������� ����ϴ� ��ư ����
	// �˺� ����
	m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Swordman.Create(m_siX+316, m_siY+139 + (28 * 0),
					m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Xsize, m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Ysize,
					pMyOnlineWorld->pOnlineText->Get(ON_TEXT_RECRUIT_ARMY_SWORDMAN), BUTTON_PUT_LEFT, TRUE);
	// â�� ����
	m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Spearman.Create(m_siX+316, m_siY+139 + (28 * 1),
					m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Xsize, m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Ysize,
					pMyOnlineWorld->pOnlineText->Get(ON_TEXT_RECRUIT_ARMY_SPEARMAN), BUTTON_PUT_LEFT, TRUE);
	// �ú� ����
	m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Archerman.Create(m_siX+316, m_siY+139 + (28 * 2),
					m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Xsize, m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Ysize,
					pMyOnlineWorld->pOnlineText->Get(ON_TEXT_RECRUIT_ARMY_ARCHER), BUTTON_PUT_LEFT, TRUE);
	// �⸶�� ����
	m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Horseman.Create(m_siX+316, m_siY+139 + (28 * 3),
					m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Xsize, m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Ysize,
					pMyOnlineWorld->pOnlineText->Get(ON_TEXT_RECRUIT_ARMY_HORSEMAN), BUTTON_PUT_LEFT, TRUE);
	// ���� ����
	m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Siege_tower.Create(m_siX+316, m_siY+139 + (28 * 4),
					m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Xsize, m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Ysize,
					pMyOnlineWorld->pOnlineText->Get(ON_TEXT_RECRUIT_ARMY_SIEGETOWER), BUTTON_PUT_LEFT, TRUE);
	// �߼��� ����
	m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Catapult.Create(m_siX+316, m_siY+139 + (28 * 5),
					m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Xsize, m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Ysize,
					pMyOnlineWorld->pOnlineText->Get(ON_TEXT_RECRUIT_ARMY_CATAPULT), BUTTON_PUT_LEFT, TRUE);
	// ���޼��� ����
	m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Supply_wagon.Create(m_siX+316, m_siY+139 + (28 * 6),
					m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Xsize, m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Ysize,
					pMyOnlineWorld->pOnlineText->Get(ON_TEXT_RECRUIT_ARMY_SUPPLYGOODS_WAGON), BUTTON_PUT_LEFT, TRUE);
	/*
	// ���� ����
	m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Garrison.Create(m_siX+316, m_siY+139 + (28 * 7),
					m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Xsize, m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Header.Ysize,
					pMyOnlineWorld->pOnlineText->Get(ON_TEXT_RECRUIT_ARMY_GARRISON), BUTTON_PUT_LEFT, TRUE);
	*/

	// �����޴��� ���ư��� ��ư
	m_VillageData.m_btnBack.Create(m_siX+227, m_siY+364,
					m_VillageData.m_ButImage.Header.Xsize, m_VillageData.m_ButImage.Header.Ysize,
					pMyOnlineWorld->pOnlineText->Get(1000114), BUTTON_PUT_LEFT, TRUE);
}

// robypark 2004/12/27 15:42
// ��û ���� �� - ���� �������� ���� ��ư Ȱ��ȭ ���� �����ϱ�
void	OnlineGovernMgr::UpdateVillageInfo_BtnRecruitArmy(void)
{
	// ������ �غ� ���Ͽ� ���, �����, ����� ��� �Ҽ� �������� ���¸��� ���� �ð��̸�
	if(FALSE == pMyOnlineWorld->IsInTimeDisabledRecruitArmyInGovernment())
	{
		// �˺� ����
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Swordman.SetAction(TRUE);
		// â�� ����
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Spearman.SetAction(TRUE);
		// �ú� ����
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Archerman.SetAction(TRUE);
		// �⸶�� ����
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Horseman.SetAction(TRUE);
		// ���� ����
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Siege_tower.SetAction(TRUE);
		// �߼��� ����
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Catapult.SetAction(TRUE);
		// ���޼��� ����
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Supply_wagon.SetAction(TRUE);
		/*
		// ���� ����
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Garrison.SetAction(TRUE);
		*/
	}
	else
	{
		// �˺� ����
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Swordman.SetAction(FALSE);
		// â�� ����
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Spearman.SetAction(FALSE);
		// �ú� ����
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Archerman.SetAction(FALSE);
		// �⸶�� ����
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Horseman.SetAction(FALSE);
		// ���� ����
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Siege_tower.SetAction(FALSE);
		// �߼��� ����
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Catapult.SetAction(FALSE);
		// ���޼��� ����
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Supply_wagon.SetAction(FALSE);
		/*
		// ���� ����
		m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Garrison.SetAction(FALSE);
		*/
	}
}

VOID OnlineGovernMgr::FreeVillageInfo_RecruitArmy()		// ���¸��� �������̽� �Ҵ� �޸� �ʱ�ȭ
{
	m_VillageData.m_siSupplyGoodsAmount = 0;

	///////////////////////////////////////
	// �̹��� ����
	// ���� ��ư
	clGrp.FreeXspr(m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit);
	// ��� �̹���
	clGrp.FreeXspr(m_VillageData.m_sRECRUIT_Army_InGovern.m_MainImage);
}

VOID OnlineGovernMgr::InitVillageInfo_ProduceSupplyGoods()	// ����ǰ ���� �������̽� �ʱ�ȭ
{
	///////////////////////////////////////
	// �̹��� �ε�
	// ��� �̹���
	clGrp.LoadXspr("Online\\GameSpr\\Village\\TOWN_OFFICE_SUPPLY.Spr", m_VillageData.m_sProduceSupplyGoods.m_MainImage);
	// ���� ��ư
	clGrp.LoadXspr("Online\\GameSpr\\Book\\BOOK_BUTTON_MINI.Spr", m_VillageData.m_sProduceSupplyGoods.m_sprMiniButton);

	// ����ǰ ���� ��ư
	m_VillageData.m_sProduceSupplyGoods.m_btnProduceSupplyGoods.Create(m_siX+413, m_siY+123,
					m_VillageData.m_sProduceSupplyGoods.m_sprMiniButton.Header.Xsize, m_VillageData.m_sProduceSupplyGoods.m_sprMiniButton.Header.Ysize,
					pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_PRODUCTION), BUTTON_PUT_LEFT, FALSE);

	// �����޴��� ���ư��� ��ư
	m_VillageData.m_btnBack.Create(m_siX+227, m_siY+364,
					m_VillageData.m_ButImage.Header.Xsize, m_VillageData.m_ButImage.Header.Ysize,
					pMyOnlineWorld->pOnlineText->Get(1000114), BUTTON_PUT_LEFT, TRUE);

	// ����ǰ �ϳ��� ���� ��� ����
//	m_VillageData.m_sProduceSupplyGoods.m_siCostSupplyGoods = pMyOnlineWorld->GetSOptionSiegeWarfare()->siCostSupplyGoods;
}

VOID OnlineGovernMgr::FreeVillageInfo_ProduceSupplyGoods()	// ����ǰ ���� �������̽� �Ҵ� �޸� �ʱ�ȭ
{
	///////////////////////////////////////
	// �̹��� ����
	// ��� �̹���
	clGrp.FreeXspr(m_VillageData.m_sProduceSupplyGoods.m_MainImage);
	// ���� ��ư
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
			// ���θ޴� ��ȯ
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
			// �ȳ����� Ŭ���Ͽ��� ���
			if( m_bMouseDown )
			{
				// ���� �����̰ų� ��ڸ��� �ȳ����� �� �� �ִ�.
				if( ( strcmp( pMyOnlineWorld->pOnlineVillage->GetMaxInvestmentID(), pMyOnlineWorld->pMyData->GetMyCharName() ) == 0 ) || 
					pMyOnlineWorld->m_GMFlag == ON_USERGRADE_ADMIN5 )
				{
					if( pOecEx->IsPointInRect( ptMouse ) )	Handler_OnlineEditControlEx::SetFocus( m_VillageData.m_hOecNotice );
					else									Handler_OnlineEditControlEx::ClearCurrentFocus();
				}
			}

			// �ȳ����� ��Ŀ���� �� ���� ���
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
			// ������ ��� ��ư ó�� robypark 2004/11/15 14:31
			else if (m_VillageData.m_btnToDefenceMenu.Process(m_bMouseDown))	// �����ϱ� ��ư
			{
				m_VillageData.m_uiStatus = ON_SUB_VILLAGEINFO_TO_DEFENCE_MENU;
			}
			*/
			else if (m_VillageData.m_btnRecruitArmy.Process(m_bMouseDown))		// ���� ���� ��ư
			{
				m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_INIT;
				m_VillageData.m_uiStatus = ON_SUB_VILLAGEINFO_RECRUIT_ARMY;
			}
			/*
			else if (m_VillageData.m_btnProduceSupplyGoods.Process(m_bMouseDown))	// ����ǰ ���� ��ư
			{
				m_VillageData.m_uiStatus = ON_SUB_VILLAGEINFO_PRODUCE_SUPPLYGOODS;
			}
			else if (m_VillageData.m_btnDeclareOccupation.Process(m_bMouseDown))	// ���ɼ��� ��ư
			{
				m_VillageData.m_uiStatus = ON_SUB_VILLAGEINFO_DECLARE_OCCUPATION;
			}
			*/

			// ���� ���� ���� ����
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
			// �����ȳ���, ������� ������(GovernVillageInfo, GovernDescript, GovernVillageDefenceInfo, GovernVillageDefenceSet)
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

	case ON_SUB_VILLAGEINFO_TO_DEFENCE_MENU:		// �����ϱ�
		{
			m_VillageData.m_uiStatus = ActionToDefenceMenu();
		}
		break;

	case ON_SUB_VILLAGEINFO_RECRUIT_ARMY:			// ���� ����
		{
			m_VillageData.m_uiStatus = ActionRecruitArmy();
		}
		break;

	case ON_SUB_VILLAGEINFO_PRODUCE_SUPPLYGOODS:	// ����ǰ ����
		{
			m_VillageData.m_uiStatus = ActionProduceSupplyGoods();
		}
		break;

	case ON_SUB_VILLAGEINFO_DECLARE_OCCUPATION:		// ���� ����
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
			case ON_SUB_VILLAGEINFO_TO_DEFENCE_MENU:		// �����ϱ�
				break;
			case ON_SUB_VILLAGEINFO_RECRUIT_ARMY:			// ���� ����
				if (NULL != m_VillageData.m_sRECRUIT_Army_InGovern.m_MainImage.Image)
				{
					// ��û ���� ���� �̹��� �׸���
					clGrp.PutSpriteT(m_siX, m_siY,
						m_VillageData.m_sRECRUIT_Army_InGovern.m_MainImage.Header.Xsize, 
						m_VillageData.m_sRECRUIT_Army_InGovern.m_MainImage.Header.Ysize, 
						m_VillageData.m_sRECRUIT_Army_InGovern.m_MainImage.Image);
				}

				// ��ư �̹��� �׸���
				if (NULL != m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit.Image)
				{
					// �˺� ����
					m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Swordman.Put(&m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit, 0, 2, 1, BUTTON_PUT_NOMOVE);
					// â�� ����
					m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Spearman.Put(&m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit, 0, 2, 1, BUTTON_PUT_NOMOVE);
					// �ú� ����
					m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Archerman.Put(&m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit, 0, 2, 1, BUTTON_PUT_NOMOVE);
					// �⸶�� ����
					m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Horseman.Put(&m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit, 0, 2, 1, BUTTON_PUT_NOMOVE);
					// ���� ����
					m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Siege_tower.Put(&m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit, 0, 2, 1, BUTTON_PUT_NOMOVE);
					// �߼��� ����
					m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Catapult.Put(&m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit, 0, 2, 1, BUTTON_PUT_NOMOVE);
					// ���޼��� ����
					m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Supply_wagon.Put(&m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit, 0, 2, 1, BUTTON_PUT_NOMOVE);
					/*
					// ����(�ֵб�) ����
					m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Garrison.Put(&m_VillageData.m_sRECRUIT_Army_InGovern.m_sprBtnRecruit, 0, 2, 1, BUTTON_PUT_NOMOVE);
					*/
				}
				
				// ���ư��� ��ư
				m_VillageData.m_btnBack.Put(&m_VillageData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);

				break;

			case ON_SUB_VILLAGEINFO_PRODUCE_SUPPLYGOODS:	// ����ǰ ����
				// ��û ���� ���� �̹��� �׸���
				clGrp.PutSpriteT(m_siX, m_siY,
					m_VillageData.m_sProduceSupplyGoods.m_MainImage.Header.Xsize, 
					m_VillageData.m_sProduceSupplyGoods.m_MainImage.Header.Ysize,
					m_VillageData.m_sProduceSupplyGoods.m_MainImage.Image);

				// ��ư �̹��� �׸���
				// ���� ��ư
				m_VillageData.m_sProduceSupplyGoods.m_btnProduceSupplyGoods.Put(&m_VillageData.m_sProduceSupplyGoods.m_sprMiniButton, 0, 2, 1, BUTTON_PUT_NOMOVE);
				// ���ư��� ��ư
				m_VillageData.m_btnBack.Put(&m_VillageData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
				break;

			case ON_SUB_VILLAGEINFO_DECLARE_OCCUPATION:		// ���� ����
				break;
			default:
				// ��û ���� ���� �̹��� �׸���
				clGrp.PutSpriteT(m_siX, m_siY, 
					m_VillageData.m_MainImage.Header.Xsize,
					m_VillageData.m_MainImage.Header.Ysize,
					m_VillageData.m_MainImage.Image);

				// robypark 2004/11/15 13:58
				// �������� ���� ��ư �̹��� �׸���
				// ���� ���� ��ư
				m_VillageData.m_btnRecruitArmy.Put(&m_VillageData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
				/*
				// �����ϱ� ��ư
				m_VillageData.m_btnToDefenceMenu.Put(&m_VillageData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
				// ���� ���� ��ư
				m_VillageData.m_btnRecruitArmy.Put(&m_VillageData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
				// ����ǰ ���� ��ư
				m_VillageData.m_btnProduceSupplyGoods.Put(&m_VillageData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
				// ���ɼ���
				m_VillageData.m_btnDeclareOccupation.Put(&m_VillageData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
				*/

				// ��û ���� ��ư �׸���
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
			case ON_SUB_VILLAGEINFO_TO_DEFENCE_MENU:		// �����ϱ�
				break;
			case ON_SUB_VILLAGEINFO_RECRUIT_ARMY:			// ���� ����
				// ���� �ؽ�Ʈ �׸���
				// ĳ���� ������(�ں���) ���
				MakeMoneyText(pMyOnlineWorld->pMyData->GetMoney(), szTemp);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 16, m_siY + 365, 52, 15, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CAPITAL_FUND), RGB(216, 201, 183));
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 74, m_siY + 364, 94, 17, szTemp, RGB(250, 250, 250));

				// ���� ��� ���
				// �ʵ� ���� ������ ������ ���� ����, ���� �ֵб� ������ �� �������� ���� �������̽� �۵� �߿���
//				if ((ON_SUB_VILLAGEINFO_SUB_RECRUIT_FIELD_ARMY == m_VillageData.m_uiSubStatus) 
//					|| (ON_SUB_VILLAGEINFO_SUB_RECRUIT_GARRISON_ARMY == m_VillageData.m_uiSubStatus))
				if (pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsShow())
				{
					// ���� ���� �ؽ�Ʈ �ε��� ���
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
					// xxx nn �δ� �������: aaa ��, bbb ��
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
				// ��� ���� ����ǰ ��
				MakeMoneyText(m_VillageData.m_siSupplyGoodsAmount, szTemp);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 365, m_siY + 365, 52, 15, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_COUNT_SUPPLYGOODS), RGB(216, 201, 183));
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 423, m_siY + 364, 94, 17, szTemp, RGB(250, 250, 250));
				*/

				// ���� ������ �α��� ���
				MakeMoneyText(m_VillageData.m_sRECRUIT_Army_InGovern.m_siRecruitablePopulation, szTemp);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 133, m_siY + 112 + (28 * 0), 82, 15, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_RECRUITABLED_POPULATION), RGB(216, 201, 183));
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 221, m_siY + 111 + (28 * 0), 82, 15, szTemp, RGB(250, 250, 250));

				// �˺�
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 133, m_siY + 112 + (28 * 1), 82, 15, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_COUNT_SWORDMAN_ARMY), RGB(216, 201, 183));
				sprintf(szTemp, "%d", m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SWORDMAN]);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 221, m_siY + 111 + (28 * 1), 84, 17, szTemp, RGB(250, 250, 250));
				// â��
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 133, m_siY + 112 + (28 * 2), 82, 15, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_COUNT_SPEARMAN_ARMY), RGB(216, 201, 183));
				sprintf(szTemp, "%d", m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SPEARMAN]);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 221, m_siY + 111 + (28 * 2), 84, 17, szTemp, RGB(250, 250, 250));
				// �ú�
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 133, m_siY + 112 + (28 * 3), 82, 15, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_COUNT_ARCHER_ARMY), RGB(216, 201, 183));
				sprintf(szTemp, "%d", m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_ARCHERMAN]);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 221, m_siY + 111 + (28 * 3), 84, 17, szTemp, RGB(250, 250, 250));
				// �⸶��
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 133, m_siY + 112 + (28 * 4), 82, 15, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_COUNT_HORSEMAN_ARMY), RGB(216, 201, 183));
				sprintf(szTemp, "%d", m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_HORSEMAN]);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 221, m_siY + 111 + (28 * 4), 84, 17, szTemp, RGB(250, 250, 250));
				// ����
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 133, m_siY + 112 + (28 * 5), 82, 15, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_COUNT_SIEGETOWER), RGB(216, 201, 183));
				sprintf(szTemp, "%d", m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SIEGETOWER]);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 221, m_siY + 111 + (28 * 5), 84, 17, szTemp, RGB(250, 250, 250));
				// �߼���
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 133, m_siY + 112 + (28 * 6), 82, 15, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_COUNT_CATAPULT), RGB(216, 201, 183));
				sprintf(szTemp, "%d", m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_CATAPULT]);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 221, m_siY + 111 + (28 * 6), 84, 17, szTemp, RGB(250, 250, 250));
				// ���޼���
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 133, m_siY + 112 + (28 * 7), 82, 15, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_COUNT_SUPPLYGOODS_WAGON), RGB(216, 201, 183));
				sprintf(szTemp, "%d", m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SUPPLYWAGON]);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 221, m_siY + 111 + (28 * 7), 84, 17, szTemp, RGB(250, 250, 250));
				/*
				// �ֵб�(����)
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 133, m_siY + 112 + (28 * 8), 82, 15, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_COUNT_GARRISON), RGB(216, 201, 183));
				sprintf(szTemp, "%d", m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_DEFENCESOLIDER]);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 221, m_siY + 111 + (28 * 8), 84, 17, szTemp, RGB(250, 250, 250));
				*/

				// ��ư �ؽ�Ʈ �׸���
				// �˺� ����
				m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Swordman.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				// â�� ����
				m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Spearman.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				// �ú� ����
				m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Archerman.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				// �⸶�� ����
				m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Horseman.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				// ���� ����
				m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Siege_tower.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				// �߼��� ����
				m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Catapult.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				// ���޼��� ����
				m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Supply_wagon.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				/*
				// ����(�ֵб�) ����
				m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Garrison.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				*/

				// ���ư��� ��ư
				m_VillageData.m_btnBack.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				break;

			case ON_SUB_VILLAGEINFO_PRODUCE_SUPPLYGOODS:	// ����ǰ ����
				// ���� �ؽ�Ʈ �׸���
				// ĳ���� ������(�ں���) ���
				MakeMoneyText(pMyOnlineWorld->pMyData->GetMoney(), szTemp);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 16, m_siY + 365, 52, 15, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CAPITAL_FUND), RGB(216, 201, 183));
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 74, m_siY + 364, 94, 17, szTemp, RGB(250, 250, 250));

				// ����ǰ ���� ��� ���
				// ����ǰ ���� ���� �������̽� �۵� �߿���
//				if (ON_SUB_VILLAGEINFO_SUB_PRODUCE_SUPPLYGOODS == m_VillageData.m_uiSubStatus) 
				if (pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsShow())
				{
					// ���꿡 �ʿ��� �ڱ� ���
					pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 132, m_siY + 76 + (22 * 2), 120, 17, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CAPITAL_FUND_TO_PROCUDE_SUPPLYGOODS), RGB(250, 250, 250));

					MakeMoneyText(m_VillageData.m_sProduceSupplyGoods.m_siCostSupplyGoods * pMyOnlineWorld->pOnlineNumPad->pNumberControl->GetNumber(), szTemp);
					sprintf(szTemp2, "%s %s", szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WON));
					pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 132 + 120, m_siY + 76 + (22 * 2), 151, 17, szTemp2, RGB(250, 250, 250));
				}
				else
				{
					// ���꿡 �ʿ��� �ڱ� ���
					pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 132, m_siY + 76 + (22 * 2), 120, 17, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CAPITAL_FUND_TO_PROCUDE_SUPPLYGOODS), RGB(250, 250, 250));

					MakeMoneyText(m_VillageData.m_sProduceSupplyGoods.m_siCostSupplyGoods * pMyOnlineWorld->pOnlineNumPad->pNumberControl->GetNumber(), szTemp);
					sprintf(szTemp2, "%s %s", "0", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WON));
					pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 132 + 120, m_siY + 76 + (22 * 2), 151, 17, szTemp2, RGB(250, 250, 250));
				}

				// ��� ���� ����ǰ ��
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 132, m_siY + 76 + (22 * 0), 120, 17, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CURRENT_SUPPLYGOODS_COUNT), RGB(250, 250, 250));

				MakeMoneyText(m_VillageData.m_siSupplyGoodsAmount, szTemp);
				sprintf(szTemp2, "%s %s", szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_UNIT));
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 132 + 120, m_siY + 76 + (22 * 0), 151, 17, szTemp2, RGB(250, 250, 250));

				// ����ǰ 1���� ���� ��� ���
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 132, m_siY + 76 + (22 * 1), 120, 17, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_PRICE_OF_A_SUPPLYGOODS), RGB(250, 250, 250));

				MakeMoneyText(m_VillageData.m_sProduceSupplyGoods.m_siCostSupplyGoods, szTemp);
				sprintf(szTemp2, "%s %s", szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WON));
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 132 + 120, m_siY + 76 + (22 * 1), 151, 17, szTemp2, RGB(250, 250, 250));

				// ����ǰ ���� ��ư �ؽ�Ʈ ���
				m_VillageData.m_sProduceSupplyGoods.m_btnProduceSupplyGoods.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

				// ���ư��� ��ư
				m_VillageData.m_btnBack.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				break;

			case ON_SUB_VILLAGEINFO_DECLARE_OCCUPATION:		// ���� ����
				break;
			// ��û ����
			default:
				m_VillageData.m_BDescOk.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				m_VillageData.m_BDefence.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

				// robypark 2004/11/15 13:58
				// �������� ���� ��ư �ؽ�Ʈ �׸���
				// ���� ���� ��ư
				m_VillageData.m_btnRecruitArmy.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

				/*
				// �����ϱ� ��ư
				m_VillageData.m_btnToDefenceMenu.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				// ���� ���� ��ư
				m_VillageData.m_btnRecruitArmy.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				// ����ǰ ���� ��ư
				m_VillageData.m_btnProduceSupplyGoods.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				// ���ɼ���
				m_VillageData.m_btnDeclareOccupation.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				*/

				// �α�
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 112, m_siY + 68, 82, 13, pMyOnlineWorld->pOnlineText->Get(8000489), RGB(200, 200, 200));
				sprintf(szTemp, "%d", m_VillageData.m_uiPopulation);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 199, m_siY + 66, 163, 17, szTemp, RGB(200, 200, 200));

				// ���� ������ ��
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 112, m_siY + 99, 82, 13, pMyOnlineWorld->pOnlineText->Get(8000490), RGB(200, 200, 200));
				sprintf(szTemp, "%d", m_VillageData.m_siInOutPlayer);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 199, m_siY + 99, 163, 13, szTemp, RGB(200, 200, 200));

				// ����
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 112, m_siY + 131, 82, 13, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_AP), RGB(200, 200, 200));
				sprintf(szTemp, "%I64d / %I64d", m_VillageData.m_CurDefPower, m_VillageData.m_MaxDefPower);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 199, m_siY + 131, 163, 13, szTemp, RGB(200, 200, 200));

				// ���νü�
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 384, m_siY + 68, 82, 13, pMyOnlineWorld->pOnlineText->Get(8000491), RGB(200, 200, 200));
		//		pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hdc, m_siX + 378, m_siY + 90, 125, m_VillageData.m_szStruct, RGB(200, 200, 200));
				pMyOnlineWorld->pOnlineMegaText->DrawTextInBox( hdc, m_siX + 380, m_siY + 90, 130, 60, NULL, m_VillageData.m_szStruct, RGB(200, 200, 200), DT_CENTER );

				// �����ȳ���
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

UI08	OnlineGovernMgr::ActionToDefenceMenu(void)			// �����ϱ�
{
	/*
	switch (m_VillageData.m_uiSubStatus)
	{
		break;
	}
	*/

	return ON_SUB_VILLAGEINFO_POLL;
}

UI08	OnlineGovernMgr::ActionRecruitArmy(void)			// ���¸���
{
	switch (m_VillageData.m_uiSubStatus)
	{
		case ON_SUB_VILLAGEINFO_SUB_INIT:
			{
				// ���� ����� ������ ���� ���� �δ� �� �� ���������� �α����� ���´�.
				m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedUnitCount = FALSE;
				pMyOnlineWorld->pOnlineClient->SendGetGuildUnitList(0);
				
				// ���� ����� ������ �ִ� ����ǰ ���� ���´�.
				m_VillageData.m_siSupplyGoodsAmount = 0;
				/*
				pMyOnlineWorld->pOnlineClient->SendGetGuildSupplyList(0);
				
				// ���� ���� ���� �δ� ���� ���´�.
				m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedDefenceSoldier = FALSE;
				pMyOnlineWorld->pOnlineClient->SendVillageGarrisonCount(pVillageHeader->uiVillageCode);
				*/

				m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_POLL;
			
				// ��ư ��Ȱ��ȭ
				// �����ϱ� ��ư
				m_VillageData.m_btnToDefenceMenu.SetAction(FALSE);
				// ���� ���� ��ư
				m_VillageData.m_btnRecruitArmy.SetAction(FALSE);
				// ����ǰ ���� ��ư
				m_VillageData.m_btnProduceSupplyGoods.SetAction(FALSE);
				// ���ɼ���
				m_VillageData.m_btnDeclareOccupation.SetAction(FALSE);

				// ���¸��� �������̽� ���ҽ� �ʱ�ȭ
				InitVillageInfo_RecruitArmy();

				// ������ ��ư ��Ȱ��ȭ
				m_BExit.SetAction(FALSE);
			}
			break;

		case ON_SUB_VILLAGEINFO_SUB_POLL:
			{
				// ������ �غ� ���Ͽ� ���, �����, ����� ��� �Ҽ� �������� ���¸��� ���� �ð��� ���� ��ư Ȱ��ȭ, ��Ȱ��ȭ
				UpdateVillageInfo_BtnRecruitArmy();

				// ���� �˺� ���� ��ư
				if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Swordman.Process(m_bMouseDown))
				{
					if (m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedUnitCount)
					{
						// ���� ���� ���� ��� ���
						LPSSiegeUnitTable lpsSiegeUnitTable = pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(ARRAY_INDEX_SIEGEWARFAREUNIT_SWORDMAN);

						if (lpsSiegeUnitTable)
						{
							// �����Ϸ��� ���� ������ ������ ���� �ؽ�Ʈ ������ �ε���
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyKind = lpsSiegeUnitTable->m_siID;

							// �� ����� �δ� ��� ���
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitCost = lpsSiegeUnitTable->m_siPrice;
							// �� �δ밡 �Ǳ� ���� �α���
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation = lpsSiegeUnitTable->m_siPopulation;
							// ������� m_siArmyUnitCost * m_siArmyUnitPopulation �̴�.
							// �Ҹ� ����ǰ ��
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitSupplyGoods = 0;

							// �ִ� ����� �� �ִ� �δ�� ���
							SI32 siMaxArmyUintCount = m_VillageData.m_sRECRUIT_Army_InGovern.m_siRecruitablePopulation / m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation;

							// ���� ������ �α����� �Ǵ��� �˻�
							if (siMaxArmyUintCount <= 0)
								break;

							// ���� �������̽� ����
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(siMaxArmyUintCount);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_ARMY);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(m_siX + 417, m_siY + 144);

							m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_RECRUIT_FIELD_ARMY;
						}
					}
				}
				// ���� â�� ���� ��ư
				else if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Spearman.Process(m_bMouseDown))
				{
					if (m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedUnitCount)
					{
						// ���� ���� ���� ��� ���
						LPSSiegeUnitTable lpsSiegeUnitTable = pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(ARRAY_INDEX_SIEGEWARFAREUNIT_SPEARMAN);

						if (lpsSiegeUnitTable)
						{
							// �����Ϸ��� ���� ������ ������ ���� �ؽ�Ʈ ������ �ε���
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyKind = lpsSiegeUnitTable->m_siID;

							// �� ����� �δ� ��� ���
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitCost = lpsSiegeUnitTable->m_siPrice;
							// �� �δ밡 �Ǳ� ���� �α���
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation = lpsSiegeUnitTable->m_siPopulation;
							// ������� m_siArmyUnitCost * m_siArmyUnitPopulation �̴�.
							// �Ҹ� ����ǰ ��
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitSupplyGoods = 0;

							// �ִ� ����� �� �ִ� �δ�� ���
							SI32 siMaxArmyUintCount = m_VillageData.m_sRECRUIT_Army_InGovern.m_siRecruitablePopulation / m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation;

							// ���� ������ �α����� �Ǵ��� �˻�
							if (siMaxArmyUintCount <= 0)
								break;

							// ���� �������̽� ����
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(siMaxArmyUintCount);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_ARMY);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(m_siX + 417, m_siY + 144);

							m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_RECRUIT_FIELD_ARMY;
						}
					}
				}
				// ���� �ú� ���� ��ư
				else if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Archerman.Process(m_bMouseDown))
				{
					if (m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedUnitCount)
					{
						// ���� ���� ���� ��� ���
						LPSSiegeUnitTable lpsSiegeUnitTable = pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(ARRAY_INDEX_SIEGEWARFAREUNIT_ARCHERMAN);

						if (lpsSiegeUnitTable)
						{
							// �����Ϸ��� ���� ������ ������ ���� �ؽ�Ʈ ������ �ε���
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyKind = lpsSiegeUnitTable->m_siID;

							// �� ����� �δ� ��� ���
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitCost = lpsSiegeUnitTable->m_siPrice;
							// �� �δ밡 �Ǳ� ���� �α���
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation = lpsSiegeUnitTable->m_siPopulation;
							// ������� m_siArmyUnitCost * m_siArmyUnitPopulation �̴�.
							// �Ҹ� ����ǰ ��
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitSupplyGoods = 0;

							// �ִ� ����� �� �ִ� �δ�� ���
							SI32 siMaxArmyUintCount = m_VillageData.m_sRECRUIT_Army_InGovern.m_siRecruitablePopulation / m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation;

							// ���� ������ �α����� �Ǵ��� �˻�
							if (siMaxArmyUintCount <= 0)
								break;

							// ���� �������̽� ����
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(siMaxArmyUintCount);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_ARMY);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(m_siX + 417, m_siY + 144);

							m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_RECRUIT_FIELD_ARMY;
						}
					}
				}
				// ���� �⸶�� ���� ��ư
				else if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Horseman.Process(m_bMouseDown))
				{
					if (m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedUnitCount)
					{
						// ���� ���� ���� ��� ���
						LPSSiegeUnitTable lpsSiegeUnitTable = pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(ARRAY_INDEX_SIEGEWARFAREUNIT_HORSEMAN);

						if (lpsSiegeUnitTable)
						{
							// �����Ϸ��� ���� ������ ������ ���� �ؽ�Ʈ ������ �ε���
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyKind = lpsSiegeUnitTable->m_siID;

							// �� ����� �δ� ��� ���
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitCost = lpsSiegeUnitTable->m_siPrice;
							// �� �δ밡 �Ǳ� ���� �α���
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation = lpsSiegeUnitTable->m_siPopulation;
							// ������� m_siArmyUnitCost * m_siArmyUnitPopulation �̴�.
							// �Ҹ� ����ǰ ��
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitSupplyGoods = 0;

							// �ִ� ����� �� �ִ� �δ�� ���
							SI32 siMaxArmyUintCount = m_VillageData.m_sRECRUIT_Army_InGovern.m_siRecruitablePopulation / m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation;

							// ���� ������ �α����� �Ǵ��� �˻�
							if (siMaxArmyUintCount <= 0)
								break;

							// ���� �������̽� ����
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(siMaxArmyUintCount);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_ARMY);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(m_siX + 417, m_siY + 144);

							m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_RECRUIT_FIELD_ARMY;
						}
					}
				}
				// ���� ���� ���� ��ư
				else if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Siege_tower.Process(m_bMouseDown))
				{
					if (m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedUnitCount)
					{
						// ���� ���� ���� ��� ���
						LPSSiegeUnitTable lpsSiegeUnitTable = pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(ARRAY_INDEX_SIEGEWARFAREUNIT_SIEGETOWER);

						if (lpsSiegeUnitTable)
						{
							// �����Ϸ��� ���� ������ ������ ���� �ؽ�Ʈ ������ �ε���
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyKind = lpsSiegeUnitTable->m_siID;

							// �� ����� �δ� ��� ���
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitCost = lpsSiegeUnitTable->m_siPrice;
							// �� �δ밡 �Ǳ� ���� �α���
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation = lpsSiegeUnitTable->m_siPopulation;
							// ������� m_siArmyUnitCost * m_siArmyUnitPopulation �̴�.
							// �Ҹ� ����ǰ ��
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitSupplyGoods = 0;

							// �ִ� ����� �� �ִ� �δ�� ���
							SI32 siMaxArmyUintCount = m_VillageData.m_sRECRUIT_Army_InGovern.m_siRecruitablePopulation / m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation;

							// ���� ������ �α����� �Ǵ��� �˻�
							if (siMaxArmyUintCount <= 0)
								break;

							// ���� �������̽� ����
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(siMaxArmyUintCount);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_ARMY);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(m_siX + 417, m_siY + 144);

							m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_RECRUIT_FIELD_ARMY;
						}
					}
				}
				// ���� �߼��� ���� ��ư
				else if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Catapult.Process(m_bMouseDown))
				{
					if (m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedUnitCount)
					{
						// ���� ���� ���� ��� ���
						LPSSiegeUnitTable lpsSiegeUnitTable = pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(ARRAY_INDEX_SIEGEWARFAREUNIT_CATAPULT);

						if (lpsSiegeUnitTable)
						{
							// �����Ϸ��� ���� ������ ������ ���� �ؽ�Ʈ ������ �ε���
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyKind = lpsSiegeUnitTable->m_siID;

							// �� ����� �δ� ��� ���
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitCost = lpsSiegeUnitTable->m_siPrice;
							// �� �δ밡 �Ǳ� ���� �α���
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation = lpsSiegeUnitTable->m_siPopulation;
							// ������� m_siArmyUnitCost * m_siArmyUnitPopulation �̴�.
							// �Ҹ� ����ǰ ��
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitSupplyGoods = 0;

							// �ִ� ����� �� �ִ� �δ�� ���
							SI32 siMaxArmyUintCount = m_VillageData.m_sRECRUIT_Army_InGovern.m_siRecruitablePopulation / m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation;

							// ���� ������ �α����� �Ǵ��� �˻�
							if (siMaxArmyUintCount <= 0)
								break;

							// ���� �������̽� ����
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(siMaxArmyUintCount);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_ARMY);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(m_siX + 417, m_siY + 144);

							m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_RECRUIT_FIELD_ARMY;
						}
					}
				}
				// ���� ���� ���� ���� ��ư
				else if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Supply_wagon.Process(m_bMouseDown))
				{
					if (m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedUnitCount)
					{
						// ���� ���� ���� ��� ���
						LPSSiegeUnitTable lpsSiegeUnitTable = pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(ARRAY_INDEX_SIEGEWARFAREUNIT_SUPPLYWAGON);

						if (lpsSiegeUnitTable)
						{
							// �����Ϸ��� ���� ������ ������ ���� �ؽ�Ʈ ������ �ε���
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyKind = lpsSiegeUnitTable->m_siID;

							// �� ����� �δ� ��� ���
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitCost = lpsSiegeUnitTable->m_siPrice;
							// �� �δ밡 �Ǳ� ���� �α���
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation = lpsSiegeUnitTable->m_siPopulation;
							// ������� m_siArmyUnitCost * m_siArmyUnitPopulation �̴�.
							// �Ҹ� ����ǰ ��
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitSupplyGoods = 0;

							// �ִ� ����� �� �ִ� �δ�� ���
							SI32 siMaxArmyUintCount = m_VillageData.m_sRECRUIT_Army_InGovern.m_siRecruitablePopulation / m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation;

							// ���� ������ �α����� �Ǵ��� �˻�
							if (siMaxArmyUintCount <= 0)
								break;

							// ���� �������̽� ����
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(siMaxArmyUintCount);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_ARMY);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(m_siX + 417, m_siY + 144);

							m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_RECRUIT_FIELD_ARMY;
						}
					}
				}
				/*
				// ���� ����(�ֵб�) ���� ��ư
				else if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Garrison.Process(m_bMouseDown))
				{
					if (m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedDefenceSoldier)
					{
						// ������ ������ ������ ���� ��� ���
						CItemHeader *pItemHeader = pMyOnlineWorld->pItemsInfo->GetItemHeaderFromImperium(TransKindImjinToOnline(KIND_ON_SIEGEWARFARE_GARRISON_ARMY, FALSE));

						if (pItemHeader)
						{
							// �����Ϸ��� ���� ������ ������ ���� �ؽ�Ʈ ������ �ε���
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyKind = pItemHeader->m_pImperium->m_siID;

							// �� ����� �δ� ��� ���
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitCost = pItemHeader->m_pImperium->m_siPrice;
							// �� �δ밡 �Ǳ� ���� �α���
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation = pItemHeader->m_pImperium->m_siPopulation;
							// ������� m_siArmyUnitCost * m_siArmyUnitPopulation �̴�.
							// �Ҹ� ����ǰ ��
							m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitSupplyGoods = pItemHeader->m_pImperium->m_siSupply;

							// �ִ� ����� �� �ִ� �δ�� ���
							SI32 siMaxArmyUintCount = m_VillageData.m_sRECRUIT_Army_InGovern.m_siRecruitablePopulation / m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyUnitPopulation;

							// ���� ������ �α����� �Ǵ��� �˻�
							if (siMaxArmyUintCount <= 0)
								break;

							// ���� �������̽� ����
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(siMaxArmyUintCount);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_ARMY);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(m_siX + 417, m_siY + 144);

							m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_RECRUIT_GARRISON_ARMY;
						}
					}
				}
				*/
				// ���� �޴��� ���ư��� ��ư
				else if (m_VillageData.m_btnBack.Process(m_bMouseDown))
				{
					m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_EXIT;
				}

				char szTitle[64], szContent[1024];
				ZeroMemory(szTitle, sizeof(szTitle));
				ZeroMemory(szContent, sizeof(szContent));

				/****************************************************************************************/
				// robypark 2005/1/6 11:57
				// �����Ϸ��� ���� ��ư ǳ�� ���� ó���ϱ�
				// ���� �˺� ���� ��ư
				if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Swordman.IsInMouse())
				{
					strcpy(szTitle, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SWORDMAN));
					strcpy(szContent, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TOOLTIP_RECRUITARMY_SWORDMAN));
				}
				// ���� â�� ���� ��ư
				else if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Spearman.IsInMouse())
				{
					strcpy(szTitle, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SPEARMAN));
					strcpy(szContent, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TOOLTIP_RECRUITARMY_SPEARMAN));
				}
				// ���� �ú� ���� ��ư
				else if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Archerman.IsInMouse())
				{
					strcpy(szTitle, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ARCHER));
					strcpy(szContent, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TOOLTIP_RECRUITARMY_ARCHER));
				}
				// ���� �⸶�� ���� ��ư
				else if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Horseman.IsInMouse())
				{
					strcpy(szTitle, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HORSEMAN));
					strcpy(szContent, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TOOLTIP_RECRUITARMY_HORSEMAN));
				}
				// ���� ���� ���� ��ư
				else if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Siege_tower.IsInMouse())
				{
					strcpy(szTitle, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SIEGETOWER));
					strcpy(szContent, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TOOLTIP_RECRUITARMY_SIEGETOWER));
				}
				// ���� �߼��� ���� ��ư
				else if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Catapult.IsInMouse())
				{
					strcpy(szTitle, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CATAPULT));
					strcpy(szContent, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TOOLTIP_RECRUITARMY_CATAPULT));
				}
				// ���� ���� ���� ���� ��ư
				else if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Supply_wagon.IsInMouse())
				{
					strcpy(szTitle, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SUPPLYGOODS_WAGON));
					strcpy(szContent, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TOOLTIP_RECRUITARMY_SUPPLYGOODSWAGON));
				}
				/*
				// ���� ����(�ֵб�) ���� ��ư
				else if (m_VillageData.m_sRECRUIT_Army_InGovern.m_btnSiegeWarfare_Garrison.IsInMouse())
				{
					strcpy(szTitle, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GARRISON));
					strcpy(szContent, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TOOLTIP_RECRUITARMY_GARRISON));
				}
				*/

				// ����ؾ��� ǳ�� ������ �ִٸ�
				if ('\0' != szTitle[0])
				{
					pMyOnlineWorld->pOnlineHelp->SetTextHasTitle( szTitle, szContent, IpD.Mouse_X, IpD.Mouse_Y, ON_HELP_LEFT | ON_HELP_VCENTER, TRUE, RGB(255, 255, 255), RGB(255, 255, 255) );
				}
				/****************************************************************************************/
			}
			break;
		case ON_SUB_VILLAGEINFO_SUB_WAIT:
			{
				// ������ ��ٸ���.
				pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
			}
			break;
		case ON_SUB_VILLAGEINFO_SUB_EXIT:
			{
				m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_INIT;

				// ��û ���� �ǿ��� ���Ǵ� ��ư �����ϱ�, ���¸���, ����ǰ ����, ���ɼ��� ��ư ���� ����(Ȱ��, ��Ȱ��)
				UpdateVillageInfoButton();

				// ������ ��ư Ȱ��ȭ
				m_BExit.SetAction(TRUE);

				// ���¸��� �������̽� ���ҽ� ����
				FreeVillageInfo_RecruitArmy();

				return ON_SUB_VILLAGEINFO_POLL;
			}
			break;

		case ON_SUB_VILLAGEINFO_SUB_RECRUIT_FIELD_ARMY:		// �ʵ� ���� ������ ������ ���� ����
			{
				// ���⿡�� Ȯ�� ��ư�� ������, ��ȿ�� ���� �ִ� ���
				if(pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() && pMyOnlineWorld->pOnlineNumPad->siCount > 0)
				{
					// ������ �غ� ���Ͽ� ���, �����, ����� ��� �Ҽ� �������� ���¸��� ���� �ð��̸�
					if (FALSE == pMyOnlineWorld->IsInTimeDisabledRecruitArmyInGovernment())
					{
						m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_WAIT;

						// ���� ���� ���� ��� ���
						LPSSiegeUnitTable lpsSiegeUnitTable = pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTableByArrayIdx(m_VillageData.m_sRECRUIT_Army_InGovern.m_sArmyInformation_InGovern.m_siArmyKind);

						if (lpsSiegeUnitTable)
						{
							// ���� ���� ���� ��û �޽��� ����
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
						// ���� ���� ���� �ð��� �ƴ϶�� �޽��� �ڽ�  ���
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
		case ON_SUB_VILLAGEINFO_SUB_RECRUIT_GARRISON_ARMY:	// ���� �ֵб� ����
			{
				// ���⿡�� Ȯ�� ��ư�� ������, ��ȿ�� ���� �ִ� ���
				if(pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() && pMyOnlineWorld->pOnlineNumPad->siCount > 0)
				{
					// ������ �غ� ���Ͽ� ���, �����, ����� ��� �Ҽ� �������� ���¸��� ���� �ð��̸�
					if (FALSE == pMyOnlineWorld->IsInTimeDisabledRecruitArmyInGovernment())
					{
						m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_WAIT;

						// ���� ���� �ֵб� ���� 
						pMyOnlineWorld->pOnlineClient->SendIncreaseVillageDefenceSoldier(pMyOnlineWorld->pOnlineNumPad->siCount, pVillageHeader->uiVillageCode);
					}
					else
					{
						m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_POLL;

						// ���� ���� ���� �ð��� �ƴ϶�� �޽��� �ڽ�  ���
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

// ���¸��� �������̽� �׸���
VOID	OnlineGovernMgr::DrawRecruitArmy(LPDIRECTDRAWSURFACE7 pSurface)
{
}

UI08	OnlineGovernMgr::ActionProduceSupplyGoods(void)		// ����ǰ ����
{
	switch (m_VillageData.m_uiSubStatus)
	{
		case ON_SUB_VILLAGEINFO_SUB_INIT:
			{
				// ���� ����� ������ �ִ� ����ǰ ���� ���´�.
				m_VillageData.m_siSupplyGoodsAmount = 0;
				pMyOnlineWorld->pOnlineClient->SendGetGuildSupplyList(0);
				// ������� ������ ���

				m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_POLL;

				// ����ǰ ���� �������̽� �ʱ�ȭ
				InitVillageInfo_ProduceSupplyGoods();
			
				// ��ư ��Ȱ��ȭ
				// �����ϱ� ��ư
				m_VillageData.m_btnToDefenceMenu.SetAction(FALSE);
				// ���� ���� ��ư
				m_VillageData.m_btnRecruitArmy.SetAction(FALSE);
				// ����ǰ ���� ��ư
				m_VillageData.m_btnProduceSupplyGoods.SetAction(FALSE);
				// ���ɼ���
				m_VillageData.m_btnDeclareOccupation.SetAction(FALSE);

				// ������ ��ư ��Ȱ��ȭ
				m_BExit.SetAction(FALSE);
			}
			break;

		case ON_SUB_VILLAGEINFO_SUB_POLL:
			{
				// ������ �غ� ���Ͽ� ���, �����, ����� ��� �Ҽ� �������� ���¸��� ���� �ð��̸�
				if(FALSE == pMyOnlineWorld->IsInTimeDisabledRecruitArmyInGovernment())
					m_VillageData.m_sProduceSupplyGoods.m_btnProduceSupplyGoods.SetAction(TRUE);
				else
					m_VillageData.m_sProduceSupplyGoods.m_btnProduceSupplyGoods.SetAction(FALSE);

				// ����ǰ ���� ��ư
				if (m_VillageData.m_sProduceSupplyGoods.m_btnProduceSupplyGoods.Process(m_bMouseDown))
				{
					// ������ �غ� ���Ͽ� ���, �����, ����� ��� �Ҽ� �������� ����ǰ ���� ���� �ð��̸�
					if (FALSE == pMyOnlineWorld->IsInTimeDisabledRecruitArmyInGovernment())
					{
						// �������� �ִٸ�
						if (pMyOnlineWorld->pMyData->GetMoney() > 0)
						{
							// �ִ� ������ �� �ִ� ����ǰ ���� ���
							SI32 siMaxSupplyGoodsCount = pMyOnlineWorld->pMyData->GetMoney() / m_VillageData.m_sProduceSupplyGoods.m_siCostSupplyGoods;

							// ���� �Ǵ��� �˻�
							if (siMaxSupplyGoodsCount <= 0)
								break;

							// ���� �������̽� ����
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(siMaxSupplyGoodsCount);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_THING);
							pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(m_VillageData.m_sProduceSupplyGoods.m_btnProduceSupplyGoods.GetX() + 30, m_VillageData.m_sProduceSupplyGoods.m_btnProduceSupplyGoods.GetY() + 15);

							m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_PRODUCE_SUPPLYGOODS;
						}
					}
				}
				// ���� �޴��� ���ư��� ��ư
				else if (m_VillageData.m_btnBack.Process(m_bMouseDown))
				{
					m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_EXIT;
				}
			}
			break;
		case ON_SUB_VILLAGEINFO_SUB_WAIT:
			{
				// ������ ��ٸ���.
				pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
			}
			break;
		case ON_SUB_VILLAGEINFO_SUB_EXIT:
			{
				m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_INIT;

				// ����ǰ ���� �������̽� ����
				FreeVillageInfo_ProduceSupplyGoods();

				// ��û ���� �ǿ��� ���Ǵ� ��ư �����ϱ�, ���¸���, ����ǰ ����, ���ɼ��� ��ư ���� ����(Ȱ��, ��Ȱ��)
				UpdateVillageInfoButton();

				// ������ ��ư Ȱ��ȭ
				m_BExit.SetAction(TRUE);

				return ON_SUB_VILLAGEINFO_POLL;
			}
			break;
		// ���� ����ǰ ����
		case ON_SUB_VILLAGEINFO_SUB_PRODUCE_SUPPLYGOODS:
			{
				// ���⿡�� Ȯ�� ��ư�� ������, ��ȿ�� ���� �ִ� ���
				if(pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() && pMyOnlineWorld->pOnlineNumPad->siCount > 0)
				{
					// ������ �غ� ���Ͽ� ���, �����, ����� ��� �Ҽ� �������� ����ǰ ���� ���� �ð��̸�
					if (FALSE == pMyOnlineWorld->IsInTimeDisabledRecruitArmyInGovernment())
					{
						m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_WAIT;

						// ��� ����ǰ ���� ��û �޽���
						pMyOnlineWorld->pOnlineClient->SendIncreaseGuildSupply(pMyOnlineWorld->pOnlineNumPad->siCount, 0);
					}
					else
					{
						m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_POLL;

						// ����ǰ ���� ���� �ð��� �ƴ϶�� �޽��� �ڽ�  ���
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

UI08	OnlineGovernMgr::ActionDeclareOccupation(void)		// ���ɼ���
{
	return ON_SUB_VILLAGEINFO_POLL;
}

// robypark 2004/11/16 15:36
// ������ ��� ���� ���� ����Ʈ ��û ����
// ON_RESPONSE_LIST_GUILD_UNIT
UI16 OnlineGovernMgr::OnResponseSiegeWarfareUnitList(BYTE *pMsg)
{
	OnResGetGuildUnitList	*pOnResGetGuildUnitList = (OnResGetGuildUnitList *)pMsg;

//	ZeroMemory(m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount, sizeof(SI32) * ON_MAX_GUILD_UNIT);

	// ���� �˺� �δ�� ����
	m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SWORDMAN] = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SWORDMAN];
	// ���� â�� �δ�� ����
	m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SPEARMAN] = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SPEARMAN];
	// ���� �ú� �δ�� ����
	m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_ARCHERMAN] = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_ARCHERMAN];
	// ���� �⸶�� �δ�� ����
	m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_HORSEMAN] = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_HORSEMAN];
	// ���� ���� �δ�� ����
	m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SIEGETOWER] = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SIEGETOWER];
	// ���� �߼��� �δ�� ����
	m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_CATAPULT] = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_CATAPULT];
	// ���� ���޼��� �δ�� ����
	m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SUPPLYWAGON] = pOnResGetGuildUnitList->GuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_SUPPLYWAGON];

	// ��� ������ ���� �α��� ����
	m_VillageData.m_sRECRUIT_Army_InGovern.m_siRecruitablePopulation = pOnResGetGuildUnitList->uiPopulation;

	m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedUnitCount = TRUE;

	return 0;
}

// ��û���� ����̳� ����� ���� ������ ��û�� ���� �޽��� ó��
UI16 OnlineGovernMgr::OnIncreaseGuildUnit(BYTE* pMsg)
{
	OnResIncreaseGuildUnit*			pOnResIncreaseGuildUnit;

	pOnResIncreaseGuildUnit			=	(OnResIncreaseGuildUnit*)pMsg;

	m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_POLL;

	if (ON_RET_OK == pOnResIncreaseGuildUnit->dwResponse)
	{
		// ���� ����� ������ ���� ���� �δ� �� �� ���������� �α����� ���´�.
		m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedUnitCount = FALSE;
		pMyOnlineWorld->pOnlineClient->SendGetGuildUnitList(0);
		/*
		// ���� ����� ������ �ִ� ����ǰ ���� ���´�.
		m_VillageData.m_siSupplyGoodsAmount = 0;
		pMyOnlineWorld->pOnlineClient->SendGetGuildSupplyList(0);
		*/
	
		// ����Ǵ� ���� ���� ���� ��û. ���� �α��� ����
		if(pMyOnlineWorld->pOnlineVillage->GetVillageHeader()->uiVillageCode)
			pMyOnlineWorld->pOnlineClient->SendRequestVillageInfo(pMyOnlineWorld->pOnlineVillage->GetVillageHeader()->uiVillageCode);

		// ������ ����
		pMyOnlineWorld->pMyData->SetMoney(pMyOnlineWorld->pMyData->GetMoney() - pOnResIncreaseGuildUnit->mnMoney);

		// ������ �� ����ǰ �� ��ȭ ȿ�� ó��
		SEffectData					EffectData;
		ZeroMemory(&EffectData, sizeof(SEffectData));

		EffectData.m_Type = EFFECTTYPE_CHANGENUMBER;
		EffectData.m_EffectData_ChangeNum.m_Frame			= 0;
		EffectData.m_EffectData_ChangeNum.m_XPos			= m_siX + 104;
		EffectData.m_EffectData_ChangeNum.m_YPos			= m_siY + 354;
		EffectData.m_EffectData_ChangeNum.m_ChangeNumber    = -pOnResIncreaseGuildUnit->mnMoney;
		pMyOnlineWorld->pOnlineFieldEffect->AddEffect(EffectData);
		
		/*
		// ����ǰ �� ��ȭ effect
		ZeroMemory(&EffectData, sizeof(SEffectData));

		EffectData.m_Type = EFFECTTYPE_CHANGENUMBER;
		EffectData.m_EffectData_ChangeNum.m_Frame			= 0;
		EffectData.m_EffectData_ChangeNum.m_XPos			= m_siX + 474;
		EffectData.m_EffectData_ChangeNum.m_YPos			= m_siY + 354;
		EffectData.m_EffectData_ChangeNum.m_ChangeNumber    = -pOnResIncreaseGuildUnit->siSupply;
		pMyOnlineWorld->pOnlineFieldEffect->AddEffect(EffectData);
		*/
	}
	else if (ON_RET_NOT_ENOUGH_MONEY == pOnResIncreaseGuildUnit->dwResponse)			// ���� �����ϴ�.
	{
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ERRORNOTMONEY));
	}
	else if (ON_RET_NOT_ENOUGH_SUPPLY == pOnResIncreaseGuildUnit->dwResponse)			// ������ �����ϴ�.
	{
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_LACK_SUPPLYGOODS_ERROR));
	}
	else if (ON_RET_NOT_ENOUGH_POPULATION == pOnResIncreaseGuildUnit->dwResponse)			// �α����� �����ϴ�.
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
		// (ON_RET_NOT == pOnResIncreaseGuildUnit->dwResponse)			// �׹��� ����.
	}
	
	return 0;
}

// ������ ���� ���� ���� ��û �޽��� ����
UI16 OnlineGovernMgr::OnResponseIncreaseVillageDefenceSoldier(BYTE *pMsg)
{
	OnResVillageDefenceSoldier	*pOnResVillageDefenceSoldier = (OnResVillageDefenceSoldier *)pMsg;

	m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_POLL;

	if (ON_RET_OK == pOnResVillageDefenceSoldier->dwResponse)
	{
		// ���� ����� ������ ���� ���� �δ� �� �� ���������� �α����� ���´�.
		m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedUnitCount = FALSE;
		pMyOnlineWorld->pOnlineClient->SendGetGuildUnitList(0);
		// ���� ���� ���� �δ� ���� ���´�.
		m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedDefenceSoldier = FALSE;
		pMyOnlineWorld->pOnlineClient->SendVillageGarrisonCount(pVillageHeader->uiVillageCode);
		// ���� ����� ������ �ִ� ����ǰ ���� ���´�.
		m_VillageData.m_siSupplyGoodsAmount = 0;
		pMyOnlineWorld->pOnlineClient->SendGetGuildSupplyList(0);

		// ����Ǵ� ���� ���� ���� ��û. ���� �α��� ����
//		if(pMyOnlineWorld->pOnlineMap->m_MapInfo.uiVillageCode)
//			pMyOnlineWorld->pOnlineClient->SendRequestVillageInfo(pMyOnlineWorld->pOnlineMap->m_MapInfo.uiVillageCode);
		pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.uiPopulation -= pOnResVillageDefenceSoldier->siDefenceSoldier/* �δ���� �ƴ϶� �α����̴�.*/;

		// ������ ����
		pMyOnlineWorld->pMyData->SetMoney(pMyOnlineWorld->pMyData->GetMoney() - pOnResVillageDefenceSoldier->mnMoney);
	
		// ������ �� ����ǰ �� ��ȭ ȿ�� ó��
		SEffectData					EffectData;
		ZeroMemory(&EffectData, sizeof(SEffectData));

		EffectData.m_Type = EFFECTTYPE_CHANGENUMBER;
		EffectData.m_EffectData_ChangeNum.m_Frame			= 0;
		EffectData.m_EffectData_ChangeNum.m_XPos			= m_siX + 104;
		EffectData.m_EffectData_ChangeNum.m_YPos			= m_siY + 354;
		EffectData.m_EffectData_ChangeNum.m_ChangeNumber    = -pOnResVillageDefenceSoldier->mnMoney;
		pMyOnlineWorld->pOnlineFieldEffect->AddEffect(EffectData);
		
		// ����ǰ �� ��ȭ effect
		ZeroMemory(&EffectData, sizeof(SEffectData));

		EffectData.m_Type = EFFECTTYPE_CHANGENUMBER;
		EffectData.m_EffectData_ChangeNum.m_Frame			= 0;
		EffectData.m_EffectData_ChangeNum.m_XPos			= m_siX + 474;
		EffectData.m_EffectData_ChangeNum.m_YPos			= m_siY + 354;
		EffectData.m_EffectData_ChangeNum.m_ChangeNumber    = -pOnResVillageDefenceSoldier->siSupply;
		pMyOnlineWorld->pOnlineFieldEffect->AddEffect(EffectData);
	}
	else if (ON_RET_NOT_ENOUGH_MONEY == pOnResVillageDefenceSoldier->dwResponse)			// ���� �����ϴ�.
	{
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ERRORNOTMONEY));
	}
	else if (ON_RET_NOT_ENOUGH_SUPPLY == pOnResVillageDefenceSoldier->dwResponse)			// ������ �����ϴ�.
	{
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_LACK_SUPPLYGOODS_ERROR));
	}
	else if (ON_RET_NOT_ENOUGH_POPULATION == pOnResVillageDefenceSoldier->dwResponse)			// �α����� �����ϴ�.
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
		// (ON_RET_NOT == pOnResIncreaseGuildUnit->dwResponse)			// �׹��� ����.
	}
	
	return 0;
}

// ������ ���� ���� �� ���
// ON_RESPONSE_GET_VILLAGE_DEFENCE_SOLDIER
UI16 OnlineGovernMgr::OnResponseGetVillageDefenceSoldier(BYTE *pMsg)
{
	OnResGetVillageDefenceSoldier	*pOnResGetVillageDefenceSoldier = (OnResGetVillageDefenceSoldier *)pMsg;

	m_VillageData.m_sRECRUIT_Army_InGovern.m_siGuildCount[ARRAY_INDEX_SIEGEWARFAREUNIT_DEFENCESOLIDER] = pOnResGetVillageDefenceSoldier->siDefenceSoldier;

	m_VillageData.m_sRECRUIT_Army_InGovern.m_bDownloadedDefenceSoldier = TRUE;

	return 0;
}

// ������ ��� ����ǰ ���� ���
// ON_RESPONSE_LIST_GUILD_SUPPLY
UI16 OnlineGovernMgr::OnResponseListGuildSupply(BYTE *pMsg)
{
	OnResGetGuildSupplyList *pOnResGetGuildSupplyList = (OnResGetGuildSupplyList *)pMsg;

	m_VillageData.m_siSupplyGoodsAmount = pOnResGetGuildSupplyList->siGuildSupply;

	return 0;
}

// ������ ��� ����ǰ ���� ��û ����
// ON_RESPONSE_INCREASE_GUILD_SUPPLY
UI16 OnlineGovernMgr::OnResponseIncreaseGuildSupply(BYTE *pMsg)
{
	OnResIncreaseGuildSupply *pOnResIncreaseGuildSupply = (OnResIncreaseGuildSupply *)pMsg;

	m_VillageData.m_uiSubStatus = ON_SUB_VILLAGEINFO_SUB_POLL;

	if (ON_RET_OK == pOnResIncreaseGuildSupply->dwResponse)
	{
		// ������ ����
		pMyOnlineWorld->pMyData->SetMoney(pMyOnlineWorld->pMyData->GetMoney() - pOnResIncreaseGuildSupply->uiMoney);

		// ���� ����� ������ �ִ� ����� ����ǰ ���� ���´�.
		m_VillageData.m_siSupplyGoodsAmount = pOnResIncreaseGuildSupply->siSupplyQuantity;
		pMyOnlineWorld->pOnlineClient->SendGetGuildSupplyList(0);

		// ������ ��ȭ ȿ�� ó��
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
		// ������ ����
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ERRORNOTMONEY));
	}
	else
	{
		// ���� ����
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FAIL_TO_PRODUCE_SUPPLYGOODS));
	}

	return 0;
}

