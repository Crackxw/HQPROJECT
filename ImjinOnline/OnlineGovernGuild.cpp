#include <GSL.h>

#include <clGame.h>
#include "OnlineWorld.h"
#include "OnlineFont.h"
#include "OnlineMegaText.h"
#include "Common.h"
#include "OnlineVillage.h"
#include "MakePalette.h"
#include "etc.h"
#include "OnlineVillageManager.h"

#include "OnlineGovernMgr.h"

extern	_clGame* pGame;


VOID OnlineGovernMgr::InitGuild()
{
	m_BGuild.SetAction(FALSE);
	m_GuildData.m_uiStatus = ON_SUB_GUILD_INIT;
	m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_INIT;

	clGrp.LoadXspr("Online\\GameSpr\\Village\\town_office_guild.spr", m_GuildData.m_MainImage);
	clGrp.LoadXspr("Online\\GameSpr\\Village\\town_button_medium.spr", m_GuildData.m_ButImage);
	clGrp.LoadXspr("Online\\GameSpr\\Village\\town_office_guild_scrbar.spr", m_GuildData.m_ScrBackImage);
	clGrp.LoadXspr("Online\\Gamespr\\Book\\book_scr_handle.Spr", m_GuildData.m_ScrBarImage);
	clGrp.LoadXspr("Online\\Gamespr\\Book\\book_scr_Up.Spr", m_GuildData.m_UpImage);
	clGrp.LoadXspr("Online\\Gamespr\\Book\\book_scr_Down.Spr", m_GuildData.m_DownImage);
	clGrp.LoadXspr("Online\\VillageBack\\GuildDB.Spr", m_GuildData.m_RelationImage);

	// 화친제의 받은 표시 아이콘 이미지 로드
	clGrp.LoadXspr("Online\\Gamespr\\Village\\GUILDDBXXX.Spr", m_GuildData.m_sprOfferedPeace);

	// Main 버튼
	m_GuildData.m_BOpen.Create(m_siX+122, m_siY+78, m_GuildData.m_ButImage.Header.Xsize, m_GuildData.m_ButImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONBIL), BUTTON_PUT_LEFT, FALSE);
	m_GuildData.m_BMemberList.Create(m_siX+222, m_siY+78, m_GuildData.m_ButImage.Header.Xsize, m_GuildData.m_ButImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_GUILD_JOIN), BUTTON_PUT_LEFT, FALSE);
	m_GuildData.m_BFlag.Create(m_siX+322, m_siY+78, m_GuildData.m_ButImage.Header.Xsize, m_GuildData.m_ButImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_FLAGCHOICE), BUTTON_PUT_LEFT, FALSE);
	m_GuildData.m_BJoinLeave.Create(m_siX+422, m_siY+78, m_GuildData.m_ButImage.Header.Xsize, m_GuildData.m_ButImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONYN), BUTTON_PUT_LEFT, FALSE);
	m_GuildData.m_BClose.Create(m_siX+122, m_siY+114, m_GuildData.m_ButImage.Header.Xsize, m_GuildData.m_ButImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONDES), BUTTON_PUT_LEFT, FALSE);
	m_GuildData.m_BAppoint.Create(m_siX+222, m_siY+114, m_GuildData.m_ButImage.Header.Xsize, m_GuildData.m_ButImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_GUILD_MANAGE), BUTTON_PUT_LEFT, FALSE);
	m_GuildData.m_BDiffGuild.Create(m_siX+322, m_siY+114, m_GuildData.m_ButImage.Header.Xsize, m_GuildData.m_ButImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_OTHERTRADEASSOCIATION), BUTTON_PUT_LEFT, TRUE);

	// Main 버튼의 활성/비활성 결정
	UpdateButtonStatus();

	// Sub 버튼
	m_GuildData.m_BOk.Create(m_siX+227, m_siY+366, m_GuildData.m_ButImage.Header.Xsize, m_GuildData.m_ButImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_BOARD_OK), BUTTON_PUT_LEFT, FALSE);
	m_GuildData.m_BCancel.Create(m_siX+322, m_siY+366, m_GuildData.m_ButImage.Header.Xsize, m_GuildData.m_ButImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONN), BUTTON_PUT_LEFT, FALSE);
	m_GuildData.m_BBack.Create(m_siX+417, m_siY+366, m_GuildData.m_ButImage.Header.Xsize, m_GuildData.m_ButImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BANK_ACCOUNTBUTTON_EXIT), BUTTON_PUT_LEFT, FALSE);
	m_GuildData.m_BDHS.Create(m_siX+37, m_siY+366, m_GuildData.m_ButImage.Header.Xsize, m_GuildData.m_ButImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_DHS_APPOINT), BUTTON_PUT_LEFT, FALSE);
	m_GuildData.m_BHND.Create(m_siX+132, m_siY+366, m_GuildData.m_ButImage.Header.Xsize, m_GuildData.m_ButImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_HND_APPOINT), BUTTON_PUT_LEFT, FALSE);
	// 다른상단 => 화친제의 버튼
	m_GuildData.m_btnOfferPeace.Create(m_siX+227, m_siY+366, m_GuildData.m_ButImage.Header.Xsize, m_GuildData.m_ButImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BUTTON_OFFER_PEACE), BUTTON_PUT_LEFT, FALSE);

	// 스크롤
	SetRect(&m_GuildData.m_rtScroll, m_siX + 498, m_siY + 164, m_siX + 498 + 9, m_siY + 164 + 186);
	m_GuildData.m_siScrollPos = m_siY + 164;
	m_GuildData.m_BScrollUp.Create(m_siX+498, m_siY+155, m_GuildData.m_UpImage.Header.Xsize, m_GuildData.m_UpImage.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);
	m_GuildData.m_BScrollDown.Create(m_siX+498, m_siY+350, m_GuildData.m_DownImage.Header.Xsize, m_GuildData.m_DownImage.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE);

	// 상단설립
	/*
	m_GuildData.m_pEditBox = new OnlineEditControl;
	m_GuildData.m_pEditBox->CreateEditBox(1000, 1000, 500, 50, pGame->Hwnd, pGame->hInst, FALSE, FALSE, ON_GUILDNAME_LENGTH );
//	SendMessage(m_GuildData.m_pEditBox->GethWnd(), EM_LIMITTEXT, (WPARAM)ON_GUILDNAME_LENGTH, 0);
	m_GuildData.m_pEditBox->SetFont(pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));
	m_GuildData.m_pEditBox->SetKeyLock('\'', TRUE);	m_GuildData.m_pEditBox->SetKeyLock('\"', TRUE);	m_GuildData.m_pEditBox->SetKeyLock('\\', TRUE);
	m_GuildData.m_pEditBox->SetKeyLock(0x0D, TRUE);	m_GuildData.m_pEditBox->SetKeyLock(0x0A, TRUE);	m_GuildData.m_pEditBox->SetKeyLock('\t', TRUE);	m_GuildData.m_pEditBox->SetKeyLock('%', TRUE);
	*/

	OnlineEditControlEx	*pOecEx;
	if( m_GuildData.m_hOecEditBox )	Handler_OnlineEditControlEx::KillControl( m_GuildData.m_hOecEditBox );
	RECT	rcRect = { m_siX + 125, m_siY + 290, m_siX + 125 + 100, m_siY + 305 };
	m_GuildData.m_hOecEditBox	= Handler_OnlineEditControlEx::CreateControl( 0, &rcRect, ON_GUILDNAME_LENGTH );
	pOecEx			= Handler_OnlineEditControlEx::GetControl( m_GuildData.m_hOecEditBox );
	pOecEx->SetFont( pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
	pOecEx->SetColor( RGB(250, 250, 250) );
	pOecEx->SetExceptionChar( '\'' );
	pOecEx->SetExceptionChar( '\"' );
	pOecEx->SetExceptionChar( 0x0D );
	pOecEx->SetExceptionChar( 0x0A );
	pOecEx->SetExceptionChar( '%'  );
	pOecEx->SetExceptionChar( '\\' );
	pOecEx->SetExceptionChar( '\t' );

}

VOID OnlineGovernMgr::FreeGuild()
{
	clGrp.FreeXspr(m_GuildData.m_MainImage);
	clGrp.FreeXspr(m_GuildData.m_ButImage);
	clGrp.FreeXspr(m_GuildData.m_ScrBackImage);
	clGrp.FreeXspr(m_GuildData.m_ScrBarImage);
	clGrp.FreeXspr(m_GuildData.m_UpImage);
	clGrp.FreeXspr(m_GuildData.m_DownImage);
	clGrp.FreeXspr(m_GuildData.m_RelationImage);

	// 화친제의 받은 표시 아이콘 이미지 해제
	clGrp.FreeXspr(m_GuildData.m_sprOfferedPeace);

	/*if(m_GuildData.m_pEditBox)
	{
		m_GuildData.m_pEditBox->Free();

		delete m_GuildData.m_pEditBox;
		m_GuildData.m_pEditBox = NULL;
	}*/
	Handler_OnlineEditControlEx::KillControl( m_GuildData.m_hOecEditBox );


	if(m_GuildData.m_pGuildList)
	{
		delete [] m_GuildData.m_pGuildList;
		m_GuildData.m_pGuildList = NULL;
	}

	if(m_GuildData.m_pVillageList)
	{
		delete [] m_GuildData.m_pVillageList;
		m_GuildData.m_pVillageList = NULL;
	}

	if(m_GuildData.m_pWaitingList)
	{
		delete [] m_GuildData.m_pWaitingList;
		m_GuildData.m_pWaitingList = NULL;
	}

	if(m_GuildData.m_pMemberList)
	{
		delete [] m_GuildData.m_pMemberList;
		m_GuildData.m_pMemberList = NULL;
	}

	pMyOnlineWorld->SetShortKey(FALSE);

	m_BGuild.SetAction(TRUE);
	m_GuildData.m_uiStatus = ON_SUB_GUILD_INIT;
	m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_INIT;
}

UI08 OnlineGovernMgr::PollGuild(LPDIRECTDRAWSURFACE7 pSurface)
{
	switch(m_GuildData.m_uiStatus)
	{
	case ON_SUB_GUILD_INIT:
		{
			InitGuild();
			m_GuildData.m_uiStatus = ON_SUB_GUILD_POLL;
		}
		break;
	case ON_SUB_GUILD_POLL:
		{
			// 메인메뉴 전환
			if(m_BVillageInfo.Process(m_bMouseDown))
			{
				m_uiReturnStatus = ON_GOVERN_MAIN_STATUS_VILLAGEINFO;
				m_GuildData.m_uiStatus = ON_SUB_GUILD_EXIT;
			}
			else if(m_BInvest.Process(m_bMouseDown))
			{
				m_uiReturnStatus = ON_GOVERN_MAIN_STATUS_INVESTMENT;
				m_GuildData.m_uiStatus = ON_SUB_GUILD_EXIT;
			}
			else if(m_BExit.Process(m_bMouseDown))
			{
				pMyOnlineWorld->pOnlineClient->SendRequestGovernment_Main_Leave();
				m_uiReturnStatus = ON_GOVERN_MAIN_STATUS_EXIT_WAIT;
				m_GuildData.m_uiStatus = ON_SUB_GUILD_EXIT;
			}

			// 상단관련 메인메뉴
			if(m_GuildData.m_BOpen.Process(m_bMouseDown))
				m_GuildData.m_uiStatus = ON_SUB_GUILD_OPEN;
			else if(m_GuildData.m_BMemberList.Process(m_bMouseDown))
				m_GuildData.m_uiStatus = ON_SUB_GUILD_WAITING_LIST;
			else if(m_GuildData.m_BFlag.Process(m_bMouseDown))
				m_GuildData.m_uiStatus = ON_SUB_GUILD_FLAG;
			else if(m_GuildData.m_BJoinLeave.Process(m_bMouseDown))
				m_GuildData.m_uiStatus = ON_SUB_GUILD_JOIN_LEAVE;
			else if(m_GuildData.m_BClose.Process(m_bMouseDown))
				m_GuildData.m_uiStatus = ON_SUB_GUILD_CLOSE;
			else if(m_GuildData.m_BAppoint.Process(m_bMouseDown))
				m_GuildData.m_uiStatus = ON_SUB_GUILD_MEMBER_LIST;
			else if(m_GuildData.m_BDiffGuild.Process(m_bMouseDown))
				m_GuildData.m_uiStatus = ON_SUB_GUILD_DIFF_GUILD;
		}
		break;
	case ON_SUB_GUILD_OPEN:				// 상단설립
		m_GuildData.m_uiStatus = ActionOpen();
		break;
	case ON_SUB_GUILD_WAITING_LIST:		// 대기자리스트
		m_GuildData.m_uiStatus = ActionWaitingList();
		break;
	case ON_SUB_GUILD_FLAG:				// 깃발선택
		m_GuildData.m_uiStatus = ActionFlag();
		break;
	case ON_SUB_GUILD_JOIN_LEAVE:		// 가입/탈퇴
		m_GuildData.m_uiStatus = ActionJoinLeave();
		break;
	case ON_SUB_GUILD_CLOSE:			// 상단해체
		m_GuildData.m_uiStatus = ActionClose();
		break;
	case ON_SUB_GUILD_MEMBER_LIST:		// 상단원임명
		m_GuildData.m_uiStatus = ActionMemberList();
		break;
	case ON_SUB_GUILD_DIFF_GUILD:		// 다른상단
		m_GuildData.m_uiStatus = ActionDiffGuild();
		break;
	case ON_SUB_GUILD_EXIT:
		{
			DrawGuild(pSurface);
			FreeGuild();
			return m_uiReturnStatus;
		}
		break;
	}

	DrawGuild(pSurface);
	
	return ON_GOVERN_MAIN_STATUS_GUILD;
}

VOID OnlineGovernMgr::DrawGuild(LPDIRECTDRAWSURFACE7 pSurface)
{
	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		clGrp.PutSpriteT(m_siX, m_siY, m_GuildData.m_MainImage.Header.Xsize, m_GuildData.m_MainImage.Header.Ysize, m_GuildData.m_MainImage.Image);

		switch(m_GuildData.m_uiStatus)
		{
		case ON_SUB_GUILD_MEMBER_LIST:
			m_GuildData.m_BDHS.Put(&m_GuildData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
			m_GuildData.m_BHND.Put(&m_GuildData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
		case ON_SUB_GUILD_WAITING_LIST:
			m_GuildData.m_BCancel.Put(&m_GuildData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
		case ON_SUB_GUILD_OPEN:
		case ON_SUB_GUILD_CLOSE:
		case ON_SUB_GUILD_JOIN_LEAVE:
			m_GuildData.m_BOk.Put(&m_GuildData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
			m_GuildData.m_BBack.Put(&m_GuildData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
			break;
		case ON_SUB_GUILD_FLAG:
			DrawFlag(pSurface);
			break;
		case ON_SUB_GUILD_DIFF_GUILD:
			DrawDiffGuildList(pSurface);
			break;
		}

		// 각 버튼들
		m_GuildData.m_BOpen.Put(&m_GuildData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_GuildData.m_BMemberList.Put(&m_GuildData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_GuildData.m_BFlag.Put(&m_GuildData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_GuildData.m_BJoinLeave.Put(&m_GuildData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_GuildData.m_BClose.Put(&m_GuildData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_GuildData.m_BAppoint.Put(&m_GuildData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
		m_GuildData.m_BDiffGuild.Put(&m_GuildData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);

		clGrp.UnlockSurface(pSurface);
	}

	HDC	hdc;
	CHAR szTemp[256];
	OnlineEditControlEx	*pOecEx;
	if(pSurface->GetDC(&hdc) == DD_OK)
	{
		SetBkMode( hdc, TRANSPARENT );
		SelectObject( hdc, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));

		switch(m_GuildData.m_uiStatus)
		{
		case ON_SUB_GUILD_OPEN:
			{
				if(m_GuildData.m_GuildOpenMoney)
				{
					pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX+50, m_siY+180, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONNEED),  RGB(250, 250, 250));
					pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX+70, m_siY+200, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONNEED1), RGB(250, 250, 250));
					pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX+70, m_siY+220, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONNEED2), RGB(250, 250, 250));
					pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX+70, m_siY+240, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONNEED3), RGB(250, 250, 250));

					sprintf((char*)szTemp, pMyOnlineWorld->pOnlineText->Get(2287), m_GuildData.m_GuildOpenMoney);
					pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX+50, m_siY+260, (char*)szTemp, RGB(250, 250, 250));

					pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX+82, m_siY+290, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_NAME), RGB(250, 250, 250));
					//m_GuildData.m_pEditBox->DrawText(hdc, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT), pMyOnlineWorld->pOnlineFonts->GetFontUnderline(ON_FONT_CHAT), RGB(250, 250, 250), m_siX+125, m_siY+290, NULL, TRUE);
					
					pOecEx = Handler_OnlineEditControlEx::GetControl(m_GuildData.m_hOecEditBox);
					pOecEx->Draw(hdc);	
				}

				m_GuildData.m_BOk.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				m_GuildData.m_BBack.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			}
			break;
		case ON_SUB_GUILD_CLOSE:
			{
				sprintf((char*)szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_REALFIRE), pMyOnlineWorld->pMyData->GetGuildName());
				pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX+110, m_siY+230, (char*)szTemp, RGB(250, 250, 250));

				m_GuildData.m_BOk.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				m_GuildData.m_BBack.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			}
			break;
		case ON_SUB_GUILD_WAITING_LIST:
			DrawMemberList(0, hdc);
			break;
		case ON_SUB_GUILD_MEMBER_LIST:
			DrawMemberList(1, hdc);
			break;
		case ON_SUB_GUILD_FLAG:
			{
				m_GuildData.m_BOk.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				m_GuildData.m_BBack.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			}
			break;
		case ON_SUB_GUILD_JOIN_LEAVE:
			{
				if(!pMyOnlineWorld->pMyData->GetGuildName())	// 상단가입
				{
					if(pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID[0])
					{
						sprintf((char*)szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONJOIN), pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID);
						pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX+110, m_siY+230, (char*)szTemp, RGB(250, 250, 250));
					}
				}
				else											// 상단탈퇴
				{
					if(pMyOnlineWorld->pMyData->GetGuildClass() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ))
						sprintf((char*)szTemp,pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONJOINCEN), pMyOnlineWorld->pMyData->GetGuildName());
					else
						sprintf((char*)szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONJOINOUT), pMyOnlineWorld->pMyData->GetGuildName());
					pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX+110, m_siY+230, (char*)szTemp, RGB(250, 250, 250));
				}

				m_GuildData.m_BOk.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				m_GuildData.m_BBack.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			}
			break;
		case ON_SUB_GUILD_DIFF_GUILD:
			DrawDiffGuildList(hdc);
			break;
		}

		// 각 버튼들
		m_GuildData.m_BOpen.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_GuildData.m_BMemberList.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_GuildData.m_BFlag.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_GuildData.m_BJoinLeave.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_GuildData.m_BClose.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_GuildData.m_BAppoint.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_GuildData.m_BDiffGuild.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		pSurface->ReleaseDC(hdc);
	}	
}

// 상단설립
UI08 OnlineGovernMgr::ActionOpen()
{
	switch(m_GuildData.m_uiSubStatus)
	{
	case ON_SUB_SUB_GUILD_INIT:
		{
			InitButtonStatus();
			//m_GuildData.m_pEditBox->EnableWindow(TRUE);
			//m_GuildData.m_pEditBox->SetFocus();
			Handler_OnlineEditControlEx::SetFocus( m_GuildData.m_hOecEditBox );
			pMyOnlineWorld->SetShortKey(TRUE);

			// 상단설립비용을 요청한다.
			pMyOnlineWorld->pOnlineClient->SendRequestGuildCreateCost();
			m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_WAIT;
		}
		break;
	case ON_SUB_SUB_GUILD_POLL:
		{
			if(m_GuildData.m_BOk.Process(m_bMouseDown))
			{
				m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_EXIT;

				if(pMyOnlineWorld->pMyData->GetFollowerParameterLevel(0) >= GUILDMAKE_MINLEVEL || pMyOnlineWorld->pMyData->GetTradeGrade() >= GUILDMAKE_MINCREDIT)
				{
					if(pMyOnlineWorld->pMyData->GetMoney() >= m_GuildData.m_GuildOpenMoney)
					{
						CHAR szGuildName[ON_GUILDNAME_LENGTH+1];
						ZeroMemory(szGuildName, sizeof(szGuildName));
						//m_GuildData.m_pEditBox->GetString(szGuildName);
						OnlineEditControlEx	*pOecEx;
						pOecEx = Handler_OnlineEditControlEx::GetControl( m_GuildData.m_hOecEditBox );
						strcpy(szGuildName,pOecEx->GetString());

						/*
						// 허용되는 상단명이 그때 그때 다르게 길이가 적용된다. NULL문자 포함 10Bytes 혹은 NULL문제 제외 10Bytes(총 11Bytes)를 허용하기도 한다.
						// NULL 문자 포함 상단명은 ON_GUILDNAME_LENGTH(10)보다 작아야만 한다.
						if (strlen(szGuildName) >= ON_GUILDNAME_LENGTH)
						{
							pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONINFOERROR1));
							break;
						}
						*/

						if(CheckLeadByte(szGuildName))
						{
							pMyOnlineWorld->pOnlineClient->SendRequestGovernment_MakeGuild((char*)szGuildName);
							m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_WAIT;
						}
						else
							pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(8000479));
					}
					else
					{
						CHAR szTemp[256];
						sprintf(szTemp, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_GUILD_OPEN_COST), m_GuildData.m_GuildOpenMoney);
						pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), (char*)szTemp);
					}
				}
				else
					pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONAGREE), GUILDMAKE_MINLEVEL, GUILDMAKE_MINCREDIT);
			}
			else if(m_GuildData.m_BBack.Process(m_bMouseDown))
			{
				m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_EXIT;
			}
		}
		break;
	case ON_SUB_SUB_GUILD_WAIT:
		// 상단설립비용, 설립성공여부 응답을 기다린다.(GovernGuildCreateCost, GovernGuildCreate)
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
		break;
	case ON_SUB_SUB_GUILD_EXIT:
		{
			OnlineEditControlEx	*pOecEx	= Handler_OnlineEditControlEx::GetControl( m_GuildData.m_hOecEditBox );
			UpdateButtonStatus();
			pMyOnlineWorld->SetShortKey(FALSE);
			//::SetFocus(pGame->Hwnd);
			pOecEx->Clear();
			Handler_OnlineEditControlEx::ClearCurrentFocus();
			m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_INIT;
			return ON_SUB_GUILD_POLL;
		}
		break;
	}
	return ON_SUB_GUILD_OPEN;
}

// 대기자리스트
UI08 OnlineGovernMgr::ActionWaitingList()
{
	switch(m_GuildData.m_uiSubStatus)
	{
	case ON_SUB_SUB_GUILD_INIT:
		{
			InitButtonStatus();
			m_GuildData.m_siSelect = m_GuildData.m_siScroll = m_GuildData.m_uiWaitingNum = 0;
			m_GuildData.m_siScrollPos = m_siY + 164;
			m_GuildData.m_siCurPage = 1;

			if(pMyOnlineWorld->pMyData->GetGuildName())
			{
				if(pMyOnlineWorld->pMyData->GetGuildClass() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
				{
					pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetJoinGuild(m_GuildData.m_siCurPage);
					m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_WAIT;
				}
				else
					m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_EXIT;
			}
			else
				m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_EXIT;
		}
		break;
	case ON_SUB_SUB_GUILD_POLL:
		{
			// actdoll (2004/08/28 18:50) : 버그 수정 - 길드 가입 요청대기자가 없을 경우는 OK와 CANCEL버튼을 비활성화시킨다.
			if( m_GuildData.m_uiWaitingNum <= 0 )
			{
				m_GuildData.m_BOk.SetAction( false );
				m_GuildData.m_BCancel.SetAction( false );
			}
			else
			{
				m_GuildData.m_BOk.SetAction( true );
				m_GuildData.m_BCancel.SetAction( true );
			}

			// 대기자, Page 선택 처리
			if(m_bMouseDown)
			{
				if(ActionMemberList(0) != -1)
				{
					pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetJoinGuild(m_GuildData.m_siCurPage);
					m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_WAIT;
				}
			}

			if(m_GuildData.m_BOk.Process(m_bMouseDown))
			{
				// 상단 가입 불가능한 시간인지 검사
				if (pMyOnlineWorld->IsInTimeDisabledTimeToJoin())
					break;

				if(m_GuildData.m_siSelect >= 0)
				{
					m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_WAIT;
					pMyOnlineWorld->pOnlineClient->SendRequestGovernment_JoinGuildPlayer((char*)m_GuildData.m_pWaitingList[m_GuildData.m_siSelect].szName);
				}
			}
			else if(m_GuildData.m_BCancel.Process(m_bMouseDown))
			{
				if(m_GuildData.m_siSelect >= 0)
				{
					m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_WAIT2;
					pMyOnlineWorld->pOnlineClient->SendRequestGovernment_JoinCancelGuildPlayer((char*)m_GuildData.m_pWaitingList[m_GuildData.m_siSelect].szName);
				}
			}
			else if(m_GuildData.m_BBack.Process(m_bMouseDown))
			{
				m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_EXIT;
			}
		}
		break;
	case ON_SUB_SUB_GUILD_WAIT:
	case ON_SUB_SUB_GUILD_WAIT2:
		// 응답을 기다린다.
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
		break;
	case ON_SUB_SUB_GUILD_EXIT:
		{
			UpdateButtonStatus();
			m_GuildData.m_uiWaitingNum = 0;
			m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_INIT;
			return ON_SUB_GUILD_POLL;
		}
		break;
	}
	return ON_SUB_GUILD_WAITING_LIST;
}

// 깃발선택
UI08 OnlineGovernMgr::ActionFlag()
{
	switch(m_GuildData.m_uiSubStatus)
	{
	case ON_SUB_SUB_GUILD_INIT:
		{
			InitButtonStatus();
			m_GuildData.m_siSelect = m_GuildData.m_siScroll = 0;
			m_GuildData.m_siScrollPos = m_siY + 164;
			pMyOnlineWorld->pOnlineClient->SendUsedFlag();
			m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_WAIT;
		}
		break;
	case ON_SUB_SUB_GUILD_POLL:
		{
			// 스크롤 및 깃발선택 처리
			if(m_bMouseDown)
				ActionFlagScroll();

			if(m_GuildData.m_BOk.Process(m_bMouseDown))
			{
				if(m_GuildData.m_siSelect >= 0)
				{
					if((m_GuildData.m_siFlagIndex[m_GuildData.m_siSelect] & 0x8000) == 0)
					{
						pMyOnlineWorld->pOnlineClient->SendChangeGuildFlag(m_GuildData.m_siFlagIndex[m_GuildData.m_siSelect]);
						m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_WAIT;
					}
				}
			}
			else if(m_GuildData.m_BBack.Process(m_bMouseDown))
			{
				m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_EXIT;
			}
		}
		break;
	case ON_SUB_SUB_GUILD_WAIT:
		// 사용중인깃발정보, 깃발선택 응답을 기다린다.(GovernGuildFlagList, ResChangeGuildFlag)
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
		break;
	case ON_SUB_SUB_GUILD_EXIT:
		{
			UpdateButtonStatus();
			m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_INIT;
			return ON_SUB_GUILD_POLL;
		}
		break;
	}
	return ON_SUB_GUILD_FLAG;
}

// 가입/탈퇴
UI08 OnlineGovernMgr::ActionJoinLeave()
{
	switch(m_GuildData.m_uiSubStatus)
	{
	case ON_SUB_SUB_GUILD_INIT:
		InitButtonStatus();
		m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_POLL;
		break;
	case ON_SUB_SUB_GUILD_POLL:
		{
			if(m_GuildData.m_BOk.Process(m_bMouseDown))
			{
				/*
				// robypark 2004/12/1 10:41
				// 공성전 소집권 아이템이 있다면 상단 가입 및 탈퇴가 불가능하다.
				if (TRUE == pMyOnlineWorld->IsExistedItemInInventoryAboutTransformationOfSiegeWarfareUnit())
				{
					pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CANNOT_JOIN_LEAVE_GUILD_IMPERIUM_ITEM));

					m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_EXIT;
					break;
				}
				*/

				if(pMyOnlineWorld->pMyData->GetGuildName())		// 상단탈퇴
				{
					// robypark 2004/12/1 17:44
					// 상단 탈퇴 불가능한 시간인지 검사
					if (pMyOnlineWorld->IsInTimeDisabledTimeToWithdraw())
					{
						pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, 
													pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), 
													pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CANNOT_LEAVE_GUILD_TIME_ERROR),
													pMyOnlineWorld->GetSOptionSiegeWarfare()->siGuildDisabledTimeToWithdraw_Begin, 
													pMyOnlineWorld->GetSOptionSiegeWarfare()->siGuildDisabledTimeToWithdraw_End);
						m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_EXIT;
						break;
					}

					m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_WAIT2;
				}
				else											// 상단가입
				{
					// 상단 가입 불가능한 시간인지 검사
					if (pMyOnlineWorld->IsInTimeDisabledTimeToJoin())
					{
						pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, 
												pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), 
												pMyOnlineWorld->pOnlineText->Get(ON_TEXT_CANNOT_JOIN_GUILD_TIME_ERROR), 
												pMyOnlineWorld->GetSOptionSiegeWarfare()->siGuildDisabledTimeToJoin_Begin, 
												pMyOnlineWorld->GetSOptionSiegeWarfare()->siGuildDisabledTimeToJoin_End);
						m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_EXIT;
						break;
					}

					m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_WAIT;
				}

				pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GuildJoinLeave();
			}
			else if(m_GuildData.m_BBack.Process(m_bMouseDown))
			{
				m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_EXIT;
			}
		}
		break;
	case ON_SUB_SUB_GUILD_WAIT:		// 가입 응답을 기다린다.
	case ON_SUB_SUB_GUILD_WAIT2:	// 탈퇴 응답을 기다린다.
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
		break;
	case ON_SUB_SUB_GUILD_EXIT:
		{
			UpdateButtonStatus();
			m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_INIT;
			return ON_SUB_GUILD_POLL;
		}
		break;
	}
	return ON_SUB_GUILD_JOIN_LEAVE;
}

// 상단해체
UI08 OnlineGovernMgr::ActionClose()
{
	switch(m_GuildData.m_uiSubStatus)
	{
	case ON_SUB_SUB_GUILD_INIT:
		InitButtonStatus();
		m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_POLL;
		break;
	case ON_SUB_SUB_GUILD_POLL:
		{
			if(m_GuildData.m_BOk.Process(m_bMouseDown))
			{
				pMyOnlineWorld->pOnlineClient->SendRequestGovernment_DeleteGuild();
				m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_WAIT;
			}
			else if(m_GuildData.m_BBack.Process(m_bMouseDown))
			{
				m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_EXIT;
			}
		}
		break;
	case ON_SUB_SUB_GUILD_WAIT:
		// 상단해체 응답을 기다린다.(GovernGuildDelete)
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
		break;
	case ON_SUB_SUB_GUILD_EXIT:
		{
			UpdateButtonStatus();
			m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_INIT;
			return ON_SUB_GUILD_POLL;
		}
		break;
	}
	return ON_SUB_GUILD_CLOSE;
}

// 상단원임명
UI08 OnlineGovernMgr::ActionMemberList()
{
	switch(m_GuildData.m_uiSubStatus)
	{
	case ON_SUB_SUB_GUILD_INIT:
		{
			InitButtonStatus();
			m_GuildData.m_siScroll = m_GuildData.m_uiMemberNum = 0;
			m_GuildData.m_siSelect = 1;
			m_GuildData.m_siScrollPos = m_siY + 164;
			m_GuildData.m_siCurPage = 1;

			if(pMyOnlineWorld->pMyData->GetGuildName() && ( pMyOnlineWorld->pMyData->GetGuildClass() != pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode( SO_GUILD_SC_JWJ ) ) )
			{
				pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetDismissalGuild(m_GuildData.m_siCurPage, 0);
				m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_WAIT;
			}
			else
				m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_EXIT;
		}
		break;
	case ON_SUB_SUB_GUILD_POLL:
		{
			// 상단원, Page 선택 처리
			if(m_bMouseDown)
			{
				if(ActionMemberList(1) != -1)
				{
					pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetDismissalGuild(m_GuildData.m_siCurPage, 0);
					m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_WAIT;
				}
			}

			// 버튼 활성/비활성화
			if(pMyOnlineWorld->pMyData->GetGuildClass() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
			{
				if(m_GuildData.m_siSelect >= 0)
				{
					if(m_GuildData.m_pMemberList[m_GuildData.m_siSelect].siClassInGuild != pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
						m_GuildData.m_BDHS.SetAction(TRUE);
					else
						m_GuildData.m_BDHS.SetAction(FALSE);
					
					if(m_GuildData.m_pMemberList[m_GuildData.m_siSelect].siClassInGuild == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_SHN))
						m_GuildData.m_BHND.SetAction(TRUE);
					else
						m_GuildData.m_BHND.SetAction(FALSE);
					
					if(m_GuildData.m_pMemberList[m_GuildData.m_siSelect].siClassInGuild == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DHS)
						|| m_GuildData.m_pMemberList[m_GuildData.m_siSelect].siClassInGuild == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_HND))
						m_GuildData.m_BOk.SetAction(TRUE);
					else
						m_GuildData.m_BOk.SetAction(FALSE);
					
					if(m_GuildData.m_pMemberList[m_GuildData.m_siSelect].siClassInGuild != pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
						m_GuildData.m_BCancel.SetAction(TRUE);
					else
						m_GuildData.m_BCancel.SetAction(FALSE);
				}
			}

			if(m_GuildData.m_BDHS.Process(m_bMouseDown))			// 대행수임명
			{
				if(m_GuildData.m_siSelect >= 0)
				{
					m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_WAIT;
					pMyOnlineWorld->pOnlineClient->SendRequestGovernment_AppointClass((char*)m_GuildData.m_pMemberList[m_GuildData.m_siSelect].szName, pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DHS));
				}
			}
			else if(m_GuildData.m_BHND.Process(m_bMouseDown))		// 행동대장임명
			{
				if(m_GuildData.m_siSelect >= 0)
				{
					m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_WAIT;
					pMyOnlineWorld->pOnlineClient->SendRequestGovernment_AppointClass((char*)m_GuildData.m_pMemberList[m_GuildData.m_siSelect].szName, pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_HND));
				}
			}
			else if(m_GuildData.m_BOk.Process(m_bMouseDown))		// 직급해임
			{
				if(m_GuildData.m_siSelect >= 0)
				{
					m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_WAIT;
					pMyOnlineWorld->pOnlineClient->SendRequestGovernment_DismissClass((char*)m_GuildData.m_pMemberList[m_GuildData.m_siSelect].szName);
				}
			}
			else if(m_GuildData.m_BCancel.Process(m_bMouseDown))	// 해고
			{
				if(m_GuildData.m_siSelect >= 0)
				{
					// 상단원 해고가 불가능한 시간인지 검사
					if (pMyOnlineWorld->IsInTimeDisabledTimeToWithdraw())
					{
						break;
					}

					m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_WAIT;
					pMyOnlineWorld->pOnlineClient->SendRequestGovernment_DismissalGuildPlayer((char*)m_GuildData.m_pMemberList[m_GuildData.m_siSelect].szName);
				}
			}
			else if(m_GuildData.m_BBack.Process(m_bMouseDown))		// 돌아가기
			{
				m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_EXIT;
			}
		}
		break;
	case ON_SUB_SUB_GUILD_WAIT:
		// 응답을 기다린다.
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
		break;
	case ON_SUB_SUB_GUILD_EXIT:
		{
			UpdateButtonStatus();
			m_GuildData.m_uiMemberNum = 0;
			m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_INIT;
			return ON_SUB_GUILD_POLL;
		}
		break;
	}
	return ON_SUB_GUILD_MEMBER_LIST;
}

// 다른상단
UI08 OnlineGovernMgr::ActionDiffGuild()
{
	switch(m_GuildData.m_uiSubStatus)
	{
	case ON_SUB_SUB_GUILD_INIT:
		{
			InitButtonStatus();
			m_GuildData.m_siSelect = m_GuildData.m_siScroll = m_GuildData.m_uiGuildNum = 0;
			m_GuildData.m_siScrollPos = m_siY + 164;

			// 상단 목록 요청
			pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetDiffGuild();
			m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_WAIT;
		}
		break;
	case ON_SUB_SUB_GUILD_POLL:
		{
			// 상단, 선전포고 선택 및 스크롤 처리
			if(m_bMouseDown)
			{
				// 화친제의를 받은 경우 표시되는 아이콘을 클릭한 경우
				bool bIsPressedBtnOfferPeace = false;
				
				// 상단목록 처리 적대설정 및 화친제의 메시지 클릭 처리
				m_GuildData.m_uiRelation = ActionGuildScroll(bIsPressedBtnOfferPeace);

				// NULL문자 제외한 상단명 길이는 10Bytes로 맞춰준다.
				// 서버에서 얻어오는 상단명이 NULL문자제외 10바이트가 넘는 경우 대방이름까지 연결되어 엉뚱한 상단명이 오게되는 버그가 존재한다. 이로인하여 11바이트(NULL문자 포함) 문자열 변수에 복사하여 사용한다.
				char szSelectedGuildName[ON_GUILDNAME_LENGTH + 1];
				ZeroMemory(szSelectedGuildName, sizeof(szSelectedGuildName));
				if (m_GuildData.m_pGuildList[m_GuildData.m_siSelect].szGuildID)
					strncpy((char*)szSelectedGuildName, (char*)m_GuildData.m_pGuildList[m_GuildData.m_siSelect].szGuildID, ON_GUILDNAME_LENGTH);

				// 상단 관계 설정 처리
				if(m_GuildData.m_uiRelation == ON_RET_GLD_ENEMY)
				{
					pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OKCANCEL, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONHOSON), (char*)szSelectedGuildName);
					m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_RELATION;
				}
				else if(m_GuildData.m_uiRelation == ON_RET_GLD_NEUTRAL)
				{
					pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OKCANCEL, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONHOSOFF), (char*)szSelectedGuildName);
					m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_RELATION;
				}
				else if(m_GuildData.m_uiRelation == ON_RET_GLD_ALLY)
				{
					pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OKCANCEL, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONALLOFF), (char*)szSelectedGuildName);
					m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_RELATION;
				}
				// 화친제의를 받은 버튼 클릭.
				else if (true == bIsPressedBtnOfferPeace)
				{
					/*
					if (ON_RET_GLD_WAR == m_GuildData.m_pGuildList[m_GuildData.m_siSelect].siRelation)
					{
						// 공성하는 날에는 화친제의를 수락 및 거절을 할 수가 없다
						pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", 
															pMyOnlineWorld->pOnlineText->Get(8100123));
						break;
					}
					*/

					// 공성시간에는 화친제의를 수락할 수 없다.
					if (pMyOnlineWorld->IsInTimeSiegeWarfare())
					{
						pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", 
															pMyOnlineWorld->pOnlineText->Get(8100124));
						break;
					}

					// 공성시간이 아닌 경우
					// 화친제의 메시지 내용 메시지 박스로 출력
					pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_ACCEPTDECLINE, "", 
															pMyOnlineWorld->pOnlineText->Get(ON_TEXT_READ_OFFERED_PEACE_DOCUMENT), 
															(char*)szSelectedGuildName, 
															(char*)m_GuildData.m_pGuildList[m_GuildData.m_siSelect].szGuildFriendlyMessage);

					m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_SHOW_OFFEREDPEACE;
				}
			}

			// 마을리스트 요청
			if(m_GuildData.m_BOk.Process(m_bMouseDown))
			{
				if(m_GuildData.m_siSelect >= 0)
				{
					m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_WAIT;
					pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetGuildVillageList((char*)m_GuildData.m_pGuildList[m_GuildData.m_siSelect].szGuildID);
				}
			}
			else if(m_GuildData.m_BBack.Process(m_bMouseDown))
				m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_EXIT;
			// 화친 제의버튼 클릭하였다면
			else if (m_GuildData.m_btnOfferPeace.Process(m_bMouseDown))
			{
				// 선택한 상단이 있다면,
				if (m_GuildData.m_siSelect >= 0)
				{
					// 아직 화친제의를 받지 않은 경우
					if (NULL == m_GuildData.m_pGuildList[m_GuildData.m_siSelect].szGuildFriendlyMessage[0])
					{
						// 선택한 상단이 적대상단이거나, 공성하는 상단인 경우
						if ((ON_RET_GLD_ENEMY == m_GuildData.m_pGuildList[m_GuildData.m_siSelect].siRelation) 
							|| (ON_RET_GLD_WAR == m_GuildData.m_pGuildList[m_GuildData.m_siSelect].siRelation))
						{
							// 공성전 시간에는 화친제의를 할 수 없다.
							if (pMyOnlineWorld->IsInTimeSiegeWarfare())
							{
								pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", 
																	pMyOnlineWorld->pOnlineText->Get(8100124));
								break;
							}

							// 공성전 시간이 아니라면
							// 화친제의 메시지 작성
							m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_OFFERPEACE;
							m_GuildData.m_uiSubStatus_OfferPeace = ON_SUB_SUB_GUILD_OFFERPEACE_INIT;

							/*
							// 선택한 상단이 적대상단이거나
							if (ON_RET_GLD_ENEMY == m_GuildData.m_pGuildList[m_GuildData.m_siSelect].siRelation) 
							{
								// 공성하는 날인가?
								if (pMyOnlineWorld->IsInDaySiegeWarfare())
								{
									// 공성하는 날에는 화친제의를 할 수가 없다
									pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", 
																		pMyOnlineWorld->pOnlineText->Get(8100123));

									break;
								}

								// 화친제의 메시지 작성
								m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_OFFERPEACE;
								m_GuildData.m_uiSubStatus_OfferPeace = ON_SUB_SUB_GUILD_OFFERPEACE_INIT;
							}
							else if (ON_RET_GLD_WAR == m_GuildData.m_pGuildList[m_GuildData.m_siSelect].siRelation)
							{
								// 공성하는 날에는 화친제의를 할 수가 없다
								pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", 
																	pMyOnlineWorld->pOnlineText->Get(8100123));
							}
							*/
						}
					}
				}
			}

		}
		break;
	case ON_SUB_SUB_GUILD_RELATION:
		{
			if(pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK)
			{
				if(m_GuildData.m_siSelect >= 0)
				{
					m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_WAIT;
					pMyOnlineWorld->pOnlineClient->SendRequestGovernment_SetRelation((char*)m_GuildData.m_pGuildList[m_GuildData.m_siSelect].szGuildID, m_GuildData.m_uiRelation);
				}
				else
					m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_POLL;
			}
			else if(pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_CANCEL)
				m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_POLL;
		}
		break;
	case ON_SUB_SUB_GUILD_VILLAGELIST:
		{
			// 페이지 선택 처리
			if(m_bMouseDown)
				ActionPage();

			if(m_GuildData.m_BBack.Process(m_bMouseDown))
			{
				m_GuildData.m_uiVillageNum = 0;
				m_GuildData.m_BOk.SetAction(TRUE);
				m_GuildData.m_BBack.SetX(m_siX+322);
				m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_POLL;
			}
		}
		break;
	case ON_SUB_SUB_GUILD_WAIT:
		// 상단리스트, 적대관계설정 응답을 기다린다.(GovernGuildList, GovernGuildSetRelation)
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
		break;
	case ON_SUB_SUB_GUILD_EXIT:
		{
			UpdateButtonStatus();
			m_GuildData.m_uiGuildNum = 0;
			m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_INIT;
			return ON_SUB_GUILD_POLL;
		}
		break;

	// 화친제의 메뉴처리
	case ON_SUB_SUB_GUILD_OFFERPEACE:
		{
			m_GuildData.m_uiSubStatus = ActionOfferPeace();
		}
		break;

	// 화친제의한 상대방 메시지 보고 처리하기
	case ON_SUB_SUB_GUILD_SHOW_OFFEREDPEACE:
		{
			// 화친제의 수락
			if(pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_ACCEPT)
			{
				if(m_GuildData.m_siSelect >= 0)
				{
					/*
					// 공성시간이면
					if (pMyOnlineWorld->IsInTimeSiegeWarfare())
					{
						m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_POLL;
						break;
					}
					*/

					m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_POLL;//ON_SUB_SUB_GUILD_WAIT;
					// 화친제의 수락 메시지 보내기
					pMyOnlineWorld->pOnlineClient->SendRequestGovernment_AcceptOfferedPeace((char*)m_GuildData.m_pGuildList[m_GuildData.m_siSelect].szGuildID);

					// 상단 목록 요청
					pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetDiffGuild();
					m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_WAIT;
				}
				else
					m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_POLL;
			}
			// 화친제의 거절
			else if(pMyOnlineWorld->m_siMsgBoxStatus == ON_MSGBOX_RETURN_DECLINE)
			{
				/*
				// 공성시간이면
				if (pMyOnlineWorld->IsInTimeSiegeWarfare())
				{
					m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_POLL;
					break;
				}
				*/

				// 화친제의 거절 메시지 보내기
				pMyOnlineWorld->pOnlineClient->SendRequestGovernment_DeclineOfferedPeace((char*)m_GuildData.m_pGuildList[m_GuildData.m_siSelect].szGuildID);
				m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_POLL;

				// 상단 목록 요청
				pMyOnlineWorld->pOnlineClient->SendRequestGovernment_GetDiffGuild();
				m_GuildData.m_uiSubStatus = ON_SUB_SUB_GUILD_WAIT;
			}
		}
		break;
	}
	return ON_SUB_GUILD_DIFF_GUILD;
}

// 화친제의하기 메뉴 처리
UI08 OnlineGovernMgr::ActionOfferPeace(void)
{
	switch (m_GuildData.m_uiSubStatus_OfferPeace)
	{
		case ON_SUB_SUB_GUILD_OFFERPEACE_INIT:	// 초기화
			{
				// 메시지 작성 Edit Box 및 Control 초기화
				OnlineEditControlEx	*pOecEx;
				SetRect(&m_GuildData.m_sInterfaceOfferPeace.m_rtEditBox, m_siX + 65, m_siY + 195, m_siX + 65 + 390, m_siY + 195 + 100);
				RECT rcRect;
				CopyRect(&rcRect, &m_GuildData.m_sInterfaceOfferPeace.m_rtEditBox);
				OffsetRect(&rcRect, 2, 2);
				InflateRect(&rcRect, -4, -4);

				if (m_GuildData.m_sInterfaceOfferPeace.m_hOecMessage)
					Handler_OnlineEditControlEx::KillControl( m_GuildData.m_sInterfaceOfferPeace.m_hOecMessage );

				m_GuildData.m_sInterfaceOfferPeace.m_hOecMessage	= Handler_OnlineEditControlEx::CreateControl( DF_FLAG_OEC_TYPE_MULTILINE, &rcRect, OFFERPEACE_MESSAGE_LENGTH);
				pOecEx	=	Handler_OnlineEditControlEx::GetControl( m_GuildData.m_sInterfaceOfferPeace.m_hOecMessage );
				pOecEx->SetFont( pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT) );
				pOecEx->SetColor( RGB(250, 250, 250) );
				pOecEx->SetExceptionChar( '\'' );
				pOecEx->SetExceptionChar( '\"' );
				pOecEx->SetExceptionChar( '\\' );

				pMyOnlineWorld->SetShortKey( true );

				// 보내기 버튼
				m_GuildData.m_sInterfaceOfferPeace.m_btnSendMessage.Create(m_siX + 150, m_siY + 195 + 100 + 10, m_GuildData.m_ButImage.Header.Xsize, m_GuildData.m_ButImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SEND_PEACE_MESSAGE), BUTTON_PUT_LEFT, TRUE);
				// 취소 버튼
				m_GuildData.m_sInterfaceOfferPeace.m_btnCancel.Create(m_siX + 250, m_siY + 195 + 100 + 10, m_GuildData.m_ButImage.Header.Xsize, m_GuildData.m_ButImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_QUESTINFO_CANCEL), BUTTON_PUT_LEFT, TRUE);

				// 다른 상단 보기 메뉴일 경우 보여지는 버튼들 비활성화
				m_GuildData.m_BOk.SetAction(FALSE);
				m_GuildData.m_BBack.SetAction(FALSE);
				m_GuildData.m_btnOfferPeace.SetAction(FALSE);	// 화친제의 버튼

				m_GuildData.m_uiSubStatus_OfferPeace = ON_SUB_SUB_GUILD_OFFERPEACE_POLL;
			}
			break;
		case ON_SUB_SUB_GUILD_OFFERPEACE_POLL:	// 메인 처리
			{
				OnlineEditControlEx	*pOecEx	= Handler_OnlineEditControlEx::GetControl( m_GuildData.m_sInterfaceOfferPeace.m_hOecMessage );

				// 보내기 버튼
				if (m_GuildData.m_sInterfaceOfferPeace.m_btnSendMessage.Process(m_bMouseDown))
				{
					// 메시지 작성 텍스트 얻기
					OnlineEditControlEx	*pOecEx	= Handler_OnlineEditControlEx::GetControl( m_GuildData.m_sInterfaceOfferPeace.m_hOecMessage );

					CHAR szDescript[512];
					ZeroMemory(szDescript, sizeof(szDescript));

					strcpy( szDescript, pOecEx->GetString() );
					if(CheckLeadByte(szDescript))
					{
						// 화친제의 메시지 보내기
						pMyOnlineWorld->pOnlineClient->SendRequestGovernment_OfferPeace(m_GuildData.m_pGuildList[m_GuildData.m_siSelect].szGuildID, szDescript);

						Handler_OnlineEditControlEx::ClearCurrentFocus();

						m_GuildData.m_uiSubStatus_OfferPeace = ON_SUB_SUB_GUILD_OFFERPEACE_EXIT;
					}
					else
						pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(8000479));
				}
				// 취소 버튼
				else if (m_GuildData.m_sInterfaceOfferPeace.m_btnCancel.Process(m_bMouseDown))
				{
					Handler_OnlineEditControlEx::ClearCurrentFocus();

					m_GuildData.m_uiSubStatus_OfferPeace = ON_SUB_SUB_GUILD_OFFERPEACE_EXIT;
				}
				else if (m_bMouseDown)
				{
					if( pOecEx->IsPointInRect( ptMouse ) )
						Handler_OnlineEditControlEx::SetFocus( m_GuildData.m_sInterfaceOfferPeace.m_hOecMessage );
					else		
						Handler_OnlineEditControlEx::ClearCurrentFocus();
				}

				// 화친제의 메시지 박스 포커스가 가 있을 경우
				if( Handler_OnlineEditControlEx::IsFocus( m_GuildData.m_sInterfaceOfferPeace.m_hOecMessage ) )
					pMyOnlineWorld->SetShortKey( true );
				else	
					pMyOnlineWorld->SetShortKey( false );
			}
			break;

		case ON_SUB_SUB_GUILD_OFFERPEACE_WAIT:	// 대기
			// 응답을 기다린다.
			pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT), pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WAIT));
			break;

		case ON_SUB_SUB_GUILD_OFFERPEACE_EXIT:	// 종료
			// Edit Control 해제
			OnlineEditControlEx	*pOecEx	= Handler_OnlineEditControlEx::GetControl( m_GuildData.m_sInterfaceOfferPeace.m_hOecMessage );
			pOecEx->Clear();
			pMyOnlineWorld->SetShortKey( false );
			Handler_OnlineEditControlEx::ClearCurrentFocus();

			if (m_GuildData.m_sInterfaceOfferPeace.m_hOecMessage)
				Handler_OnlineEditControlEx::KillControl( m_GuildData.m_sInterfaceOfferPeace.m_hOecMessage );

			m_GuildData.m_uiSubStatus_OfferPeace = ON_SUB_SUB_GUILD_OFFERPEACE_INIT;

			// 다른 상단 보기 메뉴일 경우 보여지는 버튼들 활성화
			m_GuildData.m_BOk.SetAction(TRUE);
			m_GuildData.m_BBack.SetAction(TRUE);
			m_GuildData.m_btnOfferPeace.SetAction(TRUE);	// 화친제의 버튼

			return ON_SUB_SUB_GUILD_POLL;
			break;
	}

	return ON_SUB_SUB_GUILD_OFFERPEACE;
}

VOID OnlineGovernMgr::InitButtonStatus()
{
	m_GuildData.m_BOpen.SetAction(FALSE);
	m_GuildData.m_BMemberList.SetAction(FALSE);
	m_GuildData.m_BFlag.SetAction(FALSE);
	m_GuildData.m_BJoinLeave.SetAction(FALSE);
	m_GuildData.m_BClose.SetAction(FALSE);
	m_GuildData.m_BAppoint.SetAction(FALSE);
	m_GuildData.m_BDiffGuild.SetAction(FALSE);
	m_BExit.SetAction(FALSE);

	m_GuildData.m_BOk.SetAction(TRUE);
	m_GuildData.m_BBack.SetAction(TRUE);

	m_GuildData.m_BOk.SetX(m_siX+132);
	m_GuildData.m_BCancel.SetX(m_siX+227);
	m_GuildData.m_BBack.SetX(m_siX+322);

	m_GuildData.m_BOk.SetMenuName(pMyOnlineWorld->pOnlineText->Get(ON_BOARD_OK));
	m_GuildData.m_BCancel.SetMenuName(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONN));

	switch(m_GuildData.m_uiStatus)
	{
	case ON_SUB_GUILD_OPEN:
		break;
	case ON_SUB_GUILD_WAITING_LIST:
		m_GuildData.m_BCancel.SetAction(TRUE);
		m_GuildData.m_BOk.SetMenuName(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONY));
		m_GuildData.m_BCancel.SetMenuName(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_TRADEASSOCIATIONN));
		break;
	case ON_SUB_GUILD_FLAG:
		break;
	case ON_SUB_GUILD_JOIN_LEAVE:
		break;
	case ON_SUB_GUILD_CLOSE:
		break;
	case ON_SUB_GUILD_MEMBER_LIST:
		if(pMyOnlineWorld->pMyData->GetGuildClass() != pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
			m_GuildData.m_BOk.SetAction(FALSE);
		m_GuildData.m_BOk.SetX(m_siX+227);
		m_GuildData.m_BCancel.SetX(m_siX+322);
		m_GuildData.m_BBack.SetX(m_siX+417);
		m_GuildData.m_BOk.SetMenuName(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_CLASS_DISSMISSAL));
		m_GuildData.m_BCancel.SetMenuName(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_FIRE));
		break;
	case ON_SUB_GUILD_DIFF_GUILD:
		m_GuildData.m_BOk.SetMenuName(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_VILLAGE_LIST));
		// 사용자가 대방이라면 화친제의 가능
		if(pMyOnlineWorld->pMyData->GetGuildClass() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
		{
			// 공성시간인 경우
			if (pMyOnlineWorld->IsInTimeSiegeWarfare())
			{
				break;
			}
			
			// 공성시간이 아닌 경우
			// 마을에 길드가 존재하며
			if(pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID[0])
			{
				// 사용자 상단의 마을인 경우
				if(strcmp((char*)pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID, pMyOnlineWorld->pMyData->GetGuildName()) == 0)
				{
					m_GuildData.m_btnOfferPeace.SetAction(TRUE);
					m_GuildData.m_btnOfferPeace.ShowWindow(TRUE);
				}
			}
		}
		else
			m_GuildData.m_btnOfferPeace.ShowWindow(FALSE);
		break;
	}
}

VOID OnlineGovernMgr::UpdateButtonStatus()
{
	m_GuildData.m_BOpen.SetAction(FALSE);
	m_GuildData.m_BMemberList.SetAction(FALSE);
	m_GuildData.m_BFlag.SetAction(FALSE);
	m_GuildData.m_BJoinLeave.SetAction(FALSE);
	m_GuildData.m_BClose.SetAction(FALSE);
	m_GuildData.m_BAppoint.SetAction(FALSE);
	m_GuildData.m_BDiffGuild.SetAction(TRUE);
	m_BExit.SetAction(TRUE);

	if(pMyOnlineWorld->pMyData->GetGuildName())
	{
		if(pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID[0])
		{
			// 이 마을의 길드가 자신이 속한 길드일때
			if(strcmp((char*)pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID, pMyOnlineWorld->pMyData->GetGuildName()) == 0)
			{
				m_GuildData.m_BAppoint.SetAction(TRUE);
				m_GuildData.m_BJoinLeave.SetAction(TRUE);
				if(pMyOnlineWorld->pMyData->GetGuildClass() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
				{
					m_GuildData.m_BMemberList.SetAction(TRUE);
					m_GuildData.m_BFlag.SetAction(TRUE);
					m_GuildData.m_BClose.SetAction(TRUE);

					// robypark 2005/1/28 12:00
					// 대방은 이 버튼을 사용할 수 없다. 대방이 탈퇴는 상단해체를 의미하며, 그것을 원한다면 상단해체를 이용해라
					m_GuildData.m_BJoinLeave.SetAction(FALSE);
				}
			}
		}
	}
	else
	{
		if(pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID[0])
			m_GuildData.m_BJoinLeave.SetAction(TRUE);
		else if(strcmp((char*)pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.m_MaxInvestmentID, pMyOnlineWorld->pMyData->GetMyCharName())==0)
			m_GuildData.m_BOpen.SetAction(TRUE);
	}
}

VOID OnlineGovernMgr::ActionFlagScroll()
{
	SI16 siHeight, siListCount, siMaxLine = GUILD_FLAG_YCOUNT;

	siListCount = (m_GuildData.m_siFlagCount-1)/6 + 1;

	// 스크롤 처리
	if(siListCount > siMaxLine)
	{
		if(m_GuildData.m_BScrollUp.Process(m_bMouseDown))
		{
			m_GuildData.m_siScroll--;
			if(m_GuildData.m_siScroll < 0)		m_GuildData.m_siScroll = 0;
			
			siHeight = (SI16)(m_GuildData.m_rtScroll.bottom - m_GuildData.m_rtScroll.top - m_GuildData.m_ScrBarImage.Header.Ysize);
			m_GuildData.m_siScrollPos = m_siY + 164 + (SI16)(((float)siHeight / (float)(siListCount - siMaxLine)) * (float)m_GuildData.m_siScroll);
		}		
		else if(m_GuildData.m_BScrollDown.Process(m_bMouseDown))
		{
			m_GuildData.m_siScroll++;
			if(m_GuildData.m_siScroll > (siListCount - siMaxLine))	m_GuildData.m_siScroll = siListCount - siMaxLine;

			siHeight		=	(SI16)((m_GuildData.m_rtScroll.bottom - m_GuildData.m_rtScroll.top - m_GuildData.m_ScrBarImage.Header.Ysize));
			m_GuildData.m_siScrollPos = m_siY + 164 + (SI16)(((float)siHeight / (float)(siListCount - siMaxLine))  * (float)m_GuildData.m_siScroll);
		}
		else if(PtInRect(&m_GuildData.m_rtScroll, ptMouse) == TRUE)
		{
			siHeight		=	(SI16)(m_GuildData.m_rtScroll.bottom - m_GuildData.m_rtScroll.top - m_GuildData.m_ScrBarImage.Header.Ysize);
			m_GuildData.m_siScrollPos = ptMouse.y;
			if(m_GuildData.m_siScrollPos > (m_GuildData.m_rtScroll.bottom - m_GuildData.m_ScrBarImage.Header.Ysize))
				m_GuildData.m_siScrollPos = m_GuildData.m_rtScroll.bottom - m_GuildData.m_ScrBarImage.Header.Ysize;

			m_GuildData.m_siScroll = max(0, (m_GuildData.m_siScrollPos - (m_siY + 164)) / ((float)siHeight / (float)(siListCount - siMaxLine)));
		}
	}

	// 깃발선택 처리
	SI16 siXCount, siYCount, TempSelect;
	for(siYCount = 0; siYCount < siMaxLine; siYCount++)
	{
		for(siXCount = 0; siXCount < GUILD_FLAG_XCOUNT; siXCount++)
		{
			if(ptMouse.x >= (m_siX+64 + siXCount * 70) && (ptMouse.x < (m_siX+64 + (siXCount + 1) * 70)) && ptMouse.y >= (m_siY+190 + siYCount * 133) && ptMouse.y < (m_siY+190 + (siYCount + 1) * 133))
			{
				TempSelect = (siYCount + m_GuildData.m_siScroll) * GUILD_FLAG_XCOUNT + siXCount;
				
				if(TempSelect >= m_GuildData.m_siFlagCount)					continue;
				if((m_GuildData.m_siFlagIndex[TempSelect] & 0x8000) != 0)	continue;

				m_GuildData.m_siSelect = TempSelect;
			}
		}
	}
}

VOID OnlineGovernMgr::DrawFlag(LPDIRECTDRAWSURFACE7 pSurface)
{
	// 스크롤
	clGrp.PutSpriteT(m_siX + 495, m_siY + 155, m_GuildData.m_ScrBackImage.Header.Xsize, m_GuildData.m_ScrBackImage.Header.Ysize, m_GuildData.m_ScrBackImage.Image );
	clGrp.PutSpriteT(m_siX + 498, m_GuildData.m_siScrollPos, m_GuildData.m_ScrBarImage.Header.Xsize, m_GuildData.m_ScrBarImage.Header.Ysize, m_GuildData.m_ScrBarImage.Image );
	m_GuildData.m_BScrollUp.Put(&m_GuildData.m_UpImage, 0, 1, 0, BUTTON_PUT_NOMOVE);
	m_GuildData.m_BScrollDown.Put(&m_GuildData.m_DownImage, 0, 1, 0, BUTTON_PUT_NOMOVE);

	m_GuildData.m_BOk.Put(&m_GuildData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
	m_GuildData.m_BBack.Put(&m_GuildData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
	
	// 깃발
	SI16 siXCount, siYCount, TempSelect;
	for(siYCount = 0; siYCount < GUILD_FLAG_YCOUNT; siYCount++)
	{
		for(siXCount = 0; siXCount < GUILD_FLAG_XCOUNT; siXCount++)
		{
			TempSelect = (siYCount + m_GuildData.m_siScroll) * GUILD_FLAG_XCOUNT + siXCount;

			if(TempSelect >= m_GuildData.m_siFlagCount)		continue;

			XSPR *lpXSPR = pMyOnlineWorld->pOnlineResource->GetxsprFlagV(m_GuildData.m_siFlagIndex[TempSelect] & 0x0fff);
			if(!lpXSPR)		continue;

			if((m_GuildData.m_siFlagIndex[TempSelect] & 0x8000) == 0)
			{
				if(TempSelect == m_GuildData.m_siSelect)
					clGrp.PutSpriteT(m_siX+64 + siXCount * 70, m_siY+190 + siYCount * 133, lpXSPR->Header.Xsize, lpXSPR->Header.Ysize, &lpXSPR->Image[lpXSPR->Header.Start[(GetTickCount() / 120) % lpXSPR->Header.font_number]]);
				else
					clGrp.PutSpriteT(m_siX+64 + siXCount * 70, m_siY+190 + siYCount * 133, lpXSPR->Header.Xsize, lpXSPR->Header.Ysize, &lpXSPR->Image[lpXSPR->Header.Start[0]]);
			}
			else		// 이미 사용중인 깃발
			{
				if(pMyOnlineWorld->pPaletteManager->GetPalette(pMyOnlineWorld->PaletteDataWhiteRed))
					clGrp.PutSpriteT(m_siX+64 + siXCount * 70, m_siY+190 + siYCount * 133, lpXSPR->Header.Xsize, lpXSPR->Header.Ysize, &lpXSPR->Image[0], pMyOnlineWorld->pPaletteManager->GetPalette(pMyOnlineWorld->PaletteDataWhiteRed));
				else
					clGrp.PutSpriteT(m_siX+64 + siXCount * 70, m_siY+190 + siYCount * 133, lpXSPR->Header.Xsize, lpXSPR->Header.Ysize, &lpXSPR->Image[0]);
			}
		}
	}
}

// 다른 상단 보기 인터페이스 그리기 - 이미지만
VOID OnlineGovernMgr::DrawDiffGuildList(LPDIRECTDRAWSURFACE7 pSurface)
{
	if(m_GuildData.m_uiSubStatus == ON_SUB_SUB_GUILD_VILLAGELIST)
	{
		m_GuildData.m_BBack.Put(&m_GuildData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
		return;
	}

	m_GuildData.m_BOk.Put(&m_GuildData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
	m_GuildData.m_BBack.Put(&m_GuildData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
	// 화친제의 버튼 그리기
	m_GuildData.m_btnOfferPeace.Put(&m_GuildData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);

	// 화친제의 메시지 작성 인터페이스가 활성화되어 있다면, 그리기
	if (ON_SUB_SUB_GUILD_OFFERPEACE == m_GuildData.m_uiSubStatus)
	{
		// 메시지 박스의 박스 출력
		clGrp.BoxC(m_GuildData.m_sInterfaceOfferPeace.m_rtEditBox.left, m_GuildData.m_sInterfaceOfferPeace.m_rtEditBox.top, m_GuildData.m_sInterfaceOfferPeace.m_rtEditBox.right, m_GuildData.m_sInterfaceOfferPeace.m_rtEditBox.bottom, 255);

		// 보내기 버튼 그리기
		m_GuildData.m_sInterfaceOfferPeace.m_btnSendMessage.Put(&m_GuildData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);

		// 취소 버튼 그리기
		m_GuildData.m_sInterfaceOfferPeace.m_btnCancel.Put(&m_GuildData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
		return;
	}

	// 스크롤
	if(m_GuildData.m_uiGuildNum > GUILD_LIST_MAX_LINE)
	{
		clGrp.PutSpriteT(m_siX + 495, m_siY + 155, m_GuildData.m_ScrBackImage.Header.Xsize, m_GuildData.m_ScrBackImage.Header.Ysize, m_GuildData.m_ScrBackImage.Image );
		clGrp.PutSpriteT(m_siX + 498, m_GuildData.m_siScrollPos, m_GuildData.m_ScrBarImage.Header.Xsize, m_GuildData.m_ScrBarImage.Header.Ysize, m_GuildData.m_ScrBarImage.Image );
		m_GuildData.m_BScrollUp.Put(&m_GuildData.m_UpImage, 0, 1, 0, BUTTON_PUT_NOMOVE);
		m_GuildData.m_BScrollDown.Put(&m_GuildData.m_DownImage, 0, 1, 0, BUTTON_PUT_NOMOVE);
	}

	// 상단리스트
	for(SI16 i = 0; i < GUILD_LIST_MAX_LINE; i++)
	{
		SI16 index = i + m_GuildData.m_siScroll;
		if(index >= m_GuildData.m_uiGuildNum)	continue;
		
		// 길드 깃발
		XSPR *lpXSPR = pMyOnlineWorld->pOnlineResource->GetxsprFlagM(m_GuildData.m_pGuildList[index].siFlag);
		if(lpXSPR)
		{
			if(lpXSPR->Image)
				clGrp.PutSpriteT(m_siX + 140, m_siY + 188 + (i * 20) - 2, lpXSPR->Header.Xsize, lpXSPR->Header.Ysize, &lpXSPR->Image[lpXSPR->Header.Start[0]]);
		}
		
		// 내 상태 표시
		switch(m_GuildData.m_pGuildList[index].siRelation)
		{
		case ON_RET_GLD_ALLY:		// 동맹
			clGrp.PutSpriteT(m_siX + 165, m_siY + 188 + (i * 20) - 2, m_GuildData.m_RelationImage.Header.Xsize, m_GuildData.m_RelationImage.Header.Ysize, &m_GuildData.m_RelationImage.Image[m_GuildData.m_RelationImage.Header.Start[2]]);
			break;
		case ON_RET_GLD_ENEMY:		// 적
			clGrp.PutSpriteT(m_siX + 165, m_siY + 188 + (i * 20) - 2, m_GuildData.m_RelationImage.Header.Xsize, m_GuildData.m_RelationImage.Header.Ysize, &m_GuildData.m_RelationImage.Image[m_GuildData.m_RelationImage.Header.Start[1]]);
			break;
		case ON_RET_GLD_WAR:		// 오늘 공성이 있다
			// 깜빡이게
//			if (0 == ((GetTickCount() / 100) & 31))
//				break;
			// 오늘 이 상단과 공성이 있다는 것은 적대상단을 의미한다.
			clGrp.PutSpriteT(m_siX + 165, m_siY + 188 + (i * 20) - 2, m_GuildData.m_RelationImage.Header.Xsize, m_GuildData.m_RelationImage.Header.Ysize, &m_GuildData.m_RelationImage.Image[m_GuildData.m_RelationImage.Header.Start[1]]);
			break;
		case ON_RET_GLD_NEUTRAL:	// 중립
		default:
			clGrp.PutSpriteT(m_siX + 165, m_siY + 188 + (i * 20) - 2, m_GuildData.m_RelationImage.Header.Xsize, m_GuildData.m_RelationImage.Header.Ysize, &m_GuildData.m_RelationImage.Image[m_GuildData.m_RelationImage.Header.Start[0]]);
			break;
		}
		/*
		// 상대 길드 표시
		switch(m_GuildData.m_pGuildList[index].siRelationOfOpponent)
		{
		case ON_RET_GLD_ALLY:		// 동맹
			clGrp.PutSpriteT(m_siX + 187, m_siY + 188 + (i * 20), m_GuildData.m_RelationImage.Header.Xsize, m_GuildData.m_RelationImage.Header.Ysize, &m_GuildData.m_RelationImage.Image[m_GuildData.m_RelationImage.Header.Start[2]]);
			break;
		case ON_RET_GLD_ENEMY:		// 적
			clGrp.PutSpriteT(m_siX + 187, m_siY + 188 + (i * 20), m_GuildData.m_RelationImage.Header.Xsize, m_GuildData.m_RelationImage.Header.Ysize, &m_GuildData.m_RelationImage.Image[m_GuildData.m_RelationImage.Header.Start[1]]);
			break;
		case ON_RET_GLD_NEUTRAL:	// 중립
		default:
			clGrp.PutSpriteT(m_siX + 187, m_siY + 188 + (i * 20), m_GuildData.m_RelationImage.Header.Xsize, m_GuildData.m_RelationImage.Header.Ysize, &m_GuildData.m_RelationImage.Image[m_GuildData.m_RelationImage.Header.Start[0]]);
			break;
		}
		*/
		
		// 화친 제의를 받은 경우 아이콘 표시
		if (NULL != m_GuildData.m_pGuildList[index].szGuildFriendlyMessage[0])
		{
			if (((GetTickCount() / 100) & 3))	// 75%[3/4] 확률로 그려짐. 아이콘 깜빡이도록
				clGrp.PutSpriteT(m_siX + 188, m_siY + 188 + (i * 20), m_GuildData.m_sprOfferedPeace.Header.Xsize, m_GuildData.m_sprOfferedPeace.Header.Ysize, m_GuildData.m_sprOfferedPeace.Image);
		}
	}
/*
	m_GuildData.m_BOk.Put(&m_GuildData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
	m_GuildData.m_BBack.Put(&m_GuildData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
	// 화친제의 버튼 그리기
	m_GuildData.m_btnOfferPeace.Put(&m_GuildData.m_ButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
*/
}

// 다른 상단 보기 인터페이스 그리기 - 텍스트만
VOID OnlineGovernMgr::DrawDiffGuildList(HDC hdc)
{
	if(m_GuildData.m_uiSubStatus == ON_SUB_SUB_GUILD_VILLAGELIST)
	{
		DrawVillageList(hdc);
		return;
	}

	CHAR szTemp1[512], szTemp2[512];
	DWORD TextColor;

	m_GuildData.m_BOk.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
	m_GuildData.m_BBack.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
	// 화친제의 버튼 텍스트 출력
	m_GuildData.m_btnOfferPeace.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

	// 화친제의 메시지 작성 인터페이스가 활성화되어 있다면, 그리기
	if (ON_SUB_SUB_GUILD_OFFERPEACE == m_GuildData.m_uiSubStatus)
	{
		// %s 상단에 화친제의 메시지를 작성합니다.
		ZeroMemory(szTemp2, sizeof(szTemp2));
		strncpy((char*)szTemp2, (char*)m_GuildData.m_pGuildList[m_GuildData.m_siSelect].szGuildID, ON_GUILDNAME_LENGTH);
		sprintf(szTemp1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_OFFER_PEACE), szTemp2);
		pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX + 38, m_siY + 160, (char*)szTemp1, RGB(250, 250, 250));

		// 보내기 버튼 텍스트 그리기
		m_GuildData.m_sInterfaceOfferPeace.m_btnSendMessage.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		// 취소 버튼 텍스트 그리기
		m_GuildData.m_sInterfaceOfferPeace.m_btnCancel.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		// 메시지 작성 텍스트 출력
		OnlineEditControlEx	*pOecEx	= Handler_OnlineEditControlEx::GetControl( m_GuildData.m_sInterfaceOfferPeace.m_hOecMessage );
		if (pOecEx)
			pOecEx->Draw(hdc);

		return;
	}

	// 제목
	sprintf((char*)szTemp1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_THEOTHERS2));
	pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX + 72, m_siY + 163, (char*)szTemp1, RGB(250, 250, 250));

	// 내용(상단정보)
	for(SI16 i = 0; i < GUILD_LIST_MAX_LINE; i++)
	{
		SI16 index = i + m_GuildData.m_siScroll;

		if(index >= m_GuildData.m_uiGuildNum)	continue;

		if(index == m_GuildData.m_siSelect) TextColor = RGB(0, 250, 0);
		else								TextColor = RGB(250, 250, 250);

		// 오늘 공성하는 상단이라면 글자색을 변경해준다.
		if (ON_RET_GLD_WAR == m_GuildData.m_pGuildList[index].siRelation)
		{
			TextColor = RGB(250, 0, 0);	// 빨간색

			if (index == m_GuildData.m_siSelect)
				TextColor = RGB(200, 200, 0);	// 노란색

			// 깜빡이게
//			if (0 == ((GetTickCount() / 100) & 31))
//				continue;
		}
		
		sprintf((char*)szTemp1,  "%3d.", index + 1);
		pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX + 50, m_siY + 188 + (i * 20), (char*)szTemp1, TextColor);
	
		ZeroMemory(szTemp1, sizeof(szTemp1));
		strncpy((char*)szTemp1, (char*)m_GuildData.m_pGuildList[index].szGuildID, ON_GUILDNAME_LENGTH);
		sprintf((char*)szTemp2,  "%s", szTemp1);
		pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX + 72, m_siY + 188 + (i * 20), (char*)szTemp2, TextColor);
	
		ZeroMemory(szTemp1, sizeof(szTemp1));
		strncpy((char*)szTemp1, (char*)m_GuildData.m_pGuildList[index].szDAIID, ON_ID_LENGTH);
		sprintf((char*)szTemp2,  "%s", szTemp1);
		pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX + 250, m_siY + 188 + (i * 20), (char*)szTemp2, TextColor);
		
		sprintf((char*)szTemp1,  "%d", m_GuildData.m_pGuildList[index].uiOwnVillageNum);
		pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hdc, m_siX + 360, m_siY + 188 + (i * 20), (char*)szTemp1, TextColor);
		
		sprintf((char*)szTemp1,  "%d/%d", m_GuildData.m_pGuildList[index].uiGuildMemberNum, m_GuildData.m_pGuildList[index].uiMaxGuildMembersNum);
		pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hdc, m_siX + 450, m_siY + 188 + (i * 20), (char*)szTemp1, TextColor);
	}
/*
	m_GuildData.m_BOk.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
	m_GuildData.m_BBack.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
	// 화친제의 버튼 텍스트 출력
	m_GuildData.m_btnOfferPeace.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
*/
}

// 상단 목록 처리
// [OUT] bool &bIsPressedBtnOfferPeace: 화친제의를 받은 경우 표시되는 아이콘을 클릭한 경우
UI16 OnlineGovernMgr::ActionGuildScroll(bool &bIsPressedBtnOfferPeace)
{
	SI16 siHeight, siListCount;

	siListCount = m_GuildData.m_uiGuildNum;

	// 화친제의를 받은 경우 표시되는 아이콘을 클릭한 경우
	bIsPressedBtnOfferPeace = false;

	// 스크롤 처리
	if(siListCount > GUILD_LIST_MAX_LINE)
	{
		if(m_GuildData.m_BScrollUp.Process(m_bMouseDown))
		{
			m_GuildData.m_siScroll--;
			if(m_GuildData.m_siScroll < 0)		m_GuildData.m_siScroll = 0;
			
			siHeight = (SI16)(m_GuildData.m_rtScroll.bottom - m_GuildData.m_rtScroll.top - m_GuildData.m_ScrBarImage.Header.Ysize);
			m_GuildData.m_siScrollPos = m_siY + 164 + (SI16)(((float)siHeight / (float)(siListCount - GUILD_LIST_MAX_LINE)) * (float)m_GuildData.m_siScroll);
		}		
		else if(m_GuildData.m_BScrollDown.Process(m_bMouseDown))
		{
			m_GuildData.m_siScroll++;
			if(m_GuildData.m_siScroll > (siListCount - GUILD_LIST_MAX_LINE))	m_GuildData.m_siScroll = siListCount - GUILD_LIST_MAX_LINE;

			siHeight		=	(SI16)((m_GuildData.m_rtScroll.bottom - m_GuildData.m_rtScroll.top - m_GuildData.m_ScrBarImage.Header.Ysize));
			m_GuildData.m_siScrollPos = m_siY + 164 + (SI16)(((float)siHeight / (float)(siListCount - GUILD_LIST_MAX_LINE))  * (float)m_GuildData.m_siScroll);
		}
		else if(PtInRect(&m_GuildData.m_rtScroll, ptMouse) == TRUE)
		{
			siHeight		=	(SI16)(m_GuildData.m_rtScroll.bottom - m_GuildData.m_rtScroll.top - m_GuildData.m_ScrBarImage.Header.Ysize);
			m_GuildData.m_siScrollPos = ptMouse.y;
			if(m_GuildData.m_siScrollPos > (m_GuildData.m_rtScroll.bottom - m_GuildData.m_ScrBarImage.Header.Ysize))
				m_GuildData.m_siScrollPos = m_GuildData.m_rtScroll.bottom - m_GuildData.m_ScrBarImage.Header.Ysize;

			m_GuildData.m_siScroll = max(0, (m_GuildData.m_siScrollPos - (m_siY + 164)) / ((float)siHeight / (float)(siListCount - GUILD_LIST_MAX_LINE)));
		}
	}

	// 상단선택 처리
	SI16 siYCount, TempSelect;
	for(siYCount = 0; siYCount < GUILD_LIST_MAX_LINE; siYCount++)
	{
		if(ptMouse.x >= m_siX+50 && ptMouse.x < m_siX+450 && ptMouse.y >= (m_siY+188 + siYCount * 20) && ptMouse.y < (m_siY+188 + (siYCount + 1) * 20))
		{
			TempSelect = siYCount + m_GuildData.m_siScroll;

			if(TempSelect >= m_GuildData.m_uiGuildNum)
				continue;

			m_GuildData.m_siSelect = TempSelect;
			break;
		}
	}

	// 선전포고 선택

	// 상단이 있는 마을인지
	if(!pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID[0])				return 0;
	if(!pMyOnlineWorld->pMyData->GetGuildName())										return 0;

	// 직급이 대방인지
	if(pMyOnlineWorld->pMyData->GetGuildClass() != pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
		return 0;

	// 마을소속 상단과 본인상단이 일치하는지
	if(strncmp((char*)pMyOnlineWorld->pOnlineVillage->m_OnRecvVillageInfo.szGuildID, 
		(char*)pMyOnlineWorld->pMyData->GetGuildName(), ON_GUILDNAME_LENGTH) != 0)		return 0;

	for(siYCount = 0; siYCount < GUILD_LIST_MAX_LINE; siYCount++)
	{
//		if(ptMouse.x >= m_siX+155 && ptMouse.x < m_siX+225 && ptMouse.y >= (m_siY+188 + siYCount * 20) && ptMouse.y < (m_siY+188 + (siYCount + 1) * 20))
		if(ptMouse.x >= m_siX+155 && ptMouse.x < m_siX+187 && ptMouse.y >= (m_siY+188 + siYCount * 20) && ptMouse.y < (m_siY+188 + (siYCount + 1) * 20))
		{
			TempSelect = siYCount + m_GuildData.m_siScroll;
			if(TempSelect >= m_GuildData.m_uiGuildNum)			continue;
			m_GuildData.m_siSelect = TempSelect;

			// 선택한 상단이 자기상단일 경우 제외
			if(strncmp((char*)m_GuildData.m_pGuildList[m_GuildData.m_siSelect].szGuildID, (char*)
				pMyOnlineWorld->pMyData->GetGuildName(), ON_GUILDNAME_LENGTH) == 0)		continue;

			if(m_GuildData.m_pGuildList[m_GuildData.m_siSelect].siRelation == ON_RET_GLD_ENEMY)
			{
				// 적대상단에 적대설정(선전포고) 해제는 화친제의로만 가능하다
				return 0;
				return ON_RET_GLD_NEUTRAL;
			}
			else if(m_GuildData.m_pGuildList[m_GuildData.m_siSelect].siRelation == ON_RET_GLD_WAR)
				return 0;
			else
				return ON_RET_GLD_ENEMY;
		}
	}

	// 적대상단이 화친제의를 한 경우 표시되는 아이콘 클릭여부
	for(siYCount = 0; siYCount < GUILD_LIST_MAX_LINE; siYCount++)
	{
		if(ptMouse.x >= m_siX+188 && ptMouse.x < m_siX+188+22 && ptMouse.y >= (m_siY+188 + siYCount * 20) && ptMouse.y < (m_siY+188 + (siYCount + 1) * 20))
		{
			TempSelect = siYCount + m_GuildData.m_siScroll;
			if(TempSelect >= m_GuildData.m_uiGuildNum)			continue;
			m_GuildData.m_siSelect = TempSelect;

			// 선택한 상단이 자기상단일 경우 제외
			if(strncmp((char*)m_GuildData.m_pGuildList[m_GuildData.m_siSelect].szGuildID, (char*)
				pMyOnlineWorld->pMyData->GetGuildName(), ON_GUILDNAME_LENGTH) == 0)		continue;

			// 화친제의를 해온 상단인가?
			if(NULL != m_GuildData.m_pGuildList[m_GuildData.m_siSelect].szGuildFriendlyMessage[0])
			{
				// 화친제의를 받은 경우 표시되는 아이콘을 클릭한 경우
				bIsPressedBtnOfferPeace = true;
				return 0;
			}
		}
	}

	return 0;
}

SI16 OnlineGovernMgr::ActionPage()
{
	SI16 StartPage, EndPage, Prior, Next, PageCount, RenderPosX;
	CheckRenderPage((SI16)m_GuildData.m_siMaxPage, (SI16)m_GuildData.m_siCurPage, StartPage, EndPage, Prior, Next);

	// Page 선택
	PageCount = EndPage - StartPage + 1;
	RenderPosX = m_siX + 265 - (PageCount * 30) / 2;
	for(SI16 i = 0; i < PageCount; i++)
	{
		if(ptMouse.x >= (RenderPosX+i*30) && ptMouse.x < (RenderPosX+(i+1)*30) && ptMouse.y >= (m_siY+316) && ptMouse.y <= (m_siY+332))
		{
			if(m_GuildData.m_siCurPage != (StartPage+i))
			{
				m_GuildData.m_siCurPage = StartPage + i;
				return m_GuildData.m_siCurPage;
			}
		}
	}

	if(Prior)
	{
		RenderPosX -= 30;
		if(ptMouse.x >= RenderPosX && ptMouse.x < (RenderPosX+30) && ptMouse.y >= (m_siY+316) && ptMouse.y <= (m_siY+332))
		{
			if(m_GuildData.m_siCurPage != (StartPage-1))
			{
				m_GuildData.m_siCurPage = StartPage - 1;
				return m_GuildData.m_siCurPage;
			}
		}
	}
	if(Next)
	{
		RenderPosX += PageCount * 30;
		if(ptMouse.x >= RenderPosX && ptMouse.x < (RenderPosX+30) && ptMouse.y >= (m_siY+316) && ptMouse.y <= (m_siY+332))
		{
			if(m_GuildData.m_siCurPage != (EndPage+1))
			{
				m_GuildData.m_siCurPage = EndPage + 1;
				return m_GuildData.m_siCurPage;
			}
		}
	}
	return -1;
}

VOID OnlineGovernMgr::DrawVillageList(HDC hdc)
{
	CHAR szTemp1[256], szTemp2[256];
	DWORD TextColor = RGB(250, 250, 250);

	// 제목(번호, 마을이름, 최고투자자, 최고투자액, 마을방어력)
	sprintf((char*)szTemp1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_NUMBER));
	pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX+50, m_siY+170, (char*)szTemp1, TextColor);
	sprintf((char*)szTemp1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_VILLAGE_NAME));
	pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX+110, m_siY+170, (char*)szTemp1, TextColor);
	sprintf((char*)szTemp1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_VILLAGE_OWNERNAME));
	pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX+200, m_siY+170, (char*)szTemp1, TextColor);
	sprintf((char*)szTemp1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_VILLAGE_INVESTORMONEY));
	pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hdc, m_siX+370, m_siY+170, (char*)szTemp1, TextColor);
	sprintf((char*)szTemp1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOVERNMENT_VILLAGE_DEFENCE));
	pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hdc, m_siX+470, m_siY+170, (char*)szTemp1, TextColor);

	// 마을리스트 출력
	for(SI16 i = 0; i < PAGE_LIST_MAX_LINE; i++)
	{
		SI16 index = i + (m_GuildData.m_siCurPage - 1) * PAGE_LIST_MAX_LINE;

		if(index >= m_GuildData.m_uiVillageNum || index < 0)	continue;

		// 번호
		sprintf((char*)szTemp1,  "%3d", index + 1);
		pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX+54, m_siY+195 + (i * 20), (char*)szTemp1, TextColor);
		
		// 마을이름
		ZeroMemory(szTemp1, sizeof(szTemp1));
		VillageHeader *pVillageHeader = pMyOnlineWorld->pOnlineVillageManager->GetVillageByCode(m_GuildData.m_pVillageList[index].uiVillageCode);
		if(pVillageHeader)
		{
			strcpy((char*)szTemp1, (char*)pMyOnlineWorld->pOnlineText->Get(pVillageHeader->siVillageName));
			sprintf((char*)szTemp2,  "%3s", szTemp1);
			pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX+110, m_siY+195 + (i * 20), (char*)szTemp2, TextColor);
		}
		
		// 최고투자자
		sprintf((char*)szTemp1,  "%3s", m_GuildData.m_pVillageList[index].szBestInvestorName);
		pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX+200, m_siY+195 + (i * 20), (char*)szTemp1, TextColor);
		
		// 최고투자액
		ZeroMemory(szTemp1, sizeof(szTemp1));
		MakeMoneyText(m_GuildData.m_pVillageList[index].mnBestInvestorMoney, (char*)szTemp1);
		sprintf((char*)szTemp2,  pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_MONEY), szTemp1);
		pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hdc, m_siX+370, m_siY+195 + (i * 20), (char*)szTemp2, TextColor);
		
		// 마을방어도
		sprintf((char*)szTemp1,  "%I64d", m_GuildData.m_pVillageList[index].vdVillageDefencePower);
		pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hdc, m_siX+470, m_siY+195 + (i * 20), (char*)szTemp1, TextColor);
	}

	// Page 번호
	DrawPage(hdc);

	m_GuildData.m_BBack.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
}

SI16 OnlineGovernMgr::ActionMemberList(UI08 uiType)
{
	SI16 siListCount;

	if(uiType == 0)			// 대기자 리스트
	{
		siListCount = m_GuildData.m_uiWaitingNum;
	}
	else if(uiType == 1)	// 상단원 리스트
	{
		siListCount = m_GuildData.m_uiMemberNum;
	}

	// 상단원 선택 처리
	SI16 siYCount, TempSelect;
	for(siYCount = 0; siYCount < GUILD_LIST_MAX_LINE; siYCount++)
	{
		if(ptMouse.x >= m_siX+50 && ptMouse.x < m_siX+450 && ptMouse.y >= (m_siY+188 + siYCount * 20) && ptMouse.y < (m_siY+188 + (siYCount + 1) * 20))
		{
			TempSelect = siYCount + m_GuildData.m_siScroll;
			if(TempSelect >= siListCount)			continue;
			m_GuildData.m_siSelect = TempSelect;

			// 상단원 목록 중에 대방은 선택할 수 없다.
			if (uiType == 1)
			{
				if (pMyOnlineWorld->m_cltIGuildClass.GetDAICode() == m_GuildData.m_pMemberList[m_GuildData.m_siSelect].siClassInGuild)
				{
					if (pMyOnlineWorld->m_cltIGuildClass.GetDAICode() == m_GuildData.m_pMemberList[0].siClassInGuild)
						m_GuildData.m_siSelect = 1;
					else
						m_GuildData.m_siSelect = 0;
				}
			}

			break;
		}
	}

	// Page 선택
	return ActionPage();
}

VOID OnlineGovernMgr::DrawMemberList(UI08 uiType, HDC hdc)
{
	CHAR szTemp1[256], szTemp2[256];
	DWORD TextColor = RGB(250, 250, 250);

	if(uiType == 0)			// 대기자 리스트
	{
		// 제목(번호, 아이디, 레벨, 재산현황, 신용등급)
		sprintf((char*)szTemp1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_THEOTHERS));
		pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX+50, m_siY+170, (char*)szTemp1, TextColor);
		
		// 상단원리스트 출력
		for(SI16 i = 0; i < PAGE_LIST_MAX_LINE; i++)
		{
			SI16 index = i + (m_GuildData.m_siCurPage - 1) * PAGE_LIST_MAX_LINE;
			
			if(i >= m_GuildData.m_uiWaitingNum || index < 0)	continue;
			
			if(i == m_GuildData.m_siSelect) TextColor = RGB(0, 250, 0);
			else								TextColor = RGB(250, 250, 250);
			
			// 번호
			sprintf((char*)szTemp1,  "%3d", index + 1);
			pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX+52, m_siY+195 + (i * 20), (char*)szTemp1, TextColor);
			
			// 아이디
			ZeroMemory(szTemp1, sizeof(szTemp1));
			strncpy((char*)szTemp1, (char*)m_GuildData.m_pWaitingList[i].szName, ON_ID_LENGTH);
			sprintf((char*)szTemp2,  "%3s", szTemp1);
			pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX+93, m_siY+195 + (i * 20), (char*)szTemp2, TextColor);
			
			// 레벨
			sprintf((char*)szTemp1,  "%4d", m_GuildData.m_pWaitingList[i].siLevel);
			pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hdc, m_siX+190, m_siY+195 + (i * 20), (char*)szTemp1, TextColor);
			
			// 재산현황
			ZeroMemory(szTemp1, sizeof(szTemp1));
			MakeMoneyText(m_GuildData.m_pWaitingList[i].mnPropery, (char*)szTemp1);
			sprintf((char*)szTemp2,  pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_MONEY), szTemp1);
			pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hdc, m_siX+297, m_siY+195 + (i * 20), (char*)szTemp2, TextColor);
			
			// 신용등급
			sprintf((char*)szTemp1,  "%5d", m_GuildData.m_pWaitingList[i].uiTradeCredit);
			pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX+340, m_siY+195 + (i * 20), (char*)szTemp1, TextColor);
		}
	}
	else if(uiType == 1)	// 상단원 리스트
	{
		// 제목(번호, 아이디, 레벨, 재산현황, 신용등급)
		sprintf((char*)szTemp1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_THEOTHERS1));
		pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX+50, m_siY+170, (char*)szTemp1, TextColor);
		
		// 상단원리스트 출력
		for(SI16 i = 0; i < PAGE_LIST_MAX_LINE; i++)
		{
			SI16 index = i + (m_GuildData.m_siCurPage - 1) * PAGE_LIST_MAX_LINE;
			
			if(i >= m_GuildData.m_uiMemberNum || index < 0)	continue;

			if(i == m_GuildData.m_siSelect) TextColor = RGB(0, 250, 0);
			else								TextColor = RGB(250, 250, 250);
			
			// 번호
			sprintf((char*)szTemp1,  "%3d", index + 1);
			pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX+52, m_siY+195 + (i * 20), (char*)szTemp1, TextColor);
			
			// 아이디
			ZeroMemory(szTemp1, sizeof(szTemp1));
			strncpy((char*)szTemp1, (char*)m_GuildData.m_pMemberList[i].szName, ON_ID_LENGTH);
			sprintf((char*)szTemp2,  "%3s", szTemp1);
			pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX+93, m_siY+195 + (i * 20), (char*)szTemp2, TextColor);

			// 직급
			sprintf((char*)szTemp1,  "%5s", pMyOnlineWorld->pOnlineText->Get(pMyOnlineWorld->m_cltIGuildClass.GetClassName(m_GuildData.m_pMemberList[i].siClassInGuild)));
			pMyOnlineWorld->pOnlineMegaText->DrawText(hdc, m_siX+165, m_siY+195 + (i * 20), (char*)szTemp1, TextColor);
	
			// 레벨
			sprintf((char*)szTemp1,  "%4d", m_GuildData.m_pMemberList[i].siLevel);
			pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hdc, m_siX+260, m_siY+195 + (i * 20), (char*)szTemp1, TextColor);
			
			// 재산현황
			ZeroMemory(szTemp1, sizeof(szTemp1));
			MakeMoneyText(m_GuildData.m_pMemberList[i].mnPropery, (char*)szTemp1);
			sprintf((char*)szTemp2,  pMyOnlineWorld->pOnlineText->Get(ON_TEXT_GOV_MONEY), szTemp1);
			pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hdc, m_siX+360, m_siY+195 + (i * 20), (char*)szTemp2, TextColor);
			
			// 신용등급
			sprintf((char*)szTemp1,  "%5d", m_GuildData.m_pMemberList[i].uiTradeCredit);
			pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hdc, m_siX+436, m_siY+195 + (i * 20), (char*)szTemp1, TextColor);
			
			m_GuildData.m_BDHS.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			m_GuildData.m_BHND.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		}
	}

	// Page 번호
	DrawPage(hdc);

	m_GuildData.m_BOk.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
	m_GuildData.m_BCancel.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
	m_GuildData.m_BBack.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
}

VOID OnlineGovernMgr::DrawPage(HDC hdc)
{
	CHAR szTemp1[256];
	DWORD TextColor = RGB(250, 250, 250);

	SI16 StartPage, EndPage, Prior, Next, PageCount, RenderPosX;
	CheckRenderPage((SI16)m_GuildData.m_siMaxPage, (SI16)m_GuildData.m_siCurPage, StartPage, EndPage, Prior, Next);

	PageCount = EndPage - StartPage + 1;
	RenderPosX = m_siX + 265 - (PageCount * 30) / 2;
	for(SI16 i = 0; i < PageCount; i++)
	{
		SI16 index = StartPage + i;
		if(m_GuildData.m_siCurPage == index) TextColor = RGB(0, 250, 0);
		else								 TextColor = RGB(250, 250, 250);

		sprintf((char*)szTemp1, "[%d]", index);
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, RenderPosX + i * 30, m_siY + 316, 30, 16, (char*)szTemp1, TextColor);
	}

	if(Prior)
	{
		RenderPosX -= 30;
		sprintf((char*)szTemp1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_PREV));
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, RenderPosX, m_siY + 316, 30, 16, (char*)szTemp1, RGB(250, 250, 250));
	}
	if(Next)
	{
		RenderPosX += PageCount * 30;
		sprintf((char*)szTemp1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_NEXT));
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, RenderPosX, m_siY + 316, 30, 16, (char*)szTemp1, RGB(250, 250, 250));
	}
}

// robypark 2004/11/25 16:4
// 화친제의 메시지 보낸 응답
// ON_RESPONSE_GUILD_FRIELDLY_MESSAGE_OFFER
UI16 OnlineGovernMgr::OnResponseGuildFrieldlyMessageOffer(BYTE *pMsg)
{
	OnResGuildFriendlyMessageOffer *pOnResGuildFriendlyMessageOffer = (OnResGuildFriendlyMessageOffer *)pMsg;

	// 화친제의 성공
	if (ON_RET_OK == pOnResGuildFriendlyMessageOffer->dwResponse)
	{
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SUCESS_TO_SEND_PEACE_MESSAGE));
	}
	// 화친제의 실패
	else
	{
		pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FAIL_TO_SEND_PEACE_MESSAGE));
	}

	return 0;
}
