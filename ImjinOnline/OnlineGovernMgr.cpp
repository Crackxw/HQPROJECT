#include <GSL.h>

#include "OnlineWorld.h"
#include <Mouse.h>
#include "OnlineVillage.h"
#include "OnlineFont.h"
#include "OnlineMegaText.h"
#include "OnlineMercenary.h"
#include "OnlineGovernMgr.h"
#include <etc.h>


extern _InputDevice		IpD;


OnlineGovernMgr::OnlineGovernMgr()
{
	m_uiMainStatus = ON_GOVERN_MAIN_STATUS_ENTER_WAIT;
	m_VillageData.m_uiStatus = ON_SUB_VILLAGEINFO_INIT;
	m_InvestData.m_uiStatus = ON_SUB_INVEST_INIT;
	m_GuildData.m_uiStatus = ON_SUB_GUILD_INIT;
}

OnlineGovernMgr::~OnlineGovernMgr()
{
	clGrp.FreeXspr(m_TabImage);
	clGrp.FreeXspr(m_ExitImage);
}

VOID OnlineGovernMgr::Init(UI16 VillageCode, cltOnlineWorld* pOnlineWorld, VillageStructManager* pVillageStructManager)
{
	OnlineVillageBaseStructure::Init(VillageCode, pOnlineWorld, pVillageStructManager);

	m_siX = 134;
	m_siY = 45;

	clGrp.LoadXspr("Online\\GameSpr\\Village\\town_office_tab.spr", m_TabImage);
	clGrp.LoadXspr("Online\\GameSpr\\Inventory\\main_button1.Spr", m_ExitImage);

	m_BVillageInfo.Create(m_siX+63, m_siY+31, m_TabImage.Header.Xsize, m_TabImage.Header.Ysize+5, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TRADE_VILLAGEINFO), BUTTON_PUT_LEFT, TRUE);
	m_BInvest.Create(m_siX+205, m_siY+31, m_TabImage.Header.Xsize, m_TabImage.Header.Ysize+5, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_INVESTMENT), BUTTON_PUT_LEFT, TRUE);
	m_BGuild.Create(m_siX+348, m_siY+31, m_TabImage.Header.Xsize, m_TabImage.Header.Ysize+5, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATION), BUTTON_PUT_LEFT, TRUE);
	m_BExit.Create(m_siX+232, m_siY+394, m_ExitImage.Header.Xsize, m_ExitImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_EXIT), BUTTON_PUT_LEFT, TRUE);

	// 관청에 들어간다고 서버에 알려준다.
	pMyOnlineWorld->pOnlineClient->SendRequestGovernment_Main_Enter();
}

BOOL OnlineGovernMgr::Poll(LPDIRECTDRAWSURFACE7 pSurface)
{
	// 마을의 기본 정보들을 그린다.(건물 외부)
	pMyOnlineWorld->pOnlineVillage->DrawVillageInfo(pSurface, TRUE);

	pMyOnlineWorld->pIOnlineMouse->GetPos(&ptMouse);

	switch(m_uiMainStatus)
	{
	case ON_GOVERN_MAIN_STATUS_ENTER_WAIT:
		// GovernEnter 응답을 기다린다.
		break;
	case ON_GOVERN_MAIN_STATUS_VILLAGEINFO:
		{
			m_uiMainStatus = PollVillageInfo(pSurface);
		}
		break;
	case ON_GOVERN_MAIN_STATUS_INVESTMENT:
		{
			m_uiMainStatus = PollInvestment(pSurface);
		}
		break;
	case ON_GOVERN_MAIN_STATUS_GUILD:
		{
			m_uiMainStatus = PollGuild(pSurface);
		}
		break;
	case ON_GOVERN_MAIN_STATUS_EXIT_WAIT:
		// 응답을 기다린다.(GovernLeave)
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
		break;
	case ON_GOVERN_MAIN_STATUS_EXIT:
		{
			return TRUE;
		}
		break;
	}

	DrawMgr(pSurface);

	if(IpD.LeftPressSwitch) m_bMouseDown = TRUE;	
	else				    m_bMouseDown = FALSE;

	return FALSE;
}

VOID OnlineGovernMgr::DrawMgr(LPDIRECTDRAWSURFACE7 pSurface)
{
	if(m_uiMainStatus != ON_GOVERN_MAIN_STATUS_VILLAGEINFO && m_uiMainStatus != ON_GOVERN_MAIN_STATUS_INVESTMENT
		&& m_uiMainStatus != ON_GOVERN_MAIN_STATUS_GUILD)
		return;

	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		m_BVillageInfo.Put(&m_TabImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_BInvest.Put(&m_TabImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_BGuild.Put(&m_TabImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_BExit.Put(&m_ExitImage, 0, 2, 1, BUTTON_PUT_NOMOVE);

		clGrp.UnlockSurface( pSurface );
	}

	pMyOnlineWorld->m_pMercenary->Draw(GETSCREENSURFACE(pMyOnlineWorld));

	// actdoll (2004/01/06 17:08) : 다른 방법으로 NPC 초상그리기
	pMyOnlineWorld->m_pPortrait->DrawAsNPC( m_siX + 15, m_siY + 65, NPC_KIND_GOV, pMyOnlineWorld->pOnlineVillage->GetNation() );

	HDC	hdc;
	if(pSurface->GetDC(&hdc) == DD_OK)
	{
		SetBkMode( hdc, TRANSPARENT );
		SelectObject( hdc, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));

		// 관청
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, m_siX + 194, m_siY + 5, 143, 16, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), RGB(200, 200, 200));

		m_BVillageInfo.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_BInvest.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_BGuild.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_BExit.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		
		pSurface->ReleaseDC(hdc);
	}

	pMyOnlineWorld->pOnlineMsgBox->Draw(pSurface);
}

